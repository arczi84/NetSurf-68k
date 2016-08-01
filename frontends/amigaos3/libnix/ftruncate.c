#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#define DEVICES_TIMER_H
#include <dos/dosextens.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <stdio_libnix.h>
#include "stabs.h"
#include <signal.h>
#include <dos/dos.h>

/*
**
*/
extern void __seterrno(void);

/*
**
*/
static StdFileDes **stdfiledes;
static unsigned long stdfilesize=0;
static long stderrdes=0; /* The normal Amiga shell sets no process->pr_CES stream -
                          * we use Open("*",MODE_NEWFILE) in this case
                          */

/*
**
*/
static void _setup_file(StdFileDes *fp)
{ fp->lx_inuse  = 1;
  fp->lx_isatty = IsInteractive(fp->lx_fh) ? -1 : 0;
}

/*
**
*/
static __inline StdFileDes *_allocfd(void)
{ StdFileDes *fp,**sfd;
  int file,max;

  for(sfd=stdfiledes,max=stdfilesize,file=3;file<max;file++)
    if(!sfd[file] || !sfd[file]->lx_inuse)
      break;

  if(file>SHRT_MAX)
  { errno=EMFILE;
    return NULL;
  }

#define ADDFD 10
  if(file==max)
  { if((sfd=realloc(stdfiledes,(file+ADDFD)*sizeof(fp)))==NULL)
    { errno=ENOMEM;
      return NULL;
    }
    stdfiledes=sfd;
    stdfilesize+=ADDFD;
    for(max=file;max<stdfilesize;max++)
      sfd[max] = NULL;
  }

  if((fp=sfd[file])==NULL)
  { if((sfd[file]=fp=malloc(sizeof(*fp)))==NULL)
    { errno=ENOMEM;
      return NULL;
    }
    fp->lx_pos = file;
  }

  return fp;
}

int ftruncate(int fd, off_t length)
{
  StdFileDes *sfd = _lx_fhfromfd(fd);
  long pos,r,file;

  if(!sfd)
  { errno = EBADF;
    return EOF;
  }
  file=sfd->lx_fh;
  pos=Seek(file,0,OFFSET_CURRENT);

  if((r=SetFileSize(file,length,OFFSET_BEGINNING))==EOF)
  { errno=EIO;
    return EOF;
  }
  if((r=Seek(file,length>pos?pos:length,OFFSET_BEGINNING))==EOF)
  { errno=EIO;
    return EOF;
  }
  return 0;
}

int truncate(const char *path, off_t length)
{
  int fd,r;

  if(!(path && *path))
  { errno=EFAULT;
    return EOF;
  }

  if((fd=open(path,0))==EOF)
  { errno=ENOENT;
    return EOF;
  }

  r=ftruncate(fd,length);
  close(fd);

  return r;
}

#if 0
#if __GNUC__ < 3
# define UNUSED __attribute__((unused))
#else
# define UNUSED __attribute__((used))
#endif
static unsigned char __auth[] UNUSED = "$(diegocr:libnix)";
#endif
