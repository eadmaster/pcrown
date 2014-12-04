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

#ifndef xml_h__
#define xml_h__

typedef struct
{
   int id;
   char filename[MAX_PATH];
   int width;
   int height;
} bitmap_struct;

typedef struct
{
   char character[32];
   unsigned long code;
   int bm;
   int x;
   int y;
   int width;
   int height;
   int avg_width;
   int lsb;
} glyph_struct;

typedef struct
{
   int base;
   int height;
   bitmap_struct *bitmaps;
   int num_bitmaps;
   glyph_struct *glyphs;
   int num_glyphs;
} font_struct;

BOOL read_font_xml(char *filename, font_struct *font);

#endif // xml_h__