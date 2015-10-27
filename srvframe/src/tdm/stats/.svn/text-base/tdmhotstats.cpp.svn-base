/*
 * tdmhotstats.cpp
 *
 *  Created on: Dec 11, 2013
 *      Author: root
 */

#include "tdmhotstats.h"

#define HTTP_THRESHOLD 3
#define THUNDER_THRESHOLD 1
#define DEFAULT_THRESHOLD 1

int CTDMHotStats::getStatsThreshold( protocol_type_t proto )
{
	switch( proto ) {
	case PROTOCOL_HTTP: {
		return HTTP_THRESHOLD;
	}
	case PROTOCOL_THUNDER: {
		return THUNDER_THRESHOLD;
	}
	default: {
		return DEFAULT_THRESHOLD;
	}
	}
}
