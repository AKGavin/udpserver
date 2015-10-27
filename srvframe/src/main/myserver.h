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
#ifndef    __MYSERVER_H
#define    __MYSERVER_H
#include "commonthread.h"
#include "threadgroup.h"
#include "timerthread.h"
#include "commonstat.h"
#include "baseconf.h"
#include <signal.h>
#include "classfactory.h"
#include "framecommon/framecommon.h"
using namespace MYFRAMECOMMON;


class CWorkThread;

const int RECVPKGNUMSTAT=1;
const int ADDEPOLL=2;
const int DELEPOLL=3;
const int MODEPOLL=4;
const int SOCKETTIMEOUTSTAT=5;
const int MAXENTITYNUM=200;
/**
* dec:
* ClassName:CMyserver
* author: wangqifeng
* version:
*CreateTime: 2006.8.23
*/
class CMyserver
{
public:
	CMyserver();     

	virtual ~CMyserver()
	{
		if (m_pClassFactory != NULL)
			delete m_pClassFactory;
	};

	static CMyserver *GetInstance()
	{        
		return m_pMyServer;
	};

	/**
	*server 数据初始化
	*  \return   static int
	*/
	static int Init();
	static int SetServer(CMyserver *pMyserver);

	virtual int InitServer();
	virtual int StartServer();

	/**
	*发送消息到指定线程
	*  \param   *pMsg
	*  \return   void
	*/
	void PutMsgToThread(CMsg *pMsg);

	/**
	*注册线程组
	*  \param   *pThreadGroup
	*  \return   int
	*/
	int RegisterThreadGroup(CThreadGroup *pThreadGroup);

	/**
	*
	*  \param   iCurrentPoint
	*  \param   &tmpTime
	*  \return   int
	*/
	int SetTimerPoint(int iCurrentPoint, timeval &tmpTime);

	/**
	*
	*  \return   int
	*/
	int  GetTimerPoint()
	{
		return m_iCurrentTimerPoint;
	};

	/**
	*获取当前时间
	*  \return   timeval *
	*/
	timeval&  GetTimeVal()
	{
		return m_timeval;
	}


	/**
	*统计项
	*  \return   CCommonStat&
	*/
	CCommonStat& GetCommonStat()
	{
		return m_commonstat;
	}

	/**
	*基本配置
	*  \return   CBaseConf
	*/
	CBaseConf &GetBaseConf()
	{
		return m_baseConf;
	}            

	CThreadGroup *GetThreadGroup(int threadEntity);                

	CClassFactory *GetClassFactory()
	{
		return m_pClassFactory;
	};


	int CommonStartServer();


protected:        

	int m_iCurrentTimerPoint;
	timeval m_timeval;
	CThreadGroup*  m_threadGroupList[MAXENTITYNUM]; 

	static CMyserver * m_pMyServer;
	CClassFactory *m_pClassFactory;   
	CCommonStat m_commonstat; //统计项        
	CBaseConf m_baseConf;      //基本配置
	int m_nRunFlag;
};
#endif
