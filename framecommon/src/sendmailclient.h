#ifndef __SENDMAIL_CLIENT_H
#define __SENDMAIL_CLIENT_H
#include <string>
#include "mysocket.h"
using namespace std;

namespace  MYFRAMECOMMON{
        
const int MAXMAILRECVBUF=10240;
class CSendMailClient
{
public:
        CSendMailClient(string &sAddr, string &sFromMail, string &sToMail, string &sSubject, string &sMailString)
        {
                m_sServerAddr = sAddr;
                m_fromMail = sFromMail;
                m_toMail = sToMail;
                m_sSubject = sSubject;
                m_sMailString = sMailString;
        }
        ~CSendMailClient()
        {
                
        }
        int SendMail();
private:
        CTcpSocket  m_tcpSocket;
        string m_sServerAddr;
        string m_fromMail;
        string m_toMail;
        string m_sSubject;
        string m_sMailString;
};

}
#endif
