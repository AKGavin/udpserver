/*
* =======================================================================
* Copyright (c) 2009-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: udpcommonsock.cpp
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
#include "udpcommonsock.h"
#include "writeRunInfo.h"
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

namespace MYFRAMECOMMON{
CUdpCommonSock::CUdpCommonSock(bool bIfNoBlock /* = false */,bool bIfReuse /* = false */)
{
	m_iUdpSocket = INVALID_SOCKET;
	m_strServerIp = "";
	m_usServerPort = 0;
	m_bIfNoBlock = bIfNoBlock;
	m_bIfReuse = bIfReuse;
	m_iRecvPacketLen = 0;
	memset(&m_saServer, 0, sizeof(m_saServer));
	memset(&m_saCurRecvFrom, 0 , sizeof(m_saCurRecvFrom));
}

CUdpCommonSock::CUdpCommonSock(string strServerIp, unsigned short usPort, 
							   bool bIfNoBlock /* = false */, bool bIfReuse /* = false */)
{
	m_iUdpSocket = INVALID_SOCKET;
	m_strServerIp = strServerIp;
	m_usServerPort = usPort;
	m_bIfNoBlock = bIfNoBlock;
	m_bIfReuse = bIfReuse;
	m_iRecvPacketLen = 0;
	memset(&m_saServer, 0, sizeof(m_saServer));
	memset(&m_saCurRecvFrom, 0 , sizeof(m_saCurRecvFrom));
}

int CUdpCommonSock::CreateUdpSocket()
{
	if(m_iUdpSocket != INVALID_SOCKET)
		return 0;
	memset(&m_saServer, 0, sizeof(m_saServer));
	if(m_strServerIp != "")
	{
		if(inet_pton(AF_INET, m_strServerIp.c_str(), &m_saServer.sin_addr) <= 0 )			
			return -1;
	}
	else
		m_saServer.sin_addr.s_addr = htonl(INADDR_ANY);

	m_saServer.sin_family = AF_INET;
	m_saServer.sin_port = htons(m_usServerPort);

	m_iUdpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if( m_iUdpSocket < 0 )
		return -1;

	if(SetNonBlocking(m_bIfNoBlock) || SetReusable(m_bIfReuse) || SetRecvAndSendBuf())
	{
		CloseSocket();
		return -1;
	}
	if(bind(m_iUdpSocket ,(struct sockaddr *)&m_saServer, sizeof(m_saServer)) < 0) 
	{
		CloseSocket();
		return -1;
	}
	return m_iUdpSocket;
}
int CUdpCommonSock::CloseSocket()
{
	int iRet = 0;
	if(m_iUdpSocket != INVALID_SOCKET)
	{
		iRet = close(m_iUdpSocket);
		m_iUdpSocket = INVALID_SOCKET;
	}
	return iRet;
}
int CUdpCommonSock::UdpRecv()
{
	return (m_iRecvPacketLen = UdpRecv(m_iUdpSocket, m_RecvBuf, MAX_UDP_RECV_PACKET_LEN, m_saCurRecvFrom));
}
int CUdpCommonSock::UdpRecv(int iSocket, char RecvBuf[], int iRecvBufLen, struct sockaddr_in &saRecvFrom)
{
	if(RecvBuf == NULL)
		return -1;
	socklen_t sockLen = sizeof(saRecvFrom);
	int iRecvPacketLen = recvfrom(iSocket, m_RecvBuf, iRecvBufLen, 0,
		(sockaddr *)&saRecvFrom, &sockLen);
	if(iRecvPacketLen < 0)
	{
		WriteRunInfo::WriteLog("%s:%d:%s : Udp Recv failed!", __FILE__, __LINE__, __FUNCTION__);
	}
	else
	{
		WRITE_DAT(RecvBuf, iRecvPacketLen, "////Received UDP Packet////");
	}
	return iRecvPacketLen;
}
int CUdpCommonSock::UdpSend(char SendBuf[], int iSendBufLen, unsigned int uiDstIp, unsigned short usDstPort)
{
	return UdpSend(m_iUdpSocket, SendBuf, iSendBufLen, uiDstIp, usDstPort);	
}
int CUdpCommonSock::UdpSend(int iSocket, char SendBuf[], int iSendBufLen, unsigned int uiDstIp, unsigned short usDstPort)
{
	if(iSocket == INVALID_SOCKET || iSendBufLen < 0 || SendBuf == NULL)
	{
		WriteRunInfo::WriteError( "Error : %s:%d:%s : Wrong parameter in UdpSend" , __FILE__ , __LINE__ , __FUNCTION__ );
		return -1;
	}

	sockaddr_in addrClient;
	addrClient.sin_family = AF_INET;
	addrClient.sin_addr.s_addr = htonl(uiDstIp);
	addrClient.sin_port = htons(usDstPort);
	int iSendPackLen = sendto(iSocket, SendBuf, iSendBufLen, 0, (sockaddr*)&addrClient, sizeof(addrClient));
	if (iSendPackLen < iSendBufLen)
	{                             
		WriteRunInfo::WriteError( "Error : %s:%d:%s : UdpSend failed\n" , __FILE__ , __LINE__ , __FUNCTION__ );
		return -1;
	}
	WRITE_DAT(SendBuf, iSendPackLen, "////Sended UDP Packet////");
	return iSendPackLen;
}

string CUdpCommonSock::GetServerIp()
{
	return m_strServerIp;
}
unsigned short CUdpCommonSock::GetServerPort()
{
	return m_usServerPort;
}
struct sockaddr_in & CUdpCommonSock::GetServerAddr()
{
	return m_saServer;
}
struct sockaddr_in & CUdpCommonSock::GetCurSrcAddr()
{
	return m_saCurRecvFrom;
}
int CUdpCommonSock::GetSocket()
{
	return m_iUdpSocket;
}
char * CUdpCommonSock::GetRecvBuf()
{
	return (char *)m_RecvBuf;
}
int CUdpCommonSock::GetRecvPacketLen()
{
	return m_iRecvPacketLen;
}
int CUdpCommonSock::SetNonBlocking(bool bIfNoBlock /* = true */)
{
	int opts;
	opts=fcntl(m_iUdpSocket, F_GETFL);
	if(opts < 0)
		return -1;
	if(bIfNoBlock == true)
	{
		opts |= O_NONBLOCK;
	}
	else
	{
		opts &= (~O_NONBLOCK);
	}

	if(fcntl(m_iUdpSocket,F_SETFL,opts)<0)
		return -1;
	return 0;
}
int CUdpCommonSock::SetRecvAndSendBuf(int iRecvBufLen /* = 102400 */, int iSendBufLen /* = 102400 */)
{
	int iRet = 0;
	iRet = setsockopt(m_iUdpSocket,SOL_SOCKET,SO_SNDBUF,(const  char*)&iSendBufLen,sizeof(int));
	if(iRet != 0)
		return -1;
	iRet = setsockopt(m_iUdpSocket,SOL_SOCKET,SO_RCVBUF,(const  char*)&iRecvBufLen,sizeof(int));   
	if(iRet != 0)
		return -1;
	return 0;
}
int CUdpCommonSock::SetReusable(bool bIfReusable /* = true */)
{
	int iAddrReuseFlag;
	if(bIfReusable == true)
		iAddrReuseFlag = 1;
	else iAddrReuseFlag = 0;
	int iRet = setsockopt(m_iUdpSocket , SOL_SOCKET, SO_REUSEADDR, &iAddrReuseFlag, sizeof(iAddrReuseFlag));
	if(iRet != 0)
		return -1;
	return 0;
}
}

