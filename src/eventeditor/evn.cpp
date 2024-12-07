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
#include <tchar.h>
#include <assert.h>
#include <commctrl.h>
#include <stdio.h>
#include <io.h>
#include "evn.h"
#include "main.h"
#include "util.h"
#include "resource.h"
#include "texttbl.h"
#include "trnstext.h"
#ifdef USE_GUI
#include "uihelper.h"
#endif

//////////////////////////////////////////////////////////////////////////////

#define MAX_LINE_PIXEL_WIDTH 272

bool file_loaded=false;
unsigned char *event_data=NULL;
unsigned long num_commands=0;
command_struct *command=NULL;
textdataheader_struct textdataheader;
transtext_struct maintranstext;
ttentry_struct *names_data=NULL;
unsigned long num_names=0;

typedef struct 
{
   unsigned short offsets1[8];
   unsigned short cmp_tbl_offset;
   unsigned short offsets2[8];
   unsigned short cmp_data_offset;
   unsigned short offsets3[8];
} evn_header_struct;

evn_header_struct header;

unsigned char cmd43_var=0xFF;
int special_flags=0;
char **pattern_list=NULL;
int num_patterns=0;

unsigned char *font_data=NULL;

bool IsDialogueCommand(command_struct *command)
{
   if (command->cmd == 0x04)
      return true;

   return false;
}

int EVNFreeFile()
{
   if (event_data)
   {
      free(event_data);
      event_data = NULL;
   }

   if (command)
   {
      free(command);
      command = NULL;
      num_commands = 0;
   }

   // free commands, etc. here

   file_loaded = false;

   return 0;
}

void EVNParseHeader(unsigned char *data, evn_header_struct *header)
{
   int i;

   header->offsets1[0] = 0x40;

   for (i = 1; i < 8; i++)
   {
      header->offsets1[i] = WordSwap(*((unsigned short*)data));
      data+=2;
   }

   header->cmp_tbl_offset = WordSwap(*((unsigned short*)data));
   data += 2;

   for (i = 0; i < 8; i++)
   {
      header->offsets2[i] = WordSwap(*((unsigned short*)data));
      data+=2;
   }

   header->cmp_data_offset = WordSwap(*((unsigned short*)data));
   data += 2;

   for (i = 0; i < 8; i++)
   {
      header->offsets3[i] = WordSwap(*((unsigned short*)data));
      data+=2;
   }
}


void null_list_func(command_struct *command, int index, HWND hWnd)
{
}

void jump_list_func(command_struct *cmd, int index, HWND hWnd)
{
#ifdef USE_GUI
   unsigned long i;
   char text[128];
   int offset = DoubleWordSwap(*((unsigned long *)cmd->arg));

   if (offset == 0)
      ListView_AddTextItem(hWnd, IDC_LVEVENTLIST, index, 2, _T("End"));

   for (i = 0; i < num_commands; i++)
   {
      if (command[i].offset == offset)
      {
         sprintf_s(text, sizeof(text), "%d", i);
         ListView_AddTextItem(hWnd, IDC_LVEVENTLIST, index, 2, text);
         break;
      }
   }
#endif
}

void dodialog_list_func(command_struct *command, int index, HWND hWnd)
{
#ifdef USE_GUI
   WCHAR unicode[2048];

   GenerateDialogText(command, FALSE, unicode, sizeof(unicode) / sizeof(WCHAR), TRUE);
   ListView_AddTextItemW(hWnd, IDC_LVEVENTLIST, index, 2, unicode);
#endif
}

void condbra_list_func(command_struct *cmd, int index, HWND hWnd)
{
#ifdef USE_GUI
   unsigned long i;
   char text[128];
   int offset = DoubleWordSwap(*((unsigned long *)(cmd->arg+3)));

   for (i = 0; i < num_commands; i++)
   {
      if (command[i].offset == offset)
      {
         sprintf_s(text, sizeof(text), "%d", i);
         ListView_AddTextItem(hWnd, IDC_LVEVENTLIST, index, 2, text);
         break;
      }
   }
#endif
}

void cmd31_list_func(command_struct *cmd, int index, HWND hWnd)
{
#ifdef USE_GUI
   char text[128];

   sprintf_s(text, sizeof(text), "%d", cmd->arg[0]);
   ListView_AddTextItem(hWnd, IDC_LVEVENTLIST, index, 2, text);
#endif
}

void print_hex2str(unsigned char* arg, int arg_length){
   for(int i=0; i<arg_length; i++) {
      printf("%02X",(int)arg[i]);
   }
   printf("\n");
}
int DoCommandParse(int cmd, char *name, unsigned char *buf, int size, BOOL count_events, void (*list_func)(command_struct *command, int index, HWND), command_struct *command)
{
   if (!count_events)
   {
      command->cmd = cmd;
      command->offset = (int)(buf-event_data)-1;
      command->name = name;
      command->arg_length = size;
      memcpy(command->arg, buf, command->arg_length);
      command->list_func = list_func;
   }
   #ifdef DEBUG_MODE
      printf("\n--- command parsed:\n");
      printf("name: %s\n", name );
      printf("value: ");
      print_hex2str(command->arg, command->arg_length);
      printf("offset: %X\n", (int)(buf-event_data)-1 );
   #endif
   return size;
}

