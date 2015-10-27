/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2006-1-10
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#include "timerthread.h"
#include "myserver.h"

int CTimerThread::Run()
{
        while (m_nRunFlag)
        {
                gettimeofday(&m_timeval, NULL);
                m_iCurrentTimePoint = m_timeval.tv_sec%TIMERCYCLE;
                m_timer.SetCurTimePoint(m_timeval.tv_sec);
                CMyserver::GetInstance()->SetTimerPoint(m_iCurrentTimePoint, m_timeval);
                m_timer.TimerCheck();
                CMsg *pMsg = m_inputList.get();
                if (pMsg != NULL)
                {
                        Process(pMsg);
                        delete pMsg;
                }
                else
                {
                        //定时线程实时性要求不高, 可以睡眠10ms
                        usleep(10000);
                }
        }
        return 0;
}

int CTimerThread::PutMsgToInputList(CMsg * pMsg)
{
        m_inputList.put(pMsg);
        return 0;
}
        

int CTimerThread::Process(CMsg *pMsg)
{
        TMsg *pTMsg = pMsg->GetTMsg();
        switch (pTMsg->msgType)
        {
                case TIMERADDTYPE: //增加接口
                {                              
                        CBufMsgBody *pBufMsg = (CBufMsgBody*)pMsg->GetMsgBody();
                        TTimeEvent *pTEvent = (TTimeEvent*)pBufMsg->GetBuf();

                        TTimerStr tmpTimerStr;
                        tmpTimerStr.iEntityId = pTMsg->srcEntityId;
                        tmpTimerStr.iEntityType = pTMsg->srcEntityType;
                        tmpTimerStr.iTaskId = pTMsg->taskId;
                        memcpy(&(tmpTimerStr.event), pTEvent, sizeof(TTimeEvent));                
                        m_timer.AddOneTimer(tmpTimerStr);
                        break;
                }
                case TIMERDELTYPE: //删除接口
                {
                        CBufMsgBody *pBufMsg = (CBufMsgBody*)pMsg->GetMsgBody();
                        TTimeEvent *pTEvent = (TTimeEvent*)pBufMsg->GetBuf();
                        m_timer.DeleteOneTimer(pTEvent->m_nTimerPoint, pTEvent->m_iTimerId);// int iTimePoint, int iTimerId);
                        break;
                }
                case PRINTTIMER:
                {
                        m_timer.PrintTimer();
                        break;
                }
                default:
                {
                        WriteRunInfo::WriteLog("Timer thread , recv invaid msg %d", pTMsg->msgType);
                        break;
                }
        }
        return 0;
}


