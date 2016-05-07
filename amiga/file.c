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

#include <proto/asl.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/icon.h>
#include <workbench/icon.h>

#include "utils/nsoption.h"
#include "utils/file.h"
#include "utils/messages.h"
#include "content/content.h"
#include "content/fetch.h"
#include "desktop/browser.h"
#include "desktop/save_complete.h"
#include "desktop/save_pdf.h"
#include "desktop/save_text.h"
#include "desktop/gui_window.h"

#include "amiga/gui.h"
#include "amiga/bitmap.h"
#include "amiga/download.h"
#include "amiga/file.h"
#include "amiga/filetype.h"
#include "amiga/icon.h"
#include "amiga/iff_dr2d.h"
#include "amiga/misc.h"
#include "amiga/save_pdf.h"
#include "amiga/theme.h"

static struct Hook aslhookfunc;

HOOKF(ULONG, ami_file_asl_mime_hook, struct FileRequester *, fr, struct AnchorPathOld *)
{
	char fname[1024];
	BOOL ret = FALSE;
	char *mt = NULL;
	lwc_string *lwc_mt = NULL;
	lwc_error lerror;
	content_type ct;

	if(msg->ap_Info.fib_DirEntryType > 0) return(TRUE);

	strcpy(fname,fr->fr_Drawer);
	AddPart(fname, msg->ap_Info.fib_FileName,1024);

  	mt = strdup(fetch_filetype(fname));
	lerror = lwc_intern_string(mt, strlen(mt), &lwc_mt);
	if (lerror != lwc_error_ok)
		return FALSE;

	ct = content_factory_type_from_mime_type(lwc_mt);
	lwc_string_unref(lwc_mt);

	if(ct != CONTENT_NONE) ret = TRUE;

	free(mt);
	return ret;
}

void ami_file_open(struct gui_window_2 *gwin)
{
	char *temp;
	nsurl *url;

	if(AslRequestTags(filereq,
			ASLFR_TitleText, messages_get("NetSurf"),
			ASLFR_Window, gwin->win,
			ASLFR_SleepWindow, TRUE,
			ASLFR_Screen, scrn,
			ASLFR_DoSaveMode, FALSE,
			ASLFR_RejectIcons, TRUE,
			ASLFR_FilterFunc, &aslhookfunc,
			TAG_DONE))
	{
		if((temp = AllocVecTagList(1024, NULL)))
		{
			strlcpy(temp, filereq->fr_Drawer, 1024);
			AddPart(temp, filereq->fr_File, 1024);

			if (netsurf_path_to_nsurl(temp, &url) != NSERROR_OK) {
				warn_user("NoMemory", 0);
			} else {
				browser_window_navigate(gwin->gw->bw,
					url,
					NULL,
					BW_NAVIGATE_HISTORY,
					NULL,
					NULL,
					NULL);
				nsurl_unref(url);
			}

			FreeVec(temp);
		}
	}
}

static void ami_file_set_type(const char *path, lwc_string *mime_type)
{
	content_type type = content_factory_type_from_mime_type(mime_type);
	const char *default_type;

	switch(type) {
		case CONTENT_HTML:
			default_type = "html";
			break;
		default:
			default_type = NULL;
			break;
	}

	if (default_type != NULL) {
		struct DiskObject *dobj = NULL;

		dobj = GetIconTags(NULL,ICONGETA_GetDefaultName,default_type,
				    ICONGETA_GetDefaultType,WBPROJECT,
				    TAG_DONE);		
			    
		PutIconTags(path, dobj,
				 ICONPUTA_NotifyWorkbench, TRUE, TAG_DONE);
	}
}

void ami_file_save(int type, char *fname, struct Window *win,
		struct hlcache_handle *object, struct hlcache_handle *favicon,
		struct browser_window *bw)
{
	BPTR lock, fh;
	const char *source_data;
	ULONG source_size;
	struct bitmap *bm;

	ami_update_pointer(win, GUI_POINTER_WAIT);

