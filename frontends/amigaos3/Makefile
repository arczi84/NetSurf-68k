# ----------------------------------------------------------------------------
# Amigaos3 target setup
# ----------------------------------------------------------------------------

override  TARGET = amigaos3
override  NETSURF_FB_FONTPATH="PROGDIR:Fonts"
CFLAGS += '-DNETSURF_FB_RESPATH="$(NETSURF_FB_RESPATH)"'

USE_FPU		:=  YES
SDL_NOVA 	:= 	NO
LIBNIX_VER 	:= 	3
DEBUG_BUILD	:=	NO
USE_TIMER	:=	YES
LIBNIX		:=  NO
TTF 		:=  YES

ifeq ($(JS),)
override	NETSURF_USE_DUKTAPE := YES
CFLAGS += -DJS
else
override	NETSURF_USE_DUKTAPE := NO
endif

ifeq ($(HOST),ixemul-nova)
SDL_NOVA:=YES
RTG:=YES
USE_TIMER:=NO
endif

ifeq ($(USE_FPU),NO)
CFLAGS += -DNO_FPU
endif

ifeq ($(USE_TIMER),NO)
CFLAGS += -DNO_TIMER
endif

ifeq ($(NO_MUI),YES)
CFLAGS += -DNO_MUI
endif

$(eval $(call feature_enabled,PNG,-DWITH_PNG,,PNG (libpng)  ))
$(eval $(call feature_enabled,GIF,-DWITH_GIF,,GIF (libnsgif)  ))
$(eval $(call feature_enabled,BMP,-DWITH_BMP,,BMP (libnsbmp)  ))
$(eval $(call feature_switch,JPEG,JPEG (libjpeg),-DWITH_JPEG,-ljpeg,-UWITH_JPEG,))


CFLAGS += -DFB_USE_FREETYPE 
ifeq ($(HOST),clib2)
  	LDFLAGS += -lfreetype2
	NETSURF_USE_DUKTAPE := YES
	override USE_TIMER			:=	NO
	CFLAGS +=  -DNO_TIMER -D__clib2__ -DNO_FPU
else
 	LDFLAGS += -lft2 
endif

ifeq ($(HOST),clib2)
LIBNIX := NO
CFLAGS += #-DHAVE_STRNDUP
endif
ifeq ($(HOST),libnix)
LIBNIX := YES
endif

ifeq ($(LIBNIX),YES)
	ifeq ($(LIBNIX_VER),3)
		  CFLAGS +=-DHAVE_STRNDUP=1
	endif
endif

CFLAGS +=  -Dnsamigaos3 -Dnsframebuffer -D__BIG_ENDIAN_BGRA__ \
		-Dsmall	-D__AMIGA__ -D__USE_INLINE__  -D__m68k__

ifeq ($(HOST),clib2)
CFLAGS += -I/opt/netsurf/share/include 
else
CFLAGS += -I/opt/netsurf/share/include \
		-I/usr/local/amiga/m68k-amigaos/include
endif		

CFLAGS += -std=c99   -DPATH_MAX=1024
	
ifeq ($(HOST),clib2)
	PREF=/opt/netsurf/m68k-unknown-amigaos	
	LDFLAGS += -L/opt/netsurf/m68k-unknown-amigaos/env/lib \
			-L/opt/netsurf/m68k-unknown-amigaos/cross/m68k-unknown-amigaos/lib 
	LDFLAGS += -L/opt/netsurf/share/lib 
else
	ifeq ($(LIBNIX),YES)
		LDFLAGS += -L/usr/local/amiga/m68k-amigaos/lib 
		LDFLAGS += -L/opt/netsurf/share/lib-nix
	else
		LDFLAGS += -L/usr/local/amiga/m68k-amigaos/lib 
		LDFLAGS += -L/opt/netsurf/share/lib
	endif
endif

LDFLAGS += -liconv  -lutf8proc -lnsutils
LDFLAGS += -Wl,--whole-archive -ldom -Wl,--no-whole-archive -lcss -lexpat -lhubbub

ifeq ($(LIBNIX),YES)
	#LDFLAGS += -lcurl.new-libnix -lssl.new-libnix -lcrypto.ok -lcrypto.new-libnix  -lsocket -lsocket881 #-lnet
	LDFLAGS += -lcurl-nix -lssl -lcrypto -lsocket #-lsocket881
else
	LDFLAGS += -lcurl -lssl -lcrypto
