/* Toolbar.c */


const char *
add_theme_path(const char *icon) 
{
	static char path[128];

	strcpy(path, "NetSurf:Resources/theme/");
	strcat(path, nsoption_charp(theme));
	strcat(path, icon);

	return path;
}
	
void rerun_netsurf(void)
{
	char run[256];
	strcpy(run, "run NetSurf:");
#ifdef RTG
#	ifndef NOTTF
	strcat(run, "netsurf");
#	else
	strcat(run, "netsurf-nottf");
#	endif	
#else
#	ifndef NOTTF
#		ifdef AGA685
		strcat(run, "netsurfaga");
#		else
		strcat(run, "netsurfagalow");
#		endif		
#	else
#		ifdef AGA685
		strcat(run, "netsurfaga-nottf");
#		else
		strcat(run, "netsurfagalow-nottf");
#		endif	
#	endif		
#endif	

	Execute(run, 0, 0);
	
	fb_complete = true;
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
		
	fbtk_set_bitmap(widget, load_bitmap(icon_file));

	if (cbi->event->value.keycode == NSFB_KEY_MOUSE_3) {
		nsoption_read(options, nsoptions);
		SDL_Delay(500);
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
	
	fbtk_set_bitmap(widget, load_bitmap(icon_file));
	
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
		
	fbtk_set_bitmap(widget, load_bitmap(icon_file));
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
		
	fbtk_set_bitmap(widget, load_bitmap(icon_file));
		
	if (ReadClip())
		{

		 char str[100];
		 int len;
		 len = (int) strlen(ReadClip());
		 
		 strcpy(str,strdup(ReadClip()));
		 
		 len--;
		 while (isspace(*(str + len) )) {
			  len--;
		 }
		 
		 *(str + len + 1) = '\0';
		 
		fb_url_enter(bw, str);
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
		
	fbtk_set_bitmap(widget, load_bitmap(icon_file));
			
	char *clip = strdup(nsurl_access(hlcache_handle_get_url(bw->current_content)));
	
	WriteClip(clip);
		
	return 1;
}

char* usunhttp(char* s)
{
     int p = 0, q = 0, i = 7;
     while (s[++p] != '\0') if (p >= i) s[q++] = s[p];
     s[q] = '\0';

    return s;
}

void 
get_video(struct browser_window *bw) 
{   	
	char *cmd = AllocVec(1000, MEMF_ANY | MEMF_CLEAR);
	char *url = strdup(nsurl_access(hlcache_handle_get_url(bw->current_content)));
	
/*	char *addr = AllocVec(900, MEMF_ANY | MEMF_CLEAR);
	addr = strdup("http://www.getlinkinfo.com/info?link=");
	strcat(addr, url);
	fb_url_enter(bw, addr);
	FreeVec(addr);*/
		
	strcpy(cmd, "run Sys:Rexxc/rx S:getvideo.rexx "); 
	strcat(cmd, url);
	strcat(cmd, " play ");
	
	Execute("Mount >NIL: TCP: from AmiTCP:devs/Inet-Mountlist",0,0);
	Execute(cmd, 0, 0);
	//SetTaskPri(FindTask(0), -1);
	
	FreeVec(cmd);
}

/* get video icon click routine */
int
fb_getvideo_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;

	if (cbi->event->type == NSFB_EVENT_KEY_UP)
		return 0;
		
    get_video(bw);
    
	fbtk_set_bitmap(widget, load_bitmap(icon_file));
	
	return 1;
}


/* set current url as homepage icon click routine */
int
fb_sethome_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
 	struct browser_window *bw = cbi->context;
	
	if (cbi->event->type == NSFB_EVENT_KEY_UP) 
		return 0;
		
	fbtk_set_bitmap(widget, load_bitmap(icon_file));
	
	nsoption_charp(homepage_url) = strdup(nsurl_access(hlcache_handle_get_url(bw->current_content)));
	nsoption_write(options, NULL, NULL);
	free(nsoption_charp(homepage_url));
	
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
		
	fbtk_set_bitmap(widget, load_bitmap(icon_file));
				
	BPTR fh;
	char *cmd;

	int inum = 0;
	
	/* Show select window */
	
	cmd = strdup("RequestChoice > ENV:NSfav TITLE=\"Select favourite\" BODY=\"Choose which favourite slot you would like to use\" GADGETS=\"___1___|___2___|___3___|___4___|___5___|___6___|___7___|___8___|___9___|___10___|___11___|___12___|Cancel\"");

	Execute(cmd, 0, 0);
	fh = Open("ENV:NSfav",MODE_OLDFILE);

	char snum[3];
	
	Read(fh,snum,2);
	inum = atoi(snum);

	Close(fh);
	
	Execute("delete ENV:NSfav", 0, 0);
	
	/* Download favicon */		
	
	char *wget = AllocVec(1000, MEMF_ANY | MEMF_CLEAR);
	char *opt = strdup("?&format=png&width=16&height=16&canAudit=false -OResources/Icons/favicon");
	//char *opt = strdup("?&format=png&width=16&height=16 -OResources/Icons/favicon");

	strcpy(wget, "echo \" C:wget ");
	strcat(wget, "-PNetSurf:Resources/Icons/ ");
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
		strcat(wget, " \" >script");
			
		Execute(wget, 0, 0);
		Execute("echo \"endcli\" >>script", 0, 0);
		Execute("execute script", 0, 0);
		Execute("delete script", 0, 0);
	
		nsoption_write(options, NULL, NULL);
				
		if (fav != NULL)
			{	
			bitmap = strdup("NetSurf:Resources/Icons/favicon");
			strcat(bitmap, snum);
			strcat(bitmap, ".png");
			fbtk_set_bitmap(fav, load_bitmap(bitmap));
			fbtk_set_text(label, stitle);
			}
	}

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

	return 1;
}

