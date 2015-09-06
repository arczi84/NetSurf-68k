/*
 * Copyright 2012 Michael Drake <tlsa@netsurf-browser.org>
 *
 * This file is part of libnsfb, http://www.netsurf-browser.org/
 * Licenced under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 *
 * This is the *internal* interface for the cursor. 
 */


#if defined (RTG) || (AGA685)
#include "amigaos3/libnsfb/paletteRTG.h"
#else
#include "amigaos3/libnsfb/paletteAGA.h"
#endif

extern uint8_t table_for_cube_676[];

/** switch for RGB level dithering of palette 676 */
extern bool dither676;

/* global variable to toggle RGB level pushing; for a very primitive dithering effect */
extern int pushRGBlevel;
