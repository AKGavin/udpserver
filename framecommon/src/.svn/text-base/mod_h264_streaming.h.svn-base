#pragma once
#include "moov.h"
//the caller SHOULD delete the header_data via "delete [] header_data"!
int BuildH264SeekHeader(/*in*/const char *file_name, 
 						/*in*/double start, /*in*/double end,
 						/*out*/char **header_data, /*out*/unsigned int *header_size,
 						/*out*/unsigned int *start_pos, /*out*/unsigned int *data_len);

//the caller SHOULD delete the header_data via "delete [] header_data"!
int BuildH264SeekHeader(/*in*/const char *media_header_buf, /*in*/unsigned int media_header_size,
						/*in*/__int64 file_size, 
						/*in*/double start, /*in*/double end,
						/*out*/char **header_data, /*out*/unsigned int *header_size,
						/*out*/unsigned int *start_pos, /*out*/unsigned int *data_len);



