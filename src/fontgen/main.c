/*  Copyright 2012-2014 Theo Berkau

    This file is part of FONTGEN.

    FONTGEN is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FONTGEN is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FONTGEN; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#include <stdio.h>

#define USE_CONSOLE
#include <allegro.h>
#include <winalleg.h>
//#include <loadpng.h>
#include <math.h>
#include "xml.h"

#define LINESPACE       8
#define LINENIDENT      8
#define SPACEBETWEEN    4
#define SPACETOPBELOW   32

#define ENABLEADDR    1
//#undef ENABLEADDR

BOOL round_up_width=FALSE;

void copy_glyph(glyph_struct *glyph, BITMAP *bmp, unsigned char *buffer)
{
   int i;
   int width;
   int w_count=0;
   int h_count=0;

   if (glyph->width > 12)
   {
      printf("Warning: Character %c will have width clipped\n", glyph->code);
      width = 12;
   }
   else
      width = glyph->width;

   if (glyph->height > 12)
   {
      int j;
      int black=makecol8(0, 0, 0);
      for (j = 12; j < glyph->height; j++)
      {
         for (i = 0; i < glyph->width; i++)
         {
            if (getpixel(bmp, glyph->x+i, glyph->y+j) != black)
            {
               printf("Warning: Character %c will have height clipped\n", glyph->code);
               break;
            }
         }
      }
   }

   // Write width to vwf table
   if (width == 0 && glyph->code == ' ')
   {
      buffer[glyph->code] = glyph->avg_width;
      if (round_up_width && buffer[glyph->code] % 2 == 1)
         buffer[glyph->code]++;
      return;
   }
   else
      buffer[glyph->code] = glyph->width;

   if (round_up_width && buffer[glyph->code] % 2 == 1)
      buffer[glyph->code]++;

   buffer += (glyph->code * (12 * 12 / 8));

   for (;;)
   {
      unsigned char data=0;

      for (i = 0; i < 8; i++)
      {
         int pixel;
         int black=makecol8(0, 0, 0);
         int white=makecol8(255, 255, 255);

         if (w_count < width)
            pixel = getpixel(bmp, glyph->x+w_count, glyph->y+h_count);
         else
            pixel = 0;

         data <<= 1;
         data |= pixel > 1;
         w_count++;
         if (w_count == 12)
         {
            w_count = 0;
            h_count++;
         }
      }

      // Write data
      buffer[0] = data;
      buffer++;

      if (h_count == 12)
         break;
   }
}

void convert_slash(char *str)
{
   char *p;
   if ((p = strrchr(str, '/')) != NULL)
   {
      do
      {
         p[0] = '\\';
      } while ((p = strrchr(str, '/')) != NULL);
   }
}

int main(int argc, char *argv[])
{
   char *input_filename=NULL;
   char *output_filename=NULL;
   BITMAP *bmp=NULL;
   unsigned char *output_buffer=NULL;
   FILE *fp;
   int i;
   STARTUPINFO si;
   PROCESS_INFORMATION pi;
   char path[MAX_PATH];
   char args[MAX_PATH * 3];
   RGB pal[256];
   char xml_filename[MAX_PATH], png_filename[MAX_PATH];
   char *p;
   font_struct font;
	char *bmfontgen_filename="BMFontGen.exe";

   memset(&font, 0, sizeof(font));
   if (argc < 4)
   {
      printf ("fontgen [options] [filename].txt -o [filename].bin\n");
      goto error;
   }

   for (i = 1; i < argc;)
   {
      if (strcmp(argv[i], "-2") == 0)
      {
         round_up_width = TRUE;
         i++;
      }
		else if (strcmp(argv[i], "-b") == 0)
		{
			bmfontgen_filename = argv[i+1];
			i += 2;
		}
      else if (strcmp (argv[i], "-o") == 0)
      {
         output_filename = argv[i+1];
         i += 2;
      }
      else
      {
         input_filename = argv[i];
         i++;
      }
   }
   
   if (input_filename == NULL)
   {
      printf ("no input filename specified\n");
      exit (1);
   }

   if (output_filename == NULL)
   {
      printf ("no output filename specified\n");
      exit (1);
   }

   convert_slash(input_filename);
   convert_slash(output_filename);

   // Execute BMFontGen
   sprintf_s(args, sizeof(args), "%s -optfile ", bmfontgen_filename);
   strcat_s(args, sizeof(args), input_filename);

   strcpy_s(path, sizeof(path), input_filename);

   if ((p = strrchr(path, '\\')) != NULL)
      p[1] = '\0';

   strcat_s(args, sizeof(args), " -output ");
   strcat_s(args, sizeof(args), path);
   strcat_s(args, sizeof(args), "font");

   ZeroMemory(&si, sizeof(si));
   si.cb = sizeof(si);
   si.wShowWindow = SW_HIDE;
   si.dwFlags = 0;

   printf("%s\n", args);
   if (CreateProcessA(NULL, args, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi) == 0)
   {
      LPVOID lpMsgBuf;
      DWORD err=GetLastError();
      FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
         FORMAT_MESSAGE_FROM_SYSTEM |
         FORMAT_MESSAGE_IGNORE_INSERTS,
         NULL,
         err,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
         (LPTSTR) &lpMsgBuf, 0, NULL);

      printf("Error executing BMFontGen: %s\n", lpMsgBuf);
      LocalFree(lpMsgBuf);
      exit(1);
   }

   printf("Creating font...");
   WaitForSingleObject(pi.hProcess, INFINITE);

   printf("done\n");

   // Close process and thread handles. 
   CloseHandle( pi.hProcess );
   CloseHandle( pi.hThread );

   // Load XML file
   strcpy_s(xml_filename, sizeof(xml_filename), path);
   strcat_s(xml_filename, sizeof(xml_filename), "font.xml");
   read_font_xml(xml_filename, &font);

   // Start going through XML and copying font to our 12x12 1BPP buffer
   allegro_init();
   loadpng_init();

   set_color_depth(8);

   if (strrchr(font.bitmaps[0].filename, '\\') == NULL)
   {
      strcpy_s(png_filename, sizeof(png_filename), input_filename);
      p = strrchr(png_filename, '\\');
      strcpy_s(p+1, sizeof(png_filename)-((p+1)-png_filename), font.bitmaps[0].filename);
   }
   else
      strcpy_s(png_filename, sizeof(png_filename), font.bitmaps[0].filename);

   if ((bmp = load_bitmap(png_filename, pal)) == NULL)
	{
		printf("Error loading generated png file\n");
		exit(1);
	}

   output_buffer = malloc(12 * 12 / 8 * 1024);

   if (output_buffer == NULL)
   {
      printf("Error allocating buffer\n");
      exit(1);
   }

   memset(output_buffer, 0, 12 * 12 / 8 * 1024);

   // Copy glyphs to raw buffer
   for (i = 0; i < font.num_glyphs; i++)
      copy_glyph(&font.glyphs[i], bmp, output_buffer);

   // Write final output  
   if (fopen_s(&fp, output_filename, "wb") != 0)
   {
      printf("Error writing output\n");
      goto error;
   }

   fwrite((void *)output_buffer, 1, 12 * 12 / 8 * 1024, fp);
   fclose(fp);
error:
   if (bmp)
      destroy_bitmap(bmp);

   if (font.bitmaps)
      free(font.bitmaps);
   if (font.glyphs)
      free(font.glyphs);

   //printf ( "Press any key to continue..." );
   //getch();

   return 0;
}

END_OF_MAIN()