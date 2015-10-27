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
#include "workthread.h"
#include "myserver.h"
#include <iostream>

int CWorkThread::Init()
{
        return 0;
}

CTask *CWorkThread::FindTask(ULONG64 iTaskId)
{
        CTask *pTmpTask = NULL;
        map<ULONG64, CTask*>::iterator itr = m_taskList.find(iTaskId);
        if (itr != m_taskList.end())
        {
                pTmpTask = itr->second;
        }
        return pTmpTask;        
}

int CWorkThread::DelTask(ULONG64 taskId)
{
        m_taskList.erase(taskId);
        return 0;
}

int CWorkThread::AddTask(CTask *pTask)
{
        m_taskList.insert(make_pair(pTask->GetTaskId(), pTask));
        return 0;
}

int CWorkThread::TimerMsgProcess(CMsg *pMsg)
{
        TMsg *pTMsg = pMsg->GetTMsg();
        CTask *pTask = FindTask(pTMsg->taskId);
        if (pTask != NULL)
        {
                CBufMsgBody *pBufMsg = (CBufMsgBody*)pMsg->GetMsgBody();
                TTimeEvent *pTimerEv = (TTimeEvent*)pBufMsg->GetBuf();
                pTask->OnTimer(*pTimerEv);
        }
        else
        {
                WriteRunInfo::WriteLog("The task(%llu) is not exist", pTMsg->taskId);
        }
        return 0;
}

void CWorkThread::SendMsg(int destEntityType, int destEntityId, ULONG64 lTaskId, int msgType,  CMsgBody *pMsgBody, ULONG64 lSrcTaskId)
{
//	std::cout << "CWorkThread::SendMsg, destEntityType=" << destEntityType << std::endl;
        TMsg tMsg;
        tMsg.destEntityType = destEntityType;
        tMsg.destEntityId = destEntityId;
        tMsg.srcEntityId = GetEntityId();
        tMsg.srcEntityType= GetEntityType();
        tMsg.srcTaskId = lSrcTaskId;
        tMsg.taskId = lTaskId;
        tMsg.msgType = msgType;
        CMsg *pMsg = new CMsg(tMsg, pMsgBody);
        CMyserver::GetInstance()->PutMsgToThread(pMsg);
}

