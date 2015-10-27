/*
 * tdmclassfactory.cpp
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#include <string>

#include "tdmclassfactory.h"
#include "tdmconstants.h"
#include "tdmhttpsendthread.h"
#include "tdmthundersendthread.h"
#include "tdmdcmprocessthread.h"
#include "tdmmaintainthread.h"
#include "tdmudprecvthread.h"
#include "tdmconf.h"

CWorkThread * CTDMClassFactory::GenWorkThread(int nEntityType, int nEntityId, string sAnnexData, void *arg)
{
	CWorkThread* pWorkThread = NULL;
	if ( nEntityType > RECV_START_THREADENTITY && nEntityType < RECV_END_THREADENTITY ) {
		TDMProtoConfItem* protoConf = (TDMProtoConfItem*) arg;
		string sHost = protoConf->sServerAddr;
		int nPort = protoConf->nUdpPort;
		protocol_type_t proto = protoConf->proto;
		pWorkThread = new CTDMUdpRecvThread( nEntityType, nEntityId, sHost, nPort, proto );
	}
	else if ( nEntityType > RECV_END_THREADENTITY && nEntityType < SEND_END_THREADENTITY ) {
		TDMProtoConfItem* protoConf = (TDMProtoConfItem*) arg;
		protocol_type_t proto = protoConf->proto;

		if ( nEntityType == SEND_HTTP_THREADENTITY ) {
			pWorkThread = new CTDMHttpSendThread( nEntityType, nEntityId, proto );
		}
		else if ( nEntityType == SEND_THUNDER_THREADENTITY ) {
			pWorkThread = new CTDMThunderSendThread( nEntityType, nEntityId, proto );
		}
		else if ( nEntityType == SEND_DCM_THREADENTITY ) {
			pWorkThread = new CTDMDCMProcessThread( nEntityType, nEntityId );
		}
		else if ( nEntityType == SEND_MAINTAIN_THREADENTITY ) {
			pWorkThread = new CTDMMaintainThread( nEntityType, nEntityId );
		}
	}

	return pWorkThread;
}



