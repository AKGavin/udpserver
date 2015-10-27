/*
 * mainserver.cpp
 *
 *  Created on: Dec 9, 2013
 *      Author: root
 */
#include "mainserver.h"
#include "tdmclassfactory.h"
#include "tdmconstants.h"
#include "tdmwhitelist.h"
#include "tdmhotstats.h"
#include <iostream>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include </usr/local/include/google/sparse_hash_map>
using namespace std;

#define DTM_KEEPALIVE_MSG 0

#define SEND_HEART_TIMEINTERVAL 10
#define SLEEP_INTERVAL 5
#define LOAD_WHITELIST_FROM_DB


#pragma pack(1)

typedef struct {
	unsigned char type;
	uint32_t proto;
} dtm_keepalive_data_t;

#pragma pack()

int sessionId = 0;



google::sparse_hash_map<string, whiteList_data_t *> ThunderWhiteList;
google::sparse_hash_map<string, whiteList_data_t *> HttpWhiteList;
google::sparse_hash_map<string, hotItem_t *> ThunderHotList;
google::sparse_hash_map<string, hotItem_t *> HttpHotList;



int main(void)
{
	printf( "TDM server starting...\n" );
	WriteRunInfo::WriteLog("[main] TDM server starting...");

	HttpHotList.set_deleted_key(" ");
	ThunderHotList.set_deleted_key(" ");
	ThunderWhiteList.set_deleted_key(" ");
	HttpWhiteList.set_deleted_key(" ");

	int ret = CMainServer::Init();
	if ( ret == -1 ) {
		printf( "[main] TDM server init fail!" );
		exit(1);
	}

	CMainServer* server = (CMainServer*) CMainServer::GetInstance();
	ret = server->StartServer();
	if ( ret == -1 ) {
		printf( "[main] TDM server start fail!" );
		exit( 1 );
	}

	printf( "TDM server started successfully.\n" );
	WriteRunInfo::WriteLog("[main] TDM server started");

//	int flag = 0;
	int count = 0;
	while ( true ) {
		sleep( SLEEP_INTERVAL );
		count += 1;

		if ( count * SLEEP_INTERVAL >= SEND_HEART_TIMEINTERVAL ) {
			server->CheckHealth();
		}

//		flag  = server->GetRecvThreadRunFlag();
//		if ( !flag ) break;
	}
	return 0;
}


CMainServer::CMainServer()
{
	monitorSock = -1;
	DTMSock = -1;
}


CMainServer::~CMainServer()
{
	if ( monitorSock != -1 ) {
		close( monitorSock );
	}

	if ( DTMSock != -1 ) {
		close( DTMSock );
	}
}


int CMainServer::Init()
{
	m_pMyServer = new CMainServer();
	int ret = m_pMyServer->InitServer();
	return ret;
}


int CMainServer::InitServer()
{
//	signal( SIGIO, SIG_IGN);
//	signal( SIGPIPE, SIG_IGN);
//	signal( SIGTERM, sighandler);
//	signal( SIGHUP,sighandler);
//	signal( SIGCHLD, sighandler);
//	signal( SIGUSR1, sighandler);
//	signal( SIGINT, sighandler);

	int ret =  m_tdmConf.Init();
	if (ret != 0) {
		return -1;
	}

	map<protocol_type_t, TDMProtoConfItem*> configMap = m_tdmConf.GetConfigItemMap();
	map<protocol_type_t, TDMProtoConfItem*>::iterator iter;
	TDMProtoConfItem* confItem;
	CTDMServerList* serverList;
	protocol_type_t proto;

	for ( iter = configMap.begin(); iter != configMap.end(); iter++ ) {
		proto = iter->first;
		confItem = iter->second;
		serverList = new CTDMServerList( confItem->dcmServers );
		m_dcmServerList.insert( pair<protocol_type_t, CTDMServerList*>(proto, serverList) );

		serverList = new CTDMServerList( confItem->dtmServers );
		m_dtmServerList.insert( pair<protocol_type_t, CTDMServerList*>(proto, serverList) );
	}

	pthread_mutex_lock( &m_monitorMutex );
	if ( m_tdmConf.m_sMonitorServerAddr.length() > 0 ) {
		m_nMonitorIp = inet_addr( m_tdmConf.m_sMonitorServerAddr.c_str() );
	}
	else {
		m_nMonitorIp = -1;
	}

	if ( m_tdmConf.m_nMonitorPort > 0 ) {
		m_nMonitorPort = m_tdmConf.m_nMonitorPort;
	}
	else {
		m_nMonitorPort = -1;
	}
	pthread_mutex_unlock( &m_monitorMutex );

	return 0;
}


