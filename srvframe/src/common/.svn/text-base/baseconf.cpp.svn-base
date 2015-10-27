#include "baseconf.h"
#include <netdb.h>
 #include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int CBaseConf::Init()
{
	//∂¡»°≈‰÷√Œƒº˛
	Magic_Config config(string(BASECONFIGFILE));
	int ret = config.Init();
	if (ret != 0)
	{
		WriteRunInfo::WriteLog("Read config file error.");
		return -1;
	}
	string tmpStr;

	config.ReadItem("SERVER", "SERVERADDR", "",  m_configItem.sServerAddr);

	config.ReadItem("SERVER", "MYSQLHOST", "",  m_configItem.sMySqlHost);
	config.ReadItem("SERVER", "MYSQLUSER", "",  m_configItem.sMySqlUser);
	config.ReadItem("SERVER", "MYSQLPASS", "",  m_configItem.sMySqlPass);
	config.ReadItem("SERVER", "MYSQLDB", "", 	m_configItem.sMySqlDb);
	config.ReadItem("SERVER", "MYSQLTB", "", 	m_configItem.sMySqlTb);

	config.ReadItem("SERVER", "TCPPORT", "5200",  tmpStr);
	m_configItem.nTcpPort = atoi(tmpStr.c_str());
	config.ReadItem("SERVER", "UDPPORT", "6200",  tmpStr);
	m_configItem.nUdpPort = atoi(tmpStr.c_str());
	config.ReadItem("SERVER", "TCPTHREADNUM", "1",  tmpStr);
	m_configItem.nTcpThreadNum = atoi(tmpStr.c_str());
	config.ReadItem("SERVER", "SERVICETHREADNUM", "10",  tmpStr);
	m_configItem.nServiceThreadNum = atoi(tmpStr.c_str());


	config.ReadItem("SERVER", "MAINTAINIP", "",  m_configItem.sMaintainIp);
	config.ReadItem("SERVER", "MAINTAINPORT", "27500",  tmpStr);
	m_configItem.nMaintainPort = atoi(tmpStr.c_str());        


	config.ReadItem("SERVER", "MAXFDSIZE", "10000",  tmpStr);
	m_configItem.nMaxFdSize = atoi(tmpStr.c_str());

	config.ReadItem("SERVER", "RECVTYPE", "0",  tmpStr);
	m_configItem.nRecvTaskType = atoi(tmpStr.c_str());      
	config.ReadItem("SERVER", "SOCKETTIMEOUT", "120",  tmpStr);
	m_configItem.nSocketTimeout = atoi(tmpStr.c_str());      
	config.ReadItem("SERVER", "LISTENTHREADNUM", "7",  tmpStr);
	m_configItem.nListenThreadNum = atoi(tmpStr.c_str());      
	config.ReadItem("SERVER", "ISPOSTHTTPSRV", "0",  tmpStr);
	m_configItem.nIfPostHttp = atoi(tmpStr.c_str());      
	return 0;
}

