/****************************************************************************
**
**  Copyright  (c)  2008-2009,  Baofeng, Inc.  All Rights Reserved.
**
**  Subsystem:    VPeer
**  File:         	     MysqlWrapper.h
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
#ifndef _MYSQLWRAPPER_H_H_
#define _MYSQLWRAPPER_H_H_

#include <mysql.h>
#include <string>
#include <map>

namespace MYFRAMECOMMON
{
class CMysqlWrapper
{
public:
	CMysqlWrapper( void );
	CMysqlWrapper( const char *pszcHost, const char *pszcUsername, const char *pszcPassword, const char *pszcDatabase );
	virtual ~CMysqlWrapper( void );

	bool Connect( void );
	bool Connect( const char *pszcHost, const char *pszcUsername, const char *pszcPassword, const char *pszcDatabase );
	bool Execute( const char * pszcSql );
	const char *GetString( int i );
	const char *GetString( const char *pszcField );
	
	bool GetLength( int i, unsigned long &ulLength );
	bool GetLength( const char *pszcField, unsigned long &ulLength );
	
	bool FetchRow( void );
	int GetRowNum( void );	
	int GetColNum( void );
	unsigned int GetLastInsertId ( void );
	bool GetResult( const char * pszcSql );
	void FreeResult( void );

	void SetLastError( std::string strError );	
	std::string GetLastError( void );
	bool RowSeek( unsigned long long ullOffset = 0 );
	bool EscapeString( std::string &str );
private:
	void Close( void );
	
private:
	std::string m_strHost;
	std::string m_strUsername;
	std::string m_strPassword;
	std::string m_strDatabase;
	std::string m_strError;
	bool          m_bConnected;
		
	MYSQL 		m_mysql;
	MYSQL_RES * m_pRes;
	MYSQL_ROW   m_row;
	unsigned long *m_pLengths;

	typedef std::map<std::string, int>::iterator FileldIter;
	std::map<std::string, int> m_mField;
	int                  m_iField;
};
}
#endif //_MYSQLWRAPPER_H_H_
