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
#include <commctrl.h>
#include <Shlobj.h>
#include <richedit.h>
#include <stdio.h>
#include <io.h>
#include "evn.h"
#include "main.h"
#include "resource.h"
#include "trnstext.h"
#include "uihelper.h"

// Program Name/version Strings
#define PROG_NAME "Princess Crown Event Editor"
#define PROG_VERSION "v0.1"

//////////////////////////////////////////////////////////////////////////////

HINSTANCE g_hInstance;
TCHAR inifilename[MAX_PATH];

//////////////////////////////////////////////////////////////////////////////

OPENFILENAME file;
TCHAR gszOpenName[MAX_PATH];
TCHAR gszTransFile[MAX_PATH];
TCHAR gszOpenDaichiFileName[MAX_PATH];
TCHAR gszOpenFontFileName[MAX_PATH];
TCHAR gszWorkName[MAX_PATH];
TCHAR gszInitialDir[MAX_PATH];
TCHAR evnfilenames[12 * 500]=TEXT("\0");

bool sortcmd=false;
bool checkforlinespills=false;

bool tt_changes=false;
bool dupe_trans=false;

HWND hWindow;
easythread_struct *et_dump=NULL;

unsigned char *LoadPCrownFont(const char *filename)
{
   FILE *fp=NULL;
   unsigned char *buf=NULL;

   if (fopen_s(&fp, filename, "rb") == 0)
   {
      buf = (unsigned char *)malloc(_filelength(_fileno(fp)));
      if (buf)
         fread((void *)buf, 1, _filelength(_fileno(fp)), fp);

      fclose(fp);  
   }

   return buf;
}

