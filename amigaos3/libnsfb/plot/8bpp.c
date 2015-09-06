/*
 * Copyright 2009 Vincent Sanders <vince@simtec.co.uk>
 * Copyright 2010 Michael Drake <tlsa@netsurf-browser.org>
 *
 * This file is part of libnsfb, http://www.netsurf-browser.org/
 * Licenced under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 */

#include <stdbool.h>
#include <endian.h>
#include <stdlib.h>
#include <string.h>
#include <sdl/sdl.h>

#include "libnsfb.h"
#include "libnsfb_plot.h"
#include "libnsfb_plot_util.h"

#include "nsfb.h"
#include "palette.h"
#include "plot.h"


static inline uint8_t *get_xy_loc(nsfb_t *nsfb, int x, int y)
{
        return (uint8_t *)(nsfb->ptr + (y * nsfb->linelen) + (x));
}

static inline nsfb_colour_t pixel_to_colour(nsfb_t *nsfb, uint8_t pixel)
{
        if (nsfb->palette == NULL)
                return 0;

        return nsfb->palette->data[pixel];
}

/** Find best palette match for given colour. */
static uint8_t colour_to_pixel(nsfb_t *nsfb, nsfb_colour_t c)
{
#if defined AGA676
	register int r, g, b;

	b = c & 0xFFFFFF;
	g = 0xFF;
	r = b & g;
	b = b >> 8;
	g = b & g;
	b = b >> 8;

	if (r > 250)
		if (g > 250)
			if (b > 250) return 3; /* 100 %  white */
				 
	if (g == b)
		if (g == r) /* this color is gray */
			if (g > 0x79) 
				if (g < 0xE2) {
					if (g < 0x85)
						return 0; /* 50 % gray */
					if (g > 0xD6)
						return 2; /* 86 % gray */
					if (g > 0xAD)
					    if (g < 0xB9)
						return 1; /* 70 % gray */
				}
				
	if (dither676) {
		if (pushRGBlevel = ~pushRGBlevel) { /* push up every 2. pixel  */
			r += 22;
			g += 19;
			b += 22;
		}
	}
	else {
			r += 11;
			g +=  9;
			b += 11;
	}

    return table_for_cube_676[r+556] + table_for_cube_676[g+278] + table_for_cube_676[b];
	
#else
        if (nsfb->palette == NULL)
                return 0;

			
		return nsfb_palette_best_match_dither(nsfb->palette,c);		
#endif
}

#define PLOT_TYPE uint8_t
#define PLOT_LINELEN(ll) (ll)

#include "common.c"

static bool fill(nsfb_t *nsfb, nsfb_bbox_t *rect, nsfb_colour_t c)
{
        int y;
        uint8_t ent;
        uint8_t *pvideo;

        if (!nsfb_plot_clip_ctx(nsfb, rect))
                return true; /* fill lies outside current clipping region */

        pvideo = get_xy_loc(nsfb, rect->x0, rect->y0);

        ent = colour_to_pixel(nsfb, c);

        for (y = rect->y0; y < rect->y1; y++) {
                memset(pvideo, ent, rect->x1 - rect->x0);
                pvideo += nsfb->linelen;
        }

        return true;
}

const nsfb_plotter_fns_t _nsfb_8bpp_plotters = {
        .line = line,
        .fill = fill,
        .point = point,
        .bitmap = bitmap,
		.bitmap_tiles = bitmap_tiles,
        .glyph8 = glyph8,
        .glyph1 = glyph1,
        .readrect = readrect,
};

/*
 * Local Variables:
 * c-basic-offset:8
 * End:
 */
