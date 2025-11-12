#include "renderer_modern.h"
#include "spaceglider/types.h"
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Shader program ID
static unsigned int shaderProgram = 0;

// Function to read shader source from file
char* read_shader_file(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Failed to open shader file: %s\n", filepath);
        return NULL;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory and read file
    char* buffer = malloc(length + 1);
    if (buffer) {
        fread(buffer, 1, length, file);
        buffer[length] = '\0';
    }

    fclose(file);
    return buffer;
}

unsigned int compile_shader(const char* source, int type) {
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);

    // Check for compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        printf("Shader compilation failed: %s\n", infoLog);
    }

    return id;
}

unsigned int create_shader_program(const char* vertex_shader_path, const char* fragment_shader_path) {
    // Read shader files
    char* vertexSource = read_shader_file(vertex_shader_path);
    char* fragmentSource = read_shader_file(fragment_shader_path);

    if (!vertexSource || !fragmentSource) {
        if (vertexSource) free(vertexSource);
        if (fragmentSource) free(fragmentSource);
        return 0;
    }

    // Compile shaders
    unsigned int vertexShader = compile_shader(vertexSource, GL_VERTEX_SHADER);
    unsigned int fragmentShader = compile_shader(fragmentSource, GL_FRAGMENT_SHADER);

    // Create shader program
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        printf("Shader program linking failed: %s\n", infoLog);
    }

    // Clean up
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    free(vertexSource);
    free(fragmentSource);

    return program;
}

void initialize_modern_renderer() {
    // Create and compile shader program
    shaderProgram = create_shader_program(
        "shaders/vertex.glsl",
        "shaders/fragment.glsl"
    );

    if (shaderProgram == 0) {
        printf("Failed to create shader program\n");
        return;
    }

    printf("Modern renderer initialized with shaders\n");
}

void render_with_modern_renderer(const GameState* game_state, const VehicleState* vehicle_state, const PlanetData* planet_data) {
    if (shaderProgram == 0) {
        return;
    }

    // Use shader program
    glUseProgram(shaderProgram);

    // Set uniforms (placeholder values - should be based on game state)
    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    int viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
    int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");

    // Set basic uniforms (these would need to be calculated from game state)
    // For now using placeholder values
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float[]){
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    });

    glUniform3f(viewPosLoc, game_state->campos.x, game_state->campos.y, game_state->campos.z);
    glUniform3f(lightPosLoc, 0.0f, 0.0f, 0.0f);  // Placeholder light position
    glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);  // White light

    // Render scene - this is a simplified representation
    // In a real implementation, you would render the actual game objects
    glBegin(GL_TRIANGLES);
    // Placeholder vertices for demonstration
    glVertex3f(-0.5f, -0.5f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);
    glEnd();

    // Reset shader program
    glUseProgram(0);
}

void cleanup_modern_renderer() {
    if (shaderProgram != 0) {
        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
    }
    printf("Modern renderer cleaned up\n");
}