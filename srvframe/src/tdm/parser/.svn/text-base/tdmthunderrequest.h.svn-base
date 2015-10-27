/*
 * tdmthunderrequest.h
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#ifndef TDMTHUNDERREQUEST_H_
#define TDMTHUNDERREQUEST_H_

#include "commmsg.h"
#include "tdmconstants.h"
#include "tdmprotorequest.h"

class CTDMThunderRequest : public CTDMProtoRequest
{
public:
	thunder_data_t m_req;

public:
	thunder_data_t* GetReq() {
		return &m_req;
	}

	bool decode( CUdpCommonBufMsgBody* msgBody );
	cmd_data_t* composeProtoPacketForDCMDownload( );

	unsigned long long GetContentSize();

private:
	bool ProcKeyValue( char* pKey, char* pValue );
	int GetDataSize( thunder_data_t* data );
};


#endif /* TDMTHUNDERREQUEST_H_ */
