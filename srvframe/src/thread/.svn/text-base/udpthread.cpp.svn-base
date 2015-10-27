/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2005-12-9
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#include "udpthread.h"
#include "sockettask.h"
#include "commmsg.h"
#include "myserver.h"

int CUdpThread::Init()
{
        int ret = CMySocket::CreateSock(SOCK_DGRAM, m_addr,  m_port, m_sock);
        if (ret == -1)
        {
               WriteRunInfo::WriteLog("Udpthread Init fail ......");
               return -1;
        }
        return 0;
}

int CUdpThread::Run()
{
        while (m_nRunFlag)
        {                
                m_nAddrLen =sizeof(sockaddr_in); // = 0;                
                memset(&m_fromAddr, 0,  sizeof(m_fromAddr));                                
                m_nRecvLen = 0;
                m_nRecvLen = recvfrom(m_sock,   m_recvBuf, MAXRECVBUF, 0, (sockaddr*)&m_fromAddr, &m_nAddrLen); // ×èÈû¶Ë¿Ú                               
                //add server logic
                ProcessMsg();
        }
        return 0;
}