/* add bookmark icon click routine */
int
fb_add_bookmark_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->type == NSFB_EVENT_KEY_UP) 
		return 0;	
			
	fbtk_set_bitmap(widget, load_bitmap(icon_file));
	
	get_url = strdup(nsurl_access(hlcache_handle_get_url(bw->current_content)));
	char *cmd = AllocVec(strlen("echo <li><a href=\"") + strlen(get_url) + strlen(stitle) + strlen("</a></li >> Resources/Bookmarks.htm") + 10, MEMF_ANY | MEMF_CLEAR);

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
      	char *cmd;
      	char buffer[1];
		int buff = 0;
		
      	cmd = strdup("RequestChoice > ENV:NSsrcheng TITLE=\"Select search engine\" BODY=\"\" GADGETS=\"Google|Yahoo|Bing|DuckDuckGo|YouTube|Ebay|Allegro|Aminet|Wikipedia|Cancel\"");

      	Execute(cmd, 0, 0);
      	fh = Open("ENV:NSsrcheng",MODE_OLDFILE);

      	Read(fh,buffer,1);
		buff = atoi(buffer);
		
      	Close(fh);

      	Execute("delete ENV:NSsrcheng", 0, 0);

      	  if (buff == 1) {
      		   nsoption_charp(def_search_bar) = strdup("http://www.google.com/search?q=");
      		   fbtk_set_bitmap(widget, load_bitmap("NetSurf:Resources/Icons/google.png"));
				Execute("copy Resources/Icons/google.png Resources/Icons/search.png", 0, 0);
				}
      	  else if (buff == 2) {
      		   nsoption_charp(def_search_bar) = strdup("http://search.yahoo.com/search?p=");
      		   fbtk_set_bitmap(widget, load_bitmap("NetSurf:Resources/Icons/yahoo.png"));		   
				Execute("copy Resources/Icons/yahoo.png Resources/Icons/search.png", 0, 0);
			   }
      	  else if (buff == 3) {
      		   nsoption_charp(def_search_bar) = strdup("http://www.bing.com/search?q=");
      		   fbtk_set_bitmap(widget, load_bitmap("NetSurf:Resources/Icons/bing.png"));		   
				Execute("copy Resources/Icons/bing.png Resources/Icons/search.png", 0, 0);
			   }
      	  else if (buff == 4) {
      		   nsoption_charp(def_search_bar) = strdup("http://www.duckduckgo.com/html/?q=");
      		   fbtk_set_bitmap(widget, load_bitmap("NetSurf:Resources/Icons/duckduckgo.png"));		   
				Execute("copy Resources/Icons/duckduckgo.png Resources/Icons/search.png", 0, 0);
			   }			
      	  else if (buff == 5) {
      		   nsoption_charp(def_search_bar) = strdup("http://www.youtube.com/results?search_query=");
      		   fbtk_set_bitmap(widget, load_bitmap("NetSurf:Resources/Icons/youtube.png"));
				Execute("copy Resources/Icons/youtube.png Resources/Icons/search.png", 0, 0);
			   }
      	  else if (buff == 6) {
      		   nsoption_charp(def_search_bar) = strdup("http://shop.ebay.com/items/");
      		   fbtk_set_bitmap(widget, load_bitmap("NetSurf:Resources/Icons/ebay.png"));		   
				Execute("copy Resources/Icons/ebay.png Resources/Icons/search.png", 0, 0);
			   }			
      	  else if (buff == 7) {
      		   nsoption_charp(def_search_bar) = strdup("http://allegro.pl/listing.php/search?string=");
      		   fbtk_set_bitmap(widget, load_bitmap("NetSurf:Resources/Icons/allegro.png"));
				Execute("copy Resources/Icons/allegro.png Resources/Icons/search.png", 0, 0);
			   }			
      	  else if (buff == 8) {
      		   nsoption_charp(def_search_bar) = strdup("http://aminet.net/search?query=");
      		   fbtk_set_bitmap(widget,  load_bitmap("NetSurf:Resources/Icons/aminet.png"));
				Execute("copy Resources/Icons/aminet.png Resources/Icons/search.png", 0, 0);
			   }
      	  else if (buff == 9) {
      		   nsoption_charp(def_search_bar) = strdup("http://en.wikipedia.org/w/index.php?title=Special:Search&search=");
      		   fbtk_set_bitmap(widget, load_bitmap("NetSurf:Resources/Icons/wiki.png"));
				Execute("copy Resources/Icons/wiki.png Resources/Icons/search.png", 0, 0);
			   }

      	nsoption_write(options, NULL, NULL);
		free(cmd);
		
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
		char addr[500];

		strcpy(addr, nsoption_charp(def_search_bar));
		strcat(addr, text);
		fb_url_enter(bw, addr);
		
		}
	return 0;
}

