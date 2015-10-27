/****************************************************************************
**
**  Copyright  (c)  2008-2009,  Baofeng, Inc.  All Rights Reserved.
**
**  Subsystem:    ConnectServer
**  File:         	     LongConnectedThread.cpp
**  Created By:   fwx
**  Created On:   2009/01/08
**
**  Purpose:
**     ForwardTaskThread
**
**  History:
**  Programmer      Date         Version        Number      Description
**  --------------- --------   ---------- ------------------------------
**    fwx       2009/01/08         1.0            01          creation
****************************************************************************/
#include <string>

#include "commmsg.h"
#include "LongConnectedThread.h"


CLongConnectedThread::CLongConnectedThread( int iEntityType, int iEntityId, int iListType/* = 0*/ )
	: CNetProcessThread(iEntityType, iEntityId)
	, m_pEpollReactor( NULL )
	, m_iListType( iListType )
{

}
        
CLongConnectedThread::~CLongConnectedThread( void )
{
	Stop();
	if ( m_pEpollReactor )
	{
		delete m_pEpollReactor;
		m_pEpollReactor = NULL;
	}
}

int CLongConnectedThread::Init( void )
{
	m_pEpollReactor = new CEpollReactor;
	if ( !m_pEpollReactor )
	{
		return -1;
	}
	if ( !m_pEpollReactor->StartService() )
	{
		return -1;
	}
	return CNetProcessThread::Init();
}

int  CLongConnectedThread::Process(CMsg *pMsg)
{
        TMsg *pTMsg = pMsg->GetTMsg();
	if ( !pTMsg )
	{
		return 0;
	}
        WRITE_TRACE("The LongConnected thread recv a  msg , msgtype %d...", pTMsg->msgType);        

	if ( pTMsg->srcEntityType == EPOLLREACTORENTITY )
	{
                if (pTMsg->msgType == CONNECTMSGTYPE)
                {
                        CConnectMsgBody *pBody = (CConnectMsgBody*)pMsg->GetMsgBody();
			   if ( !pBody )
			   {
			   	return 0;
			   }
			   if ( pBody->GetState() == 0 )
			   {
   			   	   std::string tmpStr = pBody->GetRemoteIp();
	                        CTcpSocket *pSocket = new CTcpSocket( tmpStr, pBody->GetRemotePort(), pBody->GetSocket() );
				   if ( !pSocket )
				   {
				   	return 0;
				   }
	                        CSocketTask *tmpSocketTask = NULL;
				   CCommonTaskArg commonTaskArg( pBody->GetDestMachineType(), pBody->GetDestMachineIndex(), true, m_iListType );
	                        tmpSocketTask = CMyserver::GetInstance()->GetClassFactory()->GenSocketTask(this,  
	                                pSocket, COMMONTASK, &commonTaskArg );
	                        tmpSocketTask->Init();
				   WriteRunInfo::WriteLog( "connect to %d %d %d succ ", ( int )pBody->GetDestMachineType(), ( int )pBody->GetDestMachineIndex(), pBody->GetSocket() );
			   }
			   else
			   {
				   WriteRunInfo::WriteLog( "EPOLLREACTORENTITY reconnect %d %d", ( int )pBody->GetDestMachineType(), ( int )pBody->GetDestMachineIndex() );
				   if ( m_pEpollReactor )
				   {
				   	m_pEpollReactor->AsyncConnect( pBody->GetRemoteIp(), pBody->GetRemotePort(), LONGCONNECTEDENTITY, 0, GetTime( 3 ), pBody->GetDestMachineType(), pBody->GetDestMachineIndex() );
				   }
			   }
                }
                else
                {
                        WriteRunInfo::WriteLog("Send&recv  thread recv invalid msgtype %d", pTMsg->msgType);
                }
	}
	else if ( pTMsg->srcEntityType== MAINENTITY )
        {
		if (pTMsg->msgType == TOCONNECTMSGTYPE)
		{
			CToConnectMsgBody *pBody = (CToConnectMsgBody*)pMsg->GetMsgBody();
			if ( !pBody )
			{
				return 0;
			}						
			if ( m_pEpollReactor )
			{
				WriteRunInfo::WriteLog("connect  %d %d", pBody->GetDestMachineType(), pBody->GetDestMachineIndex());
				m_pEpollReactor->AsyncConnect( pBody->GetRemoteIp(), pBody->GetRemotePort(), LONGCONNECTEDENTITY, 
										  0, GetTime( 3 ), pBody->GetDestMachineType(), pBody->GetDestMachineIndex() );
			}
		}
		else if ( pTMsg->msgType == CLIENTMSGTYPE )
		{
			CClientMsgBody *pBody = ( CClientMsgBody * )pMsg->GetMsgBody();
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
					if ( pSocketTask )
					{
						pSocketTask->PutMsgToSendList( pBody->GetBuffer(), pBody->GetLen() );
					}
					it ++;
				}
			}
			else
			{
				std::map<ULONG64, CSocketTask*>::iterator it = m_pMapSocketList.begin();
				while( it != m_pMapSocketList.end() )
				{
					CSocketTask * pSocketTask = ( *it ).second;
					if ( pSocketTask )
					{				
						pSocketTask->PutMsgToSendList( pBody->GetBuffer(), pBody->GetLen() );
					}
					it ++;
				}
			}
			
		}
		else
		{
		        WriteRunInfo::WriteLog("Send msg thread recv invalid msgtype %d", pTMsg->msgType);
		}
        }
	else if ( pTMsg->srcEntityType== LONGCONNECTEDENTITY )
	{
		  if ( pTMsg->msgType == TOCONNECTMSGTYPE )
                {
                        CToConnectMsgBody *pBody = (CToConnectMsgBody*)pMsg->GetMsgBody();
			   if ( !pBody )
			   {
			   	return 0;
			   }						
			   if ( m_pEpollReactor )
			   {
  				WriteRunInfo::WriteLog("LONGCONNECTEDENTITY  reconnect  %d %d", pBody->GetDestMachineType(), pBody->GetDestMachineIndex());
			   	m_pEpollReactor->AsyncConnect( pBody->GetRemoteIp(), pBody->GetRemotePort(), LONGCONNECTEDENTITY, 
											  0, GetTime( 3 ), pBody->GetDestMachineType(), pBody->GetDestMachineIndex() );
			   }
                }
                else
                {
                        WriteRunInfo::WriteLog("Send msg thread recv invalid msgtype %d", pTMsg->msgType);
                }
	}
	else if ( pTMsg->srcEntityType== RECVANDSENDMSGENTITY )
	{
		if ( pTMsg->msgType == CLIENTMSGTYPE )
		{
			CClientMsgBody *pBody = ( CClientMsgBody * )pMsg->GetMsgBody();
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
					if ( pSocketTask )
					{
						pSocketTask->PutMsgToSendList( pBody->GetBuffer(), pBody->GetLen() );
					}
					it ++;
				}
			}
			else
			{
				map<ULONG64, CSocketTask*>::iterator it = m_pMapSocketList.begin();
				while( it != m_pMapSocketList.end() )
				{
					CSocketTask * pSocketTask = ( *it ).second;
					if ( pSocketTask )
					{
						pSocketTask->PutMsgToSendList( pBody->GetBuffer(), pBody->GetLen() );
					}
					it ++;
				}
			}
			
		}
	}
        else
        {
                CNetProcessThread::Process( pMsg);
        }
	 return 0;
}

