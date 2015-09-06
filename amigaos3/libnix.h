/* Libnix prototypes file */

#define _FTOA_TOO_LARGE   -2   /* |input| > 2147483520 */ 
#define _FTOA_TOO_SMALL   -1   /* |input| < 0.0000001 */ 
#define BUFSIZE (sizeof(long) * 8 + 1)

const struct sigaction *act;
struct sigaction *oact;
int sigaction(int sig, const struct sigaction *act, struct sigaction *oact);
LONG CloseSocket( LONG sock );
char *basename(char *path);
char *strndup(const char *s, size_t n);
int fcntl(int fd,int  cmd);
int __set_errno(int error);
void closesocket(int s);
