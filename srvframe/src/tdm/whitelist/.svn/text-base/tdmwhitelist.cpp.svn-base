/*
 * tdmwhitelist.cpp
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */


#include "tdmwhitelist.h"
#include "mainserver.h"
#include <mysql/mysql.h>
#include <stdlib.h>


static time_t getsecondtime(char *strsectime)
{
	int yeartime, montime, datetime, hourtime, mintime, sectime;
// 2014-05-08 15:14:14
	char strtime[30];
	memset( strtime, 0, 30 );
	strcpy(strtime, strsectime);

	char* start = strtime;
	char* end = start + 4;
	*end = '\0';
	yeartime = atoi( start );

	start = end + 1;
	end = start + 2;
	*end = '\0';
	montime = atoi( start );

	start = end + 1;
	end = start + 2;
	*end = '\0';
	datetime = atoi( start );

	start = end + 1;
	end = start + 2;
	*end = '\0';
	hourtime = atoi( start );

	start = end + 1;
	end = start + 2;
	*end = '\0';
	mintime = atoi( start );

	start = end + 1;
	end = start + 2;
	*end = '\0';
	sectime = atoi( start );

	struct tm update;
	update.tm_year = yeartime - 1900;
	update.tm_mon = montime - 1;
	update.tm_mday = datetime;
	update.tm_hour = hourtime;
	update.tm_min = mintime;
	update.tm_sec = sectime;
	time_t updatet = mktime(&update);

	return updatet;
}


//error : -1; ok: 0
int CTDMWhiteList::loadItemFromDB( protocol_type_t proto, char* infohash, whiteList_data_t** whitelistItem )
{
	char* tableName = NULL;
	char* fieldName = NULL;
	int ret = getTableName( proto, &tableName, &fieldName );
	if ( ret == -1 || tableName == NULL || fieldName == NULL ) return -1;

	whiteList_data_t* tdmwhiteItem = NULL;

	char pszcSql[1024];
	memset( pszcSql, 0, 1024 );
	sprintf(pszcSql,"select * from %s where %s = '%s'", tableName, fieldName, infohash );
	free( tableName );
	free( fieldName );

	CMainServer* server = (CMainServer*) CMainServer::GetInstance();
	CMysqlWrapper* tdmmysqlwraper = server->OpenConnection();
	if (!tdmmysqlwraper->GetResult(pszcSql)) {
		string error = tdmmysqlwraper->GetLastError();
		WriteRunInfo::WriteLog( "[CTDMWhiteList] GetResult error, %s, %s", error.c_str(), pszcSql );
		tdmmysqlwraper->SetLastError("GetResult is failed!");
		server->CloseConnection( tdmmysqlwraper );
		*whitelistItem = NULL;
		return -1;
	}

	if (!tdmmysqlwraper->FetchRow()) {
		string error = tdmmysqlwraper->GetLastError();
		WriteRunInfo::WriteLog( "[CTDMWhiteList] no row fetched, %s, %s", error.c_str(), pszcSql );
		server->CloseConnection( tdmmysqlwraper );
		*whitelistItem = NULL;
		return 0;
	}

	tdmwhiteItem = new whiteList_data_t();
	tdmwhiteItem->data.type = proto;
	FillWhitelistItem( tdmmysqlwraper, proto, tdmwhiteItem );
	*whitelistItem = tdmwhiteItem;

	tdmmysqlwraper->FreeResult();
	server->CloseConnection( tdmmysqlwraper );
	return 0;
}


