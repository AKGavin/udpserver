/*
 * tdmudpsendthread.cpp
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#include "tdmudpsendthread.h"

/*
 * tdmudpsendthread.cpp
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#include "tdmdbwhitelist.h"
#include "tdmsparsehashhotstats.h"
#include "tdmconstants.h"
#include "mainserver.h"
#include "tdmserverlist.h"
#include "tdmhotstats.h"


CTDMUdpSendThread::CTDMUdpSendThread(int iEntityType,  int iEntityId, protocol_type_t proto):CUdpCommonSendThread(iEntityType, iEntityId) {
		m_nProto = proto;
		hotTimeThreshold = CTDMHotStats::getStatsThreshold( proto );
}


CTDMUdpSendThread::~CTDMUdpSendThread(){

}


int CTDMUdpSendThread::MsgProcess(CMsg * pMsg)
{
	CUdpCommonBufMsgBody* body = (CUdpCommonBufMsgBody*) pMsg->GetMsgBody();
	CTDMProtoRequest* req = DecodeRequest( body );
	if ( req == NULL ) {
		return -1;
	}

	ProcessReq( req );

	delete req;
	return 0;
}


void CTDMUdpSendThread::ProcessStatsTask( CTDMProtoRequest* req )
{
	WriteRunInfo::WriteLog( "[CTDMUdpSendThread] ProcessStatsTask, proto=%d, hash=%s", m_nProto, req->GetInfohash() );
	whiteList_data_t* whitelistItem = NULL;
	CTDMWhiteList* whiteList = new CTDMDBWhiteList();
	int ret = whiteList->findEntry( m_nProto, req->GetInfohash(), &whitelistItem );
	delete whiteList;

	if ( ret == -1 ) {
		return;
	}

	if ( whitelistItem == NULL ) {
		//put it into hospoint list
		CTDMHotStats* hotStats = new CTDMSparseHashHotStats();
		hotItem_t* hotItem = NULL;
		ret = addToHotpoint( hotStats, req, &hotItem );
		if ( ret == -1 ) {
			delete hotStats;
			WriteRunInfo::WriteLog( "[CTDMUdpSendThread] addToHotPoint error,proto=%d, hash=%s", m_nProto, req->GetInfohash() );
			return ;
		}

		if ( hotItem->count >= hotTimeThreshold ) {
			//delete from hotpoint
			hotStats->deleteEntry( m_nProto, req->GetInfohash() );
			delete hotStats;
			ProcessDownloadTask( req );
		}
	}
	else {
		if ( whitelistItem->status == WHITELIST_STATUS_OK ) {
			tdm_dtm_whiteitem_t* sendPacket = (tdm_dtm_whiteitem_t*) malloc( sizeof(tdm_dtm_whiteitem_t) );
			ret = composeDTMAddWhiteItemPacket( sendPacket, m_nProto, req->GetInfohash(), whitelistItem );
			if ( ret != -1 ) {
				m_bSendDataFlag = true;
				m_iSendPackecLen = sizeof(tdm_dtm_whiteitem_t);
				m_pSendBuf = (char*) sendPacket;
			}
			WriteRunInfo::WriteLog( "[CTDMUdpSendThread] composeDTMAddWhiteItemPacket, proto=%d, hash=%s", m_nProto, req->GetInfohash() );
		}
		else if ( whitelistItem->status == WHITELIST_STATUS_DISABLED ) {
			tdm_dtm_disableitem_t* sendPacket = (tdm_dtm_disableitem_t*) malloc( sizeof(tdm_dtm_disableitem_t) );
			ret = composeDTMDisableItemPacket( sendPacket, m_nProto, req->GetInfohash(), whitelistItem );
			if ( ret != -1 ) {
				m_bSendDataFlag = true;
				m_iSendPackecLen = sizeof(tdm_dtm_disableitem_t);
				m_pSendBuf = (char*) sendPacket;
			}
			WriteRunInfo::WriteLog( "[CTDMUdpSendThread] composeDTMDisableItemPacket, proto=%d, hash=%s", m_nProto, req->GetInfohash() );
		}
		else if ( whitelistItem->status == WHITELIST_STATUS_PENDING ) {
			//do noting
		}
		else {
			WriteRunInfo::WriteLog( "[CTDMUdpSendThread] whitelist item status invalid, infohash=%s", req->GetInfohash() );
		}

		CTDMWhiteList::freeWhitelistItem( m_nProto, whitelistItem );
	}
}


void CTDMUdpSendThread::ProcessDownloadTask( CTDMProtoRequest* req )
{
	whiteList_data_t* whiteItem = NULL;
	char* infohash = req->GetInfohash();
	WriteRunInfo::WriteLog( "[CTDMUdpSendThread] ProcessDownloadTask, proto=%d, hash=%s", m_nProto, infohash );

	int ret = addToWhiteList( req, &whiteItem );
	if ( ret == -1 ) {
		WriteRunInfo::WriteLog( "[CTDMUdpSendThread] add white list error, proto=%d, hash=%s", m_nProto, infohash );
		return;
	}

	if ( whiteItem->status == WHITELIST_STATUS_OK ) {
		tdm_dtm_whiteitem_t* sendPacket = (tdm_dtm_whiteitem_t*) malloc( sizeof(tdm_dtm_whiteitem_t) );
		ret = composeDTMAddWhiteItemPacket( sendPacket, m_nProto, infohash, whiteItem );
		if ( ret != -1 ) {
			m_bSendDataFlag = true;
			m_iSendPackecLen = sizeof(tdm_dtm_whiteitem_t);
			m_pSendBuf = (char*) sendPacket;
		}
		WriteRunInfo::WriteLog( "[CTDMUdpSendThread] composeDTMAddWhiteItemPacket, proto=%d, hash=%s", m_nProto, infohash );
	}
	else if ( whiteItem->status == WHITELIST_STATUS_DISABLED ) {
		tdm_dtm_disableitem_t* sendPacket = (tdm_dtm_disableitem_t*) malloc( sizeof(tdm_dtm_disableitem_t) );
		ret = composeDTMDisableItemPacket( sendPacket, m_nProto, infohash, whiteItem );
		if ( ret != -1 ) {
			m_bSendDataFlag = true;
			m_iSendPackecLen = sizeof(tdm_dtm_disableitem_t);
			m_pSendBuf = (char*) sendPacket;
		}
		WriteRunInfo::WriteLog( "[CTDMUdpSendThread] composeDTMDisableItemPacket, proto=%d, hash=%s", m_nProto, infohash );
	}
	else if ( whiteItem->status == WHITELIST_STATUS_PENDING ) {
		if ( ret == 0 || ShouldSendTaskToDCM(whiteItem, req) ) {
			//send download task to dcm
			SendDownloadTaskToDCM( req );
		}
	}
	else {
		WriteRunInfo::WriteLog( "[CTDMUdpSendThread] whitelist item status invalid, infohash=%s", infohash );
	}

	CTDMWhiteList::freeWhitelistItem( m_nProto, whiteItem );
}


bool CTDMUdpSendThread::ShouldSendTaskToDCM( whiteList_data_t* whiteItem, CTDMProtoRequest* req )
{
	if ( whiteItem == NULL ) return true;

	bool shouldSend = true;
	time_t now;
	time( &now );
	time_t timeout = 0;

	if ( whiteItem->status == WHITELIST_STATUS_PENDING ) {
		shouldSend = true;
		if ( whiteItem->updateTime > 0 ) {
			int64_t size = req->GetContentSize();
			if ( size > 0 ) {
				timeout = (size * 8) / 500;		//default speed is 500kbps
			}
			else {
				timeout = 25 * 60;				//default timeout is 25 minutes
			}

			if ( now - whiteItem->updateTime < timeout ) {
				shouldSend =  false;
			}
		}
	}
	else if ( whiteItem->status == WHITELIST_STATUS_DISABLED ) {
		shouldSend = false;
		if ( whiteItem->updateTime > 0 ) {
			timeout = 8 * 3600;		//default timeout is 8 hours
			if ( now - whiteItem->updateTime > timeout ) {
				shouldSend = true;
			}
		}
	}

	if ( shouldSend ) {
		//update updatetime in db
		CTDMDBWhiteList* manager = new CTDMDBWhiteList();
		manager->changeEntryStatus( whiteItem->data.type, whiteItem->infohash, WHITELIST_STATUS_PENDING, whiteItem->dcmIp, whiteItem );
		delete manager;
	}
	return shouldSend;
}


void CTDMUdpSendThread::SendDownloadTaskToDCM( CTDMProtoRequest* req )
{
	WriteRunInfo::WriteLog( "[CTDMUdpSendThread] SendDownloadTaskToDCM, proto=%d, hash=%s", m_nProto, req->GetInfohash() );
	cmd_data_t* cmd = req->composePacketForDCMDownload();
	if ( cmd == NULL ) {
		WriteRunInfo::WriteLog( "[CTDMUdpSendThread] compose dcm task packet error, proto=%d, hash=%s", m_nProto, req->GetInfohash() );
		return;
	}

	CTDMServerList* serverList = ((CMainServer*) CMainServer::GetInstance())->GetServerList( m_nProto );
	if ( serverList == NULL ) {
		free(cmd);
		WriteRunInfo::WriteLog( "[CTDMUdpSendThread] no valid DCM server, proto=%d, hash=%s", m_nProto, req->GetInfohash() );
		return;
	}

	server_info_t* dcmServer = NULL;
	lb_type_t lbType = ((CMainServer*) CMainServer::GetInstance())->m_tdmConf.GetProtoConfigItem( m_nProto )->nDcmLbType;
	if ( lbType == LB_ROTATION ) {
		dcmServer = serverList->GetNextServerByRotation();
	}
	else {
		dcmServer = serverList->GetNextServerByHash( req->GetInfohash() );
	}

	if ( dcmServer == NULL ) {
		free(cmd);
		WriteRunInfo::WriteLog( "[CTDMUdpSendThread] no valid DCM server2, proto=%d, hash=%s", m_nProto, req->GetInfohash() );
		return;
	}

	int ret = SendUdpPacket( (char*) (*(dcmServer->m_sServerAddr)).c_str(), dcmServer->m_nPort, (char*)cmd, sizeof(cmd_data_t) );
	free(cmd);
	if ( ret == -1 ) {
		WriteRunInfo::WriteLog( "[CTDMUdpSendThread] error to send request to dcm, proto=%d, hash=%s", m_nProto, req->GetInfohash() );
	}
	else {
		WriteRunInfo::WriteLog( "[CTDMUdpSendThread] send task to dcm server %s:%d, proto=%d, hash=%s",
				(*dcmServer->m_sServerAddr).c_str(), dcmServer->m_nPort ,m_nProto, req->GetInfohash() );
	}
}



//return -1: error
//return 0: success, whiteItem not exists
//return 1: success, whiteItem already exists
int CTDMUdpSendThread::addToWhiteList( CTDMProtoRequest* req, whiteList_data_t** whiteItem )
{
	pkg_data_t packageData;
	composeWhiteItemData( req, &packageData );
	CTDMWhiteList* whiteList = new CTDMDBWhiteList();
	char* infohash = req->GetInfohash();
	int ret = whiteList->findEntry( m_nProto, infohash, whiteItem );
	if ( ret == -1 ) {
		delete whiteList;
		return -1;
	}

	if ( *whiteItem != NULL ) {
		delete whiteList;
		return 1;
	}
	else {
		ret = whiteList->addEntry( WHITEITEM_FROM_DTM_TASK, infohash, &packageData, WHITELIST_STATUS_PENDING, 0, whiteItem );
		delete whiteList;
		if ( ret == -1 ) return -1;
		else return 0;
	}
}


int CTDMUdpSendThread::addToHotpoint( CTDMHotStats* hotStats, CTDMProtoRequest* req, hotItem_t** hotItem )
{
	pkg_data_t packageData;
	composeWhiteItemData( req, &packageData );
	int ret = hotStats->addEntry( req->GetInfohash(), &packageData, hotItem );
	return ret;
}
