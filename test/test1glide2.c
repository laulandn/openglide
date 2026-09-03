#include <SDL/SDL.h>
#include <glide.h>

void draw_alpha_test(void) {
    // Opaque background square (Grey)
    // Layout: X, Y, Z, R, G, B, OOZ, A
    GrVertex b1 = { 200.0f, 140.0f, 0.0f, 100.0f, 100.0f, 100.0f, 1.0f, 255.0f };
    GrVertex b2 = { 440.0f, 140.0f, 0.0f, 100.0f, 100.0f, 100.0f, 1.0f, 255.0f };
    GrVertex b3 = { 440.0f, 340.0f, 0.0f, 100.0f, 100.0f, 100.0f, 1.0f, 255.0f };
    GrVertex b4 = { 200.0f, 340.0f, 0.0f, 100.0f, 100.0f, 100.0f, 1.0f, 255.0f };

    // Translucent foreground triangle (Cyan with ~50% Alpha)
    // Layout: X, Y, Z, R, G, B, OOZ, A
    GrVertex t1 = { 320.0f, 100.0f, 0.0f,   0.0f, 255.0f, 255.0f, 1.0f, 128.0f };
    GrVertex t2 = { 500.0f, 380.0f, 0.0f,   0.0f, 255.0f, 255.0f, 1.0f, 128.0f };
    GrVertex t3 = { 140.0f, 380.0f, 0.0f,   0.0f, 255.0f, 255.0f, 1.0f, 128.0f };

    // 1. Draw opaque background (Disable Alpha Blending)
    grAlphaBlendFunction(GR_BLEND_ONE, GR_BLEND_ZERO, GR_BLEND_ONE, GR_BLEND_ZERO);
    grColorCombine(GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE, GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_NONE, FXFALSE);
    
    //grBegin();
    grDrawTriangle(&b1, &b2, &b3);
    grDrawTriangle(&b1, &b3, &b4);
    //grEnd();

    // 2. Enable Alpha Blending (Source Alpha / 1 - Source Alpha)
    grAlphaBlendFunction(
        GR_BLEND_SRC_ALPHA,           // RGB Source factor
        GR_BLEND_ONE_MINUS_SRC_ALPHA, // RGB Dest factor
        GR_BLEND_ONE,                 // Alpha Source factor
        GR_BLEND_ZERO                 // Alpha Dest factor
    );

    // Tell the pipeline to process both iterated colors and iterated alpha channels
    grAlphaCombine(GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE, GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_NONE, FXFALSE);

    //grBegin();
    grDrawTriangle(&t1, &t2, &t3);
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
        // draw_gouraud_quad();
        draw_alpha_test();

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
