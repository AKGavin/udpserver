/*
 * tdmconstants.h
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */
#include <string>
#include <iostream>
#include <netinet/in.h>
#include <list>

using namespace std;

#ifndef TDMCONSTANTS_H_
#define TDMCONSTANTS_H_

#define MSG_BUFFER_LEN 1024

#define RECV_START_THREADENTITY 10
#define RECV_HTTP_THREADENTITY 11
#define RECV_THUNDER_THREADENTITY 12
#define RECV_DCM_THREADENTITY 47
#define RECV_MAINTAIN_THREADENTITY 48
#define RECV_END_THREADENTITY 49

#define SEND_HTTP_THREADENTITY 50
#define SEND_THUNDER_THREADENTITY 51
#define SEND_DCM_THREADENTITY 97
#define SEND_MAINTAIN_THREADENTITY 98
#define SEND_END_THREADENTITY 99

#define CMD_HEADER_SIZE (sizeof(int) * 2 + sizeof(unsigned int) + sizeof(pkg_type_t) + sizeof(protocol_type_t))
#define MONITOR_CMD_HEADER_SIZE (sizeof(int) * 2 + sizeof(unsigned int) + sizeof(pkg_type_t))

#pragma pack(push,1)

typedef enum {
	SERVER_STATUS_OK,
	SERVER_STATUS_ERROR
} server_status_t;

typedef struct {
	string* m_sServerAddr;
	int m_nPort;
	int m_nIp;
	server_status_t m_nStatus;
} server_info_t;


typedef unsigned char url_hash_t[20];



////////////////////////////////////////////////////////////////////////////
//constants for thunder protocol


typedef enum {
	DTM_PUT1,
	DTM_PUT2,
	DCM_GET,
	DCM_GET_RES
} thunder_req_type_t;


//struct for TDM request
//typedef struct {
//	thunder_req_type_t m_nReqType;
//	char m_strInfohash[41];
//	char m_strCid[41];
//	char m_strPeerId[17];
//	unsigned long long m_nFilesize;
//} thunder_data_t;


//struct for other request
typedef struct {
	thunder_req_type_t m_nReqType;
	url_hash_t m_strInfohash;
	char m_strCid[40];
	char m_strPeerId[16];
	unsigned long long m_nFilesize;
} thunder_data_t;


////////////////////////////////////////////////////////////////////////////
//constants for http protocol

//typedef struct http_req {
//	char* client_ip;
//	char* server_ip;
//	char* sitename;
//	char* urlhash;
//	char* req_url;
//	char* req_agent;
//	char* req_refer;
//	char* req_cookie;
//} http_data_t;


typedef struct {
	url_hash_t urlhash;
	char* client_ip;
	char* server_ip;
	char* sitename;
	char* req_url;
	char* req_agent;
	char* req_refer;
	char* req_cookie;
} http_data_t;

/////////////////////////////////////////////////////////////////////////////
//constants for interchange packet

typedef enum {
	//
	CMD_OK,
	CMD_RETRY,
	CMD_USED,
	//
	CMD_SEND_AAC_LOG,
	//
	CMD_DTM_TDM_REQ_REG,
	//
	CMD_TDM_DTM_WHITE_ADD,
	CMD_TDM_DTM_WHITE_DEL,
	CMD_TDM_DTM_BLACK_ADD,
	CMD_TDM_DTM_BLACK_DEL,
	//
	CMD_CHANGE_DCM_SERVER,
	CMD_CHANGE_TDM_SERVER,
	//
	CMD_TDM_DCM_CONTENT_GET,
	//
	CMD_DCM_TDM_CONTENT_ADD,
	CMD_DCM_TDM_CONTENT_DEL,
	CMD_DCM_TDM_CONTENT_DISABLED,
	CMD_DCM_TDM_CONTENT_THUNDER_QUERY,
	CMD_DCM_TDM_CONTENT_THUNDER_QUERY_REPLY,

	CMD_CKS_HEART = 90,
	CMD_CKS_ACK,
	CMD_CKS_CHECK,
	CMD_CKS_DOWN_REVIVE
} pkg_type_t;


typedef enum {
	PROTOCOL_BT = 0,
	PROTOCOL_EMULE = 1,
	PROTOCOL_HTTP = 2,
	PROTOCOL_EMULE_CRYPT = 3,
	PROTOCOL_THUNDER = 5,
	PROTOCOL_PPSTREAM = 13,
	PROTOCOL_PPLIVE = 15,
	PROTOCOL_QQLIVE = 16,
	PROTOCOL_BF = 17,
	PROTOCOL_DCM = 99,
	PROTOCOL_MAINTAIN
} protocol_type_t;


typedef enum {
	WHITEITEM_FROM_DTM_TASK,
	WHITEITEM_FROM_DCM_DOWNLOAD
} whiteItem_from;

typedef struct {
	url_hash_t infohash;
	unsigned long ip;
} dcm_tdm_white_data_t;

typedef struct {
	url_hash_t infohash;
} tdm_dcm_white_confirm_data_t;

