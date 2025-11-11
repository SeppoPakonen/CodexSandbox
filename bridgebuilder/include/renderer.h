#ifndef RENDERER_H
#define RENDERER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "physics.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct {
    GLFWwindow* window;
    int width;
    int height;
    float aspect_ratio;
    bool use_shader_renderer;
    
    // Shader handles (only if using shader renderer)
    GLuint shader_program;
    GLuint vertex_shader;
    GLuint fragment_shader;
    
    // VAO and VBO for rendering
    GLuint vao;
    GLuint vbo;
    
    // Texture handles
    GLuint background_texture;
    GLuint font_texture;
} Renderer;

// Function declarations
Renderer* init_renderer(int width, int height, bool use_shader_renderer);
void cleanup_renderer(Renderer* renderer);
void render_frame(Renderer* renderer, PhysicsWorld* world);
void update_window_size(Renderer* renderer, int width, int height);
void set_viewport(Renderer* renderer);

// Software rendering functions
void draw_line_software(float x1, float y1, float x2, float y2, float r, float g, float b);
void draw_circle_software(float x, float y, float radius, float r, float g, float b);
void render_background_software(PhysicsWorld* world);

#ifdef __cplusplus
}
#endif

#endif