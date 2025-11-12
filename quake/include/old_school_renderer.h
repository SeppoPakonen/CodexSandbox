#ifndef OLDSCHOOL_RENDERER_H
#define OLDSCHOOL_RENDERER_H

#include "renderer.h"

// Old-school software renderer without shaders
typedef struct {
    Renderer base;
    
    // Additional data for software rendering
    unsigned char *depth_buffer;  // For z-buffering
    float *z_buffer;              // For depth testing
} OldSchoolRenderer;

// Initialize old-school renderer
OldSchoolRenderer* init_old_school_renderer(int width, int height);

// Clean up old-school renderer
void cleanup_old_school_renderer(OldSchoolRenderer *renderer);

// Render a simple scene (placeholder implementation)
void render_old_school(OldSchoolRenderer *renderer, void *game_state);

#endif // OLDSCHOOL_RENDERER_H