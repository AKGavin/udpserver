/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2005-8-26
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef __UDP_THREAD_H
#define __UDP_THREAD_H
#include <string>
#include "commonthread.h"
#include "framecommon/framecommon.h"
using namespace MYFRAMECOMMON;

using namespace std;

const int MAXRECVBUF=8096;


class CUdpThread:public CCommonThread
{
public:
        CUdpThread(string &sAddr, short iPort)
        {
                m_addr = sAddr;
                m_port = iPort;
                m_nRecvLen = 0;
                m_nAddrLen = 0;
        }
        ~CUdpThread()
        {        
                
        }
        int Init();

        int Run();

        virtual int ProcessMsg()=0;
protected:
        int m_sock;
        string m_addr;
        short m_port;
        char m_recvBuf[MAXRECVBUF];
        int m_nRecvLen;
        sockaddr_in m_fromAddr;
        socklen_t m_nAddrLen;
};
#endif
 
