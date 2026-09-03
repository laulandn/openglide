#include <stdio.h>
#include <conio.h>
#include <glide.h>

int main(void) {
    GrHwConfiguration hwConfig;
    GrVertex v1, v2, v3;

    // 1. Initialize the Glide subsystem
    grGlideInit();

    // 2. Detect and check for available 3dfx hardware
    if (!grSstQueryBoards(&hwConfig)) {
        printf("Error: No 3dfx Voodoo hardware detected!\n");
        grGlideShutdown();
        return 1;
    }

    // Select the first available Voodoo board (Board 0)
    grSstSelect(0);

    // 3. Open a full-screen graphics window (640x480 resolution, 60Hz refresh rate)
    // Allocates a triple buffer (2 front/back, 1 depth)
    if (!grSstWinOpen(0, GR_RESOLUTION_640x480, GR_REFRESH_60Hz, 
                      GR_COLORFORMAT_ARGB, GR_ORIGIN_LOWER_LEFT, 2, 1)) {
        printf("Error: Failed to open Glide window!\n");
        grGlideShutdown();
        return 1;
    }

    // 4. Setup initial state: Disable texturing, blending, and depth testing
    grColorCombine(GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE,
                   GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_NONE, FXFALSE);

    // 5. Define vertices for a simple triangle
    // Glide uses a flat structure where color components are float values (0.0 to 255.0)
    
    // Top Vertex
    v1.x = 320.0f; v1.y = 380.0f; v1.oow = 1.0f;
    v1.r = 0.0f;   v1.g = 255.0f; v1.b = 0.0f;

    // Bottom Left Vertex
    v2.x = 140.0f; v2.y = 100.0f; v2.oow = 1.0f;
    v2.r = 0.0f;   v2.g = 255.0f; v2.b = 0.0f;

    // Bottom Right Vertex
    v3.x = 500.0f; v3.y = 100.0f; v3.oow = 1.0f;
    v3.r = 0.0f;   v3.g = 255.0f; v3.b = 0.0f;

    // 6. Main rendering loop simulation (draws one frame and waits)
    // Clear the back buffer to a dark blue color (R:0, G:0, B:100)
    grBufferClear(0x00000064, 0, 0);

    // Draw the green triangle
    grDrawTriangle(&v1, &v2, &v3);

    // Swap buffers to display the rendered frame on screen
    grBufferSwap(1);

    // Wait for a DOS keypress before exiting
    printf("Press any key to exit...\n");
    getch();

    // 7. Cleanup and safely close hardware access
    grSstWinClose();
    grGlideShutdown();

    return 0;
}

