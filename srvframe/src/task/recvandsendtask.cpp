/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2006-7-25
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#include "recvandsendtask.h"
#include "threadgroup.h"
#include <errno.h>
#include "netprocessthread.h"
#include "myserver.h"

CRecvAndSendTask::CRecvAndSendTask(CTcpSocket * pSocket, CNetProcessThread * pWorkThread, int nListType):
        CRecvTask(pSocket, pWorkThread, nListType)
{        
        m_nRecvLen = 0;
        m_iHasRecvLen = 0;        
        m_iCurSendLen = 0;
        m_pCurSendMsg = NULL;
}

CRecvAndSendTask::~CRecvAndSendTask()
{
        if (m_pCurSendMsg != NULL)
                delete m_pCurSendMsg;
        
        list<CSendMsg*>::iterator listItr =  m_sendList.begin();        
        for (; listItr != m_sendList.end(); )
        {
                list<CSendMsg*>::iterator tmpItr = listItr;
                listItr++;
                delete *tmpItr;
                m_sendList.erase(tmpItr);
        }
}

int CRecvAndSendTask::Init()
{
	AddToEpoll();
	return 0;
}

int CRecvAndSendTask::HandleProcess(int events)
{        
        if ((events&MYPOLLERR) == MYPOLLERR || (events&MYPOLLHUP) == MYPOLLHUP)
        {
                WriteRunInfo::WriteLog("Recv a invalid data pack, event %d, close the socket %d error %d [threadtype = %d]", events, 
                        m_pTcpSocket->GetSocket(), errno, m_pThread->GetEntityType());
                CMyserver::GetInstance()->GetCommonStat().AddNormalStat(SOCKETERRORTYPE);
                DestroySelf();
                return 0;
        }
        //不断改变开始时间, 如果超过２分钟没有响应,就会自动断开 
        m_nStartTime = CMyserver::GetInstance()->GetTimeVal().tv_sec;
        
        int ret = 0;
        if ((events& MYPOLLIN) == MYPOLLIN) //读事件
        {
                ret = RecvProcess();
                if (ret == -1)
                {
                    DestroySelf();
                    return -1;
                }
        }
        
        if ((events& MYPOLLOUT) == MYPOLLOUT) //写事件
        {
                ret =  SendProcess(); 
                if ( ret == -1 )
                {
                    DestroySelf();
                    return -1;                	
                }
        }               
        return 0;
}

int CRecvAndSendTask::AddToEpoll()
{          
        int tmpEvent = MYPOLLERR|MYPOLLHUP|MYPOLLET|MYPOLLIN;                         
        if (m_sendList.GetNum() != 0 || m_pCurSendMsg != NULL)
        {
                tmpEvent = tmpEvent|MYPOLLOUT;
        }
        
        CNetProcessThread *pNetThread = (CNetProcessThread*)m_pThread;
        CEpoll *pEoll = pNetThread->GetEpoll();
        pEoll->AddToEpoll(this, tmpEvent);
        return 0;
}


CSendMsg *CRecvAndSendTask::GetMsgFromSendList()
{
        if (m_sendList.begin() == m_sendList.end())
        {
                return NULL;
        }
        list<CSendMsg*>::iterator itr = m_sendList.begin();
        CSendMsg *pMsg = (*itr);
        m_sendList.erase(itr);
        return pMsg;                
}


int CRecvAndSendTask::PutMsgToSendList(const char *pBuf, int nLen)
{        
        if (pBuf == NULL || nLen == 0)
        {       
                return -1;
        }               
        char *pTmpBuf = new char[nLen];
        memcpy(pTmpBuf, pBuf, nLen);
        CSendMsg *pSendMsg = new CSendMsg(nLen, pTmpBuf);
        
        m_sendList.push_back(pSendMsg);
        int tmpEvent = MYPOLLERR|MYPOLLHUP|MYPOLLET|MYPOLLIN|MYPOLLOUT;
        CNetProcessThread *pNetThread = (CNetProcessThread*)m_pThread;
        CEpoll *pEoll = pNetThread->GetEpoll();
        pEoll->ModifyFromEpoll(this, tmpEvent);
        return 0;
}

int CRecvAndSendTask::Process(CMsg *pMsg)
{
        TMsg *pTMsg = pMsg->GetTMsg();
        if (pTMsg->msgType == SENDMSGTYPE)
        {
                CSendMsgBody *pBody = (CSendMsgBody*)pMsg->GetMsgBody();
                pBody->SetSendBuf(NULL);                
                PutMsgToSendList(pBody->GetSendBuf(), pBody->GetSendLen());
        }
        else
        {
                WriteRunInfo::WriteLog("The task recv a invalid msg %d", pTMsg->msgType);
        }
        return 0;
}


int CRecvAndSendTask::SendData()
{         
        int retLen = 0;
        int retValue = 0;
        unsigned int sendLen = (SENDMAXLEN >  (m_pCurSendMsg->GetLen()- m_iCurSendLen))?
                 (m_pCurSendMsg->GetLen()- m_iCurSendLen):SENDMAXLEN;
        retLen = send( m_pTcpSocket->GetSocket(), m_pCurSendMsg->GetBody()+ m_iCurSendLen, 
                sendLen, MSG_DONTWAIT);
       // WRITE_TRACE("send data %d",retLen);
        if (retLen > 0)
        {               
                m_iCurSendLen += retLen;
                if (m_iCurSendLen == m_pCurSendMsg->GetLen()) //全部发送出去
                {                
                        delete m_pCurSendMsg;
                        m_pCurSendMsg = NULL;
                        m_iCurSendLen = 0;
                        retValue = 1;         
                }
                else  //发送了一部分
                {                        
                        retValue = 0;
                }
        }
        else //<= 0
        {
                if (errno == ECONNRESET||
                errno == EDESTADDRREQ||
                errno == ENOTCONN||
                errno == EPIPE||
                errno == ETIMEDOUT || errno == EMSGSIZE)
                {
                        WriteRunInfo::WriteLog("Send error %d:%s", errno, strerror(errno));
                        return -1;
                }
                else
                {
                    //写日志
                    WriteRunInfo::WriteTrace("Send data len invalid %d", retLen);
                    return 2;
                }
        }
        return retValue;
}


int CRecvAndSendTask::SendBufProcess(const char * pBuf, int nLen)
{
        if (pBuf == NULL || nLen == 0)
        {
                return -1;
        }
        
        int retValue = 0;
        int ret = send(m_pTcpSocket->GetSocket(), pBuf, nLen, MSG_DONTWAIT);
        if (ret == nLen) //发送完毕
        {
                retValue =  -1;
        }
        else if (ret >= 0 && ret < nLen) //将剩下的放入队列
        {

                PutMsgToSendList(pBuf + ret, nLen - ret);
                retValue =  0;
        }
        else //<　０
        {
                if (errno != EINTR && errno != EAGAIN)
                {
                        WriteRunInfo::WriteLog("send fail, error(%d) is %s", errno, strerror(errno));
                        CMyserver::GetInstance()->GetCommonStat().AddNormalStat(SOCKETERRORTYPE);
                        retValue = -1;
                }          
                else    //继续接收消息

                {
                        WRITE_TRACE("EAGAIN, EINTR ");
                        PutMsgToSendList(pBuf, nLen);
                        retValue = 0;
                }                        
        }        
        return retValue;
}


