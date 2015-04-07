/*  Copyright 2013 Theo Berkau

    This file is part of SATCODE-INSERT.

    SATCODE-INSERT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    SATCODE-INSERT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SATCODE-INSERT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <io.h>
#include <string.h>
#include "../eventeditor/util.h"

typedef struct
{
   int offset;
   int max_size;
   int patch_offset;
   int patch_size;
   unsigned char *data;
} patch_struct;

void free_patch(patch_struct *patch, int num_patches)
{
   int i;

   if (patch)
   {
      for (i = 0; i < num_patches; i++)
      {
         if (patch[i].data)
            free(patch[i].data);
      }
      free(patch);
   }
}

patch_struct *load_patch(char *filename, int *num_patches)
{
   FILE *fp=NULL;
   patch_struct *patch=NULL;
   int i;

   num_patches[0]=0;

   // Load up compiled code
   if (fopen_s(&fp, filename, "rb") != 0)
   {
      printf("Error opening %s\n", filename);
      goto error;
   }

   fread((void *)num_patches, 4, 1, fp);
   num_patches[0] = DoubleWordSwap(num_patches[0]);

   patch = (patch_struct *)calloc(num_patches[0], sizeof(patch_struct));

   if (patch == NULL)
   {
      printf("Error allocating memory\n");
      goto error;
   }

   for (i = 0; i < num_patches[0]; i++)
   {
      fread((void *)&patch[i].offset, sizeof(int), 1, fp);
      fread((void *)&patch[i].max_size, sizeof(int), 1, fp);
      fread((void *)&patch[i].patch_offset, sizeof(int), 1, fp);
      fread((void *)&patch[i].patch_size, sizeof(int), 1, fp);
      patch[i].offset = DoubleWordSwap(patch[i].offset);
      patch[i].max_size = DoubleWordSwap(patch[i].max_size);
      patch[i].patch_offset = DoubleWordSwap(patch[i].patch_offset);
      patch[i].patch_size = DoubleWordSwap(patch[i].patch_size);

      if (patch[i].patch_size > patch[i].max_size)
      {
         printf("Error patch %d is too large\n", i+1);
         goto error;
      }
   }

   for (i = 0; i < num_patches[0]; i++)
   {
      if ((patch[i].data = malloc(patch[i].patch_size)) == NULL)
      {
         printf("Error allocating memory\n");
         exit(1);
      }

      fseek(fp, patch[i].patch_offset, SEEK_SET);
      fread((void *)patch[i].data, 1, patch[i].patch_size, fp);
   }

   fclose(fp);
   return patch;

error:
   if (fp)
      fclose(fp);

   free_patch(patch, num_patches[0]);
   return NULL;
}

int main(int argc, char *argv[])
{
   char *input_filename;
   char *output_filename;
   int i;
   FILE *fp;
   patch_struct *patch=NULL;
   int num_patches=0;
   int size;
   unsigned char *data=NULL;

   if (argc != 3)
   {
      printf ("satcode-insert compiled_code.bin target_file.bin\n");
      goto error;
   }

   input_filename = argv[1];
   output_filename = argv[2];

   // Load up patch
   if ((patch = load_patch(input_filename, &num_patches)) == NULL)
      exit(1);

   // Load up output
   if (fopen_s(&fp, output_filename, "rb") != 0)
   {
      printf("Error opening %s\n", output_filename);
      goto error;
   }

   size = _filelength(_fileno(fp));
   if ((data = malloc(size)) == NULL)
      goto error;

   fread((void *)data, 1, size, fp);
   fclose(fp);

   // Now apply patches
   for (i = 0; i < num_patches; i++)
      memcpy(data+patch[i].offset, patch[i].data, patch[i].patch_size);

   // Lastly, let's write file
   if (fopen_s(&fp, output_filename, "wb") != 0)
   {
      printf("Error writing %s\n", output_filename);
      goto error;
   }

   fwrite((void *)data, 1, size, fp);
   fclose(fp);

   printf("done\n");

error:
   free_patch(patch, num_patches);

   if (data)
      free(data);
   if (fp)
      fclose(fp);

   return 0;
}
