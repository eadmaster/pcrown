/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */ 
/* Latest source code is available at http://ppcenter.free.fr. */


#ifndef MKSPLASH_COMMON_H
#define MKSPLASH_COMMON_H

/* Splash image width and height */
#define SPLASH_IMG_WIDTH  320
#define SPLASH_IMG_HEIGHT 224


/**
 * Memory-related informations.
 * 0x0000 Loader (max 8KB)
 * 0x2000 0.bin offset (4 bytes)
 * 0x2004 0.bin length (4 bytes)
 * 0x2008 splash offset (4 bytes)
 * 0x200C splash length (4 bytes)
 * 0x2010 0.bin + splash data
**/
#define LOADER_EXEC_ADDR 0x06004000
#define LOADER_MAX_SIZE (8*1024)
#define PARAMS_SIZE     (16)


#define OFFSET_ZEROBIN_OFS (LOADER_MAX_SIZE +  0)
#define OFFSET_ZEROBIN_LEN (LOADER_MAX_SIZE +  4)
#define OFFSET_SPLASH_OFS  (LOADER_MAX_SIZE +  8)
#define OFFSET_SPLASH_LEN  (LOADER_MAX_SIZE + 12)

/* Max 1MB (HRAM size) per file. */
#define DATA_MAX_SIZE (1024*1024 - 0x4000)


#endif // MKSPLASH_COMMON_H

