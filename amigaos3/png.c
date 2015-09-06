/*
** PNG Loader Module
**
** This file is part of the AV Project. See AV.h
**
** $VER: png.library 1.0 (02.03.97)
*/
/// Includes
#include <unistd.h>
#include <stdlib.h>
#include <png.h>
#include <exec/memory.h>
#include <clib/exec_protos.h>
#include <pragmas/exec_pragmas.h>
#include <setjmp.h>
#include <zlib.h>
#include <proto/exec.h>

///
/// "Load"
UBYTE *LoadPNG(STRPTR filename, ULONG *Width, ULONG *Height, BOOL *HasAlpha) {

	int i, number_passes;
	FILE *fh = NULL;
	png_structp png_ptr;
	png_infop info_ptr;
	png_infop end_info;
	char *buffer;
	png_bytep row_pointer;
	UBYTE *ret;

	//
	//  Create structures
	//
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
			NULL, NULL);
	if (!png_ptr) goto ld_fail;

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) goto ld_fail;

	end_info = png_create_info_struct(png_ptr);
	if (!end_info) goto ld_fail;

	//
	//  setup error handling
	//
	if (setjmp(png_ptr->jmpbuf))
		goto ld_fail;

	//
	//  Open the file, init I/O of libpng, and read file info
	//
	fh = fopen(filename, "rb");
	if (!fh) goto ld_fail;

	png_init_io(png_ptr, fh);
	png_read_info(png_ptr, info_ptr);

	//
	//  Transformations: Expand, Reduce, Unpack, ...
	//
	if (info_ptr->color_type == PNG_COLOR_TYPE_PALETTE && info_ptr->bit_depth < 8)
		png_set_expand(png_ptr);        //  CLUT < 8 bit

	if (info_ptr->color_type == PNG_COLOR_TYPE_GRAY && info_ptr->bit_depth < 8)
		png_set_expand(png_ptr);        //  grayscale < 8 bit

	if (info_ptr->valid & PNG_INFO_tRNS)
		png_set_expand(png_ptr);        //  expand alpha channel

	if (info_ptr->bit_depth == 16)
		png_set_strip_16(png_ptr);      //  reduce 16 bpg to 8 bpg

	if (info_ptr->color_type == PNG_COLOR_TYPE_GRAY || info_ptr->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png_ptr);   //  expand grayscale to rgb

	// TODO: insert code that handles backgroung transparency on user demand
	//       including input of a color and disabeling of this feature

	if (info_ptr->valid & PNG_INFO_bKGD)    //  set background
		png_set_background(png_ptr, &(info_ptr->background), PNG_BACKGROUND_GAMMA_FILE, 1, 1.0);

	// TODO: insert code that handles gamma correction on user demand

	//if (info_ptr->valid & PNG_INFO_gAMA)
	//  png_set_gamma(png_ptr, 1.25, info_ptr->gamma);

	if (info_ptr->interlace_type)
		number_passes = png_set_interlace_handling(png_ptr);

	png_read_update_info(png_ptr, info_ptr);

	*Width  = (ULONG)info_ptr->width;
	*Height = (ULONG)info_ptr->height;
	*HasAlpha = FALSE;

	//
	//  Allocate image buffer
	//

	ret = AllocVec(info_ptr->width*info_ptr->height*3, MEMF_ANY);
	if (!ret) goto ld_fail;

	buffer = AllocVec(info_ptr->rowbytes, MEMF_ANY);
	if (!buffer) goto ld_fail;

	row_pointer = buffer;

	//  TODO: progressive images

	for (i = 0; i < info_ptr->height; i++) {
		png_read_row(png_ptr, row_pointer, NULL);              //  Read row into buffer
		memcpy(ret+i*info_ptr->width*3, row_pointer, info_ptr->rowbytes);
	}

	png_read_end(png_ptr, end_info);

	FreeVec(buffer);
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	fclose(fh);
	fh = NULL;
	return(ret);

ld_fail:
	if (fh) fclose(fh);
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	if (buffer) FreeVec(buffer);
	if (ret) FreeVec(ret);
	return(0);
}

///



