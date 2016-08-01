#ifndef GUI_OPTIONS_CAT_H
#define GUI_OPTIONS_CAT_H 1

/* Locale Catalog Source File
 *
 * Automatically created by SimpleCat V3
 * Do NOT edit by hand!
 *
 * SimpleCat ©1992-2010 Guido Mersmann
 *
 */



/****************************************************************************/


#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifdef CATCOMP_ARRAY
#undef CATCOMP_NUMBERS
#undef CATCOMP_STRINGS
#define CATCOMP_NUMBERS
#define CATCOMP_STRINGS
#endif

#ifdef CATCOMP_BLOCK
#undef CATCOMP_STRINGS
#define CATCOMP_STRINGS
#endif



/****************************************************************************/


#ifdef CATCOMP_NUMBERS

#define MSG_AppDescription 0
#define MSG_AppCopyright 1
#define MSG_MNlabel0Quit 2
#define MSG_WI_Preferences 3
#define MSG_GR_Tabs0 4
#define MSG_GR_Tabs1 5
#define MSG_GR_Tabs2 6
#define MSG_GR_Tabs3 7
#define MSG_GR_Tabs4 8
#define MSG_GR_Tabs5 9
#define MSG_GR_Tabs6 10
#define MSG_GR_Tabs7 11
#define MSG_GR_General_HomepageTitle 12
#define MSG_STR_label_URL 13
#define MSG_BT_HomepageNotify0 14
#define MSG_BT_Homepage 15
#define MSG_BT_Homepage_2 16
#define MSG_BT_Homepage_3Notify0 17
#define MSG_BT_Homepage_3 18
#define MSG_GR_General_Content_blockingTitle 19
#define MSG_CM_Hide_advertisements 20
#define MSG_GR_General_Content_languageTitle 21
#define MSG_CM_Get_from_Locale_prefs 22
#define MSG_STR_Get_from_Locale_prefs 23
#define MSG_STR_charset 24
#define MSG_GR_General_HistoryTitle 25
#define MSG_Keep_history_for 26
#define MSG_LA_days 27
#define MSG_GR_Gen_ScriptingTitle 28
#define MSG_CM_Enable_JavaScript 29
#define MSG_GR_PrivacyTitle 30
#define MSG_CH_send_referer 31
#define MSG_CH_do_not_track 32
#define MSG_GR_ScreenTitle 33
#define MSG_BT_select 34
#define MSG_STR_width 35
#define MSG_STR_height 36
#define MSG_STR_depth 37
#define MSG_CH_autodepth 38
#define MSG_CH_fullscreen 39
#define MSG_GR_ThemeTitle 40
#define MSG_GR_DitheringTitle 41
#define MSG_CH_dithering 42
#define MSG_GR_HTTP_ProxyTitle 43
#define MSG_LA_Proxy_type 44
#define MSG_CY_Proxy_type0 45
#define MSG_CY_Proxy_type1 46
#define MSG_CY_Proxy_type2 47
#define MSG_STR_Host 48
#define MSG_STR_Port 49
#define MSG_STR_Username 50
#define MSG_STR_Password 51
#define MSG_STR_Bypass 52
#define MSG_GR_FetchingTitle 53
#define MSG_LA_Maximum_fetchers 54
#define MSG_LA_Maximum_retried_fetches 55
#define MSG_LA_Fetchers_per_host 56
#define MSG_LA_Fetch_timeout 57
#define MSG_LA_Cached_connections 58
#define MSG_GR_ImagesTitle 59
#define MSG_CH_Foreground_images 60
#define MSG_CH_Background_images 61
#define MSG_GR_AnimationsTitle 62
#define MSG_CH_Disable_animations 63
#define MSG_STR_gif_delay 64
#define MSG_LA_seconds 65
#define MSG_GR_ScalingTitle 66
#define MSG_LA_scale 67
#define MSG_LA_scale_aga 68
#define MSG_GR_CPU_usageTitle 69
#define MSG_CH_cpu_speed 70
#define MSG_GR_Fonts_familiesTitle 71
#define MSG_LA_Sans_serif 72
#define MSG_LA_Serif 73
#define MSG_LA_Sans_serif_italic 74
#define MSG_LA_Sans_serif_bold 75
#define MSG_LA_Sans_serif_bold_italic 76
#define MSG_LA_Monospace 77
#define MSG_LA_Cursive 78
#define MSG_LA_Fantasy 79
#define MSG_GR_Font_sizeTitle 80
#define MSG_LA_Default 81
#define MSG_LA_pt_2 82
#define MSG_LA_Minimum 83
#define MSG_LA_pt_1 84
#define MSG_LA_label_dpi 85
#define MSG_CH_bitmap_fonts 86
#define MSG_GR_Memory_cacheTitle 87
#define MSG_SL_Memory_cache 88
#define MSG_LA_MB 89
#define MSG_GR_Disc_cacheTitle 90
#define MSG_LA_Directory 91
#define MSG_SL_Disc_cache 92
#define MSG_LA_MBC 93
#define MSG_BT_modify 94
#define MSG_GR_DownloadsTitle 95
#define MSG_LA_Download_manager 96
#define MSG_LA_Download 97
#define MSG_GR_YoutubeTitle 98
#define MSG_LA_YouTube_url 99
#define MSG_GR_PlayersTitle 100
#define MSG_LA_MP3 101
#define MSG_LA_Module 102
#define MSG_BT_save 103
#define MSG_BT_restart 104
#define MSG_BT_cancel 105

