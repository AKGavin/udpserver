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

#include "HttpRecvAndSendTaskProcess.h"

CHttpRecvAndSendTaskProcess::CHttpRecvAndSendTaskProcess(
	CNetProcessThread *pNetProcessThread,
	CRecvAndSendTask *pRecvAndSendTask,
	CTcpSocket *pTcpSocket)
{
	m_pNetProcessThread = pNetProcessThread;
	m_pRecvAndSendTask = pRecvAndSendTask;
	m_pTcpSocket = pTcpSocket;
	m_bDisConnectAfterRecv = false;
	m_bDisConnectAfterSend = false;
}

CHttpRecvAndSendTaskProcess::~CHttpRecvAndSendTaskProcess()
{

}

int CHttpRecvAndSendTaskProcess::ProcessRecvPacket(const char *pPacket, unsigned int uiBufLen)
{
	int nLen = uiBufLen;
	if(uiBufLen <= 0)
		return -1;
	char *pBuf = new char[nLen];
	memcpy(pBuf, pPacket, nLen);
	CBufMsgBody *pBody = new CBufMsgBody(pBuf, nLen);
	m_pNetProcessThread->SendMsg(PROCESSENTITY, -1, -1, CLIENTMSGTYPE, pBody, m_pRecvAndSendTask->GetTaskId());
	return 0;
}

const char *CHttpRecvAndSendTaskProcess::ProcessSendPacket(const char *pOldPacket, unsigned int uiOldBufLen, unsigned int &uiNewBufLen)
{
	uiNewBufLen = uiOldBufLen;
	return pOldPacket;
}

int CHttpRecvAndSendTaskProcess::ProcessGetReq()
{
	return 0;
}

bool CHttpRecvAndSendTaskProcess::GetIfDisConnectAfterRecv()
{
	return m_bDisConnectAfterRecv;
}

void CHttpRecvAndSendTaskProcess::SetIfDisConnAfterRecv(bool bDisConnectAfterRecv)
{
	m_bDisConnectAfterRecv = bDisConnectAfterRecv;
}

bool CHttpRecvAndSendTaskProcess::GetIfDisConnectAfterSend()
{
	return m_bDisConnectAfterSend;
}

void CHttpRecvAndSendTaskProcess::SetDisConnectAfterSend(bool bDisConnectAfterSend)
{
	m_bDisConnectAfterSend = bDisConnectAfterSend;
}