int
fb_bookmarks_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->type == NSFB_EVENT_KEY_UP) 
		return 0;	

	fbtk_set_bitmap(widget, load_bitmap(icon_file));
	
	char *cmd = strdup("file:///NetSurf:Resources/Bookmarks.htm");
	
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
		
	fbtk_set_bitmap(widget, load_bitmap(icon_file));
	
	char *text_editor = AllocVec(200, MEMF_ANY | MEMF_CLEAR);

	if (nsoption_int(fullscreen) == 1) {
		//SetTaskPri(FindTask(0), -1);
		ScreenToFront(Workbench);
		strcpy(text_editor, "run ");
		strcat(text_editor, nsoption_charp(text_editor));	
	} else
		strcpy(text_editor, nsoption_charp(text_editor));
	
	strcat(text_editor, " NetSurf:Resources/Options"); 

	Execute(text_editor,0,0);
#ifndef RTG	
	Execute("RequestChoice TITLE=\"Options\" BODY=\"Click ok after changing options \" GADGETS=\"OK\"",0,0);
#endif	
	FreeVec(text_editor);
	
	nsoption_read(options, nsoptions);

	read_labels();
	
	return 1;
} 
 
int
fb_getpage_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->type == NSFB_EVENT_KEY_UP) 
		return 0;

	fbtk_set_bitmap(widget, load_bitmap(icon_file));  

	BPTR fh;
	char result[2];
	int resint = 0;

	char *cmd = strdup("RequestChoice > env:nswget TITLE=\"Download\" BODY=\"Download \" GADGETS=\"using wget|as PDF|Cancel\"");
			   
	Execute(cmd, 0, 0);
	fh = Open("ENV:NSwget",MODE_OLDFILE);    
	Read(fh, result, 1);
	resint = atoi(result);

	Close(fh); 
	Execute("delete ENV:NSwget", 0, 0);	
	free(cmd);
	
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
		 
		char *wget = AllocVec(1000, MEMF_ANY | MEMF_CLEAR );
		
		char *url = strdup(nsurl_access(hlcache_handle_get_url(bw->current_content)));
		
		if ( resint == 2 ) {
			usunhttp(url);
			char *wsk;
			wsk=strchr(url,'/');
			*wsk='-';
			strlcpy(url,url,strlen(url));
		}
		strcpy(wget, "run C:wget ");
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
		else {
			char* wget2 = strndup(wget, strlen(wget)-1);
			strcpy(wget,wget2);
			free(wget2);}
			
		url = strdup(strrchr(url, '/'));
		strcat(wget, url);

		if ( resint == 2 ) 			
			strcat(wget, ".pdf");

		fh = Open("CON:", MODE_NEWFILE);

		Execute(wget, fh, 0);
		
		Close(fh);
		
		FreeVec(wget);
		free(url);
		}  
		
	return 1;
}

