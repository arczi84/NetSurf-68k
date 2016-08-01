
#include "amigaos3/mui/mui.h"

#include <SDI_hook.h>

#include "gui_optionsExtern.h"
#include "gui_options.h"
#include "gui_locale.h"
#include "content/hlcache.h"
#include "utils/nsoption.h"
#include "utils/messages.h"
#include "utils/nsurl.h"
#include "amigaos3/misc.h"
#include "amiga/utf8.h"

void CleanExit(CONST_STRPTR s);
BOOL InitPrefs(void);
void Extra(struct ObjApp * obj);
void ReadOptions(struct ObjApp * obj);
void WriteOptions(struct ObjApp * obj);
void InsertElements(struct ObjApp * obj);
struct ObjApp * obj;

const char * var;	
char cvar[5];
int cint;


void ReadOptions(struct ObjApp * obj)
{
	/*General*/
	set(obj->STR_label_URL, MUIA_String_Contents, nsoption_charp(homepage_url));
	set(obj->CM_Hide_advertisements,MUIA_Selected, nsoption_bool(block_advertisements));	
	set(obj->CM_Enable_JavaScript,MUIA_Selected, nsoption_bool(enable_javascript));	
	set(obj->CH_send_referer, MUIA_Selected, nsoption_bool(send_referer));
	set(obj->CH_do_not_track, MUIA_Selected, nsoption_bool(do_not_track));
	set(obj->CM_Get_from_Locale_prefs, MUIA_Selected, nsoption_bool(accept_lang_locale));
		nsoption_charp(accept_language) = strndup(nsoption_charp(accept_language),2);
	set(obj->STR_Get_from_Locale_prefs, MUIA_String_Contents, nsoption_charp(accept_language));
		sprintf(cvar, "%d",  nsoption_int(disc_cache_age));	
	set(obj->STR_history, MUIA_String_Contents, cvar);
	set(obj->STR_charset, MUIA_String_Contents, nsoption_charp(accept_charset));
	/*Connection*/
	set(obj->STR_Host, MUIA_String_Contents, nsoption_charp(http_proxy_host));		
		sprintf(cvar, "%d",  nsoption_int(http_proxy_port));
	set(obj->STR_Port, MUIA_String_Contents, cvar);			
	set(obj->STR_Username, MUIA_String_Contents, nsoption_charp(http_proxy_auth_user));		
	set(obj->STR_Password, MUIA_String_Contents, nsoption_charp(http_proxy_auth_pass));	
	set(obj->STR_Bypass, MUIA_String_Contents, nsoption_charp(http_proxy_noproxy));
		sprintf(cvar, "%d",  nsoption_int(max_fetchers));
	set(obj->STR_Maximum_fetchers, MUIA_String_Contents, cvar);
		sprintf(cvar, "%d",  nsoption_int(max_retried_fetches));
	set(obj->STR_Maximum_retried, MUIA_String_Contents, cvar);
		sprintf(cvar, "%d",  nsoption_int(max_fetchers_per_host));	
	set(obj->STR_Fetchers_per_host, MUIA_String_Contents, cvar);
		sprintf(cvar, "%d",  nsoption_int(curl_fetch_timeout));	
	set(obj->STR_Fetch_timeout, MUIA_String_Contents, cvar);
		sprintf(cvar, "%d",  nsoption_int(max_cached_fetch_handles));	
	set(obj->STR_Cached_connections, MUIA_String_Contents, cvar);	
	/*Display*/	
		sprintf(cvar, "%d",  nsoption_int(window_width));
	set(obj->STR_width, MUIA_String_Contents, cvar);
		sprintf(cvar, "%d",  nsoption_int(window_height));
	set(obj->STR_height, MUIA_String_Contents, cvar);
		sprintf(cvar, "%d",  nsoption_int(window_depth));
	set(obj->STR_depth, MUIA_String_Contents, cvar);
	set(obj->CH_autodepth, MUIA_Selected, nsoption_bool(autodetect_depth));
	set(obj->CH_fullscreen, MUIA_Selected, nsoption_bool(fullscreen));
	set(obj->PA_theme, MUIA_String_Contents, nsoption_charp(theme));
	set(obj->CH_dithering, MUIA_Selected, nsoption_bool(low_dither_quality));
	/*Rendering*/
	set(obj->CH_Foreground_images,MUIA_Selected, nsoption_bool(foreground_images));
	set(obj->CH_Background_images,MUIA_Selected, nsoption_bool(background_images));
	set(obj->CH_Disable_animations,MUIA_Selected, !nsoption_bool(animate_images));
			sprintf(cvar, "%.2f",  (float)nsoption_int(minimum_gif_delay)/100);	
	set(obj->STR_gif_delay, MUIA_String_Contents, cvar);
	set(obj->SL_scale, MUIA_Slider_Level, scale_cp);
	set(obj->SL_scale_aga, MUIA_Slider_Level, nsoption_int(scale_aga));	
	set(obj->CH_cpu_speed,MUIA_Selected, nsoption_bool(warp_mode));
	/*Cache*/
	set(obj->SL_Memory_cache, MUIA_Slider_Level, nsoption_int(memory_cache_size) / (1024 * 1024));
	set(obj->SL_Disc_cache, MUIA_Slider_Level, nsoption_int(disc_cache_size) / (1024 * 1024));
	set(obj->PA_Cache_directory, MUIA_String_Contents, nsoption_charp(cache_dir));
	/*Fonts*/
	set(obj->STR_PA_Sans_serif, MUIA_String_Contents, nsoption_charp(fb_face_sans_serif));		
	set(obj->STR_PA_Sans_serif_italic, MUIA_String_Contents, nsoption_charp(fb_face_sans_serif_italic));
	set(obj->STR_PA_Serif, MUIA_String_Contents, nsoption_charp(fb_face_serif));	
	set(obj->STR_PA_Sans_serif_bold, MUIA_String_Contents, nsoption_charp(fb_face_sans_serif_bold));
	set(obj->STR_PA_Sans_serif_bold_italic, MUIA_String_Contents, nsoption_charp(fb_face_sans_serif_italic_bold));	
	set(obj->STR_PA_Monospace, MUIA_String_Contents, nsoption_charp(fb_face_monospace));
	set(obj->STR_PA_Cursive, MUIA_String_Contents, nsoption_charp(fb_face_cursive));
	set(obj->STR_PA_Fantasy, MUIA_String_Contents, nsoption_charp(fb_face_fantasy));	
		sprintf(cvar, "%d",  nsoption_int(font_size)/10);	
	set(obj->STR_Default, MUIA_String_Contents, cvar);
		sprintf(cvar, "%d",  nsoption_int(font_min_size)/10);	
	set(obj->STR_Minimum, MUIA_String_Contents, cvar);	
	set(obj->SL_dpi, MUIA_Slider_Level, nsoption_int(browser_dpi));
	set(obj->CH_bitmap_fonts, MUIA_Selected, nsoption_bool(bitmap_fonts));
	/*Other*/
	set(obj->STR_PA_Download_manager, MUIA_String_Contents, nsoption_charp(download_manager));	
	set(obj->PA_Download_dir, MUIA_String_Contents, nsoption_charp(download_path));
	set(obj->STR_YT, MUIA_String_Contents, nsoption_charp(youtube_handler));
	set(obj->PA_Mp3_player, MUIA_String_Contents, nsoption_charp(net_player));	
	set(obj->PA_Module, MUIA_String_Contents, nsoption_charp(module_player));	
	
}

