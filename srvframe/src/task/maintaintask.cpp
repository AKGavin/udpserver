/*
* FileName:       
* Author:         wangqifeng  Version: 1.0  Date: 2006-6-17
* Description:    
* Version:        
* Function List:  
*                 1.
* History:        
*     <author>   <time>    <version >   <desc>
*/
#include "maintaintask.h"
#include "myserver.h"

CMaintainTask::CMaintainTask(ULONG64   taskId,  CWorkThread *pThread):CTask(taskId, pThread)
{
	AddOneTimer(STATUPDATETIMEREVENT, STATUPDATETIMERLENGTH);
}

int CMaintainTask::Process(CMsg *pMsg)
{
	return 0;
}

int CMaintainTask::OnTimer(TTimeEvent &timeEvent)
{
	CTask::OnTimer(timeEvent);
	switch(timeEvent.m_nEvent)
	{
	case STATUPDATETIMEREVENT:
		{
			WriteRunInfo::WriteLog("Begin print stat info ...");
			WriteRunInfo::WriteStat("Now task num is %d", m_nSTaskNum);
			CMyserver::GetInstance()->GetCommonStat().Update();                         
			AddOneTimer(STATUPDATETIMEREVENT, STATUPDATETIMERLENGTH);
			break;
		}                
	default:
		{
			WriteRunInfo::WriteLog("maintain task recv invalid timer event %d", timeEvent.m_nEvent);
			break;
		}
	}
	return 0;          
}


int CMaintainTask::DestroySelf()
{
	delete this;
	return 0;
}