	if(ami_download_check_overwrite(fname, win, 0)) {
		switch(type) {
			case AMINS_SAVE_SOURCE:
				if((source_data = content_get_source_data(object, &source_size))) {
					BPTR fh;
					if((fh = FOpen(fname, MODE_NEWFILE,0))) {
						FWrite(fh, source_data, 1, source_size);
						FClose(fh);
					}
				}
			break;

			case AMINS_SAVE_TEXT:
				save_as_text(object, fname);
			break;

			case AMINS_SAVE_COMPLETE:
				if((lock = CreateDir(fname))) {
					UnLock(lock);
					save_complete(object, fname, ami_file_set_type);
					amiga_icon_superimpose_favicon(fname, favicon, NULL);
				}
			break;

			case AMINS_SAVE_PDF:
#ifdef WITH_PDF_EXPORT
				if(save_as_pdf(object, fname))
					amiga_icon_superimpose_favicon(fname, favicon, "pdf");
#endif
			break;

			case AMINS_SAVE_IFF:
				if((bm = content_get_bitmap(object))) {
					bm->url = (char *)nsurl_access(hlcache_handle_get_url(object));
					bm->title = (char *)content_get_title(object);
					amiga_bitmap_save(bm, fname, 0);
				}
#ifdef WITH_NS_SVG
				else if(ami_mime_compare(object, "svg") == true) {
					ami_save_svg(object, fname);
				}
#endif
			break;

			case AMINS_SAVE_SELECTION:
				if((source_data = browser_window_get_selection(bw))) {
					if((fh = FOpen(fname, MODE_NEWFILE,0))) {
						FWrite(fh, source_data, 1, strlen(source_data));
						FClose(fh);
					}
					free((void *)source_data);
				}
			break;
		}
		if(object) SetComment(fname, nsurl_access(hlcache_handle_get_url(object)));
	}

	ami_update_pointer(win, GUI_POINTER_DEFAULT);
}

void ami_file_save_req(int type, struct gui_window_2 *gwin,
		struct hlcache_handle *object)
{
	char *fname = AllocVecTagList(1024, NULL);
	char *initial_fname = NULL;
	char *fname_with_ext = NULL;
	bool strip_ext = true;

	if(object) {
		if(type == AMINS_SAVE_SOURCE) strip_ext = false;
		nsurl_nice(hlcache_handle_get_url(object), &initial_fname, strip_ext);
	}

	if(initial_fname != NULL) {
		fname_with_ext = AllocVecTagList(strlen(initial_fname) + 5, NULL); /* 5 = .ext\0 */

		strcpy(fname_with_ext, initial_fname);

		switch(type)
		{
			case AMINS_SAVE_TEXT:
			case AMINS_SAVE_SELECTION:
				strcat(fname_with_ext, ".txt");
			break;
			case AMINS_SAVE_IFF:
				strcat(fname_with_ext, ".iff");
			break;
			case AMINS_SAVE_PDF:
				strcat(fname_with_ext, ".pdf");
			break;
			default:
			break;
		}

		if(initial_fname) free(initial_fname);
	}

	if(AslRequestTags(savereq,
			ASLFR_Window, gwin->win,
			ASLFR_SleepWindow, TRUE,
			ASLFR_TitleText, messages_get("NetSurf"),
			ASLFR_Screen, scrn,
			ASLFR_InitialFile, fname_with_ext ? fname_with_ext : "",
			TAG_DONE))
	{
		strlcpy(fname, savereq->fr_Drawer, 1024);
		AddPart(fname, savereq->fr_File, 1024);

		ami_file_save(type, fname, gwin->win, object, gwin->gw->favicon, gwin->gw->bw);
	}

	if(fname) FreeVec(fname);
	if(fname_with_ext) FreeVec(fname_with_ext);
}

void ami_file_req_init(void)
{
	const char *initial_dir = nsoption_charp(download_dir);
	Tag initial_dir_tag = ASLFR_InitialDrawer;

	if(initial_dir == NULL) initial_dir_tag = TAG_IGNORE;

	filereq = (struct FileRequester *)AllocAslRequest(ASL_FileRequest, NULL);
	savereq = (struct FileRequester *)AllocAslRequestTags(ASL_FileRequest,
							ASLFR_DoSaveMode, TRUE,
							ASLFR_RejectIcons, TRUE,
							initial_dir_tag, initial_dir,
							TAG_DONE);

	aslhookfunc.h_Entry = (void *)&ami_file_asl_mime_hook;
	aslhookfunc.h_SubEntry = NULL;
	aslhookfunc.h_Data = NULL;
}

void ami_file_req_free(void)
{
	FreeAslRequest(filereq);
	FreeAslRequest(savereq);
}

