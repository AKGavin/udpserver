/*
 * tdmdcmprocessthread.cpp
 *
 *  Created on: 2013-12-29
 *      Author: root
 */

#include "tdmdcmprocessthread.h"
#include "tdmdbwhitelist.h"
#include "mainserver.h"
#include <unistd.h>


CTDMDCMProcessThread::CTDMDCMProcessThread(int iEntityType,  int iEntityId):CUdpCommonSendThread(iEntityType, iEntityId)
{
	m_nDTMSock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
};


CTDMDCMProcessThread::~CTDMDCMProcessThread()
{
	if ( m_nDTMSock != -1 ) {
		close( m_nDTMSock );
	}
}


int CTDMDCMProcessThread::MsgProcess(CMsg * pMsg)
{
	CUdpCommonBufMsgBody* body = (CUdpCommonBufMsgBody*) pMsg->GetMsgBody();
	cmd_data_t* cmd = (cmd_data_t*) body->GetBuf();
	int len = body->Size();
	int expectLen = cmd->pkg_len + 3 * sizeof(int) + sizeof(pkg_type_t);

	if ( len != expectLen ) {
		WriteRunInfo::WriteLog( "[CTDMDCMProcessThread] invalid packet size, len=%d, expected len=%d", len, expectLen );
		return -1;
	}

	char* p = (char*) cmd;
	p += CMD_HEADER_SIZE;
	unsigned checksum = calc_hash( (const unsigned char*)p, 8 );
	if ( checksum != cmd->pkg_chk ) {
		WriteRunInfo::WriteLog( "[CTDMDCMProcessThread] invalid packet check" );
		return -1;
	}

	pkg_type_t type = cmd->pkg_type;
	switch ( type ) {
	case CMD_DCM_TDM_CONTENT_ADD: {
		ProcessAddWhiteItem( cmd );
		break;
	}
	case CMD_DCM_TDM_CONTENT_DEL: {
		ProcessDelWhiteItem( cmd );
		break;
	}
	case CMD_DCM_TDM_CONTENT_DISABLED: {
		ProcessDisableWhiteItem( cmd );
		break;
	}
	case CMD_DCM_TDM_CONTENT_THUNDER_QUERY: {
		ProcessThunderQuery( cmd );
		break;
	}
	default: {
		WriteRunInfo::WriteLog( "[CTDMDCMProcessThread] invalid cmd type" );
	}
	}

	return 0;
}


int CTDMDCMProcessThread::ProcessAddWhiteItem( cmd_data_t* cmd )
{
	dcm_tdm_white_data_t* whiteData = & cmd->pkg_data.white_data;

	protocol_type_t proto = cmd->pkg_data.type;
	char infohash[41];
	ByteHash2HexString( infohash, whiteData->infohash );
	infohash[40] = '\0';

	if ( strlen(infohash) == 0 ) {
		WriteRunInfo::WriteLog( "[CTDMDCMProcessThread] add white item message error, infohash is null" );
	}
	else {
		WriteRunInfo::WriteLog( "[CTDMDCMProcessThread] receive add white item message, proto=%d, hash=%s", proto, infohash );
	}

	CTDMDBWhiteList* whiteListManager = new CTDMDBWhiteList();
	whiteList_data_t* whitelistItem = NULL;
	int ret = whiteListManager->findEntry( proto, infohash, &whitelistItem );
	if ( ret == -1 ) {
		delete whiteListManager;
		return -1;
	}

	if ( whitelistItem != NULL ) {
		//change status
		ret = whiteListManager->changeEntryStatus( proto, infohash, WHITELIST_STATUS_OK, whiteData->ip, whitelistItem );
	}
	else {
		//add white item
		ret = whiteListManager->addEntry( WHITEITEM_FROM_DCM_DOWNLOAD, infohash, &cmd->pkg_data, WHITELIST_STATUS_OK, whiteData->ip, &whitelistItem );
	}

	delete whiteListManager;

	//上报dtm白名单
	if ( whitelistItem != NULL ) {
		AddWhiteItemToDTM( cmd, whitelistItem, infohash );
		CTDMWhiteList::freeWhitelistItem( proto, whitelistItem );
	}

	return 0;
}