int
fb_openfile_click(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	
	if (cbi->event->type == NSFB_EVENT_KEY_UP)
		return 0;
		
	fbtk_set_bitmap(widget, load_bitmap(icon_file));
	
	char *file = AllocVec(1024,MEMF_ANY | MEMF_CLEAR);

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
		strcat(file, "NetSurf:");
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
		Execute("delete NetSurf:Resources/Icons/favicon1.png", 0, 0);
		return 0;
	}	

	if ((nsoption_charp(favourite_1_url) != NULL) && (nsoption_charp(favourite_1_url)[0] != '\0'))
		fb_url_enter(bw, nsoption_charp(favourite_1_url));
	
	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap("NetSurf:Resources/Icons/favicon1.png"));
		
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
		fbtk_set_bitmap(widget,  load_bitmap("NetSurf:Resources/Icons/favicon2.png"));
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
		Execute("delete NetSurf:Resources/Icons/favicon3.png", 0, 0);
		return 0;
	}	
	
	if (nsoption_charp(favourite_3_url) != NULL && nsoption_charp(favourite_3_url)[0]!= '\0')		
		fb_url_enter(bw, nsoption_charp(favourite_3_url));

	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap("NetSurf:Resources/Icons/favicon3.png"));
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
		Execute("delete NetSurf:Resources/Icons/favicon4.png", 0, 0);
		return 0;
	}	
	
	if (nsoption_charp(favourite_4_url) != NULL && nsoption_charp(favourite_4_url)[0]!= '\0')		
		fb_url_enter(bw, nsoption_charp(favourite_4_url));

	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap("NetSurf:Resources/Icons/favicon4.png"));
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
		Execute("delete NetSurf:Resources/Icons/favicon5.png", 0, 0);
		return 0;
	}	
	
	if (nsoption_charp(favourite_5_url) != NULL && nsoption_charp(favourite_5_url)[0]!= '\0')		
		fb_url_enter(bw, nsoption_charp(favourite_5_url));

	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap("NetSurf:Resources/Icons/favicon5.png"));
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
		Execute("delete NetSurf:Resources/Icons/favicon6.png", 0, 0);
		return 0;
	}	
		
	if (nsoption_charp(favourite_6_url) != NULL && nsoption_charp(favourite_6_url)[0]!= '\0')		
		fb_url_enter(bw, nsoption_charp(favourite_6_url));

	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap("NetSurf:Resources/Icons/favicon6.png"));
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
		Execute("delete NetSurf:Resources/Icons/favicon7.png", 0, 0);
		return 0;
	}	
	
	if (nsoption_charp(favourite_7_url) != NULL && nsoption_charp(favourite_7_url)[0]!= '\0')		
		fb_url_enter(bw, nsoption_charp(favourite_7_url));

	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap("NetSurf:Resources/Icons/favicon7.png"));
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
		Execute("delete NetSurf:Resources/Icons/favicon8.png", 0, 0);
		return 0;
	}	
	
	if (nsoption_charp(favourite_8_url) != NULL && nsoption_charp(favourite_8_url)[0]!= '\0')		
		fb_url_enter(bw, nsoption_charp(favourite_8_url));

	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap("NetSurf:Resources/Icons/favicon8.png"));
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
		Execute("delete NetSurf:Resources/Icons/favicon9.png", 0, 0);
		return 0;
	}	
	
	if (nsoption_charp(favourite_9_url) != NULL && nsoption_charp(favourite_9_url)[0]!= '\0')
		fb_url_enter(bw, nsoption_charp(favourite_9_url));

	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap("NetSurf:Resources/Icons/favicon9.png"));
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
		Execute("delete NetSurf:Resources/Icons/favicon10.png", 0, 0);
		return 0;
	}	
	
	if (nsoption_charp(favourite_10_url) != NULL && nsoption_charp(favourite_10_url)[0]!= '\0')
		fb_url_enter(bw, nsoption_charp(favourite_10_url));

	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap("NetSurf:Resources/Icons/favicon10.png"));
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
		Execute("delete NetSurf:Resources/Icons/favicon11.png", 0, 0);
		return 0;
	}	
	
	if (nsoption_charp(favourite_11_url) != NULL && nsoption_charp(favourite_11_url)[0]!= '\0')		
		fb_url_enter(bw, nsoption_charp(favourite_11_url));

	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap("NetSurf:Resources/Icons/favicon11.png"));
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
		Execute("delete NetSurf:Resources/Icons/favicon12.png", 0, 0);
		return 0;
	}	
	
	if (nsoption_charp(favourite_12_url) != NULL && nsoption_charp(favourite_12_url)[0]!= '\0')		
		fb_url_enter(bw, nsoption_charp(favourite_12_url));

	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap("NetSurf:Resources/Icons/favicon12.png"));
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
	
	if (icon_file)
	   fbtk_set_bitmap(button,  load_bitmap(icon_file));
	
	return 0;
}

