#include "renderer.h"
#include <stdlib.h>
#include <string.h>

// Initialize renderer
Renderer* init_renderer(int width, int height) {
    Renderer *renderer = malloc(sizeof(Renderer));
    if (!renderer) return NULL;
    
    renderer->width = width;
    renderer->height = height;
    renderer->framebuffer = malloc(width * height * 3 * sizeof(float));
    if (!renderer->framebuffer) {
        free(renderer);
        return NULL;
    }
    
    return renderer;
}

// Clean up renderer
void cleanup_renderer(Renderer *renderer) {
    if (renderer) {
        if (renderer->framebuffer) {
            free(renderer->framebuffer);
        }
        free(renderer);
    }
}

// Clear framebuffer with a color
void clear_framebuffer(Renderer *renderer, float r, float g, float b) {
    if (!renderer || !renderer->framebuffer) return;
    
    int size = renderer->width * renderer->height;
    float *fb = renderer->framebuffer;
    
    for (int i = 0; i < size; i++) {
        fb[i * 3 + 0] = r;  // R
        fb[i * 3 + 1] = g;  // G
        fb[i * 3 + 2] = b;  // B
    }
}

// Present the rendered frame (stub implementation)
void present_frame(Renderer *renderer) {
    // In a real implementation, this would display the framebuffer to screen
    // For now, it's just a stub
    (void)renderer; // Suppress unused parameter warning
}