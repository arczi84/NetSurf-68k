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

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "utils/errors.h"
#include "utils/file.h"
#include "image/bitmap.h"
#include "content/hlcache.h"
#include "content/backing_store.h"

#include "desktop/save_pdf.h"
#include "desktop/download.h"
#include "desktop/searchweb.h"
#include "desktop/gui_download.h"
#include "desktop/gui_fetch.h"
#include "desktop/gui_misc.h"
#include "desktop/gui_window.h"
#include "desktop/gui_search.h"
#include "desktop/gui_clipboard.h"
#include "desktop/gui_utf8.h"
#include "desktop/netsurf.h"

/**
 * The global interface table.
 */
struct netsurf_table *guit = NULL;


static void gui_default_window_set_title(struct gui_window *g, const char *title)
{
}

static nserror gui_default_window_set_url(struct gui_window *g, struct nsurl *url)
{
	return NSERROR_OK;
}

static void gui_default_window_start_throbber(struct gui_window *g)
{
}

static void gui_default_window_stop_throbber(struct gui_window *g)
{
}

static bool gui_default_window_drag_start(struct gui_window *g,
					  gui_drag_type type,
					  const struct rect *rect)
{
	return true;
}

static nserror gui_default_window_save_link(struct gui_window *g,
					 nsurl *url,
					 const char *title)
{
	return NSERROR_OK;
}

static void gui_default_window_set_icon(struct gui_window *g,
					hlcache_handle *icon)
{
}

static void gui_default_window_scroll_visible(struct gui_window *g,
				       int x0, int y0,
				       int x1, int y1)
{
	guit->window->set_scroll(g, x0, y0);
}

static void gui_default_window_new_content(struct gui_window *g)
{
}


static bool gui_default_window_scroll_start(struct gui_window *g)
{
	return true;
}

static void gui_default_window_set_pointer(struct gui_window *g,
					   gui_pointer_shape shape)
{
}

static void gui_default_window_set_status(struct gui_window *g,
					  const char *text)
{
}

static void gui_default_window_place_caret(struct gui_window *g,
					   int x, int y, int height,
					   const struct rect *clip)
{
}

static void gui_default_window_remove_caret(struct gui_window *g)
{
}

static void gui_default_window_create_form_select_menu(struct gui_window *g,
						struct form_control *control)
{
}

static void gui_default_window_file_gadget_open(struct gui_window *g,
						hlcache_handle *hl,
						struct form_control *gadget)
{
}

static void gui_default_window_drag_save_object(struct gui_window *g,
						hlcache_handle *c,
						gui_save_type type)
{
}

static void gui_default_window_drag_save_selection(struct gui_window *g,
						   const char *selection)
{
}

static void gui_default_window_start_selection(struct gui_window *g)
{
}


