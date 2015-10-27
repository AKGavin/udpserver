/****************************************************************************
**
**  Copyright  (c)  2008-2009,  Baofeng, Inc.  All Rights Reserved.
**
**  Subsystem:    ConnectServer
**  File:         	     AsyncQueue.h
**  Created By:   fwx
**  Created On:   2009/01/08
**
**  Purpose:
**     AsyncQueue
**
**  History:
**  Programmer      Date         Version        Number      Description
**  --------------- --------   ---------- ------------------------------
**    fwx       2009/01/08         1.0            01          creation
****************************************************************************/

#ifndef _ASYNCQUEUE_H_H_
#define _ASYNCQUEUE_H_H_

#include <queue>

#include "SyncWrapper.h"

namespace MYFRAMECOMMON{
	
template<typename T>
class CAsyncQueue
{
public:
	CAsyncQueue( void )
	{
	}

	~CAsyncQueue( void )
	{
	}

	void  Put( const T &t )
	 {   
	 	CMHelper csHelper( m_mutex );
		m_queue.push( t );
	}

	bool Get( T &t )
	{   
	 	CMHelper csHelper( m_mutex );
		if ( m_queue.size() <= 0 ) return false;
		t = m_queue.front();
		m_queue.pop();
		return true;
	}
	
private:
	CMutexWrapper     m_mutex;
	std::queue<T>   m_queue;
};
}
#endif //_ASYNCQUEUE_H_H_