#define CATCOMP_LASTID 105

#endif /* CATCOMP_NUMBERS */


/****************************************************************************/


#ifdef CATCOMP_STRINGS

#define MSG_AppDescription_STR "GUI Options for NetSurf"
#define MSG_AppCopyright_STR "Artur Jarosik"
#define MSG_MNlabel0Quit_STR "Quit"
#define MSG_WI_Preferences_STR "Preferences"
#define MSG_GR_Tabs0_STR "General"
#define MSG_GR_Tabs1_STR "Display"
#define MSG_GR_Tabs2_STR "Connection"
#define MSG_GR_Tabs3_STR "Rendering"
#define MSG_GR_Tabs4_STR "Fonts"
#define MSG_GR_Tabs5_STR "Cache"
#define MSG_GR_Tabs6_STR "Favs"
#define MSG_GR_Tabs7_STR "Other"
#define MSG_GR_General_HomepageTitle_STR "Home page"
#define MSG_STR_label_URL_STR "URL"
#define MSG_BT_HomepageNotify0_STR "about:welcome"
#define MSG_BT_Homepage_STR "Use default"
#define MSG_BT_Homepage_2_STR "Use current page"
#define MSG_BT_Homepage_3Notify0_STR "www.google.com"
#define MSG_BT_Homepage_3_STR "Use Google"
#define MSG_GR_General_Content_blockingTitle_STR "Content blocking"
#define MSG_CM_Hide_advertisements_STR "Hide advertisements"
#define MSG_GR_General_Content_languageTitle_STR "Content language"
#define MSG_CM_Get_from_Locale_prefs_STR "Get from Locale prefs"
#define MSG_STR_Get_from_Locale_prefs_STR ""
#define MSG_STR_charset_STR "Charset"
#define MSG_GR_General_HistoryTitle_STR "History"
#define MSG_Keep_history_for_STR "Keep history for"
#define MSG_LA_days_STR "days"
#define MSG_GR_Gen_ScriptingTitle_STR "Scripting"
#define MSG_CM_Enable_JavaScript_STR "Enable JavaScript"
#define MSG_GR_PrivacyTitle_STR "Privacy"
#define MSG_CH_send_referer_STR "Send site referral information"
#define MSG_CH_do_not_track_STR "Send header to tell websites not to track"
#define MSG_GR_ScreenTitle_STR "Screen"
#define MSG_BT_select_STR "Screenmode"
#define MSG_STR_width_STR "W"
#define MSG_STR_height_STR "H"
#define MSG_STR_depth_STR "@"
#define MSG_CH_autodepth_STR "Autodepth"
#define MSG_CH_fullscreen_STR "Fullscreen"
#define MSG_GR_ThemeTitle_STR "Theme"
#define MSG_GR_DitheringTitle_STR "Dithering"
#define MSG_CH_dithering_STR "Low quality but faster dithering"
#define MSG_GR_HTTP_ProxyTitle_STR "HTTP Proxy"
#define MSG_LA_Proxy_type_STR "         Proxy type"
#define MSG_CY_Proxy_type0_STR "No proxy"
#define MSG_CY_Proxy_type1_STR "Simple proxy"
#define MSG_CY_Proxy_type2_STR "Basic authentication"
#define MSG_STR_Host_STR "                    Host"
#define MSG_STR_Port_STR ":"
#define MSG_STR_Username_STR "          Username"
#define MSG_STR_Password_STR "            Password"
#define MSG_STR_Bypass_STR "Bypass proxy for"
#define MSG_GR_FetchingTitle_STR "Fetching"
#define MSG_LA_Maximum_fetchers_STR "Maximum fetchers"
#define MSG_LA_Maximum_retried_fetches_STR "Maximum retried fetches"
#define MSG_LA_Fetchers_per_host_STR "Fetchers per host"
#define MSG_LA_Fetch_timeout_STR "Fetch timeout"
#define MSG_LA_Cached_connections_STR "Cached connections"
#define MSG_GR_ImagesTitle_STR "Images"
#define MSG_CH_Foreground_images_STR "Foreground images"
#define MSG_CH_Background_images_STR "Background images"
#define MSG_GR_AnimationsTitle_STR "Animations"
#define MSG_CH_Disable_animations_STR "Disable animations"
#define MSG_STR_gif_delay_STR "Limit speed to"
#define MSG_LA_seconds_STR "seconds between frames"
#define MSG_GR_ScalingTitle_STR "Scaling"
#define MSG_LA_scale_STR "Scale"
#define MSG_LA_scale_aga_STR "Scale AGA"
#define MSG_GR_CPU_usageTitle_STR "Speed"
#define MSG_CH_cpu_speed_STR "100% CPU  usage for faster rendering"
#define MSG_GR_Fonts_familiesTitle_STR "Fonts families"
#define MSG_LA_Sans_serif_STR "Sans serif"
#define MSG_LA_Serif_STR "Serif"
#define MSG_LA_Sans_serif_italic_STR "Sans serif italic"
#define MSG_LA_Sans_serif_bold_STR "Sans serif bold"
#define MSG_LA_Sans_serif_bold_italic_STR "Sans serif bold italic"
#define MSG_LA_Monospace_STR "Monospace"
#define MSG_LA_Cursive_STR "Cursive"
#define MSG_LA_Fantasy_STR "Fantasy"
#define MSG_GR_Font_sizeTitle_STR "Font size"
#define MSG_LA_Default_STR "Default"
#define MSG_LA_pt_2_STR "pt"
#define MSG_LA_Minimum_STR "Minimum"
#define MSG_LA_pt_1_STR "pt"
#define MSG_LA_label_dpi_STR "DPI"
#define MSG_CH_bitmap_fonts_STR "Bitmap fonts"
#define MSG_GR_Memory_cacheTitle_STR "Memory cache"
#define MSG_SL_Memory_cache_STR "Size"
#define MSG_LA_MB_STR "MB"
#define MSG_GR_Disc_cacheTitle_STR "Disc cache"
#define MSG_LA_Directory_STR "Directory"
#define MSG_SL_Disc_cache_STR "Size"
#define MSG_LA_MBC_STR "MB"
#define MSG_BT_modify_STR "Modify"
#define MSG_GR_DownloadsTitle_STR "Downloads"
#define MSG_LA_Download_manager_STR "Download manager"
#define MSG_LA_Download_STR "Initial download location"
#define MSG_GR_YoutubeTitle_STR "YouTube"
#define MSG_LA_YouTube_url_STR "YouTube web handler"
#define MSG_GR_PlayersTitle_STR "Players"
#define MSG_LA_MP3_STR "MP3 Player"
#define MSG_LA_Module_STR "Module player"
#define MSG_BT_save_STR "Save"
#define MSG_BT_restart_STR "Restart"
#define MSG_BT_cancel_STR "Cancel"