int CTDMWhiteList::insertItemToDB( protocol_type_t proto, whiteList_data_t* whitelistItem )
{
	if(! whitelistItem ) return -1;

	thunder_data_t *thunderdata;
	http_data_t    *httpdata;
	char  pszcSql[4096] = {0};

	struct tm *t = localtime( &whitelistItem->updateTime );
	char updateTime[24] = {0};
	sprintf(updateTime,"%04d-%02d-%02d %02d:%02d:%02d",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);

	t = localtime( &whitelistItem->createTime );
	char createTime[24] = {0};
	sprintf(createTime,"%04d-%02d-%02d %02d:%02d:%02d",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);

	CMainServer* server = (CMainServer*) CMainServer::GetInstance();
	CMysqlWrapper* tdmmysqlwraper = server->OpenConnection();

	switch(proto){
		case PROTOCOL_THUNDER:
		{
			thunderdata = & whitelistItem->data.thunder_data;

			char cid[41];
			char peerId[17];
			strncpy( cid, thunderdata->m_strCid, 40 );
			cid[40] = '\0';
			strncpy( peerId, thunderdata->m_strPeerId, 16 );
			peerId[16] = '\0';

			sprintf(pszcSql,"insert into data_thunder (dcmip, dcmport, updatedate, reqtype, infohash, cid, peerid, filesize, status, createdate) values "\
					"('%lu',%d,'%s',%d,'%s','%s','%s',%llu,%d, '%s')",
					whitelistItem->dcmIp,whitelistItem->dcmPort, updateTime, thunderdata->m_nReqType,\
					whitelistItem->infohash,cid,peerId, thunderdata->m_nFilesize,whitelistItem->status, createTime);
			break;
		}
		case PROTOCOL_HTTP:
		{
			httpdata = & whitelistItem->data.http_data;
			sprintf(pszcSql,"INSERT INTO data_http (dcmip, dcmport, updatedate, client_ip, server_ip, sitename, urlhash, req_url, req_agent, req_refer, req_cookie, status, createdate ) VALUES "\
					"(%lu,%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s',%d, '%s')",
					whitelistItem->dcmIp,whitelistItem->dcmPort,updateTime,httpdata->client_ip,httpdata->server_ip,httpdata->sitename,
					whitelistItem->infohash, httpdata->req_url,httpdata->req_agent,httpdata->req_refer,httpdata->req_cookie, whitelistItem->status, createTime);
		 	break;
		}
		case PROTOCOL_PPLIVE:
		case PROTOCOL_PPSTREAM:
		case PROTOCOL_BF:
		{
			char* tableName = NULL;
			char* fieldName = NULL;
			getTableName( proto, &tableName, &fieldName );
			sprintf( pszcSql, "insert into %s (infohash, status, updatedate, createdate) values ('%s', %d, '%s', '%s')",
					tableName, whitelistItem->infohash, whitelistItem->status, updateTime, createTime );
			free( tableName );
			free( fieldName );
			break;
		}
		default: {
			server->CloseConnection( tdmmysqlwraper );
			return -1;
		}
		}


	if(!tdmmysqlwraper->Execute(pszcSql)) {
		string error = tdmmysqlwraper->GetLastError();
		WriteRunInfo::WriteLog( "[CTDMWhiteList] Execute error, %s, %s", error.c_str(), pszcSql );
		server->CloseConnection( tdmmysqlwraper );
 		return -1;
 	}

 	tdmmysqlwraper->FreeResult();
	server->CloseConnection( tdmmysqlwraper );
	return 0;
}


int CTDMWhiteList::updateItemToDB( protocol_type_t proto, char* infohash, whiteList_status_t status, unsigned long dcmIp, time_t updateTime )
{
	char strTime[24];
	memset( strTime, 0, 24 );
	struct tm *t = localtime( &updateTime );
	sprintf(strTime,"%04d-%02d-%02d %02d:%02d:%02d",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);

	char* tableName = NULL;
	char* fieldName = NULL;
	int ret = getTableName( proto, &tableName, &fieldName );
	if ( ret == -1 ) return -1;

	char  sql[1024];
	memset( sql, 0, 1024 );

	if ( proto == PROTOCOL_THUNDER || proto == PROTOCOL_HTTP ) {
		sprintf( sql, "update %s set status = %d, dcmip=%lu, updatedate='%s' where %s = '%s'",
				tableName, status, dcmIp, strTime, fieldName, infohash );
	}
	else {
		sprintf( sql, "update %s set status = %d, updatedate='%s' where %s = '%s'",
				tableName, status, strTime, fieldName, infohash );
	}
	free( tableName );
	free( fieldName );

	CMainServer* server = (CMainServer*) CMainServer::GetInstance();
	CMysqlWrapper* tdmmysqlwraper = server->OpenConnection();

	if(!tdmmysqlwraper->Execute(sql)) {
		string error = tdmmysqlwraper->GetLastError();
		WriteRunInfo::WriteLog( "[CTDMWhiteList] Execute error, %s, %s", error.c_str(), sql );
		server->CloseConnection( tdmmysqlwraper );
		return -1;
	}
	else {
		server->CloseConnection( tdmmysqlwraper );
		return 0;
	}
}


