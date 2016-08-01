/*************************************************************/
/* Includes and other common stuff for the MUI demo programs */
/*************************************************************/

/* MUI */
#include <libraries/mui.h>

/* System */
#include <dos/dos.h>
#include <graphics/gfxmacros.h>
#include <workbench/workbench.h>

/* Prototypes */
#include <clib/alib_protos.h>
#include <clib/exec_protos.h>
#include <clib/dos_protos.h>
#include <clib/icon_protos.h>
#include <clib/graphics_protos.h>
#include <clib/intuition_protos.h>
#include <clib/gadtools_protos.h>
#include <clib/utility_protos.h>
#include <clib/asl_protos.h>

#include <inline/muimaster.h>

struct Library *MUIMasterBase;

#define reg(a) __asm(#a)

/* ANSI C */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Compiler specific stuff */

/*************************/
/* Init & Fail Functions */
/*************************/


#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif


LONG __stack = 32768;
