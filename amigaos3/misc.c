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
#include <sys/time.h>

#include "utils/config.h"
#include "utils/log.h"
#include "utils/utils.h"
#include <amigaos3/misc.h>
#include "utils/nsoption.h"

#include <proto/timer.h>
ULONG __timerunit = UNIT_MICROHZ;

long __gmtoffset;

#if 1

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

void warnx(void);
void __iob(void){};
#if 1
void warnx(void){};

int scandir(const char *dir, struct dirent ***namelist,
  int (*filter)(const struct dirent *),
  int (*compar)(const struct dirent **, const struct dirent **))
{
	/*\todo stub function, needs writing, preferably into clib2 */
	return 0;
}
#endif
// reverses a string 'str' of length 'len'
void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}

 // Converts a given integer x to string str[].  d is the number
 // of digits required in output. If d is more than the number
 // of digits in x, then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
	
	if (x < 100)
		x=x*10;
		
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }
 
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
 
    reverse(str, i);
    str[i] = '\0';
    return i;
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

void warn_user(const char *warning, const char *detail)
{
	LOG("%s %s", warning, detail);
}
