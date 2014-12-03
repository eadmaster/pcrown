/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */ 
/* Latest source code is available at http://ppcenter.free.fr. */


#ifndef SATCOM_COMPRESS_H
#define SATCOM_COMPRESS_H

//#include "sc_common.h"



//----------------------------------------------------------------------
// Compression routines.
// 
//----------------------------------------------------------------------


/**
 * Compressed data's maximum header size.
 * unsigned char version, unsigned long decompressed_length
**/
#define SC_COMP_HEADER_MAXSIZE 5

/**
 * Minmum compression size.
 * (there is no need to compress data smaller than this, since
 *  compression itself may not be efficient, also transmission
 *  overhead due to compression commands may be bigger than
 *  overhead without compression.)
**/
#define SC_COMP_MINSIZE 128
#define SC_BUFF_MAXSIZE (64*1024)

#ifndef SC_COMP_DISABLE
/**
 * Compress data.
 *  ind : input data.
 *  inl : input data length.
 *  outd: output data.
 * At most inl+SC_COMP_HEADER_MAXSIZE bytes are required in the output buffer.
 * Return compressed data size.
**/
unsigned long comp_exec(unsigned char* ind, unsigned long inl, unsigned char* outd);
#endif // SC_COMP_DISABLE


#ifndef SC_DECOMP_DISABLE
/**
 * Decompress data.
 * It is supposed there is enough space to decompress the data.
 * Note : output data length is automatically computed from input data.
 *  ind : input data.
 *  inl : input data length.
 *  outd: output data.
 * Return decompressed data size.
**/
unsigned long decomp_exec(unsigned char* ind, unsigned long inl, unsigned char* outd);
/**
 * Get data size when decompressed.
 *  ind : input data.
 *  inl : input data length.
 * Return decompressed data size.
**/
unsigned long decomp_getsize(unsigned char* ind, unsigned long inl);
#endif // SC_DECOMP_DISABLE



#endif //SATCOM_COMPRESS_H
