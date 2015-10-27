/*******************************************************************************
 mod_h264_streaming.c

 mod_h264_streaming - A lighttpd plugin for pseudo-streaming Quicktime/MPEG4 files.
 http://h264.code-shop.com

 Copyright (C) 2007 CodeShop B.V.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/ 

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "moov.h"
#include "mod_h264_streaming.h"
#include "assert.h"

void write_char(unsigned char* outbuffer, int value)
{
  outbuffer[0] = (unsigned char)(value);
}

void write_int32(unsigned char* outbuffer, long value)
{
  outbuffer[0] = (unsigned char)((value >> 24) & 0xff);
  outbuffer[1] = (unsigned char)((value >> 16) & 0xff);
  outbuffer[2] = (unsigned char)((value >> 8) & 0xff);
  outbuffer[3] = (unsigned char)((value >> 0) & 0xff);
}

struct atom_t
{
  unsigned char type_[4];
  uint64_t size_;
  uint64_t start_;
  uint64_t end_;
};

#define ATOM_PREAMBLE_SIZE 8

unsigned int atom_header_size(unsigned char* atom_bytes)
{
  return (atom_bytes[0] << 24) +
         (atom_bytes[1] << 16) +
         (atom_bytes[2] << 8) +
         (atom_bytes[3]);
}

int atom_read_header(FILE* infile, struct atom_t* atom)
{
  unsigned char atom_bytes[ATOM_PREAMBLE_SIZE];

  atom->start_ = ftell(infile);

  fread(atom_bytes, ATOM_PREAMBLE_SIZE, 1, infile);
  memcpy(&atom->type_[0], &atom_bytes[4], 4);
  atom->size_ = atom_header_size(atom_bytes);
  atom->end_ = atom->start_ + atom->size_;

  return 1;
}

int atom_read_header_from_buffer(const char *pBuf, uint64_t &offset, struct atom_t* atom)
{
	unsigned char atom_bytes[ATOM_PREAMBLE_SIZE];

	atom->start_ = offset;

	memcpy(atom_bytes, pBuf+offset, ATOM_PREAMBLE_SIZE);
	offset += ATOM_PREAMBLE_SIZE;
	memcpy(&atom->type_[0], &atom_bytes[4], 4);
	atom->size_ = atom_header_size(atom_bytes);
	atom->end_ = atom->start_ + atom->size_;

	return 1;
}


void atom_write_header(unsigned char* outbuffer, struct atom_t* atom)
{
  int i;
  write_int32(outbuffer, atom->size_);
  for(i = 0; i != 4; ++i)
    write_char(outbuffer + 4 + i, atom->type_[i]);
}

int atom_is(struct atom_t const* atom, const char* type)
{
  return (atom->type_[0] == type[0] &&
          atom->type_[1] == type[1] &&
          atom->type_[2] == type[2] &&
          atom->type_[3] == type[3])
         ;
}

void atom_skip(FILE* infile, struct atom_t const* atom)
{
  fseek(infile, atom->end_, SEEK_SET);
}


unsigned int get_file_size(FILE *fp)
{
	fseek(fp, 0, SEEK_END);
	return ftell(fp);
}

int write_seek_data(const char *old_file_name, 
					double start, double end,
					char *header_data, unsigned int header_data_size,
					unsigned int start_pos, unsigned int data_len)
{
	FILE *fp_old, *fp_new;

	fp_old = fopen(old_file_name, "rb");
	if (!fp_old)
	{
		return -1;
	}

	char new_file_name[260];
	//sprintf(new_file_name, "d:\\%d-%d.mp4", (int)start, (int)end);
	snprintf(new_file_name, sizeof(new_file_name), "d:\\%d-%d.mp4", (int)start, (int)end);
	fp_new = fopen(new_file_name, "wb");
	if (!fp_new)
	{
		fclose(fp_old);
		return -2;
	}

	char *buf = new char [data_len];
	if (!buf)
	{
		fclose(fp_old);
		fclose(fp_new);
		return -3;
	}

	fseek(fp_old, start_pos, SEEK_SET);
	fread(buf, sizeof(char), data_len, fp_old);

	fwrite(header_data, sizeof(char), header_data_size, fp_new);
	fwrite(buf, sizeof(char), data_len, fp_new);

	fclose(fp_old);
	fclose(fp_new);

	delete [] buf;

	return 0;
 }

int BuildH264SeekHeader(/*in*/const char *file_name, 
						/*in*/double start, /*in*/double end,
						/*out*/char **header_data, /*out*/unsigned int *header_size,
						/*out*/unsigned int *start_pos, /*out*/unsigned int *data_len)
{

	if (!file_name)
	{
		return -1;
	}

	char *b = new char[1024*1024*50]; //50MB
	unsigned int b_offset = 0;

	FILE *infile;
	struct atom_t ftyp_atom;
	struct atom_t moov_atom;
	struct atom_t mdat_atom;
	unsigned char* moov_data = 0;
	unsigned char* ftyp_data = 0;

	infile = fopen(file_name, "rb");
	if(!infile) 
	{
		return -2;
	}

	unsigned int filesize = get_file_size(infile);
	if (0==filesize || (unsigned)(-1) == filesize)
	{
		return -3; 
	}

	fseek(infile, 0, SEEK_SET);

	struct atom_t leaf_atom;
	while(ftell(infile) < filesize)
	{
		if(!atom_read_header(infile, &leaf_atom))
			break;

		if(atom_is(&leaf_atom, "ftyp"))
		{
			ftyp_atom = leaf_atom;
			ftyp_data = (unsigned char*)malloc(ftyp_atom.size_);
			fseek(infile, ftyp_atom.start_, SEEK_SET);
			fread(ftyp_data, ftyp_atom.size_, 1, infile);
		}
		else if(atom_is(&leaf_atom, "moov"))
		{
			moov_atom = leaf_atom;
			moov_data = (unsigned char*)malloc(moov_atom.size_);
			fseek(infile, moov_atom.start_, SEEK_SET);
			fread(moov_data, moov_atom.size_, 1, infile);
		}
		else if(atom_is(&leaf_atom, "mdat"))
		{
			mdat_atom = leaf_atom;
		}
		atom_skip(infile, &leaf_atom);
	}

	fclose(infile);

	if(!moov_data)
	{
		return -3;
	}

	unsigned int mdat_start = (ftyp_data ? ftyp_atom.size_ : 0) + moov_atom.size_ + 42;
	if(!moov_seek(moov_data,
		&moov_atom.size_,
		start, end,
		&mdat_atom.start_, &mdat_atom.size_,
		mdat_start - mdat_atom.start_))
	{
		return -4;
	}

	if(ftyp_data)
	{
		memcpy(b+b_offset, ftyp_data, ftyp_atom.size_);
		//buffer_append_memory(b, ftyp_data, ftyp_atom.size_);
		b_offset += ftyp_atom.size_;
		free(ftyp_data);
	}

	static char const free_data[] = {
		0x0, 0x0, 0x0,  42, 'f', 'r', 'e', 'e',
		'v', 'i', 'd', 'e', 'o', ' ', 's', 'e',
		'r', 'v', 'e', 'd', ' ', 'b', 'y', ' ',
		'm', 'o', 'd', '_', 'h', '2', '6', '4',
		'_', 's', 't', 'r', 'e', 'a', 'm', 'i',
		'n', 'g'
	};
	//buffer_append_memory(b, free_data, sizeof(free_data));
	memcpy(b+b_offset, free_data, sizeof(free_data));
	b_offset += sizeof(free_data);
	
	//buffer_append_memory(b, moov_data, moov_atom.size_);
	memcpy(b+b_offset, moov_data, moov_atom.size_);
	b_offset += moov_atom.size_;
	free(moov_data);
	
	unsigned char mdat_bytes[ATOM_PREAMBLE_SIZE];
	atom_write_header(mdat_bytes, &mdat_atom);
	//buffer_append_memory(b, mdat_bytes, ATOM_PREAMBLE_SIZE);
	//b->used++; /* add virtual \0 */
	memcpy(b+b_offset, mdat_bytes, ATOM_PREAMBLE_SIZE);
	b_offset += ATOM_PREAMBLE_SIZE;
	//b_offset += 1; ///* add virtual \0 */

	*header_data = b;
	*header_size = b_offset;
/*
//XXXXXXXXXXXXXXXXXXXXXXXXXX
	write_seek_data(file_name, start, end, b, b_offset, 
		mdat_atom.start_ + ATOM_PREAMBLE_SIZE,
		mdat_atom.size_ - ATOM_PREAMBLE_SIZE);
//XXXXXXXXXXXXXXXXXXXXXXXXXX
*/
	*start_pos = mdat_atom.start_ + ATOM_PREAMBLE_SIZE;
	*data_len = mdat_atom.size_ - ATOM_PREAMBLE_SIZE;
	

	return 0;
}

