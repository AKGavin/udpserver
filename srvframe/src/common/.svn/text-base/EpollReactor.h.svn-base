/****************************************************************************
**
**  Copyright  (c)  2008-2009,  Baofeng, Inc.  All Rights Reserved.
**
**  Subsystem:    ConnectServer
**  File:         	     EpollReactor.h
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

#ifndef _EPOLLREACTOR_H_H_
#define _EPOLLREACTOR_H_H_

#include <string>
#include <list>
#include <sys/epoll.h>

#include "framecommon/framecommon.h"
using namespace MYFRAMECOMMON;

class CEpollReactor
{
public:
	CEpollReactor( void );
	virtual ~CEpollReactor( void );

	int StartService( void );
	void StopService( void );

	/**
	  * Returns ERROR_SUCCESS or ERROR_OUTOFMEMORY.
	  */
	int AsyncConnect( const char * pcszRemoteIp, unsigned short usRemotePort, int iEntityType, int iEntityId, 
						   unsigned long long ullExpireTime, unsigned char ucMachineType, unsigned short usMachineIndex );

	enum
	{
		ERROR_SUCCESS = 0,
		ERROR_INVALID_PARAM,
		ERROR_CREATE_SOCKET,
		ERROR_SET_NOBLOCK_SOCKET,
		ERROR_CONVERSION_IP,
		ERROR_CTL_ADD,
		ERROR_INSERT_MAP,
		ERROR_OUTOFMEMORY,
		ERROR_NOEINPROGRESSANDNOEWOULDBLOCK,
		ERROR_OPERATION_ABORT,
		ERROR_CONNECT
	};
	
private:
	static void * CallRunWorkerThread( void *pArg );
	void RunWorkThread( void );

	void AddToEpoll( void );
	
	void DispatchExpiredOperations( void );
	void DispatchOperations( int iFds, epoll_event *events );
	void DispatchOperation( int iSocketId, int iState );
	
	int SendMsg( int iSocketId, const char * pcszRemoteIp, unsigned short usRemotePort, unsigned char ucMachineType, 
				    unsigned short usMachineIndex, int iState, int iDestEntityType, int iDestEntityId );
private:

	enum STATE_SOCKET_IN_EPOLL
	{
		STATE_SOCKET_INIT = 0,
		STATE_NEED_ADD_TO_EPOLL,
		STATE_ADDED_TO_EPOLL,
		STATE_NEED_DEL_FROM_EPOLL,
		STATE_DELED_FROM_EPOLL
	};
	class CConnectItem
	{
	public:
		CConnectItem( int iSocketId, const char * pcszRemoteIp, unsigned short usRemotePort, 
						  int iDestEntityType, int iDestEntityId, ULONG64 ullExpireTime, 
						  unsigned char ucMachineType, unsigned short usMachineIndex )
			: m_iSocketId( iSocketId )
			, m_iDestEntityType( iDestEntityType )			
			, m_iDestEntityId( iDestEntityId )
			, m_ullExpireTime( ullExpireTime )
			, m_sRemoteIp( pcszRemoteIp )
			, m_usRemotePort( usRemotePort )
			, m_ucMachineType( ucMachineType )
			, m_usMachineIndex( usMachineIndex )
			, m_eStateSocketInEpoll( STATE_SOCKET_INIT )
			, m_iLastError( ERROR_SUCCESS )
		{
		}

		~CConnectItem( void )
		{
		}
		
		int GetSocketId( void )
		{
			return m_iSocketId;
		}

		int GetDestEntityType( void )
		{
			return m_iDestEntityType;
		}

		int GetDestEntityId( void )
		{
			return m_iDestEntityId;
		}
		
		ULONG64 GetExpireTime( void )
		{
			return m_ullExpireTime;
		}

		const char * GetRemoteIp( void )
		{
			return m_sRemoteIp.c_str();
		}

		unsigned short GetRemotePort( void )
		{
			return m_usRemotePort;
		}

		unsigned char GetDestMachineType( void )
		{
			return m_ucMachineType;
		}

		unsigned short GetDestMachineIndex( void )
		{
			return m_usMachineIndex;
		}

		void SetStateSocketInEpoll( STATE_SOCKET_IN_EPOLL eState )
		{
			m_eStateSocketInEpoll = eState;
		}
		
		STATE_SOCKET_IN_EPOLL GetStateSocketInEpoll( void )
		{
			return m_eStateSocketInEpoll;
		}
		
		void SetLastError( int iState )
		{
			m_iLastError = iState;
		}

		int GetLastError( void )
		{
			return m_iLastError;
		}
	private:
		int   			 			m_iSocketId;		
		int    	  	 			m_iDestEntityType;
		int    		 			m_iDestEntityId;
		ULONG64         			m_ullExpireTime;
		std::string       			m_sRemoteIp;
		unsigned short 			m_usRemotePort;
		unsigned char 			m_ucMachineType;
		unsigned short 			m_usMachineIndex;
		STATE_SOCKET_IN_EPOLL    m_eStateSocketInEpoll;
		int                  			m_iLastError;
	};
	
	int           		m_iEpFd;
	int                        m_iInterruptSocket;
	bool                      m_bIsStop;	
	pthread_t              m_iThreadId;
	CPipeInterrupter    m_interrupter;
	bool                      m_bThreadOk;
	CAsyncQueue<CConnectItem *> m_QConnectItem;

	typedef std::map<int, CConnectItem *>::iterator ConnectItemIter;
	typedef std::pair<ConnectItemIter, bool> ConnectItemPair;
	std::map<int, CConnectItem *>      m_mAddedConnectItems;
};
#endif //_EPOLLREACTOR_H_H_


































