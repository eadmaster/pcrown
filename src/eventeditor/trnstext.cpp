/*  Copyright 2004-2006, 2012 Theo Berkau

    This file is part of EventEditor.

    EventEditor is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    EventEditor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with EventEditor; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "evn.h"
#include "main.h"
#include "trnstext.h"
#include "util.h"

BOOL TransConvertText(transtext_struct *transtext)
{
   char text[8192 * 2];
   int max_ttentry=0;
	int id=0;
   
   transtext->ttentry = NULL;
   transtext->num = 0;

	for (int j = -1; j < 8; j++)
	{
		unsigned long num=EVNGetCompressionNum(j);

		if (j > -1 && EVNGetCompressionOffsetAlt(j) == 0)
			break;
		for (unsigned long i = 0; i < num; i++)
		{
			command_struct cmd;
			cmd.cmd43_var = j;
			cmd.arg[0] = i;
			GenerateDialogText(&cmd, TRUE, text, sizeof(text), FALSE);

			ReallocMem((void **)&transtext->ttentry, sizeof(ttentry_struct), &max_ttentry, transtext->num);
			memset(&transtext->ttentry[transtext->num], 0, sizeof(ttentry_struct));
			transtext->ttentry[transtext->num].event_id = ((j+1)<<16) | i;
			id++;
			transtext->ttentry[transtext->num].orig_text = _strdup(text);
			if (dupe_trans)
				transtext->ttentry[transtext->num].trans_text = _strdup(text);
			else
				transtext->ttentry[transtext->num].trans_text = NULL;
			if (transtext->ttentry[transtext->num].orig_text == NULL)
				return FALSE;
			transtext->num++;
		}
	}

   return TRUE;
}

bool TransTextSave(const char *filename, transtext_struct *transtext)
{
   int i;
   FILE *fp;

   if (fopen_s(&fp, filename, "wt") != 0)
      return false;

   for (i = 0; i < transtext->num; i++)
   {
      fprintf(fp, "%d|\n%s|\n",
              transtext->ttentry[i].event_id,
				  transtext->ttentry[i].orig_text == NULL ? "" : transtext->ttentry[i].orig_text);
   }
   fclose(fp);
   return true;
}

int TransTextLoad(const char *filename, transtext_struct *transtext)
{
   int i,j;
   FILE *fp;
   char *event_text=NULL;
   char *orig_text=NULL;
   int tt_count=0;
   int ret;
   int max_data=0;
   int buffer_size=0x10000;
   int line=1;

   if (fopen_s(&fp, filename, "rt") != 0)
      return -1;

   event_text = (char *)malloc(buffer_size);
   orig_text = (char *)malloc(buffer_size);

   if (event_text == NULL || orig_text == NULL)
   {
      if (event_text)
         free(event_text);
      if (orig_text)
         free(orig_text);
   }

   transtext->num = tt_count;
   tt_count = 0;
   i = 0;
   for(;;)
   {
      char *text_list[] = { event_text, orig_text };
      bool done=false;

      memset(event_text, 0, buffer_size);
      memset(orig_text, 0, buffer_size);

      for (j = 0; j < 2; j++)
      {
         // Keep reading until EOF or | at end
         for (;;)
         {
            if ((ret = fscanf_s(fp, "%[^\n]\n", text_list[j]+strlen(text_list[j]), buffer_size-strlen(text_list[j]))) != 1)
            {
               if (feof(fp) && j != 0)
               {
                  printf("%s(%d): Unexpected end of file\n", filename, line);
                  free(transtext->ttentry);
                  i = 0;
                  ret = -2;
               }
               else
                  ret = 0;
               done = true;
               break;
            }
            else
               line++;

            if (text_list[j][strlen(text_list[j])-1] == '|')
               break;
         }
         if (done)
            break;
      }

      if (done)
         break;

      event_text[strlen(event_text)-1] = '\0';
      orig_text[strlen(orig_text)-1] = '\0';

      ReallocMem((void **)&transtext->ttentry, sizeof(ttentry_struct), &max_data, i);
      memset(&transtext->ttentry[i], 0, sizeof(ttentry_struct));

      if (strlen(event_text) == 0 || (atoi(event_text) == 0 && strcmp(event_text, "0") != 0))
      {
         printf("%s(%d): Event ID is invalid\n", filename, line-4);
         free(transtext->ttentry);
         i = 0;
         ret = -3;
         break;
      }

      // Search for duplicates
      for (j = 0; j < i; j++)
      {
         if (transtext->ttentry[j].event_id == atoi(event_text))
         {
            printf("%s(%d): Event ID already used\n", filename, line-4);
            free(transtext->ttentry);
            i = 0;
            ret = -4;
            done = true;
            break;
         }
      }

      if (done)
         break;

      transtext->ttentry[i].event_id = atoi(event_text);
      transtext->ttentry[i].orig_text = strlen(orig_text) > 0 ? _strdup(orig_text) : NULL;
      i++;
   }
   transtext->num = i;
   free(event_text);
   free(orig_text);
   fclose(fp);
   return ret;
}

void TransTextFree(transtext_struct *transtext)
{
   int i;

   if (transtext)
   {
      if (transtext->ttentry)
      {
         for (i = 0; i < transtext->num; i++)
         {
            if (transtext->ttentry[i].trans_text)
               free(transtext->ttentry[i].trans_text);
            if (transtext->ttentry[i].notes)
               free(transtext->ttentry[i].notes);
         }
         free(transtext->ttentry);
      }
   }
}
