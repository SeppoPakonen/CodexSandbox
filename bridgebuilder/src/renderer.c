#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifdef USE_SHADER_RENDERER
const char* vertex_shader_source = 
    "#version 330 core\n"
    "layout (location = 0) in vec2 aPos;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
    "}\0";

const char* fragment_shader_source = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec2 resolution;\n"
    "uniform int editMode;\n"
    "void main() {\n"
    "    vec2 uv = gl_FragCoord.xy / resolution.xy;\n"
    "    uv = uv * 2.0 - 1.0;\n"
    "    uv.x *= resolution.x / resolution.y;\n"
    "    \n"
    "    // Simple background\n"
    "    vec3 color = vec3(0.0, 0.5, 0.8) * (1.0 + uv.y * 0.8);\n"
    "    \n"
    "    FragColor = vec4(color, 1.0);\n"
    "}\0";
#endif

Renderer* init_renderer(int width, int height, bool use_shader_renderer) {
    Renderer* renderer = malloc(sizeof(Renderer));
    if (!renderer) return NULL;
    
    // Initialize GLFW
    if (!glfwInit()) {
        free(renderer);
        return NULL;
    }
    
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // Create window
    renderer->window = glfwCreateWindow(width, height, "Bridge Builder", NULL, NULL);
    if (!renderer->window) {
        glfwTerminate();
        free(renderer);
        return NULL;
    }
    
    glfwMakeContextCurrent(renderer->window);
    glfwSwapInterval(1);  // Enable vsync
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        free(renderer);
        return NULL;
    }
    
    // Store renderer properties
    renderer->width = width;
    renderer->height = height;
    renderer->aspect_ratio = (float)width / (float)height;
    renderer->use_shader_renderer = use_shader_renderer;
    
#ifdef USE_SHADER_RENDERER
    if (use_shader_renderer) {
        // Compile shaders
        int success;
        char info_log[512];
        
        // Vertex shader
        renderer->vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(renderer->vertex_shader, 1, &vertex_shader_source, NULL);
        glCompileShader(renderer->vertex_shader);
        
        // Check for shader compile errors
        glGetShaderiv(renderer->vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(renderer->vertex_shader, 512, NULL, info_log);
            printf("Vertex shader compilation failed: %s\n", info_log);
        }
        
        // Fragment shader
        renderer->fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(renderer->fragment_shader, 1, &fragment_shader_source, NULL);
        glCompileShader(renderer->fragment_shader);
        
        // Check for shader compile errors
        glGetShaderiv(renderer->fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(renderer->fragment_shader, 512, NULL, info_log);
            printf("Fragment shader compilation failed: %s\n", info_log);
        }
        
        // Link shaders
        renderer->shader_program = glCreateProgram();
        glAttachShader(renderer->shader_program, renderer->vertex_shader);
        glAttachShader(renderer->shader_program, renderer->fragment_shader);
        glLinkProgram(renderer->shader_program);
        
        // Check for linking errors
        glGetProgramiv(renderer->shader_program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(renderer->shader_program, 512, NULL, info_log);
            printf("Shader program linking failed: %s\n", info_log);
        }
        
        // Delete shaders as they're linked into our program now
        glDeleteShader(renderer->vertex_shader);
        glDeleteShader(renderer->fragment_shader);
    }
#endif
    
    // Set up vertex data and buffers
    glGenVertexArrays(1, &renderer->vao);
    glGenBuffers(1, &renderer->vbo);
    glBindVertexArray(renderer->vao);
    
    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    
    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Set up OpenGL state
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    return renderer;
}

void cleanup_renderer(Renderer* renderer) {
    if (!renderer) return;
    
    if (renderer->window) {
        glfwDestroyWindow(renderer->window);
    }
    
#ifdef USE_SHADER_RENDERER
    if (renderer->use_shader_renderer) {
        glDeleteProgram(renderer->shader_program);
    }
#endif
    
    glDeleteVertexArrays(1, &renderer->vao);
    glDeleteBuffers(1, &renderer->vbo);
    
    glfwTerminate();
    free(renderer);
}

void update_window_size(Renderer* renderer, int width, int height) {
    renderer->width = width;
    renderer->height = height;
    renderer->aspect_ratio = (float)width / (float)height;
    
    glViewport(0, 0, width, height);
}

void set_viewport(Renderer* renderer) {
    glViewport(0, 0, renderer->width, renderer->height);
}

void draw_line_software(float x1, float y1, float x2, float y2, float r, float g, float b) {
    // In a real software renderer, you would implement Bresenham's algorithm or similar
    // For now, this is a placeholder that just prints the line
    printf("Line from (%.3f, %.3f) to (%.3f, %.3f) with color (%.3f, %.3f, %.3f)\n", 
           x1, y1, x2, y2, r, g, b);
}

void draw_circle_software(float x, float y, float radius, float r, float g, float b) {
    // In a real software renderer, you would implement midpoint circle algorithm or similar
    // For now, this is a placeholder that just prints the circle
    printf("Circle at (%.3f, %.3f) with radius %.3f and color (%.3f, %.3f, %.3f)\n", 
           x, y, radius, r, g, b);
}

void render_background_software(PhysicsWorld* world) {
    // Render a simple background in software mode
    // This would typically involve setting up a framebuffer and drawing pixels directly
    printf("Rendering background in software mode\n");
}

void render_frame(Renderer* renderer, PhysicsWorld* world) {
    // Clear the screen
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (renderer->use_shader_renderer) {
        glUseProgram(renderer->shader_program);
        
        // Pass resolution and edit mode to shader
        int resolution_loc = glGetUniformLocation(renderer->shader_program, "resolution");
        int edit_mode_loc = glGetUniformLocation(renderer->shader_program, "editMode");
        
        if (resolution_loc != -1) {
            glUniform2f(resolution_loc, (float)renderer->width, (float)renderer->height);
        }
        
        if (edit_mode_loc != -1) {
            glUniform1i(edit_mode_loc, world->edit_mode ? 1 : 0);
        }
        
        // Render using shader - full screen quad
        glBindVertexArray(renderer->vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);  // Draw a full-screen quad
    } else {
        // Software rendering fallback
        render_background_software(world);
        
        // Render nodes
        glPointSize(5.0f);
        glBegin(GL_POINTS);
        for (int i = 0; i < world->node_count; i++) {
            if (world->nodes[i].pinned) {
                glColor3f(0.0f, 0.0f, 0.0f);  // Black for pinned nodes
            } else {
                glColor3f(1.0f, 0.0f, 0.0f);  // Red for free nodes
            }
            glVertex2f(world->nodes[i].pos.x, world->nodes[i].pos.y);
        }
        glEnd();
        
        // Render segments
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        if (world->edit_mode) {
            glColor3f(1.0f, 1.0f, 0.0f);  // Yellow for segments in edit mode
        } else {
            glColor3f(0.0f, 1.0f, 0.0f);  // Green for segments in sim mode
        }
        for (int i = 0; i < world->segment_count; i++) {
            int node_a = world->segments[i].node_a_idx;
            int node_b = world->segments[i].node_b_idx;
            
            glVertex2f(world->nodes[node_a].pos.x, world->nodes[node_a].pos.y);
            glVertex2f(world->nodes[node_b].pos.x, world->nodes[node_b].pos.y);
        }
        glEnd();
    }
    
    // Swap buffers
    glfwSwapBuffers(renderer->window);
}