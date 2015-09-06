/*
 * Copyright 2009 Daniel Silverstone <dsilvers@netsurf-browser.org>
 *
 * This file is part of NetSurf, http://www.netsurf-browser.org/
 *
 * NetSurf is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * NetSurf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>
#include <sdl/sdl.h>
#include <sys/types.h>
#include <exec/types.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/pngflux.h>
#include <proto/intuition.h>

#include "palette.h"

struct Library *PngfluxBase;


/* structure for framebuffer toolkit bitmaps  */
struct fbtk_bitmap {
        int width;
        int height;
        uint32_t pixdata;
        bool opaque;

        /* The following two are only used for cursors */
        int hot_x;
        int hot_y;
};


struct fbtk_bitmap *load_bitmap(const char *filename)
{

	if (!(PngfluxBase = OpenLibrary (PNGFLUXNAME,PNGFLUXVERSION)))
			{
			Printf (PNGFLUXNAME " is missing.\n");
			return NULL;
			}

	struct PNGFObject *obj;

	if (obj = pngfLoad (filename))
		{
		uint32_t array;

		array = (uint32_t) pngfGetAttr (obj,PNGFA_ARGBDATA);
	
		int size = obj->pfo_Width * obj->pfo_Height;

		//for (unsigned int * rgba_ptr = rgbaBuffer, * argb_ptr = argbBuffer + size - 1; argb_ptr >= argbBuffer; rgba_ptr++, argb_ptr--)
	/*	for(int i = 0; i < (size); i++) { 
		{
			// *argb_ptr = *rgba_ptr >> 8 | 0xff000000;  // - this version doesn't change endianess
			array[i] = SDL_Swap32(array) >> 8 | 0xff000000;  // This does
		}
			*/
		struct fbtk_bitmap *bm;

		bm = AllocVec(size*4,MEMF_ANY|MEMF_CLEAR);
		//Bpp = 0;
		if(bm != NULL)
			{

					bm->width = obj->pfo_Width;
					bm->height = obj->pfo_Height;
					bm->pixdata = array;
					bm->opaque = false;

					return bm;
			
			}
		//Bpp = 32;
		FreeVec (obj);
        FreeVec (bm);		
		}
		else
			{
			PrintFault (IoErr(),filename);
			return NULL;
			}		
		
        return NULL;
}



/*
 * Local Variables:
 * c-basic-offset:8
 * End:
 */
