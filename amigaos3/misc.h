/* AmigaOS 3 misc.h file */

#include <inttypes.h>
#include <dos/dos.h>

#include "utils/nsurl.h"
#include "desktop/browser.h"
#include <proto/asl.h>

int TimeOut;

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
char * RemoveSpaces(char * source, char * target);
char * itoa(unsigned int n);
int check_version(void);
void reverse_string(char *str);
const char *add_theme_path(const char* icon);
void PrintG(char *str);
int detect_screen(void);
int64_t GetFileSize(BPTR fh);
struct browser_window *bw_window;
struct gui_window *g2;
nsurl *url_global;
int OpenPrefs(void);
bool restart, save;
void SSLv2_client_method(void);
int scale_cp;