LRESULT MainDlgInit(HWND hWnd)
{
   DWORD exstyle;

   SetWindowText(hWnd, PROG_NAME);

   exstyle = (DWORD)SendDlgItemMessage(hWnd, IDC_LVEVENTLIST, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
   SendDlgItemMessage(hWnd, IDC_LVEVENTLIST, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, exstyle | LVS_EX_FULLROWSELECT);

   ListView_InsertTextColumn(hWnd, IDC_LVEVENTLIST, 80, 0, "Event Number");
   ListView_InsertTextColumn(hWnd, IDC_LVEVENTLIST, 120, 1, "Event Name");
   ListView_InsertTextColumn(hWnd, IDC_LVEVENTLIST, 200, 2, "Other Info");
#ifdef _DEBUG
   ListView_InsertTextColumn(hWnd, IDC_LVEVENTLIST, 200, 3, "Offset");
#endif

   EnableWindow(GetDlgItem(hWnd, IDC_EDITID), FALSE);
   EnableWindow(GetDlgItem(hWnd, IDC_EDITNUMEVENTS), FALSE);
   return TRUE;
}

LRESULT MainDlgCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
   char tempstr[128];

   switch (LOWORD(wParam))
   {
   case IDM_OPENFILE:
      {
         memset(&file, 0, sizeof(OPENFILENAME));
         file.lStructSize = sizeof(OPENFILENAME);
         file.hwndOwner = hWnd;
         file.lpstrFilter = "Princess Crown Event Files\0*.EVN\0All Files\0*.*\0";
         file.lpstrFile = gszOpenName;
         file.nMaxFile = sizeof(gszOpenName);
         file.lpstrInitialDir = gszInitialDir;
         file.Flags = OFN_HIDEREADONLY;

         if (GetOpenFileNameA(&file))
         {
            char *p;
            HMENU hMenu;

            // load file
            if (EVNLoadFile(gszOpenName) != 0)
            {
               // error message
               MessageBox(hWnd, "Error loading file", "Error",
                  MB_OK | MB_ICONINFORMATION);
               return 0L;
            }

            strcpy_s(gszTransFile, sizeof(gszTransFile), gszOpenName);
            p = strrchr(gszTransFile, '.');
            if (p)
               p[0] = '\0';

            strcat_s(gszTransFile, sizeof(gszTransFile), ".TXT");

            if (TransTextLoad(gszTransFile, &maintranstext) != 0)
            {
               if (!TransConvertText(&maintranstext))
               {
                  TransTextFree(&maintranstext);
                  maintranstext.num = 0;
               }
            }

            SendDlgItemMessage(hWnd, IDC_CBEVENTLIST, CB_RESETCONTENT, 0, 0);

            // enable save file/save as file menus here
            hMenu = GetMenu(hWnd);
            EnableMenuItem(hMenu, IDM_SAVEFILE, MF_ENABLED);
            EnableMenuItem(hMenu, IDM_SAVEASFILE, MF_ENABLED);
            EnableMenuItem(hMenu, IDM_IMPORTDAICHI, MF_ENABLED);

            UpdateEvent(hWnd);

            p = strrchr(gszOpenName, '\\');
            if (p == NULL)
               p = gszOpenName;
            else
               p++;

            sprintf_s(tempstr, sizeof(tempstr), "%s - %s\0", p, PROG_NAME);
            SetWindowText(hWnd, tempstr);
         }

         break;
      }
   case IDM_SAVEFILE:
      {
         if (EVNSaveFile(gszOpenName) != 0)
         {
            // error message
            MessageBox(hWnd, "Error saving file", "Error",
               MB_OK | MB_ICONINFORMATION);
            return 0L;
         }

         break;
      }
   case IDM_SAVEASFILE:
      {
         memset(&file, 0, sizeof(OPENFILENAME));
         file.lStructSize = sizeof(OPENFILENAME);
         file.hwndOwner = hWnd;
         file.lpstrFilter = "Princess Crown Event Files\0*.EVN\0All Files\0*.*\0";
         file.lpstrFile = gszOpenName;
         file.nMaxFile = sizeof(gszOpenName);
         //               file.lpstrInitialDir = gszInitialDir;
         file.Flags = OFN_OVERWRITEPROMPT;

         if (GetSaveFileName(&file))
         {
            if (EVNSaveFile(gszOpenName) != 0)
            {
               // error message
               MessageBox(hWnd, "Error saving file", "Error",
                  MB_OK | MB_ICONINFORMATION);
               return 0L;
            }
         }

         break;
      }
   case ID_FILE_LOADSCRIPT:
      {
         memset(&file, 0, sizeof(OPENFILENAME));
         file.lStructSize = sizeof(OPENFILENAME);
         file.hwndOwner = hWnd;
         file.lpstrFilter = "Princess Crown Script Files\0*.TXT\0All Files\0*.*\0";
         file.lpstrFile = gszTransFile;
         file.nMaxFile = sizeof(gszTransFile);
         file.lpstrInitialDir = gszInitialDir;
         file.Flags = OFN_HIDEREADONLY;

         if (GetOpenFileNameA(&file))
         {
            if (TransTextLoad(gszTransFile, &maintranstext) != 0)
            {
               // error message
               MessageBox(hWnd, "Error loading file", "Error",
                  MB_OK | MB_ICONINFORMATION);
               return 0L;
            }
         }
         break;
      }
   case ID_FILE_SAVESCRIPT:
      if (!TransTextSave(gszTransFile, &maintranstext))
      {
         // error message
         MessageBox(hWnd, "Error saving file", "Error",
            MB_OK | MB_ICONINFORMATION);
         return 0L;
      }
      break;
   case ID_FILE_SAVESCRIPTAS:
      {
         memset(&file, 0, sizeof(OPENFILENAME));
         file.lStructSize = sizeof(OPENFILENAME);
         file.hwndOwner = hWnd;
         file.lpstrFilter = "Princess Crown Script Files\0*.TXT\0All Files\0*.*\0";
         file.lpstrFile = gszTransFile;
         file.nMaxFile = sizeof(gszTransFile);
         file.Flags = OFN_OVERWRITEPROMPT;

         if (GetSaveFileName(&file))
         {
            if (!TransTextSave(gszTransFile, &maintranstext))
            {
               // error message
               MessageBox(hWnd, "Error saving file", "Error",
                  MB_OK | MB_ICONINFORMATION);
               return 0L;
            }
         }
         break;
      }
   case ID_FILE_BATCHDUMPSCRIPT:
      {
         static OPENFILENAME ofn;
         static char outdir[MAX_PATH];
         static dumpthread_struct dt;

         if (et_dump)
            CloseEasyThread(et_dump);

         SetupOFN(&ofn, OFN_MULTILOAD, hWnd, TEXT("Event Files\0*.EVN\0All Files\0*.*\0\0"),
            evnfilenames, sizeof(evnfilenames));

         if (GetOpenFileName(&ofn))
         {
            BROWSEINFO bi;
            LPITEMIDLIST pidl;

            dt.ofn = &ofn;
            dt.outdir = outdir;
            
            ZeroMemory(&bi, sizeof(bi));
            bi.hwndOwner = hWnd;
            bi.pszDisplayName = outdir;
            bi.lpszTitle = "Please select ticket directory";
            bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX;
            if ((pidl = SHBrowseForFolder(&bi)))
            {
               if (SHGetPathFromIDList (pidl, outdir))
                  et_dump = CreateEasyThread((LPTHREAD_START_ROUTINE)DumpThread, &dt);

               CoTaskMemFree(pidl);
            }


         }
         break;
      }
#if 0
   case IDM_SETTINGS:
      {
         DialogBoxA(g_hInstance, MAKEINTRESOURCE(SETTINGSDLG), hWnd, (DLGPROC)SettingsDlg);
         break;
      }
#endif
   case IDM_ABOUT:
      {
         DialogBoxA(g_hInstance, MAKEINTRESOURCE(ABOUTDLG), hWnd, (DLGPROC)AboutDlg);
         break;
      }
   case IDM_EXIT:
      {
         PostMessage(hWnd, WM_CLOSE, 0, 0);
         break;
      }
   case IDC_BUTINSERTCMD:
      {
         unsigned long event_num;
         //unsigned long command_num;
         //command_struct newcommand;

         event_num = (unsigned long)SendDlgItemMessage(hWnd, IDC_CBEVENTLIST, CB_GETCURSEL, 0, 0);

         /*
         if (checkforlinespills)
         {
         // Do addition calculation
         }
         else
         {
         for (unsigned long i = 0; i < event[event_num].num_commands; i++)
         {
         if ((command_num = (unsigned long)SendDlgItemMessage(hWnd, IDC_CBEVENTLIST, LVM_GETITEMSTATE, i, LVIS_SELECTED)) == LVIS_SELECTED)
         break;

         if (i == (event[event_num].num_commands - 1))
         return 0L;
         }
         }

         // Startup dialog box to retrieve command input
         if (DialogBoxParamA(g_hInstance, MAKEINTRESOURCE(COMMANDINPUTDLG), hWnd, (DLGPROC)CommandInputDlgProc, (LPARAM)&newcommand) != TRUE)
         return 0L;

         // Add command here
         if (InsertRSAEventCommand(event_num, command_num, newcommand.name, newcommand.time1, newcommand.time2) != 0)
         {
         MessageBox(hWnd, "Ran into problems inserting command", "Error",
         MB_OK | MB_ICONINFORMATION);
         }

         UpdateEvent(hWnd);
         */
         break;
      }
   case IDC_BUTDELETECMD:
      {
         unsigned long event_num;
         //unsigned long command_num;

         if (!file_loaded)
            return 0L;

         event_num = (unsigned long)SendDlgItemMessage(hWnd, IDC_CBEVENTLIST, CB_GETCURSEL, 0, 0);

         /*
         if (checkforlinespills)
         {
         // Do addition calculation
         }
         else
         {
         for (unsigned long i = 0; i < event[event_num].num_commands; i++)
         {
         if (SendDlgItemMessage(hWnd, IDC_LVEVENTLIST, LVM_GETITEMSTATE, i, LVIS_SELECTED) == LVIS_SELECTED)
         {
         command_num = i;
         break;
         }

         if (i == (event[event_num].num_commands - 1))
         return 0L;
         }
         }

         if (DeleteRSAEventCommand(event_num, command_num) != 0)
         {
         MessageBox(hWnd, "Ran into problems deleting command", "Error",
         MB_OK | MB_ICONINFORMATION);
         }

         UpdateEvent(hWnd);

         // return to old position here
         */
         break;
      }
   case IDC_CBEVENTLIST:
      {
         switch(HIWORD(wParam))
         {
         case CBN_SELCHANGE:
            {
               UpdateEvent(hWnd);
               break;
            }
         default: break;
         }
         break;
      }
   case IDC_CBTRANSCMDS:
      {
         if (HIWORD(wParam) == BN_CLICKED)
         {
            if (SendDlgItemMessage(hWnd, IDC_CBTRANSCMDS, BM_GETCHECK, 0, 0) == BST_CHECKED)
               sortcmd = true;
            else
               sortcmd = false;                     

            UpdateEvent(hWnd);
         }
         break;
   case IDC_TRANSLATEDTEXT:
         if (HIWORD(wParam) == EN_CHANGE)
         {
            char text[1024];
            GetDlgItemText(hWnd, IDC_TRANSLATEDTEXT, text, sizeof(text));
            if (strlen(text) != 0)
               tt_changes = true;
         }
      }
   default: break;
   }

   return FALSE;
}

