/* 

TestFonts

Test dynamic font loading code

Copyright (C) A. Schiffler, August 2001, GPL

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "SDL.h"

#ifdef WIN32
#include <windows.h>
#include "SDL_gfxPrimitives.h"
#else
#include "SDL/SDL_gfxPrimitives.h"
#endif

#define WIDTH	640
#define HEIGHT	200

void WaitForEvent()
{
	int done;
	SDL_Event event; 

	/* Check for events */
	done = 0;
	while (!done) {
		SDL_PollEvent(&event);
		switch (event.type) {
			 case SDL_KEYDOWN:
			 case SDL_QUIT:
				 done = 1;
				 break;
		}
		SDL_Delay(100);
	}
}

void ClearScreen(SDL_Surface *screen)
{
	int i;
	/* Set the screen to black */
	if ( SDL_LockSurface(screen) == 0 ) {
		Uint32 black;
		Uint8 *pixels;
		black = SDL_MapRGB(screen->format, 0, 0, 0);
		pixels = (Uint8 *)screen->pixels;
		for ( i=0; i<screen->h; ++i ) {
			memset(pixels, black,
				screen->w*screen->format->BytesPerPixel);
			pixels += screen->pitch;
		}
		SDL_UnlockSurface(screen);
	}
}

int FileExists(const char * filename)
{
	FILE *file;
	if (file = fopen(filename, "r"))
	{
		fclose(file);
		return 1;
	}
	return 0;
}

#define NUM_SDLGFX_FONTS	7

void Draw(SDL_Surface *screen)
{
	int i;
	FILE *file;
	char *myfont;
	char *fontfile[NUM_SDLGFX_FONTS] = {
		"default",
		"5x7.fnt",
		"7x13.fnt",
		"7x13B.fnt",
		"7x13O.fnt",
		"9x18.fnt",
		"9x18B.fnt",
	};
	int  fontw[NUM_SDLGFX_FONTS] = {
		8,
		5,
		7,
		7,
		7,
		9,
		9,
	};	
	int  fonth[NUM_SDLGFX_FONTS] = {
		8,
		7,
		13,
		13,
		13,
		18,
		18,
	};	
	int  fontsize[NUM_SDLGFX_FONTS] = {
		1,
		1792,
		3328,
		3328,
		3328,
		9216,
		9216,
	};	
	char mytext[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int x,y;
	char filename[128];

	/* Black screen */
	ClearScreen(screen);

	/* Render all fonts */ 
	y=0; 
	for (i=0; i<NUM_SDLGFX_FONTS; i++) {
		fprintf(stderr,"Drawing Font: size %ix%i, file '%s', %i bytes\n",fontw[i], fonth[i], fontfile[i],fontsize[i]);
		/* Reset line pos */
		x=4;
		/* Allocate memory for font data */
		myfont=(char *)malloc(fontsize[i]);
		if (myfont) {
			if (strcmp(fontfile[i],"default")) {
				/* Load a font data */
				sprintf(filename,"../Fonts/%s",fontfile[i]);
				if (!FileExists(filename))
				{
					sprintf(filename,"..\\Fonts\\%s",fontfile[i]);
					if (!FileExists(filename))
					{
						sprintf(filename,"..\\..\\..\\Fonts\\%s",fontfile[i]);
						if (!FileExists(filename))
						{
							fprintf(stderr,"Cannot find fontfile: %s\n", fontfile[i]);
						}
					}
				}
				file = fopen(filename,"r");
				fread(myfont,fontsize[i],1,file);
				fclose(file);
				/* Set font data and use it */
				gfxPrimitivesSetFont(myfont,fontw[i],fonth[i]);
			}   
			y += fonth[i];
			stringRGBA(screen,x,y,fontfile[i],255,255,255,255);
			x += 128;
			stringRGBA(screen,x,y,mytext,255,255,255,255);
			y += 10;
			/* Clean up font-data */
			free(myfont);
		}
	}

	/* Display by flipping screens */
	SDL_Flip(screen); 
}

int main(int argc, char *argv[])
{
	SDL_Surface *screen;
	Uint8  video_bpp;
	Uint32 videoflags;
	char title[64];

	/* Generate title strings */
	sprintf (title, "TestFonts - v%i.%i.%i", SDL_GFXPRIMITIVES_MAJOR, SDL_GFXPRIMITIVES_MINOR, SDL_GFXPRIMITIVES_MICRO);

	/* Initialize SDL */
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);

	video_bpp = 32;
	videoflags = SDL_SWSURFACE | SDL_SRCALPHA | SDL_RESIZABLE;
	while ( argc > 1 ) {
		--argc;
		if ( strcmp(argv[argc-1], "-bpp") == 0 ) {
			video_bpp = atoi(argv[argc]);
			--argc;
		} else
			if ( strcmp(argv[argc], "-hw") == 0 ) {
				videoflags |= SDL_HWSURFACE;
			} else
				if ( strcmp(argv[argc], "-warp") == 0 ) {
					videoflags |= SDL_HWPALETTE;
				} else
					if ( strcmp(argv[argc], "-fullscreen") == 0 ) {
						videoflags |= SDL_FULLSCREEN;
					} else {
						fprintf(stderr, 
							"Usage: %s [-bpp N] [-warp] [-hw] [-fullscreen]\n",
							argv[0]);
						exit(1);
					}
	}

	/* Force double buffering */
	videoflags |= SDL_DOUBLEBUF;

	/* Set video mode */
	if ( (screen=SDL_SetVideoMode(WIDTH, HEIGHT, video_bpp, videoflags)) == NULL ) {
		fprintf(stderr, "Couldn't set %ix%i %i bpp video mode: %s\n",WIDTH,HEIGHT,video_bpp,SDL_GetError());
		exit(2);
	}

	/* Use alpha blending */
	SDL_SetAlpha(screen, SDL_SRCALPHA, 0);

	/* Set title for window */
	SDL_WM_SetCaption(title,title);

	/* Do all the drawing work */
	Draw(screen);

	WaitForEvent();

	return(0);
}
