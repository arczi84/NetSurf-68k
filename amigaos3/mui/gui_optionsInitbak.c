
#include "demo.h"
#include <SDI_hook.h>

#include "gui_optionsExtern.h"
#include "gui_options.h"
#include "content/hlcache.h"
#include "utils/nsoption.h"
#include "utils/nsurl.h"
#include "amigaos3/misc.h"

void CleanExit(CONST_STRPTR s);
BOOL InitPrefs(void);
void ReadOptions(struct ObjApp * obj);
void WriteOptions(struct ObjApp * obj);
void InsertElements(struct ObjApp * obj);
struct ObjApp * obj;
HOOKPROTONHNONP(ScreenMode, ULONG);

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
	set(obj->CH_fullscreen, MUIA_Selected, nsoption_bool(fullscreen));
	set(obj->PA_theme, MUIA_String_Contents, nsoption_charp(theme));
	/*Rendering*/
	set(obj->CH_Foreground_images,MUIA_Selected, nsoption_bool(foreground_images));
	set(obj->CH_Background_images,MUIA_Selected, nsoption_bool(background_images));
	set(obj->CH_Disable_animations,MUIA_Selected, !nsoption_bool(animate_images));
			sprintf(cvar, "0.%d",  nsoption_int(minimum_gif_delay));	
	set(obj->STR_gif_delay, MUIA_String_Contents, cvar);
	set(obj->SL_scale, MUIA_Slider_Level, nsoption_int(scale));
	set(obj->SL_scale_aga, MUIA_Slider_Level, nsoption_int(scale_aga));	
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
	get(obj->PA_theme, MUIA_String_Contents, &var);
		nsoption_charp(theme) = strdup(var);
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


