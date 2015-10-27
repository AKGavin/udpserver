/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2006-11-22
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef  __HTTP_SOCKET_H
#define  __HTTP_SOCKET_H
#include "mysocket.h"
#include "toolkit.h"
 #include <poll.h>
  #include <sys/time.h>
#include <stdlib.h>
namespace MYFRAMECOMMON{

const int HTTPPORT=80;
const int PRERECVLEN=1024;
const int HTTPRECVTIMEOUT=60;

//return value
const int HTTPOK = 0;

template<class T>
class CHttpSocket:public CTcpSocket
{      
public:
        CHttpSocket(sockaddr_in &toAddr, socklen_t toLen,   int iSocket):CTcpSocket(toAddr,  toLen,  iSocket)
        {            
        		m_sSetHeader = "";
                 m_pDataProcess = NULL;
                Init();               
        }

        CHttpSocket(string &sAddr, short sPort, int iSocket):CTcpSocket(sAddr, sPort, iSocket)
        {
        	  m_sSetHeader = ""; 
              m_pDataProcess = NULL;
                Init();                
        }   
        
        CHttpSocket()
        {
        	  m_sSetHeader = "";
               m_pDataProcess = NULL;
                Init();               
        }

        ~CHttpSocket()
        {
            if (m_pDataProcess != NULL)
                delete m_pDataProcess;
        }

        void Init()
        {
                m_nContentLen = 0;
                m_nLastModify = -1;
                m_nRealContentLen = 0;
                m_sResponseNumber = "";
                //m_sResPage = "";                
                m_sHosts = "";
                m_sRequesHead = "";
                m_sResponseHead = "";               
                //m_sSetHeader = "";
                //m_sRes = "";
				m_nIfChunked=0;
				m_nCurChunkedLen=-1;
				m_nCurChunkedRecLen=0;
                if (m_pDataProcess != NULL)
                    {
                    delete m_pDataProcess;
                    m_pDataProcess = NULL;
                    }
        }

        
        void SetDataProcess(T *pDataProcess)
        {
            m_pDataProcess = pDataProcess;
        }

        void ProcessData(const char *pBuf, int nLen, int nIfHead=0)
        {
            m_pDataProcess->ProcessData(pBuf, nLen, nIfHead);
        }

        int SendGetRequest(string &sUrlFile, int ifHead=0);
        
        int RecvResponse();
        //string &GetResponsePage();        
        void SetHosts(string &sHosts)
        {
                m_sHosts = sHosts;
        }
        
        string &GetResHead();
        string &GetRequestHead();

        void SetContentLen(int nContentLen)
        {
                m_nContentLen = nContentLen;
        }

        int GetRealContentLen()
        {
                return m_nRealContentLen;
        }

        string &GetResponseNumber()
        {
                return m_sResponseNumber;
        }


        int GetPageLastModifyTime();

        void SetHeader(string sHeader);                

//        string &GetRes()
//        {
//                return m_sRes;
//        }
        
        int SendPostRequest(string &sUrlFile, string &sPostStr);

		int GetChunked(char *pBuf, int nLen);

private:              
        int m_nContentLen;
        int m_nRealContentLen;
		//chunked
		int m_nIfChunked;
		int m_nCurChunkedLen;
		int m_nCurChunkedRecLen;
        
        //string m_sResPage;                
        string m_sHosts;
        string m_sRequesHead;
        string m_sResponseHead;
        string m_sResponseNumber;
		string m_sResChunkedBody;

        //string m_sRes;

        int m_nLastModify;

        string m_sSetHeader;
	int m_isConnected;

