#include <stdint.h>
#include <sys/types.h>
#include <stdbool.h>
#include <malloc.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_video.h>

#include "amigaos3/bitmap.h"

void convert8(struct SDL_Surface *surface, unsigned int *dest)
{
	unsigned int i, j, pix;
	unsigned char *src;
	SDL_Color *color;
	Uint8 r, g, b, a;

	src = (unsigned char*)surface->pixels;

	for(i = 0; i < surface->h; ++i)
	{
		for(j = 0; j < surface->w; ++j)
		{
			pix = *src++;
			SDL_GetRGBA(pix, surface->format, &r, &g, &b, &a);
			pix = (unsigned int)r << 24;
			pix |= (unsigned int)g << 16;
			pix |= (unsigned int)b << 8;
			pix |= (unsigned int)a;

			*dest++ = pix;
		}

		src += surface->pitch - surface->w;
	}
}

void convert16(struct SDL_Surface *surface, unsigned int *dest)
{
	unsigned int i, j, pix;
	unsigned char *src;
	Uint8 r, g, b, a;

	src = (unsigned char*)surface->pixels;

	for(i = 0; i < surface->h; ++i)
	{
		for(j = 0; j < surface->w; ++j)
		{
			pix = (unsigned int)*src++ << 8;
			pix |= (unsigned int)*src++;
			SDL_GetRGBA(pix, surface->format, &r, &g, &b, &a);
			pix = (unsigned int)r << 24;
			pix |= (unsigned int)g << 16;
			pix |= (unsigned int)b << 8;
			pix |= (unsigned int)a;

			*dest++ = pix;
		}

		src += surface->pitch - (surface->w << 1);
	}
}

void convert24(struct SDL_Surface *surface, unsigned int *dest)
{
	unsigned int i, j, pix;
	unsigned char *src;
	Uint8 r, g, b, a;

	src = (unsigned char*)surface->pixels;

	for(i = 0; i < surface->h; ++i)
	{
		for(j = 0; j < surface->w; ++j)
		{
			pix = (unsigned int)*src++ << 16;
			pix |= (unsigned int)*src++ << 8;
			pix |= (unsigned int)*src++;
			SDL_GetRGBA(pix, surface->format, &r, &g, &b, &a);
			pix = (unsigned int)r << 24;
			pix |= (unsigned int)g << 16;
			pix |= (unsigned int)b << 8;
			pix |= (unsigned int)a;

			*dest++ = pix;
		}

		src += surface->pitch - surface->w * 3;
	}
}

void convert32(struct SDL_Surface *surface, unsigned int *dest)
{
	unsigned int i, j;
	unsigned int pix;
	unsigned char *src;
	Uint8 r, g, b, a;

	src = (unsigned char*)surface->pixels;

	for(i = 0; i < surface->h; ++i)
	{
		for(j = 0; j < surface->w; ++j)
		{
			pix = *((unsigned int*)src);
			SDL_GetRGBA(pix, surface->format, &r, &g, &b, &a);
			pix = (unsigned int)r << 24;
			pix |= (unsigned int)g << 16;
			pix |= (unsigned int)b << 8;
			pix |= (unsigned int)a;

			src += 4;
			*dest++ = pix;
		}

		src += surface->pitch - (surface->w << 2);
	}
}

short convertbitmap(struct SDL_Surface *surface, unsigned int *dest)
{
	switch(surface->format->BytesPerPixel)
	{
		case 1:

			convert8(surface, dest);
			break;

		case 2:

			convert16(surface, dest);
			break;

		case 3:

			convert24(surface, dest);
			break;

		case 4:

			convert32(surface, dest);
			break;

		default:

			return false;
	}

	return true;
}

struct bitmap *load_bitmap(const char *filename)
{
	struct SDL_Surface *surface;

	surface = IMG_Load(filename);

	if(surface != NULL)
	{
		struct bitmap *b;

		b = malloc(sizeof(struct bitmap));

		if(b != NULL)
		{
			unsigned int *pix;

			pix = malloc((surface->w * surface->h) << 2);

			if(pix != NULL)
			{
				if(convertbitmap(surface, pix) == true)
				{
					b->width = surface->w;
					b->height = surface->h;
					b->pixdata = (uint8_t*)pix;
					b->opaque = false;

					SDL_FreeSurface(surface);
					return b;
				}

				free(pix);
			}

			free(b);
		}

		SDL_FreeSurface(surface);
	}

	return NULL;
}
