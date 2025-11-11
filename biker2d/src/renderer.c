#include "renderer.h"
#include "physics.h"  // Include physics functions for track rendering
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Function to read a file into a string
char* read_file(const char* filepath) {
    FILE* file = fopen(filepath, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", filepath);
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* buffer = malloc(length + 1);
    if (!buffer) {
        fclose(file);
        fprintf(stderr, "Failed to allocate memory for file: %s\n", filepath);
        return NULL;
    }
    
    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    
    fclose(file);
    return buffer;
}

// Load and compile shader
GLuint load_shader(const char* filepath, GLenum shader_type) {
    char* source = read_file(filepath);
    if (!source) {
        return 0;
    }
    
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, (const GLchar**)&source, NULL);
    glCompileShader(shader);
    
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "Shader compilation failed for %s: %s\n", filepath, info_log);
    }
    
    free(source);
    return shader;
}

// Create shader program from vertex and fragment shaders
GLuint create_shader_program(const char* vertex_path, const char* fragment_path) {
    GLuint vertex_shader = load_shader(vertex_path, GL_VERTEX_SHADER);
    GLuint fragment_shader = load_shader(fragment_path, GL_FRAGMENT_SHADER);
    
    if (!vertex_shader || !fragment_shader) {
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return 0;
    }
    
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(program, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "Shader program linking failed: %s\n", info_log);
        glDeleteProgram(program);
        program = 0;
    }
    
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    return program;
}

// Initialize renderer
void init_renderer(int width, int height, int renderer_mode) {
    // Initialize OpenGL settings
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);
    
    // Enable blending for transparency effects
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    if (renderer_mode == 1) { // Modern renderer with shaders
        // Check if shaders are available
        if (GLEW_VERSION_2_0) {
            printf("OpenGL 2.0+ available, shaders supported\n");
        } else {
            fprintf(stderr, "Warning: OpenGL 2.0+ not available, shaders may not work\n");
        }
    }
}

// Old school renderer without shaders
void render_old_school(GameState* state) {
    // Clear the screen with a sky blue background
    glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set up orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, state->width, 0.0, state->height);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Apply camera transformation (follow the bike)
    glTranslatef(-state->camera_x * 200.0f, -state->camera_y * 100.0f + 100.0f, 0.0f);
    
    // Draw track terrain
    glColor3f(0.25f, 0.25f, 0.1f); // Brownish color for terrain
    glBegin(GL_QUAD_STRIP);
    
    // Draw a simple representation of the track
    for (int x = -50; x < 50; x++) {
        float world_x = x * 0.1f;
        float y1 = -dtrack(world_x, 0.0f) * 100.0f;  // Scale up for visibility from physics.h
        float y2 = -dtrack(world_x + 0.1f, 0.0f) * 100.0f;
        
        glVertex2f(world_x * 200.0f, y1);
        glVertex2f((world_x + 0.1f) * 200.0f, y2);
    }
    glEnd();
    
    // Draw the bike particles (body and wheels)
    for (int i = 0; i < NPARTICLES; i++) {
        float x = state->particles[i][0] * 200.0f;  // Scale position
        float y = state->particles[i][1] * 100.0f;  // Scale position
        
        if (i == PID_BODY) {
            // Draw body as a yellow circle
            glColor3f(1.0f, 1.0f, 0.0f); // Yellow for body
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(x, y);
            for (int j = 0; j <= 16; j++) {
                float angle = 2.0f * 3.14159265359f * j / 16.0f;
                glVertex2f(x + 8.0f * cosf(angle), y + 8.0f * sinf(angle));
            }
            glEnd();
        } else {
            // Draw wheels as gray circles
            glColor3f(0.1f, 0.1f, 0.1f); // Dark gray for wheels
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(x, y);
            for (int j = 0; j <= 16; j++) {
                float angle = 2.0f * 3.14159265359f * j / 16.0f;
                glVertex2f(x + 6.0f * cosf(angle), y + 6.0f * sinf(angle));
            }
            glEnd();
            
            // Draw tire tread
            glColor3f(0.3f, 0.3f, 0.3f);
            glBegin(GL_LINES);
            for (int j = 0; j < 8; j++) {
                float angle = 2.0f * 3.14159265359f * j / 8.0f;
                float inner_x = x + 4.0f * cosf(angle);
                float inner_y = y + 4.0f * sinf(angle);
                float outer_x = x + 6.0f * cosf(angle);
                float outer_y = y + 6.0f * sinf(angle);
                glVertex2f(inner_x, inner_y);
                glVertex2f(outer_x, outer_y);
            }
            glEnd();
        }
    }
    
    // Draw bike frame connecting particles
    glColor3f(0.5f, 0.1f, 0.1f); // Reddish frame
    glBegin(GL_LINES);
    // Body to back wheel
    glVertex2f(state->particles[PID_BODY][0] * 200.0f, state->particles[PID_BODY][1] * 100.0f);
    glVertex2f(state->particles[PID_WHEEL_BACK][0] * 200.0f, state->particles[PID_WHEEL_BACK][1] * 100.0f);
    
    // Body to front wheel
    glVertex2f(state->particles[PID_BODY][0] * 200.0f, state->particles[PID_BODY][1] * 100.0f);
    glVertex2f(state->particles[PID_WHEEL_FRONT][0] * 200.0f, state->particles[PID_WHEEL_FRONT][1] * 100.0f);
    
    // Back wheel to front wheel
    glVertex2f(state->particles[PID_WHEEL_BACK][0] * 200.0f, state->particles[PID_WHEEL_BACK][1] * 100.0f);
    glVertex2f(state->particles[PID_WHEEL_FRONT][0] * 200.0f, state->particles[PID_WHEEL_FRONT][1] * 100.0f);
    glEnd();
    
    // Draw UI (score)
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, state->width, 0.0, state->height);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glColor3f(0.2f, 1.0f, 0.2f); // Green for UI
    // This is a simple representation - in a real implementation, we'd draw text
    char score_str[32];
    sprintf(score_str, "Score: %.0f", state->score);
    // (Text rendering would go here in a complete implementation)
    
    // Restore matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    glMatrixMode(GL_MODELVIEW);
}

// Modern renderer using shaders
void render_modern(GameState* state) {
    // Set up viewport
    glViewport(0, 0, state->width, state->height);
    
    // For a full implementation, we would:
    // 1. Create and use shader programs based on the GLSL files
    // 2. Pass the game state as uniforms to the shaders
    // 3. Render using the same rendering logic as in the Shadertoy
    
    // For now, we'll use a simplified approach, but in practice
    // we would need to implement the exact rendering from the Shadertoy
    
    // Since the original Shadertoy shaders are complex and not directly compatible
    // with a standard OpenGL pipeline, we'll create simplified shader versions
    // based on the original logic
    
    // For now, we'll fall back to old school rendering
    // In a complete implementation, we would need to convert the Shadertoy shaders
    // to standard OpenGL vertex/fragment shaders
    render_old_school(state);
}

void cleanup_renderer() {
    // Clean up any resources if needed
}

void set_viewport(int width, int height) {
    glViewport(0, 0, width, height);
}