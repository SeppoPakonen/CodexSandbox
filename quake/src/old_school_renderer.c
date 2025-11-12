#include "old_school_renderer.h"
#include <stdlib.h>

// Initialize old-school renderer
OldSchoolRenderer* init_old_school_renderer(int width, int height) {
    OldSchoolRenderer *renderer = malloc(sizeof(OldSchoolRenderer));
    if (!renderer) return NULL;
    
    // Initialize base renderer
    renderer->base.width = width;
    renderer->base.height = height;
    renderer->base.framebuffer = malloc(width * height * 3 * sizeof(float));
    if (!renderer->base.framebuffer) {
        free(renderer);
        return NULL;
    }
    
    // Initialize depth buffer
    renderer->depth_buffer = malloc(width * height * sizeof(unsigned char));
    if (!renderer->depth_buffer) {
        free(renderer->base.framebuffer);
        free(renderer);
        return NULL;
    }
    
    renderer->z_buffer = malloc(width * height * sizeof(float));
    if (!renderer->z_buffer) {
        free(renderer->depth_buffer);
        free(renderer->base.framebuffer);
        free(renderer);
        return NULL;
    }
    
    // Initialize depth buffer to far plane
    for (int i = 0; i < width * height; i++) {
        renderer->z_buffer[i] = 1.0f;  // Far plane
        renderer->depth_buffer[i] = 0xFF;
    }
    
    return renderer;
}

// Clean up old-school renderer
void cleanup_old_school_renderer(OldSchoolRenderer *renderer) {
    if (renderer) {
        if (renderer->base.framebuffer) {
            free(renderer->base.framebuffer);
        }
        if (renderer->depth_buffer) {
            free(renderer->depth_buffer);
        }
        if (renderer->z_buffer) {
            free(renderer->z_buffer);
        }
        free(renderer);
    }
}

// Render a simple scene (placeholder implementation)
void render_old_school(OldSchoolRenderer *renderer, void *game_state) {
    // Clear framebuffer to a dark color (typical for Quake)
    clear_framebuffer((Renderer*)renderer, 0.1f, 0.1f, 0.1f);
    
    // In a real implementation, this would perform software rendering:
    // - Ray casting or ray marching based on the Quake map structure
    // - Texture mapping
    // - Lighting calculations
    
    // For now, just draw a simple pattern as a placeholder
    float *fb = renderer->base.framebuffer;
    int width = renderer->base.width;
    int height = renderer->base.height;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = (y * width + x) * 3;
            
            // Create a simple pattern
            float u = (float)x / width;
            float v = (float)y / height;
            
            fb[idx + 0] = u;      // R
            fb[idx + 1] = v;      // G
            fb[idx + 2] = 0.2f;   // B
        }
    }
    (void)game_state; // Suppress unused parameter warning
}