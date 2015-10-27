/****************************************************************************
**
**  Copyright  (c)  2008-2009,  Baofeng, Inc.  All Rights Reserved.
**
**  Subsystem:    VPeer
**  File:         	     MysqlWrapper.cpp
**  Created By:   fwx
**  Created On:   2009/01/08
**
**  Purpose:
**     MysqlWrapper
**
**  History:
**  Programmer      Date         Version        Number      Description
**  --------------- --------   ---------- ------------------------------
**    fwx       2009/01/08         1.0            01          creation
****************************************************************************/
#include <cstring>
#include "MysqlWrapper.h"
namespace MYFRAMECOMMON
{

CMysqlWrapper::CMysqlWrapper( void )
	: m_bConnected( false )
	, m_pRes( NULL )
	, m_row( NULL )
	, m_pLengths( NULL )
	, m_iField( 0 )
{
}

CMysqlWrapper::CMysqlWrapper( const char *pszcHost, const char *pszcUsername, const char *pszcPassword, const char *pszcDatabase )
	: m_strHost( pszcHost )
	, m_strUsername( pszcUsername )
	, m_strPassword( pszcPassword )
	, m_strDatabase( pszcDatabase )
	, m_bConnected( false )
	, m_pRes( NULL )
	, m_row( NULL )
	, m_pLengths( NULL )
	, m_iField( 0 )
{
}

CMysqlWrapper::~CMysqlWrapper( void )
{
	Close();
}

void CMysqlWrapper::Close( void )
{
	FreeResult();
	if ( m_bConnected )
	{
		mysql_close( &m_mysql );
		m_bConnected = false;
	}
}

bool CMysqlWrapper::Connect( void )
{
	Close();
	
	if ( !m_bConnected )
	{
		if ( !mysql_init( &m_mysql ) )
		{
			SetLastError( mysql_error( &m_mysql ) );
			return false;
		}

		if ( !mysql_real_connect( &m_mysql, m_strHost.c_str(), m_strUsername.c_str(), m_strPassword.c_str(),
			m_strDatabase.c_str(), 0, NULL, 0 ) )
		{
			SetLastError( mysql_error( &m_mysql ) );
			return false;
		}

		m_bConnected = true;
	}
	return true;
}

bool CMysqlWrapper::Connect( const char *pszcHost, const char *pszcUsername, const char *pszcPassword, const char *pszcDatabase )
{
	m_strHost = pszcHost;
	m_strUsername = pszcUsername;
	m_strPassword = pszcPassword;
	m_strDatabase = pszcDatabase;
	return Connect();
}

bool CMysqlWrapper::Execute( const char * pszcSql )
{
	if ( !m_bConnected )
	{
		if ( !Connect() )
		{
			return false;
		}
	}
	else
	{
		FreeResult();
		if ( mysql_ping( &m_mysql ) )
		{
			SetLastError( mysql_error( &m_mysql ) );
			m_bConnected = false;
			if ( !Connect() )
			{
				return false;
			}
		}
	}
	
	if ( mysql_real_query( &m_mysql, pszcSql, strlen( pszcSql ) ) )
	{
		SetLastError( mysql_error( &m_mysql ) );
		return false;
	}

	return true;	
}

const char *CMysqlWrapper::GetString( int i )
{
	if ( !m_row || i >= m_iField || i < 0  )
	{
		return NULL;
	}
	return m_row[i];
}

const char *CMysqlWrapper::GetString( const char *pszcField )
{
	if ( !pszcField )
	{
		return NULL;
	}

	FileldIter it = m_mField.find( pszcField );
	if ( it == m_mField.end() )
	{
		return NULL;
	}

	int i = it->second;
	return GetString( i );
}

bool CMysqlWrapper::FetchRow( void )
{
	if ( !m_pRes )
	{
		SetLastError( "m_pRes is NULL" );
		return false;
	}
	
	m_row = mysql_fetch_row( m_pRes );
	if ( !m_row )
	{
		SetLastError( "res is end" );
		return false;
	}

	m_pLengths = mysql_fetch_lengths( m_pRes );
	return true;
}

bool CMysqlWrapper::GetLength( int i, unsigned long &ulLength )
{
	if ( !m_pLengths )
	{
		SetLastError( "m_pLengths is NULL" );
		return false;
	}

	if ( i >= m_iField || i < 0  )
	{
		SetLastError( "i error" );
		return false;
	}

	ulLength = m_pLengths[i];
	return true;
}	

bool CMysqlWrapper::GetLength( const char *pszcField, unsigned long &ulLength )
{
	if ( !pszcField )
	{
		return false;
	}

	FileldIter it = m_mField.find( pszcField );
	if ( it == m_mField.end() )
	{
		return false;
	}

	int i = it->second;
	return GetLength( i, ulLength );
}

int CMysqlWrapper::GetRowNum( void )
{
	return  m_pRes ? mysql_num_rows( m_pRes ) : 0;
}

int CMysqlWrapper::GetColNum( void )
{
	return m_iField;
}

unsigned int CMysqlWrapper::GetLastInsertId ( void )
{
	return ( unsigned int )mysql_insert_id( &m_mysql );
}

bool CMysqlWrapper::GetResult( const char * pszcSql )
{
	if ( !Execute( pszcSql ) )
	{
		return false;
	}

	m_pRes = mysql_store_result( &m_mysql );
	if ( !m_pRes )
	{
		SetLastError( "m_pRes is NULL" );
		return false;
	}
	MYSQL_FIELD * pField = mysql_fetch_field( m_pRes );
	
	int i = 0;
	while ( pField )
	{
		if ( pField->name )
		{
			m_mField[pField->name] = i;
		}
		pField = mysql_fetch_field( m_pRes );
		i ++;
	}

	m_iField = i;	
	return true;
}

void CMysqlWrapper::FreeResult( void )
{
	if ( m_pRes )
	{
		mysql_free_result( m_pRes );
		m_pRes = NULL;
		m_row = NULL;
	}
	m_mField.clear();
	m_iField = 0;
}

void CMysqlWrapper::SetLastError( std::string strError )
{
	m_strError = strError;
}

std::string CMysqlWrapper::GetLastError( void )
{
	return m_strError;
}

bool CMysqlWrapper::RowSeek( unsigned long long ullOffset/* = 0*/ )
{
	if ( !m_pRes ) 
	{
		return false;
	}
	mysql_data_seek( m_pRes, ullOffset );
	return true;
}

bool CMysqlWrapper::EscapeString( std::string &str )
{
	if ( str.length() == 0 )
	{
		return true;
	}

	char *pBuffer= new char[str.length()*2 + 1];
	if ( pBuffer )
	{
		mysql_real_escape_string (&m_mysql, pBuffer, str.c_str(), str.length() );
		str = pBuffer;
		delete[] pBuffer;	
		return true;
	}

	return false;
}

}

