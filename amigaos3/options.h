/*
 * Copyright 2008, 2010 Daniel Silverstone <dsilvers@netsurf-browser.org>
 *
 * This file is part of NetSurf, http://www.netsurf-browser.org/
 *
 * NetSurf is free software, NULL) you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, NULL) version 2 of the License.
 *
 * NetSurf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY, NULL) without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef _NETSURF_FRAMEBUFFER_OPTIONS_H_
#define _NETSURF_FRAMEBUFFER_OPTIONS_H_
#endif
/***** surface options *****/

#include "utils/nsoption.h"

/***** Amiga options *****/

NSOPTION_INTEGER(scale_aga, 74)
NSOPTION_INTEGER(browser_dpi, 84)
NSOPTION_INTEGER(window_depth, 0)
NSOPTION_INTEGER(mobile_mode, -1)
NSOPTION_INTEGER(priority, 1)
NSOPTION_INTEGER(gui_font_size, 12)

NSOPTION_BOOL(warp_mode, false)
NSOPTION_BOOL(fullscreen, false)
NSOPTION_BOOL(bitmap_fonts, false)
NSOPTION_BOOL(autodetect_depth, true)
NSOPTION_BOOL(module_autoplay, false)
NSOPTION_BOOL(youtube_autoplay, false)
NSOPTION_BOOL(accept_lang_locale, true)

NSOPTION_STRING(youtube_handler, NULL)
NSOPTION_STRING(screen_modeid, NULL)
NSOPTION_STRING(download_manager, NULL)
NSOPTION_STRING(download_path, NULL)
NSOPTION_STRING(def_search_bar, NULL)
NSOPTION_STRING(text_editor, NULL)
NSOPTION_STRING(net_player, NULL)
NSOPTION_STRING(module_player, NULL)
NSOPTION_STRING(theme, NULL)
NSOPTION_STRING(cache_dir, "PROGDIR:Resources/Cache")
NSOPTION_STRING(favicon_source, NULL)

/***** toolkit options *****/

/** toolkit furniture size */
NSOPTION_INTEGER(fb_furniture_size, 18)
/** toolbar furniture size */
NSOPTION_INTEGER(fb_toolbar_size, 58)
/** toolbar layout */
#ifdef RTG
NSOPTION_STRING(fb_toolbar_layout, NULL)
#else
NSOPTION_STRING(fb_toolbar_layout, "blfrhcuvaqetk12345gdyop")
#endif

/***** font options *****/

/** size of font glyph cache in kilobytes. */
NSOPTION_INTEGER(fb_font_cachesize, 2048)

NSOPTION_STRING(fb_face_sans_serif, NULL) /**< default sans face */	
NSOPTION_STRING(fb_face_sans_serif_bold, NULL) /**< bold sans face */	
NSOPTION_STRING(fb_face_sans_serif_italic, NULL) /**< bold sans face */	
NSOPTION_STRING(fb_face_sans_serif_italic_bold, NULL) /**< bold sans face */
NSOPTION_STRING(fb_face_serif, NULL) /**< serif face */			
NSOPTION_STRING(fb_face_serif_bold, NULL) /**< bold serif face */	
NSOPTION_STRING(fb_face_monospace, NULL) /**< monospace face */		
NSOPTION_STRING(fb_face_monospace_bold, NULL) /**< bold monospace face */
NSOPTION_STRING(fb_face_cursive, NULL) /**< cursive face */		
NSOPTION_STRING(fb_face_fantasy, NULL) /**< fantasy face */	

/***** favourite options *****/

NSOPTION_STRING(favourite_1_url, NULL)
NSOPTION_STRING(favourite_2_url, NULL)
NSOPTION_STRING(favourite_3_url, NULL)
NSOPTION_STRING(favourite_4_url, NULL)
NSOPTION_STRING(favourite_5_url, NULL)
NSOPTION_STRING(favourite_6_url, NULL)
NSOPTION_STRING(favourite_7_url, NULL)
NSOPTION_STRING(favourite_8_url, NULL)
NSOPTION_STRING(favourite_9_url, NULL)
NSOPTION_STRING(favourite_10_url, NULL)
NSOPTION_STRING(favourite_11_url, NULL)
NSOPTION_STRING(favourite_12_url, NULL)
NSOPTION_STRING(favourite_1_label, NULL)
NSOPTION_STRING(favourite_2_label, NULL)
NSOPTION_STRING(favourite_3_label, NULL)
NSOPTION_STRING(favourite_4_label, NULL)
NSOPTION_STRING(favourite_5_label, NULL)
NSOPTION_STRING(favourite_6_label, NULL)
NSOPTION_STRING(favourite_7_label, NULL)
NSOPTION_STRING(favourite_8_label, NULL)
NSOPTION_STRING(favourite_9_label, NULL)
NSOPTION_STRING(favourite_10_label, NULL)
NSOPTION_STRING(favourite_11_label, NULL)
NSOPTION_STRING(favourite_12_label, NULL)


/*
 * Local Variables:
 * c-basic-offset:8
 * End:
 */