int CMainServer::StartServer()
{
	//set class factory
	m_pClassFactory = new CTDMClassFactory();

	monitorSock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if ( monitorSock == -1 ) {
		WriteRunInfo::WriteLog( "[CMainServer] open monitor socket error" );
		return -1;
	}

	DTMSock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if ( DTMSock == -1 ) {
		WriteRunInfo::WriteLog( "[CMainServer] open dtm keepalive socket error" );
		return -1;
	}

	//create db connection pool
	CTDMMySQLWrapperPool::init( m_tdmConf.m_sDBHost.c_str(),
			m_tdmConf.m_sDBUser.c_str(),
			m_tdmConf.m_sDBPasswd.c_str(),
			m_tdmConf.m_sDBName.c_str(),
			m_tdmConf.m_nDBPoolSize );
	WriteRunInfo::WriteLog( "[CMainServer] db pool inited." );

#ifdef LOAD_WHITELIST_FROM_DB
	//load white list from db
	LoadWhiteList();
#endif

	//udp send threadgroup
	int ret = CreateSendThreadGroup();
	if ( ret == 0 ) {
		WriteRunInfo::WriteLog("[CMainServer] Init worker thread group fail");
		return -1;
	}
	else {
		WriteRunInfo::WriteLog("[CMainServer] Init worker thread successfully");
	}

	//init receive ThreadGroup
	ret = CreateRecvThreadGroup();
	if ( ret == 0 ) {
		WriteRunInfo::WriteLog( "[CMainServer] no listener thread group created" );
		return -1;
	}
	else {
		WriteRunInfo::WriteLog("[CMainServer] Init listener thread successfully");
	}

	return 0;
}


int CMainServer::CreateRecvThreadGroup()
{
	//init receive ThreadGroup
	map<protocol_type_t, TDMProtoConfItem*> configMap = m_tdmConf.GetConfigItemMap();
	if ( configMap.empty() ) {
		WriteRunInfo::WriteLog( "[CMainServer] create listener thread fail, config error, no protocol" );
		return -1;
	}

	map<protocol_type_t, TDMProtoConfItem*>::iterator iter;
	TDMProtoConfItem* protoConf;
	int count = 0;
	int ret = 0;
	for ( iter = configMap.begin(); iter != configMap.end(); iter++ ) {
		protoConf = iter->second;
		if ( !protoConf->enabled ) continue;
		ret = CreateSimpleRecvThreadGroup( protoConf );
		if ( ret != 0 ) {
			WriteRunInfo::WriteLog("[CMainServer] protocol listen thread created fail: protocol %d", protoConf->proto );
			return -1;
		}
		else {
			count++;
			WriteRunInfo::WriteLog("[CMainServer] protocol listen thread created successfully: protocol %d", protoConf->proto );
		}
	}

	//create dcm thread
	TDMProtoConfItem* conf = new TDMProtoConfItem();
	conf->sServerAddr = m_tdmConf.m_sServerAddr;
	conf->nUdpPort = m_tdmConf.m_nPort;
	conf->proto = PROTOCOL_DCM;
	WriteRunInfo::WriteLog("[CMainServer] try to create dcm listen thread, ip=%s, port=%d", conf->sServerAddr.c_str(), conf->nUdpPort );
	ret = CreateSimpleRecvThreadGroup( conf );
	if ( ret != 0 ) {
		WriteRunInfo::WriteLog("[CMainServer] dcm listen thread created fail" );
		delete conf;
		return -1;
	}
	else {
		count++;
		WriteRunInfo::WriteLog("[CMainServer] dcm listen thread created successfully" );
	}

	//create maintain thread
	conf->sServerAddr = m_tdmConf.m_sMaintainServerAddr;
	conf->nUdpPort = m_tdmConf.m_nMaintainPort;
	conf->proto = PROTOCOL_MAINTAIN;
	WriteRunInfo::WriteLog("[CMainServer] try to create maintain listen thread, ip=%s, port=%d", conf->sServerAddr.c_str(), conf->nUdpPort );
	ret = CreateSimpleRecvThreadGroup( conf );
	delete conf;
	if ( ret != 0 ) {
		WriteRunInfo::WriteLog("[CMainServer] maintain listen thread created fail, ip=%s, port=%d", conf->sServerAddr.c_str(), conf->nUdpPort );
		return -1;
	}
	else {
		count++;
		WriteRunInfo::WriteLog("[CMainServer] maintain listen thread created successfully, ip=%s, port=%d", conf->sServerAddr.c_str(), conf->nUdpPort );
	}

	return count;
}


