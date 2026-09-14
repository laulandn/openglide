/*********************************************************************
** Demonstration of programming the 3Dfx Voodoo cards using Glide,  **
**   by Steven Don                                                  **
**                                                                  **
** For questions, feel free to e-mail me.                           **
**                                                                  **
**    shd@earthling.net                                             **
**    http://shd.cjb.net                                            **
**                                                                  **
**********************************************************************
** NOTE: This example only works with Microsoft Visual C++ for Win  **
**       Be sure to add GLIDE2X.LIB to your project when compiling. **
**       Also, the following files should be in your include path:  **
**         3DFX.H                                                   **
**         GLIDE.H                                                  **
**         GLIDESYS.H                                               **
**         GLIDEUTL.H                                               **
**         SST1VID.H                                                **
**       The library and header files come with the 3Dfx Glide SDK, **
**       which can be downloaded from http://www.3dfx.com           **
**                                                                  **
** NOTE: This example only works with Glide version 2.x             **
**********************************************************************
** To quit this program, just press Alt-F4 like with any other Win  **
** application.                                                     **
*********************************************************************/

#include "math.h"
#include "stdio.h"
#ifdef WIN32
#include "windows.h"
#endif
#include "glide.h"

#ifdef WIN32
//Handles to Windows stuff
HINSTANCE   InstanceHandle;
HWND        WindowHandle;
MSG         Message;
#endif


//Keeps track of Voodoo memory, for loading textures
int         MinAddr;

//The brick texture
int         BrickStart;
GrTexInfo   BrickInfo;
//The wood texture
int         WoodStart;
GrTexInfo   WoodInfo;

//Rotational angles for the rotating cube
float       xAngle, yAngle, zAngle;

//Global allocator, set to -1 if the program should quit
int         Done = 0;

#include "GlideInterface.c"
#include "WindowStuff.c"
#include "Rotation.c"

#ifdef WIN32
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
#else
int SDL_main(int argc,char **argv)
#endif
{
#ifdef WIN32
  //Store instance
  InstanceHandle = hInstance;
#endif

  //Initialize window
  InitWindow ();

  //Initialize Glide
  if (!InitGlide ()) return 0;

  //Load the textures
  LoadTexture ("BRICK.RAW", 256, &BrickStart, &BrickInfo);
  LoadTexture ("WOOD.RAW", 256, &WoodStart, &WoodInfo);

  do {
#ifdef WIN32
    //Process Windows messages
    if (PeekMessage(&Message, WindowHandle, 0, 0, PM_REMOVE)) {
      TranslateMessage (&Message);
      DispatchMessage (&Message);
    }
#endif

    //Clear buffers
    grBufferClear (0x00, 0, GR_ZDEPTHVALUE_FARTHEST);

    //This is where you should do the drawing, I'm rotating a cube
    RotateCube ();

    //Swap buffers, synchronizing to 1 refresh (--> 60Hz)
    grBufferSwap (1);
    while (grBufferNumPending () > 0);
  } while (!Done);

#ifdef WIN32
  //Remove window
  DestroyWindow (WindowHandle);
#endif

  //Close the Glide library
  grGlideShutdown ();

#ifdef WIN32
  return Message.wParam;
#else
  return 0;
#endif
}