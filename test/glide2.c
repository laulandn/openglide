#include <stdio.h>

#include <sdk2_glide.h>


int main(void) {
    GrVertex v0, v1, v2;
    
    // 1. Initialize Glide and the 3dfx Voodoo hardware
    grGlideInit();
    grSstSelect(0);
    if (!grSstWinOpen((FxU)0,GR_RESOLUTION_640x480, GR_REFRESH_60Hz, GR_COLORFORMAT_ABGR, GR_ORIGIN_UPPER_LEFT, 2, 1)) {
        grGlideShutdown();
        return -1;
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

    // 4. Clean up and close device
    grSstWinClose();
    grGlideShutdown();
    return 0;
}

