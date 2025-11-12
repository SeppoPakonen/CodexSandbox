#define _GNU_SOURCE
#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// GLFW callbacks
static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

Renderer* init_renderer(RendererType type, int width, int height, const char* title) {
    Renderer* renderer = (Renderer*)malloc(sizeof(Renderer));
    if (!renderer) return NULL;
    
    renderer->type = type;
    renderer->width = width;
    renderer->height = height;
    
    if (type == RENDERER_SOFTWARE) {
        // Initialize software renderer
        renderer->frame_buffer = (unsigned int*)malloc(width * height * sizeof(unsigned int));
        renderer->depth_buffer = (float*)malloc(width * height * sizeof(float));
        if (!renderer->frame_buffer || !renderer->depth_buffer) {
            free(renderer->frame_buffer);
            free(renderer->depth_buffer);
            free(renderer);
            return NULL;
        }
        
        // Initialize depth buffer to far plane
        for (int i = 0; i < width * height; i++) {
            renderer->depth_buffer[i] = 1.0f;
        }
        
        // Create a dummy window for event handling
        glfwSetErrorCallback(error_callback);
        if (!glfwInit()) {
            free(renderer->frame_buffer);
            free(renderer->depth_buffer);
            free(renderer);
            return NULL;
        }
        
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        GLFWwindow* dummy_window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (!dummy_window) {
            glfwTerminate();
            free(renderer->frame_buffer);
            free(renderer->depth_buffer);
            free(renderer);
            return NULL;
        }
        
        // Make the dummy context current then release it
        glfwMakeContextCurrent(dummy_window);
        glfwSwapInterval(1);
        glfwMakeContextCurrent(NULL);
        glfwDestroyWindow(dummy_window);
        
        renderer->window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (!renderer->window) {
            glfwTerminate();
            free(renderer->frame_buffer);
            free(renderer->depth_buffer);
            free(renderer);
            return NULL;
        }
        
        glfwMakeContextCurrent(renderer->window);
        glfwSetFramebufferSizeCallback(renderer->window, framebuffer_size_callback);
        
    } else {  // OpenGL renderer
        glfwSetErrorCallback(error_callback);
        if (!glfwInit()) {
            free(renderer);
            return NULL;
        }
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        renderer->window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (!renderer->window) {
            glfwTerminate();
            free(renderer);
            return NULL;
        }
        
        glfwMakeContextCurrent(renderer->window);
        glfwSwapInterval(1);
        glfwSetFramebufferSizeCallback(renderer->window, framebuffer_size_callback);
    }
    
    return renderer;
}

void cleanup_renderer(Renderer* renderer) {
    if (!renderer) return;
    
    if (renderer->type == RENDERER_SOFTWARE) {
        free(renderer->frame_buffer);
        free(renderer->depth_buffer);
    }
    
    if (renderer->window) {
        glfwDestroyWindow(renderer->window);
    }
    
    glfwTerminate();
    free(renderer);
}

