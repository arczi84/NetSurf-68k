/* 
AmigaOS 3 related prototypes
*/

#include "image/ico.h"
#include "amigaos3/misc.h"

void OpenLibs(void);
void CloseLibs(void);
char* usunhttp(char* s);

extern struct gui_file_table *amiga_file_table;
char *exe_name;
//static colour frame_colour;
static int first_run = 1;
char **respaths; /** resource search path vector */
char *options;
struct browser_window *bw_url;
struct gui_window *g2;
fbtk_callback_info *Cbi;
static int mouse_2_click = 0;
char *get_url;
char* usunznaki(char* s, char *c);
char *status_txt, *stitle;
static char *icon_file = NULL;
char *ReadClip( void );
int WriteClip(const char * );
int alt, ctrl, shift, x_pos, selected, getvideo_click;
static bool mp4ee = false;
unsigned short szerokosc(char *text);
unsigned short zliczanie(char *text);

extern struct gui_download_table *amiga_download_table;
static int throbber_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);

struct fbtk_bitmap *load_bitmap(const char *filename);
struct fbtk_bitmap *favicon_bitmap;

void get_video(struct browser_window *bw);
void getvideo_mp4ee(struct browser_window *bw);  
void redraw_gui(void);
void rerun_netsurf(void);
void read_labels(void);

fbtk_widget_t *searchbar, *home, *addfav, *addbook, *quick, 
			  *prefs, *javascript, *sethome, *getvideo, *copy, *paste;
			  
char *searchbar_i, *home_i, *addfav_i, *addbook_i, *quick_i, 
			  *prefs_i, *javascript_i, *sethome_i, *getvideo_i, *copy_i, *paste_i;
			  
const char *label5txt,*label6txt,*label7txt,*label8txt,*label9txt,
			  *label10txt,*label11txt,*label12txt;

int fb_prefs_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_add_fav_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
//int fb_redraw_bitmap(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_leftarrow_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_rightarrow_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_reload_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_stop_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_home_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_copy_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_paste_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_getvideo_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_sethome_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_screenmode_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_add_bookmark_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_search_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_bookmarks_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_javascript_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_openfile_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_url_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_getpage_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_searchbar_enter(void *pw, char *text);
int fb_fav1_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_fav2_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_fav3_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_fav4_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_fav5_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_fav6_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_fav7_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_fav8_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_fav9_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_fav10_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_fav11_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);
int fb_fav12_click(fbtk_widget_t *widget, fbtk_callback_info *cbi);

fbtk_widget_t *fav1;
fbtk_widget_t *fav2;
fbtk_widget_t *fav3;
fbtk_widget_t *fav4;
fbtk_widget_t *fav5;
fbtk_widget_t *fav6;
fbtk_widget_t *fav7;
fbtk_widget_t *fav8;
fbtk_widget_t *fav9;
fbtk_widget_t *fav10;
fbtk_widget_t *fav11;
fbtk_widget_t *fav12;

fbtk_widget_t *label1;
fbtk_widget_t *label2;
fbtk_widget_t *label3;
fbtk_widget_t *label4;
fbtk_widget_t *label5;
fbtk_widget_t *label6;
fbtk_widget_t *label7;	
fbtk_widget_t *label8;
fbtk_widget_t *label9;
fbtk_widget_t *label10;
fbtk_widget_t *label11;
fbtk_widget_t *label12;

fbtk_widget_t *toolbar;
fbtk_widget_t *favicon;
fbtk_widget_t *button;
fbtk_widget_t *spacer1;
fbtk_widget_t *spacer2;
fbtk_widget_t *spacer3;
fbtk_widget_t *url;

extern struct fbtk_bitmap throbber00;
extern struct fbtk_bitmap throbber01;
extern struct fbtk_bitmap throbber02;
extern struct fbtk_bitmap throbber03;
extern struct fbtk_bitmap throbber04;
extern struct fbtk_bitmap throbber05;
extern struct fbtk_bitmap throbber06;
extern struct fbtk_bitmap throbber07;
extern struct fbtk_bitmap throbber08;
extern struct fbtk_bitmap throbber09;
extern struct fbtk_bitmap throbber10;
extern struct fbtk_bitmap pointer;
extern struct fbtk_bitmap caret;
extern struct fbtk_bitmap menu;
extern struct fbtk_bitmap progress;
extern struct fbtk_bitmap null_image;

#define hand hand_image

static void gui_window_redraw_window(struct gui_window *g); //byle bez static
static void gui_window_set_status(struct gui_window *g, const char *text);

fbtk_widget_t *create_fav_widget(int nr, int xpos, int text_w, fbtk_widget_t *toolbar, struct gui_window *gw);	

int text_w4,text_w5,text_w6,text_w7,text_w8,text_w9,text_w10,text_w11,text_w12;

struct gui_download_window *dw;

void gui_window_redraw(struct gui_window *g, int x0, int y0, int x1, int y1);
void gui_window_hide_pointer(struct gui_window *g);
void gui_download_window_error(struct gui_download_window *dw, const char *error_msg);
void gui_download_window_done(struct gui_download_window *dw);
void *download_icon(void *argument);

nserror gui_launch_url(struct nsurl *url);

nserror gui_download_window_data(struct gui_download_window *dw, const char *data, unsigned int size);


/* 
AmigaOS 4 related prototypes
*/
bool ami_locate_resource(char *fullpath, const char *file);
