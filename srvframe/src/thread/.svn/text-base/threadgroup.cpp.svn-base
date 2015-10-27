/*
* FileName:       
* Author:         wangqifeng  Version: 1.0  Date: 2005-9-4
* Description:    
* Version:        
* Function List:  
*                 1.
* History:        
*     <author>   <time>    <version >   <desc>
*/
#include "threadgroup.h"
#include "myserver.h"
#include <iostream>

CThreadGroup::~CThreadGroup()
{
}

int CThreadGroup::PutMsgToThread(CMsg *pMsg)
{
	TMsg *pTMsg = pMsg->GetTMsg();
	if (pTMsg->destEntityType != m_iThreadEntityType) //先做简单的判断
	{
		//写日志，过滤此消息
		WriteRunInfo::WriteLog("Error entity type(%d) , %d", pTMsg->destEntityType, m_iThreadEntityType);

		////直接将此消息删除
		delete pMsg;
		return -1;
	}

	if (pTMsg->destEntityId == RANDOMENTITYID) //未指定entityid, 轮循
	{                
		m_rotationMutex.Lock();
		m_threadList[m_iRotationEntityId]->PutMsgToInputList(pMsg);               
		m_iRotationEntityId ++;
		m_iRotationEntityId = m_iRotationEntityId%m_iEntityIdSize;
		m_rotationMutex.UnLock();
	}
	else  //指定entityid
	{
		if (pTMsg->destEntityId < 0 || pTMsg->destEntityId >= m_iEntityIdSize) //实体ID范围不正确
		{
			//写日志，过滤此消息
			WriteRunInfo::WriteLog("Error entityid (%d) , %d", pTMsg->destEntityId, m_iEntityIdSize);
			//直接将此消息删除
			delete pMsg;
			return -1;
		}
		//否则就将消息挂入相应的线程中
		m_threadList[pTMsg->destEntityId]->PutMsgToInputList(pMsg);
	}
	return 0;        
}

int CThreadGroup::Init()
{    
	for (int i=0; i < m_iEntityIdSize; i++)
	{
		CWorkThread *pThread = CMyserver::GetInstance()->GetClassFactory()->GenWorkThread(m_iThreadEntityType, i, m_sAnnexData, 
			m_arg);
		if (pThread == NULL)
		{
			WriteRunInfo::WriteLog("Malloc memory error");
			return -1;
		}

		int ret = pThread->Start();
		if (ret != 0)
		{
			WriteRunInfo::WriteLog("Start thread fail");
			return -1;
		}
		WriteRunInfo::WriteLog("The threadid is %d, pid is %d", pThread->GetThreadId(), pThread->GetPid());
		m_threadList.push_back(pThread);
	} 
	//将这个线程组注册到服务器中
	CMyserver::GetInstance()->RegisterThreadGroup(this);
	return 0;
}



int CThreadGroup::GetRunFlag()
{
	int ret = 0;
	for (unsigned int i=0; i < m_threadList.size(); i++)
	{
		ret += m_threadList[i]->GetRunFlag();
	}
	return ret;
}

int CThreadGroup::StopAllThread()
{
	for (unsigned int i=0; i < m_threadList.size(); i++)
	{
		m_threadList[i]->Stop();
	}
	return 0;
}

