/*
 * tdmserverlist.cpp
 *
 *  Created on: Dec 12, 2013
 *      Author: root
 */

#include "tdmserverlist.h"
#include "tdmconstants.h"
#include <stdlib.h>
#include <arpa/inet.h>

int CTDMServerList::selectCount = 0;


CTDMServerList::CTDMServerList( list<host_t*>& hostList )
{
	if ( hostList.size() > 0 ) {
//		pthread_mutex_lock( &m_mutex );
		list<host_t*>::iterator iter;
		host_t* host;
		server_info_t* server;
		for ( iter = hostList.begin(); iter != hostList.end(); iter++ ) {
			host = *iter;
			server = (server_info_t*) malloc( sizeof(server_info_t) );
			server->m_sServerAddr = new string( host->sServerAddr );
			server->m_nIp = (int) inet_addr( host->sServerAddr );
			server->m_nPort = host->nPort;
			server->m_nStatus = SERVER_STATUS_OK;
			m_serverList.push_back( server );
		}
//		pthread_mutex_unlock( &m_mutex );
	}

	m_nLoadBalanceType = LB_ROTATION;
	selectCount = 0;
}


//TODO: how to release container class ?
CTDMServerList::~CTDMServerList()
{
	if ( m_serverList.size() > 0 ) {
		list<server_info_t*>::iterator iter;
		server_info_t* server;
		for ( iter = m_serverList.begin(); iter != m_serverList.end(); iter++ ) {
			server = *iter;
			free( server->m_sServerAddr );
			free( server );
		}

		m_serverList.clear();
	}
}


void CTDMServerList::changeServerStatus( string serverAddr, int port, server_status_t status )
{
	if ( m_serverList.size() > 0 ) {
		list<server_info_t*>::iterator iter;
		server_info_t* server;
		for ( iter = m_serverList.begin(); iter != m_serverList.end(); iter++ ) {
			server = *iter;
			if ( serverAddr.compare(*server->m_sServerAddr) == 0 && port == server->m_nPort ) {
				server->m_nStatus = status;
			}
		}
	}
}


void CTDMServerList::changeServerStatusByIp( int ip, int port, server_status_t status )
{
	if ( m_serverList.size() > 0 ) {
		list<server_info_t*>::iterator iter;
		server_info_t* server;
		for ( iter = m_serverList.begin(); iter != m_serverList.end(); iter++ ) {
			server = *iter;
			if ( ip == server->m_nIp && port == server->m_nPort ) {
				server->m_nStatus = status;
			}
		}
	}
}


server_info_t* CTDMServerList::GetNextServerByHash( char* str )
{
	int count = GetServerCount( true );
	if ( count == 0 ) return NULL;

	unsigned int hash = BKDRHash( str );
	int index = hash % count;
	return GetValidServerAtIndex( index );
}



server_info_t* CTDMServerList::GetNextServerByRotation()
{
	int count = GetServerCount( true );
	if ( count == 0 ) return NULL;

	int index = selectCount % count;
	selectCount++;
	return GetValidServerAtIndex( index );
}


server_info_t* CTDMServerList::GetValidServerAtIndex( int index )
{
	if ( index < 0 || index >= (int)m_serverList.size() ) return NULL;

	list<server_info_t*>::iterator iter;
	server_info_t* server;
	int i = 0;
	for ( iter = m_serverList.begin(); iter != m_serverList.end(); iter++ ) {
		server = *iter;
		if ( server->m_nStatus == SERVER_STATUS_OK ) {
			if ( i == index ) {
				return server;
			}
			else {
				i++;
			}
		}
	}
	return NULL;
}


int CTDMServerList::GetServerCount( bool onlyValid )
{
	list<server_info_t*>::iterator iter;
	server_info_t* server;
	int count = 0;
	for ( iter = m_serverList.begin(); iter != m_serverList.end(); iter++ ) {
		server = *iter;
		if ( onlyValid ) {
			if ( server->m_nStatus == SERVER_STATUS_OK ) count++;
		}
		else {
			count++;
		}
	}
	return count;
}


void CTDMServerList::GetServers( list<server_info_t*>& hostList, bool onlyValid )
{
	list<server_info_t*>::iterator iter;
	server_info_t* server;
	for ( iter = m_serverList.begin(); iter != m_serverList.end(); iter++ ) {
		server = *iter;
		if ( onlyValid ) {
			if ( server->m_nStatus == SERVER_STATUS_OK ) {
				hostList.push_back( server );
			}
		}
		else {
			hostList.push_back( server );
		}
	}
}


server_info_t* CTDMServerList::GetServer( int ip, int port )
{
	list<server_info_t*>::iterator iter;
	server_info_t* server;
	for ( iter = m_serverList.begin(); iter != m_serverList.end(); iter++ ) {
		server = *iter;
		if ( server->m_nIp == ip && server->m_nPort == port ) {
			return server;
		}
	}

	return NULL;
}


void CTDMServerList::AddServer( server_info_t* server )
{
	pthread_mutex_lock( &m_mutex );

	server_info_t* s = GetServer( server->m_nIp, server->m_nPort );
	if ( s == NULL ) {
		m_serverList.push_back( server );
	}
	pthread_mutex_unlock( &m_mutex );

}

