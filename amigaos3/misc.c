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
#include <dos/dos.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <sys/time.h>
#include <ctype.h>

#include "utils/config.h"
#include "utils/log.h"
#include "utils/utils.h"
#include "amigaos3/misc.h"
#include "utils/nsoption.h"

#include <proto/timer.h>
/*
extern float ceilf(float x);

extern void __assertion_failure(const char *file_name,int line_number,const char * expression){};

float ceilf(float x)
{
	return ceil(x);
}
*/

void SSLv2_client_method()
{
	
};

static void
reverse(char *s)
{
	int i, j, c;

	for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

#ifdef JS
void __show_error(char *msg);

void __show_error(char *msg)
{
		Printf(msg);
}
#endif

void PrintG(char *str)
{
	if (strlen(str) > 0)
	{
		char *cmd = malloc(strlen(str) + (strlen("RequestChoice TITLE=\"NetSurf\" BODY=\" \" GADGETS=\"OK"))+20);
		
		strcpy(cmd, "RequestChoice TITLE=\"NetSurf\" BODY=\"");
		strcat(cmd, str);
		strcat(cmd,"\" GADGETS=\"OK\" ");
		
		Execute(cmd,0,0);
		
		free(cmd);
	}
}

/* DOS */
int64_t GetFileSize(BPTR fh)
{
	int32_t size = 0;
	struct FileInfoBlock *fib = AllocVec(sizeof(struct FileInfoBlock), MEMF_ANY);
	if(fib == NULL) return 0;

	ExamineFH(fh, fib);
	size = fib->fib_Size;

	FreeVec(fib);
	return (int64_t)size;
}

char * RemoveSpaces(char * source, char * target)
{
     while(*source++ && *target)
     {
        if (!isspace(*source)) 
             *target++ = *source;
     }
	 *target = 0;
	 
     return target;
}


#ifdef NO_FPU
typedef float DFtype __attribute__ ((mode (DF)));
typedef float SFtype __attribute__ ((mode (SF)));

typedef unsigned int USItype __attribute__ ((mode (SI)));
typedef int SItype __attribute__ ((mode (SI)));

DFtype __floatunsidf (USItype u);
SFtype __floatunsisf (USItype u);

DFtype
__floatunsidf (USItype u)
{
  SItype s = (SItype) u;
  DFtype r = (DFtype) s;
  if (s < 0)
    r += (DFtype)2.0 * (DFtype) ((USItype) 1
				 << (sizeof (USItype) * __CHAR_BIT__ - 1));
  return r;
}

SFtype
__floatunsisf (USItype u)
{
  SItype s = (SItype) u;
  if (s < 0)
    {
      /* As in expand_float, compute (u & 1) | (u >> 1) to ensure
	 correct rounding if a nonzero bit is shifted out.  */
      return (SFtype) 2.0 * (SFtype) (SItype) ((u & 1) | (u >> 1));
    }
  else
    return (SFtype) s;
}

#endif

ULONG __timerunit = UNIT_MICROHZ;

long __gmtoffset;

#if 0
#if 0
//#define ECLOCK

uint32_t __wrap_SDL_GetTicks(void)
{
	struct EClockVal time1;
	long efreq;
	long long eval;
	struct timeval tv;
	Uint32 ticks;
    if (!TimerBase)gettimerbase();
#ifndef ECLOCK
    GetSysTime(&tv);
	if(basetime.tv_micro > tv.tv_micro)
	{
		tv.tv_secs --;
          
		tv.tv_micro += 1000000;
	}
    ticks = ((tv.tv_secs - basetime.tv_secs) * 1000) + ((tv.tv_micro - basetime.tv_micro)/1000);
    
#else
    efreq = ReadEClock(&time1);
	eval = time1.ev_lo;
	eval +=(time1.ev_hi << 32);
	ticks = eval /(efreq/1000);
#endif
	
	return ticks;
}
#endif

int __wrap_gettimeofday(struct timeval *tv, struct timezone *tzp)
{
  if (tv) {
    #if 1// libnix < 3.x
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
    tzp->tz_minuteswest = 0;
    tzp->tz_dsttime = 0;
  }

  return 0;
}
#endif

//void warnx(void);
//void __iob(void){};

#if 1
//void warnx(void){};

int scandir(const char *dir, struct dirent ***namelist,
  int (*filter)(const struct dirent *),
  int (*compar)(const struct dirent **, const struct dirent **))
{
	/*\todo stub function, needs writing, preferably into clib2 */
	return 0;
}
#endif

void warn_user(const char *warning, const char *detail)
{
	LOG("%s %s", warning, detail);
}
