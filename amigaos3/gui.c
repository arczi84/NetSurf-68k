/*
 * Copyright 2008, 2014 Vincent Sanders <vince@netsurf-browser.org>
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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <limits.h>
#include <unistd.h>
#include <getopt.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include <sdl/sdl.h>
#include <libnsfb.h>
#include <libnsfb_plot.h>
#include <libnsfb_event.h>
#include "palette.h"

#include "desktop/browser_history.h"
#include "desktop/browser_private.h"
#include "desktop/gui_utf8.h"
#include "desktop/mouse.h"
#include "desktop/plotters.h"
#include "desktop/netsurf.h"
#include "desktop/textinput.h"
#include "desktop/download.h"
#include "desktop/gui_download.h"
#include "desktop/gui_window.h"
#include "desktop/gui_misc.h"

#include "utils/nsoption.h"
#include "utils/filepath.h"
#include "utils/log.h"
#include "utils/messages.h"
#include "utils/utils.h"

#include "framebuffer/gui.h"
#include "framebuffer/fbtk.h"
#include "framebuffer/framebuffer.h"
#include "framebuffer/schedule.h"
#include "framebuffer/findfile.h"
#include "framebuffer/image_data.h"
#include "framebuffer/font.h"
#ifdef USE_OLD_FETCH	
#include "framebuffer/fetch.h"
#endif
#include "framebuffer/clipboard.h"
#include "framebuffer/bitmap.h"

#include "content/urldb.h"
#include "content/fetch.h"
#include "content/fetchers.h"
#include "content/hlcache.h"
#include "content/backing_store.h"

/* NetSurf Amiga platform includes */
#include "amiga/utf8.h"
#include "amiga/misc.h"
#include "amiga/filetype.h"
#include "amiga/fs_backing_store.h"
#include "amigaos3/fetch.h"
#include "amigaos3/gui.h"
#include "amigaos3/misc.h"

#define   BUTTON_SIZE   24

#ifdef RTG
#	define NSFB_TOOLBAR_DEFAULT_LAYOUT "blfrhuvaqetk123456789xwzgdnmyop"
#else
#	define NSFB_TOOLBAR_DEFAULT_LAYOUT "blfrhcuvaqetk12345gdyop"
#endif

#include <exec/exec.h>
#include <libraries/asl.h>
#include <proto/asl.h>
#include <proto/openurl.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <dos/dostags.h>
#include <proto/intuition.h>
//#include <sys/time.h>
#include <clib/timer_protos.h>
#include <clib/exec_protos.h>

#if defined AGA || __libnix__

struct Device* TimerBase;
static struct IORequest timereq;

#endif

struct Library *AslBase;
struct Library *OpenURLBase;
struct Library *LocaleBase;
#if defined AGA || NOVA_SDL 
struct Library * KeymapBase;
struct Library * CxBase;
#endif
struct Screen *Workbench;

fbtk_widget_t *fbtk;

static bool fb_complete = false;
struct gui_window *input_window = NULL;
struct gui_window *search_current_window;
struct gui_window *window_list = NULL;

extern struct gui_utf8_table *amiga_utf8_table;

int scale;
/**
 * Cause an abnormal program termination.
 *
 * \note This never returns and is intended to terminate without any cleanup.
 *
 * \param error The message to display to the user.
 */
static void die(const char *error)
{
	fprintf(stderr, "%s\n", error);

	exit(1);
}

/* private data for browser user widget */
struct browser_widget_s {
	struct browser_window *bw; /**< The browser window connected to this gui window */
	int scrollx, scrolly; /**< scroll offsets. */

	/* Pending window redraw state. */
	bool redraw_required; /**< flag indicating the foreground loop
			       * needs to redraw the browser widget.
			       */
	bbox_t redraw_box; /**< Area requiring redraw. */
	bool pan_required; /**< flag indicating the foreground loop
			    * needs to pan the window.
			    */
	int panx, pany; /**< Panning required. */
};

static struct gui_drag {
	enum state {
		GUI_DRAG_NONE,
		GUI_DRAG_PRESSED,
		GUI_DRAG_DRAG
	} state;
	int button;
	int x;
	int y;
	bool grabbed_pointer;
} gui_drag;


/* queue a redraw operation, co-ordinates are relative to the window */
static void
fb_queue_redraw(struct fbtk_widget_s *widget, int x0, int y0, int x1, int y1)
{
	struct browser_widget_s *bwidget = fbtk_get_userpw(widget);

	bwidget->redraw_box.x0 = min(bwidget->redraw_box.x0, x0);
	bwidget->redraw_box.y0 = min(bwidget->redraw_box.y0, y0);
	bwidget->redraw_box.x1 = max(bwidget->redraw_box.x1, x1);
	bwidget->redraw_box.y1 = max(bwidget->redraw_box.y1, y1);

	if (fbtk_clip_to_widget(widget, &bwidget->redraw_box)) {
		bwidget->redraw_required = true;
		fbtk_request_redraw(widget);
	} else {
		bwidget->redraw_box.y0 = bwidget->redraw_box.x0 = INT_MAX;
		bwidget->redraw_box.y1 = bwidget->redraw_box.x1 = -(INT_MAX);
		bwidget->redraw_required = false;
	}
}

/* queue a window scroll */
static void
widget_scroll_y(struct gui_window *gw, int y, bool abs)
{
	struct browser_widget_s *bwidget = fbtk_get_userpw(gw->browser);
	int content_width, content_height;
	int height;


	LOG("window scroll");
	if (abs)
		bwidget->pany = y - bwidget->scrolly;
	 else
		bwidget->pany += y;


	browser_window_get_extents(gw->bw, true,
			&content_width, &content_height);

	height = fbtk_get_height(gw->browser);

	/* dont pan off the top */
	if ((bwidget->scrolly + bwidget->pany) < 0)
		bwidget->pany = -bwidget->scrolly;

	/* do not pan off the bottom of the content */
	if ((bwidget->scrolly + bwidget->pany) > (content_height - height))
		bwidget->pany = (content_height - height) - bwidget->scrolly;

	if (bwidget->pany == 0)
		return;

	bwidget->pan_required = true;

	fbtk_request_redraw(gw->browser);

	fbtk_set_scroll_position(gw->vscroll, bwidget->scrolly + bwidget->pany);
}

/* queue a window scroll */
static void
widget_scroll_x(struct gui_window *gw, int x, bool abs)
{
	struct browser_widget_s *bwidget = fbtk_get_userpw(gw->browser);
	int content_width, content_height;
	int width;


	if (abs) {
		bwidget->panx = x - bwidget->scrollx;
	} else {
		bwidget->panx += x;
	}

	browser_window_get_extents(gw->bw, true,
			&content_width, &content_height);


	width = fbtk_get_width(gw->browser);

	/* dont pan off the left */
	if ((bwidget->scrollx + bwidget->panx) < 0)
		bwidget->panx = - bwidget->scrollx;

	/* do not pan off the right of the content */
	if ((bwidget->scrollx + bwidget->panx) > (content_width - width))
		bwidget->panx = (content_width - width) - bwidget->scrollx;

	if (bwidget->panx == 0)
		return;

	bwidget->pan_required = true;

	fbtk_request_redraw(gw->browser);

	fbtk_set_scroll_position(gw->hscroll, bwidget->scrollx + bwidget->panx);
}

static void
fb_pan(fbtk_widget_t *widget,
       struct browser_widget_s *bwidget,
       struct browser_window *bw)
{
	int x;
	int y;
	int width;
	int height;
	nsfb_bbox_t srcbox;
	nsfb_bbox_t dstbox;

	nsfb_t *nsfb = fbtk_get_nsfb(widget);

	height = fbtk_get_height(widget);
	width = fbtk_get_width(widget);

	LOG("panning %d, %d", bwidget->panx, bwidget->pany);

	x = fbtk_get_absx(widget);
	y = fbtk_get_absy(widget);

	/* if the pan exceeds the viewport size just redraw the whole area */
	if (bwidget->pany >= height || bwidget->pany <= -height ||
	    bwidget->panx >= width || bwidget->panx <= -width) {

		bwidget->scrolly += bwidget->pany;
		bwidget->scrollx += bwidget->panx;
		fb_queue_redraw(widget, 0, 0, width, height);

		/* ensure we don't try to scroll again */
		bwidget->panx = 0;
		bwidget->pany = 0;
		bwidget->pan_required = false;
		return;
	}

	if (bwidget->pany < 0) {
		/* pan up by less then viewport height */
		srcbox.x0 = x;
		srcbox.y0 = y;
		srcbox.x1 = srcbox.x0 + width;
		srcbox.y1 = srcbox.y0 + height + bwidget->pany;

		dstbox.x0 = x;
		dstbox.y0 = y - bwidget->pany;
		dstbox.x1 = dstbox.x0 + width;
		dstbox.y1 = dstbox.y0 + height + bwidget->pany;

		/* move part that remains visible up */
		nsfb_plot_copy(nsfb, &srcbox, nsfb, &dstbox);

		/* redraw newly exposed area */
		bwidget->scrolly += bwidget->pany;
		fb_queue_redraw(widget, 0, 0, width, - bwidget->pany);


	} else if (bwidget->pany > 0) {
		/* pan down by less then viewport height */
		srcbox.x0 = x;
		srcbox.y0 = y + bwidget->pany;
		srcbox.x1 = srcbox.x0 + width;
		srcbox.y1 = srcbox.y0 + height - bwidget->pany;

		dstbox.x0 = x;
		dstbox.y0 = y;
		dstbox.x1 = dstbox.x0 + width;
		dstbox.y1 = dstbox.y0 + height - bwidget->pany;

		/* move part that remains visible down */
		nsfb_plot_copy(nsfb, &srcbox, nsfb, &dstbox);

		/* redraw newly exposed area */
		bwidget->scrolly += bwidget->pany;
		fb_queue_redraw(widget, 0, height - bwidget->pany,
				width, height);
	}

	if (bwidget->panx < 0) {
		/* pan left by less then viewport width */
		srcbox.x0 = x;
		srcbox.y0 = y;
		srcbox.x1 = srcbox.x0 + width + bwidget->panx;
		srcbox.y1 = srcbox.y0 + height;

		dstbox.x0 = x - bwidget->panx;
		dstbox.y0 = y;
		dstbox.x1 = dstbox.x0 + width + bwidget->panx;
		dstbox.y1 = dstbox.y0 + height;

		/* move part that remains visible left */
		nsfb_plot_copy(nsfb, &srcbox, nsfb, &dstbox);

		/* redraw newly exposed area */
		bwidget->scrollx += bwidget->panx;
		fb_queue_redraw(widget, 0, 0, -bwidget->panx, height);
	

	} else if (bwidget->panx > 0) {
		/* pan right by less then viewport width */
		srcbox.x0 = x + bwidget->panx;
		srcbox.y0 = y;
		srcbox.x1 = srcbox.x0 + width - bwidget->panx;
		srcbox.y1 = srcbox.y0 + height;

		dstbox.x0 = x;
		dstbox.y0 = y;
		dstbox.x1 = dstbox.x0 + width - bwidget->panx;
		dstbox.y1 = dstbox.y0 + height;

		/* move part that remains visible right */
		nsfb_plot_copy(nsfb, &srcbox, nsfb, &dstbox);

		/* redraw newly exposed area */
		bwidget->scrollx += bwidget->panx;
		fb_queue_redraw(widget, width - bwidget->panx, 0,
				width, height);
	}

	bwidget->pan_required = false;
	bwidget->panx = 0;
	bwidget->pany = 0;
}

static void
fb_redraw(fbtk_widget_t *widget,
	  struct browser_widget_s *bwidget,
	  struct browser_window *bw)
{
	int x;
	int y;
	int caret_x, caret_y, caret_h;
	struct rect clip;
	struct redraw_context ctx = {
		.interactive = true,
		.background_images = true,
		.plot = &fb_plotters
	};
	nsfb_t *nsfb = fbtk_get_nsfb(widget);
	float scale = browser_window_get_scale(bw);
	
	LOG("%d,%d to %d,%d",
	     bwidget->redraw_box.x0,
	     bwidget->redraw_box.y0,
	     bwidget->redraw_box.x1,
	     bwidget->redraw_box.y1);

	x = fbtk_get_absx(widget);
	y = fbtk_get_absy(widget);

	/* adjust clipping co-ordinates according to window location */
	bwidget->redraw_box.y0 += y;
	bwidget->redraw_box.y1 += y;
	bwidget->redraw_box.x0 += x;
	bwidget->redraw_box.x1 += x;

	nsfb_claim(nsfb, &bwidget->redraw_box);

	/* redraw bounding box is relative to window */
	clip.x0 = bwidget->redraw_box.x0;
	clip.y0 = bwidget->redraw_box.y0;
	clip.x1 = bwidget->redraw_box.x1;
	clip.y1 = bwidget->redraw_box.y1;

	browser_window_redraw(bw,
			(x - bwidget->scrollx) / scale,
			(y - bwidget->scrolly) / scale,
			&clip, &ctx);

	if (fbtk_get_caret(widget, &caret_x, &caret_y, &caret_h)) {
		/* This widget has caret, so render it */
		nsfb_bbox_t line;
		nsfb_plot_pen_t pen;

		line.x0 = x - bwidget->scrollx + caret_x;
		line.y0 = y - bwidget->scrolly + caret_y;
		line.x1 = x - bwidget->scrollx + caret_x;
		line.y1 = y - bwidget->scrolly + caret_y + caret_h;

		pen.stroke_type = NFSB_PLOT_OPTYPE_SOLID;
		pen.stroke_width = 1;
		pen.stroke_colour = 0xFF0000FF;

		nsfb_plot_line(nsfb, &line, &pen);
	}
	nsfb_update(fbtk_get_nsfb(widget), &bwidget->redraw_box);

	bwidget->redraw_box.y0 = bwidget->redraw_box.x0 = INT_MAX;
	bwidget->redraw_box.y1 = bwidget->redraw_box.x1 = INT_MIN;
	bwidget->redraw_required = false;
}

static int
fb_browser_window_redraw(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct gui_window *gw = cbi->context;
	struct browser_widget_s *bwidget;

	bwidget = fbtk_get_userpw(widget);
	if (bwidget == NULL) {
		LOG("browser widget from widget %p was null", widget);
		return -1;
	}

	if (bwidget->pan_required) {
		fb_pan(widget, bwidget, gw->bw);
	}

	if (bwidget->redraw_required) {
		fb_redraw(widget, bwidget, gw->bw);
	} else {
		bwidget->redraw_box.x0 = 0;
		bwidget->redraw_box.y0 = 0;
		bwidget->redraw_box.x1 = fbtk_get_width(widget);
		bwidget->redraw_box.y1 = fbtk_get_height(widget);
		fb_redraw(widget, bwidget, gw->bw);
	}
	return 0;
}

static int fb_browser_window_destroy(fbtk_widget_t *widget,
		fbtk_callback_info *cbi)
{
	struct browser_widget_s *browser_widget;

	if (widget == NULL) {
		return 0;
	}

	/* Free private data */
	browser_widget = fbtk_get_userpw(widget);
	free(browser_widget);

	return 0;
}

char *options;
static const char *fename;
static int febpp;
static int fewidth;
static int feheight;
static const char *feurl;

static bool
process_cmdline(int argc, char** argv)
{
	int opt;

	LOG("argc %d, argv %p", argc, argv);
	
	fename = "sdl";
	
#ifdef RTG
	if (nsoption_int(window_depth) == 0) { nsoption_int(window_depth) = 32;
		nsoption_write(options, NULL, NULL);}

	fewidth = nsoption_int(window_width);
	feheight = nsoption_int(window_height);
	febpp =  nsoption_int(window_depth);
#else	
	nsoption_int(window_depth) = 8;
	febpp = 8;
	fewidth = 640;
	feheight = 512;
#endif
	
	if ((nsoption_charp(homepage_url) != NULL) && 
	    (nsoption_charp(homepage_url)[0] != '\0')) {
		if (nsoption_charp(lastpage_url) == NULL)
			feurl = nsoption_charp(homepage_url); 
		else {
			feurl = nsoption_charp(lastpage_url);
			nsoption_charp(lastpage_url) = NULL;
			nsoption_write(options, NULL, NULL);
			}	
	}		

	while((opt = getopt(argc, argv, "f:b:w:h:")) != -1) {
		switch (opt) {
		case 'f':
			fename = optarg;
			break;

		case 'b':
			febpp = atoi(optarg);
			break;

		case 'w':
			fewidth = atoi(optarg);
			break;

		case 'h':
			feheight = atoi(optarg);
			break;

		default:
			fprintf(stderr,
				"Usage: %s [-f frontend] [-b bpp] url\n",
				argv[0]);
			return false;
		}
	}

	if (optind < argc) {
		feurl = argv[optind];
	}

	return true;
}

void redraw_gui(void)
{
	fbtk_request_redraw(toolbar);
	fbtk_request_redraw(url);
	if (label1)
		fbtk_request_redraw(label1);
	if (label2)
		fbtk_request_redraw(label2);
	if (label3)
		fbtk_request_redraw(label3);
	if (label4)
		fbtk_request_redraw(label4);
	if (label5)
		fbtk_request_redraw(label5);
	if (label6)
		fbtk_request_redraw(label6);
	if (label7)
		fbtk_request_redraw(label7);
	if (label8)
		fbtk_request_redraw(label8);
	if (label9)
		fbtk_request_redraw(label9);
	if (label10)		
		fbtk_request_redraw(label10);
	if (label11)		
		fbtk_request_redraw(label11);
	if (label12)		
		fbtk_request_redraw(label12);

	gui_window_redraw_window(g2);
}
 
/**
 * Set option defaults for framebuffer frontend
 *
 * @param defaults The option table to update.
 * @return error status.
 */
