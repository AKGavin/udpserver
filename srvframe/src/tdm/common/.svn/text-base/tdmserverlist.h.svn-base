/*
 * tdmserverlist.h
 *
 *  Created on: Dec 12, 2013
 *      Author: root
 */

#ifndef TDMSERVERLIST_H_
#define TDMSERVERLIST_H_

#include "tdmconstants.h"
#include <list>

using namespace std;

class CTDMServerList
{
private:
	list<server_info_t*> m_serverList;
	lb_type_t m_nLoadBalanceType;
	pthread_mutex_t m_mutex;
	static int selectCount;

public:
	CTDMServerList( list<host_t*>& list );
	~CTDMServerList();

	void changeServerStatus( string serverAddr, int port, server_status_t status );
	void changeServerStatusByIp( int ip, int port, server_status_t status );

	server_info_t* GetNextServerByRotation();
	server_info_t* GetNextServerByHash( char* str );

	int GetServerCount( bool onlyvalid );
	void GetServers( list<server_info_t*>& hostList, bool onlyValid ) ;
	server_info_t* GetServer( int ip, int port );

	void AddServer( server_info_t* server );

private:
	server_info_t* GetValidServerAtIndex( int index );
};


#endif /* TDMSERVERLIST_H_ */
