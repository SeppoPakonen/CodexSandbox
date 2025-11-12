#ifndef RENDERER_H
#define RENDERER_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    float m[16];  // 4x4 matrix in column-major order
} Mat4;

typedef enum {
    RENDERER_SOFTWARE,
    RENDERER_OPENGL
} RendererType;

typedef struct {
    RendererType type;
    GLFWwindow* window;
    int width, height;
    float* depth_buffer;  // For software renderer
    unsigned int* frame_buffer;  // For software renderer
    GLuint shader_program;  // For OpenGL renderer
} Renderer;

// Function declarations
Renderer* init_renderer(RendererType type, int width, int height, const char* title);
void cleanup_renderer(Renderer* renderer);

// Render loop functions
void start_frame(Renderer* renderer);
void end_frame(Renderer* renderer);

// Rendering functions
void clear_color_buffer(Renderer* renderer, float r, float g, float b);
void clear_depth_buffer(Renderer* renderer);
void draw_point(Renderer* renderer, int x, int y, float z, float r, float g, float b);
void draw_line(Renderer* renderer, Vec3 p0, Vec3 p1, float r, float g, float b);
void draw_triangle(Renderer* renderer, Vec3 p0, Vec3 p1, Vec3 p2, float r, float g, float b);
void draw_sphere(Renderer* renderer, Vec3 center, float radius, float r, float g, float b);

// Matrix operations
Mat4 create_identity_matrix(void);
Mat4 create_perspective_matrix(float fov, float aspect, float near, float far);
Mat4 create_look_at_matrix(Vec3 eye, Vec3 center, Vec3 up);
Mat4 multiply_matrices(Mat4 a, Mat4 b);

#endif