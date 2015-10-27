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
#ifndef __LISTEN_THREAD_H
#define __LISTEN_THREAD_H
#include "netprocessthread.h"
#include "listentask.h"
/**
* dec:
* ClassName:CListenThread
* author: 
* version:
*CreateTime: 2006.9.6
*/
class CListenThread:public CNetProcessThread
{
public:
        CListenThread(int iEntityType, int iEntityId, string sHost, int nPort, TListenBlock *pListenBlock);
        ~CListenThread();
        void SetListenSocket(CTcpSocket *pSocket)
        {
                m_pTcpSocket  = pSocket;
        }
        int Init();
        TListenBlock *GetListenBlock()
        {
            return m_pListenBlock;
        }
protected:
        string m_sHost;
        int m_nPort;
        CTcpSocket *m_pTcpSocket;
        CListenTask *m_pListenTask;        
        TListenBlock *m_pListenBlock;
};

#endif