static nserror set_defaults(struct nsoption_s *defaults)
{
	/* Set defaults for absent option strings */
	nsoption_setnull_charp(cookie_file, strdup("PROGDIR:Resources/Cookies"));
	nsoption_setnull_charp(cookie_jar, strdup("PROGDIR:Resources/Cookies"));
#ifdef RTG	
	//nsoption_int(scale) = 100;
	nsoption_setnull_charp(fb_toolbar_layout, strdup("blfsrhuvaqetk123456789xwzgdmyop"));	
#else
	scale = nsoption_int(scale);
	nsoption_int(scale) = nsoption_int(scale_aga);	
	nsoption_setnull_charp(fb_toolbar_layout, strdup("blfrcuvaqetk12345gdyop"));
	nsoption_int(window_depth) = 8;
	nsoption_int(fullscreen) = 1;
#endif
#ifdef JS
	nsoption_bool(enable_javascript) = true;
#endif

	nsoption_setnull_charp(net_player, strdup("C:ffplay"));
	nsoption_setnull_charp(download_path, strdup("PROGDIR:Downloads"));
	nsoption_setnull_charp(homepage_url, strdup("www.netsurf-browser.org/welcome"));
	nsoption_setnull_charp(theme, strdup("default"));
	nsoption_setnull_charp(text_editor, strdup("C:ed"));
	nsoption_setnull_charp(download_manager, strdup("wallget"));
	nsoption_setnull_charp(favicon_source,strdup("http://www.google.com/s2/favicons?domain=")); 
	
	if (nsoption_charp(cookie_file) == NULL ||
	    nsoption_charp(cookie_jar) == NULL) {
		LOG("Failed initialising cookie options");
		return NSERROR_BAD_PARAMETER;
	}

		/* set system colours for framebuffer ui */
	nsoption_set_colour(sys_colour_ActiveBorder, 0x00000000);
	nsoption_set_colour(sys_colour_ActiveCaption, 0x00ddddcc);
	nsoption_set_colour(sys_colour_AppWorkspace, 0x00eeeeee);
	nsoption_set_colour(sys_colour_Background, 0x00aa0000);
	nsoption_set_colour(sys_colour_ButtonFace, 0x00dddddd);
	nsoption_set_colour(sys_colour_ButtonHighlight, 0x00cccccc);
	nsoption_set_colour(sys_colour_ButtonShadow, 0x00bbbbbb);
	nsoption_set_colour(sys_colour_ButtonText, 0x00000000);
	nsoption_set_colour(sys_colour_CaptionText, 0x00000000);
	nsoption_set_colour(sys_colour_GrayText, 0x00777777);
	nsoption_set_colour(sys_colour_Highlight, 0x00ee0000);
	nsoption_set_colour(sys_colour_HighlightText, 0x00000000);
	nsoption_set_colour(sys_colour_InactiveBorder, 0x00000000);
	nsoption_set_colour(sys_colour_InactiveCaption, 0x00ffffff);
	nsoption_set_colour(sys_colour_InactiveCaptionText, 0x00cccccc);
	nsoption_set_colour(sys_colour_InfoBackground, 0x00aaaaaa);
	nsoption_set_colour(sys_colour_InfoText, 0x00000000);
	nsoption_set_colour(sys_colour_Menu, 0x00aaaaaa);
	nsoption_set_colour(sys_colour_MenuText, 0x00000000);
	nsoption_set_colour(sys_colour_Scrollbar, 0x00aaaaaa);
	nsoption_set_colour(sys_colour_ThreeDDarkShadow, 0x00555555);
	nsoption_set_colour(sys_colour_ThreeDFace, 0x00dddddd);
	nsoption_set_colour(sys_colour_ThreeDHighlight, 0x00aaaaaa);
	nsoption_set_colour(sys_colour_ThreeDLightShadow, 0x00999999);
	nsoption_set_colour(sys_colour_ThreeDShadow, 0x00777777);
	nsoption_set_colour(sys_colour_Window, 0x00aaaaaa);
	nsoption_set_colour(sys_colour_WindowFrame, 0x00cfcfcf);
	nsoption_set_colour(sys_colour_WindowText, 0x00000000);
	
	return NSERROR_OK;
}

/**
 * Ensures output logging stream is correctly configured
 */
static bool nslog_stream_configure(FILE *fptr)
{
        /* set log stream to be non-buffering */
	setbuf(fptr, NULL);

	return true;
}

static void framebuffer_run(void)
{
	nsfb_event_t event;
	int timeout; /* timeout in miliseconds */

	while (fb_complete != true) {
		/* run the scheduler and discover how long to wait for
		 * the next event.
		 */
		timeout = schedule_run();	
		/* if redraws are pending do not wait for event,
		 * return immediately
		 */
#if defined  AGA || __clib2__

	if (nsoption_bool(warp_mode))	
			timeout = 0;	
	else
			timeout = TimeOut;

#ifdef NO_TIMER
		timeout = 0;
#endif	

#else
		if (fbtk_get_redraw_pending(fbtk))
			timeout = 0;
	
#endif		
		
		if (fbtk_event(fbtk, &event, timeout)) {
			if ((event.type == NSFB_EVENT_CONTROL) &&
			    (event.value.controlcode ==  NSFB_CONTROL_QUIT))

				fb_complete = true;
		}

		fbtk_redraw(fbtk);
	}
}

static void gui_quit(void)
{
	LOG("gui_quit");
	nsoption_int(scale) = scale;
	urldb_save_cookies(nsoption_charp(cookie_jar));
	
	framebuffer_finalise();
}

/* called back when click in browser window */
static int
fb_browser_window_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct gui_window *gw = cbi->context;
	Cbi = cbi;
	struct browser_widget_s *bwidget = fbtk_get_userpw(widget);
	browser_mouse_state mouse;
	float scale = browser_window_get_scale(gw->bw);
	int x = (cbi->x + bwidget->scrollx) / scale;
	int y = (cbi->y + bwidget->scrolly) / scale;
	unsigned int time_now;
	static struct {
		enum { CLICK_SINGLE, CLICK_DOUBLE, CLICK_TRIPLE } type;
		unsigned int time;
	} last_click;

	if (cbi->event->type != NSFB_EVENT_KEY_DOWN &&
	    cbi->event->type != NSFB_EVENT_KEY_UP)
		return 0;

	LOG("browser window clicked at %d,%d", cbi->x, cbi->y);

	switch (cbi->event->type) {
	case NSFB_EVENT_KEY_DOWN:
		switch (cbi->event->value.keycode) {
		case NSFB_KEY_MOUSE_1:
			browser_window_mouse_click(gw->bw,
					BROWSER_MOUSE_PRESS_1, x, y);
			gui_drag.state = GUI_DRAG_PRESSED;
			gui_drag.button = 1;
			gui_drag.x = x;
			gui_drag.y = y;
			break;
			
#ifndef RTG			
		case NSFB_KEY_MOUSE_2:
			browser_window_mouse_click(gw->bw,
					BROWSER_MOUSE_PRESS_2, x, y);
			SetTaskPri(FindTask(0), -1);
			ScreenToFront(Workbench);
			break;
#endif
			
		case NSFB_KEY_MOUSE_3:
			browser_window_mouse_click(gw->bw,
					BROWSER_MOUSE_PRESS_2, x, y);
			gui_drag.state = GUI_DRAG_PRESSED;
			gui_drag.button = 2;
			gui_drag.x = x;
			gui_drag.y = y;
			
			mouse_2_click = 1;
			break;

		case NSFB_KEY_MOUSE_4:
			/* scroll up */
			if (browser_window_scroll_at_point(gw->bw, x, y,
					0, -100) == false)
				widget_scroll_y(gw, -100, false);
			break;

		case NSFB_KEY_MOUSE_5:
			/* scroll down */
			if (browser_window_scroll_at_point(gw->bw, x, y,
					0, 100) == false)
				widget_scroll_y(gw, 100, false);
			break;

		default:
			break;

		}

		break;
	case NSFB_EVENT_KEY_UP:

		mouse = 0;
		time_now = wallclock();

		switch (cbi->event->value.keycode) {
		case NSFB_KEY_MOUSE_1:
			if (gui_drag.state == GUI_DRAG_DRAG) {
				/* End of a drag, rather than click */

				if (gui_drag.grabbed_pointer) {
					/* need to ungrab pointer */
					fbtk_tgrab_pointer(widget);
					gui_drag.grabbed_pointer = false;
				}

				gui_drag.state = GUI_DRAG_NONE;

				/* Tell core */
				browser_window_mouse_track(gw->bw, 0, x, y);
				break;
			}
			/* This is a click;
			 * clear PRESSED state and pass to core */
			gui_drag.state = GUI_DRAG_NONE;
			mouse = BROWSER_MOUSE_CLICK_1;
			break;

		case NSFB_KEY_MOUSE_3:
			if (gui_drag.state == GUI_DRAG_DRAG) {
				/* End of a drag, rather than click */
				gui_drag.state = GUI_DRAG_NONE;

				if (gui_drag.grabbed_pointer) {
					/* need to ungrab pointer */
					fbtk_tgrab_pointer(widget);
					gui_drag.grabbed_pointer = false;
				}

				/* Tell core */
				browser_window_mouse_track(gw->bw, 0, x, y);
				break;
			}
			/* This is a click;
			 * clear PRESSED state and pass to core */
			gui_drag.state = GUI_DRAG_NONE;
			mouse = BROWSER_MOUSE_CLICK_2;
			break;

		default:
			break;

		}

		/* Determine if it's a double or triple click, allowing
		 * 0.5 seconds (50cs) between clicks */
		if (time_now < last_click.time + 50 &&
				cbi->event->value.keycode != NSFB_KEY_MOUSE_4 &&
				cbi->event->value.keycode != NSFB_KEY_MOUSE_5) {
			if (last_click.type == CLICK_SINGLE) {
				/* Set double click */
				mouse |= BROWSER_MOUSE_DOUBLE_CLICK;
				last_click.type = CLICK_DOUBLE;

			} else if (last_click.type == CLICK_DOUBLE) {
				/* Set triple click */
				mouse |= BROWSER_MOUSE_TRIPLE_CLICK;
				last_click.type = CLICK_TRIPLE;
			} else {
				/* Set normal click */
				last_click.type = CLICK_SINGLE;
			}
		} else {
			last_click.type = CLICK_SINGLE;
		}

		if (mouse)
			browser_window_mouse_click(gw->bw, mouse, x, y);

		last_click.time = time_now;

		break;
	default:
		break;


	}
	return 1;
}

/* called back when movement in browser window */
static int
fb_browser_window_move(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	browser_mouse_state mouse = 0;
	struct gui_window *gw = cbi->context;
	struct browser_widget_s *bwidget = fbtk_get_userpw(widget);
	float scale = browser_window_get_scale(gw->bw);
	int x = (cbi->x + bwidget->scrollx) / scale;
	int y = (cbi->y + bwidget->scrolly) / scale;

	if (gui_drag.state == GUI_DRAG_PRESSED &&
			(abs(x - gui_drag.x) > 5 ||
			 abs(y - gui_drag.y) > 5)) {
		/* Drag started */
		if (gui_drag.button == 1) {
			browser_window_mouse_click(gw->bw,
					BROWSER_MOUSE_DRAG_1,
					gui_drag.x, gui_drag.y);
		} else {
			browser_window_mouse_click(gw->bw,
					BROWSER_MOUSE_DRAG_2,
					gui_drag.x, gui_drag.y);
		}
		gui_drag.grabbed_pointer = fbtk_tgrab_pointer(widget);
		gui_drag.state = GUI_DRAG_DRAG;
	}

	if (gui_drag.state == GUI_DRAG_DRAG) {
		/* set up mouse state */
		mouse |= BROWSER_MOUSE_DRAG_ON;

		if (gui_drag.button == 1)
			mouse |= BROWSER_MOUSE_HOLDING_1;
		else
			mouse |= BROWSER_MOUSE_HOLDING_2;
	}

	browser_window_mouse_track(gw->bw, mouse, x, y);

	return 0;
}

int ucs4_pop = 0;

void rerun_netsurf(void)
{
	char run[150];

	strcpy(run, "run > nil: ");
	strcat(run, "NetSurf");
#if !defined __libnix__ && !defined AGA && !defined NOTTF
	strcat(run, PARAMS);
#endif	
	Execute(run, 0, 0);
	
	fb_complete = true;
}

static void fb_update_back_forward(struct gui_window *gw);

int ctrl = 0;

static int
fb_browser_window_input(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct gui_window *gw = cbi->context;
	static fbtk_modifier_type modifier = FBTK_MOD_CLEAR;

	int ucs4 = -1;

	LOG("got value %d", cbi->event->value.keycode);
	LOG("ctrl %d", ctrl);


	switch (cbi->event->type) {
	case NSFB_EVENT_KEY_DOWN:


		switch (cbi->event->value.keycode) {
			
		case NSFB_KEY_PAGEUP:
			if (browser_window_key_press(gw->bw,
					NS_KEY_PAGE_UP) == false)
				widget_scroll_y(gw, -fbtk_get_height(
						gw->browser), false);
			break;
			
		case NSFB_KEY_PAGEDOWN:
			if (browser_window_key_press(gw->bw,
					NS_KEY_PAGE_DOWN) == false)
				widget_scroll_y(gw, fbtk_get_height(
						gw->browser), false);
			break;

		case NSFB_KEY_RIGHT:
			if (modifier & FBTK_MOD_RCTRL ||
					modifier & FBTK_MOD_LCTRL) {
				/* CTRL held */
				if (browser_window_key_press(gw->bw,
						NS_KEY_LINE_END) == false)
					widget_scroll_x(gw, INT_MAX, true);

			} else if (modifier & FBTK_MOD_RSHIFT ||
					modifier & FBTK_MOD_LSHIFT) {
				/* SHIFT held */
				if (browser_window_key_press(gw->bw,
						NS_KEY_WORD_RIGHT) == false)
					widget_scroll_x(gw, fbtk_get_width(
						gw->browser), false);
			} else {
				/* no modifier */
				if (browser_window_key_press(gw->bw,
						NS_KEY_RIGHT) == false)
					widget_scroll_x(gw, 100, false);
			}
			break;
			 	
					
			case NSFB_KEY_LEFT:
				if (modifier & FBTK_MOD_RCTRL ||
						modifier & FBTK_MOD_LCTRL) {
					/* CTRL held */
					if (browser_window_key_press(gw->bw,
							NS_KEY_LINE_START) == false)
						widget_scroll_x(gw, 0, true);

				} else 
					if (modifier & FBTK_MOD_RSHIFT ||
						modifier & FBTK_MOD_LSHIFT) {
					/* SHIFT held */
					if (browser_window_key_press(gw->bw,
							NS_KEY_WORD_LEFT) == false)
						widget_scroll_x(gw, -fbtk_get_width(
							gw->browser), false);

				} else {
				/* no modifier */
				if (browser_window_key_press(gw->bw,
						NS_KEY_LEFT) == false)
					widget_scroll_x(gw, -100, false);
			}
			break;

		case NSFB_KEY_UP:
			if (browser_window_key_press(gw->bw,
					NS_KEY_UP) == false)
				widget_scroll_y(gw, -100, false);
			break;

		case NSFB_KEY_DOWN:
			if (browser_window_key_press(gw->bw,
					NS_KEY_DOWN) == false)
				widget_scroll_y(gw, 100, false);
			break;

		case NSFB_KEY_RSHIFT:
			SDL_EnableKeyRepeat(0, 50);
			modifier |= FBTK_MOD_RSHIFT;
			break;

		case NSFB_KEY_LSHIFT:
			SDL_EnableKeyRepeat(0, 50);
			modifier |= FBTK_MOD_LSHIFT;
			break;
			
		case NSFB_KEY_RSUPER:
		case NSFB_KEY_RCTRL:
			SDL_EnableKeyRepeat(0, 50);
			SDL_EnableUNICODE(false);
			modifier |= FBTK_MOD_RCTRL;
			ctrl = 1;
			break;
			
		case NSFB_KEY_LSUPER:
		case NSFB_KEY_LCTRL:
			SDL_EnableKeyRepeat(0, 50);
			SDL_EnableUNICODE(false);
			modifier |= FBTK_MOD_LCTRL;
			ctrl = 1;
			break;
#ifndef RTG	
		case NSFB_KEY_m:
			if (ctrl == 1)
				SetTaskPri(FindTask(0), -1);
			browser_window_key_press(gw->bw, cbi->event->value.keycode);
			break;
#endif			
		case NSFB_KEY_F4:	
			getvideo_mp4ee(gw->bw);
			break;
			
		case NSFB_KEY_F5:
			browser_window_reload(gw->bw, true);
			break;

		case NSFB_KEY_F6:
			nsoption_charp(lastpage_url) = strdup(nsurl_access(hlcache_handle_get_url(gw->bw->current_content)));
			rerun_netsurf();  
			break;
			
		case NSFB_KEY_F7:
			nsoption_charp(lastpage_url) = strdup(nsurl_access(hlcache_handle_get_url(gw->bw->current_content)));
			nsoption_write("PROGDIR:Resources/Options", NULL,NULL);
			rerun_netsurf(); 
			break;
		
		case NSFB_KEY_ESCAPE:
			fb_complete = true;
			break;

		case NSFB_KEY_DELETE:
			{
			browser_window_key_press(gw->bw, NS_KEY_DELETE_RIGHT);
			if ( (strcmp(nsurl_access(hlcache_handle_get_url(gw->bw->current_content)),
                    "file:///PROGDIR:Resources/Bookmarks.htm") == 0) && (strncmp(status_txt, "http",4) == 0) )
				{

				char *cmd = malloc(200);	
				char CurDir[128];
				GetCurrentDirName(CurDir,128);

				strcpy(cmd, "run >nil: C:sed -i \"\\,");		
				strcat(cmd, status_txt);  
				strcat(cmd,  ",d\" ");
				strcat(cmd, CurDir);
				strcat(cmd, "/resources/bookmarks.htm");
	
				Execute(cmd, 0, 0);
				free(cmd);		
				
				SDL_Delay(100);
					
				browser_window_reload(gw->bw, true);
				}			
			}
			break;
			
		case NSFB_KEY_y:
		case NSFB_KEY_z:
			if (cbi->event->value.keycode == NSFB_KEY_z &&
					(modifier & FBTK_MOD_RCTRL ||
					 modifier & FBTK_MOD_LCTRL) &&
					(modifier & FBTK_MOD_RSHIFT ||
					 modifier & FBTK_MOD_LSHIFT)) {
				/* Z pressed with CTRL and SHIFT held */
				browser_window_key_press(gw->bw, NS_KEY_REDO);
				break;

			} else if (cbi->event->value.keycode == NSFB_KEY_z &&
					(modifier & FBTK_MOD_RCTRL ||
					 modifier & FBTK_MOD_LCTRL)) {
				/* Z pressed with CTRL held */
				browser_window_key_press(gw->bw, NS_KEY_UNDO);
				break;

			} else if (cbi->event->value.keycode == NSFB_KEY_y &&
					(modifier & FBTK_MOD_RCTRL ||
					 modifier & FBTK_MOD_LCTRL)) {
				/* Y pressed with CTRL held */
				browser_window_key_press(gw->bw, NS_KEY_REDO);
				break;

			}			
			/* Z or Y pressed but not undo or redo;
			 * Fall through to default handling */			
		default:
			SDL_EnableUNICODE(true);
			if (strcmp(nsoption_charp(accept_charset), "AmigaPL") == 0) {
				switch (cbi->event->value.keycode) { 
					case 172: cbi->event->value.keycode = 175;
						break;	
					case 177: cbi->event->value.keycode = 191;
						break;
					case 202: cbi->event->value.keycode = 198;
						break;
					case 203: cbi->event->value.keycode = 202;
						break;
					case 234: cbi->event->value.keycode = 230;
						break;
					case 235: cbi->event->value.keycode = 234;
						break;
					 default:
					 /* do nothing */
						break;
				}
			}
		
			ucs4 = fbtk_keycode_to_ucs4(cbi->event->value.keycode,
						    modifier);

			if (ucs4 != -1)
				browser_window_key_press(gw->bw, ucs4);
			break;
		}
		break;

		
	case NSFB_EVENT_KEY_UP:
		switch (cbi->event->value.keycode) {
		case NSFB_KEY_RSHIFT:
			modifier &= ~FBTK_MOD_RSHIFT;
			SDL_EnableKeyRepeat(300, 50);
			shift = 0;
			break;

		case NSFB_KEY_LSHIFT:
			modifier &= ~FBTK_MOD_LSHIFT;
			SDL_EnableKeyRepeat(300, 50);
			shift = 0;
			break;
		
		case NSFB_KEY_LALT:
		case NSFB_KEY_RALT:
			SDL_EnableKeyRepeat(300, 50);
			alt = 0;
			break;

		case NSFB_KEY_BACKSPACE:
			if (ctrl==1)
			{
				struct browser_window *bw = gw->bw;
				
			if (browser_window_back_available(bw))
				browser_window_history_back(bw, false);
					
				fb_update_back_forward(gw);
			}
			ctrl = 0;
			break;

		case NSFB_KEY_RSUPER:
		case NSFB_KEY_RCTRL:
			SDL_EnableKeyRepeat(300, 50);
			SDL_EnableUNICODE(true);
			modifier &= ~FBTK_MOD_RCTRL;
			ctrl = 0;
			break;

		case NSFB_KEY_LSUPER:
		case NSFB_KEY_LCTRL:
			SDL_EnableKeyRepeat(300, 50);
			SDL_EnableUNICODE(true);
			modifier &= ~FBTK_MOD_LCTRL;
			ctrl = 0;
			break;

		default:		
			break;
		}
		
		break;	

	default:
		break;
	}

	return 0;
}

