#define MUI_OBSOLETE

#include <libraries/mui.h>

#include <clib/alib_protos.h>
#include <proto/muimaster.h>
#include <proto/exec.h>
#include <proto/intuition.h>


#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

#include "gui_optionsExtern.h"
#include "gui_options.h"


struct ObjApp * CreateApp(void)
{
	struct ObjApp * object;

	APTR	MNlabel0Quit, GROUP_ROOT_0, GR_Tabs, GR_General_Homepage, obj_aux0;
	APTR	obj_aux1, GR_BT_Homepage, BT_Homepage, BT_Homepage_2, BT_Homepage_3;
	APTR	GR_Cont_block_Locale, Space_18, GR_General_Content_blocking, obj_aux2;
	APTR	obj_aux3, Space_2, GR_General_Content_language, obj_aux4, obj_aux5;
	APTR	obj_aux6, obj_aux7, obj_aux8, obj_aux9, Space_23, Space_20, GR_History_Scripting;
	APTR	GR_General_History, Space_17C, Keep_history_for, LA_days, Space_21;
	APTR	GR_Gen_Scripting, Space_19, obj_aux10, obj_aux11, Space_22, GR_Privacy;
	APTR	LA_send_referer, LA_do_not, GR_Screen, obj_aux12, obj_aux13, obj_aux14;
	APTR	obj_aux15, obj_aux16, obj_aux17, obj_aux18, obj_aux19, obj_aux20;
	APTR	obj_aux21, GR_Theme, GR_HTTP_Proxy, GR_Proxy_type, GR_Host, obj_aux22;
	APTR	obj_aux23, obj_aux24, obj_aux25, obj_aux26, obj_aux27, obj_aux28;
	APTR	obj_aux29, obj_aux30, obj_aux31, GR_Fetching, LA_Maximum_fetchers;
	APTR	LA_Maximum_retried_fetches, LA_Fetchers_per_host, LA_Fetch_timeout;
	APTR	LA_Cached_connections, Space_4, Space_15, GR_Images, obj_aux32, obj_aux33;
	APTR	Space_7, obj_aux34, obj_aux35, Space_8, GR_Animations, obj_aux36;
	APTR	obj_aux37, Space_6, Space_9, obj_aux38, obj_aux39, LA_seconds, Space_10;
	APTR	GR_Scaling, LA_scale, LA_scale_aga, GR_CPU_usage, obj_aux40, obj_aux41;
	APTR	GR_Fonts_families, LA_Sans_serif, LA_Serif, LA_Sans_serif_italic;
	APTR	LA_Sans_serif_bold, LA_Sans_serif_bold_italic, LA_Monospace, LA_Cursive;
	APTR	LA_Fantasy, GR_Font_size, LA_Default, LA_pt_2, LA_Minimum, LA_pt_1;
	APTR	LA_label_dpi, obj_aux42, obj_aux43, GR_Memory_cache, obj_aux44, obj_aux45;
	APTR	LA_MB, GR_Disc_cache, GR_Cache_directory, LA_Directory, GR_Disc_cache_slider;
	APTR	obj_aux46, obj_aux47, LA_MBC, GR_Favs, GR_editlist, GR_grp_18, GR_Downloads;
	APTR	LA_Download_manager, LA_Download, GR_Youtube, LA_YouTube_url, GR_Players;
	APTR	LA_MP3, LA_Module, GR_Restart;
#if defined(__amigaos4__)
	static const struct Hook ReloadHook = { { NULL,NULL }, (HOOKFUNC)Reload, NULL, NULL };
#else
	static const struct Hook ReloadHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)Reload, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook RestartHook = { { NULL,NULL }, (HOOKFUNC)Restart, NULL, NULL };
#else
	static const struct Hook RestartHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)Restart, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook SaveHook = { { NULL,NULL }, (HOOKFUNC)Save, NULL, NULL };
#else
	static const struct Hook SaveHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)Save, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook SetUrlHook = { { NULL,NULL }, (HOOKFUNC)SetUrl, NULL, NULL };
#else
	static const struct Hook SetUrlHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)SetUrl, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook FavConstructorHook = { { NULL,NULL }, (HOOKFUNC)FavConstructor, NULL, NULL };
#else
	static const struct Hook FavConstructorHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)FavConstructor, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook FavDestructorHook = { { NULL,NULL }, (HOOKFUNC)FavDestructor, NULL, NULL };
#else
	static const struct Hook FavDestructorHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)FavDestructor, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook FavDisplayerHook = { { NULL,NULL }, (HOOKFUNC)FavDisplayer, NULL, NULL };
#else
	static const struct Hook FavDisplayerHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)FavDisplayer, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook GetFavAttrHook = { { NULL,NULL }, (HOOKFUNC)GetFavAttr, NULL, NULL };
#else
	static const struct Hook GetFavAttrHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)GetFavAttr, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook ModifyFavHook = { { NULL,NULL }, (HOOKFUNC)ModifyFav, NULL, NULL };
#else
	static const struct Hook ModifyFavHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)ModifyFav, NULL };
#endif
#if defined(__amigaos4__)
	static const struct Hook ProxyHook = { { NULL,NULL }, (HOOKFUNC)Proxy, NULL, NULL };
#else
	static const struct Hook ProxyHook = { { NULL,NULL }, HookEntry, (HOOKFUNC)Proxy, NULL };
