#include <SDL3/SDL.h>
#include <glide.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    // SDL3 initialization remains simple
    if (!SDL_Init(SDL_INIT_VIDEO)) return 1;

    // SDL3 Window Creation (No window position flags needed)
    SDL_Window* window = SDL_CreateWindow("Glide SDL3", 640, 480, 0);
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
            // SDL3 uses the SDL_EVENT_ prefix
            if (event.type == SDL_EVENT_QUIT) {
                running = 0;
            }
            // SDL3 Mouse Handling
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    // event.button.x and y are now floats in SDL3
                    printf("Left click at X: %.2f, Y: %.2f\n", event.button.x, event.button.y);
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

