/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compress.h"
#include "common.h"





/* Read loader file from disk. */
void loader_read(char* infile, unsigned char** buffer, unsigned long* buffer_len)
{
    unsigned long len;
    unsigned char* buffer1 = NULL;
    FILE* fp;

    /* Init output parameters. */
    *buffer     = NULL;
    *buffer_len = 0;

    fopen_s(&fp, infile, "rb");
    if(!fp)
    {
        printf(" [LOADER]Error : could't open file (%s) !\n", infile);
        return;
    }

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    printf(" [LOADER]Read %d bytes from disk.\n", len);
    if(len > LOADER_MAX_SIZE)
    {
        printf(" [LOADER]Error : file length is too big (%d bytes, max = %d bytes) !\n", len, LOADER_MAX_SIZE);
        fclose(fp); fp = NULL;
        return;
    }

    buffer1 = (unsigned char*)malloc(DATA_MAX_SIZE);
    if(!buffer1)
    {
        fclose(fp); fp = NULL;
        printf(" [LOADER]Alloc error#1 (%d bytes) !\n", DATA_MAX_SIZE);
        return;
    }

    memset((void*)buffer1, 'P', DATA_MAX_SIZE);
    fread(buffer1, 1, len, fp);
    fclose(fp); fp = NULL;

    /* Update output parameters. */
    *buffer     = buffer1;
    *buffer_len = len;
}

/* Read 0.bin file, compress it and return compressed size. */
void zero_bin_read(char* infile, unsigned char** buffer, unsigned long* buffer_len)
{
    unsigned long ret, len;
    unsigned char* buffer1 = NULL;
    unsigned char* buffer2 = NULL;
    FILE* fp;

    /* Init output parameters. */
    *buffer     = NULL;
    *buffer_len = 0;

    fopen_s(&fp, infile, "rb");
    if(!fp)
    {
        printf(" [BIN   ]Error : could't open file (%s) !\n", infile);
        return;
    }

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    buffer1 = (unsigned char*)malloc(len);
    if(!buffer1)
    {
        fclose(fp); fp = NULL;
        printf(" [BIN   ]Alloc error#1 (%d bytes) !\n", len);
        return;
    }

    fread(buffer1, 1, len, fp);
    fclose(fp); fp = NULL;

    buffer2 = (unsigned char*)malloc(len+SC_COMP_HEADER_MAXSIZE);
    if(!buffer2)
    {
        free(buffer1); buffer1 = NULL;
        printf(" [BIN   ]Alloc error#2 (%d bytes) !\n", len+SC_COMP_HEADER_MAXSIZE);
        return;
    }

    ret = comp_exec(buffer1, len, buffer2);
    printf(" [BIN   ]Compressed size = %d bytes (input size * %f %%)\n", ret, ((float)(100*ret))/len);
    free(buffer1); buffer1 = NULL;

    /* Update output parameters. */
    *buffer     = buffer2;
    *buffer_len = ret;
}



