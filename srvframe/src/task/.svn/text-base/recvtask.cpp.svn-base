/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2005-12-8
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#include "recvtask.h"
#include "myserver.h"
#include "netprocessthread.h"


CRecvTask::CRecvTask(CTcpSocket *pSocket, CNetProcessThread *pWorkThread, int nListType):CSocketTask(
                pSocket, pWorkThread, nListType)
{
        m_iTaskType = RECVTASK;
        m_nRecvLen = 0;
        m_iHasRecvLen = 0;
}

CRecvTask::~CRecvTask()
{
}

int CRecvTask::Init()
{
	AddToEpoll();
	return 0;
}

int CRecvTask::HandleProcess(int events)
{
//        DelFromEpoll();
        if ((events&MYPOLLERR) == MYPOLLERR || (events&MYPOLLHUP) == MYPOLLHUP)
        {
                CMyserver::GetInstance()->GetCommonStat().AddNormalStat(SOCKETERRORTYPE);
                WriteRunInfo::WriteLog("Recv a invalid data pack, event %d, close the socket %d", events, 
                        m_pTcpSocket->GetSocket());
                DestroySelf();
                return 0;
        }
        int ret = 0;
        if (events& (MYPOLLIN|MYPOLLRDNORM)) //读事件
        {
                ret = RecvProcess();
                if (ret != 0)
                {
                        if (ret == -1)                                
                                WriteRunInfo::WriteLog("The socket occur error");
                        
                        DestroySelf();
                        return -1;
                }
        }
        else //错误的事件
        {
               //写日志
               WriteRunInfo::WriteLog("Error epoll event, %d", events);
               DestroySelf();
               return -1;
        }                
//        AddToEpoll();
        return 0;
}

int CRecvTask::PutMsgToSendList(const char *pBuf, int nLen)
{
	return 0;
}

int CRecvTask::RecvBody()
{
        int retValue = 0;
        int ret = 0;
        //接着接收消息体
        int recvLen = (MAXRECVLEN > m_nRecvLen - m_iHasRecvLen)?(m_nRecvLen - m_iHasRecvLen):MAXRECVLEN;
        WRITE_TRACE("recvlen is %d", recvLen);
        ret = recv(m_pTcpSocket->GetSocket(), m_recvBuf, recvLen, MSG_DONTWAIT);
        if (ret == 0) //对方关闭端口
        {
                //写日志
                WriteRunInfo::WriteLog("The client has closed the socket, the socket %d is invalid\n", m_pTcpSocket->GetSocket());
                return -1;
        }
        else if (ret == recvLen)  //接收成功
        {
                m_iHasRecvLen += ret;                
                m_sRecvStr.append(m_recvBuf, ret);
                if (m_sRecvStr.length() >= m_nRecvLen)
                {
                        retValue = 1; //接收完毕
                }
                else
                {
                        retValue = 0; //还没有接收完毕
                }
        }
        else if (ret > 0)  //部分接收成功
        {
                m_iHasRecvLen += ret;
                retValue = 0;
                m_sRecvStr.append(m_recvBuf, ret);
        }
        else // < 0
        {
                //将一些错误过滤
                if (errno != EINTR && errno != EAGAIN)
                {
                        retValue = -1;
                }          
                else
                {
                        retValue = 0;
                }
        }
        return retValue;
}