static int
fb_localhistory_btn_clik(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct gui_window *gw = cbi->context;

	fbtk_set_bitmap(widget,  load_bitmap(add_theme_path("/history_h.png")));
	
	if (cbi->event->type == NSFB_EVENT_KEY_UP) {
		fbtk_set_bitmap(widget,  load_bitmap(add_theme_path("/history.png")));	
		
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
		button = widget;
		icon_file = strdup(add_theme_path("/back.png"));
		fbtk_set_bitmap(widget, load_bitmap(add_theme_path("/back_h.png")));
	}
	gui_window_set_status(g2, "Go to previous page");

	return 0;
}

static int
set_forward_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	struct browser_window *bw = cbi->context;
	if (browser_window_forward_available(bw)) {
		fbtk_set_bitmap(widget, load_bitmap(add_theme_path("/forward_h.png")));
		button = widget;
		icon_file = strdup(add_theme_path("/forward.png"));
	}	
	gui_window_set_status(g2, "Go to next page");

	return 0;
}
#ifndef RTG
static int
set_close_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	gui_window_set_status(g2, "Close or restart(RMB) program");

	return 0;
}
#endif
static int
set_stop_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	button = widget;
	icon_file = strdup(add_theme_path("/stop.png"));
	
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("/stop_h.png")));
	
	gui_window_set_status(g2, "Stop loading of current page");

	return 0;
}


static int
set_local_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	button = widget;
	icon_file = strdup(add_theme_path("/history.png"));
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("/history_h.png")));
	
	gui_window_set_status(g2, "Show local history treeview");

	return 0;
}
#ifdef RTG	
static int
set_reload_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	button = widget;
	icon_file = strdup(add_theme_path("/reload.png"));
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("/reload_h.png")));
	
	gui_window_set_status(g2, "Reload");

	return 0;
}

