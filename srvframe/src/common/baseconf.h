/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2006-5-27
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef  __BASE_CONF_H
#define __BASE_CONF_H
#include "framecommon/framecommon.h"
using namespace MYFRAMECOMMON;

#define BASECONFIGFILE  "conf/base.conf"

struct TConfigItem
{
        string sServerAddr;

        string sMySqlHost;
	    string sMySqlUser;
	   	string sMySqlPass;
	    string sMySqlDb;
	    string sMySqlTb;

        int  nTcpPort;
        int  nUdpPort;
        int  nTcpThreadNum;
        int nServiceThreadNum;
        string sMaintainIp;
        int nMaintainPort;
        int nMaxFdSize;
        int nRecvTaskType;
        int nSocketTimeout;
        int nListenThreadNum;
        int nIfPostHttp;
};

class CBaseConf
{
public:
        CBaseConf()
        {
        }
        ~CBaseConf()
        {
        }
        int Init();
        TConfigItem& GetConfigItem()
        {
                return m_configItem;
        }
private:
        TConfigItem m_configItem;        
};
#endif
