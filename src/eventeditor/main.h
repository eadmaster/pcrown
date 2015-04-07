/*  Copyright 2005 Theo Berkau

    This file is part of Arcturus Event Editor

    Arcturus Event Editor is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Arcturus Event Editor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Arcturus Event Editor; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef main_h
#define main_h

extern bool sortcmd;
extern bool dupe_trans;

typedef struct
{
   OPENFILENAME *ofn;
   char *outdir;
} dumpthread_struct;

DWORD WINAPI DumpThread( LPVOID lpParam );

LRESULT CALLBACK AboutDlg(HWND hDlg, UINT uMsg, WPARAM wParam,
                              LPARAM lParam);

LRESULT CALLBACK SettingsDlg(HWND hDlg, UINT uMsg, WPARAM wParam,
                                 LPARAM lParam);

LRESULT CALLBACK CommandInputDlg(HWND hDlg, UINT uMsg, WPARAM wParam,
                                     LPARAM lParam);
#endif