LRESULT MainDlgNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
   NMHDR *hdr;

   hdr = (NMHDR *)lParam;

   switch (hdr->idFrom)
   {
   case IDC_LVEVENTLIST:
      {
         switch(hdr->code)
         {
         case NM_CLICK:
            {
               int i;
               int lastmatch=-1;
               WCHAR unicode[2048];

               int maxcount = (int)SendDlgItemMessage(hWnd, IDC_LVEVENTLIST, LVM_GETITEMCOUNT, 0, 0);

               if (ListView_GetSelectedCount(GetDlgItem(hWnd, IDC_LVEVENTLIST)))
               {
                  for (i = 0; i < maxcount; i++)
                  {
                     if (ListView_GetItemState(GetDlgItem(hWnd, IDC_LVEVENTLIST), i, LVIS_SELECTED) == LVIS_SELECTED)
                        lastmatch = i;
                  }
               }

               if (lastmatch == -1)
                  break;

               if (IsDialogueCommand(&command[lastmatch]) == true)
               {
                  GenerateDialogText(&command[lastmatch], TRUE, unicode, sizeof(unicode) / sizeof(WCHAR), TRUE);

                  SetDlgItemTextW(hWnd, IDC_ORIGTEXT, unicode);

                  for (i = 0; i < maintranstext.num; i++)
                  {
                     if (lastmatch == maintranstext.ttentry[i].event_id)
                     {
                        if (maintranstext.ttentry[i].trans_text)
                           SetDlgItemText(hWnd, IDC_TRANSLATEDTEXT, maintranstext.ttentry[i].trans_text);
                        if (maintranstext.ttentry[i].notes)
                           SetDlgItemText(hWnd, IDC_NOTES, maintranstext.ttentry[i].notes);
                     }
                  }
               }

               /*
               if (checkforlinespills)
               {
               // Do addition calculation
               }
               else
               {
               // fix me
               for (unsigned long i = 0; i < event[event_num].num_commands; i++)
               {
               if ((command_num = (unsigned long)SendDlgItemMessage(hWnd, IDC_CBEVENTLIST, LVM_GETITEMSTATE, i, LVIS_SELECTED)) == LVIS_SELECTED)
               break;

               if (i == (event[event_num].num_commands - 1))
               return 0L;
               }
               }

               MessageBox(NULL, "bloop2", "Notice",
               MB_OK | MB_ICONINFORMATION);

               if (DialogBoxParamA(g_hInstance, MAKEINTRESOURCE(COMMANDINPUTDLG), hWnd, (DLGPROC)CommandInputDlgProc, (LPARAM)&event[event_num].command[command_num]) != TRUE)
               return 0L;
               */
               break;
            }
         default: break;
         }
         break;
      }
   default: break;
   }
   return FALSE;
}

