

#include "content/content_protected.h"
#include "content/hlcache.h"
#include "utils/messages.h"

#define content_update_status content_update_status_core

#include <proto/exec.h>
static void content_update_status(struct content *c)
{
	if (c->status == CONTENT_STATUS_LOADING ||
			c->status == CONTENT_STATUS_READY) {
		/* Not done yet */
		snprintf(c->status_message, sizeof (c->status_message),
				"%s%s%s", messages_get("Fetching"),
				c->sub_status[0] != '\0' ? ", " : " ",
				c->sub_status);
		Timeout = 0;
		SetTaskPri(FindTask(0), 10);
	} else {
		unsigned int time = c->time;
		snprintf(c->status_message, sizeof (c->status_message),
				"%s (%.1fs)", messages_get("Done"),
				(float) time / 100);
		Timeout = 1;
		SetTaskPri(FindTask(0), -1);
	}
}
