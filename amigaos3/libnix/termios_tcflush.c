/*
 * $Id: termios_tcflush.c,v 1.6 2015/06/26 11:21:59 obarthel Exp $
 *
 * :ts=4
 *
 * Portable ISO 'C' (1994) runtime library for the Amiga computer
 * Copyright (c) 2002-2015 by Olaf Barthel <obarthel (at) gmx.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   - Neither the name of Olaf Barthel nor the names of contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef	_TERMIOS_HEADERS_H
#include "termios_headers.h"
#endif /* _TERMIOS_HEADERS_H */

/****************************************************************************/

/*
 * This is a bit messy and might possibly be done in a better way, but console
 * termios support is a bit of a hack at best.
 * tcflush() could possibly be implemented with using CMD_CLEAR with the
 * console.device, but this would be just as messy and possibly break
 * even more if e.g. stdin is a pipe.
 */

int
tcflush(int file_descriptor,int queue)
{
	int result = ERROR;
	struct fd *fd;

	ENTER();

	SHOWVALUE(file_descriptor);

	if(__check_abort_enabled)
		__check_abort();

	__stdio_lock();

	fd = __get_file_descriptor(file_descriptor);
	if(fd == NULL || FLAG_IS_CLEAR(fd->fd_Flags,FDF_TERMIOS))
	{
		__set_errno(EBADF);
		goto out;
	}

	__fd_lock(fd);

	if(queue < TCIFLUSH || queue > TCIOFLUSH)
	{
		SHOWMSG("Invalid queue specified to tcflush().");

		__set_errno(EINVAL);
		goto out;
	}

	if(queue == TCIFLUSH || queue == TCIOFLUSH)
	{
		LONG num_bytes_read;
		char buf[64];
		struct termios *tios;
		BPTR file;

		tios = fd->fd_Aux;

		file = __resolve_fd_file(fd);
		if(file == ZERO)
		{
			__set_errno(EBADF);
			goto out;
		}

		/* Set raw mode so we can read without blocking. */
		if(FLAG_IS_SET(tios->c_lflag,ICANON))
		{
			SetMode(file,DOSTRUE);

			SET_FLAG(fd->fd_Flags,FDF_NON_BLOCKING);
		}

		while(WaitForChar(file,1) != DOSFALSE)
		{
			if(__check_abort_enabled && FLAG_IS_SET(SetSignal(0,0),__break_signal_mask))
				break;

			/* Read away available data. (upto 8k) */
			num_bytes_read = Read(file,buf,64);
			if(num_bytes_read == ERROR || num_bytes_read == 0)
				break;
		}

		/* Restore the Raw/Cooked mode. */
		if(FLAG_IS_SET(tios->c_lflag,ICANON))
		{
			SetMode(file,DOSFALSE); /* Set Cooked = Canonical mode. */

			CLEAR_FLAG(fd->fd_Flags,FDF_NON_BLOCKING);
		}

		result = OK;
	}
	else if (queue == TCOFLUSH || queue == TCIOFLUSH)
	{
		/* TODO: Can we actually discard data buffered on the file?
		 * For now we do the same as tcdrain().
		 */
		result = tcdrain(file_descriptor);
	}
	else
	{
		/* ZZZ is this the correct result? */
		result = OK;
	}

 out:

	__fd_unlock(fd);

	__stdio_unlock();

	RETURN(result);
	return(result);
}