int CTDMWhiteList::deleteItemFromDB( protocol_type_t proto, char* infohash )
{
	CMainServer* server = (CMainServer*) CMainServer::GetInstance();
	CMysqlWrapper* tdmmysqlwraper = server->OpenConnection();

//		if(!tdmmysqlwraper->Connect()) {
//			cout<<"mysql connect failed!"<<endl;
//			server->CloseConnection( tdmmysqlwraper );
//			return -1;
//		}

	char* tableName = NULL;
	char* fieldName = NULL;
	int ret = getTableName( proto, &tableName, &fieldName );
	if ( ret == -1 ) return ret;

	char sql[1024];
	memset( sql, 0, 1024 );
	sprintf(sql, "delete from %s where %s = '%s'", tableName, fieldName, infohash );
	free( tableName );
	free( fieldName );

	if (!tdmmysqlwraper->Execute(sql)) {
		server->CloseConnection(tdmmysqlwraper);
		string error = tdmmysqlwraper->GetLastError();
		WriteRunInfo::WriteLog("[CTDMWhiteList] Execute error, %s, %s",
				error.c_str(), sql);
		return -1;
	}

	tdmmysqlwraper->FreeResult();
	server->CloseConnection(tdmmysqlwraper);
	return 0;
}



//int CTDMWhiteList::saveAllEntriesToDB( protocol_type_t protocol )
//{
//	return 0;
//}


//int CTDMWhiteList::loadAllEntriesFromDB( protocol_type_t proto )
//{
//	char* tableName = NULL;
//	char* fieldName = NULL;
//	int ret = getTableName( proto, &tableName, &fieldName );
//	if ( ret == -1 ) return -1;
//
//	char sql[1024];
//	memset( sql, 0, 1024 );
//	sprintf( sql, "select * from %s", tableName );
//	free( tableName );
//	free( fieldName );
//
//	CMainServer* server = (CMainServer*) CMainServer::GetInstance();
//	CMysqlWrapper* tdmmysqlwraper = server->OpenConnection();
//
//	if( !tdmmysqlwraper->GetResult(sql)) {
//		string error = tdmmysqlwraper->GetLastError();
//		WriteRunInfo::WriteLog( "[CTDMWhiteList] Execute error, %s, %s", error.c_str(), sql );
//		server->CloseConnection( tdmmysqlwraper );
//		return -1;
//	}
//
//	int count = 0;
//	whiteList_data_t* tdmwhiteItem = NULL;
//	while ( tdmmysqlwraper->FetchRow() ) {
//		switch(proto){
//		case PROTOCOL_THUNDER: {
//			tdmwhiteItem = new whiteList_data_t();
//			tdmwhiteItem->data.type = PROTOCOL_THUNDER;
//			thunder_data_t* newData = (thunder_data_t*) malloc(sizeof(thunder_data_t));
//			tdmwhiteItem->data.data = newData;
//			FillWhitelistItem(tdmmysqlwraper, proto, tdmwhiteItem);
//
//			if ( !(ThunderWhiteList[newData->m_strInfohash] = tdmwhiteItem) ) {
//				free( newData );
//				delete tdmwhiteItem;
//			}
//			else {
//				count++;
//			}
//		break;
//		}
//		case PROTOCOL_HTTP: {
//			tdmwhiteItem = new whiteList_data_t();
//			tdmwhiteItem->data.type = PROTOCOL_HTTP;
//			http_data_t* newData = (http_data_t*) malloc(sizeof(http_data_t));
//			tdmwhiteItem->data.data = newData;
//			FillWhitelistItem(tdmmysqlwraper, proto, tdmwhiteItem);
//
//			if (!(HttpWhiteList[newData->urlhash] = tdmwhiteItem)) {
//				if (newData->client_ip != NULL)
//					free(newData->client_ip);
//				if (newData->server_ip != NULL)
//					free(newData->server_ip);
//				if (newData->sitename != NULL)
//					free(newData->sitename);
//				if (newData->urlhash != NULL)
//					free(newData->urlhash);
//				if (newData->req_url != NULL)
//					free(newData->req_url);
//				if (newData->req_agent != NULL)
//					free(newData->req_agent);
//				if (newData->req_refer != NULL)
//					free(newData->req_refer);
//				if (newData->req_cookie != NULL)
//					free(newData->req_cookie);
//				free(newData);
//				delete tdmwhiteItem;
//				server->CloseConnection( tdmmysqlwraper );
//				return -1;
//			}
//			else {
//				count++;
//			}
//			break;
//		}
//		default :
//			break;
//	}
//	}
//
//	server->CloseConnection( tdmmysqlwraper );
//	return count;
//}


