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
