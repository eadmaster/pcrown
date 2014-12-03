
#ifndef _VDP_H_
#define _VDP_H_

#include "vdp1.h"
#include "vdp2.h"


/* Global data */
extern volatile unsigned short *vdp1_vram;
extern volatile unsigned short *vdp1_fb;
extern volatile unsigned short *vdp2_vram;
extern volatile unsigned short *vdp2_cram;

/* Function prototypes */
void vdp_init(void);
void vdp_shutdown();


/* Create a 5:5:5 RGB color with the MSB set */
#define COLOR(r,g,b) (((r)&0x1F)|((g)&0x1F)<<5|((b)&0x1F)<<10|0x8000)
#define TRANSCOLOR   0x0000


/**********************************************************************
 * VDP1 Commands stuff                                               *
**********************************************************************/
unsigned short vdp1_list_get_index(void);
void vdp1_list_set_index(unsigned short i);
void vdp1_list_start(void);
void vdp1_mesh(int n);


void vdp1_add_end(void);
void vdp1_add_system_clip(short x, short y);
void vdp1_add_user_clip(short x1, short y1, short x2, short y2);
void vdp1_add_local_coords(short x, short y);
void vdp1_add_polyline(short x1, short y1, short x2, short y2, short x3, short y3, short x4, short y4, unsigned short color);
void vdp1_add_polygon(short x1, short y1, short x2, short y2, short x3, short y3, short x4, short y4, unsigned short color, unsigned long grda);
void vdp1_add_distorted_sprite(short x1, short y1, short x2, short y2, short x3, short y3, short x4, short y4, unsigned long source, int w, int h);



#endif /* _VDP_H_ */

