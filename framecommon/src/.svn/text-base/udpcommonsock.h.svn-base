/*
* =======================================================================
* Copyright (c) 2009-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: udpcommonsock.h
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
#ifndef _UDPCOMMONSOCKET_H_00E122BB_31F3_400d_A507_609BC54F017C_
#define _UDPCOMMONSOCKET_H_00E122BB_31F3_400d_A507_609BC54F017C_

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <iostream>
#include <string>
using namespace std;

namespace MYFRAMECOMMON{

const int MAX_UDP_RECV_PACKET_LEN = 2048;
const int MAX_UDP_SEND_PACKET_LEN = 2048;

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

class CUdpCommonSock
{
public:
	~CUdpCommonSock(){}
public:
	CUdpCommonSock(bool bIfNoBlock = false,bool bIfReuse = false);
	CUdpCommonSock(string strServerIp, unsigned short usPort, bool bIfNoBlock = false, bool bIfReuse = false);
	int CreateUdpSocket();
	int CloseSocket();
	int UdpRecv();
	int UdpRecv(int iSocket, char RecvBuf[], int iRecvBufLen, struct sockaddr_in &saRecvFrom);
	int UdpSend(char SendBuf[], int iSendBufLen, unsigned int uiDstIp, unsigned short usDstPort);
	int UdpSend(int iSocket, char SendBuf[], int iSendBufLen, unsigned int uiDstIp, unsigned short usDstPort);

	string GetServerIp();
	unsigned short GetServerPort();
	struct sockaddr_in & GetServerAddr();
	struct sockaddr_in & GetCurSrcAddr();
	int GetSocket();
	char * GetRecvBuf();
	int GetRecvPacketLen();
protected:
	int SetNonBlocking(bool bIfNoBlock = true);
	int SetRecvAndSendBuf(int iRecvBufLen = 102400, int iSendBufLen = 102400);
	int SetReusable(bool bIfReusable = true);
protected:
	int m_iUdpSocket;
	bool m_bIfNoBlock;
	bool m_bIfReuse;	

	struct sockaddr_in m_saServer;
	string m_strServerIp;
	unsigned short m_usServerPort;

	struct sockaddr_in m_saCurRecvFrom;
	char m_RecvBuf[MAX_UDP_RECV_PACKET_LEN];
	int m_iRecvPacketLen;
};
}
#endif


