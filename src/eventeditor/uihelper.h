#ifndef UIHELPER_H
#define UIHELPER_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#include <windows.h>
#include <commctrl.h>

void ListView_InsertTextColumnW(HWND hDlg, int nIDDlgItem, int width, int id, LPCWSTR text);
void ListView_InsertTextColumnA(HWND hDlg, int nIDDlgItem, int width, int id, LPCSTR text);
void ListView_AddTextItemW(HWND hDlg, int nIDDlgItem, int iItem, int iSubItem, LPCWSTR text);
void ListView_AddTextItemA(HWND hDlg, int nIDDlgItem, int iItem, int iSubItem, LPCSTR text);
void ListView_AddIntItem(HWND hDlg, int nIDDlgItem, int iItem, int iSubItem, int val, BOOL hex);
int ListView_GetTextSelItemW(HWND hDlg, int nIDDlgItem, int iSubItem, LPWSTR text, int cchTextMax);
int ListView_GetTextSelItemA(HWND hDlg, int nIDDlgItem, int iSubItem, LPSTR text, int cchTextMax);
int ListView_SetCurSel(HWND hDlg, int nIDDlgItem, int iItem);

#ifdef UNICODE
#define ListView_InsertTextColumn ListView_InsertTextColumnW
#define ListView_AddTextItem ListView_AddTextItemW
#define ListView_GetTextSelItem ListView_GetTextSelItemW
#else
#define ListView_InsertTextColumn ListView_InsertTextColumnA
#define ListView_AddTextItem ListView_AddTextItemA
#define ListView_GetTextSelItem ListView_GetTextSelItemA
#endif

HTREEITEM TreeView_AddTextItemW(HWND hDlg, int nIDDlgItem, HTREEITEM hParent, HTREEITEM hInsertAfter, LPWSTR lpszItem, LPARAM lParam);
HTREEITEM TreeView_AddTextItemA(HWND hDlg, int nIDDlgItem, HTREEITEM hParent, HTREEITEM hInsertAfter, LPSTR lpszItem, LPARAM lParam);

#ifdef UNICODE
#define TreeView_AddTextItem TreeView_AddTextItemW
#else
#define TreeView_AddTextItem TreeView_AddTextItemA
#endif

typedef struct
{
   HWND *balloon;
   int numballoons;
} helpballoon_struct;

int CreateHelpBalloonW(HINSTANCE hInstance, helpballoon_struct *hb, HWND hwndParent, LPWSTR string);
int CreateHelpBalloonA(HINSTANCE hInstance, helpballoon_struct *hb, HWND hwndParent, LPSTR string);
void DestroyAllBalloons(helpballoon_struct *hb);

#ifdef UNICODE
#define CreateHelpBalloon CreateHelpBalloonW
#else
#define CreateHelpBalloon CreateHelpBalloonA
#endif

enum { OFN_DEFAULTSAVE=0, OFN_DEFAULTLOAD, OFN_MULTILOAD };

void SetupOFNW(OPENFILENAMEW *ofn, int type, HWND hwnd, LPCWSTR lpstrFilter, LPWSTR lpstrFile, DWORD nMaxFile);
void SetupOFNA(OPENFILENAMEA *ofn, int type, HWND hwnd, LPCSTR lpstrFilter, LPSTR lpstrFile, DWORD nMaxFile);

#ifdef UNICODE
#define SetupOFN SetupOFNW
#else
#define SetupOFN SetupOFNA
#endif

int AttachDebug(void);
void DetachDebug(void);

typedef struct
{
   HANDLE handle;
   DWORD thread_id;
   int killflag;
   void *extra;
} easythread_struct;

typedef struct
{
   HINSTANCE hInstance;
   HWND hwnd;
   HWND hparent;
   int id;
} easybusydlg_struct;

easythread_struct *CreateEasyThread(LPTHREAD_START_ROUTINE lpStartAddress, void *extra);
void CloseEasyThread(easythread_struct *et);
easythread_struct *CreateEasyBusyDialogW(HINSTANCE hInstance, LPCWSTR title, HWND hwndParent, int id);
easythread_struct *CreateEasyBusyDialogA(HINSTANCE hInstance, LPCSTR title, HWND hwndParent, int id);
void CloseEasyBusyDialog(easythread_struct *et);
#ifdef UNICODE
#define CreateEasyBusyDialog CreateEasyBusyDialogW
#else
#define CreateEasyBusyDialog CreateEasyBusyDialogA
#endif

#define _tsizeof(f) (sizeof(f) / sizeof(TCHAR))

#ifdef UNICODE
#define teasyatow(lpMultiByteStr, lpWideCharStr) \
   MultiByteToWideChar(CP_ACP, MB_COMPOSITE, lpMultiByteStr, -1, lpWideCharStr, sizeof(lpWideCharStr) / sizeof(WCHAR));
#define teasywtoa(lpWideCharStr, lpMultiByteStr) \
   WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, lpWideCharStr, -1, lpMultiByteStr, sizeof(lpMultiByteStr), NULL, NULL);
#else
#define teasyatow(lpMultiByteStr, lpWideCharStr) \
   strcpy(lpWideCharStr, lpMultiByteStr);
#define teasywtoa(lpWideCharStr, lpMultiByteStr) \
   strcpy(lpMultiByteStr, lpWideCharStr);
#endif
#endif

#ifdef _DEBUG
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

#ifdef __cplusplus
}
#endif

#endif