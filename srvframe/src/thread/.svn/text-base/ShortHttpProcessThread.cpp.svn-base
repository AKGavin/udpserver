/*
* =======================================================================
* Copyright (c) 2009-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: ShortHttpProcessThread.cpp
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
#include "ShortHttpProcessThread.h"
CShortHttpProcessThread::CShortHttpProcessThread(int iEntityType, int iEntityId) : CNetProcessThread(iEntityType, iEntityId)
{

}

CShortHttpProcessThread::~CShortHttpProcessThread()
{

}

int CShortHttpProcessThread::Process(CMsg * pMsg)
{
	TMsg *pTMsg = pMsg->GetTMsg();
	if ( !pTMsg )
	{
		return 0;
	}
	WriteRunInfo::WriteTrace("The ShortConnected thread recv a  msg , msgtype %d...", pTMsg->msgType);

	if (pTMsg->srcEntityType == PROCESSENTITY)	//process message from thread with entitytype of PROCESSENTITY(wangtiefeng : 09-07-16) 
	{
		if (pTMsg->msgType == CLIENTMSGTYPE)	//process message with type of CLIENTMSGTYPE
		{
			CClientMsgBody *pBody = (CClientMsgBody *)pMsg->GetMsgBody();
			if ( !pBody )
			{
				return 0;
			}
			CSocketTask *pSocketTask = FindSocketTask(pTMsg->taskId);
			if (pSocketTask != NULL)
			{
				pSocketTask->PutMsgToSendList(pBody->GetBuffer(), pBody->GetLen());
				pBody->DisAttachBuf();
			}
		}
		else
		{
			WriteRunInfo::WriteLog("Send&recv  thread recv invalid msgtype %d", pTMsg->msgType);
		}
	}
	else if (pTMsg->srcEntityType == LISTENENTITY)
	{
		if (pTMsg->msgType == RECVMSGTYPE)
		{
			CRecvMsgBody *pBody = (CRecvMsgBody*)pMsg->GetMsgBody();
			CTcpSocket *pSocket = new CTcpSocket(pBody->GetSockAddr(), sizeof(sockaddr_in), pBody->GetSocket());
			CSocketTask *tmpSocketTask = NULL;
			tmpSocketTask = CMyserver::GetInstance()->GetClassFactory()->GenSocketTask(this,  
				pSocket, RECVANDSENDTASK);     //create task related to current thread(wangtiefeng : 09-07-16)          
			tmpSocketTask->Init();         
			tmpSocketTask->SetTimeStat(pBody->GetTimeStat());
		}
		else
		{
			WriteRunInfo::WriteLog("Send&recv  thread recv invalid msgtype %d", pTMsg->msgType);
		}
	}
	else
	{
		CNetProcessThread::Process( pMsg );
	}
	return 0;
}
