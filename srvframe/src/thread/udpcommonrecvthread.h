/*
* =======================================================================
* Copyright (c) 2009-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: udpcommonrecvthread.h
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
#ifndef _UDPCOMMONRECVTHREAD_H_35B4EEE1_4FBF_4646_BDAC_BFEF2D18623C_
#define _UDPCOMMONRECVTHREAD_H_35B4EEE1_4FBF_4646_BDAC_BFEF2D18623C_

#include <string>
#include "workthread.h"
#include "framecommon/framecommon.h"
using namespace MYFRAMECOMMON;
using namespace std;

class CUdpCommonRecvThread : public CWorkThread
{
public:
	CUdpCommonRecvThread(int iEntityType,  int iEntityId, string &strServerIp, unsigned short usPort);
	~CUdpCommonRecvThread(){}
public:
	virtual int Init();
	virtual int Run();
	virtual int PutMsgToInputList(CMsg *pMsg);
protected:
	virtual int Process(CMsg *pMsg);
protected:
	CUdpCommonSock m_UdpSocket;
};
#endif



