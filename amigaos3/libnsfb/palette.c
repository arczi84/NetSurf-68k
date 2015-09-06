/*
 * Copyright 2012 Michael Drake <tlsa@netsurf-browser.org>
 *
 * This file is part of libnsfb, http://www.netsurf-browser.org/
 * Licenced under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 */

/** \file
 * Palette (implementation).
 */

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "palette.h"

uint8_t table_for_cube_676[834];

/** switch for RGB level dithering of palette 676 */
bool dither676;

/* global variable to toggle RGB level pushing; for a very primitive dithering effect */
int pushRGBlevel;

/** Create an empty palette object. */
#if defined (RTG) || (AGA685)
bool nsfb_palette_new(struct nsfb_palette_s **palette, int width)
{
	*palette = malloc(sizeof(struct nsfb_palette_s));
	if (*palette == NULL) {
		return false;
	}

	(*palette)->type = NSFB_PALETTE_EMPTY;
	(*palette)->last = 0;

	(*palette)->dither = false;
	(*palette)->dither_ctx.data_len = width * 3;
	(*palette)->dither_ctx.data = malloc(width * 3 * sizeof(int));
	if ((*palette)->dither_ctx.data == NULL) {
		nsfb_palette_free(*palette);
		return false;
	}

	return true;
}

/** Free a palette object. */
void nsfb_palette_free(struct nsfb_palette_s *palette)
{
	if (palette != NULL) {
		if (palette->dither_ctx.data != NULL) {
			free(palette->dither_ctx.data);
		}
		free(palette);
	}
}

/** Init error diffusion for a plot. */
void nsfb_palette_dither_init(struct nsfb_palette_s *palette, int width)
{
	palette->dither = true;
	memset(palette->dither_ctx.data, 0, palette->dither_ctx.data_len);
	palette->dither_ctx.width = width * 3;
	palette->dither_ctx.current = 0;
}

/** Finalise error diffusion after a plot. */
void nsfb_palette_dither_fini(struct nsfb_palette_s *palette)
{
	palette->dither = false;
}

/** Generate libnsfb 8bpp default palette. */
void nsfb_palette_generate_nsfb_8bpp(struct nsfb_palette_s *palette)
{
	int rloop, gloop, bloop;
	int loop = 0;
	uint8_t r, g, b;

	/* Build a linear 6-8-5 levels RGB colour cube palette.
	 * This accounts for 240 colours */
#define RLIM 6
#define GLIM 8
#define BLIM 5
	for (rloop = 0; rloop < RLIM; rloop++) {
		for (gloop = 0; gloop < GLIM; gloop++) {
			for (bloop = 0; bloop < BLIM; bloop++) {
				r = ((rloop * 255 * 2) + RLIM - 1) /
						(2 * (RLIM - 1));
				g = ((gloop * 255 * 2) + GLIM - 1) /
						(2 * (GLIM - 1));
				b = ((bloop * 255 * 2) + BLIM - 1) /
						(2 * (BLIM - 1));

				palette->data[loop] = r | g << 8 | b << 16;
				loop++;
			}
		}
	}
#undef RLIM
#undef GLIM
#undef BLIM

	/* Should have 240 colours set */
	assert(loop == 240);

	/* Fill index 240 to index 255 with grayscales */
	/* Note: already have full black and full white from RGB cube */
	for (; loop < 256; loop++) {
		int ngray = loop - 240 + 1;
		r = ngray * 15; /* 17*15 = 255 */

		g = b = r;

		palette->data[loop] = r | g << 8 | b << 16;
	}

	/* Set palette details */
	palette->type = NSFB_PALETTE_NSFB_8BPP;
	palette->last = 255;
}

	
#else
bool nsfb_palette_new(struct nsfb_palette_s **palette)
{
	*palette = malloc(sizeof(struct nsfb_palette_s));
	if (*palette == NULL) {
		return false;
	}

	(*palette)->type = NSFB_PALETTE_EMPTY;
	(*palette)->last = 0;

	dither676 = false;
	pushRGBlevel = 0;

	return true;
}

/** Generate cube 676 palette. */
void nsfb_palette_generate_cube_676(struct nsfb_palette_s *palette)
{
	int rloop, gloop, bloop;
	int loop = 4, n;
	uint8_t r, g, b;

	if (palette == NULL)
		return;
		
	if (palette->type == NSFB_PALETTE_CUBE_676)
		return; /* it's already done */

	/* Build a linear 6-7-6 RGB color cube palette.
	 * This accounts for 252 colors, color 4 - 255 */

	for (rloop = 0, r = 18; rloop < 6; rloop++) {
		for (gloop = 0, g = 16; gloop < 7; gloop++) {
			for (bloop = 0, b = 18; bloop < 6; bloop++) {
				palette->data[loop] = r | g << 8 | b << 16;
				loop++;
				b += 44;
			}
			g += 37;
		}
		r += 44;
	}
	
	palette->data[0] = 0x7F7F7F; /* 50 % gray */
	palette->data[1] = 0xB3B3B3; /* 70 % gray */
	palette->data[2] = 0xDCDCDC; /* 86 % gray */
	palette->data[3] = 0xFFFFFF; /* 100% white */
	palette->data[4] = 0x000000; /*  pure black */
	
		/* Set palette details */
	palette->type = NSFB_PALETTE_CUBE_676;
	palette->last = 255;

	for ( n = 0; n < 51; n++) {
		table_for_cube_676[    n] =   4;
		table_for_cube_676[227+n] =   9;
		table_for_cube_676[556+n] =   0;
		table_for_cube_676[783+n] = 210;
	}
	
	for ( n = 0; n < 44; n++) {
		table_for_cube_676[ 51+n] =   5;
		table_for_cube_676[ 95+n] =   6;
		table_for_cube_676[139+n] =   7;
		table_for_cube_676[183+n] =   8;
		table_for_cube_676[607+n] =  42;
		table_for_cube_676[651+n] =  84;
		table_for_cube_676[695+n] = 126;
		table_for_cube_676[739+n] = 168;
		table_for_cube_676[278+n] =   0;
	}
		
	for ( n = 0; n < 37; n++) {
		table_for_cube_676[322+n] =   6;
		table_for_cube_676[359+n] =  12;
		table_for_cube_676[396+n] =  18;
		table_for_cube_676[433+n] =  24;
		table_for_cube_676[470+n] =  30;
	}
	
	for ( n = 0; n < 49; n++)	
		table_for_cube_676[507+n] =  36;
}
#endif

