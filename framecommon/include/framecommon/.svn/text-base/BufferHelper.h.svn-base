/****************************************************************************
**
**  Copyright  (c)  2008-2009,  Baofeng, Inc.  All Rights Reserved.
**
**  Subsystem:    ConnectServer
**  File:         	     BufferHelper.h
**  Created By:   fwx
**  Created On:   2009/01/08
**
**  Purpose:
**     BufferHelper
**
**  History:
**  Programmer      Date         Version        Number      Description
**  --------------- --------   ---------- ------------------------------
**    fwx       2009/01/08         1.0            01          creation
****************************************************************************/
#ifndef _BUFFERHELPER_H_H_
#define _BUFFERHELPER_H_H_

#include <vector>
#include <string>

#include "CommonFun.h"

namespace MYFRAMECOMMON{
	
class CBufferHelper
{
public:
	CBufferHelper( bool bNetTransport = true )
		: m_iOffset( 0 )
		, m_bNetTransport( bNetTransport )
	{
	}

	CBufferHelper( const char *pBuffer, int iLen, bool bNetTransport = true )
		: m_iOffset( 0 )
		, m_bNetTransport( bNetTransport )		
	{
		Put( pBuffer, iLen );
	}
	virtual ~CBufferHelper( void ){}

	template<class T>
	void Put( T& t )
	{
		m_vBuf.insert( m_vBuf.end(), ( char * )&t, ( char * )&t + sizeof( t ) );		
	}

	void Put( short& t )
	{
		short n = t;
		if ( m_bNetTransport )
		{
			n = ( short )htons( t );
		}
		m_vBuf.insert( m_vBuf.end(), ( char * )&n, ( char * )&n + sizeof( t ) );	
	}

	void Put( int& t )
	{
		int n = t;
		if ( m_bNetTransport )
		{
			n = ( int )htonl( t );
		}
		m_vBuf.insert( m_vBuf.end(), ( char * )&n, ( char * )&n + sizeof( t ) );	
	}

	void Put( long long& t )
	{
		long long n = t;
		if ( m_bNetTransport )
		{
			n = ( long long )htonll( t );
		}
		m_vBuf.insert( m_vBuf.end(), ( char * )&n, ( char * )&n + sizeof( t ) );
	}
	
	void Put( std::string &sString )
	{
		int iLen = ( int )sString.length();
		Put( iLen );
		Put( sString.c_str(), sString.length() );
	}

	void Put( const char *pcszBuf )
	{
		int iStrLen = strlen( pcszBuf );
		int iLen = ( int )htonl( iStrLen );
		Put( iLen );

		Put( pcszBuf, iStrLen );
	}
		
	template<class T>
	void Put( T* pBuf, int iLen )
	{
		m_vBuf.insert( m_vBuf.end(), ( char * )pBuf, ( char * )pBuf + iLen );
	}
	
	template<class T>
	int Get( T & t )
	{
		if ( m_iOffset + ( int )sizeof( T ) > ( int )m_vBuf.size() )
		{
			return -1;
		}

		memcpy( &t, &m_vBuf[m_iOffset], sizeof( t ) );
		m_iOffset += sizeof( t );
		return 0;
	}

	int Get( short & t )
	{
		if ( m_iOffset + ( int )sizeof( t ) > ( int )m_vBuf.size() )
		{
			return -1;
		}

		if ( m_bNetTransport )
		{
			t =  ( short )ntohs( *( short * )&m_vBuf[m_iOffset] );
		}
		else
		{
			t = *( short * )&m_vBuf[m_iOffset];
		}
		m_iOffset += sizeof( t );
		return 0;
	}

	int Get( int & t )
	{
		if ( m_iOffset + ( int )sizeof( t ) > ( int )m_vBuf.size() )
		{
			return -1;
		}

		if ( m_bNetTransport )
		{
			t =  ( int )ntohl( *( int * )&m_vBuf[m_iOffset] );
		}
		else
		{
			t = *( int * )&m_vBuf[m_iOffset];
		}
		m_iOffset += sizeof( t );
		return 0;
	}
	
	int Get( long long & t )
	{
		if ( m_iOffset + ( int )sizeof( t ) > ( int )m_vBuf.size() )
		{
			return -1;
		}

		if ( m_bNetTransport )
		{
			t =  ( long long )ntohll( *( long long * )&m_vBuf[m_iOffset] );
		}
		else
		{
			t = *( long long * )&m_vBuf[m_iOffset];
		}
		m_iOffset += sizeof( t );
		return 0;
	}

	int Get( std::string &sBuf )
	{
		int iLen;
		if ( -1 == Get( iLen ) )
		{
			return -1;
		}

		return Get( sBuf, iLen );
	}

	int Get( std::string &sBuf, int iLen )
	{
		if ( m_iOffset + iLen > ( int )m_vBuf.size() || iLen < 0)
		{
		        return -1;
		}
		sBuf.append( &m_vBuf[m_iOffset], iLen );
		m_iOffset += iLen;
		return 0;
	}

	template<class T>
	int Get( T *pBuf, int iLen )
	{
		if ( m_iOffset + iLen > ( int )m_vBuf.size() || iLen < 0 )
		{
		        return -1;
		}
		memcpy( pBuf, &m_vBuf[m_iOffset], iLen );
		m_iOffset += iLen;
		return 0;
	}

	const char *GetBuffer( void )
	{
		return &m_vBuf[0];
	}

	int GetLen( void )
	{
		return ( int )m_vBuf.size();
	}

private:
	std::vector<char> m_vBuf;
	int m_iOffset;
	bool m_bNetTransport;		
};
}
#endif //_BUFFERHELPER_H_H_
