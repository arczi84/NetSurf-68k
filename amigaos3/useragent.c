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

#ifndef NETSURF_UA_FORMAT_STRING
#define NETSURF_UA_FORMAT_STRING "NetSurf/%d.%d (%s; %s)"
#endif

/**
 * Prepare core_user_agent_string with a string suitable for use as a
 * user agent in HTTP requests.
 */
static void
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

        LOG(("Built user agent \"%s\"", core_user_agent_string));
}

/* This is a function so that later we can override it trivially */
const char *
user_agent_string(void)
{
        if (core_user_agent_string == NULL)
                user_agent_build_string();
	return core_user_agent_string;
}
