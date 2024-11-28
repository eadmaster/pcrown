/*  Copyright 2013 Theo Berkau

    This file is part of PRGUTIL.

    PRGUTIL is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    PRGUTIL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with PRGUTIL; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>

int ReallocMem(void **data, int data_size, int *max_data, int index)
{
   if (index >= max_data[0])
   {
      void *temp;
      int old_max_data=max_data[0];

      if (max_data[0] == 0)
      {
         max_data[0] = 10;
         if (index > max_data[0])
            max_data[0] = index * 2;
         if ((data[0] = malloc(data_size * max_data[0])) == NULL)
            return FALSE;
         return TRUE;
      }

      max_data[0] = index * 2;
      if ((temp = malloc(data_size * max_data[0])) == NULL)
         return FALSE;

      if (data[0] != NULL)
      {
         memcpy(temp, data[0], data_size * old_max_data);
         free(data[0]);
      }

      data[0] = temp;
   }

   return TRUE;
}

typedef struct  
{
   unsigned char unknown1[0x1C];
   unsigned long addr1;
   unsigned long addr2;
   unsigned long addr3;
   unsigned long addr4;
   unsigned long addr5;
   unsigned long unknown2;
   unsigned char unknown3[4];
   unsigned short unknown4;
   unsigned short unknown5;
   unsigned long addr6;
} prg_header_struct;

enum
{
   ERR_NONE=0, ERR_FILENOTFOUND, ERR_MEM
};

int PRGDump(char *filename, char *out_dir)
{
   FILE *fp;
   unsigned char *buf;
   int size;
   char out_filename[MAX_PATH];

   if (fopen_s(&fp, filename, "rb") != 0)
      return ERR_FILENOTFOUND;

   size = _filelength(_fileno(fp));
   buf = (unsigned char *)malloc(size);
   if (buf == NULL)
   {
      fclose(fp);
      return ERR_MEM;
   }

   fread((void *)buf, 1, size, fp);
   fclose(fp);

   strcpy_s(out_filename, sizeof(out_filename), out_dir);
   char *p = strrchr(filename, '\\');

   if (p)
      strcat_s(out_filename, sizeof(out_filename), p);
   else
   {
      strcat_s(out_filename, sizeof(out_filename), "\\");
      strcat_s(out_filename, sizeof(out_filename), filename);
   }

   p = strrchr(out_filename, '.');

   if (p)
      strcpy_s(p, sizeof(out_filename)-(p-out_filename), ".TXT");
   else
      strcat_s(out_filename, sizeof(out_filename), ".TXT");

   if (fopen_s(&fp, out_filename, "wt") != 0)
   {
      free(buf);
      return ERR_FILENOTFOUND;
   }

   int offset=0x40;
   for (int i = 0; i < 4; i++)
   {
      fprintf(fp, "%s\n", buf+offset);
      offset += (int)strlen((char *)buf+offset)+1;
      offset = (offset + 3) & 0xFFFFFFFC;
      offset += 0x90;
   }

   fclose(fp);
   free(buf);

   return ERR_NONE;
}

int PRGInsert(char *filename, char *out_dir)
{
   return ERR_NONE;
}

typedef struct  
{
   char filename[MAX_PATH];
} input_file_struct;

int main(int argc, char *argv[])
{
   int i;
   char out_dir[MAX_PATH];
   int input_count=0, max_input_file=0;
   input_file_struct *input=NULL;
   BOOL dump_prg = TRUE;
   BOOL insert_prg = FALSE;

   if (argc < 2)
   {
      printf ("prgutil [options] [list of prg or text files]\n");
      exit(1);
   }

   for (i = 1; i < argc;)
   {
      if (strcmp(argv[i], "-d") == 0)
      {
         dump_prg = TRUE;
         i++;
      }
      else if (strcmp (argv[i], "-i") == 0)
      {
         insert_prg = TRUE;
         i++;
      }
      else if (strcmp (argv[i], "-o") == 0)
      {
         strcpy_s(out_dir, sizeof(out_dir), argv[i+1]);
         i+=2;
      }
      else
      {
         ReallocMem((void **)&input, sizeof(input_file_struct), &max_input_file, input_count);
         strcpy_s(input[input_count].filename, sizeof(input[input_count].filename), argv[i]);
         input_count++;
         i++;
      }
   }

   if (insert_prg)
      dump_prg = FALSE;

   if (input == NULL)
   {
      printf ("no input filename(s) specified\n");
      exit (1);
   }

   for (i = 0; i < input_count; i++)
   {
      if (dump_prg)
         PRGDump(input[i].filename, out_dir);
      else if (insert_prg)
         PRGInsert(input[i].filename, out_dir);
   }

   return 0;
}