endif
LDFLAGS += -lwapcaplet -lparserutils 
LDFLAGS += -lnsgif -lnsbmp -lpng16 -lmz # -Wl,--allow-multiple-definition
ifneq ($(HOST),clib2)
LDFLAGS += -Wl,--wrap=adler32 -Wl,--wrap=inflateEnd -Wl,--wrap=inflate   -Wl,--wrap=deflate 
LDFLAGS += -Wl,--wrap=inflateInit2_   -Wl,--wrap=deflateInit2_ -Wl,--wrap=deflateReset 
LDFLAGS += -Wl,--wrap=deflateInit -Wl,--wrap=deflateEnd -Wl,--wrap=deflateBound
endif
#LDFLAGS += -Wl,--wrap=crc32 

			
ifeq ($(HOST),clib2)
LDFLAGS += -ltre -lpbl
endif

CFLAGS += 	-DENABLE_DOWNLOAD

ifeq ($(RTG),YES)
	ifeq ($(SDL_NOVA),YES)
		CFLAGS += -DNOVA_SDL #SDL_systimer.go 
		ifeq ($(HOST),clib2)
			LDFLAGS +=   /media/sf_E_DRIVE/usr/local/amiga/m68k-amigaos/lib/libSDL_nova4.a
		else
			LDFLAGS +=  -lsdl_nova4
		endif
	else  ## NOVA_SDL 
		ifeq ($(LIBNIX),YES) 
			LDFLAGS +=  -lSDL_libnix -lgl_dummy
		else
			ifeq ($(HOST),clib2-disable)
				LDFLAGS +=   /media/sf_E_DRIVE/usr/local/amiga/m68k-amigaos/lib/libSDL.a
			else
				LDFLAGS +=  -lSDL
			endif
		endif
	endif 
else
	CFLAGS +=  -DNO_TIMER
	ifeq ($(USE_TIMER),YES)
		LDFLAGS +=  -lSDL_AGA_040 c2p8_040_asm.o c2p8_040_dbl_asm.o
	else
		LDFLAGS +=  -lSDL-AGA c2p8_040_asm.o c2p8_040_dbl_asm.o  
	endif
endif  ## RTG SDL_systimer.go

ifeq ($(HOST),clib2)
LDFLAGS += -lm -lamiga2 -lc2 -lclib0 -lnet -leprintf
#LDFLAGS += -Wl,--wrap=netsurf_version 
#LDFLAGS +=  -Wl,--wrap=memcpy  -Wl,--wrap=memset  -Wl,--wrap=realloc -Wl,--wrap=memmove 
else
ifeq ($(LIBNIX),YES)
	ifeq ($(LIBNIX_VER),2)
		LDFLAGS += -Wl,--wrap=poll -Wl,--wrap=gmtime -Wl,--wrap=signal 
	endif
	LDFLAGS +=  -lregex -lc2-nix
	#LDFLAGS += -Wl,--wrap=content_update_status
	#LDFLAGS += -Wl,--allow-multiple-definition #-Wl,--wrap=user_agent_build_string	
else
  LDFLAGS += -lc2
endif		 
endif

ifeq ($(LIBNIX),YES)
	ifeq ($(LIBNIX_VER),2)
		LDFLAGS += -lnix2.2
	else
		LDFLAGS += -lnix3-fpu 
	endif
LDFLAGS += -lm2.2 -lnix2.2 -lstubs
#LDFLAGS += -Wl,--wrap=gettimeofday
else
LDFLAGS += -lc
endif

ifeq ($(NETSURF_USE_DUKTAPE), YES)
CFLAGS += -DJS  #-DDUK_F_VBCC -DDUK_USE_MATH_FMAX -DDUK_USE_MATH_FMIN
LDFLAGS += -lm3
endif

ifeq ($(DEBUG_BUILD),YES)
	LDFLAGS +=  -ltheme-o  -lthrobber 
endif

ifeq ($(NO_MUI),)
CFLAGS  += -DNO_INLINE_STDARG
LDFLAGS += -lmui 
endif
#-lamiga
LDFLAGS += -L. -ltheme  -lauto -ldebug
 
ifeq ($(LIBNIX),YES)
LDFLAGS += -lstubs
endif


# ----------------------------------------------------------------------------
# Source file setup
# ----------------------------------------------------------------------------

# Amiga sources 
ifeq ($(DEBUG_BUILD),YES)
S_AMIGAOS3 :=  gui-fb.c
else
S_AMIGAOS3 :=  gui.c
endif

S_AMIGAOS3 +=  framebuffer.c misc.c  utf8.c  loadpng.c
S_AMIGAOS3 +=  ami_clipboard.c clipboard.c version.c miniz.c #wrap.c

ifeq ($(LIBNIX),YES)
S_AMIGAOS3 +=  libnix.c #ftruncate.c
	ifeq ($(LIBNIX_VER),2)
		 S_AMIGAOS3 += poll.c gmtime.c signal.c ftruncate.c
		#fchdir.c currentdir.c  ftoa3.c  wrap.c
	endif		
endif

ifeq ($(HOST),clib2)
S_AMIGAOS3 += clib2/clib2.c
endif

