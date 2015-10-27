/****************************************************************************
**
**  Copyright  (c)  2008-2009,  Baofeng, Inc.  All Rights Reserved.
**
**  Subsystem:    ConnectServer
**  File:         	     EpollReactor.cpp
**  Created By:   fwx
**  Created On:   2009/01/08
**
**  Purpose:
**     connect to remote computer
**
**  History:
**  Programmer      Date         Version        Number      Description
**  --------------- --------   ---------- ------------------------------
**    fwx       2009/01/08         1.0            01          creation
****************************************************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include "framecommon/framecommon.h"
#include "myserver.h"
#include "commmsg.h"
#include "EpollReactor.h"
using namespace MYFRAMECOMMON;

#define MAXCLIENT 128
#define   SCAN_TIME   1000 
#define   STACK_SIZE ( 102400 * 20 )  

CEpollReactor::CEpollReactor( void )
	: m_iEpFd( -1 )
	, m_iInterruptSocket( -1 )
	, m_bIsStop( false )
	, m_bThreadOk( false )
{
}

CEpollReactor::~CEpollReactor( void )
{	
	StopService();

	CConnectItem *pConnectItem = NULL;
	bool bRet;
	while ( ( bRet = m_QConnectItem.Get( pConnectItem ) ) )
	{
		if ( pConnectItem )
		{
			delete pConnectItem;
		}
	}

	ConnectItemIter it = m_mAddedConnectItems.begin();
	while ( it != m_mAddedConnectItems.end() )
	{
		CConnectItem * pConnectItem = ( *it ).second;
		if ( pConnectItem )
		{
			delete pConnectItem;
		}
		m_mAddedConnectItems.erase( it ++ );
	}
	
	if ( m_iEpFd > 0 )
	{
		close( m_iEpFd );
	}
	
	if ( m_iThreadId >= 0 )
	{
		pthread_detach( m_iThreadId );
	}
		
}

int CEpollReactor::StartService( void )
{
	if ( -1 == m_iInterruptSocket )
	{
		m_iInterruptSocket = socket(  AF_INET, SOCK_STREAM, 0 );
	}
	
	if ( -1 == m_iInterruptSocket )
	{
		return 0;
	}

	if ( -1 == SetNoBlock( m_iInterruptSocket ) )
	{
		return 0;
	}
			
	if ( m_iEpFd <= 0 )
	{
		m_iEpFd = epoll_create( MAXCLIENT );
	}
	if ( m_iEpFd <= 0 )
	{
		return 0;
	}
	else
	{
		epoll_event ev = { 0, { 0 } };
		ev.events = EPOLLIN;
		ev.data.fd = m_interrupter.GetReadDescriptor();
		epoll_ctl(m_iEpFd, EPOLL_CTL_ADD, m_interrupter.GetReadDescriptor(), &ev);
	}

	pthread_attr_t attr;
	pthread_attr_init( &attr );
	size_t stackSize = STACK_SIZE;
	pthread_attr_setstacksize( &attr, stackSize );	
	
	if ( !m_bThreadOk )
	{
		if( pthread_create( &m_iThreadId , &attr, CEpollReactor::CallRunWorkerThread , this ) != 0 )
		{
			return 0;
		}
		m_bThreadOk = true;
	}
	return 1;
}

void CEpollReactor::StopService( void )
{
	if ( m_bIsStop ) return;
	m_bIsStop = true;

	m_interrupter.Interrupt();
	
	if ( m_iThreadId >= 0 )
	{
		pthread_join( m_iThreadId, NULL );
	}

}

int CEpollReactor::AsyncConnect( const char * pcszRemoteIp, unsigned short usRemotePort, int iEntityType, 
								    int iEntityId, unsigned long long ullExpireTime, unsigned char ucMachineType, unsigned short usMachineIndex )
{
	int iState, iSocketId;
	
	do
	{
		if ( !pcszRemoteIp )
		{
			iState = ERROR_INVALID_PARAM;
			break;
		}

		iSocketId = socket( AF_INET, SOCK_STREAM, 0 );
		if ( -1 == iSocketId )
		{
			iState = ERROR_CREATE_SOCKET;
			break;
		}

		if ( -1 == SetNoBlock( iSocketId ) )
		{
			iState = ERROR_SET_NOBLOCK_SOCKET;
			break;
		}
		
		unsigned long saddr;
		if( inet_pton( AF_INET , pcszRemoteIp , &saddr ) <= 0 )
		{
			iState = ERROR_CONVERSION_IP;
			break;
		}

		sockaddr_in servaddr;
		memset( &servaddr , 0 , sizeof( servaddr ) );
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = saddr;
		servaddr.sin_port = htons( usRemotePort );

		int iRet = connect( iSocketId , ( struct sockaddr* )&servaddr , sizeof( servaddr ) );
		if ( 0 == iRet )
		{
			iState = ERROR_SUCCESS;
		}
		else
		{
			CConnectItem *pConnectItem = new CConnectItem( iSocketId, pcszRemoteIp, usRemotePort, 
												    iEntityType, iEntityId, ullExpireTime, 
												    ucMachineType, usMachineIndex );
			if ( pConnectItem )
			{
				if ( EINPROGRESS == errno )
				{
					pConnectItem->SetStateSocketInEpoll( STATE_NEED_ADD_TO_EPOLL );
				}
				else
				{
					pConnectItem->SetLastError( ERROR_NOEINPROGRESSANDNOEWOULDBLOCK );
				}
				m_QConnectItem.Put( pConnectItem );
				return ERROR_SUCCESS;
			}
			else
			{
				iState = ERROR_OUTOFMEMORY;
			}

		}

	}while( 0 );

	return SendMsg( iSocketId, pcszRemoteIp, usRemotePort, ucMachineType, usMachineIndex, iState, iEntityType, iEntityId );

}

void * CEpollReactor::CallRunWorkerThread( void *pArg )
{
	CEpollReactor* pThis = ( CEpollReactor * )pArg;
	if ( pThis )
	{
		pThis->RunWorkThread();
	}
	
	return NULL;
}

void CEpollReactor::RunWorkThread( void )
{
	while( !m_bIsStop )
	{
		epoll_event events[MAXCLIENT];
	
		AddToEpoll();
		int iFds = epoll_wait( m_iEpFd, events, MAXCLIENT, SCAN_TIME );
		if ( iFds < 0 )
		{
			if( EINTR == errno ) continue;
			WriteRunInfo::WriteLog( "epoll_wait return -1 error %d", errno );
		}
		DispatchExpiredOperations();
		DispatchOperations( iFds, events );
	}
}

void CEpollReactor::AddToEpoll( void )
{
	bool bRet;	
	CConnectItem *pConnectItem = NULL;
	int iState = ERROR_SUCCESS;
	
	while ( ( bRet = m_QConnectItem.Get( pConnectItem ) ) )
	{
		if ( pConnectItem )
		{
			if ( GetTime( 0 ) >= pConnectItem->GetExpireTime() )
			{
				int iLastState = pConnectItem->GetLastError();
				if ( ERROR_SUCCESS == iLastState )
				{
					iState = ERROR_OPERATION_ABORT;
				}
				else
				{
					iState = iLastState;
				}
				pConnectItem->SetLastError( iState );
			}
			else
			{
				if ( STATE_NEED_ADD_TO_EPOLL == pConnectItem->GetStateSocketInEpoll() )
				{
					ConnectItemPair ret = m_mAddedConnectItems.insert( std::make_pair( pConnectItem->GetSocketId(),  pConnectItem ) );
		
					if ( !ret.second )
					{
						iState = ERROR_INSERT_MAP;
						pConnectItem->SetLastError( iState );
					}
					else
					{
						epoll_event ev = { 0, { 0 } };
						ev.data.fd = pConnectItem->GetSocketId();
						ev.events = EPOLLOUT;
						epoll_ctl( m_iEpFd, EPOLL_CTL_ADD, pConnectItem->GetSocketId(), &ev );
						pConnectItem->SetStateSocketInEpoll( STATE_ADDED_TO_EPOLL );
					}
				}
			}
		}
	}

}

void CEpollReactor::DispatchExpiredOperations( void )
{
	ConnectItemIter it = m_mAddedConnectItems.begin();
	while ( it != m_mAddedConnectItems.end() )
	{
		CConnectItem *pConnectItem = ( *it ).second;
		if ( pConnectItem )
		{
			if ( GetTime( 0 ) >= pConnectItem->GetExpireTime() )
			{
				if ( STATE_ADDED_TO_EPOLL == pConnectItem->GetStateSocketInEpoll() )
				{
					struct epoll_event ev = { 0, { 0 } };
					ev.data.fd = pConnectItem->GetSocketId();			
					epoll_ctl( m_iEpFd, EPOLL_CTL_DEL, pConnectItem->GetSocketId(), &ev );
					pConnectItem->SetStateSocketInEpoll( STATE_DELED_FROM_EPOLL );
				}

				int iState;
				int iLastState = pConnectItem->GetLastError();
				if (  iLastState == ERROR_SUCCESS )
				{
					iState = ERROR_OPERATION_ABORT;
				}
				else
				{
					iState = iLastState;
				}
				
				if ( ERROR_SUCCESS == SendMsg( pConnectItem->GetSocketId(),  pConnectItem->GetRemoteIp(), pConnectItem->GetRemotePort(), 
					      		    pConnectItem->GetDestMachineType(), pConnectItem->GetDestMachineIndex(),
							    iState, pConnectItem->GetDestEntityType(),  pConnectItem->GetDestEntityId() ) )
				{
					delete pConnectItem;
					m_mAddedConnectItems.erase( it ++ );
					continue;
				}
			}

		}
		else
		{
			m_mAddedConnectItems.erase( it ++ );	
			continue;
		}
		it ++;
	}

}

void CEpollReactor::DispatchOperations( int iFds, epoll_event *events )
{
	for ( int iAI = 0; iAI < iFds; iAI ++ )
	{
		int iSocketId = events[iAI].data.fd;
		if ( iSocketId == m_interrupter.GetReadDescriptor() )
		{
			m_interrupter.Reset();
		}
		else
		{
			int iState;
			if ( events[iAI].events & EPOLLOUT )
			{
			   	socklen_t len = 1;
				socklen_t error = 1;
				if ( getsockopt( iSocketId, SOL_SOCKET , SO_ERROR , &error , &len ) < 0 )
				{
					iState = ERROR_CONNECT;
				}
				else
				{
					if ( error )
					{
						iState = ERROR_CONNECT;
					}
					else
					{
						iState = ERROR_SUCCESS;					
					}
				}
			}
			else
			{
				iState = ERROR_CONNECT;
			}

			DispatchOperation( iSocketId, iState );	
		}

	}
	
}

void CEpollReactor::DispatchOperation( int iSocketId, int iState )
{
	ConnectItemIter it = m_mAddedConnectItems.find( iSocketId );
	if ( it != m_mAddedConnectItems.end() )
	{		
		CConnectItem * pConnectItem = ( *it ).second;
		if ( pConnectItem )
		{
			pConnectItem->SetLastError( iState );
			if ( STATE_ADDED_TO_EPOLL == pConnectItem->GetStateSocketInEpoll() )
			{
				epoll_event ev = { 0, { 0 } };
				epoll_ctl( m_iEpFd, EPOLL_CTL_DEL, iSocketId, &ev );
				pConnectItem->SetStateSocketInEpoll( STATE_DELED_FROM_EPOLL );
			}
			if ( ERROR_SUCCESS == iState )
			{
				if ( ERROR_SUCCESS == SendMsg( pConnectItem->GetSocketId(),  pConnectItem->GetRemoteIp(), pConnectItem->GetRemotePort(), 
											    pConnectItem->GetDestMachineType(), pConnectItem->GetDestMachineIndex(),
						   	    			 	    iState, pConnectItem->GetDestEntityType(), pConnectItem->GetDestEntityId() ) )
				{
					m_mAddedConnectItems.erase( it );
					delete pConnectItem;
				}
			}
		}
		else
		{
			m_mAddedConnectItems.erase( it );
		}
	}
	else
	{
		epoll_event ev = { 0, { 0 } };
		epoll_ctl( m_iEpFd, EPOLL_CTL_DEL, iSocketId, &ev );
	}
}
		
int CEpollReactor::SendMsg( int iSocketId, const char * pcszRemoteIp, unsigned short usRemotePort, 
						     unsigned char ucMachineType, unsigned short usMachineIndex,
						     int iState, int iDestEntityType, int iDestEntityId )
{
        TMsg tMsg;
        tMsg.destEntityType = iDestEntityType;
        tMsg.destEntityId = iDestEntityId;
        tMsg.srcEntityId = 0;
        tMsg.srcEntityType= EPOLLREACTORENTITY;
        tMsg.srcTaskId = 0;
        tMsg.taskId = 0;
        tMsg.msgType = CONNECTMSGTYPE;
	 CMsgBody * pMsgBody = new CConnectMsgBody( iSocketId, pcszRemoteIp, usRemotePort, ucMachineType, usMachineIndex, iState );
	 if ( !pMsgBody ) 
	 {
	 	return ERROR_OUTOFMEMORY;
	 }

	CMsg *pMsg = new CMsg(tMsg, pMsgBody);
	if ( !pMsg )
	{
		delete pMsgBody;
		return ERROR_OUTOFMEMORY;
	}
	
        CMyserver::GetInstance()->PutMsgToThread( pMsg );
	return ERROR_SUCCESS;
}













