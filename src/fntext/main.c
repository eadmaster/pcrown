/*  Copyright 2012-2014 Theo Berkau

    This file is part of FNTEXT.

    FNTEXT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FNTEXT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FNTEXT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#include <stdio.h>

#define USE_CONSOLE
#include <allegro.h>
#include <winalleg.h>
#include <math.h>

#define LINESPACE       8
#define LINENIDENT      8
#define SPACEBETWEEN    4
#define SPACETOPBELOW   32

#define ENABLEADDR    1
//#undef ENABLEADDR

int main(int argc, char *argv[])
{
   char *input_filename;
   char *output_filename;
   BITMAP *temp_buffer;
   BITMAP *output_buffer;
   FILE *fp;
   int i, i2, i3;
   unsigned long f_size=0;
   unsigned long f_size2=0;

   if (argc < 4)
   {
      printf ("fntext [filename].bin -o [filename].bmp\n");
      exit (1);
   }
   if (strcmp (argv[argc - 2], "-o") == 0)
   {
      output_filename = argv[argc - 1];
   }
   else
   {
      printf ("no filename specified\n");
      exit (1);
   }

   input_filename = argv[1];

   if (fopen_s (&fp, input_filename, "rb") != 0)
   {
      printf("Could not open Princess Crown font file: %s\n", input_filename);
      exit (1);
   }

   fseek (fp, 0, SEEK_END);
   f_size = ftell(fp) - 1;
   fseek (fp, 0, SEEK_SET);

   allegro_init();

   set_color_depth(8);

   output_buffer = create_bitmap(40 + LINENIDENT + ((12+SPACEBETWEEN) * 16), ((f_size / 18) / 16) * (12+LINESPACE) + (SPACETOPBELOW*2));

   temp_buffer = create_bitmap(12, 12);

   clear_bitmap(output_buffer);

   if (output_buffer == NULL || temp_buffer == NULL)
   {
      printf("couldn't create buffers\n");
      exit (1);
   }

   for (i2 = 0; i2 < 0x400; i2++)
   {
#ifdef ENABLEADDR
      textprintf(output_buffer, font, 0, SPACETOPBELOW + ((12 + LINESPACE) * i2) + 2, 255, "%04x", 16 * i2);
#endif

      for (i = 0; i < 16; i++)
      {
         int j;
         unsigned char pix=0;
         unsigned long counter=0;

         for(i3 = 0; i3 < (12 + 6); i3++)
         {
            fread((void *)&pix, 1, 1, fp);

            for(j = 0; j < 8; j++)
            {
               putpixel(temp_buffer, (counter + j) % 12,
                        (counter + j) / 12,
                        (pix >> 7) & 1);
               pix <<= 1;
            }
            counter += 8;
         }
         blit(temp_buffer, output_buffer, 0, 0, 40 + LINENIDENT + (i * (12+SPACEBETWEEN)), SPACETOPBELOW+(i2 * (12+LINESPACE)), 12, 12);
      }
   }
  
   fclose (fp);

   save_bmp(output_filename, output_buffer, default_palette);
   destroy_bitmap(output_buffer);
   destroy_bitmap(temp_buffer);

   return 0;
}
