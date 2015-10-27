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
#include "myserver.h"
#include "threadgroup.h"
#include "workthread.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

void sighandler(int signum);
CMyserver *CMyserver::m_pMyServer=NULL;

CMyserver::CMyserver()
{                
	m_iCurrentTimerPoint = 0;
	gettimeofday(&m_timeval, NULL);
	m_iCurrentTimerPoint=m_timeval.tv_sec%TIMERCYCLE;
	m_nRunFlag = 1; //可以运行
	for (int i=0; i < MAXENTITYNUM; i++)
	{
		m_threadGroupList[i] = NULL;             
	}
}

int CMyserver::Init()
{
	//        m_pMyServer = new CMyserver();
	//        int ret = m_pMyServer->InitServer();
	return 0;
}

int CMyserver::InitServer()
{
	signal( SIGIO, SIG_IGN);
	signal( SIGPIPE, SIG_IGN);
	signal( SIGTERM, sighandler);
	signal( SIGHUP,sighandler);
	signal( SIGCHLD, sighandler);
	signal( SIGUSR1, sighandler);
	signal( SIGINT, sighandler); 

	int ret =  m_baseConf.Init();
	if (ret != 0)
	{
		WriteRunInfo::WriteLog("Load base conf fail ...");
		return -1;
	}

	//默认增加socketerror的统计类型
	TCommandStr cmdStr;
	cmdStr.nType = RECVPKGNUMSTAT;
	//strcpy(cmdStr.sCmdName, "recvpkgnum");
	strncpy(cmdStr.sCmdName, "recvpkgnum", sizeof(cmdStr.sCmdName));
	m_commonstat.AddNormalStatItem(cmdStr);

	cmdStr.nType = ADDEPOLL;
	//strcpy(cmdStr.sCmdName, "addepoll");
	strncpy(cmdStr.sCmdName, "addepoll", sizeof(cmdStr.sCmdName));
	m_commonstat.AddNormalStatItem(cmdStr);

	cmdStr.nType = DELEPOLL;
	//strcpy(cmdStr.sCmdName, "delepoll");
	strncpy(cmdStr.sCmdName, "delepoll", sizeof(cmdStr.sCmdName));
	m_commonstat.AddNormalStatItem(cmdStr);

	cmdStr.nType = MODEPOLL;
	//strcpy(cmdStr.sCmdName, "modepoll");
	strncpy(cmdStr.sCmdName, "modepoll" , sizeof(cmdStr.sCmdName));
	m_commonstat.AddNormalStatItem(cmdStr);

	cmdStr.nType = SOCKETTIMEOUTSTAT;
	//strcpy(cmdStr.sCmdName, "socketimeout");
	strncpy(cmdStr.sCmdName, "socketimeout", sizeof(cmdStr.sCmdName));
	m_commonstat.AddNormalStatItem(cmdStr);
	return 0;
}


int CMyserver::RegisterThreadGroup(CThreadGroup *pThreadGroup)
{
	//m_threadGroupList.insert(make_pair(pThreadGroup->GetThreadEntityType(), pThreadGroup));
	if (pThreadGroup->GetThreadEntityType() >= MAXENTITYNUM ||pThreadGroup->GetThreadEntityType() < 0)
	{
		return -1;
	}
	m_threadGroupList[pThreadGroup->GetThreadEntityType()] = pThreadGroup;
	return 0;
}

void CMyserver::PutMsgToThread(CMsg *pMsg)
{
	TMsg *pTMsg = pMsg->GetTMsg();
	//map<int, CThreadGroup*>::iterator itr = m_threadGroupList.find(pTMsg->destEntityType);
	if (pTMsg->destEntityType < 0 || pTMsg->destEntityType >= MAXENTITYNUM)
	{
		WriteRunInfo::WriteLog("Error entity type %d, delete the msg", pTMsg->destEntityType);
		delete pMsg;
		return;
	}
	if (m_threadGroupList[pTMsg->destEntityType] == NULL)
	{
		WriteRunInfo::WriteLog("Error entity type %d, CThreadGroup is null , delete the msg", pTMsg->destEntityType);
		delete pMsg;
		return;
	}
	m_threadGroupList[pTMsg->destEntityType] ->PutMsgToThread(pMsg);

	//        if (itr == m_threadGroupList.end())
	//        {
	//                WriteRunInfo::WriteLog("Error entity type %d, delete the msg", pTMsg->destEntityType);
	//                delete pMsg;
	//                return ;
	//        }
	//        (itr->second)->PutMsgToThread(pMsg);
	return ;
}