    T * m_pDataProcess;
};

template<class T>
void CHttpSocket<T>::SetHeader(string sHeader)
{
        m_sSetHeader = sHeader;
}

template<class T>
int CHttpSocket<T>::SendGetRequest(string &sUrlFile, int ifHead)
{
        m_sRequesHead = (ifHead==0)?"GET ":"HEAD ";            
        m_sRequesHead +=  sUrlFile + " HTTP/1.1\r\n"
                + "Accept: */*\r\n"
                + "Accept-Language: zh-cn \r\n"
                + "Host: " + m_sHosts + "\r\n"
                + "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0;windows NT 5.1; SV1)\r\n"
                + "Connection: keep-alive\r\n";
        m_sRequesHead += m_sSetHeader;
        m_sRequesHead += "\r\n";                  
    
        int nResult = 0;
        Send((char*)m_sRequesHead.c_str(), m_sRequesHead.length(), 10, nResult);        
        return nResult;
}

template<class T>
int CHttpSocket<T>::SendPostRequest(string &sUrlFile, string &sPostStr)
{
        m_sRequesHead = "POST " + sUrlFile + " HTTP/1.1\r\n"
                + "Accept: */*\r\n"
                + "Accept-Language: zh-cn \r\n"
                + "Host: " + m_sHosts + "\r\n"
                + "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0;windows NT 5.1; SV1)\r\n"
                + "Connection: keep-alive\r\n";
        m_sRequesHead += m_sSetHeader;
        m_sRequesHead += "Content-Length:"+ CToolKit::IntToStr(sPostStr.length())+ "\r\n";
        m_sRequesHead += "\r\n";                  

        string tmpStr = m_sRequesHead + sPostStr;
        int nResult = 0;
        Send((char*)tmpStr.c_str(), tmpStr.length(), 20, nResult);        
        return nResult;
}

template<class T>
int CHttpSocket<T>::RecvResponse()
{               
        pollfd tmppollfd[1];
        tmppollfd[0].fd = m_iSocket;
        tmppollfd[0].events = POLLERR|POLLHUP|POLLIN;
        int beginTime = time(NULL);
        char recvBuf[10240]={0};
        int nHasRecvLen=0;
        //m_sRes = "";
        
        int nGetStatus = 0;
        int nHasRecvConLen = 0;
        //m_sResPage = "";
        
        timeval tm1, tm2;
        gettimeofday(&tm1, NULL);

		ProcessData(m_sRequesHead.c_str(), m_sRequesHead.length(), 2);
        int retvalue = 0;
        while (1)
        {
                int ret = poll(tmppollfd, 1,  1000);
                if (ret > 0)
                {
                        if ((tmppollfd[0].revents & POLLERR) == POLLERR | (tmppollfd[0].revents & POLLHUP) == POLLHUP)
                        {
                                WriteRunInfo::WriteLog("Recv error event %d", tmppollfd[0]);
                                return -1;
                        }
                        
                        if ((tmppollfd[0].revents & POLLIN) == POLLIN)
                        {
                                ret = recv(m_iSocket, recvBuf+nHasRecvLen, 10240, MSG_DONTWAIT);
                                if (ret == 0)
                                {
                                        WriteRunInfo::WriteLog("The socket has been closed by server");
                                        Close();      
                                        retvalue = 2;
                                        break;
                                }     
                                else if (ret > 0)
                                {                                                                               
                                        if (nGetStatus == 0)
                                        {      
                                                nHasRecvLen += ret;
                                                char *pTmp = strstr(recvBuf, "\r\n\r\n");
                                                if (pTmp != NULL)
                                                {                                                                                                            
                                                        m_sResponseHead = string(recvBuf, pTmp-recvBuf+4);
														ProcessData(m_sResponseHead.c_str(), m_sResponseHead.length(), 1);
                                                           //判断是否是成功的响应                                                                                                                                                                     
                                                        CToolKit::GetStringByLabel(m_sResponseHead, " ", " ", m_sResponseNumber);                                                       
                                                                                                              
                                                        string sConStr;
                                                        int nRet =  CToolKit::GetStringByLabel(m_sResponseHead, "Content-Length:", "\r\n", sConStr);
                                                        if (nRet != -1)
                                                        {
                                                            m_nRealContentLen = atoi(sConStr.c_str());                                                                                                                               
                                                            if (m_nContentLen == 0 || m_nContentLen > m_nRealContentLen)
                                                            {
                                                                m_nContentLen = m_nRealContentLen;
																ProcessData(NULL, m_nContentLen); //处理数据
                                                            }                                                                                             
                                                        }
                                                        else
                                                        {
                                                            //chunked
                                                            nRet=CToolKit::GetStringByLabel(m_sResponseHead, "Transfer-Encoding:", "\r\n", sConStr);
                                                            if (nRet != -1)
                                                            {
                                                                CToolKit::StringTrim(sConStr);
                                                                if (sConStr == "chunked")
                                                                {
                                                                    m_nIfChunked=1;
                                                                }
                                                            }
                                                        }

                                                        
                                                                                                                
                                                        if (m_nIfChunked == 0)
                                                        {
                                                            nGetStatus = 1;
                                                            //m_sResPage = m_sRes.substr(nPos+4);                                                             
                                                            nHasRecvConLen = nHasRecvLen - (pTmp-recvBuf+4);
                                                            ProcessData(pTmp+4, nHasRecvConLen); //处理数据
                                                            if (m_nContentLen != 0)
                                                            {
                                                                    if(nHasRecvConLen >= m_nContentLen)
                                                                    {                                                          
                                                                            break;
                                                                    }
                                                            }                                                            
                                                        }
                                                        else
                                                        {
                                                            //m_sResChunkedBody =m_sRes.substr(nPos+4);
                                                            nGetStatus = 2;
                                                            if (GetChunked(pTmp+4, nHasRecvLen - (pTmp-recvBuf+4)) == 1)
                                                            {
                                                                break;
                                                            }
                                                        }
                                                        nHasRecvLen=0;
                                                }
                                        }
                                        else if (nGetStatus == 1)
                                        {
                                                //m_sResPage += string(recvBuf, ret);
                                                ProcessData(recvBuf, ret);                                                
                                                nHasRecvConLen += ret;
                                                if (m_nContentLen != 0)
                                                {
                                                        if(nHasRecvConLen >= m_nContentLen)
                                                        {                                                                                           
                                                                if (m_sResponseHead.find("Connection: close") != string::npos)
                                                                {
                                                                        Close();
                                                                }                                                  
                                                                break;
                                                        }
                                                }
                                        }
                                        else //status=2
                                        {
                                            //m_sResChunkedBody += string(recvBuf,ret);
                                            int tmp=GetChunked(recvBuf, ret);                                           
                                            if (tmp==1)
                                            {
                                                break;
                                            }
                                        }
                                }
                                else  //error
                                {
                                        m_isConnected = 0;
                                        break;
                                }                              
                        }
                }
                        
                int nowTime = time(NULL);
                if (m_pDataProcess->GetTimeOut() > 0 && nowTime - beginTime > m_pDataProcess->GetTimeOut()) //time out
                {
                        break;
                }
         }
         gettimeofday(&tm2, NULL);
         WriteRunInfo::WriteLog("Http reques cost %d sec and %d usec", tm2.tv_sec - tm1.tv_sec, tm2.tv_usec - tm1.tv_usec);
         return retvalue;        
}

//template<class T>
//string & CHttpSocket::GetResponsePage()
//{
//        return m_sResPage;
//}

template<class T>
string &CHttpSocket<T>::GetResHead()
{
        return m_sResponseHead;
}

template<class T>
string &CHttpSocket<T>::GetRequestHead()
{
        return m_sRequesHead;
}

template<class T>
int CHttpSocket<T>::GetPageLastModifyTime()
{
        int nLastModify = 0;
        if (m_nLastModify == -1) //未获取
        {
                string sOut;
                int ret = CToolKit::GetStringByLabel(m_sResponseHead, "Last-Modified: ", "\r\n", sOut);
                if (ret == -1)
                {
                    m_nLastModify = 0;
                    nLastModify = time(NULL);
                }                
                else
                {       
                    m_nLastModify = CToolKit::TimeToSec(sOut);
                    if (m_nLastModify == 0)
                    {
                       nLastModify = time(NULL);
                    }
                    else
                    {
                       nLastModify = m_nLastModify;
                    }
                }
        }
        else if (m_nLastModify == 0)
        {
                nLastModify = time(NULL);
        }
        else
        {
                nLastModify = m_nLastModify;
        }
        return nLastModify;
}

template<class T>
int CHttpSocket<T>::GetChunked(char *pBuf, int nLen)
{
    int ret=0;
    m_sResChunkedBody.append(pBuf, nLen);
    while (1)
    {
        if (m_nCurChunkedLen == -1)
        {   
            string sTmp;
            int nRet= CToolKit::GetStringByLabel(m_sResChunkedBody, "", "\r\n", sTmp, 0, 1);            
            if (nRet != -1)
            {
                m_nCurChunkedLen=strtoul(sTmp.c_str(), 0, 16);
                m_nCurChunkedRecLen = nRet;
                if(m_nCurChunkedLen == 0)
                {
                    ret=1; //结束
                    break;
                }
                
                if (m_nCurChunkedLen + 2 + m_nCurChunkedRecLen  > (int)m_sResChunkedBody.length())
                {
                    //m_sResPage.append(m_sResChunkedBody.substr(m_nCurChunkedRecLen));
                    break;
                }
                else
                {
                    ProcessData(m_sResChunkedBody.substr(m_nCurChunkedRecLen, m_nCurChunkedLen).c_str(), m_nCurChunkedLen);
                    //m_nCurChunkedRecLen += m_nCurChunkedLen + 2;                    
                    m_sResChunkedBody = m_sResChunkedBody.substr(m_nCurChunkedRecLen+2+m_nCurChunkedLen);
                    m_nCurChunkedLen = -1;
                    if (0 == m_sResChunkedBody.length())
                        break;
                }
            }
            else
            {
               // WriteRunInfo::WriteLog("m_nCurChunkedRecLen is %d, m_sResChunkedBody.length is %d", m_nCurChunkedRecLen, m_sResChunkedBody.length());
                break;
            }
        }
        else //已经获取长度但是没有获取数据
        {          
            if (m_nCurChunkedLen+2+m_nCurChunkedRecLen > (int)m_sResChunkedBody.length())
            {
                //m_sResPage.append(m_sResChunkedBody.substr(m_nCurChunkedRecLen));
                break;
            }
            else
            {
                ProcessData(m_sResChunkedBody.substr(m_nCurChunkedRecLen, m_nCurChunkedLen).c_str(), m_nCurChunkedLen);
                //m_nCurChunkedRecLen += m_nCurChunkedLen + 2;
                m_sResChunkedBody = m_sResChunkedBody.substr(m_nCurChunkedLen+m_nCurChunkedRecLen+2);
                m_nCurChunkedLen = -1;
                if ( 0 == m_sResChunkedBody.length())
                        break;
            }
        }
    }
    return ret;
}
}
#endif