LRESULT CALLBACK MainDlg(HWND hWnd, UINT uMsg, WPARAM wParam,
                            LPARAM lParam)
{
   switch (uMsg)
   {
      case WM_INITDIALOG:
         return MainDlgInit(hWnd);
      case WM_COMMAND:
         return MainDlgCommand(hWnd, wParam, lParam);
      case WM_NOTIFY:
         return MainDlgNotify(hWnd, wParam, lParam);
      case WM_CLOSE:
      {
         EndDialog(hWnd, TRUE);

         if (et_dump)
            CloseEasyThread(et_dump);
         PostQuitMessage(0);
         return TRUE;
      }
   }

   return FALSE;
}

DWORD WINAPI DumpThread( LPVOID lpParam )
{
   easythread_struct *et=(easythread_struct *)lpParam;
   dumpthread_struct *dt=(dumpthread_struct *)et->extra;
   OPENFILENAME *ofn=(OPENFILENAME *)dt->ofn;
   char *outdir=(char *)dt->outdir;
   char text[512];
   char filename[MAX_PATH];
   BOOL multifiles=FALSE;
   int done=0;
   HWND hbusy=NULL;

   if(ofn->lpstrFile[ofn->nFileOffset-1] != '\0')
   {
      // Single select
      strcpy_s(filename, sizeof(filename), ofn->lpstrFile);
      multifiles = FALSE;
   }
   else
   {
      // Multi select
      strcpy_s(filename, sizeof(filename), ofn->lpstrFile);
      strcat_s(filename, sizeof(filename), "\\");
      strcat_s(filename, sizeof(filename), ofn->lpstrFile + ofn->nFileOffset);
      ofn->nFileOffset += (WORD)strlen(ofn->lpstrFile+ofn->nFileOffset)+1;
      multifiles = TRUE;
   }

   done = 0;

   while (!done && !et->killflag)
   {
      transtext_struct maintranstext;

      if (EVNLoadFile(filename) != 0)
      {
         sprintf_s(text, sizeof(text), "Error loading %s", filename);
         MessageBox(hWindow, text, "Notice", MB_OK);
      }

      if (TransConvertText(&maintranstext))
      {
         char *p;

         // Get the base name
         p = strrchr(filename, '\\');
         if (p)
            strcpy_s(text, sizeof(text), p+1);
         else
            strcpy_s(text, sizeof(text), filename);

         // Replace extension
         p = strrchr(text, '.');
         if (p)
            p[0] = '\0';
         strcat_s(text, sizeof(text), ".TXT");

         strcpy_s(filename, sizeof(filename), outdir);
         strcat_s(filename, sizeof(filename), "\\");
         strcat_s(filename, sizeof(filename), text);

         TransTextSave(filename, &maintranstext);
      }

      TransTextFree(&maintranstext);
      maintranstext.num = 0;

      EVNFreeFile();

      if (multifiles == FALSE || ofn->lpstrFile[ofn->nFileOffset] == '\0')
         done = 1;
      else
      {
         // get next file
         strcpy_s(filename, sizeof(filename), ofn->lpstrFile);
         strcat_s(filename, sizeof(filename),"\\");
         strcat_s(filename, sizeof(filename),ofn->lpstrFile+ofn->nFileOffset);
         ofn->nFileOffset += (WORD)strlen(ofn->lpstrFile+ofn->nFileOffset)+1;
      }

      Sleep(0);
   }

   MessageBox(hWindow, "All Done", "Notice", MB_OK);
   return 0;
}


