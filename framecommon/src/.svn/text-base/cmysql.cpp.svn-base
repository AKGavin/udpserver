#include "cmysql.h"
#include "writeRunInfo.h"
using namespace std;

namespace MYFRAMECOMMON{

CMysql:: CMysql()
{
	m_bConnected = false;
	m_iField = 0;
	m_result = NULL;
	m_bFieldIndexInitialized = false;
}


CMysql::CMysql(const string& host, const string& user, const string& pass)
{
	m_bConnected = false;
	m_iField = 0;
	m_result = NULL;
	m_bFieldIndexInitialized = false;

	m_host = host;
	m_user = user;
	m_pass = pass;
}

CMysql:: ~CMysql()
{
        Close();
        
}

int CMysql:: Close()
{
	if (m_bConnected)
	{
		FreeResult();
		mysql_close(&m_connection);
		m_bConnected= false;
	}
	return 0;
}

int CMysql:: EscapeString (string &str)
{
	if (str.size()==0) return 0;

	char *buff= new char[str.size()*2+1];
	int nLen = mysql_real_escape_string (&m_connection,buff, str.c_str(), str.size());
	str= string(buff, nLen);
	delete[] buff;
	return 0;
}


int CMysql::Init()
{
	Close();
	mysql_init(&m_connection);
	return 0;
}

int CMysql::Connect(const string& host, const string& user, const string& pass)
{
	m_host = host;
	m_user = user;
	m_pass = pass;
	if (host == m_hostKey)
	{
		return Connect();
	}
	//Close();
	return Connect();
}

int CMysql::Connect()
{
	if (!m_bConnected)
	{
		//mysql_init(&m_connection);

		if (mysql_real_connect(&m_connection, m_host.c_str(), m_user.c_str(), m_pass.c_str(), NULL, 0, NULL, CLIENT_MULTI_STATEMENTS) == NULL)
		{
			//sprintf(m_ErrMsg, "connect[-h%s -u%s -p%s] fail.\nError %u (%s)\n", m_host.c_str(), m_user.c_str(), m_pass.c_str(), mysql_errno(&m_connection), mysql_error(&m_connection));
			snprintf(m_ErrMsg, sizeof(m_ErrMsg), "connect[-h%s -u%s -p%s] fail.\nError %u (%s)\n", m_host.c_str(), m_user.c_str(), m_pass.c_str(), mysql_errno(&m_connection), mysql_error(&m_connection));
			//logMsg( m_ErrMsg);
		              throw CMysqlException(mysql_error(&m_connection));
		}
		m_bConnected = true;
	}
	return 0;
}

bool CMysql::IfConnected(const string& host)
{
	if (m_bConnected)
	{
		if (m_host == host)
		{
			return true;
		}
	}
	return false;
}

int CMysql::Query(const string& sqlString)
{
	Connect();
       WRITE_TRACE("sql is %s", sqlString.c_str());
	if (mysql_real_query(&m_connection, sqlString.c_str(), sqlString.length()) != 0)
	{
		//logMsg( "mysql query fail [%s].\nError=[%s]\nSQL=%s", m_host.c_str(),  mysql_error(&m_connection), sqlString.c_str());
		throw CMysqlException(mysql_error(&m_connection), m_sqlStream.str());
	}
	return 0;
}

int CMysql::FreeResult()
{
	if (m_result != NULL)
		mysql_free_result (m_result);
	m_iField = 0;
	m_result = NULL;
	if (m_bFieldIndexInitialized)
	{
		m_FieldIndex.erase(m_FieldIndex.begin(), m_FieldIndex.end());
		m_bFieldIndexInitialized = false;
	}
	return 0;
}

int CMysql::StoreResult()
{
	FreeResult();
	m_result = mysql_store_result (&m_connection);
	if (m_result == NULL)
	{
		//sprintf(m_ErrMsg, "保存结果出错:%s!", mysql_error(&m_connection));
		snprintf(m_ErrMsg, sizeof(m_ErrMsg), "保存结果出错:%s!", mysql_error(&m_connection));
		//logMsg(m_ErrMsg);
		throw CMysqlException(mysql_error(&m_connection));
	}
	m_iField = mysql_num_fields (m_result);
	m_iRows = mysql_num_rows (m_result);
	return 0;
}

char** CMysql:: FetchRow()
{
	if (m_result == NULL)
		StoreResult();
	m_row = mysql_fetch_row (m_result);
	return m_row;
}

int CMysql:: InitFieldName()
{
	if ((!m_bFieldIndexInitialized) && (m_result!=NULL))
	{
		unsigned int i;
		MYSQL_FIELD *fields;

		fields = mysql_fetch_fields(m_result);
		for(i = 0; i < m_iField; i++)
		{
			m_FieldIndex[fields[i].name] = i;
		}
		m_bFieldIndexInitialized = true;
	}
	return 0;
}

const char* CMysql:: GetFieldName(int iField)
{
	if (m_result==NULL) 
	{
		return NULL;
	}
	MYSQL_FIELD *fields;
	fields = mysql_fetch_fields(m_result);
	if ((unsigned int)iField> m_iField) 
	{
		return NULL;
	}
	return fields[iField].name;
}

/*
	返回结果的行数
*/
int CMysql:: GetAffectedRows()
{
	my_ulonglong iNumRows;

	if (!m_bConnected) return 0;
	iNumRows = mysql_affected_rows(&m_connection);

	return (int)iNumRows;
}

/* 返回最近一次insert的ID
*/
unsigned int CMysql:: GetLastInsertId ()
{
	return (unsigned int)mysql_insert_id(&m_connection);
}

/*
	按照字段名取回当前行的结果
*/
char* CMysql:: GetField(const string& fieldName)
{
	InitFieldName();
	return GetField(m_FieldIndex[fieldName]);
}

/*
	按照字段索引取回当前行的结果
*/
char* CMysql:: GetField(unsigned int iField)
{
	if (iField > m_iField)
		return NULL;
	return m_row[iField];
}

char* CMysql::GetField(int iField)
{
	return GetField((unsigned int)iField);
}

int  CMysql::SetOptions(enum mysql_option option, const char *arg)
{
    return mysql_options(&m_connection, option, arg);
}
}

