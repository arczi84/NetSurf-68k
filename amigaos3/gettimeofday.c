/**
***  simple gettimeofday() replacement; no timezone handling
***
***  Jochen Wiedmann, 14-Feb-95
***
***  This is in the public domain, use it as you want.
**/


/****************************************************************
    This file uses the auto initialization possibilities of
    Dice, gcc and SAS/C, respectively.

    Dice does this by using the keywords __autoinit and
    __autoexit, SAS uses names beginning with _STI or
    _STD, respectively. gcc uses the asm() instruction,
    to emulate C++ constructors and destructors.
****************************************************************/


#if defined(__SASC)
#define __autoinit
#define __autoexec
extern void _XCEXIT(ULONG);
#elif defined(__GNUC__)
#define __autoinit
#define __autoexit
__asm ("  .text;  .stabs \"___CTOR_LIST__\",22,0,0,__STIInitGetTimeOfDay");
__asm ("  .text;  .stabs \"___DTOR_LIST__\",22,0,0,__STDTermGetTimeOfDay");
#elif defined(_DCC)
extern void _AutoFail0(void);
#else
#error "Don't know how to handle your compiler."
#endif

#include <stdlib.h>
#include <errno.h>

#include <exec/types.h>
#include <exec/io.h>
#include <devices/timer.h>
#include <proto/exec.h>


STATIC struct MsgPort *port     = NULL;
STATIC struct timerequest *req  = NULL;

__autoinit LONG _STIInitGetTimeOfDay(VOID)

{ if ((port = CreateMsgPort()))
  { if ((req = CreateIORequest(port, sizeof(*req))))
    { if (!(OpenDevice((STRPTR) "timer.device", UNIT_MICROHZ,
		       (struct IORequest *) req, 0)))
      { return(FALSE);
      }
      DeleteIORequest(req);
      req = NULL;
    }
    DeleteMsgPort(port);
    port = NULL;
  }
#if defined(__GNUC__)
  abort();
#elif defined(_DCC)
  _AutoFail0();
#endif
  return(TRUE);
}


__autoexit VOID _STDTermGetTimeOfDay(VOID)

{ if (req)
  { CloseDevice((struct IORequest *) req);
    DeleteIORequest(req);
    req = NULL;
  }
  if (port)
  { DeleteMsgPort(port);
    port = NULL;
  }
}


struct timezone;
int gettimeofday(struct timeval *tv, struct timezone *tz)

{ int error;

  req->tr_node.io_Command = TR_GETSYSTIME;
  error = DoIO((struct IORequest *) req);
  *tv = req->tr_time;
  if (error)
  { errno = ENOMEM;
    return(-1);
  }
  return(0);
}

int setsystime(struct timeval *tv, struct timezone *tz)

{ int error;

  req->tr_time = *tv;
  req->tr_node.io_Command = TR_SETSYSTIME;
  error = DoIO((struct IORequest *) req);
  if (error)
  { errno = ENOMEM;
    return(-1);
  }
  return(0);
}
