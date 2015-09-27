
/*
 * Copyright (c) 1992 Commodore-Amiga, Inc.
 *
 * This example is provided in electronic form by Commodore-Amiga, Inc. for
 * use with the "Amiga ROM Kernel Reference Manual: Devices", 3rd Edition,
 * published by Addison-Wesley (ISBN 0-201-56775-X).
 *
 * The "Amiga ROM Kernel Reference Manual: Devices" contains additional
 * information on the correct usage of the techniques and operating system
 * functions presented in these examples.  The source and executable code
 * of these examples may only be distributed in free electronic form, via
 * bulletin board or as part of a fully non-commercial and freely
 * redistributable diskette.  Both the source and executable code (including
 * comments) must be included, without modification, in any copy.  This
 * example may not be published in printed form or distributed with any
 * commercial product.  However, the programming techniques and support
 * routines set forth in these examples may be used in the development
 * of original executable software products for Commodore Amiga computers.
 *
 * All other rights reserved.
 *
 * This example is provided "as-is" and is subject to change; no
 * warranties are made.  All use is at your own risk. No liability or
 * responsibility is assumed.
 *
 **************************************************************************
 *
 *
 * Cbio.c
 *
 * Provide standard clipboard device interface routines
 *            such as Open, Close, Post, Read, Write, etc.
 *
 * Compile with SAS C 5.10: LC -b1 -cfistq -v -y
 *
 *  NOTES: These functions are useful for writing and reading simple
 *         FTXT.  Writing and reading complex FTXT, ILBM, etc.,
 *         requires more work.  You should use the iffparse.library
 *         to write and read FTXT, ILBM and other IFF file types.
 *
 *         When this code is used with older versions of the Amiga OS
 *         (i.e., before V36) a memory loss of 536 bytes will occur due
 *         to bugs in the clipboard device.
 *
 */

#include "exec/types.h"
#include "exec/ports.h"
#include "exec/io.h"
#include "exec/memory.h"
#include "devices/clipboard.h"
#include "libraries/dosextens.h"
#include "libraries/dos.h"

#define CBIO 1

#include "ami_clipboard.h"

#include <proto/exec.h>
#include <clib/exec_protos.h>
#include <clib/alib_protos.h>


#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/****** cbio/CBOpen *************************************************
*
*   NAME
*       CBOpen() -- Open the clipboard.device
*
*   SYNOPSIS
*       ior = CBOpen(unit)
*
*       struct IOClipReq *CBOpen( ULONG )
*
*   FUNCTION
*       Opens the clipboard.device.  A clipboard unit number
*       must be passed in as an argument.  By default, the unit
*       number should be 0 (currently valid unit numbers are
*       0-255).
*
*   RESULTS
*       A pointer to an initialized IOClipReq structure, or
*       a NULL pointer if the function fails.
*
*********************************************************************/


struct IOClipReq *CBOpen(unit)
ULONG unit;
{
struct MsgPort *mp;
struct IORequest *ior;

if (mp = CreatePort(0L,0L))
    {
    if (ior=(struct IORequest *)CreateExtIO(mp,sizeof(struct IOClipReq)))
        {
        if (!(OpenDevice("clipboard.device",unit,ior,0L)))
            {
            return((struct IOClipReq *)ior);
            }
        DeleteExtIO(ior);
        }
    DeletePort(mp);
    }
return(NULL);

}

/****** cbio/CBClose ************************************************
*
*   NAME
*       CBClose() -- Close the clipboard.device
*
*   SYNOPSIS
*       CBClose()
*
*       void CBClose()
*
*   FUNCTION
*       Close the clipboard.device unit which was opened via
*       CBOpen().
*
*********************************************************************/

void CBClose(ior)
struct IOClipReq *ior;
{
struct MsgPort *mp;

mp = ior->io_Message.mn_ReplyPort;

CloseDevice((struct IORequest *)ior);
DeleteExtIO((struct IORequest *)ior);
DeletePort(mp);

}