void usage()
{
#ifdef USE_GUI
   MessageBox(NULL, "Usage: eventeditor [operation] -o [output filename]", "Error", MB_OK);
#else
	printf("Usage: eventeditor [operation] -o [output filename]\n");
#endif
}

#if 0
int InsertTranslatedText(HWND hDlg, char *evn_filename, char *txt_filename, char *out_filename)
{
   if (EVNLoadFile(evn_filename) != 0)
   {
      // error message
      MessageBox(hDlg, "Error loading evn file", "Error",
         MB_OK | MB_ICONINFORMATION);
      return -1;
   }
#if 1
   if (!TransTextLoad(txt_filename, &maintranstext))
   {
      EVNFreeFile();
      // error message
      MessageBox(hDlg, "Error loading txt file", "Error",
         MB_OK | MB_ICONINFORMATION);
      return -2;
   }
#else
   if (!TransConvertText(&maintranstext))
   {
      TransTextFree(&maintranstext);
      maintranstext.num = 0;
   }
#endif
   if (EVNSaveFile(bmp_filename) != 0)
   {
      EVNFreeFile();
      TransTextFree(&maintranstext);
      // error message
      MessageBox(hDlg, "Error saving evn file", "Error",
         MB_OK | MB_ICONINFORMATION);
      return -3;
   }

   EVNFreeFile();
   TransTextFree(&maintranstext);
   return 0;
}
#endif

int InsertTranslatedText(HWND hDlg, char *evn_filename, char *txt_filename, char *out_filename)
{
	if (EVNLoadFile(evn_filename) != 0)
	{
		printf("Error loading evn file\n");
		return -1;
	}

#if 0
	if (!TransConvertText(&maintranstext))
	{
		TransTextFree(&maintranstext);
		maintranstext.num = 0;
	}
#endif

	if (TransTextLoad(txt_filename, &maintranstext) != 0)
	{
		EVNFreeFile();
		printf("Error loading txt file\n");
		return -2;
	}

	if (EVNSaveFile(out_filename) != 0)
	{
		EVNFreeFile();
		TransTextFree(&maintranstext);
		printf("Error saving evn file\n");
		return -3;
	}

	EVNFreeFile();
	TransTextFree(&maintranstext);
	return 0;
}

int DumpUnTranslatedText(char *evn_filename, char *out_filename)
{
	transtext_struct maintranstext;
	char text[MAX_PATH];

	if (EVNLoadFile(evn_filename) != 0)
	{
		printf(text, "Error loading %s", evn_filename);
		return -1;
	}

	if (TransConvertText(&maintranstext))
	{
		if (!TransTextSave(out_filename, &maintranstext))
		{
			printf("Error saving txt file\n");
			TransTextFree(&maintranstext);
			return -2;
		}
	}

	TransTextFree(&maintranstext);
	maintranstext.num = 0;

	EVNFreeFile();
	return 0;
}

