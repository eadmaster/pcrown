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
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <TCHAR.h>

#include "uihelper.h"

void ListView_InsertTextColumnW(HWND hDlg, int nIDDlgItem, int width, int id, LPCWSTR text)
{
   LVCOLUMNW coldata;
        
   coldata.mask = LVCF_TEXT | LVCF_WIDTH;
   coldata.pszText = (LPWSTR)text;
   coldata.cchTextMax = (int)lstrlenW(coldata.pszText);
   coldata.cx = width;
   SendDlgItemMessageW(hDlg, nIDDlgItem, LVM_INSERTCOLUMN, (WPARAM)id, (LPARAM)&coldata);
}

void ListView_InsertTextColumnA(HWND hDlg, int nIDDlgItem, int width, int id, LPCSTR text)
{
   LVCOLUMNA coldata;
        
   coldata.mask = LVCF_TEXT | LVCF_WIDTH;
   coldata.pszText = (LPSTR)text;
   coldata.cchTextMax = (int)lstrlenA(coldata.pszText);
   coldata.cx = width;
   SendDlgItemMessageA(hDlg, nIDDlgItem, LVM_INSERTCOLUMN, (WPARAM)id, (LPARAM)&coldata);
}

void ListView_AddTextItemW(HWND hDlg, int nIDDlgItem, int iItem, int iSubItem, LPCWSTR text)
{
   LVITEMW itemdata;

   itemdata.mask = LVIF_TEXT;
   itemdata.iItem = iItem;
   itemdata.iSubItem = iSubItem;
   itemdata.pszText = (LPWSTR)text;
   itemdata.cchTextMax = (int)lstrlenW(itemdata.pszText);

   if (itemdata.iSubItem == 0)
      SendDlgItemMessage(hDlg, nIDDlgItem, LVM_INSERTITEMW, 0, (LPARAM)&itemdata);
   else
      SendDlgItemMessage(hDlg, nIDDlgItem, LVM_SETITEMW, 0, (LPARAM)&itemdata);
}

void ListView_AddTextItemA(HWND hDlg, int nIDDlgItem, int iItem, int iSubItem, LPCSTR text)
{
   LVITEMA itemdata;

   itemdata.mask = LVIF_TEXT;
   itemdata.iItem = iItem;
   itemdata.iSubItem = iSubItem;
   itemdata.pszText = (LPSTR)text;
   itemdata.cchTextMax = (int)lstrlenA(itemdata.pszText);

   if (itemdata.iSubItem == 0)
      SendDlgItemMessageA(hDlg, nIDDlgItem, LVM_INSERTITEMA, 0, (LPARAM)&itemdata);
   else
      SendDlgItemMessageA(hDlg, nIDDlgItem, LVM_SETITEMA, 0, (LPARAM)&itemdata);
}

void ListView_AddIntItem(HWND hDlg, int nIDDlgItem, int iItem, int iSubItem, int val, BOOL hex)
{
   TCHAR text[128];
   if (hex)
      _stprintf_s(text, sizeof(text)/sizeof(WCHAR), _T("%x"), val);
   else
      _stprintf_s(text, sizeof(text)/sizeof(WCHAR), _T("%d"), val);
   ListView_AddTextItem(hDlg, nIDDlgItem, iItem, iSubItem, text);
}

