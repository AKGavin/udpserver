/*
* =======================================================================
* Copyright (c) 2009-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: HttpRecvAndSendTask.h
* Description	: 
* -----------------------------------------------------------------------
* Author        : wangtiefeng (wtf), wangtf418@163.com
* Company		: 
* Date			: 2009-07-15
* -----------------------------------------------------------------------
* History		: 
*		<author>		  <time>		<version>		<description>
*      ---------------------------------------------------------------
*		wangtiefeng		2009-07-15			1.0			
* =======================================================================
*/
#ifndef _HTTPRECVANDSENDTASK_H_8C3D8EE1_8D04_42c6_BC62_E8D86F2B138D_
#define _HTTPRECVANDSENDTASK_H_8C3D8EE1_8D04_42c6_BC62_E8D86F2B138D_

#include "framecommon/framecommon.h"
#include "recvandsendtask.h"
#include "netprocessthread.h"

using namespace MYFRAMECOMMON;

#define HTTP_LIND_END "\r\n"
#define HTTP_HEAD_END "\r\n\r\n"
#define HTTP_COMMON_CONTENT_TYPE "application/octet-stream"
#define HTTP_COMMON_CONNECT_TYPE "Close"

//#define HTTP_RETURN_VERSION "HTTP/1.1 200 OK"
#define HTTP_RETURN_VERSION "HTTP/1.1"

#define GEN_HTTP_RET_HEAD(sHTTPHEAD, sContentLen, sStatusCode, sReasonPhrase) \
	(sHTTPHEAD.clear(), sHTTPHEAD + HTTP_RETURN_VERSION +  " " + sStatusCode + " " + sReasonPhrase + HTTP_LIND_END\
	+ "Content-Length: " + sContentLen + HTTP_LIND_END\
	+ "Content-Type: " + HTTP_COMMON_CONTENT_TYPE + HTTP_LIND_END\
	+ "Connection: " + HTTP_COMMON_CONNECT_TYPE + HTTP_HEAD_END);

const int TORNADO_ENCRYPT_EXTRA_LEN = 16;

static const char RESPONSECODE[][2] = {
										{"200", "OK"},
										{"404", "Not Found"},
									  };

template<typename T>
class CHttpRecvAndSendTask : public CRecvAndSendTask
{
public:
	CHttpRecvAndSendTask(CTcpSocket *pSocket, CNetProcessThread *pWorkThread, int nListType=0);
	virtual ~CHttpRecvAndSendTask();
	virtual int DestroySelf();
	virtual int Init();
	virtual int PutMsgToSendList(const char *pBuf, int iLen);
	virtual int ProcessPacket(const char *pPacket, unsigned int uiPacketLen);

	int SetResponseCodeAndPhrase(string& strResponseCode, string& strResponsePhrase);
protected:        
	virtual int RecvProcess();
	virtual int SendProcess();
protected:
	void ReSetRecvParameter();
	bool IfFinishRecv();
protected:
	int AddToEpoll( void );
	int SendToList(char *pBuf, int nLen);
	int ParseOnePacket(const char *pPacket, unsigned int uiPacketLen);
	int ParsePackets(char *pPacket, unsigned int uiPacketLen);

	int GetHttpHead();
	int GetHttpBody();
	int GetChunked(const char *pBuf, int nLen);

protected:
	bool m_bIfClose;

	enum State
	{
		ReadHttpHead = 0,
		ReadHttpBody,
	};
	enum SendStat
	{
		SendHttpHead = 0,
		SendHttpBody
	};
	State m_state;
	SendStat m_SendState;
	char m_newRecvBuf[RECVBODYMAXLEN + TORNADO_ENCRYPT_EXTRA_LEN];
	unsigned int m_iHasRecvLen;
	unsigned int m_iBodyHaseRecvLen;
	string m_sHttpHead;
	ULONG64 m_nContentLen;
	T m_DataProcess;

	string m_strResponseCode;
	string m_strResponsePhrase;
	//bool m_bIfDisConnAfterRecv;
	//bool m_bIfDisConnAfterSend;
public:
	static const char RESPONSECODE[][2];
};

template<typename T>
CHttpRecvAndSendTask<T>::CHttpRecvAndSendTask(CTcpSocket *pSocket, CNetProcessThread *pWorkThread, int nListType/* =0 */)
	: CRecvAndSendTask(pSocket, pWorkThread, nListType), m_DataProcess(pWorkThread, this, pSocket)
{
	m_bIfClose = false;
	ReSetRecvParameter();
	m_state = ReadHttpHead;
	m_SendState = SendHttpHead;

	m_strResponseCode.assign("200");
	m_strResponsePhrase.assign("OK");
}

