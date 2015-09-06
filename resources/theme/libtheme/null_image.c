/*null_image*/

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <libnsfb.h>

#include "desktop/plot_style.h"
#include "framebuffer/gui.h"
#include "framebuffer/fbtk.h"

static uint8_t null_image_pixdata[] = {
	0xff, 0xff, 0xff, 0xff, 
};

struct fbtk_bitmap null_image = {
	.width		= 1,
	.height		= 1,
	.hot_x		= 0,
	.hot_y		= 0,
	.pixdata	= null_image_pixdata,
};