const char *
add_theme_path(const char *icon) 
{
	static char path[128];

	strcpy(path, "PROGDIR:Resources/theme/");
	strcat(path, nsoption_charp(theme));
	strcat(path,"/");
	strcat(path, icon);

	return path;
}

static void
fb_update_back_forward(struct gui_window *gw)
{
	struct browser_window *bw = gw->bw;

	fbtk_set_bitmap(gw->back,
			(browser_window_back_available(bw)) ?			
			load_bitmap(add_theme_path("back.png")) :  load_bitmap(add_theme_path("back_g.png")));
		
	fbtk_set_bitmap(gw->forward,
			(browser_window_forward_available(bw)) ?			
			load_bitmap(add_theme_path("forward.png")) :  load_bitmap(add_theme_path("forward_g.png")));


}

/* left icon click routine */

int
fb_leftarrow_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct gui_window *gw = cbi->context;
	struct browser_window *bw = gw->bw;
	
 	if (cbi->event->type != NSFB_EVENT_KEY_UP)	
		return 0;
	
	if (browser_window_back_available(bw))
		browser_window_history_back(bw, false);
		
	fb_update_back_forward(gw);
	
	return 1;
}

/* right arrow icon click routine */

int
fb_rightarrow_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct gui_window *gw = cbi->context;
	struct browser_window *bw = gw->bw;

	if (cbi->event->type == NSFB_EVENT_KEY_UP) 
		return 0;
	
	if (browser_window_forward_available(bw))
		browser_window_history_forward(bw, false);

	fb_update_back_forward(gw);
	
	return 1;

}

/* reload icon click routine */

int
fb_reload_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;

	if (cbi->event->type == NSFB_EVENT_KEY_UP) 
		return 0;


	if (cbi->event->value.keycode == NSFB_KEY_MOUSE_3) {
		//nsoption_read(options, nsoptions);
		//SDL_Delay(500);
		nsoption_charp(lastpage_url) = strdup(nsurl_access(hlcache_handle_get_url(bw->current_content)));
		nsoption_write(options, NULL, NULL);
		rerun_netsurf();  	
	}
	
	browser_window_reload(bw, true);

	return 1;
}

/* stop icon click routine */
int
fb_stop_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->type == NSFB_EVENT_KEY_UP) 
		return 0;
	
	//fbtk_set_bitmap(widget, load_bitmap(icon_file));
	
	browser_window_stop(bw);

	return 1;
}
#ifndef RTG	
/* close browser window icon click routine */
static int
fb_close_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->type != NSFB_EVENT_KEY_UP)
		return 0;
		
		if (cbi->event->value.keycode == NSFB_KEY_MOUSE_3) {
			nsoption_read(options, nsoptions);
			SDL_Delay(500);
			nsoption_charp(lastpage_url) = strdup(nsurl_access(hlcache_handle_get_url(bw->current_content)));
			nsoption_write(options, NULL, NULL);
			rerun_netsurf();  	
		}
		else
			fb_complete = true;
		
		return 0;
		
}
#endif
static int
fb_scroll_callback(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct gui_window *gw = cbi->context;

	switch (cbi->type) {
	case FBTK_CBT_SCROLLY:
		widget_scroll_y(gw, cbi->y, true);
		break;

	case FBTK_CBT_SCROLLX:
		widget_scroll_x(gw, cbi->x, true);
		break;

	default:
		break;
	}
	return 0;
}


static int
fb_url_enter(void *pw, char *text)
{
	struct browser_window *bw = pw;
	nsurl *url;
	nserror error;

	error = nsurl_create(text, &url);
	if (error != NSERROR_OK) {
		warn_user(messages_get_errorcode(error), 0);
	} else {
		browser_window_navigate(bw, url, NULL, BW_NAVIGATE_HISTORY,
				NULL, NULL, NULL);

		nsurl_unref(url);
	}

	return 0;
}

int
fb_url_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
#ifdef RTG	
	framebuffer_set_cursor(&null_image);
	SDL_ShowCursor(SDL_ENABLE);	
#else
	framebuffer_set_cursor(&pointer);
	SDL_ShowCursor(SDL_DISABLE);
#endif	
	return 0;
}

/* home icon click routine */
int
fb_home_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->type == NSFB_EVENT_KEY_UP)	
		return 0;

	fb_url_enter(bw, nsoption_charp(homepage_url));
			
	return 1;
}

/* paste from clipboard click routine */
int
fb_paste_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->type == NSFB_EVENT_KEY_UP) 
		return 0;

		
	if (ReadClip() != NULL)
		{
		int len = (int) strlen(ReadClip());		 
		char * str = strdup(ReadClip());
		 
		 len--;
		 while (isspace(*(str + len) )) {
			  len--;
		 }
		 
		 *(str + len + 1) = '\0';
		 
		fb_url_enter(bw, str);
		free(str);
		}
	
	return 1;
}

/* write to clipboard icon click routine */
int
fb_copy_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->type == NSFB_EVENT_KEY_UP) 
		return 0;

			
	char *clip = strdup(nsurl_access(hlcache_handle_get_url(bw->current_content)));
	
	WriteClip(clip);
		
	return 1;
}

char* usunznaki(char* s, char *c)
{
	 int i = strlen(c);
     int p = 0, q = 0;
     while (s[++p] != '\0') if (p >= i) s[q++] = s[p];
     s[q] = '\0';

    return s;
}

void 
get_video(struct browser_window *bw) 
{	
	char *cmd = AllocVec(1000, MEMF_ANY);
	char *url = strdup(nsurl_access(hlcache_handle_get_url(bw->current_content)));
	
	strcpy(cmd, "run Sys:Rexxc/rx S:getvideo.rexx "); 
	strcat(cmd, url);
	strcat(cmd, " play ");
	
	Execute("Mount >NIL: TCP: from AmiTCP:devs/Inet-Mountlist",0,0);
	Execute(cmd, 0, 0);

	FreeVec(cmd);
}

void 
getvideo_mp4ee(struct browser_window *bw) 
{   	
	char *url = strdup(nsurl_access(hlcache_handle_get_url(bw->current_content)));
	char *addr = malloc(strlen(url) + strlen("http://www.mp4.ee/watch?v="));
	
	mp4ee = true;
	strcpy(addr,"http://www.mp4.ee/watch?v=");
	strcat(addr, url);
	fb_url_enter(bw, addr);

	free(addr);
	free(url);
}

/* get video icon click routine */
int
fb_getvideo_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;

	if (cbi->event->type == NSFB_EVENT_KEY_UP)
		return 0;
		
	getvideo_click = true;
#ifdef getvideo			
    get_video(bw);
#endif
	getvideo_mp4ee(bw);
    
	//fbtk_set_bitmap(widget, load_bitmap(icon_file));
	
	return 1;
}

/* set current url as homepage icon click routine */
int
fb_sethome_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
 	struct browser_window *bw = cbi->context;
	
	if (cbi->event->type == NSFB_EVENT_KEY_UP) 
		return 0;

	
	nsoption_charp(homepage_url) = strdup(nsurl_access(hlcache_handle_get_url(bw->current_content)));
	nsoption_write(options, NULL, NULL);
	//free(nsoption_charp(homepage_url));
	
	return 1;
}

/* add favourites icon click routine */
int
fb_add_fav_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	fbtk_widget_t *fav = NULL;
	fbtk_widget_t *label = NULL;
	char *bitmap = NULL;
	
	if (cbi->event->type == NSFB_EVENT_KEY_UP) 
		return 0;

		
	BPTR fh;
	char *cmd = AllocVec(1000, MEMF_ANY);

	int inum = 0;
	
	/* Show select window */
	
	strcpy(cmd, "RequestChoice > ENV:NSfav TITLE=\"NetSurf\" BODY=\"");
	strcat(cmd, messages_get("FavouriteAdd"));
	strcat(cmd, "\" GADGETS=\"___1___|___2___|___3___|___4___|___5___|___6___|___7___|___8___|___9___|___10___|___11___|___12___|");
	strcat(cmd, messages_get("Cancel"));
	strcat(cmd,"\"");

	Execute(cmd, 0, 0);
	fh = Open("ENV:NSfav",MODE_OLDFILE);

	char snum[3];
	
	Read(fh,snum,2);
	inum = atoi(snum);

	Close(fh);
	FreeVec(cmd);
	/* Download favicon */		
	
	char *wget = AllocVec(1000, MEMF_ANY);
	char *opt = strdup("?&format=png&width=16&height=16&canAudit=false -OResources/Icons/favicon");
	//char *opt = strdup("?&format=png&width=16&height=16 -OResources/Icons/favicon");

	strcpy(wget, "echo >nil: \" C:wget >nil: ");
	strcat(wget, "-PPROGDIR:Resources/Icons/ ");
	//strcat(wget, " 	http://g.fvicon.com/");
	//strcat(wget, " 	http://www.google.com/s2/favicons?domain=");
	strcat(wget, nsoption_charp(favicon_source));
	
	if (inum > 0) {
		get_url = strdup(nsurl_access(hlcache_handle_get_url(bw->current_content)));	
		}	
		
	if (inum == 1 ) {	
		   nsoption_charp(favourite_1_url) = strdup(get_url);
		   nsoption_charp(favourite_1_label) = strdup(stitle);
		   fav = fav1;
		   label = label1;
		   }
	else if (inum == 2 ) {
		   nsoption_charp(favourite_2_url) = strdup(get_url);
		   nsoption_charp(favourite_2_label) = strdup(stitle);	   	   
		   fav = fav2;
		   label = label2;	   
		   } 
	else if (inum == 3 ) {
		   nsoption_charp(favourite_3_url) = strdup(get_url);
		   nsoption_charp(favourite_3_label) = strdup(stitle);	   
		   fav = fav3;
		   label = label3;   
		   } 
	else if (inum == 4 ) {
		   nsoption_charp(favourite_4_url) = strdup(get_url);
		   nsoption_charp(favourite_4_label) = strdup(stitle);	   	   
		   fav = fav4;
		   label = label4;	   
		   } 	
	else if (inum == 5 )  {
		   nsoption_charp(favourite_5_url) = strdup(get_url);
		   nsoption_charp(favourite_5_label) = strdup(stitle);	   
		   fav = fav5;
		   label = label5;	   
		   } 
	else if (inum == 6  ) {
		   nsoption_charp(favourite_6_url) = strdup(get_url);
		   nsoption_charp(favourite_6_label) = strdup(stitle);	   
		   fav = fav6;
		   label = label6;	   
		   } 
	else if (inum == 7  ) {
		   nsoption_charp(favourite_7_url) = strdup(get_url);
		   nsoption_charp(favourite_7_label) = strdup(stitle);	   	   
		   fav = fav7;
		   label = label7;	   
		   } 	
	else if (inum == 8  ) {
		   nsoption_charp(favourite_8_url) = strdup(get_url);
		   strcat(wget, get_url);
		   nsoption_charp(favourite_8_label) = strdup(stitle);   
		   strcat(wget, opt);
		   fav = fav8;	
		   label = label8;	   
		   }	 
	else if (inum == 9 ) {
		   nsoption_charp(favourite_9_url) = strdup(get_url);
		   nsoption_charp(favourite_9_label) = strdup(stitle);	      
		   fav = fav9;
		   label = label9;	   
		   } 
	else if (inum == 10 ) {
		   nsoption_charp(favourite_10_url) = strdup(get_url);
		   nsoption_charp(favourite_10_label) = strdup(stitle);	   	   		   
		   fav = fav10;
		   label = label10;	   
		   } 	
	else if (inum == 11 ) {
		   nsoption_charp(favourite_11_url) = strdup(get_url);
		   nsoption_charp(favourite_11_label) = strdup(stitle);	   
		   fav = fav11;
		   label = label11;	   
		   } 
	else if (inum == 12 ) {
		   nsoption_charp(favourite_12_url) = strdup(get_url);
		   nsoption_charp(favourite_12_label) = strdup(stitle);   
		   fav = fav12;
		   label = label12;	   
		} 				
		
	if (inum != 0 ) 
	{
		sprintf(snum, "%d", inum);
		
		strcat(wget, get_url);
		strcat(wget, opt);	
		strcat(wget, snum);
		strcat(wget, ".png ");
		strcat(wget, " \" >RAM:script");
			
		Execute(wget, 0, 0);
		Execute("echo \"endcli\" >>RAM:script", 0, 0);
		Execute("execute RAM:script >nil:", 0, 0);
		Execute("delete RAM:script >nil: ", 0, 0);
	
		nsoption_write(options, NULL, NULL);
				
		if (fav != NULL)
			{	
			bitmap = malloc(strlen("PROGDIR:Resources/Icons/favicon.png") + inum);
			strcpy(bitmap,"PROGDIR:Resources/Icons/favicon");
			strcat(bitmap, snum);
			strcat(bitmap, ".png");
			fbtk_set_bitmap(fav, load_bitmap(bitmap));
			fbtk_set_text(label, stitle);
			}
	}
	
	free(bitmap);
	FreeVec(wget);

	return 1;
}

static int
set_throbber_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{

	gui_window_set_status(g2, "Go to Workbench");

	return 0;
}

int
throbber_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{

	if (cbi->event->type == NSFB_EVENT_KEY_UP) 
		return 0;

	SetTaskPri(FindTask(0), -1);
	ScreenToFront(Workbench);
	Execute("RequestChoice TITLE=\"NetSurf\" BODY=\"Return to NetSurf\" GADGETS=\"OK\" > nil:",0,0);
	ScreenToBack(Workbench);
	
	return 1;
}

/* add bookmark icon click routine */
int
fb_add_bookmark_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->type == NSFB_EVENT_KEY_UP) 
		return 0;	
	
	stitle = strdup(ami_to_utf8_easy(stitle));

	get_url = strdup(nsurl_access(hlcache_handle_get_url(bw->current_content)));
	char *cmd = AllocVec(strlen("echo <li><a href=\"") + strlen(get_url) + strlen(stitle) + 
			strlen("</a></li >> PROGDIR:Resources/Bookmarks.htm") + 10, MEMF_ANY);

	strcpy(cmd, "echo \"<li><a href=");
	strcat(cmd, get_url);
	strcat(cmd, ">");
	strcat(cmd, stitle );
	strcat(cmd, "</a></li>\" >> Resources/Bookmarks.htm");

	Execute(cmd, 0, 0);

	FreeVec(cmd);
	
	return 1;
}

/* search icon click routine */
int
fb_search_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{

	if (cbi->event->type == NSFB_EVENT_KEY_UP) {		
      	BPTR fh;
      	char buffer[1];
		int buff = 0;
		
      	char *cmd =  AllocVec(300, MEMF_ANY);		
      	strcpy(cmd, "RequestChoice > ENV:NSsrcheng TITLE=\"");
		strcat(cmd, messages_get("SearchProvider"));
		strcat(cmd, "\" BODY=\"\" GADGETS=\"Google|Yahoo|Bing|DuckDuckGo|YouTube|Ebay|Allegro|Aminet|Wikipedia|");
		strcat(cmd, messages_get("Cancel"));
		strcat(cmd,"\"");

      	Execute(cmd, 0, 0);
      	fh = Open("ENV:NSsrcheng",MODE_OLDFILE);

      	Read(fh,buffer,1);
		buff = atoi(buffer);
		
      	Close(fh);
		FreeVec(cmd);
		
      	  if (buff == 1) {
      		   nsoption_charp(def_search_bar) = strdup("http://www.google.com/search?q=");
      		   fbtk_set_bitmap(widget, load_bitmap("PROGDIR:Resources/Icons/google.png"));
				Execute("copy Resources/Icons/google.png Resources/Icons/search.png", 0, 0);
				}
      	  else if (buff == 2) {
      		   nsoption_charp(def_search_bar) = strdup("http://search.yahoo.com/search?p=");
      		   fbtk_set_bitmap(widget, load_bitmap("PROGDIR:Resources/Icons/yahoo.png"));		   
				Execute("copy Resources/Icons/yahoo.png Resources/Icons/search.png", 0, 0);
			   }
      	  else if (buff == 3) {
      		   nsoption_charp(def_search_bar) = strdup("http://www.bing.com/search?q=");
      		   fbtk_set_bitmap(widget, load_bitmap("PROGDIR:Resources/Icons/bing.png"));		   
				Execute("copy Resources/Icons/bing.png Resources/Icons/search.png", 0, 0);
			   }
      	  else if (buff == 4) {
      		   nsoption_charp(def_search_bar) = strdup("http://www.duckduckgo.com/html/?q=");
      		   fbtk_set_bitmap(widget, load_bitmap("PROGDIR:Resources/Icons/duckduckgo.png"));		   
				Execute("copy Resources/Icons/duckduckgo.png Resources/Icons/search.png", 0, 0);
			   }			
      	  else if (buff == 5) {
      		   nsoption_charp(def_search_bar) = strdup("http://www.youtube.com/results?search_query=");
      		   fbtk_set_bitmap(widget, load_bitmap("PROGDIR:Resources/Icons/youtube.png"));
				Execute("copy Resources/Icons/youtube.png Resources/Icons/search.png", 0, 0);
			   }
      	  else if (buff == 6) {
      		   nsoption_charp(def_search_bar) = strdup("http://shop.ebay.com/items/");
      		   fbtk_set_bitmap(widget, load_bitmap("PROGDIR:Resources/Icons/ebay.png"));		   
				Execute("copy Resources/Icons/ebay.png Resources/Icons/search.png", 0, 0);
			   }			
      	  else if (buff == 7) {
      		   nsoption_charp(def_search_bar) = strdup("http://allegro.pl/listing.php/search?string=");
      		   fbtk_set_bitmap(widget, load_bitmap("PROGDIR:Resources/Icons/allegro.png"));
				Execute("copy Resources/Icons/allegro.png Resources/Icons/search.png", 0, 0);
			   }			
      	  else if (buff == 8) {
      		   nsoption_charp(def_search_bar) = strdup("http://aminet.net/search?query=");
      		   fbtk_set_bitmap(widget,  load_bitmap("PROGDIR:Resources/Icons/aminet.png"));
				Execute("copy Resources/Icons/aminet.png Resources/Icons/search.png", 0, 0);
			   }
      	  else if (buff == 9) {
      		   nsoption_charp(def_search_bar) = strdup("http://en.wikipedia.org/w/index.php?title=Special:Search&search=");
      		   fbtk_set_bitmap(widget, load_bitmap("PROGDIR:Resources/Icons/wiki.png"));
				Execute("copy Resources/Icons/wiki.png Resources/Icons/search.png", 0, 0);
			   }

      	nsoption_write(options, NULL, NULL);
		
		return 0;
		}
	return 1;
}

