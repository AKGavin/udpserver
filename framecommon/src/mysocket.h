#ifndef __SEND_SOCKET_H
#define __SEND_SOCKET_H
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <cstring>
#include "writeRunInfo.h"
using namespace std;
namespace MYFRAMECOMMON{
	int SetNonBlocking(int nSock);

class CMySocket
{
public:
        CMySocket(sockaddr_in &toAddr, socklen_t toLen,   int iSocket)
        {
                memcpy(&m_toAddr, &toAddr, sizeof(sockaddr_in));
                m_toLen = toLen;
                m_iSocket = iSocket;
        }

        CMySocket(const string &sAddr, short sPort, int iSocket)
        {
                sockaddr_in tmpAddr;
                memset((char *) &tmpAddr, 0, sizeof(tmpAddr));
                tmpAddr.sin_family = AF_INET;
                tmpAddr.sin_port = htons(sPort);               
                inet_aton(sAddr.c_str(), &tmpAddr.sin_addr);
                memcpy(&m_toAddr, &tmpAddr, sizeof(sockaddr_in));
                m_iSocket = iSocket;
                m_toLen = sizeof(sockaddr_in);
        }

        CMySocket()
        {
			memset(&m_toAddr, 0, sizeof(sockaddr_in));
			m_iSocket = -1;
        }

        virtual ~CMySocket()
        {
        }
        virtual int Size()
        {
                return 4 + sizeof(sockaddr_in) + sizeof(socklen_t);
        }
        virtual int Send(char *pSendBuf, int iSendLen) = 0;
        virtual int Receive(char *pReceiveBuf, int iRecvLen, int timeout, int &nResult, const string &sEndStr="")=0;

        static int CreateSock(const int socket_type,  const string &sAddr, const unsigned short port, int &iSocket);
        static int CreateNoListenSock(const int socket_type,  int &iSocket);
        int  SetNonBlocking();
        int GetSocket()
        {
                return m_iSocket;
        }

        sockaddr_in &GetAddr()
        {
                return m_toAddr;
        }
		
        int SetSockOpt(int  level,  int  optname,  const  void  *optval,
                socklen_t optlen);
protected:
        int m_iSocket;                
        sockaddr_in m_toAddr;
        socklen_t m_toLen;
};

class CTcpSocket:public CMySocket
{
public:
        CTcpSocket(sockaddr_in &toAddr, socklen_t toLen,   int iSocket):CMySocket(toAddr,  toLen,  iSocket)
        {
                AddNum(1);
        }

        CTcpSocket(const string &sAddr, short sPort, int iSocket):CMySocket(sAddr, sPort, iSocket)
        {
                AddNum(1);
        }
		
        CTcpSocket()
        {
                m_iSocket = -1;
                AddNum(1);
        }

        int Connect(const string &sAddr, short nPort);                
        int Connect(sockaddr_in &toAddr); 
        int Connect(const string &sAddr, short nPort, int nTimeOut);  
        int Close()
        {
                if (m_iSocket != -1)
                {
                        close(m_iSocket);
                        m_iSocket = -1;
                }
                return 0;
        }

        ~CTcpSocket() //关闭句柄
        {
                Close();
                AddNum(-1);
        }
        
        int Send(char *pSendBuf, int iSendLen);        
        

        int SendRealBuf(char *pSendBuf, int iSendLen);        

        int Receive(char *pReceiveBuf, int iRecvLen, int timeout, int &nResult, const string &sEndStr="");
        int Send(char *pSendBuf, int iSendLen, int timeout, int &nResult);
  
        static int GetNum()
        {
                return m_snNum;
        }
private:
        static void AddNum(int nNum);
        static CThreadMutex m_sNumMutex;
        static int m_snNum;
};


class CUdpSocket:public CMySocket
{
public:
        CUdpSocket(sockaddr_in &toAddr, socklen_t toLen,   int iSocket):CMySocket(toAddr,  toLen,  iSocket)
        {
        }
        
        CUdpSocket(const string &sAddr, short sPort, int iSocket):CMySocket(sAddr, sPort, iSocket)
        {
        }
        
        ~CUdpSocket()
        {
        }
        int Send(char *pSendBuf, int iSendLen);                  
        int Receive(char *pReceiveBuf, int iRecvLen, int timeout, int &nResult, const string &sEndStr="");
protected:
        static  CThreadMutex m_udpMutex; //因为在服务端upd端口只有一个， 要加锁发送
};
}
#endif

