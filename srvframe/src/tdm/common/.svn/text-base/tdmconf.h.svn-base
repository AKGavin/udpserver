/*
 * tdmconf.h
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#ifndef TDMCONF_H_
#define TDMCONF_H_

#include "framecommon/framecommon.h"
#include <list>
#include <map>
#include <string>
#include "tdmconstants.h"

using namespace MYFRAMECOMMON;

#define TDMCONFIGFILE  "conf/tdm.conf"

struct TDMProtoConfItem
{
	bool enabled;
	protocol_type_t proto;
	string sServerAddr;
	int nUdpPort;
	int nProcessThreadNum;

	list<host_t*> dcmServers;
	lb_type_t nDcmLbType;

	list<host_t*> dtmServers;


	~TDMProtoConfItem() {
		list<host_t*>::iterator iter;
		host_t* t;
		for ( iter = dcmServers.begin(); iter != dcmServers.end(); iter++ ) {
			t = *iter;
			free( t->sServerAddr );
			free( t );
		}

		for ( iter = dtmServers.begin(); iter != dtmServers.end(); iter++ ) {
			t = *iter;
			free( t->sServerAddr );
			free( t );
		}
	}
};


class CTDMConf
{
public:
       ~CTDMConf();


       int Init();

       TDMProtoConfItem* GetProtoConfigItem( protocol_type_t proto )
        {
    	   map<protocol_type_t, TDMProtoConfItem*>::iterator iter = m_configItem.find( proto );
    	   if ( iter != m_configItem.end() ) {
    		   return iter->second;
    	   }
    	   else {
    		   return NULL;
    	   }
        }

        map<protocol_type_t, TDMProtoConfItem*> GetConfigItemMap()
		{
    	   return m_configItem;
		}

private:
	int loadProtoConf( protocol_type_t protoType, Magic_Config* config, TDMProtoConfItem* confItem );

public:
	string m_sServerAddr;
	int m_nPort;
	int m_nProcessThreadNum;

	string m_sMaintainServerAddr;
	int m_nMaintainPort;

	string m_sMonitorServerAddr;
	int m_nMonitorPort;

	string m_sDBHost;
	string m_sDBUser;
	string m_sDBPasswd;
	string m_sDBName;
	int m_nDBPoolSize;
	int m_nDtmHeartPort;

private:
	map<protocol_type_t, TDMProtoConfItem*> m_configItem;

};




#endif /* TDMCONF_H_ */
