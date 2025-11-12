#include "opengl_renderer.h"
#include "billiard.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Vertex data for a sphere (ball)
float sphere_vertices[] = {
    // Positions          // Normals
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// Generate sphere vertices programmatically for better quality
void generate_sphere_vertices(float* vertices, unsigned int* indices, int* vertex_count, int* index_count, int stacks, int slices) {
    *vertex_count = 0;
    *index_count = 0;
    
    // Generate vertices
    for (int i = 0; i <= stacks; ++i) {
        float phi = PI * i / stacks;  // phi: 0 to PI
        
        for (int j = 0; j <= slices; ++j) {
            float theta = 2.0f * PI * j / slices;  // theta: 0 to 2*PI
            
            float x = cosf(theta) * sinf(phi);
            float y = cosf(phi);
            float z = sinf(theta) * sinf(phi);
            
            // Position
            vertices[(*vertex_count) * 6 + 0] = x * 0.46f; // scale to ball size
            vertices[(*vertex_count) * 6 + 1] = y * 0.46f;
            vertices[(*vertex_count) * 6 + 2] = z * 0.46f;
            
            // Normal
            vertices[(*vertex_count) * 6 + 3] = x;
            vertices[(*vertex_count) * 6 + 4] = y;
            vertices[(*vertex_count) * 6 + 5] = z;
            
            (*vertex_count)++;
        }
    }
    
    // Generate indices
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            int first = i * (slices + 1) + j;
            int second = first + slices + 1;
            
            // First triangle
            indices[*index_count] = first;
            indices[*index_count + 1] = second;
            indices[*index_count + 2] = first + 1;
            *index_count += 3;
            
            // Second triangle
            indices[*index_count] = second;
            indices[*index_count + 1] = second + 1;
            indices[*index_count + 2] = first + 1;
            *index_count += 3;
        }
    }
}

// Initialize OpenGL renderer
OpenGLRenderer* init_opengl_renderer(int width, int height, const char* title) {
    OpenGLRenderer* renderer = (OpenGLRenderer*)malloc(sizeof(OpenGLRenderer));
    if (!renderer) return NULL;
    
    // Initialize GLFW
    if (!glfwInit()) {
        free(renderer);
        return NULL;
    }
    
    // Set OpenGL version to 3.3 (or later)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // Create window
    renderer->window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!renderer->window) {
        glfwTerminate();
        free(renderer);
        return NULL;
    }
    
    glfwMakeContextCurrent(renderer->window);
    
#ifndef __APPLE__
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        glfwDestroyWindow(renderer->window);
        glfwTerminate();
        free(renderer);
        return NULL;
    }
