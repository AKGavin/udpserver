/****************************************************************************
**
**  Copyright  (c)  2008-2009,  Baofeng, Inc.  All Rights Reserved.
**
**  Subsystem:    ConnectServer
**  File:         	     LongConnectedListenTask.cpp
**  Created By:   fwx
**  Created On:   2009/01/08
**
**  Purpose:
**     LongConnectedListenTask
**
**  History:
**  Programmer      Date         Version        Number      Description
**  --------------- --------   ---------- ------------------------------
**    fwx       2009/01/08         1.0            01          creation
****************************************************************************/
#include "listenthread.h"
#include "LongConnectedListenTask.h"


CLongConnectedListenTask::CLongConnectedListenTask( CTcpSocket *pSocket, CNetProcessThread *pWorkThread )
	: CSocketTask( pSocket,  pWorkThread )
{
	m_iTaskType = LONGCONNECTEDLISTENTASK;
	m_nTaskTimeOut = -1;
}

CLongConnectedListenTask::~CLongConnectedListenTask()
{
	
}

int CLongConnectedListenTask::Init( void )
{
	AddToEpoll();
	return 0;
}

int CLongConnectedListenTask::Process( CMsg *pMsg )
{
	return 0;
}

int CLongConnectedListenTask::HandleProcess( int event )
{
//	#ifndef __USE_EPOLL
//	DelFromEpoll();
//	#endif
	CListenThread *pThread = ( CListenThread* )m_pThread;
	if ( !pThread )
	{
		return 0;
	}
	 
	pThread->GetListenBlock()->mutex.Lock();
	if ( event & (MYPOLLIN | MYPOLLRDNORM) )
	{
	    struct sockaddr_in tmpAddr;
	    memset(&tmpAddr, 0, sizeof(sockaddr_in));
	    int iSocketSize=sizeof(sockaddr_in);
	    //int ret  = 1;
	    while (1)
	    {                                             
	        int iSocket = accept(m_pTcpSocket->GetSocket(), (struct sockaddr *)&tmpAddr, (socklen_t*)&iSocketSize);
	        if (iSocket > 0)
	        {
	            WriteRunInfo::WriteLog("Recv a long connect request from %s:%d", inet_ntoa(tmpAddr.sin_addr), ntohs(tmpAddr.sin_port));
	            WRITE_TRACE("Recv a client connect request");
	            CMyserver::GetInstance()->GetCommonStat().AddNormalStat(RECVPKGNUMSTAT);
	             
	            if (CTcpSocket::GetNum() > CMyserver::GetInstance()->GetBaseConf().GetConfigItem().nMaxFdSize) //如果太忙了， 不再接入
	            {
	                WriteRunInfo::WriteInfoRun("The fdsize is %d, greater than %d, close the connect", CTcpSocket::GetNum(),
	                        CMyserver::GetInstance()->GetBaseConf().GetConfigItem().nMaxFdSize);
	                close(iSocket);
	                continue;
	            }
	            CTimeStat timestat;
	            timestat.Begin(); //打印开始的时间

	            CRecvMsgBody *pBody = new CRecvMsgBody(iSocket, tmpAddr);
			    if ( !pBody )
			    {
					return 0;
			    }
	            pBody->SetTimeStat(timestat);
	            m_pThread->SendMsg( LONGCONNECTEDPROCESSENTITY, RANDOMENTITYID, 0, RECVANDSENDMSGTYPE, pBody, 0);               
	        }
	        else  //句柄accept完毕
	        {
	            break;
	        }
	    }
	}
	else
	{
	    WriteRunInfo::WriteRunLog("Receive invalid event %d", event);
	}
	pThread->GetListenBlock()->mutex.UnLock();

	return 0;
}

int CLongConnectedListenTask::PutMsgToSendList(const char *pBuf, int nLen)
{
	return 0;
}

int CLongConnectedListenTask::DestroySelf()
{
        delete this;
        return 0;
}

int CLongConnectedListenTask::AddToEpoll()
{
	int tmpEvent = MYPOLLIN | MYPOLLRDNORM;
	CNetProcessThread* pNetThread = (CNetProcessThread*)m_pThread;
	if ( pNetThread )
	{
		CEpoll *pEoll = pNetThread->GetEpoll();
		if ( pEoll )
		{
				pEoll->AddToEpoll(this, tmpEvent);
		}
	}
	return 0;
}
