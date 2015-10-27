/*
 * tdmconf.cpp
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#include "tdmconf.h"
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <tdmconstants.h>

#define DEFAULT_UDP_PORT 6870
#define DEFAULT_MAINTAIN_PORT 6871
#define DEFAULT_MONITOR_PORT 9000
#define DEFAULT_DB_POOLSIZE 20

CTDMConf::~CTDMConf()
{
	map<protocol_type_t, TDMProtoConfItem*>::iterator iter;
	for ( iter = m_configItem.begin(); iter != m_configItem.end(); iter++ ) {
		delete iter->second;
	}

	m_configItem.clear();
}


int CTDMConf::Init()
{
	//¶ÁÈ¡ÅäÖÃÎÄ¼þ
	Magic_Config config(string(TDMCONFIGFILE));
	int ret = config.Init();
	if (ret != 0) {
		WriteRunInfo::WriteLog("[CTDMConf] Read config file error.");
		return -1;
	}

	config.ReadItem( "TDM", "UDPSERVERADDR", "127.0.0.1", m_sServerAddr );

	string tmpStr;
	config.ReadItem( "TDM", "UDPPORT", "",  tmpStr);
	if ( tmpStr.length() > 0 ) {
		m_nPort = atoi(tmpStr.c_str());
	}
	else {
		m_nPort = DEFAULT_UDP_PORT;
	}

	config.ReadItem( "TDM", "PROCESSTHREADNUM", "2",  tmpStr);
	m_nProcessThreadNum = atoi(tmpStr.c_str());
	WriteRunInfo::WriteLog("[CTDMConf] server addr=%s, port=%d, work threads=%d", m_sServerAddr.c_str(), m_nPort, m_nProcessThreadNum );

	config.ReadItem( "TDM", "MAINTAINSERVERADDR", "127.0.0.1", m_sMaintainServerAddr );

	config.ReadItem( "TDM", "MAINTAINPORT", "",  tmpStr);
	if ( tmpStr.length() > 0 ) {
		m_nMaintainPort = atoi(tmpStr.c_str());
	}
	else {
		m_nMaintainPort = DEFAULT_MAINTAIN_PORT;
	}
	WriteRunInfo::WriteLog("[CTDMConf] maintain addr=%s, port=%d", m_sMaintainServerAddr.c_str(), m_nMaintainPort );

	config.ReadItem( "TDM", "MONITORSERVERADDR", "", m_sMonitorServerAddr );

	config.ReadItem( "TDM", "MONITORPORT", "",  tmpStr);
	if ( tmpStr.length() > 0 ) {
		m_nMonitorPort = atoi(tmpStr.c_str());
	}
	else {
		m_nMonitorPort = DEFAULT_MONITOR_PORT;
	}
	WriteRunInfo::WriteLog("[CTDMConf] monitor addr=%s, port=%d", m_sMonitorServerAddr.c_str(), m_nMonitorPort );

	config.ReadItem( "TDM", "DTMHEARTPORT", "", tmpStr );
	if ( tmpStr.length() > 0 ) {
		m_nDtmHeartPort = atoi( tmpStr.c_str() );
	}
	else {
		m_nDtmHeartPort = -1;
	}

	config.ReadItem( "TDM", "MYSQL_HOST", "", m_sDBHost );
	config.ReadItem( "TDM", "MYSQL_USER", "", m_sDBUser );
	config.ReadItem( "TDM", "MYSQL_PASSWD", "", m_sDBPasswd );
	config.ReadItem( "TDM", "MYSQL_DB", "", m_sDBName );

	config.ReadItem( "TDM", "MYSQL_POOLSIZE", "",  tmpStr);
	if ( tmpStr.length() > 0 ) {
		m_nDBPoolSize = atoi(tmpStr.c_str());
	}
	else {
		m_nDBPoolSize = DEFAULT_DB_POOLSIZE;
	}
//	WriteRunInfo::WriteLog("[CTDMConf] db host=%s, user=%s, passwd=%s, db=%s, pool_size=%d",
//			m_sDBHost.c_str(), m_sDBUser.c_str(), m_sDBPasswd.c_str(), m_sDBName.c_str(), m_nDBPoolSize );

	protocol_type_t protocols[] = {PROTOCOL_HTTP, PROTOCOL_THUNDER, PROTOCOL_PPSTREAM, PROTOCOL_PPLIVE};
	protocol_type_t proto;
	TDMProtoConfItem* confItem = NULL;
	for ( int i=0; i<4; i++ ) {
		proto = protocols[i];
		confItem = new TDMProtoConfItem();
		ret = loadProtoConf( proto, &config, confItem );
		if ( ret == 0 ) {
			m_configItem.insert( pair<protocol_type_t, TDMProtoConfItem*>(proto, confItem) );
		}
		else {
			delete confItem;
			if ( ret == -1 ) {
				WriteRunInfo::WriteLog("[CTDMConf] load config fail, protocol conf error" );
				return -1;
			}
		}
	}

	WriteRunInfo::WriteLog("[CTDMConf] load config ok" );

	return 0;
}

//0: success; -1:fail; 1:unsupported protocol
int CTDMConf::loadProtoConf( protocol_type_t protoType, Magic_Config* config, TDMProtoConfItem* confItem )
{
	string section = "";
	switch ( protoType ) {
	case PROTOCOL_HTTP: {
		section = "HTTP";
		break;
	}
	case PROTOCOL_THUNDER: {
		section = "THUNDER";
		break;
	}
	case PROTOCOL_PPSTREAM: {
		section = "PPS";
		break;
	}
	case PROTOCOL_PPLIVE: {
		section = "PPLIVE";
		break;
	}
	default: {
		return -1;
	}
	}

	confItem->proto = protoType;
	config->ReadItem( section, "TDMSERVERADDR", "127.0.0.1", confItem->sServerAddr );

	string tmpStr;
	config->ReadItem( section, "ENABLED", "true", tmpStr );
	if ( tmpStr == "true" ) {
		confItem->enabled = true;
	}
	else {
		confItem->enabled = false;
	}

	config->ReadItem( section, "TDMUDPPORT", "",  tmpStr);
	if ( tmpStr.length() > 0 ) {
		confItem->nUdpPort = atoi(tmpStr.c_str());
	}
	else {
		confItem->nUdpPort = DEFAULT_UDP_PORT + protoType * 10;
	}

	config->ReadItem( section, "TDMPROCESSTHREADNUM", "5",  tmpStr);
	confItem->nProcessThreadNum = atoi(tmpStr.c_str());

	config->ReadItem( section, "DCMSERVER", "", tmpStr );
	char* strHost = (char*) tmpStr.c_str();
	parseHostList( confItem->dcmServers, strHost );
	if ( confItem->dcmServers.size() == 0 ) {
		WriteRunInfo::WriteLog( "[CTDMConf] no dcm server for %s", section.c_str() );
	}

	config->ReadItem( section, "DCMLBTYPE", "HASH", tmpStr );
	if ( tmpStr == "ROTATION" ) {
		confItem->nDcmLbType = LB_ROTATION;
	}
	else {
		confItem->nDcmLbType = LB_HASH;
	}

	//dtm
	config->ReadItem( section, "DTMSERVER", "", tmpStr );
	strHost = (char*) tmpStr.c_str();
	parseHostList( confItem->dtmServers, strHost );
	if ( confItem->dtmServers.size() == 0 ) {
		WriteRunInfo::WriteLog( "[CTDMConf] no dtm server for %s in conf file", section.c_str() );
		return -1;
	}

	return 0;
}




