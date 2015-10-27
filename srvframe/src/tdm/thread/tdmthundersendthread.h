/*
 * tdmthundersendthread.h
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#ifndef TDMTHUNDERSENDTHREAD_H_
#define TDMTHUNDERSENDTHREAD_H_

#include "tdmudpsendthread.h"
#include "tdmthunderrequest.h"


class CTDMThunderSendThread : public CTDMUdpSendThread
{
public:
	CTDMThunderSendThread(int iEntityType,  int iEntityId, protocol_type_t proto):CTDMUdpSendThread(iEntityType, iEntityId, proto){};

protected:
	CTDMProtoRequest* DecodeRequest( CUdpCommonBufMsgBody* body );
	void ProcessReq( CTDMProtoRequest* req ) ;
	void composeWhiteItemData( CTDMProtoRequest* req, pkg_data_t* data );

private:
	void ProcessQueryPeerTask( CTDMThunderRequest* req );
};



#endif /* TDMUDPSENDTHREAD_H_ */