# FRAMEBUFFER sources 
S_FRAMEBUFFER :=   localhistory.c bitmap.c schedule.c

S_AMIGAOS3 += fetch.c findfile.c 
ifeq ($(NO_MUI),)
S_AMIGAOS3 += mui/gui_optionsinit.c mui/gui_options.c mui/gui_locale.c
endif

S_FRAMEBUFFER_FBTK +=  fbtk.c fill.c user.c window.c

ifeq ($(DEBUG_BUILD),YES)
	S_FRAMEBUFFER_FBTK += event.c  text.c bitmap.c scroll.c ../../amigaos3/getopt.c
else
	S_AMIGAOS3_FBTK := event.c  text.c bitmap.c scroll.c 
endif

S_LIBNSFB_PLOT := api.c util.c generic.c  32bpp-xbgr8888.c 16bpp.c 8bpp.c
S_LIBNSFB_SURFACE := surface.c able.c ram.c sdl.c
S_LIBNSFB := libnsfb.c dump.c cursor.c palette.c

S_AMIGAOS3  += font_freetype.c font_internal.c

ifeq ($(RTG),YES)
S_LIBNSFB_SURFACE +=  getpixel.c
endif

S_FRAMEBUFFER := $(addprefix frontends/framebuffer/,$(S_FRAMEBUFFER)) $(addprefix frontends/framebuffer/fbtk/,$(S_FRAMEBUFFER_FBTK)) 
S_AMIGAOS3 :=	$(addprefix frontends/amigaos3/,$(S_AMIGAOS3)) $(addprefix frontends/amigaos3/fbtk/,$(S_AMIGAOS3_FBTK))
S_LIBNSFB :=  $(addprefix frontends/amigaos3/libnsfb/,$(S_LIBNSFB))  $(addprefix frontends/amigaos3/libnsfb/plot/,$(S_LIBNSFB_PLOT))  \
							$(addprefix frontends/amigaos3/libnsfb/surface/,$(S_LIBNSFB_SURFACE))  

S_AMIGA_FB :=  $(S_FRAMEBUFFER) $(S_AMIGAOS3) $(S_LIBNSFB)

# This is the final source build list
# Note this is deliberately *not* expanded here as common and image
#   are not yet available
SOURCES = $(S_COMMON) $(S_IMAGE) $(S_BROWSER) $(S_AMIGA_FB) $(S_IMAGES) $(S_FONTS)


ifeq ($(RTG),NO)
	ifeq ($(AGA),685)
		PARAM1="-AGA"
	else
		PARAM1="-AGAlq"
	endif
endif

ifeq ($(LIBNIX),YES)
	PARAM2="-libnix"
endif

ifeq ($(HOST),clib2)
	#PARAM2="-clib2"
endif

ifeq ($(NETSURF_USE_DUKTAPE),NO)
	ifneq ($(HOST),clib2)
		PARAM4="-nojs"
	endif
endif

ifeq ($(DEBUG),YES)
	PARAM5="-debug"
endif

ifeq ($(SDL_NOVA),YES)
	PARAM6="-24bit"
endif

ifeq ($(USE_FPU),NO)
	PARAM7="-nofpu"
endif
	
PARAMS:=$(PARAM1)$(PARAM2)$(PARAM3)$(PARAM4)$(PARAM5)$(PARAM6)$(PARAM7)
EXETARGET:=NetSurf$(PARAMS)

CFLAGS+= -DPARAMS='$(PARAMS)'


NETSURF_FRAMEBUFFER_RESOURCE_LIST := adblock.css credits.html	\
 	default.css internal.css licence.html netsurf.png quirks.css \
	 welcome.html maps.html Messages


# ----------------------------------------------------------------------------
# Install target
# ----------------------------------------------------------------------------

install-amiga:

# ----------------------------------------------------------------------------
# Package target
# ----------------------------------------------------------------------------

package-amiga: netsurf-m68k.lha

