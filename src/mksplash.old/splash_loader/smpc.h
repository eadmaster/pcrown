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
#ifndef SMPC_H_
#define SMPC_H_

#include <stdio.h>

#define SMPC_BASE   0x20100000

#define IREG0       (*(volatile unsigned char*)(SMPC_BASE+0x01))
#define IREG1       (*(volatile unsigned char*)(SMPC_BASE+0x03))
#define IREG2       (*(volatile unsigned char*)(SMPC_BASE+0x05))
#define IREG3       (*(volatile unsigned char*)(SMPC_BASE+0x07))
#define IREG4       (*(volatile unsigned char*)(SMPC_BASE+0x09))
#define IREG5       (*(volatile unsigned char*)(SMPC_BASE+0x0b))
#define IREG6       (*(volatile unsigned char*)(SMPC_BASE+0x0d))
#define COMREG      (*(volatile unsigned char*)(SMPC_BASE+0x1f))
#define OREG0       (*(volatile unsigned char*)(SMPC_BASE+0x21))
#define OREG1       (*(volatile unsigned char*)(SMPC_BASE+0x23))
#define OREG2       (*(volatile unsigned char*)(SMPC_BASE+0x25))
#define OREG3       (*(volatile unsigned char*)(SMPC_BASE+0x27))
#define OREG4       (*(volatile unsigned char*)(SMPC_BASE+0x29))
#define OREG5       (*(volatile unsigned char*)(SMPC_BASE+0x2b))
#define OREG6       (*(volatile unsigned char*)(SMPC_BASE+0x2d))
#define OREG7       (*(volatile unsigned char*)(SMPC_BASE+0x2f))
#define OREG8       (*(volatile unsigned char*)(SMPC_BASE+0x31))
#define OREG9       (*(volatile unsigned char*)(SMPC_BASE+0x33))
#define OREG10      (*(volatile unsigned char*)(SMPC_BASE+0x35))
#define OREG11      (*(volatile unsigned char*)(SMPC_BASE+0x37))
#define OREG12      (*(volatile unsigned char*)(SMPC_BASE+0x39))
#define OREG13      (*(volatile unsigned char*)(SMPC_BASE+0x3b))
#define OREG14      (*(volatile unsigned char*)(SMPC_BASE+0x3d))
#define OREG15      (*(volatile unsigned char*)(SMPC_BASE+0x3f))
#define OREG16      (*(volatile unsigned char*)(SMPC_BASE+0x41))
#define OREG17      (*(volatile unsigned char*)(SMPC_BASE+0x43))
#define OREG18      (*(volatile unsigned char*)(SMPC_BASE+0x45))
#define OREG19      (*(volatile unsigned char*)(SMPC_BASE+0x47))
#define OREG20      (*(volatile unsigned char*)(SMPC_BASE+0x49))
#define OREG21      (*(volatile unsigned char*)(SMPC_BASE+0x4b))
#define OREG22      (*(volatile unsigned char*)(SMPC_BASE+0x4d))
#define OREG23      (*(volatile unsigned char*)(SMPC_BASE+0x4f))
#define OREG24      (*(volatile unsigned char*)(SMPC_BASE+0x51))
#define OREG25      (*(volatile unsigned char*)(SMPC_BASE+0x53))
#define OREG26      (*(volatile unsigned char*)(SMPC_BASE+0x55))
#define OREG27      (*(volatile unsigned char*)(SMPC_BASE+0x57))
#define OREG28      (*(volatile unsigned char*)(SMPC_BASE+0x59))
#define OREG29      (*(volatile unsigned char*)(SMPC_BASE+0x5b))
#define OREG30      (*(volatile unsigned char*)(SMPC_BASE+0x5d))
#define OREG31      (*(volatile unsigned char*)(SMPC_BASE+0x5f))
#define SR          (*(volatile unsigned char*)(SMPC_BASE+0x61))
#define SF          (*(volatile unsigned char*)(SMPC_BASE+0x63))
#define PDR1        (*(volatile unsigned char*)(SMPC_BASE+0x75))
#define PDR2        (*(volatile unsigned char*)(SMPC_BASE+0x77))
#define DDR1        (*(volatile unsigned char*)(SMPC_BASE+0x79))
#define DDR2        (*(volatile unsigned char*)(SMPC_BASE+0x7b))
#define IOSEL       (*(volatile unsigned char*)(SMPC_BASE+0x7d))
#define IOSEL1      1
#define IOSEL2      2

#define EXLE        (*(volatile unsigned char*)(SMPC_BASE+0x7f))
#define EXLE1       1
#define EXLE2       2

/* SMPC commands */
#define MSHON       0x00
#define SSHON       0x02
#define SSHOFF      0x03
#define SNDON       0x06
#define SNDOFF      0x07
#define CDON        0x08
#define CDOFF       0x09
#define SYSRES      0x0d
#define CKCHG352    0x0e
#define CKCHG320    0x0f
#define INTBACK     0x10
#define SETTIME     0x16
#define SETSMEM     0x17
#define NMIREQ      0x18
#define RESENAB     0x19
#define RESDISA     0x1a

#endif /* SMPC_H_ */
