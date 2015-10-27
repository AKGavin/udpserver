/*
 * tdmthunderrequest.h
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#ifndef TDMHTTPREQUEST_H_
#define TDMHTTPREQUEST_H_

#include "commmsg.h"
#include "tdmconstants.h"
#include "tdmprotorequest.h"

class CTDMHttpRequest : public CTDMProtoRequest
{
public:
	http_data_t m_req;

public:
	CTDMHttpRequest();
	virtual ~CTDMHttpRequest();
	http_data_t* GetReq() {
		return &m_req;
	}

	bool decode( CUdpCommonBufMsgBody* msgBody );
	cmd_data_t* composeProtoPacketForDCMDownload( );

	static int GetDataSize( http_data_t* data );

private:
	int parse_udp_packet( char * recv,int datalen );
};


#endif /* TDMTHUNDERREQUEST_H_ */
