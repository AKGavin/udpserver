/*
 * tdmudpsendthread.h
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#ifndef TDMUDPSENDTHREAD_H_
#define TDMUDPSENDTHREAD_H_

#include "udpcommonsendthread.h"
#include "tdmconstants.h"
#include "tdmprotorequest.h"
#include "tdmwhitelist.h"
#include "tdmhotstats.h"


class CTDMUdpSendThread : public CUdpCommonSendThread
{
public:
	CTDMUdpSendThread(int iEntityType,  int iEntityId, protocol_type_t proto);
	virtual ~CTDMUdpSendThread();

protected:
	int MsgProcess(CMsg * pMsg);
	virtual CTDMProtoRequest* DecodeRequest( CUdpCommonBufMsgBody* body ) = 0;
	virtual void ProcessReq( CTDMProtoRequest* req ) = 0;
	void ProcessDownloadTask( CTDMProtoRequest* req );
	void ProcessStatsTask( CTDMProtoRequest* req );
	void SendDownloadTaskToDCM( CTDMProtoRequest* req );

	virtual void composeWhiteItemData( CTDMProtoRequest* req, pkg_data_t* data ) = 0;

	int addToWhiteList( CTDMProtoRequest* req, whiteList_data_t** whiteItem );
	int addToHotpoint( CTDMHotStats* hotStats, CTDMProtoRequest* req, hotItem_t** hotItem );

private:
	static bool ShouldSendTaskToDCM( whiteList_data_t* whiteItem, CTDMProtoRequest* req );


protected:
	protocol_type_t m_nProto;

public:
	unsigned int hotTimeThreshold;
};



#endif /* TDMUDPSENDTHREAD_H_ */
