# makefile for libpng using gcc (generic, static library)
# Copyright (C) 2008 Glenn Randers-Pehrson
# Copyright (C) 2000 Cosmin Truta
# Copyright (C) 1995 Guy Eric Schalnat, Group 42, Inc.
#
# This code is released under the libpng license.
# For conditions of distribution and use, see the disclaimer
# and license in png.h

#$(call ./convert_image.exe hand.png hand.c hand)

# Location of the lib library and include files
INC = -I/home/adminuser/host/projects/netsurf -I/home/adminuser/host/opt/netsurf/share/include/

# Compiler, linker, lib and other tools
CC = /home/adminuser/host/usr/local/amiga/clib/bin/m68k-unknown-amigaos-gcc -std=c99 -s -O3 -fomit-frame-pointer -m68060 -mnobitfield
LD = $(CC) 
AR_RC = /home/adminuser/host/usr/local/amiga/clib/bin/m68k-unknown-amigaos-ar rcs
RANLIB = /home/adminuser/host/usr/local/amiga/clib/bin/m68k-unknown-amigaos-ranlib
RM_F = rm -f


CFLAGS = -W -Wall -DLIBTHEME
LDFLAGS = $(LDRELEASE)
LIBS = -lz -lm

# File extensions
O=.o
A=.a
EXE=

# Variables
OBJS = throbber00.o throbber01.o throbber02.o throbber03.o throbber04.o throbber05.o \
		throbber06.o throbber07.o throbber08.o throbber09.o throbber10.o caret.o \
		progress.o pointer.o menu.o hand.o null_image.o

# Targets
all: static

.c$(O):
	$(CC) -c $(CFLAGS) $(INC) $<

static: libtheme$(A) 


libtheme$(A): $(OBJS)
	$(AR_RC) $@ $(OBJS)
	$(RANLIB) $@
	cp $@ /home/adminuser/host/opt/netsurf/share/lib

clean:
	$(RM_F) *$(O) libtheme$(A)

