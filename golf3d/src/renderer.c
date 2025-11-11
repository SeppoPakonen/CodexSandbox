#include "renderer.h"
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

// Simple old school renderer without shaders
void render_old_school(GameState* state) {
    // Clear the screen
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);  // Sky blue background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Set up perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)state->width/state->height, 0.1, 100.0);
    
    // Set up camera view
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Apply camera transformation
    float target[3] = {state->ball_pos[0], state->ball_pos[1], state->ball_pos[2]};
    float up[3] = {0.0f, 1.0f, 0.0f};
    
    // Calculate camera direction
    float view_dir[3] = {
        target[0] - state->cam_pos[0],
        target[1] - state->cam_pos[1],
        target[2] - state->cam_pos[2]
    };
    
    // Normalize
    float len = sqrtf(view_dir[0]*view_dir[0] + view_dir[1]*view_dir[1] + view_dir[2]*view_dir[2]);
    if (len > 0.0001f) {
        view_dir[0] /= len;
        view_dir[1] /= len;
        view_dir[2] /= len;
    }
    
    // Calculate right vector
    float right[3];
    right[0] = up[1]*view_dir[2] - up[2]*view_dir[1];
    right[1] = up[2]*view_dir[0] - up[0]*view_dir[2];
    right[2] = up[0]*view_dir[1] - up[1]*view_dir[0];
    
    // Normalize right
    len = sqrtf(right[0]*right[0] + right[1]*right[1] + right[2]*right[2]);
    if (len > 0.0001f) {
        right[0] /= len;
        right[1] /= len;
        right[2] /= len;
    }
    
    // Calculate actual up vector
    float actual_up[3];
    actual_up[0] = view_dir[1]*right[2] - view_dir[2]*right[1];
    actual_up[1] = view_dir[2]*right[0] - view_dir[0]*right[2];
    actual_up[2] = view_dir[0]*right[1] - view_dir[1]*right[0];
    
    // Set up the view matrix (simplified)
    gluLookAt(
        state->cam_pos[0], state->cam_pos[1], state->cam_pos[2],
        target[0], target[1], target[2],
        actual_up[0], actual_up[1], actual_up[2]
    );
    
    // Draw terrain
    glColor3f(0.0f, 0.8f, 0.0f);  // Green terrain
    glBegin(GL_QUADS);
    // Draw a simple terrain grid
    float terrain_size = 50.0f;
    glVertex3f(-terrain_size, 0.0f, -terrain_size);
    glVertex3f(terrain_size, 0.0f, -terrain_size);
    glVertex3f(terrain_size, 0.0f, terrain_size);
    glVertex3f(-terrain_size, 0.0f, terrain_size);
    glEnd();
    
    // Draw hole
    glColor3f(0.2f, 0.2f, 0.2f);  // Dark hole
    glPushMatrix();
    glTranslatef(state->hole_pos[0], state->hole_pos[1], state->hole_pos[2]);
    // Draw a simple cylinder for the hole
    int segments = 32;
    glBegin(GL_TRIANGLE_FAN);
    // Center of the hole
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * M_PI * i / segments;
        glVertex3f(cosf(angle) * HOLE_RADIUS, 0.0f, sinf(angle) * HOLE_RADIUS);
    }
    glEnd();
    glPopMatrix();
    
    // Draw flag near hole
    glColor3f(0.8f, 0.0f, 0.0f);  // Red flag
    glPushMatrix();
    glTranslatef(state->hole_pos[0], state->hole_pos[1] + HOLE_RADIUS, state->hole_pos[2]);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.3f, 0.0f);  // Flag pole
    glEnd();
    
    // Draw flag triangle
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f, 0.3f, 0.0f);  // Top of flag pole
    glVertex3f(0.1f, 0.2f, 0.0f);  // Bottom right of flag
    glVertex3f(0.1f, 0.3f, 0.0f);  // Top right of flag
    glEnd();
    glPopMatrix();
    
    // Draw ball
    glColor3f(1.0f, 1.0f, 1.0f);  // White ball
    glPushMatrix();
    glTranslatef(state->ball_pos[0], state->ball_pos[1], state->ball_pos[2]);
    // Draw a simple sphere approximation
    int slices = 16;
    int stacks = 16;
    
    for (int i = 0; i < stacks; i++) {
        float phi1 = M_PI * (-0.5f + (float)i / stacks);
        float phi2 = M_PI * (-0.5f + (float)(i + 1) / stacks);
        
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; j++) {
            float theta = 2.0f * M_PI * j / slices;
            
            float x1 = cosf(phi1) * cosf(theta);
            float y1 = sinf(phi1);
            float z1 = cosf(phi1) * sinf(theta);
            
            float x2 = cosf(phi2) * cosf(theta);
            float y2 = sinf(phi2);
            float z2 = cosf(phi2) * sinf(theta);
            
            glVertex3f(x1 * BALL_RADIUS, y1 * BALL_RADIUS, z1 * BALL_RADIUS);
            glVertex3f(x2 * BALL_RADIUS, y2 * BALL_RADIUS, z2 * BALL_RADIUS);
        }
        glEnd();
    }
    glPopMatrix();
    
    // Draw HUD (heads up display)
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, state->width, 0, state->height, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Disable depth test for HUD
    glDisable(GL_DEPTH_TEST);
    
    // Draw power indicator if the ball is stationary
    if (state->stationary) {
        // Swing position (where to drag from)
        float swing_x = state->width * 0.499f;
        float swing_y = state->height * 0.299f;
        float swing_rad = state->height * 0.03f;
        
        // Draw circle at swing position
        glColor3f(1.0f, 1.0f, 0.0f);  // Yellow circle
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 32; i++) {
            float angle = 2.0f * M_PI * i / 32;
            glVertex2f(
                swing_x + cosf(angle) * swing_rad,
                swing_y + sinf(angle) * swing_rad
            );
        }
        glEnd();
        
        // Draw power indicator (arrow)
        float strength = 0.0f;
        if (state->mouse_drag_start[0] != -1.0f) {
            // Calculate strength based on drag
            float drag_x = state->mouse[0] - state->mouse_drag_start[0];
            float drag_y = state->mouse[1] - state->mouse_drag_start[1];
            strength = fminf(fmaxf((-drag_y)/(0.25f * state->height), 0.0f), 1.0f);
        }
        
        if (strength > 0.0f) {
            float strength_y = strength * state->height * 0.25f;
            
            // Draw dotted line from circle to arrow
            glColor3f(0.0f, 0.0f, 0.0f);
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(1, 0xF0F0);  // Dashed line
            glBegin(GL_LINES);
            glVertex2f(swing_x, swing_y);
            glVertex2f(swing_x, swing_y + strength_y);
            glEnd();
            glDisable(GL_LINE_STIPPLE);
            
            // Draw arrow
            glColor3f(1.0f, 1.0f, 0.0f);
            glBegin(GL_TRIANGLES);
            glVertex2f(swing_x - 5.0f, swing_y + strength_y);
            glVertex2f(swing_x + 5.0f, swing_y + strength_y);
            glVertex2f(swing_x, swing_y + strength_y + 10.0f);
            glEnd();
        }
    }
    
    // Restore matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    glMatrixMode(GL_MODELVIEW);
    
    // Re-enable depth test
    glEnable(GL_DEPTH_TEST);
}