int CMyserver::SetTimerPoint(int iCurrentPoint, timeval &tmpTime)
{       
	m_iCurrentTimerPoint = iCurrentPoint;
	memcpy(&m_timeval, &tmpTime, sizeof(timeval));
	return 0;
}


int CMyserver::StartServer()
{
	return 0;
}

int CMyserver::SetServer(CMyserver *pMyserver)
{
	m_pMyServer = pMyserver;
	return 0;
}

CThreadGroup *CMyserver::GetThreadGroup(int threadEntity)
{
	CThreadGroup *pThreadGroup = NULL;
	//        map<int, CThreadGroup*>::iterator mapItr = m_threadGroupList.find(threadEntity);
	//        if (mapItr != m_threadGroupList.end())
	//        {
	//                pThreadGroup = mapItr->second;
	//        }
	if (threadEntity < 0 || threadEntity >= MAXENTITYNUM)
	{
		WriteRunInfo::WriteLog("Error entity type %d, delete the msg",  threadEntity);
		return NULL;
	}
	return pThreadGroup;
}

void sighandler(int signum)
{
	sigset_t block_mask;

	if (signum == SIGCHLD)
	{
		int errno2, result; 
		pid_t deadpid;
		errno2 = errno;
		while((deadpid = waitpid(0, &result, WNOHANG)) > 0)
		{
		}
		errno = errno2;
		signal(SIGCHLD, sighandler);
	}
	else if (signum == SIGUSR1)
	{
		signal(SIGUSR1, sighandler);
	}
	else if ( signum == SIGINT || signum == SIGTERM)
	{                                                
		/*屏蔽所有可能的信号*/
		sigemptyset( &block_mask );
		sigaddset( &block_mask, SIGINT );
		sigaddset( &block_mask, SIGQUIT );
		sigaddset( &block_mask, SIGTERM );
		sigaddset( &block_mask, SIGCHLD );
		sigaddset( &block_mask, SIGIO );
		sigaddset( &block_mask, SIGPIPE );
		sigaddset( &block_mask, SIGHUP );
		sigaddset( &block_mask, SIGSEGV);
		sigprocmask( SIG_BLOCK, &block_mask, NULL );
		exit(0);
	}
}

int CMyserver::CommonStartServer()
{
	int ret = 0;
	//定时线程
	CThreadGroup * pTimerThreadGrp = new CThreadGroup(1, TIMERENTITY);
	ret = pTimerThreadGrp->Init();
	if (ret != 0)
	{       
		WriteRunInfo::WriteLog("Init timer thread fail");
		return -1;
	}
	WriteRunInfo::WriteLog("Init timer thread succeed");

	//收发线程组
	int nTcpThreadNum =  m_baseConf.GetConfigItem().nTcpThreadNum;
	CThreadGroup *pRecvAndSendThreadGrp = new CThreadGroup(nTcpThreadNum, RECVANDSENDMSGENTITY);
	ret = pRecvAndSendThreadGrp->Init();
	if (ret != 0)
	{       
		WriteRunInfo::WriteLog("Init recvandsend thread fail");
		return -1;
	}       
	WriteRunInfo::WriteLog("Init recvandsend thread succeed");

	//维护线程
	CThreadGroup * pMaintainThreadGrp = new CThreadGroup(1, MAINTAINENTITY);
	ret = pMaintainThreadGrp->Init();
	if (ret != 0)
	{       
		WriteRunInfo::WriteLog("Init maintain thread fail");
		return -1;
	}       
	WriteRunInfo::WriteLog("Init maintain thread succeed");

	//监听线程
	int nPort = m_baseConf.GetConfigItem().nTcpPort;
	string sHost = m_baseConf.GetConfigItem().sServerAddr;
	string sTmpData = string((char*)(&nPort), 4);
	sTmpData.append(sHost);

	CThreadGroup *pListenThreadGrp = new CThreadGroup(1, LISTENENTITY, sTmpData);
	ret = pListenThreadGrp->Init();
	if (ret != 0)
	{       
		WriteRunInfo::WriteLog("Init listen thread fail");
		return -1;
	}
	WriteRunInfo::WriteLog("Init listen thread succeed");                
	return 0;
}

