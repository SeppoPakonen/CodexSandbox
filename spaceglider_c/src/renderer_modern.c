#include "renderer_modern.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// For this implementation, we'll use a fixed-function pipeline approach
// since modern OpenGL shader functions are not available without GLEW

static int initialized = 0;

void initialize_modern_renderer() {
    // Set up OpenGL state (using fixed pipeline like old renderer, 
    // but conceptually we're thinking in terms of modern practices)
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    
    initialized = 1;
    printf("Modern renderer initialized (using compatibility mode)\n");
}

void render_spacecraft_modern(const VehicleState* vehicle_state) {
    glPushMatrix();
    
    // Position the spacecraft
    glTranslatef(vehicle_state->localr.x, vehicle_state->localr.y, vehicle_state->localr.z);
    
    // Apply rotation - use the localB matrix from the vehicle state
    float rotation_matrix[16] = {
        vehicle_state->localB.m[0][0], vehicle_state->localB.m[1][0], vehicle_state->localB.m[2][0], 0.0f,
        vehicle_state->localB.m[0][1], vehicle_state->localB.m[1][1], vehicle_state->localB.m[2][1], 0.0f,
        vehicle_state->localB.m[0][2], vehicle_state->localB.m[1][2], vehicle_state->localB.m[2][2], 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    glMultMatrixf(rotation_matrix);
    
    // Draw a simple representation of the spacecraft
    // A basic cone or arrow shape
    glColor3f(0.0f, 0.8f, 1.0f);  // Light blue color for spacecraft
    
    // Draw a simple cone representing the spacecraft
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Point cone in +Z direction
    
    // Draw the main body (cone)
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.1, 0.5, 2.0, 16, 1); // Base radius, top radius, height
    gluDeleteQuadric(quad);
    
    glPopMatrix();
    
    glPopMatrix();
}

void render_planet_modern(const PlanetData* planet_data) {
    glPushMatrix();
    
    // Position the planet at origin
    glTranslatef(0.0f, 0.0f, 0.0f);
    
    // Draw a basic sphere representing the planet
    glColor3f(0.1f, 0.3f, 0.8f); // Darker blue color for planet
    
    GLUquadric* quad = gluNewQuadric();
    gluQuadricDrawStyle(quad, GLU_FILL);
    gluSphere(quad, planet_data->radius * 0.001f, 50, 50); // Scale down for visualization
    
    gluDeleteQuadric(quad);
    glPopMatrix();
}

void render_hud_modern(const GameState* game_state, const VehicleState* vehicle_state) {
    // Save the current matrix state
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    // Switch to orthographic projection for HUD
    gluOrtho2D(0.0, 800.0, 0.0, 600.0);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // Disable depth testing for HUD
    glDisable(GL_DEPTH_TEST);
    
    // Set color for HUD elements
    glColor3f(0.0f, 1.0f, 1.0f);  // Cyan for modern HUD
    
    // As a placeholder, draw simple geometric indicators
    // Draw a simple throttle indicator (vertical bar)
    glColor3f(0.0f, 1.0f, 1.0f); // Cyan
    glBegin(GL_QUADS);
        glVertex2f(10.0f, 540.0f);  // Bottom left
        glVertex2f(20.0f, 540.0f);  // Bottom right  
        glVertex2f(20.0f, 540.0f + 100.0f * vehicle_state->throttle);  // Top right
        glVertex2f(10.0f, 540.0f + 100.0f * vehicle_state->throttle);  // Top left
    glEnd();
    
    // Draw a simple crosshair in the center
    glColor3f(0.0f, 1.0f, 1.0f); // Cyan
    glBegin(GL_LINES);
        // Horizontal line
        glVertex2f(395.0f, 300.0f);
        glVertex2f(405.0f, 300.0f);
        // Vertical line
        glVertex2f(400.0f, 295.0f);
        glVertex2f(400.0f, 305.0f);
    glEnd();
    
    // Re-enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Restore matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void render_with_modern_renderer(const GameState* game_state, const VehicleState* vehicle_state, const PlanetData* planet_data) {
    // Set up perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0/600.0, 0.1, 100000.0);  // 45 degree FOV
    
    // Set up view matrix based on camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Set camera position and orientation
    float cam_x = vehicle_state->localr.x + 15.0f;
    float cam_y = vehicle_state->localr.y + 15.0f;
    float cam_z = vehicle_state->localr.z + 15.0f;
    
    float look_at_x = vehicle_state->localr.x;
    float look_at_y = vehicle_state->localr.y;
    float look_at_z = vehicle_state->localr.z;
    
    float up_x = 0.0f;
    float up_y = 0.0f;
    float up_z = 1.0f;
    
    gluLookAt(cam_x, cam_y, cam_z, look_at_x, look_at_y, look_at_z, up_x, up_y, up_z);
    
    // Clear the color and depth buffers
    glClearColor(0.05f, 0.05f, 0.15f, 1.0f); // Darker blue background (space)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Render the scene
    render_planet_modern(planet_data);
    render_spacecraft_modern(vehicle_state);
    
    // Render HUD elements
    render_hud_modern(game_state, vehicle_state);
}

void cleanup_modern_renderer() {
    // No special cleanup needed
    printf("Modern renderer cleaned up\n");
}