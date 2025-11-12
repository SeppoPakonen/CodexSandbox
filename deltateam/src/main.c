#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "physics.h"
#include "game_mechanics.h"

// Game constants
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define RESOLUTION_X 640.0f
#define RESOLUTION_Y 360.0f

// Game state
typedef struct {
    GameState_t game_state;
    GLuint shaderProgram;
    GLuint classicShaderProgram;
} MainGameState;

// Function prototypes
void processInput(GLFWwindow *window, MainGameState *mainGameState);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
char* loadShaderSource(const char* filepath);
GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath);
void renderClassic(MainGameState *mainGameState);
void renderModern(MainGameState *mainGameState);

// Global game state
MainGameState mainGameState;

// Main function
int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Delta Team", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // Initialize game state
    init_game_state(&mainGameState.game_state);

    // Set callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load and compile shaders
    mainGameState.shaderProgram = createShaderProgram(
        "shaders/terrain_vertex.glsl", 
        "shaders/terrain_fragment.glsl"
    );
    
    mainGameState.classicShaderProgram = createShaderProgram(
        "shaders/classic_vertex.glsl", 
        "shaders/classic_fragment.glsl"
    );

    // Set up vertex data for a full-screen quad
    float vertices[] = {
        // positions        // texture coords
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,  // top left
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,  // bottom left
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,  // bottom right
         
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,  // top left
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,  // bottom right
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f   // top right
    };

    // Create vertex array object and buffers
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Set up render loop
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        handle_input(window, &mainGameState.game_state);
        update_game_state(&mainGameState.game_state, deltaTime);

        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use appropriate renderer
        if (mainGameState.game_state.use_modern_renderer) {
            renderModern(&mainGameState);
        } else {
            renderClassic(&mainGameState);
        }

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(mainGameState.shaderProgram);
    glDeleteProgram(mainGameState.classicShaderProgram);

    glfwTerminate();
    return 0;
}

// Process input - This function is now handled by the new game_mechanics module
// The main input handling is now done in handle_input() from game_mechanics.c
void processInput(GLFWwindow *window, MainGameState *mainGameState) {
    // This function is now replaced by the handle_input() function in game_mechanics.c
    // but we keep this here for compatibility if needed
    if (mainGameState->game_state.keys[GLFW_KEY_ESCAPE]) {
        glfwSetWindowShouldClose(window, true);
    }
}

// Framebuffer size callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Mouse callback
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    // Mouse handling is now in the game_mechanics module
    // This callback is kept for GLFW setup but the actual handling is done
    // in the update_game_state function
}

// Key callback
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Key handling is now in the game_mechanics module
    // This callback is kept for GLFW setup but the actual handling is done
    // in the handle_input function
}

// Load shader source from file
char* loadShaderSource(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "Failed to open shader file: %s\n", filepath);
        return NULL;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory and read content
    char* buffer = malloc(length + 1);
    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    fclose(file);
    return buffer;
}

// Create shader program
GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath) {
    // Load shader sources
    char* vertexSource = loadShaderSource(vertexPath);
    char* fragmentSource = loadShaderSource(fragmentPath);

    if (!vertexSource || !fragmentSource) {
        return 0;
    }

    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const char**)&vertexSource, NULL);
    glCompileShader(vertexShader);

    // Check for compilation errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "Vertex shader compilation failed: %s\n", infoLog);
    }

    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const char**)&fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Check for compilation errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "Fragment shader compilation failed: %s\n", infoLog);
    }

    // Create shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "Shader program linking failed: %s\n", infoLog);
    }

    // Clean up
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    free(vertexSource);
    free(fragmentSource);

    return shaderProgram;
}

// Render using classic renderer
void renderClassic(MainGameState *mainGameState) {
    glUseProgram(mainGameState->classicShaderProgram);

    // Set uniforms
    glUniform3f(glGetUniformLocation(mainGameState->classicShaderProgram, "iResolution"), 
                RESOLUTION_X, RESOLUTION_Y, 0.0f);
    glUniform1f(glGetUniformLocation(mainGameState->classicShaderProgram, "iTime"), glfwGetTime());
    glUniform4f(glGetUniformLocation(mainGameState->classicShaderProgram, "iMouse"), 
                mainGameState->game_state.mouse_x, mainGameState->game_state.mouse_y, 0.0f, 0.0f);

    // Bind textures if needed
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0); // Placeholder texture ID
    glUniform1i(glGetUniformLocation(mainGameState->classicShaderProgram, "iChannel1"), 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0); // Placeholder texture ID
    glUniform1i(glGetUniformLocation(mainGameState->classicShaderProgram, "iChannel2"), 2);

    // Draw the full-screen quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

// Render using modern renderer
void renderModern(MainGameState *mainGameState) {
    glUseProgram(mainGameState->shaderProgram);

    // Set uniforms
    glUniform3f(glGetUniformLocation(mainGameState->shaderProgram, "iResolution"), 
                RESOLUTION_X, RESOLUTION_Y, 0.0f);
    glUniform1f(glGetUniformLocation(mainGameState->shaderProgram, "iTime"), glfwGetTime());
    glUniform4f(glGetUniformLocation(mainGameState->shaderProgram, "iMouse"), 
                mainGameState->game_state.mouse_x, mainGameState->game_state.mouse_y, 0.0f, 0.0f);

    // Pass player position and rotation as part of a "buffer" texture
    float buffer_data[4] = {
        mainGameState->game_state.player_pos.x,
        mainGameState->game_state.player_pos.y, 
        mainGameState->game_state.player_pos.z,
        0.0f  // Placeholder for movement state
    };
    
    // Set camera rotation
    float rotation_data[4] = {
        mainGameState->game_state.player_rot.x,  // pitch
        mainGameState->game_state.player_rot.y,  // yaw
        0.0f,  // roll
        0.0f   // placeholder
    };
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0); // Placeholder texture ID
    glUniform1i(glGetUniformLocation(mainGameState->shaderProgram, "iChannel0"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0); // Placeholder texture ID
    glUniform1i(glGetUniformLocation(mainGameState->shaderProgram, "iChannel1"), 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0); // Placeholder texture ID
    glUniform1i(glGetUniformLocation(mainGameState->shaderProgram, "iChannel2"), 2);

    // For keyboard input texture (simplified approach)
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, 0); // Placeholder texture ID
    glUniform1i(glGetUniformLocation(mainGameState->shaderProgram, "iChannel3"), 3);

    // Draw the full-screen quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
}