/* Read BMP file, convert to RGB555, compress it and return compressed size. */
#define RGB_SET(R,G,B) (((B)<<10)|((G)<<5)|(R) | 0x8000)
void splash_read(char* infile, unsigned char** buffer, unsigned long* buffer_len)
{
    FILE* fp;

    unsigned char*  buff_bmp    = NULL; // BMP image
    unsigned short* buff_rgb555 = NULL; // RGB555 image, 320x224
    unsigned char*  buff_comp   = NULL; // RGB555 image, 320x224, compressed

    unsigned long   len_bmp;
    unsigned long   len_rgb555;
    unsigned long   len_comp;


    unsigned char*  bmp_pallette = NULL;
    unsigned char*  bmp_data     = NULL;
    unsigned long   bmp_w, bmp_h;
    unsigned short  bmp_bitcount;
    int i, j, ii, jj, index, x_offset, y_offset;


    /* Init output parameters. */
    *buffer     = NULL;
    *buffer_len = 0;



    /* Read BMP file. */
    fopen_s(&fp, infile, "rb");
    if(!fp)
    {
        printf(" [SPLASH]Error : could't open file (%s) !\n", infile);
        return;
    }

    fseek(fp, 0, SEEK_END);
    len_bmp = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    buff_bmp = (unsigned char*)malloc(len_bmp);
    if(!buff_bmp)
    {
        fclose(fp); fp = NULL;
        printf(" [SPLASH]Alloc error#1 (%d bytes) !\n", len_bmp);
        return;
    }
    fread(buff_bmp, 1, len_bmp, fp);
    fclose(fp); fp = NULL;


    /* Read BMP header data. */
    memcpy(&bmp_w, buff_bmp+18, sizeof(bmp_w));
    memcpy(&bmp_h, buff_bmp+22, sizeof(bmp_h));
    memcpy(&bmp_bitcount, buff_bmp+28, sizeof(bmp_bitcount));

    len_rgb555 = SPLASH_IMG_WIDTH * SPLASH_IMG_HEIGHT * sizeof(unsigned short);
    buff_rgb555 = (unsigned short*)malloc(len_rgb555);
    if(!buff_rgb555)
    {
        free(buff_bmp); buff_bmp = NULL;
        printf(" [SPLASH]Alloc error#2 (%d bytes) !\n", len_rgb555);
        return;
    }
    memset((void*)buff_rgb555, 0, len_rgb555);

    /* In the case image in smaller than 320x224, draw it at the center of screen. */
    x_offset = 0;
    if(bmp_w < SPLASH_IMG_WIDTH)
    {
        x_offset = (SPLASH_IMG_WIDTH - bmp_w) / 2;
    }
    y_offset = 0;
    if(bmp_h < SPLASH_IMG_HEIGHT)
    {
        y_offset = (SPLASH_IMG_HEIGHT - bmp_h) / 2;
    }

    /* Read BMP pixel data. */
    if(bmp_bitcount == 8)
    { /* Palette 256 colors bitmap. */
        bmp_pallette = buff_bmp + 54;
        bmp_data     = buff_bmp + 54 + 1024;

        /* Read 8 bits DIB image */
        jj=0;
        for(j=bmp_h-1; j>=0; j--)
        {
            ii=0;
            for(i=0; i<(int)bmp_w; i++)
            {
                if((i < SPLASH_IMG_WIDTH) && (j < SPLASH_IMG_HEIGHT))
                {
                    unsigned char b, g, r;
                    unsigned short rgb555;

                    index = 4*bmp_data[j*bmp_w + i];

                    b = bmp_pallette[index + 0] >> 3;
                    g = bmp_pallette[index + 1] >> 3;
                    r = bmp_pallette[index + 2] >> 3;
                    rgb555 = RGB_SET(r, g, b);
                    rgb555 = ((rgb555 & 0x00FF) << 8) | ((rgb555 & 0xFF00) >> 8);

                    buff_rgb555[(y_offset + jj)*bmp_w + (x_offset + ii)] = rgb555;
                }

                ii++;
            }
            jj++;
        }
        
    }
    else //if(bmp_bitcount == 24)
    { /* 24 bits bitmap. */
        bmp_data = buff_bmp + 54;
        /* Read 24 bits DIB image */
        jj=0;
        for(j=bmp_h-1; j>=0; j--)
        {
            ii=0;
            for(i=0; i<(int)bmp_w; i++)
            {
                if((i < SPLASH_IMG_WIDTH) && (j < SPLASH_IMG_HEIGHT))
                {
                    unsigned char b, g, r;
                    unsigned short rgb555;

                    index = (j*bmp_w + i)*3;

                    b = bmp_data[index + 0] >> 3;
                    g = bmp_data[index + 1] >> 3;
                    r = bmp_data[index + 2] >> 3;
                    rgb555 = RGB_SET(r, g, b);
                    rgb555 = ((rgb555 & 0x00FF) << 8) | ((rgb555 & 0xFF00) >> 8);

                    buff_rgb555[(y_offset + jj)*bmp_w + (x_offset + ii)] = rgb555;
                }

                ii++;
            }
            jj++;
        }
    
    }

    free(buff_bmp); buff_bmp = NULL;
    bmp_pallette = NULL;
    bmp_data     = NULL;



    /* Compress 320x224 RGB555 image. */
    buff_comp = (unsigned char*)malloc(len_rgb555+SC_COMP_HEADER_MAXSIZE);
    if(!buff_comp)
    {
        free(buff_rgb555); buff_rgb555 = NULL;
        printf(" [SPLASH]Alloc error#3 (%d bytes) !\n", len_rgb555+SC_COMP_HEADER_MAXSIZE);
        return;
    }

    len_comp = comp_exec((unsigned char*)buff_rgb555, len_rgb555, buff_comp);
    printf(" [SPLASH]Compressed size = %d bytes (input size * %f %%)\n", len_comp, ((float)(100*len_comp))/len_rgb555);
    free(buff_rgb555); buff_rgb555 = NULL;

    /* Update output parameters. */
    *buffer     = buff_comp;
    *buffer_len = len_comp;
}