int CTDMWhiteList::getTableName( protocol_type_t proto, char** tableName, char** fieldName )
{
	*tableName = (char*) malloc(21);
	memset( *tableName, 0, 21 );

	*fieldName = (char*) malloc(21);
	memset( *fieldName, 0, 21 );

	switch ( proto ) {
	case PROTOCOL_THUNDER: {
		strcpy( *tableName, "data_thunder" );
		strcpy( *fieldName, "infohash" );
		break;
	}
	case PROTOCOL_HTTP: {
		strcpy( *tableName, "data_http" );
		strcpy( *fieldName, "urlhash" );
		break;
	}
	case PROTOCOL_PPSTREAM: {
		strcpy( *tableName, "data_ppstream" );
		strcpy( *fieldName, "infohash" );
		break;
	}
	case PROTOCOL_PPLIVE: {
		strcpy( *tableName, "data_pplive" );
		strcpy( *fieldName, "infohash" );
		break;
	}
	case PROTOCOL_BF: {
		strcpy( *tableName, "data_bf" );
		strcpy( *fieldName, "infohash" );
		break;
	}
	default: {
		free( *tableName );
		*tableName = NULL;
		free( *fieldName );
		fieldName = NULL;
		return -1;
	}
	}

	return 0;
}


int CTDMWhiteList::FillWhitelistItem( CMysqlWrapper* wrapper, protocol_type_t proto, whiteList_data_t* whitelistItem )
{
	char tmp[40];
	GetFieldValue( wrapper, "status", &whitelistItem->status, TYPE_INT, "0");
	GetFieldValue( wrapper, "updatedate", &whitelistItem->updateTime, TYPE_DATETIME, "0");
	GetFieldValue( wrapper, "createdate", &whitelistItem->createTime, TYPE_DATETIME, "0");
	memset( whitelistItem->infohash, 0, 41 );

	switch ( proto ) {
	case PROTOCOL_THUNDER: {
		GetFieldValue( wrapper, "dcmip", &whitelistItem->dcmIp, TYPE_LONG, "0");
		GetFieldValue( wrapper, "dcmport", &whitelistItem->dcmPort, TYPE_INT, "0");

		thunder_data_t * thunderData = & whitelistItem->data.thunder_data;
		GetFieldValue( wrapper, "infohash", tmp,TYPE_CHARARRAY, NULL);
		hexStringToByte( thunderData->m_strInfohash, tmp );
		strncpy( whitelistItem->infohash, tmp, 40 );

		GetFieldValue( wrapper, "reqtype", &thunderData->m_nReqType, TYPE_INT, "0");
		GetFieldValue( wrapper, "cid", thunderData->m_strCid, TYPE_CHARARRAY, NULL);
		GetFieldValue( wrapper, "peerid", thunderData->m_strPeerId, TYPE_CHARARRAY, NULL);
		GetFieldValue( wrapper, "filesize", &thunderData->m_nFilesize, TYPE_LONG, NULL);

		WriteRunInfo::WriteLog( "[CTDMWhiteList] load row: dcmip=%lu, dcmPort=%d, status=%d, updateTime=%ld, createTime=%ld, infohash=%s",
				whitelistItem->dcmIp, whitelistItem->dcmPort, whitelistItem->status, whitelistItem->updateTime,
				whitelistItem->createTime, whitelistItem->infohash );
		break;
	}
	case PROTOCOL_HTTP: {
		GetFieldValue( wrapper, "dcmip", &whitelistItem->dcmIp, TYPE_LONG, "0");
		GetFieldValue( wrapper, "dcmport", &whitelistItem->dcmPort, TYPE_INT, "0");

		http_data_t * httpData = & whitelistItem->data.http_data;
		GetStringFieldValue( wrapper, "client_ip", &httpData->client_ip, NULL);
		GetStringFieldValue( wrapper, "server_ip", &httpData->server_ip, NULL);
		GetStringFieldValue( wrapper, "sitename", &httpData->sitename, NULL);

		GetFieldValue( wrapper, "urlhash", tmp,TYPE_CHARARRAY, NULL);
		hexStringToByte( httpData->urlhash, tmp );
		strncpy( whitelistItem->infohash, tmp, 40 );

		GetStringFieldValue( wrapper, "req_url", &httpData->req_url, NULL);
		GetStringFieldValue( wrapper, "req_agent", &httpData->req_agent, NULL);
		GetStringFieldValue( wrapper, "req_refer", &httpData->req_refer, NULL);
		GetStringFieldValue( wrapper, "req_cookie", &httpData->req_cookie, NULL);
		break;
	}
	default: {
		GetFieldValue( wrapper, "infohash", tmp, TYPE_CHARARRAY, NULL);
		hexStringToByte( whitelistItem->data.infohash_data, tmp );
		strncpy( whitelistItem->infohash, tmp, 40 );
		return -1;
	}
	}
	return 1;
}