AMIGA_LANGUAGES := de en it ja nl pl
AMIGA_PLATFORM_RESOURCES := Themes Icons default.css default.css.info favicon.png LangNames Resource.map splash.png bookmarks.htm Options
AMIGA_GENERIC_RESOURCES := $(AMIGA_LANGUAGES) ca-bundle 
AMIGA_RESOURCES := $(addprefix frontends/amigaos3/resources/,$(AMIGA_PLATFORM_RESOURCES)) $(addprefix frontends/amigaos3/resources/,$(AMIGA_GENERIC_RESOURCES))
AMIGA_DISTRIBUTION_FILES := frontends/amigaos3/dist/*
AMIGA_INSTALL_TARGET_DIR := NetSurf_Amiga

netsurf-m68k.lha: $(EXETARGET)

	$(VQ)echo Creating netsurf-m68k.lha
	$(Q)rm -rf $(AMIGA_INSTALL_TARGET_DIR)
	$(Q)mkdir -p $(AMIGA_INSTALL_TARGET_DIR)/NetSurf
	$(Q)cp -p NetSurf $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/
	$(Q)cp -p NetSurf-AGA $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/	
	$(Q)mkdir $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Resources
	$(Q)cp -r $(AMIGA_RESOURCES) $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Resources
	$(Q)cp -r $(AMIGA_DISTRIBUTION_FILES) $(AMIGA_INSTALL_TARGET_DIR)/NetSurf
	$(Q)cp \!NetSurf/Resources/AdBlock,f79 $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Resources/adblock.css
	$(Q)cp \!NetSurf/Resources/CSS,f79 $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Resources/nsdefault.css
	$(Q)cp \!NetSurf/Resources/internal.css,f79 $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Resources/internal.css
	$(Q)cp \!NetSurf/Resources/Quirks,f79 $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Resources/quirks.css
	$(Q)cp \!NetSurf/Resources/netsurf.png,b60 $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Resources/netsurf.png
	$(Q)cp frontends/amigaos3/pkg/drawer.info $(AMIGA_INSTALL_TARGET_DIR)/NetSurf.info
	chmod 777 Netsurf_amiga *.* -R
	$(Q)cd $(AMIGA_INSTALL_TARGET_DIR); \
	  lha a netsurf-m68k.lha NetSurf NetSurf.info
	$(Q)cp $(AMIGA_INSTALL_TARGET_DIR)/netsurf-m68k.lha "e:\release"
	
upload:
	$(VQ)echo Uploading...
	$(Q)cp $(AMIGA_INSTALL_TARGET_DIR)/netsurf-m68k.lha "b:\dropbox\netsurf\3.6"
	$(Q)date > "b:\dropbox\netsurf\wip\version.txt"

ifeq ($(REL),AMIKIT)	

package-amikit: netsurf-amikit.lha

AMIGA_LANGUAGES := de en it ja nl pl
AMIGA_PLATFORM_RESOURCES_AK := Themes Icons default.css default.css.info favicon.png LangNames Resource.map splash.png bookmarks.htm Options-AK
AMIGA_GENERIC_RESOURCES := $(AMIGA_LANGUAGES) ca-bundle 
AMIGA_RESOURCES := $(addprefix frontends/amigaos3/resources/,$(AMIGA_PLATFORM_RESOURCES_AK)) $(addprefix frontends/amigaos3/resources/,$(AMIGA_GENERIC_RESOURCES))
AMIGA_DISTRIBUTION_FILES := frontends/amigaos3/dist/*
AMIGA_INSTALL_TARGET_DIR := NetSurf_Amiga

netsurf-amikit.lha: $(EXETARGET)

	$(VQ)echo Creating netsurf-amikit.lha
	$(Q)rm -rf $(AMIGA_INSTALL_TARGET_DIR)
	$(Q)mkdir -p $(AMIGA_INSTALL_TARGET_DIR)/NetSurf
	$(Q)cp -p NetSurf $(AMIGA_INSTALL_TARGET_DIR)/NetSurf
	$(Q)mkdir $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Resources
	$(Q)cp -r $(AMIGA_RESOURCES) $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Resources
	$(Q)cp -r $(AMIGA_DISTRIBUTION_FILES) $(AMIGA_INSTALL_TARGET_DIR)/NetSurf
	$(Q)cp \!NetSurf/Resources/AdBlock,f79 $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Resources/adblock.css
	$(Q)cp \!NetSurf/Resources/CSS,f79 $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Resources/nsdefault.css
	$(Q)cp \!NetSurf/Resources/internal.css,f79 $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Resources/internal.css
	$(Q)cp \!NetSurf/Resources/Quirks,f79 $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Resources/quirks.css
	$(Q)cp \!NetSurf/Resources/netsurf.png,b60 $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Resources/netsurf.png
	$(Q)cp frontends/amigaos3/pkg/drawer-ak.info $(AMIGA_INSTALL_TARGET_DIR)/NetSurf.info
	$(Q)cp -f frontends/amigaos3/pkg/ak/*.info $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/

	chmod 777 Netsurf_amiga *.* -R
	mv $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Resources/Options-AK $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Resources/Options
	rm -R $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/C
	rm $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/NetSurf-AGA.info
	rm -R $(AMIGA_INSTALL_TARGET_DIR)/NetSurf/Libs
	
	$(Q)cd $(AMIGA_INSTALL_TARGET_DIR); \
	  lha a netsurf-amikit.lha NetSurf NetSurf.info

upload-amikit:
	$(VQ)echo Uploading...
	$(Q)cp $(AMIGA_INSTALL_TARGET_DIR)/netsurf-amikit.lha "b:\dropbox\netsurf"
	$(Q)date > "b:\dropbox\netsurf\wip\version.txt"
endif