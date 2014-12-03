/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */ 
/* Latest source code is available at http://ppcenter.free.fr. */


#include "compress.h"




//----------------------------------------------------------------------
// LZF compression
// 
//----------------------------------------------------------------------



#ifndef SC_COMP_DISABLE
//----------------------------------------------------------------------
//----- lzf_c.c
#include "lzfP.h"

#define HSIZE (1 << (HLOG))

/*
 * don't play with this unless you benchmark!
 * decompression is not dependent on the hash function
 * the hashing function might seem strange, just believe me
 * it works ;)
 */
#ifndef FRST
# define FRST(p) (((p[0]) << 8) | p[1])
# define NEXT(v,p) (((v) << 8) | p[2])
# if ULTRA_FAST
#  define IDX(h) ((( h             >> (3*8 - HLOG)) - h  ) & (HSIZE - 1))
# elif VERY_FAST
#  define IDX(h) ((( h             >> (3*8 - HLOG)) - h*5) & (HSIZE - 1))
# else
#  define IDX(h) ((((h ^ (h << 5)) >> (3*8 - HLOG)) - h*5) & (HSIZE - 1))
# endif
#endif
/*
 * IDX works because it is very similar to a multiplicative hash, e.g.
 * ((h * 57321 >> (3*8 - HLOG)) & (HSIZE - 1))
 * the latter is also quite fast on newer CPUs, and compresses similarly.
 *
 * the next one is also quite good, albeit slow ;)
 * (int)(cos(h & 0xffffff) * 1e6)
 */

#if 0
/* original lzv-like hash function, much worse and thus slower */
# define FRST(p) (p[0] << 5) ^ p[1]
# define NEXT(v,p) ((v) << 5) ^ p[2]
# define IDX(h) ((h) & (HSIZE - 1))
#endif

#define        MAX_LIT        (1 <<  5)
#define        MAX_OFF        (1 << 13)
#define        MAX_REF        ((1 << 8) + (1 << 3))

#if __GNUC__ >= 3
# define expect(expr,value)         __builtin_expect ((expr),(value))
# define inline                     inline
#else
# define expect(expr,value)         (expr)
# define inline                     static
#endif

#define expect_false(expr) expect ((expr) != 0, 0)
#define expect_true(expr)  expect ((expr) != 0, 1)

/*
 * compressed format
 *
 * 000LLLLL <L+1>    ; literal
 * LLLooooo oooooooo ; backref L
 * 111ooooo LLLLLLLL oooooooo ; backref L+7
 *
 */