void WriteOptions(struct ObjApp * obj)
{
	/*General*/
	get(obj->CM_Enable_JavaScript, MUIA_Selected, &var);
		nsoption_bool(enable_javascript) = var;
	get(obj->CM_Hide_advertisements,MUIA_Selected, &var);
		nsoption_bool(block_advertisements) = var;	
	get(obj->STR_label_URL, MUIA_String_Contents, &var);
		nsoption_charp(homepage_url) = strdup(var);
	get(obj->CH_send_referer, MUIA_Selected, &var);
		nsoption_bool(send_referer) = strdup(var);
	get(obj->CH_do_not_track, MUIA_Selected, &var);
		nsoption_bool(do_not_track) = strdup(var);	
	get(obj->CM_Get_from_Locale_prefs, MUIA_Selected, &var);
		nsoption_bool(accept_lang_locale) = var;
	get(obj->STR_Get_from_Locale_prefs, MUIA_String_Contents, &var);
		nsoption_charp(accept_language) = strdup(var);
	get(obj->STR_charset, MUIA_String_Contents, &var);
		nsoption_charp(accept_charset) = strdup(var);
	/*Connection*/
	get(obj->STR_Host, MUIA_String_Contents, &var);	
		nsoption_charp(http_proxy_host) = strdup(var);	
	get(obj->STR_Port, MUIA_String_Contents, &var);		
		nsoption_int(http_proxy_port) = atoi(var);
	get(obj->STR_Username, MUIA_String_Contents, &var);
		nsoption_charp(http_proxy_auth_user) = strdup(var);
	get(obj->STR_Password, MUIA_String_Contents, &var);
		nsoption_charp(http_proxy_auth_pass) = strdup(var);
	get(obj->STR_Bypass, MUIA_String_Contents, &var);
		nsoption_charp(http_proxy_noproxy) = strdup(var);	
	get(obj->STR_Maximum_fetchers, MUIA_String_Contents, &var);
		nsoption_int(max_fetchers) = atoi(var);
	get(obj->STR_Maximum_retried, MUIA_String_Contents, &var);
		nsoption_int(max_retried_fetches) = atoi(var);
	get(obj->STR_Fetchers_per_host, MUIA_String_Contents, &var);
		nsoption_int(max_fetchers_per_host) = atoi(var);	
	get(obj->STR_Fetch_timeout, MUIA_String_Contents, &var);
		nsoption_int(curl_fetch_timeout) = atoi(var);	
	get(obj->STR_Cached_connections, MUIA_String_Contents, &var);
		nsoption_int(max_cached_fetch_handles) = atoi(var);
	/*Display*/
	get(obj->STR_width, MUIA_String_Contents, &var);
		nsoption_int(window_width)= atoi(var);
	get(obj->STR_height, MUIA_String_Contents, &var);
		nsoption_int(window_height)= atoi(var);		
	get(obj->STR_depth, MUIA_String_Contents, &var);
		nsoption_int(window_depth)= atoi(var);	
	get(obj->CH_fullscreen, MUIA_Selected, &var);
		nsoption_bool(fullscreen)= var;
	get(obj->CH_autodepth, MUIA_Selected, &var);
		nsoption_bool(autodetect_depth)= var;
	get(obj->PA_theme, MUIA_String_Contents, &var);
		nsoption_charp(theme) = strdup(var);
	get(obj->CH_dithering, MUIA_Selected, &var);
		nsoption_bool(low_dither_quality) = var;
	/*Rendering*/	
	get(obj->CH_Foreground_images,MUIA_Selected, &var);
		nsoption_bool(foreground_images) = var;
	get(obj->CH_Background_images,MUIA_Selected, &var);
		nsoption_bool(background_images) = var;
	get(obj->CH_Disable_animations,MUIA_Selected, &var);
		nsoption_bool(animate_images) = !var;
	get(obj->STR_gif_delay, MUIA_String_Contents, &var);
		nsoption_int(minimum_gif_delay) = atof(var)*100+0.01;	
	get(obj->SL_scale, MUIA_Slider_Level, &cint);
		nsoption_int(scale) = cint;
	get(obj->SL_scale_aga, MUIA_Slider_Level, &cint);
		nsoption_int(scale_aga) = cint;	
	get(obj->CH_cpu_speed,MUIA_Selected, &var);
		nsoption_bool(warp_mode) = var;
	/*Cache*/
	get(obj->SL_Memory_cache, MUIA_Slider_Level, &cint);
		nsoption_int(memory_cache_size) = cint * 1024 * 1024;
	get(obj->SL_Disc_cache, MUIA_Slider_Level, &cint);
		nsoption_int(disc_cache_size) = cint * 1024 * 1024;	
	get(obj->PA_Cache_directory, MUIA_String_Contents, &var);	
		nsoption_charp(cache_dir) = strdup(var);
	/*Fonts*/	
	get(obj->STR_PA_Sans_serif, MUIA_String_Contents, &var);
		nsoption_charp(fb_face_sans_serif) = strdup(var);	
	get(obj->STR_PA_Sans_serif_italic, MUIA_String_Contents, &var);
		nsoption_charp(fb_face_sans_serif_italic) = strdup(var);
	get(obj->STR_PA_Serif, MUIA_String_Contents, &var);
		nsoption_charp(fb_face_serif) = strdup(var);
	get(obj->STR_PA_Sans_serif_bold, MUIA_String_Contents, &var);
		nsoption_charp(fb_face_sans_serif_bold) = strdup(var);
	get(obj->STR_PA_Sans_serif_bold_italic, MUIA_String_Contents, &var);
		nsoption_charp(fb_face_sans_serif_italic_bold) = strdup(var);		
	get(obj->STR_PA_Monospace, MUIA_String_Contents, &var);
		nsoption_charp(fb_face_monospace) = strdup(var);		
	get(obj->STR_PA_Cursive, MUIA_String_Contents, &var);
		nsoption_charp(fb_face_cursive) = strdup(var);	
	get(obj->STR_PA_Fantasy, MUIA_String_Contents, &var);
		nsoption_charp(fb_face_fantasy) = strdup(var);	
	get(obj->STR_Default, MUIA_String_Contents, &var);
		nsoption_int(font_size) = atoi(var) * 10;			
	get(obj->STR_Minimum, MUIA_String_Contents, &var);
		nsoption_int(font_min_size) = atoi(var) * 10;
	get(obj->SL_dpi, MUIA_Slider_Level, &cint);
		nsoption_int(browser_dpi) = cint;
	get(obj->CH_bitmap_fonts, MUIA_Selected, &var);
		nsoption_bool(bitmap_fonts)= var;
	/*Other*/
	get(obj->STR_PA_Download_manager, MUIA_String_Contents, &var);
		nsoption_charp(download_manager) = strdup(var);
	get(obj->PA_Download_dir, MUIA_String_Contents, &var);	
		nsoption_charp(download_path) = strdup(var);
	get(obj->STR_YT, MUIA_String_Contents, &var);
		nsoption_charp(youtube_handler) = strdup(var);
	get(obj->PA_Mp3_player, MUIA_String_Contents, &var);
		nsoption_charp(net_player) = strdup(var);
	get(obj->PA_Module, MUIA_String_Contents, &var);
		nsoption_charp(module_player) = strdup(var);

	nsoption_write("PROGDIR:Resources/Options", NULL, NULL);
}


