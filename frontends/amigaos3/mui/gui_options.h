#include <exec/types.h>

struct ObjApp
{
	APTR	App;
	APTR	MN_Quit;
	APTR	WI_Preferences;
	APTR	GR_General;
	APTR	STR_label_URL;
	APTR	CM_Hide_advertisements;
	APTR	CM_Get_from_Locale_prefs;
	APTR	STR_Get_from_Locale_prefs;
	APTR	STR_charset;
	APTR	STR_history;
	APTR	CM_Enable_JavaScript;
	APTR	CH_send_referer;
	APTR	CH_do_not_track;
	APTR	GR_Display;
	APTR	BT_select;
	APTR	STR_width;
	APTR	STR_height;
	APTR	STR_depth;
	APTR	CH_autodepth;
	APTR	CH_fullscreen;
	APTR	PA_theme;
	APTR	STR_PA_theme;
	APTR	CH_dithering;
	APTR	GR_Connection;
	APTR	LA_Proxy_type;
	APTR	CY_Proxy_type;
	APTR	STR_Host;
	APTR	STR_Port;
	APTR	STR_Username;
	APTR	STR_Password;
	APTR	STR_Bypass;
	APTR	STR_Maximum_fetchers;
	APTR	STR_Maximum_retried;
	APTR	STR_Fetchers_per_host;
	APTR	STR_Fetch_timeout;
	APTR	STR_Cached_connections;
	APTR	GR_Rendering;
	APTR	CH_Foreground_images;
	APTR	CH_Background_images;
	APTR	CH_Disable_animations;
	APTR	STR_gif_delay;
	APTR	SL_scale;
	APTR	SL_scale_aga;
	APTR	CH_cpu_speed;
	APTR	GR_Fonts;
	APTR	PA_Sans_serif;
	APTR	STR_PA_Sans_serif;
	APTR	PA_Serif;
	APTR	STR_PA_Serif;
	APTR	PA_Sans_serif_italic;
	APTR	STR_PA_Sans_serif_italic;
	APTR	PA_Sans_serif_bold;
	APTR	STR_PA_Sans_serif_bold;
	APTR	PA_Sans_serif_bold_italic;
	APTR	STR_PA_Sans_serif_bold_italic;
	APTR	PA_Monospace;
	APTR	STR_PA_Monospace;
	APTR	PA_Cursive;
	APTR	STR_PA_Cursive;
	APTR	PA_Fantasy;
	APTR	STR_PA_Fantasy;
	APTR	STR_Default;
	APTR	STR_Minimum;
	APTR	SL_dpi;
	APTR	CH_bitmap_fonts;
	APTR	GR_Cache;
	APTR	SL_Memory_cache;
	APTR	PA_Cache_directory;
	APTR	STR_PA_Cache_directory;
	APTR	SL_Disc_cache;
	APTR	Listview;
	APTR	STR_fav_label;
	APTR	STR_fav_url;
	APTR	BT_modify;
	APTR	GR_Other;
	APTR	PA_Download_manager;
	APTR	STR_PA_Download_manager;
	APTR	PA_Download_dir;
	APTR	STR_PA_Download_dir;
	APTR	STR_YT;
	APTR	PA_Mp3_player;
	APTR	STR_PA_Mp3_player;
	APTR	PA_Module;
	APTR	STR_PA_Module;
	APTR	BT_save;
	APTR	BT_restart;
	APTR	BT_cancel;
	CONST_STRPTR	STR_GR_Tabs[9];
	CONST_STRPTR	CY_Proxy_typeContent[4];
};


extern struct ObjApp * CreateApp(void);
extern void DisposeApp(struct ObjApp *);
