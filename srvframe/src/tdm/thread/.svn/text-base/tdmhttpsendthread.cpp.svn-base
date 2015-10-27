/*
 * tdmudpsendthread.cpp
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#include "tdmhttpsendthread.h"
#include "tdmhttprequest.h"
#include "tdmconstants.h"


CTDMProtoRequest* CTDMHttpSendThread::DecodeRequest( CUdpCommonBufMsgBody* body )
{
	CTDMHttpRequest* req = new CTDMHttpRequest();
	bool b = req->decode( body );
	if ( !b ) {
		delete req;
		WriteRunInfo::WriteLog( "[CTDMHttpSendThread] receive http task message, parse error" );
		return NULL;
	}
	else {
		http_data_t* data = req->GetReq();
		WriteRunInfo::WriteLog( "[CTDMHttpSendThread] receive http task message, url=%s, urlhash=%s", data->req_url, req->GetInfohash() );
		return req;
	}
}


void CTDMHttpSendThread::ProcessReq( CTDMProtoRequest* req )
{
	ProcessStatsTask( req );
}


void CTDMHttpSendThread::composeWhiteItemData( CTDMProtoRequest* r, pkg_data_t* data )
{
	CTDMHttpRequest* req = (CTDMHttpRequest*)r;
	data->type = PROTOCOL_HTTP;
	memcpy( &data->http_data, &req->m_req, sizeof(http_data_t) );
}


