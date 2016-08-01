/*
 * Copyright 2012 Michael Drake <tlsa@netsurf-browser.org>
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

/** \file
  * nsfb internal clipboard handling
  */

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "utils/log.h"
#include "netsurf/browser_window.h"
#include "netsurf/clipboard.h"

#include "framebuffer/gui.h"
#include "framebuffer/clipboard.h"

#include "exec/types.h"
#include "ami_clipboard.h"
#include "amigaos3/misc.h"

static struct gui_clipboard {
	char *buffer;
	size_t buffer_len;
	size_t length;
} gui_clipboard;

/**
 * Core asks front end for clipboard contents.
 *
 * \param  buffer  UTF-8 text, allocated by front end, ownership yeilded to core
 * \param  length  Byte length of UTF-8 text in buffer
 */
static void gui_get_clipboard(char **buffer, size_t *length)
{
	*buffer = NULL;
	*length = 0;

	if (strlen(ReadClip()) > 0) {
	
		*buffer = malloc(strlen(ReadClip()));
		gui_clipboard.buffer = strdup(ReadClip());
		
		if (ReadClip() != NULL) {
			memcpy(*buffer, ReadClip(),
					strlen(ReadClip()));
			*length = gui_clipboard.length = strlen(ReadClip());	
		}
	}
}


/**
 * Core tells front end to put given text in clipboard
 *
 * \param  buffer    UTF-8 text, owned by core
 * \param  length    Byte length of UTF-8 text in buffer
 * \param  styles    Array of styles given to text runs, owned by core, or NULL
 * \param  n_styles  Number of text run styles in array
 */
static void gui_set_clipboard(const char *buffer, size_t length,
		nsclipboard_styles styles[], int n_styles)
{
	WriteClip((char *)buffer);
}

static struct gui_clipboard_table clipboard_table = {
	.get = gui_get_clipboard,
	.set = gui_set_clipboard,
};

struct gui_clipboard_table *framebuffer_clipboard_table = &clipboard_table;
