#ifndef RENDERER_H
#define RENDERER_H

#include "biker2d.h"

// Function prototypes
void init_renderer(int width, int height, int renderer_mode);
void render_old_school(GameState* state);
void render_modern(GameState* state);
void cleanup_renderer();
void set_viewport(int width, int height);

// Shader utilities
GLuint load_shader(const char* filepath, GLenum shader_type);
GLuint create_shader_program(const char* vertex_path, const char* fragment_path);

#endif