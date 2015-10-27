/*
* =======================================================================
* Copyright (c) 2009-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: udpcommonsendthread.cpp
* Description	: 
* -----------------------------------------------------------------------
* Author        : wangtiefeng (wtf), wangtf418@163.com
* Company		: 
* Date			: 2009-07-24
* -----------------------------------------------------------------------
* History		: 
*		<author>		  <time>		<version>		<description>
*      ---------------------------------------------------------------
*		wangtiefeng		2009-07-24			1.0			
* =======================================================================
*/
#include "framecommon/framecommon.h"
#include "commmsg.h"
#include "udpcommonsendthread.h"

using namespace MYFRAMECOMMON;

CUdpCommonSendThread::CUdpCommonSendThread(int iEntityType, int iEntityId)
	: CWorkThread(iEntityType, iEntityId)
{
	m_iCurSendSocket = INVALID_SOCKET;
	m_pSendBuf = NULL;
	ClearSendDataFlag();
}


int CUdpCommonSendThread::PutMsgToInputList( CMsg *pMsg )
{
	m_InputList.put( pMsg );
	return 0;
}

int CUdpCommonSendThread::Run()
{
	while (m_nRunFlag)
	{
		CMsg *pMsg = m_InputList.get();
		if (pMsg != NULL)
		{
			Process(pMsg);
			if(IfHaveDataToSend())
			{
				CUdpCommonBufMsgBody *pMsgBody = (CUdpCommonBufMsgBody *)pMsg->GetMsgBody();
				if(pMsgBody != NULL && m_pSendBuf != NULL && m_iSendPackecLen > 0)
				{
					m_UdpSock.UdpSend(pMsgBody->GetSocket(), (char *)m_pSendBuf, m_iSendPackecLen, pMsgBody->GetClientIp(), pMsgBody->GetCientPort());
					DelSendBuf();
				}
				ClearSendDataFlag();
			}
			delete pMsg;
		}
	}
	return 0;
}

int CUdpCommonSendThread::Process(CMsg * pMsg)
{
	if(pMsg == NULL)
	{
		WriteRunInfo::WriteError( "Errror : %s:%d:%s : pMsg is Null!" , __FILE__ , __LINE__ , __FUNCTION__ );
		return -1;
	}
	TMsg *pTMsg = pMsg->GetTMsg();
	if (pTMsg->msgType == UDPCLIENTMSG)
	{
		MsgProcess(pMsg);
	}    
	else  
	{
		WriteRunInfo::WriteError("%s:%d:%s : recv invalid msg %d", __FILE__ , __LINE__ , __FUNCTION__  , pTMsg->msgType);
	}
	return 0;
}

void CUdpCommonSendThread::SetSendDataFlag()
{
	m_bSendDataFlag = true;
}

void CUdpCommonSendThread::ClearSendDataFlag()
{
	m_bSendDataFlag = false;
}

bool CUdpCommonSendThread::IfHaveDataToSend()
{
	return m_bSendDataFlag;
}

void CUdpCommonSendThread::DelSendBuf()
{
	if(m_pSendBuf != NULL)
	{
		delete [] m_pSendBuf;
		m_pSendBuf = NULL;
		m_iSendPackecLen = 0;
	}
}