long ScreenMode(void)
{

	if((AslBase = OpenLibrary("asl.library", 0)) != NULL)
	{

		struct ScreenModeRequester *sm;

		if((sm = AllocAslRequest(ASL_ScreenModeRequest,NULL)))
		{
		#ifdef DEBUG
		Printf("sm %08lx\n", sm);
		#endif
			if(AslRequestTags(sm,
					ASLSM_DoDepth, TRUE,
					ASLSM_DoWidth, TRUE,
					ASLSM_DoHeight, TRUE,
					ASLSM_MinDepth, 8,
					ASLSM_MaxDepth, 32,
					ASLSM_MinWidth, 640,
					ASLSM_MinHeight,480,
					ASLSM_MaxWidth, 1920,
					ASLSM_MaxHeight,1080,
					TAG_DONE))
			{

					sprintf(cvar, "%d",  sm->sm_DisplayWidth);
				set(obj->STR_width, MUIA_String_Contents, cvar);
					sprintf(cvar, "%d",  sm->sm_DisplayHeight);
				set(obj->STR_height, MUIA_String_Contents, cvar);
					sprintf(cvar, "%d",  sm->sm_DisplayDepth);
				set(obj->STR_depth, MUIA_String_Contents, cvar);
				
				int depth = sm->sm_DisplayDepth;
				if (depth == 8)
					set(obj->CH_fullscreen, MUIA_Selected, TRUE);
				else if ((depth > 8) && (depth < 16))
					set(obj->STR_depth, MUIA_String_Contents, "16");
				else if (depth> 16) 
					set(obj->STR_depth, MUIA_String_Contents, "32");	
			}

			FreeAslRequest(sm);
		}
		
	if (AslBase)
		CloseLibrary(AslBase);
	}

	return 0;
}
MakeStaticHook(ScreenModeHook, ScreenMode);

