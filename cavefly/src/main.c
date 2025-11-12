#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

// Constants for the ship
#define SHIP_SIZE 0.05f
#define SHIP_NOSE_X 0.0f
#define SHIP_NOSE_Y 1.0f * SHIP_SIZE
#define SHIP_LEFT_WING_X 0.5f * SHIP_SIZE
#define SHIP_LEFT_WING_Y (-0.3f) * SHIP_SIZE
#define SHIP_RIGHT_WING_X (-0.5f) * SHIP_SIZE
#define SHIP_RIGHT_WING_Y (-0.3f) * SHIP_SIZE

// Game mode enum
typedef enum {
    CLASSIC_RENDERER,
    MODERN_RENDERER
} RenderMode;

// Game state structure (from physics.c)
typedef struct {
    float position_x;
    float position_y;
    float rotation;
    float thrust;  // w component
    float velocity_x;
    float velocity_y;
    float angular_velocity;
    float extra_velocity; // w component
    float zoom;
} ShipState;

// Function declarations from physics.c
float rand_val(float x, float y);
int get_tile(int x, int y);
float get_map_sdf(float pixel_x, float pixel_y);
float sample_map_at(float world_x, float world_y);
void update_physics(ShipState* state, int key_up, int key_left, int key_right, float time_delta);
void init_ship_state(ShipState* state);

// Global game state
ShipState g_ship_state;
RenderMode g_render_mode = MODERN_RENDERER; // Start with modern renderer
bool g_key_states[1024]; // To store key states

// Function prototypes
void render_classic_scene(ShipState* state);
void render_shader_scene(ShipState* state, GLFWwindow* window);
void update_time(ShipState* state, float delta_time);

// Process input
void process_input(GLFWwindow* window) {
    // Switch between renderers
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        g_render_mode = CLASSIC_RENDERER;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        g_render_mode = MODERN_RENDERER;
    }
    
    // Update key states for physics
    g_key_states[GLFW_KEY_UP] = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
    g_key_states[GLFW_KEY_LEFT] = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;
    g_key_states[GLFW_KEY_RIGHT] = glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
}

// Initialize OpenGL
void init_opengl() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

// Render the classic cave environment (from classic_renderer.c)
void render_classic_scene(ShipState* state) {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Set background color (dark brown/cave-like)
    glClearColor(0.4f, 0.2f, 0.1f, 1.0f);
    
    // Draw a simple cave environment using basic shapes
    glColor3f(0.6f, 0.4f, 0.3f); // Cave rock color
    
    // Draw some basic cave formations (just simple shapes)
    glBegin(GL_QUADS);
    // Example: simple cave walls
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(-0.8f, -1.0f);
    glVertex2f(-0.8f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    
    glVertex2f(0.8f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(0.8f, 1.0f);
    glEnd();
    
    // Simple representation of cave floor/ceiling
    glColor3f(0.5f, 0.3f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, 0.8f);
    glVertex2f(1.0f, 0.8f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, -0.8f);
    glVertex2f(-1.0f, -0.8f);
    glEnd();
    
    // Draw the player ship (simple triangle)
    glPushMatrix();
    glTranslatef(state->position_x, state->position_y, 0.0f);
    glRotatef(state->rotation * 180.0f / M_PI, 0.0f, 0.0f, 1.0f);
    
    glColor3f(0.8f, 0.8f, 0.0f); // Yellow ship
    glBegin(GL_TRIANGLES);
    glVertex2f(SHIP_NOSE_X, SHIP_NOSE_Y);       // Nose
    glVertex2f(SHIP_LEFT_WING_X, SHIP_LEFT_WING_Y);  // Left wing
    glVertex2f(SHIP_RIGHT_WING_X, SHIP_RIGHT_WING_Y); // Right wing
    glEnd();
    glPopMatrix();
}

// Render the scene using shaders (from modern_renderer.c)
void render_shader_scene(ShipState* state, GLFWwindow* window) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
    
    // In a full implementation, we would set up proper shader rendering here
    // For now, we'll just render a basic scene as a placeholder
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(0.5f, 0.7f, 1.0f); // Placeholder color
    
    // Draw a basic representation of the scene
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(-0.5f, 0.5f);
    glEnd();
    
    // Draw the ship
    glPushMatrix();
    glTranslatef(state->position_x, state->position_y, 0.0f);
    glRotatef(state->rotation * 180.0f / M_PI, 0.0f, 0.0f, 1.0f);
    
    glColor3f(0.8f, 0.8f, 0.0f); // Yellow ship
    glBegin(GL_TRIANGLES);
    glVertex2f(SHIP_NOSE_X, SHIP_NOSE_Y);       // Nose
    glVertex2f(SHIP_LEFT_WING_X, SHIP_LEFT_WING_Y);  // Left wing
    glVertex2f(SHIP_RIGHT_WING_X, SHIP_RIGHT_WING_Y); // Right wing
    glEnd();
    glPopMatrix();
}

// Update game time
void update_time(ShipState* state, float delta_time) {
    // In a full implementation, we would update the game time
    // This is a simplified version
}

// Callback for keyboard input
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    // Initialize the ship state
    init_ship_state(&g_ship_state);
    
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
    GLFWwindow* window = glfwCreateWindow(800, 600, "CaveFly", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    
    // Make the context current
    glfwMakeContextCurrent(window);
    
    // Set the key callback
    glfwSetKeyCallback(window, key_callback);
    
    // Initialize OpenGL settings
    init_opengl();
    
    // Time tracking
    float last_frame_time = glfwGetTime();
    
    // Game loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        float current_frame_time = glfwGetTime();
        float delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;
        
        // Process input
        process_input(window);
        
        // Update physics
        update_physics(
            &g_ship_state, 
            g_key_states[GLFW_KEY_UP], 
            g_key_states[GLFW_KEY_LEFT], 
            g_key_states[GLFW_KEY_RIGHT], 
            delta_time
        );
        
        // Render based on selected mode
        if (g_render_mode == CLASSIC_RENDERER) {
            render_classic_scene(&g_ship_state);
        } else {
            render_shader_scene(&g_ship_state, window);
        }
        
        // Update any time-dependent values
        update_time(&g_ship_state, delta_time);
        
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Clean up
    glfwTerminate();
    return 0;
}