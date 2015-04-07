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
#ifndef _MISC_H_
#define _MISC_H_

#include <stdio.h>
#include <string.h> /* For memset,memcpy */


/**
 * Copy/execute stub.
**/

/* Parameters: tmp_address(4), length(4), exec_address(4) */
void stub_execute(unsigned char* tmp_address, unsigned long length, unsigned char* exec_address);



/**
 * Pad related
**/
#define PAD_START   (1<<11)
#define PAD_A       (1<<10)
#define PAD_C       (1<<9)
#define PAD_B       (1<<8)
int InitPad(void);
unsigned short ReadPad(int pad_connected);




/**
 * USB dev cart ROM code
**/
#define ROM_CHECK_ADDRESS 0x0203FFFF

#define ROMCODE_MSK_USB_DEV_CART 0x81
#define ROMCODE_VAL_USB_DEV_CART 0x01
#define ROM_USB_EXEC_ADDRESS  0x0203F800
#define rom_usb_dev_cart_io() (((*((unsigned char*)ROM_CHECK_ADDRESS)) & ROMCODE_MSK_USB_DEV_CART) == ROMCODE_VAL_USB_DEV_CART ? ((int (*)(void))ROM_USB_EXEC_ADDRESS)() : -1)



/**
 * ExitToMultiplayer
**/

void ExitToMultiplayer(void);


#endif /* _MISC_H_ */