template<typename T>
CHttpRecvAndSendTask<T>::~CHttpRecvAndSendTask()
{
}

template<typename T>
int CHttpRecvAndSendTask<T>::DestroySelf()
{
	delete this;
	return 0;
}

template<typename T>
int CHttpRecvAndSendTask<T>::Init()
{
	AddToEpoll();
	return 0;
}

template<typename T>
int CHttpRecvAndSendTask<T>::AddToEpoll(void)
{          
//	int tmpEvent = MYPOLLERR | MYPOLLHUP | MYPOLLET | MYPOLLIN ;  
	int tmpEvent = MYPOLLERR | MYPOLLHUP | MYPOLLIN;		//使用水平触发
	if (m_sendList.GetNum() != 0 || m_pCurSendMsg != NULL)
	{
		tmpEvent = tmpEvent|MYPOLLOUT;
	}

	CNetProcessThread *pNetThread = (CNetProcessThread*)m_pThread;
	CEpoll *pEoll = pNetThread->GetEpoll();
	pEoll->AddToEpoll(this, tmpEvent);
	return 0;
}

template<typename T>
int CHttpRecvAndSendTask<T>::PutMsgToSendList(const char *pBuf, int iLen)
{
	int iNewLen = 0;
	const char *pNewSendBuf = m_DataProcess.ProcessSendPacket(pBuf, (unsigned)iLen, (unsigned&)iNewLen);
	if(pNewSendBuf == NULL)
	{
		delete [] pBuf;
		DestroySelf();
		return -1;
	}
	else
	{
		if(pNewSendBuf != pBuf)
			delete [] pBuf;
	}

	return SendToList((char *)pNewSendBuf, iNewLen);
}

template<typename T>
int CHttpRecvAndSendTask<T>::SetResponseCodeAndPhrase(string& strResponseCode, string& strResponsePhrase)
{
	m_strResponseCode = strResponseCode;
	m_strResponsePhrase = strResponsePhrase;
	return 0;
}

template<typename T>
int CHttpRecvAndSendTask<T>::SendToList(char *pBuf, int nLen)
{        
	/*if (pBuf == NULL || nLen == 0)
	{       
		return -1;
	}*/

	if (pBuf == NULL && nLen != 0)
	{
		return -1;
	}

	//Add Http Head====================================
	string strHTTPHead;
	char BodLen[12];
	memset(BodLen, 0 , sizeof(BodLen));
	snprintf(BodLen, sizeof(BodLen), "%u", nLen);
	strHTTPHead = GEN_HTTP_RET_HEAD(strHTTPHead, BodLen, m_strResponseCode, m_strResponsePhrase);

	//char *pHttpReHeadBuf = new char[strHTTPHead.length()];
	//memcpy(pHttpReHeadBuf, strHTTPHead.c_str(), strHTTPHead.length());
	//CSendMsg *pSendMsg = new CSendMsg((int)strHTTPHead.length(),pHttpReHeadBuf);
	//m_sendList.push_back(pSendMsg);
	//pSendMsg = new CSendMsg(nLen, pBuf);
	//m_sendList.push_back(pSendMsg);
	char *pHttpReHeadBuf = new char[strHTTPHead.length() + nLen];
	memcpy(pHttpReHeadBuf, strHTTPHead.c_str(), strHTTPHead.length());
	memcpy(pHttpReHeadBuf + strHTTPHead.length(), pBuf, nLen);
	delete [] pBuf;
	CSendMsg *pSendMsg = new CSendMsg(strHTTPHead.length() + nLen, pHttpReHeadBuf);
	m_sendList.push_back(pSendMsg);

	int tmpEvent = MYPOLLERR|MYPOLLHUP|MYPOLLIN| MYPOLLOUT;
	CNetProcessThread *pNetThread = (CNetProcessThread*)m_pThread;
	CEpoll *pEoll = pNetThread->GetEpoll();
	pEoll->ModifyFromEpoll(this, tmpEvent);
	return 0;
}

//if this function returns -1 ,task will be destroy
template<typename T>
int CHttpRecvAndSendTask<T>::RecvProcess()
{
	int iResult = 0;
	switch( m_state ) 
	{
	case ReadHttpHead:
		{
			iResult = GetHttpHead();
			break;
		}

	case ReadHttpBody:
		{
			iResult = GetHttpBody();
			break;
		}
	}
	if (IfFinishRecv())
	{
		//int ret = ParseOnePacket(m_newRecvBuf + m_sHttpHead.size(), m_nContentLen);
		int ret = ParsePackets((char *)m_newRecvBuf, m_iHasRecvLen);
		//ReSetRecvParameter();
		if(ret != 0)
		{
			return -1;
		}
	}

	return iResult; 
}


