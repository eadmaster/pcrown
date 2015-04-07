
#include <stdio.h>
#include "vdp.h"



/* Global data */
volatile unsigned short *vdp1_vram = (volatile unsigned short *)VDP1_VRAM;
volatile unsigned short *vdp1_fb   = (volatile unsigned short *)VDP1_FRAMEBUFFER;
volatile unsigned short *vdp2_vram = (volatile unsigned short *)VDP2_VRAM;
volatile unsigned short *vdp2_cram = (volatile unsigned short *)VDP2_CRAM;
volatile unsigned short *vdp2_reg  = (volatile unsigned short *)VDP2_REGISTER_BASE;


/*
 * Global variables stuff.
 * Warning : globals are not directly usable from outside.
 */
unsigned short _list_index = 0; /* Offset into the command list */
unsigned short _mesh_flag  = 0; /* Global mesh enable flag */
void vdp_init(void)
{
    //void vdp2_init(void)
    int i;
    TVMD = 0x0000;

    *((unsigned short *) 0x25f8000e) = *((unsigned short *) 0x25f8000e) & (~0x3000);

    // Map Offset Register: Bitmap screen will be located at VRAM offset 0
    /*0x25f8003c*/MPOFN = 0;

    // Character Control Register: 256 colors, enable NBG0 as a bitmap
    // |8 for 1024x256 bitmap
    /*0x25f80028*/CHCTLA = 0x0012|8;

    // Screen Scroll Value Registers: No scroll
    /*0x25f80070*/SCXIN0 = 0;
    /*0x25f80072*/SCXDN0 = 0;
    /*0x25f80074*/SCYIN0 = 0;

    // Screen Display Enable Register: Invalidate the transparency code for
    // NBG0 and display NBG0
    /*0x25f80020*/BGON = 0x0001;

    /* Clear VRAM */
    for(i = 0; i < 0x40000; i++)
        vdp2_vram[i] = 0x0000;

    /* Clear CRAM */
    for(i = 0; i < 0x0800; i++)
        vdp2_cram[i] = 0x0000;



    //void vdp1_init(void)
    /* Set up VDP1 to appear under VDP2 graphics (0=hide, 1-7=visible) */
    PRISA = 0x0101;
    PRISB = 0x0101;
    PRISC = 0x0101;
    PRISD = 0x0101;

    /* Set up VDP1 registers */
    TVMR = 0x0000;
    FBCR = 0x0003;       /* VPD1 refresh : "Manual mode (change)"          */
    PTMR = 0x0002;       /* Starts drawing automatically with frame change */
    EWDR = COLOR(0,0,0); /* Erase color: black                             */
    EWLR = 0x0000;       /* Erase range: (0,0) -> (352,240)                */
    EWRR = 0x50DF;

    /* Initialize display list */
    _list_index = 0;
    _mesh_flag  = 0;
}

void vdp_shutdown(void)
{
    int i;

    //TVMD = 0x0000;

    for(i = 0; i < 0x40000; i++)
    {
        vdp1_vram[i] = 0x0000;
    }
    vdp1_vram[0] = Ctrl_END;

    //for(i = 0; i < 0x100; i++)
    //{
    //    vdp2_reg[i] = 0x0000;
    //}

    for(i = 0; i < 0x40000; i++)
    {
        vdp2_vram[i] = 0x0000;
    }

    for(i = 0; i < 0x0800; i++)
    {
        vdp2_cram[i] = 0x0000;
    }
}




/**********************************************************************
 * VDP1 Commands stuff                                               *
**********************************************************************/


/* Macros */
/* Get the address of a command in the VDP1 VRAM */
#define CMD_ADDRESS(index) ((unsigned short *)(VDP1_VRAM + ((index) << 5)))




/* Global variables "get" functions */
unsigned short vdp1_list_get_index(void)
{
    return _list_index;
}
void vdp1_list_set_index(unsigned short i)
{
    _list_index = i;
}
/* Set or clear the global mesh processing flag */
void vdp1_mesh(int n)
{
    _mesh_flag=(n?0x0100:0x0000);
}