/* Endianness conversion. */
unsigned long bswap_ul(unsigned long in)
{
    unsigned long out;
    out  = ((in >> 24) & 0xFF) <<  0;
    out |= ((in >> 16) & 0xFF) <<  8;
    out |= ((in >>  8) & 0xFF) << 16;
    out |= ((in >>  0) & 0xFF) << 24;
    return out;
}




int main (int argc, char *argv[])
{
    int ret = 1;

    char* file_loader;
    char* file_splash;
    char* file_zero;
    char* file_out;

    unsigned char* buff_out    = NULL;
    unsigned char* buff_zero   = NULL;
    unsigned char* buff_splash = NULL;

    unsigned long  len_out     = 0; // Total (loader + 0.bin + splash) size, in bytes
    unsigned long  len_zero    = 0; // Compressed size, in bytes
    unsigned long  len_splash  = 0; // Compressed size, in bytes

    unsigned long* ptr;
    unsigned long ofs, len;

    FILE* fp = NULL;

    printf("--- mksplash v0.1 by cafe-alpha. ---\n");
    if(argc <= 1)
    {
        printf(" Type %s -h for help.\n", argv[0]);
        return 0;
    }
    if(argc >= 2)
    {
        if((!strcmp(argv[1], "-h"))
        || (!strcmp(argv[1], "-help"))
        || (!strcmp(argv[1], "-?")))
        {
            printf(" Note: uses LibLZF: http://oldhome.schmorp.de/marc/liblzf.html\n");
            printf(" Usage : <loader.bin> <splash.bmp> <0.bin> <out.bin>\n");
            printf("         -> Pack loader.bin, splash.bmp, 0.bin in out.bin file.\n");
            printf("            After startup, splash.bmp is displayed\n");
            printf("            and 0.bin is loaded after a key is pressed.\n");
            return 0;
        }
    }

    if(argc != 5) return 1;

    file_loader = argv[1];
    file_splash = argv[2];
    file_zero   = argv[3];
    file_out    = argv[4];

    /* Read loader file. */
    loader_read(file_loader, &buff_out, &len_out);
    if((buff_out == NULL) || (len_out == 0)) goto end;

    /* Read, convert and compress splash image file. */
    splash_read(file_splash, &buff_splash, &len_splash);
    if((buff_splash == NULL) || (len_splash == 0)) goto end;

    /* Read and compress 0.bin file. */
    zero_bin_read(file_zero, &buff_zero, &len_zero);
    if((buff_zero == NULL) || (len_zero == 0)) goto end;


    /* Compute and check total size. */
    len_out = LOADER_MAX_SIZE + PARAMS_SIZE + len_zero + len_splash;
    if(len_out > DATA_MAX_SIZE)
    {
        printf("Error : total size too big (required %d bytes, max %d bytes) !\n", len_out, DATA_MAX_SIZE);
        goto end;
    }


    /* Add compressed 0.bin stuff. */
    ofs = LOADER_MAX_SIZE + PARAMS_SIZE;
    len = len_zero;

    ptr = (unsigned long*)(buff_out + OFFSET_ZEROBIN_OFS);
    *ptr = bswap_ul(LOADER_EXEC_ADDR + ofs);
    ptr = (unsigned long*)(buff_out + OFFSET_ZEROBIN_LEN);
    *ptr = bswap_ul(len);

    memcpy((void*)(buff_out+ofs), (void*)buff_zero, len);


    /* Add compressed splash image stuff. */
    ofs = LOADER_MAX_SIZE + PARAMS_SIZE + len_zero;
    len = len_splash;

    ptr = (unsigned long*)(buff_out + OFFSET_SPLASH_OFS);
    *ptr = bswap_ul(LOADER_EXEC_ADDR + ofs);
    ptr = (unsigned long*)(buff_out + OFFSET_SPLASH_LEN);
    *ptr = bswap_ul(len);

    memcpy((void*)(buff_out+ofs), (void*)buff_splash, len);


    /* Write whole stuff to disk. */
    fopen_s(&fp, file_out, "wb");
    if(fp)
    {
        fwrite(buff_out, 1, len_out, fp);
        fclose(fp); fp = NULL;
        printf("Wrote %d bytes (%d KB) to disk.\n", len_out, len_out>>10);
    }
    else
    {
        printf("Error : could not write to file (%s) !\n", file_out);
        goto end;
    }


    ret = 0;
end:
    if(buff_splash)
    {
        free(buff_splash); buff_splash = NULL;
    }
    if(buff_zero)
    {
        free(buff_zero); buff_zero = NULL;
    }
    if(buff_out)
    {
        free(buff_out); buff_out = NULL;
    }

    return ret;
}



