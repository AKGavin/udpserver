/****************************************************************************
**
**  Copyright  (c)  2008-2009,  Baofeng, Inc.  All Rights Reserved.
**
**  Subsystem:    ConnectServer
**  File:         	     CommonTask.cpp
**  Created By:   fwx
**  Created On:   2009/01/08
**
**  Purpose:
**     CommonTask
**
**  History:
**  Programmer      Date         Version        Number      Description
**  --------------- --------   ---------- ------------------------------
**    fwx       2009/01/08         1.0            01          creation
****************************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include "CommonTask.h"
#define LONGCONNECT_RECVSNED_NUM 20
std::list<TTaskConnectAttrib> CCommonTask::m_listTaskConnectAttrib;
void CCommonTask::AddTaskConnectAttrib( unsigned char ucMachineType , unsigned short usMachineIndex , int iConnectState )
{
	std::list<TTaskConnectAttrib>::iterator iterA;
	for ( iterA = m_listTaskConnectAttrib.begin() ; iterA != m_listTaskConnectAttrib.end() ; ++iterA )
	{
		if ( iterA->ucMachineType == ucMachineType && iterA->usMachineIndex == usMachineIndex )
			break;		
	}
	if ( iterA == m_listTaskConnectAttrib.end() )
	{
		TTaskConnectAttrib TaskConnectAttrib;
		TaskConnectAttrib.ucMachineType = ucMachineType;
		TaskConnectAttrib.usMachineIndex = usMachineIndex;
		TaskConnectAttrib.iConnectState = iConnectState;
		m_listTaskConnectAttrib.push_back( TaskConnectAttrib );
	}
	else
	{
		iterA->iConnectState = iConnectState;
	}	
}

int CCommonTask::CheckConnectState( unsigned char ucMachineType , unsigned short usMachineIndex )
{
	std::list<TTaskConnectAttrib>::iterator iterA;
	for ( iterA = m_listTaskConnectAttrib.begin() ; iterA != m_listTaskConnectAttrib.end() ; ++iterA )
	{
		if ( iterA->ucMachineType == ucMachineType && iterA->usMachineIndex == usMachineIndex )
			break;		
	}
	if( iterA != m_listTaskConnectAttrib.end() )
	{
		if ( iterA->iConnectState == CONNECT_ATTRIBUTE_ESTABLISH )
			return 1;
		else
			return 0;
	}
	else return 0;
}

CCommonTask::CCommonTask( CTcpSocket *pSocket, CNetProcessThread *pWorkThread, unsigned char ucMachineType, unsigned short usMachineIndex, bool bActive, int iListType )
	: CRecvAndSendTask( pSocket, pWorkThread, iListType )
	, m_uReceivePos( 0 )	
	, m_uReceiveSize( 0 )
	, m_ucDestMachineType( ucMachineType )
	, m_usDestMachineIndex( usMachineIndex )
	, m_bActive( bActive )
	, m_state( ReadPacketHead )
{
	ResetRecvBuffer( 0, sizeof( SERVERCOMMANDPACKET ) );
    m_nTaskTimeOut = -1; //长连接， 由底层进行维护,不需要进行删除
	AddTaskConnectAttrib( ucMachineType , usMachineIndex , CONNECT_ATTRIBUTE_ESTABLISH );
}

CCommonTask::~CCommonTask( void )
{
	AddTaskConnectAttrib( m_ucDestMachineType , m_usDestMachineIndex , CONNECT_ATTRIBUTE_CLOSE );
	if ( m_bActive )
	{
		WriteRunInfo::WriteLog( "connect close %d %d succ", ( int )m_ucDestMachineType, ( int )m_usDestMachineIndex );
		SendConnectMsg();		
	}
}

int CCommonTask::Init()
{
	int ikeepAlive = 1; // 开启keepalive属性
	int ikeepIdle = 60; // 如该连接在60秒内没有任何数据往来,则进行探测 
	int ikeepInterval = 5; // 探测时发包的时间间隔为5 秒
	int ikeepCount = 3; // 探测尝试的次数.如果第1次探测包就收到响应了,则后2次的不再发.

	setsockopt( GetSocket(), SOL_SOCKET, SO_KEEPALIVE, ( void * )&ikeepAlive, sizeof( ikeepAlive ) );
	setsockopt( GetSocket(), SOL_TCP, TCP_KEEPIDLE, ( void* )&ikeepIdle, sizeof( ikeepIdle ) );
	setsockopt( GetSocket(), SOL_TCP, TCP_KEEPINTVL, ( void * )&ikeepInterval, sizeof( ikeepInterval ) );
	setsockopt( GetSocket(), SOL_TCP, TCP_KEEPCNT, ( void * )&ikeepCount, sizeof( ikeepCount ) );
	
	AddToEpoll();
	return 0;
}

int CCommonTask::DestroySelf( void )
{
	delete this;
	return 0;
}

int CCommonTask::RecvProcess( void )
{
	unsigned int uMaxReceive;
	int iReced;

    int ii=0;
    while (ii < LONGCONNECT_RECVSNED_NUM)
    {
    	uMaxReceive = m_uReceiveSize - m_uReceivePos;
    	iReced = recv( GetSocket(), &m_vReceiveBuffer[0] + m_uReceivePos,  uMaxReceive, MSG_DONTWAIT );
    	if ( iReced == 0 )
    	{
    	    WriteRunInfo::WriteLog("client close the socket, %d", GetSocket());
    		return -1;
    	}
    	
    	if ( iReced < 0 )
    	{
    		if ( errno != EINTR && errno != EAGAIN )
    		{
    			return -1;
    		}
    		return 0;
    	}
    	
    	m_uReceivePos += iReced;
    	
    	switch( m_state ) 
    	{
    	case ReadPacketHead:
    		{
    			if ( !PacketFinished() ) break;
    			PSERVERCOMMANDPACKET pServerCommandPacket = ( PSERVERCOMMANDPACKET )&m_vReceiveBuffer[0];
    			int iNeedRecvLen = ( int )ntohl( pServerCommandPacket->iMsgLen );
    			if ( iNeedRecvLen > ( 400 * 1024 * 1024 ) || iNeedRecvLen <= 0 )
    			{
    				return -1;
    			}
    			m_state = ReadPacket;
    			ResetRecvBuffer( sizeof( SERVERCOMMANDPACKET ), iNeedRecvLen + sizeof( SERVERCOMMANDPACKET ) );
    		}
    		break;
    	case ReadPacket:
    		{
    			if ( !PacketFinished() ) break;

    			ParseOnePacket();

    			m_state = ReadPacketHead;
    			ResetRecvBuffer( 0, sizeof( SERVERCOMMANDPACKET ) );
    		}
    		break;
    	}
        ii++;
    }
	return 0;
}

int CCommonTask::SendProcess( void )
{
	int retValue = 0;
    int ii=0;
    while (ii < LONGCONNECT_RECVSNED_NUM)
    {
    	if (m_pCurSendMsg != NULL)
    	{
    	        retValue = SendData();
    	}
    	else
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
    	            retValue = 1;
    	        }
    	}
        if (retValue == 2 || retValue == -1)
             break;
        ii++;
    }

	return retValue;
}

int CCommonTask::PutMsgToSendList( const char *pBuf, int iLen )
{
	if ( !pBuf )
	{
		return -1;
	}
	if ( ( int )sizeof( SERVERCOMMANDPACKET ) >= iLen )
	{
		return -1;
	}
	
	PSERVERINNERCOMMANDPACKET pServerInnerCommandPacket = ( PSERVERINNERCOMMANDPACKET )pBuf;
	if ( pServerInnerCommandPacket->ucDestMachineType != m_ucDestMachineType ||
	      pServerInnerCommandPacket->usDestMachineIndex != m_usDestMachineIndex )
	{
		return -1;
	}

	WRITE_TRACE( "PutMsgToSendList %d %d", m_ucDestMachineType, m_usDestMachineIndex );
	int n = sizeof( pServerInnerCommandPacket->ucDestMachineType ) + sizeof( pServerInnerCommandPacket->usDestMachineIndex );
	return SendToList( ( ( char *)pServerInnerCommandPacket ) + n, iLen - n );
}

int CCommonTask::ProcessPacket( PSERVERCOMMANDPACKET pServerCommandPacket )
{
	if ( !pServerCommandPacket )
	{
		return -1;
	}
	
	TMsg tMsg;
	tMsg.destEntityType = RECVANDSENDMSGENTITY;
	tMsg.destEntityId = ( int )ntohl( pServerCommandPacket->iDestProcessId );
	tMsg.srcEntityId = 0;
	tMsg.srcEntityType= LONGCONNECTEDENTITY;
	tMsg.srcTaskId = GetTaskId();
	tMsg.taskId = ( unsigned long long )ntohll( pServerCommandPacket->ullDestTaskId );
	tMsg.msgType = CLIENTMSGTYPE;
	
	int iLength = ( int )ntohl( pServerCommandPacket->iMsgLen);
	char *pTmpBuf = new char[iLength];
	memcpy( pTmpBuf , pServerCommandPacket->serverCommandPacketContent , iLength );

	CMsgBody * pMsgBody = new CClientMsgBody( pTmpBuf , iLength );
	
	CMsg *pMsg = new CMsg( tMsg, pMsgBody );
	
	CMyserver::GetInstance()->PutMsgToThread( pMsg );

	return 0;
}

void CCommonTask::ResetRecvBuffer( unsigned int uReceivePos, unsigned int uReceiveSize )
{
	m_uReceivePos = uReceivePos;
	m_uReceiveSize = uReceiveSize;
	if ( ( unsigned int )m_vReceiveBuffer.size() < m_uReceiveSize )
	{
		m_vReceiveBuffer.resize( m_uReceiveSize );
	}
}

int CCommonTask::ParseOnePacket( void )
{
	PSERVERCOMMANDPACKET pServerCommandPacket = ( PSERVERCOMMANDPACKET )&m_vReceiveBuffer[0];
	if ( !pServerCommandPacket )
	{
		return -1;
	}

	return ProcessPacket( pServerCommandPacket );
}

int CCommonTask::SendConnectMsg( void )
{
	struct sockaddr_in &tmpSockAddr = GetTcpSocket()->GetAddr();
	std::string sRemoteIp = ::inet_ntoa( tmpSockAddr.sin_addr );
	unsigned short usRemotePort = ::htons( tmpSockAddr.sin_port );
	
	TMsg tMsg;
	tMsg.destEntityType = LONGCONNECTEDENTITY;
	tMsg.destEntityId = 0;
	tMsg.srcEntityId = 0;
	tMsg.srcEntityType= LONGCONNECTEDENTITY;
	tMsg.srcTaskId = 0;
	tMsg.taskId = 0;
	tMsg.msgType = TOCONNECTMSGTYPE;

	CMsgBody * pMsgBody = new CToConnectMsgBody( sRemoteIp.c_str(), usRemotePort, m_ucDestMachineType, m_usDestMachineIndex );
	if ( !pMsgBody ) 
	{
		return -1;
	}

	CMsg *pMsg = new CMsg( tMsg, pMsgBody );
	if ( !pMsg )
	{
		delete pMsgBody;
		return -1;
	}
	CMyserver::GetInstance()->PutMsgToThread( pMsg );
	return 0;
}

int CCommonTask::AddToEpoll( void )
{          
        int tmpEvent = MYPOLLERR | MYPOLLHUP | MYPOLLIN;                         
       if (m_sendList.GetNum() != 0 || m_pCurSendMsg != NULL)
        {
                tmpEvent = tmpEvent|MYPOLLOUT;
        }
       
        CNetProcessThread *pNetThread = (CNetProcessThread*)m_pThread;
        CEpoll *pEoll = pNetThread->GetEpoll();
        pEoll->AddToEpoll(this, tmpEvent);
        return 0;
}

int CCommonTask::DelFromEpoll( void )
{
        CNetProcessThread *pNetThread = (CNetProcessThread*)m_pThread;
        CEpoll *pEoll = pNetThread->GetEpoll();
        pEoll->DelFromEpoll( this );
        return 0;
}

int CCommonTask::SendToList(const char *pBuf, int nLen)
{        
        if (pBuf == NULL || nLen == 0)
        {       
                return -1;
        }               
        char *pTmpBuf = new char[nLen];
        memcpy(pTmpBuf, pBuf, nLen);
        CSendMsg *pSendMsg = new CSendMsg(nLen, pTmpBuf);
        
        m_sendList.push_back(pSendMsg);
        if (m_sendList.GetNum() == 1) //只有刚刚变化时才修改状态
        {
            int tmpEvent = MYPOLLERR|MYPOLLHUP|MYPOLLIN | MYPOLLOUT;
            CNetProcessThread *pNetThread = (CNetProcessThread*)m_pThread;
            CEpoll *pEoll = pNetThread->GetEpoll();
            pEoll->ModifyFromEpoll(this, tmpEvent);
        }
        return 0;
}

