#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include "billiard.h"

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

// OpenGL Renderer structure
typedef struct {
    GLFWwindow* window;
    unsigned int VAO, VBO;
    unsigned int shader_program;
    unsigned int ball_VAO, ball_VBO;
    unsigned int table_VAO, table_VBO, table_EBO;
    
    // Shader IDs
    unsigned int vertex_shader;
    unsigned int fragment_shader;
} OpenGLRenderer;

// Initialize OpenGL renderer
OpenGLRenderer* init_opengl_renderer(int width, int height, const char* title);

// Free OpenGL renderer
void free_opengl_renderer(OpenGLRenderer* renderer);

// Load shader from file
unsigned int load_shader_from_file(const char* filepath, GLenum shader_type);

// Create shader program
unsigned int create_shader_program(const char* vertex_path, const char* fragment_path);

// Render the scene using OpenGL
void render_scene_opengl(OpenGLRenderer* renderer, GameState* state);

// Check if window should close
int should_window_close(OpenGLRenderer* renderer);

// Poll events
void poll_events();

// Swap buffers
void swap_buffers(OpenGLRenderer* renderer);

// Update viewport
void update_viewport(OpenGLRenderer* renderer, int width, int height);

#endif // OPENGL_RENDERER_H