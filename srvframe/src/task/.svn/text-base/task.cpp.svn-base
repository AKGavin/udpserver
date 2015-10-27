/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2005-8-31
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
*/
#include "task.h"
#include "workthread.h"
#include "myserver.h"

ULONG64  CTask::m_lCurTaskId=0;
CThreadMutex CTask::m_taskIdMutex;
int CTask::m_nSTaskNum = 0;
CThreadMutex CTask::m_nTaskNumMutex;

CTask::CTask(ULONG64  taskId, CWorkThread * pThread)
{ 
        m_iTaskId = taskId;   
        m_pThread = pThread;
        m_iTaskType = 0;
        m_status = INITSTATUS;
        m_pThread->AddTask(this);
        UpdateTaskNum(1);
        m_nStartTime = CMyserver::GetInstance()->GetTimeVal().tv_sec;
        m_nTaskTimeOut = -1;
}

CTask::~CTask()
{
        WRITE_TRACE("Delete a  task %llu", m_iTaskId);
        //将任务从线程任务列表中删除
        m_pThread->DelTask(m_iTaskId);     
        //删除所有定时器             
        DeleteAllTimer();
        UpdateTaskNum(-1);
}

int CTask::OnTimer(TTimeEvent &timeEvent)
{
        DirectDeleteOneTimer(timeEvent.m_iTimerId);
        return 0;
}

void CTask::UpdateTaskNum(int nNum)
{
        m_nTaskNumMutex.Lock();
        m_nSTaskNum += nNum;
        m_nTaskNumMutex.UnLock();
}

int CTask::CheckTimeOut()
{        
        if (m_nTaskTimeOut == -1)
        {
                return 0;
        }
        
        int ret = 0;
        if ((CMyserver::GetInstance()->GetTimeVal().tv_sec - m_nStartTime) > m_nTaskTimeOut)
        {
                DestroySelf();
                ret = 1;
        }
        return ret;
}

int CTask::DeleteAllTimer()
{
        map<ULONG64, TTimeEvent>::iterator mapItr=m_timerList.begin();
        for (; mapItr != m_timerList.end(); )
        {
                map<ULONG64, TTimeEvent>::iterator tmpItr = mapItr;
                mapItr++;                           
                SendTimerOutMsg(TIMERDELTYPE, tmpItr->second);
                m_timerList.erase(tmpItr);        
        }
        return 0;
}
        
int CTask::AddOneTimer(int event, int timeLength)
{
        TTimeEvent tmpEvent;

        ULONG64 iTimerId = CTimer::GentTimerId();

        tmpEvent.m_iTimerId = iTimerId;
        tmpEvent.m_nEvent = event;
        tmpEvent.m_nTimerPoint = (CMyserver::GetInstance()->GetTimerPoint() + timeLength)%TIMERCYCLE;        
        
        m_timerList.insert(make_pair(iTimerId, tmpEvent));

        //需要再考虑下
        SendTimerOutMsg(TIMERADDTYPE, tmpEvent);
        return 0;
}
        
        
int CTask::DeleteOneTimer(ULONG64 iTimerId)
{       
        //删除自身的定时器
        map<ULONG64, TTimeEvent>::iterator mapItr = m_timerList.find(iTimerId);
        if (mapItr != m_timerList.end())
        {
                 //删除定时器    
                SendTimerOutMsg(TIMERDELTYPE, mapItr->second);
                m_timerList.erase(mapItr);               
        }
        return 0;
}

int CTask::DirectDeleteOneTimer(ULONG64 iTimerId)
{
        //删除自身的定时器
        map<ULONG64, TTimeEvent>::iterator mapItr = m_timerList.find(iTimerId);
        if (mapItr != m_timerList.end())
        {
                 //删除定时器    
                m_timerList.erase(mapItr);               
        }
        return 0;
}


int CTask::SendTimerOutMsg(int msgType,  TTimeEvent &timerEvent)
{
        TMsg tMsg;
        tMsg.srcEntityId = m_pThread->GetEntityId();
        tMsg.srcEntityType = m_pThread->GetEntityType();
        tMsg.destEntityId = 0;
        tMsg.destEntityType = TIMERENTITY;
        tMsg.msgType = msgType;
        tMsg.taskId = m_iTaskId; 
        char *tmpBuf = new char[sizeof(TTimeEvent)];
        memcpy(tmpBuf, &timerEvent, sizeof(TTimeEvent));        
        CBufMsgBody *pBufBody=  new CBufMsgBody(tmpBuf, sizeof(TTimeEvent));        
        CMsg *pMsg = new CMsg(tMsg, pBufBody);        
        CMyserver::GetInstance()->PutMsgToThread(pMsg);
        return 0;
}

ULONG64 CTask::GentTaskId()
{        
        ULONG64        ret = 0;
        m_taskIdMutex.Lock();
        m_lCurTaskId++;
        ret = m_lCurTaskId;
        m_taskIdMutex.UnLock();
        return ret;
}

