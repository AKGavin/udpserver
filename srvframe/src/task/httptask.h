/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2009-6-3
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef __HTTP_TASK_H
#define __HTTP_TASK_H
#include "recvandsendtask.h"
#include "netprocessthread.h"

const int GETHTTPHEAD=1;
const int GETHTTPBODY=2;

const int DEFAULT_HTTPPORT=80;
#define HTTPSOCKETTASK 100

#define GEN_REQ_HEAD(sMethod, urlFile, sHosts) \
              sMethod + " " + urlFile + \
                " HTTP/1.1\r\n" \
                + "Accept: */*\r\n" \
                + "Accept-Language: zh-cn \r\n"\
                + "Host: " + sHosts + "\r\n" \
                + "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0;windows NT 5.1; SV1)\r\n"\
                + "Connection: keep-alive\r\n";

template<class T>
class CHttpTask:public CRecvAndSendTask
{
public:
    CHttpTask(CNetProcessThread *pWorkThread): CRecvAndSendTask(NULL, pWorkThread, LINKEDLISTTYPE)
    {		
    	InitPara();
		
		m_nPort=0;
		m_pDataProcess = NULL;
    }
    
    ~CHttpTask()
    {
        if (m_pDataProcess != NULL)
            delete m_pDataProcess;
    }

	int HandleProcess(int events);

	
	ULONG64 GetContentLen()
	{
		return m_nContentLen;
	}

	int GetResponseNum()
	{
		return m_nResponseNum;
	}
	
    int SetDataProcess(T *pDataProcess)
    {
    	if (m_pDataProcess != NULL)
			delete m_pDataProcess;
		
        m_pDataProcess = pDataProcess;

		int nRet=0;
		if (pDataProcess != NULL)
		{
			pDataProcess->SetTask(this);
			nRet = pDataProcess->InitTask();
		}
		return nRet;
    }

    void SetReqMethod(const string &sMethod)
    {
        m_sMethod = sMethod;
    }

    int SetUrl(const string &sUrlFile, const string &sHost, const string &sIp, int nPort)
    {
        int nRet=0;
        do
        {
            //string sUrlFile;
//            int ret = CToolKit::ParseUrl(sUrl, m_sIpAddr, m_nPort, sUrlFile, m_sHost);
//            if (ret != 0)
//                break;
//			if (m_nPort == -1)
//				m_nPort = DEFAULT_HTTPPORT;
			m_sHost = sHost;
			m_sUrl = "http://" + m_sHost ;
			m_nPort = nPort;
			m_sIpAddr = sIp;
			if (m_nPort != DEFAULT_HTTPPORT)
				m_sUrl += ":" + CToolKit::IntToStr(m_nPort);
			m_sReqHead = GEN_REQ_HEAD(m_sMethod, sUrlFile, m_sHost)
        }while(0);
        
        return nRet;
    }

    void SetHeader(const string &str)
    {
        m_sReqHead += str + "\r\n";
    }

    virtual int Init()
    {
    	if (ConnectServer() != 0)
			return -1;
       	
       	SendReq();
        return 0;
    }

	int Reset(const string &sUrlFile)
	{
		InitPara();
		
		m_sUrl = "http://" + m_sHost ;
		if (m_nPort != DEFAULT_HTTPPORT)
			m_sUrl += ":" + CToolKit::IntToStr(m_nPort);	
		
		m_sReqHead = GEN_REQ_HEAD(m_sMethod, sUrlFile, m_sHost)		
		return 0;
	}		
protected:      
	void InitPara()
	{
		if (m_pTcpSocket == NULL)
		m_status = INITSTATUS;
		else
		{
			m_status = GETHTTPHEAD;
			
		}
		
		m_iHasRecvLen=0;
	   	m_bIfClose = false;
		m_nResponseNum=0;
        m_nIfChunked = 0;
        m_nContentLen = -1;
        m_nCurChunkedLen = -1;
        m_nCurChunkedRecLen = -1;
		m_sResChunkedBody="";
		m_sMethod = "GET";
		
	}
	
	//重发http请求
	int SendReq()
	{		
        //发送http请求
        m_sReqHead += "\r\n"; //增加一个空行
        //发送请求
        WriteRunInfo::WriteTrace("%s", m_sReqHead.c_str());
		
        PutMsgToSendList(m_sReqHead.c_str(), m_sReqHead.length()); 
		return 0;
	}
	
    virtual int DestroySelf()
    {
        delete this;
        return 0;
    }

	int CloseConnect()
	{
		DelFromEpoll();
		if (m_pTcpSocket != NULL)
		{
			delete m_pTcpSocket;
			m_pTcpSocket = NULL;
			WriteRunInfo::WriteTrace("delete connect");
		}
		return 0;
	}
	
