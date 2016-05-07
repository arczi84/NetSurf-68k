/*
 * Copyright 2014 Vincent Sanders <vince@netsurf-browser.org>
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
 * Interfaces for fetch table.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "utils/nsurl.h"
#include "utils/log.h"
#include "utils/filepath.h"
#include "utils/file.h"
#include "utils/nsoption.h"
#include "desktop/gui_window.h"
#include "desktop/gui_fetch.h"

#include "framebuffer/findfile.h"
#include "framebuffer/fetch.h"

#include <proto/dos.h>
#include <proto/locale.h>

#include "amigaos3/os3support.h"
#include "amiga/filetype.h"
#include "amiga/gui.h"

static char *current_user_dir;

bool ami_gui_map_filename(char **remapped, const char *path, const char *file,
	const char *map);
	
/**
 * Translate resource to full url.
 *
 * Transforms a resource: path into a full URL. The returned URL
 * is used as the target for a redirect. The caller takes ownership of
 * the returned nsurl including unrefing it when finished with it.
 *
 * \param path The path of the resource to locate.
 * \return A string containing the full URL of the target object or
 *         NULL if no suitable resource can be found.
 */

bool ami_gui_map_filename(char **remapped, const char *path, const char *file, const char *map)
{
	BPTR fh = 0;
	char *mapfile = NULL;
	size_t mapfile_size = 0;
	char buffer[1024];
	char *realfname;
	bool found = false;

	netsurf_mkpath(&mapfile, &mapfile_size, 2, path, map);

	if(mapfile == NULL) return false;

	fh = Open(mapfile, MODE_OLDFILE);
	if(fh)
	{
		while(FGets(fh, buffer, 1024) != 0)
		{
			if((buffer[0] == '#') ||
				(buffer[0] == '\n') ||
				(buffer[0] == '\0')) continue;

			realfname = strchr(buffer, ':');
			if(realfname)
			{
				if(strncmp(buffer, file, strlen(file)) == 0)
				{
					if(realfname[strlen(realfname)-1] == '\n')
						realfname[strlen(realfname)-1] = '\0';
					*remapped = strdup(realfname + 1);
					found = true;
					break;
				}
			}
		}
		Close(fh);
	}

	if(found == false) *remapped = strdup(file);
		else LOG("Remapped %s to %s in path %s using %s", file, *remapped, path, map);
		
	if (mapfile)
		free(mapfile);

	return found;
}

static bool ami_gui_check_resource(char *fullpath, const char *file)
{
	bool found = false;
	char *remapped = malloc(1024);
	BPTR lock = 0;
	size_t fullpath_len = 1024;

	ami_gui_map_filename(&remapped, fullpath, file, "Resource.map");
	netsurf_mkpath(&fullpath, &fullpath_len, 2, fullpath, remapped);

	LOG("Checking for %s\n", fullpath);
	
	lock = Lock(fullpath, ACCESS_READ);
	if(lock)
	{
		UnLock(lock);
		found = true;
	}

	if(found) 
		LOG("Found %s", fullpath);
	if (remapped)
		free(remapped);

	return found;
}
bool ami_locate_resource(char *fullpath, const char *file)
{
	struct Locale *locale;
	int i;
	bool found = false;
	char *remapped;
	size_t fullpath_len = 1024;

	/* Check NetSurf user data area first */

		strcpy(fullpath,"PROGDIR:Resources/");
		found = ami_gui_check_resource(fullpath, file);

		if(found) return true;

	/* If not found, start on the user's preferred languages */

	if (nsoption_bool(accept_lang_locale))
		{
	
		locale = OpenLocale(NULL);

		for(i=0;i<7;i++) {
			strcpy(fullpath,"PROGDIR:Resources/");

			if(locale->loc_PrefLanguages[i])
			{
				ami_gui_map_filename(&remapped, "PROGDIR:Resources/",
					locale->loc_PrefLanguages[i], "LangNames");
				netsurf_mkpath(&fullpath, &fullpath_len, 2, fullpath, remapped);

				found = ami_gui_check_resource(fullpath, file);
				nsoption_set_charp(accept_language, strdup(remapped)); 
			}else {
				continue;
			}

			if(found) break;
		}

		if(!found) {
			/* If not found yet, check in PROGDIR:Resources/en,
			 * might not be in user's preferred languages */

			strcpy(fullpath, "PROGDIR:Resources/en/");
			found = ami_gui_check_resource(fullpath, file);
		}

		CloseLocale(locale);
		}
	else
		{
			strcpy(fullpath, "PROGDIR:Resources/");
			strncat(fullpath, nsoption_charp(accept_language), 3);			
			strcat(fullpath, "/");			
			
			found = ami_gui_check_resource(fullpath, file);
		}

	if(!found) {
		/* Lastly check directly in PROGDIR:Resources */

		strcpy(fullpath,"PROGDIR:Resources/en/");
		found = ami_gui_check_resource(fullpath, file);

	}

	return found;
}