int CTDMDCMProcessThread::ProcessDelWhiteItem( cmd_data_t* cmd )
{
	dcm_tdm_white_data_t* whiteData = & cmd->pkg_data.white_data;
	protocol_type_t proto = cmd->pkg_data.type;

	char infohash[41];
	ByteHash2HexString( infohash, whiteData->infohash );
	infohash[40] = '\0';

	if ( strlen(infohash) == 0 ) {
		WriteRunInfo::WriteLog( "[CTDMDCMProcessThread] delete white item message error, infohash is null" );
	}
	else {
		WriteRunInfo::WriteLog( "[CTDMDCMProcessThread] receive delete white item message, proto=%d, hash=%s", proto, infohash );
	}

	CTDMDBWhiteList* whiteListManager = new CTDMDBWhiteList();
	whiteList_data_t* whiteItem = NULL;
	int ret = whiteListManager->findEntry( proto, infohash, &whiteItem );
	if ( ret == -1 ) {
		delete whiteListManager;
		return -1;
	}

	if ( whiteItem != NULL ) {
		whiteListManager->deleteEntry( proto, infohash );
	}

	delete whiteListManager;

	//上报dtm白名单
	if ( whiteItem != NULL ) {
		DelWhiteItemToDTM( cmd, whiteItem, infohash );
		CTDMWhiteList::freeWhitelistItem( proto, whiteItem );
	}

	return 0;
}


int CTDMDCMProcessThread::ProcessDisableWhiteItem( cmd_data_t* cmd )
{
	dcm_tdm_disable_data_t* whiteData = & cmd->pkg_data.black_data;
	protocol_type_t proto = cmd->pkg_data.type;

	char infohash[41];
	ByteHash2HexString( infohash, whiteData->infohash );
	infohash[40] = '\0';

	if ( strlen(infohash) == 0 ) {
		WriteRunInfo::WriteLog( "[CTDMDCMProcessThread] disable white item message error, infohash is null" );
	}
	else {
		WriteRunInfo::WriteLog( "[CTDMDCMProcessThread] receive disable white item message, proto=%d, hash=%s", proto, infohash );
	}

	CTDMDBWhiteList* whiteListManager = new CTDMDBWhiteList();
	whiteList_data_t* whiteItem = NULL;
	int ret = whiteListManager->findEntry( proto, infohash, &whiteItem );
	if ( ret == -1 ) {
		delete whiteListManager;
		return -1;
	}

	if ( whiteItem != NULL ) {
		ret = whiteListManager->changeEntryStatus( proto, infohash, WHITELIST_STATUS_DISABLED, 0, whiteItem );
	}
	else {
		ret = whiteListManager->addEntry( WHITEITEM_FROM_DCM_DOWNLOAD, infohash, &cmd->pkg_data, WHITELIST_STATUS_DISABLED, 0, &whiteItem );
	}
	delete whiteListManager;

	//上报dtm白名单
	if ( whiteItem != NULL ) {
		DisableWhiteItemToDTM( cmd, whiteItem, infohash );
		CTDMWhiteList::freeWhitelistItem( proto, whiteItem );
	}

	return 0;
}


int CTDMDCMProcessThread::ProcessThunderQuery( cmd_data_t* cmd )
{
	char infohash[41];
	ByteHash2HexString( infohash, cmd->pkg_data.infohash_data );
	infohash[40] = '\0';

	if ( strlen(infohash) == 0 ) {
		WriteRunInfo::WriteLog( "[CTDMDCMProcessThread] thunder query info message error, infohash is null" );
	}
	else {
		WriteRunInfo::WriteLog( "[CTDMDCMProcessThread] receive thunder query info message" );
	}

	CTDMDBWhiteList* whiteListManager = new CTDMDBWhiteList();
	whiteList_data_t* whiteItem = NULL;
	int ret = whiteListManager->findEntry( cmd->pkg_data.type, infohash, &whiteItem );
	delete whiteListManager;
	if ( ret == -1 ) return -1;

	cmd_data_t* reply = (cmd_data_t*) malloc( sizeof(cmd_data_t) );
	reply->s_id = cmd->s_id;
	reply->pkg_type = CMD_DCM_TDM_CONTENT_THUNDER_QUERY_REPLY;
	reply->pkg_len = sizeof(cmd_data_t) - 3 * sizeof(int) - sizeof(pkg_type_t);
	reply->pkg_data.type = PROTOCOL_THUNDER;

	thunder_data_t* data = & reply->pkg_data.thunder_data;
	data->m_nReqType = DCM_GET_RES;
	memcpy( data->m_strInfohash, cmd->pkg_data.infohash_data, 20 );

	if ( whiteItem != NULL ) {
		thunder_data_t* thunderData = & whiteItem->data.thunder_data;
		strncpy( data->m_strCid,thunderData->m_strCid, 40 );
		strncpy( data->m_strPeerId, thunderData->m_strPeerId, 16 );
		data->m_nFilesize = thunderData->m_nFilesize;
		CTDMWhiteList::freeWhitelistItem( cmd->pkg_data.type, whiteItem );
	}
	else {
		memset( data->m_strCid, 0, 40 );
		memset( data->m_strPeerId, 0, 16 );
		data->m_nFilesize = 0;
	}

	reply->pkg_chk = calc_hash( (const unsigned char*) data, 8 );

	this->m_bSendDataFlag = true;
	this->m_pSendBuf = (char*) reply;
	this->m_iSendPackecLen = sizeof(cmd_data_t);

	return 0;
}