void Restart(struct Hook *h, Object *o)
{
	restart = true;
}

void Save(struct Hook *h, Object *o)
{
	save = true;
}

void Proxy(struct Hook *h, Object *o)
{
	int num;
	
	get(obj->CY_Proxy_type, MUIA_Cycle_Active, &var);
		num = (int)var;

	if (num == 0)
	{
		set(obj->STR_Host, MUIA_Disabled, TRUE);
		set(obj->STR_Port, MUIA_Disabled, TRUE);		
		set(obj->STR_Username, MUIA_Disabled, TRUE);
		set(obj->STR_Password, MUIA_Disabled, TRUE);
		set(obj->STR_Bypass, MUIA_Disabled, TRUE);
		nsoption_bool(http_proxy) = false;
		nsoption_int(http_proxy_auth) = OPTION_HTTP_PROXY_AUTH_NONE;		
	}		
	else if (num == 1)
	{
		set(obj->STR_Username, MUIA_Disabled, TRUE);
		set(obj->STR_Password, MUIA_Disabled, TRUE);
		nsoption_bool(http_proxy) = true;
		nsoption_int(http_proxy_auth) = OPTION_HTTP_PROXY_AUTH_NONE;		
	}
	else if (num == 2)
	{
		nsoption_bool(http_proxy) = true;
		nsoption_int(http_proxy_auth) = OPTION_HTTP_PROXY_AUTH_BASIC;
	}
	
}