	int ConnectServer()
	{
		if (m_pTcpSocket != NULL)
			return 0;
		
	 	//连接http服务器
        int nSocket=-1;
        int retValue = CMySocket::CreateNoListenSock(SOCK_STREAM,  nSocket);
		if (retValue == -1)
			return -1;
		
        struct sockaddr_in address;
        memset((char *) &address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_port = htons(m_nPort);
        inet_aton(m_sIpAddr.c_str(), &address.sin_addr);

		static int sTotalConNum=0;
		static int sDelayNum=0;
		static int sPreStatTime=CMyserver::GetInstance()->GetTimeVal().tv_sec;
		sTotalConNum++;

		//加入epoll
        m_pTcpSocket = new CTcpSocket(address, sizeof(sockaddr), nSocket);
		m_pTcpSocket->SetNonBlocking(); //设置异步		
        AddToEpoll();
        int tmp = connect(nSocket, (sockaddr*)&address, sizeof(sockaddr));
	        if (tmp != 0)
        {
        	sDelayNum++;
			int now = CMyserver::GetInstance()->GetTimeVal().tv_sec;
			if (now > sPreStatTime)
			{
            	//WriteRunInfo::WriteLog("Connect server; total %d,  delay %d", 
            	//	sTotalConNum, sDelayNum);
            //close(nSocket);
            //return -1;
				sTotalConNum = 0;
				sDelayNum =0;
				sPreStatTime = now;
			}
        }   
		else
		{
			m_status = GETHTTPHEAD;
		}
		return 0;
	}
	
	int RecvProcess();
	int SendProcess();
	int GetHttpBody();
	int GetHttpHead();
	int GetChunked(const char *pBuf, int nLen);

	int Reconnect()
	{
		int nRet=0;
		nRet = CloseConnect();
		if (m_sendList.GetNum() > 0)
			nRet = ConnectServer();
		return 0;
	}
protected:
	bool m_bIfClose;
    string m_sMethod;
    string m_sHost;
    int m_nPort;
	
	int m_nIfChunked;
    ULONG64 m_nContentLen;    
    int m_nCurChunkedLen;
    int m_nCurChunkedRecLen;
	
    string m_sIpAddr;

    
    string m_sUrl;
    string m_sReqHead;
    string m_sHttpHead;
   
    string   m_sResChunkedBody;
    T* m_pDataProcess;

	char m_newRecvBuf[RECVBODYMAXLEN];
	//返回代码
	int m_nResponseNum;
};

template<class T>
int CHttpTask<T>::HandleProcess(int events)
{        
        if ((events&MYPOLLERR) == MYPOLLERR || (events&MYPOLLHUP) == MYPOLLHUP)
        {
                WriteRunInfo::WriteLog("Recv a invalid data pack, event %d, close the socket %d error %d", events, 
                        m_pTcpSocket->GetSocket(), errno);
				if (m_status == INITSTATUS) //如果连接不上，直接删除此task，不要反复连了
					DestroySelf();
				else	
					Reconnect();
                return 0;
        }
        //不断改变开始时间, 如果超过２分钟没有响应,就会自动断开 
        m_nStartTime = CMyserver::GetInstance()->GetInstance()->GetTimeVal().tv_sec;
        
        int ret = 0;
        if ((events& MYPOLLIN) == MYPOLLIN) //读事件
        {
                ret = RecvProcess();
                if (ret == -1)
                {
                        //DestroySelf();
                        Reconnect();
                        return -1;
                }
				else if (ret == -2) //一般是主动删除
				{
					DestroySelf(); 
				}
        }
        
        if ((events& MYPOLLOUT) == MYPOLLOUT) //写事件
        {
                ret =  SendProcess();                                     
                if ( ret == -1 )
                {
                        Reconnect();
                        return -1;                	
                }
        }               
        return 0;
}

template<class T>
int CHttpTask<T>::RecvProcess()
{
    int nRet = 0;
    switch(m_status)
    {
	    case INITSTATUS:
	    case GETHTTPHEAD:
		    {        
			    nRet = GetHttpHead();
			    break;
		    }
	    case GETHTTPBODY:
		    {           
			    nRet = GetHttpBody();
			    break;
		    }
	    default:
		    {
			    nRet = -1;
			    break;
		    }
    }

	if (nRet > 0) //此次http已经完了
	{
		if (m_pDataProcess != NULL)
			nRet=m_pDataProcess->EndRecv();
	}
    return nRet;      
}

template<class T>
int CHttpTask<T>::SendProcess()
{
	if (m_pTcpSocket == NULL)
	{
		WriteRunInfo::WriteLog("Send data, tcpsocket is NULL");
		return 2;
	}

	if (m_status == INITSTATUS)
		m_status = GETHTTPHEAD;
	int ii=0;
    int retValue=0;
	while (ii < 5)
	{
	   
	    if (m_pCurSendMsg != NULL)
	    {
	        retValue = SendData();
	    }	
	    else if (m_pCurSendMsg == NULL)//如果继续为空获取数据发送出去
	    {
	        m_pCurSendMsg = GetMsgFromSendList();
	        if (m_pCurSendMsg != NULL)
	        {
	            retValue = SendData();                        
	        }                        
	        else
	        {
	        	WriteRunInfo::WriteTrace("modify epoll ");
	            int tmpEvent = MYPOLLERR|MYPOLLHUP|MYPOLLIN;
	            CNetProcessThread *pNetThread = (CNetProcessThread*)m_pThread;
	            CEpoll *pEoll = pNetThread->GetEpoll();
	            pEoll->ModifyFromEpoll(this, tmpEvent);
	            retValue = 2;  
	        }
	    }
 		if (retValue ==0 || retValue == 2 || retValue == -1)
 			break;
		ii++;
	}
    return retValue;
}

//virtual int ProcessRecvBodyData(const char *pBody, int nLen)=0;   
template<class T>
int CHttpTask<T>::GetHttpBody()
{       
    int nRet=0;
    int ret = recv(m_pTcpSocket->GetSocket(), m_newRecvBuf, RECVBODYMAXLEN, MSG_DONTWAIT);
    if (ret == 0) //对方已经关闭
    {
	    WriteRunInfo::WriteTrace("The client close the http socket");
	    nRet = -1;
    }
    else if (ret > 0)
    {
	    if (m_nIfChunked == 0) //no chuncked
	    {
	        m_iHasRecvLen += ret;
	        nRet = m_pDataProcess->ProcessRecvBodyData(m_newRecvBuf, ret);
	        if(nRet == -2)	//处理主动切掉连接
	        	return nRet;
            if (m_nContentLen >= 0 && m_iHasRecvLen >= m_nContentLen) //收完了可以进行优化
            {
                WriteRunInfo::WriteTrace("Recv finish");
				if (m_iHasRecvLen > m_nContentLen)
				{
					WriteRunInfo::WriteError("CHttpTask<T>::GetHttpHead : recv too long, m_nContentLen = %d, m_iHasRecvLen = %d", m_nContentLen, m_iHasRecvLen);
				}
				nRet = (m_bIfClose)?-1:1;
            }
	    }
	    else //chunked
	    {
	        nRet = GetChunked(m_newRecvBuf, ret);
	    }
    }
    else //<0
    {
        if ( errno != EINTR && errno != EAGAIN )//错误
	    {
		    WriteRunInfo::WriteLog("socket error , errnor is %d", errno);
		    nRet = -1;
	    }      
    }
    return nRet;
}

template<class T>
int CHttpTask<T>::GetHttpHead()
{
 	int nRet=0;
//    if (m_status == INITSTATUS)
//	    m_status = GETHTTPHEAD;
    
    int needRecv = MAXRECVLEN - m_iHasRecvLen;
    if (needRecv <= 0)//头太大了 
    {   
        WriteRunInfo::WriteLog("The httphead is too large");
	    return -1;
    }

    int iReced = recv(m_pTcpSocket->GetSocket(), m_newRecvBuf + m_iHasRecvLen,  
		    needRecv, MSG_DONTWAIT );
    if (iReced == 0)
    {
	    WriteRunInfo::WriteTrace("the httpclient close the socket %d", m_pTcpSocket->GetSocket());
	    return -1;
    }
    else if (iReced > 0) //正常
    {
	    m_iHasRecvLen += iReced;
	    char *pTmp = strstr(m_newRecvBuf, "\r\n\r\n");
	    if (pTmp != NULL) //头结束标志
	    {	    	
		    m_status = GETHTTPBODY;
		    m_sHttpHead = string(m_newRecvBuf, pTmp - m_newRecvBuf + 4);

			WriteRunInfo::WriteTrace("%s", m_sHttpHead.c_str());
			
			string sOut;
			int nTmp = CToolKit::GetStringByLabel(m_sHttpHead, " ", " ", sOut, 0, 0);
			if (nTmp == 0)
			{
				m_nResponseNum = atoi(sOut.c_str());
			}
			
			nTmp= CToolKit::GetStringByLabel(m_sHttpHead, "Connection:",  
				"\r\n", sOut, 0, 0);
			if (nTmp == 0)
			{
				if (sOut.find("close") != string::npos)
				{
					m_bIfClose = true;
				}
			}
		    //查询下是否有content-len
		    
		    nTmp = CToolKit::GetStringByLabel(m_sHttpHead, "Content-Length:", "\r\n",
				    sOut, 0, 0);
		    if (nTmp == 0) //no chuncked
		    {
			    m_nContentLen = strtoull(sOut.c_str(), NULL, 10);
			    m_iHasRecvLen = m_iHasRecvLen - (pTmp - m_newRecvBuf + 4);
                if (m_iHasRecvLen > 0)
                {
                    m_pDataProcess->ProcessRecvBodyData(pTmp+4,  m_iHasRecvLen);
                }
				if (m_iHasRecvLen >= m_nContentLen)
				{
					WriteRunInfo::WriteTrace("Recv finish");
					if (m_iHasRecvLen > m_nContentLen)
					{
						WriteRunInfo::WriteError("CHttpTask<T>::GetHttpHead : recv too long, m_nContentLen = %d, m_iHasRecvLen = %d", m_nContentLen, m_iHasRecvLen);
					}
					nRet = (m_bIfClose)?-1:1;
				}
		    }
		    else
		    {
			    //chunked
			    string sConStr;
			    int nRet=CToolKit::GetStringByLabel(m_sHttpHead, "Transfer-Encoding:", "\r\n", sConStr);
			    if (nRet != -1)
			    {
				    CToolKit::StringTrim(sConStr);
				    if (sConStr == "chunked")
				    {
					    m_nIfChunked=1;
				    }
			    }
			    m_iHasRecvLen = 0;
		    }
	    }
    }
    else // < 0
    {
	    if ( errno != EINTR && errno != EAGAIN )
	    {
	        WriteRunInfo::WriteLog("socket error , errnor is %d", errno);
		    return -1;
	    }
	    return 0;
    }
	return nRet;
}

template<class T>
int CHttpTask<T>::GetChunked(const char *pBuf, int nLen)
{
    int ret=0;
    m_sResChunkedBody.append(pBuf, nLen);
    while (1)
    {
        if (m_nCurChunkedLen == -1)
        {   
            string sTmp;
            int nRet= CToolKit::GetStringByLabel(m_sResChunkedBody, "", "\r\n", sTmp, 0, 1);            
            if (nRet != -1)
            {
                m_nCurChunkedLen=strtoul(sTmp.c_str(), 0, 16);
                m_nCurChunkedRecLen = nRet;
                if(m_nCurChunkedLen == 0)
                {
                	ret = (m_bIfClose)?-1:1;              
                    break;
                }
                
                if (m_nCurChunkedLen + 2 + m_nCurChunkedRecLen  > (int)m_sResChunkedBody.length())
                {
                    //m_sResPage.append(m_sResChunkedBody.substr(m_nCurChunkedRecLen));
                    break;
                }
                else
                {
                    m_pDataProcess->ProcessRecvBodyData(m_sResChunkedBody.substr(m_nCurChunkedRecLen, m_nCurChunkedLen).c_str(), m_nCurChunkedLen);
                    //m_nCurChunkedRecLen += m_nCurChunkedLen + 2;                    
                    m_sResChunkedBody = m_sResChunkedBody.substr(m_nCurChunkedRecLen+2+m_nCurChunkedLen);
                    m_nCurChunkedLen = -1;
                    if (0 == m_sResChunkedBody.length())
                        break;
                }
            }
            else
            {
               // WriteRunInfo::WriteLog("m_nCurChunkedRecLen is %d, m_sResChunkedBody.length is %d", m_nCurChunkedRecLen, m_sResChunkedBody.length());
                break;
            }
        }
        else //已经获取长度但是没有获取数据
        {          
            if (m_nCurChunkedLen+2+m_nCurChunkedRecLen > (int)m_sResChunkedBody.length())
            {
                //m_sResPage.append(m_sResChunkedBody.substr(m_nCurChunkedRecLen));
                break;
            }
            else
            {
                m_pDataProcess->ProcessRecvBodyData(m_sResChunkedBody.substr(m_nCurChunkedRecLen, m_nCurChunkedLen).c_str(), m_nCurChunkedLen);
                //m_nCurChunkedRecLen += m_nCurChunkedLen + 2;
                m_sResChunkedBody = m_sResChunkedBody.substr(m_nCurChunkedLen+m_nCurChunkedRecLen+2);
                m_nCurChunkedLen = -1;
                if ( 0 == m_sResChunkedBody.length())
                        break;
            }
        }
    }
    return ret;
}
    
#endif
  