int BuildH264SeekHeader(/*in*/const char *media_header_buf, /*in*/unsigned int media_header_size,
						/*in*/__int64 file_size, 
						/*in*/double start, /*in*/double end,
						/*out*/char **header_data, /*out*/unsigned int *header_size,
						/*out*/unsigned int *start_pos, /*out*/unsigned int *data_len)

{

	if (!media_header_buf || 
		0 == media_header_size)
	{
		return -1;
	}

	char *b = new char[1024*1024*50]; //50MB
	unsigned int b_offset = 0;

	//FILE *infile;

	struct atom_t ftyp_atom = {{0},0,0,0};
	struct atom_t moov_atom = {{0},0,0,0};
	struct atom_t mdat_atom = {{0},0,0,0};
	unsigned char* moov_data = 0;
	unsigned char* ftyp_data = 0;

	bool b_mdat_atom_inited = false;

	
	uint64_t offset = 0;
	struct atom_t leaf_atom = {{0},0,0,0};
	while(offset < /*file_size*/media_header_size)
	{
		if(!atom_read_header_from_buffer(media_header_buf, offset, &leaf_atom))
			break;

		if(atom_is(&leaf_atom, "ftyp"))
		{
			ftyp_atom = leaf_atom;
			ftyp_data = (unsigned char*)malloc(ftyp_atom.size_);

			//fseek(infile, ftyp_atom.start_, SEEK_SET);
			//fread(ftyp_data, ftyp_atom.size_, 1, infile);
			
			memcpy(ftyp_data, media_header_buf+ftyp_atom.start_, ftyp_atom.size_);
			offset += ftyp_atom.start_;
			offset += ftyp_atom.size_;
		}
		else if(atom_is(&leaf_atom, "moov"))
		{
			moov_atom = leaf_atom;
			moov_data = (unsigned char*)malloc(moov_atom.size_);
			
			//fseek(infile, moov_atom.start_, SEEK_SET);
			//fread(moov_data, moov_atom.size_, 1, infile);

			memcpy(moov_data, media_header_buf+moov_atom.start_, moov_atom.size_);
			offset += moov_atom.start_;
			offset += moov_atom.size_;
		}
		else if(atom_is(&leaf_atom, "mdat"))
		{
			mdat_atom = leaf_atom;
			b_mdat_atom_inited = true;
		}
		//atom_skip(infile, &leaf_atom);
		offset = leaf_atom.end_;
	}

	//fclose(infile);

	if(!moov_data)
	{
		return -3;
	}

	assert(b_mdat_atom_inited);
	if (!b_mdat_atom_inited)
	{
		return -10;
	}

	unsigned int mdat_start = (ftyp_data ? ftyp_atom.size_ : 0) + moov_atom.size_ + 42;
	if(!moov_seek(moov_data,
		&moov_atom.size_,
		start, end,
		&mdat_atom.start_, &mdat_atom.size_,
		mdat_start - mdat_atom.start_))
	{
		return -4;
	}

	if(ftyp_data)
	{
		memcpy(b+b_offset, ftyp_data, ftyp_atom.size_);
		//buffer_append_memory(b, ftyp_data, ftyp_atom.size_);
		b_offset += ftyp_atom.size_;
		free(ftyp_data);
	}

	static char const free_data[] = {
		0x0, 0x0, 0x0,  42, 'f', 'r', 'e', 'e',
		'v', 'i', 'd', 'e', 'o', ' ', 's', 'e',
		'r', 'v', 'e', 'd', ' ', 'b', 'y', ' ',
		'm', 'o', 'd', '_', 'h', '2', '6', '4',
		'_', 's', 't', 'r', 'e', 'a', 'm', 'i',
		'n', 'g'
	};
	//buffer_append_memory(b, free_data, sizeof(free_data));
	memcpy(b+b_offset, free_data, sizeof(free_data));
	b_offset += sizeof(free_data);

	//buffer_append_memory(b, moov_data, moov_atom.size_);
	memcpy(b+b_offset, moov_data, moov_atom.size_);
	b_offset += moov_atom.size_;
	free(moov_data);

	unsigned char mdat_bytes[ATOM_PREAMBLE_SIZE];
	atom_write_header(mdat_bytes, &mdat_atom);
	//buffer_append_memory(b, mdat_bytes, ATOM_PREAMBLE_SIZE);
	//b->used++; /* add virtual \0 */
	memcpy(b+b_offset, mdat_bytes, ATOM_PREAMBLE_SIZE);
	b_offset += ATOM_PREAMBLE_SIZE;
	//b_offset += 1; ///* add virtual \0 */

	*header_data = b;
	*header_size = b_offset;
	/*
	//XXXXXXXXXXXXXXXXXXXXXXXXXX
	write_seek_data(file_name, start, end, b, b_offset, 
	mdat_atom.start_ + ATOM_PREAMBLE_SIZE,
	mdat_atom.size_ - ATOM_PREAMBLE_SIZE);
	//XXXXXXXXXXXXXXXXXXXXXXXXXX
	*/
	*start_pos = mdat_atom.start_ + ATOM_PREAMBLE_SIZE;
	*data_len = mdat_atom.size_ - ATOM_PREAMBLE_SIZE;


	return 0;
}