void Reload(struct Hook *h, Object *o)
{
}

void SetUrl(struct Hook *h, Object *o)
{
	set(o, MUIA_String_Contents,
		nsurl_access(url_global));
}

void CleanExit(CONST_STRPTR s)
{

	CloseLibrary(MUIMasterBase);
	Locale_Close();
}

/* Struktura elementu listy */

struct Ulubione
 {
  char *nazwa;
  char *adres;
 };


/* tu wstawiam elementy do listy dwiema różnymi metodami */

void InsertElements(struct ObjApp * obj)
 {

/* Elementy umieszczane na liście przy jej tworzeniu */

struct Ulubione Adresy[] = {
							{nsoption_charp(favourite_1_label), nsoption_charp(favourite_1_url)},
							{nsoption_charp(favourite_2_label), nsoption_charp(favourite_2_url)},
						    {nsoption_charp(favourite_3_label), nsoption_charp(favourite_3_url)},
							{nsoption_charp(favourite_4_label), nsoption_charp(favourite_4_url)},
							{nsoption_charp(favourite_5_label), nsoption_charp(favourite_5_url)},
						    {nsoption_charp(favourite_6_label), nsoption_charp(favourite_6_url)},						
							{nsoption_charp(favourite_7_label), nsoption_charp(favourite_7_url)},
							{nsoption_charp(favourite_8_label), nsoption_charp(favourite_8_url)},
						    {nsoption_charp(favourite_9_label), nsoption_charp(favourite_9_url)},
							{nsoption_charp(favourite_10_label), nsoption_charp(favourite_10_url)},
							{nsoption_charp(favourite_11_label), nsoption_charp(favourite_11_url)},
						    {nsoption_charp(favourite_12_label), nsoption_charp(favourite_12_url)}
							};

struct Ulubione *Lista[] = {&Adresy[0], &Adresy[1], &Adresy[2], &Adresy[3], &Adresy[4], &Adresy[5], &Adresy[6],
							&Adresy[7], &Adresy[8], &Adresy[9], &Adresy[10], &Adresy[11], NULL};

  DoMethod (obj->Listview, MUIM_List_Insert, Lista, -1, MUIV_List_Insert_Bottom);


  return;
 }


 long FavDisplayer (struct Ulubione *ulubione reg(a1), char **teksty reg(a2))
 {
	static char nazl[11], url[80];

	if (!ulubione)
     {
     teksty[0] = (char*) messages_get("TreeviewLabelTitle");
     teksty[1] = (char*) messages_get("Link");
     return 0;
     }	 
   
	sprintf (nazl, "%s\n", ulubione->nazwa);
	teksty[0] = nazl;
	sprintf (url, "%s\n", ulubione->adres);	
	teksty[1] = url;
	return 0;
  
 };
 

