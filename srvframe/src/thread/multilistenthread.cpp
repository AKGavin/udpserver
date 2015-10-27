#include "multilistenthread.h"

int CMultiListenThread::AddListen(const string &sHost, unsigned short nPort)
{
	int nSocket = -1;
	int retValue = CMySocket::CreateSock(SOCK_STREAM, sHost, nPort, nSocket);
	if (retValue < 0)
	{
		WriteRunInfo::WriteLog("Create listen sock(%s:%d) fail %d", sHost.c_str(), nPort, retValue);
		return retValue;
	}
	CTcpSocket *pTcpSocket = new CTcpSocket(sHost, nPort, nSocket);
       if (pTcpSocket == NULL)
       {
                WriteRunInfo::WriteLog("New listen socket object fail");
		   close(nSocket);
                return -1;
       }
        
        //Æð¼àÌý¶Ë¿Ú                    
        CListenTask *pListenTask = (CListenTask*)CMyserver::GetInstance()->GetClassFactory()->GenSocketTask(this, pTcpSocket,  LISTENTASK);        
	 if (pListenTask == NULL)
        {
                WriteRunInfo::WriteLog("Malloc memory fail or listen fail");
                return -1;
        }                 
        return pListenTask->Init();            
}