static nsurl *gui_get_resource_url(const char *path)
{
	char buf[1024];
	char path2[1024];
	nsurl *url = NULL;

	if(ami_locate_resource(buf, path) == false)
	{
		if((strncmp(path + strlen(path) - SLEN(".htm"), ".htm", SLEN(".htm")) == 0) ||
			(strncmp(path + strlen(path) - SLEN(".html"), ".html", SLEN(".html")) == 0))
		{
			/* Try with RISC OS HTML filetype, might work */
			strcpy(path2, path);
			strcat(path2, ",faf");

			if(ami_locate_resource(buf, path2) == false)
			{
				return NULL;
			}
		}
		else return NULL;
	}

	netsurf_path_to_nsurl(buf, &url);

	return url;
}

char CurDir[128];

static nsurl *gui_get_resource_url2(const char *path)
{
	char buf[PATH_MAX];
	nsurl *url = NULL;

	if (strcmp(path, "favicon.ico") == 0)
		path = "favicon.png";

	char *file = malloc(128);

	if (strlen(CurDir)==0)
				GetCurrentDirName(CurDir,128);

	strcpy(file,CurDir);		
	strcat(file,"/Resources/");

	strcat(file,path);

	netsurf_path_to_nsurl(file, &url);
	if (file)
		free(file);
	
	return url;
}


/**
 * filetype -- determine the MIME type of a local file
 */
const char *fetch_filetype(const char *unix_path)
{
	int l;
	LOG("unix path %s", unix_path);
	l = strlen(unix_path);
	if (2 < l && strcasecmp(unix_path + l - 3, "css") == 0)
		return "text/css";
	if (2 < l && strcasecmp(unix_path + l - 3, "f79") == 0)
		return "text/css";
	if (2 < l && strcasecmp(unix_path + l - 3, "jpg") == 0)
		return "image/jpeg";
	if (3 < l && strcasecmp(unix_path + l - 4, "jpeg") == 0)
		return "image/jpeg";
	if (2 < l && strcasecmp(unix_path + l - 3, "gif") == 0)
		return "image/gif";
	if (2 < l && strcasecmp(unix_path + l - 3, "png") == 0)
		return "image/png";
	if (2 < l && strcasecmp(unix_path + l - 3, "b60") == 0)
		return "image/png";
	if (2 < l && strcasecmp(unix_path + l - 3, "jng") == 0)
		return "image/jng";
	if (2 < l && strcasecmp(unix_path + l - 3, "svg") == 0)
		return "image/svg";
	return "text/html";
}

/* table for fetch operations */
static struct gui_fetch_table fetch_table = {
	.filetype = fetch_filetype,

	.get_resource_url = gui_get_resource_url,
};

struct gui_fetch_table *amiga_fetch_table = &fetch_table;
