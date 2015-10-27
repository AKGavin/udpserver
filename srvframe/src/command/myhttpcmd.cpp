/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2007-4-5
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#include "myhttpcmd.h"

CMyHttpCmd::CMyHttpCmd(string &sInHead, string &sInBody, string &sUrl)
{       
        m_sInHead = sInHead;
        m_sInBody = sInBody;
        m_sUrl = sUrl;
        m_nResult = 0;
        m_nIfSendRes = 1;
        m_sResHeader = "HTTP/1.1 200 OK\r\n";
        m_nIfUuid = 0;
        m_nPreTime = 0;
        m_nIfAnnexInfo = 1;
        m_nIfExtra = 1;

        m_sPeerId="";
        //SO_LINGER
        m_lingerOn=1;
}

CMyHttpCmd::~CMyHttpCmd()
{
};

int CMyHttpCmd::GetParaList()
{
        CToolKit::GetStringByLabel(m_sInHead, "Host:", "\r\n", m_sHostDomain);
        CToolKit::StringTrim(m_sHostDomain);   
        

        string sPara;
        CToolKit::GetStringByLabel(m_sUrl, "?", "", sPara);
        vector<string> strList;
        string sDelim = "&";
        CToolKit::SplitString(sPara, sDelim, strList);
        int nTmpNum = strList.size();
        int i=0;
        for ( i=0; i < nTmpNum; i++)
        {
                string sParaName;
                string sParaValue;
                CToolKit::GetStringByLabel(strList[i], "", "=", sParaName);
                CToolKit::GetStringByLabel(strList[i], "=", "", sParaValue);
                if (sParaName != "")
                {
                        CToolKit::StringTrim(sParaName);
                        CToolKit::StringTrim(sParaValue);
                        m_paraList.insert(make_pair(sParaName, sParaValue));
                }
        }

        
        //获取cookie
        strList.clear();
        sDelim = ";";
        string sCookieStr;
        CToolKit::GetStringByLabel(m_sInHead, "Cookie:",  "\r\n", sCookieStr);
        if (sCookieStr != "")
        {
                CToolKit::StringTrim(sCookieStr);
                CToolKit::SplitString(sCookieStr, sDelim, strList);
                nTmpNum = strList.size();
                for ( i=0; i < nTmpNum; i++)
                {
                        string sParaName;
                        string sParaValue;
                        CToolKit::GetStringByLabel(strList[i], "", "=", sParaName);
                        CToolKit::GetStringByLabel(strList[i], "=", "", sParaValue);
                        if (sParaName != "")
                        {
                                CToolKit::StringTrim(sParaName);
                                CToolKit::StringTrim(sParaValue);
                                m_cookieList.insert(make_pair(sParaName, sParaValue));
                        }
                }
        }

        CToolKit::GetStringByLabel(m_sInHead, "Referer:", "\r\n", m_sReferUrl);
        return 0;
}


int CMyHttpCmd::ProcessCmd()
{               
        //获取参数列表
        if (m_nIfExtra == 1)
            GetParaList();

        int ret  = 0;
        ret = Decode();
        if (ret == 0)
        {
                Process();
        }
        
        if (m_nIfSendRes == 1)
        {
                ret = Encode();                    
                //生成http头             
                m_sOut = m_sResHeader +
                        "Connection: close\r\n"
                        "Accept-Ranges: bytes\r\n"
                        "Server: MyHttpSrv(1.0)\r\n"
                        "Date:" + CToolKit::SecToHttpTime(time(NULL)) + "\r\n"
                        "Content-Length: " + CToolKit::IntToStr(m_sOutBody.length()) + "\r\n";
                               
                m_sOut += "\r\n";
                if (m_sOutBody.length() > 0)
                        m_sOut += m_sOutBody;
        }
        return 0;
}