#endif


	if (!(object = AllocVec(sizeof(struct ObjApp), MEMF_PUBLIC|MEMF_CLEAR)))
		return NULL;

	object->STR_GR_Tabs[0] = "General";
	object->STR_GR_Tabs[1] = "Display";
	object->STR_GR_Tabs[2] = "Connection";
	object->STR_GR_Tabs[3] = "Rendering";
	object->STR_GR_Tabs[4] = "Fonts";
	object->STR_GR_Tabs[5] = "Cache";
	object->STR_GR_Tabs[6] = "Favs";
	object->STR_GR_Tabs[7] = "Other";
	object->STR_GR_Tabs[8] = NULL;
	object->CY_Proxy_typeContent[0] = "No proxy";
	object->CY_Proxy_typeContent[1] = "Simple proxy";
	object->CY_Proxy_typeContent[2] = "Basic authentication";
	object->CY_Proxy_typeContent[3] = NULL;

	object->STR_label_URL = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_label_URL",
		MUIA_String_Contents, "about:welcome",
	End;

	obj_aux1 = Label2("URL");

	obj_aux0 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux1,
		Child, object->STR_label_URL,
	End;

	BT_Homepage = SimpleButton("Use default page");

	BT_Homepage_2 = SimpleButton("Use current page");

	BT_Homepage_3 = SimpleButton("Use Google");

	GR_BT_Homepage = GroupObject,
		MUIA_HelpNode, "GR_BT_Homepage",
		MUIA_Group_Rows, 1,
		MUIA_Group_HorizSpacing, 20,
		Child, BT_Homepage,
		Child, BT_Homepage_2,
		Child, BT_Homepage_3,
	End;

	GR_General_Homepage = GroupObject,
		MUIA_HelpNode, "GR_General_Homepage",
		MUIA_Background, MUII_RequesterBack,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Home page",
		MUIA_Group_Rows, 2,
		Child, obj_aux0,
		Child, GR_BT_Homepage,
	End;

	Space_18 = HVSpace;

	object->CM_Hide_advertisements = CheckMark(TRUE);

	obj_aux3 = Label2("Hide advertisements");

	obj_aux2 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux3,
		Child, object->CM_Hide_advertisements,
	End;

	Space_2 = VSpace(0);

	GR_General_Content_blocking = GroupObject,
		MUIA_HelpNode, "GR_General_Content_blocking",
		MUIA_Background, MUII_RequesterBack,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Content blocking",
		MUIA_Group_SameWidth, TRUE,
		MUIA_Group_HorizSpacing, 0,
		Child, obj_aux2,
		Child, Space_2,
	End;

	object->CM_Get_from_Locale_prefs = CheckMark(TRUE);

	obj_aux5 = Label2("Get from Locale prefs");

	obj_aux4 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux5,
		Child, object->CM_Get_from_Locale_prefs,
	End;

	object->STR_Get_from_Locale_prefs = StringObject,
		MUIA_Disabled, TRUE,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Get_from_Locale_prefs",
		MUIA_String_Contents, "en",
		MUIA_String_MaxLen, 3,
	End;

	obj_aux7 = Label2("");

	obj_aux6 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux7,
		Child, object->STR_Get_from_Locale_prefs,
	End;

	object->STR_charset = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_charset",
		MUIA_String_Contents, "ISO-8859-2",
		MUIA_String_Accept, "-0123456789",
		MUIA_String_MaxLen, 12,
	End;

	obj_aux9 = Label2("Charset");

	obj_aux8 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux9,
		Child, object->STR_charset,
	End;

	Space_23 = HVSpace;

	GR_General_Content_language = GroupObject,
		MUIA_HelpNode, "GR_General_Content_language",
		MUIA_Background, MUII_RequesterBack,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Content language",
		MUIA_Group_Rows, 2,
		MUIA_Group_HorizSpacing, 0,
		Child, obj_aux4,
		Child, obj_aux6,
		Child, obj_aux8,
		Child, Space_23,
	End;

	Space_20 = HVSpace;

	GR_Cont_block_Locale = GroupObject,
		MUIA_HelpNode, "GR_Cont_block_Locale",
		MUIA_Group_Columns, 4,
		MUIA_Group_HorizSpacing, 40,
		Child, Space_18,
		Child, GR_General_Content_blocking,
		Child, GR_General_Content_language,
		Child, Space_20,
	End;

	Space_17C = HVSpace;

	Keep_history_for = Label("Keep history for");

	object->STR_history = StringObject,
		MUIA_Weight, 15,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_history",
		MUIA_String_Contents, "28",
		MUIA_String_Accept, "-0123456789",
		MUIA_String_MaxLen, 3,
		MUIA_String_Format, MUIV_String_Format_Right,
	End;

	LA_days = Label("days");

	Space_21 = HVSpace;

	GR_General_History = GroupObject,
		MUIA_HelpNode, "GR_General_History",
		MUIA_Background, MUII_RequesterBack,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "History",
		MUIA_Group_Columns, 5,
		Child, Space_17C,
		Child, Keep_history_for,
		Child, object->STR_history,
		Child, LA_days,
		Child, Space_21,
	End;

	Space_19 = HSpace(20);

	object->CM_Enable_JavaScript = CheckMark(FALSE);

	obj_aux11 = Label2("Enable JavaScript");

	obj_aux10 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux11,
		Child, object->CM_Enable_JavaScript,
	End;

	Space_22 = HSpace(20);

	GR_Gen_Scripting = GroupObject,
		MUIA_HelpNode, "GR_Gen_Scripting",
		MUIA_Background, MUII_RequesterBack,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Scripting",
		MUIA_Group_Columns, 3,
		Child, Space_19,
		Child, obj_aux10,
		Child, Space_22,
	End;

	GR_History_Scripting = GroupObject,
		MUIA_HelpNode, "GR_History_Scripting",
		MUIA_Group_Columns, 2,
		MUIA_Group_HorizSpacing, 40,
		Child, GR_General_History,
		Child, GR_Gen_Scripting,
	End;

	object->CH_send_referer = ImageObject,
		MUIA_Weight, 0,
		MUIA_InputMode, MUIV_InputMode_Toggle,
		MUIA_Image_Spec, MUII_CheckMark,
		MUIA_Image_FreeVert, TRUE,
		MUIA_Selected, TRUE,
		MUIA_ShowSelState, FALSE,
	End;

	LA_send_referer = Label("Send site referral information                  ");

	object->CH_do_not_track = CheckMark(TRUE);

	LA_do_not = Label("Send header to tell websites not to track ");

	GR_Privacy = GroupObject,
		MUIA_HelpNode, "GR_Privacy",
		MUIA_Background, MUII_RequesterBack,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Privacy",
		MUIA_Group_Rows, 2,
		Child, object->CH_send_referer,
		Child, LA_send_referer,
		Child, object->CH_do_not_track,
		Child, LA_do_not,
	End;

	object->GR_General = GroupObject,
		MUIA_HelpNode, "GR_General",
		MUIA_FramePhantomHoriz, TRUE,
		MUIA_Group_Rows, 4,
		Child, GR_General_Homepage,
		Child, GR_Cont_block_Locale,
		Child, GR_History_Scripting,
		Child, GR_Privacy,
	End;

	object->BT_select = SimpleButton("Screenmode");

	object->STR_width = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_width",
		MUIA_String_Contents, "800",
		MUIA_String_Accept, "-0123456789",
		MUIA_String_MaxLen, 5,
	End;

	obj_aux13 = Label2("Width");

	obj_aux12 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux13,
		Child, object->STR_width,
	End;

	object->STR_height = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_height",
		MUIA_String_Contents, "600",
		MUIA_String_Accept, "-0123456789",
		MUIA_String_MaxLen, 5,
	End;

	obj_aux15 = Label2("Height");

	obj_aux14 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux15,
		Child, object->STR_height,
	End;

	object->STR_depth = StringObject,
		MUIA_Disabled, TRUE,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_depth",
		MUIA_String_Contents, "32",
		MUIA_String_Accept, "-0123456789",
		MUIA_String_MaxLen, 3,
	End;

	obj_aux17 = Label2("Depth");

	obj_aux16 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux17,
		Child, object->STR_depth,
	End;

	object->CH_autodepth = CheckMark(TRUE);

	obj_aux19 = Label2("Autodepth");

	obj_aux18 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux19,
		Child, object->CH_autodepth,
	End;

	object->CH_fullscreen = CheckMark(FALSE);

	obj_aux21 = Label2("Fullscreen");

	obj_aux20 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux21,
		Child, object->CH_fullscreen,
	End;

	GR_Screen = GroupObject,
		MUIA_HelpNode, "GR_Screen",
		MUIA_Background, MUII_RequesterBack,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Screen",
		MUIA_Group_Rows, 1,
		Child, object->BT_select,
		Child, obj_aux12,
		Child, obj_aux14,
		Child, obj_aux16,
		Child, obj_aux18,
		Child, obj_aux20,
	End;

	object->STR_PA_theme = String("", 80);

	object->PA_theme = PopButton(MUII_PopDrawer);

	object->PA_theme = PopaslObject,
		MUIA_HelpNode, "PA_theme",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, object->STR_PA_theme,
		MUIA_Popstring_Button, object->PA_theme,
	End;

	GR_Theme = GroupObject,
		MUIA_HelpNode, "GR_Theme",
		MUIA_Background, MUII_RequesterBack,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Theme",
		Child, object->PA_theme,
	End;

	object->GR_Display = GroupObject,
		MUIA_HelpNode, "GR_Display",
		MUIA_Group_Rows, 2,
		Child, GR_Screen,
		Child, GR_Theme,
	End;

	object->LA_Proxy_type = Label("         Proxy type");

	object->CY_Proxy_type = CycleObject,
		MUIA_HelpNode, "CY_Proxy_type",
		MUIA_Frame, MUIV_Frame_Button,
		MUIA_Cycle_Entries, object->CY_Proxy_typeContent,
	End;

	GR_Proxy_type = GroupObject,
		MUIA_HelpNode, "GR_Proxy_type",
		MUIA_Group_Columns, 2,
		Child, object->LA_Proxy_type,
		Child, object->CY_Proxy_type,
	End;

	object->STR_Host = StringObject,
		MUIA_Disabled, TRUE,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Host",
	End;

	obj_aux23 = Label2("                    Host");

	obj_aux22 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux23,
		Child, object->STR_Host,
	End;

	object->STR_Port = StringObject,
		MUIA_Disabled, TRUE,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Port",
		MUIA_String_Contents, "8080",
		MUIA_String_Accept, "-0123456789",
		MUIA_String_MaxLen, 5,
	End;

	obj_aux25 = Label2(":");

	obj_aux24 = GroupObject,
		MUIA_Group_Columns, 2,
		MUIA_Weight, 2,
		Child, obj_aux25,
		Child, object->STR_Port,
	End;

	GR_Host = GroupObject,
		MUIA_HelpNode, "GR_Host",
		MUIA_Group_Columns, 2,
		Child, obj_aux22,
		Child, obj_aux24,
	End;

	object->STR_Username = StringObject,
		MUIA_Disabled, TRUE,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Username",
	End;

	obj_aux27 = Label2("          Username");

	obj_aux26 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux27,
		Child, object->STR_Username,
	End;

	object->STR_Password = StringObject,
		MUIA_Disabled, TRUE,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Password",
	End;

	obj_aux29 = Label2("            Password");

	obj_aux28 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux29,
		Child, object->STR_Password,
	End;

	object->STR_Bypass = StringObject,
		MUIA_Disabled, TRUE,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Bypass",
		MUIA_String_Contents, "localhost",
	End;

	obj_aux31 = Label2("Bypass proxy for");

	obj_aux30 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux31,
		Child, object->STR_Bypass,
	End;

	GR_HTTP_Proxy = GroupObject,
		MUIA_HelpNode, "GR_HTTP_Proxy",
		MUIA_Background, MUII_RequesterBack,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "HTTP Proxy",
		MUIA_Group_Rows, 5,
		Child, GR_Proxy_type,
		Child, GR_Host,
		Child, obj_aux26,
		Child, obj_aux28,
		Child, obj_aux30,
	End;

	LA_Maximum_fetchers = Label("Maximum fetchers");

	object->STR_Maximum_fetchers = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Maximum_fetchers",
		MUIA_String_Contents, "24",
		MUIA_String_Accept, "-0123456789",
		MUIA_String_MaxLen, 3,
	End;

	LA_Maximum_retried_fetches = Label("Maximum retried fetches");

	object->STR_Maximum_retried = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Maximum_retried",
		MUIA_String_Contents, "1",
		MUIA_String_Accept, "-0123456789",
		MUIA_String_MaxLen, 3,
	End;

	LA_Fetchers_per_host = Label("Fetchers per host");

	object->STR_Fetchers_per_host = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Fetchers_per_host",
		MUIA_String_Contents, "6",
		MUIA_String_Accept, "-0123456789",
		MUIA_String_MaxLen, 3,
	End;

	LA_Fetch_timeout = Label("Fetch timeout");

	object->STR_Fetch_timeout = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Fetch_timeout",
		MUIA_String_Contents, "30",
		MUIA_String_Accept, "-0123456789",
		MUIA_String_MaxLen, 3,
	End;

	LA_Cached_connections = Label("Cached connections");

	object->STR_Cached_connections = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Cached_connections",
		MUIA_String_Contents, "5",
		MUIA_String_Accept, "-0123456789",
		MUIA_String_MaxLen, 3,
	End;

	Space_4 = HVSpace;

	Space_15 = HSpace(0);

	GR_Fetching = GroupObject,
		MUIA_HelpNode, "GR_Fetching",
		MUIA_Background, MUII_RequesterBack,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Fetching",
		MUIA_Group_Columns, 4,
		MUIA_Group_SameWidth, TRUE,
		Child, LA_Maximum_fetchers,
		Child, object->STR_Maximum_fetchers,
		Child, LA_Maximum_retried_fetches,
		Child, object->STR_Maximum_retried,
		Child, LA_Fetchers_per_host,
		Child, object->STR_Fetchers_per_host,
		Child, LA_Fetch_timeout,
		Child, object->STR_Fetch_timeout,
		Child, LA_Cached_connections,
		Child, object->STR_Cached_connections,
		Child, Space_4,
		Child, Space_15,
	End;

	object->GR_Connection = GroupObject,
		MUIA_HelpNode, "GR_Connection",
		MUIA_Group_Rows, 2,
		Child, GR_HTTP_Proxy,
		Child, GR_Fetching,
	End;

	object->CH_Foreground_images = CheckMark(TRUE);

	obj_aux33 = Label2("Foreground images");

	obj_aux32 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux33,
		Child, object->CH_Foreground_images,
	End;

	Space_7 = HVSpace;

	object->CH_Background_images = CheckMark(TRUE);

	obj_aux35 = Label2("Background images");

	obj_aux34 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux35,
		Child, object->CH_Background_images,
	End;

	Space_8 = HVSpace;

	GR_Images = GroupObject,
		MUIA_HelpNode, "GR_Images",
		MUIA_Background, MUII_RequesterBack,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Images",
		MUIA_Group_Rows, 2,
		Child, obj_aux32,
		Child, Space_7,
		Child, obj_aux34,
		Child, Space_8,
	End;

	object->CH_Disable_animations = CheckMark(FALSE);

	obj_aux37 = Label2("Disable animations");

	obj_aux36 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux37,
		Child, object->CH_Disable_animations,
	End;

	Space_6 = HVSpace;

	Space_9 = HVSpace;

	object->STR_gif_delay = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_gif_delay",
		MUIA_String_Contents, "0.10",
		MUIA_String_Accept, "-0123456789.",
		MUIA_String_MaxLen, 5,
	End;

	obj_aux39 = Label2("Limit speed to");

	obj_aux38 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux39,
		Child, object->STR_gif_delay,
	End;

	LA_seconds = Label("seconds between frames");

	Space_10 = HVSpace;

	GR_Animations = GroupObject,
		MUIA_HelpNode, "GR_Animations",
		MUIA_Background, MUII_RequesterBack,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Animations",
		MUIA_Group_Columns, 3,
		Child, obj_aux36,
		Child, Space_6,
		Child, Space_9,
		Child, obj_aux38,
		Child, LA_seconds,
		Child, Space_10,
	End;

	LA_scale = Label("Scale");

	object->SL_scale = SliderObject,
		MUIA_HelpNode, "SL_scale",
		MUIA_Frame, MUIV_Frame_Slider,
		MUIA_Slider_Min, 50,
		MUIA_Slider_Max, 150,
		MUIA_Slider_Level, 100,
	End;

	LA_scale_aga = Label("Scale AGA");

	object->SL_scale_aga = SliderObject,
		MUIA_HelpNode, "SL_scale_aga",
		MUIA_Frame, MUIV_Frame_Slider,
		MUIA_Slider_Min, 50,
		MUIA_Slider_Max, 150,
		MUIA_Slider_Level, 75,
	End;

	GR_Scaling = GroupObject,
		MUIA_HelpNode, "GR_Scaling",
		MUIA_Background, MUII_RequesterBack,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Scaling",
		MUIA_Group_Columns, 2,
		Child, LA_scale,
		Child, object->SL_scale,
		Child, LA_scale_aga,
		Child, object->SL_scale_aga,
	End;

	object->CH_cpu_speed = CheckMark(FALSE);

	obj_aux41 = Label2("100% CPU  usage for faster rendering");

	obj_aux40 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux41,
		Child, object->CH_cpu_speed,
	End;

	GR_CPU_usage = GroupObject,
		MUIA_HelpNode, "GR_CPU_usage",
		MUIA_Background, MUII_RequesterBack,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Speed",
		Child, obj_aux40,
	End;

	object->GR_Rendering = GroupObject,
		MUIA_HelpNode, "GR_Rendering",
		MUIA_Group_Rows, 4,
		Child, GR_Images,
		Child, GR_Animations,
		Child, GR_Scaling,
		Child, GR_CPU_usage,
	End;

	LA_Sans_serif = Label("Sans serif");

	object->STR_PA_Sans_serif = String("", 80);

	object->PA_Sans_serif = PopButton(MUII_PopFile);

	object->PA_Sans_serif = PopaslObject,
		MUIA_HelpNode, "PA_Sans_serif",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, object->STR_PA_Sans_serif,
		MUIA_Popstring_Button, object->PA_Sans_serif,
	End;

	LA_Serif = Label("Serif");

	object->STR_PA_Serif = String("", 80);

	object->PA_Serif = PopButton(MUII_PopFile);

	object->PA_Serif = PopaslObject,
		MUIA_HelpNode, "PA_Serif",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, object->STR_PA_Serif,
		MUIA_Popstring_Button, object->PA_Serif,
	End;

	LA_Sans_serif_italic = Label("Sans serif italic");

	object->STR_PA_Sans_serif_italic = String("", 80);

	object->PA_Sans_serif_italic = PopButton(MUII_PopFile);

	object->PA_Sans_serif_italic = PopaslObject,
		MUIA_HelpNode, "PA_Sans_serif_italic",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, object->STR_PA_Sans_serif_italic,
		MUIA_Popstring_Button, object->PA_Sans_serif_italic,
	End;

	LA_Sans_serif_bold = Label("Sans serif bold");

	object->STR_PA_Sans_serif_bold = String("", 80);

	object->PA_Sans_serif_bold = PopButton(MUII_PopFile);

	object->PA_Sans_serif_bold = PopaslObject,
		MUIA_HelpNode, "PA_Sans_serif_bold",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, object->STR_PA_Sans_serif_bold,
		MUIA_Popstring_Button, object->PA_Sans_serif_bold,
	End;

	LA_Sans_serif_bold_italic = Label("Sans serif bold italic");

	object->STR_PA_Sans_serif_bold_italic = String("", 80);

	object->PA_Sans_serif_bold_italic = PopButton(MUII_PopFile);

	object->PA_Sans_serif_bold_italic = PopaslObject,
		MUIA_HelpNode, "PA_Sans_serif_bold_italic",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, object->STR_PA_Sans_serif_bold_italic,
		MUIA_Popstring_Button, object->PA_Sans_serif_bold_italic,
	End;

	LA_Monospace = Label("Monospace");

	object->STR_PA_Monospace = String("", 80);

	object->PA_Monospace = PopButton(MUII_PopFile);

	object->PA_Monospace = PopaslObject,
		MUIA_HelpNode, "PA_Monospace",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, object->STR_PA_Monospace,
		MUIA_Popstring_Button, object->PA_Monospace,
	End;

	LA_Cursive = Label("Cursive");

	object->STR_PA_Cursive = String("", 80);

	object->PA_Cursive = PopButton(MUII_PopFile);

	object->PA_Cursive = PopaslObject,
		MUIA_HelpNode, "PA_Cursive",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, object->STR_PA_Cursive,
		MUIA_Popstring_Button, object->PA_Cursive,
	End;

	LA_Fantasy = Label("Fantasy");

	object->STR_PA_Fantasy = String("", 80);

	object->PA_Fantasy = PopButton(MUII_PopFile);

	object->PA_Fantasy = PopaslObject,
		MUIA_HelpNode, "PA_Fantasy",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, object->STR_PA_Fantasy,
		MUIA_Popstring_Button, object->PA_Fantasy,
	End;

	GR_Fonts_families = GroupObject,
		MUIA_HelpNode, "GR_Fonts_families",
		MUIA_Background, MUII_RequesterBack,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Fonts families",
		MUIA_Group_Columns, 2,
		Child, LA_Sans_serif,
		Child, object->PA_Sans_serif,
		Child, LA_Serif,
		Child, object->PA_Serif,
		Child, LA_Sans_serif_italic,
		Child, object->PA_Sans_serif_italic,
		Child, LA_Sans_serif_bold,
		Child, object->PA_Sans_serif_bold,
		Child, LA_Sans_serif_bold_italic,
		Child, object->PA_Sans_serif_bold_italic,
		Child, LA_Monospace,
		Child, object->PA_Monospace,
		Child, LA_Cursive,
		Child, object->PA_Cursive,
		Child, LA_Fantasy,
		Child, object->PA_Fantasy,
	End;

	LA_Default = Label("Default");

	object->STR_Default = StringObject,
		MUIA_Weight, 8,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Default",
		MUIA_String_Contents, "12",
		MUIA_String_MaxLen, 3,
		MUIA_String_Format, MUIV_String_Format_Right,
	End;

	LA_pt_2 = Label("pt");

	LA_Minimum = Label("Minimum");

	object->STR_Minimum = StringObject,
		MUIA_Weight, 8,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_Minimum",
		MUIA_String_Contents, "8",
		MUIA_String_MaxLen, 3,
		MUIA_String_Format, MUIV_String_Format_Right,
	End;

	LA_pt_1 = Label("pt");

	LA_label_dpi = Label("DPI");

	object->SL_dpi = SliderObject,
		MUIA_HelpNode, "SL_dpi",
		MUIA_Frame, MUIV_Frame_Slider,
		MUIA_Slider_Min, 50,
		MUIA_Slider_Max, 150,
		MUIA_Slider_Level, 72,
	End;

	object->CH_bitmap_fonts = CheckMark(FALSE);

	obj_aux43 = Label2("Bitmap fonts");

	obj_aux42 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux43,
		Child, object->CH_bitmap_fonts,
	End;

	GR_Font_size = GroupObject,
		MUIA_HelpNode, "GR_Font_size",
		MUIA_Background, MUII_RequesterBack,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Font size",
		MUIA_Group_Horiz, TRUE,
		MUIA_Group_VertSpacing, 0,
		Child, LA_Default,
		Child, object->STR_Default,
		Child, LA_pt_2,
		Child, LA_Minimum,
		Child, object->STR_Minimum,
		Child, LA_pt_1,
		Child, LA_label_dpi,
		Child, object->SL_dpi,
		Child, obj_aux42,
	End;

	object->GR_Fonts = GroupObject,
		MUIA_HelpNode, "GR_Fonts",
		MUIA_Group_Rows, 2,
		Child, GR_Fonts_families,
		Child, GR_Font_size,
	End;

	object->SL_Memory_cache = SliderObject,
		MUIA_HelpNode, "SL_Memory_cache",
		MUIA_Background, MUII_PropBack,
		MUIA_Frame, MUIV_Frame_Slider,
		MUIA_Slider_Min, 0,
		MUIA_Slider_Max, 100,
		MUIA_Slider_Level, 12,
	End;

	obj_aux45 = Label2("Size");

	obj_aux44 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux45,
		Child, object->SL_Memory_cache,
	End;

	LA_MB = Label("MB");

	GR_Memory_cache = GroupObject,
		MUIA_HelpNode, "GR_Memory_cache",
		MUIA_Background, MUII_RequesterBack,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Memory cache",
		MUIA_Group_Columns, 2,
		Child, obj_aux44,
		Child, LA_MB,
	End;

	LA_Directory = Label("Directory");

	object->STR_PA_Cache_directory = String("", 80);

	object->PA_Cache_directory = PopButton(MUII_PopDrawer);

	object->PA_Cache_directory = PopaslObject,
		MUIA_HelpNode, "PA_Cache_directory",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, object->STR_PA_Cache_directory,
		MUIA_Popstring_Button, object->PA_Cache_directory,
	End;

	GR_Cache_directory = GroupObject,
		MUIA_HelpNode, "GR_Cache_directory",
		MUIA_Background, MUII_RequesterBack,
		MUIA_Group_Columns, 2,
		Child, LA_Directory,
		Child, object->PA_Cache_directory,
	End;

	object->SL_Disc_cache = SliderObject,
		MUIA_HelpNode, "SL_Disc_cache",
		MUIA_Background, MUII_PropBack,
		MUIA_Frame, MUIV_Frame_Slider,
		MUIA_Slider_Min, 0,
		MUIA_Slider_Max, 1024,
		MUIA_Slider_Level, 100,
	End;

	obj_aux47 = Label2("Size");

	obj_aux46 = GroupObject,
		MUIA_Group_Columns, 2,
		Child, obj_aux47,
		Child, object->SL_Disc_cache,
	End;

	LA_MBC = Label("MB");

	GR_Disc_cache_slider = GroupObject,
		MUIA_HelpNode, "GR_Disc_cache_slider",
		MUIA_Background, MUII_RequesterBack,
		MUIA_Group_Columns, 2,
		Child, obj_aux46,
		Child, LA_MBC,
	End;

	GR_Disc_cache = GroupObject,
		MUIA_HelpNode, "GR_Disc_cache",
		MUIA_Background, MUII_RequesterBack,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Disc cache",
		MUIA_Group_Rows, 2,
		Child, GR_Cache_directory,
		Child, GR_Disc_cache_slider,
	End;

	object->GR_Cache = GroupObject,
		MUIA_HelpNode, "GR_Cache",
		MUIA_Group_Rows, 2,
		Child, GR_Memory_cache,
		Child, GR_Disc_cache,
	End;

	object->Listview = ListObject,
		MUIA_Frame, MUIV_Frame_InputList,
		MUIA_List_Format, "BAR ,",
		MUIA_List_ConstructHook, &FavConstructorHook,
		MUIA_List_DestructHook, &FavDestructorHook,
		MUIA_List_DisplayHook, &FavDisplayerHook,
		MUIA_List_Active, TRUE,
		MUIA_List_Title, TRUE,
	End;

	object->Listview = ListviewObject,
		MUIA_HelpNode, "Listview",
		MUIA_Listview_DoubleClick, TRUE,
		MUIA_Listview_List, object->Listview,
	End;

	object->STR_fav_label = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_fav_label",
		MUIA_String_MaxLen, 10,
	End;

	object->STR_fav_url = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_fav_url",
	End;

	GR_grp_18 = GroupObject,
		MUIA_HelpNode, "GR_grp_18",
		MUIA_Group_Columns, 2,
		Child, object->STR_fav_label,
		Child, object->STR_fav_url,
	End;

	object->BT_modify = SimpleButton("Modify");

	GR_editlist = GroupObject,
		MUIA_HelpNode, "GR_editlist",
		Child, GR_grp_18,
		Child, object->BT_modify,
	End;

	GR_Favs = GroupObject,
		MUIA_HelpNode, "GR_Favs",
		Child, object->Listview,
		Child, GR_editlist,
	End;

	LA_Download_manager = Label("Download manager");

	object->STR_PA_Download_manager = String("", 80);

	object->PA_Download_manager = PopButton(MUII_PopDrawer);

	object->PA_Download_manager = PopaslObject,
		MUIA_HelpNode, "PA_Download_manager",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, object->STR_PA_Download_manager,
		MUIA_Popstring_Button, object->PA_Download_manager,
	End;

	LA_Download = Label("Initial download location");

	object->STR_PA_Download_dir = String("", 80);

	object->PA_Download_dir = PopButton(MUII_PopDrawer);

	object->PA_Download_dir = PopaslObject,
		MUIA_HelpNode, "PA_Download_dir",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, object->STR_PA_Download_dir,
		MUIA_Popstring_Button, object->PA_Download_dir,
	End;

	GR_Downloads = GroupObject,
		MUIA_HelpNode, "GR_Downloads",
		MUIA_Background, MUII_RequesterBack,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Downloads",
		MUIA_Group_Columns, 2,
		Child, LA_Download_manager,
		Child, object->PA_Download_manager,
		Child, LA_Download,
		Child, object->PA_Download_dir,
	End;

	LA_YouTube_url = Label("YouTube web handler");

	object->STR_YT = StringObject,
		MUIA_Frame, MUIV_Frame_String,
		MUIA_HelpNode, "STR_YT",
		MUIA_String_Contents, "http://youtubeinmp4.com/youtube.php?video=",
		MUIA_String_MaxLen, 128,
	End;

	GR_Youtube = GroupObject,
		MUIA_HelpNode, "GR_Youtube",
		MUIA_Background, MUII_RequesterBack,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "YouTube",
		MUIA_Group_Columns, 2,
		Child, LA_YouTube_url,
		Child, object->STR_YT,
	End;

	LA_MP3 = Label("MP3 Player");

	object->STR_PA_Mp3_player = String("", 80);

	object->PA_Mp3_player = PopButton(MUII_PopDrawer);

	object->PA_Mp3_player = PopaslObject,
		MUIA_HelpNode, "PA_Mp3_player",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, object->STR_PA_Mp3_player,
		MUIA_Popstring_Button, object->PA_Mp3_player,
	End;

	LA_Module = Label("Module player");

	object->STR_PA_Module = String("", 80);

	object->PA_Module = PopButton(MUII_PopDrawer);

	object->PA_Module = PopaslObject,
		MUIA_HelpNode, "PA_Module",
		MUIA_Popasl_Type, 0,
		MUIA_Popstring_String, object->STR_PA_Module,
		MUIA_Popstring_Button, object->PA_Module,
	End;

	GR_Players = GroupObject,
		MUIA_HelpNode, "GR_Players",
		MUIA_Background, MUII_RequesterBack,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_FrameTitle, "Players",
		MUIA_Group_Rows, 2,
		Child, LA_MP3,
		Child, object->PA_Mp3_player,
		Child, LA_Module,
		Child, object->PA_Module,
	End;

	object->GR_Other = GroupObject,
		MUIA_HelpNode, "GR_Other",
		MUIA_FramePhantomHoriz, TRUE,
		MUIA_Group_Rows, 3,
		Child, GR_Downloads,
		Child, GR_Youtube,
		Child, GR_Players,
	End;

	GR_Tabs = RegisterObject,
		MUIA_Register_Titles, object->STR_GR_Tabs,
		MUIA_Frame, MUIV_Frame_Group,
		Child, object->GR_General,
		Child, object->GR_Display,
		Child, object->GR_Connection,
		Child, object->GR_Rendering,
		Child, object->GR_Fonts,
		Child, object->GR_Cache,
		Child, GR_Favs,
		Child, object->GR_Other,
	End;

	object->BT_save = SimpleButton("Save");

	object->BT_restart = SimpleButton("Restart");

	object->BT_cancel = SimpleButton("Cancel");

	GR_Restart = GroupObject,
		MUIA_HelpNode, "GR_Restart",
		MUIA_Group_Columns, 3,
		Child, object->BT_save,
		Child, object->BT_restart,
		Child, object->BT_cancel,
	End;

	GROUP_ROOT_0 = GroupObject,
		MUIA_Frame, MUIV_Frame_Group,
		MUIA_Group_Rows, 2,
		Child, GR_Tabs,
		Child, GR_Restart,
	End;

	object->WI_Preferences = WindowObject,
		MUIA_Window_Title, "Preferences",
		MUIA_HelpNode, "WI_Preferences",
		MUIA_Window_ID, MAKE_ID('0', 'W', 'I', 'N'),
		WindowContents, GROUP_ROOT_0,
	End;

	MNlabel0Quit = MenuitemObject,
		MUIA_Menuitem_Title, "Quit",
	End;

	object->MN_Quit = MenustripObject,
		MUIA_Family_Child, MNlabel0Quit,
	End;

	object->App = ApplicationObject,
		MUIA_Application_Author, "Artur Jarosik",
		MUIA_Application_Menustrip, object->MN_Quit,
		MUIA_Application_Base, "NetSurf",
		MUIA_Application_Title, "NetSurf",
		MUIA_Application_Version, "3.5",
		MUIA_Application_Copyright, "Artur Jarosik",
		MUIA_Application_Description, "GUI Options for NetSurf",
		MUIA_Application_HelpFile, "character.guide",
		SubWindow, object->WI_Preferences,
	End;


	if (!object->App)
	{
		FreeVec(object);
		return NULL;
	}

	DoMethod(MNlabel0Quit,
		MUIM_Notify, MUIA_Menu_Enabled, TRUE,
		object->App,
		2,
		MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit
		);

	DoMethod(object->WI_Preferences,
		MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
		object->WI_Preferences,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
		);

	DoMethod(object->WI_Preferences,
		MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
		object->App,
		2,
		MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit
		);

	DoMethod(object->STR_label_URL,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		object->STR_label_URL,
		3,
		MUIM_Set, MUIA_String_Contents, MUIV_TriggerValue
		);

	DoMethod(BT_Homepage,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->STR_label_URL,
		3,
		MUIM_Set, MUIA_String_Contents, "about:welcome"
		);

	DoMethod(BT_Homepage_2,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->STR_label_URL,
		2,
		MUIM_CallHook, &SetUrlHook
		);

	DoMethod(BT_Homepage_3,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->STR_label_URL,
		3,
		MUIM_Set, MUIA_String_Contents, "www.google.com"
		);

	DoMethod(object->CM_Hide_advertisements,
		MUIM_Notify, MUIA_Selected, MUIV_EveryTime,
		object->CM_Hide_advertisements,
		3,
		MUIM_Set, MUIA_Selected, MUIV_TriggerValue
		);

	DoMethod(object->CM_Get_from_Locale_prefs,
		MUIM_Notify, MUIA_Selected, TRUE,
		object->STR_Get_from_Locale_prefs,
		3,
		MUIM_Set, MUIA_Disabled, TRUE
		);

	DoMethod(object->CM_Get_from_Locale_prefs,
		MUIM_Notify, MUIA_Selected, FALSE,
		object->STR_Get_from_Locale_prefs,
		3,
		MUIM_Set, MUIA_Disabled, FALSE
		);

	DoMethod(object->STR_Get_from_Locale_prefs,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		object->STR_Get_from_Locale_prefs,
		3,
		MUIM_Set, MUIA_String_Contents, MUIV_TriggerValue
		);

	DoMethod(object->STR_charset,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		object->STR_charset,
		3,
		MUIM_Set, MUIA_String_Contents, MUIV_TriggerValue
		);

	DoMethod(object->STR_history,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		object->STR_history,
		3,
		MUIM_Set, MUIA_String_Contents, MUIV_TriggerValue
		);

	DoMethod(object->CM_Enable_JavaScript,
		MUIM_Notify, MUIA_Selected, MUIV_EveryTime,
		object->CM_Enable_JavaScript,
		3,
		MUIM_Set, MUIA_Selected, MUIV_TriggerValue
		);

	DoMethod(object->CM_Enable_JavaScript,
		MUIM_Notify, MUIA_Selected, MUIV_EveryTime,
		object->WI_Preferences,
		2,
		MUIM_CallHook, &ReloadHook
		);

	DoMethod(object->CH_send_referer,
		MUIM_Notify, MUIA_Selected, MUIV_EveryTime,
		object->CH_send_referer,
		3,
		MUIM_Set, MUIA_Selected, MUIV_TriggerValue
		);

	DoMethod(object->CH_do_not_track,
		MUIM_Notify, MUIA_Selected, MUIV_EveryTime,
		object->CH_do_not_track,
		3,
		MUIM_Set, MUIA_Selected, MUIV_TriggerValue
		);

	DoMethod(object->STR_width,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		GR_Restart,
		3,
		MUIM_Set, MUIA_ShowMe, TRUE
		);

	DoMethod(object->STR_height,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		GR_Restart,
		3,
		MUIM_Set, MUIA_ShowMe, TRUE
		);

	DoMethod(object->STR_depth,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		GR_Restart,
		3,
		MUIM_Set, MUIA_ShowMe, TRUE
		);

	DoMethod(object->CH_autodepth,
		MUIM_Notify, MUIA_Selected, FALSE,
		object->STR_depth,
		3,
		MUIM_Set, MUIA_Disabled, FALSE
		);

	DoMethod(object->CH_autodepth,
		MUIM_Notify, MUIA_Selected, TRUE,
		object->STR_depth,
		3,
		MUIM_Set, MUIA_Disabled, TRUE
		);

	DoMethod(object->CH_autodepth,
		MUIM_Notify, MUIA_Selected, MUIV_EveryTime,
		object->CH_autodepth,
		3,
		MUIM_Set, MUIA_Selected, MUIV_TriggerValue
		);

	DoMethod(object->CH_fullscreen,
		MUIM_Notify, MUIA_Selected, MUIV_EveryTime,
		object->CH_fullscreen,
		3,
		MUIM_Set, MUIA_Selected, MUIV_TriggerValue
		);

	DoMethod(object->CY_Proxy_type,
		MUIM_Notify, MUIA_Cycle_Active, MUIV_EveryTime,
		object->STR_Host,
		3,
		MUIM_Set, MUIA_Disabled, FALSE
		);

	DoMethod(object->CY_Proxy_type,
		MUIM_Notify, MUIA_Cycle_Active, MUIV_EveryTime,
		object->STR_Port,
		3,
		MUIM_Set, MUIA_Disabled, FALSE
		);

	DoMethod(object->CY_Proxy_type,
		MUIM_Notify, MUIA_Cycle_Active, MUIV_EveryTime,
		object->STR_Username,
		3,
		MUIM_Set, MUIA_Disabled, FALSE
		);

	DoMethod(object->CY_Proxy_type,
		MUIM_Notify, MUIA_Cycle_Active, MUIV_EveryTime,
		object->STR_Password,
		3,
		MUIM_Set, MUIA_Disabled, FALSE
		);

	DoMethod(object->CY_Proxy_type,
		MUIM_Notify, MUIA_Cycle_Active, MUIV_EveryTime,
		object->STR_Bypass,
		3,
		MUIM_Set, MUIA_Disabled, FALSE
		);

	DoMethod(object->CY_Proxy_type,
		MUIM_Notify, MUIA_Cycle_Active, MUIV_EveryTime,
		object->CY_Proxy_type,
		2,
		MUIM_CallHook, &ProxyHook
		);

	DoMethod(object->STR_Username,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		object->STR_Username,
		3,
		MUIM_Set, MUIA_String_Contents, MUIV_TriggerValue
		);

	DoMethod(object->STR_Password,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		object->STR_Password,
		3,
		MUIM_Set, MUIA_String_Contents, MUIV_TriggerValue
		);

	DoMethod(object->STR_Bypass,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		object->STR_Bypass,
		3,
		MUIM_Set, MUIA_String_Contents, MUIV_TriggerValue
		);

	DoMethod(object->STR_Maximum_fetchers,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		object->STR_Maximum_fetchers,
		3,
		MUIM_Set, MUIA_String_Contents, MUIV_TriggerValue
		);

	DoMethod(object->STR_Maximum_retried,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		object->STR_Maximum_retried,
		3,
		MUIM_Set, MUIA_String_Contents, MUIV_TriggerValue
		);

	DoMethod(object->STR_Fetchers_per_host,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		object->STR_Fetchers_per_host,
		3,
		MUIM_Set, MUIA_String_Contents, MUIV_TriggerValue
		);

	DoMethod(object->STR_Fetch_timeout,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		object->STR_Fetch_timeout,
		3,
		MUIM_Set, MUIA_String_Contents, MUIV_TriggerValue
		);

	DoMethod(object->STR_Cached_connections,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		object->STR_Cached_connections,
		3,
		MUIM_Set, MUIA_String_Contents, MUIV_TriggerValue
		);

	DoMethod(object->CH_Foreground_images,
		MUIM_Notify, MUIA_Selected, MUIV_EveryTime,
		object->CH_Foreground_images,
		3,
		MUIM_Set, MUIA_Selected, MUIV_TriggerValue
		);

	DoMethod(object->CH_Background_images,
		MUIM_Notify, MUIA_Selected, MUIV_EveryTime,
		object->CH_Background_images,
		3,
		MUIM_Set, MUIA_Selected, MUIV_TriggerValue
		);

	DoMethod(object->CH_Disable_animations,
		MUIM_Notify, MUIA_Selected, MUIV_EveryTime,
		object->CH_Disable_animations,
		3,
		MUIM_Set, MUIA_Selected, MUIV_TriggerValue
		);

	DoMethod(object->STR_gif_delay,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		object->STR_gif_delay,
		3,
		MUIM_Set, MUIA_String_Contents, MUIV_TriggerValue
		);

	DoMethod(object->STR_Default,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		object->STR_Default,
		3,
		MUIM_Set, MUIA_String_Contents, MUIV_TriggerValue
		);

	DoMethod(object->STR_Minimum,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		object->STR_Minimum,
		3,
		MUIM_Set, MUIA_String_Contents, MUIV_TriggerValue
		);

	DoMethod(object->SL_Memory_cache,
		MUIM_Notify, MUIA_Slider_Level, MUIV_EveryTime,
		object->SL_Memory_cache,
		3,
		MUIM_Set, MUIA_Slider_Level, MUIV_TriggerValue
		);

	DoMethod(object->SL_Disc_cache,
		MUIM_Notify, MUIA_Slider_Level, MUIV_EveryTime,
		object->SL_Disc_cache,
		3,
		MUIM_Set, MUIA_Slider_Level, MUIV_TriggerValue
		);

	DoMethod(object->Listview,
		MUIM_Notify, MUIA_Listview_DoubleClick, TRUE,
		object->Listview,
		2,
		MUIM_CallHook, &GetFavAttrHook
		);

	DoMethod(object->Listview,
		MUIM_Notify, MUIA_List_Active, MUIV_EveryTime,
		object->WI_Preferences,
		3,
		MUIM_Set, MUIA_Window_ActiveObject, object->Listview
		);

	DoMethod(object->STR_fav_label,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		object->STR_fav_label,
		3,
		MUIM_Set, MUIA_String_Contents, MUIV_TriggerValue
		);

	DoMethod(object->STR_fav_url,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		object->STR_fav_url,
		3,
		MUIM_Set, MUIA_String_Contents, MUIV_TriggerValue
		);

	DoMethod(object->BT_modify,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->Listview,
		2,
		MUIM_CallHook, &ModifyFavHook
		);

	DoMethod(object->STR_YT,
		MUIM_Notify, MUIA_String_Contents, MUIV_EveryTime,
		object->STR_YT,
		3,
		MUIM_Set, MUIA_String_Contents, MUIV_TriggerValue
		);

	DoMethod(object->BT_save,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->App,
		2,
		MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit
		);

	DoMethod(object->BT_save,
		MUIM_Notify, MUIA_Pressed, TRUE,
		object->BT_save,
		2,
		MUIM_CallHook, &SaveHook
		);

	DoMethod(object->BT_restart,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->WI_Preferences,
		2,
		MUIM_CallHook, &RestartHook
		);

	DoMethod(object->BT_restart,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->WI_Preferences,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
		);

	DoMethod(object->BT_cancel,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->App,
		2,
		MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit
		);

	DoMethod(object->BT_cancel,
		MUIM_Notify, MUIA_Pressed, FALSE,
		object->WI_Preferences,
		3,
		MUIM_Set, MUIA_Window_Open, FALSE
		);

	DoMethod(object->WI_Preferences,
		MUIM_Window_SetCycleChain, object->GR_General,
		object->STR_label_URL,
		object->CM_Hide_advertisements,
		object->CM_Get_from_Locale_prefs,
		object->STR_Get_from_Locale_prefs,
		object->STR_charset,
		object->STR_history,
		object->CM_Enable_JavaScript,
		object->CH_send_referer,
		object->CH_do_not_track,
		object->GR_Display,
		object->BT_select,
		object->STR_width,
		object->STR_height,
		object->STR_depth,
		object->CH_autodepth,
		object->CH_fullscreen,
		object->PA_theme,
		object->GR_Connection,
		object->CY_Proxy_type,
		object->STR_Host,
		object->STR_Port,
		object->STR_Username,
		object->STR_Password,
		object->STR_Bypass,
		object->STR_Maximum_fetchers,
		object->STR_Maximum_retried,
		object->STR_Fetchers_per_host,
		object->STR_Fetch_timeout,
		object->STR_Cached_connections,
		object->GR_Rendering,
		object->CH_Foreground_images,
		object->CH_Background_images,
		object->CH_Disable_animations,
		object->STR_gif_delay,
		object->SL_scale,
		object->SL_scale_aga,
		object->CH_cpu_speed,
		object->GR_Fonts,
		object->PA_Sans_serif,
		object->PA_Serif,
		object->PA_Sans_serif_italic,
		object->PA_Sans_serif_bold,
		object->PA_Sans_serif_bold_italic,
		object->PA_Monospace,
		object->PA_Cursive,
		object->PA_Fantasy,
		object->STR_Default,
		object->STR_Minimum,
		object->SL_dpi,
		object->CH_bitmap_fonts,
		object->GR_Cache,
		object->SL_Memory_cache,
		object->PA_Cache_directory,
		object->SL_Disc_cache,
		object->Listview,
		object->STR_fav_label,
		object->STR_fav_url,
		object->BT_modify,
		object->GR_Other,
		object->PA_Download_manager,
		object->PA_Download_dir,
		object->STR_YT,
		object->PA_Mp3_player,
		object->PA_Module,
		object->BT_save,
		object->BT_restart,
		object->BT_cancel,
		0
		);

	set(object->WI_Preferences,
		MUIA_Window_Open, TRUE
		);


	return object;
}

void DisposeApp(struct ObjApp * object)
{
	if (object)
	{
		MUI_DisposeObject(object->App);
		FreeVec(object);
		object = NULL;
	}
}
