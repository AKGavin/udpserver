/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2007-7-27
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#include "filereqhttpcmd.h"
CMineType CFileReqHttpCmd::m_mineType;
string CFileReqHttpCmd::m_sFileRootDir="filedir/";
CFileReqHttpCmd::CFileReqHttpCmd(string &sInHead, string &sInBody, string &sUrl):CMyHttpCmd(sInHead, sInBody, sUrl)
{        
        m_nIfAnnexInfo = 0;
}

CFileReqHttpCmd::~CFileReqHttpCmd()
{
}

int CFileReqHttpCmd::Process()
{      
        try
        {
                if (m_sUrl.find("..") != string::npos)
                {
                        m_nResult = -1;
                        return -1;
                }               
                
                                
                string sFile = m_sFileRootDir + m_sUrl;
                CMyFile myfile(sFile);
                myfile.FOpen("r");
                struct stat *pStat = myfile.GetStatBuf();
				m_sFileModTime = CToolKit::SecToHttpTime(pStat->st_mtime);
				string sOut;
			    int ret = CToolKit::GetStringByLabel(m_sInHead, "If-Modified-Since:", "\r\n", sOut);
				if (ret == 0) //有If-Modified-Since:
				{
					time_t modTime = CToolKit::TimeToSec(sOut);
					if (pStat->st_mtime == modTime)
					{
						m_nResult = 1;
						return 0;
					}
				}
                char *pBuf = new char[pStat->st_size];
                CBufMgr bufMgr(pBuf); //自动释放
                
                myfile.FRead(pBuf, pStat->st_size);
                m_sOutBody = string(pBuf, pStat->st_size);
        }
        catch(CMyException &e)
        {
                m_nResult = -1;
                WriteRunInfo::WriteError("Error, %s", e.GetErrMsg().c_str());
        }
        return 0;
}

int CFileReqHttpCmd::Decode()
{        
        return 0;
}

int CFileReqHttpCmd::Encode()
{
        if (m_nResult < 0)
        {
            m_sResHeader =  "HTTP/1.1 404 NOT FOUND\r\n";
        }
		else if (m_nResult == 1) //304
		{
			m_sResHeader = "HTTP/1.1 304 Not Modified\r\n";
			m_sResHeader += "Last-Modified: "+ m_sFileModTime + "\r\n"; 
		}
        else //==0
        {
        	m_sResHeader += "Last-Modified:"+ m_sFileModTime + "\r\n";
            string sSuffix;
            CToolKit::GetStringByLabel(m_sUrl, ".", "", sSuffix);
            map<string, string>::iterator mapItr = m_mineType.m_minetype.find(sSuffix);
            if (mapItr != m_mineType.m_minetype.end())
            {
                m_sResHeader += "Content-Type: " + mapItr->second + "\r\n";
            }
        }
        return 0;
}

