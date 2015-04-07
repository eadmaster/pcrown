/*
 * Sega Saturn start screen & bootloader
 * By cafe-alpha, 2013
 *
 * From Sega Saturn cartridge flash tool
 * by Anders Montonen, 2012
 *
 * Original software by ExCyber
 * Graphics routines by Charles MacDonald
 *
 * Creative Commons Attribution-ShareAlike 3.0 Unported (CC BY-SA 3.0)
 */
#include <stdio.h>
#include <string.h> /* For memset,memcpy */

#include "vdp.h"
#include "../compress.h"
#include "../common.h"

#include "misc.h"




void _main(void)
{
    unsigned char* BootDataPtr    = *((unsigned char**)(LOADER_EXEC_ADDR + OFFSET_ZEROBIN_OFS));
    unsigned long  BootDataLen    = *((unsigned long *)(LOADER_EXEC_ADDR + OFFSET_ZEROBIN_LEN));
    unsigned char* SplashImagePtr = *((unsigned char**)(LOADER_EXEC_ADDR + OFFSET_SPLASH_OFS ));
    unsigned long  SplashImageLen = *((unsigned long *)(LOADER_EXEC_ADDR + OFFSET_SPLASH_LEN));

    unsigned short* splash_ptr = (unsigned short *)0x25C40000;
    unsigned char*  boot_ptr   = (unsigned char  *)0x00200000;
    unsigned char*  exec_ptr   = (unsigned char  *)0x06004000;
    unsigned long   boot_len;

    unsigned short padData;
    int pad_connected;

    pad_connected = InitPad();

    vdp_init();

    /* Set up VDP1 to appear under VDP2 graphics (0=hide, 1-7=visible) */
    PRISA = 0x0101;
    PRISB = 0x0101;
    PRISC = 0x0101;
    PRISD = 0x0101;

    /* Set up VDP1 registers */
    TVMR = 0x0000;
    FBCR = 0x0000;
    PTMR = 0x0002;
    EWDR = COLOR(7,7,7);    /* Erase color: gray */
    EWLR = 0x0000;          /* Erase range: (0,0) -> (352,240) */
    EWRR = 0x50DF;

    /* Start Filling VDP1 command list with buffer common stuff. */
    vdp1_mesh(0);

    vdp1_add_system_clip(320, 224);
    vdp1_add_user_clip(0, 0, 319, 223);
    vdp1_add_local_coords(0, 0);

    /* Decompress splash image. */
    decomp_exec(SplashImagePtr, SplashImageLen, (unsigned char*)splash_ptr);

    vdp1_add_distorted_sprite(
            0                 , 0, 
            SPLASH_IMG_WIDTH-1, 0, 
            SPLASH_IMG_WIDTH-1, SPLASH_IMG_HEIGHT-1, 
            0                 , SPLASH_IMG_HEIGHT-1, 
            (((unsigned long)splash_ptr) & 0xFFFFF)>>3, 
            SPLASH_IMG_WIDTH, SPLASH_IMG_HEIGHT);

    vdp1_add_polyline(
            0                 , 0, 
            SPLASH_IMG_WIDTH-1, 0, 
            SPLASH_IMG_WIDTH-1, SPLASH_IMG_HEIGHT-1, 
            0                 , SPLASH_IMG_HEIGHT-1, 
            COLOR(31, 31, 31));

    vdp1_add_end();


    /* Screen on */
    TVMD = 0x8000;
    TVMR = 0x0000;
    FBCR = 0x0001;
    EWRR = 0x50DF;

    /* Decompress boot code. */
    boot_len = decomp_exec(BootDataPtr, BootDataLen, (unsigned char*)boot_ptr);



    while (1)
    {
        padData = ReadPad(pad_connected);

        /* B : Load USB dev cart ROM code. */
        if (padData & PAD_B)
        {
            rom_usb_dev_cart_io();
        }

        /* A,C or Start : exit splash screen. */
        if (padData & PAD_START)
            break;
        if (padData & PAD_A)
            break;
        if (padData & PAD_C)
            break;


        WaitForVBLANKIn();
        WaitForVBLANKOut();
    }


    vdp_shutdown();

    /* Exec boot code. */
    stub_execute(boot_ptr/*tmp_address*/, boot_len, exec_ptr/*exec_address*/);

//    ExitToMultiplayer();

}



