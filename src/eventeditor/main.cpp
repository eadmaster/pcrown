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
#include <shlobj.h>
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

void usage()
{
#ifdef USE_GUI
   MessageBox(NULL, "Usage: eventeditor [operation] -o [output filename]", "Error", MB_OK);
#else
	printf("Usage: eventeditor [operation] -o [output filename]\n");
#endif
}

int InsertTranslatedText(HWND hDlg, char *evn_filename, char *txt_filename, char *out_filename)
{
	if (EVNLoadFile(evn_filename) != 0)
	{
		printf("Error loading evn file\n");
		return -1;
	}

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
				attr1 & FILE_ATTRIBUTE_DIRECTORY)
			{
				printf("Invalid input EVN filename\n", argv[i+1]);
				usage();
				return 4;
			}
			else if(attr2 == INVALID_FILE_ATTRIBUTES ||
				     attr2 & FILE_ATTRIBUTE_DIRECTORY)
			{
				printf("Invalid input TXT filename: %s\n", argv[i+2]);
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