int
fb_searchbar_enter(void *pw, char *text)
{
    struct browser_window *bw = pw;

	if (text)
		{
		char *address = AllocVec(strlen(text)
			 + strlen(nsoption_charp(def_search_bar))+1, MEMF_ANY );

		strcpy(address, nsoption_charp(def_search_bar));
		strcat(address, text);
		
		fb_url_enter(bw, address);
		
		FreeVec(address);
		}
	return 0;
}

int
fb_bookmarks_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->type == NSFB_EVENT_KEY_UP) 
		return 0;	
	
	char *cmd = strdup("file:///PROGDIR:Resources/Bookmarks.htm");
	
	fb_url_enter(bw, cmd);
	free(cmd);
	
	return 1;
}

void
read_labels(void)
{
fbtk_set_text(label1, nsoption_charp(favourite_1_label));
fbtk_set_text(label2, nsoption_charp(favourite_2_label));
fbtk_set_text(label3, nsoption_charp(favourite_3_label));
fbtk_set_text(label4, nsoption_charp(favourite_4_label));
fbtk_set_text(label5, nsoption_charp(favourite_5_label));
fbtk_set_text(label6, nsoption_charp(favourite_6_label));
fbtk_set_text(label7, nsoption_charp(favourite_7_label));
fbtk_set_text(label8, nsoption_charp(favourite_8_label));
fbtk_set_text(label9, nsoption_charp(favourite_9_label));
fbtk_set_text(label10, nsoption_charp(favourite_10_label));
fbtk_set_text(label11, nsoption_charp(favourite_11_label));
fbtk_set_text(label12, nsoption_charp(favourite_12_label));
}

int 
fb_prefs_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{	
	if (cbi->event->type == NSFB_EVENT_KEY_UP) 
		return 0;

	
	char *text_editor = AllocVec(200, MEMF_ANY);

#ifndef RTG	
	nsoption_int(fullscreen) = 1;
#endif	
	if (nsoption_int(fullscreen) == 1) {
		SetTaskPri(FindTask(0), -1);
	#if defined __clib2__		
		//Timeout = 100000;
	#endif
		ScreenToFront(Workbench);
	}
	strcpy(text_editor, nsoption_charp(text_editor));
	strcat(text_editor, " Resources/Options"); 
	Execute(text_editor,0,0);

if (nsoption_int(fullscreen) == 1){
	ScreenToBack(Workbench);
	#if defined __clib2__	
	//Timeout = 1;
	#endif
	}
	
	FreeVec(text_editor);
	
	nsoption_read(options, nsoptions);

	read_labels();
	
	return 1;
} 
 
char* usunhttp(char* s)
{
     int p = 0, q = 0, i = 7;
     while (s[++p] != '\0') if (p >= i) s[q++] = s[p];
     s[q] = '\0';

    return s;
}

int
fb_getpage_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->type == NSFB_EVENT_KEY_UP) 
		return 0;

	fbtk_set_bitmap(widget, load_bitmap(icon_file));  

	BPTR fh;
	UBYTE *result = strdup("0");
	int resint;

	char *cmd = strdup("RequestChoice > env:nstemp TITLE=\"Download\" BODY=\"Download \" GADGETS=\"using Wget|as PDF|Cancel\"");
			   
	Execute(cmd, 0, 0);
	free(cmd);
	fh = Open("ENV:NStemp",MODE_OLDFILE);    
	Read(fh, result, 1);	
	resint = atoi(result);
	Close(fh); 
			
	if (resint > 0)  
		{
		if (strcmp(nsoption_charp(download_path),"") == 0)
			{
			AslBase = OpenLibrary("asl.library", 0);
			struct FileRequester *savereq;

			savereq = (struct FileRequester *)AllocAslRequestTags(ASL_FileRequest,
								ASLFR_DoSaveMode,TRUE,
								ASLFR_RejectIcons,TRUE,
								ASLFR_InitialDrawer,0,
								TAG_DONE);
			AslRequest(savereq,NULL);
			nsoption_charp(download_path) = strdup(savereq->fr_Drawer);
			nsoption_write(options, NULL, NULL);

			FreeAslRequest(savereq);
			CloseLibrary(AslBase);
			}
		 
		char *wget = AllocVec(2000, MEMF_ANY);
		char *url = strdup(nsurl_access(hlcache_handle_get_url(bw->current_content)));
		
		if ( resint == 2 ) {
			usunhttp(url);
			char *wsk;
			wsk=strchr(url,'/');
			*wsk='-';
			strlcpy(url,url,strlen(url));
		}
		strcpy(wget, "run c/wget ");
		strcat(wget, "-P");
		strcat(wget, nsoption_charp(download_path));		
		strcat(wget, " ");
		
		if ( resint == 2 )	
			strcat(wget, "http://pdfmyurl.com?url=");	
			
		strcat(wget, url);
		strcat(wget, " -x -O ");		
		strcat(wget, nsoption_charp(download_path));
		
		if ( resint == 2 )			
			strcat(wget, "/");

		url = strdup(strrchr(url, '/'));

		strcat(wget, url);
				
		if ( resint == 2 ) 			
			strcat(wget, ".pdf");

		fh = Open("CON:", MODE_NEWFILE);	

		Execute(wget, fh, 0);	

		Close(fh);

		free(url);
		FreeVec(wget);			
		}  

			
	return 1;
}

int
fb_javascript_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
			nsoption_read(options, nsoptions);
		return 0;
	}
	if (nsoption_bool(enable_javascript))
		{nsoption_bool(enable_javascript) = false;
		fbtk_set_bitmap(widget, load_bitmap(add_theme_path("no_java.png")));}
	else
		{nsoption_bool(enable_javascript) = true;
		fbtk_set_bitmap(widget, load_bitmap(add_theme_path("java.png")));}	
	   
	nsoption_write(options, NULL, NULL);
		
	return 1;
}

int
fb_openfile_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->type == NSFB_EVENT_KEY_UP)
		return 0;

	
	char *file = AllocVec(1024,MEMF_ANY);

	AslBase = OpenLibrary("asl.library", 0);
	struct FileRequester *savereq;

	savereq = (struct FileRequester *)AllocAslRequestTags(ASL_FileRequest,
						ASLFR_DoSaveMode,TRUE,
						ASLFR_RejectIcons,TRUE,
						ASLFR_InitialDrawer,0,
						TAG_DONE);
						
	AslRequest(savereq,NULL);
	
	strcpy(file,"file:///");
	strcat(file, savereq->fr_Drawer);
	if (strcmp(savereq->fr_Drawer,"") == 0)
		strcat(file, "PROGDIR:");
	else
		strcat(file, "/");	
	strcat(file, savereq->fr_File);

	FreeAslRequest(savereq);
	CloseLibrary(AslBase);

	if (strcmp(file, "file:///") != 0 )
		fb_url_enter(bw, file);

	FreeVec(file);
	
	return 1;
}
/* fav1 icon click routine */
int
fb_fav1_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->value.keycode == NSFB_KEY_MOUSE_3)	{
		fbtk_set_bitmap(widget, NULL);
		nsoption_charp(favourite_1_url) = NULL;
		nsoption_charp(favourite_1_label) = NULL;
		fbtk_set_text(label1, NULL);
	    nsoption_write(options, NULL, NULL);
		Execute("delete PROGDIR:Resources/Icons/favicon1.png", 0, 0);
		return 0;
	}	

	if ((nsoption_charp(favourite_1_url) != NULL) && (nsoption_charp(favourite_1_url)[0] != '\0'))
		fb_url_enter(bw, nsoption_charp(favourite_1_url));
	
	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap("PROGDIR:Resources/Icons/favicon1.png"));
		
		return 0;
	}	
	return 1;
}
/* fav2 icon click routine */
int
fb_fav2_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->value.keycode == NSFB_KEY_MOUSE_3)	{
		fbtk_set_bitmap(widget, NULL);
		nsoption_charp(favourite_2_url) = NULL;
		nsoption_charp(favourite_2_label) = NULL;
		fbtk_set_text(label2, NULL);
	    nsoption_write(options, NULL, NULL);
		Execute("delete Netsurf:Resources/Icons/favicon2.png", 0, 0);
		return 0;
	}		

	if (nsoption_charp(favourite_2_url) != NULL && nsoption_charp(favourite_2_url)[0] != '\0')		
		fb_url_enter(bw, nsoption_charp(favourite_2_url));

	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap("PROGDIR:Resources/Icons/favicon2.png"));
		return 0;
	}	
	return 1;
}
/* fav3 icon click routine */
int
fb_fav3_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->value.keycode == NSFB_KEY_MOUSE_3)	{
		fbtk_set_bitmap(widget, NULL);
		nsoption_charp(favourite_3_url) = NULL;
		nsoption_charp(favourite_3_label) = NULL;
		fbtk_set_text(label3, NULL);
	    nsoption_write(options, NULL, NULL);
		Execute("delete PROGDIR:Resources/Icons/favicon3.png", 0, 0);
		return 0;
	}	
	
	if (nsoption_charp(favourite_3_url) != NULL && nsoption_charp(favourite_3_url)[0]!= '\0')		
		fb_url_enter(bw, nsoption_charp(favourite_3_url));

	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap("PROGDIR:Resources/Icons/favicon3.png"));
		return 0;
	}	
	return 1;
}
/* fav4 icon click routine */
int
fb_fav4_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->value.keycode == NSFB_KEY_MOUSE_3)	{
		fbtk_set_bitmap(widget, NULL);
		nsoption_charp(favourite_4_url) = NULL;
		nsoption_charp(favourite_4_label) = NULL;
		fbtk_set_text(label4, NULL);
	    nsoption_write(options, NULL, NULL);
		Execute("delete PROGDIR:Resources/Icons/favicon4.png", 0, 0);
		return 0;
	}	
	
	if (nsoption_charp(favourite_4_url) != NULL && nsoption_charp(favourite_4_url)[0]!= '\0')		
		fb_url_enter(bw, nsoption_charp(favourite_4_url));

	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap("PROGDIR:Resources/Icons/favicon4.png"));
		return 0;
	}	
	return 1;
}
/* fav5 icon click routine */
int
fb_fav5_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->value.keycode == NSFB_KEY_MOUSE_3)	{
		fbtk_set_bitmap(widget, NULL);
		nsoption_charp(favourite_5_url) = NULL;
		nsoption_charp(favourite_5_label) = NULL;
		fbtk_set_text(label5, NULL);
	    nsoption_write(options, NULL, NULL);
		Execute("delete PROGDIR:Resources/Icons/favicon5.png", 0, 0);
		return 0;
	}	
	
	if (nsoption_charp(favourite_5_url) != NULL && nsoption_charp(favourite_5_url)[0]!= '\0')		
		fb_url_enter(bw, nsoption_charp(favourite_5_url));

	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap("PROGDIR:Resources/Icons/favicon5.png"));
		return 0;
	}	
	return 1;
}
/* fav6 icon click routine */
int
fb_fav6_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->value.keycode == NSFB_KEY_MOUSE_3)	{
		fbtk_set_bitmap(widget, NULL);
		nsoption_charp(favourite_6_url) = NULL;
		nsoption_charp(favourite_6_label) = NULL;
		fbtk_set_text(label6, NULL);
	    nsoption_write(options, NULL, NULL);
		Execute("delete PROGDIR:Resources/Icons/favicon6.png", 0, 0);
		return 0;
	}	
		
	if (nsoption_charp(favourite_6_url) != NULL && nsoption_charp(favourite_6_url)[0]!= '\0')		
		fb_url_enter(bw, nsoption_charp(favourite_6_url));

	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap("PROGDIR:Resources/Icons/favicon6.png"));
		return 0;
	}	
	return 1;
}
/* fav7 icon click routine */
int
fb_fav7_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->value.keycode == NSFB_KEY_MOUSE_3)	{
		fbtk_set_bitmap(widget, NULL);
		nsoption_charp(favourite_7_url) = NULL;
		nsoption_charp(favourite_7_label) = NULL;
		fbtk_set_text(label7, NULL);	
	    nsoption_write(options, NULL, NULL);
		Execute("delete PROGDIR:Resources/Icons/favicon7.png", 0, 0);
		return 0;
	}	
	
	if (nsoption_charp(favourite_7_url) != NULL && nsoption_charp(favourite_7_url)[0]!= '\0')		
		fb_url_enter(bw, nsoption_charp(favourite_7_url));

	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap("PROGDIR:Resources/Icons/favicon7.png"));
		return 0;
	}	
	return 1;
}
/* fav8 icon click routine */
int
fb_fav8_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->value.keycode == NSFB_KEY_MOUSE_3)	{
		fbtk_set_bitmap(widget, NULL);
		nsoption_charp(favourite_8_url) = NULL;
		nsoption_charp(favourite_8_label) = NULL;
		fbtk_set_text(label8, NULL);
	    nsoption_write(options, NULL, NULL);
		Execute("delete PROGDIR:Resources/Icons/favicon8.png", 0, 0);
		return 0;
	}	
	
	if (nsoption_charp(favourite_8_url) != NULL && nsoption_charp(favourite_8_url)[0]!= '\0')		
		fb_url_enter(bw, nsoption_charp(favourite_8_url));

	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap("PROGDIR:Resources/Icons/favicon8.png"));
		return 0;
	}	
	return 1;
}
/* fav9 icon click routine */
int
fb_fav9_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->value.keycode == NSFB_KEY_MOUSE_3)	{
		fbtk_set_bitmap(widget, NULL);
		nsoption_charp(favourite_9_url) = NULL;
		nsoption_charp(favourite_9_label) = NULL;
		fbtk_set_text(label9, NULL);
	    nsoption_write(options, NULL, NULL);
		Execute("delete PROGDIR:Resources/Icons/favicon9.png", 0, 0);
		return 0;
	}	
	
	if (nsoption_charp(favourite_9_url) != NULL && nsoption_charp(favourite_9_url)[0]!= '\0')
		fb_url_enter(bw, nsoption_charp(favourite_9_url));

	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap("PROGDIR:Resources/Icons/favicon9.png"));
		return 0;
	}	
	return 1;
}
/* fav10 icon click routine */
int
fb_fav10_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->value.keycode == NSFB_KEY_MOUSE_3)	{
		fbtk_set_bitmap(widget, NULL);
		nsoption_charp(favourite_10_url) = NULL;
		nsoption_charp(favourite_10_label) = NULL;
		fbtk_set_text(label10, NULL);
	    nsoption_write(options, NULL, NULL);
		Execute("delete PROGDIR:Resources/Icons/favicon10.png", 0, 0);
		return 0;
	}	
	
	if (nsoption_charp(favourite_10_url) != NULL && nsoption_charp(favourite_10_url)[0]!= '\0')
		fb_url_enter(bw, nsoption_charp(favourite_10_url));

	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap("PROGDIR:Resources/Icons/favicon10.png"));
		return 0;
	}	
	return 1;
}
/* fav11 icon click routine */
int
fb_fav11_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->value.keycode == NSFB_KEY_MOUSE_3)	{
		fbtk_set_bitmap(widget, NULL);
		nsoption_charp(favourite_11_url) = NULL;
		nsoption_charp(favourite_11_label) = NULL;
		fbtk_set_text(label11, NULL);
	    nsoption_write(options, NULL, NULL);
		Execute("delete PROGDIR:Resources/Icons/favicon11.png", 0, 0);
		return 0;
	}	
	
	if (nsoption_charp(favourite_11_url) != NULL && nsoption_charp(favourite_11_url)[0]!= '\0')		
		fb_url_enter(bw, nsoption_charp(favourite_11_url));

	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap("PROGDIR:Resources/Icons/favicon11.png"));
		return 0;
	}	
	return 1;
}
/* fav12 icon click routine */
int
fb_fav12_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->value.keycode == NSFB_KEY_MOUSE_3)	{
		fbtk_set_bitmap(widget, NULL);
		nsoption_charp(favourite_12_url) = NULL;
		nsoption_charp(favourite_12_label) = NULL;
		fbtk_set_text(label12, NULL);
	    nsoption_write(options, NULL, NULL);
		Execute("delete PROGDIR:Resources/Icons/favicon12.png", 0, 0);
		return 0;
	}	
	
	if (nsoption_charp(favourite_12_url) != NULL && nsoption_charp(favourite_12_url)[0]!= '\0')		
		fb_url_enter(bw, nsoption_charp(favourite_12_url));

	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap("PROGDIR:Resources/Icons/favicon12.png"));
		return 0;
	}	
	return 1;
}


static int
fb_url_move(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
    bw_url =  cbi->context;
	
	framebuffer_set_cursor(&caret);
	SDL_ShowCursor(SDL_DISABLE);
	
	return 0;
}

static int
set_ptr_default_move(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
#ifdef  RTG
	framebuffer_set_cursor(&null_image); 	
    SDL_ShowCursor(SDL_ENABLE);
#else
	framebuffer_set_cursor(&pointer); 	
    SDL_ShowCursor(SDL_DISABLE);
#endif
	
	return 0;
}

static int
fb_localhistory_btn_clik(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct gui_window *gw = cbi->context;

	fbtk_set_bitmap(widget,  load_bitmap(add_theme_path("history_h.png")));
	
	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap(add_theme_path("history.png")));	
		
		fb_localhistory_map(gw->localhistory);
			
		return 0;
	}
	return 1;
}


static int
set_back_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	

	if (browser_window_back_available(bw)) {
		if (cbi->type==FBTK_CBT_POINTERLEAVE) {
			fbtk_set_bitmap(widget, load_bitmap(add_theme_path("back.png")));
			return 0;
		}	
		fbtk_set_bitmap(widget, load_bitmap(add_theme_path("back_h.png")));
	}
	gui_window_set_status(g2, messages_get("Back"));

	return 0;
}

static int
set_forward_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
		
	if (browser_window_forward_available(bw)) {
		if (cbi->type==FBTK_CBT_POINTERLEAVE) {
			fbtk_set_bitmap(widget, load_bitmap(add_theme_path("forward.png")));
		return 0;
		}
		fbtk_set_bitmap(widget, load_bitmap(add_theme_path("forward_h.png")));
	}
	gui_window_set_status(g2, messages_get("Forward"));

	return 0;
}

static int
set_close_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	gui_window_set_status(g2, messages_get("Quit"));

	return 0;
}

static int
set_stop_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	if (cbi->type==FBTK_CBT_POINTERLEAVE) {
		fbtk_set_bitmap(widget, load_bitmap(add_theme_path("stop.png")));
		return 0;
	}
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("stop_h.png")));
	
	gui_window_set_status(g2, messages_get("Stop"));

	return 0;
}


static int
set_local_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	if (cbi->type==FBTK_CBT_POINTERLEAVE) {
		fbtk_set_bitmap(widget, load_bitmap(add_theme_path("history.png")));
		return 0;
	}
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("history_h.png")));
	
	gui_window_set_status(g2, messages_get("History"));

	return 0;
}
#ifdef RTG	
static int
set_reload_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	if (cbi->type==FBTK_CBT_POINTERLEAVE) {
		fbtk_set_bitmap(widget, load_bitmap(add_theme_path("reload.png")));
		return 0;
	}
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("reload_h.png")));
	
	gui_window_set_status(g2, messages_get("Redraw"));

	return 0;
}

