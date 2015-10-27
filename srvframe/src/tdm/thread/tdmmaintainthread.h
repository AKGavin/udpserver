/*
 * tdmmaintainthread.h
 *
 *  Created on: 2013-12-29
 *      Author: root
 */

#ifndef TDMMAINTAINTHREAD_H_
#define TDMMAINTAINTHREAD_H_

#include "tdmconstants.h"
#include "udpcommonsendthread.h"


class CTDMMaintainThread : public CUdpCommonSendThread
{
public:
	CTDMMaintainThread(int iEntityType,  int iEntityId):CUdpCommonSendThread(iEntityType, iEntityId) {}

private:
	int ProcessMonitorCheck( unsigned int ip, int port );
	int ProcessMonitorDownRevive( monitor_cmd_data_t* cmd );

protected:
	int MsgProcess(CMsg * pMsg);

};

#endif /* TDMMAINTAINTHREAD_H_ */
