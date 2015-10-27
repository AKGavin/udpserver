/*
 * tdmmysqlwrapperpool.cpp
 *
 *  Created on: 2013-12-31
 *      Author: root
 */

#include "tdmmysqlwrapperpool.h"


CTDMMySQLWrapperPool* CTDMMySQLWrapperPool::m_pool = NULL;



CTDMMySQLWrapperPool::CTDMMySQLWrapperPool( const char* host, const char* user, const char* passwd, const char* db, int size )
{
	if ( size > 0 ) {
		m_maxsize = size;
	}
	else {
		m_maxsize = MAX_QUEUE_SIZE;
	}

	sem_init(&m_sem_write, 0, 0);
	sem_init(&m_sem_read, 0, m_maxsize - 1);
	pthread_mutex_init(&m_mutex_write, NULL);

	CMysqlWrapper* w = NULL;
	pthread_mutex_lock(&m_mutex_write);
	for ( size_t i=0; i<m_maxsize; i++ ) {
		w = new CMysqlWrapper( host, user, passwd, db );
		w->Connect();
		m_queue.push_back(w);
		m_list.push_back( w );
	}
	pthread_mutex_unlock(&m_mutex_write);
}



CTDMMySQLWrapperPool::~CTDMMySQLWrapperPool() {
	list<CMysqlWrapper*>::iterator iter;
	CMysqlWrapper* w = NULL;

	pthread_mutex_lock(&m_mutex_write);
	for (iter = m_list.begin(); iter != m_list.end(); iter++) {
		w = *iter;
		delete w;
	}

	m_queue.clear();
	m_list.clear();
	pthread_mutex_unlock(&m_mutex_write);

	sem_destroy(&m_sem_read);
	sem_destroy(&m_sem_write);
	pthread_mutex_destroy(&m_mutex_write);
}


CMysqlWrapper* CTDMMySQLWrapperPool::open() {
	CMysqlWrapper *w = NULL;
	sem_wait(&m_sem_read);
	pthread_mutex_lock(&m_mutex_write);
	deque<CMysqlWrapper*>::iterator tmpItr = m_queue.begin();
	if (tmpItr != m_queue.end()) {
		w = *tmpItr;
		m_queue.pop_front();
	}
	pthread_mutex_unlock(&m_mutex_write);
	sem_post(&m_sem_write);
	return w;
}


int CTDMMySQLWrapperPool::close(CMysqlWrapper *w) {
	sem_wait(&m_sem_write);

	pthread_mutex_lock(&m_mutex_write);
	m_queue.push_back(w);
	pthread_mutex_unlock(&m_mutex_write);

	return sem_post(&m_sem_read);
}


size_t CTDMMySQLWrapperPool::size() {
	return m_queue.size();
}


size_t CTDMMySQLWrapperPool::length() {
	return m_maxsize;
}


CTDMMySQLWrapperPool* CTDMMySQLWrapperPool::getInstance() {
	return m_pool;
}


void CTDMMySQLWrapperPool::init(const char* host, const char* user, const char* passwd,
		const char* db, int size) {
	m_pool = new CTDMMySQLWrapperPool(host, user, passwd, db, size);
}