#endif /* CATCOMP_STRINGS */


/****************************************************************************/


#ifdef CATCOMP_ARRAY

struct CatCompArrayType
{
    LONG   cca_ID;
    STRPTR cca_Str;
};

static const struct CatCompArrayType CatCompArray[] =
{
    {MSG_AppDescription,(STRPTR)MSG_AppDescription_STR},
    {MSG_AppCopyright,(STRPTR)MSG_AppCopyright_STR},
    {MSG_MNlabel0Quit,(STRPTR)MSG_MNlabel0Quit_STR},
    {MSG_WI_Preferences,(STRPTR)MSG_WI_Preferences_STR},
    {MSG_GR_Tabs0,(STRPTR)MSG_GR_Tabs0_STR},
    {MSG_GR_Tabs1,(STRPTR)MSG_GR_Tabs1_STR},
    {MSG_GR_Tabs2,(STRPTR)MSG_GR_Tabs2_STR},
    {MSG_GR_Tabs3,(STRPTR)MSG_GR_Tabs3_STR},
    {MSG_GR_Tabs4,(STRPTR)MSG_GR_Tabs4_STR},
    {MSG_GR_Tabs5,(STRPTR)MSG_GR_Tabs5_STR},
    {MSG_GR_Tabs6,(STRPTR)MSG_GR_Tabs6_STR},
    {MSG_GR_Tabs7,(STRPTR)MSG_GR_Tabs7_STR},
    {MSG_GR_General_HomepageTitle,(STRPTR)MSG_GR_General_HomepageTitle_STR},
    {MSG_STR_label_URL,(STRPTR)MSG_STR_label_URL_STR},
    {MSG_BT_HomepageNotify0,(STRPTR)MSG_BT_HomepageNotify0_STR},
    {MSG_BT_Homepage,(STRPTR)MSG_BT_Homepage_STR},
    {MSG_BT_Homepage_2,(STRPTR)MSG_BT_Homepage_2_STR},
    {MSG_BT_Homepage_3Notify0,(STRPTR)MSG_BT_Homepage_3Notify0_STR},
    {MSG_BT_Homepage_3,(STRPTR)MSG_BT_Homepage_3_STR},
    {MSG_GR_General_Content_blockingTitle,(STRPTR)MSG_GR_General_Content_blockingTitle_STR},
    {MSG_CM_Hide_advertisements,(STRPTR)MSG_CM_Hide_advertisements_STR},
    {MSG_GR_General_Content_languageTitle,(STRPTR)MSG_GR_General_Content_languageTitle_STR},
    {MSG_CM_Get_from_Locale_prefs,(STRPTR)MSG_CM_Get_from_Locale_prefs_STR},
    {MSG_STR_Get_from_Locale_prefs,(STRPTR)MSG_STR_Get_from_Locale_prefs_STR},
    {MSG_STR_charset,(STRPTR)MSG_STR_charset_STR},
    {MSG_GR_General_HistoryTitle,(STRPTR)MSG_GR_General_HistoryTitle_STR},
    {MSG_Keep_history_for,(STRPTR)MSG_Keep_history_for_STR},
    {MSG_LA_days,(STRPTR)MSG_LA_days_STR},
    {MSG_GR_Gen_ScriptingTitle,(STRPTR)MSG_GR_Gen_ScriptingTitle_STR},
    {MSG_CM_Enable_JavaScript,(STRPTR)MSG_CM_Enable_JavaScript_STR},
    {MSG_GR_PrivacyTitle,(STRPTR)MSG_GR_PrivacyTitle_STR},
    {MSG_CH_send_referer,(STRPTR)MSG_CH_send_referer_STR},
    {MSG_CH_do_not_track,(STRPTR)MSG_CH_do_not_track_STR},
    {MSG_GR_ScreenTitle,(STRPTR)MSG_GR_ScreenTitle_STR},
    {MSG_BT_select,(STRPTR)MSG_BT_select_STR},
    {MSG_STR_width,(STRPTR)MSG_STR_width_STR},
    {MSG_STR_height,(STRPTR)MSG_STR_height_STR},
    {MSG_STR_depth,(STRPTR)MSG_STR_depth_STR},
    {MSG_CH_autodepth,(STRPTR)MSG_CH_autodepth_STR},
    {MSG_CH_fullscreen,(STRPTR)MSG_CH_fullscreen_STR},
    {MSG_GR_ThemeTitle,(STRPTR)MSG_GR_ThemeTitle_STR},
    {MSG_GR_DitheringTitle,(STRPTR)MSG_GR_DitheringTitle_STR},
    {MSG_CH_dithering,(STRPTR)MSG_CH_dithering_STR},
    {MSG_GR_HTTP_ProxyTitle,(STRPTR)MSG_GR_HTTP_ProxyTitle_STR},
    {MSG_LA_Proxy_type,(STRPTR)MSG_LA_Proxy_type_STR},
    {MSG_CY_Proxy_type0,(STRPTR)MSG_CY_Proxy_type0_STR},
    {MSG_CY_Proxy_type1,(STRPTR)MSG_CY_Proxy_type1_STR},
    {MSG_CY_Proxy_type2,(STRPTR)MSG_CY_Proxy_type2_STR},
    {MSG_STR_Host,(STRPTR)MSG_STR_Host_STR},
    {MSG_STR_Port,(STRPTR)MSG_STR_Port_STR},
    {MSG_STR_Username,(STRPTR)MSG_STR_Username_STR},
    {MSG_STR_Password,(STRPTR)MSG_STR_Password_STR},
    {MSG_STR_Bypass,(STRPTR)MSG_STR_Bypass_STR},
    {MSG_GR_FetchingTitle,(STRPTR)MSG_GR_FetchingTitle_STR},
    {MSG_LA_Maximum_fetchers,(STRPTR)MSG_LA_Maximum_fetchers_STR},
    {MSG_LA_Maximum_retried_fetches,(STRPTR)MSG_LA_Maximum_retried_fetches_STR},
    {MSG_LA_Fetchers_per_host,(STRPTR)MSG_LA_Fetchers_per_host_STR},
    {MSG_LA_Fetch_timeout,(STRPTR)MSG_LA_Fetch_timeout_STR},
    {MSG_LA_Cached_connections,(STRPTR)MSG_LA_Cached_connections_STR},
    {MSG_GR_ImagesTitle,(STRPTR)MSG_GR_ImagesTitle_STR},
    {MSG_CH_Foreground_images,(STRPTR)MSG_CH_Foreground_images_STR},
    {MSG_CH_Background_images,(STRPTR)MSG_CH_Background_images_STR},
    {MSG_GR_AnimationsTitle,(STRPTR)MSG_GR_AnimationsTitle_STR},
    {MSG_CH_Disable_animations,(STRPTR)MSG_CH_Disable_animations_STR},
    {MSG_STR_gif_delay,(STRPTR)MSG_STR_gif_delay_STR},
    {MSG_LA_seconds,(STRPTR)MSG_LA_seconds_STR},
    {MSG_GR_ScalingTitle,(STRPTR)MSG_GR_ScalingTitle_STR},
    {MSG_LA_scale,(STRPTR)MSG_LA_scale_STR},
    {MSG_LA_scale_aga,(STRPTR)MSG_LA_scale_aga_STR},
    {MSG_GR_CPU_usageTitle,(STRPTR)MSG_GR_CPU_usageTitle_STR},
    {MSG_CH_cpu_speed,(STRPTR)MSG_CH_cpu_speed_STR},
    {MSG_GR_Fonts_familiesTitle,(STRPTR)MSG_GR_Fonts_familiesTitle_STR},
    {MSG_LA_Sans_serif,(STRPTR)MSG_LA_Sans_serif_STR},
    {MSG_LA_Serif,(STRPTR)MSG_LA_Serif_STR},
    {MSG_LA_Sans_serif_italic,(STRPTR)MSG_LA_Sans_serif_italic_STR},
    {MSG_LA_Sans_serif_bold,(STRPTR)MSG_LA_Sans_serif_bold_STR},
    {MSG_LA_Sans_serif_bold_italic,(STRPTR)MSG_LA_Sans_serif_bold_italic_STR},
    {MSG_LA_Monospace,(STRPTR)MSG_LA_Monospace_STR},
    {MSG_LA_Cursive,(STRPTR)MSG_LA_Cursive_STR},
    {MSG_LA_Fantasy,(STRPTR)MSG_LA_Fantasy_STR},
    {MSG_GR_Font_sizeTitle,(STRPTR)MSG_GR_Font_sizeTitle_STR},
    {MSG_LA_Default,(STRPTR)MSG_LA_Default_STR},
    {MSG_LA_pt_2,(STRPTR)MSG_LA_pt_2_STR},
    {MSG_LA_Minimum,(STRPTR)MSG_LA_Minimum_STR},
    {MSG_LA_pt_1,(STRPTR)MSG_LA_pt_1_STR},
    {MSG_LA_label_dpi,(STRPTR)MSG_LA_label_dpi_STR},
    {MSG_CH_bitmap_fonts,(STRPTR)MSG_CH_bitmap_fonts_STR},
    {MSG_GR_Memory_cacheTitle,(STRPTR)MSG_GR_Memory_cacheTitle_STR},
    {MSG_SL_Memory_cache,(STRPTR)MSG_SL_Memory_cache_STR},
    {MSG_LA_MB,(STRPTR)MSG_LA_MB_STR},
    {MSG_GR_Disc_cacheTitle,(STRPTR)MSG_GR_Disc_cacheTitle_STR},
    {MSG_LA_Directory,(STRPTR)MSG_LA_Directory_STR},
    {MSG_SL_Disc_cache,(STRPTR)MSG_SL_Disc_cache_STR},
    {MSG_LA_MBC,(STRPTR)MSG_LA_MBC_STR},
    {MSG_BT_modify,(STRPTR)MSG_BT_modify_STR},
    {MSG_GR_DownloadsTitle,(STRPTR)MSG_GR_DownloadsTitle_STR},
    {MSG_LA_Download_manager,(STRPTR)MSG_LA_Download_manager_STR},
    {MSG_LA_Download,(STRPTR)MSG_LA_Download_STR},
    {MSG_GR_YoutubeTitle,(STRPTR)MSG_GR_YoutubeTitle_STR},
    {MSG_LA_YouTube_url,(STRPTR)MSG_LA_YouTube_url_STR},
    {MSG_GR_PlayersTitle,(STRPTR)MSG_GR_PlayersTitle_STR},
    {MSG_LA_MP3,(STRPTR)MSG_LA_MP3_STR},
    {MSG_LA_Module,(STRPTR)MSG_LA_Module_STR},
    {MSG_BT_save,(STRPTR)MSG_BT_save_STR},
    {MSG_BT_restart,(STRPTR)MSG_BT_restart_STR},
    {MSG_BT_cancel,(STRPTR)MSG_BT_cancel_STR},
};

