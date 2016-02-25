#ifndef evn_h__
#define evn_h__

#include "trnstext.h"

typedef struct command_struct
{
   unsigned char cmd;
   int offset;
   char *name;
   void (*list_func)(struct command_struct *, int index, HWND hDlg);
   int arg_length;
   unsigned char arg[256];
   unsigned char cmd43_var;
   int time_diff;
} command_struct;

typedef struct
{
   unsigned short offset_offset;
   unsigned short data_offset;
   unsigned long data_size;
} textdataheader_struct;

typedef struct
{
   unsigned long arg_length;
   char *name;
} text_struct;

typedef struct
{
   unsigned long decode_counter;
   unsigned char magic_number;
   unsigned char *textbuffer;
   BOOL finished;
} dcmp_text_struct;

extern command_struct *command;
extern unsigned long num_commands;
extern bool file_loaded;
extern unsigned char *font_data;
extern ttentry_struct *names_data;
extern unsigned long num_names;


int EVNFreeFile();
int EVNLoadFile(const char *filename);
unsigned long EVNGetCompressionOffsetAlt(int i);
unsigned long EVNGetCompressionNum(int index);

int EVNSaveFile(const char *filename, const char *names_filename);
int EVNSaveFile(const char *filename);
void UpdateEvent(HWND hWnd);
bool IsDialogueCommand(command_struct *command);
void GenerateDialogText(command_struct *command, BOOL include_format, void *text, size_t text_size, BOOL unicode_convert);
void DumpBin(unsigned long tbl_addr, int tbl_size, unsigned long cmp_addr);
void DecompressText(dcmp_text_struct *dcmp_info, unsigned char *data, unsigned long offset, unsigned long start, unsigned long end, BOOL include_format, char *text, size_t text_size);
int EVNLoadNamesItemsText(const char *filename, ttentry_struct **ttentry, unsigned long *tbl_size);
unsigned short *CompressTextAlt(unsigned char *outbuf, int *out_size, int max_out_size, ttentry_struct *ttentry, int text_num, int compress);
#endif // evn_h__