/****** cbio/CBWriteFTXT *********************************************
*
*   NAME
*       CBWriteFTXT() -- Write a string of text to the clipboard.device
*
*   SYNOPSIS
*       success = CBWriteFTXT( ior, string)
*
*       int CBWriteFTXT(struct IOClipReq *, char *)
*
*   FUNCTION
*       Write a NULL terminated string of text to the clipboard.
*       The string will be written in simple FTXT format.
*
*       Note that this function pads odd length strings automatically
*       to conform to the IFF standard.
*
*   RESULTS
*       TRUE if the write succeeded, else FALSE.
*
*********************************************************************/

int CBWriteFTXT(ior,string)
struct IOClipReq *ior;
char *string;
{

ULONG length, slen;
BOOL odd;
int success;

slen = strlen(string);
odd = (slen & 1);               /* pad byte flag */

length = (odd) ? slen+1 : slen;

/* initial set-up for Offset, Error, and ClipID */

ior->io_Offset = 0;
ior->io_Error  = 0;
ior->io_ClipID = 0;


/* Create the IFF header information */

WriteLong(ior, (long *) "FORM");     /* "FORM"             */
length+=12L;                         /* + "[size]FTXTCHRS" */
WriteLong(ior, &length);             /* total length       */
WriteLong(ior, (long *) "FTXT");     /* "FTXT"             */
WriteLong(ior, (long *) "CHRS");     /* "CHRS"             */
WriteLong(ior, &slen);               /* string length      */

/* Write string */
ior->io_Data    = (STRPTR)string;
ior->io_Length  = slen;
ior->io_Command = CMD_WRITE;
DoIO( (struct IORequest *) ior);

/* Pad if needed */
if (odd)
    {
    ior->io_Data   = (STRPTR)"";
    ior->io_Length = 1L;
    DoIO( (struct IORequest *) ior);
    }

/* Tell the clipboard we are done writing */

ior->io_Command=CMD_UPDATE;
DoIO( (struct IORequest *) ior);

/* Check if io_Error was set by any of the preceding IO requests */
success = ior->io_Error ? FALSE : TRUE;

return(success);
}

int
WriteLong(ior, ldata)
struct IOClipReq *ior;
long *ldata;
{

ior->io_Data    = (STRPTR)ldata;
ior->io_Length  = 4L;
ior->io_Command = CMD_WRITE;
DoIO( (struct IORequest *) ior);

if (ior->io_Actual == 4)
    {
    return( ior->io_Error ? FALSE : TRUE);
    }

return(FALSE);

}

/****** cbio/CBQueryFTXT **********************************************
*
*   NAME
*       CBQueryFTXT() -- Check to see if clipboard contains FTXT
*
*   SYNOPSIS
*       result = CBQueryFTXT( ior )
*
*       int CBQueryFTXT(struct IOClipReq *)
*
*   FUNCTION
*       Check to see if the clipboard contains FTXT.  If so,
*       call CBReadCHRS() one or more times until all CHRS
*       chunks have been read.
*
*   RESULTS
*       TRUE if the clipboard contains an FTXT chunk, else FALSE.
*
*   NOTES
*       If this function returns TRUE, you must either call
*       CBReadCHRS() until CBReadCHRS() returns FALSE, or
*       call CBReadDone() to tell the clipboard.device that
*       you are done reading.
*
*********************************************************************/

int CBQueryFTXT(ior)
struct IOClipReq *ior;
{
ULONG cbuff[4];


/* initial set-up for Offset, Error, and ClipID */

ior->io_Offset = 0;
ior->io_Error  = 0;
ior->io_ClipID = 0;

/* Look for "FORM[size]FTXT" */

ior->io_Command = CMD_READ;
ior->io_Data    = (STRPTR)cbuff;
ior->io_Length  = 12;

DoIO( (struct IORequest *) ior);


/* Check to see if we have at least 12 bytes */

if (ior->io_Actual == 12L)
    {
    /* Check to see if it starts with "FORM" */
    if (cbuff[0] == ID_FORM)
        {
        /* Check to see if its "FTXT" */
        if (cbuff[2] == ID_FTXT)
            return(TRUE);
        }

    /* It's not "FORM[size]FTXT", so tell clipboard we are done */
    }

CBReadDone(ior);

return(FALSE);
}


