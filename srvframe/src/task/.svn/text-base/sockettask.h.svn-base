/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2005-8-24
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef __SOCKET_TASK_H
#define __SOCKET_TASK_H
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include "task.h"
#include "compoll.h"
#include "timestat.h"
#include <map>
#include <list>
using namespace std;

const unsigned int MAXHEADLEN = 60;

const unsigned int  MAXRECVLEN=10*1024;
const unsigned int RECVBODYMAXLEN=102400;
const unsigned int MAXHTTPHEADLEN=4096;


//◊¥Ã¨
const int PREREADSTATUS=0;
const int READHTTPHEADSTATUS=1;
const int READMSGBODYSTATUS=2;
const int READGETHTTPSTATUS=3;


const unsigned int SENDMAXLEN=200*1024;


const int SOCKETTASKOUT=120;

class CNetProcessThread;

enum SOCKETTASKTYPE
{
        LISTENTASK = 0,
        SENDTASK = 1,
        RECVTASK = 2,
        RECVANDSENDTASK=3,
        COMMONTASK = 4,
        LONGCONNECTEDLISTENTASK = 5
};


class CSendMsg
{
public:
        CSendMsg(int len, char *buf)
        {
                m_sendLen = len;
                m_sendBody = buf;
        };        
        ~CSendMsg()
        {
                if (m_sendBody != NULL)
                {
                        delete []m_sendBody;
                }
        };

        unsigned int GetLen()
        {
                return m_sendLen;
        };
        
        char *GetBody()
        {
                return m_sendBody;
        };
protected:
        unsigned int m_sendLen;
        char* m_sendBody;
};

class CSocketTask;
/**
* dec:
* ClassName:CSocketPos
* author: 
* version:
*CreateTime: 2009.1.6
*/
class CSocketPos
{
public:
    CSocketPos(){}
    ~CSocketPos(){}

	void SetMapItr(map<ULONG64, CSocketTask*>::iterator mapItr)
	{
		m_mapItr = mapItr;
	}

	void SetListItr(list<CSocketTask*>::iterator listItr)
	{
		m_listItr = listItr;
	}
	
	map<ULONG64, CSocketTask*>::iterator GetMapItr()
	{
		return m_mapItr;
	}
	
	list<CSocketTask*>::iterator GetListItr()
	{
		return m_listItr;
	}

	
private:
	map<ULONG64, CSocketTask*>::iterator m_mapItr;
	list<CSocketTask*>::iterator m_listItr;
};


enum LISTTYPE
{
	MAPLISTTYPE=0,
	LINKEDLISTTYPE=1	
};
/**
* dec:
* ClassName:CSocketTask
* author: 
* version:
*CreateTime: 2005.8.24
*/
class CSocketTask
{
public:        
        CSocketTask(CTcpSocket *pTcpSocket, CNetProcessThread *pWorkThread, int nListType=MAPLISTTYPE);
        
        virtual ~CSocketTask();

	virtual int Init() = 0;
        /**
         *
         *  \param   msg
         *  \return   int
         */
        virtual int Process(CMsg * pMsg)=0;
        
        /**
         *
         *  \return   int
         */
        virtual int HandleProcess(int event)=0;
        
       virtual int PutMsgToSendList(const char *pBuf, int nLen) = 0;
        /**
         *
         *  \return   int
         */
       virtual int DestroySelf() = 0;
        
   

        CTcpSocket *GetTcpSocket()
        {
                return m_pTcpSocket;
        }
        
        int GetSocket()
        {
                return m_pTcpSocket->GetSocket();
        }    

	sockaddr_in &GetAddr()
	{
		return m_pTcpSocket->GetAddr();
	}
		
        void SetTimeStat(CTimeStat &timeStat)
        {
                timeStat.Clone(m_timeStat);
        }

        CTimeStat &GetTimeStat()
        {
                return m_timeStat;
        }

        void SetLinger();

        CSocketPos* GetPos();
        int CheckTimeOut();

		int GetPollPos();
		void SetPollPos(int pos);

		ULONG64 GetTaskId()
		{
			return m_lTaskId;
		}

		int GetListType()
		{
			return m_nListType;
		}
protected:           
        CNetProcessThread *m_pThread;
        CSocketPos* m_pPos;
		ULONG64 m_lTaskId; //taskid
	    int	m_nListType;  //¡¥Ω”¿‡–Õ0: list  1:map
        int DelFromEpoll();
        CTcpSocket *m_pTcpSocket;
        
        CTimeStat m_timeStat;
        int m_nTaskTimeOut;
        int m_nStartTime;
        int m_iTaskType;
        int m_status;
		int m_pollPos;

		static CThreadMutex m_sMutex;
		static ULONG64 m_sCurTaskId;
		ULONG64 GenTaskId();
};
#endif

