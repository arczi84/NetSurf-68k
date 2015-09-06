#undef __USE_INLINE__
#define __USE_INLINE__

#include "amigaos3/os3support.h"

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/graphics.h>
#include <proto/datatypes.h>
#include <proto/pngalpha.h>
#include <datatypes/pictureclass.h>

#include <clib/alib_protos.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdbool.h>
#include <malloc.h>

#include <sdl/sdl.h>

/* structure for framebuffer toolkit bitmaps  */
struct fbtk_bitmap {
        int width;
        int height;
        ULONG *pixdata;
        bool opaque;

        /* The following two are only used for cursors */
        int hot_x;
        int hot_y;
};

/**
 * Create a bitmap.
 *
 * \param  width   width of image in pixels
 * \param  height  width of image in pixels
 * \param  state   a flag word indicating the initial state
 * \return an opaque struct bitmap, or NULL on memory exhaustion
 */

void *bitmap_create(int width, int height, unsigned int state)
{
	struct bitmap *bitmap;
	
	bitmap = AllocVec(sizeof(struct bitmap),MEMF_PRIVATE | MEMF_CLEAR);
	if(bitmap)
	{
	
		bitmap->pixdata = AllocVec(width*height*4,MEMF_PRIVATE | MEMF_CLEAR); //os3
						
		bitmap->width = width;
		bitmap->height = height;

		if(state & BITMAP_OPAQUE) bitmap->opaque = true;
			else bitmap->opaque = false;
	}

	return bitmap;
}

/**
 * Return a pointer to the pixel data in a bitmap.
 *
 * \param  bitmap  a bitmap, as returned by bitmap_create()
 * \return pointer to the pixel buffer
 *
 * The pixel data is packed as BITMAP_FORMAT, possibly with padding at the end
 * of rows. The width of a row in bytes is given by bitmap_get_rowstride().
 */

unsigned char *bitmap_get_buffer(void *bitmap)
{
	nsfb_t *bm = bitmap;
	unsigned char *bmpptr;

	//assert(bm != NULL);

	//nsfb_get_buffer(bm, &bmpptr, NULL);
					
	return bm->pixdata = xrgb;
}


void ami_bitmap_argb_to_rgba(struct bitmap *bm)
{
	if(bm == NULL) return;
	
	ULONG *data = (ULONG *)bitmap_get_buffer(bm);
	
	alpha = AllocVec(h*w*4,MEMF_ANY|MEMF_CLEAR);		
	alpha = pngalpha_Load((STRPTR) filename, (UWORD)w, (UWORD)h); 
	
	for(int i = 0; i < ((bitmap_get_rowstride(bm) / sizeof(ULONG)) * bitmap_get_height(bm)); i++) { 
		data[i] = (data[i] << 8) | (data[i] >> 24);
	}
}

struct fbtk_bitmap *load_bitmap(APTR filename)
{	
	Object *o;
		
	if (o = NewDTObject (filename,
			DTA_GroupID,GID_PICTURE,
			PDTA_Remap,FALSE,
			PDTA_DestMode,PMODE_V43,
			TAG_END))
		{
		struct BitMapHeader *bmhd = NULL;

		GetDTAttrs (o,PDTA_BitMapHeader,&bmhd,TAG_END);

		if (bmhd)
			{
            UWORD w = (UWORD)bmhd->bmh_Width;
            UWORD h = (UWORD)bmhd->bmh_Height;
			ULONG *xrgb;
			UBYTE *alpha;	
			
			xrgb= AllocVec(h*w*4,MEMF_ANY|MEMF_CLEAR);
			
			DoMethod (o,PDTM_READPIXELARRAY,xrgb,PBPAFMT_ARGB,w*4,0,0,w,h);

			alpha = AllocVec(h*w*4,MEMF_ANY|MEMF_CLEAR);		
			alpha = pngalpha_Load((STRPTR) filename, (UWORD)w, (UWORD)h); 
				
			//for(int i = 0; i < ((((w)*4) / sizeof(ULONG*)) * h); i++) { 
			//	xrgb[i] = (xrgb[i] << 8) | (alpha[i] >> 24); }
			
            for(int i = 0; i < (w * h); i++) { 
                xrgb[i] = (alpha[i] << 24) | (xrgb[i] & 0x00ffffff);}		
					
			struct fbtk_bitmap *bm;
			
			bm = bitmap_create(w, h, 0);
			//malloc(sizeof(struct fbtk_bitmap));

			if(bm != NULL)
				{
				bm->width = w;
				bm->height = h;
				bm->pixdata = xrgb;
				//bm->opaque = bitmap_test_opaque(bm);				

				//free(bm);
				}
				FreeVec (xrgb);
			 }

		DisposeDTObject (o);
				
		}
		
	return bm;
}

struct fbtk_bitmap *load_bitmap2(APTR filename)
{	
	Object *dto;
	struct bitmap *bm = NULL;

	int bm_format = PBPAFMT_ARGB;

	if(dto = NewDTObject(filename,
					DTA_GroupID, GID_PICTURE,
					PDTA_DestMode, PMODE_V43,
					TAG_DONE))
	{
		struct BitMapHeader *bmh;
		struct RastPort rp;

		if(GetDTAttrs(dto, PDTA_BitMapHeader, &bmh, TAG_DONE))
		{
			bm = bitmap_create(bmh->bmh_Width, bmh->bmh_Height, 0);

			DoMethod(dto, PDTM_READPIXELARRAY, bitmap_get_buffer(bm),
				bm_format, bitmap_get_rowstride(bm), 0, 0,
				bmh->bmh_Width, bmh->bmh_Height);
#ifndef __amigaos4__
				ami_bitmap_argb_to_rgba(bm);
#endif				
			bitmap_set_opaque(bm, bitmap_test_opaque(bm));			
			
		}
		DisposeDTObject(dto);
		
	}
		 
	return bm;
}
