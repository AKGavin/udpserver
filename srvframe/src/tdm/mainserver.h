/*
 * mainserver.h
 *
 *  Created on: Dec 9, 2013
 *      Author: root
 */

#ifndef MAINSERVER_H_
#define MAINSERVER_H_

#include "myserver.h"
#include "threadgroup.h"
#include "tdmconf.h"
#include "tdmserverlist.h"
#include "tdmmysqlwrapperpool.h"
#include "framecommon/framecommon.h"
#include <pthread.h>

class CMainServer : public CMyserver
{
public:
	CMainServer();
	~CMainServer();
	static int Init();
	int InitServer();
	int StartServer();
	int GetRecvThreadRunFlag();
	CTDMServerList* GetServerList( protocol_type_t proto );

	map<protocol_type_t, CTDMServerList*> GetDCMServersMap() {
		return m_dcmServerList;
	}

	map<protocol_type_t, CTDMServerList*> GetDTMServersMap() {
		return m_dtmServerList;
	}

	void GetDTMServerList( protocol_type_t proto, bool onlyValid, list<server_info_t*>& serverList );
	void GetDCMServerList( protocol_type_t proto, bool onlyValid, list<server_info_t*>& serverList );
	void AddDTMServer( protocol_type_t proto, server_info_t* serverInfo );
	void AddDCMServer( protocol_type_t proto, server_info_t* serverInfo );

	void SetMonitor( unsigned int ip, int port ) {
		pthread_mutex_lock( &m_monitorMutex );
		m_nMonitorIp = ip;
		m_nMonitorPort = port;
		pthread_mutex_unlock( &m_monitorMutex );
	}

	int CheckHealth();

	CMysqlWrapper* OpenConnection();
	void CloseConnection( CMysqlWrapper* mysql );

private:
	int CreateRecvThreadGroup();
	int CreateSendThreadGroup();
	static int CreateSimpleRecvThreadGroup( TDMProtoConfItem* conf );
	int CheckThreadGroupHealth( monitor_cmd_data_t* cmd, TDMProtoConfItem* confItem );
	int SendPacketToMonitor( monitor_cmd_data_t* cmd );
	int SendHeartPacketToDTM( monitor_cmd_data_t* cmd );
	void LoadWhiteList();


public:
	CTDMConf m_tdmConf;
private:
	map<protocol_type_t, CTDMServerList*> m_dcmServerList;
	map<protocol_type_t, CTDMServerList*> m_dtmServerList;

	int monitorSock;
	unsigned int m_nMonitorIp;
	int m_nMonitorPort;
	pthread_mutex_t m_monitorMutex;

	int DTMSock;
};



#endif /* MAINSERVER_H_ */