/* Terminate current command list. */
void vdp1_add_end(void)
{
    unsigned short *p;

    p = CMD_ADDRESS(_list_index);

    p[CMDCTRL] = 0x8000; // END
}


/* Add system clip to command list */
void vdp1_add_system_clip(short x, short y)
{
    unsigned short *p;

    p = CMD_ADDRESS(_list_index);

    p[CMDCTRL] = 0x0009; /* Set system clip */
    p[CMDLINK] = 0x0000; /* Jump address */
    p[CMDXC]   = x;
    p[CMDYC]   = y;

    _list_index++;
}


/* Add user clip to command list */
void vdp1_add_user_clip(short x1, short y1, short x2, short y2)
{
    unsigned short *p;

    p = CMD_ADDRESS(_list_index);

    p[CMDCTRL] = 0x0008; /* Set user clip */
    p[CMDLINK] = 0x0000; /* Jump address */
    p[CMDXA]   = x1;
    p[CMDYA]   = y1;
    p[CMDXC]   = x2;
    p[CMDYC]   = y2;

    _list_index++;
}

/* Add local coords to command list */
void vdp1_add_local_coords(short x, short y)
{
    unsigned short *p;

    p = CMD_ADDRESS(_list_index);

    p[CMDCTRL] = 0x000A; /* Set local coords */
    p[CMDLINK] = 0x0000; /* Jump address */
    p[CMDXA]   = x;
    p[CMDYA]   = y;

    _list_index++;
}



/* Add local coords to command list */
void vdp1_add_polyline(short x1, short y1, short x2, short y2, short x3, short y3, short x4, short y4, unsigned short color)
{
    unsigned short *p;

    p = CMD_ADDRESS(_list_index);

    p[CMDCTRL] = 0x0005; /* Polyline */
    p[CMDLINK] = 0x0000; /* Jump address */
    p[CMDPMOD] = 0x00C0 | _mesh_flag;
    p[CMDCOLR] = color;
    p[CMDXA]   = x1;
    p[CMDYA]   = y1;
    p[CMDXB]   = x2;
    p[CMDYB]   = y2;
    p[CMDXC]   = x3;
    p[CMDYC]   = y3;
    p[CMDXD]   = x4;
    p[CMDYD]   = y4;

    _list_index++;
}

/* Add local coords to command list */
void vdp1_add_polygon(short x1, short y1, short x2, short y2, short x3, short y3, short x4, short y4, unsigned short color, unsigned long grda)
{
    unsigned short *p;

    p = CMD_ADDRESS(_list_index);

    p[CMDCTRL] = 0x0004; /* Polygon */
    p[CMDLINK] = 0x0000; /* Jump address */
    p[CMDPMOD] = 0x00C0 | _mesh_flag | 4;
    p[CMDCOLR] = color;
    p[CMDXA]   = x1;
    p[CMDYA]   = y1;
    p[CMDXB]   = x2;
    p[CMDYB]   = y2;
    p[CMDXC]   = x3;
    p[CMDYC]   = y3;
    p[CMDXD]   = x4;
    p[CMDYD]   = y4;
    p[CMDGRDA] = grda>>3;

    _list_index++;
}

/* Add local coords to command list */
void vdp1_add_distorted_sprite(short x1, short y1, short x2, short y2, short x3, short y3, short x4, short y4, unsigned long source, int w, int h)
{
    unsigned short *p;

    p = CMD_ADDRESS(_list_index);

    p[CMDCTRL] = 0x0002; /* Distorted sprite, RGB mode */
    p[CMDLINK] = 0x0000; /* Jump address */
    p[CMDPMOD] = 0x00A8 | _mesh_flag;
    p[CMDCOLR] = 0;
    p[CMDSRCA] = source;
    p[CMDSIZE] = (((w>>3)&0x3F)<<8)|(h&0xFF);
    p[CMDXA]   = x1;
    p[CMDYA]   = y1;
    p[CMDXB]   = x2;
    p[CMDYB]   = y2;
    p[CMDXC]   = x3;
    p[CMDYC]   = y3;
    p[CMDXD]   = x4;
    p[CMDYD]   = y4;


    _list_index++;
}