int CMainServer::CreateSimpleRecvThreadGroup( TDMProtoConfItem* conf )
{
	int threadEntityType = GetRecvThreadEntityType( conf->proto );
	if ( threadEntityType == 0 ) {
		return -1;
	}

	CThreadGroup* pRecvThreadGroup = new CThreadGroup( 1, threadEntityType, "",(void*)conf );
	int ret = pRecvThreadGroup->Init();
	return ret;
}


int CMainServer::CreateSendThreadGroup()
{
	WriteRunInfo::WriteLog( "[CMainServer] create worker threads..." );
	map<protocol_type_t, TDMProtoConfItem*> configMap = m_tdmConf.GetConfigItemMap();
	if ( configMap.empty() ) {
		WriteRunInfo::WriteLog( "[CMainServer] create worker threads fail: config error, no protocol" );
		return -1;
	}

	map<protocol_type_t, TDMProtoConfItem*>::iterator iter;
	protocol_type_t proto;
	TDMProtoConfItem* protoConf;
	int count = 0;
	int threadEntityType = 0;

	//创建线程：接收dtm关于协议的下载任务
	for ( iter = configMap.begin(); iter != configMap.end(); iter++ ) {
		proto = (protocol_type_t) iter->first;
		protoConf = iter->second;
		if ( !protoConf->enabled ) continue;

		threadEntityType = GetSendThreadEntityType( proto );
		if ( threadEntityType > 0 ) {
			CThreadGroup* pThreadGroup = new CThreadGroup( protoConf->nProcessThreadNum, threadEntityType, "",(void*)protoConf );
			int ret = pThreadGroup->Init();
			if ( ret != 0 ) {
				WriteRunInfo::WriteLog("[CMainServer] protocol worker thread created fail: protocol %d, entityType=%d, num=%d",
						proto, threadEntityType, protoConf->nProcessThreadNum );
				return -1;
			}
			else {
				count++;
				WriteRunInfo::WriteLog("[CMainServer] protocol worker thread created successfully: protocol %d, entityType=%d, num=%d",
						proto, threadEntityType, protoConf->nProcessThreadNum );
			}
		}
	}

	//dcm message process thread: 接收dcm消息的线程
	protoConf = new TDMProtoConfItem();
	protoConf->sServerAddr = m_tdmConf.m_sServerAddr;
	protoConf->nUdpPort = m_tdmConf.m_nPort;
	protoConf->proto = PROTOCOL_DCM;
	threadEntityType = SEND_DCM_THREADENTITY;
	CThreadGroup* pThreadGroup = new CThreadGroup( m_tdmConf.m_nProcessThreadNum, threadEntityType, "",(void*)protoConf );
	int ret = pThreadGroup->Init();

	if ( ret != 0 ) {
		WriteRunInfo::WriteLog("[CMainServer] dcm worker thread group created fail, entityType=%d, num=%d", SEND_DCM_THREADENTITY, m_tdmConf.m_nProcessThreadNum );
		delete protoConf;
		return -1;
	}
	else {
		count++;
		WriteRunInfo::WriteLog("[CMainServer] dcm worker thread created successfully, entityType=%d, num=%d", SEND_DCM_THREADENTITY, m_tdmConf.m_nProcessThreadNum );
	}

	//maintain thread
	protoConf->sServerAddr = m_tdmConf.m_sMaintainServerAddr;
	protoConf->nUdpPort = m_tdmConf.m_nMaintainPort;
	protoConf->proto = PROTOCOL_MAINTAIN;
	threadEntityType = SEND_MAINTAIN_THREADENTITY;
	pThreadGroup = new CThreadGroup( 1, threadEntityType, "",(void*)protoConf );
	ret = pThreadGroup->Init();
	delete protoConf;

	if ( ret != 0 ) {
		WriteRunInfo::WriteLog("[CMainServer] maintain worker thread created fail, entityType=%d", SEND_MAINTAIN_THREADENTITY );
		return -1;
	}
	else {
		count++;
		WriteRunInfo::WriteLog("[CMainServer] maintain worker thread created successfully, entityType=%d", SEND_MAINTAIN_THREADENTITY );
	}

	WriteRunInfo::WriteLog( "[CMainServer] worker threads created, total=%d", count );
	return count;
}


