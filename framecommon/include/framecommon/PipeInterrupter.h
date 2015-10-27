/****************************************************************************
**
**  Copyright  (c)  2008-2009,  Baofeng, Inc.  All Rights Reserved.
**
**  Subsystem:    ConnectServer
**  File:         	     PipeInterrupt.h
**  Created By:   fwx
**  Created On:   2009/01/08
**
**  Purpose:
**     Interrupt epoll_wait or select etc.....
**
**  History:
**  Programmer      Date         Version        Number      Description
**  --------------- --------   ---------- ------------------------------
**    fwx       2009/01/08         1.0            01          creation
****************************************************************************/
#ifndef _PIPEINTERRUPT_H_H_
#define _PIPEINTERRUPT_H_H_

#include <unistd.h>

namespace MYFRAMECOMMON{
	
class CPipeInterrupter
{
public:
	CPipeInterrupter( void )
	{
		int aPipeFds[2];
		if ( pipe( aPipeFds ) == 0 )
		{
			m_iReadDescriptor = aPipeFds[0];
			::fcntl( m_iReadDescriptor, F_SETFL, O_NONBLOCK );
			m_iWriteDescriptor = aPipeFds[1];
			::fcntl( m_iWriteDescriptor, F_SETFL, O_NONBLOCK );
		}
	}

	~CPipeInterrupter( void )
	{
		if ( m_iReadDescriptor != -1 )
		{
			 ::close( m_iReadDescriptor );
		}
		if (m_iWriteDescriptor != -1)
		{
			 ::close( m_iWriteDescriptor );
		}
	}

	void Interrupt( void )
	{
		char cByte = 0;
		::write( m_iWriteDescriptor, &cByte, 1 );
	}

	bool Reset( void )
	{
		char szData[1024];
		int iBytesRead = ::read( m_iReadDescriptor, szData, sizeof( szData ) );
		bool bInterrupted = ( iBytesRead > 0 );
		while ( iBytesRead == sizeof( szData ) )
		  iBytesRead = ::read( m_iReadDescriptor, szData, sizeof( szData ) );
		return bInterrupted;
	}

	int GetReadDescriptor( void ) const
	{
		return m_iReadDescriptor;
	}

private:
  int m_iReadDescriptor;
  int m_iWriteDescriptor;
};
}
#endif //_PIPEINTERRUPT_H_H_

