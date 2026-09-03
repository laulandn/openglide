#include <SDL2/SDL.h>
#include <glide.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;

    // SDL 2 Window Creation
    SDL_Window* window = SDL_CreateWindow("Glide SDL2", SDL_WINDOWPOS_UNDEFINED, 
                                          SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (!window) return 1;

    grGlideInit();
    // Pass the SDL_Window pointer directly to Glide
    if (!grSstWinOpen((FxU32)window, GR_RESOLUTION_640x480, GR_REFRESH_60Hz, 
                      GR_COLORFORMAT_ARGB, GR_ORIGIN_LOWER_LEFT, 2, 1)) {
        return 1;
    }

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            // SDL 2 Mouse Handling
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    printf("Left click at X: %d, Y: %d\n", event.button.x, event.button.y);
                }
            }
        }

        grBufferClear(0x00000000, 0, 0);
        grBufferSwap(1);
    }

    grGlideShutdown();
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

