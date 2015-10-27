/*
* FileName:       
* Author:         wangqifeng  Version: 1.0  Date: 2006-10-21
* Description:    
* Version:        
* Function List:  
*                 1.
* History:        
*     <author>   <time>    <version >   <desc>
*/
#include "classfactory.h"
#include "listenthread.h"
#include "sendmsgthread.h"
#include "maintainthread.h"
#include "timerthread.h"

CClassFactory::CClassFactory()
{
}

CClassFactory::~CClassFactory()
{
}


CSocketTask *CClassFactory::GenSocketTask(CNetProcessThread *pWorkThread, CTcpSocket *pTcpSocket,
										  int iTaskType, CTaskArg * pTaskArg/* = NULL*/ )
{
	CSocketTask *pSocketTask = NULL;
	switch(iTaskType)
	{
	case LISTENTASK:
		{
			pSocketTask = new CListenTask(pTcpSocket, pWorkThread);
			break;
		}
	default:
		{  
			//WriteRunInfo::WriteLog("Invalid socket task type %d", iTaskType);
			break;
		}
	}                  
	return pSocketTask;
}

CWorkThread * CClassFactory::GenWorkThread(int nEntityType, int nEntityId, string sAnnexData, void *arg)
{
	CWorkThread *pWorkThread = NULL;
	switch(nEntityType)
	{
	case TIMERENTITY:
		{
			pWorkThread = new CTimerThread(nEntityType, nEntityId);
			break;
		}
	case LISTENENTITY:
		{
			int nPort;
			memcpy(&nPort, sAnnexData.c_str(), 4);
			string sHost = sAnnexData.substr(4); 
			TListenBlock *pListenBlock = NULL;
			if (arg == NULL)
				pListenBlock = new TListenBlock;
			else
				pListenBlock = (TListenBlock*)arg;
			pWorkThread = new CListenThread(nEntityType, nEntityId, sHost, nPort, pListenBlock);
			break;
		}
	case RECVANDSENDMSGENTITY:
		{
			pWorkThread = new CRecvAndSendMsgThread(nEntityType, nEntityId);
			break;
		}
	case MAINTAINENTITY:
		{
			pWorkThread = new CMaintainThread(nEntityType, nEntityId);
			break;
		}
	

	default:
		{                        
			break;
		}
	}          
	return pWorkThread;
}


CBaseMaintainCmd *CClassFactory::GenMaintainCmd(int nMsgType, int nBufLen, char *pBuf)
{
	CBaseMaintainCmd *pCmd = NULL;
	if (nMsgType == CMD_DEBUGINFO || nMsgType == PRINTSTATINFO)
	{
		pCmd = new CBaseMaintainCmd(nMsgType, nBufLen, pBuf);
	}
	return pCmd;
}


CMyHttpCmd *CClassFactory::GenHttpCmd(string &sInHead, string &sInBody)
{
	return NULL;
}