/****** cbio/CBReadCHRS **********************************************
*
*   NAME
*       CBReadCHRS() -- Reads the next CHRS chunk from clipboard
*
*   SYNOPSIS
*       cbbuf = CBReadCHRS( ior )
*
*       struct cbbuf *CBReadCHRS(struct IOClipReq * )
*
*   FUNCTION
*       Reads and returns the text in the next CHRS chunk
*       (if any) from the clipboard.
*
*       Allocates memory to hold data in next CHRS chunk.
*
*   RESULTS
*       Pointer to a cbbuf struct (see cb.h), or a NULL indicating
*       a failure (e.g., not enough memory, or no more CHRS chunks).
*
*       ***Important***
*
*       The caller must free the returned buffer when done with the
*       data by calling CBFreeBuf().
*
*   NOTES
*       This function strips NULL bytes, however, a full reader may
*       wish to perform more complete checking to verify that the
*       text conforms to the IFF standard (stripping data as required).
*
*       Under 2.0, the AllocVec() function could be used instead of
*       AllocMem() in which case the cbbuf structure may not be
*       needed.
*
*********************************************************************/

struct cbbuf *CBReadCHRS(ior)
struct IOClipReq *ior;
{
ULONG chunk,size;
struct cbbuf *buf;
int looking;

/* Find next CHRS chunk */

looking = TRUE;
buf = NULL;

while (looking)
      {
      looking = FALSE;

      if (ReadLong(ior,&chunk))
          {
          /* Is CHRS chunk ? */
          if (chunk == ID_CHRS)
              {
              /* Get size of chunk, and copy data */
              if (ReadLong(ior,&size))
                  {
                  if (size)
                      buf=FillCBData(ior,size);
                  }
              }

            /* If not, skip to next chunk */
          else
              {
              if (ReadLong(ior,&size))
                  {
                   looking = TRUE;
                   if (size & 1)
                       size++;    /* if odd size, add pad byte */

                    ior->io_Offset += size;
                  }
              }
          }
      }

if (buf == NULL)
    CBReadDone(ior);        /* tell clipboard we are done */

return(buf);
}

int
ReadLong(ior, ldata)
struct IOClipReq *ior;
ULONG *ldata;
{
ior->io_Command = CMD_READ;
ior->io_Data    = (STRPTR)ldata;
ior->io_Length  = 4L;

DoIO( (struct IORequest *) ior);

if (ior->io_Actual == 4)
    {
    return( ior->io_Error ? FALSE : TRUE);
    }

return(FALSE);
}


struct cbbuf *FillCBData(ior,size)
struct IOClipReq *ior;
ULONG size;
{
register UBYTE *to,*from;
register ULONG x,count;

ULONG length;
struct cbbuf *buf,*success;

success = NULL;

if (buf = AllocMem(sizeof(struct cbbuf),MEMF_PUBLIC))
    {

    length = size;
    if (size & 1)
        length++;            /* if odd size, read 1 more */

    if (buf->mem = AllocMem(length+1L,MEMF_PUBLIC))
        {
        buf->size = length+1L;

        ior->io_Command = CMD_READ;
        ior->io_Data    = (STRPTR)buf->mem;
        ior->io_Length  = length;

        to = buf->mem;
        count = 0L;

        if (!(DoIO( (struct IORequest *) ior)))
            {
            if (ior->io_Actual == length)
                {
                success = buf;      /* everything succeeded */

                /* strip NULL bytes */
                for (x=0, from=buf->mem ;x<size;x++)
                     {
                     if (*from)
                         {
                         *to = *from;
                         to++;
                         count++;
                         }

                     from++;
                     }
                *to=0x0;            /* Null terminate buffer */
                buf->count = count; /* cache count of chars in buf */
                }
            }

        if (!(success))
            FreeMem(buf->mem,buf->size);
        }
    if (!(success))
        FreeMem(buf,sizeof(struct cbbuf));
    }

return(success);
}

/****** cbio/CBReadDone **********************************************
*
*   NAME
*       CBReadDone() -- Tell clipboard we are done reading
*
*   SYNOPSIS
*       CBReadDone( ior )
*
*       void CBReadDone(struct IOClipReq * )
*
*   FUNCTION
*       Reads past end of clipboard file until io_Actual is equal to 0.
*       This is tells the clipboard that we are done reading.
*
*********************************************************************/

