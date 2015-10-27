/*
 * tdmthunderrequest.h
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#ifndef TDMPROTOREQUEST_H_
#define TDMPROTOREQUEST_H_

#include "commmsg.h"
#include "tdmconstants.h"
#include <stdlib.h>

class CTDMProtoRequest
{
protected:
	char infohash[41];

public:
	virtual ~CTDMProtoRequest() {};
	virtual bool decode( CUdpCommonBufMsgBody* msgBody ) = 0;
	virtual cmd_data_t* composeProtoPacketForDCMDownload() = 0;
	virtual unsigned long long GetContentSize() { return 0; };
	char* GetInfohash() { return infohash; };

	cmd_data_t* composePacketForDCMDownload();
};


#endif /* TDMTHUNDERREQUEST_H_ */
