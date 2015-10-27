/*
 * tdmwhitelist.h
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#ifndef TDMWHITELIST_H_
#define TDMWHITELIST_H_

#include "framecommon/framecommon.h"
#include "tdmconstants.h"
#include </usr/local/include/sparsehash/sparse_hash_map>

typedef enum {
	TYPE_INT,
	TYPE_LONG,
	TYPE_STRING,
	TYPE_CHARARRAY,
	TYPE_DATETIME
} field_type_t;



extern google::sparse_hash_map<string, whiteList_data_t *> ThunderWhiteList;
extern google::sparse_hash_map<string, whiteList_data_t *> HttpWhiteList;


class CTDMWhiteList
{
public:
	CTDMWhiteList(){};

	virtual ~CTDMWhiteList() {};

	virtual int findEntry( protocol_type_t protocol, char* urlhash, whiteList_data_t** whitelistItem ) = 0;
	virtual int addEntry( whiteItem_from from, char* infohash, pkg_data_t* data, whiteList_status_t status, unsigned long dcmip, whiteList_data_t** whitelistItem ) = 0;
	virtual int deleteEntry( protocol_type_t protocol, char* urlHash ) = 0;
	virtual int changeEntryStatus( protocol_type_t protocol, char* urlHash, whiteList_status_t status, unsigned long dcmIp, whiteList_data_t* whiteItem ) = 0 ;
	virtual int flushEntries( protocol_type_t ) = 0;
//	virtual int saveAllEntriesToFile( protocol_type_t protocol ) = 0;
//	virtual int loadAllEntriesFromFile( protocol_type_t protocol ) = 0;

	 static int loadItemFromDB( protocol_type_t proto, char* infohash, whiteList_data_t** whitelistItem );
	 static int insertItemToDB( protocol_type_t proto, whiteList_data_t* whitelistItem );
	 static int updateItemToDB( protocol_type_t proto, char* infohash, whiteList_status_t status, unsigned long dcmIp, time_t updateTime );
	 static int deleteItemFromDB( protocol_type_t proto, char* infohash );

//	 static int saveAllEntriesToDB( protocol_type_t protocol );
//	 static int loadAllEntriesFromDB( protocol_type_t protocol );

	 static void freeWhitelistItem( protocol_type_t proto, whiteList_data_t* whitelistItem );

private:
	 static void GetFieldValue( CMysqlWrapper* tdmmysqlwraper, const char* name, void* value, field_type_t type, const char* defaultValue );
	 static void GetStringFieldValue( CMysqlWrapper* tdmmysqlwraper, const char* name, char** value, const char* defaultValue );
	 static int FillWhitelistItem( CMysqlWrapper* tdmmysqlwraper, protocol_type_t proto, whiteList_data_t* whitelistItem );
	 static int getTableName( protocol_type_t proto, char** tableName, char** fieldName );

};




#endif /* TDMWHITELIST_H_ */