int EVNParse(unsigned char *evn_buf, int index, BOOL count_events, command_struct *command)
{
   unsigned char cmd = evn_buf[index];
   index++;

   switch(cmd)
   {
      case 0x00: // Jump command
      {
         int offset=DoubleWordSwap(*((unsigned long *)(evn_buf+index)));

         index += DoCommandParse(cmd, "Jump", evn_buf+index, 4, count_events, jump_list_func, command);
         if (offset == 0)
            return -1;

         break;
      }
      case 0x01: // Start Loop command
         index += DoCommandParse(cmd, "StartLoop", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x02: // Another Loop command
         index += DoCommandParse(cmd, "StartLoop2", evn_buf+index, 9, count_events, null_list_func, command);
         break;
      case 0x03: // End Loop command
         index += DoCommandParse(cmd, "End Loop", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x04: // Do Dialogue
         index += DoCommandParse(cmd, "Do Dialogue", evn_buf+index, 1, count_events, dodialog_list_func, command);
         break;
      case 0x05: // Conditional Branch
         index += DoCommandParse(cmd, "Conditional Branch", evn_buf+index, 7, count_events, condbra_list_func, command);
         break;
      case 0x06: // Stub
         index += DoCommandParse(cmd, "Null1", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x07: // Stub
         index += DoCommandParse(cmd, "Null2", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x08: // ??
         index += DoCommandParse(cmd, "CMD08", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x09: // ??
         index += DoCommandParse(cmd, "CMD09", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x0A: // ??
         index += DoCommandParse(cmd, "CMD0A", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x0B: // ??
         index += DoCommandParse(cmd, "CMD0B", evn_buf+index, 2, count_events,null_list_func,  command);
         break;
      case 0x0C: // ??
         index += DoCommandParse(cmd, "CMD0C", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x0D: // ??
         index += DoCommandParse(cmd, "CMD0D", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x0E: // ??
         index += DoCommandParse(cmd, "CMD0E", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x0F: // ??
         index += DoCommandParse(cmd, "CMD0F", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x10: // ??
         index += DoCommandParse(cmd, "CMD10", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x11: // ??
         index += DoCommandParse(cmd, "CMD11", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x12: // ??
         index += DoCommandParse(cmd, "CMD12", evn_buf+index, 4, count_events, null_list_func, command);
         break;
      case 0x13: // Stub
         index += DoCommandParse(cmd, "Null3", evn_buf+index, 1, count_events, null_list_func, command);
         break;
      case 0x14: // ??
         index += DoCommandParse(cmd, "CMD14", evn_buf+index, 1, count_events, null_list_func, command);
         break;
      case 0x15: // Fade in. arg: mask?(byte), rate of transition(byte). If rate of transition is greater than 5, it doesn't fade in at all.
         index += DoCommandParse(cmd, "Fade In", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x16: // ??
         index += DoCommandParse(cmd, "CMD16", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x17: // ??
         index += DoCommandParse(cmd, "CMD17", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x18: // ??
         index += DoCommandParse(cmd, "CMD18", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x19: // ??
         index += DoCommandParse(cmd, "CMD19", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x1A: // ??
         index += DoCommandParse(cmd, "CMD1A", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x1B: // ???
         index += DoCommandParse(cmd, "CMD1B", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x1C: // ???
         index += DoCommandParse(cmd, "CMD1C", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x1D: // ???
         index += DoCommandParse(cmd, "CMD1D", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x1E: // ??
         index += DoCommandParse(cmd, "CMD1E", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x1F: // Stub
         index += DoCommandParse(cmd, "Null4", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x20: // ??
         index += DoCommandParse(cmd, "CMD20", evn_buf+index, 0x22, count_events, null_list_func, command);
         break;
      case 0x21: // ??
         index += DoCommandParse(cmd, "CMD21", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x22: // ??
         index += DoCommandParse(cmd, "CMD22", evn_buf+index, 13, count_events, null_list_func, command);
         break;
      case 0x23: // Scroll Screen. arg: number of x increments(word), increment size(word). First arg should always be larger
         index += DoCommandParse(cmd, "Scroll Screen", evn_buf+index, 4, count_events, null_list_func, command);
         break;
      case 0x24: // ??
         index += DoCommandParse(cmd, "CMD24", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x25: // ??
         index += DoCommandParse(cmd, "CMD25", evn_buf+index, 4, count_events, null_list_func, command);
         break;
      case 0x26: // ??
         index += DoCommandParse(cmd, "CMD26", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x27: // ??
         index += DoCommandParse(cmd, "CMD27", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x28: // ??
         index += DoCommandParse(cmd, "CMD28", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x29: // ??
         index += DoCommandParse(cmd, "CMD29", evn_buf+index, 3, count_events, null_list_func, command);
         break;
      case 0x2A: // ??
         index += DoCommandParse(cmd, "CMD2A", evn_buf+index, 3, count_events, null_list_func, command);
         break;
      case 0x2B: // ??
         index += DoCommandParse(cmd, "CMD2B", evn_buf+index, 9, count_events, null_list_func, command);
         break;
      case 0x2C: // Stub
         index += DoCommandParse(cmd, "Null5", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x2D: // Stub
         index += DoCommandParse(cmd, "Null6", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x2E: // ??
         index += DoCommandParse(cmd, "CMD2E", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x2F: // Draw Sprite. arg: arg1(word), orientation(byte), xpos(word), ypos(byte)
         index += DoCommandParse(cmd, "Draw Sprite", evn_buf+index, 6, count_events, null_list_func, command);
         break;
      case 0x30: // ??
         index += DoCommandParse(cmd, "CMD30", evn_buf+index, 7, count_events, null_list_func, command);
         break;
      case 0x31: // ??
      {
         int arg = evn_buf[index];

         index += DoCommandParse(cmd, "CMD31", evn_buf+index, 1, count_events, cmd31_list_func, command);
         switch (arg)
         {
         case 0:
            command->arg_length += 1;
            memcpy(command->arg+1, evn_buf+index, 1);
            index += 1;
            break;
         case 3:
            command->arg_length += 8;
            memcpy(command->arg+1, evn_buf+index, 8);
            index += 8;
            break;
         case 4:
            command->arg_length += 7;
            memcpy(command->arg+1, evn_buf+index, 7);
            index += 7;
            break;
         case 9:
         case 11:
            // no additional args
            break;
         case 10:
         case 12:
            // no additional args
            break;

         case 13:
         case 14:
            command->arg_length += 2;
            memcpy(command->arg+1, evn_buf+index, 2);
            index += 2;
            break;
         case 15:
            command->arg_length += 6;
            memcpy(command->arg+1, evn_buf+index, 6);
            index += 6;
            break;
         case 18:
            command->arg_length += 6;
            memcpy(command->arg+1, evn_buf+index, 6);
            index += 6;
            break;
         default: break;
         }
         break;
      }
      case 0x32: // ??
         index += DoCommandParse(cmd, "CMD32", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x33: // ??
         index += DoCommandParse(cmd, "CMD33", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x34: // ??
         index += DoCommandParse(cmd, "CMD34", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x35: // ??
         index += DoCommandParse(cmd, "CMD35", evn_buf+index, 1, count_events, null_list_func, command);
         break;
      case 0x36: // ???
         index += DoCommandParse(cmd, "CMD36", evn_buf+index, 1, count_events, null_list_func, command);
         break;
      case 0x37: // ???
         index += DoCommandParse(cmd, "CMD37", evn_buf+index, 1, count_events, null_list_func, command);
         break;
      case 0x38: // ???
         index += DoCommandParse(cmd, "CMD38", evn_buf+index, 1, count_events, null_list_func, command);
         break;
      case 0x39: // ???
         index += DoCommandParse(cmd, "CMD39", evn_buf+index, 1, count_events, null_list_func, command);
         break;
      case 0x3A: // ???
         index += DoCommandParse(cmd, "CMD3A", evn_buf+index, 1, count_events, null_list_func, command);
         break;
      case 0x3B: // ???
         index += DoCommandParse(cmd, "CMD3B", evn_buf+index, 1, count_events, null_list_func, command);
         break;
      case 0x3C: // ??
         index += DoCommandParse(cmd, "CMD3C", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x3D: // ??
         index += DoCommandParse(cmd, "CMD3D", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x3E: // ??
         index += DoCommandParse(cmd, "CMD3E", evn_buf+index, 14, count_events, null_list_func, command);
         break;
      case 0x3F: // ??
         index += DoCommandParse(cmd, "CMD3F", evn_buf+index, 6, count_events, null_list_func, command);
         break;
      case 0x40: // Character Walk
         index += DoCommandParse(cmd, "Character Walk", evn_buf+index, 9, count_events, null_list_func, command);
         if (!(special_flags & 0x400))
         {
            memcpy(&command->arg[command->arg_length], evn_buf+index, 1);
            command->arg_length++;
            index++;
         }
         break;
      case 0x41: // ???
         index += DoCommandParse(cmd, "CMD41", evn_buf+index, 1, count_events, null_list_func, command);
         break;
      case 0x42: // ??
         index += DoCommandParse(cmd, "CMD42", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x43: // ???
         index += DoCommandParse(cmd, "CMD43", evn_buf+index, 1, count_events, null_list_func, command);
         break;
      case 0x44: // Character Animate
         index += DoCommandParse(cmd, "Character Animate", evn_buf+index, 4, count_events, null_list_func, command);
         break;
      case 0x45: // ???
         index += DoCommandParse(cmd, "CMD45", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x46: // ???
      {
         int length = 8 + 2;

         if (evn_buf[index+7] != 0)
            length += 4;
         else
            length += 4;

         index += DoCommandParse(cmd, "CMD46", evn_buf+index, length, count_events, null_list_func, command);
         break;
      }
      case 0x47: // ???
         index += DoCommandParse(cmd, "CMD47", evn_buf+index, 7, count_events, null_list_func, command);
         break;
      case 0x48: // ???
         index += DoCommandParse(cmd, "CMD48", evn_buf+index, 9, count_events, null_list_func, command);
         break;
      case 0x49: // ???
         index += DoCommandParse(cmd, "CMD49", evn_buf+index, 11, count_events, null_list_func, command);
         break;
      case 0x4A: // ???
         index += DoCommandParse(cmd, "CMD4A", evn_buf+index, 6, count_events, null_list_func, command);
         break;
      case 0x4B: // ???
         index += DoCommandParse(cmd, "CMD4B", evn_buf+index, 7, count_events, null_list_func, command);
         break;
      case 0x4C: // ???
         index += DoCommandParse(cmd, "CMD4C", evn_buf+index, 8, count_events, null_list_func, command);
         break;
      case 0x4D: // Change Sprite Priority
         index += DoCommandParse(cmd, "Change Sprite Priority", evn_buf+index, 5, count_events, null_list_func, command);
         break;
      case 0x4E: // ???
         index += DoCommandParse(cmd, "CMD4E", evn_buf+index, 9, count_events, null_list_func, command);
         break;
      case 0x4F: // ???
         index += DoCommandParse(cmd, "CMD4F", evn_buf+index, 10, count_events, null_list_func, command);
         break;
      case 0x50: // ??
         index += DoCommandParse(cmd, "CMD50", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x51: // ??
         index += DoCommandParse(cmd, "CMD51", evn_buf+index, 3, count_events, null_list_func, command);
         break;
      case 0x52: // ??
         index += DoCommandParse(cmd, "CMD52", evn_buf+index, 6, count_events, null_list_func, command);
         break;
      case 0x53: // ??
         index += DoCommandParse(cmd, "CMD53", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x54: // ??
         index += DoCommandParse(cmd, "CMD54", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x55: // ??
         index += DoCommandParse(cmd, "CMD55", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x56: // ??
         index += DoCommandParse(cmd, "CMD56", evn_buf+index, 4, count_events, null_list_func, command);
         break;
      case 0x57: // ??
         index += DoCommandParse(cmd, "CMD57", evn_buf+index, 5, count_events, null_list_func, command);
         break;
      case 0x58: // ??
         index += DoCommandParse(cmd, "CMD58", evn_buf+index, 1, count_events, null_list_func, command);
         break;
      case 0x59: // ??
         index += DoCommandParse(cmd, "CMD59", evn_buf+index, 3, count_events, null_list_func, command);
         break;
      case 0x5A: // ??
         index += DoCommandParse(cmd, "CMD5A", evn_buf+index, 3, count_events, null_list_func, command);
         break;
      case 0x5B: // ??
         index += DoCommandParse(cmd, "CMD5B", evn_buf+index, 9, count_events, null_list_func, command);
         break;
      case 0x5C: // ??
         index += DoCommandParse(cmd, "CMD5C", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x5D: // ??
         index += DoCommandParse(cmd, "CMD5D", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x5E: // ??
         index += DoCommandParse(cmd, "CMD5E", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x5F: // ??
         index += DoCommandParse(cmd, "CMD5F", evn_buf+index, 7, count_events, null_list_func, command);
         break;
      case 0x60: // ??
         index += DoCommandParse(cmd, "CMD60", evn_buf+index, 4, count_events, null_list_func, command);
         break;
      case 0x61: // ??
         index += DoCommandParse(cmd, "CMD61", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x62: // ??
         index += DoCommandParse(cmd, "CMD62", evn_buf+index, 1, count_events, null_list_func, command);
         break;
      case 0x63: // ??
         index += DoCommandParse(cmd, "CMD63", evn_buf+index, 4, count_events, null_list_func, command);
         break;
      case 0x64: // ??
         index += DoCommandParse(cmd, "CMD64", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x65: // ??
         index += DoCommandParse(cmd, "CMD65", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x66: // ??
         index += DoCommandParse(cmd, "CMD66", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x67: // ??
         index += DoCommandParse(cmd, "CMD67", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x68: // ??
         index += DoCommandParse(cmd, "CMD68", evn_buf+index, 1, count_events, null_list_func, command);
         break;
      case 0x69: // ??
         index += DoCommandParse(cmd, "CMD69", evn_buf+index, 3, count_events, null_list_func, command);
         break;
      case 0x6A: // ??
         index += DoCommandParse(cmd, "CMD6A", evn_buf+index, 3, count_events, null_list_func, command);
         break;
      case 0x6B: // ??
         index += DoCommandParse(cmd, "CMD6B", evn_buf+index, 3, count_events, null_list_func, command);
         break;
      case 0x6C: // ??
         index += DoCommandParse(cmd, "CMD6C", evn_buf+index, 4, count_events, null_list_func, command);
         break;
      case 0x6D: // ??
         index += DoCommandParse(cmd, "CMD6D", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x6E: // ??
         index += DoCommandParse(cmd, "CMD6E", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x6F: // ??
         index += DoCommandParse(cmd, "CMD6F", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x70: // ??
         index += DoCommandParse(cmd, "CMD70", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x71: // ??
         index += DoCommandParse(cmd, "CMD71", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x72: // Stub
         index += DoCommandParse(cmd, "Null7", evn_buf+index, 1, count_events, null_list_func, command);
         break;
      case 0x73: // ??
         index += DoCommandParse(cmd, "CMD73", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x74: // ??
         index += DoCommandParse(cmd, "CMD74", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x75: // ??
         index += DoCommandParse(cmd, "CMD75", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x76: // ??
         index += DoCommandParse(cmd, "CMD76", evn_buf+index, 6, count_events, null_list_func, command);
         break;
      case 0x77: // ??
         index += DoCommandParse(cmd, "CMD77", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x78: // ??
         index += DoCommandParse(cmd, "CMD78", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x79: // ??
         index += DoCommandParse(cmd, "CMD79", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x7A: // ??
         index += DoCommandParse(cmd, "CMD7A", evn_buf+index, 4, count_events, null_list_func, command);
         break;
      case 0x7B: // ??
         index += DoCommandParse(cmd, "CMD7B", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x7C: // ??
         index += DoCommandParse(cmd, "CMD7C", evn_buf+index, 5, count_events, null_list_func, command);
         break;
      case 0x7D: // ??
         index += DoCommandParse(cmd, "CMD7D", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x7E: // ??
         index += DoCommandParse(cmd, "CMD7E", evn_buf+index, 1, count_events, null_list_func, command);
         break;
      case 0x7F: // ??
         index += DoCommandParse(cmd, "CMD7F", evn_buf+index, 1, count_events, null_list_func, command);
         break;
      case 0x80: // ??
         index += DoCommandParse(cmd, "CMD80", evn_buf+index, 2, count_events, null_list_func, command);
         break;
      case 0x81: // ??
         index += DoCommandParse(cmd, "CMD81", evn_buf+index, 3, count_events, null_list_func, command);
         break;
      case 0x82: // ??
         index += DoCommandParse(cmd, "CMD82", evn_buf+index, 1, count_events, null_list_func, command);
         break;
      case 0x83: // ??
         index += DoCommandParse(cmd, "CMD83", evn_buf+index, 1, count_events, null_list_func, command);
         break;
      case 0x84: // ??
         index += DoCommandParse(cmd, "CMD84", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x85: // ??
         index += DoCommandParse(cmd, "CMD85", evn_buf+index, 1, count_events, null_list_func, command);
         break;
      case 0x86: // ??
         index += DoCommandParse(cmd, "CMD86", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x87: // ??
         index += DoCommandParse(cmd, "CMD87", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      case 0x88: // ??
         index += DoCommandParse(cmd, "CMD88", evn_buf+index, 0, count_events, null_list_func, command);
         break;
      /*
      case 0xA8: // ??  found in 015_00_1  https://github.com/eadmaster/pcrown/issues/30
         index += DoCommandParse(cmd, "CMDA8", evn_buf+index, 2, count_events, null_list_func, command);         
         // TESTED len: 6, 0, 1, 2, 3, 4, 9 (Character Walk), 8 
         break;
      */
      
      default:
         {
            printf("Unknown command %02X at offset %08X\n", cmd, index);
            /*
            char tempstr[512];
            if (count_events)
            {
               sprintf_s(tempstr, sizeof(tempstr), "Unknown command %02X at offset %08X", cmd, index);
               MessageBoxA(NULL, tempstr, "Notice",
                  MB_OK | MB_ICONINFORMATION);
            }*/
            return -2;

         }
   }

   return index;
}

int EVNLoadFile(const char *filename)
{
   FILE *fp;
   unsigned long f_size;
   unsigned long cmdoffset=0;
   int done = 0;
   unsigned char *buf;
   unsigned long i;
   int index;
   int max_command=0;
   int *list=NULL, max_list=0, list_size=0;

   if (file_loaded)
      EVNFreeFile();      

   cmd43_var = 0xFF;
   special_flags=0;

   if (fopen_s(&fp, filename, "rb") != 0)
      return -1;

   f_size = _filelength(_fileno(fp));

   if ((event_data = (unsigned char *)malloc(f_size)) == NULL)
   {
      fclose(fp);
      return -3;
   }

   // Read event data in
   fread((void *)event_data, 1, f_size, fp);
   fclose(fp);

   EVNParseHeader(event_data+0x1000, &header);
   buf = event_data;

   index = 0;
   while (!done)
   {
      command_struct cmd;

      if (event_data[index] == 0x43 && header.offsets1[event_data[index+1]] != 0)
      {
         ReallocMem((void **)&list, sizeof(int), &max_list, list_size);
         list[list_size] = event_data[index+1];
         //printf("list_size++: list[%d]\n", list_size, list[list_size]);
         list_size++;
      }

      index = EVNParse(event_data, index, FALSE, &cmd);
      if (index < 0)
      {
         if (index != -2)
         {
            cmd.cmd43_var = cmd43_var;
            ReallocMem((void **)&command, sizeof(command_struct), &max_command, num_commands);
            memcpy(&command[num_commands], &cmd, sizeof(command_struct));
            num_commands++;
         }

         /*
         printf("list_size = %d\n", list_size);

         if (list_size > 1)
         {
            printf("Error unsupported list size > 1\n");
            fclose(fp);
            if (list)
               free(list);
            EVNFreeFile();      
            return -4;
         }
         */
         for (i = 0; i < (unsigned long)list_size; i++)
         {
            unsigned short unknown;
            cmd43_var = list[i];
            special_flags |= 0x400;
            index = 0x1000+header.offsets1[list[i]]; 
            unknown = WordSwap(*((unsigned short *)(event_data+index)));
            for (;;)
            {
               int old_index=index;

               index += 2;
               index = EVNParse(event_data, index, FALSE, &cmd);
               cmd.time_diff = (event_data[old_index] << 8) | event_data[old_index+1];
               if (index < 0)
               {
                  if (index != -2)
                  {
                     cmd.cmd43_var = cmd43_var;
                     ReallocMem((void **)&command, sizeof(command_struct), &max_command, num_commands);
                     memcpy(&command[num_commands], &cmd, sizeof(command_struct));
                     num_commands++;
                  }

                  break;
               }
               else
               {
                  cmd.cmd43_var = cmd43_var;
                  ReallocMem((void **)&command, sizeof(command_struct), &max_command, num_commands);
                  memcpy(&command[num_commands], &cmd, sizeof(command_struct));
                  num_commands++;
               }

               if (i != list_size-1 && (index+2) >= 0x1000+header.offsets1[list[i+1]])
                  break;
            }
         }
         done = 1;
      }
      else
      {
         cmd.cmd43_var = cmd43_var;
         ReallocMem((void **)&command, sizeof(command_struct), &max_command, num_commands);
         memcpy(&command[num_commands], &cmd, sizeof(command_struct));
         num_commands++;
      }
   }


   // I shouldn't have to this, but I screwed up with my original dumps
   for (i = 0; i < num_commands-1; i++)
   {
      // Check for any overlapping commands
      if (command[i].offset+command[i].arg_length >= command[i+1].offset)
      {
         command[i].arg_length = 0;
         //printf("command overlap: %d. offset = %08X, length = %d", i, command[i].offset, command[i].length);
      }
   }

   //cmd43_var = 0xFF;
   file_loaded = true;
   fclose(fp);
   if (list)
      free(list);

   return 0;
}

unsigned long EVNGetCompressionNum(int index)
{
	unsigned long start, end, i;
	if (index == -1)
	{
		if (header.cmp_data_offset != 0)
		{
			start = header.cmp_tbl_offset;
			end = header.cmp_data_offset;
		}
		else
			return 0;
	}
	else
	{
		if (header.offsets2[index] == 0 && index != 0)
			return 0;
		end = header.offsets2[index+1];
		if (end == 0 || index == 7)
		{
			end = header.cmp_data_offset;
			if (end == 0)
				end = header.offsets3[0];
		}
		start = header.offsets2[index];
	}

	for (i = start+2; i < end; i+=2)
	{
		if (WordSwap(*((unsigned short *)(event_data+0x1000+i))) == 0)
		{
			end = i;
			break;
		}
	}

	return ((end-start)/2)-1;
}

unsigned long EVNGetCompressionOffsetAlt(int i)
{
	return header.offsets3[i];
}

void EVNBuildHeader(unsigned char *data, evn_header_struct *header)
{
   int i;
   for (i = 1; i < 8; i++)
   {
      *((unsigned short*)data) = WordSwap(header->offsets1[i]);
      data+=2;
   }

   *((unsigned short*)data) = WordSwap(header->cmp_tbl_offset);
   data += 2;

   for (i = 0; i < 8; i++)
   {
      *((unsigned short*)data) = WordSwap(header->offsets2[i]);
      data+=2;
   }

   *((unsigned short*)data) =  WordSwap(header->cmp_data_offset);
   data += 2;

   for (i = 0; i < 8; i++)
   {
      *((unsigned short*)data) = WordSwap(header->offsets3[i]);
      data+=2;
   }
}

void CompressCheckWriteMagicNumber(unsigned char *enc_count, unsigned char *magic_number, unsigned char **outbuf, int *out_size)
{
   if (enc_count[0] == 4)
   {
      #ifdef DEBUG_MODE
         printf("%02X", (unsigned char)magic_number[0]);
      #endif
      
      outbuf[0][0] = magic_number[0];
      outbuf[0]++;
      out_size[0]++;
      enc_count[0] = 0;
      magic_number[0] = 0;
   }
}

void CompressAddWord(unsigned short word, unsigned char **outbuf, int *out_size, unsigned char *magic_number, unsigned char *enc_count)
{
   #ifdef DEBUG_MODE
      printf("%02X", (unsigned char)word);
   #endif
      
   outbuf[0][0] = (unsigned char)word;  // truncate upper byte
   outbuf[0]++;
   out_size[0]++;
   magic_number[0] <<= 2;
   magic_number[0] |= (word >> 8) & 0x3;
   enc_count[0]++;

   CompressCheckWriteMagicNumber(enc_count, magic_number, outbuf, out_size);
}

// CompressAddCC(0x03ED, text, &i, 4, &outbuf, out_size, &magic_number, &enc_count);
BOOL CompressAddCC(unsigned short word, char *text, size_t *i, int num_words, unsigned char **outbuf, int *out_size, unsigned char *magic_number, unsigned char *enc_count)
{
   int j;
   char *p;

   if (word != 0)
      CompressAddWord(word, outbuf, out_size, magic_number, enc_count);

   if (num_words > 0)
   {
      p = strchr(text+i[0], '=');
      if (p == NULL)
         return FALSE;
      else
         i[0] = (int)(p-text)+1;
   }

   for (j = 0; j < num_words; j++)
   {
      unsigned short data=0;
      int count=0;

      // Get 4 hex characters
      while (count < 4)
      {
         if (text[i[0]] >= 'A' && text[i[0]] <= 'F')
         {
             data <<= 4;
             data |= text[i[0]] - 'A' + 10;
             count++;
         }
         else if (text[i[0]] >= '0' || text[i[0]] <= '9')
         {
            data <<= 4;
            data |= text[i[0]] - '0';
            count++;
         }
         else if (text[i[0]] == '>')
            // error
            return FALSE;
         i[0]++;
      }

      //if (word == 0)
      //   data += 0xFE00;

      CompressAddWord(data, outbuf, out_size, magic_number, enc_count);
   }
   
   p = strchr(text+i[0], '>');
   if (p == NULL)
      return FALSE;
   else
      i[0] = (int)(p-text)+1;

   return TRUE;
}

BOOL CheckStringDupe(int index, char * str, int cur_cmd43_var)
{
   int i,k;
   char *text;

   for (k = 0; k < (int)index; k++)
   {
      if (command[k].cmd != 0x04 || command[k].cmd43_var != cur_cmd43_var)
         continue;

      for (i = 0; i < maintranstext.num; i++)
      {
         if (maintranstext.ttentry[i].event_id == k)
         {
            if (maintranstext.ttentry[i].trans_text != NULL)
               text = maintranstext.ttentry[i].trans_text;
            else
               text = maintranstext.ttentry[i].orig_text;
            break;
         }
      }

      if (strcmp(text, str) == 0 && k != index)
         return TRUE;
   }

   return FALSE;
}

int Align16Byte(int size)
{
   if (size % 16 != 0)
      return size+(16-(size % 16));
   else
      return size;
}

int CopyText(char *text, int *i, int num_words, char **outbuf, int *out_size, int *out_index)
{
   char *p;
   int j;
   int old_i = i[0];

   if (num_words > 0)
   {
      p = strchr(text+i[0], '=');
      if (p == NULL)
         return FALSE;
      else
         i[0] = (int)(p-text)+1;
   }

   for (j = 0; j < num_words; j++)
   {
      unsigned short data=0;
      int count=0;

      // Get 4 hex characters
      while (count < 4)
      {
         if (text[i[0]] >= 'A' && text[i[0]] <= 'F')
         {
            data <<= 4;
            data |= text[i[0]] - 'A' + 10;
            count++;
         }
         else if (text[i[0]] >= '0' || text[i[0]] <= '9')
         {
            data <<= 4;
            data |= text[i[0]] - '0';
            count++;
         }
         else if (text[i[0]] == '>')
            // error
            return FALSE;
         i[0]++;
      }

      //if (word == 0)
      //   data += 0xFE00;

   }

   p = strchr(text+i[0], '>');
   if (p == NULL)
      return FALSE;
   else
      i[0] = (int)(p-text)+1;

   ReallocMem((void **)outbuf, 1, out_size, out_index[0]+i[0]-old_i);
   strncpy_s(outbuf[0]+out_index[0], out_size[0]-out_index[0], text+old_i, i[0]-old_i);
   out_index[0]+=i[0]-old_i;

   return TRUE;
}

void TagInject(char **outbuf, int *out_size, int *out_index, char *tag)
{
   int tag_size = (int)strlen(tag);
   ReallocMem((void **)outbuf, 1, out_size, out_index[0]+tag_size);
   strncpy_s(outbuf[0]+out_index[0], out_size[0]-out_index[0], tag, tag_size);
   out_index[0]+=tag_size;
}

int GetTextWidth(char *text, int size)
{
   int width=0;

   for (int i = 0; i < size; i++)
      width += font_data[text[i]];
   return width;
}

int EVNLoadNamesItemsText(const char *filename, ttentry_struct **ttentry, unsigned long *tbl_size)
{
   FILE *fp=NULL;
   int max_text=0;

   tbl_size[0] = 0;

   if (fopen_s(&fp, filename, "rt") != 0)
      return ERROR_FILE_NOT_FOUND;

   for (;;)
   {
      char text[1024];

      if (fgets ( text, sizeof(text), fp ) == NULL && feof(fp))
         break;

      char *p = strpbrk(text, "\r\n");

      if (p != NULL)
         p[0] = '\0';

      ReallocMem((void **)ttentry, sizeof(ttentry_struct), &max_text, tbl_size[0]);
      ttentry[0][tbl_size[0]].trans_text = _strdup(text);
      tbl_size[0]++;
   }

   fclose(fp);
   return ERROR_SUCCESS;
}

char *FixLineWrap(char *text, int event_id)
{
   char *ret=NULL;
   int char_x=0;
   int line_index=0;
   int i;
   int out_size=0, out_index=0;
   bool last_text_pause=false;
   bool last_text_ascii=false;
   bool parse_string=true;

   for (i = 0; i < (int)strlen(text);)
   {
      if (text[i] == '<')
      {
         char text2[512];

         sscanf_s(text+i+1, " %[^=>] ", text2, sizeof(text2));

         // Convert control code
         if (strcmp(text2, "string") == 0)
         {
            if (parse_string)
            {
               sscanf_s(text+i+1+1+strlen(text2), " %[^>] ", text2, sizeof(text2));
               unsigned long index = strtol(text2, NULL, 16);

               // Only do the following calculation if <CC03FC=0004> is not used
               // Calculate line width from name
               if (index < num_names)
               {
                  char *p=strchr(names_data[index].trans_text, '<');
                  int len;
                  if (p)
                     len = (int)(p-names_data[index].trans_text);
                  else
                     len = (int)strlen(names_data[index].trans_text);

                  char_x += GetTextWidth(names_data[index].trans_text, len);
               }
               else
               {
                  // Not sure how to handle these
               }
            }

            CopyText(text, &i, 1, &ret, &out_size, &out_index);
         }
         else if (strcmp(text2, "lineend") == 0)
         {
            //if (line_index == 3)
            //{
            //   // Skip <lineend> and insert <pause>
            //   TagInject(&ret, &out_size, &out_index, "<pause>");
            //   line_index = 0;
            //   i += (int)strlen(text2);
            //}
            //else
            CopyText(text, &i, 0, &ret, &out_size, &out_index);

            char_x = 0;
            line_index++;

            if (line_index == 3 || last_text_pause == true)
               line_index = 0;
            last_text_pause = false;
         }
         else if (strcmp(text2, "pause") == 0)
         {
            CopyText(text, &i, 0, &ret, &out_size, &out_index);
            line_index = 0;
            last_text_pause = true;
         }
         else if (strcmp(text2, "CC03EA") == 0)
         {
            if (last_text_ascii)
            {
               printf("Warning: found <CC03EA> tag with text before and no <lineend>/<pause> tags for event id %d\n", event_id);
               TagInject(&ret, &out_size, &out_index, "<lineend>");
               TagInject(&ret, &out_size, &out_index, "<pause>");
            }
            CopyText(text, &i, 0, &ret, &out_size, &out_index);
         }
         else if (strcmp(text2, "color") == 0)
            CopyText(text, &i, 1, &ret, &out_size, &out_index);
         else if (strcmp(text2, "CC03EC") == 0)
            CopyText(text, &i, 0, &ret, &out_size, &out_index);
         else if (strcmp(text2, "CC03ED") == 0)
            CopyText(text, &i, 4, &ret, &out_size, &out_index);
         else if (strcmp(text2, "CC03EF") == 0)
            CopyText(text, &i, 1, &ret, &out_size, &out_index);
         else if (strcmp(text2, "CC03F0") == 0)
            CopyText(text, &i, 7, &ret, &out_size, &out_index);
         else if (strcmp(text2, "CC03F1") == 0)
            CopyText(text, &i, 1, &ret, &out_size, &out_index);
         else if (strcmp(text2, "CC03F2") == 0)
            CopyText(text, &i, 7, &ret, &out_size, &out_index);
         else if (strcmp(text2, "CC03F3") == 0)
            CopyText(text, &i, 1, &ret, &out_size, &out_index);
         else if (strcmp(text2, "CC03F4") == 0)
            CopyText(text, &i, 0, &ret, &out_size, &out_index);
         else if (strcmp(text2, "choose") == 0)
            CopyText(text, &i, 0, &ret, &out_size, &out_index);
         else if (strcmp(text2, "endchoose") == 0)
            CopyText(text, &i, 0, &ret, &out_size, &out_index);
         else if (strcmp(text2, "CC03F7") == 0)
            CopyText(text, &i, 0, &ret, &out_size, &out_index);
         else if (strcmp(text2, "CC03F8") == 0)
            CopyText(text, &i, 1, &ret, &out_size, &out_index);
         else if (strcmp(text2, "CC03F9") == 0)
            CopyText(text, &i, 1, &ret, &out_size, &out_index);
         else if (strcmp(text2, "CC03FA") == 0)
            CopyText(text, &i, 9, &ret, &out_size, &out_index);
         else if (strcmp(text2, "CC03FB") == 0)
            CopyText(text, &i, 1, &ret, &out_size, &out_index);
         else if (strcmp(text2, "CC03FC") == 0)
         {
            sscanf_s(text+i+1+1+strlen(text2), " %[^>] ", text2, sizeof(text2));
            int value = strtol(text2, NULL, 16);
            if (value == 4)
               parse_string=false;
            CopyText(text, &i, 1, &ret, &out_size, &out_index);
         }
         else
         {
            printf("Error: Invalid tag %s for event id %d\n", text2, event_id);
            return NULL;
         }
         last_text_ascii = false;
      }
      // Is ascii text?
      else if ((unsigned char)text[i] <= 0x7F)
      {
         // Keep going until I hit a control code, etc.
         int j = 0;

         // Find end of word
         for (;;)
         {
            if (isspace(text[i+j]) ||
                text[i+j] == '<' || text[i+j] > 0x7F || 
                strlen(text+i+j) == 0)
            {
               // Are we over the end of line?
               if (char_x+GetTextWidth(text+i, j) > MAX_LINE_PIXEL_WIDTH)
               {
                  bool newpage=false;
                  line_index++;

                  // Is there still more space in the window?
                  if (line_index >= 3)
                  {
                     // Inject pause
                     TagInject(&ret, &out_size, &out_index, "<pause>");
                     line_index = 0;
                     newpage=true;
                  }

                  // Inject line end
                  TagInject(&ret, &out_size, &out_index, "<lineend>");
                  char_x=0;

                  if (newpage)
                     TagInject(&ret, &out_size, &out_index, "<CC03EA>");                  
               }

               if (isspace(text[i+j]))
                  j++;

               // Add word
               ReallocMem((void **)&ret, 1, &out_size, out_index+j);
               strncpy_s(ret + out_index, out_size-out_index, text+i, j);
               char_x += GetTextWidth(text+i, j);
               i += j;
               out_index += j;
               j = 0;

               if (text[i+j] == '<' || text[i+j] > 0x7F || strlen(text+i+j) == 0)
               {
                  last_text_ascii = true;
                  break;
               }
            }
            else
               j++;
         }
      }
      // Shift-JIS, convert 4 characters to 
      else
      {
         ReallocMem((void **)&ret, 1, &out_size, out_index+2);
         ret[out_index] = text[i];
         ret[out_index+1] = text[i+1];
         i+=2;
         out_index+=2;
      }
   }

   ReallocMem((void **)&ret, 1, &out_size, out_index+1);
   ret[out_index] = '\0';
   return ret;
}


unsigned short AsciiCharToTexttblIndex(char c) {
   if ( c  == ' ') return(0);
   else if ( c  == '*') return(3);
   else if ( c  == '?') return(4);
   else if ( c  == '!') return(5);
   else if ( c  == '_') return(6);
   else if ( c  == '#') return(7);
   else if ( c  == '+') return(8);
   else if ( c  == '~') return(9);
   else if ( c  == '(') return(10);
   else if ( c  == ')') return(11);
   else if ( c  == '[') return(12);
   else if ( c  == ']') return(13);
   else if ( c  == '-') return(14);
   else if ( c  == ',') return(0x3DA);
   else if ( c  == '.') return(0x3DB);
   else if ( c  == '\"') return(0x3DC);
   else if ( c  == '\'') return(0x3DD);
   else if ( c  == ':') return(0x3DE);
   else if ( c  == '&') return(0x3DF);
   else if ( c  == '%') return(0x0F);
   else if ( c  == '0') return(0x11);
   else if ( c  == '1') return(0x12);
   else if ( c  == '2') return(0x13);
   else if ( c  == '3') return(0x14);
   else if ( c  == '4') return(0x15);
   else if ( c  == '5') return(0x16);
   else if ( c  == '6') return(0x17);
   else if ( c  == '7') return(0x18);
   else if ( c  == '8') return(0x19);
   else if ( c  == '9') return(0x1A);
   else if ( c  >= 'A' && c <= 'Z') return(0x3C0 + c - 'A');
   else if ( c  >= 'a' && c <= 'z') return(0x3E0 + c - 'a');
   else return(0xFFF);  // invalid
}



unsigned short *CompressText(int cur_cmd43_var, unsigned char *outbuf, int *out_size, evn_header_struct *header, int *text_num)
{
   unsigned char magic_number=0;
   size_t i;
   int j;
   unsigned long k;
   int l;
   unsigned char enc_count=0;
   int last_cmd43_var=-1;
   unsigned short *text_pointer_list=NULL;
   int max_text_size=0;
   unsigned char *cmpbuf=outbuf;
   int num_text=0;

   out_size[0] = 0;
   text_num[0] = 0;

	num_text = EVNGetCompressionNum(cur_cmd43_var);
   
   if (num_text == 0)
      return NULL;

   // Make sure we're 16-byte aligned   
   text_num[0] = Align16Byte(num_text * 2) / 2;
   if ((text_pointer_list = (unsigned short *)calloc(sizeof(unsigned short), text_num[0])) == NULL)
      return NULL;
   for (l = 0; l < num_text; l++)
   {
		char *text=NULL;
		int event_id=-1;
      for (k = 0; k < maintranstext.num; k++)
      {
			if (((maintranstext.ttentry[k].event_id >> 16) & 0xFFFF) == cur_cmd43_var+1 &&
				 (maintranstext.ttentry[k].event_id & 0xFFFF) == l)
			{				
				event_id = maintranstext.ttentry[k].event_id;
				text = maintranstext.ttentry[k].orig_text;
				break;
			}
		}

		if (text == NULL)
		{
			printf("Event ID %d not present in script\n", l);
			free (text_pointer_list);
			return NULL;
		
		}

/*
		text = FixLineWrap(text, event_id);  // 2FIX: crash with ascii text
		if (text == NULL)
		{
			free (text_pointer_list);
			return NULL;
		}
  */
      
      #ifdef DEBUG_MODE
         printf("\nevent_id: %d\n", event_id );
         printf("text_pointer_list index: %d\n", l );
         printf("text: %s\n", text );
         printf("compressed: ");
         //printf("\nnum_text: %d\n", num_text );
      #endif
      
		// Create new entry for pointer
		text_pointer_list[l] = out_size[0];
		for (i = 0; i < strlen(text);)
		{
			// Is it a control code?
			if (text[i] == '<')
			{
				char text2[512];

				sscanf_s(text+i+1, " %[^=>] ", text2, sizeof(text2));

				// Add 0x3FF to buffer
				CompressAddWord(0x03FF, &outbuf, out_size, &magic_number, &enc_count);

				// Convert control code
				if (strcmp(text2, "lineend") == 0)
					CompressAddCC(0x03E8, text, &i, 0, &outbuf, out_size, &magic_number, &enc_count);
				else if (strcmp(text2, "pause") == 0)
					CompressAddCC(0x03E9, text, &i, 0, &outbuf, out_size, &magic_number, &enc_count);
				else if (strcmp(text2, "CC03EA") == 0)
					CompressAddCC(0x03EA, text, &i, 0, &outbuf, out_size, &magic_number, &enc_count);
				else if (strcmp(text2, "color") == 0)
					CompressAddCC(0x03EB, text, &i, 1, &outbuf, out_size, &magic_number, &enc_count);
				else if (strcmp(text2, "CC03EC") == 0)
					CompressAddCC(0x03EC, text, &i, 0, &outbuf, out_size, &magic_number, &enc_count);
				else if (strcmp(text2, "CC03ED") == 0)
					CompressAddCC(0x03ED, text, &i, 4, &outbuf, out_size, &magic_number, &enc_count);
				else if (strcmp(text2, "CC03EF") == 0)
					CompressAddCC(0x03EF, text, &i, 1, &outbuf, out_size, &magic_number, &enc_count);
				else if (strcmp(text2, "CC03F0") == 0)
					CompressAddCC(0x03F0, text, &i, 7, &outbuf, out_size, &magic_number, &enc_count);
				else if (strcmp(text2, "CC03F1") == 0)
					CompressAddCC(0x03F1, text, &i, 1, &outbuf, out_size, &magic_number, &enc_count);
				else if (strcmp(text2, "CC03F2") == 0)
					CompressAddCC(0x03F2, text, &i, 7, &outbuf, out_size, &magic_number, &enc_count);
				else if (strcmp(text2, "CC03F3") == 0)
					CompressAddCC(0x03F3, text, &i, 1, &outbuf, out_size, &magic_number, &enc_count);
				else if (strcmp(text2, "CC03F4") == 0)
					CompressAddCC(0x03F4, text, &i, 0, &outbuf, out_size, &magic_number, &enc_count);
				else if (strcmp(text2, "choose") == 0)
					CompressAddCC(0x03F5, text, &i, 0, &outbuf, out_size, &magic_number, &enc_count);
				else if (strcmp(text2, "endchoose") == 0)
					CompressAddCC(0x03F6, text, &i, 0, &outbuf, out_size, &magic_number, &enc_count);
				else if (strcmp(text2, "CC03F7") == 0)
					CompressAddCC(0x03F7, text, &i, 0, &outbuf, out_size, &magic_number, &enc_count);
				else if (strcmp(text2, "CC03F8") == 0)
					CompressAddCC(0x03F8, text, &i, 1, &outbuf, out_size, &magic_number, &enc_count);
				else if (strcmp(text2, "CC03F9") == 0)
					CompressAddCC(0x03F9, text, &i, 1, &outbuf, out_size, &magic_number, &enc_count);
				else if (strcmp(text2, "CC03FA") == 0)
					CompressAddCC(0x03FA, text, &i, 9, &outbuf, out_size, &magic_number, &enc_count);
				else if (strcmp(text2, "CC03FB") == 0)
					CompressAddCC(0x03FB, text, &i, 1, &outbuf, out_size, &magic_number, &enc_count);
				else if (strcmp(text2, "CC03FC") == 0)
					CompressAddCC(0x03FC, text, &i, 1, &outbuf, out_size, &magic_number, &enc_count);
				else if (strcmp(text2, "string") == 0)
					CompressAddCC(0, text, &i, 1, &outbuf, out_size, &magic_number, &enc_count);
			}
			// Is ascii text?
			else if ((unsigned char)text[i] <= 0x7F)
			{
				BOOL pattern_found = FALSE;
				// See if it's a dictionary pattern
				for (j = 0; j < num_patterns; j++)
				{
					if (strcmp(text+i, pattern_list[j]) == 0)
					{
						CompressAddWord(0x80+j, &outbuf, out_size, &magic_number, &enc_count);
						i += strlen(pattern_list[j]);
						pattern_found = TRUE;
					}
				}
            
				if (!pattern_found)
				{
					// Just add as is
					//CompressAddWord(text[i], &outbuf, out_size, &magic_number, &enc_count);
					//i++;
               // convert to 16-bit char
                              
               // Find Character and encode
               unsigned short k = AsciiCharToTexttblIndex(text[i]);
               if (k != 0xFFF)
                  CompressAddWord(k, &outbuf, out_size, &magic_number, &enc_count);
               else 
                  printf("invalid char found (skipped): %c\n", text[i]);
               
               i++;
				}
			}
			// Shift-JIS, convert 4 characters to 
			else
			{
				// Find Character and encode
				for (j = 0; j < (sizeof(texttbl) / sizeof(unsigned short)); j++)
				{
               if(j>0xEA0 && j<0x35EE) {
                  // reserved for items translations storage
                  continue;
               }
					else if ((((unsigned char)text[i] << 8) | (unsigned char)text[i+1]) == texttbl[j])
					{
						CompressAddWord(j, &outbuf, out_size, &magic_number, &enc_count);
                  // i+= 2;
						break;
					}
				}
            i+= 2;  // skip char if not found
			}
		}

		if (text)
			free(text);

		if (l == num_text-1)
		{
			while (out_size[0] % 5 != 0)
				CompressAddWord(0, &outbuf, out_size, &magic_number, &enc_count);

			// increase text_num to compensate
			int old_text_num=text_num[0], max_size=text_num[0];
			num_text++;
			text_num[0] = Align16Byte(num_text * 2) / 2;
			ReallocMem((void **)&text_pointer_list, sizeof(unsigned short), &max_size, text_num[0]-1);
			if (text_num[0] > old_text_num)
				memset(&text_pointer_list[old_text_num], 0, (text_num[0]-old_text_num)*sizeof(unsigned short));
			text_pointer_list[l+1] = out_size[0];
			break;
		}
   }

   // Go through offsets and fill in any null offsets
   for (i = text_num[0]-2; i > 0; i--)
   {
      if (text_pointer_list[i] == 0)
      {
         // Only copy offset there's a previous offset that wasn't 0
         for (j = (int)i-1; j > 0; j--)
         {
            if (text_pointer_list[j] != 0)
            {
               text_pointer_list[i] = text_pointer_list[i+1];
               break;
            }
         }
      }
   }

	// Ensure compressed data is 16-byte aligned
   out_size[0] = Align16Byte(out_size[0]);

   return text_pointer_list;
}

// MEMO: used by itemsutil
unsigned short *CompressTextAlt(unsigned char *outbuf, int *out_size, int max_out_size, ttentry_struct *ttentry, int text_num, int lzo_compress)
{
   unsigned char magic_number=0;
   size_t i;
   int j;
   int l;
   unsigned char enc_count=0;
   int last_cmd43_var=-1;
   unsigned short *text_pointer_list=NULL;
   int max_text_size=0;
   unsigned char *cmpbuf=outbuf;

   out_size[0] = 0;

   if ((text_pointer_list = (unsigned short *)calloc(sizeof(unsigned short), text_num+1)) == NULL)
      return NULL;

   for (l = 0; l < text_num; l++)
   {
      //for (k = 0; k < ?; k++)
      {
         char *text=NULL;

         if (out_size[0] >= max_out_size)
         {
            // 2FIX: ERROR_INSUFFICIENT_BUFFER with items.txt
            free(text_pointer_list);
            printf("%d>=%d\n", out_size[0], max_out_size);
            if (ttentry[l].trans_text != NULL)
               puts(ttentry[l].trans_text);
            return NULL;
         }

         // Set translated text 
         if (ttentry[l].trans_text != NULL)
            text = ttentry[l].trans_text;
         else
            text = ttentry[l].orig_text;

         // Create new entry for pointer
         text_pointer_list[l] = out_size[0];
         for (i = 0; i < strlen(text);)
         {
            if (lzo_compress)
            {
            }
            else
            {
               // Is it a control code?
               if (text[i] == '<')
               {
                  char text2[512];

                  sscanf_s(text+i+1, " %[^=>] ", text2, sizeof(text2));

                  // Add 0x3FF to buffer
                  CompressAddWord(0x03FF, &outbuf, out_size, &magic_number, &enc_count);

                  // Convert control code
                  if (strcmp(text2, "lineend") == 0)
                     CompressAddCC(0x03E8, text, &i, 0, &outbuf, out_size, &magic_number, &enc_count);
                  else if (strcmp(text2, "pause") == 0)
                     CompressAddCC(0x03E9, text, &i, 0, &outbuf, out_size, &magic_number, &enc_count);
                  else if (strcmp(text2, "CC03EA") == 0)
                     CompressAddCC(0x03EA, text, &i, 0, &outbuf, out_size, &magic_number, &enc_count);
                  else if (strcmp(text2, "color") == 0)
                     CompressAddCC(0x03EB, text, &i, 1, &outbuf, out_size, &magic_number, &enc_count);
                  else if (strcmp(text2, "CC03EC") == 0)
                     CompressAddCC(0x03EC, text, &i, 0, &outbuf, out_size, &magic_number, &enc_count);
                  else if (strcmp(text2, "CC03ED") == 0)
                     CompressAddCC(0x03ED, text, &i, 4, &outbuf, out_size, &magic_number, &enc_count);
                  else if (strcmp(text2, "CC03EF") == 0)
                     CompressAddCC(0x03EF, text, &i, 1, &outbuf, out_size, &magic_number, &enc_count);
                  else if (strcmp(text2, "CC03F0") == 0)
                     CompressAddCC(0x03F0, text, &i, 7, &outbuf, out_size, &magic_number, &enc_count);
                  else if (strcmp(text2, "CC03F1") == 0)
                     CompressAddCC(0x03F1, text, &i, 1, &outbuf, out_size, &magic_number, &enc_count);
                  else if (strcmp(text2, "CC03F2") == 0)
                     CompressAddCC(0x03F2, text, &i, 7, &outbuf, out_size, &magic_number, &enc_count);
                  else if (strcmp(text2, "CC03F3") == 0)
                     CompressAddCC(0x03F3, text, &i, 1, &outbuf, out_size, &magic_number, &enc_count);
                  else if (strcmp(text2, "CC03F4") == 0)
                     CompressAddCC(0x03F4, text, &i, 0, &outbuf, out_size, &magic_number, &enc_count);
                  else if (strcmp(text2, "choose") == 0)
                     CompressAddCC(0x03F5, text, &i, 0, &outbuf, out_size, &magic_number, &enc_count);
                  else if (strcmp(text2, "endchoose") == 0)
                     CompressAddCC(0x03F6, text, &i, 0, &outbuf, out_size, &magic_number, &enc_count);
                  else if (strcmp(text2, "CC03F7") == 0)
                     CompressAddCC(0x03F7, text, &i, 0, &outbuf, out_size, &magic_number, &enc_count);
                  else if (strcmp(text2, "CC03F8") == 0)
                     CompressAddCC(0x03F8, text, &i, 1, &outbuf, out_size, &magic_number, &enc_count);
                  else if (strcmp(text2, "CC03F9") == 0)
                     CompressAddCC(0x03F9, text, &i, 1, &outbuf, out_size, &magic_number, &enc_count);
                  else if (strcmp(text2, "CC03FA") == 0)
                     CompressAddCC(0x03FA, text, &i, 9, &outbuf, out_size, &magic_number, &enc_count);
                  else if (strcmp(text2, "CC03FB") == 0)
                     CompressAddCC(0x03FB, text, &i, 1, &outbuf, out_size, &magic_number, &enc_count);
                  else if (strcmp(text2, "CC03FC") == 0)
                     CompressAddCC(0x03FC, text, &i, 1, &outbuf, out_size, &magic_number, &enc_count);
                  else if (strcmp(text2, "string") == 0)
                     CompressAddCC(0, text, &i, 1, &outbuf, out_size, &magic_number, &enc_count);
                  else
                  {
                     char *p = strchr(text+i, '>');
                     size_t len=(size_t)(p-text)+i+1;
                     char tag[512];
                     strncpy_s(tag, sizeof(tag), text+i, len);
                     printf("Error, unknown tag %s at line %d\n", tag, i+1);
                     i+=len; 
                  }
               }
               // Is ascii text?
               else if ((unsigned char)text[i] <= 0x7F)
               {
                  BOOL pattern_found = FALSE;
                  // See if it's a dictionary pattern
                  for (j = 0; j < num_patterns; j++)
                  {
                     if (strcmp(text+i, pattern_list[j]) == 0)
                     {
                        CompressAddWord(0x80+j, &outbuf, out_size, &magic_number, &enc_count);
                        i += strlen(pattern_list[j]);
                        pattern_found = TRUE;
                     }
                  }

                  if (!pattern_found)
                  {
                     // Just add as is
                     //CompressAddWord(text[i], &outbuf, out_size, &magic_number, &enc_count);
                     
                     unsigned short k = AsciiCharToTexttblIndex(text[i]);
                     if (k != 0xFFF)
                        CompressAddWord(k, &outbuf, out_size, &magic_number, &enc_count);
                     else 
                        printf("invalid char found (skipped): %c\n", text[i]);
                        
                     i++;
                  }
               }
               // Shift-JIS, convert 4 characters to 
               else
               {
                  // Find Character and encode
                  for (j = 0; j < (sizeof(texttbl) / sizeof(unsigned short)); j++)
                  {
                     if ((((unsigned char)text[i] << 8) | (unsigned char)text[i+1]) == texttbl[j])
                     {
                        CompressAddWord(j, &outbuf, out_size, &magic_number, &enc_count);
                        break;
                     }
                  }
                  i+= 2;
               }
            }
         }

         if (text)
            free(text);
      }
   }

   text_pointer_list[text_num] = out_size[0];

   return text_pointer_list;
}

int EVNSaveFile(const char *filename)
{
   FILE *fp;
   unsigned char header_data[0x40];
   unsigned char evn_data[0xFFFF];
   unsigned char *cmp_text[9];
   unsigned long i,j;
   unsigned short size;
   int total_size;
   int cmp_text_size[9];
   unsigned short *text_pointer_list[9];
   unsigned short tbl[8];
   int text_num[9];

   if (!file_loaded)
      return -1;

   if (fopen_s(&fp, filename, "wb") != 0)
      return -2;

   memset(evn_data, 0, sizeof(evn_data));

   // Rebuild the compressed text
   cmp_text[0] = (unsigned char *)calloc(1, 0xFFFF);
   text_pointer_list[0] = CompressText(-1, cmp_text[0], cmp_text_size, &header, text_num);

   for (i = 0; i < 8; i++)
   {
      cmp_text[i+1] = (unsigned char *)calloc(1, 0xFFFF);
      text_pointer_list[i+1] = CompressText(i, cmp_text[i+1], &cmp_text_size[i+1], &header, &text_num[i+1]);
   }

   // Now it's time to rebuild 0xFF event data
   for (i = 0; i < num_commands; i++)
   {
      if (command[i].cmd43_var == 0xFF)
      {
         evn_data[command[i].offset] = command[i].cmd;
         memcpy(evn_data+command[i].offset+1, command[i].arg, command[i].arg_length);
      }
      else
         break;
   }

   int offset=0x1040;

   int last_offset;
   unsigned long count=0;

   // Rebuild the rest of the data
   for (j = 0; j < 7; j++)
   {
      int base_offset=-1;
      bool tbl_used=false;

      last_offset = 0;
      for (i = 0; i < num_commands; i++)
      {
         if (command[i].cmd43_var == j)
         {
            if (base_offset == -1)
               base_offset = command[i].offset-2; 

            int k=offset+command[i].offset-base_offset-2;

            evn_data[k] = (command[i].time_diff >> 8) & 0xFF;
            evn_data[k+1] = command[i].time_diff & 0xFF;
            evn_data[k+2] = command[i].cmd;
            memcpy(evn_data+k+3, command[i].arg, command[i].arg_length);
            last_offset = k+3+command[i].arg_length;
            tbl_used=true;
         }
      }

      // need to change this so it doesn't write last entry
      if (tbl_used)
      {
         offset = Align16Byte(last_offset);
         tbl[count] = offset-0x1000;
         count++;
      }
   }

   header.offsets1[0] = offset-0x1000;

   if (count)
   {
      for (i = 0; i < count-1; i++)
         header.offsets1[i+1] = tbl[i];
   }

   // Inject 0xFF tbl
   if (text_pointer_list[0])
   {
      header.cmp_tbl_offset = offset-0x1000;

      // Copy over compressed data offsets to buffer
      for (i = 0; i < (unsigned long)text_num[0]; i++)
         text_pointer_list[0][i] = WordSwap(text_pointer_list[0][i]);
      memcpy((void *)(evn_data+offset), text_pointer_list[0], sizeof(unsigned short)*text_num[0]);
      offset += sizeof(unsigned short)*text_num[0];
   }
   else if (text_pointer_list[1])
      // Fill first Table offset
      header.cmp_tbl_offset = offset-0x1000;

   for (j = 0; j < 8; j++)
   {
      if (text_pointer_list[j+1])
      {
         header.offsets2[j] = offset-0x1000;

         // Copy over compressed data offsets to buffer
         for (i = 0; i < (unsigned long)text_num[j+1]; i++)
            text_pointer_list[j+1][i] = WordSwap(text_pointer_list[j+1][i]);
         memcpy((void *)(evn_data+offset), text_pointer_list[j+1], sizeof(unsigned short)*text_num[j+1]);
         offset += sizeof(unsigned short)*text_num[j+1];
      }
      else
         break;
   }

   // Inject 0xFF compressed text
   if (text_pointer_list[0])
   {
      header.cmp_data_offset = offset-0x1000;
      // Copy over compressed data to buffer   
      memcpy((void *)(evn_data+offset), cmp_text[0], cmp_text_size[0]);
      offset += cmp_text_size[0];
   }

   for (j = 0; j < 8; j++)
   {
      if (text_pointer_list[j+1])
      {
         //if (j != 0)
         header.offsets3[j] = offset-0x1000;

         // Copy over compressed data to buffer   
         memcpy((void *)(evn_data+offset), cmp_text[j+1], cmp_text_size[j+1]);
         offset += cmp_text_size[j+1];
      }
      else
         break;
   }
   
   // Generate header
   memset(header_data, 0, sizeof(header_data));

   // Copy over size of offsets + buffer here
   size = WordSwap(offset-0x1000);

   // offset 0x1000 is used only when there's event data after header
   // offset 0x100E is offset from 0x1000 where compressed data's offset table starts
   // offset 0x1032 is size of data after 0x1000
   memcpy((void *)(header_data+0x32), &size, sizeof(size));

   EVNBuildHeader(header_data, &header);

   // Copy header over to buffer
   memcpy((void *)(evn_data+0x1000), header_data, sizeof(header_data));

   total_size = offset;
   fwrite((void *)evn_data, 1, total_size, fp);
   fclose(fp);
   return 0;
}

void DecodeWord(dcmp_text_struct *dcmp_info, unsigned long *counter, unsigned long size, unsigned short *textword)
{
   if (dcmp_info->decode_counter > 3)
   {
      dcmp_info->textbuffer++;
      dcmp_info->decode_counter = 0;
      // increment some kind of counter here
      counter[0]++;
   }

   if (1)
   {
      if (dcmp_info->decode_counter == 0)
      {
         // Make sure we're not actually at the end of the text area.
         //if ((counter[0]+4) >= size)
         //   return -1;

         dcmp_info->magic_number = dcmp_info->textbuffer[4];
      }

      //loc_6072146
      *textword = (((dcmp_info->magic_number >> ((3 - dcmp_info->decode_counter) * 2)) & 0x3) << 8) | dcmp_info->textbuffer[0];

      dcmp_info->decode_counter++;
      counter[0]++;
      dcmp_info->textbuffer++;
   }
   else
   {
   }

   //   printf("counter = %08X, decode_counter = %d, textbuffer = %08X, buffer = %08X. difference = %08X\n", *counter, decode_counter, textbuffer, buffer, textbuffer - buffer);

   if (counter[0] >= size ||
       (size % 5 == 0 && counter[0]+1 >= size))
      dcmp_info->finished = TRUE;
}

int CCHandle(dcmp_text_struct *dcmp_info, unsigned long *counter, unsigned long end, unsigned short textword, char *name, BOOL include_format, int num_words, char *out, size_t out_size)
{
   char work_text[512];
   int i;

   if (num_words == 1)
   {
      if (include_format)
      {
         if (strcmp(name, "CC") == 0)
            sprintf_s(work_text, sizeof(work_text), "<%s%04X>", name, textword);
         else
            sprintf_s(work_text, sizeof(work_text), "<%s>", name);
         strcat_s(out, out_size, work_text);
      }
   }
   else
   {
      if (include_format)
      {
         if (strcmp(name, "CC") == 0)
            sprintf_s(work_text, sizeof(work_text), "<%s%04X=", name, textword);
         else
            sprintf_s(work_text, sizeof(work_text), "<%s=", name);
         strcat_s(out, out_size, work_text);
      }

      for (i = 1; i < num_words; i++)
      {
         DecodeWord(dcmp_info, counter, end, &textword);

         if (include_format)
         {
            sprintf_s(work_text, sizeof(work_text), "%04X", textword);
            strcat_s(out, out_size, work_text);
         }
      }

      if (include_format)
         strcat_s(out, out_size, ">");
   }

   return TRUE;
}

void DecompressText(dcmp_text_struct *dcmp_info, unsigned char *data, unsigned long offset, unsigned long start, unsigned long end, BOOL include_format, char *text, size_t text_size)
{
   char work_text[512];
   unsigned long counter=start;
   unsigned short textword;

   dcmp_info->textbuffer = data+0x1000+offset+start;
   dcmp_info->finished = FALSE;

   memset(text, 0, sizeof(text_size));

   // Dump text
   for(;;)
   {
      DecodeWord(dcmp_info, &counter, end, &textword);

      if (textword == 0x3FF)
      {
         // Handle control code
         DecodeWord(dcmp_info, &counter, end, &textword);

         if (textword >= 0x03E8)
         {
            switch (textword - 0x03E8)
            {
            case 0x00: // 0x03E8(move to next line, or end of line)
               {
                  if (!CCHandle(dcmp_info, &counter, end, textword, "lineend", include_format, 1, text, text_size))
                     return;
                  break;
               }
            case 0x01: // 0x03E9(wait for keypress)
               {
                  if (!CCHandle(dcmp_info, &counter, end, textword, "pause", include_format, 1, text, text_size))
                     return;
                  break;
               }
            case 0x02: // 0x03EA(???)
               {
                  if (!CCHandle(dcmp_info, &counter, end, textword, "CC", include_format, 1, text, text_size))
                     return;
                  //assert(0);
                  break;
               }
            case 0x03: // 0x03EB(change text color)
               {
                  if (!CCHandle(dcmp_info, &counter, end, textword, "color", include_format, 2, text, text_size))
                     return;
                  break;
               }
            case 0x04: // 0x03EC(???)
               {
                  if (!CCHandle(dcmp_info, &counter, end, textword, "CC", include_format, 1, text, text_size))
                     return;
                  break;
               }
            case 0x05: // 0x03ED(???)
               {
                  if (!CCHandle(dcmp_info, &counter, end, textword, "CC", include_format, 5, text, text_size))
                     return;
                  break;
               }
            case 0x06: // 0x03EE(Stub)
               {
                  assert(0);
                  break;
               }
            case 0x07:
               {
                  if (!CCHandle(dcmp_info, &counter, end, textword, "CC", include_format, 2, text, text_size))
                     return;
                  break;
               }
            case 0x08:
               {
                  if (!CCHandle(dcmp_info, &counter, end, textword, "CC", include_format, 8, text, text_size))
                     return;
                  break;
               }
            case 0x09:
               {
                  if (!CCHandle(dcmp_info, &counter, end, textword, "CC", include_format, 2, text, text_size))
                     return;
                  break;
               }
            case 0x0A:
               {
                  if (!CCHandle(dcmp_info, &counter, end, textword, "CC", include_format, 8, text, text_size))
                     return;
                  break;
               }
            case 0x0B: // 0x03F3
               {
                  if (!CCHandle(dcmp_info, &counter, end, textword, "CC", include_format, 2, text, text_size))
                     return;
                  break;
               }
            case 0x0C: // 0x03F4
               {
                  if (!CCHandle(dcmp_info, &counter, end, textword, "CC", include_format, 1, text, text_size))
                     return;
                  break;
               }
            case 0x0D: // 0x03F5(choose)
               {
                  if (!CCHandle(dcmp_info, &counter, end, textword, "choose", include_format, 1, text, text_size))
                     return;
                  break;
               }
            case 0x0E: // 0x03F6(endchoose)
               {
                  if (!CCHandle(dcmp_info, &counter, end, textword, "endchoose", include_format, 1, text, text_size))
                     return;
                  break;
               }
            case 0x0F: // 0x03F7
               {
                  if (!CCHandle(dcmp_info, &counter, end, textword, "CC", include_format, 1, text, text_size))
                     return;
                  break;
               }
            case 0x10: // 0x03F8
               {
                  if (!CCHandle(dcmp_info, &counter, end, textword, "CC", include_format, 2, text, text_size))
                     return;
                  break;
               }
            case 0x11: // 0x03F9
               {
                  if (!CCHandle(dcmp_info, &counter, end, textword, "CC", include_format, 2, text, text_size))
                     return;
                  break;
               }
            case 0x12: // 0x03FA
               {
                  if (!CCHandle(dcmp_info, &counter, end, textword, "CC", include_format, 10, text, text_size))
                     return;
                  break;
               }
            case 0x13: // 0x03FB
               {
                  if (!CCHandle(dcmp_info, &counter, end, textword, "CC", include_format, 2, text, text_size))
                     return;
                  break;
               }
            case 0x14: // 0x03FC
               {
                  if (!CCHandle(dcmp_info, &counter, end, textword, "CC", include_format, 2, text, text_size))
                     return;
                  break;
               }
            default:
               if (!CCHandle(dcmp_info, &counter, end, textword, "CC", include_format, 1, text, text_size))
                  return;
               break;
            }
         }
         else
         {
            // Handle the 0x0000-0x3E7 control codes
            if (include_format)
            {
               sprintf_s(work_text, sizeof(work_text), "<string=%04X>", textword);
               strcat_s(text, text_size, work_text);
            }
         }
      }
      else
      {
         if (texttbl[textword] != 0)
            sprintf_s(work_text, sizeof(work_text), "%c%c", texttbl[textword] >> 8, texttbl[textword] & 0xFF);
         else
            sprintf_s(work_text, sizeof(work_text), "<%04X>", textword);
         strcat_s(text, text_size, work_text);
      }
      if (dcmp_info->finished)
         break;
   }
}

unsigned short FetchTextOffset(int index)
{
   unsigned short offset;
   unsigned short *ptr;

   if (cmd43_var == 0xFF)
   {
      offset = header.cmp_tbl_offset;
   }
   else
   {
      offset = header.offsets2[cmd43_var];
   }

   ptr = (unsigned short *)(event_data+0x1000+offset);
   return WordSwap(ptr[index]);
}

void CalcOffsets(command_struct *command, unsigned short *stuff, unsigned short *start, unsigned short *end)
{
   if (cmd43_var == 0xFF)
      stuff[0] = header.cmp_data_offset;
   else
      stuff[0] = header.offsets3[cmd43_var];

   // (0x1000 + 0x40), arg = 0x2, result = 00000070
   start[0]=FetchTextOffset(command->arg[0]);
   // (0x1000 + 0x40), arg = 0x3, result = 000000A2
   end[0]=FetchTextOffset(command->arg[0]+1)-1;
}

void GenerateDialogText(command_struct *command, BOOL include_format, void *text, size_t text_size, BOOL unicode_convert)
{
   unsigned short offset, start, end;
   char tempstr[8192 * 2];
   unsigned long long result;
   dcmp_text_struct dcmp_info;
   unsigned short var1;

   cmd43_var = command->cmd43_var;
   CalcOffsets(command, &offset, &start, &end);
   if (end == 0xFFFF)
   {
      printf("Something weird happened with offsets fetching\n");
      return;
   }
   result = (unsigned long long)start * 0x00000000CCCCCCCD;
   result = (result >> 34) * 5;
   var1 = (unsigned short)result;
   dcmp_info.decode_counter = start-var1;
   //var3 = start;

   //if (???)
      dcmp_info.magic_number = event_data[0x1000+offset+var1+4];
   //else
   //   dcmp_info.magic_number = unk_60B1524[var1];

   DecompressText(&dcmp_info, event_data, offset, start, end, include_format, tempstr, sizeof(tempstr));

   if (unicode_convert)
      MultiByteToWideChar(/* CP_ACP */ 932, MB_COMPOSITE, tempstr, -1, (LPWSTR)text, (int)text_size);
   else
      strcpy_s((char *)text, text_size, tempstr);
}

void DumpBin(unsigned long tbl_addr, int tbl_size, unsigned long cmp_addr)
{

   unsigned short start, end;
   char tempstr[8192 * 2];
   dcmp_text_struct dcmp_info;
   FILE *fp;
   unsigned char *buf;

   if (fopen_s(&fp, "files\\0.bin", "rb") != 0)
      return;

   int size = _filelength(_fileno(fp));
   if ((buf = (unsigned char *)malloc(size)) == NULL)
   {
      fclose(fp);
      return;
   }

   fread((void *)buf, 1, size, fp);

   int start_index = 0x62;
   int end_index = 0x63;
   start_index <<= 1;
   end_index <<= 1;
   start = (buf[tbl_addr+start_index] << 8) | buf[tbl_addr+start_index+1];
   end = (buf[tbl_addr+end_index] << 8) | buf[tbl_addr+end_index+1];
   dcmp_info.decode_counter = 0;
   dcmp_info.magic_number = buf[cmp_addr+start+4];

   char work_text[512];
   unsigned long counter=start;
   unsigned short textword;
   char *text=tempstr;
   int text_size=sizeof(tempstr);
   BOOL include_format = TRUE;
   dcmp_info.textbuffer = buf+cmp_addr+start;
   dcmp_info.finished = FALSE;

   memset(tempstr, 0, sizeof(tempstr));

   // Dump text
   for(;;)
   {
      DecodeWord(&dcmp_info, &counter, end, &textword);

      if (textword == 0x3FF)
      {
         // Handle control code
         DecodeWord(&dcmp_info, &counter, end, &textword);

         if (textword >= 0x03E8)
         {
            switch (textword - 0x03E8)
            {
            case 0x00: // 0x03E8(move to next line, or end of line)
               {
                  if (!CCHandle(&dcmp_info, &counter, end, textword, "lineend", include_format, 1, text, text_size))
                     return;
                  break;
               }
            case 0x01: // 0x03E9(wait for keypress)
               {
                  if (!CCHandle(&dcmp_info, &counter, end, textword, "pause", include_format, 1, text, text_size))
                     return;
                  break;
               }
            case 0x02: // 0x03EA(???)
               {
                  if (!CCHandle(&dcmp_info, &counter, end, textword, "CC", include_format, 1, text, text_size))
                     return;
                  //assert(0);
                  break;
               }
            case 0x03: // 0x03EB(change text color)
               {
                  if (!CCHandle(&dcmp_info, &counter, end, textword, "color", include_format, 2, text, text_size))
                     return;
                  break;
               }
            case 0x04: // 0x03EC(???)
               {
                  if (!CCHandle(&dcmp_info, &counter, end, textword, "CC", include_format, 1, text, text_size))
                     return;
                  break;
               }
            case 0x05: // 0x03ED(???)
               {
                  if (!CCHandle(&dcmp_info, &counter, end, textword, "CC", include_format, 5, text, text_size))
                     return;
                  break;
               }
            case 0x06: // 0x03EE(Stub)
               {
                  assert(0);
                  break;
               }
            case 0x07:
               {
                  if (!CCHandle(&dcmp_info, &counter, end, textword, "CC", include_format, 2, text, text_size))
                     return;
                  break;
               }
            case 0x08:
               {
                  if (!CCHandle(&dcmp_info, &counter, end, textword, "CC", include_format, 8, text, text_size))
                     return;
                  break;
               }
            case 0x09:
               {
                  if (!CCHandle(&dcmp_info, &counter, end, textword, "CC", include_format, 2, text, text_size))
                     return;
                  break;
               }
            case 0x0A:
               {
                  if (!CCHandle(&dcmp_info, &counter, end, textword, "CC", include_format, 8, text, text_size))
                     return;
                  break;
               }
            case 0x0B: // 0x03F3
               {
                  if (!CCHandle(&dcmp_info, &counter, end, textword, "CC", include_format, 2, text, text_size))
                     return;
                  break;
               }
            case 0x0C: // 0x03F4
               {
                  if (!CCHandle(&dcmp_info, &counter, end, textword, "CC", include_format, 1, text, text_size))
                     return;
                  break;
               }
            case 0x0D: // 0x03F5(choose)
               {
                  if (!CCHandle(&dcmp_info, &counter, end, textword, "choose", include_format, 1, text, text_size))
                     return;
                  break;
               }
            case 0x0E: // 0x03F6(endchoose)
               {
                  if (!CCHandle(&dcmp_info, &counter, end, textword, "endchoose", include_format, 1, text, text_size))
                     return;
                  break;
               }
            case 0x0F: // 0x03F7
               {
                  if (!CCHandle(&dcmp_info, &counter, end, textword, "CC", include_format, 1, text, text_size))
                     return;
                  break;
               }
            case 0x10: // 0x03F8
               {
                  if (!CCHandle(&dcmp_info, &counter, end, textword, "CC", include_format, 2, text, text_size))
                     return;
                  break;
               }
            case 0x11: // 0x03F9
               {
                  if (!CCHandle(&dcmp_info, &counter, end, textword, "CC", include_format, 2, text, text_size))
                     return;
                  break;
               }
            case 0x12: // 0x03FA
               {
                  if (!CCHandle(&dcmp_info, &counter, end, textword, "CC", include_format, 10, text, text_size))
                     return;
                  break;
               }
            case 0x13: // 0x03FB
               {
                  if (!CCHandle(&dcmp_info, &counter, end, textword, "CC", include_format, 2, text, text_size))
                     return;
                  break;
               }
            case 0x14: // 0x03FC
               {
                  if (!CCHandle(&dcmp_info, &counter, end, textword, "CC", include_format, 2, text, text_size))
                     return;
                  break;
               }
            default:
               if (!CCHandle(&dcmp_info, &counter, end, textword, "CC", include_format, 1, text, text_size))
                  return;
               break;
            }
         }
         else
         {
            // Handle the 0x0000-0x3E7 control codes
            if (include_format)
            {
               sprintf_s(work_text, sizeof(work_text), "<string=%04X>", textword);
               strcat_s(text, text_size, work_text);
            }
         }
      }
      else
      {
         if (texttbl[textword] != 0)
            sprintf_s(work_text, sizeof(work_text), "%c%c", texttbl[textword] >> 8, texttbl[textword] & 0xFF);
         else
            sprintf_s(work_text, sizeof(work_text), "%04X ", textword);
         strcat_s(text, text_size, work_text);
      }
      if (dcmp_info.finished)
         break;
   }

   fclose(fp);
}
