#ifndef __CLIB_QUEUE__
#define __CLIB_QUEUE__

extern "C" {
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
}

#include <deque>
#include "framecommon/framecommon.h"


using namespace std;

#define MAX_QUEUE_SIZE	512000
#define CQUE_PRI_HIGH   1




class CTDMMySQLWrapperPool
{
private:
    static CTDMMySQLWrapperPool* m_pool;

public:
	static CTDMMySQLWrapperPool* getInstance();
	static void init( const char* host, const char* user, const char* passwd, const char* db, int size );

		CTDMMySQLWrapperPool( const char* host, const char* user, const char* passwd, const char* db, int size );
        ~CTDMMySQLWrapperPool();

        CMysqlWrapper*   open();
        int  close(CMysqlWrapper *w);
    
    
        size_t  size();
        size_t  length();

private:
    size_t m_maxsize;

    pthread_mutex_t m_mutex_write;
    sem_t m_sem_read;               /* �ɶ�ȡ��Ԫ�ظ��� */
    sem_t m_sem_write;              /* ��д��Ŀ�λ���� */

    deque<CMysqlWrapper*> m_queue;               /* �������ݵĶ��� */
    list<CMysqlWrapper*> m_list;               /* �������ݵĶ��� */

};

#endif /* __CLIB_QUEUE__ */
