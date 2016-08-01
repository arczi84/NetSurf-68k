/* Filename: small.c
 *
 * Copyright 2001 by Intel Corp.
 *
 * This software has been developed by Intel Corporation.
 * Intel specifically disclaims all warranties, express or
 * implied, and all liability, including consequential and
 * other indirect damages, for the use of this program, including
 * liability for infringement of any proprietary rights,
 * and including the warranties of merchantability and fitness
 * for a particular purpose. Intel does not assume any
 * responsibility for and errors which may appear in this program
 * not any responsibility to update it.
 */

/*
 * Highly optimized memcpy for the Intel(R) XScale(TM) microarchitecture.
 * Source data is aligned to the largest boundary possible. Prefetch is
 * used if data becomes cache-line aligned. Consideration of data fetch
 * latencies are included in data prefetch calculations.
 */

#include <stddef.h>

/* Note: The following defines are approximations only because */
/* the BTB can change branch latencies depending on predictions. */

/* Core to bus cycle multiplier */
#define _MULTIPLIER 3

/* Convert from BUS clock to core clocks */
#define _CC(bus) (_MULTIPLIER*(bus))

/* Number of BUS clocks to first word */
#define _FIRST_WORD 8

/* Number of BUS clocks to next word */
#define _NEXT_WORD 1

/* Cycles for asm instructions that perform a cacheline copy */
#define _LOOP_CYCLES 16

/* Loop overhead estimate - include prefetch instructions */
#define _LOOP_OVERHEAD 5

/* The slower memory is the further ahead we need to prefetch. */
/* If cacheline size changes then the assembly code must change. */
/* This is only an approximation since other factors can affect */
/* cache lookahead (cache writebacks, instruction cache fills, ...). */
#define _CACHE_LINE_SIZE 32
#define _CACHE_ENTRIES ((_CACHE_LINE_SIZE)/sizeof(long long))
#if !defined(_CACHE_LOOKAHEAD)
#define _CACHE_LOOKAHEAD \
 (((_CC(_FIRST_WORD)+(_CACHE_ENTRIES-1)*_CC(_NEXT_WORD))-_LOOP_CYCLES)/\
  (_LOOP_CYCLES+_LOOP_OVERHEAD)+1)
#endif
#define _CACHE_LOOKAHEAD_SIZE (_CACHE_LINE_SIZE*_CACHE_LOOKAHEAD)

#define CACHE_ALIGN(p) (char *)(((unsigned)(p)+31)&~31)
//#define CACHE_ALIGN(p) (p)

/* Macro's for bytebliting unaligned data blocks */

/* Little-endian MACROS */

#if defined(__ARMEL__)

#define SH1 "asl"
#define SH2 "lsr"

#define BYTE_BLIT_INIT(s,h,o) \
(h) = *((unsigned *)(s))++ >> (o)

#define BYTE_BLIT_STEP(d,s,h,o) \
{ register unsigned _v_; _v_ = *((unsigned *)(s))++; \
 *((unsigned *)(d))++ = (h) | _v_ << (32-(o)); \
 (h) = _v_ >> (o); \
}

/* Big-endian MACROS */

#elif defined(__ARMEB__)

#define SH1 "lsr"
#define SH2 "asl"

#define BYTE_BLIT_INIT(s,h,o) \
(h) = *((unsigned *)(s))++ << (o)

#define BYTE_BLIT_STEP(d,s,h,o) \
{ register unsigned _v_; _v_ = *((unsigned *)(s))++; \
 *((unsigned *)(d))++ = (h) | _v_ >> (32-(o)); \
 (h) = _v_ << (o); \
}

#endif

/* Define a prefetch macro */
#define prefetch(_x_,_o_) \
__builtin_prefetch(((char *)(_x_))+(_o_))

/* Define an optimized macro to copy an aligned cache line. */
/* since the source cache line is double word aligned we can use */
/* the LDRD instruction for loads. */
#define ALIGNED_CACHE_LINE_COPY(s,d) \
__asm__ __volatile__( \
  "ldrd	r0, [%0], #8\n\t" \
  "ldrd	r2, [%0], #8\n\t" \
  "ldrd	r4, [%0], #8\n\t" \
  "str	r0, [%1], #4\n\t" \
  "str	r1, [%1], #4\n\t" \
  "ldrd	r0, [%0], #8\n\t" \
  "str	r2, [%1], #4\n\t" \
  "str	r3, [%1], #4\n\t" \
  "str	r4, [%1], #4\n\t" \
  "str	r5, [%1], #4\n\t" \
  "str	r0, [%1], #4\n\t" \
  "str	r1, [%1], #4" \
  : "+r" (s), "+r" (d) \
  : : "r0", "r1", "r2", "r3", "r4", "r5" \
  )

