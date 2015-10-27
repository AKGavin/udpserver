/*
* =======================================================================
* Copyright (c) 2009-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: udpcommonsendthread.h
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
#ifndef _UDPCOMMONSENDTHREAD_H_44178A90_4896_47c5_8D93_234E10099222_
#define _UDPCOMMONSENDTHREAD_H_44178A90_4896_47c5_8D93_234E10099222_

#include "workthread.h"
#include "synqueue.h"
#include "framecommon/framecommon.h"
using namespace MYFRAMECOMMON;

class CUdpCommonSendThread : public CWorkThread
{
public:
	CUdpCommonSendThread(int iEntityType,  int iEntityId);
	~CUdpCommonSendThread(){}
public:
	virtual int Run();
	virtual int PutMsgToInputList(CMsg *pMsg);
protected:
	virtual int Process(CMsg * pMsg);
	virtual int MsgProcess(CMsg * pMsg) = 0;
	void SetSendDataFlag();
	void ClearSendDataFlag();
	bool IfHaveDataToSend();
	void DelSendBuf();

protected:
	char *m_pSendBuf;
	int m_iSendPackecLen;
	bool m_bSendDataFlag;
protected:
	int m_iCurSendSocket;
	CUdpCommonSock m_UdpSock;
protected:
	CQueue m_InputList;
	
};


#endif

