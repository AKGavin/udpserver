#include "maintainudpthread.h"
#include "commserver.h"
#include "basemaintaincmd.h"

int CMaintainUdpThread::ProcessMsg()
{
        int ret = 0;
        if (m_nRecvLen >= MAINTAINHEADSIZE)
        {
                TMaintainHead head;                        
                memcpy(&head, m_recvBuf, sizeof(TMaintainHead));
                head.nMsgBodyLen = ntohl(head.nMsgBodyLen);
                head.nMsgType = ntohl(head.nMsgType);
                if (m_nRecvLen != head.nMsgBodyLen + MAINTAINHEADSIZE)
                {
                        WriteRunInfo::WriteLog("CMaintainUdpThread::ProcessMsg m_nRecvLen(%d) != head.nMsgBodyLen(%d) + sizeof(TMaintainHead)(%d)",
                                m_nRecvLen, head.nMsgBodyLen, MAINTAINHEADSIZE);
                        return -1;
                }
                char *pBody = NULL;
                if (head.nMsgBodyLen > 0)
                {
                        pBody = m_recvBuf + sizeof(TMaintainHead);
                }
                
                CBaseMaintainCmd *pCmd = CMyserver::GetInstance()->GetClassFactory()->GenMaintainCmd(head.nMsgType, head.nMsgBodyLen, pBody);
                if (pCmd != NULL)
                {
                        pCmd->Process();
                        if (pCmd->GetSendBuf().GetRealBufLen() > 0 && pCmd->GetSendBuf().GetBuf() != NULL)
                        {
                                sendto(m_sock, pCmd->GetSendBuf().GetBuf(), pCmd->GetSendBuf().GetRealBufLen(), MSG_DONTWAIT,  (sockaddr*)&m_fromAddr, m_nAddrLen);
                        }
                        delete pCmd;
                }  
        }
        return ret;
}