/* Define an optimized macro to copy an unaligned cache line. */
/* since the source cache line is double word aligned we can use */
/* the LDRD instruction for loads. */
#define UNALIGNED_CACHE_LINE_COPY(s,d,h,shift) \
__asm__ __volatile__( \
  "ldrd	r0, [%0], #8\n\t" \
  "ldrd	r2, [%0], #8\n\t" \
  "orr	r6, %2, r0, " SH1 " %3\n\t" \
  "ldrd	r4, [%0], #8\n\t" \
  "mov	%2, r0, " SH2 " %4\n\t" \
  "str	r6, [%1], #4\n\t" \
  "orr	r6, %2, r1, " SH1 " %3\n\t" \
  "mov	%2, r1, " SH2 " %4\n\t" \
  "str	r6, [%1], #4\n\t" \
  "ldrd	r0, [%0], #8\n\t" \
  "orr	r6, %2, r2, " SH1 " %3\n\t" \
  "mov	%2, r2, " SH2 " %4\n\t" \
  "str	r6, [%1], #4\n\t" \
  "orr	r6, %2, r3, " SH1 " %3\n\t" \
  "mov	%2, r3, " SH2 " %4\n\t" \
  "str	r6, [%1], #4\n\t" \
  "orr	r6, %2, r4, " SH1 " %3\n\t" \
  "mov	%2, r4, " SH2 " %4\n\t" \
  "str	r6, [%1], #4\n\t" \
  "orr	r6, %2, r5, " SH1 " %3\n\t" \
  "mov	%2, r5, " SH2 " %4\n\t" \
  "str	r6, [%1], #4\n\t" \
  "orr	r6, %2, r0, " SH1 " %3\n\t" \
  "mov	%2, r0, " SH2 " %4\n\t" \
  "str	r6, [%1], #4\n\t" \
  "orr	r6, %2, r1, " SH1 " %3\n\t" \
  "mov	%2, r1, " SH2 " %4\n\t" \
  "str	r6, [%1], #4\n\t" \
  : "+r" (s), "+r" (d), "+r" (h) \
  : "M"(shift), "M"(32-(shift)) \
  : "r0", "r1", "r2", "r3", "r4", "r5", "r6" \
  )


