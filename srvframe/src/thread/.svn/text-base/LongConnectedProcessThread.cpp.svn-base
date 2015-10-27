/****************************************************************************
**
**  Copyright  (c)  2008-2009,  Baofeng, Inc.  All Rights Reserved.
**
**  Subsystem:    ConnectServer
**  File:         	     LongConnectedProcessThread.cpp
**  Created By:   fwx
**  Created On:   2009/01/08
**
**  Purpose:
**     LongConnectedProcessThread
**
**  History:
**  Programmer      Date         Version        Number      Description
**  --------------- --------   ---------- ------------------------------
**    fwx       2009/01/08         1.0            01          creation
****************************************************************************/
#include "LongConnectedProcessThread.h"

CLongConnectedProcessThread::CLongConnectedProcessThread( int iEntityType, int iEntityId, int iListType/* = 0*/ )
	: CNetProcessThread( iEntityType, iEntityId )
	, m_iListType( iListType )
{

}

CLongConnectedProcessThread::~CLongConnectedProcessThread( void )
{
}

int CLongConnectedProcessThread::Process( CMsg * pMsg )
{
	TMsg *pTMsg = pMsg->GetTMsg();
	if ( !pTMsg )
	{
		return 0;
	}

	WRITE_TRACE("The recvsend thread recv a  msg , msgtype %d...", pTMsg->msgType);        

	if ( pTMsg->srcEntityType == LONGCONNECTEDLISTENENTITY ) //收到接收请求
	{
		if ( pTMsg->msgType == RECVANDSENDMSGTYPE )
		{
			CRecvMsgBody *pBody = (CRecvMsgBody*)pMsg->GetMsgBody();
			if ( !pBody )
			{
				return 0;
			}
			CTcpSocket *pSocket = new CTcpSocket(pBody->GetSockAddr(), sizeof(sockaddr_in), pBody->GetSocket());
			if ( !pSocket )
			{
				return 0;
			}
			CSocketTask *tmpSocketTask = NULL;
			CCommonTaskArg commonTaskArg( 0, 0, false, m_iListType );						
			tmpSocketTask = CMyserver::GetInstance()->GetClassFactory()->GenSocketTask( this,  
																pSocket, COMMONTASK, &commonTaskArg );   
			tmpSocketTask->Init();                     
			tmpSocketTask->SetTimeStat( pBody->GetTimeStat() );
		}
		else
		{
			WriteRunInfo::WriteLog("Send&recv  thread recv invalid msgtype %d", pTMsg->msgType);
		}
	}        
	else if ( pTMsg->msgType == CLIENTRETMSGTYPE )
	{
		CClientRetMsgBody *pBody = ( CClientRetMsgBody * )pMsg->GetMsgBody();
		if ( !pBody )
		{
			return 0;
		}

		if ( m_iListType )
		{
			std::list<CSocketTask*>::iterator it = m_pSocketList.begin();
			while( it != m_pSocketList.end() )
			{
				CSocketTask * pSocketTask = ( *it );
				if ( pSocketTask && pSocketTask->GetTaskId() == pBody->GetDestTaskId() )
				{
					pSocketTask->PutMsgToSendList( pBody->GetBuffer(), pBody->GetLen() );
				}
				it ++;
			}
		}
		else
		{
			std::map<ULONG64, CSocketTask*>::iterator it = m_pMapSocketList.find( pBody->GetDestTaskId() );
			if ( it != m_pMapSocketList.end() )
			{
				CSocketTask * pSocketTask = ( *it ).second;
				if ( pSocketTask )
				{				
					pSocketTask->PutMsgToSendList( pBody->GetBuffer(), pBody->GetLen() );
				}
			}
		}

	}
	else if ( pTMsg->srcEntityType == TIMERENTITY )
	{
		TimerMsgProcess(pMsg);
	}    
	else
	{
		CNetProcessThread::Process( pMsg);
	}
	return 0;
}