//if this function returns -1 ,task will be destroy
template<typename T>
int CHttpRecvAndSendTask<T>::SendProcess()
{
	int retValue = 0;
	if (m_pCurSendMsg != NULL)
	{
		retValue = SendData();
	}
	else //NULL
	{
		m_pCurSendMsg = GetMsgFromSendList();
		if (m_pCurSendMsg != NULL)
		{
			retValue = SendData();                        
		} 
		else
		{
			int tmpEvent = MYPOLLERR|MYPOLLHUP|MYPOLLIN;
			CNetProcessThread *pNetThread = (CNetProcessThread*)m_pThread;
			CEpoll *pEoll = pNetThread->GetEpoll();
			pEoll->ModifyFromEpoll(this, tmpEvent);
		}
	}

	if (retValue == 1)
	{
		//if(m_SendState == SendHttpHead)
		//{
		//	m_SendState = SendHttpBody;
		//}
		//else
		//{
		//	m_SendState = SendHttpHead;
			if(m_DataProcess.GetIfDisConnectAfterSend())
				DestroySelf();
		//	WRITE_TRACE("Send Http Cmd Res Succeed!");
		//}
	}
	
	return retValue;
}

template<typename T>
void CHttpRecvAndSendTask<T>::ReSetRecvParameter()
{
	m_iHasRecvLen = 0;
	m_iBodyHaseRecvLen = 0;
	m_nContentLen = 0;
	m_state = ReadHttpHead;
	memset(m_newRecvBuf, 0, sizeof(m_newRecvBuf));
	m_sHttpHead.clear();
}

template<typename T>
bool CHttpRecvAndSendTask<T>::IfFinishRecv()
{
	if(m_iHasRecvLen > 0 && m_iBodyHaseRecvLen > 0 && (m_iHasRecvLen - (unsigned)m_sHttpHead.size()) >= m_nContentLen)
		return true;
	else return false;
}

template<typename T>
int CHttpRecvAndSendTask<T>::ParseOnePacket(const char *pPacket, unsigned int uiPacketLen)
{
	int ret = 0;
	ret = m_DataProcess.ProcessRecvPacket(pPacket, uiPacketLen);
	if(ret != 0)
		return -1;
	else return ProcessPacket(pPacket, uiPacketLen);
}

template<typename T>
int CHttpRecvAndSendTask<T>::ParsePackets(char *pPacket, unsigned int uiPacketLen)
{
	char *pCurPos = pPacket;
	char *pEndPos = pPacket + uiPacketLen;
	int nRet = 0;
	int nRestLen = pEndPos - pCurPos;
	while (1)
	{
		char *pTmp = strstr(pCurPos, "\r\n\r\n");
		if(pTmp == NULL)
		{
			m_state = ReadHttpHead;
			break;
		}
		//Get Http Head
		m_sHttpHead = string(pCurPos, pTmp - pCurPos + 4);
		string sOut;
		int nTmp;		
		//find content-len
		nTmp = CToolKit::GetStringByLabel(m_sHttpHead, "Content-Length:", "\r\n", sOut, 0, 0);
		if(nTmp != 0)
		{
			WriteRunInfo::WriteLog("There is no Content-Length");
			return -1;
		}
		m_nContentLen = strtoull(sOut.c_str(), NULL, 10);
		
		if (m_nContentLen > (pEndPos - pCurPos) - m_sHttpHead.length())
		{
			m_state = ReadHttpBody;
			m_iBodyHaseRecvLen = (pEndPos - pCurPos) - m_sHttpHead.length();
			break;
		}
		nRet = ParseOnePacket(pTmp + 4, m_nContentLen);
		if(nRet != 0)
		{
			WriteRunInfo::WriteLog("ParseOnePacket Failed");
			WriteRunInfo::WriteLog("Buf = %s", pCurPos);
			return -1;
		}

		pCurPos += (m_sHttpHead.length() + m_nContentLen);
		nRestLen = pEndPos - pCurPos;
	}
	
	char TmpBuf[RECVBODYMAXLEN];
	nRestLen = pEndPos - pCurPos;
	m_iHasRecvLen = nRestLen;
	if (nRestLen > 0)
	{
		if (nRestLen <= (int)RECVBODYMAXLEN)
		{
			memcpy(TmpBuf, pCurPos, nRestLen);
			memset(m_newRecvBuf, 0, RECVBODYMAXLEN);
			memcpy(m_newRecvBuf, TmpBuf, nRestLen);
		}
		else
		{
			nRet = -1; 
		}		
	}
	else
	{
		memset(m_newRecvBuf, 0, RECVBODYMAXLEN);
		m_state = ReadHttpHead;
	}
	return nRet;	
}



