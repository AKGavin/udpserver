/*
 * tdmsparsehashhotstats.h
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#ifndef TDMSPARSEHASHHOTSTATS_H_
#define TDMSPARSEHASHHOTSTATS_H_

#include <unistd.h>
#include <pthread.h>
#include </usr/local/include/sparsehash/sparse_hash_map>
#include "tdmhotstats.h"

using namespace google;

extern sparse_hash_map<string, hotItem_t *> ThunderHotList;
extern sparse_hash_map<string, hotItem_t *> HttpHotList;


class CTDMSparseHashHotStats : public CTDMHotStats
{
public:
	CTDMSparseHashHotStats()
	{
		pthread_mutex_init(&m_mutex_write, NULL);
	};
	~CTDMSparseHashHotStats() {
		pthread_mutex_destroy(&m_mutex_write);
	};

	int addEntry( char* infohash, pkg_data_t* data, hotItem_t** hotItem );
	int deleteEntry( protocol_type_t protocol, char* urlHash );
	int flushEntries( protocol_type_t );
	int saveAllEntriesToFile( protocol_type_t protocol );

private:
	pthread_mutex_t m_mutex_write;
};


#endif /* TDMSPARSEHASHHOTSTATS_H_ */
