/*
 * tdmdcmprocessthread.h
 *
 *  Created on: 2013-12-29
 *      Author: root
 */

#ifndef TDMDCMPROCESSTHREAD_H_
#define TDMDCMPROCESSTHREAD_H_

#include "tdmconstants.h"
#include "udpcommonsendthread.h"

class CTDMDCMProcessThread : public CUdpCommonSendThread
{
private:
	int m_nDTMSock;

public:
	CTDMDCMProcessThread(int iEntityType,  int iEntityId);
	~CTDMDCMProcessThread();

protected:
	int MsgProcess(CMsg * pMsg);

private:
	int ProcessAddWhiteItem( cmd_data_t* cmd );
	int ProcessDelWhiteItem( cmd_data_t* cmd );
	int ProcessDisableWhiteItem( cmd_data_t* cmd );
	int ProcessThunderQuery( cmd_data_t* cmd );

	int AddWhiteItemToDTM( cmd_data_t* cmd, whiteList_data_t* whiteItem, char* infohash );
	int DelWhiteItemToDTM( cmd_data_t* cmd, whiteList_data_t* whiteItem, char* infohash );
	int DisableWhiteItemToDTM( cmd_data_t* cmd, whiteList_data_t* whiteItem, char* infohash );
	int SendPacketToDTM( protocol_type_t proto, void* data, int len );


};

#endif /* TDMDCMPROCESSTHREAD_H_ */
