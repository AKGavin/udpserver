/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2006-11-21
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#include "snappage.h"
#include "toolkit.h"
#include "csha1.h"
#include <sys/types.h>
#include <utime.h>
namespace MYFRAMECOMMON{

CSnapPage::CSnapPage(string &sUrl)
{
        m_sUrl = sUrl;
        m_nContentLen = 0;
        m_sSetHeader = "";
}

CSnapPage::~CSnapPage()
{
        
}

void CSnapPage::SetUrl(string &sUrl)
{
        m_sUrl = sUrl;
        m_nContentLen = 0;
        m_sSetHeader = "";
}

void CSnapPage::SetRange(int nBegin, int nEnd, int nContentLen)
{
        char sHeader[256] = {0};
        if (nBegin != -1)
        {
                if (nEnd != -1)
                {
                        //sprintf(sHeader, "Range: bytes=%d-%d\r\n", nBegin, nEnd);
						snprintf(sHeader, sizeof(sHeader), "Range: bytes=%d-%d\r\n", nBegin, nEnd);
                }
                else
                {
                        //sprintf(sHeader, "Range: bytes=%d-\r\n", nBegin);
						snprintf(sHeader, sizeof(sHeader), "Range: bytes=%d-\r\n", nBegin);
                }
        }
        m_nContentLen = nContentLen;
        m_sSetHeader += string(sHeader);
}

int CSnapPage::Snap(string sOutFile)
{
        //初始化
        m_socket.Init();

        string sHosts;
        int ret = CToolKit::ParseUrl(m_sUrl, m_sHosts, m_nPort, m_sUrlFile, sHosts);
        if (ret != 0)
        {
                WriteRunInfo::WriteLog("Parse url fail %s", m_sUrl.c_str());
                return -1;
        }
        if (m_nPort == -1)
                m_nPort = HTTPPORT;

        if (sOutFile == "")
        {
                if (m_sUrlFile == "/")
                {
                        m_sFileName = "index.html";
                }
                else
                {
                        string::size_type nPos =  m_sUrlFile.rfind("/");
                        if (nPos != string::npos)
                        {
                                m_sFileName = m_sUrlFile.substr(nPos+1);
                        }
                        else
                        {
                                m_sFileName = "index.html";
                        }
                }
        }
        else
        {
                m_sFileName = sOutFile;
        }               
        CHttpDataProcess *pData = new CHttpDataProcess(m_sFileName);
        pData->Init();
        
        m_socket.SetDataProcess(pData);
        m_socket.SetHosts(sHosts);


        ret = m_socket.Connect(m_sHosts, m_nPort);
        if (ret != 0)
        {
                WriteRunInfo::WriteLog("Connect server fail %s:%d", m_sHosts.c_str(), m_nPort);
                return -1;
        }
        //长度, 以及http头设置
        m_socket.SetContentLen(m_nContentLen);
        m_socket.SetHeader(m_sSetHeader);

        ret = m_socket.SendGetRequest(m_sUrlFile);
        if (ret != 0)
        {
                WriteRunInfo::WriteLog("Send request fail");
                return -1;
        }
        
        ret =  m_socket.RecvResponse();
        if (ret < 0)
        {
                WriteRunInfo::WriteLog("Recv response fail");
                return -1;
        }          
        
        if (ret == 0)
        {
                if (m_nContentLen != 0) //其他
                {
                        m_socket.Close();
                }

                if (m_socket.GetResponseNumber() == "302")
                {
                        string sLabel1 = "Location: ";
                        string sLabel2 = "\r\n";
                        CToolKit::GetStringByLabel(m_socket.GetResHead(), sLabel1, sLabel2, m_sRedirectUrl);
                        return SNAPREDIRECT;
                }        
        }
        return ret;
}

int CSnapPage::GetCid(string &sCid)
{
/*
        m_nContentLen = DATABLOCKLENGTH;
        int retValue = -1;
        int ret = Snap();
        string sSnapData;
        if (ret == 0 && m_socket.GetResponseNumber() != "404")
        {                       
                retValue = 0;
                sSnapData = GetPage().substr(0, DATABLOCKLENGTH);                
                int nRealLen = GetHttpSocket().GetRealContentLen();
                m_nPageRealSize = nRealLen;
                
                int nCenterPos = nRealLen/3;
                //初始化
                GetHttpSocket().Close();               
                SetUrl(m_sUrl);
                SetRange(nCenterPos, nCenterPos + DATABLOCKLENGTH -1, DATABLOCKLENGTH);                
                ret = Snap();

                if (ret != 0)
                {                      
                        WriteRunInfo::WriteLog("snap fail");
                        return -1;
                }            
               WriteRunInfo::WriteLog("m_socket.GetResponseNumber()  is %s\n", m_socket.GetResponseNumber().c_str());
                if (m_socket.GetResponseNumber() == "503")  //防止多线程下载的机制
                {
                        WriteRunInfo::WriteLog("Retry snap");
                        GetHttpSocket().Close();
                        sleep(1); //休眠一秒钟, 继续抓取                                
                        ret = Snap();
                        if (ret != 0)
                        {
                                return -1;
                        }
                        
                        if (m_socket.GetResponseNumber() == "503") 
                        {
                                return -1;
                        }
                }
               
                sSnapData += GetPage();  

                //初始化
                GetHttpSocket().Close();               
                SetUrl(m_sUrl);
                
                int nEndPos = nRealLen - DATABLOCKLENGTH;
                SetRange(nEndPos, nRealLen -1, DATABLOCKLENGTH);                      
                ret = Snap();
                if (ret != 0)
                {
                        WriteRunInfo::WriteLog("Snap fail");
                        return -1;        
                }
                
                if (m_socket.GetResponseNumber() == "503")  //防止多线程下载的机制
                {
                        WriteRunInfo::WriteLog("Retry snap");
                        GetHttpSocket().Close();
                        sleep(1); //休眠一秒钟, 继续抓取                                
                        ret = Snap();
                        if (ret != 0)
                        {
                                return -1;
                        }
                        
                        if (m_socket.GetResponseNumber() == "503") 
                        {
                                return -1;
                        }
                }
                        
                sSnapData += GetPage();               

                unsigned char cid[20];
                ctx_sha1 hash;
                hash.update((unsigned char*)sSnapData.c_str(), sSnapData.length());
                hash.finish(cid); 

                char sTmpCid[50] = {0};
                char *pEnd = sTmpCid;
                for (int i=0; i < 20; i++)
                {
                        //pEnd += sprintf(pEnd, "%02X", cid[i]);
                        pEnd += snprintf(pEnd, sizeof(sTmpCid) - (pEnd - sTmpCid) , "%02X", cid[i]);
                }
                sCid = string(sTmpCid);
                WriteRunInfo::WriteLog("snapdata len is %d\n", sSnapData.length());
        }
        return retValue;
        */
	return 0;
}

int CSnapPage::DumpPage()
{
/*
        FILE *fp = fopen(m_sFileName.c_str(), "w");
        if (fp == NULL)
        {
                return 0;
        }        
        fwrite(GetPage().c_str(), GetPage().length(), 1, fp);        
        fclose(fp);
        utimbuf tmpBuf;
        tmpBuf.actime = time(NULL);
        tmpBuf.modtime = m_socket.GetPageLastModifyTime();
        utime(m_sFileName.c_str(), &tmpBuf);
        return 0;
        */
	return 0;
}

CHttpSocket<CHttpDataProcess>& CSnapPage::GetHttpSocket()
{
        return m_socket;
}

int CSnapPage::GetPageRealSize()
{
        return m_nPageRealSize;
}
}
