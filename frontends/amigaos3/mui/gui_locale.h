#ifndef CODE_H
#define CODE_H 1

/*
** code.h
**
** (c) 2006 by Guido Mersmann
**
** Object source created by SimpleCat
*/

/*************************************************************************/
#define CATCOMP_BLOCK 1
#define CATCOMP_ARRAY 1

#include "gui_options_cat.h" /* change name to correct locale header if needed */

/*
** Prototypes
*/

BOOL   Locale_Open( STRPTR catname, ULONG version, ULONG revision);
void   Locale_Close(void);
STRPTR GetMBString(long ID);

/*************************************************************************/

#endif /* CODE_H */