#endif
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Set viewport
    glViewport(0, 0, width, height);
    
    // Generate and bind VAO and VBO for balls
    glGenVertexArrays(1, &renderer->ball_VAO);
    glGenBuffers(1, &renderer->ball_VBO);
    
    glBindVertexArray(renderer->ball_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->ball_VBO);
    
    // Generate sphere vertices
    int max_vertices = 30 * 30 * 6;  // Maximum possible vertices
    int max_indices = 30 * 30 * 6;   // Maximum possible indices
    float* sphere_verts = (float*)malloc(max_vertices * sizeof(float));
    unsigned int* sphere_indices = (unsigned int*)malloc(max_indices * sizeof(unsigned int));
    int vertex_count, index_count;
    
    generate_sphere_vertices(sphere_verts, sphere_indices, &vertex_count, &index_count, 20, 20);
    
    glBufferData(GL_ARRAY_BUFFER, vertex_count * 6 * sizeof(float), sphere_verts, GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    free(sphere_verts);
    free(sphere_indices);
    
    // Create table VAO/VBO/EBO
    glGenVertexArrays(1, &renderer->table_VAO);
    glGenBuffers(1, &renderer->table_VBO);
    glGenBuffers(1, &renderer->table_EBO);
    
    float table_vertices[] = {
        // positions          // normals
        -8.0f, -0.1f, -14.0f,  0.0f, 1.0f, 0.0f,  // 0
         8.0f, -0.1f, -14.0f,  0.0f, 1.0f, 0.0f,  // 1
         8.0f, -0.1f,  14.0f,  0.0f, 1.0f, 0.0f,  // 2
        -8.0f, -0.1f,  14.0f,  0.0f, 1.0f, 0.0f   // 3
    };
    
    unsigned int table_indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    glBindVertexArray(renderer->table_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, renderer->table_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(table_vertices), table_vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->table_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(table_indices), table_indices, GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Create shader program
    renderer->shader_program = create_shader_program(
        "/home/sblo/Dev/CodexSandbox/billiard/shaders/billiard.vert",
        "/home/sblo/Dev/CodexSandbox/billiard/shaders/billiard.frag"
    );
    
    if (!renderer->shader_program) {
        glfwDestroyWindow(renderer->window);
        glfwTerminate();
        free(renderer);
        return NULL;
    }
    
    return renderer;
}

// Free OpenGL renderer
void free_opengl_renderer(OpenGLRenderer* renderer) {
    if (!renderer) return;
    
    if (renderer->shader_program) {
        glDeleteProgram(renderer->shader_program);
    }
    if (renderer->ball_VAO) {
        glDeleteVertexArrays(1, &renderer->ball_VAO);
    }
    if (renderer->ball_VBO) {
        glDeleteBuffers(1, &renderer->ball_VBO);
    }
    if (renderer->table_VAO) {
        glDeleteVertexArrays(1, &renderer->table_VAO);
    }
    if (renderer->table_VBO) {
        glDeleteBuffers(1, &renderer->table_VBO);
    }
    if (renderer->table_EBO) {
        glDeleteBuffers(1, &renderer->table_EBO);
    }
    
    if (renderer->window) {
        glfwDestroyWindow(renderer->window);
    }
    
    glfwTerminate();
    free(renderer);
}

// Load shader from file
unsigned int load_shader_from_file(const char* filepath, GLenum shader_type) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        printf("Failed to open shader file: %s\n", filepath);
        return 0;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Read file content
    char* content = (char*)malloc(length + 1);
    fread(content, 1, length, file);
    content[length] = '\0';
    
    fclose(file);
    
    // Create and compile shader
    unsigned int shader = glCreateShader(shader_type);
    const char* src = content;
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    
    // Check for compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Shader compilation failed: %s\n", infoLog);
        free(content);
        return 0;
    }
    
    free(content);
    return shader;
}

// Create shader program
unsigned int create_shader_program(const char* vertex_path, const char* fragment_path) {
    unsigned int vertex_shader = load_shader_from_file(vertex_path, GL_VERTEX_SHADER);
    if (!vertex_shader) return 0;
    
    unsigned int fragment_shader = load_shader_from_file(fragment_path, GL_FRAGMENT_SHADER);
    if (!fragment_shader) {
        glDeleteShader(vertex_shader);
        return 0;
    }
    
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    
    // Check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        printf("Shader program linking failed: %s\n", infoLog);
        glDeleteProgram(shader_program);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return 0;
    }
    
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    return shader_program;
}

