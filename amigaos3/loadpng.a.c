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

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <png.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdbool.h>
#include <malloc.h>
#include <exec/types.h>

#include <clib/dos_protos.h>

#if PNG_LIBPNG_VER < 10209
#define png_set_expand_gray_1_2_4_to_8(png) png_set_gray_1_2_4_to_8(png)
#endif

static png_structp png;
static png_infop info;
static int interlace;
static size_t rowbytes;
static int raw_width, raw_height;
static int rowstride;
static unsigned char *bitmap_data;
static bool is_cursor = false;
static int raw_hot_x, raw_hot_y;

static void info_callback(png_structp png, png_infop info);
static void row_callback(png_structp png, png_bytep new_row,
                         png_uint_32 row_num, int pass);
static void end_callback(png_structp png, png_infop info);

/* structure for framebuffer toolkit bitmaps  */
struct fbtk_bitmap {
        int width;
        int height;
        uint8_t *pixdata;
        bool opaque;

        /* The following two are only used for cursors */
        int hot_x;
        int hot_y;
};

static void info_callback(png_structp png, png_infop info);
static void row_callback(png_structp png, png_bytep new_row,
                         png_uint_32 row_num, int pass);
static void end_callback(png_structp png, png_infop info);

struct fbtk_bitmap *load_bitmap(const char *filename)
{
        BPTR f;
        unsigned char buffer[1024];
        int br;
        int x, y, c;
          
        png = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
        info = png_create_info_struct(png);
        
        png_set_progressive_read_fn(png, NULL, info_callback, row_callback, end_callback);
        
        f = Open(filename, MODE_OLDFILE);
        if (!f) {
                printf("          Unable to open %s\n", filename);
				Close(f);		
                return NULL;
        }
        
        do {
                br = Read(f, buffer, 1024);
                if (br > 0) {
                        png_process_data(png, info, buffer, br);
                }
        } while (br > 0);
        
        if (br < 0) {
                printf("Error reading input: %s\n");
                return NULL;
        }
		
        Close(f);		
		
		struct fbtk_bitmap *bm;

		bm = malloc(sizeof(struct fbtk_bitmap));

		if(bm != NULL)
		{

					bm->width = raw_width;
					bm->height = raw_height;
					bm->pixdata = (uint8_t*) bitmap_data;
					bm->opaque = false;

					return bm;
			
		}
		
        free(bm);
        return NULL;
}

static void
info_callback(png_structp png, png_infop info)
{
	int bit_depth, color_type, interlace, intent;
	double gamma;
	png_uint_32 width, height;
	
	/* Read the PNG details */
	png_get_IHDR(png, info, &width, &height, &bit_depth,
			&color_type, &interlace, 0, 0);
        
        /* Set up our transformations */
	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png);
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png);
	if (png_get_valid(png, info, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png);
	if (bit_depth == 16)
		png_set_strip_16(png);
	if (color_type == PNG_COLOR_TYPE_GRAY ||
			color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png);
	if (!(color_type & PNG_COLOR_MASK_ALPHA))
		png_set_filler(png, 0xff, PNG_FILLER_AFTER);
	/* gamma correction - we use 2.2 as our screen gamma
	 * this appears to be correct (at least in respect to !Browse)
	 * see http://www.w3.org/Graphics/PNG/all_seven.html for a test case
	 */
	if (png_get_sRGB(png, info, &intent))
	        png_set_gamma(png, 2.2, 0.45455);
	else {
	        if (png_get_gAMA(png, info, &gamma))
	                png_set_gamma(png, 2.2, gamma);
	        else
	                png_set_gamma(png, 2.2, 0.45455);
	}


	png_read_update_info(png, info);

	rowbytes = png_get_rowbytes(png, info);
	interlace = (interlace == PNG_INTERLACE_ADAM7);
	raw_width = width;
	raw_height = height;
        
        rowstride = raw_width * 4;
        bitmap_data = malloc(rowstride * raw_height);
}

static unsigned int interlace_start[8] = {0, 16, 0, 8, 0, 4, 0};
static unsigned int interlace_step[8] = {28, 28, 12, 12, 4, 4, 0};
static unsigned int interlace_row_start[8] = {0, 0, 4, 0, 2, 0, 1};
static unsigned int interlace_row_step[8] = {8, 8, 8, 4, 4, 2, 2};

static void
row_callback(png_structp png, png_bytep new_row,
             png_uint_32 row_num, int pass)
{
	unsigned long i, j;
	unsigned int start, step;
        unsigned char *row = bitmap_data + (rowstride * row_num);
                
        if (new_row == 0)
                return;
        
        if (interlace) {
		start = interlace_start[pass];
 		step = interlace_step[pass];
		row_num = interlace_row_start[pass] +
                        interlace_row_step[pass] * row_num;

		/* Copy the data to our current row taking interlacing
		 * into consideration */
                row = bitmap_data + (rowstride * row_num);
		for (j = 0, i = start; i < rowbytes; i += step) {
			row[i++] = new_row[j++];
			row[i++] = new_row[j++];
			row[i++] = new_row[j++];
			row[i++] = new_row[j++];
		}
        } else {
                memcpy(row, new_row, rowbytes);
        }
}

static void
end_callback(png_structp png, png_infop info)
{
}

/*
 * Local Variables:
 * c-basic-offset:8
 * End:
 */
