/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2007-7-31
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef __NORMAL_SERVER_H
#define __NORMAL_SERVER_H
#include "myserver.h"
#include "threadgroup.h"
#include "maintainudpthread.h"
/**
* dec:
* ClassName:CNormalServer
* author: 
* version:
*CreateTime: 2007.7.31
*/
class CNormalServer:public CMyserver
{
public:
        CNormalServer();
        ~CNormalServer();
        int PreStartServer();
protected:
        CThreadGroup *m_pListenThreadGrp;
        CThreadGroup *m_pRecvAndSendThreadGrp;
        CThreadGroup *m_pMaintainThreadGrp;
        CThreadGroup *m_pTimerThreadGrp;        
        CMaintainUdpThread *m_pMaintainUpdThread;
};
#endif

