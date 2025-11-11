#ifndef RENDERER_MODERN_H
#define RENDERER_MODERN_H

#include "spaceglider/types.h"

// Initialize modern renderer (with shaders)
void initialize_modern_renderer();

// Render using modern methods (shaders)
void render_with_modern_renderer(const GameState* game_state, const VehicleState* vehicle_state, const PlanetData* planet_data);

// Clean up modern renderer
void cleanup_modern_renderer();

// Helper functions for shader management
unsigned int create_shader_program(const char* vertex_shader_path, const char* fragment_shader_path);
unsigned int compile_shader(const char* source, int type);
void check_compile_errors(unsigned int shader, int type);

#endif // RENDERER_MODERN_H