int CRecvTask::RecvHttpRequest()
{       
        int retValue = 0;
        int ret = 0;
        //接着接收消息体
        int recvLen = MAXHTTPHEADLEN - m_iHasRecvLen;
        ret = recv(m_pTcpSocket->GetSocket(), m_recvBuf, recvLen, MSG_DONTWAIT);
        if (ret == 0) //对方关闭端口
        {
                //写日志
                WriteRunInfo::WriteLog("The client has closed the socket, the socket %d is invalid\n",  m_pTcpSocket->GetSocket());
                return -1;
        }
        else if (ret > 0) //
        {
                //判断下http头是否接收完毕
                m_iHasRecvLen += ret;
                m_sRecvStr.append(m_recvBuf, ret);                
                const char *tmp = strstr((char*)m_sRecvStr.c_str(), "\r\n\r\n"); //连续两个换行
                if (tmp == NULL) //http头没有结束
                {
                        if (m_sRecvStr.length() >= MAXHTTPHEADLEN) //超过了最大http头子的长度
                        {
                                WriteRunInfo::WriteLog("The http  head is invalid, the len is greater than maxlen %d", MAXHTTPHEADLEN);
                                return -1;
                        }
                        else   //http头没有结束
                        {
                                retValue = 0; //继续是READHTTPHEADSTATUS
                        }
                }
                else //http头结束了
                {
                        m_sRecvHead = string(m_sRecvStr,0, (int)(tmp-m_sRecvStr.c_str()));
                        string sTmpLen;
                        int ret = CToolKit::GetStringByLabel(m_sRecvStr, "Content-Length:", "\r\n", sTmpLen);
                        if (ret != 0)
                        {
                                WriteRunInfo::WriteLog("The http head is invalid, it not include Content-Length item");
                                return -1;
                        }
                        unsigned int nConLen = atoi(sTmpLen.c_str());
                        if (nConLen > MAXRECVLEN || nConLen <= 0)
                        {
                                WriteRunInfo::WriteLog("The http bodylen(%d) is invalid", nConLen);
                                return -1;
                        }
                        int nLeftLen = tmp - m_sRecvStr.c_str() + 4 + nConLen - m_sRecvStr.length();
                        if (nLeftLen > 0)
                        {
                               int tmpBodyLen = m_sRecvStr.length() -(tmp - m_sRecvStr.c_str()) - 4;
                               char *pTmpbody = new char[tmpBodyLen];
                               CBufMgr bufMgr1(pTmpbody);
                               memcpy(pTmpbody, tmp + 4, tmpBodyLen);
                               m_sRecvStr = string(pTmpbody, tmpBodyLen);
                               m_nRecvLen= nConLen;
                               m_iHasRecvLen = tmpBodyLen;
                               retValue = 0;  //表示接收未完成标志
                               m_status = READMSGBODYSTATUS;
                        }
                        else  //包体已经接收完了
                        {                                               
                               char *pTmpbody = new char[nConLen];
                               CBufMgr bufMgr1(pTmpbody);
                               memcpy(pTmpbody, tmp + 4, nConLen);
                               m_sRecvStr = string(pTmpbody, nConLen);
                               m_nRecvLen= nConLen;
                               m_iHasRecvLen = nConLen;
                               retValue = 1;  //表示接收完成标志
                        }
                }                
        }       
        else // < 0
        {
                //继续接收消息
                if (errno != EINTR && errno != EAGAIN)
                {
                        WriteRunInfo::WriteLog("Recv fail, error(%d) is %s", errno, strerror(errno));
                        retValue = -1;
                }          
                else
                {
                        retValue = 0;
                }
        }                
        return retValue;
}

int CRecvTask::RecvGetHttpRequest()
{
        int retValue = 0;
        int ret = 0;
        //接着接收消息体
        int recvLen = MAXHTTPHEADLEN - m_iHasRecvLen;
        ret = recv(m_pTcpSocket->GetSocket(), m_recvBuf, recvLen, MSG_DONTWAIT);
        if (ret == 0) //对方关闭端口
        {
                //写日志
                WriteRunInfo::WriteLog("The client has closed the socket, the socket %d is invalid\n",  m_pTcpSocket->GetSocket());
                return -1;
        }
        else if (ret > 0) //
        {
                m_iHasRecvLen += ret;
                m_sRecvStr.append(m_recvBuf, ret); 
                //unsigned int nPos = m_sRecvStr.find("\r\n\r\n");
                const char *pTmp = strstr((char*)m_sRecvStr.c_str(), "\r\n\r\n");
                if (pTmp != NULL) //接收完成
                {
                        retValue = 1;
                }
                else
                {
                        m_status = READGETHTTPSTATUS;
                }
        }
        else
        {
                        //继续接收消息
                if (errno != EINTR && errno != EAGAIN)
                {
                        WriteRunInfo::WriteLog("Recv fail, error(%d) is %s", errno, strerror(errno));
                        retValue = -1;
                }          
                else
                {
                        retValue = 0;
                }
        }
        return retValue;
}


int CRecvTask::Process(CMsg *pMsg)
{       
        return 0;
}


//int CRecvTask::OnTimer(TTimeEvent &timeEvent)
//{
//        WRITE_TRACE("The request receive time out ....");
//        CTask::OnTimer(timeEvent);
//        if (timeEvent.m_nEvent == RECVDATATIMEOUTEVENT)
//        {                
//                //写日志
//                DestroySelf();                
//        }
//        else  //
//        {
//               //写日志， 说明这个超时是错误的
//               WriteRunInfo::WriteLog("Error time event %d",timeEvent);
//               DestroySelf();
//        }
//        return 0;
//}

//int CRecvTask::DestroySelf()
//{
//        delete this;        
//        return 0;
//}

int CRecvTask::AddToEpoll()
{
        if (m_pTcpSocket == NULL )
        {
                return 0;
        }
        //int tmpEvent = MYPOLLERR|MYPOLLHUP|MYPOLLIN|MYPOLLET|MYPOLLRDNORM;
        int tmpEvent = MYPOLLIN|MYPOLLET|MYPOLLRDNORM;
        
        CNetProcessThread *pNetThread = (CNetProcessThread*)m_pThread;
        CEpoll *pEoll = pNetThread->GetEpoll();
        pEoll->AddToEpoll(this, tmpEvent);
        return 0;
}