void *memcpySmall(void *d, const void *s, size_t c)
{
  void *r = d;

  /* Prefetch data while we are setting */
  /* up for memory copy. */

  prefetch(CACHE_ALIGN(s),0);
  prefetch(CACHE_ALIGN(d),0);

  if (c > 4)
  {
	unsigned x, a, h, align;

	/* Align the destination to a word boundry. */
	/* This is done in an endian independant manner. */
	switch ((unsigned) d & 3)
	{
	 case 1: *((char *)d)++ = *((char *)s)++; c--;
	 case 2: *((char *)d)++ = *((char *)s)++; c--;
	 case 3: *((char *)d)++ = *((char *)s)++; c--;
	}
	/* Choose a copy scheme based on the source */
	/* alignment relative to destination. */
	switch((unsigned) s & 3)
	{
	 case 0x0: /* Both byte offsets are aligned */

	   /* Align source to a cache line boundry. */
	   /* Endian doesn't matter for copies once we are word aligned */
	   /* Check to see if we would copy more bytes than we have left */
	   /* inorder to align to the cache boundry. */
	   x = 32 - ((unsigned) s & 0x1C);
	   if(x > c) x = c & 0x1C;
	   switch(x >> 2)
	   {
		case 7: *((unsigned *)d)++ = *((unsigned *)s)++;
		case 6: *((unsigned *)d)++ = *((unsigned *)s)++;
		case 5: *((unsigned *)d)++ = *((unsigned *)s)++;
		case 4: *((unsigned *)d)++ = *((unsigned *)s)++;
		case 3: *((unsigned *)d)++ = *((unsigned *)s)++;
		case 2: *((unsigned *)d)++ = *((unsigned *)s)++;
		case 1: *((unsigned *)d)++ = *((unsigned *)s)++;
		  c -= x;
	   }
	   align = -(int) d & 31;
	   /* Main cache aligned copy with lookahead */
	   for (; c >= _CACHE_LOOKAHEAD_SIZE+32; c -= 32)
	   {
		 prefetch(s,_CACHE_LOOKAHEAD_SIZE);
		 prefetch(d,_CACHE_LOOKAHEAD_SIZE+align);
		 ALIGNED_CACHE_LINE_COPY(s,d);
	   }
	   if (c > _CACHE_LOOKAHEAD_SIZE)
	   {
		 /* If there is more data, then prefetch the partial line */
		 /* otherwise don't put un-necessary traffic on the bus */
		 prefetch(s,_CACHE_LOOKAHEAD_SIZE);
		 prefetch(d,_CACHE_LOOKAHEAD_SIZE+align);
	   }
	   /* Finish up the copy with no lookahead */
	   /* All remaining data should be on the way to the cache */
	   for (; c >= 32; c -= 32)
		 ALIGNED_CACHE_LINE_COPY(s,d);

	   /* Finish copying a partial cache line */
	   switch (c >> 2)
	   {
		case 7: *((unsigned *)d)++ = *((unsigned *)s)++;
		case 6: *((unsigned *)d)++ = *((unsigned *)s)++;
		case 5: *((unsigned *)d)++ = *((unsigned *)s)++;
		case 4: *((unsigned *)d)++ = *((unsigned *)s)++;
		case 3: *((unsigned *)d)++ = *((unsigned *)s)++;
		case 2: *((unsigned *)d)++ = *((unsigned *)s)++;
		case 1: *((unsigned *)d)++ = *((unsigned *)s)++;
	   }
	   break;

	 case 0x1: /* Unaligned - Off by 1 */
	   /* Word align the source */
	   a = (unsigned) s & ~3;
	   /* Load the holding buffer */
	   BYTE_BLIT_INIT(a,h,8);
	   /* Align source to a cache line boundry. */
	   /* Byteblit the data */
	   x = 32 - ((unsigned) a & 0x1C);
	   if(x > c) x = c & 0x1C;
	   switch(x >> 2)
	   {
		case 7: BYTE_BLIT_STEP(d,a,h,8);
		case 6: BYTE_BLIT_STEP(d,a,h,8);
		case 5: BYTE_BLIT_STEP(d,a,h,8);
		case 4: BYTE_BLIT_STEP(d,a,h,8);
		case 3: BYTE_BLIT_STEP(d,a,h,8);
		case 2: BYTE_BLIT_STEP(d,a,h,8);
		case 1: BYTE_BLIT_STEP(d,a,h,8);
		  c -= x;
	   }
	   align = -(int) d & 31;
	   /* Source is now cache line aligned */
	   for (; c >= _CACHE_LOOKAHEAD_SIZE+32; c -= 32)
	   {
		 prefetch(a,_CACHE_LOOKAHEAD_SIZE);
		 prefetch(d,_CACHE_LOOKAHEAD_SIZE+align);
		 UNALIGNED_CACHE_LINE_COPY(a,d,h,24);
	   }
	   if (c > _CACHE_LOOKAHEAD_SIZE)
	   {
		 /* If there is more data, then prefetch the partial line */
		 /* otherwise don't put un-necessary traffic on the bus */
		 prefetch(a,_CACHE_LOOKAHEAD_SIZE);
		 prefetch(d,_CACHE_LOOKAHEAD_SIZE+align);
	   }
	   /* Finish up the copy with no lookahead */
	   /* All remaining data should be on the way to the cache */
	   for (; c >= 32; c -= 32)
		 UNALIGNED_CACHE_LINE_COPY(a,d,h,24);

	   /* Finish copying a partial cache line */
	   switch(c >> 2)
	   {
		case 7: BYTE_BLIT_STEP(d,a,h,8);
		case 6: BYTE_BLIT_STEP(d,a,h,8);
		case 5: BYTE_BLIT_STEP(d,a,h,8);
		case 4: BYTE_BLIT_STEP(d,a,h,8);
		case 3: BYTE_BLIT_STEP(d,a,h,8);
		case 2: BYTE_BLIT_STEP(d,a,h,8);
		case 1: BYTE_BLIT_STEP(d,a,h,8);
	   }
	   /* Realign the source */
	   (unsigned) s = a - 3;
	   break;

	 case 0x2: /* Unaligned - Off by 2 */
	   /* Word align the source */
	   a = (unsigned) s & ~3;
	   /* Load the holding buffer */
	   BYTE_BLIT_INIT(a,h,16);
	   /* Align source to a cache line boundry. */
	   /* Byteblit the data */
	   x = 32 - ((unsigned) a & 0x1C);
	   if(x > c) x = c & 0x1C;
	   switch(x >> 2)
	   {
		case 7: BYTE_BLIT_STEP(d,a,h,16);
		case 6: BYTE_BLIT_STEP(d,a,h,16);
		case 5: BYTE_BLIT_STEP(d,a,h,16);
		case 4: BYTE_BLIT_STEP(d,a,h,16);
		case 3: BYTE_BLIT_STEP(d,a,h,16);
		case 2: BYTE_BLIT_STEP(d,a,h,16);
		case 1: BYTE_BLIT_STEP(d,a,h,16);
		  c -= x;
	   }
	   align = -(int) d & 31;
	   /* Source is now cache line aligned */
	   for (; c >= _CACHE_LOOKAHEAD_SIZE+32; c -= 32)
	   {
		 prefetch(a,_CACHE_LOOKAHEAD_SIZE);
		 prefetch(d,_CACHE_LOOKAHEAD_SIZE+align);
		 UNALIGNED_CACHE_LINE_COPY(a,d,h,16);
	   }
	   if (c > _CACHE_LOOKAHEAD_SIZE)
	   {
		 /* If there is more data, then prefetch the partial line */
		 /* otherwise don't put un-necessary traffic on the bus */
		 prefetch(a,_CACHE_LOOKAHEAD_SIZE);
		 prefetch(d,_CACHE_LOOKAHEAD_SIZE+align);
	   }
	   /* Finish up the copy with no lookahead */
	   /* All remaining data should be on the way to the cache */
	   for (; c >= 32; c -= 32)
		 UNALIGNED_CACHE_LINE_COPY(a,d,h,16);

	   /* Finish copying a partial cache line */
	   switch(c >> 2)
	   {
		case 7: BYTE_BLIT_STEP(d,a,h,16);
		case 6: BYTE_BLIT_STEP(d,a,h,16);
		case 5: BYTE_BLIT_STEP(d,a,h,16);
		case 4: BYTE_BLIT_STEP(d,a,h,16);
		case 3: BYTE_BLIT_STEP(d,a,h,16);
		case 2: BYTE_BLIT_STEP(d,a,h,16);
		case 1: BYTE_BLIT_STEP(d,a,h,16);
	   }
	   /* Realign the source */
	   (unsigned) s = a - 2;
	   break;

	 case 0x3: /* Unaligned - Off by 3 */
	   /* Word align the source */
	   a = (unsigned) s & ~3;
	   /* Load the holding buffer */
	   BYTE_BLIT_INIT(a,h,24);
	   /* Align source to a cache line boundry. */
	   /* Byteblit the data */
	   x = 32 - ((unsigned) a & 0x1C);
	   if(x > c) x = c & 0x1C;
	   switch(x >> 2)
	   {
		case 7: BYTE_BLIT_STEP(d,a,h,24);
		case 6: BYTE_BLIT_STEP(d,a,h,24);
		case 5: BYTE_BLIT_STEP(d,a,h,24);
		case 4: BYTE_BLIT_STEP(d,a,h,24);
		case 3: BYTE_BLIT_STEP(d,a,h,24);
		case 2: BYTE_BLIT_STEP(d,a,h,24);
		case 1: BYTE_BLIT_STEP(d,a,h,24);
		  c -= x;
	   }
	   align = -(int) d & 31;
	   /* Source is now cache line aligned */
	   for (; c >= _CACHE_LOOKAHEAD_SIZE+32; c -= 32)
	   {
		 prefetch(a,_CACHE_LOOKAHEAD_SIZE);
		 prefetch(d,_CACHE_LOOKAHEAD_SIZE+align);
		 UNALIGNED_CACHE_LINE_COPY(a,d,h,8);
	   }
	   if (c > _CACHE_LOOKAHEAD_SIZE)
	   {
		 /* If there is more data, then prefetch the partial line */
		 /* otherwise don't put un-necessary traffic on the bus */
		 prefetch(a,_CACHE_LOOKAHEAD_SIZE);
		 prefetch(d,_CACHE_LOOKAHEAD_SIZE+align);
	   }
	   /* Finish up the copy with no lookahead */
	   /* All remaining data should be on the way to the cache */
	   for (; c >= 32; c -= 32)
		 UNALIGNED_CACHE_LINE_COPY(a,d,h,8);

	   /* Finish copying a partial cache line */
	   switch(c >> 2)
	   {
		case 7: BYTE_BLIT_STEP(d,a,h,24);
		case 6: BYTE_BLIT_STEP(d,a,h,24);
		case 5: BYTE_BLIT_STEP(d,a,h,24);
		case 4: BYTE_BLIT_STEP(d,a,h,24);
		case 3: BYTE_BLIT_STEP(d,a,h,24);
		case 2: BYTE_BLIT_STEP(d,a,h,24);
		case 1: BYTE_BLIT_STEP(d,a,h,24);
	   }
	   /* Realign the source */
	   (unsigned) s = a - 1;
	   break;
	}

	/* Finish off any remaining bytes */
	c &= 3;
	goto finish;
  }
  else
  {
 finish:
	/* simple fast copy, ... unless a cache boundry is crossed */
	switch(c)
	{
	 case 4: *((char *)d)++ = *((char *)s)++;
	 case 3: *((char *)d)++ = *((char *)s)++;
	 case 2: *((char *)d)++ = *((char *)s)++;
	 case 1: *((char *)d)++ = *((char *)s)++;
	}
  }
  return r;
}
