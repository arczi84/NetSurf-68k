/*
 * Copyright 2011 Chris Young <chris@unsatisfactorysoftware.co.uk>
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

//#include "testament.h"

/* NB: AmigaOS revision numbers start at 1 (not 0) and are monotonically
 * incremental (v1.20 is higher than v1.3 and not the same as v1.2).
 * Consequently, this version pair may not match the user-facing one in
 * desktop/version.c.
 */
 
#include "version.h"

#ifdef RTG
#define GFX
#else
#define GFX AGA
#endif

const char version[] = "\0$VER:NetSurf" NETSURF_VERSION_MAJOR "." NETSURF_VERSION_MINOR "Dev (" __DATE__ ") port by Artur Jarosik  (c)2009-2016 \0";
