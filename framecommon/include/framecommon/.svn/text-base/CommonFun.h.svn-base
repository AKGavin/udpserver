/****************************************************************************
**
**  Copyright  (c)  2008-2009,  Baofeng, Inc.  All Rights Reserved.
**
**  Subsystem:    ConnectServer
**  File:         	     CommonFun.h
**  Created By:   fwx
**  Created On:   2009/01/08
**
**  Purpose:
**     CommonFun
**
**  History:
**  Programmer      Date         Version        Number      Description
**  --------------- --------   ---------- ------------------------------
**    fwx       2009/01/08         1.0            01          creation
****************************************************************************/
#ifndef _COMMONFUN_H_H_
#define _COMMONFUN_H_H_

#include <arpa/inet.h>

namespace MYFRAMECOMMON{
static inline unsigned long long htonll( unsigned long long n  )
{
	unsigned int a = ( unsigned int )( n & 0x00000000FFFFFFFFll );
	unsigned int b = ( unsigned int )( ( n & 0xFFFFFFFF00000000ll ) >> 32 );
	return ( unsigned long long )htonl( a ) << 32 | ( unsigned long long )htonl( b );
}

static inline unsigned long long ntohll( unsigned long long n  )
{
	unsigned int a = ( unsigned int )( n & 0x00000000FFFFFFFFll );
	unsigned int b = ( unsigned int )( ( n & 0xFFFFFFFF00000000ll ) >> 32 );
	return ( unsigned long long )ntohl( a ) << 32 | ( unsigned long long )ntohl( b );
}

static inline unsigned long long GetTime( int post )
{
	time_t ullTime;
	
	if( time( &ullTime ) == -1 ) return 0;								
	ullTime += post;
	return ullTime;
}

static inline int SetNoBlock( int s )
{
	int opts;
	opts = fcntl( s, F_GETFL );
	if( opts < 0 )
	{
		return -1;
	}
	opts = opts | O_NONBLOCK;
	opts = opts | O_NDELAY;
	if( fcntl( s, F_SETFL, opts ) < 0 )
	{
		return -1;
	}
	return 0;
}
	
}
#endif //_COMMONFUN_H_H_
