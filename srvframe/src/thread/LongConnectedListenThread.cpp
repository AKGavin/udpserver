/****************************************************************************
**
**  Copyright  (c)  2008-2009,  Baofeng, Inc.  All Rights Reserved.
**
**  Subsystem:    ConnectServer
**  File:         	     LongConnectedListenThread.cpp
**  Created By:   fwx
**  Created On:   2009/01/08
**
**  Purpose:
**     LongConnectedListenThread
**
**  History:
**  Programmer      Date         Version        Number      Description
**  --------------- --------   ---------- ------------------------------
**    fwx       2009/01/08         1.0            01          creation
****************************************************************************/
#include "LongConnectedListenThread.h"

CLongConnectedListenThread::CLongConnectedListenThread( int iEntityType, int iEntityId, string sHost, int nPort, PLISTENBLOCK pListenBlock )
	: CNetProcessThread( iEntityType, iEntityId )
{
	m_sHost = sHost;
	m_nPort = nPort;
	m_pListenBlock = pListenBlock;
}

CLongConnectedListenThread::~CLongConnectedListenThread()
{

}

int CLongConnectedListenThread::Init()
{
        int retValue = 0;
        retValue = CNetProcessThread::Init();
        if (retValue != 0)
        {
                return -1;
        }
        m_pListenBlock->mutex.Lock();
        if (m_pListenBlock->nSock == -1)
        {
            //生成一个监听句柄
            int nSocket;                           
            retValue = CMySocket::CreateSock(SOCK_STREAM, m_sHost, m_nPort, nSocket);
            if (retValue < 0)
            {
                    WriteRunInfo::WriteLog("Create listen sock fail %d", retValue);
                    m_pListenBlock->mutex.UnLock();
                    return -1;
            }
            m_pListenBlock->nSock=nSocket;
        }
        m_pListenBlock->mutex.UnLock();
        
        m_pTcpSocket = new CTcpSocket(m_sHost, m_nPort, m_pListenBlock->nSock);
        if (m_pTcpSocket == NULL)
        {
                WriteRunInfo::WriteLog("New listen socket object fail");
                return -1;
        }
        
        //起监听端口                    
        m_pListenTask = (CLongConnectedListenTask*)CMyserver::GetInstance()->GetClassFactory()->GenSocketTask( this, m_pTcpSocket,  LONGCONNECTEDLISTENTASK );        
        m_pListenTask->Init();
        if (m_pListenTask == NULL)
        {
                WriteRunInfo::WriteLog("Malloc memory fail or listen fail");
                return -1;
        }                      
        return 0;
}
