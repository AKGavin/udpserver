/****************************************************************************
**
**  Copyright  (c)  2008-2009,  Baofeng, Inc.  All Rights Reserved.
**
**  Subsystem:    ConnectServer
**  File:         	     SyncWrapper.h
**  Created By:   fwx
**  Created On:   2009/01/08
**
**  Purpose:
**     SyncWrapper
**
**  History:
**  Programmer      Date         Version        Number      Description
**  --------------- --------   ---------- ------------------------------
**    fwx       2009/01/08         1.0            01          creation
****************************************************************************/
#ifndef _SYNC_WRAPPER_H_H_
#define _SYNC_WRAPPER_H_H_


#include "SyncObjectBase.h"

namespace MYFRAMECOMMON{
	
class CMutexWrapper
	: public CSyncObjectBase
{
public:
	CMutexWrapper( void )
	{
		::pthread_mutex_init( &m_mutex, NULL );
	}

	~CMutexWrapper( void )
	{
		::pthread_mutex_destroy( &m_mutex );
	}

	void Lock( void )
	{
		::pthread_mutex_lock( &m_mutex );
	}

	void Unlock( void )
	{
		::pthread_mutex_unlock( &m_mutex );
	}

private:
	pthread_mutex_t m_mutex;
};

template<typename T>
class CSyncHelper
{
public:
	CSyncHelper( T & t, bool InitiallyLocked = true )
		: t_( t )
		, lock_( false )
	{
		if ( InitiallyLocked )
		{
			Lock();
		}
	}

	~CSyncHelper( void )
	{
		Unlock();
	}

	void Lock( void )
	{
		if ( lock_ )
		{
			return;
		}

		t_.Lock();
		lock_ = true;
	}

	void Unlock( void )
	{
		if ( !lock_ )
		{
			return;
		}

		t_.Unlock();
		lock_ = false;
	}
private:
	T & t_;
	bool lock_;	
};

typedef CSyncHelper<CMutexWrapper> CMHelper;
}
#endif //_SYNC_WRAPPER_H_H_