static int
set_home_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	if (cbi->type==FBTK_CBT_POINTERLEAVE) {
		fbtk_set_bitmap(widget, load_bitmap(add_theme_path("home.png")));
		return 0;
	}
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("home_h.png")));
	
	gui_window_set_status(g2, messages_get("Home"));

	return 0;
}
#endif

static int
set_add_fav_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	if (cbi->type==FBTK_CBT_POINTERLEAVE) {
		fbtk_set_bitmap(widget, load_bitmap(add_theme_path("add_fav.png")));
		return 0;
	}
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("add_fav_h.png")));
	
	gui_window_set_status(g2, messages_get("FavouriteAdd"));

	return 0;
}

static int
set_add_bookmark_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	if (cbi->type==FBTK_CBT_POINTERLEAVE) {
		fbtk_set_bitmap(widget, load_bitmap(add_theme_path("add_bookmark.png")));
		return 0;
	}
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("add_bookmark_h.png")));
	
	gui_window_set_status(g2, messages_get("HotlistAdd"));

	return 0;
}

static int
set_bookmarks_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	if (cbi->type==FBTK_CBT_POINTERLEAVE) {
		fbtk_set_bitmap(widget, load_bitmap(add_theme_path("bookmarks.png")));
		return 0;
	}
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("bookmarks_h.png")));
	
	gui_window_set_status(g2, messages_get("HotlistShowNS"));

	return 0;
}

static int
set_search_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	gui_window_set_status(g2, messages_get("SearchProvider"));

	return 0;
}

static int
set_prefs_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	if (cbi->type==FBTK_CBT_POINTERLEAVE) {
		fbtk_set_bitmap(widget, load_bitmap(add_theme_path("prefs.png")));
		return 0;
	}
	
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("prefs_h.png")));
	
	gui_window_set_status(g2, messages_get("SettingsEdit"));

	return 0;
}

static int
set_getpage_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	if (cbi->type==FBTK_CBT_POINTERLEAVE) {
		fbtk_set_bitmap(widget, load_bitmap(add_theme_path("getpage.png")));
		return 0;
	}

	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("getpage_h.png")));
	
	gui_window_set_status(g2, messages_get("GetURL"));

	return 0;
}

static int
set_javascript_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	if (cbi->type==FBTK_CBT_POINTERLEAVE) {
		if (nsoption_bool(enable_javascript))
			fbtk_set_bitmap(widget, load_bitmap(add_theme_path("java.png")));	
		else
			fbtk_set_bitmap(widget, load_bitmap(add_theme_path("no_java.png")));	
		return 0;
	}
	
	if (nsoption_bool(enable_javascript))
		fbtk_set_bitmap(widget, load_bitmap(add_theme_path("java_h.png")));
	else
		fbtk_set_bitmap(widget, load_bitmap(add_theme_path("no_java_h.png")));		
	gui_window_set_status(g2, messages_get("EnableJS"));

	return 0;
}

static int
set_sethome_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	if (cbi->type==FBTK_CBT_POINTERLEAVE) {
		fbtk_set_bitmap(widget, load_bitmap(add_theme_path("sethome.png")));
		return 0;
	}
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("sethome_h.png")));
	
	gui_window_set_status(g2, messages_get("HomePageCurrent"));

	return 0;
}

static int
set_getvideo_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	if (cbi->type==FBTK_CBT_POINTERLEAVE) {
		fbtk_set_bitmap(widget, load_bitmap(add_theme_path("getvideo.png")));
		return 0;
	}
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("getvideo_h.png")));

	gui_window_set_status(g2, messages_get("PlayYouTube"));

	return 0;
}

static int
set_paste_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	if (cbi->type==FBTK_CBT_POINTERLEAVE) {
		fbtk_set_bitmap(widget, load_bitmap(add_theme_path("paste.png")));
		return 0;
	}
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("paste_h.png")));
	
	gui_window_set_status(g2, messages_get("PasteURL"));

	return 0;
}

static int
set_copy_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	if (cbi->type==FBTK_CBT_POINTERLEAVE) {
		fbtk_set_bitmap(widget, load_bitmap(add_theme_path("copy.png")));
		return 0;
	}
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("copy_h.png")));
	
	gui_window_set_status(g2, messages_get("CopyURL"));

	return 0;
}
const char *fav_num;
static int
set_fav1_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	gui_window_set_status(g2, (const char *)"Favourite #01");		
	return 0;
}
static int
set_fav2_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	gui_window_set_status(g2, (const char *)"Favourite #02");
	return 0;
}
static int
set_fav3_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	gui_window_set_status(g2, (const char *)"Favourite #03");
	return 0;
}
static int
set_fav4_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	gui_window_set_status(g2, (const char *)"Favourite #04");
	return 0;
}
static int
set_fav5_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	gui_window_set_status(g2, (const char *)"Favourite #05");
	return 0;
}
static int
set_fav6_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	gui_window_set_status(g2, (const char *)"Favourite #06");
	return 0;
}
static int
set_fav7_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	gui_window_set_status(g2, (const char *)"Favourite #07");
	return 0;
}
static int
set_fav8_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	gui_window_set_status(g2,(const char *) "Favourite #08");
	return 0;
}
static int
set_fav9_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	gui_window_set_status(g2, (const char *)"Favourite #09");
	return 0;
}
static int
set_fav10_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	gui_window_set_status(g2, (const char *)"Favourite #10");
	return 0;
}
static int
set_fav11_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	gui_window_set_status(g2,(const char *) "Favourite #11");
	return 0;
}
static int
set_fav12_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	gui_window_set_status(g2, (const char *) "Favourite #12");
	return 0;
}

unsigned short zliczanie(char *text)
{
  char znak;
  unsigned short i;
  int j=0;
  i=0;
  while((znak=text[i++])!='\0')
  {
    if ((znak=='l') || (znak=='i') || (znak=='.'))
		j++;
  }

  return j;
}

unsigned short szerokosc(char *text)
{
  unsigned short szer, dl; 
  dl = strlen(text);
  if (dl > 10)
	text = strndup(text, 10);
  dl = strlen(text);
  szer = (9 * dl) - (zliczanie(text) * 6);
  
#ifdef NOTTF
	if(dl>0)
		szer += 10;
#endif
 
  return szer;
}

int x4,x5,x6,x7,x8,x9,x10,x11,x12 = 0;
int text_w = 0;
	
fbtk_widget_t *	
create_fav_widget(int nr, int xpos, int text_w, fbtk_widget_t *toolbar, struct gui_window *gw){
		
	fbtk_callback status = NULL, click = NULL; 
	fbtk_widget_t *fav = NULL, *label = NULL;
	char *image = NULL, *label_txt = NULL;

	if (text_w==0)
		return 0;
	
	switch (nr) {

		case 5:
			click = fb_fav5_click;
			image = strdup("PROGDIR:Resources/Icons/favicon5.png");
			status = set_fav5_status;
			label_txt = strndup(nsoption_charp(favourite_5_label), 10);
			break;
		case 6:
			click = fb_fav6_click;
			image = strdup("PROGDIR:Resources/Icons/favicon6.png");
			status = set_fav6_status;
			label_txt = strndup(nsoption_charp(favourite_6_label), 10);
			break;
		case 7:
			click = fb_fav7_click;
			image = strdup("PROGDIR:Resources/Icons/favicon7.png");
			status = set_fav7_status;
			label_txt = strndup(nsoption_charp(favourite_7_label), 10);
			break;
		case 8:
			click = fb_fav8_click;
			image = strdup("PROGDIR:Resources/Icons/favicon8.png");
			status = set_fav8_status;
			label_txt = strndup(nsoption_charp(favourite_8_label), 10);
			break;
		case 9:
			click = fb_fav9_click;
			image = strdup("PROGDIR:Resources/Icons/favicon9.png");
			status = set_fav9_status;
			label_txt = strndup(nsoption_charp(favourite_9_label), 10);
			break;
		case 10:
			click = fb_fav10_click;
			image = strdup("PROGDIR:Resources/Icons/favicon10.png");
			status = set_fav10_status;
			label_txt = strndup(nsoption_charp(favourite_10_label), 10);
			break;
		case 11:
			click = fb_fav11_click;
			image = strdup("PROGDIR:Resources/Icons/favicon11.png");
			status = set_fav11_status;
			label_txt = strndup(nsoption_charp(favourite_11_label), 10);
			break;
		case 12:
			click = fb_fav12_click;
			image = strdup("PROGDIR:Resources/Icons/favicon12.png");
			status = set_fav12_status;
			label_txt = strndup(nsoption_charp(favourite_12_label), 10);
			break;
		
		}
		
		fav = fbtk_create_button(toolbar,
						    xpos,
						    30+2,
						    16,
						    16,
						    FB_FRAME_COLOUR,
						    load_bitmap(image),
						    click,
						    gw->bw);
							
			fbtk_set_handler(fav, FBTK_CBT_POINTERENTER, 
				 status, gw->bw);	
	
			label = fbtk_create_text(gw->window,
				      xpos+22,
				      28+2,
				      text_w, 20,
				      FB_FRAME_COLOUR, FB_COLOUR_BLACK,
				      false);
					 					
			fbtk_set_text(label, label_txt);

			fbtk_set_handler(label, FBTK_CBT_CLICK, 
				 click, gw->bw);
			fbtk_set_handler(label, FBTK_CBT_POINTERENTER, 
				 status, gw->bw);

		switch (nr) {
		case 5:		
			fav5 = fav;	
			label5 = label;
			break;
		case 6:		
			fav6 = fav;	
			label6 = label;
			break;
		case 7:		
			fav7 = fav;	
			label7 = label;
			break;
		case 8:		
			fav8 = fav;	
			label8 = label;
			break;
		case 9:		
			fav9 = fav;	
			label9 = label;
			break;
		case 10:		
			fav10 = fav;	
			label10 = label;
			break;
		case 11:		
			fav11 = fav;	
			label11 = label;
			break;
		case 12:		
			fav12 = fav;	
			label12 = label;
			break;
		}
	
	return fav;
}

/** Create a toolbar window and populate it with buttons. 
 *
 * The toolbar layout uses a character to define buttons type and position:
 * b - back
 * l - local history
 * f - forward
 * s - stop 
 * r - refresh
 * u - url bar expands to fit remaining space
 * t - throbber/activity indicator
 * c - close the current window
 *
 * The default layout is "blfsrut" there should be no more than a
 * single url bar entry or behaviour will be undefined.
 *
 * @param gw Parent window 
 * @param toolbar_height The height in pixels of the toolbar
 * @param padding The padding in pixels round each element of the toolbar
 * @param frame_col Frame colour.
 * @param toolbar_layout A string defining which buttons and controls
 *                       should be added to the toolbar. May be empty
 *                       string to disable the bar..
 * 
 */

static fbtk_widget_t *
create_toolbar(struct gui_window *gw, 
	       int toolbar_height, 
	       int padding,
	       colour frame_col,
	       const char *toolbar_layout)
{
	fbtk_widget_t *toolbar;

	fbtk_widget_t *widget;

	int xpos; /* The position of the next widget. */
	int xlhs = 0; /* extent of the left hand side widgets */
	int xdir = 1; /* the direction of movement + or - 1 */
	int width = 0; /* width of widget */
	const char *itmtype; /* type of the next item */

	char *label = strdup("blfrhuvaqetk123456789xwzgdnmyop");
	
	if (toolbar_layout == NULL) {
		toolbar_layout = NSFB_TOOLBAR_DEFAULT_LAYOUT;
	}
		
	LOG("Using toolbar layout %s", toolbar_layout);

	itmtype = toolbar_layout;

	if (*itmtype == 0) {
		return NULL;
	}

	toolbar = fbtk_create_window(gw->window, 0, 0, 0, 
				     toolbar_height, 
				     frame_col);

	if (toolbar == NULL) {
		return NULL;
	}

	fbtk_set_handler(toolbar, 
			 FBTK_CBT_POINTERENTER, 
			 set_ptr_default_move, 
			 NULL);

	unsigned int calc_x = fbtk_get_width(gw->window) - (BUTTON_SIZE*6+12);
	xpos = padding;

	/* loop proceeds creating widget on the left hand side until
	 * it runs out of layout or encounters a url bar declaration
	 * wherupon it works backwards from the end of the layout
	 * untill the space left is for the url bar
	 */
	while ((itmtype >= toolbar_layout) && 
	       (*itmtype != 0) && 
	       (xdir !=0)) {

		LOG("toolbar adding %c", *itmtype);

		switch (*itmtype) {

		case 'b': /* back */
			widget = fbtk_create_button(toolbar, 
						    (xdir == 1) ? xpos : 
						     xpos - 22, 
						    padding, 
						    BUTTON_SIZE, 
						    BUTTON_SIZE, 
						    frame_col, 
						    load_bitmap(add_theme_path("back_g.png")), 
						    fb_leftarrow_click, 
						    gw);
			gw->back = widget; /* keep reference */
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_back_status, gw->bw);
			fbtk_set_handler(widget, FBTK_CBT_POINTERLEAVE, 
				 set_back_status, gw->bw);			
			break;

		case 'l': /* local history */
			widget = fbtk_create_button(toolbar,
						    (xdir == 1) ? xpos : 
						     xpos - 22,
						    padding,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    frame_col,
						    load_bitmap(add_theme_path("history.png")), 
						    fb_localhistory_btn_clik,
						    gw);
			gw->history = widget;	
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_local_status, gw->bw);
			fbtk_set_handler(widget, FBTK_CBT_POINTERLEAVE, 
				 set_local_status, gw->bw);					  
			break;

		case 'f': /* forward */
			widget = fbtk_create_button(toolbar,
						    (xdir == 1)?xpos : 
						    xpos - 22,
						    padding,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    frame_col,
						    load_bitmap(add_theme_path("forward_g.png")),
						    fb_rightarrow_click,
						    gw);
			gw->forward = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, set_forward_status, gw->bw);
			fbtk_set_handler(widget, FBTK_CBT_POINTERLEAVE, set_forward_status, gw->bw);				
			break;

		case 'c': /* close the current window */
#ifndef RTG	
			widget = fbtk_create_button(toolbar,
							80,
						    padding,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    FB_FRAME_COLOUR,
						    load_bitmap(add_theme_path("stop.png")),
						    fb_close_click,
						    gw->bw);
			gw->close = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, set_close_status, gw->bw);
			fbtk_set_handler(widget, FBTK_CBT_POINTERLEAVE, set_close_status, gw->bw);					 
#endif			 
			break;

		case 's': /* stop  */
			widget = fbtk_create_button(toolbar,
						    (xdir == 1)?xpos : 
						    xpos - 22,
						    padding,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    frame_col,
						    load_bitmap(add_theme_path("stop.png")),
						    fb_stop_click,
						    gw->bw);
			gw->stop = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, set_stop_status, gw->bw); 
			fbtk_set_handler(widget, FBTK_CBT_POINTERLEAVE, set_stop_status, gw->bw);					 
			break;

		case 'r': /* reload */
#ifdef RTG	
			widget = fbtk_create_button(toolbar,
						    (xdir == 1)?xpos : 
						    xpos - 22,
						    padding,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    frame_col,
						    load_bitmap(add_theme_path("reload_g.png")),
						    fb_reload_click,
						    gw->bw); 
			gw->reload = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, set_reload_status, gw->bw);
			fbtk_set_handler(widget, FBTK_CBT_POINTERLEAVE, set_reload_status, gw->bw);	
#endif	
			break;

		case 'h': /* home */
#ifdef RTG
			widget = fbtk_create_button(toolbar,
						    (xdir == 1)?xpos : 
						    xpos - 22,
						    padding,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    frame_col,
						    load_bitmap(add_theme_path("home.png")),
						    fb_home_click,
						    gw->bw);
			home = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, set_home_status, gw->bw);
			fbtk_set_handler(widget, FBTK_CBT_POINTERLEAVE, set_home_status, gw->bw);		
#endif									
			break;
			
		case 'u': /* url bar*/
			width = fbtk_get_width(gw->window) - xpos - 287;
#ifndef RTG
			width = width + 88;
			xpos = xpos - 47;
#else
			width = width - 9;
#endif			
			widget = fbtk_create_writable_text(toolbar,
						    xpos,
						    padding,
						    width,
						    23,
						    FB_COLOUR_WHITE,
							FB_COLOUR_BLACK,
							true,
						    fb_url_enter,
						    gw->bw);
							
			fbtk_set_handler(widget, 
					 FBTK_CBT_POINTERENTER, 
					 fb_url_move, gw->bw);
					 
			gw->url = widget;
			break;

		case 'v': /* add to favourites button */
			widget = fbtk_create_button(toolbar,
						    fbtk_get_width(gw->window) - throbber00.width - 261
#ifndef RTG		
    						+40
#else
						    - 9
#endif							
						     ,
						    padding,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    frame_col,
						    load_bitmap(add_theme_path("add_fav.png")),
						    fb_add_fav_click,
						    gw->bw);
			addfav = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_add_fav_status, gw->bw);
			fbtk_set_handler(widget, FBTK_CBT_POINTERLEAVE, 
				 set_add_fav_status, gw->bw);					 
			break;
			
		case 'a': /* add to bookmarks button */
			widget = fbtk_create_button(toolbar,
						    fbtk_get_width(gw->window) - throbber00.width - 237
#ifndef RTG		
						   +40
#else
    						- 8
#endif								
							,
						    padding,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    frame_col,
						    load_bitmap(add_theme_path("add_bookmark.png")),
						    fb_add_bookmark_click,
						    gw->bw);
			addbook = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_add_bookmark_status, gw->bw);
			fbtk_set_handler(widget, FBTK_CBT_POINTERLEAVE, 
				 set_add_bookmark_status, gw->bw);	
			break;
			
		case 'q': /* quick search button */
#ifdef RTG		
			spacer1 = fbtk_create_bitmap(toolbar,
										fbtk_get_width(gw->window) - throbber00.width - 210-7,
										padding + 2,
										2,
										20,
										frame_col, 
										load_bitmap("PROGDIR:Resources/Icons/spacer.png"));
#endif						
			widget = fbtk_create_button(toolbar,
						    fbtk_get_width(gw->window) - throbber00.width - 210
#ifndef RTG		
							+40
#endif								
							,
						    padding + 4,
						    16,
						    16,
						    frame_col,
						    load_bitmap("PROGDIR:Resources/Icons/search.png"),
						    fb_search_click,
						    gw->bw);
			quick = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_search_status, gw->bw);
			fbtk_set_handler(widget, FBTK_CBT_POINTERLEAVE, 
				 set_search_status, gw->bw);					 
			break;

		case 'e': /* quick search bar*/
			widget = fbtk_create_writable_text(toolbar,
						    xpos
#ifdef RTG
						    + 9
#endif
						    ,
						    padding,
						    185
#ifndef RTG		
							-40
#endif								
							,
						    23,
						    FB_COLOUR_WHITE,
							FB_COLOUR_BLACK,
							true,
						    fb_searchbar_enter,
						    gw->bw);		   
			searchbar = widget;
			fbtk_set_handler(widget, 
					 FBTK_CBT_POINTERENTER, 
					 fb_url_move, gw->bw);
			break;
			
		case 't': /* throbber/activity indicator */
			widget = fbtk_create_bitmap(toolbar,
						    fbtk_get_width(gw->window) - throbber00.width - 3,
						    padding,
						    throbber00.width,
						    throbber00.height,
						    frame_col, 
						    &throbber00);
						    //load_bitmap(add_theme_path("throbber/throbber00.png")));
			gw->throbber = widget;
