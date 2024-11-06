/*  Copyright 2012-2014 Theo Berkau

    This file is part of ITEMSUTIL.

    ITEMSUTIL is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ITEMSUTIL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ITEMSUTIL; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include "../eventeditor/evn.h"
#include "../eventeditor/texttbl.h"
#include "../eventeditor/trnstext.h"
#include "../eventeditor/util.h"

bool dupe_trans=false;

typedef struct  
{
   unsigned long tbl_addr;
   unsigned long tbl_size;
   unsigned long txt_offset;
   unsigned long txt_size;
   unsigned long txt_addr;
   unsigned long txt_ptr_offset[10];
   char *txt_filename;
   unsigned long prog_addr;
   unsigned long x_reft[10];
   unsigned long x_reft_off[10];
   int num_x_reft;
} text_info_struct;

int extract_data(text_info_struct *ti, const char *filename, const char *out_filename)
{
   unsigned long i;
   FILE *fp=NULL, *out_fp=NULL;
   unsigned short *tbl=NULL;
   dcmp_text_struct dcmp_info;
   char text[512];
   int ret=ERROR_SUCCESS;
   unsigned char *data;

   if (fopen_s(&fp, filename, "rb") != 0)
   {
      ret = ERROR_FILE_NOT_FOUND;
      goto error;
   }

   if ((tbl = (unsigned short *)malloc(ti->tbl_size)) == NULL)
   {
      ret = ERROR_NOT_ENOUGH_MEMORY;
      goto error;
   }

   if ((data = (unsigned char *)malloc(ti->txt_size+0x1000)) == NULL)
   {
      ret = ERROR_NOT_ENOUGH_MEMORY;
      goto error;
   }

   fseek(fp, ti->tbl_addr, SEEK_SET);
   fread(tbl, 1, ti->tbl_size, fp);

   memset(data, 0, 0x1000);
   fseek(fp, ti->txt_offset, SEEK_SET);
   fread(data+0x1000, 1, ti->txt_size, fp);

   fclose(fp);

   if (fopen_s(&out_fp, out_filename, "wt") != 0)
   {
      ret = ERROR_FILE_NOT_FOUND;
      goto error;
   }

   for (i = 0; i < ti->tbl_size-2; i+=2)
   {
      unsigned long start, end;

      // Fetch offset
      start = WordSwap(tbl[i>>1]);
      if (i + 2 < ti->tbl_size)
         end = WordSwap(tbl[(i+2)>>1]);
      else
         end = ti->txt_size;
      
      if (start % 5 == 0)
      {
         dcmp_info.decode_counter = 0;
         dcmp_info.magic_number = 0;
      }
      else
      {
         dcmp_info.decode_counter = start % 5;
         dcmp_info.magic_number = data[0x1000+(start / 5) * 5 + 4];
      }
      memset(text, 0, sizeof(text));
      if (start<end)
         DecompressText(&dcmp_info, data, 0, start, end, TRUE, text, sizeof(text));
      fprintf(out_fp, "%s\n", text);
   }

error:
   if (fp)
      fclose(fp);
   if (out_fp)
      fclose(out_fp);
   if (tbl)
      free(tbl);
   if (data)
      free(data);

   return ret;
}

int read_file(const char *filename, void **buffer, int *size)
{
   FILE *fp;

   if (fopen_s(&fp, filename, "rb") != 0)
      return ERROR_FILE_NOT_FOUND;

   size[0] = _filelength(_fileno(fp));
   buffer[0] = malloc(size[0]);

   if (buffer[0] == NULL)
   {
      fclose(fp);
      return ERROR_NOT_ENOUGH_MEMORY;
   }

   fread((void *)buffer[0], 1, size[0], fp);
   fclose(fp);
   return ERROR_SUCCESS;
}


// insert_data(&ti[i], filename = argv[2,3] -> names.txt, items.txt, out_filename = argv[4] = 0.bin, &out_size, FALSE);

int insert_data(text_info_struct *ti, const char *filename, const char *out_filename, int *out_size, int lzo_compress)
{
   FILE *fp=NULL, *out_fp=NULL;
   int ret=ERROR_SUCCESS;
   unsigned long tbl_size=0;
   int max_text=0;
   ttentry_struct *ttentry;
   unsigned char *buffer, *txt_buffer;
   int size=0, txt_size=0;

   if ((ret = read_file(out_filename, (void **)&buffer, &size)) != ERROR_SUCCESS)
      return ret;

   if (ti->txt_filename)  // used for items.txt
   {
      if ((ret = read_file(ti->txt_filename, (void **)&txt_buffer, &txt_size)) != ERROR_SUCCESS)
         return ret;
   }

   EVNLoadNamesItemsText(filename, &ttentry, &tbl_size);

   unsigned short *tbl;

   if (ti->txt_filename)  // used for items.txt
      tbl = CompressTextAlt(txt_buffer+ti->txt_offset, out_size, ti->txt_size, ttentry, tbl_size, lzo_compress);
   else   // used for names.txt
      tbl = CompressTextAlt(buffer+ti->txt_offset, out_size, ti->txt_size, ttentry, tbl_size, lzo_compress);

   if (tbl == NULL)
   {
      ret = ERROR_INSUFFICIENT_BUFFER;
      goto error;
   }
   for (unsigned long i = 0; i < (tbl_size+1); i++)
         WordSwapPtr(tbl+i);
   memcpy(buffer+ti->tbl_addr, tbl, (tbl_size+1)*sizeof(unsigned short));
   free(tbl);

   if (((tbl_size+1) * 2) > ti->tbl_size)
   {
      printf("Error table size is too large. Are there extra lines?\n");
      ret = ERROR_BUFFER_OVERFLOW;
      goto error;
   }

   // Patch pointers
   for (int i = 0; i < ti->num_x_reft; i++)
   {
      unsigned long *data=(unsigned long *)(buffer+ti->x_reft[i]-ti->prog_addr);
      data[0] = DoubleWordSwap(ti->txt_addr+ti->x_reft_off[i]);
   }

   if (fopen_s(&fp, out_filename, "wb") != 0)
   {
      ret = ERROR_ACCESS_DENIED;
      goto error;
   }

   fwrite((void *)buffer, 1, size, fp);
   fclose(fp);

   if (ti->txt_filename)  // used for items.txt
   {
      if (fopen_s(&out_fp, ti->txt_filename, "wb") != 0)
      {
         ret = ERROR_ACCESS_DENIED;
         goto error;
      }

      fwrite((void *)txt_buffer, 1, txt_size, fp);
      fclose(fp);
   }

error:
   if (fp)
      fclose(fp);
   if (out_fp)
      fclose(out_fp);
   if (buffer)
      free(buffer);

   return ret;
}

void print_usage()
{
   printf("usage: -d [input filename] [names filename] [items filename]\n");
   printf("       -i [names filename] [items filename] [0.bin filename] [text output filename] [text output offset]\n");
}

int main(int argc, char *argv[])
{
   if (argc < 2)
   {
      print_usage();
      exit(1);
   }

   if (strcmp(argv[1], "-d") == 0)
   {      
      text_info_struct ti[2] = 
      {
         { 0xAF3F8, 0x108, 0xAEC4C, 0x434 },
         { 0xAF080, 0x378, 0xAD520, 0x172C },
      };

      if (argc != 5)
      {
         print_usage();
         exit(1);
      }

      for (int i = 0; i < 2; i++)
      {
         printf("extracting %s from %s...", argv[i+3], argv[2]);
         int ret = extract_data(ti+i, argv[2], argv[i+3]);
         if (ret == ERROR_SUCCESS)
            printf("done.\n");
         else
         {
            LPSTR pBuffer;

            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                          NULL, ret, 0,(LPTSTR)&pBuffer, 0, NULL) ;

            printf("error: %s\n", pBuffer);
            LocalFree(pBuffer);
         }
      }
   }
   else if (strcmp(argv[1], "-i") == 0)
   {
      int ret;
      text_info_struct ti[2] = 
      {
           // tbl_addr, tbl_size, txt_offset, txt_size
           { 0xAF3F8, 0x108, 0xAD520, 0x1728 }, // names.txt: table addr: 0x060B33F8, text addr: 0x060B2C4C
           { 0xAF080, 0x378, 0xAD520, 0x3180 }, // items.txt: table addr: 0x060B3080, text addr: 0x060B1524
      };
      int out_size;

      // used for names.txt
      ti[0].prog_addr = ti[1].prog_addr = 0x06004000;
      ti[0].txt_filename = NULL;
      ti[0].txt_addr = 0x060B1520;
      memset(ti[0].x_reft_off, 0, sizeof(ti[0].x_reft_off));
      ti[0].x_reft[0] = 0x06074594;
      ti[0].x_reft[1] = 0x06074498; // pointer + 4
      ti[0].x_reft_off[1] = 4;
      ti[0].num_x_reft = 2;

      // used for items.txt
      ti[1].txt_filename = argv[5];  // KANJI.BIN
      ti[1].txt_addr = 0x05E6B800+strtol(argv[6], NULL, 0);  // 0x2400
      ti[1].txt_offset = strtol(argv[6], NULL, 0);  
      memset(ti[1].x_reft_off, 0, sizeof(ti[1].x_reft_off));
      ti[1].x_reft[0] = 0x0607204C; // pointer + 4
      ti[1].x_reft[1] = 0x06072214; // pointer + 4
      ti[1].x_reft[2] = 0x06072218;
      ti[1].x_reft[3] = 0x06072328; // pointer + 4
      ti[1].x_reft[4] = 0x06072330;
      ti[1].x_reft[5] = 0x06073458; // pointer + 4
      ti[1].x_reft[6] = 0x060734CC;
      ti[1].x_reft[7] = 0x06074494; // pointer + 4
      ti[1].x_reft[8] = 0x060744A8;
      ti[1].x_reft[9] = 0x06074E48; // pointer + 4
      ti[1].x_reft_off[0] = 4;
      ti[1].x_reft_off[1] = 4;
      ti[1].x_reft_off[3] = 4;
      ti[1].x_reft_off[5] = 4;
      ti[1].x_reft_off[7] = 4;
      ti[1].x_reft_off[9] = 4;
      ti[1].num_x_reft = 10;

      if (argc != 7)
      {
         print_usage();
         exit(1);
      }

      for (int i = 0; i < 2; i++)
      {
         printf("Inserting %s into %s and %s...", argv[i+2], argv[4], argv[5]);
         ret = insert_data(&ti[i], argv[i+2], argv[4], &out_size, FALSE);

         if (ret == ERROR_SUCCESS)
            printf("done.\n");
         else
         {
            LPSTR pBuffer;

            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
               NULL, ret, 0,(LPTSTR)&pBuffer, 0, NULL) ;

            printf("error: %s\n", pBuffer);
            LocalFree(pBuffer);
         }
      }
   }
   else
   {
      print_usage();
      exit(1);
   }

  printf("Done.\n");
}