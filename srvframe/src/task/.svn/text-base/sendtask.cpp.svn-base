/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2005-8-31
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#include "sendtask.h"
#include "netprocessthread.h"
#include "myserver.h"

CSendTask::CSendTask(CTcpSocket *pSocket, CNetProcessThread *pWorkThread, int nListType):
                CSocketTask(pSocket, pWorkThread, nListType)
{
        m_iCurSendLen=0;
        m_iTaskType = SENDTASK;
        m_pSendMsg = NULL;
        AddToEpoll();
}

CSendTask::~CSendTask()
{                      
        //WRITE_TRACE("delete a send task %llu", m_iTaskId);
        if (m_pSendMsg != NULL)
                delete m_pSendMsg;
        list<CSendMsg*>::iterator listItr = m_pSendMsgList.begin();
        for (; listItr != m_pSendMsgList.end();)
        {
                list<CSendMsg*>::iterator tmpItr = listItr;        
                listItr++;
                delete *tmpItr;
                m_pSendMsgList.erase(tmpItr);
        }        
}

int CSendTask::Init()
{
	AddToEpoll();
	return 0;
}

int CSendTask::Process(CMsg *pMsg)
{
        return 0;
}


int CSendTask::HandleProcess(int events)
{        
//        DelFromEpoll();        
        if ((events&MYPOLLERR) == MYPOLLERR || (events&MYPOLLHUP) == MYPOLLHUP)
        {
                CMyserver::GetInstance()->GetCommonStat().AddNormalStat(SOCKETERRORTYPE);
                WriteRunInfo::WriteLog("Receive abnormity event %d", events);
                DestroySelf();
                return 0;
        }  
        int ret = 0;
        //每次最多写10K
        if (events &(MYPOLLOUT|MYPOLLRDNORM)) //可写事件
        {               
                ret = SendData();               
        }
        else //其他事件
        {
                DestroySelf();
                return 0;
        }
        
        if (ret == -1 || ret == 1) //客户端已经关闭, 或者是句柄错误, 或者已经发送完毕
        {
                DestroySelf();
                return 0;
        }
        //当这个任务还没有完的时候才能够加进去
//        AddToEpoll();
        return 0;
}

//int CSendTask::OnTimer(TTimeEvent &timeEvent)
//{
//        CTask::OnTimer(timeEvent);
//        if (timeEvent.m_nEvent == SENDDATATIMEOUTEVENT)
//        {
//                //写日志
//                DestroySelf(); //将句柄删除
//                return 0;
//        }
//        else 
//        {
//                //写日志忽略这些超时事件
//                WriteRunInfo::WriteLog("Error time out event %d", timeEvent);
//                DestroySelf();                
//        }        
//        return 0;
//}

int CSendTask::DestroySelf()
{      
        delete this;
        return 0;
}


int CSendTask::SendData()
{
        if (m_pSendMsg == NULL)
        {
                m_pSendMsg = GetSendMsg();
                if (m_pSendMsg == NULL)
                {
                        WRITE_TRACE("Get send msg , but no msg");
                        return 0;
                }
                m_iCurSendLen = 0;
        }

        if (m_pSendMsg->GetBody() == NULL)
        {
                WRITE_TRACE("The sendmsg body is NULL, exit the sendtask .....");
                return 1;
        }
 
        
        int retLen = 0;
        int retValue = 0;
        int sendLen = (SENDMAXLEN >  (m_pSendMsg->GetLen()- m_iCurSendLen))?
                 (m_pSendMsg->GetLen()- m_iCurSendLen):SENDMAXLEN;
        retLen = send( m_pTcpSocket->GetSocket(), m_pSendMsg->GetBody()+ m_iCurSendLen, 
                sendLen, MSG_DONTWAIT);
        if (retLen == sendLen) //全部发送出去
        {
                delete m_pSendMsg; //发送完毕,把消息删除掉
                m_pSendMsg = NULL;
                m_iCurSendLen = 0;
                retValue = 0;         
                //再取一个消息出来
                m_pSendMsg = GetSendMsg();
                if (m_pSendMsg != NULL)
                {
                        if (m_pSendMsg->GetBody() == NULL)
                        {
                                retValue = 1; //结束了
                        }
                }                
        }
        else if (retLen > 0 && retLen < sendLen) //发送了一部分
        {
                m_iCurSendLen += retLen;
                retValue = 0;
        }
        else //<= 0
        {
                if (errno == ECONNRESET||
                errno == EDESTADDRREQ||
                errno == ENOTCONN||
                errno == EPIPE||
                errno == ETIMEDOUT || errno == EMSGSIZE)
                {
                        retValue = -1;
                }
                //写日志
                WRITE_TRACE("Send data len invalid %d", retLen);                        
        }
        return retValue;
}

int CSendTask::AddToEpoll()
{          
        int tmpEvent = MYPOLLERR|MYPOLLHUP|MYPOLLET|MYPOLLOUT|MYPOLLRDNORM;                         
        CNetProcessThread *pNetThread = (CNetProcessThread*)m_pThread;
        CEpoll *pEoll = pNetThread->GetEpoll();
        pEoll->AddToEpoll(this, tmpEvent);
        return 0;
}

CSendMsg* CSendTask::GetSendMsg()
{
        CSendMsg *pSendMsg = NULL;
        if (m_pSendMsgList.size() == 0)
        {
                return pSendMsg;
        }
        
        list<CSendMsg*>::iterator listItr =  m_pSendMsgList.begin();
        pSendMsg = *listItr;
        //将这个项去掉
        m_pSendMsgList.erase(listItr);
        return pSendMsg;
}

void CSendTask::PutEndMsg()
{
        CSendMsg *pSendMsg = new CSendMsg(0, NULL);
        PutSendMsg(pSendMsg);        
}


void CSendTask::Routine()
{
        return;
}

