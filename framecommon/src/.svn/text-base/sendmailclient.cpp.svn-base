#include "sendmailclient.h"
#include "mysocket.h"
#include <sys/time.h>
#include "writeRunInfo.h"
namespace MYFRAMECOMMON{
const string ENDSTR="\r\n";

int CSendMailClient::SendMail()
{               
        WRITE_TRACE("Begin send mail ......");
        WriteRunInfo::WriteNowDetailTime();
        int ret =  m_tcpSocket.Connect(m_sServerAddr, 25); //连接服务器
        if (ret != 0)
        {
                WRITE_TRACE("Connect to mail server fail");
                return -1;
        }
        m_tcpSocket.SetNonBlocking();
        char sRecivBuf[MAXMAILRECVBUF];        
        int nResult = 0;
        ret =  m_tcpSocket.Receive(sRecivBuf, MAXMAILRECVBUF, 10, nResult, ENDSTR);
        if (nResult != 0) //收到完整的请求
        {
                WriteRunInfo::WriteRunLog("Recv mail server connect response fail");
                return -1;
        }
        if (strncmp(sRecivBuf, "220", 3) != 0)
        {
                WriteRunInfo::WriteRunLog("Mail server connect response parm error, %s", sRecivBuf);
                return -1;
        }
        WRITE_TRACE("recv 220 ok ......");
        string sCommand;
        sCommand = "HELO register\r\n";
        m_tcpSocket.SendRealBuf((char*)sCommand.c_str(), sCommand.length());
        
        ret =  m_tcpSocket.Receive(sRecivBuf, MAXMAILRECVBUF, 5, nResult, ENDSTR);
        if (nResult != 0) //收到完整的请求
        {
                WriteRunInfo::WriteRunLog("Recv mail server connect response fail");
                return -1;
        }
        WRITE_TRACE("recv helo register resp ok ......");
        if (strncmp(sRecivBuf, "250", 3) != 0)
        {
                WriteRunInfo::WriteRunLog("Mail server HELO response parm error, %s", sRecivBuf);
                return -1;
        }

        sCommand = "MAIL FROM:<";
        sCommand += m_fromMail;
        sCommand += ">\r\n";
        m_tcpSocket.SendRealBuf((char*)sCommand.c_str(), sCommand.length());
        
        ret =  m_tcpSocket.Receive(sRecivBuf, MAXMAILRECVBUF, 5, nResult, ENDSTR);
        if (nResult != 0) //收到完整的请求
        {
                WriteRunInfo::WriteRunLog("Recv mail server MAIL FROM response fail");
                return -1;
        }

        if (strncmp(sRecivBuf, "250", 3) != 0)
        {
                WriteRunInfo::WriteRunLog("Mail server MAIL FROM response parm error, %s", sRecivBuf);
                return -1;
        }
        WRITE_TRACE("recv MAIL FROM resp ok ......");

        sCommand = "RCPT TO:<";
        sCommand += m_toMail;
        sCommand += ">\r\n";
        m_tcpSocket.SendRealBuf((char*)sCommand.c_str(), sCommand.length());
        ret =  m_tcpSocket.Receive(sRecivBuf, MAXMAILRECVBUF, 5, nResult, ENDSTR);
        if (nResult != 0) //收到完整的请求
        {
                WriteRunInfo::WriteRunLog("Recv mail server RCPT TO response fail");
                return -1;
        }
        if (strncmp(sRecivBuf, "250", 3) != 0)
        {
                WriteRunInfo::WriteRunLog("Mail server RCPT TO response parm error, %s", sRecivBuf);
                return -1;
        }
        WRITE_TRACE("recv RCPT TO: resp ok ......");

        sCommand = "DATA\r\n";
        m_tcpSocket.SendRealBuf((char*)sCommand.c_str(), sCommand.length());
        ret =  m_tcpSocket.Receive(sRecivBuf, MAXMAILRECVBUF, 5, nResult, ENDSTR);
        if (nResult != 0) //收到完整的请求
        {
                WriteRunInfo::WriteRunLog("Recv mail server DATA response fail");
                return -1;
        }
        if (strncmp(sRecivBuf, "354", 3) != 0)
        {
                WriteRunInfo::WriteRunLog("Mail server DATA response parm error, %s", sRecivBuf);
                return -1;
        }
        WRITE_TRACE("recv (data) 354 ok ......");

        sCommand = "Subject: ";
        sCommand += m_sSubject;
        sCommand += "\r\n";
        sCommand += m_sMailString;
        sCommand += "\r\n.\r\n";
        m_tcpSocket.SendRealBuf((char*)sCommand.c_str(), sCommand.length());
        ret =  m_tcpSocket.Receive(sRecivBuf, MAXMAILRECVBUF, 5, nResult, ENDSTR);
        if (nResult != 0) //收到完整的请求
        {
                WriteRunInfo::WriteRunLog("Recv mail server Subject response fail");
                return -1;
        }
        if (strncmp(sRecivBuf, "250", 3) != 0)
        {
                WriteRunInfo::WriteRunLog("Mail server Subject response parm error, %s", sRecivBuf);
                return -1;
        }
        sCommand = "QUIT\r\n";
        m_tcpSocket.SendRealBuf((char*)sCommand.c_str(), sCommand.length());
        WRITE_TRACE("End send mail ......");
        WriteRunInfo::WriteNowDetailTime();
        return 0;
}
}

