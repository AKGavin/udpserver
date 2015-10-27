/****************************************************************************
**
**  Copyright  (c)  2008-2009,  Baofeng, Inc.  All Rights Reserved.
**
**  Subsystem:    ConnectServer
**  File:         	     ShortConnectedProcessThread.cpp
**  Created By:   fwx
**  Created On:   2009/01/08
**
**  Purpose:
**     ShortConnectedProcessThread
**
**  History:
**  Programmer      Date         Version        Number      Description
**  --------------- --------   ---------- ------------------------------
**    fwx       2009/01/08         1.0            01          creation
****************************************************************************/

#include "ShortConnectedProcessThread.h"

CShortConnectedProcessThread::CShortConnectedProcessThread( int iEntityType, int iEntityId )
	: CNetProcessThread( iEntityType, iEntityId )
{
}

CShortConnectedProcessThread::~CShortConnectedProcessThread()
{
}

int  CShortConnectedProcessThread::Process(CMsg *pMsg)
{
	TMsg *pTMsg = pMsg->GetTMsg();
	if ( !pTMsg )
	{
		return 0;
	}
        WriteRunInfo::WriteTrace("The ShortConnected thread recv a  msg , msgtype %d...", pTMsg->msgType);

	if ( pTMsg->srcEntityType == LONGCONNECTEDENTITY )
	{
		if (pTMsg->msgType == CLIENTMSGTYPE )
		{
			CClientMsgBody *pBody = ( CClientMsgBody * )pMsg->GetMsgBody();
			if ( !pBody )
			{
				return 0;
			}
            CSocketTask *pSocketTask = FindSocketTask(pTMsg->taskId);
            if (pSocketTask != NULL)
            {
                pSocketTask->PutMsgToSendList( pBody->GetBuffer(), pBody->GetLen());
                pBody->DisAttachBuf(); //把buffer传到sockettask中了
            }
            
//			std::list<CSocketTask*>::iterator it = m_pSocketList.begin();
//			while( it != m_pSocketList.end() )
//			{
//				CSocketTask * pSocketTask = ( *it );
//				if ( pSocketTask )
//				{
//					pSocketTask->PutMsgToSendList( pBody->GetBuffer(), pBody->GetLen() );
//				}
//				it ++;
//			}
		}
		else
		{
			WriteRunInfo::WriteLog("Send&recv  thread recv invalid msgtype %d", pTMsg->msgType);
		}
	}
	else if ( pTMsg->srcEntityType == LISTENENTITY )
	{
		 if (pTMsg->msgType == RECVMSGTYPE)
                {
                        CRecvMsgBody *pBody = (CRecvMsgBody*)pMsg->GetMsgBody();
                        CTcpSocket *pSocket = new CTcpSocket(pBody->GetSockAddr(), sizeof(sockaddr_in), pBody->GetSocket());
                        CSocketTask *tmpSocketTask = NULL;
                        tmpSocketTask = CMyserver::GetInstance()->GetClassFactory()->GenSocketTask(this,  
                                pSocket, RECVANDSENDTASK);               
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


