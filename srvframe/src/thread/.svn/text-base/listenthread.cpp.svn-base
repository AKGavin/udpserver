/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2006-9-6
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#include "listenthread.h"
#include "myserver.h"
CListenThread::CListenThread(int iEntityType, int iEntityId, string sHost, int nPort, TListenBlock *pListenBlock)
	:CNetProcessThread(iEntityType, iEntityId)
{
        m_sHost = sHost;
        m_nPort = nPort;
        m_pListenBlock = pListenBlock;
}

CListenThread::~CListenThread()
{
}


int CListenThread::Init()
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
        m_pListenTask = (CListenTask*)CMyserver::GetInstance()->GetClassFactory()->GenSocketTask(this, m_pTcpSocket,  LISTENTASK);        
        m_pListenTask->Init();
        if (m_pListenTask == NULL)
        {
                WriteRunInfo::WriteLog("Malloc memory fail or listen fail");
                return -1;
        }                      
        return 0;
}

