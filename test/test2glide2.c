#include <SDL/SDL.h>
#include <glide.h>

void draw_gouraud_quad(void) {
    // 4 Vertices forming a fullscreen/centered quad
    GrVertex v1 = { 160.0f, 120.0f, 0.0f, 255.0f, 0.0f, 0.0f, 1.0f, 255.0f };   // Red
    GrVertex v2 = { 480.0f, 120.0f, 0.0f, 0.0f, 255.0f, 0.0f, 1.0f, 255.0f };   // Green
    GrVertex v3 = { 480.0f, 360.0f, 0.0f, 0.0f, 0.0f, 255.0f, 1.0f, 255.0f };   // Blue
    GrVertex v4 = { 160.0f, 360.0f, 0.0f, 255.0f, 255.0f, 0.0f, 1.0f, 255.0f }; // Yellow

    // Set up Glide to use iterated (Gouraud) vertex colors
    grColorCombine(
        GR_COMBINE_FUNCTION_LOCAL, 
        GR_COMBINE_FACTOR_NONE,
        GR_COMBINE_LOCAL_ITERATED, 
        GR_COMBINE_OTHER_NONE, 
        FXFALSE
    );

    // Glide 2.x doesn't have a native Quad primitive; render via two triangles
    //grBegin();
    grDrawTriangle(&v1, &v2, &v3);
    grDrawTriangle(&v1, &v3, &v4);
    //grEnd();
}


int main(int argc, char* argv[]) {
    grGlideInit();
    grSstSelect(0);
    
    // Set coordinate origin to upper left to match standard screen-space math
    grSstWinOpen(0, GR_RESOLUTION_640x480, GR_REFRESH_60Hz, 
                 GR_COLORFORMAT_ARGB, GR_ORIGIN_UPPER_LEFT, 2, 1);

    float rotation_angle = 0.0f;
    int running = 1000; // Run for 1000 validation frames

    while(running--) {
        // Clear screen to deep black
        grBufferClear(0x00000000, 0, 0);

        // --- Call Test Functions Here ---
        //draw_rotating_cube(rotation_angle);
        draw_gouraud_quad();
        // draw_alpha_test();

        grBufferSwap(1);
        
        rotation_angle += 1.0f;
        if (rotation_angle >= 360.0f) rotation_angle = 0.0f;
    }
    
    running = 1;
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
    return 0;
}
