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
#ifndef __SNAP_PAGE_H
#define __SNAP_PAGE_H
#include "httpsocket.h"
#include "httptools.h"

namespace MYFRAMECOMMON{


const int DATABLOCKLENGTH =20*1024;
const int SNAPOK=0;
const int SNAPREDIRECT=1;
const int SERVERCLOSE=2;

class CSnapPage
{
public:
        CSnapPage(string &sUrl);
        ~CSnapPage();
        int Snap(string sOutFile="");
//        string &GetPage()
//        {
//                return m_socket.GetResponsePage();
//        }
        int DumpPage();

        CHttpSocket<CHttpDataProcess> &GetHttpSocket();
        
        void SetUrl(string &sUrl);
        void SetRange(int nBegin, int nEnd, int nContentLen);
        int GetCid(string &sCid);

        int GetPageRealSize();
        string &GetRedirectUrl()
        {
                return m_sRedirectUrl;
        }

        void AddHeader(string sHeader)
        {
                m_sSetHeader += sHeader;
        }
private:        
        int m_isConnected;
        string m_sUrl;
        CHttpSocket<CHttpDataProcess> m_socket; 
        string m_sHosts;
        string m_sUrlFile;
        int m_nPort;
        string m_sFileName;

        
        int m_nPageRealSize;

        string m_sRedirectUrl;

        int m_nContentLen;
        string m_sSetHeader;
};
}
#endif
