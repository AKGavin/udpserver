/*
* =======================================================================
* Copyright (c) 2009-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: HttpRecvAndSendTaskProcess.h
* Description	: 
* -----------------------------------------------------------------------
* Author        : wangtiefeng (wtf), wangtf418@163.com
* Company		: 
* Date			: 2009-07-15
* -----------------------------------------------------------------------
* History		: 
*		<author>		  <time>		<version>		<description>
*      ---------------------------------------------------------------
*		wangtiefeng		2009-07-15			1.0			
* =======================================================================
*/

#ifndef _HTTPRECVANDSENDTASKPROCESS_H_D54D65BC_1269_4913_A61D_7E0DAC804B6E_
#define _HTTPRECVANDSENDTASKPROCESS_H_D54D65BC_1269_4913_A61D_7E0DAC804B6E_

#include "netprocessthread.h"
#include "recvandsendtask.h"


class CHttpRecvAndSendTaskProcess
{
public:
	CHttpRecvAndSendTaskProcess(CNetProcessThread *pNetProcessThread, CRecvAndSendTask *pRecvAndSendTask, CTcpSocket *pTcpSocket);
	virtual ~CHttpRecvAndSendTaskProcess();
public:
	virtual int ProcessRecvPacket(const char *pPacket, unsigned int uiBufLen);
	virtual const char *ProcessSendPacket(const char *pOldPacket, unsigned int uiOldBufLen, unsigned int &uiNewBufLen);
	virtual int ProcessGetReq();
	virtual bool GetIfDisConnectAfterRecv();
	virtual void SetIfDisConnAfterRecv(bool bDisConnectAfterRecv);
	virtual bool GetIfDisConnectAfterSend();
	virtual void SetDisConnectAfterSend(bool bDisConnectAfterSend);
protected:
	CNetProcessThread *m_pNetProcessThread;
	CRecvAndSendTask *m_pRecvAndSendTask;
	CTcpSocket *m_pTcpSocket;
	bool m_bDisConnectAfterRecv;
	bool m_bDisConnectAfterSend;
	
};

#endif


