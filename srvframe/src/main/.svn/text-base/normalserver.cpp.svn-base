/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2007-7-31
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#include "normalserver.h"
CNormalServer::CNormalServer()
{
}

CNormalServer::~CNormalServer()
{
}


int CNormalServer::PreStartServer()
{
        int ret = 0;

		//////////////////////////////////////////////////////////////////////////
		//timer thread
        m_pTimerThreadGrp = new CThreadGroup(1, TIMERENTITY);
        ret = m_pTimerThreadGrp->Init();
        if (ret != 0)
        {       
                WriteRunInfo::WriteLog("Init timer thread fail");
                return -1;
        }
        WriteRunInfo::WriteLog("Init timer thread succeed");

		//////////////////////////////////////////////////////////////////////////
		//receive and send thread
        int nTcpThreadNum =  m_baseConf.GetConfigItem().nTcpThreadNum;
        m_pRecvAndSendThreadGrp = new CThreadGroup(nTcpThreadNum, RECVANDSENDMSGENTITY);
        ret = m_pRecvAndSendThreadGrp->Init();
        if (ret != 0)
        {       
                WriteRunInfo::WriteLog("Init recvandsend thread fail");
                return -1;
        }       
        WriteRunInfo::WriteLog("Init recvandsend thread succeed");

		//////////////////////////////////////////////////////////////////////////
		//maintain thread
        m_pMaintainThreadGrp = new CThreadGroup(1, MAINTAINENTITY);
        ret = m_pMaintainThreadGrp->Init();
        if (ret != 0)
        {       
                WriteRunInfo::WriteLog("Init maintain thread fail");
                return -1;
        }       
        WriteRunInfo::WriteLog("Init maintain thread succeed");
        
      
		//////////////////////////////////////////////////////////////////////////
		//¼àÌýÏß³Ì
        int nPort = m_baseConf.GetConfigItem().nTcpPort;
        string sHost = m_baseConf.GetConfigItem().sServerAddr;
        string sTmpData = string((char*)(&nPort), 4);
        sTmpData.append(sHost);
        
        m_pListenThreadGrp = new CThreadGroup(1, LISTENENTITY, sTmpData);
        ret = m_pListenThreadGrp->Init();
        if (ret != 0)
        {       
                WriteRunInfo::WriteLog("Init listen thread fail");
                return -1;
        }
        WriteRunInfo::WriteLog("Init listen thread succeed");  

		//////////////////////////////////////////////////////////////////////////
		//
        m_pMaintainUpdThread = new CMaintainUdpThread(m_baseConf.GetConfigItem().sMaintainIp, 
			m_baseConf.GetConfigItem().nMaintainPort);
        ret = m_pMaintainUpdThread->Start();
        if (ret != 0)
        {
                WriteRunInfo::WriteLog("Init maintain udp thread fail");
                return -1;
        }        
        WriteRunInfo::WriteLog("Init maintain udp thread succeed");                        
        return 0;
}