void CTDMWhiteList::freeWhitelistItem( protocol_type_t proto, whiteList_data_t* whitelistItem )
{
	if ( whitelistItem == NULL ) return;

	if ( proto == PROTOCOL_HTTP ) {
		http_data_t* httpData = & whitelistItem->data.http_data;
		free( httpData->client_ip );
		free( httpData->server_ip );
		free( httpData->sitename );
		free( httpData->req_url );
		free( httpData->req_agent );
		free( httpData->req_refer );
		free( httpData->req_cookie );
	}

	free( whitelistItem );
	whitelistItem = NULL;
}



void CTDMWhiteList::GetFieldValue( CMysqlWrapper* tdmmysqlwraper, const char* name, void* value, field_type_t type, const char* defaultValue )
{
	const char* s = tdmmysqlwraper->GetString( name );
	if ( s != NULL ) {
		if ( type == TYPE_INT ) {
			int* v = (int*)value;
			*v = atoi(s);
		}
		else if ( type == TYPE_LONG ) {
			long* v = (long*)value;
			*v = atol(s);
		}
		else if ( type == TYPE_DATETIME ) {
			time_t* v = (time_t*)value;
			*v = getsecondtime((char *)s);
		}
		else if ( type == TYPE_CHARARRAY ) {
			char* v = (char*) value;
			memcpy( v, s, strlen(s) );
		}
	}
	else {
		if ( type == TYPE_INT ) {
			int* v = (int*)value;
			*v = atoi(defaultValue);
		}
		else if ( type == TYPE_LONG ) {
			long* v = (long*)value;
			*v = atol(defaultValue);
		}
		else if ( type == TYPE_DATETIME ) {
			time_t* v = (time_t*)value;
			*v = 0;
		}
		else if ( type == TYPE_CHARARRAY ) {
			char* v = (char*) value;
			if ( defaultValue ) {
				memcpy( v, defaultValue, strlen(defaultValue) );
			}
			else {
				*v = '\0';
			}
		}
	}
}


void CTDMWhiteList::GetStringFieldValue( CMysqlWrapper* tdmmysqlwraper, const char* name, char** value, const char* defaultValue )
{
	const char* s = tdmmysqlwraper->GetString( name );
	if ( s != NULL ) {
		*value = (char*) calloc( strlen(s) + 1, sizeof(char) );
		strcpy( *value, s );
	}
	else {
		if ( defaultValue == NULL ) {
			*value = NULL;
		}
		else {
			*value = (char*) calloc( strlen(defaultValue) + 1, sizeof(char) );
			strcpy( *value, defaultValue );
		}
	}
}

