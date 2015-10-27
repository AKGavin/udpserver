/*
 * tdmhotstats.h
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#ifndef TDMHOTSTATS_H_
#define TDMHOTSTATS_H_

//#include <map>
#include "tdmconstants.h"
#include <time.h>
#include </usr/local/include/sparsehash/sparse_hash_map>

typedef struct {
	unsigned int count;
	time_t createTime;
	google::sparse_hash_map <string,time_t> hotIpList;
} hotItem_t;


class CTDMHotStats
{
public:
	CTDMHotStats(){};
	virtual ~CTDMHotStats(){};

	virtual int addEntry( char* infohash, pkg_data_t* data, hotItem_t** hotItem ) = 0;
	virtual int deleteEntry( protocol_type_t protocol, char* urlHash ) = 0;
	virtual int flushEntries( protocol_type_t proto ) = 0;
	virtual int saveAllEntriesToFile( protocol_type_t protocol ) = 0;

	static int getStatsThreshold( protocol_type_t proto );
};



#endif /* TDMHOTSTATS_H_ */
