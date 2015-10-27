/*
 * tdmhttpsendthread.h
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#ifndef TDMHTTPSENDTHREAD_H_
#define TDMHTTPSENDTHREAD_H_

#include "tdmudpsendthread.h"


class CTDMHttpSendThread : public CTDMUdpSendThread
{
public:
	CTDMHttpSendThread(int iEntityType,  int iEntityId, protocol_type_t proto):CTDMUdpSendThread(iEntityType, iEntityId, proto){};

protected:
	CTDMProtoRequest* DecodeRequest( CUdpCommonBufMsgBody* body );
	void ProcessReq( CTDMProtoRequest* req ) ;
	void composeWhiteItemData( CTDMProtoRequest* req, pkg_data_t* data );
};



#endif /* TDMUDPSENDTHREAD_H_ */
