/*
LodePNG Examples

Copyright (c) 2005-2012 Lode Vandevenne

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/

/*
Compile command for Linux:
gcc lodepng.c example_sdl.c -ansi -pedantic -Wall -Wextra -lSDL -O3 -o showpng

*/

/*
LodePNG SDL example
This example displays a PNG with a checkerboard pattern to show tranparency.
It requires the SDL library to compile and run.
If multiple filenames are given to the command line, it shows all of them.
Press any key to see next image, or esc to quit.
*/

#include <sys/types.h>
#include <stdbool.h>
#include <exec/types.h>
#include <proto/exec.h>
#include <clib/dos_protos.h>

#include "lodepng.h"

#include <SDL/SDL.h>

//* structure for framebuffer toolkit bitmaps  */
struct fbtk_bitmap {
        int width;
        int height;
        uint8_t *pixdata;
        bool opaque;

        /* The following two are only used for cursors */
        int hot_x;
        int hot_y;
};

struct fbtk_bitmap *load_bitmap(const char *filename)
{
  unsigned error;
  unsigned char* image;
  unsigned width, height;

  //printf("showing %s\n", filename);

  /*load the PNG in one function call*/
  error = lodepng_decode32_file(&image, &width, &height, filename);
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
	
		struct fbtk_bitmap *bm;

		bm = AllocVec(width*height*4,MEMF_ANY|MEMF_CLEAR);

		if(bm != NULL)
		{

					bm->width = width;
					bm->height = height;
					bm->pixdata =  (uint8_t *)image;
					bm->opaque = false;

					return bm;
		}
		
        FreeVec (bm);

  /*cleanup*/
  free(image);

}