#ifdef USE_GUI
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
   DWORD                       inifilenamesize=0;
   char                        *pinifilename;
   char                        tempstr[128];
   HWND hwnd;
   MSG msg;
   LPWSTR *szArgList;
   int argCount;
   bool insertText=false;
   char evn_filename[MAX_PATH], txt_filename[MAX_PATH], out_filename[MAX_PATH];

#if 0
   DumpBin(0xAF3F8, 0x108, 0xAEC50);
#endif

   szArgList = CommandLineToArgvW(GetCommandLineW(), &argCount);
   if (szArgList == NULL)
   {
      MessageBox(NULL, "Unable to parse command line", "Error", MB_OK);
      return 10;
   }


   for(int i = 1; i < argCount; i++)
   {
      if (wcscmp(szArgList[i], L"-i") == 0)
      {
         // insert translated text
         DWORD attr1, attr2;

         if (argCount-i < 2)
         {
            usage();
            return 10;
         }

         // Make sure the next two args are valid filenames
         attr1 = GetFileAttributesW(szArgList[i+1]);
         attr2 = GetFileAttributesW(szArgList[i+2]);

         if (attr1 == INVALID_FILE_ATTRIBUTES ||
             attr1 & FILE_ATTRIBUTE_DIRECTORY ||
             attr2 == INVALID_FILE_ATTRIBUTES ||
             attr2 & FILE_ATTRIBUTE_DIRECTORY)
         {
            usage();
            return 10;
         }

         WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, szArgList[i+1], -1, evn_filename, sizeof(evn_filename), NULL, NULL);
         WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, szArgList[i+2], -1, txt_filename, sizeof(txt_filename), NULL, NULL);
         i+=2;
         insertText = true;
      }
      else if (wcscmp(szArgList[i], L"-o") == 0)
      {
         if (argCount-i < 1)
         {
            usage();
            return 10;
         }

         WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, szArgList[i+1], -1, out_filename, sizeof(out_filename), NULL, NULL);
         i++;
      }
   }

   LocalFree(szArgList);

   // Load font settings
   font_data = LoadPCrownFont("files\\kanji.bin");

   if (insertText)
   {
      int ret = InsertTranslatedText(HWND_DESKTOP, evn_filename, txt_filename, out_filename);
      if (font_data)
         free(font_data);
      return ret;
   }

   InitCommonControls(); 

   LoadLibrary("riched32.dll");

   gszOpenName[0] = 0;
   gszOpenFontFileName[0] = 0;
   gszWorkName[0] = 0;
   gszInitialDir[0] = 0;
   g_hInstance = hInstance;

   // Get program pathname
   inifilenamesize = GetModuleFileName(g_hInstance, inifilename, MAX_PATH);

   // Set pointer to start of extension
   pinifilename = strrchr(inifilename, '.');

   // Replace .exe with .ini
   pinifilename[0] = '\0';
   strcat_s(inifilename, sizeof(inifilename), ".ini");

   GetPrivateProfileString("General", "FontPath", "", gszOpenFontFileName, MAX_PATH, inifilename);
   GetPrivateProfileString("General", "CheckForLineSpills", "False", tempstr, 5, inifilename);

   if (_stricmp(tempstr, "True") == 0)
      checkforlinespills = true;
   else
      checkforlinespills = false;
   AttachDebug();

   // Create a window
   hwnd = CreateDialog(hInstance, MAKEINTRESOURCE(MAINWND), GetDesktopWindow(), (DLGPROC)MainDlg);
   hWindow = hwnd;

   ShowWindow (hwnd, nCmdShow);
   UpdateWindow (hwnd);

   while (GetMessage(&msg,NULL,0,0)) 
   {
      if (!IsDialogMessage(hwnd, &msg)) 
      {
         TranslateMessage(&msg);
         DispatchMessage(&msg);
      }

   }

   EVNFreeFile();
   DetachDebug();

   if (font_data)
      free(font_data);

   return FALSE;
}
#else
int main(int argc, char *argv[])
{
	bool dumpText=false, insertText=false;
	char evn_filename[MAX_PATH], txt_filename[MAX_PATH], out_filename[MAX_PATH];

	for(int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-d") == 0)
		{
			// dump translated text
			DWORD attr;

			if (argc-i < 1)
			{
				usage();
				return 1;
			}

			// Make sure the next arg is valid filename
			attr = GetFileAttributesA(argv[i+1]);

			if (attr == INVALID_FILE_ATTRIBUTES ||
				attr & FILE_ATTRIBUTE_DIRECTORY)
			{
				usage();
				return 2;
			}

			strcpy_s(evn_filename, sizeof(evn_filename), argv[i+1]);
			i++;
			dumpText = true;
		}
		else if (strcmp(argv[i], "-i") == 0)
		{
			// insert translated text
			DWORD attr1, attr2;

			if (argc-i < 2)
			{
				usage();
				return 3;
			}

			// Make sure the next two args are valid filenames
			attr1 = GetFileAttributesA(argv[i+1]);
			attr2 = GetFileAttributesA(argv[i+2]);

			if (attr1 == INVALID_FILE_ATTRIBUTES ||
				attr1 & FILE_ATTRIBUTE_DIRECTORY ||
				attr2 == INVALID_FILE_ATTRIBUTES ||
				attr2 & FILE_ATTRIBUTE_DIRECTORY)
			{
				usage();
				return 4;
			}

			strcpy_s(evn_filename, sizeof(evn_filename), argv[i+1]);
			strcpy_s(txt_filename, sizeof(txt_filename), argv[i+2]);
			i+=2;
			insertText = true;
		}
		else if (strcmp(argv[i], "-o") == 0)
		{
			if (argc-i < 1)
			{
				usage();
				return 5;
			}

			strcpy_s(out_filename, sizeof(out_filename), argv[i+1]);
			i++;
		}
	}

	// Load font settings
	font_data = LoadPCrownFont("files\\kanji.bin");

	// Load Names file for calculating line length of speaker if necessary
	EVNLoadNamesItemsText("..\\script\\eng\\names.txt", &names_data, &num_names);

	if (dumpText)
	{
		int ret = DumpUnTranslatedText(evn_filename, out_filename);
		if (font_data)
			free(font_data);
		exit(ret);
	}

	if (insertText)
	{
		int ret = InsertTranslatedText(HWND_DESKTOP, evn_filename, txt_filename, out_filename);
		if (font_data)
			free(font_data);
		exit(ret);
	}

	if (font_data)
		free(font_data);
	exit(0);
	return 0;
}
#endif