/* Hook konstrukcyjny */

long FavConstructor (struct Ulubione *ulubione reg(a1), APTR mempool reg(a2))
 {
  struct Ulubione *t_copy;
  char *n_copy;

  if (t_copy = AllocPooled (mempool, sizeof (struct Ulubione)))
   {
    if (n_copy = AllocPooled (mempool, strlen (ulubione->nazwa) + 1))
     {
      strcpy (n_copy, ulubione->nazwa);
      t_copy->nazwa = n_copy;
	   if (n_copy = AllocPooled (mempool, strlen (ulubione->adres) + 1))
		{
		  strcpy (n_copy, ulubione->adres);
		  t_copy->adres = n_copy;
			}
      return (long)t_copy;
     }
    else FreePooled (mempool, t_copy, sizeof (struct Ulubione));
   }
  return 0;
 }

/* Hook destrukcyjny */

long FavDestructor (struct Ulubione *ulubione reg(a1), APTR mempool reg(a2))
 {
  if (ulubione->nazwa) FreePooled (mempool, ulubione->nazwa, strlen (ulubione->nazwa)
   + 1);
  if (ulubione->adres) FreePooled (mempool, ulubione->adres, strlen (ulubione->adres)
   + 1);
  if (ulubione) FreePooled (mempool, ulubione, sizeof (struct Ulubione));
  return 0;
 }


	
void GetFavAttr(struct Hook *h, Object *o)
{	
	struct Ulubione *ulubione2;
	
	DoMethod (obj->Listview, MUIM_List_GetEntry, MUIV_List_GetEntry_Active, &ulubione2);

	set(obj->STR_fav_label, MUIA_String_Contents, strndup(ulubione2->nazwa,10));
	set(obj->STR_fav_url, MUIA_String_Contents, strndup(ulubione2->adres,79));
}

void ModifyFav(struct Hook *h, Object *o)
{
char nazwa[10];
char url[80];
const char *val;
int inum;

	get(obj->STR_fav_label, MUIA_String_Contents, &val);
		snprintf (nazwa, 10, "%s", val);
	get(obj->STR_fav_url, MUIA_String_Contents, &val);
		snprintf (url, 80, "%s", val);
	get(obj->Listview, MUIA_List_Active, &val);
		inum = (int)val+1;

DoMethod (obj->Listview, MUIM_List_Remove, MUIV_List_GetEntry_Active);

struct Ulubione nowy_adres= {nazwa, url};

DoMethod (obj->Listview, MUIM_List_InsertSingle, &nowy_adres, MUIV_List_GetEntry_Active);
DoMethod (obj->Listview, MUIM_List_Redraw, MUIV_List_GetEntry_Active);


	if (inum == 1 ) {	
		   nsoption_charp(favourite_1_url) = strdup(url);
		   nsoption_charp(favourite_1_label) = strndup(nazwa,10);
		   }
	else if (inum == 2 ) {
		   nsoption_charp(favourite_2_url) = strdup(url);
		   nsoption_charp(favourite_2_label) = strndup(nazwa,10);	   	      
		   } 
	else if (inum == 3 ) {
		   nsoption_charp(favourite_3_url) = strdup(url);
		   nsoption_charp(favourite_3_label) = strndup(nazwa,10);	     
		   } 
	else if (inum == 4 ) {
		   nsoption_charp(favourite_4_url) = strdup(url);
		   nsoption_charp(favourite_4_label) = strndup(nazwa,10);	   	      
		   } 	
	else if (inum == 5 )  {
		   nsoption_charp(favourite_5_url) = strdup(url);
		   nsoption_charp(favourite_5_label) = strndup(nazwa,10);	   	   
		   } 
	else if (inum == 6  ) {
		   nsoption_charp(favourite_6_url) = strdup(url);
		   nsoption_charp(favourite_6_label) = strndup(nazwa,10);	   	   
		   } 
	else if (inum == 7  ) {
		   nsoption_charp(favourite_7_url) = strdup(url);
		   nsoption_charp(favourite_7_label) = strndup(nazwa,10);	   	     
		   } 	
	else if (inum == 8  ) {
		   nsoption_charp(favourite_8_url) = strdup(url);
		   nsoption_charp(favourite_8_label) = strndup(nazwa,10);  
		   }	 
	else if (inum == 9 ) {
		   nsoption_charp(favourite_9_url) = strdup(url);
		   nsoption_charp(favourite_9_label) = strndup(nazwa,10);	         
		   } 
	else if (inum == 10 ) {
		   nsoption_charp(favourite_10_url) = strdup(url);
		   nsoption_charp(favourite_10_label) = strndup(nazwa,10);	   	   		     
		   } 	
	else if (inum == 11 ) {
		   nsoption_charp(favourite_11_url) = strdup(url);
		   nsoption_charp(favourite_11_label) = strndup(nazwa,10);	   
		   } 
	else if (inum == 12 ) {
		   nsoption_charp(favourite_12_url) = strdup(url);
		   nsoption_charp(favourite_12_label) = strndup(nazwa,10); 
		}

	//free(url);
	//free(nazwa);

}
 