void start_frame(Renderer* renderer) {
    if (renderer->type == RENDERER_SOFTWARE) {
        // Clear software frame buffer
        memset(renderer->frame_buffer, 0, renderer->width * renderer->height * sizeof(unsigned int));
        // Clear depth buffer to far plane
        for (int i = 0; i < renderer->width * renderer->height; i++) {
            renderer->depth_buffer[i] = 1.0f;
        }
    } else {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void end_frame(Renderer* renderer) {
    if (renderer->type == RENDERER_SOFTWARE) {
        // Blit software frame buffer to screen
        glClear(GL_COLOR_BUFFER_BIT);
        glRasterPos2f(-1, -1);
        glDrawPixels(renderer->width, renderer->height, GL_RGBA, GL_UNSIGNED_BYTE, renderer->frame_buffer);
        glfwSwapBuffers(renderer->window);
    } else {
        glfwSwapBuffers(renderer->window);
    }
    
    glfwPollEvents();
}

void clear_color_buffer(Renderer* renderer, float r, float g, float b) {
    if (renderer->type == RENDERER_SOFTWARE) {
        unsigned int color = ((int)(b * 255) << 16) | 
                            ((int)(g * 255) << 8) | 
                            (int)(r * 255) |
                            (0xFF << 24);
        for (int i = 0; i < renderer->width * renderer->height; i++) {
            renderer->frame_buffer[i] = color;
        }
    } else {
        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void clear_depth_buffer(Renderer* renderer) {
    if (renderer->type == RENDERER_SOFTWARE) {
        for (int i = 0; i < renderer->width * renderer->height; i++) {
            renderer->depth_buffer[i] = 1.0f;
        }
    } else {
        glClear(GL_DEPTH_BUFFER_BIT);
    }
}

void draw_point(Renderer* renderer, int x, int y, float z, float r, float g, float b) {
    if (x < 0 || x >= renderer->width || y < 0 || y >= renderer->height) return;
    
    if (renderer->type == RENDERER_SOFTWARE) {
        // Check depth
        int idx = y * renderer->width + x;
        if (z < renderer->depth_buffer[idx]) {
            renderer->depth_buffer[idx] = z;
            unsigned int color = ((int)(b * 255) << 16) | 
                                ((int)(g * 255) << 8) | 
                                (int)(r * 255) |
                                (0xFF << 24);
            renderer->frame_buffer[idx] = color;
        }
    } else {
        glBegin(GL_POINTS);
        glColor3f(r, g, b);
        glVertex3f((float)x / renderer->width * 2.0f - 1.0f, 
                   (float)y / renderer->height * 2.0f - 1.0f, 
                   -z);
        glEnd();
    }
}

void draw_line(Renderer* renderer, Vec3 p0, Vec3 p1, float r, float g, float b) {
    if (renderer->type == RENDERER_SOFTWARE) {
        // Bresenham's line algorithm implementation for 3D with depth
        int x0 = (int)((p0.x + 1.0f) * 0.5f * renderer->width);
        int y0 = (int)((p0.y + 1.0f) * 0.5f * renderer->height);
        int x1 = (int)((p1.x + 1.0f) * 0.5f * renderer->width);
        int y1 = (int)((p1.y + 1.0f) * 0.5f * renderer->height);
        
        int dx = abs(x1 - x0);
        int dy = abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;
        
        while (1) {
            // Calculate depth interpolation
            float t = (dx > dy) ? 
                     (float)(abs(x0 - x0)) / dx : 
                     (float)(abs(y0 - y0)) / dy;
            float z = p0.z + (p1.z - p0.z) * t;
            
            draw_point(renderer, x0, y0, z, r, g, b);
            
            if (x0 == x1 && y0 == y1) break;
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x0 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y0 += sy;
            }
        }
    } else {
        glBegin(GL_LINES);
        glColor3f(r, g, b);
        glVertex3f(p0.x, p0.y, p0.z);
        glVertex3f(p1.x, p1.y, p1.z);
        glEnd();
    }
}

void draw_triangle(Renderer* renderer, Vec3 p0, Vec3 p1, Vec3 p2, float r, float g, float b) {
    if (renderer->type == RENDERER_SOFTWARE) {
        // Simple triangle rasterization with depth buffering
        // Calculate bounding box
        int min_x = (int)fmin(fmin(p0.x, p1.x), p2.x);
        int max_x = (int)fmax(fmax(p0.x, p1.x), p2.x);
        int min_y = (int)fmin(fmin(p0.y, p1.y), p2.y);
        int max_y = (int)fmax(fmax(p0.y, p1.y), p2.y);
        
        // Clamp to screen bounds
        min_x = fmax(0, min_x);
        max_x = fmin(renderer->width - 1, max_x);
        min_y = fmax(0, min_y);
        max_y = fmin(renderer->height - 1, max_y);
        
        // Convert vertices to screen space
        int x0 = (int)((p0.x + 1.0f) * 0.5f * renderer->width);
        int y0 = (int)((p0.y + 1.0f) * 0.5f * renderer->height);
        int x1 = (int)((p1.x + 1.0f) * 0.5f * renderer->width);
        int y1 = (int)((p1.y + 1.0f) * 0.5f * renderer->height);
        int x2 = (int)((p2.x + 1.0f) * 0.5f * renderer->width);
        int y2 = (int)((p2.y + 1.0f) * 0.5f * renderer->height);
        
        // Barycentric coordinates for depth interpolation
        int det = (y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2);
        
        for (int y = min_y; y <= max_y; y++) {
            for (int x = min_x; x <= max_x; x++) {
                int w0 = ((y1 - y2) * (x - x2) + (x2 - x1) * (y - y2)) / det;
                int w1 = ((y2 - y0) * (x - x2) + (x0 - x2) * (y - y2)) / det;
                int w2 = 1 - w0 - w1;
                
                if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                    // Interpolate depth
                    float z = p0.z * w0 + p1.z * w1 + p2.z * w2;
                    draw_point(renderer, x, y, z, r, g, b);
                }
            }
        }
    } else {
        glBegin(GL_TRIANGLES);
        glColor3f(r, g, b);
        glVertex3f(p0.x, p0.y, p0.z);
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
        glEnd();
    }
}

void draw_sphere(Renderer* renderer, Vec3 center, float radius, float r, float g, float b) {
    if (renderer->type == RENDERER_SOFTWARE) {
        // Draw sphere as a collection of points
        for (float phi = 0; phi <= M_PI; phi += 0.1f) {
            for (float theta = 0; theta <= 2.0f * M_PI; theta += 0.1f) {
                Vec3 point;
                point.x = center.x + radius * sin(phi) * cos(theta);
                point.y = center.y + radius * sin(phi) * sin(theta);
                point.z = center.z + radius * cos(phi);
                
                int x = (int)((point.x + 1.0f) * 0.5f * renderer->width);
                int y = (int)((point.y + 1.0f) * 0.5f * renderer->height);
                float z = point.z; // This is a simplified depth
                
                draw_point(renderer, x, y, z, r, g, b);
            }
        }
    } else {
        // For OpenGL, use a more sophisticated sphere rendering
        glBegin(GL_POINTS);
        glColor3f(r, g, b);
        
        for (float phi = 0; phi <= M_PI; phi += 0.1f) {
            for (float theta = 0; theta <= 2.0f * M_PI; theta += 0.1f) {
                float x = center.x + radius * sin(phi) * cos(theta);
                float y = center.y + radius * sin(phi) * sin(theta);
                float z = center.z + radius * cos(phi);
                
                glVertex3f(x, y, z);
            }
        }
        glEnd();
    }
}

Mat4 create_identity_matrix(void) {
    Mat4 m;
    for (int i = 0; i < 16; i++) {
        m.m[i] = (i % 5 == 0) ? 1.0f : 0.0f;  // Only diagonal elements are 1
    }
    return m;
}

Mat4 create_perspective_matrix(float fov, float aspect, float near, float far) {
    Mat4 m = create_identity_matrix();
    float tan_half_fov = tan(fov / 2.0f);
    
    m.m[0] = 1.0f / (aspect * tan_half_fov);
    m.m[5] = 1.0f / tan_half_fov;
    m.m[10] = -(far + near) / (far - near);
    m.m[14] = -(2.0f * far * near) / (far - near);
    m.m[11] = -1.0f;
    m.m[15] = 0.0f;
    
    return m;
}

Mat4 create_look_at_matrix(Vec3 eye, Vec3 center, Vec3 up) {
    Vec3 f = {center.x - eye.x, center.y - eye.y, center.z - eye.z};
    float f_len = sqrt(f.x * f.x + f.y * f.y + f.z * f.z);
    f.x /= f_len; f.y /= f_len; f.z /= f_len;
    
    Vec3 up_norm = {up.x, up.y, up.z};
    float up_len = sqrt(up_norm.x * up_norm.x + up_norm.y * up_norm.y + up_norm.z * up_norm.z);
    up_norm.x /= up_len; up_norm.y /= up_len; up_norm.z /= up_len;
    
    Vec3 s = {f.y * up_norm.z - f.z * up_norm.y,
              f.z * up_norm.x - f.x * up_norm.z,
              f.x * up_norm.y - f.y * up_norm.x};
    
    Vec3 u = {s.y * f.z - s.z * f.y,
              s.z * f.x - s.x * f.z,
              s.x * f.y - s.y * f.x};
    
    Mat4 m;
    m.m[0] = s.x; m.m[4] = s.y; m.m[8] = s.z; m.m[12] = 0;
    m.m[1] = u.x; m.m[5] = u.y; m.m[9] = u.z; m.m[13] = 0;
    m.m[2] = -f.x; m.m[6] = -f.y; m.m[10] = -f.z; m.m[14] = 0;
    m.m[3] = 0; m.m[7] = 0; m.m[11] = 0; m.m[15] = 1;
    
    return m;
}

Mat4 multiply_matrices(Mat4 a, Mat4 b) {
    Mat4 result;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            float sum = 0.0f;
            for (int i = 0; i < 4; i++) {
                sum += a.m[row + i * 4] * b.m[i + col * 4];
            }
            result.m[row + col * 4] = sum;
        }
    }
    return result;
}