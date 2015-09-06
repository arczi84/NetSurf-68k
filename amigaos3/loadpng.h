#ifndef __LOADPNG_H__
#define __LOADPNG_H__

/* $Id: loadpng.h,v 1.1 2008/06/27 22:01:48 itix Exp $ */

/* structure for framebuffer toolkit bitmaps  */
struct fbtk_bitmap {
        int width, height, modulo;
        UBYTE *pixdata;
        BOOL opaque;
	    ULONG data[0];
	
        /* The following two are only used for cursors */
        int hot_x;
        int hot_y;
};

//APTR png_load(CONST_STRPTR filename);

#endif /* __LOADPNG_H__ */
