/*
 * tdmsparsehashwhitelist.cpp
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#include "tdmdbwhitelist.h"
#include "tdmhttprequest.h"
#include <string.h>
#include <fstream>
#include <time.h>
#include "framecommon/framecommon.h"


int CTDMDBWhiteList::findEntry( protocol_type_t protocol, char* urlhash, whiteList_data_t** whitelistItem )
{
	int ret = loadItemFromDB( protocol, urlhash, whitelistItem );
	return ret;
}


//return -1: fail; 0: success;
int CTDMDBWhiteList::addEntry( whiteItem_from from, char* infohash, pkg_data_t* data, whiteList_status_t status, unsigned long dcmip, whiteList_data_t** whiteItem )
{
	whiteList_data_t* whitelistItem = new whiteList_data_t();
	strcpy( whitelistItem->infohash, infohash );
	whitelistItem->status 		= status;
	whitelistItem->createTime	= time(NULL);
	whitelistItem->updateTime 	= time(NULL);
	whitelistItem->dcmIp		= dcmip ;
	whitelistItem->dcmPort		= 0;
	whitelistItem->data.type	= data->type;

	if ( from == WHITEITEM_FROM_DTM_TASK ) {
		switch(data->type){
			case PROTOCOL_THUNDER: {
				memcpy( &whitelistItem->data.thunder_data, &data->thunder_data, sizeof(thunder_data_t) );
				break;
			}
			case PROTOCOL_HTTP: {
				memcpy( &whitelistItem->data.http_data, &data->http_data, sizeof(http_data_t) );
				break;
			}
			case PROTOCOL_PPSTREAM:
			case PROTOCOL_PPLIVE:
			case PROTOCOL_BF: {
				memcpy( (char*) whitelistItem->data.infohash_data, (char*) data->infohash_data, 20 );
				break;
			}
			default : {
				delete whitelistItem;
				whiteItem = NULL;
				return -1;
			}
		}
	}
	else if ( from == WHITEITEM_FROM_DCM_DOWNLOAD ) {
		dcm_tdm_white_data_t* whiteData = & data->white_data;
		switch(data->type){
			case PROTOCOL_THUNDER: {
				thunder_data_t* thunderData = &whitelistItem->data.thunder_data;
				memcpy( thunderData->m_strInfohash, whiteData->infohash, 20 );
				break;
			}
			case PROTOCOL_HTTP: {
				http_data_t* httpData = & whitelistItem->data.http_data;
				memcpy( httpData->urlhash, whiteData->infohash, 20 );
				break;
			}
			case PROTOCOL_PPSTREAM:
			case PROTOCOL_PPLIVE:
			case PROTOCOL_BF: {
				memcpy( whitelistItem->data.infohash_data, whiteData->infohash, 20 );
				break;
			}
			default : {
				delete whitelistItem;
				whiteItem = NULL;
				return -1;
			}
		}
	}
	else {
		delete whitelistItem;
		whiteItem = NULL;
		return -1;
	}

	int ret = insertItemToDB( data->type, whitelistItem );
	if ( ret == -1 ) {
		delete whitelistItem;
		whiteItem = NULL;
		return -1;
	}
	else {
		*whiteItem = whitelistItem;
		return 0;
	}
}


int CTDMDBWhiteList::deleteEntry( protocol_type_t protocol, char* urlHash )
{
	int ret = deleteItemFromDB( protocol, urlHash );
	return ret;
}


int CTDMDBWhiteList::changeEntryStatus( protocol_type_t protocol, char* urlHash, whiteList_status_t status, unsigned long dcmIp, whiteList_data_t* whitelistItem)
{
		whitelistItem->status = status;
		whitelistItem->updateTime = time(NULL);
		if ( dcmIp > 0 ) {
			whitelistItem->dcmIp = dcmIp;
		}
		else {
			whitelistItem->dcmIp = 0;
		}
		int ret = updateItemToDB( protocol, urlHash, status, whitelistItem->dcmIp, whitelistItem->updateTime );
		return ret;
}


int CTDMDBWhiteList::flushEntries( protocol_type_t proto )
{
	return 0;
}


int CTDMDBWhiteList::getInfoHashFromPackageData( pkg_data_t* pkgData, char* infohash )
{
	if ( pkgData == NULL ) return -1;

	memset( infohash, 0, 21 );
	protocol_type_t proto = pkgData->type;
	switch ( proto ) {
	case PROTOCOL_THUNDER: {
		memcpy( infohash, pkgData->thunder_data.m_strInfohash, 20 );
		return 0;
	}
	case PROTOCOL_HTTP: {
		memcpy( infohash, pkgData->http_data.urlhash, 20 );
		return 0;
	}
	case PROTOCOL_PPLIVE:
	case PROTOCOL_PPSTREAM:
	case PROTOCOL_BF: {
		memcpy( infohash, pkgData->infohash_data, 20 );
		return 0;
	}
	default: {
		return -1;
	}
	}
}



