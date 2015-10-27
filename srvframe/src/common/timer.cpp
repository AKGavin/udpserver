
#include "timer.h"
#include "task.h"
#include "myserver.h"
#include "commmsg.h"

ULONG64 CTimer::m_iTimerId=0;
CThreadMutex CTimer::m_sTimerIdMutex;


CTimer::CTimer()
{        
        m_nCurTimePoint = 0;
        m_nPreTimePoint = 0;
        for (int i=0; i < TIMERCYCLE; i++)
        {
                map<ULONG64, TTimerStr>  tmpList;
                m_timerList.push_back(tmpList);
        }
}

CTimer::~CTimer()
{
}


void CTimer::PrintTimer()
{
        for (int i=0; i < TIMERCYCLE; i++)
        {
                WriteRunInfo::WriteInfo("The timepoint %d has %d timer event\n", i, m_timerList[i].size());        
        }
}

int CTimer::AddOneTimer(TTimerStr &timer)
{      
        if (timer.event.m_nTimerPoint < TIMERCYCLE)
        {
                m_timerList[timer.event.m_nTimerPoint].insert(make_pair(timer.event.m_iTimerId,  timer));
        }
        return 0;
}

int CTimer::DeleteOneTimer(int iTimePoint,  ULONG64  iTimerId)
{                  
        if (iTimePoint < TIMERCYCLE)
        {
                m_timerList[iTimePoint].erase(iTimerId);
        }
        return 0;
}

int CTimer::TimerCheck()
{
        int tmpProcessNum = 0;
        map<ULONG64, TTimerStr>::iterator mapItr;
		
		if ( m_nCurTimePoint >= m_nPreTimePoint )
		{
			for ( int iTimePoint = m_nPreTimePoint ; iTimePoint <= m_nCurTimePoint ; iTimePoint ++ )
			{
				mapItr = m_timerList[iTimePoint].begin();
				for (; mapItr != m_timerList[iTimePoint].end(); )
				{
					map<ULONG64, TTimerStr>::iterator tmpItr = mapItr;
					mapItr++;
					//发送超时消息
					SendTimerOutMsg(tmpItr->second);
					//删除这个节点
					m_timerList[iTimePoint].erase(tmpItr);
					tmpProcessNum ++;
				}
			}			
			m_nPreTimePoint = m_nCurTimePoint;
		}
		else
		{
			for ( int iTimePoint = m_nPreTimePoint ; iTimePoint <= TIMERCYCLE - 1 ; iTimePoint ++ )
			{
				mapItr = m_timerList[iTimePoint].begin();
				for (; mapItr != m_timerList[iTimePoint].end(); )
				{
					map<ULONG64, TTimerStr>::iterator tmpItr = mapItr;
					mapItr++;
					//发送超时消息
					SendTimerOutMsg(tmpItr->second);
					//删除这个节点
					m_timerList[iTimePoint].erase(tmpItr);
					tmpProcessNum ++;
				}
			}
			for ( int iTimePoint = 0 ; iTimePoint <= m_nCurTimePoint ; iTimePoint ++ )
			{
				mapItr = m_timerList[iTimePoint].begin();
				for (; mapItr != m_timerList[iTimePoint].end(); )
				{
					map<ULONG64, TTimerStr>::iterator tmpItr = mapItr;
					mapItr++;
					//发送超时消息
					SendTimerOutMsg(tmpItr->second);
					//删除这个节点
					m_timerList[iTimePoint].erase(tmpItr);
					tmpProcessNum ++;
				}
			}
			m_nPreTimePoint = m_nCurTimePoint;
		}
        return tmpProcessNum;
}

int CTimer::SendTimerOutMsg(TTimerStr &timerStr)
{
        TMsg tMsg;
        tMsg.srcEntityId = 0;
        tMsg.srcEntityType = TIMERENTITY;
        tMsg.destEntityId = timerStr.iEntityId;
        tMsg.destEntityType = timerStr.iEntityType;
        tMsg.msgType = TIMERREACHTYPE;
        tMsg.taskId = timerStr.iTaskId;
        char *tmpBuf = new char[sizeof(TTimeEvent)];
        memcpy(tmpBuf, &(timerStr.event), sizeof(TTimeEvent));
        CBufMsgBody *pBufBody = new CBufMsgBody(tmpBuf, sizeof(TTimeEvent));
        CMsg *pMsg = new CMsg(tMsg, pBufBody);

        CMyserver::GetInstance()->PutMsgToThread(pMsg);
        return 0;
}

ULONG64 CTimer::GentTimerId()
{
        ULONG64 ret = 0;
        m_sTimerIdMutex.Lock();
        ret = ++m_iTimerId;
        m_sTimerIdMutex.UnLock();
        return ret;
}


