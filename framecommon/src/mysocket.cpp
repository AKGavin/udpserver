#include <errno.h>
#include "mysocket.h"
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>

namespace MYFRAMECOMMON{

int CTcpSocket::m_snNum=0; 
CThreadMutex CTcpSocket::m_sNumMutex;

CThreadMutex CUdpSocket::m_udpMutex;

int CMySocket::CreateSock(const int socket_type,  const string &sAddr, const unsigned short port, int &iSocket)
{
        struct sockaddr_in address;
        int reuse_addr = 1;

        memset((char *) &address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        int ret = 0;

        if (sAddr != "")
        {
                inet_aton(sAddr.c_str(), &address.sin_addr);
        }
        else
        {
                address.sin_addr.s_addr = htonl(INADDR_ANY);
        }
        

        iSocket = socket(AF_INET, socket_type, 0);
        if (iSocket < 0) 
        {
                WriteRunInfo::WriteLog("Socket error");
                return -1;
        }

        setsockopt(iSocket, SOL_SOCKET, SO_REUSEADDR, (void*)(&(reuse_addr)), 
        sizeof(reuse_addr));

        if (bind(iSocket, (struct sockaddr *) &address, sizeof(address)) < 0) 
        {
                WriteRunInfo::WriteLog("Bind  port %d error, error(errno is %d) is %s", port,  errno, strerror(errno));
                close(iSocket);
                return -1;
        }
        
        if (socket_type == SOCK_STREAM) 
        {                
                ret = listen(iSocket, 250); /* Queue up to 50 connections before having them automatically rejected. */
                if (ret == -1)
                {
                        WriteRunInfo::WriteLog("listen port %d error", port);
                        return -1;
                }
        }
        return iSocket;
}

int CMySocket::CreateNoListenSock(const int socket_type,  int &iSocket)
{
        iSocket = socket(AF_INET, socket_type, 0);
        if (iSocket < 0) 
        {
                WriteRunInfo::WriteLog("Socket error");
                return -1;
        }

        return iSocket;
}

int SetNonBlocking(int nSocket)
{
     int opts;
     opts=fcntl(nSocket, F_GETFL);
     if(opts<0)
     {
          return -1;
     }
     opts = opts|O_NONBLOCK;
     opts = opts|O_NDELAY;
     if(fcntl(nSocket,F_SETFL,opts)<0)
     {
          return -1;
     }

//     //设置socket句柄的tcpcork属性
//     int on = 1;
//     setsockopt (m_iSocket, SOL_TCP, TCP_CORK, &on, sizeof (on)); 

     int  nSendBuf=100*1024;//设置为100K  
     setsockopt(nSocket,SOL_SOCKET,SO_SNDBUF,(const  char*)&nSendBuf,sizeof(int));  
	 setsockopt(nSocket,SOL_SOCKET,SO_RCVBUF,(const  char*)&nSendBuf,sizeof(int));   	
     int on = 1;
     setsockopt(nSocket, SOL_TCP, TCP_NODELAY,  &on, sizeof(on));
	 return 0;
}

int  CMySocket::SetNonBlocking()
{
	return MYFRAMECOMMON::SetNonBlocking(m_iSocket);
}

int CMySocket::SetSockOpt(int  level,  int  optname,  const  void  *optval,
                socklen_t optlen)
{
        return   setsockopt(m_iSocket, level,  optname,  optval, optlen);
}


void  CTcpSocket::AddNum(int nNum)
{
        m_sNumMutex.Lock();
        m_snNum += nNum;
        m_sNumMutex.UnLock();
}

int CTcpSocket::Send(char *pSendBuf, int iSendLen)
{
        int nResult = 0;
        Send(pSendBuf, iSendLen, 10, nResult);
        return nResult;
}

int CTcpSocket::SendRealBuf(char *pSendBuf, int iSendLen)
{
    ssize_t ret;
    size_t size = 0;
    const char *ptr;

    if(!iSendLen) return 0;
    
    ptr = (const char*)pSendBuf;
    for(;;) 
    {
        if((ret = send(m_iSocket, ptr, iSendLen, MSG_DONTWAIT)) < (ssize_t)iSendLen) {
            if(ret > 0) {
                ptr += ret;
                iSendLen -= ret;
                size += ret;
            }else {
                if ((errno == EINTR)||(errno == EAGAIN)) {
                    continue;
                }else if(errno == ECONNRESET||
                        errno == EDESTADDRREQ||
                        errno == ENOTCONN||
                        errno == EPIPE||
                    errno == ETIMEDOUT) {
                    return 0; // sender should reconnect the receiver 
                }else if(errno == EMSGSIZE) {
                    return ret;
                }
                  else 
                  {
                    return ret;
                }
            }
        }else {
            size += ret;
            return size;
        }
    }
}


int CTcpSocket::Connect(const string & sAddr, short nPort)
{        
        m_iSocket = socket(AF_INET,  SOCK_STREAM, 0);
        m_toAddr.sin_family = AF_INET;
        m_toAddr.sin_port = htons(nPort);
        inet_aton(sAddr.c_str(), &(m_toAddr.sin_addr));
        m_toLen = sizeof(sockaddr_in);
        int ret = connect(m_iSocket, (sockaddr*)&m_toAddr, m_toLen);
        if (ret == 0)
        SetNonBlocking();
        return ret;
}

int CTcpSocket::Connect(sockaddr_in &toAddr)
{
        m_iSocket = socket(toAddr.sin_family,  SOCK_STREAM, 0);
        memcpy(&m_toAddr, &toAddr, sizeof(sockaddr_in));
        m_toLen = sizeof(sockaddr_in);
        int ret = connect(m_iSocket, (sockaddr*)&m_toAddr, m_toLen);
        if (ret == 0)
        SetNonBlocking();
        return ret;
}

int CTcpSocket::Connect(const string &sAddr, short nPort, int nTimeOut)
{
	if( nTimeOut < 0 ) return -1;
	m_iSocket = socket(AF_INET,  SOCK_STREAM, 0);
	m_toAddr.sin_family = AF_INET;
	m_toAddr.sin_port = htons(nPort);
	inet_aton(sAddr.c_str(), &(m_toAddr.sin_addr));
	m_toLen = sizeof(sockaddr_in);

	SetNonBlocking();
	connect(m_iSocket, (sockaddr*)&m_toAddr, m_toLen);

	struct timeval tvSelectWait;
	tvSelectWait.tv_sec = nTimeOut;
	tvSelectWait.tv_usec = 0;

	fd_set setWrite;
	while(1)
	{
		FD_ZERO(&setWrite);
		FD_SET(m_iSocket, &setWrite);
		int nRet = 0;
		if((nRet = select(m_iSocket + 1, NULL, &setWrite , NULL, &tvSelectWait)) < 0)		// socket出错时返回-1，无事件时返回0 
		{
			if(errno == EINTR) continue;
			return -1;
		}
		else 
		{
			if(nRet == 0) return 1;				//超时 
			else return 0;						//sock可写 
		}
	}	
}

int CTcpSocket::Receive(char *pReceiveBuf, int iRecvLen, int timeout, int &nResult, const string &sEndStr)
{
        memset(pReceiveBuf, 0, iRecvLen);   
        pollfd tmppollfd[1];
        tmppollfd[0].fd = m_iSocket;
        tmppollfd[0].events = POLLERR|POLLHUP|POLLIN;
        int beginTime = time(NULL);
        int nHasRecvLen = 0;
        while (nHasRecvLen < iRecvLen)
        {
                int ret = poll(tmppollfd, 1,  100);
                if (ret > 0)
                {
                        if ((tmppollfd[0].revents & POLLERR) == POLLERR | (tmppollfd[0].revents & POLLHUP) == POLLHUP)
                        {
                                return -1;
                        }
                        
                        if ((tmppollfd[0].revents & POLLIN) == POLLIN)
                        {
                                nResult = 0;
                                ret = recv(m_iSocket, pReceiveBuf+nHasRecvLen, iRecvLen-nHasRecvLen, MSG_DONTWAIT);
                                if (ret == 0)
                                {
                                        WriteRunInfo::WriteLog("the socket has been closed :%s", strerror(errno));
                                        break;
                                }
                                nHasRecvLen += ret;
                        }
                }
                
                int nowTime = time(NULL);
                if (sEndStr != "")
                {
                    if (strstr(pReceiveBuf, sEndStr.c_str()) != NULL)
                    {
                        break;
                    }
                }
                if (nowTime - beginTime > timeout)
                {                
                        break;
                }
        }       
        return nHasRecvLen;
}

int CTcpSocket::Send(char *pSendBuf, int iSendLen, int timeout, int &nResult)
{
        pollfd tmppollfd[1];
        tmppollfd[0].fd = m_iSocket;
        tmppollfd[0].events = POLLERR|POLLHUP|POLLOUT;
        int beginTime = time(NULL);
        int nHasSendLen = 0;
        while (nHasSendLen < iSendLen)
        {
                int ret = poll(tmppollfd, 1,  100);
                if (ret > 0)
                {
                        if ((tmppollfd[0].revents & POLLERR) == POLLERR | (tmppollfd[0].revents & POLLHUP) == POLLHUP)
                        {
                                return -1;
                        }
                        
                        if ((tmppollfd[0].revents & POLLOUT) == POLLOUT)
                        {
                                nResult = 0;
                                ret = send(m_iSocket, pSendBuf+nHasSendLen, iSendLen-nHasSendLen, MSG_DONTWAIT);
                                if (ret == -1)
                                {
                                        if ((errno == EINTR)||(errno == EAGAIN)) 
                                        {
                                             continue;
                                        }
                                        else if(errno == ECONNRESET||
                                                errno == EDESTADDRREQ||
                                                errno == ENOTCONN||
                                                errno == EPIPE||
                                                errno == ETIMEDOUT) 
                                        {
                                                break;
                                        }
                                        else if(errno == EMSGSIZE) 
                                        {
                                                break;
                                        }
                                        else 
                                        {
                                                break;
                                        }
                                }
                                nHasSendLen += ret;
                        }
                }
                int nowTime = time(NULL);
                if (nowTime - beginTime > timeout)
                {
                        break;
                }
        }     
        return nHasSendLen;
}


int CUdpSocket::Send(char *pSendBuf, int iSendLen)
{
        int ret = 0;
        m_udpMutex.Lock();
        ret = sendto(m_iSocket, pSendBuf, iSendLen, 0, (sockaddr*)&m_toAddr,  m_toLen);
        m_udpMutex.UnLock();
        if (ret < iSendLen)
        {                             
                WriteRunInfo::WriteLog("udp send fail sock is %d, ret %d,m_toAddr %s:%d  m_tolen %d", m_iSocket,
                        ret, inet_ntoa(m_toAddr.sin_addr), m_toAddr.sin_port, m_toLen);
                return -1;
        }
        return ret;
}

int CUdpSocket::Receive(char *pReceiveBuf, int iRecvLen, int timeout, int &nResult, const string &sEndStr)
{
        int ret = 0;
        ret = recvfrom(m_iSocket, pReceiveBuf, iRecvLen, 0, (sockaddr*)&m_toAddr, &m_toLen);       
        return ret;
}
}


