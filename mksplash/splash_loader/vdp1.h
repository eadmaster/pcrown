#ifndef VDP1_H_
#define VDP1_H_

#define VDP1_BASE           0x25c00000
#define VDP1_VRAM           VDP1_BASE
#define VDP1_VRAM_TOP       VDP1_BASE+0x80000
#define VDP1_FRAMEBUFFER    VDP1_BASE+0x80000
#define VDP1_REGISTER_BASE  VDP1_BASE+0x100000

/* Macros specific for processor. */
#define CMD_TABLE(x, y) (0x25c00000 + ((x) << 5) + (((y) << 1) & 0x1f))
#define VRAM(x, y)      (0x25c04000 + ((x) + ((y) << 1))
#define GOURAUD(x, y)   (0x25c7d800 + ((x) << 4) + ((y) << 1))
#define CLUT(x, y)      (0x25c7e000 + ((x) << 4) + ((y) << 1))

/* Set or clear the global mesh processing flag */
#define MESH(n) (mesh_flag=(n)?0x0100:0x0000)

/* Command table elements */
#define CMDCTRL 0x00
#define CMDLINK 0x01
#define CMDPMOD 0x02
#define CMDCOLR 0x03
#define CMDSRCA 0x04
#define CMDSIZE 0x05
#define CMDXA   0x06
#define CMDYA   0x07
#define CMDXB   0x08
#define CMDYB   0x09
#define CMDXC   0x0A
#define CMDYC   0x0B
#define CMDXD   0x0C
#define CMDYD   0x0D
#define CMDGRDA 0x0E
#define CMDDUMY 0x0F






#define TVMR    (*(volatile unsigned short *)(VDP1_REGISTER_BASE+0x00))
#define VBE     (1<<3)
#define TVMNormal   (0)
#define TVMHiRes    (1)
#define TVMRot16    (2)
#define TVMRot8     (3)
#define TVMHDTV     (4)

#define FBCR    (*(volatile unsigned short *)(VDP1_REGISTER_BASE+0x02))
#define EOS     (1<<4)
#define DIE     (1<<3)
#define DIL     (1<<2)
#define FCM     (1<<1)
#define FCT     (1<<0)

#define PTMR    (*(volatile unsigned short *)(VDP1_REGISTER_BASE+0x04))
#define PlotStop    (0)
#define PlotStart   (1)
#define PlotAuto    (2)

#define EWDR    (*(volatile unsigned short *)(VDP1_REGISTER_BASE+0x06))
#define EWLR    (*(volatile unsigned short *)(VDP1_REGISTER_BASE+0x08))
#define EWRR    (*(volatile unsigned short *)(VDP1_REGISTER_BASE+0x0a))
#define ENDR    (*(volatile unsigned short *)(VDP1_REGISTER_BASE+0x0c))
#define EDSR    (*(volatile unsigned short *)(VDP1_REGISTER_BASE+0x10))
#define CEF     (1<<1)
#define BEF     (1<<0)

#define LOPR    (*(volatile unsigned short *)(VDP1_REGISTER_BASE+0x12))
#define COPR    (*(volatile unsigned short *)(VDP1_REGISTER_BASE+0x14))
#define MODR    (*(volatile unsigned short *)(VDP1_REGISTER_BASE+0x16))

// typedef struct
// {
    // unsigned short CMDCTRL;
    // unsigned short CMDLINK;
    // unsigned short CMDPMOD;
    // unsigned short CMDCOLR;
    // unsigned short CMDSRCA;
    // unsigned short CMDSIZE;
    // signed short CMDXA;
    // signed short CMDYA;
    // signed short CMDXB;
    // signed short CMDYB;
    // signed short CMDXC;
    // signed short CMDYC;
    // signed short CMDXD;
    // signed short CMDYD;
    // unsigned short CMDGRDA;
    // unsigned short dummy;
// } drawing_command_t; 

/* CMDCTRL */

#define Ctrl_END            0x8000
#define Ctrl_SPRITE         0
#define Ctrl_SCALED_SPRITE  1
#define Ctrl_DIST_SPRITE    2
#define Ctrl_POLY           4
#define Ctrl_POLYLINE       5
#define Ctrl_LINE           6
#define Ctrl_USERCLIP       8
#define Ctrl_SYSTEMCLIP     9
#define Ctrl_LOCALCOORD     10

#define Ctrl_JUMPNEXT       (0<<12)
#define Ctrl_JUMPASSIGN     (1<<12)
#define Ctrl_JUMPCALL       (2<<12)
#define Ctrl_JUMPRET        (3<<12)
#define Ctrl_SKIPNEXT       (4<<12)
#define Ctrl_SKIPASSIGN     (5<<12)
#define Ctrl_SKIPCALL       (6<<12)
#define Ctrl_SKIPRET        (7<<12)

#define Ctrl_ZP_TWOCOORD    (0<<8)
#define Ctrl_ZP_UL          (5<<8)
#define Ctrl_ZP_UC          (6<<8)
#define Ctrl_ZP_UR          (7<<8)
#define Ctrl_ZP_CL          (9<<8)
#define Ctrl_ZP_CC          (10<<8)
#define Ctrl_ZP_CR          (11<<8)
#define Ctrl_ZP_LL          (13<<8)
#define Ctrl_ZP_LC          (14<<8)
#define Ctrl_ZP_LR          (15<<8)

#define Ctrl_Dir_NORMAL     (0<<4)
#define Ctrl_Dir_HFLIP      (1<<4)
#define Ctrl_Dir_VFLIP      (2<<4)
#define Ctrl_Dir_HVFLIP     (3<<4)

/* CMDPMOD */

#define Pmod_MON            (1<<15)
#define Pmod_HSS            (1<<12)
#define Pmod_PCLP           (1<<11)
#define Pmod_CLIP           (1<<10)
#define Pmod_CMOD           (1<<9)
#define Pmod_MESH           (1<<8)
#define Pmod_ECD            (1<<7)
#define Pmod_SPD            (1<<6)
#define Pmod_CMB16          (0<<3)
#define Pmod_CML16          (1<<3)
#define Pmod_CMB64          (2<<3)
#define Pmod_CMB128         (3<<3)
#define Pmod_CMB256         (4<<3)
#define Pmod_CMRGB          (5<<3)
#define Pmod_CM0            Pmod_CMB16
#define Pmod_CM1            Pmod_CML16
#define Pmod_CM2            Pmod_CMB64
#define Pmod_CM3            Pmod_CMB128
#define Pmod_CM4            Pmod_CMB256
#define Pmod_CM5            Pmod_CMRGB
#define Pmod_CC_GOURAUD     (1<<2)
#define Pmod_CC_HALFLUM     (1<<1)
#define Pmod_CC_SHADOW      (1<<0)
#define Pmod_CC_REPLACE     (0)
#define Pmod_CC_HALFTRANS   (3)

typedef struct
{
    unsigned short A;
    unsigned short B;
    unsigned short C;
    unsigned short D;
} gouraud_table_t;

typedef struct
{
    unsigned short col0;
    unsigned short col1;
    unsigned short col2;
    unsigned short col3;
    unsigned short col4;
    unsigned short col5;
    unsigned short col6;
    unsigned short col7;
    unsigned short col8;
    unsigned short col9;
    unsigned short cola;
    unsigned short colb;
    unsigned short colc;
    unsigned short cold;
    unsigned short cole;
    unsigned short colf;
} color_table_t;

#endif /* VDP1_H_ */
