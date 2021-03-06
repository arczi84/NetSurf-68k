
/*
** code.c
**
** (c) 2006 by Guido Mersmann
**
** Object source created by SimpleCat.
**
** DO NOT EDIT THIS FILE UNLESS YOU KNOW WHAT YOU ARE DOING
**
** Usage is quite simple.
**
** 1. Add this file to object creation list in makefile.
**
** 2. Call Locale_Open() and Locale_Close() on startup and shut down of your code.
**    There is no need to check any result.
**
** 3. Make sure the marked include line within 'code.h' header file uses
**    the name of your string header file. Thats name in .CS file which is set with
**    CFile command or when using the GUI in  target list .
**
** 4. Include the 'code.h' header everywhere you need a locale string.
**
** 5. Call GetString( id ) where ever you need a locale string.
**
** 6. Thats all!
**
*/

	#include <proto/exec.h>
	#include <proto/dos.h>

	#define __NOLIBBASE__
	#define __NOGLOBALIFACE__
	#include <proto/locale.h>

	#define CATCOMP_NUMBERS
	#define CATCOMP_STRINGS

#include "gui_locale.h" /* prototypes and catcomp block */

/*************************************************************************/


struct Library  *LocaleBase;


static struct Locale      *locale_locale;
static struct Catalog     *locale_catalog;

/*************************************************************************/

/* /// Locale_Open */

/*************************************************************************/
	
BOOL Locale_Open( STRPTR catname, ULONG version, ULONG revision)
{
	if (!LocaleBase)
		LocaleBase = OpenLibrary ("locale.library",0);

	if (LocaleBase)
			locale_catalog = OpenCatalog(NULL, "netsurf.catalog", TAG_END);

	if (locale_catalog) {
		#ifdef DEBUG
		Printf("catalog opened :) \n ");	
		#endif
		return(TRUE);
	}

		CloseLibrary( LocaleBase );
		LocaleBase = NULL;
		#ifdef DEBUG
		Printf("library closed \n ");
		#endif
		
	return(FALSE);
}
/* \\\ Locale_Open */

/* /// Locale_Close */

/*************************************************************************/

void Locale_Close(void)
{
    if( LocaleBase) {
        if( locale_catalog) {
            CloseCatalog(locale_catalog);
            locale_catalog = NULL;
        }
        if( locale_locale) {
            CloseLocale(locale_locale);
            locale_locale = NULL;
        }

		CloseLibrary( (APTR) LocaleBase );
		LocaleBase = NULL;
    }
}
/* \\\ */

/* /// GetString */

/*************************************************************************/

STRPTR GetMBString(long id)
{
LONG   *l;
UWORD  *w;
STRPTR  builtin;

	l = (LONG *)CatCompBlock;

    while (*l != id ) {
        w = (UWORD *)((ULONG)l + 4);
        l = (LONG *)((ULONG)l + (ULONG)*w + 6);
    }
    builtin = (STRPTR)((ULONG)l + 6);

	//printf("Catalog %08lx\n", locale_catalog);
	
    if ( locale_catalog && LocaleBase ) {
        return( (APTR) GetCatalogStr( locale_catalog, id, builtin));
    }
    return(builtin);
}
/* \\\ GetString */