LRESULT CALLBACK AboutDlg(HWND hDlg, UINT uMsg, WPARAM wParam,
                              LPARAM lParam)
{
   switch (uMsg)
   {
      case WM_INITDIALOG:
      {
         char text[512];
         strcpy_s(text, sizeof(text), "About " PROG_NAME);
         SetWindowText(hDlg, text);

         sprintf_s(text, sizeof(text), "%s %s", PROG_NAME, PROG_VERSION);
         SetDlgItemText(hDlg, IDC_PROGINFO, text);

         return TRUE;
      }
      case WM_COMMAND:
      {
         switch (wParam)
         {
            case IDOK:
            {
               EndDialog(hDlg, TRUE);
               return TRUE;
            }
            default: break;
         }
         break;
      }
   }

   return FALSE;
}

LRESULT CALLBACK SettingsDlg(HWND hDlg, UINT uMsg, WPARAM wParam,
                                 LPARAM lParam)
{
   switch (uMsg)
   {
      case WM_INITDIALOG:
      {
         SetDlgItemText(hDlg, IDC_EDITFONTPATH, gszOpenFontFileName);

         if (checkforlinespills)
            SendDlgItemMessage(hDlg, IDC_CBCHECKFORLINESPILLS, BM_SETCHECK, BST_CHECKED, 0);
         else
            SendDlgItemMessage(hDlg, IDC_CBCHECKFORLINESPILLS, BM_SETCHECK, BST_UNCHECKED, 0);

         return TRUE;
      }
      case WM_COMMAND:
      {
         switch (wParam)
         {
            case IDOK:
            {
               GetDlgItemText(hDlg, IDC_EDITFONTPATH, gszOpenFontFileName, sizeof(gszOpenFontFileName));

               WritePrivateProfileString("General", "FontPath", gszOpenFontFileName, inifilename);

               if (SendDlgItemMessage(hDlg, IDC_CBCHECKFORLINESPILLS, BM_GETCHECK, 0, 0) == BST_CHECKED)
               {
                  WritePrivateProfileString("General", "CheckForLineSpills", "True", inifilename);
                  checkforlinespills = true;
               }
               else
               {
                  WritePrivateProfileString("General", "CheckForLineSpills", "False", inifilename);
                  checkforlinespills = false;
               }

               EndDialog(hDlg, TRUE);
               return TRUE;
            }
            case IDCANCEL:
            {
               EndDialog(hDlg, FALSE);
               return TRUE;
            }
            case IDC_BUTBROWSE:
            {
               memset(&file, 0, sizeof(OPENFILENAME));
               file.lStructSize = sizeof(OPENFILENAME);
               file.hwndOwner = hDlg;
               file.lpstrFilter = "font files\0*.DAT\0All Files\0*.*\0";
               file.lpstrFile = gszWorkName;
               file.nMaxFile = sizeof(gszWorkName);
               file.lpstrInitialDir = gszInitialDir;
               file.Flags = OFN_HIDEREADONLY; // fix me

               if (GetOpenFileName(&file))
               {
                  SetDlgItemText(hDlg, IDC_EDITFONTPATH, gszWorkName);
               }

               break;
            }
            default: break;
         }
         break;
      }
   }

   return FALSE;
}