static int
set_home_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	button = widget;
	icon_file = strdup(add_theme_path("/home.png"));
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("/home_h.png")));
	
	gui_window_set_status(g2, "Go to homepage");

	return 0;
}
#endif

static int
set_add_fav_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	button = widget;
	icon_file = strdup(add_theme_path("/add_fav.png"));
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("/add_fav_h.png")));
	
	gui_window_set_status(g2, "Add current page to favourites");

	return 0;
}

static int
set_add_bookmark_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	button = widget;
	icon_file = strdup(add_theme_path("/add_bookmark.png"));
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("/add_bookmark_h.png")));
	
	gui_window_set_status(g2, "Add current page to bookmarks");

	return 0;
}

static int
set_bookmarks_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	button = widget;
	icon_file = strdup(add_theme_path("/bookmarks.png"));
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("/bookmarks_h.png")));
	
	gui_window_set_status(g2, "Go to bookmarks");

	return 0;
}

static int
set_search_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	gui_window_set_status(g2, "Select search engine");

	return 0;
}

static int
set_prefs_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	button = widget;
	icon_file = strdup(add_theme_path("/prefs.png"));
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("/prefs_h.png")));
	
	gui_window_set_status(g2, "Open preferences file in editor");

	return 0;
}

static int
set_getpage_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	button = widget;
	icon_file = strdup(add_theme_path("/getpage.png"));
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("/getpage_h.png")));
	
	gui_window_set_status(g2, "Save file using wget or as pdf");

	return 0;
}

static int
set_sethome_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	button = widget;
	icon_file = strdup(add_theme_path("/sethome.png"));
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("/sethome_h.png")));
	
	gui_window_set_status(g2, "Set current site as homepage");

	return 0;
}

static int
set_getvideo_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	button = widget;
	icon_file = strdup(add_theme_path("/getvideo.png"));
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("/getvideo_h.png")));
	
	gui_window_set_status(g2, "Play or save multimedia to disk using GetVideo plugin");

	return 0;
}

static int
set_paste_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	button = widget;
	icon_file = strdup(add_theme_path("/paste.png"));
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("/paste_h.png")));
	
	gui_window_set_status(g2, "Paste and open URL from clipboard");

	return 0;
}

