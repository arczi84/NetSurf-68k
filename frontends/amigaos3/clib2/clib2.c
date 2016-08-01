
#include <sys/types.h>

uint32_t
__bswapsi2 (uint32_t x)
{
 /* return ((((u) & 0xff000000) >> 24)
	  | (((u) & 0x00ff0000) >>  8)
	  | (((u) & 0x0000ff00) <<  8)
	  | (((u) & 0x000000ff) << 24));*/
	  
		__asm__ __volatile (
		"rorw #8, %0; swap %0; rorw #8, %0" : "=d" (x) : "0" (x));
				return x;	  
}