/** verify window table is valid */
static nserror verify_window_register(struct gui_window_table *gwt)
{
	/* check table is present */
	if (gwt == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	/* check the mandantory fields are set */
	if (gwt->create == NULL) {
		return NSERROR_BAD_PARAMETER;
	}
	if (gwt->destroy == NULL) {
		return NSERROR_BAD_PARAMETER;
	}
	if (gwt->redraw == NULL) {
		return NSERROR_BAD_PARAMETER;
	}
	if (gwt->update == NULL) {
		return NSERROR_BAD_PARAMETER;
	}
	if (gwt->get_scroll == NULL) {
		return NSERROR_BAD_PARAMETER;
	}
	if (gwt->set_scroll == NULL) {
		return NSERROR_BAD_PARAMETER;
	}
	if (gwt->get_dimensions == NULL) {
		return NSERROR_BAD_PARAMETER;
	}
	if (gwt->update_extent == NULL) {
		return NSERROR_BAD_PARAMETER;
	}
	if (gwt->reformat == NULL) {
		return NSERROR_BAD_PARAMETER;
	}


	/* fill in the optional entries with defaults */
	if (gwt->set_title == NULL) {
		gwt->set_title = gui_default_window_set_title;
	}
	if (gwt->set_url == NULL) {
		gwt->set_url = gui_default_window_set_url;
	}
	if (gwt->set_icon == NULL) {
		gwt->set_icon = gui_default_window_set_icon;
	}
	if (gwt->set_status == NULL) {
		gwt->set_status = gui_default_window_set_status;
	}
	if (gwt->set_pointer == NULL) {
		gwt->set_pointer = gui_default_window_set_pointer;
	}
	if (gwt->place_caret == NULL) {
		gwt->place_caret = gui_default_window_place_caret;
	}
	if (gwt->remove_caret == NULL) {
		gwt->remove_caret = gui_default_window_remove_caret;
	}
	if (gwt->start_throbber == NULL) {
		gwt->start_throbber = gui_default_window_start_throbber;
	}
	if (gwt->stop_throbber == NULL) {
		gwt->stop_throbber = gui_default_window_stop_throbber;
	}
	if (gwt->drag_start == NULL) {
		gwt->drag_start = gui_default_window_drag_start;
	}
	if (gwt->save_link == NULL) {
		gwt->save_link = gui_default_window_save_link;
	}
	if (gwt->scroll_visible == NULL) {
		gwt->scroll_visible = gui_default_window_scroll_visible;
	}
	if (gwt->new_content == NULL) {
		gwt->new_content = gui_default_window_new_content;
	}
	if (gwt->scroll_start == NULL) {
		gwt->scroll_start = gui_default_window_scroll_start;
	}
	if (gwt->create_form_select_menu == NULL) {
		gwt->create_form_select_menu =
				gui_default_window_create_form_select_menu;
	}
	if (gwt->file_gadget_open == NULL) {
		gwt->file_gadget_open = gui_default_window_file_gadget_open;
	}
	if (gwt->drag_save_object == NULL) {
		gwt->drag_save_object = gui_default_window_drag_save_object;
	}
	if (gwt->drag_save_selection == NULL) {
		gwt->drag_save_selection = gui_default_window_drag_save_selection;
	}
	if (gwt->start_selection == NULL) {
		gwt->start_selection = gui_default_window_start_selection;
	}

	return NSERROR_OK;
}



static struct gui_download_window *
gui_default_download_create(download_context *ctx, struct gui_window *parent)
{
	return NULL;
}

static nserror gui_default_download_data(struct gui_download_window *dw,
				  const char *data, unsigned int size)
{
	return NSERROR_OK;
}

static void gui_default_download_error(struct gui_download_window *dw,
				const char *error_msg)
{
}

static void gui_default_download_done(struct gui_download_window *dw)
{
}

static struct gui_download_table default_download_table = {
	.create = gui_default_download_create,
	.data = gui_default_download_data,
	.error = gui_default_download_error,
	.done = gui_default_download_done,
};

/** verify download window table is valid */
static nserror verify_download_register(struct gui_download_table *gdt)
{
	/* check table is present */
	if (gdt == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	/* all enties are mandantory */
	if (gdt->create == NULL) {
		return NSERROR_BAD_PARAMETER;
	}
	if (gdt->data == NULL) {
		return NSERROR_BAD_PARAMETER;
	}
	if (gdt->error == NULL) {
		return NSERROR_BAD_PARAMETER;
	}
	if (gdt->done == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	return NSERROR_OK;
}

static void gui_default_get_clipboard(char **buffer, size_t *length)
{
	*buffer = NULL;
	*length = 0;
}

static void gui_default_set_clipboard(const char *buffer, size_t length,
		nsclipboard_styles styles[], int n_styles)
{
}

static struct gui_clipboard_table default_clipboard_table = {
	.get = gui_default_get_clipboard,
	.set = gui_default_set_clipboard,
};

/** verify clipboard table is valid */
static nserror verify_clipboard_register(struct gui_clipboard_table *gct)
{
	/* check table is present */
	if (gct == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	/* optional operations */
	if (gct->get == NULL) {
		gct->get = gui_default_get_clipboard;
	}
	if (gct->set == NULL) {
		gct->set = gui_default_set_clipboard;
	}
	return NSERROR_OK;
}

/**
 * The default utf8 conversion implementation.
 *
 * The default implementation assumes the local encoding is utf8
 * allowing the conversion to be a simple copy.
 *
 * @param [in] string The source string.
 * @param [in] len The \a string length or 0 to compute it.
 * @param [out] result A pointer to the converted string.
 * @result NSERROR_OK or NSERROR_NOMEM if memory could not be allocated.
 */
static nserror gui_default_utf8(const char *string, size_t len, char **result)
{
	assert(string && result);

	if (len == 0)
		len = strlen(string);

	*result = strndup(string, len);
	if (!(*result))
		return NSERROR_NOMEM;

	return NSERROR_OK;
}

static struct gui_utf8_table default_utf8_table = {
	.utf8_to_local = gui_default_utf8,
	.local_to_utf8 = gui_default_utf8,
};

/** verify clipboard table is valid */
static nserror verify_utf8_register(struct gui_utf8_table *gut)
{
	/* check table is present */
	if (gut == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	/* mandantory operations */
	if (gut->utf8_to_local == NULL) {
		return NSERROR_BAD_PARAMETER;
	}
	if (gut->local_to_utf8 == NULL) {
		return NSERROR_BAD_PARAMETER;
	}
	return NSERROR_OK;
}

static void gui_default_status(bool found, void *p)
{
}

static void gui_default_hourglass(bool active, void *p)
{
}

static void gui_default_add_recent(const char *string, void *p)
{
}

static void gui_default_forward_state(bool active, void *p)
{
}

static void gui_default_back_state(bool active, void *p)
{
}

static struct gui_search_table default_search_table = {
	.status = gui_default_status,
	.hourglass = gui_default_hourglass,
	.add_recent = gui_default_add_recent,
	.forward_state = gui_default_forward_state,
	.back_state = gui_default_back_state,
};

/** verify search table is valid */
static nserror verify_search_register(struct gui_search_table *gst)
{
	/* check table is present */
	if (gst == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	/* fill in the optional entries with defaults */
	if (gst->status == NULL) {
		gst->status = default_search_table.status;
	}
	if (gst->hourglass == NULL) {
		gst->hourglass = default_search_table.hourglass;
	}
	if (gst->add_recent == NULL) {
		gst->add_recent = default_search_table.add_recent;
	}
	if (gst->forward_state == NULL) {
		gst->forward_state = default_search_table.forward_state;
	}
	if (gst->back_state == NULL) {
		gst->back_state = default_search_table.back_state;
	}

	return NSERROR_OK;
}

static nserror
gui_default_provider_update(const char *provider_name,
			    struct bitmap *provider_bitmap)
{
	return NSERROR_OK;
}

static struct gui_search_web_table default_search_web_table = {
	.provider_update = gui_default_provider_update,
};

/** verify search table is valid */
static nserror verify_search_web_register(struct gui_search_web_table *gswt)
{
	/* check table is present */
	if (gswt == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	/* mandantory operations */
	if (gswt->provider_update == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	return NSERROR_OK;
}

/** verify low level cache persistant backing store table is valid */
static nserror verify_llcache_register(struct gui_llcache_table *glt)
{
	/* check table is present */
	if (glt == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	/* mandantory operations */
	if (glt->store == NULL) {
		return NSERROR_BAD_PARAMETER;
	}
	if (glt->fetch == NULL) {
		return NSERROR_BAD_PARAMETER;
	}
	if (glt->invalidate == NULL) {
		return NSERROR_BAD_PARAMETER;
	}
	if (glt->release == NULL) {
		return NSERROR_BAD_PARAMETER;
	}
	if (glt->initialise == NULL) {
		return NSERROR_BAD_PARAMETER;
	}
	if (glt->finalise == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	return NSERROR_OK;
}

static nsurl *gui_default_get_resource_url(const char *path)
{
	return NULL;
}

static nserror gui_default_get_resource_data(const char *path, const uint8_t **data, size_t *data_len)
{
	return NSERROR_NOT_FOUND;
}

static nserror gui_default_release_resource_data(const uint8_t *data)
{
	return NSERROR_OK;
}

static char *gui_default_mimetype(const char *path)
{
	return strdup(guit->fetch->filetype(path));
}

/** verify fetch table is valid */
static nserror verify_fetch_register(struct gui_fetch_table *gft)
{
	/* check table is present */
	if (gft == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	/* check the mandantory fields are set */
	if (gft->filetype == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	/* fill in the optional entries with defaults */
	if (gft->get_resource_url == NULL) {
		gft->get_resource_url = gui_default_get_resource_url;
	}
	if (gft->get_resource_data == NULL) {
		gft->get_resource_data = gui_default_get_resource_data;
	}
	if (gft->release_resource_data == NULL) {
		gft->release_resource_data = gui_default_release_resource_data;
	}
	if (gft->mimetype == NULL) {
		gft->mimetype = gui_default_mimetype;
	}

	return NSERROR_OK;
}

/** verify file table is valid */
static nserror verify_file_register(struct gui_file_table *gft)
{
	/* check table is present */
	if (gft == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	/* check the mandantory fields are set */
	if (gft->mkpath == NULL) {
		return NSERROR_BAD_PARAMETER;
	}
	if (gft->basename == NULL) {
		return NSERROR_BAD_PARAMETER;
	}
	if (gft->nsurl_to_path == NULL) {
		return NSERROR_BAD_PARAMETER;
	}
	if (gft->path_to_nsurl == NULL) {
		return NSERROR_BAD_PARAMETER;
	}
	if (gft->mkdir_all == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	return NSERROR_OK;
}

/**
 * verify bitmap table is valid
 *
 * \param gbt The bitmap table to verify.
 * \return NSERROR_OK if teh table is valid else NSERROR_BAD_PARAMETER.
 */
static nserror verify_bitmap_register(struct gui_bitmap_table *gbt)
{
	/* check table is present */
	if (gbt == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	/* check the mandantory fields are set */
	if (gbt->create == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	if (gbt->destroy == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	if (gbt->set_opaque == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	if (gbt->get_opaque == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	if (gbt->test_opaque == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	if (gbt->get_buffer == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	if (gbt->get_rowstride == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	if (gbt->get_width == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	if (gbt->get_height == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	if (gbt->get_bpp == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	if (gbt->save == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	if (gbt->modified == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	if (gbt->render == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	return NSERROR_OK;
}

static void gui_default_quit(void)
{
}


static nserror gui_default_launch_url(struct nsurl *url)
{
	return NSERROR_NO_FETCH_HANDLER;
}


static void gui_default_cert_verify(nsurl *url,
				    const struct ssl_cert_info *certs,
				    unsigned long num,
				    nserror (*cb)(bool proceed, void *pw),
				    void *cbpw)
{
	cb(false, cbpw);
}

static void gui_default_401login_open(nsurl *url, const char *realm,
		nserror (*cb)(bool proceed, void *pw), void *cbpw)
{
	cb(false, cbpw);
}

static void
gui_default_pdf_password(char **owner_pass, char **user_pass, char *path)
{
	*owner_pass = NULL;
	save_pdf(path);
}

/** verify browser table is valid */
static nserror verify_browser_register(struct gui_browser_table *gbt)
{
	/* check table is present */
	if (gbt == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	/* check the mandantory fields are set */
	if (gbt->schedule == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	/* fill in the optional entries with defaults */
	if (gbt->quit == NULL) {
		gbt->quit = gui_default_quit;
	}
	if (gbt->launch_url == NULL) {
		gbt->launch_url = gui_default_launch_url;
	}
	if (gbt->cert_verify == NULL) {
		gbt->cert_verify = gui_default_cert_verify;
	}
	if (gbt->login == NULL) {
		gbt->login = gui_default_401login_open;
	}
	if (gbt->pdf_password == NULL) {
		gbt->pdf_password = gui_default_pdf_password;
	}
	return NSERROR_OK;
}


/* exported interface documented in desktop/netsurf.h */
nserror netsurf_register(struct netsurf_table *gt)
{
	nserror err;

	/* ensure not already initialised */
	if (guit != NULL) {
		return NSERROR_INIT_FAILED;
	}

	/* check table is present */
	if (gt == NULL) {
		return NSERROR_BAD_PARAMETER;
	}

	/* browser table */
	err = verify_browser_register(gt->browser);
	if (err != NSERROR_OK) {
		return err;
	}

	/* window table */
	err = verify_window_register(gt->window);
	if (err != NSERROR_OK) {
		return err;
	}

	/* fetch table */
	err = verify_fetch_register(gt->fetch);
	if (err != NSERROR_OK) {
		return err;
	}

	/* bitmap table */
	err = verify_bitmap_register(gt->bitmap);
	if (err != NSERROR_OK) {
		return err;
	}

	/* file table */
	if (gt->file == NULL) {
		gt->file = default_file_table;
	}
	err = verify_file_register(gt->file);
	if (err != NSERROR_OK) {
		return err;
	}

	/* download table */
	if (gt->download == NULL) {
		/* set default download table */
		gt->download = &default_download_table;
	}
	err = verify_download_register(gt->download);
	if (err != NSERROR_OK) {
		return err;
	}

	/* clipboard table */
	if (gt->clipboard == NULL) {
		/* set default clipboard table */
		gt->clipboard = &default_clipboard_table;
	}
	err = verify_clipboard_register(gt->clipboard);
	if (err != NSERROR_OK) {
		return err;
	}

	/* utf8 table */
	if (gt->utf8 == NULL) {
		/* set default utf8 table */
		gt->utf8 = &default_utf8_table;
	}
	err = verify_utf8_register(gt->utf8);
	if (err != NSERROR_OK) {
		return err;
	}

	/* search table */
	if (gt->search == NULL) {
		/* set default search table */
		gt->search = &default_search_table;
	}
	err = verify_search_register(gt->search);
	if (err != NSERROR_OK) {
		return err;
	}

	/* web search table */
	if (gt->search_web == NULL) {
		/* set default search table */
		gt->search_web = &default_search_web_table;
	}
	err = verify_search_web_register(gt->search_web);
	if (err != NSERROR_OK) {
		return err;
	}

	/* llcache table */
	if (gt->llcache == NULL) {
		/* set default backing store table */
		gt->llcache = null_llcache_table;
	}
	err = verify_llcache_register(gt->llcache);
	if (err != NSERROR_OK) {
		return err;
	}

	guit = gt;

	return NSERROR_OK;
}
