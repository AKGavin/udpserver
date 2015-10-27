/*
 * tdmsparsehashwhitelist.h
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#ifndef TDMDBWHITELIST_H_
#define TDMDBWHITELIST_H_

#include "tdmwhitelist.h"


class CTDMDBWhiteList : public CTDMWhiteList
{
public:
	CTDMDBWhiteList(){};

	int findEntry( protocol_type_t protocol, char* urlhash, whiteList_data_t** whitelistItem );
	int addEntry( whiteItem_from from, char* infohash, pkg_data_t* data, whiteList_status_t status, unsigned long dcmip, whiteList_data_t** whiteItem );
	int deleteEntry( protocol_type_t protocol, char* urlHash ) ;
	int changeEntryStatus( protocol_type_t protocol, char* urlHash, whiteList_status_t status, unsigned long dcmIp, whiteList_data_t* whiteItem ) ;
	int flushEntries( protocol_type_t ) ;

private:
	static int getInfoHashFromPackageData( pkg_data_t* pkgData, char* infohash );
};


#endif /* TDMDBWHITELIST_H_ */

