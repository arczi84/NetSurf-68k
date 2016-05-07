/*
 * Copyright 2010 Chris Young <chris@unsatisfactorysoftware.co.uk>
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

#ifndef AMIGA_MISC_H
#define AMIGA_MISC_H
#include <exec/types.h>

extern struct gui_file_table *amiga_file_table;
struct Window;

void *ami_misc_allocvec_clear(int size, UBYTE value);

/* Itempool cross-compatibility */
APTR ami_misc_itempool_create(int size);
void ami_misc_itempool_delete(APTR pool);
APTR ami_misc_itempool_alloc(APTR pool, int size);
void ami_misc_itempool_free(APTR pool, APTR item, int size);

char *translate_escape_chars(const char *s);
void ami_misc_fatal_error(const char *message);
int32 ami_warn_user_multi(const char *body,
	const char *opt1, const char *opt2, struct Window *win);
#endif