#endif /* CATCOMP_ARRAY */


/****************************************************************************/


#ifdef CATCOMP_BLOCK

static const char CatCompBlock[] =
{
    "\x00\x00\x00\x00\x00\x18"
    MSG_AppDescription_STR "\x00"
    "\x00\x00\x00\x01\x00\x0E"
    MSG_AppCopyright_STR "\x00"
    "\x00\x00\x00\x02\x00\x06"
    MSG_MNlabel0Quit_STR "\x00\x00"
    "\x00\x00\x00\x03\x00\x0C"
    MSG_WI_Preferences_STR "\x00"
    "\x00\x00\x00\x04\x00\x08"
    MSG_GR_Tabs0_STR "\x00"
    "\x00\x00\x00\x05\x00\x08"
    MSG_GR_Tabs1_STR "\x00"
    "\x00\x00\x00\x06\x00\x0C"
    MSG_GR_Tabs2_STR "\x00\x00"
    "\x00\x00\x00\x07\x00\x0A"
    MSG_GR_Tabs3_STR "\x00"
    "\x00\x00\x00\x08\x00\x06"
    MSG_GR_Tabs4_STR "\x00"
    "\x00\x00\x00\x09\x00\x06"
    MSG_GR_Tabs5_STR "\x00"
    "\x00\x00\x00\x0A\x00\x06"
    MSG_GR_Tabs6_STR "\x00\x00"
    "\x00\x00\x00\x0B\x00\x06"
    MSG_GR_Tabs7_STR "\x00"
    "\x00\x00\x00\x0C\x00\x0A"
    MSG_GR_General_HomepageTitle_STR "\x00"
    "\x00\x00\x00\x0D\x00\x04"
    MSG_STR_label_URL_STR "\x00"
    "\x00\x00\x00\x0E\x00\x0E"
    MSG_BT_HomepageNotify0_STR "\x00"
    "\x00\x00\x00\x0F\x00\x0C"
    MSG_BT_Homepage_STR "\x00"
    "\x00\x00\x00\x10\x00\x12"
    MSG_BT_Homepage_2_STR "\x00\x00"
    "\x00\x00\x00\x11\x00\x10"
    MSG_BT_Homepage_3Notify0_STR "\x00\x00"
    "\x00\x00\x00\x12\x00\x0C"
    MSG_BT_Homepage_3_STR "\x00\x00"
    "\x00\x00\x00\x13\x00\x12"
    MSG_GR_General_Content_blockingTitle_STR "\x00\x00"
    "\x00\x00\x00\x14\x00\x14"
    MSG_CM_Hide_advertisements_STR "\x00"
    "\x00\x00\x00\x15\x00\x12"
    MSG_GR_General_Content_languageTitle_STR "\x00\x00"
    "\x00\x00\x00\x16\x00\x16"
    MSG_CM_Get_from_Locale_prefs_STR "\x00"
    "\x00\x00\x00\x17\x00\x02"
    MSG_STR_Get_from_Locale_prefs_STR "\x00\x00"
    "\x00\x00\x00\x18\x00\x08"
    MSG_STR_charset_STR "\x00"
    "\x00\x00\x00\x19\x00\x08"
    MSG_GR_General_HistoryTitle_STR "\x00"
    "\x00\x00\x00\x1A\x00\x12"
    MSG_Keep_history_for_STR "\x00\x00"
    "\x00\x00\x00\x1B\x00\x06"
    MSG_LA_days_STR "\x00\x00"
    "\x00\x00\x00\x1C\x00\x0A"
    MSG_GR_Gen_ScriptingTitle_STR "\x00"
    "\x00\x00\x00\x1D\x00\x12"
    MSG_CM_Enable_JavaScript_STR "\x00"
    "\x00\x00\x00\x1E\x00\x08"
    MSG_GR_PrivacyTitle_STR "\x00"
    "\x00\x00\x00\x1F\x00\x20"
    MSG_CH_send_referer_STR "\x00\x00"
    "\x00\x00\x00\x20\x00\x2A"
    MSG_CH_do_not_track_STR "\x00"
    "\x00\x00\x00\x21\x00\x08"
    MSG_GR_ScreenTitle_STR "\x00\x00"
    "\x00\x00\x00\x22\x00\x0C"
    MSG_BT_select_STR "\x00\x00"
    "\x00\x00\x00\x23\x00\x02"
    MSG_STR_width_STR "\x00"
    "\x00\x00\x00\x24\x00\x02"
    MSG_STR_height_STR "\x00"
    "\x00\x00\x00\x25\x00\x02"
    MSG_STR_depth_STR "\x00"
    "\x00\x00\x00\x26\x00\x0A"
    MSG_CH_autodepth_STR "\x00"
    "\x00\x00\x00\x27\x00\x0C"
    MSG_CH_fullscreen_STR "\x00\x00"
    "\x00\x00\x00\x28\x00\x06"
    MSG_GR_ThemeTitle_STR "\x00"
    "\x00\x00\x00\x29\x00\x0A"
    MSG_GR_DitheringTitle_STR "\x00"
    "\x00\x00\x00\x2A\x00\x22"
    MSG_CH_dithering_STR "\x00\x00"
    "\x00\x00\x00\x2B\x00\x0C"
    MSG_GR_HTTP_ProxyTitle_STR "\x00\x00"
    "\x00\x00\x00\x2C\x00\x14"
    MSG_LA_Proxy_type_STR "\x00"
    "\x00\x00\x00\x2D\x00\x0A"
    MSG_CY_Proxy_type0_STR "\x00\x00"
    "\x00\x00\x00\x2E\x00\x0E"
    MSG_CY_Proxy_type1_STR "\x00\x00"
    "\x00\x00\x00\x2F\x00\x16"
    MSG_CY_Proxy_type2_STR "\x00\x00"
    "\x00\x00\x00\x30\x00\x1A"
    MSG_STR_Host_STR "\x00\x00"
    "\x00\x00\x00\x31\x00\x02"
    MSG_STR_Port_STR "\x00"
    "\x00\x00\x00\x32\x00\x14"
    MSG_STR_Username_STR "\x00\x00"
    "\x00\x00\x00\x33\x00\x16"
    MSG_STR_Password_STR "\x00\x00"
    "\x00\x00\x00\x34\x00\x12"
    MSG_STR_Bypass_STR "\x00\x00"
    "\x00\x00\x00\x35\x00\x0A"
    MSG_GR_FetchingTitle_STR "\x00\x00"
    "\x00\x00\x00\x36\x00\x12"
    MSG_LA_Maximum_fetchers_STR "\x00\x00"
    "\x00\x00\x00\x37\x00\x18"
    MSG_LA_Maximum_retried_fetches_STR "\x00"
    "\x00\x00\x00\x38\x00\x12"
    MSG_LA_Fetchers_per_host_STR "\x00"
    "\x00\x00\x00\x39\x00\x0E"
    MSG_LA_Fetch_timeout_STR "\x00"
    "\x00\x00\x00\x3A\x00\x14"
    MSG_LA_Cached_connections_STR "\x00\x00"
    "\x00\x00\x00\x3B\x00\x08"
    MSG_GR_ImagesTitle_STR "\x00\x00"
    "\x00\x00\x00\x3C\x00\x12"
    MSG_CH_Foreground_images_STR "\x00"
    "\x00\x00\x00\x3D\x00\x12"
    MSG_CH_Background_images_STR "\x00"
    "\x00\x00\x00\x3E\x00\x0C"
    MSG_GR_AnimationsTitle_STR "\x00\x00"
    "\x00\x00\x00\x3F\x00\x14"
    MSG_CH_Disable_animations_STR "\x00\x00"
    "\x00\x00\x00\x40\x00\x10"
    MSG_STR_gif_delay_STR "\x00\x00"
    "\x00\x00\x00\x41\x00\x18"
    MSG_LA_seconds_STR "\x00\x00"
    "\x00\x00\x00\x42\x00\x08"
    MSG_GR_ScalingTitle_STR "\x00"
    "\x00\x00\x00\x43\x00\x06"
    MSG_LA_scale_STR "\x00"
    "\x00\x00\x00\x44\x00\x0A"
    MSG_LA_scale_aga_STR "\x00"
    "\x00\x00\x00\x45\x00\x06"
    MSG_GR_CPU_usageTitle_STR "\x00"
    "\x00\x00\x00\x46\x00\x26"
    MSG_CH_cpu_speed_STR "\x00\x00"
    "\x00\x00\x00\x47\x00\x10"
    MSG_GR_Fonts_familiesTitle_STR "\x00\x00"
    "\x00\x00\x00\x48\x00\x0C"
    MSG_LA_Sans_serif_STR "\x00\x00"
    "\x00\x00\x00\x49\x00\x06"
    MSG_LA_Serif_STR "\x00"
    "\x00\x00\x00\x4A\x00\x12"
    MSG_LA_Sans_serif_italic_STR "\x00"
    "\x00\x00\x00\x4B\x00\x10"
    MSG_LA_Sans_serif_bold_STR "\x00"
    "\x00\x00\x00\x4C\x00\x18"
    MSG_LA_Sans_serif_bold_italic_STR "\x00\x00"
    "\x00\x00\x00\x4D\x00\x0A"
    MSG_LA_Monospace_STR "\x00"
    "\x00\x00\x00\x4E\x00\x08"
    MSG_LA_Cursive_STR "\x00"
    "\x00\x00\x00\x4F\x00\x08"
    MSG_LA_Fantasy_STR "\x00"
    "\x00\x00\x00\x50\x00\x0A"
    MSG_GR_Font_sizeTitle_STR "\x00"
    "\x00\x00\x00\x51\x00\x08"
    MSG_LA_Default_STR "\x00"
    "\x00\x00\x00\x52\x00\x04"
    MSG_LA_pt_2_STR "\x00\x00"
    "\x00\x00\x00\x53\x00\x08"
    MSG_LA_Minimum_STR "\x00"
    "\x00\x00\x00\x54\x00\x04"
    MSG_LA_pt_1_STR "\x00\x00"
    "\x00\x00\x00\x55\x00\x04"
    MSG_LA_label_dpi_STR "\x00"
    "\x00\x00\x00\x56\x00\x0E"
    MSG_CH_bitmap_fonts_STR "\x00\x00"
    "\x00\x00\x00\x57\x00\x0E"
    MSG_GR_Memory_cacheTitle_STR "\x00\x00"
    "\x00\x00\x00\x58\x00\x06"
    MSG_SL_Memory_cache_STR "\x00\x00"
    "\x00\x00\x00\x59\x00\x04"
    MSG_LA_MB_STR "\x00\x00"
    "\x00\x00\x00\x5A\x00\x0C"
    MSG_GR_Disc_cacheTitle_STR "\x00\x00"
    "\x00\x00\x00\x5B\x00\x0A"
    MSG_LA_Directory_STR "\x00"
    "\x00\x00\x00\x5C\x00\x06"
    MSG_SL_Disc_cache_STR "\x00\x00"
    "\x00\x00\x00\x5D\x00\x04"
    MSG_LA_MBC_STR "\x00\x00"
    "\x00\x00\x00\x5E\x00\x08"
    MSG_BT_modify_STR "\x00\x00"
    "\x00\x00\x00\x5F\x00\x0A"
    MSG_GR_DownloadsTitle_STR "\x00"
    "\x00\x00\x00\x60\x00\x12"
    MSG_LA_Download_manager_STR "\x00\x00"
    "\x00\x00\x00\x61\x00\x1A"
    MSG_LA_Download_STR "\x00"
    "\x00\x00\x00\x62\x00\x08"
    MSG_GR_YoutubeTitle_STR "\x00"
    "\x00\x00\x00\x63\x00\x14"
    MSG_LA_YouTube_url_STR "\x00"
    "\x00\x00\x00\x64\x00\x08"
    MSG_GR_PlayersTitle_STR "\x00"
    "\x00\x00\x00\x65\x00\x0C"
    MSG_LA_MP3_STR "\x00\x00"
    "\x00\x00\x00\x66\x00\x0E"
    MSG_LA_Module_STR "\x00"
    "\x00\x00\x00\x67\x00\x06"
    MSG_BT_save_STR "\x00\x00"
    "\x00\x00\x00\x68\x00\x08"
    MSG_BT_restart_STR "\x00"
    "\x00\x00\x00\x69\x00\x08"
    MSG_BT_cancel_STR "\x00\x00"
};

#endif /* CATCOMP_BLOCK */


/****************************************************************************/



struct LocaleInfo
{
    APTR li_LocaleBase;
    APTR li_Catalog;
};




/****************************************************************************/



#endif /* gui_options_cat.h */