int CMainServer::GetRecvThreadRunFlag()
{
	map<protocol_type_t, TDMProtoConfItem*> configMap = m_tdmConf.GetConfigItemMap();
	if ( configMap.empty() ) {
		WriteRunInfo::WriteLog( "config error, no protocol" );
		return -1;
	}

	map<protocol_type_t, TDMProtoConfItem*>::iterator iter;
	protocol_type_t proto;
	int threadEntityType;
	int flag = 0;

	for ( iter = configMap.begin(); iter != configMap.end(); iter++ ) {
		proto = (protocol_type_t) iter->first;
		threadEntityType = GetRecvThreadEntityType( proto );
		if ( threadEntityType == 0 ) continue;
		flag += m_threadGroupList[threadEntityType]->GetRunFlag();
		return flag;
	}

	return flag;
}


CTDMServerList* CMainServer::GetServerList( protocol_type_t proto )
{
	map<protocol_type_t, CTDMServerList*>::iterator iter;
	iter = m_dcmServerList.find( proto );
	if ( iter == m_dcmServerList.end() ) {
		return NULL;
	}
	else {
		return iter->second;
	}
}


int CMainServer::CheckHealth()
{
	if ( m_nMonitorIp <= 0 || m_nMonitorPort <= 0 ) {
		return 0;
	}

	monitor_cmd_data_t* cmd = (monitor_cmd_data_t*) malloc( sizeof(monitor_cmd_data_t) );
	cmd->pkg_len = sizeof( monitor_pkg_data_t );
	cmd->pkg_type = CMD_CKS_HEART;
	cmd->pkg_chk = 0;

	TDMProtoConfItem* confItem = new TDMProtoConfItem();
	confItem->proto = PROTOCOL_DCM;
	confItem->sServerAddr = this->m_tdmConf.m_sServerAddr;
	confItem->nUdpPort = this->m_tdmConf.m_nPort;
	CheckThreadGroupHealth( cmd, confItem );
	delete confItem;

	map<protocol_type_t, TDMProtoConfItem*> confMap = this->m_tdmConf.GetConfigItemMap();
	map<protocol_type_t, TDMProtoConfItem*>::iterator iter;
	for ( iter = confMap.begin(); iter != confMap.end(); iter++ ) {
		confItem = iter->second;
		if ( confItem->enabled ) {
			CheckThreadGroupHealth( cmd, confItem );
		}
	}

	free( cmd );
	return 0;
}


int CMainServer::CheckThreadGroupHealth( monitor_cmd_data_t* cmd, TDMProtoConfItem* confItem )
{
	int threadEntityType = GetRecvThreadEntityType( confItem->proto );
	CThreadGroup* tg = this->GetThreadGroup( threadEntityType );
	int flag = tg->GetRunFlag();

	cmd->s_id = getSessionId();
	cks_heart_data_t* heartData = (cks_heart_data_t*) &cmd->pkg_data;
	heartData->serverType = CON_TDM;

	string workerIp = confItem->sServerAddr;
	int workerPort = confItem->nUdpPort;

	heartData->ip = (int) inet_addr( workerIp.c_str() );
	heartData->port =  htons(workerPort);
	heartData->status = ( flag>0 ? 0 : -1 );
	heartData->proto = confItem->proto;

	SendPacketToMonitor( cmd );

	//dtm不需要tdm发送心跳包, 2014-4-4
	/*if ( confItem->proto < PROTOCOL_DCM ) {
		//p2p or http protocols, send heart packet to dtm
		SendHeartPacketToDTM( cmd );
	}*/
	return 0;
}


int CMainServer::SendPacketToMonitor( monitor_cmd_data_t* cmd )
{
	pthread_mutex_lock( &m_monitorMutex );
	unsigned int ip = m_nMonitorIp;
	int port = m_nMonitorPort;
	pthread_mutex_unlock( &m_monitorMutex );

	cks_heart_data_t* data = (cks_heart_data_t*) &cmd->pkg_data;
	protocol_type_t proto = data->proto;
	int status = data->status;

	if ( ip <= 0 ||port <= 0 ) {
		WriteRunInfo::WriteLog( "[CMainServer] send heart packet to monitor error, ip or port conf error" );
		return -1;
	}

	char* p = (char*) cmd;
	p += MONITOR_CMD_HEADER_SIZE;
	cmd->pkg_chk = calc_hash( (const unsigned char*) p, 8 );

	int ret = SendUdpPacketWithTimeout2( monitorSock, ip, port, (char*) cmd , sizeof(monitor_cmd_data_t), 3000 );
	if ( ret == -1 ) {
		WriteRunInfo::WriteLog( "[CMainServer] send heart packet to monitor error, proto=%d, status=%d", proto, status );
		return -1;
	}
	else {
//		WriteRunInfo::WriteLog( "[CMainServer] send heart packet to monitor, proto=%d, status=%d", proto, status );
	}
	return 0;
}