// Modern renderer implementation using shaders
void render_modern(GameState* state) {
    // Set up viewport
    glViewport(0, 0, state->width, state->height);
    
    // In a complete implementation, we would:
    // 1. Create framebuffers for buffer A and B
    // 2. Run the physics shader (Buf A) to update ball position and velocity
    // 3. Run the ray marching shader (Buf B) to render the scene
    // 4. Run the final image shader to add HUD
    
    // For now, we'll use a simplified ray marching approach
    // First, we need to compile and use the appropriate shaders
    
    // Create a shader program for the ray marching renderer
    GLuint ray_march_program = create_shader_program("shaders/buf_a.vert", "shaders/buf_b.frag");
    
    if (ray_march_program != 0) {
        glUseProgram(ray_march_program);
        
        // Pass uniforms to the shader
        glUniform3f(glGetUniformLocation(ray_march_program, "iResolution"), 
                   (float)state->width, (float)state->height, 1.0f);
        glUniform1f(glGetUniformLocation(ray_march_program, "iTime"), state->time);
        glUniform1i(glGetUniformLocation(ray_march_program, "iFrame"), state->frame);
        glUniform4f(glGetUniformLocation(ray_march_program, "iMouse"), 
                   state->mouse[0], state->mouse[1], state->mouse[2], state->mouse[3]);
        
        // We'd need to pass the game state as a texture in a complete implementation
        
        // Render a full-screen quad to execute the fragment shader for each pixel
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
        glEnd();
        
        glUseProgram(0);
        glDeleteProgram(ray_march_program);
    } else {
        // Fallback rendering if shaders fail to compile
        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Set up perspective projection
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, (double)state->width/state->height, 0.1, 100.0);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        // Set up camera
        gluLookAt(
            state->cam_pos[0], state->cam_pos[1], state->cam_pos[2],   // Camera position
            state->ball_pos[0], state->ball_pos[1], state->ball_pos[2], // Look at the ball
            0.0f, 1.0f, 0.0f  // Up vector
        );
        
        // Draw ground
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glColor3f(0.0f, 0.7f, 0.0f); // Green ground
        glVertex3f(-50.0f, 0.0f, -50.0f);
        glVertex3f(50.0f, 0.0f, -50.0f);
        glVertex3f(50.0f, 0.0f, 50.0f);
        glVertex3f(-50.0f, 0.0f, 50.0f);
        glEnd();
        
        // Draw hole
        glColor3f(0.05f, 0.05f, 0.05f); // Dark hole
        glPushMatrix();
        glTranslatef(state->hole_pos[0], state->hole_pos[1], state->hole_pos[2]);
        // Draw sphere manually with OpenGL primitives
        int slices = 16;
        int stacks = 16;
        for (int i = 0; i < stacks; i++) {
            float phi1 = M_PI * (-0.5f + (float)i / stacks);
            float phi2 = M_PI * (-0.5f + (float)(i + 1) / stacks);
            
            glBegin(GL_QUAD_STRIP);
            for (int j = 0; j <= slices; j++) {
                float theta = 2.0f * M_PI * j / slices;
                
                float x1 = cosf(phi1) * cosf(theta);
                float y1 = sinf(phi1);
                float z1 = cosf(phi1) * sinf(theta);
                
                float x2 = cosf(phi2) * cosf(theta);
                float y2 = sinf(phi2);
                float z2 = cosf(phi2) * sinf(theta);
                
                glVertex3f(x1 * HOLE_RADIUS, y1 * HOLE_RADIUS, z1 * HOLE_RADIUS);
                glVertex3f(x2 * HOLE_RADIUS, y2 * HOLE_RADIUS, z2 * HOLE_RADIUS);
            }
            glEnd();
        }
        glPopMatrix();
        
        // Draw ball
        glColor3f(1.0f, 1.0f, 1.0f); // White ball
        glPushMatrix();
        glTranslatef(state->ball_pos[0], state->ball_pos[1], state->ball_pos[2]);
        // Draw sphere manually with OpenGL primitives
        for (int i = 0; i < stacks; i++) {
            float phi1 = M_PI * (-0.5f + (float)i / stacks);
            float phi2 = M_PI * (-0.5f + (float)(i + 1) / stacks);
            
            glBegin(GL_QUAD_STRIP);
            for (int j = 0; j <= slices; j++) {
                float theta = 2.0f * M_PI * j / slices;
                
                float x1 = cosf(phi1) * cosf(theta);
                float y1 = sinf(phi1);
                float z1 = cosf(phi1) * sinf(theta);
                
                float x2 = cosf(phi2) * cosf(theta);
                float y2 = sinf(phi2);
                float z2 = cosf(phi2) * sinf(theta);
                
                glVertex3f(x1 * BALL_RADIUS, y1 * BALL_RADIUS, z1 * BALL_RADIUS);
                glVertex3f(x2 * BALL_RADIUS, y2 * BALL_RADIUS, z2 * BALL_RADIUS);
            }
            glEnd();
        }
        glPopMatrix();
        
        // Draw flag
        glColor3f(0.8f, 0.0f, 0.0f); // Red flag
        glPushMatrix();
        glTranslatef(state->hole_pos[0], state->hole_pos[1] + HOLE_RADIUS, state->hole_pos[2]);
        
        // Draw flag pole
        glBegin(GL_LINES);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.3f, 0.0f);
        glEnd();
        
        glPopMatrix();
    }
    
    // Render HUD (heads up display) using the final shader
    GLuint final_program = create_shader_program("shaders/buf_a.vert", "shaders/final.frag");
    
    if (final_program != 0) {
        glUseProgram(final_program);
        
        // Pass uniforms to the final shader
        glUniform3f(glGetUniformLocation(final_program, "iResolution"), 
                   (float)state->width, (float)state->height, 1.0f);
        glUniform1f(glGetUniformLocation(final_program, "iTime"), state->time);
        glUniform1i(glGetUniformLocation(final_program, "iFrame"), state->frame);
        glUniform4f(glGetUniformLocation(final_program, "iMouse"), 
                   state->mouse[0], state->mouse[1], state->mouse[2], state->mouse[3]);
        
        // Render the HUD overlay
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
        glEnd();
        
        glUseProgram(0);
        glDeleteProgram(final_program);
    } else {
        // Draw power indicator as fallback
        if (state->stationary) {
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            glOrtho(0, state->width, 0, state->height, -1, 1);
            
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glDisable(GL_DEPTH_TEST);
            
            // Draw power indicator
            float swing_x = state->width * 0.499f;
            float swing_y = state->height * 0.299f;
            float swing_rad = state->height * 0.03f;
            
            glColor3f(1.0f, 1.0f, 0.0f); // Yellow
            glBegin(GL_LINE_LOOP);
            for (int i = 0; i < 32; i++) {
                float angle = 2.0f * M_PI * i / 32;
                glVertex2f(
                    swing_x + cosf(angle) * swing_rad,
                    swing_y + sinf(angle) * swing_rad
                );
            }
            glEnd();
            
            glPopMatrix();
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(45.0, (double)state->width/state->height, 0.1, 100.0);
            glMatrixMode(GL_MODELVIEW);
            glEnable(GL_DEPTH_TEST);
        }
    }
}

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

void cleanup_renderer() {
    // Clean up any resources if needed
}

void set_viewport(int width, int height) {
    glViewport(0, 0, width, height);
}