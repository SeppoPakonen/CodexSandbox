#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../include/rollercoaster.h"

// Game constants
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define FOV 45.0f
#define NEAR_PLANE 0.1f
#define FAR_PLANE 100.0f

// Global game state
static RollerCoasterState g_state;
static GLFWwindow* g_window = NULL;
static bool g_mouse_locked = false;
static double g_last_mouse_x = 0.0, g_last_mouse_y = 0.0;
static bool g_first_mouse = true;
static float g_time = 0.0f;

// Function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void process_input(GLFWwindow* window);
GLuint load_shader(const char* filepath, GLenum shader_type);
GLuint create_shader_program(const char* vertex_path, const char* fragment_path);

// Old school renderer functions
void render_old_school();
void setup_ortho_projection();

// Modern renderer functions
GLuint g_shader_program = 0;
GLuint g_vao = 0, g_vbo = 0;
GLuint g_quad_vao = 0, g_quad_vbo = 0;
void render_modern();

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
    g_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Roller Coaster", NULL, NULL);
    if (g_window == NULL) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(g_window);
    glfwSetFramebufferSizeCallback(g_window, framebuffer_size_callback);
    glfwSetCursorPosCallback(g_window, mouse_callback);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // Initialize roller coaster
    initRollerCoaster(&g_state);
    
    // Set up modern rendering
    render_modern();
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Main game loop
    while (!glfwWindowShouldClose(g_window)) {
        float current_time = (float)glfwGetTime();
        float delta_time = current_time - g_time;
        g_time = current_time;
        
        // Process input
        process_input(g_window);
        
        // Update roller coaster state
        updateRollerCoaster(&g_state, g_time);
        
        // Set sun direction
        float sun_time = 0.02f * g_time;
        g_state.sunDir = vec3_normalize((vec3){cosf(sun_time), 1.0f, sinf(sun_time)});
        
        // Clear screen
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Render the scene
        if (g_state.riding) {
            // Modern shader-based rendering for first-person view
            render_modern();
        } else {
            // Old school fixed-function rendering for overview
            render_old_school();
        }
        
        // Swap buffers and poll events
        glfwSwapBuffers(g_window);
        glfwPollEvents();
    }
    
    // Cleanup
    glDeleteVertexArrays(1, &g_quad_vao);
    glDeleteBuffers(1, &g_quad_vbo);
    
    glfwTerminate();
    return 0;
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        g_state.riding = !g_state.riding;
        if (g_state.riding) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (g_first_mouse) {
        g_last_mouse_x = xpos;
        g_last_mouse_y = ypos;
        g_first_mouse = false;
    }
    
    float xoffset = xpos - g_last_mouse_x;
    float yoffset = g_last_mouse_y - ypos; // Reversed since y-coordinates go from bottom to top
    
    g_last_mouse_x = xpos;
    g_last_mouse_y = ypos;
    
    // Only process mouse movement if we're in overview mode
    if (!g_state.riding) {
        // Update camera orientation based on mouse movement
        // This would involve changing the camera position/orientation for overview
    }
}

// Old school renderer implementation
void render_old_school() {
    // Set up orthographic projection for old school rendering
    setup_ortho_projection();
    
    // Render basic shapes to represent the roller coaster
    glUseProgram(0); // Use fixed-function pipeline
    
    // Example: Render simple lines for the track
    glBegin(GL_LINES);
    glColor3f(0.8f, 0.8f, 0.9f);
    
    // Render a simplified track path
    for (int i = 0; i < NSEG - 1; i++) {
        glVertex3f(g_state.cPt[i].x, g_state.cPt[i].y, g_state.cPt[i].z);
        glVertex3f(g_state.cPt[i+1].x, g_state.cPt[i+1].y, g_state.cPt[i+1].z);
    }
    
    // Close the loop
    glVertex3f(g_state.cPt[NSEG-1].x, g_state.cPt[NSEG-1].y, g_state.cPt[NSEG-1].z);
    glVertex3f(g_state.cPt[0].x, g_state.cPt[0].y, g_state.cPt[0].z);
    
    glEnd();
    
    // Render the cars
    for (int i = 0; i < NCAR; i++) {
        glPushMatrix();
        glTranslatef(g_state.carPos[i].x, g_state.carPos[i].y, g_state.carPos[i].z);
        
        // Draw a simple box for each car
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        // Front face
        glVertex3f(-0.1f, -0.1f, 0.1f);
        glVertex3f(0.1f, -0.1f, 0.1f);
        glVertex3f(0.1f, 0.1f, 0.1f);
        glVertex3f(-0.1f, 0.1f, 0.1f);
        // Back face
        glVertex3f(-0.1f, -0.1f, -0.1f);
        glVertex3f(-0.1f, 0.1f, -0.1f);
        glVertex3f(0.1f, 0.1f, -0.1f);
        glVertex3f(0.1f, -0.1f, -0.1f);
        glEnd();
        
        glPopMatrix();
    }
}