int ListView_GetTextSelItemW(HWND hDlg, int nIDDlgItem, int iSubItem, LPWSTR text, int cchTextMax)
{
   LVITEMW itemdata;
   int cursel=(int)SendDlgItemMessageW(hDlg, nIDDlgItem, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

   if (cursel == -1)
      return cursel;

   itemdata.mask = LVIF_TEXT;
   itemdata.iItem = cursel;
   itemdata.iSubItem = iSubItem;
   itemdata.pszText = text;
   itemdata.cchTextMax = cchTextMax;
   SendDlgItemMessageW(hDlg, nIDDlgItem, LVM_GETITEM, 0, (LPARAM)&itemdata);

   return cursel;
}

int ListView_GetTextSelItemA(HWND hDlg, int nIDDlgItem, int iSubItem, LPSTR text, int cchTextMax)
{
   LVITEMA itemdata;
   int cursel=(int)SendDlgItemMessageA(hDlg, nIDDlgItem, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

   if (cursel == -1)
      return cursel;

   itemdata.mask = LVIF_TEXT;
   itemdata.iItem = cursel;
   itemdata.iSubItem = iSubItem;
   itemdata.pszText = text;
   itemdata.cchTextMax = cchTextMax;
   SendDlgItemMessageA(hDlg, nIDDlgItem, LVM_GETITEM, 0, (LPARAM)&itemdata);

   return cursel;
}

int ListView_SetCurSel(HWND hDlg, int nIDDlgItem, int iItem)
{
   LVITEM itemdata;

   itemdata.stateMask = LVIS_SELECTED;
   itemdata.state = 0;

   SendDlgItemMessage(hDlg, nIDDlgItem, LVM_SETITEMSTATE, -1, (LPARAM)&itemdata);

   itemdata.state = LVIS_SELECTED;
   return (int)SendDlgItemMessage(hDlg, nIDDlgItem, LVM_SETITEMSTATE, iItem, (LPARAM)&itemdata);
}

HTREEITEM TreeView_AddTextItemW(HWND hDlg, int nIDDlgItem, HTREEITEM hParent, HTREEITEM hInsertAfter, LPWSTR lpszItem, LPARAM lParam)
{ 
    TVITEMW tvi; 
    TVINSERTSTRUCTW tvins;

    tvi.mask = TVIF_TEXT | TVIF_PARAM;

    tvi.pszText = lpszItem; 
    tvi.cchTextMax = sizeof(tvi.pszText)/sizeof(tvi.pszText[0]); 
    tvi.lParam = lParam;

    tvins.item = tvi; 
    tvins.hInsertAfter = hInsertAfter; 

    // Set the parent item based on the specified level. 
    tvins.hParent = hParent; 

    return(HTREEITEM)SendDlgItemMessageW(hDlg, nIDDlgItem, TVM_INSERTITEM, 
                                        0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);
} 

HTREEITEM TreeView_AddTextItemA(HWND hDlg, int nIDDlgItem, HTREEITEM hParent, HTREEITEM hInsertAfter, LPSTR lpszItem, LPARAM lParam)
{ 
    TVITEMA tvi; 
    TVINSERTSTRUCTA tvins;

    tvi.mask = TVIF_TEXT | TVIF_PARAM;

    tvi.pszText = lpszItem; 
    tvi.cchTextMax = sizeof(tvi.pszText)/sizeof(tvi.pszText[0]); 
    tvi.lParam = lParam;

    tvins.item = tvi; 
    tvins.hInsertAfter = hInsertAfter; 

    // Set the parent item based on the specified level. 
    tvins.hParent = hParent; 

    return(HTREEITEM)SendDlgItemMessageA(hDlg, nIDDlgItem, TVM_INSERTITEM, 
                                        0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);
} 

// Create a Tool tip for the specified HWND parent, using string
int CreateHelpBalloonW(HINSTANCE hInstance, helpballoon_struct *hb, HWND hwndParent, LPWSTR string)
{
   TOOLINFOW ti;
   RECT rect;
   HWND *hwnd;

   if ((hwnd = calloc(hb->numballoons+1, sizeof(HWND))) == NULL)
      return FALSE;

   hwnd[hb->numballoons] = CreateWindowExW(WS_EX_TOPMOST,
                                          TOOLTIPS_CLASSW,
                                          NULL,
                                          WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
                                          CW_USEDEFAULT,
                                          CW_USEDEFAULT,
                                          CW_USEDEFAULT,
                                          CW_USEDEFAULT,
                                          hwndParent,
                                          NULL,
                                          hInstance,
                                          NULL);

   if (!hwnd[hb->numballoons])
   {
      free(hwnd);
      return FALSE;
   }

   SetWindowPos(hwnd[hb->numballoons], HWND_TOPMOST, 0, 0, 0, 0,
                SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

   // Create some help balloons
   ti.cbSize = sizeof(TOOLINFO);
   ti.uFlags = TTF_SUBCLASS;
   ti.hwnd = hwndParent;
   ti.hinst = hInstance;
   ti.uId = 0;
   ti.lpszText = string;
   GetClientRect(hwndParent, &rect);
   ti.rect.left = rect.left;
   ti.rect.top = rect.top;
   ti.rect.right = rect.right;
   ti.rect.bottom = rect.bottom;

   // Add it
   SendMessageW(hwnd[hb->numballoons], TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);
   if (hb->balloon)
   {
      memcpy(hwnd, hb->balloon, sizeof(HWND) * hb->numballoons);
      free(hb->balloon);
   }
   hb->balloon = hwnd;
   hb->numballoons++;

   return TRUE;
}

int CreateHelpBalloonA(HINSTANCE hInstance, helpballoon_struct *hb, HWND hwndParent, LPSTR string)
{
   TOOLINFOA ti;
   RECT rect;
   HWND *hwnd;

   if ((hwnd = calloc(hb->numballoons+1, sizeof(HWND))) == NULL)
      return FALSE;

   hwnd[hb->numballoons] = CreateWindowExA(WS_EX_TOPMOST,
                                          TOOLTIPS_CLASSA,
                                          NULL,
                                          WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
                                          CW_USEDEFAULT,
                                          CW_USEDEFAULT,
                                          CW_USEDEFAULT,
                                          CW_USEDEFAULT,
                                          hwndParent,
                                          NULL,
                                          hInstance,
                                          NULL);

   if (!hwnd[hb->numballoons])
   {
      free(hwnd);
      return FALSE;
   }

   SetWindowPos(hwnd[hb->numballoons], HWND_TOPMOST, 0, 0, 0, 0,
                SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

   // Create some help balloons
   ti.cbSize = sizeof(TOOLINFO);
   ti.uFlags = TTF_SUBCLASS;
   ti.hwnd = hwndParent;
   ti.hinst = hInstance;
   ti.uId = 0;
   ti.lpszText = string;
   GetClientRect(hwndParent, &rect);
   ti.rect.left = rect.left;
   ti.rect.top = rect.top;
   ti.rect.right = rect.right;
   ti.rect.bottom = rect.bottom;

   // Add it
   SendMessageA(hwnd[hb->numballoons], TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);
   if (hb->balloon)
   {
      memcpy(hwnd, hb->balloon, sizeof(HWND) * hb->numballoons);
      free(hb->balloon);
   }
   hb->balloon = hwnd;
   hb->numballoons++;

   return TRUE;
}

void DestroyAllBalloons(helpballoon_struct *hb)
{
   int i;
   if (hb->balloon != NULL)
   {
      for (i = 0; i < hb->numballoons; i++)
      {
         if (hb->balloon[i])
            DestroyWindow(hb->balloon[i]);
      }
      
      free(hb->balloon);
      hb->balloon = NULL;
   }

   hb->numballoons = 0;
}

void SetupOFNW(OPENFILENAMEW *ofn, int type, HWND hwnd, LPCWSTR lpstrFilter, LPWSTR lpstrFile, DWORD nMaxFile)
{
   ZeroMemory(ofn, sizeof(OPENFILENAMEW));
   ofn->lStructSize = sizeof(OPENFILENAMEW);
   ofn->hwndOwner = hwnd;
   ofn->lpstrFilter = lpstrFilter;
   ofn->nFilterIndex = 1;
   ofn->lpstrFile = lpstrFile;
   ofn->nMaxFile = nMaxFile;

   switch (type)
   {
      case OFN_DEFAULTSAVE:
      {
         ofn->Flags = OFN_OVERWRITEPROMPT;
         break;
      }
      case OFN_DEFAULTLOAD:
      {
         ofn->Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
         break;
      }
      case OFN_MULTILOAD:
      {
         ofn->Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;
         break;
      }
   }

}

void SetupOFNA(OPENFILENAMEA *ofn, int type, HWND hwnd, LPCSTR lpstrFilter, LPSTR lpstrFile, DWORD nMaxFile)
{
   ZeroMemory(ofn, sizeof(OPENFILENAMEA));
   ofn->lStructSize = sizeof(OPENFILENAMEA);
   ofn->hwndOwner = hwnd;
   ofn->lpstrFilter = lpstrFilter;
   ofn->nFilterIndex = 1;
   ofn->lpstrFile = lpstrFile;
   ofn->nMaxFile = nMaxFile;

   switch (type)
   {
      case OFN_DEFAULTSAVE:
      {
         ofn->Flags = OFN_OVERWRITEPROMPT;
         break;
      }
      case OFN_DEFAULTLOAD:
      {
         ofn->Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
         break;
      }
      case OFN_MULTILOAD:
      {
         ofn->Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;
         break;
      }
   }

}

int AttachDebug(void)
{
   FILE *fp;
   int consolefd;

   if (AllocConsole() == 0)
      return -1;

   if ((consolefd = _open_osfhandle((intptr_t)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT)) == -1)
   {
      FreeConsole();
      return -1;
   }

   fflush(stdin);
   if ((fp = _fdopen(consolefd, "w")) == NULL)
   {
      FreeConsole();
      return -1;
   }

   *stdout = *fp;
   setvbuf(stdout, NULL, _IONBF, 0);

   if ((consolefd = _open_osfhandle((intptr_t)GetStdHandle(STD_ERROR_HANDLE), _O_TEXT)) == -1)
   {
      FreeConsole();
      return -1;
   }

   fflush(stdin);
   if ((fp = _fdopen(consolefd, "w")) == NULL)
   {
      FreeConsole();
      return -1;
   }

   *stderr = *fp;
   setvbuf(stderr, NULL, _IONBF, 0);

   return 0;
}

void DetachDebug(void)
{
   FreeConsole();
}

easythread_struct *CreateEasyThread(LPTHREAD_START_ROUTINE lpStartAddress, void *extra)
{
   easythread_struct *et=malloc(sizeof(easythread_struct));

   if (et == NULL)
      return NULL;

   et->killflag = FALSE;
   et->extra = extra;
   et->handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)lpStartAddress, (void *)et, 0, &et->thread_id);

   if (et->handle == INVALID_HANDLE_VALUE)
   {
      free(et);
      return NULL;
   }

   return et;
}

void CloseEasyThread(easythread_struct *et)
{
   if (et)
   {
      if (et->handle != INVALID_HANDLE_VALUE)
      {
         et->killflag = TRUE;
         if (WaitForSingleObject(et->handle, INFINITE) == WAIT_TIMEOUT)
         {
            // Couldn't close thread cleanly
            TerminateThread(et->handle,0);                                  
         }          
         CloseHandle(et->handle);
         et->handle = INVALID_HANDLE_VALUE;
      }

      free(et);
   }
}


LRESULT CALLBACK EasyBusyDlg (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   static easythread_struct *et;
   static easybusydlg_struct *ebd;
//   HWND hctl;

   switch(message) {
      case WM_INITDIALOG:
      {
         et=(easythread_struct *)lParam;
         ebd=(easybusydlg_struct *)et->extra;
         ebd->hwnd = hwnd;
         return TRUE;
      }
      case WM_COMMAND:
         switch (LOWORD(wParam))
         {
            case IDCANCEL:
               EndDialog(hwnd, FALSE);
#if 0
               hctl = FindWindowExA(hwnd, NULL, "STATIC", NULL);
               SetWindowText(hctl, "Cancelling... Please wait.");
               EnableWindow(GetDlgItem(hwnd, IDCANCEL), FALSE);
#endif
               et->killflag = TRUE;
               return TRUE;
            default: 
                return FALSE;
         }
      default:
         return FALSE;
   }
}

DWORD WINAPI EasyBusyDlgThread(void *b)
{
   easythread_struct *et=(easythread_struct *)b;
   volatile easybusydlg_struct *ebd=et->extra;
   DialogBoxParam(ebd->hInstance, MAKEINTRESOURCE(ebd->id), ebd->hparent, (DLGPROC)EasyBusyDlg, (LPARAM)et);
   return 0;
}

easythread_struct *CreateEasyBusyDialogW(HINSTANCE hInstance, LPCWSTR title, HWND hwndParent, int id)
{
   volatile easybusydlg_struct *ebd;
   easythread_struct *et;

   if ((ebd = (volatile easybusydlg_struct *)malloc(sizeof(easybusydlg_struct))) == NULL)
      return NULL;

   ebd->hInstance = hInstance;
   ebd->hwnd = NULL;
   ebd->hparent = hwndParent;
   ebd->id = id;

   if ((et = CreateEasyThread((LPTHREAD_START_ROUTINE)EasyBusyDlgThread, (void *)ebd)) == NULL)
   {
      free((void *)ebd);
      return NULL;
   }
   while (ebd->hwnd == NULL) { Sleep(10); }
   SetWindowTextW(ebd->hwnd, title);
   EnableWindow(GetDlgItem(ebd->hwnd, IDCANCEL), FALSE);
   return et;
}

easythread_struct *CreateEasyBusyDialogA(HINSTANCE hInstance, LPCSTR title, HWND hwndParent, int id)
{
   volatile easybusydlg_struct *ebd;
   easythread_struct *et;

   if ((ebd = (volatile easybusydlg_struct *)malloc(sizeof(easybusydlg_struct))) == NULL)
      return NULL;

   ebd->hInstance = hInstance;
   ebd->hwnd = NULL;
   ebd->hparent = hwndParent;
   ebd->id = id;

   if ((et = CreateEasyThread((LPTHREAD_START_ROUTINE)EasyBusyDlgThread, (void *)ebd)) == NULL)
   {
      free((void *)ebd);
      return NULL;
   }
   while (ebd->hwnd == NULL) { Sleep(10); }
   SetWindowTextA(ebd->hwnd, title);
   EnableWindow(GetDlgItem(ebd->hwnd, IDCANCEL), FALSE);
   return et;
}

void CloseEasyBusyDialog(easythread_struct *et)
{
   volatile easybusydlg_struct *ebd=et->extra;
   EndDialog(ebd->hwnd, TRUE);
   free((void *)ebd);
   CloseEasyThread(et);
}
