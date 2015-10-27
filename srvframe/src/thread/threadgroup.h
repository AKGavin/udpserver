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
#ifndef  __THREAD_GROUP_H
#define  __THREAD_GROUP_H
#include <iostream>
#include "commmsg.h"
#include "workthread.h"
#include "framecommon/framecommon.h"
using namespace MYFRAMECOMMON;

const int APPOINTENTITYIDTYPE=0; //指定entity ID
const int ROTATIONTYPE=1;     //各个线程无差别， 轮循模式
const int RANDOMENTITYID=-1;

/**
* dec:
* ClassName:CThreadGroup
* author: 
* version:
*CreateTime: 2005.9.4
*/
class CThreadGroup
{
public:
	CThreadGroup(int iThreadGroupSize, 
		int iThreadEntityType, string sAnnexData="", void *arg=NULL)
	{
		m_iEntityIdSize = iThreadGroupSize;
		m_iRotationEntityId = 0;
		m_iThreadEntityType = iThreadEntityType;
		m_sAnnexData = sAnnexData;
		m_arg = arg;
	}
	virtual ~CThreadGroup();

	int Init();
	int ThreadOutMsgProcess();
	int PutMsgToThread(CMsg *pMsg);        


	void SetThreadEntityType(int iThreadEntityType)
	{
		m_iThreadEntityType = iThreadEntityType;
	}

	int GetThreadEntityType()
	{
		return m_iThreadEntityType;
	}

	vector<CWorkThread*> *GetThreadList()
	{
		return &m_threadList;
	}

	int StopAllThread();
	int GetRunFlag();        
protected:
	vector<CWorkThread *>  m_threadList; //线程的下标就是线程ID
	int m_iThreadEntityType; //线程实体类型
	int m_iEntityIdSize;
	string m_sAnnexData;
	void *m_arg;
	CThreadMutex m_rotationMutex;
	int m_iRotationEntityId; //轮循到的ID
};


#endif

