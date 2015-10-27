/*
* =======================================================================
* Copyright (c) 2009-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: udpcommonrecvthread.cpp
* Description	: 
* -----------------------------------------------------------------------
* Author        : wangtiefeng (wtf), wangtf418@163.com
* Company		: 
* Date			: 2009-07-24
* -----------------------------------------------------------------------
* History		: 
*		<author>		  <time>		<version>		<description>
*      ---------------------------------------------------------------
*		wangtiefeng		2009-07-24			1.0			
* =======================================================================
*/
#include "framecommon/framecommon.h"
#include "commmsg.h"
#include "threadgroup.h"
#include "udpcommonrecvthread.h"


CUdpCommonRecvThread::CUdpCommonRecvThread(int iEntityType, int iEntityId, string &strServerIp, unsigned short usPort) 
	: CWorkThread(iEntityType, iEntityId), m_UdpSocket(strServerIp, usPort)
{
}

int CUdpCommonRecvThread::Init()
{
	if(m_UdpSocket.CreateUdpSocket() < 0)
	{
		WriteRunInfo::WriteLog("Create Udp Socket Failed");
		return -1;
	}

	return 0;
}

int CUdpCommonRecvThread::Run()
{
	int iRecvLen = 0;
	while( m_nRunFlag )
	{
		iRecvLen = m_UdpSocket.UdpRecv();
		if(iRecvLen > 0)
		{
			char *pTmpBuf = new char[iRecvLen];
			memcpy(pTmpBuf , m_UdpSocket.GetRecvBuf(),iRecvLen);
			CUdpCommonBufMsgBody * pUdpBufMsgBody = new CUdpCommonBufMsgBody(pTmpBuf, iRecvLen, m_UdpSocket.GetSocket(), 
				ntohl(m_UdpSocket.GetCurSrcAddr().sin_addr.s_addr), ntohs(m_UdpSocket.GetCurSrcAddr().sin_port));
			SendMsg(UDPSENDANDPROCESSENTITY, RANDOMENTITYID, 0 ,UDPCLIENTMSG, pUdpBufMsgBody, 0);
		}
	}
	return 0;
}

int CUdpCommonRecvThread::PutMsgToInputList(CMsg *pMsg)
{
	if(pMsg != NULL)
		delete pMsg;
	return 0;
}

int CUdpCommonRecvThread::Process(CMsg *pMsg)
{
	return 0;
}

