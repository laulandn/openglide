#include <sdk2_glide.h>
#include <stddef.h> // For offsetof() macro

// Define a custom, tightly-packed vertex structure
typedef struct {
    float x, y;       // Screen coordinates
    float oow;        // 1/W for depth or perspective correction
    float r, g, b, a; // Color components
} MyVertex;

int main(void) {
    // 1. Initialize Glide 3.x and open context
    grGlideInit();
    grSstSelect(0);
    if (!grSstWinOpen((FxU)NULL,GR_RESOLUTION_640x480, GR_REFRESH_60Hz, GR_COLORFORMAT_ABGR, GR_ORIGIN_UPPER_LEFT, 2, 1)) {
        grGlideShutdown();
        return -1;
    }

    // 2. Configure the Dynamic Vertex Layout
    // You must explicitly tell Glide 3.x where each attribute lives in your custom structure.
    grVertexLayout(GR_PARAM_XY,  offsetof(MyVertex, x),   GR_PARAM_ENABLE);
    grVertexLayout(GR_PARAM_Q,   offsetof(MyVertex, oow), GR_PARAM_ENABLE);
    grVertexLayout(GR_PARAM_PARGB, offsetof(MyVertex, r),   GR_PARAM_ENABLE); 

    // 3. Create a Vertex Buffer Array
    // Three vertices forming a flat-shaded triangle
    MyVertex triangleVertices[3] = {
        // Top vertex (Red)
        { 320.0f, 100.0f, 1.0f, 255.0f, 0.0f,   0.0f,   255.0f },
        // Bottom-right vertex (Green)
        { 520.0f, 380.0f, 1.0f, 0.0f,   255.0f, 0.0f,   255.0f },
        // Bottom-left vertex (Blue)
        { 120.0f, 380.0f, 1.0f, 0.0f,   0.0f,   255.0f, 255.0f }
    };

    // 4. Render loop
    grBufferClear(0x00000000, 0, 0xFFFF); // Clear screen to black

    // Configure the color combiner to read the iterated vertex color parameters
    grColorCombine(GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE, 
                   GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_NONE, FXFALSE);

    // Draw the triangle primitive directly from the continuous memory block
    // GR_TRIANGLES tells Glide to treat every 3 items as a distinct triangle
    grDrawVertexArrayContiguous(GR_TRIANGLES, 3, triangleVertices, sizeof(MyVertex));

    grBufferSwap(1); // Present the frame buffer to the screen

    // 5. Clean up resources
    grSstWinClose();
    grGlideShutdown();
    return 0;
}

