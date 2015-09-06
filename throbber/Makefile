# makefile for libpng using gcc (generic, static library)
# Copyright (C) 2008 Glenn Randers-Pehrson
# Copyright (C) 2000 Cosmin Truta
# Copyright (C) 1995 Guy Eric Schalnat, Group 42, Inc.
#
# This code is released under the libpng license.
# For conditions of distribution and use, see the disclaimer
# and license in png.h

 ----------------------------------------------------------------------------
# built-in resource setup
# ----------------------------------------------------------------------------


#FB_IMAGE_left_arrow := framebuffer/res/icons/back.png
#FB_IMAGE_right_arrow := framebuffer/res/icons/forward.png
#FB_IMAGE_reload := framebuffer/res/icons/reload.png
#FB_IMAGE_stop_image := framebuffer/res/icons/stop.png
#FB_IMAGE_history_image := framebuffer/res/icons/history.png

#FB_IMAGE_left_arrow_g := framebuffer/res/icons/back_g.png
#FB_IMAGE_right_arrow_g := framebuffer/res/icons/forward_g.png
#FB_IMAGE_reload_g := framebuffer/res/icons/reload_g.png
#FB_IMAGE_stop_image_g := framebuffer/res/icons/stop_g.png
#FB_IMAGE_history_image_g := framebuffer/res/icons/history_g.png

#FB_IMAGE_scrolll := framebuffer/res/icons/scrolll.png
#FB_IMAGE_scrollr := framebuffer/res/icons/scrollr.png
#FB_IMAGE_scrollu := framebuffer/res/icons/scrollu.png
#FB_IMAGE_scrolld := framebuffer/res/icons/scrolld.png

FB_IMAGE_pointer := normalcursor_s.png
FB_IMAGE_hand := hand.png
FB_IMAGE_caret := textcursor.png
FB_IMAGE_menu := menu.png
FB_IMAGE_progress := waitcursor.png
FB_IMAGE_move := move.png

FB_IMAGE_throbber00 := throbber/throbber00.png
FB_IMAGE_throbber01 := throbber/throbber01.png
FB_IMAGE_throbber02 := throbber/throbber02.png
FB_IMAGE_throbber03 := throbber/throbber03.png
FB_IMAGE_throbber04 := throbber/throbber04.png
FB_IMAGE_throbber05 := throbber/throbber05.png
FB_IMAGE_throbber06 := throbber/throbber06.png
FB_IMAGE_throbber07 := throbber/throbber07.png
FB_IMAGE_throbber08 := throbber/throbber08.png
FB_IMAGE_throbber09 := throbber/throbber09.png
FB_IMAGE_throbber10 := throbber/throbber10.png

# 1: input file
# 2: output file
# 3: bitmap name
define convert_image

S_IMAGES += $(2)

$(2): $(1) $(TOOLROOT)/convert_image
	$(Q)$(TOOLROOT)/convert_image $(1) $(2) $(3)

endef

S_IMAGES :=

$(eval $(foreach V,$(filter FB_IMAGE_%,$(.VARIABLES)),$(call convert_image,$($(V)),$(OBJROOT)/image-$(patsubst FB_IMAGE_%,%,$(V)).c,$(patsubst FB_IMAGE_%,%,$(V)))))


# Location of the lib library and include files
INC = -I/projects/netsurf -I/opt/netsurf/share/include/
LIB = 

# Compiler, linker, lib and other tools
CC = m68k-amigaos-gcc -s -O3 -fomit-frame-pointer -m68060 -mnobitfield  -noixemul
LD = $(CC) 
AR_RC = m68k-amigaos-ar rcs
RANLIB = m68k-amigaos-ranlib
RM_F = rm -f


CFLAGS = -W -Wall 
LDFLAGS = $(LDRELEASE)
LIBS = -lz -lm

# File extensions
O=.o
A=.a
EXE=

# Variables
OBJS =  throbber00.o throbber01.o throbber02.o throbber03.o throbber04.o throbber05.o \
		throbber06.o throbber07.o throbber08.o throbber09.o throbber10.o caret.o
		progress.o pointer.o menu.o move.o hand.o

# Targets
all: static

.c$(O):
	$(CC) -c $(CFLAGS) $(INC) $<

static: libtheme$(A) 


libtheme$(A): $(OBJS)
	$(AR_RC) $@ $(OBJS)
	$(RANLIB) $@

clean:
	$(RM_F) *$(O) libtheme$(A)

