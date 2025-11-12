#ifndef MODERN_RENDERER_H
#define MODERN_RENDERER_H

#include "renderer.h"

// Modern renderer using OpenGL shaders
typedef struct {
    Renderer base;
    
    // OpenGL-specific data
    unsigned int shader_program;
    unsigned int vao, vbo;
    unsigned int framebuffer_id;
    
    // Uniform locations
    int resolution_loc;
    int time_loc;
} ModernRenderer;

// Initialize modern renderer
ModernRenderer* init_modern_renderer(int width, int height);

// Clean up modern renderer
void cleanup_modern_renderer(ModernRenderer *renderer);

// Render a scene using the Quake shader
void render_modern(ModernRenderer *renderer, void *game_state);

// Load and compile a shader from file
unsigned int load_shader(const char *filepath, int type);

#endif // MODERN_RENDERER_H