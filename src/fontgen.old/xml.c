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

#include <windows.h>
#include <stdio.h>
#include <libxml/xmlreader.h>
#include "xml.h"

int get_prop_int(xmlNode *node, const xmlChar *name)
{
   xmlAttrPtr attr = xmlHasProp(node, name);
   if (attr)
      return atoi(attr->children->content);
   return 0;
}

int get_prop_hex(xmlNode *node, const xmlChar *name)
{
   xmlAttrPtr attr = xmlHasProp(node, name);
   if (attr)
   {
      int ret;
      sscanf_s(attr->children->content, "%x", &ret);
      return ret;
   }
   return 0;
}

char *get_prop_string(xmlNode *node, const xmlChar *name)
{
   xmlAttrPtr attr = xmlHasProp(node, name);
   if (attr)
      return attr->children->content;
   return NULL;
}

void get_prop_pos(xmlNode *node, const xmlChar *name, int *x, int *y)
{
   xmlAttrPtr attr = xmlHasProp(node, name);
   if (attr)
   {
      sscanf_s(attr->children->content, "%d,%d", x, y);
      return;
   }
}

void get_prop_dimension(xmlNode *node, const xmlChar *name, int *w, int *h)
{
   xmlAttrPtr attr = xmlHasProp(node, name);
   if (attr)
   {
      sscanf_s(attr->children->content, "%dx%d", w, h);
      return;
   }
}

xmlNode *find_node(xmlNode *cur, const xmlChar *name, int index)
{
   xmlNode *cur_node = NULL;
   int count=0;

   for (cur_node = cur->children; cur_node->next != NULL; cur_node = cur_node->next)
   {
      if (strcmp(cur_node->name, name) == 0)
      {
         if (index == count)
            return cur_node;
         else
            count++;
      }
   }
   return NULL;
}

int get_child_num(xmlNode *cur, const xmlChar *name)
{
   xmlNode *cur_node;
   int count=0;

   for (cur_node = cur->children; cur_node->next != NULL; cur_node = cur_node->next)
   {
      if (strcmp(cur_node->name, name) == 0)
         count++;
   }
   return count;
}

BOOL read_font_xml(char *filename, font_struct *font)
{
    xmlDocPtr doc;
    xmlNode *root_element = NULL;
    xmlNode *cur_node = NULL, *bitmaps=NULL, *glyphs=NULL;
    int i;

    // Initialize library
    LIBXML_TEST_VERSION

    doc = xmlReadFile(filename, NULL, 0);

    if (doc == NULL) {
       printf("error: could not parse file %s\n", filename);
    }

    // Get the root element node
    root_element = xmlDocGetRootElement(doc);

    // Get Font properties
    memset(font, 0, sizeof(font_struct));
    font->base = get_prop_int(root_element, "base");
    font->height = get_prop_int(root_element, "height");

    // Get Bitmap properties
    bitmaps = find_node(root_element, "bitmaps", 0);
    font->num_bitmaps = get_child_num(bitmaps, "bitmap");
    if ((font->bitmaps = malloc(font->num_bitmaps * sizeof(bitmap_struct))) == NULL)
    {
       printf("Error allocating memory\n");
       goto error;
    }

    for (i = 0; i < font->num_bitmaps; i++)
    {
       cur_node = find_node(bitmaps, "bitmap", i);
       font->bitmaps[i].id = get_prop_int(cur_node, "id");
       strcpy_s(font->bitmaps[i].filename, sizeof(font->bitmaps[i].filename), get_prop_string(cur_node, "name"));
       get_prop_dimension(cur_node, "size", &font->bitmaps[i].width, &font->bitmaps[i].height);
    }

    // Get Glyphs
    glyphs = find_node(root_element, "glyphs", 0);
    font->num_glyphs = get_child_num(glyphs, "glyph");
    if ((font->glyphs = malloc(font->num_glyphs * sizeof(glyph_struct))) == NULL)
    {
       printf("Error allocating memory\n");
       goto error;
    }

    for (i = 0; i < font->num_glyphs; i++)
    {
       cur_node = find_node(glyphs, "glyph", i);
       font->glyphs[i].code = get_prop_hex(cur_node, "code");
       font->glyphs[i].bm = get_prop_int(cur_node, "bm");
       get_prop_pos(cur_node, "origin", &font->glyphs[i].x, &font->glyphs[i].y);
       get_prop_dimension(cur_node, "size", &font->glyphs[i].width, &font->glyphs[i].height);
       font->glyphs[i].avg_width = get_prop_int(cur_node, "aw");
       font->glyphs[i].lsb = get_prop_int(cur_node, "lsb");
    }

    if (doc != NULL) 
	    xmlFreeDoc(doc);
    else
       return FALSE;

    xmlCleanupParser();
    return TRUE;
error:
    if (doc != NULL) 
       xmlFreeDoc(doc);
    else
       return FALSE;

    xmlCleanupParser();

    if (font->bitmaps)
       free(font->bitmaps);
    return FALSE;
}
