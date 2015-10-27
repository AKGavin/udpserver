#ifndef _CMYSQL_H_20070824
#define _CMYSQL_H_20070824

#include <stdio.h>
#include <mysql.h>
#include <string>
#include <map>
#include <sstream>
using namespace std;
namespace MYFRAMECOMMON{

class CMysqlException
{
public:
        CMysqlException(string sErrorMsg, string sSql="") 
	{
                m_str = sErrorMsg;
                m_sSql = sSql;
        }

        const char* GetErrMsg() 
	{	
		return m_str.c_str(); 
	};
        const string &GetSqlStr()
        {
                return m_sSql;
        }
public:
        string m_str;
        string m_sSql;
};

class CMysql
{
public:

	CMysql();
	CMysql(const string& host, const string& user, const string& pass);
	~CMysql();

	void GetDBConf(const string& host, const string& user, const string& pass);
	int Close();
	int Init(); //call this BEFORE Connect!
	int Connect(const string& host, const string& user, const string& pass); //MUST call Init before call this!
	int Connect();
	bool IfConnected(const string& host);

	int Query(const string& sqlString);
	int StoreResult();
	int FreeResult();
	char** FetchRow();
	const char* GetFieldName(int iField);
	char* GetField(unsigned int iField);
	char* GetField(int iField);
	char* GetField(const string& fieldName);
	int GetAffectedRows();
	unsigned int GetAffectedCols() {return m_iField;};
	MYSQL* GetConnectHandle() { return &m_connection; }

	int EscapeString (string &str);

	unsigned int GetLastInsertId();
	unsigned int  m_iRows;
	int  SetOptions(enum mysql_option option, const char *arg); //call this after Init and before Connect!
	stringstream &GetSqlStream()
	{
		return m_sqlStream;
	}
private:

	unsigned int  m_iField;
	MYSQL m_connection;
	MYSQL_RES *m_result;
	MYSQL_ROW m_row;
	map<string, int> m_FieldIndex;
	bool m_bFieldIndexInitialized;
	bool m_bConnected;

	char m_ErrMsg[1024];

	std::string m_userKey;
	std::string m_hostKey;
	std::string m_passKey;

	std::string m_user;
	std::string m_host;
	std::string m_pass;

	stringstream m_sqlStream;  

	int InitFieldName();
};
}

#endif

