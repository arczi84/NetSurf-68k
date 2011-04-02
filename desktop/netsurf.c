/*
 * Copyright 2003 Phil Mellor <monkeyson@users.sourceforge.net>
 * Copyright 2007 James Bursa <bursa@users.sourceforge.net>
 * Copyright 2004 Andrew Timmins <atimmins@blueyonder.co.uk>
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

#include <locale.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <libxml/encoding.h>
#include <libxml/globals.h>
#include <libxml/xmlversion.h>

#include <libwapcaplet/libwapcaplet.h>

#include "utils/config.h"
#include "utils/utsname.h"
#include "content/fetch.h"
#include "content/hlcache.h"
#include "content/urldb.h"
#include "css/css.h"
#include "desktop/netsurf.h"
#include "desktop/401login.h"
#include "desktop/browser.h"
#include "desktop/gui.h"
#include "desktop/options.h"
#include "desktop/searchweb.h"
#include "utils/log.h"
#include "utils/url.h"
#include "utils/utf8.h"
#include "utils/utils.h"
#include "utils/messages.h"

bool netsurf_quit = false;
bool verbose_log = false;

static void netsurf_lwc_iterator(lwc_string *str, void *pw)
{
	LOG(("%.*s", (int) lwc_string_length(str), lwc_string_data(str)));
}

/**
 * Dispatch a low-level cache query to the frontend
 *
 * \param query  Query descriptor
 * \param pw     Private data
 * \param cb     Continuation callback
 * \param cbpw   Private data for continuation
 * \return NSERROR_OK
 */
static nserror netsurf_llcache_query_handler(const llcache_query *query,
		void *pw, llcache_query_response cb, void *cbpw)
{
	switch (query->type) {
	case LLCACHE_QUERY_AUTH:
		gui_401login_open(query->url, query->data.auth.realm, cb, cbpw);
		break;
	case LLCACHE_QUERY_REDIRECT:
		/** \todo Need redirect query dialog */
		/* For now, do nothing, as this query type isn't emitted yet */
		break;
	case LLCACHE_QUERY_SSL:
		gui_cert_verify(query->url, query->data.ssl.certs, 
				query->data.ssl.num, cb, cbpw);
		break;
	}

	return NSERROR_OK;
}

/**
 * Initialise components used by gui NetSurf.
 */

nserror netsurf_init(int *pargc, 
		     char ***pargv, 
		     const char *options, 
		     const char *messages)
{
	struct utsname utsname;
	nserror ret = NSERROR_OK;

#ifdef HAVE_SIGPIPE
	/* Ignore SIGPIPE - this is necessary as OpenSSL can generate these
	 * and the default action is to terminate the app. There's no easy
	 * way of determining the cause of the SIGPIPE (other than using
	 * sigaction() and some mechanism for getting the file descriptor
	 * out of libcurl). However, we expect nothing else to generate a
	 * SIGPIPE, anyway, so may as well just ignore them all. */
	
	signal(SIGPIPE, SIG_IGN);
#endif

#ifndef HAVE_STDOUT
	ret = nslog_init(nslog_ensure, pargc, *pargv);
#else
	ret = nslog_init(NULL, pargc, *pargv);
#endif

	if (ret != NSERROR_OK) 
		return ret;

#ifdef _MEMDEBUG_H_
	memdebug_memdebug("memdump");
#endif
	LOG(("version '%s'", netsurf_version));
	if (uname(&utsname) < 0)
		LOG(("Failed to extract machine information"));
	else
		LOG(("NetSurf on <%s>, node <%s>, release <%s>, version <%s>, "
				"machine <%s>", utsname.sysname,
				utsname.nodename, utsname.release,
				utsname.version, utsname.machine));

	LOG(("Using '%s' for Options file", options));
	options_read(options);

	messages_load(messages);

	url_init();

	setlocale(LC_ALL, "C");

	fetch_init();
	
	/* Initialise the hlcache and allow it to init the llcache for us */
	hlcache_initialise(netsurf_llcache_query_handler, NULL);

	/* Initialize system colours */
	gui_system_colour_init();

	options_commandline(pargc, *pargv);

	return ret;
}


/**
 * Gui NetSurf main loop.
 */
int netsurf_main_loop(void)
{
	while (!netsurf_quit) {
		gui_poll(fetch_active);
		hlcache_poll();
	}

	return 0;
}

/**
 * Clean up components used by gui NetSurf.
 */

void netsurf_exit(void)
{
	hlcache_stop();
	
	LOG(("Closing GUI"));
	gui_quit();

	/* Clean up after CSS */
	css_cleanup();

	LOG(("Closing search and related resources"));
	search_web_cleanup();

	LOG(("Finalising high-level cache"));
	hlcache_finalise();

	LOG(("Finalising low-level cache"));
	llcache_finalise();

	LOG(("Closing fetches"));
	fetch_quit();

	LOG(("Closing utf8"));
	utf8_finalise();

	LOG(("Destroying URLdb"));
	urldb_destroy();

	LOG(("Destroying System colours"));
	gui_system_colour_finalize();

	LOG(("Remaining lwc strings:"));
	lwc_iterate_strings(netsurf_lwc_iterator, NULL);

	LOG(("Exited successfully"));
}