typedef struct {
	url_hash_t infohash;
} dcm_tdm_disable_data_t;

typedef struct {
	url_hash_t infohash;
} tdm_dcm_disable_add_confirm_data_t;


//typedef struct {
//	protocol_type_t type;
//	void* data;
//} pkg_data_t;





///////////////////////////////////////////////////////////////////////////////////////////
// DTM <=> TDM
///////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
	unsigned char cmd;
	unsigned char infohash[20];
	time_t detect_time;
	protocol_type_t protocol;
	unsigned int checksum;
} tdm_dtm_whiteitem_t;


typedef struct {
	char cmd;
	unsigned char infohash[20];
	protocol_type_t protocol;
	unsigned int checksum;
} tdm_dtm_disableitem_t;


//TODO: define values!
#define CMD_TDM2DTM_ADD_WHITEITEM 0xd2
#define CMD_TDM2DTM_DEL_WHITEITEM 0xd3
#define CMD_TDM2DTM_ADD_DISABLEDITEM 0xd4
//#define CMD_TDM2DTM_THUNDER_ADD_WHITEITEM 0xb2
//#define CMD_TDM2DTM_THUNDER_DEL_WHITEITEM 0xb3
//#define CMD_TDM2DTM_THUNDER_ADD_DISABLEDITEM 0x02
//
//#define CMD_TDM2DTM_HTTP_ADD_WHITEITEM 0xd1
//#define CMD_TDM2DTM_HTTP_DEL_WHITEITEM 0xd2
//#define CMD_TDM2DTM_HTTP_ADD_DISABLEDITEM 0xd3



typedef enum {
	WHITELIST_STATUS_PENDING,
	WHITELIST_STATUS_OK,
	WHITELIST_STATUS_DISABLED
} whiteList_status_t;



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////maintain constants

typedef enum {
	CON_DCM,
	CON_TDM,
	CON_LOG,
	CON_DTM
} heart_type_t;

typedef enum {
	CON_DOWN,
	CON_REVIVE
} down_revive_type_t;


typedef struct {
	heart_type_t serverType;
	int status;
	int port;
	int ip;
	protocol_type_t proto;
} cks_heart_data_t;


typedef struct {
	heart_type_t serverType;
	down_revive_type_t type;
	int port;
	int ip;
	protocol_type_t proto;
} cks_down_revive_data_t;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
	char* sServerAddr;
	unsigned int nPort;
} host_t;


typedef enum {
	LB_ROTATION,
	LB_HASH
} lb_type_t;


typedef struct {
	protocol_type_t type;
	union {
		http_data_t http_data;
		thunder_data_t thunder_data;
		dcm_tdm_white_data_t white_data;
		dcm_tdm_disable_data_t black_data;
		url_hash_t infohash_data;
	};
} pkg_data_t;


typedef struct {
	char infohash[41];
	whiteList_status_t status;
	time_t createTime;
	time_t updateTime;
	unsigned long dcmIp;
	int dcmPort;
	pkg_data_t data;
} whiteList_data_t;


typedef struct {
	int s_id;	//session id
	int pkg_len;
	unsigned int pkg_chk;
	pkg_type_t pkg_type;
	pkg_data_t pkg_data;
} cmd_data_t;


typedef struct {
	union {
		cks_heart_data_t heart_data;
		cks_down_revive_data_t down_data;
	};
} monitor_pkg_data_t;



typedef struct {
	int s_id;	//session id
	int pkg_len;
	unsigned int pkg_chk;
	pkg_type_t pkg_type;
	monitor_pkg_data_t pkg_data;
} monitor_cmd_data_t;


#pragma pack(pop)

//common functions
int GetRecvThreadEntityType( protocol_type_t proto );
int GetSendThreadEntityType( protocol_type_t proto );

void hexStringToByte(unsigned char *dstByte,const char *srcHexString);
void ByteHash2HexString(char * dstHexString,unsigned char * srcByte);

int SendUdpPacket( char* serverAddr, int port, char* buffer, int len );
int SendUdpPacketWithTimeout( int socketfd, char* serverAddr, int port, char* buffer, int len, int timeout );
int SendUdpPacketWithTimeout2( int socketfd, unsigned int ip, int port, char* buffer, int len, int timeout );

int parseHostList( list<host_t*>& hostList, char* strHost );
int getSessionId();

unsigned int BKDRHash( char* str );
unsigned int calc_hash(const unsigned char *key, unsigned int length);

int composeDTMAddWhiteItemPacket( tdm_dtm_whiteitem_t* sendPacket, protocol_type_t proto, char* infohash, whiteList_data_t* whiteItem );
int composeDTMDelWhiteItemPacket( tdm_dtm_whiteitem_t* sendPacket, protocol_type_t proto, char* infohash, whiteList_data_t* whiteItem );
int composeDTMDisableItemPacket( tdm_dtm_disableitem_t* sendPacket, protocol_type_t proto, char* infohash, whiteList_data_t* whiteItem );


#endif /* TDMCONSTANTS_H_ */