unsigned int
lzf_compress (const void *const in_data, unsigned int in_len,
	      void *out_data, unsigned int out_len
#if LZF_STATE_ARG
              , LZF_STATE htab
#endif
              )
{
#if !LZF_STATE_ARG
  LZF_STATE htab;
#endif
  const u8 **hslot;
  const u8 *ip = (const u8 *)in_data;
        u8 *op = (u8 *)out_data;
  const u8 *in_end  = ip + in_len;
        u8 *out_end = op + out_len;
  const u8 *ref;

  /* off requires a type wide enough to hold a general pointer difference.
   * ISO C doesn't have that (size_t might not be enough and ptrdiff_t only
   * works for differences within a single object). We also assume that no
   * no bit pattern traps. Since the only platform that is both non-POSIX
   * and fails to support both assumptions is windows 64 bit, we make a
   * special workaround for it.
   */
#if defined (WIN32) && defined (_M_X64)
  unsigned _int64 off; /* workaround for missing POSIX compliance */
#else
  unsigned long off;
#endif
  unsigned int hval;
  int lit;

  if (!in_len || !out_len)
    return 0;

#if INIT_HTAB
  memset (htab, 0, sizeof (htab));
# if 0
  for (hslot = htab; hslot < htab + HSIZE; hslot++)
    *hslot++ = ip;
# endif
#endif

  lit = 0; op++; /* start run */

  hval = FRST (ip);
  while (ip < in_end - 2)
    {
      hval = NEXT (hval, ip);
      hslot = htab + IDX (hval);
      ref = *hslot; *hslot = ip;

      if (1
#if INIT_HTAB
          && ref < ip /* the next test will actually take care of this, but this is faster */
#endif
          && (off = ip - ref - 1) < MAX_OFF
          && ip + 4 < in_end
          && ref > (u8 *)in_data
#if STRICT_ALIGN
          && ref[0] == ip[0]
          && ref[1] == ip[1]
          && ref[2] == ip[2]
#else
          && *(u16 *)ref == *(u16 *)ip
          && ref[2] == ip[2]
#endif
        )
        {
          /* match found at *ref++ */
          unsigned int len = 2;
          unsigned int maxlen = in_end - ip - len;
          maxlen = maxlen > MAX_REF ? MAX_REF : maxlen;

          if (expect_false (op + 3 + 1 >= out_end)) /* first a faster conservative test */
            if (op - !lit + 3 + 1 >= out_end) /* second the exact but rare test */
              return 0;

          op [- lit - 1] = lit - 1; /* stop run */
          op -= !lit; /* undo run if length is zero */

          for (;;)
            {
              if (expect_true (maxlen > 16))
                {
                  len++; if (ref [len] != ip [len]) break;
                  len++; if (ref [len] != ip [len]) break;
                  len++; if (ref [len] != ip [len]) break;
                  len++; if (ref [len] != ip [len]) break;

                  len++; if (ref [len] != ip [len]) break;
                  len++; if (ref [len] != ip [len]) break;
                  len++; if (ref [len] != ip [len]) break;
                  len++; if (ref [len] != ip [len]) break;

                  len++; if (ref [len] != ip [len]) break;
                  len++; if (ref [len] != ip [len]) break;
                  len++; if (ref [len] != ip [len]) break;
                  len++; if (ref [len] != ip [len]) break;

                  len++; if (ref [len] != ip [len]) break;
                  len++; if (ref [len] != ip [len]) break;
                  len++; if (ref [len] != ip [len]) break;
                  len++; if (ref [len] != ip [len]) break;
                }

              do
                len++;
              while (len < maxlen && ref[len] == ip[len]);

              break;
            }

          len -= 2; /* len is now #octets - 1 */
          ip++;

          if (len < 7)
            {
              *op++ = (off >> 8) + (len << 5);
            }
          else
            {
              *op++ = (off >> 8) + (  7 << 5);
              *op++ = len - 7;
            }

          *op++ = off;
          lit = 0; op++; /* start run */

          ip += len + 1;

          if (expect_false (ip >= in_end - 2))
            break;

#if ULTRA_FAST || VERY_FAST
          --ip;
# if VERY_FAST && !ULTRA_FAST
          --ip;
# endif
          hval = FRST (ip);

          hval = NEXT (hval, ip);
          htab[IDX (hval)] = ip;
          ip++;

# if VERY_FAST && !ULTRA_FAST
          hval = NEXT (hval, ip);
          htab[IDX (hval)] = ip;
          ip++;
# endif
#else
          ip -= len + 1;

          do
            {
              hval = NEXT (hval, ip);
              htab[IDX (hval)] = ip;
              ip++;
            }
          while (len--);
#endif
        }
      else
        {
          /* one more literal byte we must copy */
          if (expect_false (op >= out_end))
            return 0;

          lit++; *op++ = *ip++;

          if (expect_false (lit == MAX_LIT))
            {
              op [- lit - 1] = lit - 1; /* stop run */
              lit = 0; op++; /* start run */
            }
        }
    }

  if (op + 3 > out_end) /* at most 3 bytes can be missing here */
    return 0;

  while (ip < in_end)
    {
      lit++; *op++ = *ip++;

      if (expect_false (lit == MAX_LIT))
        {
          op [- lit - 1] = lit - 1; /* stop run */
          lit = 0; op++; /* start run */
        }
    }

  op [- lit - 1] = lit - 1; /* end run */
  op -= !lit; /* undo run if length is zero */

  return op - (u8 *)out_data;
}



/*
 * Compress in_len bytes stored at the memory block starting at
 * in_data and write the result to out_data, up to a maximum length
 * of out_len bytes.
 *
 * If the output buffer is not large enough or any error occurs return 0,
 * otherwise return the number of bytes used, which might be considerably
 * more than in_len (but less than 104% of the original size), so it
 * makes sense to always use out_len == in_len - 1), to ensure _some_
 * compression, and store the data uncompressed otherwise (with a flag, of
 * course.
 *
 * lzf_compress might use different algorithms on different systems and
 * even different runs, thus might result in different compressed strings
 * depending on the phase of the moon or similar factors. However, all
 * these strings are architecture-independent and will result in the
 * original data when decompressed using lzf_decompress.
 *
 * The buffers must not be overlapping.
 *
 * If the option LZF_STATE_ARG is enabled, an extra argument must be
 * supplied which is not reflected in this header file. Refer to lzfP.h
 * and lzf_c.c.
 *
 */
//unsigned int 
//lzf_compress (const void *const in_data,  unsigned int in_len,
//              void             *out_data, unsigned int out_len);

