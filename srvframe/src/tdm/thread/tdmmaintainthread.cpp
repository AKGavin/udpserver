/*
 * tdmmaintainthread.cpp
 *
 *  Created on: 2013-12-29
 *      Author: root
 */

#include "tdmmaintainthread.h"
#include "mainserver.h"


int CTDMMaintainThread::MsgProcess(CMsg * pMsg)
{
	CUdpCommonBufMsgBody* body = (CUdpCommonBufMsgBody*) pMsg->GetMsgBody();
	monitor_cmd_data_t* cmd = (monitor_cmd_data_t*) body->GetBuf();
	int len = body->Size();
	int expectLen = cmd->pkg_len + 3 * sizeof(int) + sizeof(pkg_type_t);

	if ( len != expectLen ) {
		WriteRunInfo::WriteLog( "[CTDMDCMProcessThread] invalid packet size, len=%d, expected len=%d", len, expectLen );
		return -1;
	}

	unsigned int checksum = calc_hash( (const unsigned char*)cmd, 8 );
	if ( checksum != cmd->pkg_chk ) {
		WriteRunInfo::WriteLog( "[CTDMDCMProcessThread] invalid packet check" );
		return -1;
	}

	pkg_type_t type = cmd->pkg_type;
	switch ( type ) {
	case CMD_CKS_CHECK: {
		ProcessMonitorCheck( (unsigned int) body->GetClientIp(), body->GetCientPort() );
		break;
	}
	case CMD_CKS_DOWN_REVIVE: {
		ProcessMonitorDownRevive( cmd );
		break;
	}
	default: {
		WriteRunInfo::WriteLog( "[CTDMMaintainThread] receive invalid message" );
	}
	}

	return 0;
}


int CTDMMaintainThread::ProcessMonitorCheck( unsigned int ip, int port )
{
//	WriteRunInfo::WriteLog( "[CTDMMaintainThread] receive MonitorCheck message from %u:%d", ip, port );
	CMainServer* server = (CMainServer*) CMainServer::GetInstance();
	server->SetMonitor( ip, port );
	return 0;
}


int CTDMMaintainThread::ProcessMonitorDownRevive( monitor_cmd_data_t* cmd )
{
	cks_down_revive_data_t* data = & cmd->pkg_data.down_data;
	heart_type_t serverType = data->serverType;
	if ( serverType != CON_DCM && serverType != CON_DTM ) return 0;

	int port = ntohs(data->port);

	char sip[16];
	memset( sip, 0, 16 );
	inet_ntop( AF_INET, (void*) &data->ip, sip, 16 );
	if ( port <= 0 || sip == NULL ) {
		WriteRunInfo::WriteLog( "[CTDMMaintainThread] invalid MonitorDownRevive message" );
		return 0;
	}
	WriteRunInfo::WriteLog( "[CTDMMaintainThread] receive MonitorDownRevive message, type=%d, module=%d, ip=%d(%s), port=%d, proto=%d", data->type, data->serverType, data->ip, sip, port, data->proto );

	protocol_type_t proto = data->proto;
	CMainServer* server = (CMainServer*) CMainServer::GetInstance();
	list<server_info_t*> serverList;

	if ( serverType == CON_DCM ) {
		server->GetDCMServerList( proto, false, serverList );
	}
	else if ( serverType == CON_DTM ) {
		server->GetDCMServerList( proto, false, serverList );
	}

	server_info_t* s;
	if ( data->type == CON_DOWN ) {
		list<server_info_t*>::iterator iter;
		for ( iter = serverList.begin(); iter != serverList.end(); iter++ ) {
			s = *iter;
			if ( s->m_nIp == data->ip && s->m_nPort == port ) {
				s->m_nStatus = SERVER_STATUS_ERROR;
				WriteRunInfo::WriteLog( "[CTDMMaintainThread] change server status to down, ip=%d, port=%d", data->ip, port );
			}
		}
	}
	else if ( data->type == CON_REVIVE ) {
		list<server_info_t*>::iterator iter;
		bool found = false;
		for ( iter = serverList.begin(); iter != serverList.end(); iter++ ) {
			s = *iter;
			if ( s->m_nIp == data->ip && s->m_nPort == port ) {
				s->m_nStatus = SERVER_STATUS_OK;
				WriteRunInfo::WriteLog( "[CTDMMaintainThread] change server status to up, ip=%d, port=%d", data->ip, port );
				found = true;
			}
		}

		if ( !found ) {
			server_info_t* s = new server_info_t();
			s->m_nIp = data->ip;
			s->m_nPort = data->port;
			s->m_nStatus = SERVER_STATUS_OK;
			s->m_sServerAddr = new string( sip );

			if ( serverType == CON_DCM ) {
				WriteRunInfo::WriteLog( "[CTDMMaintainThread] add new dcm server, ip=%d, port=%d", data->ip, port );
				server->AddDCMServer( proto, s );
			}
			else if ( serverType == CON_DTM ) {
				WriteRunInfo::WriteLog( "[CTDMMaintainThread] add new dtm server, ip=%d, port=%d", data->ip, port );
				server->AddDTMServer( proto, s );
			}
		}
	}
	return 0;
}
