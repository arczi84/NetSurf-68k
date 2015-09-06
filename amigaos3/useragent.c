/*
 * Copyright 2007 Daniel Silverstone <dsilvers@digital-scurf.org>
 * Copyright 2007 Rob Kendrick <rjek@netsurf-browser.org>
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

#include <stdio.h>
#include <stdlib.h>

#include "utils/config.h"
#include "utils/utsname.h"
#include "desktop/netsurf.h"
#include "utils/log.h"
#include "utils/useragent.h"

static const int netsurf_version_major = 3;
static const int netsurf_version_minor = 4;

static const char *core_user_agent_string = NULL;
static void user_agent_build_string_core(void);
static const char * user_agent_string_core(void);
void user_agent_build_string(void);

#undef NETSURF_UA_FORMAT_STRING
#define NETSURF_UA_FORMAT_STRING "NetSurf/%d.%d (%s; %s)"

/**
 * Prepare core_user_agent_string with a string suitable for use as a
 * user agent in HTTP requests.
 */
#define user_agent_build_string user_agent_build_string_core
#define user_agent_string user_agent_string_core

void
user_agent_build_string(void)
{
        struct utsname un;
        const char *sysname = "Unknown";
        const char *machine = "Unknown";
        char *ua_string;
        int len;

        if (uname(&un) >= 0) {
                sysname = un.sysname;
                machine = un.machine;
        }

        len = snprintf(NULL, 0, NETSURF_UA_FORMAT_STRING,
                       netsurf_version_major,
                       netsurf_version_minor,
                       sysname,
                       machine);
        ua_string = malloc(len + 1);
        if (!ua_string) {
                /** \todo this needs handling better */
                return;
        }
        snprintf(ua_string, len + 1,
                 NETSURF_UA_FORMAT_STRING,
                 netsurf_version_major,
                 netsurf_version_minor,
                 sysname,
                 machine);

        core_user_agent_string = ua_string;

        LOG("Built user agent \"%s\"", core_user_agent_string);
}

/* This is a function so that later we can override it trivially */
const char *
user_agent_string(void)
{
       // if (core_user_agent_string == NULL)
                user_agent_build_string();
	return core_user_agent_string;
}