int CTDMDCMProcessThread::AddWhiteItemToDTM( cmd_data_t* cmd, whiteList_data_t* whiteItem, char* infohash )
{
	tdm_dtm_whiteitem_t packet;
	protocol_type_t proto = cmd->pkg_data.type;
	int ret = composeDTMAddWhiteItemPacket( &packet, proto , infohash, whiteItem );
	if ( ret == -1 ) {
		WriteRunInfo::WriteLog( "[CTDMDCMProcessThread] add white item message to dtm error, proto=%d, hash=%s", proto, infohash );
		return -1;
	}

	SendPacketToDTM( proto, &packet, sizeof(tdm_dtm_whiteitem_t) );
	WriteRunInfo::WriteLog( "[CTDMDCMProcessThread] add white item message to dtm success, proto=%d, hash=%s", proto, infohash );
	return 0;
}


int CTDMDCMProcessThread::DelWhiteItemToDTM( cmd_data_t* cmd, whiteList_data_t* whiteItem, char* infohash )
{
	tdm_dtm_whiteitem_t packet;
	protocol_type_t proto = cmd->pkg_data.type;
	int ret = composeDTMDelWhiteItemPacket( &packet, proto , infohash, whiteItem );
	if ( ret == -1 ) {
		WriteRunInfo::WriteLog( "[CTDMDCMProcessThread] delete white item message from dtm error, proto=%d, hash=%s", proto, infohash );
		return -1;
	}
	else {
		WriteRunInfo::WriteLog( "[CTDMDCMProcessThread] delete white item message from dtm success, proto=%d, hash=%s", proto, infohash );
	}

	SendPacketToDTM( proto, &packet, sizeof(tdm_dtm_whiteitem_t) );
	return 0;
}


int CTDMDCMProcessThread::DisableWhiteItemToDTM( cmd_data_t* cmd, whiteList_data_t* whiteItem, char* infohash )
{
	tdm_dtm_disableitem_t packet;
	protocol_type_t proto = cmd->pkg_data.type;
	int ret = composeDTMDisableItemPacket( &packet, proto , infohash, whiteItem );
	if ( ret == -1 ) {
		WriteRunInfo::WriteLog( "[CTDMDCMProcessThread] disable white item message from dtm error, proto=%d, hash=%s", proto, infohash );
		return -1;
	}
	else {
		WriteRunInfo::WriteLog( "[CTDMDCMProcessThread] disable white item message from dtm success, proto=%d, hash=%s", proto, infohash );
	}

	SendPacketToDTM( proto, &packet, sizeof(tdm_dtm_disableitem_t) );
	return 0;
}



int CTDMDCMProcessThread::SendPacketToDTM( protocol_type_t proto, void* data, int len )
{
	list<server_info_t*> serverList;
	CMainServer* server = (CMainServer*) CMainServer::GetInstance();
	server->GetDTMServerList( proto, true, serverList );
	if ( serverList.size() == 0 ) {
		WriteRunInfo::WriteLog( "[CTDMDCMProcessThread] send white item message to dtm, but no dtm server alive" );
		return 0;
	}

	list<server_info_t*>::iterator iter;
	server_info_t* s;
	for ( iter = serverList.begin(); iter != serverList.end(); iter++ ) {
		s = *iter;
		WriteRunInfo::WriteLog( "[CTDMDCMProcessThread] send white item message to dtm, ip=%s, port=%d", s->m_sServerAddr->c_str(), s->m_nPort );
		SendUdpPacketWithTimeout( m_nDTMSock, (char*) s->m_sServerAddr->c_str(), s->m_nPort, (char*) data, len, 3000 );
	}
	return 0;
}
