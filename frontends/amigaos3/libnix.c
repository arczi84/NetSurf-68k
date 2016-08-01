/*
 * Copyright 2008 Vincent Sanders <vince@simtec.co.uk>
 *
 * This file is part of NetSurf, http://www.netsurf-browser.org/
 *
 * NetSurf is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * NetSurf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <errno.h>

#include <dos/dos.h>
#include <proto/dos.h>

#include <machine/param.h>
#include <ncurses/ncurses.h>
#include <libraries/bsdsocket.h>
#include <clib/exec_protos.h>
//#include <inline/socket.h>

#include "utils/log.h"
#include "utils/utils.h"
#include "utils/messages.h"
#include <amigaos3/misc.h>
#include "amigaos3/libnix.h"

#include <unistd.h>
//#include <sys/ttycom.h>
//#include <sys/termios.h>


#define NSIG     9

struct WBStartup *_WBenchMsg=NULL;


//int __argv;
//char* __argc ;

#include <stdlib.h>
#include <string.h>

size_t strlcat(char *dst, const char *src, size_t siz)
{
  char *d = dst;
  const char *s = src;
  size_t n = siz;
  size_t dlen;

  while(n-- != 0 && *d != '\0')
    d++;
  dlen = d - dst;
  n = siz - dlen;

  if(n == 0)
    return(dlen + strlen(s));
  while(*s != '\0') {
    if(n != 1) {
      *d++ = *s;
      n--;
    }
    s++;
  }
  *d = '\0';

  return(dlen + (s - src));
} 

int uname(struct utsname *uts)
{
	struct Resident *res;
	ULONG ver, rev;

	ver = 3;
	rev = 9;

	strcpy(uts->sysname, "Workbench");
	strcpy(uts->nodename, "amiga");
	strcpy(uts->machine, "m68k");
}

#if 0

static void content_update_status(struct content *c)
{
	if (c->status == CONTENT_STATUS_LOADING ||
			c->status == CONTENT_STATUS_READY) {
		/* Not done yet */
		snprintf(c->status_message, sizeof (c->status_message),
				"%s%s%s", messages_get("Fetching"),
				c->sub_status[0] != '\0' ? ", " : " ",
				c->sub_status);
	} else {
		unsigned int time = c->time;
		snprintf(c->status_message, sizeof (c->status_message),
				"%s (%s)", messages_get("Done"),
				time);
	}
}


char *substring(char *string, int position, int length) 
{
   char *pointer;
   int c;
 
   pointer = malloc(length+1);
 
   if( pointer == NULL )
       return 0;
 
   for( c = 0 ; c < length ; c++ ) 
      *(pointer+c) = *((string+position-1)+c);       
 
   *(pointer+c) = '\0';
 
   return pointer;
}

char *insert_dot(char *a)
{
   char *f, *e;
   int length;
   char *b = strdup(".");

   length = strlen(a);
   int position = length - 1;
 
   f = substring(a, 1, position - 1 );      
   e = substring(a, position, length-position+1);
 
   strcpy(a, "");
   strcat(a, f);
   free(f);
   strcat(a, b);
   strcat(a, e);
   
   free(e);
   free(b);
   strlcpy(a,a,length+1);
 
   return a;
   
}

char *addpoint(int i, char res[])
{
    char *string = malloc(6);
	res[6];
	
	if (i < 100)	
		i = i*10;
		
	intToStr(i, string, 0);	
	int length = strlen(string);
	
	//res = strdup(insert_dot(string));
	strcpy(res,insert_dot(string));

	free(string);
	
	//return res;	
	
	//printf("\n\"%s\"\n", string);
}; 


long __gmtoffset;

int __wrap_gettimeofday(struct timeval *tv, struct timezone *tzp)
{
  if (tv) {
    struct DateStamp t;
    DateStamp(&t); /* Get timestamp */
    tv->tv_sec=((t.ds_Days+2922)*1440+t.ds_Minute+__gmtoffset)*60+
               t.ds_Tick/TICKS_PER_SECOND;
    tv->tv_usec=(t.ds_Tick%TICKS_PER_SECOND)*1000000/TICKS_PER_SECOND;
  }
  if (tzp) {
    /* since AmigaOS doesn't support timezones, we always return
     * GMT...
     */
    tzp->tz_minuteswest = 0;
    tzp->tz_dsttime = 0;
  }

  return 0;
}


int tcgetattr(int fd, struct termios *t)
{
	return 0;
}

int tcsetattr(int fd, int opt, const struct termios *t)
{
	return 0;
}

char *basename(char *path)
{
  return((char *)FilePart(path));
}

int
 __set_errno(int error)
     {
     	if (error == -1)
     		error = EINTR;
     	errno = error;
     	return (error);
     }

unsigned int
	alarm (unsigned int seconds)
    {
      __set_errno (ENOSYS);
      return 0;
    } 
	 
int sigaction2 (int sig, const struct sigaction *act, struct sigaction *oact)
{
  if (sig <= 0 || sig >= NSIG)
    {
      __set_errno (EINVAL);
      return -1;
    }

  __set_errno (ENOSYS);
  return -1;
}

/* Perform file control operations on FD.  */ 
int
fcntl (int fd,int  cmd)
{

  if (fd < 0)
    {
      __set_errno (EBADF);
      return -1;
      
    }

  __set_errno (ENOSYS);
  return -1;
}

#endif