BOOL InitPrefs(void)
{
	if ((MUIMasterBase = OpenLibrary("muimaster.library", 11)) == NULL)
	{
		CleanExit("Can't open muimaster.library v11\n");
	}
				/*Open netsurf.catalog file*/
	Locale_Open("NetSurf.catalog", 0, 0);
	
	return TRUE;
}

void Extra(struct ObjApp * obj)
{
		
	if(nsoption_bool(http_proxy) == true)
		{	
		if (nsoption_int(http_proxy_auth) == OPTION_HTTP_PROXY_AUTH_NONE)
			{
			set(obj->STR_Host, MUIA_Disabled, FALSE);
			set(obj->STR_Port, MUIA_Disabled, FALSE);		
			set(obj->STR_Bypass, MUIA_Disabled, FALSE);	
			set(obj->CY_Proxy_type, MUIA_Cycle_Active, 1);	
			}
		else
			{
			set(obj->STR_Host, MUIA_Disabled, FALSE);
			set(obj->STR_Port, MUIA_Disabled, FALSE);			
			set(obj->STR_Username, MUIA_Disabled, FALSE);
			set(obj->STR_Password, MUIA_Disabled, FALSE);
			set(obj->STR_Bypass, MUIA_Disabled, FALSE);	
			set(obj->CY_Proxy_type, MUIA_Cycle_Active, 2);	
			}
		}
	
	if(nsoption_bool(autodetect_depth) == false)
		set(obj->STR_depth, MUIA_Disabled, FALSE);
	else
		set(obj->STR_depth, MUIA_Disabled, TRUE);
}

int OpenPrefs(void)
{
	InitPrefs();
	ULONG sigs = 0;
	obj = CreateApp();
	restart, save = false;
	
	if (obj)
	{
		InsertElements(obj);
		DoMethod(obj->BT_select,
		MUIM_Notify, MUIA_Pressed, FALSE,obj->BT_select,2,MUIM_CallHook, &ScreenModeHook);
		ReadOptions(obj);
		Extra(obj);

		while (DoMethod(obj->App, MUIM_Application_NewInput, (IPTR)&sigs)
			!= MUIV_Application_ReturnID_Quit)
		{
			if (restart)	
				break;
			
		    if (sigs)
		      	{
		        sigs = Wait(sigs | SIGBREAKF_CTRL_C);
		        if (sigs & SIGBREAKF_CTRL_C) break;
		      	}
		}
		
		set(obj->WI_Preferences, MUIA_Window_Open, FALSE);
		
		if (restart || save)	
			WriteOptions(obj);
		DisposeApp(obj);
	}
	else
	{
		CleanExit("Can't create application\n");
	}
	CleanExit(NULL);
	return 0;
}