#ifdef RTG			
			if (nsoption_int(fullscreen) == 1) {
#endif
					fbtk_set_handler(widget, 
							 FBTK_CBT_CLICK, 
							 throbber_click, gw->bw);	

					fbtk_set_handler(widget, 
							 FBTK_CBT_POINTERENTER, 
							 set_throbber_status, gw->bw);
#ifdef RTG		
			}
#endif			
			break;

		case 'k': /* open bookmarks button */
			xpos = 2;
			widget = fbtk_create_button(toolbar,
						    xpos,
						    padding + 27,
						    24,
						    24,
						    frame_col,
						    load_bitmap(add_theme_path("bookmarks.png")),
						    fb_bookmarks_click,
						    gw->bw);
							
			fbtk_set_handler(widget, 
					 FBTK_CBT_POINTERENTER,
					 set_bookmarks_status, gw->bw);	
			fbtk_set_handler(widget, FBTK_CBT_POINTERLEAVE, 
				 set_bookmarks_status, gw->bw);
//#ifdef RTG		
			spacer2 = fbtk_create_bitmap(toolbar,
										xpos+27,
										padding + 29,
										2,
										20,
										frame_col, 
										load_bitmap("PROGDIR:Resources/Icons/spacer.png"));
//#endif					 
			break;
			
		case '1': /* fav 1 button */
			text_w = szerokosc(nsoption_charp(favourite_1_label));
			xpos= xpos + 4;
//#ifdef RTG
			xpos = xpos + 6;
//#endif				
			fav1 = fbtk_create_button(toolbar,
						    xpos,
						    padding + 31,
						    16,
						    16,
						    frame_col,
						    load_bitmap("PROGDIR:Resources/Icons/favicon1.png"),
						    fb_fav1_click,
						    gw->bw);
			fbtk_set_handler(fav1, FBTK_CBT_POINTERENTER, 
				 set_fav1_status, gw->bw);
				 
			widget = fbtk_create_text(gw->window,
				      xpos+22,
				      padding + 29,
				      text_w, 20,
				      FB_FRAME_COLOUR, FB_COLOUR_BLACK,
				      false);
					 			
			label = strndup(nsoption_charp(favourite_1_label), 10);
			fbtk_set_text(widget, label);
			
			fbtk_set_handler(widget, FBTK_CBT_CLICK, 
				 fb_fav1_click, gw->bw);
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_fav1_status, gw->bw);	 
			label1 = widget;
			break;

		case '2': /* fav 2 button */
			xpos = xpos + 30;
			text_w = szerokosc(nsoption_charp(favourite_2_label));
			fav2 = fbtk_create_button(toolbar,
						    xpos,
						    padding + 31,
						    16,
						    16,
						    frame_col,
						    load_bitmap("PROGDIR:Resources/Icons/favicon2.png"),
						    fb_fav2_click,
						    gw->bw);
			fbtk_set_handler(fav2, FBTK_CBT_POINTERENTER, 
				 set_fav2_status, gw->bw);
				 
			widget = fbtk_create_text(gw->window,
				      xpos+22,
				      padding + 29,
				      text_w, 20,
				      FB_FRAME_COLOUR, FB_COLOUR_BLACK,
				      false);
					 			
			label = strndup(nsoption_charp(favourite_2_label), 10);	
			fbtk_set_text(widget, label);
			
			fbtk_set_handler(widget, FBTK_CBT_CLICK, 
				 fb_fav2_click, gw->bw);
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_fav2_status, gw->bw);
			label2 = widget; 
			break;
			
		case '3': /* fav 3 button */
			xpos = xpos + 30;
			text_w = szerokosc(nsoption_charp(favourite_3_label));
			fav3 = fbtk_create_button(toolbar,
						    xpos,
						    padding + 31,
						    16,
						    16,
						    frame_col,
						    load_bitmap("PROGDIR:Resources/Icons/favicon3.png"),
						    fb_fav3_click,
						    gw->bw);
			fbtk_set_handler(fav3, FBTK_CBT_POINTERENTER, 
				 set_fav3_status, gw->bw);	
				 
			widget = fbtk_create_text(gw->window,
				      xpos+22,
				      padding + 29,
				      text_w, 20,
				      FB_FRAME_COLOUR, FB_COLOUR_BLACK,
				      false);
					 			
			label = strndup(nsoption_charp(favourite_3_label), 10);
			fbtk_set_text(widget, label);
			
			fbtk_set_handler(widget, FBTK_CBT_CLICK, 
				 fb_fav3_click, gw->bw);
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_fav3_status, gw->bw);
			label3 = widget; 
			 break;

		case '4': /* fav 4 button */
			xpos = x4 = xpos + 30;
			text_w = text_w4 = szerokosc(nsoption_charp(favourite_4_label));
			fav4 = fbtk_create_button(toolbar,
						    xpos,
						    padding + 31,
						    16,
						    16,
						    frame_col,
						    load_bitmap("PROGDIR:Resources/Icons/favicon4.png"),
						    fb_fav4_click,
						    gw->bw);
			fbtk_set_handler(fav4, FBTK_CBT_POINTERENTER, 
				 set_fav4_status, gw->bw);	
				 
			widget = fbtk_create_text(gw->window,
				      xpos+22,
				      padding + 29,
				      text_w, 20,
				      FB_FRAME_COLOUR, FB_COLOUR_BLACK,
				      false);
					 			
			label = strndup(nsoption_charp(favourite_4_label), 10);
			fbtk_set_text(widget, label);
			
			fbtk_set_handler(widget, FBTK_CBT_CLICK, 
				 fb_fav4_click, gw->bw);
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_fav4_status, gw);	 
			label4 = widget; 				 
			break;

		case '5': /* fav 5 button */
			text_w5 = szerokosc(nsoption_charp(favourite_5_label));
			x5 = x4 + text_w4 + 31;
			if (nsoption_int(window_width) - 230 < (xpos) || text_w5 == 0) break;		
			fav5 = create_fav_widget(5, x5, text_w5, toolbar, gw); 				 
			break;

		case '6': /* fav 6 button */
#ifdef RTG
			text_w6 = szerokosc(nsoption_charp(favourite_6_label));
			x6 = x5 + text_w5 + 31;
			if (nsoption_int(window_width) - 250 < (xpos) || text_w6 == 0) break;	
			fav6 = create_fav_widget(6, x6, text_w6, toolbar, gw);
#endif			
			break;

		case '7': /* fav 7 button */	
#ifdef RTG		
			xpos = x7 = x6 + text_w6 + 31;
			text_w = text_w7 = szerokosc(nsoption_charp(favourite_7_label));	
			if (nsoption_int(window_width) - 250 < (xpos) || text_w7 == 0) break;
			fav7 = create_fav_widget(7, xpos, text_w, toolbar, gw);
#endif			
			break;

		case '8': /* fav 8 button */
#ifdef RTG
			xpos = x8 = x7 + text_w7 + 31;
			text_w = text_w8 = szerokosc(nsoption_charp(favourite_8_label));	
			if (nsoption_int(window_width) - 250 < (xpos) || (text_w8 == 0)) break;
			fav8 = create_fav_widget(8, xpos, text_w, toolbar, gw);
#endif			
			break;

		case '9': /* fav 9 button */
#ifdef RTG
			xpos = x9 = x8 + text_w8 + 31;
			text_w = text_w9 = szerokosc(nsoption_charp(favourite_9_label));
			if (nsoption_int(window_width) - 250 < (xpos) || (text_w9 == 0)) break;
			fav9 = create_fav_widget(9, xpos, text_w, toolbar, gw);
#endif			
			break;

		case 'x': /* fav 10 button */
#ifdef RTG
			xpos = x10 = x9 + text_w9 + 31;
			text_w = text_w10 = szerokosc(nsoption_charp(favourite_10_label));
			if ((nsoption_int(window_width) - 250 < (xpos)) || (text_w10 == 0)) break;
			fav10 = create_fav_widget(10, xpos, text_w, toolbar, gw);
#endif			
			break;

		case 'w': /* fav 11 button */
#ifdef RTG
			xpos = x11 = x10 + text_w10 + 31;
			text_w = text_w11 = szerokosc(nsoption_charp(favourite_11_label));
			if (nsoption_int(window_width) - 250 < (xpos) || (text_w11 == 0)) break;
			fav11 = create_fav_widget(11, xpos, text_w, toolbar, gw);
#endif			
			break;

		case 'z': /* fav 12 button */
#ifdef RTG
			xpos = x12 = x11 + text_w11 + 31;
			text_w = text_w12 = szerokosc(nsoption_charp(favourite_12_label));
			if (nsoption_int(window_width) - 250 < (xpos) || (text_w12 == 0)) break;
			fav12 = create_fav_widget(12, xpos, text_w, toolbar, gw);
#endif			
			break;
		
		case 'g': /* edit preferences file button */
			xpos = calc_x;
#ifndef RTG		
			xpos = xpos	+ 20;
//#else
#endif
		spacer3 = fbtk_create_bitmap(toolbar,
										xpos-7,
										padding + 29,
										2,
										20,
										frame_col, 
										load_bitmap("PROGDIR:Resources/Icons/spacer.png"));
				
			widget = fbtk_create_button(toolbar,
						    xpos,
						    padding + 27,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    frame_col,
						    load_bitmap(add_theme_path("prefs.png")),
						    fb_prefs_click,
						    gw->bw);
			prefs = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_prefs_status, gw->bw);
			fbtk_set_handler(widget, FBTK_CBT_POINTERLEAVE, 
				 set_prefs_status, gw->bw);					 
			break;
#ifdef JS			
		case 'd': /* javascript button */
			widget = fbtk_create_button(toolbar,
						    xpos,
						    padding + 27,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    frame_col,
						    load_bitmap(add_theme_path("java.png")),
						    fb_javascript_click,
						    gw->bw);
			javascript = widget;

			if (!nsoption_bool(enable_javascript))
				fbtk_set_bitmap(widget, load_bitmap(add_theme_path("no_java.png")));

			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_javascript_status, gw->bw);
			fbtk_set_handler(widget, FBTK_CBT_POINTERLEAVE, 
				 set_javascript_status, gw->bw);					 
			break;
#else
		case 'd': /* download page or website button */
			widget = fbtk_create_button(toolbar,
						    xpos,
						    padding + 27,
						    22,
						    22,
						    frame_col,
						    load_bitmap(add_theme_path("getpage.png")),
						    fb_getpage_click,
						    gw->bw);
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_getpage_status, gw->bw);	
			fbtk_set_handler(widget, FBTK_CBT_POINTERLEAVE, 
				 set_getpage_status, gw->bw);	

			javascript = widget;
			break;	
#endif
		case 'm': /* set home button */
			widget = fbtk_create_button(toolbar,
						    xpos,
						    padding + 27,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    frame_col,
						    load_bitmap(add_theme_path("sethome.png")),
						    fb_sethome_click,
						    gw->bw);
			sethome = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_sethome_status, gw->bw);
			fbtk_set_handler(widget, FBTK_CBT_POINTERLEAVE, 
				 set_sethome_status, gw->bw);					 
			break;	

		case 'y': /* getvideo button */
			widget = fbtk_create_button(toolbar,
						    xpos,
						    padding + 27,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    frame_col,
						    load_bitmap(add_theme_path("getvideo.png")),
						    fb_getvideo_click,
						    gw->bw);
			getvideo = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_getvideo_status, gw->bw);
			fbtk_set_handler(widget, FBTK_CBT_POINTERLEAVE, 
				 set_getvideo_status, gw->bw);					 
			break;

		case 'o': /* copy text button */
			widget = fbtk_create_button(toolbar,
						    xpos,
						    padding + 27,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    frame_col,
						    load_bitmap(add_theme_path("copy.png")),
						    fb_copy_click,
						    gw->bw);
			copy = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_copy_status, gw->bw);
			fbtk_set_handler(widget, FBTK_CBT_POINTERLEAVE, 
				 set_copy_status, gw->bw);				 
			break;
			
		case 'p': /* paste text button */
			widget = fbtk_create_button(toolbar,
						    xpos,
						    padding + 27,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    frame_col,
						    load_bitmap(add_theme_path("paste.png")),
						    fb_paste_click,
						    gw->bw);
			paste = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_paste_status, gw->bw);
			fbtk_set_handler(widget, FBTK_CBT_POINTERLEAVE, 
				 set_paste_status, gw->bw);				 
			/* toolbar is complete */
			xdir = 0; 
			break;	
		
			/* met url going forwards, note position and
			 * reverse direction 
			 */
			itmtype = toolbar_layout + strlen(toolbar_layout);
			xdir = -1;
			xlhs = xpos;
			xpos = (1 * fbtk_get_width(toolbar)); /* ?*/
			widget = toolbar;
	
			break;

		default:
			widget = NULL;
			xdir = 0;
			LOG("Unknown element %c in toolbar layout", *itmtype);
		        break;

		}

		if (widget != NULL) {
			xpos += (xdir * (fbtk_get_width(widget) + padding));
		}

		LOG("xpos is %d",xpos);

		itmtype += xdir;
	}

	fbtk_set_mapping(toolbar, true);

	return toolbar;
}


/** Resize a toolbar.
 *
 * @param gw Parent window
 * @param toolbar_height The height in pixels of the toolbar
 * @param padding The padding in pixels round each element of the toolbar
 * @param toolbar_layout A string defining which buttons and controls
 *                       should be added to the toolbar. May be empty
 *                       string to disable the bar.
 */
