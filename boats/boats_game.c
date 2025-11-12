/*
 * boats_game.c
 * 
 * This is a C game implementation of the "Boats with Drone" Shadertoy.
 * It includes both an old-school renderer without shaders and a modern one with shaders.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <time.h>
#include "boats_physics.h"

// Game constants
#define PI 3.14159265359f
#define MAX_BOATS 3
#define MAX_DRONES 1
#define GAME_WIDTH 800
#define GAME_HEIGHT 600
#define GAME_FPS 60

// Physics constants
#define GRAVITY 9.81
#define WATER_HEIGHT 0.0f

// Game objects
typedef struct {
    float x, y, z;
    float angle;
    float speed;
    float size;
} Boat;

typedef struct {
    float x, y, z;
    float pitch, yaw, roll;
    float speed;
    float size;
} Drone;

// Global game state
static Boat boats[MAX_BOATS];
static Drone drones[MAX_DRONES];
static float camera_x = 0.0f, camera_y = 10.0f, camera_z = 20.0f;
static float camera_angle_x = 0.0f, camera_angle_y = 0.0f;
static float current_time = 0.0f;
static int use_shader_renderer = 1; // 1 for shader, 0 for old-school
static int mouse_x = 0, mouse_y = 0;
static int mouse_button_state = 0;

// Function prototypes
void init_game();
void update_game(float delta_time);
void render_scene();
void render_old_school();
void render_with_shaders();
void handle_input();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void special_keys(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void idle();
void cleanup();

// Noise functions based on Shadertoy implementation
float noise_ff(float p) {
    return fmodf(sinf(p) * 43758.5453f, 1.0f);
}

float noise_fv2(float x, float y) {
    float dot_product = x * 1.0f + y * 57.0f;
    return fmodf(sinf(dot_product) * 43758.5453f, 1.0f);
}

float fbm2(float x, float y) {
    float value = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    
    for (int i = 0; i < 5; i++) {
        value += amplitude * noise_fv2(x * frequency, y * frequency);
        amplitude *= 0.5f;
        frequency *= 2.0f;
    }
    
    return value;
}

// Wave height calculation based on Shadertoy
float wave_height(float x, float z, float time) {
    float result = 0.0f;
    float freq = 0.2f;
    float amp = 0.3f;
    float displacement_x = 0.1f * time;
    float displacement_z = 0.1f * time;
    
    for (int j = 0; j < 3; j++) {
        float t4_x = (time * 1.0f + x + displacement_x) * freq;
        float t4_z = (-time * 1.0f + z + displacement_z) * freq;
        float t2_x = noise_fv2(t4_x, t4_z);
        float t2_y = noise_fv2(t4_z, t4_x);
        
        float ta_x = fabsf(sinf(t4_x));
        float ta_y = fabsf(sinf(t4_z));
        float v4_x = (1.0f - ta_x) * (ta_x + sqrtf(1.0f - ta_x * ta_x));
        float v4_y = (1.0f - ta_y) * (ta_y + sqrtf(1.0f - ta_y * ta_y));
        
        float v2_x = powf(1.0f - powf(v4_x * v4_x, 0.65f), 8.0f);
        float v2_y = powf(1.0f - powf(v4_y * v4_y, 0.65f), 8.0f);
        
        result += (v2_x + v2_y) * amp;
        
        x *= 1.6f; z *= -1.2f; z *= 1.2f; x *= 1.6f; // Matrix rotation equivalent
        freq *= 2.0f;
        amp *= 0.2f;
    }
    
    return result;
}

// Initialize game state
void init_game() {
    // Initialize boats
    for (int i = 0; i < MAX_BOATS; i++) {
        boats[i].x = (float)(rand() % 100 - 50);
        boats[i].y = 0.0f;
        boats[i].z = (float)(rand() % 100 - 50);
        boats[i].angle = (float)(rand() % 360) * PI / 180.0f;
        boats[i].speed = 1.0f + (float)(rand() % 10) / 5.0f;
        boats[i].size = 1.0f;
    }
    
    // Initialize drones
    for (int i = 0; i < MAX_DRONES; i++) {
        drones[i].x = 0.0f;
        drones[i].y = 5.0f;
        drones[i].z = 0.0f;
        drones[i].pitch = 0.0f;
        drones[i].yaw = 0.0f;
        drones[i].roll = 0.0f;
        drones[i].speed = 0.5f;
        drones[i].size = 0.5f;
    }
    
    // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(GAME_WIDTH, GAME_HEIGHT);
    glutCreateWindow("Boats with Drone - C Implementation");
    
    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    GLfloat light_pos[] = {-0.5f, 0.5f, -1.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    
    glClearColor(0.2f, 0.3f, 0.5f, 1.0f); // Sky color
    
    // Set callbacks
    glutDisplayFunc(render_scene);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special_keys);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutIdleFunc(idle);
}

// Update game state
void update_game(float delta_time) {
    current_time += delta_time;
    
    // Update boats based on wave physics
    for (int i = 0; i < MAX_BOATS; i++) {
        // Update boat position with wave physics
        float wave_h = wave_height(boats[i].x, boats[i].z, current_time);
        boats[i].y = 0.13f + wave_h;
        
        // Simple movement
        boats[i].x += cosf(boats[i].angle) * boats[i].speed * delta_time;
        boats[i].z += sinf(boats[i].angle) * boats[i].speed * delta_time;
        
        // Add some wave motion physics
        boats[i].angle += sinf(current_time + i) * 0.01f;
    }
    
    // Update drones
    for (int i = 0; i < MAX_DRONES; i++) {
        drones[i].x += sinf(current_time * 0.5f + i) * drones[i].speed * delta_time;
        drones[i].z += cosf(current_time * 0.5f + i) * drones[i].speed * delta_time;
        
        // Adjust height based on waves and position
        float wave_h = wave_height(drones[i].x, drones[i].z, current_time);
        drones[i].y = 8.0f + wave_h * 0.5f + sinf(current_time * 2.0f + i) * 1.0f;
        
        // Rotate drone based on movement
        drones[i].pitch = sinf(current_time * 3.0f + i) * 0.1f;
        drones[i].roll = cosf(current_time * 2.5f + i) * 0.05f;
    }
}

// Render scene based on renderer selection
void render_scene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    
    // Set camera
    gluLookAt(
        camera_x, camera_y, camera_z,
        camera_x, camera_y - 5, camera_z - 10,
        0.0f, 1.0f, 0.0f
    );
    
    // Apply camera rotation
    glRotatef(camera_angle_x, 1.0f, 0.0f, 0.0f);
    glRotatef(camera_angle_y, 0.0f, 1.0f, 0.0f);
    
    if (use_shader_renderer) {
        render_with_shaders();
    } else {
        render_old_school();
    }
    
    glutSwapBuffers();
}

// Old school renderer without shaders
void render_old_school() {
    // Render water surface
    glColor3f(0.1f, 0.2f, 0.4f);
    glBegin(GL_QUADS);
    for (int x = -50; x < 50; x += 2) {
        for (int z = -50; z < 50; z += 2) {
            float h1 = wave_height(x, z, current_time);
            float h2 = wave_height(x+2, z, current_time);
            float h3 = wave_height(x+2, z+2, current_time);
            float h4 = wave_height(x, z+2, current_time);
            
            glVertex3f(x, h1, z);
            glVertex3f(x+2, h2, z);
            glVertex3f(x+2, h3, z+2);
            glVertex3f(x, h4, z+2);
        }
    }
    glEnd();
    
    // Render boats
    for (int i = 0; i < MAX_BOATS; i++) {
        glPushMatrix();
        glTranslatef(boats[i].x, boats[i].y, boats[i].z);
        glRotatef(boats[i].angle * 180.0f / PI, 0.0f, 1.0f, 0.0f);
        
        // Boat body
        glColor3f(0.8f, 0.6f, 0.2f);
        glScalef(boats[i].size * 3.0f, boats[i].size * 0.5f, boats[i].size * 1.0f);
        glutSolidCube(1.0f);
        
        // Boat cabin
        glPushMatrix();
        glTranslatef(0.0f, 0.8f, 0.0f);
        glColor3f(0.7f, 0.7f, 0.7f);
        glScalef(0.5f, 0.8f, 0.8f);
        glutSolidCube(1.0f);
        glPopMatrix();
        
        glPopMatrix();
    }
    
    // Render drones
    for (int i = 0; i < MAX_DRONES; i++) {
        glPushMatrix();
        glTranslatef(drones[i].x, drones[i].y, drones[i].z);
        glRotatef(drones[i].yaw * 180.0f / PI, 0.0f, 1.0f, 0.0f);
        glRotatef(drones[i].pitch * 180.0f / PI, 1.0f, 0.0f, 0.0f);
        glRotatef(drones[i].roll * 180.0f / PI, 0.0f, 0.0f, 1.0f);
        
        // Drone body
        glColor3f(0.8f, 0.8f, 0.2f);
        glScalef(drones[i].size, drones[i].size, drones[i].size);
        glutSolidSphere(0.5f, 8, 8);
        
        // Drone rotors
        glColor3f(0.5f, 0.5f, 0.5f);
        glPushMatrix();
        glTranslatef(0.7f, 0.0f, 0.0f);
        glRotatef(current_time * 100.0f, 1.0f, 0.0f, 0.0f);
        glutSolidCylinder(0.1f, 0.7f, 8, 1);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(-0.7f, 0.0f, 0.0f);
        glRotatef(current_time * 100.0f, 1.0f, 0.0f, 0.0f);
        glutSolidCylinder(0.1f, 0.7f, 8, 1);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.7f);
        glRotatef(current_time * 100.0f, 0.0f, 0.0f, 1.0f);
        glutSolidCylinder(0.1f, 0.7f, 8, 1);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -0.7f);
        glRotatef(current_time * 100.0f, 0.0f, 0.0f, 1.0f);
        glutSolidCylinder(0.1f, 0.7f, 8, 1);
        glPopMatrix();
        
        glPopMatrix();
    }
}

// Helper function to load and compile a shader
GLuint load_shader(const char* filepath, GLenum shader_type) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "Failed to open shader file: %s\n", filepath);
        return 0;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Read file contents
    char* source = malloc(file_size + 1);
    fread(source, 1, file_size, file);
    source[file_size] = '\0';
    fclose(file);
    
    // Create and compile shader
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, (const char**)&source, NULL);
    glCompileShader(shader);
    
    // Check for compilation errors
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

// Create shader program
GLuint create_shader_program() {
    GLuint vertex_shader = load_shader("vertex_shader.glsl", GL_VERTEX_SHADER);
    GLuint fragment_shader = load_shader("fragment_shader.glsl", GL_FRAGMENT_SHADER);
    
    if (!vertex_shader || !fragment_shader) {
        return 0;
    }
    
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    
    // Check for linking errors
    GLint success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        fprintf(stderr, "Shader program linking failed: %s\n", info_log);
    }
    
    // Clean up shaders (not needed after linking)
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    return shader_program;
}

// Renderer using shaders
void render_with_shaders() {
    static GLuint shader_program = 0;
    if (shader_program == 0) {
        shader_program = create_shader_program();
    }
    
    if (shader_program == 0) {
        // Fall back to old-school renderer if shaders fail
        render_old_school();
        return;
    }
    
    glUseProgram(shader_program);
    
    // Set uniforms
    GLint time_loc = glGetUniformLocation(shader_program, "time");
    if (time_loc != -1) {
        glUniform1f(time_loc, current_time);
    }
    
    GLint view_pos_loc = glGetUniformLocation(shader_program, "viewPos");
    if (view_pos_loc != -1) {
        glUniform3f(view_pos_loc, camera_x, camera_y, camera_z);
    }
    
    GLint light_pos_loc = glGetUniformLocation(shader_program, "lightPos");
    if (light_pos_loc != -1) {
        glUniform3f(light_pos_loc, -5.0f, 10.0f, -5.0f);
    }
    
    GLint light_color_loc = glGetUniformLocation(shader_program, "lightColor");
    if (light_color_loc != -1) {
        glUniform3f(light_color_loc, 1.0f, 1.0f, 1.0f);
    }
    
    // Render water surface
    glColor3f(0.1f, 0.2f, 0.4f);
    glBegin(GL_QUADS);
    for (int x = -50; x < 50; x += 2) {
        for (int z = -50; z < 50; z += 2) {
            float h1 = wave_height(x, z, current_time);
            float h2 = wave_height(x+2, z, current_time);
            float h3 = wave_height(x+2, z+2, current_time);
            float h4 = wave_height(x, z+2, current_time);
            
            glVertex3f(x, h1, z);
            glVertex3f(x+2, h2, z);
            glVertex3f(x+2, h3, z+2);
            glVertex3f(x, h4, z+2);
        }
    }
    glEnd();
    
    // Render boats
    for (int i = 0; i < MAX_BOATS; i++) {
        glPushMatrix();
        glTranslatef(boats[i].x, boats[i].y, boats[i].z);
        glRotatef(boats[i].angle * 180.0f / PI, 0.0f, 1.0f, 0.0f);
        
        // Boat body
        glScalef(boats[i].size * 3.0f, boats[i].size * 0.5f, boats[i].size * 1.0f);
        glutSolidCube(1.0f);
        
        // Boat cabin
        glPushMatrix();
        glTranslatef(0.0f, 0.8f, 0.0f);
        glScalef(0.5f, 0.8f, 0.8f);
        glutSolidCube(1.0f);
        glPopMatrix();
        
        glPopMatrix();
    }
    
    // Render drones
    for (int i = 0; i < MAX_DRONES; i++) {
        glPushMatrix();
        glTranslatef(drones[i].x, drones[i].y, drones[i].z);
        glRotatef(drones[i].yaw * 180.0f / PI, 0.0f, 1.0f, 0.0f);
        glRotatef(drones[i].pitch * 180.0f / PI, 1.0f, 0.0f, 0.0f);
        glRotatef(drones[i].roll * 180.0f / PI, 0.0f, 0.0f, 1.0f);
        
        // Drone body
        glScalef(drones[i].size, drones[i].size, drones[i].size);
        glutSolidSphere(0.5f, 8, 8);
        
        // Drone rotors
        glPushMatrix();
        glTranslatef(0.7f, 0.0f, 0.0f);
        glRotatef(current_time * 100.0f, 1.0f, 0.0f, 0.0f);
        glutSolidCylinder(0.1f, 0.7f, 8, 1);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(-0.7f, 0.0f, 0.0f);
        glRotatef(current_time * 100.0f, 1.0f, 0.0f, 0.0f);
        glutSolidCylinder(0.1f, 0.7f, 8, 1);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.7f);
        glRotatef(current_time * 100.0f, 0.0f, 0.0f, 1.0f);
        glutSolidCylinder(0.1f, 0.7f, 8, 1);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -0.7f);
        glRotatef(current_time * 100.0f, 0.0f, 0.0f, 1.0f);
        glutSolidCylinder(0.1f, 0.7f, 8, 1);
        glPopMatrix();
        
        glPopMatrix();
    }
    
    glUseProgram(0);
}

// Handle input
void handle_input() {
    // Input handling would be implemented here
}

// Reshape callback
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    float aspect_ratio = (float)width / (float)height;
    gluPerspective(45.0, aspect_ratio, 0.1, 1000.0);
    
    glMatrixMode(GL_MODELVIEW);
}

// Keyboard callback
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ESC key
            cleanup();
            exit(0);
            break;
        case ' ': // Space to toggle renderer
            use_shader_renderer = !use_shader_renderer;
            break;
        case 'w':
        case 'W':
            camera_z -= 1.0f;
            break;
        case 's':
        case 'S':
            camera_z += 1.0f;
            break;
        case 'a':
        case 'A':
            camera_x -= 1.0f;
            break;
        case 'd':
        case 'D':
            camera_x += 1.0f;
            break;
    }
    glutPostRedisplay();
}

// Special keys callback
void special_keys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            camera_angle_x -= 5.0f;
            break;
        case GLUT_KEY_DOWN:
            camera_angle_x += 5.0f;
            break;
        case GLUT_KEY_LEFT:
            camera_angle_y -= 5.0f;
            break;
        case GLUT_KEY_RIGHT:
            camera_angle_y += 5.0f;
            break;
    }
}

// Mouse callback
void mouse(int button, int state, int x, int y) {
    mouse_x = x;
    mouse_y = y;
    mouse_button_state = (state == GLUT_DOWN) ? 1 : 0;
}

// Mouse motion callback
void motion(int x, int y) {
    int dx = x - mouse_x;
    int dy = y - mouse_y;
    
    if (mouse_button_state) {
        camera_angle_y += dx * 0.5f;
        camera_angle_x += dy * 0.5f;
    }
    
    mouse_x = x;
    mouse_y = y;
}

// Idle callback
void idle() {
    static clock_t last_time = 0;
    clock_t current_time_c = clock();
    float delta_time = ((float)(current_time_c - last_time)) / CLOCKS_PER_SEC;
    last_time = current_time_c;
    
    if (delta_time > 0.1f) delta_time = 0.1f; // Cap delta time
    
    update_game(delta_time);
    glutPostRedisplay();
}

// Cleanup before exit
void cleanup() {
    // Any cleanup code would go here
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    
    init_game();
    
    printf("Boats with Drone - C Implementation\n");
    printf("Controls:\n");
    printf("  WASD - Move camera\n");
    printf("  Arrow Keys - Rotate camera\n");
    printf("  Space - Toggle renderer (shader vs old-school)\n");
    printf("  ESC - Exit\n");
    
    glutMainLoop();
    
    return 0;
}