template<typename T>
int CHttpRecvAndSendTask<T>::ProcessPacket(const char *pPacket, unsigned int uiPacketLen)
{
	return 0;
}

template<typename T>
int CHttpRecvAndSendTask<T>::GetHttpHead()
{
	int nRet=0;

	int needRecv = MAXRECVLEN - m_iHasRecvLen;
	if (needRecv <= 0) 
	{   
		WriteRunInfo::WriteLog("The httphead is too large");
		return -1;
	}

	int iReced = recv(m_pTcpSocket->GetSocket(), m_newRecvBuf + m_iHasRecvLen, needRecv, MSG_DONTWAIT);

	WriteRunInfo::WriteTrace("HEAD : Http Recv m_iHasRecvLen = %d", m_iHasRecvLen);
	WriteRunInfo::WriteTrace("HEAD : Http Recv Len = %d", iReced);
	nRet = -1;
	do 
	{
		//Recv socket close=======
		if (iReced == 0)
		{
			WriteRunInfo::WriteTrace("the httpclient close the socket %d", m_pTcpSocket->GetSocket());
			break;
		}

		//Recv error==============
		if(iReced < 0)
		{
			if ( errno != EINTR && errno != EAGAIN )
			{
				WriteRunInfo::WriteLog("socket error , errnor is %d", errno);
			}
			else nRet = 0;
			break;
		}

		//Recv succeed===========
		m_iHasRecvLen += iReced;

		char *pTmp = strstr(m_newRecvBuf, "\r\n\r\n");
		if(pTmp == NULL)
		{
			nRet = 0;
			break;
		}

		//Get Http Head 
		m_sHttpHead = string(m_newRecvBuf, pTmp - m_newRecvBuf + 4);
		WriteRunInfo::WriteTrace("%s", m_sHttpHead.c_str());

		string sOut;
		int nTmp;		
		//find content-len
		nTmp = CToolKit::GetStringByLabel(m_sHttpHead, "Content-Length:", "\r\n", sOut, 0, 0);
		if(nTmp != 0)
		{
			WriteRunInfo::WriteLog("There is not Content-Length in Http Head ");
			WriteRunInfo::WriteLog("%s", m_sHttpHead.c_str());
			return -1;
		}

		m_nContentLen = strtoull(sOut.c_str(), NULL, 10);
		m_iBodyHaseRecvLen = m_iHasRecvLen - (pTmp - m_newRecvBuf + 4);

		if (IfFinishRecv())
		{
			m_state = ReadHttpHead;
		}
		else
		{
			m_state = ReadHttpBody;
		}
		nRet = 0;

	} while (0);

	return nRet;
}

template<typename T>
int CHttpRecvAndSendTask<T>::GetHttpBody()
{
	int nRet=0;
	unsigned int uiNeedRecvLen = (unsigned int)m_sHttpHead.size() + m_nContentLen - m_iHasRecvLen;
	if(uiNeedRecvLen > RECVBODYMAXLEN - (unsigned int)m_iHasRecvLen)
	{
		WriteRunInfo::WriteLog("The httpbody is too large");
		return -1;
	}
	int ret = recv(m_pTcpSocket->GetSocket(), m_newRecvBuf + m_iHasRecvLen, uiNeedRecvLen, MSG_DONTWAIT);

	nRet = -1;
	do 
	{
		if (ret == 0) 
		{
			WriteRunInfo::WriteTrace("the httpclient close the socket %d", m_pTcpSocket->GetSocket());
			break;
		}

		if (ret < 0)
		{
			if ( errno != EINTR && errno != EAGAIN )
			{
				WriteRunInfo::WriteLog("socket error , errnor is %d", errno);
			} 
			else nRet = 0;
			break;
		}

		m_iHasRecvLen += ret;
		m_iBodyHaseRecvLen += ret;
		if (IfFinishRecv())
		{
			m_state = ReadHttpHead;
		}
		else
		{
			m_state = ReadHttpBody;
		}
		nRet = 0;

	} while (0);


	return nRet;
}


#endif


