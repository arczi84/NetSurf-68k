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
#include <sys/time.h>
#include <ncurses/ncurses.h>
#include <libraries/bsdsocket.h>
//#include <clib/exec_protos.h>

#include "utils/log.h"
#include "utils/utils.h"
#include <amigaos3/misc.h>
#include "amigaos3/libnix.h"

#include <unistd.h>
#include <sys/ttycom.h>
#include <sys/termios.h>

#define NSIG     9

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


void *old_ptr=0;

void free2(void *ptr)
{
	//printf("1 Pointer before:%p\n",ptr);
	if (!ptr)
		return;
	//if ((ptr!=NULL) && (ptr) && (ptr!=0)){
	//	
if (ptr != old_ptr)
			FreeVec(ptr);
	old_ptr = ptr;			
	//printf("2 Pointer after:%p\n",ptr);
		
	//}
	//
	//ptr=0;
	
	//FreeVec(ptr);
	//realloc(ptr,0);
	//printf("3 Pointer after 0:%p\n",ptr);
	return;
}

int uname(struct utsname *uts)
{
	struct Resident *res;
	ULONG ver, rev;

	ver = 3;
	rev = 1;

	strcpy(uts->sysname, "Workbench");
	strcpy(uts->nodename, "amiga");
	strcpy(uts->machine, "m68k");
}

#if 0
long __gmtoffset=0;

//#include <devices/timer.h>

int gettimeofday(struct timeval *tv, struct timezone *tzp)
{
  if (tv) {
    #if 1 // libnix < 3.x
    struct DateStamp t;
    DateStamp(&t); /* Get timestamp */
    tv->tv_sec=((t.ds_Days+2922)*1440+t.ds_Minute+__gmtoffset)*60+
               t.ds_Tick/TICKS_PER_SECOND;
    tv->tv_usec=(t.ds_Tick%TICKS_PER_SECOND)*1000000/TICKS_PER_SECOND;
    #else
    GetSysTime(tv);
    tv->tv_sec += (252460800 + (60*__gmtoffset));
    #endif
  }
  if (tzp) {
    tzp->tz_minuteswest = __gmtoffset;
    tzp->tz_dsttime = -1;
  }

  return 0;
}
#endif

/*
void closesocket(int s)
{
    CloseSocket(s);
}
*/


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

int sigaction (int sig, const struct sigaction *act, struct sigaction *oact)
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
