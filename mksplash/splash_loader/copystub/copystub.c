
#include "copystub.h"


// HRAM 0x06004000 ~ 0x060FFFFF
#   define HRAM_STT 0x06004000
#   define HRAM_END 0x06100000

// LRAM (don't erase the last bytes containing copystub executable).
#   define LRAM_STT 0x00200000
#   define LRAM_END COPYSTUB_START_ADRESS



#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))



int _main(unsigned char* src_stt, unsigned long length, unsigned char* dst_stt)
{
    /* Retrieve the parameters (buffer address, length, execution start address). */
    unsigned char* dst_end = dst_stt + length;

    /* Data used for copy/erase. */
    unsigned char* ptr1;
    unsigned char* ptr2;
    /* Data used for erase only. */
    unsigned long erase_stt, erase_end;
    unsigned char i, j;


    /* Copy the target program from the source buffer to the execution area. */
    if(src_stt != dst_stt)
    { /* In the case src == dst, don't need to copy anything. */
        if( (dst_stt >  src_stt         )
        &&  (dst_stt < (src_stt+length)) )
        { /* Special case when buffers overlap. */
            ptr1 = src_stt+length - 1;
            ptr2 = dst_end        - 1;
            while((ptr2+1) != dst_stt)
            {
                *(ptr2--) = *(ptr1--);
            }
        }
        else
        { /* Normal copy when buffers don't overlap. */
            ptr1 = src_stt;
            ptr2 = dst_stt;
            while(ptr2 != dst_end)
            {
                *(ptr2++) = *(ptr1++);
            }
        }
    }

    /* Clear High/Low RAM */
    for(i=0; i<2; i++)
    { /* HRAM-LRAM */
        for(j=0; j<2; j++)
        { /* cropped zone1-cropped zone2 */

            if(i==0)
            { /* Erase LRAM. */
                /* In order not to erase target program, crop the zone to erase. */
                if(j==0)
                { /* Start zone. */
                    erase_stt = LRAM_STT;
                    erase_end = CLAMP((unsigned long)dst_stt, LRAM_STT, LRAM_END);
                }
                else
                { /* End zone. */
                    erase_stt = CLAMP((unsigned long)dst_end, LRAM_STT, LRAM_END);
                    erase_end = LRAM_END;
                }
            }
            else
            { /* Erase HRAM. */
                /* In order not to erase target program, crop the zone to erase. */
                if(j==0)
                { /* Start zone. */
                    erase_stt = HRAM_STT;
                    erase_end = CLAMP((unsigned long)dst_stt, HRAM_STT, HRAM_END);
                }
                else
                { /* End zone. */
                    erase_stt = CLAMP((unsigned long)dst_end, HRAM_STT, HRAM_END);
                    erase_end = HRAM_END;
                }
            }

            /* Erase cropped zone. */
            ptr1 = (unsigned char*)erase_stt;
            ptr2 = (unsigned char*)erase_end;
            for(length=erase_end-erase_stt; length; length--)
            {
                *(ptr1++) = 0;
            }
        }
    }


    /* Execute the target program (-> quit this program).
     * (code scrap from antime's satclient.c, http://www.iki.fi/~antime/)
     */
    void (*pFun)(void);
    pFun = (void(*)(void))dst_stt;
    (*pFun)();

    return 0;
}
