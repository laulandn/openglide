//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*     SDL specific functions for handling display window
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originaly made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//*               Linux version by Simon White
//**************************************************************
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef C_USE_SDL

#include <stdlib.h>
#include <math.h>

#include "SDL.h"
#include "SDL_opengl.h"

#include "GlOgl.h"

#include "platform/window.h"

static struct
{
    Uint16 red[ 256 ];
    Uint16 green[ 256 ];
    Uint16 blue[ 256 ];
} old_ramp;

static bool ramp_stored  = false;
static bool wasInit      = false;

bool InitialiseOpenGLWindow(FxU wnd, int x, int y, int width, int height)
{
    bool FullScreen = UserConfig.InitFullScreen;
    fprintf(stderr,"About to SDL_WasInit...\n"); fflush(stderr);
    wasInit = SDL_WasInit(SDL_INIT_VIDEO)!=0;
    if(!wasInit)
    {
        bool err = false;
        char *oldWindowId = 0;
        char windowId[40];

    fprintf(stderr,"Checking wnd...\n"); fflush(stderr);

        if (wnd)
        {   // Set SDL window ID
            sprintf (windowId, "SDL_WINDOWID=%ld", (long)wnd);
            oldWindowId = getenv("SDL_WINDOWID");
            if (oldWindowId)
                oldWindowId = strdup(oldWindowId);
            putenv(windowId);
        }

    fprintf(stderr,"About to SDL_Init...\n"); fflush(stderr);
        if (SDL_Init(SDL_INIT_VIDEO))
        {
            GlideMsg("Can't init SDL %s",SDL_GetError());
            err = true;
        }

    fprintf(stderr,"Checking wnd again...\n"); fflush(stderr);
        if (wnd)
        {   // Restore old value
            if (!oldWindowId)
                putenv("SDL_WINDOWID");
            else
            {
                sprintf (windowId, "SDL_WINDOWID=%s", oldWindowId);
                putenv(windowId);
                free (oldWindowId);
            }
        }

        if (err)
            return false;
    } else {
     fprintf(stderr,"Going to SDL_GetVideoSurface...\n"); fflush(stderr);
       SDL_Surface* tmpSurface = SDL_GetVideoSurface();
        if (tmpSurface)
        {
            // Preserve window/fullscreen mode in SDL apps and override config file entry
           (tmpSurface->flags&SDL_FULLSCREEN) ? (FullScreen = true) : (FullScreen = false);
        }
    } 

  fprintf(stderr,"Going to SDL_GL_SetAttribute...\n"); fflush(stderr);
	/* Initialize the display */
	int w = 640;
	int h = 480;
	bool fsaa=false;
	bool accel=false;
	bool sync=false;
	int bpp=0;
	int video_flags=FullScreen ? SDL_OPENGL|SDL_FULLSCREEN : SDL_OPENGL;
	int rgb_size[3];
	
		/* See if we should detect the display depth */
	if ( bpp == 0 ) {
		if ( SDL_GetVideoInfo()->vfmt->BitsPerPixel <= 8 ) {
			bpp = 8;
		} else {
			bpp = 16;  /* More doesn't seem to work */
		}
	}

  fprintf(stderr,"Settings are: %d,%d,%d,%d,%d\n",(int)fsaa,(int)accel,(int)sync,bpp,video_flags); fflush(stderr);
	switch (bpp) {
	    case 8:
		rgb_size[0] = 3;
		rgb_size[1] = 3;
		rgb_size[2] = 2;
		break;
	    case 15:
	    case 16:
		rgb_size[0] = 5;
		rgb_size[1] = 5;
		rgb_size[2] = 5;
		break;
            default:
		rgb_size[0] = 8;
		rgb_size[1] = 8;
		rgb_size[2] = 8;
		break;
	}
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, rgb_size[0] );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, rgb_size[1] );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, rgb_size[2] );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	if ( fsaa ) {
		SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
		SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, fsaa );
	}
	if ( accel ) {
		SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
	}
	if ( sync ) {
		SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 1 );
	} else {
		SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 0 );
	}

    fprintf(stderr,"Going to SDL_SetVideoMode...\n"); fflush(stderr);
    if(SDL_SetVideoMode(w, h, bpp, video_flags ) == 0)
    {
     fprintf(stderr,"Video mode set failed...\n"); fflush(stderr);
        GlideMsg("Video mode set failed: %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
        return false;
    }

    fprintf(stderr,"Going to SDL_GL_GetAttribute...\n"); fflush(stderr);
    SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &height);
    if ( height > 16 ) {
        UserConfig.PrecisionFix = false;
    }

    fprintf(stderr,"Going to SDL_GetGammaRamp...\n"); fflush(stderr);
    if(SDL_GetGammaRamp(old_ramp.red, old_ramp.green, old_ramp.blue) != -1)
        ramp_stored = true;
        
    fprintf(stderr,"done.\n"); fflush(stderr);
    return true;
}

void FinaliseOpenGLWindow( void)
{
    if ( ramp_stored )
        SDL_SetGammaRamp(old_ramp.red, old_ramp.green, old_ramp.blue);
    if (!wasInit)
        SDL_Quit();
}

void SetGamma(float value)
{
    struct
    {
        Uint16 red[256];
        Uint16 green[256];
        Uint16 blue[256];
    } ramp;
    int i;

    for ( i = 0; i < 256; i++ )
    {
        Uint16 v = (Uint16)( 0xffff * pow( i / 255.0, 1.0 / value ) );

        ramp.red[ i ] = ramp.green[ i ] = ramp.blue[ i ] = ( v & 0xff00 );
    }

    SDL_SetGammaRamp(ramp.red, ramp.green, ramp.blue);
}

void RestoreGamma()
{
}

bool SetScreenMode(int &xsize, int &ysize)
{
    return true;
}

void ResetScreenMode()
{
}

void SwapBuffers()
{
    SDL_GL_SwapBuffers();
}

#endif // C_USE_SDL
