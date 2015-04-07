#ifndef util_h
#define util_h

#ifdef __cplusplus
extern "C" {
#endif

int round(float x);
unsigned short WordSwap(unsigned short data);
void WordSwapPtr(unsigned short *var);
unsigned long DoubleWordSwap(unsigned long data);
void DoubleWordSwapPtr(unsigned long *var);
int ReallocMem(void **data, int data_size, int *max_data, int index);

#ifdef __cplusplus
}
#endif

#endif
