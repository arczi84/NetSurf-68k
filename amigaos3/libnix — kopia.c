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

#define NSIG     9

//#include <clib/bsdsocket_protos.h>
//#include <clib/netlib_protos.h>
//int ioctl(int fd, unsigned int request, char *argp);

#include <proto/dos.h>
#include <ncurses/ncurses.h>
#include <libraries/bsdsocket.h>

#include "utils/log.h"
#include "utils/utils.h"
#include <amigaos3/misc.h>
#include "amigaos3/libnix.h"

LONG CloseSocket( LONG sock );

#if 0 
extern long __gmtoffset;

#include <devices/timer.h>

int __wrap_gettimeofday(struct timeval *tv, struct timezone *tzp)
{
  if (tv) {
    #if 0 // libnix < 3.x
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
#ifdef __libnix__


char *basename(char *path);

void closesocket(int s)
{
    CloseSocket(s);
}
/*
int SSL_CTX_set_srp_username(int *ctx, char *name){	return 0;}
int SSL_CTX_set_srp_password(int *ctx, char *password){	return 0;}
void SSL_CTX_set_next_proto_select_cb(int *ctx,
                                      int (*cb), void *arg)
{
	return 0;
}
*/
//void warnx(){};


//void __iob(){};

#include <unistd.h>
#include <sys/ttycom.h>
#include <sys/termios.h>

int tcgetattr(int fd, struct termios *t)
{
	return 0;// (ioctl(fd, TIOCGETA, t));
}

int tcsetattr(int fd, int opt, const struct termios *t)
{
	return 0;
}
/*
int tcsetattr(int fd, int opt, const struct termios *t)
{
	struct termios localterm;

	if (opt & TCSASOFT) {
		localterm = *t;
		localterm.c_cflag |= CIGNORE;
		t = &localterm;
		opt &= ~TCSASOFT;
	}
	if (opt == TCSANOW)
		return (ioctl(fd, TIOCSETA, t));
	else if (opt == TCSADRAIN)
		return (ioctl(fd, TIOCSETAW, t));
	return (ioctl(fd, TIOCSETAF, t));
}
*/

char *basename(char *path)
{
  return((char *)FilePart(path));
}
 /* 
double copysign (double x, double y)
{ return __copysign(x,y); }
*/

VOID NewRawDoFmt(CONST_STRPTR format, APTR func, STRPTR buf, ...);
#define RAWFMTFUNC_STRING 0

VOID NewRawDoFmt(CONST_STRPTR format, APTR func, STRPTR buf, ...)
{
	if (func == NULL)
	{
		va_list va;

		va_start(va, buf);
		vsprintf(buf, format, va);
		va_end(va);
	}
};

int
 __set_errno(int error)
     {
     	if (error == -1)
     		error = EINTR;
     	errno = error;
     	return (error);
     }

int
sigaction (sig, act, oact)
     int sig;
     const struct sigaction *act;
     struct sigaction *oact;
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
// math declarations
#if 0

#ifndef _STDLIB_NULL_POINTER_CHECK_H
#include "stdlib_null_pointer_check.h"
#endif /* _STDLIB_NULL_POINTER_CHECK_H */

/****************************************************************************/

#ifndef _MATH_HEADERS_H
#include "math_headers.h"
#endif /* _MATH_HEADERS_H */

/* Get a 32 bit int from a float.  */

#define GET_FLOAT_WORD(i,d)		\
do {							\
  ieee_float_shape_type gf_u;	\
  gf_u.value = (d);				\
  (i) = gf_u.word;				\
} while (0)

/* Set a float from a 32 bit int.  */

#define SET_FLOAT_WORD(d,i)		\
do {							\
  ieee_float_shape_type sf_u;	\
  sf_u.word = (i);				\
  (d) = sf_u.value;				\
} while (0)



double
atof(const char *str)
{
	double result = 0.0;

	assert( str != NULL );

	#if defined(CHECK_FOR_NULL_POINTERS)
	{
		if(str == NULL)
		{
			__set_errno(EFAULT);
			goto out;
		}
	}
	#endif /* CHECK_FOR_NULL_POINTERS */

	result = strtod(str,NULL);

 out:

	return(result);
}

INLINE STATIC const double
__frexp(double x,int * nptr)
{
	int int_exponent = 0;
	BOOL is_negative;

	if(x < 0)
	{
		is_negative = TRUE;

		x = (-x);
	}
	else
	{
		is_negative = FALSE;
	}

	if(x >= 1)
	{
		while(x >= 1)
		{
			int_exponent++;
			x /= 2;
		}
	}
	else if (0 < x && x < 0.5)
	{
		while(x < 0.5)
		{
			int_exponent--;
			x *= 2;
		}
	}

	(*nptr) = int_exponent;

	if(is_negative)
		x = (-x);

	return(x);
}


double
frexp(double x,int *nptr)
{
	double result;

	assert( nptr != NULL );

	#if defined(CHECK_FOR_NULL_POINTERS)
	{
		if(nptr == NULL)
		{
			__set_errno(EFAULT);

			result = __get_huge_val();
			goto out;
		}
	}
	#endif /* CHECK_FOR_NULL_POINTERS */

	if(x != 0.0)
	{
		result = __frexp(x,nptr);
	}
	else
	{
		result = 0.0;

		(*nptr) = 0;
	}

 out:

	return(result);
}

INLINE STATIC double
__modf(double x, double *iptr)
{
static const double one = 1.0;

	LONG i0,i1,j0;
	ULONG i;
	EXTRACT_WORDS(i0,i1,x);
	j0 = ((i0>>20)&0x7ff)-0x3ff;	/* exponent of x */
	if(j0<20) {			/* integer part in high x */
	    if(j0<0) {			/* |x|<1 */
	        INSERT_WORDS(*iptr,i0&0x80000000,0);	/* *iptr = +-0 */
		return x;
	    } else {
		i = (0x000fffff)>>j0;
		if(((i0&i)|i1)==0) {		/* x is integral */
		    ULONG high;
		    *iptr = x;
		    GET_HIGH_WORD(high,x);
		    INSERT_WORDS(x,high&0x80000000,0);	/* return +-0 */
		    return x;
		} else {
		    INSERT_WORDS(*iptr,i0&(~i),0);
		    return x - *iptr;
		}
	    }
	} else if (j0>51) {		/* no fraction part */
	    ULONG high;
	    *iptr = x*one;
	    GET_HIGH_WORD(high,x);
	    INSERT_WORDS(x,high&0x80000000,0);	/* return +-0 */
	    return x;
	} else {			/* fraction part in low x */
	    i = ((ULONG)(0xffffffff))>>(j0-20);
	    if((i1&i)==0) { 		/* x is integral */
	        ULONG high;
		*iptr = x;
		GET_HIGH_WORD(high,x);
		INSERT_WORDS(x,high&0x80000000,0);	/* return +-0 */
		return x;
	    } else {
	        INSERT_WORDS(*iptr,i0,i1&(~i));
		return x - *iptr;
	    }
	}
}

double
modf(double x,double *nptr)
{
	double result;

	assert( nptr != NULL );

	#if defined(CHECK_FOR_NULL_POINTERS)
	{
		if(nptr == NULL)
		{
			__set_errno(EFAULT);

			result = __get_huge_val();
			goto out;
		}
	}
	#endif /* CHECK_FOR_NULL_POINTERS */

	result = __modf(x,nptr);

 out:

	return(result);
}


static const float one = 1.0, Zero[] = {0.0, -0.0,};

float
fmodf(float x, float y)
{
	LONG n,hx,hy,hz,ix,iy,sx,i;

	GET_FLOAT_WORD(hx,x);
	GET_FLOAT_WORD(hy,y);
	sx = hx&0x80000000U;		/* sign of x */
	hx ^=sx;		/* |x| */
	hy &= 0x7fffffff;	/* |y| */

    /* purge off exception values */
	if(hy==0||(hx>=0x7f800000)||		/* y=0,or x not finite */
	   (hy>0x7f800000))			/* or y is NaN */
	    return (x*y)/(x*y);
	if(hx<hy) return x;			/* |x|<|y| return x */
	if(hx==hy)
	    return Zero[(ULONG)sx>>31];	/* |x|=|y| return x*0*/

    /* determine ix = ilogb(x) */
	if(hx<0x00800000) {	/* subnormal x */
	    for (ix = -126,i=(hx<<8); i>0; i<<=1) ix -=1;
	} else ix = (hx>>23)-127;

    /* determine iy = ilogb(y) */
	if(hy<0x00800000) {	/* subnormal y */
	    for (iy = -126,i=(hy<<8); i>=0; i<<=1) iy -=1;
	} else iy = (hy>>23)-127;

    /* set up {hx,lx}, {hy,ly} and align y to x */
	if(ix >= -126) 
	    hx = 0x00800000|(0x007fffff&hx);
	else {		/* subnormal x, shift x to normal */
	    n = -126-ix;
	    hx = hx<<n;
	}
	if(iy >= -126) 
	    hy = 0x00800000|(0x007fffff&hy);
	else {		/* subnormal y, shift y to normal */
	    n = -126-iy;
	    hy = hy<<n;
	}

    /* fix point fmod */
	n = ix - iy;
	while(n--) {
	    hz=hx-hy;
	    if(hz<0){hx = hx+hx;}
	    else {
	    	if(hz==0) 		/* return sign(x)*0 */
		    return Zero[(ULONG)sx>>31];
	    	hx = hz+hz;
	    }
	}
	hz=hx-hy;
	if(hz>=0) {hx=hz;}

    /* convert back to floating value and restore the sign */
	if(hx==0) 			/* return sign(x)*0 */
	    return Zero[(ULONG)sx>>31];	
	while(hx<0x00800000) {		/* normalize x */
	    hx = hx+hx;
	    iy -= 1;
	}
	if(iy>= -126) {		/* normalize output */
	    hx = ((hx-0x00800000)|((iy+127)<<23));
	    SET_FLOAT_WORD(x,hx|sx);
	} else {		/* subnormal output */
	    n = -126 - iy;
	    hx >>= n;
	    SET_FLOAT_WORD(x,hx|sx);
	    x *= one;		/* create necessary signal */
	}
	return x;		/* exact output */
}
#endif /*if 0*/

#endif
