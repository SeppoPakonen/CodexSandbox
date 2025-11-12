#define _DEFAULT_SOURCE  // This should provide cfmakeraw and usleep
#include "soccer2d.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>  // For sleep, usleep
#include <sys/ioctl.h>  // For ioctl
#include <termios.h>   // For termios functions like cfmakeraw

// Determine rendering mode at compile time
#ifdef USE_OPENGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Global variables for OpenGL rendering
GLFWwindow* window = NULL;
GLuint shader_program = 0;
GLuint VAO = 0, VBO = 0, EBO = 0;

// Function to load shader source from file
char* load_shader_source(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "Failed to open shader file: %s\n", filepath);
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* source = malloc(length + 1);
    fread(source, 1, length, file);
    source[length] = '\0';
    
    fclose(file);
    return source;
}

// Function to compile and link shaders
GLuint create_shader_program(const char* vertex_path, const char* fragment_path) {
    // Load shader source code
    char* vertex_source = load_shader_source(vertex_path);
    char* fragment_source = load_shader_source(fragment_path);
    
    if (!vertex_source || !fragment_source) {
        free(vertex_source);
        free(fragment_source);
        return 0;
    }
    
    // Compile vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, (const GLchar* const*)&vertex_source, NULL);
    glCompileShader(vertex_shader);
    
    // Check for vertex shader compilation errors
    GLint success;
    GLchar info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        fprintf(stderr, "Vertex shader compilation failed:\n%s\n", info_log);
    }
    
    // Compile fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, (const GLchar* const*)&fragment_source, NULL);
    glCompileShader(fragment_shader);
    
    // Check for fragment shader compilation errors
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        fprintf(stderr, "Fragment shader compilation failed:\n%s\n", info_log);
    }
    
    // Create shader program and link shaders
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    
    // Check for linking errors
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        fprintf(stderr, "Shader program linking failed:\n%s\n", info_log);
    }
    
    // Clean up
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    free(vertex_source);
    free(fragment_source);
    
    return shader_program;
}

// Initialize OpenGL context and shaders
int init_opengl() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return 0;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(800, 600, "Soccer 2D", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return 0;
    }
    
    glfwMakeContextCurrent(window);
    
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return 0;
    }
    
    // Create a simple quad to render our shader on
    float vertices[] = {
        // positions        // texture coords
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,  // top right
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom left
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f   // top left 
    };
    
    unsigned int indices[] = {  
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    
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
    
    // Texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Load and compile shaders
    shader_program = create_shader_program(
        "/home/sblo/Dev/CodexSandbox/soccer2d/shaders/vertex.glsl",
        "/home/sblo/Dev/CodexSandbox/soccer2d/shaders/fragment.glsl"
    );
    
    if (!shader_program) {
        return 0;
    }
    
    return 1;
}

// Handle input for OpenGL version
void handle_input_opengl(GameState* state) {
    if (glfwWindowShouldClose(window)) {
        exit(0);
    }
    
    // Handle keyboard input
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        state->key_states['A'] = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        state->key_states['S'] = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        state->demo_mode = !state->demo_mode;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        exit(0);
    }
    
    // Handle mouse input
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    state->mouse_pos.x = (float)xpos;
    state->mouse_pos.y = (float)ypos;
    
    // Check if mouse button is pressed
    state->mouse_active = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
}

#else
// For terminal-based input
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

struct termios orig_termios;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
    int ch = getchar();
    
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    
    return 0;
}

int getch()
{
    int ch;
    ch = getchar();
    return ch;
}

// Handle input for terminal version
void handle_input_terminal(GameState* state) {
    if (kbhit()) {
        int ch = getch();
        switch (ch) {
            case 'q':
            case 'Q':
                exit(0);
                break;
            case 'a':
            case 'A':
                state->key_states['A'] = 1;
                break;
            case 's':
            case 'S':
                state->key_states['S'] = 1;
                break;
            case 'e':
            case 'E':
                state->demo_mode = !state->demo_mode;  // Toggle demo mode
                break;
            case 'c':
            case 'C':
                // Toggle auto shot (not fully implemented in this version)
                break;
            default:
                state->key_states[ch] = 1;
                break;
        }
    }
    
    // For terminal-based input, mouse is not available in the same way
    state->mouse_active = false;
}
#endif

int main(int argc, char* argv[]) {
    GameState state;
    initialize_game_state(&state);
    
    // Check command line arguments to determine renderer to use
    int use_shader_renderer = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--shader") == 0 || strcmp(argv[i], "-s") == 0) {
            use_shader_renderer = 1;
        }
    }
    
#ifdef USE_OPENGL
    if (!init_opengl()) {
        fprintf(stderr, "Failed to initialize OpenGL, falling back to terminal renderer\n");
        use_shader_renderer = 0;
    }
#else
    // For terminal-based input
    struct termios orig_termios;
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
    use_shader_renderer = 0;  // Force terminal renderer if OpenGL is not available
#endif

    printf("Soccer 2D Game\n");
    printf("Controls: A=Shot, S=Pass, E=Toggle demo, Q=Quit\n");
    if (use_shader_renderer) {
        printf("Using shader renderer\n");
    } else {
        printf("Using terminal renderer\n");
    }
    sleep(2); // Brief pause to read instructions
    
    // Main game loop
    while (1) {
#ifdef USE_OPENGL
        if (use_shader_renderer) {
            handle_input_opengl(&state);
        } else {
            handle_input_terminal(&state);
        }
#else
        handle_input_terminal(&state);
#endif
        
        // Update game logic
        update_game_logic(&state);
        
        // Update physics
        update_physics(&state);
        
        // Render based on selected renderer
        if (use_shader_renderer) {
#ifdef USE_OPENGL
            // Use OpenGL shader renderer
            glUseProgram(shader_program);
            
            // Set uniforms
            glUniform1f(glGetUniformLocation(shader_program, "iTime"), state.time);
            glUniform2f(glGetUniformLocation(shader_program, "iResolution"), 
                       state.resolution.x, state.resolution.y);
            glUniform4f(glGetUniformLocation(shader_program, "iMouse"), 
                       state.mouse_pos.x, state.mouse_pos.y, 
                       state.mouse_active ? 1.0f : 0.0f, 0.0f);
            
            // Bind textures if needed (for buffer A and B)
            // This would involve updating textures with game state data
            
            // Render
            glClear(GL_COLOR_BUFFER_BIT);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            
            glfwSwapBuffers(window);
            glfwPollEvents();
#else
            render_with_shaders(&state);
#endif
        } else {
            render_old_school(&state);
        }
        
        // Reset key states for next frame (except for those that should persist)
        for (int i = 0; i < 256; i++) {
            if (i != 'A' && i != 'S' && i != 'E' && i != 'C') {
                state.key_states[i] = 0;  // Fixed: using . instead of ->
            }
        }
        
        // Slow down the game loop (60 FPS)
        usleep(1000000 / 60);  // Sleep for ~16.67ms
        
        // Clear terminal flags for next iteration
        state.key_states['A'] = 0;  // Fixed: using . instead of ->
        state.key_states['S'] = 0;  // Fixed: using . instead of ->
    }
    
    return 0;
}