static int
set_copy_status(fbtk_widget_t *widget, fbtk_callback_info *cbi)
{
	button = widget;
	icon_file = strdup(add_theme_path("/copy.png"));
	fbtk_set_bitmap(widget, load_bitmap(add_theme_path("/copy_h.png")));
	
	gui_window_set_status(g2, "Copy current URL to clipboard");

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

fbtk_widget_t *searchbar, *home, *addfav, *addbook, *quick, 
			  *prefs, *download, *sethome, *getvideo, *copy, *paste;

const char *label5txt,*label6txt,*label7txt,*label8txt,*label9txt,
			  *label10txt,*label11txt,*label12txt;

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
			image = strdup("NetSurf:Resources/Icons/favicon5.png");
			status = set_fav5_status;
			label_txt = strndup(nsoption_charp(favourite_5_label), 10);
			break;
		case 6:
			click = fb_fav6_click;
			image = strdup("NetSurf:Resources/Icons/favicon6.png");
			status = set_fav6_status;
			label_txt = strndup(nsoption_charp(favourite_6_label), 10);
			break;
		case 7:
			click = fb_fav7_click;
			image = strdup("NetSurf:Resources/Icons/favicon7.png");
			status = set_fav7_status;
			label_txt = strndup(nsoption_charp(favourite_7_label), 10);
			break;
		case 8:
			click = fb_fav8_click;
			image = strdup("NetSurf:Resources/Icons/favicon8.png");
			status = set_fav8_status;
			label_txt = strndup(nsoption_charp(favourite_8_label), 10);
			break;
		case 9:
			click = fb_fav9_click;
			image = strdup("NetSurf:Resources/Icons/favicon9.png");
			status = set_fav9_status;
			label_txt = strndup(nsoption_charp(favourite_9_label), 10);
			break;
		case 10:
			click = fb_fav10_click;
			image = strdup("NetSurf:Resources/Icons/favicon10.png");
			status = set_fav10_status;
			label_txt = strndup(nsoption_charp(favourite_10_label), 10);
			break;
		case 11:
			click = fb_fav11_click;
			image = strdup("NetSurf:Resources/Icons/favicon11.png");
			status = set_fav11_status;
			label_txt = strndup(nsoption_charp(favourite_11_label), 10);
			break;
		case 12:
			click = fb_fav12_click;
			image = strdup("NetSurf:Resources/Icons/favicon12.png");
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
		
	LOG(("Using toolbar layout %s", toolbar_layout));

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

		LOG(("toolbar adding %c", *itmtype));

		switch (*itmtype) {

		case 'b': /* back */
			widget = fbtk_create_button(toolbar, 
						    (xdir == 1) ? xpos : 
						     xpos - left_arrow.width, 
						    padding, 
						    BUTTON_SIZE, 
						    BUTTON_SIZE, 
						    frame_col, 
						    load_bitmap(add_theme_path("/back_g.png")), 
						    fb_leftarrow_click, 
						    gw);
			gw->back = widget; /* keep reference */
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_back_status, gw->bw);	
			break;

		case 'l': /* local history */
			widget = fbtk_create_button(toolbar,
						    (xdir == 1) ? xpos : 
						     xpos - left_arrow.width,
						    padding,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    frame_col,
						    load_bitmap(add_theme_path("/history.png")), 
						    fb_localhistory_btn_clik,
						    gw);
			gw->history = widget;	
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_local_status, gw->bw);			
			break;

		case 'f': /* forward */
			widget = fbtk_create_button(toolbar,
						    (xdir == 1)?xpos : 
						    xpos - right_arrow.width,
						    padding,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    frame_col,
						    load_bitmap(add_theme_path("/forward_g.png")),
						    fb_rightarrow_click,
						    gw);
			gw->forward = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_forward_status, gw->bw);	
			break;

		case 'c': /* close the current window */
#ifndef RTG	
			widget = fbtk_create_button(toolbar,
							80,
						    padding,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    FB_FRAME_COLOUR,
						    load_bitmap(add_theme_path("/stop.png")),
						    fb_close_click,
						    gw->bw);
			gw->close = widget;
			fbtk_set_handler(widget, 
				 FBTK_CBT_POINTERENTER, set_close_status, gw->bw);
#endif			 
			break;

		case 's': /* stop  */
			widget = fbtk_create_button(toolbar,
						    (xdir == 1)?xpos : 
						    xpos - stop_image.width,
						    padding,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    frame_col,
						    load_bitmap(add_theme_path("/stop.png")),
						    fb_stop_click,
						    gw->bw);
			gw->stop = widget;
			fbtk_set_handler(widget, 
				 FBTK_CBT_POINTERENTER, set_stop_status, gw->bw); 
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
						    load_bitmap(add_theme_path("/reload_g.png")),
						    fb_reload_click,
						    gw->bw); 
			gw->reload = widget;
			fbtk_set_handler(widget, 
				 FBTK_CBT_POINTERENTER, set_reload_status, gw->bw); 
#endif	
			break;

		case 'h': /* home */
