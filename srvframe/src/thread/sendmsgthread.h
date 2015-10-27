#ifndef __SEND_MSG_THREAD_H
#define __SEND_MSG_THREAD_H
#include "netprocessthread.h"
#include "framecommon/framecommon.h"
using namespace std;

class CRecvAndSendMsgThread:public CNetProcessThread
{
public:
        CRecvAndSendMsgThread(int iEntityType, int iEntityId);
        
        ~CRecvAndSendMsgThread();
        

        int  Process(CMsg *pMsg);              

 protected:
 };
#endif