/**
 * Compress data.
 *  ind : input data.
 *  inl : input data length.
 *  outd: output data.
 * At most inl+SC_COMP_HEADER_MAXSIZE bytes are required in the output buffer.
 * Return compressed data size.
**/
unsigned long _comp_header_get_data(unsigned char* header, int data_size, int index);
void _comp_header_set_data(unsigned char* header, int data_size, int index, unsigned long val);
unsigned long comp_exec(unsigned char* ind, unsigned long inl, unsigned char* outd)
{
    unsigned long ret;
    int header_len;
    unsigned long lzf_ret;

    int data_size;
    if(inl > 65500)    data_size = 4;
    else if(inl > 240) data_size = 2;
    else               data_size = 1;

    /* By default, set the header length in the case of compressible data. */
    header_len = 1+data_size;
    lzf_ret = 0;
    if(inl > 2)
    { /* As a caution, don't compress very small data (it can cause troubles ?). */
        //unsigned int 
        //lzf_compress (const void *const in_data,  unsigned int in_len,
        //              void             *out_data, unsigned int out_len);
        lzf_ret = lzf_compress(ind,  inl, outd+header_len, inl-header_len-1);
    }
    if(lzf_ret == 0)
    { /* Bad luck, could not compress, hence copy data. */
        header_len = 1;

        /* Set header version & datasize. */
        outd[0] = (0<<4) | (data_size&0x7);
        /* Append uncompressed data.
         * Note: the case ind==0 seems to covered by memcpy. */
        memcpy(outd+header_len, ind, inl);

        /* Set return value */
        ret = header_len + inl;
    }
    else
    { /* Could compress data. Fill header. */

        /* Set header version & datasize. */
        outd[0] = (1<<4) | (data_size&0x7);
        /* Set decompressed size in header. */
        _comp_header_set_data(outd, data_size, 0/*index*/, inl);
        /* Don't need to append compressed data, 
         * since it has been done in lzf_compress function. */

        /* Set return value */
        ret = header_len + lzf_ret;
    }

    /* Return header size +compressed data size. */
    return ret;
}
#endif // SC_COMP_DISABLE




#ifndef SC_DECOMP_DISABLE

//----------------------------------------------------------------------
//----- lzf_d.c

#include "lzfP.h"

#if AVOID_ERRNO
# define SET_ERRNO(n)
#else
# include <errno.h>
# define SET_ERRNO(n) errno = (n)
#endif

#if (__i386 || __amd64) && __GNUC__ >= 3
# define lzf_movsb(dst, src, len)                \
   asm ("rep movsb"                              \
        : "=D" (dst), "=S" (src), "=c" (len)     \
        :  "0" (dst),  "1" (src),  "2" (len));
#endif

unsigned int 
lzf_decompress (const void *const in_data,  unsigned int in_len,
                void             *out_data, unsigned int out_len)
{
  u8 const *ip = (const u8 *)in_data;
  u8       *op = (u8 *)out_data;
  u8 const *const in_end  = ip + in_len;
  u8       *const out_end = op + out_len;

  do
    {
      unsigned int ctrl = *ip++;

      if (ctrl < (1 << 5)) /* literal run */
        {
          ctrl++;

          if (op + ctrl > out_end)
            {
              SET_ERRNO (E2BIG);
              return 0;
            }

#if CHECK_INPUT
          if (ip + ctrl > in_end)
            {
              SET_ERRNO (EINVAL);
              return 0;
            }
#endif

#ifdef lzf_movsb
          lzf_movsb (op, ip, ctrl);
#else
          do
            *op++ = *ip++;
          while (--ctrl);
#endif
        }
      else /* back reference */
        {
          unsigned int len = ctrl >> 5;

          u8 *ref = op - ((ctrl & 0x1f) << 8) - 1;

#if CHECK_INPUT
          if (ip >= in_end)
            {
              SET_ERRNO (EINVAL);
              return 0;
            }
#endif
          if (len == 7)
            {
              len += *ip++;
#if CHECK_INPUT
              if (ip >= in_end)
                {
                  SET_ERRNO (EINVAL);
                  return 0;
                }
#endif
            }

          ref -= *ip++;

          if (op + len + 2 > out_end)
            {
              SET_ERRNO (E2BIG);
              return 0;
            }

          if (ref < (u8 *)out_data)
            {
              SET_ERRNO (EINVAL);
              return 0;
            }

#ifdef lzf_movsb
          len += 2;
          lzf_movsb (op, ref, len);
#else
          *op++ = *ref++;
          *op++ = *ref++;

          do
            *op++ = *ref++;
          while (--len);
#endif
        }
    }
  while (ip < in_end);

  return op - (u8 *)out_data;
}





