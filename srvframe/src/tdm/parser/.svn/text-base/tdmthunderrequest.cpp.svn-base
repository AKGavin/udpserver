/*
 * tdmthunderrequest.cpp
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#include "tdmthunderrequest.h"
#include "tdmconstants.h"
#include "string.h"

#define METHOD_KEY "Method"
#define INFOHASH_KEY "Infohash"
#define CID_KEY "Cid"

extern int sessionId;


bool CTDMThunderRequest::decode( CUdpCommonBufMsgBody* msgBody )
{
	int len = msgBody->Size();
	if ( len <= 0 ) return false;

	//TODO: must copy buffer to new char array?
	char* s = new char[len+1];
	strncpy( s, msgBody->GetBuf(), len );
	s[len] = '\0';

	char* pKey = s;
	char* pValue = NULL;
	char* pReturn = NULL;

	bool method_check = false;
	bool cid_check = false;
	bool infohash_check = false;
	bool peerID_check = false;

	while ( NULL != pKey && NULL != (pReturn=strstr(pKey,"\r\n")) ) {
		*pReturn = '\0';
		if ( NULL != (pValue=strstr(pKey,":")) ) {
			*pValue = '\0';
			pValue++;
			if ( !ProcKeyValue( pKey, pValue ) ) {
				return false;
			}
			else {
				if ( strcmp(pKey, "Method") == 0 ) {
					method_check = true;
				}
				else if ( strcmp(pKey, "Cid") == 0 ) {
					cid_check = true;
				}
				else if ( strcmp(pKey, "Infohash") == 0 ) {
					infohash_check = true;
				}
				else if ( strcmp(pKey, "PeerID") == 0 ) {
					peerID_check = true;
				}
			}
		}
		else {
			return false;
		}

		pKey = pReturn + strlen("\r\n");
		if ( pKey == strstr(pKey,"\r\n") ) {
			pKey = pKey + strlen( "\r\n" );
		}
	}

	delete s;

	if ( !method_check ) {
		WriteRunInfo::WriteLog( "[CTDMThunderRequest] invalid method:%d", m_req.m_nReqType );
		return false;
	}

	if ( !cid_check ) {
		WriteRunInfo::WriteLog( "[CTDMThunderRequest] invalid cid" );
		return false;
	}

	if ( !infohash_check ) {
		WriteRunInfo::WriteLog( "[CTDMThunderRequest] invalid infohash" );
		return false;
	}

	if ( !peerID_check ) {
		WriteRunInfo::WriteLog( "[CTDMThunderRequest] invalid peerid" );
		return false;
	}

	return method_check && cid_check && infohash_check && peerID_check;
}


bool CTDMThunderRequest::ProcKeyValue( char* pKey, char* pValue )
{
	if ( pKey == NULL || pValue == NULL ) {
		return false;
	}

	if ( 0 == strcasecmp(pKey, "Method") ) {
		if ( 0 == strcasecmp(pValue, "PQI") ) {
			m_req.m_nReqType = DTM_PUT1;
		}
		else if ( 0 == strcasecmp(pValue, "KPQI") ) {
			m_req.m_nReqType = DTM_PUT2;
		}
		else if ( 0 == strcasecmp(pValue, "CQ") ) {
			m_req.m_nReqType = DCM_GET;
		}
		else {
			WriteRunInfo::WriteLog( "[CTDMThunderRequest] invalid method:%s", pValue );
			return false;
		}
	}
	else if ( 0 == strcasecmp(pKey, "Cid") ) {
		if ( strlen(pValue) < 40 ) {
			memset( m_req.m_strCid, 0, 40 );
			return false;
		}
		else {
			strncpy( m_req.m_strCid, pValue, 40 );
		}
	}
	else if ( 0 == strcasecmp(pKey, "Infohash" ) ) {
		if ( strlen(pValue) < 40 ) {
			memset( infohash, 0, 41 );
			memset( m_req.m_strInfohash, 0, 20 );
			return false;
		}
		else {
			strncpy( infohash, pValue, 40 );
			infohash[40] = '\0';
			hexStringToByte( m_req.m_strInfohash, infohash );
		}
	}
	else if ( 0 == strcasecmp(pKey, "File-Size") ) {
		m_req.m_nFilesize = atoll( pValue );
	}
	else if ( 0 == strcasecmp(pKey, "PeerID") ) {
		if ( strlen(pValue) < 16 ) {
			memset( m_req.m_strPeerId, 0, 16 );
			return false;
		}
		else {
			strncpy( m_req.m_strPeerId, pValue, 16 );
		}
	}
//	else {
//		WriteRunInfo::WriteLog( "[CTDMThunderRequest] Invalid head:%s", pKey );
//		return false;
//	}

	return true;
}


cmd_data_t* CTDMThunderRequest::composeProtoPacketForDCMDownload()
{
	int dataLen = sizeof( thunder_data_t );
	int headerLen = 3 * sizeof(int) + sizeof(pkg_type_t) + sizeof(protocol_type_t);
	int memLen = headerLen + dataLen;

	cmd_data_t* cmd = (cmd_data_t*) malloc( memLen );
	memset( cmd, 0, memLen );

	cmd->s_id = getSessionId();
	cmd->pkg_type = CMD_TDM_DCM_CONTENT_GET;
	cmd->pkg_len = dataLen + sizeof(protocol_type_t);

	cmd->pkg_data.type = PROTOCOL_THUNDER;

	thunder_data_t* data = &cmd->pkg_data.thunder_data;
	memcpy( data->m_strInfohash, m_req.m_strInfohash, 20 );
	data->m_nReqType = m_req.m_nReqType;
	strncpy( data->m_strCid, m_req.m_strCid, 40 );
	strncpy( data->m_strPeerId, m_req.m_strPeerId, 16 );
	data->m_nFilesize = m_req.m_nFilesize;

	return cmd;
}

int CTDMThunderRequest::GetDataSize( thunder_data_t* data )
{
	return sizeof( thunder_data_t );
}

unsigned long long CTDMThunderRequest::GetContentSize()
{
	return m_req.m_nFilesize;
}
