/*
 * tdmudprecvthread.h
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#ifndef TDMUDPRECVTHREAD_H_
#define TDMUDPRECVTHREAD_H_

#include "udpcommonrecvthread.h"
#include "tdmconstants.h"

class CTDMUdpRecvThread : public CUdpCommonRecvThread
{
public:
	CTDMUdpRecvThread(int iEntityType,  int iEntityId, string &strServerIp, unsigned short usPort, protocol_type_t proto);
	~CTDMUdpRecvThread(){}


public:
	int Run();

private:
	protocol_type_t m_nProto;
	int m_nDestThreadEntityType;
};


#endif /* TDMUDPRECVTHREAD_H_ */