LRESULT CALLBACK CommandInputDlg(HWND hDlg, UINT uMsg, WPARAM wParam,
                                     LPARAM lParam)
{
   switch (uMsg)
   {
      case WM_INITDIALOG:
      {
//         command_struct *command=(command_struct *)lParam;
//         char tempstr[128];

//         SetDlgItemTextA(hDlg, IDC_EDITCMDNAME, command->name);
//         sprintf(tempstr, "%d", command->time1);
//         SetDlgItemTextA(hDlg, IDC_EDITCMDTIME1, tempstr);
//         sprintf(tempstr, "%d", command->time2);
//         SetDlgItemTextA(hDlg, IDC_EDITCMDTIME2, tempstr);

         return TRUE;
      }
      case WM_COMMAND:
      {
         switch (wParam)
         {
            case IDOK:
            {
               EndDialog(hDlg, TRUE);
               return TRUE;
            }
            case IDCANCEL:
            {
               EndDialog(hDlg, FALSE);
               return TRUE;
            }
            default: break;
         }
         break;
      }
   }

   return FALSE;
}

void UpdateEvent(HWND hWnd)
{
   char tempstr[1024];
   unsigned char cursel=0;
   LVITEM itemdata;

   // Update controls

   SendDlgItemMessage(hWnd, IDC_LVEVENTLIST, LVM_DELETEALLITEMS, 0, 0);

   itemdata.mask = LVIF_TEXT;

   // Rebuild Command List

   if (sortcmd)
   {
      int counter=0;
      for (unsigned long i = 0; i < num_commands; i++)
      {
         if (IsDialogueCommand(&command[i]) == false)
            continue;

         sprintf_s(tempstr, sizeof(tempstr), "%d", i);
         ListView_AddTextItemA(hWnd, IDC_LVEVENTLIST, counter, 0, tempstr);

         sprintf_s(tempstr, sizeof(tempstr), "%s", command[i].name);
         ListView_AddTextItemA(hWnd, IDC_LVEVENTLIST, counter, 1, tempstr);

         command[i].list_func(&command[i], counter, hWnd);
#ifdef _DEBUG
         sprintf_s(tempstr, sizeof(tempstr), "%02X", command[i].offset);
         ListView_AddTextItemA(hWnd, IDC_LVEVENTLIST, counter, 3, tempstr);
#endif
         counter++;
      }
   }
   else
   {
      for (unsigned long i = 0; i < num_commands; i++)
      {
         sprintf_s(tempstr, sizeof(tempstr), "%d", i);
         ListView_AddTextItemA(hWnd, IDC_LVEVENTLIST, i, 0, tempstr);

         sprintf_s(tempstr, sizeof(tempstr), "%s", command[i].name);
         ListView_AddTextItemA(hWnd, IDC_LVEVENTLIST, i, 1, tempstr);

         command[i].list_func(&command[i], i, hWnd);

#ifdef _DEBUG
         sprintf_s(tempstr, sizeof(tempstr), "%02X", command[i].offset);
         ListView_AddTextItemA(hWnd, IDC_LVEVENTLIST, i, 3, tempstr);
#endif
      }
   }

   SetDlgItemText(hWnd, IDC_ORIGTEXT, "");
   SetDlgItemText(hWnd, IDC_TRANSLATEDTEXT, "");
   SetDlgItemText(hWnd, IDC_NOTES, "");
}