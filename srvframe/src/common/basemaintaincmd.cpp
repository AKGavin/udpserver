/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2007-1-5
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#include "basemaintaincmd.h"
#include "commmsg.h"
#include "task.h"
#include "myserver.h"
CBaseMaintainCmd::CBaseMaintainCmd(int nMsgType, int nBodyLen, char *pBody)
{ 
        if (nBodyLen > 0 && pBody != NULL)
        {
                char *pBuf = new char[nBodyLen];
                memcpy(pBuf, pBody, nBodyLen);
                m_sRecvBuf.AttachBuf(pBuf, nBodyLen);
        }
        m_nMsgType = nMsgType;
}
 
CBaseMaintainCmd::~CBaseMaintainCmd()
{
}

int CBaseMaintainCmd::Process()
{
        switch(m_nMsgType)
        {                       
                case CMD_DEBUGINFO: //调试信息
                {                                           
                       //先把这个客户端关闭掉                       
                       //直接设置
                       if ( (size_t)m_sRecvBuf.GetBufLen() < sizeof(CmdModLogInfo))
                       {
                            break;
                       }
                       
                       CmdModLogInfo *pCmdModLogInfo = (CmdModLogInfo*)m_sRecvBuf.GetBuf();
                       pCmdModLogInfo->iFlag = ntohl(pCmdModLogInfo->iFlag);
                       pCmdModLogInfo->iModLogType = ntohl(pCmdModLogInfo->iModLogType);
                       WriteRunInfo::WriteLog("Recv debufInfo logmod %d flag %d",pCmdModLogInfo->iModLogType,
                                pCmdModLogInfo->iFlag);
                       //设置打印的开关 
                       WriteRunInfo::SetLogFlag(pCmdModLogInfo->iModLogType, 
                                pCmdModLogInfo->iFlag);
                       break;         
                }              
                case PRINTSTATINFO:  //打印统计信息
                {
                        WriteRunInfo::WriteStat("Now task num is %d", CTask::GetTaskNum());
                        //打印统计信息
                        CMyserver::GetInstance()->GetCommonStat().PrintStat();
                        //SendMsg(TIMERENTITY, 0, 0, PRINTTIMER, NULL, 0);
                        break;                                
                }
                default:
                {                      
                        WriteRunInfo::WriteLog("In maintain thread, the msgtype %d is invalid", m_nMsgType);                       
                        break;
                }
        }          
        return 0;
}

