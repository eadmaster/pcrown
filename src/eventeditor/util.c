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

// All sorts of annoying functions that make my life easier
#include <windows.h>
#include <stdio.h>
#include "util.h"

//////////////////////////////////////////////////////////////////////////////

int round(float x)
{
   if (x - ((int)x) <  0.5)
   {
      return((int)x);
   }
   else
   {
      return ((int)(x+1));
   }
}

//////////////////////////////////////////////////////////////////////////////

unsigned short WordSwap(unsigned short data)
{
   return (((data & 0xFF00) >> 8) + ((data & 0x00FF) << 8));
}

//////////////////////////////////////////////////////////////////////////////

void WordSwapPtr(unsigned short *var)
{
   var[0] = WordSwap(var[0]);   
}

//////////////////////////////////////////////////////////////////////////////

unsigned long DoubleWordSwap(unsigned long data)
{
   return (((data & 0xFF000000) >> 24) +
          ((data & 0x00FF0000) >> 8) +
          ((data & 0x0000FF00) << 8) + 
          ((data & 0x000000FF) << 24));
}

//////////////////////////////////////////////////////////////////////////////

void DoubleWordSwapPtr(unsigned long *var)
{
   var[0] = DoubleWordSwap(var[0]);
}

//////////////////////////////////////////////////////////////////////////////

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

