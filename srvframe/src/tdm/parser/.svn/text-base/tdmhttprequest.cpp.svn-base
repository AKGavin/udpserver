/*
 * tdmhttprequest.cpp
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#include "tdmhttprequest.h"
#include "tdmconstants.h"
#include "string.h"
#include "stdlib.h"

extern int sessionId;

CTDMHttpRequest::CTDMHttpRequest()
{
	m_req.client_ip = NULL;
	m_req.server_ip = NULL;
	m_req.sitename = NULL;
	memset(m_req.urlhash, 0, 20 );
	m_req.req_url = NULL;
	m_req.req_agent = NULL;
	m_req.req_refer = NULL;
	m_req.req_cookie = NULL;
	memset( infohash, 0, 41 );
}


CTDMHttpRequest::~CTDMHttpRequest()
{
	if ( m_req.client_ip != NULL ) {
		free( m_req.client_ip );
	}
	if ( m_req.server_ip != NULL ) {
		free( m_req.server_ip );
	}
	if ( m_req.sitename != NULL ) {
		free( m_req.sitename );
	}
	if ( m_req.req_url != NULL ) {
		free( m_req.req_url );
	}
	if ( m_req.req_agent != NULL ) {
		free( m_req.req_agent );
	}
	if ( m_req.req_refer != NULL ) {
		free( m_req.req_refer );
	}
	if ( m_req.req_cookie != NULL ) {
		free( m_req.req_cookie );
	}
}


bool CTDMHttpRequest::decode( CUdpCommonBufMsgBody* msgBody )
{
	int len = msgBody->Size();
	if ( len <= 0 ) return false;

	int ret = parse_udp_packet( msgBody->GetBuf(), len );
	if ( ret == -1 ) {
		WriteRunInfo::WriteLog( "parse http request packet error" );
		return false;
	}

	return true;
}


int CTDMHttpRequest::parse_udp_packet( char * recv,int datalen )
{
	int i = 1;
	int length = 0;
	char *p;
	char *q;

	if(recv == NULL || datalen == 0) {
		return -1;
	}

	p = recv;

	while( (q = strstr(p,"\n")) != NULL ) {
		*q = '\0';
		if(i == 1) {
			if((q-p) == 0){
				m_req.server_ip = (char *) calloc( 2, sizeof(char) );
				if( m_req.server_ip == NULL) {
						WriteRunInfo::WriteLog( "[CTDMHttpRequest] server_ip calloc fail" );
						return -1;
				}
			}
			else {
				m_req.server_ip = (char *) calloc( (q-p+1), sizeof(char) );
				if( m_req.server_ip == NULL){
					WriteRunInfo::WriteLog( "[CTDMHttpRequest] server_ip calloc fail" );
					return -1;
				}
				memcpy( m_req.server_ip, p,q-p );
				length += (q-p+1);
			}
		}

		if (i == 2) {
			if ((q - p) == 0) {
				m_req.client_ip = (char *) calloc( 2, sizeof(char) );
				if ( m_req.client_ip == NULL ) {
					WriteRunInfo::WriteLog( "[CTDMHttpRequest] m_req.client_ip calloc fail" );
					return -1;
				}
			}
			else {
				m_req.client_ip = (char *) calloc( (q - p + 1), sizeof(char) );
				if (m_req.client_ip == NULL) {
					WriteRunInfo::WriteLog( "[CTDMHttpRequest] m_req.client_ip calloc fail");
					return -1;
				}
				memcpy( m_req.client_ip, p, q - p );
				length += (q - p + 1);
			}
		}
		if (i == 3) {
			if ((q - p) == 0) {
				m_req.sitename = (char *) calloc( 2, sizeof(char) );
				if ( m_req.sitename == NULL) {
					WriteRunInfo::WriteLog( "[CTDMHttpRequest] m_req.sitename calloc fail" );
					return -1;
				}
			} else {
				m_req.sitename = (char *) calloc( (q - p + 1), sizeof(char) );
				if (m_req.sitename == NULL) {
					WriteRunInfo::WriteLog( "[CTDMHttpRequest] m_req.sitename calloc fail");
					return -1;
				}
				memcpy( m_req.sitename, p, q - p );
				length += (q - p + 1);
			}
		}
		if (i == 4) {
			if ((q - p) == 0) {
				memset( infohash, 0, 41 );
				memset( m_req.urlhash, 0, 20 );
				WriteRunInfo::WriteLog( "[CTDMHttpRequest] m_req.urlhash calloc fail");
				return -1;
			}
			else {
				int len = q-p+1;
				if ( len != 41 ) {
					WriteRunInfo::WriteLog( "[CTDMHttpRequest] urlhash invalid, len=%d", len );
					return -1;
				}

				memcpy( infohash, p, q - p );
				hexStringToByte( m_req.urlhash, infohash );
				length += (q - p + 1);
			}
		}
		if (i == 5) {
			if ( (q - p) == 0 ) {
				m_req.req_url = (char *) calloc( 2, sizeof(char) );
				if (m_req.req_url == NULL) {
					WriteRunInfo::WriteLog( "[CTDMHttpRequest] m_req.req_url calloc fail");
					return -1;
				}
			} else {
				m_req.req_url = (char *) calloc( (q - p + 1), sizeof(char) );
				if (m_req.req_url == NULL) {
					WriteRunInfo::WriteLog( "[CTDMHttpRequest] m_req.req_url calloc fail");
					return -1;
				}
				memcpy(m_req.req_url, p, q - p);
				length += (q - p + 1);
			}
		}
		if (i == 6) {
			if ((q - p) == 0) {
				m_req.req_agent = (char *) calloc( 2, sizeof(char) );
				if ( m_req.req_agent == NULL) {
					WriteRunInfo::WriteLog( "[CTDMHttpRequest] m_req.req_agent calloc fail" );
					return -1;
				}
			} else {
				m_req.req_agent = (char *) calloc( (q - p + 1), sizeof(char) );
				if ( m_req.req_agent == NULL) {
					WriteRunInfo::WriteLog( "[CTDMHttpRequest] m_req.req_agent calloc fail");
					return -1;
				}
				memcpy( m_req.req_agent, p, q - p);
				length += (q - p + 1 );
			}
		}
		if (i == 7) {
			if ((q - p) == 0) {
				m_req.req_refer = (char *) calloc( 2, sizeof(char) );
				if ( m_req.req_refer == NULL) {
					WriteRunInfo::WriteLog( "[CTDMHttpRequest] m_req.req_refer calloc fail");
					return -1;
				}
			} else {
				m_req.req_refer = (char *) calloc( (q - p + 1), sizeof(char) );
				if (m_req.req_refer == NULL) {
					WriteRunInfo::WriteLog( "[CTDMHttpRequest] m_req.req_refer calloc fail");
					return -1;
				}
				memcpy(m_req.req_refer, p, q - p);
				length += (q - p + 1);
			}
		}
		if (i == 8) {
			if ((q - p) == 0) {
				m_req.req_cookie = (char *) calloc( 2, sizeof(char) );
				if (m_req.req_cookie == NULL) {
					WriteRunInfo::WriteLog( "[CTDMHttpRequest] m_req.req_cookie calloc fail" );
					return -1;
				}
			} else {
				m_req.req_cookie = (char *) calloc( (q - p + 1), sizeof(char) );
				if (m_req.req_cookie == NULL) {
					WriteRunInfo::WriteLog( "[CTDMHttpRequest] m_req.req_cookie calloc fail");
					return -1;
				}
				memcpy(m_req.req_cookie, p, q - p);
				length += (q - p + 1);
			}
		}

		if (length == datalen) {
			break;
		}

		p = q;
		p++;
		i++;
	}

	if ( m_req.req_url == NULL || strlen(m_req.req_url) == 0 ) {
		WriteRunInfo::WriteLog( "[CTDMHttpRequest] invalid http download task, req_url is null");
		return -1;
	}

	if ( infohash == NULL || strlen(infohash) == 0 ) {
		WriteRunInfo::WriteLog( "[CTDMHttpRequest] invalid http download task, infohash is null");
		return -1;
	}

	return 0;
}


cmd_data_t* CTDMHttpRequest::composeProtoPacketForDCMDownload()
{
	int dataLen = GetDataSize( &m_req );
	int headerLen = 3 * sizeof(int) + sizeof(pkg_type_t) + sizeof(protocol_type_t);
	int memLen = headerLen + dataLen;

	cmd_data_t* cmd = (cmd_data_t*) malloc( memLen );
	memset( cmd, 0, memLen );

	cmd->s_id = getSessionId();
	cmd->pkg_type = CMD_TDM_DCM_CONTENT_GET;
	cmd->pkg_len = dataLen + sizeof(protocol_type_t);
	cmd->pkg_data.type = PROTOCOL_HTTP;

	http_data_t* pd = (http_data_t*) ((char*) cmd + headerLen);

	memcpy( pd->urlhash, m_req.urlhash, 20 );

	char* buffer = (char*) cmd + headerLen + 20;
	char* p = buffer;
	strcpy( p, m_req.client_ip );

	p += strlen(p) + 1;
	strcpy( p, m_req.server_ip );

	p += strlen(p) + 1;
	strcpy( p, m_req.sitename );

	p += strlen(p) + 1;
	strcpy( p, m_req.req_url );

	p += strlen(p) + 1;
	strcpy( p, m_req.req_agent );

	p += strlen(p) + 1;
	strcpy( p, m_req.req_refer );

	p += strlen(p) + 1;
	strcpy( p, m_req.req_cookie );

	return cmd;
}


int CTDMHttpRequest::GetDataSize( http_data_t* data )
{
	int dataLen = 20 + strlen(data->client_ip) + strlen(data->server_ip) + strlen(data->sitename) +
			strlen(data->req_url) + strlen(data->req_agent) +
			strlen(data->req_refer) + strlen(data->req_cookie) + 8;
	return dataLen;
}