#ifdef RTG
			widget = fbtk_create_button(toolbar,
						    (xdir == 1)?xpos : 
						    xpos - stop_image.width,
						    padding,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    frame_col,
						    load_bitmap(add_theme_path("/home.png")),
						    fb_home_click,
						    gw->bw);
			home = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, set_home_status, gw->bw);
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
						    load_bitmap(add_theme_path("/add_fav.png")),
						    fb_add_fav_click,
						    gw->bw);
			addfav = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
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
						    load_bitmap(add_theme_path("/add_bookmark.png")),
						    fb_add_bookmark_click,
						    gw->bw);
			addbook = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
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
										load_bitmap("NetSurf:Resources/Icons/spacer.png"));
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
						    load_bitmap("NetSurf:Resources/Icons/search.png"),
						    fb_search_click,
						    gw->bw);
			quick = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
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
						    //load_bitmap(add_theme_path("/throbber/throbber00.png")));
			gw->throbber = widget;
			
			if (nsoption_int(fullscreen) == 1) {
					fbtk_set_handler(widget, 
							 FBTK_CBT_CLICK, 
							 throbber_click, gw->bw);	

					fbtk_set_handler(widget, 
							 FBTK_CBT_POINTERENTER, 
							 set_throbber_status, gw->bw);
			}				
			break;

		case 'k': /* open bookmarks button */
			xpos = 2;
			widget = fbtk_create_button(toolbar,
						    xpos,
						    padding + 27,
						    24,
						    24,
						    frame_col,
						    load_bitmap(add_theme_path("/bookmarks.png")),
						    fb_bookmarks_click,
						    gw->bw);
							
			fbtk_set_handler(widget, 
					 FBTK_CBT_POINTERENTER, 
					 set_bookmarks_status, gw->bw);	

//#ifdef RTG		
			spacer2 = fbtk_create_bitmap(toolbar,
										xpos+27,
										padding + 29,
										2,
										20,
										frame_col, 
										load_bitmap("NetSurf:Resources/Icons/spacer.png"));
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
						    load_bitmap("NetSurf:Resources/Icons/favicon1.png"),
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
						    load_bitmap("NetSurf:Resources/Icons/favicon2.png"),
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
						    load_bitmap("NetSurf:Resources/Icons/favicon3.png"),
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
						    load_bitmap("NetSurf:Resources/Icons/favicon4.png"),
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
										load_bitmap("NetSurf:Resources/Icons/spacer.png"));
				
			widget = fbtk_create_button(toolbar,
						    xpos,
						    padding + 27,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    frame_col,
						    load_bitmap(add_theme_path("/prefs.png")),
						    fb_prefs_click,
						    gw->bw);
			prefs = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_prefs_status, gw->bw);
			break;
			
		case 'd': /* download button */
			widget = fbtk_create_button(toolbar,
						    xpos,
						    padding + 27,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    frame_col,
						    load_bitmap(add_theme_path("/getpage.png")),
						    fb_getpage_click,
						    gw->bw);
			download = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_getpage_status, gw->bw);					
			break;

		case 'm': /* set home button */
			widget = fbtk_create_button(toolbar,
						    xpos,
						    padding + 27,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    frame_col,
						    load_bitmap(add_theme_path("/sethome.png")),
						    fb_sethome_click,
						    gw->bw);
			sethome = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_sethome_status, gw->bw);					
			break;	

		case 'y': /* getvideo button */
			widget = fbtk_create_button(toolbar,
						    xpos,
						    padding + 27,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    frame_col,
						    load_bitmap(add_theme_path("/getvideo.png")),
						    fb_getvideo_click,
						    gw->bw);
			getvideo = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_getvideo_status, gw->bw);					
			break;

		case 'o': /* copy text button */
			widget = fbtk_create_button(toolbar,
						    xpos,
						    padding + 27,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    frame_col,
						    load_bitmap(add_theme_path("/copy.png")),
						    fb_copy_click,
						    gw->bw);
			copy = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
				 set_copy_status, gw->bw);
			break;
			
		case 'p': /* paste text button */
			widget = fbtk_create_button(toolbar,
						    xpos,
						    padding + 27,
						    BUTTON_SIZE,
						    BUTTON_SIZE,
						    frame_col,
						    load_bitmap(add_theme_path("/paste.png")),
						    fb_paste_click,
						    gw->bw);
			paste = widget;
			fbtk_set_handler(widget, FBTK_CBT_POINTERENTER, 
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
			LOG(("Unknown element %c in toolbar layout", *itmtype));
		        break;

		}

		if (widget != NULL) {
			xpos += (xdir * (fbtk_get_width(widget) + padding));
		}

		LOG(("xpos is %d",xpos));

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
			
		case 'd': /* download button */
			widget = download;
			
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

