/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2007-12-6
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef __HTTP_TOOLS_H
#define __HTTP_TOOLS_H
#include "httpsocket.h"
#include <string>
#include "myfile.h"
using namespace std;
namespace MYFRAMECOMMON{
class CHttpDataProcess
{
public:
    CHttpDataProcess(void *pTools, const string &sFileName, int nTimeOut=-1)
    {
        m_sFileName = sFileName;
		m_nTimeOut = nTimeOut;
		m_pTools = pTools;
    }
    CHttpDataProcess(const string &sFileName, int nTimeOut=-1)
    {
        m_sFileName = sFileName;
		m_nTimeOut = nTimeOut;
		m_pTools = NULL;
    }
    ~CHttpDataProcess()
        {
        }

    int Init()
    {
        int nRet=0;
        m_file.SetFileName(m_sFileName);
        return nRet;
    }
    int ProcessData(const char *pBuf, int nLen, int ifHead =0)
        {        
        if (pBuf == NULL || ifHead != 0)
			return 0;
        try
            {
            if (m_file.GetFp() == NULL)
				m_file.FOpen("w");
            m_file.FWrite((void*)pBuf, nLen);
            fflush(m_file.GetFp());
            }
        catch(CMyException &e)
            {
            }
        return 0;
        }
	int GetTimeOut()
	{
		return m_nTimeOut;
	}
protected:
	void *m_pTools;
    string m_sFileName;
    CMyFile m_file;
	int m_nTimeOut;
};

/**
* dec:
* ClassName:CHttpTools
* author: 
* version:
*CreateTime: 2007.12.6
*/
template <class T>
class CHttpTools
{
private:
		typedef class CHttpSocket<T> HTTPSOCKET;		
public:	
		
        CHttpTools(const string &sUrl, int nIfResponse=1, const string &fileName="");
        ~CHttpTools();
        int SetMethod(const string &sMethod);
        int Open(string sPostStr = "");
        HTTPSOCKET &GetHttpSocket()
		{
			return m_httpSocket;
    	}
        void SetHead(const string &sHead)
        {
            m_sHead = sHead;
        }
private:
        string GetFileName(const string &sUrl);
		
        HTTPSOCKET m_httpSocket;
        string m_sUrl;
        string m_sPostStr;
        string m_sMethod;
        int m_nIfRespones;
        string m_sHead;
        string m_sFileName;
};

template<class T>
CHttpTools<T>::CHttpTools(const string &sUrl, int nIfRespones, const string &sFileName)
{
    m_sUrl = sUrl;
    m_nIfRespones = nIfRespones;
    m_sFileName = sFileName;
}
template<class T>
CHttpTools<T>::~CHttpTools()
{
}

template<class T>
int CHttpTools<T>::SetMethod(const string &sMethod)
{
    m_sMethod = sMethod;
    return 0;
}

template<class T>
int CHttpTools<T>::Open(string sPostStr)
{     
        string sHosts;
        string sIpAddr;
        int nPort;
        string UrlFile;
      
        //重新初始化       
        m_httpSocket.Init();
        
        //m_httpSocket.
        
        if (m_sHead != "")
            m_httpSocket.SetHeader(m_sHead);

		
		int ret = CToolKit::ParseUrl(m_sUrl, sIpAddr, nPort, UrlFile, sHosts);
        if (ret != 0)
        {
                WriteRunInfo::WriteLog("Parse url fail %s", m_sUrl.c_str());
                return -1;
        }
        if (nPort == -1)
            nPort = HTTPPORT;

        if (m_sFileName == "")
        {
            m_sFileName = GetFileName(UrlFile);
		}

	    T *pData = new T(this, m_sFileName);
        pData->Init();
        
        m_httpSocket.SetDataProcess(pData);
	   
		m_httpSocket.SetHosts(sHosts);
		if (m_httpSocket.GetAddr().sin_addr.s_addr != inet_addr(sIpAddr.c_str()) || m_httpSocket.GetAddr().sin_port != htons(nPort))
		{
	        m_httpSocket.Close();        
	        ret =  m_httpSocket.Connect(sIpAddr, nPort);
	        if (ret !=0)
	        {
	            WriteRunInfo::WriteLog("connect server fail, %s:%d", sIpAddr.c_str(), nPort);
	            return -1;
	        }
		}
        if (m_sMethod == "GET")
        {
            m_httpSocket.SendGetRequest(UrlFile);
        }
        else if (m_sMethod == "POST")
        {
            m_httpSocket.SendPostRequest(UrlFile, sPostStr);
        }
        else if (m_sMethod == "HEAD")
        {
            m_httpSocket.SendGetRequest(UrlFile, 1);
        }
        else
        {
            WriteRunInfo::WriteLog("invalid http method %s", m_sMethod.c_str());
            return -1;
        }
        if (m_nIfRespones == 1)
        {
            ret = m_httpSocket.RecvResponse();
            if (ret != 0 && ret != 2)
            {
                WriteRunInfo::WriteLog("recv  response fail");
                return -1;
            }
        }
        return 0;
}


template<class T>
string CHttpTools<T>::GetFileName(const string &sUrl)
{
    string sFile;        
    if (sUrl == "/")
    {
            sFile = "index.html";
    }
    else
    {
            string::size_type nPos =  sUrl.rfind("/");
            if (nPos != string::npos)
            {
                    sFile = sUrl.substr(nPos+1);
            }
            else
            {
                    sFile = "index.html";
            }
    }     
    return sFile;
}


typedef CHttpTools<CHttpDataProcess> HTTPTOOLS;
}
#endif

