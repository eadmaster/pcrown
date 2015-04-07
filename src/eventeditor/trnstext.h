#ifndef trnstext_h__
#define trnstext_h__

typedef struct
{
   int event_id;
   char *orig_text;
   char *trans_text;
   char *notes;
} ttentry_struct;

typedef struct
{
   ttentry_struct *ttentry;
   int num;
} transtext_struct;

extern transtext_struct maintranstext;

BOOL TransConvertText(transtext_struct *transtext);
bool TransTextSave(const char *filename, transtext_struct *transtext);
int TransTextLoad(const char *filename, transtext_struct *transtext);
void TransTextFree(transtext_struct *transtext);
#endif // trnstext_h__