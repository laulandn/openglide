#include <SDL/SDL.h>
#include <glide.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
		//freopen ("stdout.txt", "w", stdout);
		//freopen ("stderr.txt", "w", stderr);
    GrVertex v0, v1, v2;
    // Initialize SDL Video
    fprintf(stderr,"sdl1glide2 about to SDL_Init...\n"); fflush(stderr);
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;

    // SDL 1.2 sets a video mode to create a window surface
    //fprintf(stderr,"sdl1glide2 about to SDL_SetVideoMode...\n"); fflush(stderr);
    //SDL_Surface* screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
    //if (!screen) return 1;

    // Initialize Glide
    fprintf(stderr,"sdl1glide2 about to grGlideInit...\n"); fflush(stderr);
    grGlideInit();
    fprintf(stderr,"sdl1glide2 about to grSstWinOpen...\n"); fflush(stderr);
    if (!grSstWinOpen((FxU32)0/*screen*/, GR_RESOLUTION_640x480, GR_REFRESH_60Hz, 
                      GR_COLORFORMAT_ARGB, GR_ORIGIN_LOWER_LEFT, 2, 1)) {
        return 1;
    }

    // 2. Define triangle vertex coordinates (X, Y) and RGB colors (P, R, G, B)
    v0.x = 320.0f; v0.y = 100.0f; v0.oow = 1.0f; // Top vertex (Red)
    v0.r = 255.0f; v0.g = 0.0f;   v0.b = 0.0f;

    v1.x = 520.0f; v1.y = 380.0f; v1.oow = 1.0f; // Bottom-right vertex (Green)
    v1.r = 0.0f;   v1.g = 255.0f; v1.b = 0.0f;

    v2.x = 120.0f; v2.y = 380.0f; v2.oow = 1.0f; // Bottom-left vertex (Blue)
    v2.r = 0.0f;   v2.g = 0.0f;   v2.b = 255.0f;

    // 3. Render loop 
    grBufferClear(0x00000000, 0, 0xFFFF); // Clear screen to black

    // Configure vertex layout for basic RGB rendering
    grColorCombine(GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE, 
                   GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_NONE, FXFALSE);
    
    // Draw the triangle
    grDrawTriangle(&v0, &v1, &v2);

    grBufferSwap(1); // Present the frame to the monitor

    int running = 1;
    SDL_Event event;

    fprintf(stderr,"sdl1glide2 about to enter loop...\n"); fflush(stderr);
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            // SDL 1.2 Mouse Handling
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    printf("Left click at X: %d, Y: %d\n", event.button.x, event.button.y);
                }
            }
        }

        //grBufferClear(0x00000000, 0, 0);
        // Draw Glide geometry here
        //grBufferSwap(1);
    }
    fprintf(stderr,"Done.\n"); fflush(stderr);

    grGlideShutdown();
    SDL_Quit();
    return 0;
}