void setup_ortho_projection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10.0, 10.0, -7.0, 7.0, 0.1, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Set camera position for overview
    if (!g_state.riding) {
        // Overview camera
        glTranslatef(0.0f, -5.0f, -15.0f);
        glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
    } else {
        // First-person camera would be handled differently
    }
}

// Modern renderer implementation
void render_modern() {
    if (g_shader_program == 0) {
        // Load shaders from files
        g_shader_program = create_shader_program(
            "shaders/SimpleVertexShader.vertexshader", 
            "shaders/SimpleFragmentShader.fragmentshader"
        );
        
        // Set up quad for full-screen rendering
        float quad_vertices[] = {
            // positions        // texture coords
             1.0f,  1.0f, 0.0f,  1.0f, 1.0f,  // top right
             1.0f, -1.0f, 0.0f,  1.0f, 0.0f,  // bottom right
            -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,  // bottom left
            -1.0f,  1.0f, 0.0f,  0.0f, 1.0f   // top left 
        };
        unsigned int indices[] = { 
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };
        
        glGenVertexArrays(1, &g_quad_vao);
        glGenBuffers(1, &g_quad_vbo);
        
        glBindVertexArray(g_quad_vao);
        glBindBuffer(GL_ARRAY_BUFFER, g_quad_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);
        
        unsigned int ebo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    
    // Use the shader program
    glUseProgram(g_shader_program);
    
    // Set uniforms
    int time_loc = glGetUniformLocation(g_shader_program, "iTime");
    int res_loc = glGetUniformLocation(g_shader_program, "iResolution");
    int mouse_loc = glGetUniformLocation(g_shader_program, "iMouse");
    
    if (time_loc != -1) glUniform1f(time_loc, g_time);
    if (res_loc != -1) glUniform2f(res_loc, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT);
    
    double mouse_x, mouse_y;
    glfwGetCursorPos(g_window, &mouse_x, &mouse_y);
    int mouse_down = glfwGetMouseButton(g_window, GLFW_MOUSE_BUTTON_LEFT);
    float mouse_data[4] = {(float)mouse_x, (float)mouse_y, (float)mouse_down, 0.0f};
    if (mouse_loc != -1) {
        glUniform4f(mouse_loc, mouse_data[0], mouse_data[1], mouse_data[2], mouse_data[3]);
    }
    
    // Render the quad
    glBindVertexArray(g_quad_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

// Helper function to load shader source from file
GLuint load_shader(const char* filepath, GLenum shader_type) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "Could not open shader file: %s\n", filepath);
        return 0;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Read file content
    char* source = malloc(length + 1);
    fread(source, 1, length, file);
    source[length] = '\0';
    
    fclose(file);
    
    // Create and compile shader
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, (const GLchar**)&source, NULL);
    glCompileShader(shader);
    
    // Check compilation status
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        fprintf(stderr, "Shader compilation failed: %s\n", info_log);
    }
    
    free(source);
    return shader;
}

// Helper function to create shader program
GLuint create_shader_program(const char* vertex_path, const char* fragment_path) {
    GLuint vertex_shader = load_shader(vertex_path, GL_VERTEX_SHADER);
    GLuint fragment_shader = load_shader(fragment_path, GL_FRAGMENT_SHADER);
    
    if (!vertex_shader || !fragment_shader) {
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return 0;
    }
    
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    
    // Check linking status
    GLint success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        fprintf(stderr, "Shader program linking failed: %s\n", info_log);
    }
    
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    return shader_program;
}