/*
 * tdmudpsendthread.cpp
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#include "tdmthundersendthread.h"
#include "tdmdbwhitelist.h"
#include "tdmsparsehashhotstats.h"
#include "tdmconstants.h"
#include "mainserver.h"
#include "tdmserverlist.h"

CTDMProtoRequest* CTDMThunderSendThread::DecodeRequest( CUdpCommonBufMsgBody* body )
{
	CTDMThunderRequest* req = new CTDMThunderRequest();
	bool b = req->decode( body );
	if ( !b ) {
		delete req;
		WriteRunInfo::WriteLog( "[CTDMThunderSendThread] receive thunder task message, parse error" );
		return NULL;
	}
	else {
		thunder_data_t* data = req->GetReq();

		char cid[41];
		char peerId[17];
		strncpy( cid, data->m_strCid, 40 );
		cid[40] = 0;
		strncpy( peerId, data->m_strPeerId, 16 );
		peerId[16] = 0;
		WriteRunInfo::WriteLog( "[CTDMThunderSendThread] receive thunder task message, reqType=%d, cid=%s, peerId=%s, infohash=%s, filesize=%llu",
				data->m_nReqType, cid, peerId, req->GetInfohash(), data->m_nFilesize );
		return req;
	}
}


void CTDMThunderSendThread::ProcessReq( CTDMProtoRequest* r )
{
	CTDMThunderRequest* req = (CTDMThunderRequest*)r;

	switch ( req->m_req.m_nReqType ) {
	case DTM_PUT1: {
		ProcessDownloadTask( req );
		break;
	}
	case DTM_PUT2: {
		ProcessStatsTask( req );
		break;
	}
	case DCM_GET: {
		ProcessQueryPeerTask( req );
	}
	default: {

	}
	}
}


void CTDMThunderSendThread::composeWhiteItemData( CTDMProtoRequest* r, pkg_data_t* data )
{
	CTDMThunderRequest* req = (CTDMThunderRequest*)r;
	data->type = PROTOCOL_THUNDER;
	memcpy( &data->thunder_data, &req->m_req, sizeof(thunder_data_t) );
}




void CTDMThunderSendThread::ProcessQueryPeerTask( CTDMThunderRequest* req )
{
	CTDMWhiteList* whiteList = new CTDMDBWhiteList();
	whiteList_data_t* whiteItem = NULL;
	int ret = whiteList->findEntry( PROTOCOL_THUNDER, req->GetInfohash(), &whiteItem );
	delete whiteList;

	if ( ret == -1 ) return;

	char* buffer = new char[MSG_BUFFER_LEN];
	int size = 0;
	if ( whiteItem != NULL ) {
		thunder_data_t* d = & whiteItem->data.thunder_data;
		if ( d != NULL ) {
			size = sprintf( buffer, "Method:CQR\r\nInfohash:%s\r\n", whiteItem->infohash );
			if ( d->m_nFilesize > 0 ) {
				size += sprintf( buffer + size, "Cid:%s\r\n", d->m_strCid);
			}
		}
	}
	else {
		buffer = '\0';
	}

	m_bSendDataFlag = true;
	m_iSendPackecLen = size;
	m_pSendBuf = buffer;
	CTDMWhiteList::freeWhitelistItem( m_nProto, whiteItem );
}