/*
 * Decompress data compressed with some version of the lzf_compress
 * function and stored at location in_data and length in_len. The result
 * will be stored at out_data up to a maximum of out_len characters.
 *
 * If the output buffer is not large enough to hold the decompressed
 * data, a 0 is returned and errno is set to E2BIG. Otherwise the number
 * of decompressed bytes (i.e. the original length of the data) is
 * returned.
 *
 * If an error in the compressed data is detected, a zero is returned and
 * errno is set to EINVAL.
 *
 * This function is very fast, about as fast as a copying loop.
 */
//unsigned int 
//lzf_decompress (const void *const in_data,  unsigned int in_len,
//                void             *out_data, unsigned int out_len);



/**
 * Some note about the compressed data header.
 * Header[0] : 000C LLLL, 
 *   where C is the compression flag (0-> not compressed, 1-> compressed)
 *   and L is the size data length (1, or 2, or 4)
 *    -> Next data's length depends on L.
 * In the case C=1, next data is decompressed size.
**/
unsigned long _comp_header_get_data(unsigned char* header, int data_size, int index)
{
    unsigned long ret = 0;

    if(data_size == 4)
    {
        ret = ((header[1 + index * data_size + 0]) << 24)
            | ((header[1 + index * data_size + 1]) << 16)
            | ((header[1 + index * data_size + 2]) <<  8)
            | ((header[1 + index * data_size + 3]) <<  0);
    }
    else if(data_size == 2)
    {
        ret = ((header[1 + index * data_size + 0]) <<  8)
            | ((header[1 + index * data_size + 1]) <<  0);
    }
    else// if(data_size == 1)
    {
        ret = ((header[1 + index * data_size + 0]) <<  0);
    }

    return ret;
}
void _comp_header_set_data(unsigned char* header, int data_size, int index, unsigned long val)
{
    if(data_size == 4)
    {
        header[1 + index * data_size + 0] = (val >> 24) & 0xFF;
        header[1 + index * data_size + 1] = (val >> 16) & 0xFF;
        header[1 + index * data_size + 2] = (val >>  8) & 0xFF;
        header[1 + index * data_size + 3] = (val >>  0) & 0xFF;
    }
    else if(data_size == 2)
    {
        header[1 + index * data_size + 0] = (val >>  8) & 0xFF;
        header[1 + index * data_size + 1] = (val >>  0) & 0xFF;
    }
    else// if(data_size == 1)
    {
        header[1 + index * data_size + 0] = (val >>  0) & 0xFF;
    }
}




unsigned long _decomp_exec_internal(unsigned char* ind, unsigned long inl, unsigned char* outd)
{
    unsigned long ret;
    int header_len;
    unsigned long outl;

    int data_size, compress_flag;

    /* Retrieve header data. */
    if(inl == 0) return 0;
    compress_flag = ind[0] >> 4;
    data_size     = ind[0] & 0xF;
    /* Exclude wrong headers. */
    if((compress_flag > 1) || ((data_size != 1) && (data_size != 2) && (data_size != 4))) return 0;

    /* Set compression-related parameters. */
    if(compress_flag)
    { /* Compressed. */
        header_len = 1+data_size;
        outl = _comp_header_get_data(ind, data_size, 0/*index*/);
        if(outd != NULL)
        { /* Decompress ? */
            //unsigned int 
            //lzf_decompress (const void *const in_data,  unsigned int in_len,
            //                void             *out_data, unsigned int out_len);
            ret = lzf_decompress(ind+header_len, inl-header_len, outd, outl);
        }
        else
        {
            ret = outl;
        }
    }
    else
    { /* Not compressed. */
        header_len = 1;
        outl = inl-header_len;
        if(outd != NULL)
        { /* Decompress ? */
            memcpy(outd, ind+header_len, outl);
        }
        ret = outl;
    }

    return ret;
}
/**
 * Decompress data.
 * It is supposed there is enough space to decompress the data.
 * Note : output data length is automatically computed from input data.
 *  ind : input data.
 *  inl : input data length.
 *  outd: output data.
 * Return decompressed data size.
**/
unsigned long decomp_exec(unsigned char* ind, unsigned long inl, unsigned char* outd)
{
    return _decomp_exec_internal(ind, inl, outd);
}

/**
 * Get data size when decompressed.
 *  ind : input data.
 *  inl : input data length.
 * Return decompressed data size.
**/
unsigned long decomp_getsize(unsigned char* ind, unsigned long inl)
{
    return _decomp_exec_internal(ind, inl, NULL/*No decompress*/);
}

#endif // SC_DECOMP_DISABLE
