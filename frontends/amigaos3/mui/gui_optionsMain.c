#define MUI_OBSOLETE

#include <libraries/mui.h>

#include <clib/alib_protos.h>
#include <proto/muimaster.h>
#include <proto/exec.h>
#include <proto/intuition.h>


#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif


#include "gui_options.h"

struct Library *MUIMasterBase;

void CleanExit(CONST_STRPTR s)
{
	if (s)
	{
		PutStr(s);
	}
	MyCleanExit();
	CloseLibrary(MUIMasterBase);
}

BOOL InitApp(int argc, char **argv)
{
	if ((MUIMasterBase = OpenLibrary("muimaster.library", 19)) == NULL)
	{
		CleanExit("Can't open muimaster.library v19\n");
	}
	if (!MyInitApp(argc, argv))
 	{
		CleanExit("Can't initialize application\n");
	}
	return TRUE;
}

int main(int argc, char **argv)
{
	InitApp(argc, argv);
	ULONG sigs = 0;
	struct ObjApp * obj = CreateApp();
	if (obj)
	{
		while (DoMethod(obj->App, MUIM_Application_NewInput, (IPTR)&sigs)
			!= MUIV_Application_ReturnID_Quit)
		{
			if (sigs)
			{
				sigs = Wait(sigs | SIGBREAKF_CTRL_C);
				if (sigs & SIGBREAKF_CTRL_C)
					break;
			}
		}
		DisposeApp(obj);
	}
	else
	{
		CleanExit("Can't create application\n");
	}
	CleanExit(NULL);
	return 0;
}
