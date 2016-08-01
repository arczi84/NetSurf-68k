#include <stdio.h>
#include <stdlib.h>

#include <exec/exec.h>       
#include <graphics/gfxbase.h>
#include <cybergraphx/cybergraphics.h>
#include <intuition/intuition.h>

#include <clib/exec_protos.h>
#include <clib/graphics_protos.h>
#include <clib/intuition_protos.h>

#include <proto/cybergraphics.h>

int detect_screen(void);

struct GfxBase *GfxBase;
struct Library *CyberGfxBase;
struct IntuitionBase *IntuitionBase;

int detect_screen(void)
{
	struct Screen *myScreen;
	struct Window *myWindow;
	struct RastPort myRastPort;
	struct BitMap *myBitMap;

	APTR bitMapHandle;

	ULONG result;

	int bpp = 0;
	
	if((ULONG)IntuitionBase == 0)
	{
	IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library", 39L);	
	}	


	if((ULONG)GfxBase == 0)
	{
	GfxBase = (struct GfxBase *)OpenLibrary("graphics.library", 39L);
	}

	if((ULONG)CyberGfxBase == 0)
	{
	CyberGfxBase = OpenLibrary("cybergraphics.library", 0);	
	}
	
	if(!CyberGfxBase) {
		if( !(CyberGfxBase=OpenLibrary("cybergraphics.library",40L))) {
			printf("Couldn't open cybergraphics.");
			return(-1);
		}
	}
	

	myScreen = LockPubScreen(NULL);
	if(myScreen != NULL)
	{
		UnlockPubScreen(NULL, myScreen);
	}
	else
	{
		printf("Couldnt get screen data.\n");
		CloseLibrary((struct Library*)IntuitionBase);
		CloseLibrary(CyberGfxBase);
		CloseLibrary((struct Library *)GfxBase);

		return 5;
	}


	myBitMap = AllocBitMap(1, 1, 8, BMF_MINPLANES | BMF_DISPLAYABLE, myScreen->RastPort.BitMap);
	if((ULONG)myBitMap == 0)
	{
		printf("Couldnt allocate bitmap.\n");
		CloseLibrary(CyberGfxBase);
		CloseLibrary((struct Library *)GfxBase);
		CloseLibrary((struct Library *)IntuitionBase);
		return 6;
	}

	//Creates the RastPort used for blitting
	InitRastPort(&myRastPort);
	myRastPort.BitMap = myBitMap;

	result=GetCyberMapAttr(myBitMap, CYBRMATTR_PIXFMT);

	switch(result)
	{
		case 0://PIXFMT_LUT8:
			bpp = 8; //printf("0");
			break;
		case 1://PIXFMT_RGB15:
			bpp = 16; //printf("1");
			break;
		case 2://PIXFMT_BGR15:
			bpp = 16; //printf("2");
			break;
		case 3://PIXFMT_RGB15PC:
			bpp = 16; //printf("3");
			break;
		case 4://PIXFMT_BGR15PC:
			bpp = 16; //printf("4");
			break;
		case 5://PIXFMT_RGB16:
			bpp = 16; //printf("5");
			break;
		case 6://PIXFMT_BGR16:
			bpp = 16; //printf("6");
			break;
		case 7://PIXFMT_RGB16PC:
			bpp = 16; //printf("7");
			break;
		case 8://PIXFMT_BGR16PC:
			bpp = 16; //printf("8");
			break;
		case 9://PIXFMT_RGB24:
			bpp = 24; //printf("9");
			break;
		case 10://PIXFMT_BGR24:
			bpp = 24; //printf("10");
			break;
		case 11://PIXFMT_ARGB32:
			bpp = 32; //printf("11");
			break;
		case 12://PIXFMT_BGRA32:
		#if defined NOVA_SDL 
			bpp = 16; //printf("12");
		#else
			bpp = 32; //printf("12");
		#endif
			break;
		case 13://PIXFMT_RGBA32:
			bpp = 32; //printf("13");
			break;
		default:
			bpp = 16; //printf("14");
	}
#ifdef DEBUG
	printf("Pixel format: %d \n",result);
	printf("Bpp: %d ",bpp);
#endif
	FreeBitMap(myBitMap);

	CloseLibrary((struct Library *)IntuitionBase);
	//CloseLibrary(CyberGfxBase);
	//CloseLibrary((struct Library *)GfxBase);

	return bpp;
}
