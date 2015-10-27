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
#include <netinet/in.h> 
#include "maintainthread.h"
#include "threadgroup.h"
#include "myserver.h"

int CMaintainThread::Init()
{         
        ULONG64 lTaskId = CTask::GentTaskId();
        CMaintainTask *pMaintainTask = NULL;
        pMaintainTask = new CMaintainTask(lTaskId, this);
        return 0;        
}



int  CMaintainThread::Process(CMsg *pMsg)
{
        TMsg *pTMsg = pMsg->GetTMsg();
//        CBufMsgBody *pBufMsgBody = (CBufMsgBody*)pMsg->GetMsgBody();

        if (pTMsg->srcEntityType == TIMERENTITY) //超时消息
        {
                TimerMsgProcess(pMsg);
                return 0;
        }        
        return 0;
}

int CMaintainThread::Run()
{
        while (m_nRunFlag)
        {
                //处理消息
                CMsg *pMsg = m_inputList.get();
                if (pMsg != NULL)
                {
                    Process(pMsg);
                    delete pMsg;
                }
                usleep(50000);
        }
        return 0;
}


int CMaintainThread::PutMsgToInputList(CMsg * pMsg)
{
        m_inputList.put(pMsg);
        return 0;
}

