void ftoa(float n, char *res, int afterpoint);

#include "content/content_protected.h"
#include "utils/messages.h"

//#define content_update_status content_update_status_org

static void my_content_update_status(struct content *c)
{
	if (c->status == CONTENT_STATUS_LOADING ||
			c->status == CONTENT_STATUS_READY) {
		/* Not done yet */
		snprintf(c->status_message, sizeof (c->status_message),
				"%s%s%s", messages_get("Fetching"),
				c->sub_status[0] != '\0' ? ", " : " ",
				c->sub_status);
	} else {
		unsigned int time = c->time;
		char res[5];
		ftoa((float)time/100,res,1);		
		unsigned short t1 = time/1000;
		//if t1<10 {
			//t1= t1*10}
		//t1 = t1;
		printf("***\n t1=%d \n***",t1);
		snprintf(c->status_message, sizeof (c->status_message),
				"%s (%u%ss)", messages_get("Done"),t1, res,time);
				//"%s (%dms)", messages_get("Done"), time);
	}
}
