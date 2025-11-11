#include "renderer_old.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>

void initialize_old_renderer() {
    // Set up OpenGL state for fixed pipeline rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    
    // Enable lighting for basic 3D effects
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    GLfloat light_pos[] = { 0.0f, 0.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    
    // Set up material properties
    GLfloat mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    
    printf("Old school renderer initialized\n");
}

void render_spacecraft(const VehicleState* vehicle_state) {
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
    glColor3f(1.0f, 1.0f, 1.0f);  // White color for spacecraft
    
    // Draw a simple cone representing the spacecraft
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Point cone in +Z direction
    
    // Draw the main body (cylinder/cone)
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.5, 1.0, 3.0, 16, 1); // Base radius, top radius, height
    gluDeleteQuadric(quad);
    
    glPopMatrix();
    
    glPopMatrix();
}

void render_planet(const PlanetData* planet_data) {
    glPushMatrix();
    
    // Position the planet at origin
    glTranslatef(0.0f, 0.0f, 0.0f);
    
    // Draw a basic sphere representing the planet
    glColor3f(0.2f, 0.5f, 1.0f); // Blue color for water planet
    
    GLUquadric* quad = gluNewQuadric();
    gluQuadricDrawStyle(quad, GLU_FILL);
    gluSphere(quad, planet_data->radius * 0.001f, 50, 50); // Scale down for visualization
    
    gluDeleteQuadric(quad);
    glPopMatrix();
}

void render_hud(const GameState* game_state, const VehicleState* vehicle_state) {
    // Save the current matrix state
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    // Switch to orthographic projection for HUD
    gluOrtho2D(0.0, 800.0, 0.0, 600.0);  // Assuming 800x600 window
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // Disable depth testing for HUD
    glDisable(GL_DEPTH_TEST);
    
    // Set color for HUD elements
    glColor3f(0.0f, 1.0f, 0.0f);  // Green for HUD
    
    // For now, skip text rendering since GLUT is not available
    // In a complete implementation, you would use a different text rendering approach
    // such as texture-based fonts or integrated text rendering library
    
    // As a placeholder, we could draw simple geometric indicators
    // Draw a simple throttle indicator (vertical bar)
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glBegin(GL_QUADS);
        glVertex2f(10.0f, 540.0f);  // Bottom left
        glVertex2f(20.0f, 540.0f);  // Bottom right  
        glVertex2f(20.0f, 540.0f + 100.0f * vehicle_state->throttle);  // Top right
        glVertex2f(10.0f, 540.0f + 100.0f * vehicle_state->throttle);  // Top left
    glEnd();
    
    // Draw a simple crosshair in the center
    glColor3f(0.0f, 1.0f, 0.0f); // Green
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

void render_with_old_renderer(const GameState* game_state, const VehicleState* vehicle_state, const PlanetData* planet_data) {
    // Set up perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0/600.0, 0.1, 100000.0);  // 45 degree FOV
    
    // Set up view matrix based on camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Set camera position and orientation
    // For simplicity, we'll use a basic camera look-at
    float cam_x = vehicle_state->localr.x + 10.0f;
    float cam_y = vehicle_state->localr.y + 10.0f;
    float cam_z = vehicle_state->localr.z + 10.0f;
    
    float look_at_x = vehicle_state->localr.x;
    float look_at_y = vehicle_state->localr.y;
    float look_at_z = vehicle_state->localr.z;
    
    float up_x = 0.0f;
    float up_y = 0.0f;
    float up_z = 1.0f;
    
    gluLookAt(cam_x, cam_y, cam_z, look_at_x, look_at_y, look_at_z, up_x, up_y, up_z);
    
    // Clear the color and depth buffers
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f); // Dark blue background (space)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Render the scene
    render_planet(planet_data);
    render_spacecraft(vehicle_state);
    
    // Render HUD elements
    render_hud(game_state, vehicle_state);
}

void cleanup_old_renderer() {
    // No special cleanup needed for fixed pipeline renderer
    printf("Old school renderer cleaned up\n");
}