int CMainServer::SendHeartPacketToDTM( monitor_cmd_data_t* cmd )
{
	protocol_type_t proto = cmd->pkg_data.heart_data.proto;
	map<protocol_type_t, CTDMServerList*>::iterator iter1 = m_dtmServerList.find( proto );
	if ( iter1 == m_dtmServerList.end() ) return 0;

	CTDMServerList* sl = iter1->second;

	list<server_info_t*> serverList;
	sl->GetServers( serverList, true );
	if ( serverList.size() == 0 ) return 0;

	if ( DTMSock == -1 ) {
		WriteRunInfo::WriteLog( "[CMainServer] send heart packet to dtm error, create socket error" );
		return -1;
	}

	char buffer[200];
	bzero( buffer, 200 );
	buffer[0] = 0x00;
	*(unsigned int*)(buffer+1) = htonl( 0x05 );

	list<server_info_t*>::iterator iter;
	server_info_t* server;
	int ret = 0;
	for ( iter = serverList.begin(); iter != serverList.end(); iter++ ) {
		server = *iter;
		ret = SendUdpPacketWithTimeout( DTMSock, (char*) (server->m_sServerAddr)->c_str(), m_tdmConf.m_nDtmHeartPort, (char*)buffer, 5, 3000 );
		if ( ret == -1 ) {
			WriteRunInfo::WriteLog( "[CMainServer] send heart packet to dtm error, ip=%s, port=%d", (server->m_sServerAddr)->c_str(), m_tdmConf.m_nDtmHeartPort );
		}
		else {
//			WriteRunInfo::WriteLog( "[CMainServer] send heart packet to dtm, ip=%s, port=%d", (server->m_sServerAddr)->c_str(), m_tdmConf.m_nDtmHeartPort );
		}
	}

	return 0;
}


void CMainServer::GetDTMServerList( protocol_type_t proto, bool onlyValid, list<server_info_t*>& serverList )
{
	map<protocol_type_t, CTDMServerList*>::iterator iter = m_dtmServerList.find( proto );
	if ( iter == m_dtmServerList.end() ) return ;

	CTDMServerList* sl = iter->second;
	sl->GetServers( serverList, onlyValid );
}


void CMainServer::GetDCMServerList( protocol_type_t proto, bool onlyValid, list<server_info_t*>& serverList )
{
	map<protocol_type_t, CTDMServerList*>::iterator iter = m_dcmServerList.find( proto );
	if ( iter == m_dcmServerList.end() ) return ;

	CTDMServerList* sl = iter->second;
	sl->GetServers( serverList, onlyValid );
}


void CMainServer::AddDTMServer( protocol_type_t proto, server_info_t* serverInfo )
{
	map<protocol_type_t, CTDMServerList*>::iterator iter = m_dtmServerList.find( proto );
	if ( iter == m_dtmServerList.end() ) return ;

	CTDMServerList* sl = iter->second;
	sl->AddServer( serverInfo );
}


void CMainServer::AddDCMServer( protocol_type_t proto, server_info_t* serverInfo )
{
	map<protocol_type_t, CTDMServerList*>::iterator iter = m_dcmServerList.find( proto );
	if ( iter == m_dcmServerList.end() ) return ;

	CTDMServerList* sl = iter->second;
	sl->AddServer( serverInfo );
}


void CMainServer::LoadWhiteList()
{
//	protocol_type_t proto;
//	TDMProtoConfItem* confItem;
//	map<protocol_type_t, TDMProtoConfItem*> configMap = m_tdmConf.GetConfigItemMap();
//	map<protocol_type_t, TDMProtoConfItem*>::iterator iter;
//
//	int ret = 0;
//	for ( iter = configMap.begin(); iter != configMap.end(); iter++ ) {
//		WriteRunInfo::WriteLog( "[CMainServer] begin load whitelist from db, proto=%d", proto );
//		proto = iter->first;
//		confItem = iter->second;
//		if ( !confItem->enabled ) continue;
//
//		ret = CTDMWhiteList::loadAllEntriesFromDB( proto );
//		if ( ret == -1 ) {
//			WriteRunInfo::WriteLog( "[CMainServer] load whitelist error, proto=%d", proto );
//			break;
//		}
//		else {
//			WriteRunInfo::WriteLog( "[CMainServer] %d whitelist loaded, proto=%d", ret, proto );
//		}
//	}
}


CMysqlWrapper* CMainServer::OpenConnection()
{
	return CTDMMySQLWrapperPool::getInstance()->open();
}


void CMainServer::CloseConnection( CMysqlWrapper* mysql )
{
	CTDMMySQLWrapperPool::getInstance()->close( mysql );
}
