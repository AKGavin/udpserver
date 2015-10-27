/*
 * tdmprotorequest.cpp
 *
 *  Created on: 2014-5-4
 *      Author: root
 */

#include "tdmprotorequest.h"

cmd_data_t* CTDMProtoRequest::composePacketForDCMDownload()
{
	cmd_data_t* data = composeProtoPacketForDCMDownload();
	if ( data ) {
		char* p = (char*) data;
		p += CMD_HEADER_SIZE;
		data->pkg_chk = calc_hash( (const unsigned char*)p, 8 );
	}
	return data;
}

