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
#ifndef __TIME_THREAD_H
#define __TIME_THREAD_H
#include "workthread.h"
#include "asynqueue.h"

/**
* dec:
* ClassName:CTimerThread
* author: 
* version:
*CreateTime: 2006.1.10
*/
class CTimerThread:public CWorkThread
{
public:
	CTimerThread(int iEntityType, int iEntityId):CWorkThread(iEntityType, iEntityId)
	{
		m_iCurrentTimePoint = 0;
	}

	~CTimerThread()
	{

	}
	int Run();
	int PutMsgToInputList(CMsg * pMsg);
protected:
	int Process(CMsg *pMsg);
	CAsynQueue  m_inputList;
	CTimer m_timer;
	timeval m_timeval;
	int m_iCurrentTimePoint;
};

#endif