// Render the scene using OpenGL
void render_scene_opengl(OpenGLRenderer* renderer, GameState* state) {
    // Clear the screen
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Use shader program
    glUseProgram(renderer->shader_program);
    
    // Set uniforms (simplified camera setup)
    int modelLoc = glGetUniformLocation(renderer->shader_program, "model");
    int viewLoc = glGetUniformLocation(renderer->shader_program, "view");
    int projectionLoc = glGetUniformLocation(renderer->shader_program, "projection");
    int lightPosLoc = glGetUniformLocation(renderer->shader_program, "lightPos");
    int viewPosLoc = glGetUniformLocation(renderer->shader_program, "viewPos");
    int lightColorLoc = glGetUniformLocation(renderer->shader_program, "lightColor");
    int objectTypeLoc = glGetUniformLocation(renderer->shader_program, "objectType");
    int ballColorLoc = glGetUniformLocation(renderer->shader_program, "ballColor");
    int shininessLoc = glGetUniformLocation(renderer->shader_program, "materialShininess");
    
    // Set up view and projection matrices (simplified)
    float view[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    
    float projection[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    
    // Apply perspective projection
    float aspect = 800.0f / 600.0f;  // Fixed aspect ratio
    float fov = 45.0f * PI / 180.0f;
    float near = 0.1f, far = 100.0f;
    
    float tan_half_fov = tanf(fov / 2.0f);
    projection[0] = 1.0f / (aspect * tan_half_fov);
    projection[5] = 1.0f / tan_half_fov;
    projection[10] = -(far + near) / (far - near);
    projection[11] = -1.0f;
    projection[14] = -(2.0f * far * near) / (far - near);
    
    // Set camera position (simplified)
    float camera_x = 0.0f;
    float camera_y = 15.0f;
    float camera_z = 25.0f;
    
    // Look at origin
    float target_x = 0.0f;
    float target_y = 0.0f;
    float target_z = 0.0f;
    
    // Calculate view matrix
    vec3 forward = normalize_vec3((vec3){target_x - camera_x, target_y - camera_y, target_z - camera_z});
    vec3 right = normalize_vec3(cross_vec3(forward, (vec3){0.0f, 1.0f, 0.0f}));
    vec3 up = normalize_vec3(cross_vec3(right, forward));
    
    view[0] = right.x;    view[1] = up.x;    view[2] = -forward.x;   view[3] = 0.0f;
    view[4] = right.y;    view[5] = up.y;    view[6] = -forward.y;   view[7] = 0.0f;
    view[8] = right.z;    view[9] = up.z;    view[10] = -forward.z;  view[11] = 0.0f;
    view[12] = -dot_vec3(right, (vec3){camera_x, camera_y, camera_z});
    view[13] = -dot_vec3(up, (vec3){camera_x, camera_y, camera_z});
    view[14] = dot_vec3(forward, (vec3){camera_x, camera_y, camera_z});
    view[15] = 1.0f;
    
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection);
    
    // Set light properties
    glUniform3f(lightPosLoc, 10.0f, 10.0f, 10.0f);
    glUniform3f(viewPosLoc, camera_x, camera_y, camera_z);
    glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
    
    // Draw the table
    glUniform1i(objectTypeLoc, 0); // Table object
    glUniform1f(shininessLoc, 32.0f);
    
    float table_model[16] = {1.0f}; // Identity matrix
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, table_model);
    
    glBindVertexArray(renderer->table_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    // Draw the balls
    for (int i = 0; i < NBALL; i++) {
        // Skip if ball is out of bounds
        if (state->balls[i].position.x >= 2.0f * state->hb_len) continue;
        
        glUniform1i(objectTypeLoc, 1); // Ball object
        
        // Set ball color
        float ball_hue = fmodf((float)i / (float)NBALL, 1.0f);
        float ball_sat = 1.0f - 0.3f * fmodf((float)i, 3.0f);
        float ball_val = 1.0f - 0.3f * fmodf((float)i, 2.0f);
        
        if (i == 0) { // Cue ball (white)
            glUniform3f(ballColorLoc, 1.0f, 1.0f, 1.0f);
        } else {
            // Convert HSV to RGB for ball color
            Color ball_color = hsv_to_rgb(ball_hue, ball_sat, ball_val);
            glUniform3f(ballColorLoc, 
                        ball_color.r / 255.0f, 
                        ball_color.g / 255.0f, 
                        ball_color.b / 255.0f);
        }
        
        glUniform1f(shininessLoc, 64.0f);
        
        // Calculate model matrix for this ball
        float ball_model[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            state->balls[i].position.x, 0.0f, state->balls[i].position.y, 1.0f
        };
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, ball_model);
        
        glBindVertexArray(renderer->ball_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 21 * 21 * 6); // Approximate number of vertices
    }
    
    // Draw the cue if visible
    if (state->show_cue) {
        glUniform1i(objectTypeLoc, 2); // Cue object
        glUniform3f(ballColorLoc, 0.8f, 0.6f, 0.2f); // Wood color
        glUniform1f(shininessLoc, 16.0f);
        
        // Calculate cue position based on state
        float cue_length = 2.0f;
        float cue_model[16] = {1.0f};
        
        // Apply rotation and translation for cue
        float angle = state->a_cue;
        cue_model[0] = cosf(angle);   cue_model[2] = sinf(angle);
        cue_model[8] = -sinf(angle);  cue_model[10] = cosf(angle);
        cue_model[12] = 0.0f;         cue_model[14] = -2.55f - state->d_cue;
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, cue_model);
        
        // For simplicity, use the ball VAO to draw a cylinder-like cue
        glBindVertexArray(renderer->ball_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 21 * 21 * 6);
    }
}

// Check if window should close
int should_window_close(OpenGLRenderer* renderer) {
    return glfwWindowShouldClose(renderer->window);
}

// Poll events
void poll_events() {
    glfwPollEvents();
}

// Swap buffers
void swap_buffers(OpenGLRenderer* renderer) {
    glfwSwapBuffers(renderer->window);
}

// Update viewport
void update_viewport(OpenGLRenderer* renderer, int width, int height) {
    glViewport(0, 0, width, height);
}