static void
resize_toolbar(struct gui_window *gw,
	       int toolbar_height,
	       int padding,
	       const char *toolbar_layout)
{
	fbtk_widget_t *widget;

	int xpos; /* The position of the next widget. */
	int xlhs = 0; /* extent of the left hand side widgets */
	int xdir = 1; /* the direction of movement + or - 1 */
	const char *itmtype; /* type of the next item */
	unsigned int x = 0, y = 0, w = 0, h = 0;

	if (gw->toolbar == NULL)
		return;

	if (toolbar_layout == NULL)
		toolbar_layout = NSFB_TOOLBAR_DEFAULT_LAYOUT;
	
	unsigned int calc_favs = (fbtk_get_width(gw->window) - 100) / 100;
	unsigned int calc_x = fbtk_get_width(gw->window) - (BUTTON_SIZE*6+12);

#ifdef NOTTF
calc_favs = calc_favs -1;
#endif

	if (calc_favs > 12) calc_favs = 12;

	switch (calc_favs) {
	case 12:
		toolbar_layout = strdup("blfrhuvaqetk123456789xwzgdnmyop");
		break;
	case 11:
		toolbar_layout = strdup("blfrhuvaqetk123456789xwgdnmyop");
		if (fav12 != NULL) {
			fbtk_destroy_widget(fav12);
			fbtk_destroy_widget(label12);
			fav12 = NULL;
			label12 = NULL;}		
		break;	
	case 10:
		toolbar_layout = strdup("blfrhuvaqetk123456789xgdnmyop");
		if (fav12 != NULL) {
			fbtk_destroy_widget(fav12);
			fbtk_destroy_widget(label12);
			fav12 = NULL;
			label12 = NULL;}			
		if (fav11 != NULL) {
			fbtk_destroy_widget(fav11);
			fbtk_destroy_widget(label11);
			fav11 = NULL;
			label11 = NULL;}		
		break;
	case 9:
		toolbar_layout = strdup("blfrhuvaqetk123456789gdnmyop");
		if (fav12 != NULL) {
			fbtk_destroy_widget(fav12);
			fbtk_destroy_widget(label12);
			fav12 = NULL;
			label12 = NULL;}			
		if (fav11 != NULL) {
			fbtk_destroy_widget(fav11);
			fbtk_destroy_widget(label11);
			fav11 = NULL;
			label11 = NULL;}			
		if (fav10 != NULL) {
			fbtk_destroy_widget(fav10);
			fbtk_destroy_widget(label10);
			fav10 = NULL;
			label10 = NULL;}	
		break;
	case 8:
		toolbar_layout = strdup("blfrhuvaqetk12345678gdnmyop");
		if (fav12 != NULL) {
			fbtk_destroy_widget(fav12);
			fbtk_destroy_widget(label12);
			fav12 = NULL;
			label12 = NULL;}			
		if (fav11 != NULL) {
			fbtk_destroy_widget(fav11);
			fbtk_destroy_widget(label11);
			fav11 = NULL;
			label11 = NULL;}			
		if (fav10 != NULL) {
			fbtk_destroy_widget(fav10);
			fbtk_destroy_widget(label10);
			fav10 = NULL;
			label10 = NULL;}
		if (fav9 != NULL) {
			fbtk_destroy_widget(fav9);
			fbtk_destroy_widget(label9);
			fav9 = NULL;
			label9 = NULL;}
		break;
	case 7:
		toolbar_layout = strdup("blfrhuvaqetk1234567gdnmyop");
		if (fav12 != NULL) {
			fbtk_destroy_widget(fav12);
			fbtk_destroy_widget(label12);
			fav12 = NULL;
			label12 = NULL;}			
		if (fav11 != NULL) {
			fbtk_destroy_widget(fav11);
			fbtk_destroy_widget(label11);
			fav11 = NULL;
			label11 = NULL;}			
		if (fav10 != NULL) {
			fbtk_destroy_widget(fav10);
			fbtk_destroy_widget(label10);
			fav10 = NULL;
			label10 = NULL;}
		if (fav9 != NULL) {
			fbtk_destroy_widget(fav9);
			fbtk_destroy_widget(label9);
			fav9 = NULL;
			label9 = NULL;}
		if (fav8 != NULL) {
			fbtk_destroy_widget(fav8);
			fbtk_destroy_widget(label8);
			fav8 = NULL;
			label8 = NULL;}	
		break;
	case 6:
		toolbar_layout = strdup("blfrhuvaqetk123456gdnmyop");
		if (fav12 != NULL) {
			fbtk_destroy_widget(fav12);
			fbtk_destroy_widget(label12);
			fav12 = NULL;
			label12 = NULL;}			
		if (fav11 != NULL) {
			fbtk_destroy_widget(fav11);
			fbtk_destroy_widget(label11);
			fav11 = NULL;
			label11 = NULL;}			
		if (fav10 != NULL) {
			fbtk_destroy_widget(fav10);
			fbtk_destroy_widget(label10);
			fav10 = NULL;
			label10 = NULL;}
		if (fav9 != NULL) {
			fbtk_destroy_widget(fav9);
			fbtk_destroy_widget(label9);
			fav9 = NULL;
			label9 = NULL;}
		if (fav8 != NULL) {
			fbtk_destroy_widget(fav8);
			fbtk_destroy_widget(label8);
			fav8 = NULL;
			label8 = NULL;}	
		if (fav7 != NULL) {
			fbtk_destroy_widget(fav7);
			fbtk_destroy_widget(label7);
			fav7 = NULL;
			label7 = NULL;}	
		break;
	case 5:
		toolbar_layout = strdup("blfrhuvaqetk12345gdnmyop");
		if (fav12 != NULL) {
			fbtk_destroy_widget(fav12);
			fbtk_destroy_widget(label12);
			fav12 = NULL;
			label12 = NULL;}			
		if (fav11 != NULL) {
			fbtk_destroy_widget(fav11);
			fbtk_destroy_widget(label11);
			fav11 = NULL;
			label11 = NULL;}			
		if (fav10 != NULL) {
			fbtk_destroy_widget(fav10);
			fbtk_destroy_widget(label10);
			fav10 = NULL;
			label10 = NULL;}
		if (fav9 != NULL) {
			fbtk_destroy_widget(fav9);
			fbtk_destroy_widget(label9);
			fav9 = NULL;
			label9 = NULL;}
		if (fav8 != NULL) {
			fbtk_destroy_widget(fav8);
			fbtk_destroy_widget(label8);
			fav8 = NULL;
			label8 = NULL;}	
		if (fav7 != NULL) {
			fbtk_destroy_widget(fav7);
			fbtk_destroy_widget(label7);
			fav7 = NULL;
			label7 = NULL;}	
		if (fav6 != NULL) {
			fbtk_destroy_widget(fav6);
			fbtk_destroy_widget(label6);
			fav6 = NULL;
			label6 = NULL;}	
		break;	
	case 4:
		toolbar_layout = strdup("blfrhuvaqetk1234gdnmyop");
		if (fav12 != NULL) {
			fbtk_destroy_widget(fav12);
			fbtk_destroy_widget(label12);
			fav12 = NULL;
			label12 = NULL;}			
		if (fav11 != NULL) {
			fbtk_destroy_widget(fav11);
			fbtk_destroy_widget(label11);
			fav11 = NULL;
			label11 = NULL;}			
		if (fav10 != NULL) {
			fbtk_destroy_widget(fav10);
			fbtk_destroy_widget(label10);
			fav10 = NULL;
			label10 = NULL;}
		if (fav9 != NULL) {
			fbtk_destroy_widget(fav9);
			fbtk_destroy_widget(label9);
			fav9 = NULL;
			label9 = NULL;}
		if (fav8 != NULL) {
			fbtk_destroy_widget(fav8);
			fbtk_destroy_widget(label8);
			fav8 = NULL;
			label8 = NULL;}	
		if (fav7 != NULL) {
			fbtk_destroy_widget(fav7);
			fbtk_destroy_widget(label7);
			fav7 = NULL;
			label7 = NULL;}	
		if (fav6 != NULL) {
			fbtk_destroy_widget(fav6);
			fbtk_destroy_widget(label6);
			fav6 = NULL;
			label6 = NULL;}	
		if (fav5 != NULL) {
			fbtk_destroy_widget(fav5);
			fbtk_destroy_widget(label5);
			fav5 = NULL;
			label5 = NULL;
			}	
		break;
		}
		
	if (calc_favs < 5 )
		toolbar_layout = strdup("blfrhuvaqetk1234gdnmyop");
				
	itmtype = toolbar_layout;

	if (*itmtype == 0) {
		return;
	}

	fbtk_set_pos_and_size(gw->toolbar, 0, 0, 0, toolbar_height);

	xpos = padding;
	
	/* loop proceeds creating widget on the left hand side until
	 * it runs out of layout or encounters a url bar declaration
	 * wherupon it works backwards from the end of the layout
	 * untill the space left is for the url bar
	 */
	while (itmtype >= toolbar_layout && xdir != 0) {

		switch (*itmtype) {
		case 'b': /* back */
			widget = gw->back;
			x = (xdir == 1) ? xpos : xpos - 22;
			y = padding;
			w = BUTTON_SIZE;
			h = BUTTON_SIZE;
			break;

		case 'l': /* local history */
			widget = gw->history;
			x = (xdir == 1) ? xpos : xpos - 22;
			y = padding;
			w = BUTTON_SIZE;
			h = BUTTON_SIZE;
			break;

		case 'f': /* forward */
			widget = gw->forward;
			x = (xdir == 1) ? xpos : xpos - 22;
			y = padding;
			w = BUTTON_SIZE;
			h = BUTTON_SIZE;
			break;

		case 'c': /* close the current window */
			widget = gw->close;
			x = (xdir == 1) ? xpos : xpos - 22;
			y = padding;
			w = BUTTON_SIZE;
			h = BUTTON_SIZE;
			break;

		case 's': /* stop  */
			widget = gw->stop;
			x = (xdir == 1) ? xpos : xpos - 22;
			y = padding;
			w = BUTTON_SIZE;
			h = BUTTON_SIZE;
			break;

		case 'r': /* reload */
			widget = gw->reload;
			x = (xdir == 1) ? xpos : xpos - 22;
			y = padding;
			w = BUTTON_SIZE;
			h = BUTTON_SIZE;
			break;
			
		case 'h': /* home */
			widget = home;
			x = (xdir == 1) ? xpos : xpos - 22;
			y = padding;
			w = BUTTON_SIZE;
			h = BUTTON_SIZE;
			break;
			
		case 'v': /* add to favourites button */
			widget = addfav;
			x = fbtk_get_width(gw->window) - throbber00.width - 261;
			y = padding;
			w = BUTTON_SIZE;
			h = BUTTON_SIZE; 
#ifdef RTG
			x = x - 9;
#endif			
			break;
			
		case 'a': /* add to bookmarks button */
			widget = addbook;
			x = fbtk_get_width(gw->window) - throbber00.width - 237;
			y = padding;
			w = BUTTON_SIZE;
			h = BUTTON_SIZE; 
#ifdef RTG
			x = x - 8;
#endif	
			break;
			
		case 'q': /* quick search button */
			widget = quick;
			x = fbtk_get_width(gw->window) - throbber00.width - 210;
			y = padding+4;
			w = 16;
			h = 16; 
			
			fbtk_set_pos_and_size(spacer1, x-7, y-2, 2, 20);
			break;

		case 'e':  /* searchbar */
			widget = searchbar;
			x = fbtk_get_width(gw->window) -  throbber00.width - 190;
			y = padding;
			w = 185;
			h = 23;
			break;
			
		case 't': /* throbber/activity indicator */
			widget = gw->throbber;
			x = fbtk_get_width(gw->window) - throbber00.width - 2;
			y = padding;
			w = throbber00.width;
			h = throbber00.height;
			break;

			
		case 'u': /* url bar*/
			if (xdir == -1) {
				/* met the u going backwards add url
				 * now we know available extent
				 */
				widget = gw->url;
				x = 132;
				y = padding;
				w = fbtk_get_width(gw->window) - 420;
#ifdef RTG
				w = w - 9;
#endif	
				h = 23;

				/* toolbar is complete */
				xdir = 0;
				break;

		case '5':
			if (fav5 == NULL && nsoption_charp(favourite_5_label) != NULL){	
				text_w5 = szerokosc(nsoption_charp(favourite_5_label));
				x5 = x4 + text_w4 + 30;
				fav5 = create_fav_widget(5, x5, text_w5, gw->toolbar, gw);
				widget = fav5;
				x = x5;
				y = 32;
				w = 16;
				h = 16;					
			} 			
			break;
	
		case '6':
			if (fav6 == NULL && nsoption_charp(favourite_6_label) != NULL){	
				text_w6 = szerokosc(nsoption_charp(favourite_6_label));
				x6 = x5 + text_w5 + 30;
				fav6 = create_fav_widget(6, x6, text_w6, gw->toolbar, gw);
				widget = fav6;
				x = x6;
				y = 32;
				w = 16;
				h = 16;					
			} 		
			break;
			
		case '7':
			if (fav7 == NULL && nsoption_charp(favourite_7_label) != NULL){	
				text_w7 = szerokosc(nsoption_charp(favourite_7_label));
				x7 = x6 + text_w6 + 30;
				fav7 = create_fav_widget(7, x7, text_w7, gw->toolbar, gw);
				widget = fav7;
				x = x7;
				y = 32;
				w = 16;
				h = 16;					
			} 
			break;
	
		case '8':
			if (fav8 == NULL && nsoption_charp(favourite_8_label) != NULL){	
				text_w8 = szerokosc(nsoption_charp(favourite_8_label));
				x8 = x7 + text_w7 + 30;
				fav8 = create_fav_widget(8, x8, text_w8, gw->toolbar, gw);
				widget = fav8;
				x = x8;
				y = 32;
				w = 16;
				h = 16;					
			} 
			break;
			
		case '9':	
			if (fav9 == NULL && nsoption_charp(favourite_9_label) != NULL){	
				text_w9 = szerokosc(nsoption_charp(favourite_9_label));			
				x9 = x8 + text_w8 + 30;
				fav9 = create_fav_widget(9, x9, text_w9, gw->toolbar, gw);
				widget = fav9;
				x = x9;
				y = 32;
				w = 16;
				h = 16;					
			} 	
			break;
			
		case 'x':
			if (fav10 == NULL && nsoption_charp(favourite_10_label) != NULL){	
				text_w10 = szerokosc(nsoption_charp(favourite_10_label));
				x10 = x9 + text_w9 + 30;
				fav10 = create_fav_widget(10, x10, text_w10, gw->toolbar, gw);
				widget = fav10;
				x = x10;
				y = 32;
				w = 16;
				h = 16;					
			} 
			break;
			
		case 'w':
			if (fav11 == NULL && nsoption_charp(favourite_11_label) != NULL){	
				text_w11 = szerokosc(nsoption_charp(favourite_11_label));
				x11 = x10 + text_w10 + 30;
				fav11 = create_fav_widget(11, x11, text_w11, gw->toolbar, gw);
				widget = fav11;
				x = x11;
				y = 32;
				w = 16;
				h = 16;					
			} 
			break;

		case 'z':
			if (fav12 == NULL && nsoption_charp(favourite_12_label) != NULL){
				text_w12 = szerokosc(nsoption_charp(favourite_12_label));
				x12 = x11 + text_w11 + 30;
				fav12 = create_fav_widget(12, x12, text_w12, gw->toolbar, gw);
				widget = fav12;
				x = x12;
				y = 32;
				w = 16;
				h = 16;					
			} 
			break;
			
		case 'g': /* edit preferences file button */						
			widget = prefs;
			
			x = calc_x;
			y = padding + 27;
			w = BUTTON_SIZE;
			h = BUTTON_SIZE;
			
			fbtk_set_pos_and_size(spacer3, x-5, y+2, 2, 20);
			break;
			
		case 'd': /* javascript button */
			widget = javascript;
			
			x = calc_x+BUTTON_SIZE+2;
			y = padding + 27;
			w = BUTTON_SIZE;
			h = BUTTON_SIZE;
			break;

		case 'm': /* set home button */
			widget = sethome;
			
			x = calc_x+2*BUTTON_SIZE+4;
			y = padding + 27;
			w = BUTTON_SIZE;
			h = BUTTON_SIZE;					
			break;	

		case 'y': /* getvideo button */
			widget = getvideo;
			
			x = calc_x+3*BUTTON_SIZE+5;
			y = padding + 27;
			w = BUTTON_SIZE;
			h = BUTTON_SIZE;	
			break;

		case 'o': /* copy text button */
			widget = copy;
			
			x = calc_x+4*BUTTON_SIZE+7;
			y = padding + 27;
			w = BUTTON_SIZE;
			h = BUTTON_SIZE;
			break;
			
		case 'p': /* paste text button */
			widget = paste;
			
			x = calc_x+5*BUTTON_SIZE+9;
			y = padding + 27;
			w = BUTTON_SIZE;
			h = BUTTON_SIZE;
			break;

		case 'j': /* spacer3 */						
			widget = spacer3;
			
			x = calc_x-5;
			y = padding + 29;
			w = BUTTON_SIZE;
			h = BUTTON_SIZE;
			break;			
			}


			/* met url going forwards, note position and
			 * reverse direction
			 */
			itmtype = toolbar_layout + strlen(toolbar_layout);
			xdir = -1;
			xlhs = xpos;
			w = fbtk_get_width(gw->toolbar);
			xpos = 2 * w;
			widget = gw->toolbar;
			break;

		default:
			widget = NULL;
		        break;

		}
				
		if (widget != NULL) {
			if (widget != gw->toolbar)
				fbtk_set_pos_and_size(widget, x, y, w, h);
			xpos += xdir * (w + padding);
		}
		itmtype += xdir;
	}
}

/** Routine called when "stripped of focus" event occours for browser widget.
 *
 * @param widget The widget reciving "stripped of focus" event.
 * @param cbi The callback parameters.
 * @return The callback result.
 */
static int
fb_browser_window_strip_focus(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	fbtk_set_caret(widget, false, 0, 0, 0, NULL);

	return 0;
}

static void
create_browser_widget(struct gui_window *gw, int toolbar_height, int furniture_width)
{
	struct browser_widget_s *browser_widget;
	browser_widget = calloc(1, sizeof(struct browser_widget_s));

	gw->browser = fbtk_create_user(gw->window,
				       0,
				       toolbar_height,
				       -furniture_width,
				       -furniture_width,
				       browser_widget);

	fbtk_set_handler(gw->browser, FBTK_CBT_REDRAW, fb_browser_window_redraw, gw);
	fbtk_set_handler(gw->browser, FBTK_CBT_DESTROY, fb_browser_window_destroy, gw);
	fbtk_set_handler(gw->browser, FBTK_CBT_INPUT, fb_browser_window_input, gw);
	fbtk_set_handler(gw->browser, FBTK_CBT_CLICK, fb_browser_window_click, gw);

	fbtk_set_handler(gw->browser, FBTK_CBT_STRIP_FOCUS, fb_browser_window_strip_focus, gw);
	fbtk_set_handler(gw->browser, FBTK_CBT_POINTERMOVE, fb_browser_window_move, gw);
}

static void
resize_browser_widget(struct gui_window *gw, int x, int y,
		int width, int height)
{
	fbtk_set_pos_and_size(gw->browser, x, y, width, height);
	browser_window_reformat(gw->bw, false, width, height);
}


static void
create_normal_browser_window(struct gui_window *gw, int furniture_width)
{
	fbtk_widget_t *widget;
	fbtk_widget_t *toolbar;
	int statusbar_width = 0;
	int toolbar_height = nsoption_int(fb_toolbar_size);

	LOG("Normal window");

	gw->window = fbtk_create_window(fbtk, 0, 0, 0, 0, 0);

	statusbar_width = nsoption_int(toolbar_status_size) *
		fbtk_get_width(gw->window) / 10000;
		
	/* toolbar */
	toolbar = create_toolbar(gw, 
				 toolbar_height, 
				 2, 
				 FB_FRAME_COLOUR, 
#ifdef RTG
				 nsoption_charp(fb_toolbar_layout));
#else
				 NULL);
#endif
	gw->toolbar = toolbar;
	
	/* set the actually created toolbar height */
	if (toolbar != NULL) {
		toolbar_height = fbtk_get_height(toolbar);
	} else {
		toolbar_height = 0;
	}

	/* status bar */
	gw->status = fbtk_create_text(gw->window,
				      0,
				      fbtk_get_height(gw->window) - furniture_width,
				      statusbar_width, furniture_width,
				      FB_FRAME_COLOUR, FB_COLOUR_BLACK,
				      false);

	LOG("status bar %p at %d,%d", gw->status, fbtk_get_absx(gw->status), fbtk_get_absy(gw->status));

	/* create horizontal scrollbar */
	gw->hscroll = fbtk_create_hscroll(gw->window,
					  statusbar_width,
					  fbtk_get_height(gw->window) - furniture_width,
					  fbtk_get_width(gw->window) - statusbar_width - furniture_width,
					  furniture_width,
					  nsoption_colour(sys_colour_Scrollbar),
					  FB_FRAME_COLOUR,
					  fb_scroll_callback,
					  gw);

	/* fill bottom right area */

	widget = fbtk_create_fill(gw->window,
					  fbtk_get_width(gw->window) - furniture_width,
					  fbtk_get_height(gw->window) - furniture_width,
					  furniture_width,
					  furniture_width,
					  FB_FRAME_COLOUR);

	fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, set_ptr_default_move, NULL);

	gw->bottom_right = widget;
	
	/* create vertical scrollbar */
	gw->vscroll = fbtk_create_vscroll(gw->window,
					  fbtk_get_width(gw->window) - furniture_width,
					  toolbar_height,
					  furniture_width,
					  fbtk_get_height(gw->window) - toolbar_height - furniture_width,
					  nsoption_colour(sys_colour_Scrollbar),
					  FB_FRAME_COLOUR,
					  fb_scroll_callback,
					  gw);

	/* browser widget */
	create_browser_widget(gw, toolbar_height, nsoption_int(fb_furniture_size));

	/* Give browser_window's user widget input focus */
	fbtk_set_focus(gw->browser);
}

static void
resize_normal_browser_window(struct gui_window *gw, int furniture_width)
{
	bool resized;
	int width, height;
	int statusbar_width;
	int toolbar_height = fbtk_get_height(gw->toolbar);

	/* Resize the main window widget */
	resized = fbtk_set_pos_and_size(gw->window, 0, 0, 0, 0);
	if (!resized)
		return;

	width = fbtk_get_width(gw->window);
	height = fbtk_get_height(gw->window);
	statusbar_width = nsoption_int(toolbar_status_size) * width / 10000;

	resize_toolbar(gw, toolbar_height, 2,
			nsoption_charp(fb_toolbar_layout));
	fbtk_set_pos_and_size(gw->status,
			0, height - furniture_width,
			statusbar_width, furniture_width);
	fbtk_reposition_hscroll(gw->hscroll,
			statusbar_width, height - furniture_width,
			width - statusbar_width - furniture_width,
			furniture_width);
	fbtk_set_pos_and_size(gw->bottom_right,
			width - furniture_width, height - furniture_width,
			furniture_width, furniture_width);
	fbtk_reposition_vscroll(gw->vscroll,
			width - furniture_width,
			toolbar_height, furniture_width,
			height - toolbar_height - furniture_width);
	resize_browser_widget(gw,
			0, toolbar_height,
			width - furniture_width,
			height - furniture_width - toolbar_height);
}

static void gui_window_add_to_window_list(struct gui_window *gw)
{
	gw->next = NULL;
	gw->prev = NULL;

	if (window_list == NULL) {
		window_list = gw;
	} else {
		window_list->prev = gw;
		gw->next = window_list;
		window_list = gw;
	}
}

static void gui_window_remove_from_window_list(struct gui_window *gw)
{
	struct gui_window *list;

	for (list = window_list; list != NULL; list = list->next) {
		if (list != gw)
			continue;

		if (list == window_list) {
			window_list = list->next;
			if (window_list != NULL)
				window_list->prev = NULL;
		} else {
			list->prev->next = list->next;
			if (list->next != NULL) {
				list->next->prev = list->prev;
			}
		}
		break;
	}
}

static struct gui_window *
gui_window_create(struct browser_window *bw,
		struct gui_window *existing,
		gui_window_create_flags flags)
{
	struct gui_window *gw;

	gw = calloc(1, sizeof(struct gui_window));

	if (gw == NULL)
		return NULL;

	/* associate the gui window with the underlying browser window
 	 */
	gw->bw = bw;

	create_normal_browser_window(gw, nsoption_int(fb_furniture_size));
	gw->localhistory = fb_create_localhistory(bw, fbtk, nsoption_int(fb_furniture_size));

	/* map and request redraw of gui window */
	fbtk_set_mapping(gw->window, true);

	/* Add it to the window list */
	gui_window_add_to_window_list(gw);
	
	return gw;
}

