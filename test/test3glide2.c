#include <SDL/SDL.h>
#include <glide.h>
#include <math.h>


#define PI 3.14159265f

// Define the 8 vertices of a standard 3D cube [8 vertices][3 coordinates]
typedef struct { float x, y, z; } Point3D;
Point3D cube_vertices[8] = {
    {-1.0f, -1.0f, -1.0f}, { 1.0f, -1.0f, -1.0f}, { 1.0f,  1.0f, -1.0f}, {-1.0f,  1.0f, -1.0f},
    {-1.0f, -1.0f,  1.0f}, { 1.0f, -1.0f,  1.0f}, { 1.0f,  1.0f,  1.0f}, {-1.0f,  1.0f,  1.0f}
};

// 12 edges connecting the vertex indices [12 lines][2 indices per line]
int cube_edges[12][2] = {
    {0,1}, {1,2}, {2,3}, {3,0}, // Back face
    {4,5}, {5,6}, {6,7}, {7,4}, // Front face
    {0,4}, {1,5}, {2,6}, {3,7}  // Connecting edges
};

void draw_rotating_cube(float angle) {
    GrVertex v[8]; // FIX: Correctly allocated array size to prevent memory stack overflow
    float rad = angle * PI / 180.0f;
    float cosA = cosf(rad);
    float sinA = sinf(rad);

    // Project and transform vertices
    for (int i = 0; i < 8; i++) {
        // Rotate around Y and X axes
        float x1 = cube_vertices[i].x * cosA - cube_vertices[i].z * sinA;
        float z1 = cube_vertices[i].x * sinA + cube_vertices[i].z * cosA;
        float y1 = cube_vertices[i].y * cosA - z1 * sinA;
        float z2 = cube_vertices[i].y * sinA + z1 * cosA;

        // Simple perspective projection + scale to screen coordinates
        // Assuming 640x480 resolution
        float distance = 3.5f; 
        float zoom = 300.0f;
        
        // Map explicitly to individual sequential struct variables
        v[i].x = (x1 / (z2 + distance)) * zoom + 320.0f;
        v[i].y = (y1 / (z2 + distance)) * zoom + 240.0f;
        v[i].z = 0.0f;
        
        // Color elements
        v[i].r = 255.0f; 
        v[i].g = 255.0f; 
        v[i].b = 255.0f; 
        v[i].ooz = 1.0f;  // Safe depth value
        v[i].a = 255.0f;
    }

    // Set constant color rendering mode for lines
    grColorCombine(GR_COMBINE_FUNCTION_LOCAL, GR_COMBINE_FACTOR_NONE,
                   GR_COMBINE_LOCAL_ITERATED, GR_COMBINE_OTHER_NONE, FXFALSE);

    // Draw the 12 lines
    for (int i = 0; i < 12; i++) {
        // FIX: Extract indices from edge matrix to target the correct vertex pointers
        int index1 = cube_edges[i][0];
        int index2 = cube_edges[i][1];
        grDrawLine(&v[index1], &v[index2]);
    }
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
        draw_rotating_cube(rotation_angle);
        // draw_gouraud_quad();
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