//void ScreenMode(struct Hook *h, Object *o)
HOOKPROTONHNONP(ScreenMode, ULONG)
{
	printf("hook\n");
	if((AslBase = OpenLibrary("asl.library", 0)) != NULL)
	{
		struct ScreenModeRequester *sm;
		printf("asl %08lx\n", AslBase);

		if((sm = AllocAslRequest(ASL_ScreenModeRequest,NULL)))
		{
			printf("sm %08lx\n", sm);
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
				printf("ok\n");
				
			//while 	(sm->sm_DisplayWidth < 1){
			//if (sm) {
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

	printf("hook done\n");

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

void Reload(struct Hook *h, Object *o)
{
	//Printf("test");
}

void SetUrl(struct Hook *h, Object *o)
{
	set(o, MUIA_String_Contents,
		nsurl_access(url_global));
}

void CleanExit(CONST_STRPTR s)
{
	if (s)
	{
		Printf(s);
	}
	CloseLibrary(MUIMasterBase);

}

void GetFavUrl(struct Hook *h, Object *o){};
void GetFavLabel(struct Hook *h, Object *o){};

/* Struktura elementu listy */

struct Ulubione
 {
  char *nazwa;
  char *adres;
 };


/* tu wstawiam elementy do listy dwiema róűnymi metodami */

void InsertElements(struct ObjApp * obj)
 {

/* Elementy umieszczane na liôcie przy jej tworzeniu */

struct Ulubione Adresy[] = {{nsoption_charp(favourite_1_label), nsoption_charp(favourite_1_url)},
							{nsoption_charp(favourite_2_label), nsoption_charp(favourite_2_url)},
						    {nsoption_charp(favourite_3_label), nsoption_charp(favourite_3_url)}};

struct Ulubione *Lista[] = {&Adresy[0], &Adresy[1], &Adresy[2], NULL};

//struct Ulubione Adres[] = {"diashjdia","3254asd"};
//struct Ulubione *Adr[] = {&Adr[0], NULL};

  DoMethod (obj->Listview, MUIM_List_Insert, Lista, -1, MUIV_List_Insert_Bottom);

 // DoMethod (obj->Listview, MUIM_List_Sort);       /* sortujë listë */

 // DoMethod (obj->Listview, MUIM_List_InsertSingle, Lista, MUIV_List_Insert_Bottom);
 // DoMethod (obj->Listview, MUIM_List_InsertSingle, nsoption_charp(favourite_1_url), MUIV_List_Insert_Sorted);


 
  DoMethod (obj->Listview, MUIM_List_Redraw, 0);

  return;
 }

  /* Struktura elementu listy */

struct Towar
 {
  char *nazwa;
  long ile_na_skladzie;
 };
 
 /* tu wstawiam elementy do listy dwiema róűnymi metodami */

void InsertElements2 (struct ObjApp * obj)
 {
  struct Towar amigi = {"Amiga 1200", 0};
  struct Towar hardware[] = {"BVision", 4, "BlizzardPPC", 12, "mousepady", 234};
  struct Towar *hard[] = {&hardware[0], &hardware[1], &hardware[2], NULL};

  DoMethod (obj->Listview, MUIM_List_Insert, hard, -1, MUIV_List_Insert_Top);

  DoMethod (obj->Listview, MUIM_List_Sort);       /* sortujë listë */

  DoMethod (obj->Listview, MUIM_List_InsertSingle, &amigi, MUIV_List_Insert_Sorted);


  return;
 }
 

// void FavDisplayer(struct Hook *h, Object *o)
 long FavDisplayer (struct Towar *towar reg(a1), char **teksty reg(a2))
 {
	static char textl[64], ilosc[12];

	  if (towar->ile_na_skladzie > 0)
	   {
		teksty[0] = towar->nazwa;
		sprintf (ilosc, "%ld", towar->ile_na_skladzie);
	   }
	  else
	   {
		sprintf (textl, "\33b%s\33n", towar->nazwa);
		teksty[0] = textl;
		sprintf (ilosc, "\33b%ld\33n", towar->ile_na_skladzie);
	   }
	  teksty[1] = ilosc;
	  return 0;
  
 };
 

/* Hook konstrukcyjny */

long TowarConstructor (struct Towar *towar reg(a1), APTR mempool reg(a2))
 {
  struct Towar *t_copy;
  char *n_copy;

  if (t_copy = AllocPooled (mempool, sizeof (struct Towar)))
   {
    if (n_copy = AllocPooled (mempool, strlen (towar->nazwa) + 1))
     {
      strcpy (n_copy, towar->nazwa);
      t_copy->nazwa = n_copy;
	  
      t_copy->ile_na_skladzie = towar->ile_na_skladzie;
	  
      return (long)t_copy;
     }
    else FreePooled (mempool, t_copy, sizeof (struct Towar));
   }
  return NULL;
 }

/* Hook destrukcyjny */

long TowarDestructor (struct Towar *towar reg(a1), APTR mempool reg(a2))
 {
  if (towar->nazwa) FreePooled (mempool, towar->nazwa, strlen (towar->nazwa)
   + 1);
  if (towar) FreePooled (mempool, towar, sizeof (struct Towar));
  return 0;
 }

/* Hook wyôwietlajâcy */

long TowarDisplayer (struct Towar *towar reg(a1), char **teksty reg(a2))
 {
  static char textl[12], ilosc[12];

  if (towar->ile_na_skladzie > 0)
   {
    teksty[0] = towar->nazwa;
    sprintf (ilosc, "%ld", towar->ile_na_skladzie);
   }
  else
   {
    sprintf (textl, "\33b%s\33n", towar->nazwa);
    teksty[0] = textl;
    sprintf (ilosc, "\33b%ld\33n", towar->ile_na_skladzie);
   }
  teksty[1] = ilosc;
  return 0;
 }

 
void GetFavAttr(struct Hook *h, Object *o)
{
	struct Ulubione2
		 {
		  char *nazwa;
		  char *adres;
		 };
}
 
BOOL InitPrefs(void)
{
	if ((MUIMasterBase = OpenLibrary("muimaster.library", 11)) == NULL)
	{
		CleanExit("Can't open muimaster.library v11\n");
	}
	
	return TRUE;
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
		while (DoMethod(obj->App, MUIM_Application_NewInput, (IPTR)&sigs)
			!= MUIV_Application_ReturnID_Quit)
		{
			//Printf("GUI running...");
			if (restart)	
				break;
			
		    if (sigs)
		      	{
		        sigs = Wait(sigs | SIGBREAKF_CTRL_C);
		        if (sigs & SIGBREAKF_CTRL_C) break;
		      	}
		}
		
		set(obj->WI_Preferences, MUIA_Window_Open, FALSE);
		
		//Printf("GUI stopped.");
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
