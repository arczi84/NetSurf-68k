#include <stdlib.h>
#include <proto/exec.h>
#include <string.h>


/**
 * Duplicate up to n characters of a string.
 */

char *strndup(const char *s, size_t n)
{
	size_t len;
	char *s2;

	for (len = 0; len != n && s[len]; len++)
		continue;

	s2 = malloc(len + 1);
	if (!s2)
		return 0;

	memcpy(s2, s, len);
	s2[len] = 0;
	return s2;
}


# define bcopy bCopy
static inline
# include "bcopy.c"

void *__wrap_memmove2(void *s1,const void *s2,size_t n)
{
  bCopy(s2,s1,n);
  return s1;
}

void *__wrap_realloc(void *ptr,size_t size)
{
  void *a;
  size_t l;
  if(size)
    a=malloc(size);
  else
    a=NULL;
  if(ptr!=NULL)
  { if(a!=NULL)
    { l=((ULONG *)ptr)[-1]-sizeof(ULONG);
      l=l<size?l:size;
      //CopyMem(ptr,a,l);
      bCopy(ptr,a,l);
    }
    if(size==0||a!=NULL)
      free(ptr);
  }
  return a;
}

void *__wrap_memcpy(void *s1,const void *s2,size_t n)
{
  bCopy(s2,s1,n);
  return s1;
}

void *__wrap_memset(void *s,int c,size_t n)
{ size_t m;
  if(n)
  { unsigned long *p=(unsigned long *)s;
    c*=0x01010101;
    if(n>15)
    { if((long)p&1)
      { *((char *)p)++=c;
        n--; }
      if((long)p&2)
      { *((short *)p)++=c;
        n-=2; }
      for(m=n/(8*sizeof(long));m;--m)
      { *p++=c; *p++=c; *p++=c; *p++=c; 
        *p++=c; *p++=c; *p++=c; *p++=c; }
      n&=(8*sizeof(long)-1);
      for(m=n/sizeof(long);m;--m)
        *p++=c;
      if((n&=sizeof(long)-1)==0) return s;
    }
    //do;while(*((char *)p)++=c,--n);
    switch(n)
    {
      case 15:
	*((long *)p)++ = c;
	*((long *)p)++ = c;
	*((long *)p)++ = c;
	*((short *)p)++ = c;
	*((char *)p) = c;
	break;
      case 14:
	*((long *)p)++ = c;
	*((long *)p)++ = c;
	*((long *)p)++ = c;
	*((short *)p) = c;
	break;
      case 13:
	*((long *)p)++ = c;
	*((long *)p)++ = c;
	*((long *)p)++ = c;
	*((char *)p) = c;
	break;
      case 12:
	*((long *)p)++ = c;
	*((long *)p)++ = c;
	*((long *)p) = c;
	break;
      case 11:
	*((long *)p)++ = c;
	*((long *)p)++ = c;
	*((short *)p)++ = c;
	*((char *)p) = c;
	break;
      case 10:
	*((long *)p)++ = c;
	*((long *)p)++ = c;
	*((short *)p) = c;
	break;
      case 9:
	*((long *)p)++ = c;
	*((long *)p)++ = c;
	*((char *)p) = c;
	break;
      case 8:
	*((long *)p)++ = c;
	*((long *)p) = c;
	break;
      case 7:
	*((long *)p)++ = c;
	*((short *)p)++ = c;
	*((char *)p) = c;
	break;
      case 6:
	*((long *)p)++ = c;
	*((short *)p) = c;
	break;
      case 5:
	*((long *)p)++ = c;
	*((char *)p) = c;
	break;
      case 4:
	*((long *)p) = c;
	break;
      case 3:
	*((short *)p)++ = c;
	*((char *)p) = c;
	break;
      case 2:
	*((short *)p) = c;
	break;
      case 1:
	*((char *)p) = c;
	break;
    }
  }
  return s;
}

/*
FUNCTION
	<<memmove>>---move possibly overlapping memory

INDEX
	memmove

ANSI_SYNOPSIS
	#include <string.h>
	void *memmove(void *<[dst]>, const void *<[src]>, size_t <[length]>);

TRAD_SYNOPSIS
	#include <string.h>
	void *memmove(<[dst]>, <[src]>, <[length]>)
	void *<[dst]>;
	void *<[src]>;
	size_t <[length]>;

DESCRIPTION
	This function moves <[length]> characters from the block of
	memory starting at <<*<[src]>>> to the memory starting at
	<<*<[dst]>>>. <<memmove>> reproduces the characters correctly
	at <<*<[dst]>>> even if the two areas overlap.


RETURNS
	The function returns <[dst]> as passed.

PORTABILITY
<<memmove>> is ANSI C.

<<memmove>> requires no supporting OS subroutines.

QUICKREF
	memmove ansi pure
*/

#include <string.h>
//#include <_ansi.h>
#include <stddef.h>
#include <limits.h>

/* Nonzero if either X or Y is not aligned on a "long" boundary.  */
#define UNALIGNED(X, Y) \
  (((long)X & (sizeof (long) - 1)) | ((long)Y & (sizeof (long) - 1)))

/* How many bytes are copied each iteration of the 4X unrolled loop.  */
#define BIGBLOCKSIZE    (sizeof (long) << 2)

/* How many bytes are copied each iteration of the word copy loop.  */
#define LITTLEBLOCKSIZE (sizeof (long))

/* Threshhold for punting to the byte copier.  */
#define TOO_SMALL(LEN)  ((LEN) < BIGBLOCKSIZE)

/*SUPPRESS 20*/
void *
__wrap_memmove(
	void * dst_void,
	const void * src_void,
	size_t length)
{
  char *dst = dst_void;
  const char *src = src_void;
  long *aligned_dst;
  const long *aligned_src;

  if (src < dst && dst < src + length)
    {
      /* Destructive overlap...have to copy backwards */
      src += length;
      dst += length;
      while (length--)
	{
	  *--dst = *--src;
	}
    }
  else
    {
      /* Use optimizing algorithm for a non-destructive copy to closely 
         match memcpy. If the size is small or either SRC or DST is unaligned,
         then punt into the byte copy loop.  This should be rare.  */
      if (!TOO_SMALL(length) && !UNALIGNED (src, dst))
        {
          aligned_dst = (long*)dst;
          aligned_src = (long*)src;

          /* Copy 4X long words at a time if possible.  */
          while (length >= BIGBLOCKSIZE)
            {
              *aligned_dst++ = *aligned_src++;
              *aligned_dst++ = *aligned_src++;
              *aligned_dst++ = *aligned_src++;
              *aligned_dst++ = *aligned_src++;
              length -= BIGBLOCKSIZE;
            }

          /* Copy one long word at a time if possible.  */
          while (length >= LITTLEBLOCKSIZE)
            {
              *aligned_dst++ = *aligned_src++;
              length -= LITTLEBLOCKSIZE;
            }

          /* Pick up any residual with a byte copier.  */
          dst = (char*)aligned_dst;
          src = (char*)aligned_src;
        }

      while (length--)
        {
          *dst++ = *src++;
        }
    }

  return dst_void;

}
