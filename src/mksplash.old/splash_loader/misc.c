/*
 * Sega Saturn start screen & bootloader
 * By cafe-alpha, 2013
 *
 * From Sega Saturn cartridge flash tool
 * by Anders Montonen, 2012
 *
 * Original software by ExCyber
 * Graphics routines by Charles MacDonald
 */
#include <stdio.h>
#include <stdlib.h>

#include "misc.h"


/**
 * Copystub data.
 * From copystubwrap.S
**/
extern const unsigned char CopyStubData;
extern const unsigned long CopyStubDataLen;

/* Start address, maximum length. */
#include "copystub/copystub.h"


void stub_execute(unsigned char* tmp_address, unsigned long length, unsigned char* exec_address)
{
    if(CopyStubDataLen > COPYSTUB_LENGTH) return;

    /* Copy stub to memory and call it (-> quit SatCom library).
     * Code scrap from antime's satclient.c, http://www.iki.fi/~antime/
     */
    int (*stub)(unsigned char*, unsigned long, unsigned char*);
    stub = (int(*)(unsigned char*, unsigned long, unsigned char*))COPYSTUB_START_ADRESS;

    memcpy((void*)stub, (void*)&CopyStubData, CopyStubDataLen);

    (*stub)(tmp_address, length, exec_address);
}




/**
 * Pad related
**/
#include "smpc.h"

int InitPad(void)
{
    int pad_connected = 3;
    unsigned char temp;
    int count;

    DDR1    = 0x60; /* TH,TR out, TL,D3-D0 in */
    DDR2    = 0x60; /* TH,TR out, TL,D3-D0 in */
    IOSEL   = IOSEL1 | IOSEL2; /* Manual mode for both ports */
    EXLE    = 0x00;

    /** Test pad1 **/
    PDR1 = 0x60;
    for(count=0; count < 16; count++)
    {
        PDR1 = 0x60;
    }
    temp = PDR1;

    if((temp&0x1) != 0x00)
    { /* Pad1 is disconnected. */
        pad_connected -= 1;
    }


    /** Test pad2 **/
    PDR2 = 0x60;
    for(count=0; count < 16; count++)
    {
        PDR2 = 0x60;
    }
    temp = PDR2;

    if((temp&0x1) != 0x00)
    { /* Pad2 is disconnected. */
        pad_connected -= 2;
    }

    return pad_connected;
}

unsigned short ReadPad(int pad_connected)
{
    unsigned short bits;

    if(pad_connected & 0x1)
    { /* Read input from pad1 */
        PDR1 = 0x60;
        bits = (PDR1 & 0x8) << 9;
        PDR1 = 0x40;
        bits |= (PDR1 & 0xf) << 8;
        PDR1 = 0x20;
        bits |= (PDR1 & 0xf) << 4;
        PDR1 = 0x00;
        bits |= (PDR1 & 0xf);
    }
    else
    { /* Read input from pad2 */
        PDR2 = 0x60;
        bits = (PDR2 & 0x8) << 9;
        PDR2 = 0x40;
        bits |= (PDR2 & 0xf) << 8;
        PDR2 = 0x20;
        bits |= (PDR2 & 0xf) << 4;
        PDR2 = 0x00;
        bits |= (PDR2 & 0xf);
    }

    return bits ^ 0x1FFF;
}



/**
 * ExitToMultiplayer
**/

void ExitToMultiplayer(void)
{
    /* Exit to multiplayer. */
    (**(volatile void(**)(void))0x600026C)();
}

