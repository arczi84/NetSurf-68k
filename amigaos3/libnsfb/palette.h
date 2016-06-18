/*
 * Copyright 2012 Michael Drake <tlsa@netsurf-browser.org>
 *
 * This file is part of libnsfb, http://www.netsurf-browser.org/
 * Licenced under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 *
 * This is the *internal* interface for the cursor. 
 */

#ifndef PALETTE_H
#define PALETTE_H 1

#include <stdint.h>
#include <limits.h>  // not necessary as should be included in stdint.h
#include <stdbool.h> // C99 bool does not exist without this include!
#include <string.h>  // defines NULL

#include "libnsfb.h"
#include "libnsfb_plot.h"

int Bpp;

bool dither_low_quality;

static uint8_t table_for_cube_676[];

/** switch for RGB level dithering of palette 676 */
extern bool dither676;

/* global variable to toggle RGB level pushing; for a very primitive dithering effect */
extern int pushRGBlevel;

enum nsfb_palette_type_e {
	NSFB_PALETTE_EMPTY,     /**< empty palette object */
	NSFB_PALETTE_NSFB_8BPP, /**< libnsfb's own 8bpp palette */
	NSFB_PALETTE_CUBE_676,   /**< palette with 6x7x6 + 3 colors */	
	NSFB_PALETTE_OTHER      /**< any other palette  */
};

struct nsfb_palette_s {
	enum nsfb_palette_type_e type; /**< Palette type */
	uint8_t last; /**< Last used palette index */
	nsfb_colour_t data[256]; /**< Palette for index modes */

		bool dither; /**< Whether to use error diffusion */
		struct {
			int width; /**< Length of error value buffer ring*/
			int current; /**< Current pos in ring buffer*/
			int *data; /**< Ring buffer error values */
			int data_len; /**< Max size of ring */
		} dither_ctx;
};


/** Create an empty palette object. */
bool nsfb_palette_new(struct nsfb_palette_s **palette, int width);

/** Free a palette object. */
void nsfb_palette_free(struct nsfb_palette_s *palette);

/** Init error diffusion for a plot. */
void nsfb_palette_dither_init(struct nsfb_palette_s *palette, int width);

/** Finalise error diffusion after a plot. */
void nsfb_palette_dither_fini(struct nsfb_palette_s *palette);

/** Generate libnsfb 8bpp default palette. */
void nsfb_palette_generate_nsfb_8bpp(struct nsfb_palette_s *palette);

/** Generate cube 676 palette. */
void nsfb_palette_generate_cube_676(struct nsfb_palette_s *palette);

static inline bool nsfb_palette_dithering_on(struct nsfb_palette_s *palette)
{
        return palette->dither;
}
/** Find best palette match for given colour. */
static inline uint8_t nsfb_palette_best_match(struct nsfb_palette_s *palette,
		nsfb_colour_t c, int *r_error, int *g_error, int *b_error)
{
	uint8_t best_col = 0;  // return variable
  
	uint_fast8_t col;
	nsfb_colour_t palent;  // uint32_t
	uint_fast32_t cr, cg, cb;  // color components of c
	int_fast32_t dr, dg, db; /* delta red, green, blue values */
	uint_fast32_t tmp_r, tmp_g, tmp_b;

	int_fast32_t cur_distance;
	int_fast32_t best_distance = INT_FAST32_MAX;

	uint_fast32_t tmp_c = (uint_fast32_t)c;
	cr = (tmp_c & 0xff);
	tmp_c >>= 8;
	cg = (tmp_c & 0xff);
	tmp_c >>= 8;
	cb = (tmp_c & 0xff);

	switch (palette->type) {
	case NSFB_PALETTE_NSFB_8BPP:
		/* Index into colour cube part */
		tmp_r = (cr * 5 + 128) / 256;
		tmp_g = (cg * 7 + 128) / 256;
		tmp_b = (cb * 4 + 128) / 256;
		col = 40 * tmp_r + 5 * tmp_g + tmp_b;

		palent = palette->data[col];
		uint_fast32_t tmp_palent = palent;
		dr = cr - (tmp_palent & 0xff);
		tmp_palent >>= 8;
		dg = cg - (tmp_palent & 0xff);
		tmp_palent >>= 8;
		db = cb - (tmp_palent & 0xff);
		cur_distance = (dr * dr) + (dg * dg) + (db * db);

		best_distance = cur_distance;
		best_col = col;
		*r_error = (int)dr;
		*g_error = (int)dg;
		*b_error = (int)db;

		/* Index into grayscale part */
		col = (cr + cg + cb + (45 / 2)) / (15 * 3) - 1 + 240;
		palent = palette->data[col];
		tmp_palent = palent;
		dr = cr - (tmp_palent & 0xff);
		tmp_palent >>= 8;
		dg = cg - (tmp_palent & 0xff);
		tmp_palent >>= 8;
		db = cb - (tmp_palent & 0xff);
		cur_distance = (dr * dr) + (dg * dg) + (db * db);
		if (cur_distance < best_distance) {
			// best_distance = cur_distance;
			best_col = col;
			*r_error = (int)dr;
			*g_error = (int)dg;
			*b_error = (int)db;
		}
		break;

	case NSFB_PALETTE_OTHER:
		/* Try all colours in palette */
		for (col = 0; col <= palette->last; col++) {
			palent = palette->data[col];
			uint_fast32_t tmp_palent = palent;
			dr = cr - (tmp_palent & 0xff);
			tmp_palent >>= 8;
			dg = cg - (tmp_palent & 0xff);
			tmp_palent >>= 8;
			db = cb - (tmp_palent & 0xff);
			cur_distance = (dr * dr) + (dg * dg) + (db * db);
			if (cur_distance < best_distance) {
				best_distance = cur_distance;
				best_col = col;
				*r_error = (int)dr;
				*g_error = (int)dg;
				*b_error = (int)db;
			}
		}
		break;

	default:
		break;
	}

        return best_col;
}