void CBReadDone(ior)
struct IOClipReq *ior;
{
char buffer[256];

ior->io_Command = CMD_READ;
ior->io_Data    = (STRPTR)buffer;
ior->io_Length  = 254;


/* falls through immediately if io_Actual == 0 */

while (ior->io_Actual)
      {
      if (DoIO( (struct IORequest *) ior))
          break;
      }
}

/****** cbio/CBFreeBuf **********************************************
*
*   NAME
*       CBFreeBuf() -- Free buffer allocated by CBReadCHRS()
*
*   SYNOPSIS
*       CBFreeBuf( buf )
*
*       void CBFreeBuf( struct cbbuf * )
*
*   FUNCTION
*       Frees a buffer allocated by CBReadCHRS().
*
*********************************************************************/

void CBFreeBuf(struct cbbuf *buf)
{
if (buf->size > 0) {
FreeMem(buf->mem, buf->size);
FreeMem(buf, sizeof(struct cbbuf)); }
}

/*
 * Copyright (c) 1992 Commodore-Amiga, Inc.
 *
 * This example is provided in electronic form by Commodore-Amiga, Inc. for
 * use with the "Amiga ROM Kernel Reference Manual: Devices", 3rd Edition,
 * published by Addison-Wesley (ISBN 0-201-56775-X).
 *
 * The "Amiga ROM Kernel Reference Manual: Devices" contains additional
 * information on the correct usage of the techniques and operating system
 * functions presented in these examples.  The source and executable code
 * of these examples may only be distributed in free electronic form, via
 * bulletin board or as part of a fully non-commercial and freely
 * redistributable diskette.  Both the source and executable code (including
 * comments) must be included, without modification, in any copy.  This
 * example may not be published in printed form or distributed with any
 * commercial product.  However, the programming techniques and support
 * routines set forth in these examples may be used in the development
 * of original executable software products for Commodore Amiga computers.
 *
 * All other rights reserved.
 *
 * This example is provided "as-is" and is subject to change; no
 * warranties are made.  All use is at your own risk. No liability or
 * responsibility is assumed.
 *
 ***************************************************************************
 *
 * Clipdemo.c
 *
 * Demonstrate use of clipboard I/O.  Uses general functions
 * provided in cbio.c.  Important note: when this code is run
 * with older versions of the Amiga OS (i.e., before V36) a memory
 * loss of 536 bytes will occur due to bugs in the clipboard device.
 *
 * Compile with SAS C 5.10: LC -b1 -cfistq -v -y -L+cbio.o
 *
 * Run from CLI only
 */

#define FORGETIT 0
#define READIT   1
#define WRITEIT  2
#define POSTIT   3


/*
 * Read, and output FTXT in the clipboard.
 *
 */


char *
ReadClip()
{
struct IOClipReq *ior;
struct cbbuf *buf;
char *string;// = NULL;

/* Open clipboard.device unit 0 */

if (ior=CBOpen(0L))
    {

    /* Look for FTXT in clipboard */

    if (CBQueryFTXT(ior))
        {

        /* Obtain a copy of the contents of each CHRS chunk */

        while (buf=CBReadCHRS(ior))
              {
              /* Process data */

	      	   string = strdup(buf->mem);

              /* Free buffer allocated by CBReadCHRS() */

              CBFreeBuf(buf);
              }

        /* The next call is not really needed if you are sure */
        /* you read to the end of the clip.                   */

        CBReadDone(ior);
        }
    else
        {
        puts("No FTXT in clipboard");
        }

    CBClose(ior);
    }

else
    {
    puts("Error opening clipboard unit 0");
    }
printf("Clipboard is :%s\n",string);
return(string);
}

/*
 * Write a string to the clipboard
 *
 */

int
WriteClip(string)
char *string;
{

struct IOClipReq *ior;

if (string == NULL)
    {
    puts("No string argument given");
    return(0L);
    }

/* Open clipboard.device unit 0 */

if (ior = CBOpen(0L))
    {
    if (!(CBWriteFTXT(ior,string)))
        {
        printf("Error writing to clipboard: io_Error = %ld\n",ior->io_Error);
        }    
    CBClose(ior);
    }
else
    {
    puts("Error opening clipboard.device");
    }

return(0);
}
