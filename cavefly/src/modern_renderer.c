#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <string.h>

// Maximum length for shader source
#define MAX_SHADER_LENGTH 8192

// Game state structure
typedef struct {
    float position_x;
    float position_y;
    float rotation;
    float thrust;
    float velocity_x;
    float velocity_y;
    float angular_velocity;
    float zoom;
    float time;
    float time_delta;
} GameState;

// Function to read shader source from file
char* read_shader_file(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        printf("Error: Could not open shader file: %s\n", filepath);
        return NULL;
    }
    
    char* shader_source = malloc(MAX_SHADER_LENGTH);
    if (!shader_source) {
        printf("Error: Could not allocate memory for shader source\n");
        fclose(file);
        return NULL;
    }
    
    size_t bytes_read = fread(shader_source, 1, MAX_SHADER_LENGTH - 1, file);
    shader_source[bytes_read] = '\0';
    
    fclose(file);
    return shader_source;
}

// Function to compile a shader
GLuint compile_shader(const char* source, GLenum shader_type) {
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    // Check for compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        printf("Error compiling shader: %s\n", info_log);
        return 0;
    }
    
    return shader;
}

// Function to create a shader program
GLuint create_shader_program(const char* vertex_shader_src, const char* fragment_shader_src) {
    // Compile shaders
    GLuint vertex_shader = compile_shader(vertex_shader_src, GL_VERTEX_SHADER);
    GLuint fragment_shader = compile_shader(fragment_shader_src, GL_FRAGMENT_SHADER);
    
    if (!vertex_shader || !fragment_shader) {
        return 0;
    }
    
    // Create shader program and attach shaders
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    
    // Check for linking errors
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(program, 512, NULL, info_log);
        printf("Error linking shader program: %s\n", info_log);
        return 0;
    }
    
    // Clean up shaders (not needed after program is linked)
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    return program;
}

// Initialize game state
void init_game_state(GameState* state) {
    state->position_x = 0.0f;
    state->position_y = 0.2f;
    state->rotation = 0.0f;
    state->thrust = 0.0f;
    state->velocity_x = 0.0f;
    state->velocity_y = 0.0f;
    state->angular_velocity = 0.0f;
    state->zoom = 4.0f;
    state->time = 0.0f;
    state->time_delta = 0.016f; // ~60 FPS
}

// Create default vertex shader (needed for modern rendering)
const char* default_vertex_shader = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "out vec2 TexCoord;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos, 1.0);\n"
    "    TexCoord = aTexCoord;\n"
    "}\0";

// Render the scene using shaders
void render_shader_scene(GameState* state, GLFWwindow* window) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Create our quad that will fill the screen
    float vertices[] = {
        // positions         // texture coords
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f,  // top right
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,  // bottom left
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f   // top left 
    };
    unsigned int indices[] = { 
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    
    // Create VAO, VBO, EBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Load and compile the fragment shader from file
    char* fragment_shader_src = read_shader_file("shaders/image.glsl");
    if (!fragment_shader_src) {
        // If shader file not found, use a simple fallback shader
        fragment_shader_src = strdup(
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "    FragColor = vec4(0.5, 0.3, 0.2, 1.0);\n"
            "}\0"
        );
    }
    
    // Create a minimal vertex shader for image rendering
    char* vertex_shader_src = strdup(default_vertex_shader);
    
    // Create shader program
    GLuint shader_program = create_shader_program(vertex_shader_src, fragment_shader_src);
    
    if (shader_program) {
        glUseProgram(shader_program);
        
        // Pass uniforms to the shader (simplified version)
        int resolution_loc = glGetUniformLocation(shader_program, "iResolution");
        if (resolution_loc != -1) {
            glUniform3f(resolution_loc, (float)width, (float)height, 0.0f);
        }
        
        int time_loc = glGetUniformLocation(shader_program, "iTime");
        if (time_loc != -1) {
            glUniform1f(time_loc, state->time);
        }
        
        int time_delta_loc = glGetUniformLocation(shader_program, "iTimeDelta");
        if (time_delta_loc != -1) {
            glUniform1f(time_delta_loc, state->time_delta);
        }
        
        // Draw the quad
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    
    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    if (shader_program) glDeleteProgram(shader_program);
    if (fragment_shader_src) free(fragment_shader_src);
    if (vertex_shader_src) free(vertex_shader_src);
}

// Update game time
void update_time(GameState* state, float delta_time) {
    state->time += delta_time;
    state->time_delta = delta_time;
}