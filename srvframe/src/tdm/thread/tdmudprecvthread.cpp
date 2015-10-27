/*
 * tdmudprecvthread.cpp
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */


#include "tdmudprecvthread.h"
#include "framecommon/framecommon.h"
#include "commmsg.h"
#include "threadgroup.h"
#include "tdmconstants.h"
#include <iostream>

CTDMUdpRecvThread::CTDMUdpRecvThread(int iEntityType, int iEntityId, string &strServerIp, unsigned short usPort, protocol_type_t proto)
	: CUdpCommonRecvThread(iEntityType, iEntityId, strServerIp, usPort)
{
	m_nRunFlag = 1;
	m_nProto = proto;
	m_nDestThreadEntityType = GetSendThreadEntityType( proto );
}


int CTDMUdpRecvThread::Run()
{
	int iRecvLen = 0;
	char* pTmpBuf;
	while( m_nRunFlag )
	{
		iRecvLen = m_UdpSocket.UdpRecv();
		if(iRecvLen > 0)
		{
			pTmpBuf = new char[iRecvLen];
			memcpy(pTmpBuf , m_UdpSocket.GetRecvBuf(),iRecvLen);
			CUdpCommonBufMsgBody * pUdpBufMsgBody = new CUdpCommonBufMsgBody(
					pTmpBuf, iRecvLen, m_UdpSocket.GetSocket(),
					ntohl(m_UdpSocket.GetCurSrcAddr().sin_addr.s_addr),
					ntohs(m_UdpSocket.GetCurSrcAddr().sin_port));
			SendMsg( m_nDestThreadEntityType, RANDOMENTITYID, 0 ,UDPCLIENTMSG, pUdpBufMsgBody, 0 );
		}
	}
	return 0;
}


