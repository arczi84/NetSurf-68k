/* AmigaOS 3 misc.h file */

int TimeOut;
char *substring(char *string, int position, int length);
char *insert_dot(char *a);
void append(char subject[], const char insert[], int pos);
int intToStr(int x, char str[], int d);
char *addpoint(int i,char res[]);
//char *addpoint(int i);
void reverse(char *str, int len);
const char *add_theme_path(const char* icon);