static void
gui_window_destroy(struct gui_window *gw)
{
	gui_window_remove_from_window_list(gw);
	fbtk_destroy_widget(gw->window);

	free(gw);
}

static void
gui_window_set_title(struct gui_window *g, const char *title)
{
	if(!title) 
		SDL_WM_SetCaption("NetSurf", "NetSurf");
				
	stitle = strdup(ami_utf8_easy(title));
	
	SDL_WM_SetCaption(stitle, "NetSurf");
		
	LOG("****** %p, %s ********", g, title);
}

void
gui_window_redraw(struct gui_window *g, int x0, int y0, int x1, int y1)
{
	fb_queue_redraw(g->browser, x0, y0, x1, y1);
}

void
gui_window_redraw_window(struct gui_window *g)
{
	fb_queue_redraw(g->browser, 0, 0, fbtk_get_width(g->browser), fbtk_get_height(g->browser) );
	g2 = g;
}

static void
gui_window_update_box(struct gui_window *g, const struct rect *rect)
{
	struct browser_widget_s *bwidget = fbtk_get_userpw(g->browser);
	fb_queue_redraw(g->browser,
			rect->x0 - bwidget->scrollx,
			rect->y0 - bwidget->scrolly,
			rect->x1 - bwidget->scrollx,
			rect->y1 - bwidget->scrolly);
}

static bool
gui_window_get_scroll(struct gui_window *g, int *sx, int *sy)
{
	struct browser_widget_s *bwidget = fbtk_get_userpw(g->browser);
	float scale = browser_window_get_scale(g->bw);

	*sx = bwidget->scrollx / scale;
	*sy = bwidget->scrolly / scale;

	return true;
}

static void
gui_window_set_scroll(struct gui_window *gw, int sx, int sy)
{
	struct browser_widget_s *bwidget = fbtk_get_userpw(gw->browser);
	float scale = browser_window_get_scale(gw->bw);

	assert(bwidget);

	widget_scroll_x(gw, sx * scale, true);
	widget_scroll_y(gw, sy * scale, true);
}

static void
gui_window_get_dimensions(struct gui_window *g,
			  int *width,
			  int *height,
			  bool scaled)
{
	float scale = browser_window_get_scale(g->bw);
	*width = fbtk_get_width(g->browser);
	*height = fbtk_get_height(g->browser);

	if (scaled) {
		*width /= scale;
		*height /= scale;
	}
}

static void
gui_window_update_extent(struct gui_window *gw)
{
	int w, h;
	browser_window_get_extents(gw->bw, true, &w, &h);

	fbtk_set_scroll_parameters(gw->hscroll, 0, w,

			fbtk_get_width(gw->browser), 100);

	fbtk_set_scroll_parameters(gw->vscroll, 0, h,

			fbtk_get_height(gw->browser), 100);
}

void
gui_window_set_status(struct gui_window *g, const char *text)
{
	if (text)
		{
		fbtk_set_text(g->status, text);///
		status_txt = strdup(text);
		}
}

static void
gui_window_set_pointer(struct gui_window *g, gui_pointer_shape shape)
{
	switch (shape) {
	case GUI_POINTER_POINT:
		framebuffer_set_cursor(&hand);
	    SDL_ShowCursor(SDL_DISABLE);
		break;

	case GUI_POINTER_CARET:
		framebuffer_set_cursor(&caret);
		SDL_ShowCursor(SDL_DISABLE);
		break;

	case GUI_POINTER_MENU:
		framebuffer_set_cursor(&menu);
		#ifdef  RTG
			SDL_ShowCursor(SDL_DISABLE);
		#else
			SDL_ShowCursor(SDL_DISABLE);
		#endif     
		break;

	case GUI_POINTER_PROGRESS:
		framebuffer_set_cursor(&progress);
		SDL_ShowCursor(SDL_DISABLE);           
		break;

				
	case GUI_POINTER_MOVE:
		framebuffer_set_cursor(&hand);
		SDL_ShowCursor(SDL_DISABLE); 
		break;
		
    default:	            				
		#ifdef  RTG
			SDL_ShowCursor(SDL_ENABLE);
			framebuffer_set_cursor(&null_image);
		#else
			SDL_ShowCursor(SDL_DISABLE);
			framebuffer_set_cursor(&pointer);
		#endif
		break;      
        }
}

void
gui_window_hide_pointer(struct gui_window *g)
{
	SDL_ShowCursor(SDL_ENABLE);
	framebuffer_set_cursor(&null_image);
}

static nserror
gui_window_set_url(struct gui_window *g, nsurl *url)
{
	fbtk_set_text(g->url, nsurl_access(url)); 	
	return NSERROR_OK;
}

static void
throbber_advance(void *pw)
{
	struct gui_window *g = pw;
	struct fbtk_bitmap *image;

	switch (g->throbber_index) {
	case 0:
		image = &throbber01;
		g->throbber_index = 1;
		break;

	case 1:
		image = &throbber02;
		g->throbber_index = 2;
		break;

	case 2:
		image = &throbber03;
		g->throbber_index = 3;
		break;

	case 3:
		image = &throbber04;
		g->throbber_index = 4;
		break;

	case 4:
		image = &throbber05;
		g->throbber_index = 5;
		break;

	case 5:
		image = &throbber06;
		g->throbber_index = 6;
		break;

	case 6:
		image = &throbber07;
		g->throbber_index = 7;
		break;

	case 7:
		image = &throbber08;

		g->throbber_index = 8;
		break;

	case 8:
		image = &throbber09;
		g->throbber_index = 9;
		break;

	case 9:
		image = &throbber10;
		g->throbber_index = 10;
		break;

	case 10:
		image = &throbber00;
		g->throbber_index = 0;
		break;

	default:
		return;
	}

	if (g->throbber_index >= 0) {
		fbtk_set_bitmap(g->throbber, image);
		framebuffer_schedule(100, throbber_advance, g);	
	}
}

static void
gui_window_start_throbber(struct gui_window *g)
{
	g->throbber_index = 0;
	framebuffer_schedule(100, throbber_advance, g);
}

static void
gui_window_stop_throbber(struct gui_window *gw)
{
	gw->throbber_index = -1;
	fbtk_set_bitmap(gw->throbber, &throbber00);

	fb_update_back_forward(gw);


}

static void
gui_window_remove_caret_cb(fbtk_widget_t *widget)
{
	struct browser_widget_s *bwidget = fbtk_get_userpw(widget);
	int c_x, c_y, c_h;

	if (fbtk_get_caret(widget, &c_x, &c_y, &c_h)) {
		/* browser window already had caret:
		 * redraw its area to remove it first */
		fb_queue_redraw(widget,
				c_x - bwidget->scrollx,
				c_y - bwidget->scrolly,
				c_x + 1 - bwidget->scrollx,
				c_y + c_h - bwidget->scrolly);
	}
}

static void
gui_window_place_caret(struct gui_window *g, int x, int y, int height,
		const struct rect *clip)
{
	struct browser_widget_s *bwidget = fbtk_get_userpw(g->browser);

	/* set new pos */
	fbtk_set_caret(g->browser, true, x, y, height,
			gui_window_remove_caret_cb);

	/* redraw new caret pos */
	fb_queue_redraw(g->browser,
			x - bwidget->scrollx,
			y - bwidget->scrolly,
			x + 1 - bwidget->scrollx,
			y + height - bwidget->scrolly);
}

static void
gui_window_remove_caret(struct gui_window *g)
{
	int c_x, c_y, c_h;

	if (fbtk_get_caret(g->browser, &c_x, &c_y, &c_h)) {
		/* browser window owns the caret, so can remove it */
		fbtk_set_caret(g->browser, false, 0, 0, 0, NULL);
	}
}

static struct gui_download_window *
gui_download_window_create(download_context *ctx, struct gui_window *parent)
{		
	if (Cbi->event->type == NSFB_EVENT_KEY_UP)
		return 0;
	
#ifdef ENABLE_DOWNLOAD
	char *url = strdup(nsurl_access(download_context_get_url(ctx)));
	const char *mime_type = download_context_get_mime_type(ctx);
	
	char *run = AllocVec(1500, MEMF_ANY);
	bool wget = false;
	int nodlpath = 0;
	
	if (getvideo_click) {
		getvideo_click = false;
		mp4ee = true;
		}
//Printf(mime_type);
	if (strcmp(mime_type, "audio/x-mpegurl") == 0) 
		{
			//strlcpy(url, url, strlen(url)-3);
			mime_type = strdup("audio/mpeg");//video
		}
	else if (mp4ee == true) {
			mime_type = strdup("file");
		}
		
	if (((strcmp(mime_type, "audio/mpeg") == 0)||
		(strcmp(mime_type, "audio/x-scpls") == 0)) && (mouse_2_click == 0))
			{	
			strcpy(run, "run > nil: ");
			strcat(run, nsoption_charp(net_player));
			strcat(run, "  "); 
			nodlpath = 1;
			}
	else if ((strncmp(mime_type, "video", 5) == 0) && (mouse_2_click == 0))
			{	
			strcpy(run, "run > nil: ");
			strcat(run, "ffplay ");
			nodlpath = 1;
			}	
	else if (mp4ee==true) {
			strcpy(run, "echo \"C:wget -P");			
			}
	else if (strcmp(nsoption_charp(download_manager), "wget")==0)
			{	
			strcpy(run, "run c:wget -P");
			wget = true;			
			}
	else	if (strcmp(nsoption_charp(download_manager), "wallget")==0)
			{
			#ifdef AGA
			ScreenToFront(Workbench);	
			#endif			
			strcpy(run, "run  > nil: Sys:Rexxc/rx rexx/wallget.rexx ");
			}
	else if (strcmp(nsoption_charp(download_manager), "httpresume")==0)
			strcpy(run, "run  > nil: c:httpresume GUI STARTDIR=");

	if (nodlpath == 0)
		{
		strcat(run, nsoption_charp(download_path));	
		strcat(run, " ");
	}
	strcat(run, url);	
	
	if (mp4ee==true) 
		{
			
		BPTR fh;
		fh = Open("CON:", MODE_NEWFILE);
		
		strcat(run,"  -Oram:video.mp4 --no-check-certificate \" > RAM:script"); 
		SetTaskPri(FindTask(0), -10);
		Execute(run, 0, 0);	
		
		if (nsoption_bool(youtube_autoplay)) {
			Execute("echo \"endcli\" >> ram:script", 0, 0);			
			}
			
		Execute("execute ram:script", fh, 0);		
		Execute("run > nil: C:ffplay ram:video.mp4", 0, 0);			
		Close(fh);	
		Execute("delete > nil: ram:script", 0, 0);		
		}

	if (wget)
		{
		BPTR fh;
		fh = Open("CON:", MODE_NEWFILE);
		SetTaskPri(FindTask(0), -10);
		
		Execute(run, fh, 0);
		Close(fh);
		}
	else	
		{
		SetTaskPri(FindTask(0), -10);
		Execute(run, 0, 0);
		}

	mouse_2_click = 0;	
	mp4ee = false;
	
	free(url);
	FreeVec(run);
#endif
}

nserror 
gui_launch_url(struct nsurl *url)
{
	if ((!strncmp("mailto:", nsurl_access(url), 7)) || (!strncmp("ftp:", nsurl_access(url), 4)))
	{
		if (OpenURLBase == NULL)
			OpenURLBase = OpenLibrary("openurl.library", 6);

		if (OpenURLBase)		{
			URL_OpenA((STRPTR)nsurl_access(url), NULL);
			CloseLibrary(OpenURLBase);
			}
	}
}

static void framebuffer_window_reformat(struct gui_window *gw)
{
	/** @todo if we ever do zooming reformat should be implemented */
	LOG("window:%p", gw);

	/*
	  browser_window_reformat(gw->bw, false, width, height);
	*/


}

static struct gui_window_table framebuffer_window_table = {
	.create = gui_window_create,
	.destroy = gui_window_destroy,
	.redraw = gui_window_redraw_window,
	.update = gui_window_update_box,
	.get_scroll = gui_window_get_scroll,
	.set_scroll = gui_window_set_scroll,
	.get_dimensions = gui_window_get_dimensions,
	.update_extent = gui_window_update_extent,
	.reformat = framebuffer_window_reformat,

	.set_title = gui_window_set_title,
	.set_url = gui_window_set_url,
	.set_status = gui_window_set_status,
	.set_pointer = gui_window_set_pointer,
	.place_caret = gui_window_place_caret,
	.remove_caret = gui_window_remove_caret,
	.start_throbber = gui_window_start_throbber,
	.stop_throbber = gui_window_stop_throbber,
};


static struct gui_browser_table framebuffer_browser_table = {
	.schedule = framebuffer_schedule,
	.launch_url = gui_launch_url,

	.quit = gui_quit,
};

nserror
gui_download_window_data(struct gui_download_window *dw,
			 const char *data,
			 unsigned int size)
{
	return NSERROR_OK;
}

void
gui_download_window_error(struct gui_download_window *dw,
			  const char *error_msg)
{
}

void
gui_download_window_done(struct gui_download_window *dw)
{
}

static struct gui_download_table download_table = {
	.create = gui_download_window_create,
	.data = gui_download_window_data,
	.error = gui_download_window_error,
	.done = gui_download_window_done,
};

struct gui_download_table *amiga_download_table = &download_table;

static STRPTR temp_homepage_url = NULL;
static bool cli_force = false;

struct Library *LocaleBase;

void OpenLibs()
{
#if defined AGA || NOVA_SDL 
	KeymapBase = OpenLibrary("keymap.library", 37);
	CxBase = OpenLibrary("commodities.library", 37);
#endif
#if defined __libnix__ || AGA || NOVA_SDL
 // if (!TimerBase)gettimerbase();
//#if defined __ixemul__

	OpenDevice("timer.device", 0, &timereq, 0);
	TimerBase = timereq.io_Device;
//#endif
#endif	
#ifndef USE_OLD_FETCH
	LocaleBase = OpenLibrary("locale.library",  38);
#endif
}

void CloseLibs()
{
#if defined AGA || NOVA_SDL 
	if (KeymapBase) CloseLibrary(KeymapBase);
	if (CxBase)     CloseLibrary(CxBase);
#endif
#if defined __libnix__  || AGA || NOVA_SDL
#if defined __ixemul__
	if(TimerBase) CloseDevice(&timereq);
#endif	
#endif	
	if(LocaleBase) CloseLibrary(LocaleBase);
}


nsurl *html_default_stylesheet_url;
int
main(int argc, char** argv)
{
	struct browser_window *bw;
	char *options;
	nsurl *url;
	nserror ret;
	nsfb_t *nsfb;
	STRPTR current_user_cache = NULL;
	
	struct netsurf_table framebuffer_table = {
		.browser = &framebuffer_browser_table,
		.window = &framebuffer_window_table,
		.clipboard = framebuffer_clipboard_table,
		.bitmap = framebuffer_bitmap_table,		
		.utf8 = amiga_utf8_table,
#ifdef USE_OLD_FETCH
		.fetch = framebuffer_fetch_table,
#else		
		.fetch = amiga_fetch_table,	
		.file = amiga_file_table,		
#endif		
#ifdef ENABLE_DOWNLOAD	
		.download = amiga_download_table,
#endif		
#ifdef CACHE
		.llcache = filesystem_llcache_table,
#endif
	};

	/* initialise logging. Not fatal if it fails but not much we
	 * can do about it either.
	 */
	nslog_init(nslog_stream_configure, &argc, argv);

	OpenLibs();
	
	/* user options setup */
	ret = nsoption_init(set_defaults, &nsoptions, &nsoptions_default);
	if (ret != NSERROR_OK) {
		die("Options failed to initialise");
	}
	nsoption_commandline(&argc, argv, nsoptions);
	options = strdup("PROGDIR:Resources/Options");
	nsoption_read(options, nsoptions);
	
    ret = netsurf_register(&framebuffer_table);
	if (ret != NSERROR_OK) {
		die("NetSurf operation table failed registration");
	}

	/* message init */	
#ifdef USE_OLD_FETCH	
	char *messages = strdup("PROGDIR:Resources/polski/Messages");
#else	
	char messages[200];
	if (ami_locate_resource(messages, "Messages") == false) {
		ami_misc_fatal_error("Cannot open Messages file");
		return RETURN_FAIL;
	}
#endif
    ret = messages_add_from_file(messages);	
	if (ret != NSERROR_OK)
		fprintf(stderr, "Cannot open Messages file");
	
#ifdef CACHE
	ret = netsurf_init("PROGDIR:Resources/Cache");
#else
	ret = netsurf_init(NULL);
#endif	

	/* Override, since we have no support for non-core SELECT menu */
	nsoption_set_bool(core_select_menu, true);

	if (process_cmdline(argc,argv) != true) /* calls nsoption_commandline */
		die("unable to process command line.\n");

	nsfb = framebuffer_initialise("sdl",  fewidth, feheight,  febpp);
	if (nsfb == NULL)
		die("Unable to initialise framebuffer");

	framebuffer_set_cursor(&null_image);

	if (fb_font_init() == false)
		die("Unable to initialise the font system");
			
	fbtk = fbtk_init(nsfb);

	urldb_load_cookies(nsoption_charp(cookie_file));
	
	/* create an initial browser window */

	LOG("calling browser_window_create");

	ret = nsurl_create(feurl, &url);
	if (ret == NSERROR_OK) {
		ret = browser_window_create(BW_CREATE_HISTORY,
					      url,
					      NULL,
					      NULL,
					      &bw);
		nsurl_unref(url);
	}
	
#ifdef NO_TIMER
	SetTaskPri(FindTask(0), nsoption_int(priority));
#endif
	
	if (ret != NSERROR_OK) {
		warn_user(messages_get_errorcode(ret), 0);
	} else {
		framebuffer_run();

		browser_window_destroy(bw);
	}

#ifdef CACHE	
	if(current_user_cache != NULL) free(current_user_cache);
#endif	
#ifdef USE_OLD_FETCH	
	if (messages != NULL) free(messages);
#endif
	if (status_txt != NULL) free(status_txt);
	if (options != NULL) free(options);
	if (stitle != NULL) free(stitle);
	
	CloseLibs();
	netsurf_exit();

	if (fb_font_finalise() == false)
		LOG("Font finalisation failed.");
		
	/* finalise options */
	nsoption_finalise(nsoptions, nsoptions_default);


	return 0;
}

void gui_resize(fbtk_widget_t *root, int width, int height)
{
	struct gui_window *gw;
	nsfb_t *nsfb = fbtk_get_nsfb(root);

	/* Enforce a minimum */
	if (width < 300)
		width = 300;
	if (height < 200)
		height = 200;

	if (framebuffer_resize(nsfb, width, height, Bpp) == false) {
		return;
	}

	fbtk_set_pos_and_size(root, 0, 0, width, height);

	fewidth = width;
	feheight = height;

	for (gw = window_list; gw != NULL; gw = gw->next) {
		resize_normal_browser_window(gw,
				nsoption_int(fb_furniture_size));
	}

	fbtk_request_redraw(root);
}

/*
 * Local Variables:
 * c-basic-offset:8
 * End:
 */
