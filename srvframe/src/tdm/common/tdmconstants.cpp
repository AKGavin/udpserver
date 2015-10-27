/*
 * tdmconstants.cpp
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#include "tdmconstants.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>

extern int sessionId;


int getDTMCmdTypeForWhiteItem( protocol_type_t proto, pkg_type_t type );



int GetRecvThreadEntityType( protocol_type_t proto )
{
	int threadEntity = 0;
	switch ( proto ) {
	case PROTOCOL_HTTP: {
		threadEntity = RECV_HTTP_THREADENTITY;
		break;
	}
	case PROTOCOL_THUNDER: {
		threadEntity = RECV_THUNDER_THREADENTITY;
		break;
	}
	case PROTOCOL_DCM: {
		threadEntity = RECV_DCM_THREADENTITY;
		break;
	}
	case PROTOCOL_MAINTAIN: {
		threadEntity = RECV_MAINTAIN_THREADENTITY;
		break;
	}
	default: {
	}
	}
	return threadEntity;
}


int GetSendThreadEntityType( protocol_type_t proto )
{
	int threadEntity = 0;
	switch ( proto ) {
	case PROTOCOL_HTTP: {
		threadEntity = SEND_HTTP_THREADENTITY;
		break;
	}
	case PROTOCOL_THUNDER: {
		threadEntity = SEND_THUNDER_THREADENTITY;
		break;
	}
	case PROTOCOL_DCM:
		threadEntity = SEND_DCM_THREADENTITY;
		break;
	case PROTOCOL_MAINTAIN:
		threadEntity = SEND_MAINTAIN_THREADENTITY;
		break;
	default: {
	}
	}
	return threadEntity;
}


int composeDTMAddWhiteItemPacket( tdm_dtm_whiteitem_t* sendPacket, protocol_type_t proto, char* infohash, whiteList_data_t* whiteItem )
{
	if ( whiteItem != NULL && whiteItem->status == WHITELIST_STATUS_OK ) {
		sendPacket->cmd = getDTMCmdTypeForWhiteItem( proto, CMD_DCM_TDM_CONTENT_ADD );

		//convert 40 bytes infohash to 20 bytes
		hexStringToByte( sendPacket->infohash, infohash );
		sendPacket->detect_time = whiteItem->updateTime;
		sendPacket->protocol = proto;
		sendPacket->checksum = calc_hash((const unsigned char *)sendPacket, 8);

		return 0;
	}
	else {
		return -1;
	}
}


int composeDTMDelWhiteItemPacket( tdm_dtm_whiteitem_t* sendPacket, protocol_type_t proto, char* infohash, whiteList_data_t* whiteItem )
{
	if ( whiteItem != NULL ) {
		sendPacket->cmd = getDTMCmdTypeForWhiteItem( proto, CMD_DCM_TDM_CONTENT_DEL );

		//convert 40 bytes infohash to 20 bytes
		hexStringToByte( sendPacket->infohash, infohash );

		//TODO: detect time??
		sendPacket->detect_time = whiteItem->updateTime;

		sendPacket->checksum = calc_hash( (const unsigned char*) sendPacket, 8 );
		return 0;
	}
	else {
		return -1;
	}
}


int composeDTMDisableItemPacket( tdm_dtm_disableitem_t* sendPacket, protocol_type_t proto, char* infohash, whiteList_data_t* whiteItem )
{
	if ( whiteItem != NULL && whiteItem->status == WHITELIST_STATUS_DISABLED ) {
		sendPacket->cmd = getDTMCmdTypeForWhiteItem( proto, CMD_DCM_TDM_CONTENT_DISABLED );
		hexStringToByte( sendPacket->infohash, infohash );
		sendPacket->protocol = proto;
		sendPacket->checksum = calc_hash( (const unsigned char*)sendPacket, 8 );
		return 0;
	}
	else {
		return -1;
	}
}


int getDTMCmdTypeForWhiteItem( protocol_type_t proto, pkg_type_t type )
{
	if ( type == CMD_DCM_TDM_CONTENT_ADD ) {
		return CMD_TDM2DTM_ADD_WHITEITEM;
	}
	else if ( type == CMD_DCM_TDM_CONTENT_DEL ) {
		return CMD_TDM2DTM_DEL_WHITEITEM;
	}
	else if ( type == CMD_DCM_TDM_CONTENT_DISABLED ) {
		return CMD_TDM2DTM_ADD_DISABLEDITEM;
	}
	else {
		return -1;
	}

//	switch ( proto ) {
//	case PROTOCOL_HTTP: {
//		if ( type == CMD_DCM_TDM_CONTENT_ADD ) {
//			return CMD_TDM2DTM_HTTP_ADD_WHITEITEM;
//		}
//		else if ( type == CMD_DCM_TDM_CONTENT_DEL ) {
//			return CMD_TDM2DTM_HTTP_DEL_WHITEITEM;
//		}
//		else if ( type == CMD_DCM_TDM_CONTENT_DISABLED ) {
//			return CMD_TDM2DTM_HTTP_ADD_DISABLEDITEM;
//		}
//		else {
//			return -1;
//		}
//	}
//	case PROTOCOL_THUNDER: {
//		if ( type == CMD_DCM_TDM_CONTENT_ADD ) {
//			return CMD_TDM2DTM_THUNDER_ADD_WHITEITEM;
//		}
//		else if ( type == CMD_DCM_TDM_CONTENT_DEL ) {
//			return CMD_TDM2DTM_THUNDER_DEL_WHITEITEM;
//		}
//		else if ( type == CMD_DCM_TDM_CONTENT_DISABLED ) {
//			return CMD_TDM2DTM_THUNDER_ADD_DISABLEDITEM;
//		}
//		else {
//			return -1;
//		}
//	}
//	default: {
//		return -1;
//	}
//	}
}



int SendUdpPacket( char* serverAddr, int port, char* buffer, int len )
{
	struct sockaddr_in si_other;

	int s = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if ( s == -1 ) {
		return -1;
	}

	memset( (char*)&si_other, 0, sizeof(si_other) );
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(port);
	int ret = inet_aton( serverAddr, &si_other.sin_addr);
	if ( ret == 0 ) {
		return -1;
	}

	ret = sendto( s, buffer, len, 0, (struct sockaddr*) &si_other, sizeof(si_other) );
	if ( ret == -1 ) {
		return -1;
	}

	close( s );

	return 0;
}


int SendUdpPacketWithTimeout( int socketfd, char* serverAddr, int port, char* buffer, int len, int timeout )
{
	if ( socketfd < 0 ) return -1;

	struct sockaddr_in si_other;
	memset( (char*)&si_other, 0, sizeof(si_other) );
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(port);
	int ret = inet_aton( serverAddr, &si_other.sin_addr);
	if ( ret == 0 ) {
		return -1;
	}

	setsockopt( socketfd, SOL_SOCKET, SO_SNDTIMEO, (char*) &timeout, sizeof(int) );

	ret = sendto( socketfd, buffer, len, 0, (struct sockaddr*) &si_other, sizeof(si_other) );
	if ( ret == -1 ) {
		return -1;
	}

	return 0;
}


int SendUdpPacketWithTimeout2( int socketfd, unsigned int ip, int port, char* buffer, int len, int timeout )
{
	if ( socketfd < 0 ) return -1;

	struct sockaddr_in si_other;
	memset( (char*)&si_other, 0, sizeof(si_other) );
	si_other.sin_family = AF_INET;
	si_other.sin_addr.s_addr = htonl( ip );
	si_other.sin_port = htons(port);

	setsockopt( socketfd, SOL_SOCKET, SO_SNDTIMEO, (char*) &timeout, sizeof(int) );

	int ret = sendto( socketfd, buffer, len, 0, (struct sockaddr*) &si_other, sizeof(si_other) );
	if ( ret == -1 ) {
		return -1;
	}

	return 0;
}


int getSessionId()
{
	sessionId++;
	if ( sessionId > 99999999 ) sessionId = 0;
	return sessionId;
}

int parseHostList( list<host_t*>& hostList, char* strHost )
{
	if ( strHost == NULL ) return 0;

	while ( true ) {
		char* end = strstr( strHost, ";");
		if ( NULL != end ) {
			*end = '\0';
		}

		char* strPort = strstr( strHost, ":");
		if ( NULL != strPort ) {
			*strPort = '\0';
			strPort++;
		}

		if ( strlen(strHost) > 0 && strlen(strPort)>0 ) {
			host_t* host = (host_t*) malloc( sizeof(host_t) );
			char* s = (char*) calloc( sizeof(char), strlen(strHost) + 1 );
			strcpy( s, strHost );
			host->sServerAddr = s;
			host->nPort = atoi( strPort );
			hostList.push_back( host );
		}

		if ( NULL == end ) {
			break;
		}
		else {
			strHost = end+1;
		}
	}
	return 0;
}


unsigned char toByte(char c)
{
  unsigned char value = 0;

  if (c >= '0' && c <= '9')
  value = c - '0';
  else if (c >= 'A' && c <= 'Z')
  value = c - 'A' + 10;
  else if (c >= 'a' && c <= 'z')
  value = c - 'a' + 10;

  return value;
}


void hexStringToByte(unsigned char *dstByte,const char *srcHexString)
{
  int index;

  for (int i = 0; i < 20; i++){
    index = i * 2;
    dstByte[i] = ((toByte(srcHexString[index]))<<4) | toByte(srcHexString[index+1]);
  }
}


void ByteHash2HexString(char * dstHexString,unsigned char * srcByte)
{
	for(int i=0;i<20;i++) {
		sprintf(dstHexString + i*2,"%02X",srcByte[i]);
	}
}


unsigned int BKDRHash( char* str )
{
	unsigned int seed = 131;
	unsigned int hash = 0;

	while ( *str ) {
		hash = hash * seed + (*str++);
	}

	return (hash & 0x7FFFFFFF);
}


/* Calc hash for a key */
unsigned int calc_hash(const unsigned char *key, unsigned int length)
{
    register unsigned int nr = 1, nr2 = 4;
	/* The hash implementation comes from calc_hashnr() in mysys/hash.c. */

	while (length--)
	{
		nr ^= (((nr & 63) + nr2) * ((unsigned int) (unsigned char) *key++))
				+ (nr << 8);
		nr2 += 3;
	}

	return ((unsigned int) nr);
}
