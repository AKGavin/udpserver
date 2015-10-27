/*
 * tdmsparsehashhotstats.cpp
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#include <time.h>
#include <string.h>
#include <fstream>
#include "tdmsparsehashhotstats.h"
#include "framecommon/framecommon.h"

using namespace google;

int CTDMSparseHashHotStats::addEntry( char* cInfohash, pkg_data_t* data, hotItem_t** item )
{
	time_t nowtime = time(NULL);
	sparse_hash_map<string, hotItem_t *>* HotlistMap = NULL;
	sparse_hash_map<string, hotItem_t *>::iterator l_it;

	WriteRunInfo::WriteLog(" hotlist addEntry START...");

	string infohash = cInfohash;

	switch( data->type ) {
	case PROTOCOL_THUNDER: {
		HotlistMap = &ThunderHotList;
		break;
	}
	case PROTOCOL_HTTP: {
		HotlistMap = &HttpHotList;
		break;
	}
	default: {
		WriteRunInfo::WriteLog( "error protocol type" );
		return -1;
	}
	}

	pthread_mutex_lock(&m_mutex_write);
	l_it = HotlistMap->find(infohash);
	hotItem_t* hotItem = NULL;
	if ( l_it != HotlistMap->end() ) {
		hotItem = l_it->second;
		unsigned int threshold = (unsigned int) getStatsThreshold( data->type );

		hotItem->count++;
		if ( nowtime - hotItem->createTime > 604800 && hotItem->count < threshold ) {
			HotlistMap->erase( l_it );
			delete hotItem;
			hotItem = NULL;
		}
	}
	else {
		hotItem = new hotItem_t();
		hotItem->count = 1;
		hotItem->createTime = nowtime;
		HotlistMap->insert( pair<string, hotItem_t *>(infohash, hotItem) );
	}
	pthread_mutex_unlock(&m_mutex_write);

	*item = hotItem;

	WriteRunInfo::WriteLog(" hotlist addEntry FINISHED!...");
	return 0;
}


int CTDMSparseHashHotStats::deleteEntry( protocol_type_t protocol, char* key )
{
	int lResult = 1;
	hotItem_t* hotItem = NULL;
	sparse_hash_map<string, hotItem_t *>* TempHotList;
	sparse_hash_map<string, hotItem_t *>::iterator l_it;

	string urlHash = key;

	switch (protocol) {
	case PROTOCOL_THUNDER:
		TempHotList = &ThunderHotList;
		break;

	case PROTOCOL_HTTP:
		TempHotList = &HttpHotList;
		break;

	default:
		lResult = -1;
		return lResult;
		break;
	}

	l_it = TempHotList->find(urlHash);
	if (l_it != TempHotList->end()) {
		hotItem = l_it->second;
		(hotItem->hotIpList).clear();

		TempHotList->erase(urlHash);
		delete hotItem;
		lResult = 1;
	}
	else {
		//如果没有默认删除成功
	}

	return lResult;
}


int CTDMSparseHashHotStats::flushEntries( protocol_type_t protocol)
{
	time_t NewTime;
	NewTime = time(NULL);
	hotItem_t* hotItem = NULL;

	sparse_hash_map<string, hotItem_t *> *TempHotList;
	sparse_hash_map<string, hotItem_t *>::iterator l_it;
	switch(protocol)
	{
		case PROTOCOL_THUNDER:
			TempHotList = &ThunderHotList;

			break;
		case PROTOCOL_HTTP:
			TempHotList = &HttpHotList;
			break;
		default:
			return -1;
			break;

	}

	unsigned int threshold = (unsigned int) getStatsThreshold(protocol);
	//for(l_it = (*TempHotList).begin(); l_it != (*TempHotList).end(); l_it++)
	l_it = (*TempHotList).begin();
	while (l_it != (*TempHotList).end()) {
		hotItem = l_it->second;
		if (hotItem == NULL) {
			TempHotList->erase(l_it++);
			continue;
		}

		if ((NewTime - hotItem->createTime) > 604800 && hotItem->count < threshold) {
			TempHotList->erase(l_it++);
			(hotItem->hotIpList).clear();
			delete hotItem;
		}
		else {
			l_it++;
		}
	}

	WriteRunInfo::WriteLog(" hotlist flushEntry FINISH!...");
	return 0;
}


int CTDMSparseHashHotStats::saveAllEntriesToFile( protocol_type_t protocol )
{
	int Result = 1;
	char filename[64];
	google::sparse_hash_map<string, hotItem_t *>::iterator l_it;

				switch (protocol) {
	case PROTOCOL_THUNDER:
		strcpy(filename, "tdmthunderhot.log");
		break;
	case PROTOCOL_HTTP:
		strcpy(filename, "tdmhttphot.log");
		break;
	case PROTOCOL_BT:
		break;
	case PROTOCOL_PPLIVE:
		break;
	case PROTOCOL_PPSTREAM:
		break;
	default:
		return Result;
	}

	ofstream out(filename, ios::binary);
	if (!out.is_open()) {
		cout << "File is open fail!" << endl;
		Result = -1;
	}
	switch (protocol) {
	case PROTOCOL_THUNDER:
		l_it = ThunderHotList.begin();
		while (l_it != ThunderHotList.end()) {
			out.write((char*) l_it->first.c_str(), sizeof(char *));
			out.write((char*) l_it->second, sizeof(hotItem_t *));
			l_it++;
		}
		break;

	case PROTOCOL_HTTP:
		l_it = HttpHotList.begin();
		while (l_it != HttpHotList.end()) {
			out.write((char*) l_it->first.c_str(), sizeof(char *));
			out.write((char*) l_it->second, sizeof(hotItem_t *));
			l_it++;
		}
		break;

	default:
		break;
	}
	out.close();
	WriteRunInfo::WriteLog(" hotlist svaeEntry FINISH!...");
	return Result;
}