static inline uint8_t nsfb_palette_best_match_dither(
		struct nsfb_palette_s *palette, nsfb_colour_t c)
{

	uint8_t best_col_index;  // return variable

	uint_fast32_t cr, cg, cb;  // color components of c
	int_fast32_t r, g, b;
	int current;
	int error;
	int width = palette->dither_ctx.width;

	if (palette == NULL)
		return 0;

	if (palette->dither == false)
		return nsfb_palette_best_match(palette, c, &r, &g, &b);

	current = palette->dither_ctx.current;

	uint_fast32_t tmp_c = (uint_fast32_t)c;
	cr = (tmp_c & 0xff);
	tmp_c >>= 8;
	cg = (tmp_c & 0xff);
	tmp_c >>= 8;
	cb = (tmp_c & 0xff);

	/* Get RGB components of colour, and apply error */
	r = cr + palette->dither_ctx.data[current];
	palette->dither_ctx.data[current] = 0;
	current++;
	g = cg + palette->dither_ctx.data[current];
	palette->dither_ctx.data[current] = 0;
	current++;
	b = cb + palette->dither_ctx.data[current];
	palette->dither_ctx.data[current] = 0;
	current++;

	/* Clamp new RGB components to range */
	if (r <   0) r =   0;
	if (r > 255) r = 255;
	if (g <   0) g =   0;
	if (g > 255) g = 255;
	if (b <   0) b =   0;
	if (b > 255) b = 255;

	/* Rebuild colour from modified components */
	c = b;
	c <<= 8;
	c = c | g;
	c <<= 8;
	c = c | r;  // faster than c = (b << 16) | (g << 8) | r;

	/* Get best match for pixel, and find errors for each component */
	best_col_index = nsfb_palette_best_match(palette, c, &r, &g, &b);

	if (current >= width)
		current = 0;
	palette->dither_ctx.current = current;

	/* Save errors
	 *
	 *       [*]-[N]
	 *      / | \
	 *   [l]-[m]-[r]
	 */
	error = current;

	/* Error for [N] (next) */
	if (error != 0) {
		/* The pixel exists */
		palette->dither_ctx.data[error    ] += (int)((int_fast16_t)r * 7 / 16);
		palette->dither_ctx.data[error + 1] += (int)((int_fast16_t)g * 7 / 16);
		palette->dither_ctx.data[error + 2] += (int)((int_fast16_t)b * 7 / 16);
	}

	error += width - 2 * 3;
	if (error >= width)
		error -= width;
	/* Error for [l] (below, left) */
	if (error >= 0 && error != 3) {
		/* The pixel exists */
		palette->dither_ctx.data[error    ] += (int)((int_fast16_t)r * 3 / 16);
		palette->dither_ctx.data[error + 1] += (int)((int_fast16_t)g * 3 / 16);
		palette->dither_ctx.data[error + 2] += (int)((int_fast16_t)b * 3 / 16);
	}

	error += 3;
	if (error >= width)
		error -= width;
	/* Error for [m] (below, middle) */
	palette->dither_ctx.data[error    ] += (int)((int_fast16_t)r * 5 / 16);
	palette->dither_ctx.data[error + 1] += (int)((int_fast16_t)g * 5 / 16);
	palette->dither_ctx.data[error + 2] += (int)((int_fast16_t)b * 5 / 16);

	error += 3;
	if (error >= width)
		error -= width;
	/* Error for [r] (below, right) */
	if (error != 0) {
		/* The pixel exists */
		palette->dither_ctx.data[error    ] += (int)((int_fast16_t)r / 16);
		palette->dither_ctx.data[error + 1] += (int)((int_fast16_t)g / 16);
		palette->dither_ctx.data[error + 2] += (int)((int_fast16_t)b / 16);
	}

	return best_col_index;
}

/** Find best palette match for given colour. */
static inline uint8_t nsfb_palette_best_match_dither676(nsfb_colour_t c)
{
	int r, g, b;

	r = ( c        & 0xFF);
	g = ((c >>  8) & 0xFF);
	b = ((c >> 16) & 0xFF);

	if (g == b)
		if (g == r) /* this color is gray */
			if (g > 0x7A) 
				if (g < 0xE2) {
					if (g < 0x86)
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

	if (r > 250)
		if (g > 250)
			if (b > 250) return 3; /* 100 %  white */
				 
    return table_for_cube_676[r+556] + table_for_cube_676[g+278] + table_for_cube_676[b];
}


#endif /* PALETTE_H */
