#include "renderer_old.h"
#include "spaceglider/types.h"
#include <GL/gl.h>
#include <stdio.h>
#include <math.h>
#define M_PI 3.14159265358979323846

void initialize_old_renderer() {
    // Initialize OpenGL states for fixed function pipeline
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Set up basic lighting
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    // Enable color material
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    printf("Old school renderer initialized (fixed pipeline)\n");
}

void render_with_old_renderer(const GameState* game_state, const VehicleState* vehicle_state, const PlanetData* planet_data) {
    // Set camera position and orientation using fixed pipeline
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Apply camera transformation using the game state
    // This is a simplified camera transformation
    glTranslatef(-game_state->campos.x, -game_state->campos.y, -game_state->campos.z);
    
    // Apply rotation from camera frame (simplified)
    // In a real implementation, this would properly convert the orientation matrix to OpenGL
    glMultMatrixf((float*)&game_state->camframe.m[0][0]);
    
    // Render scene using immediate mode
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Render planet (simplified as a sphere)
    glColor3f(0.3f, 0.6f, 0.9f);  // Blue color for planet
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);  // Planet at origin
    glScalef(planet_data->radius, planet_data->radius, planet_data->radius);
    
    // Draw a simple sphere approximation using quad strips
    const int slices = 20;
    const int stacks = 20;
    
    for (int i = 0; i < stacks; i++) {
        float phi1 = M_PI * (-0.5f + (float)i / stacks);
        float phi2 = M_PI * (-0.5f + (float)(i + 1) / stacks);
        
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; j++) {
            float theta = 2.0f * M_PI * (float)j / slices;
            
            float x1 = cosf(phi1) * cosf(theta);
            float y1 = sinf(phi1);
            float z1 = cosf(phi1) * sinf(theta);
            
            float x2 = cosf(phi2) * cosf(theta);
            float y2 = sinf(phi2);
            float z2 = cosf(phi2) * sinf(theta);
            
            glNormal3f(x1, y1, z1);
            glVertex3f(x1, y1, z1);
            
            glNormal3f(x2, y2, z2);
            glVertex3f(x2, y2, z2);
        }
        glEnd();
    }
    glPopMatrix();
    
    // Render vehicle (simplified as a cone)
    glColor3f(0.8f, 0.8f, 0.2f);  // Yellow color for vehicle
    glPushMatrix();
    glTranslatef(vehicle_state->localr.x, vehicle_state->localr.y, vehicle_state->localr.z);
    
    // Apply orientation from vehicle's localB matrix
    glMultMatrixf((float*)&vehicle_state->localB.m[0][0]);
    
    // Draw a simple cone to represent the vehicle
    const int segments = 16;
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < segments; i++) {
        float angle1 = 2.0f * M_PI * i / segments;
        float angle2 = 2.0f * M_PI * (i + 1) / segments;
        
        // Base triangle
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.2f * cosf(angle1), 0.0f, 0.2f * sinf(angle1));
        glVertex3f(0.2f * cosf(angle2), 0.0f, 0.2f * sinf(angle2));
        
        // Side triangle
        glVertex3f(0.2f * cosf(angle1), 0.0f, 0.2f * sinf(angle1));
        glVertex3f(0.2f * cosf(angle2), 0.0f, 0.2f * sinf(angle2));
        glVertex3f(0.0f, 0.5f, 0.0f);
    }
    glEnd();
    glPopMatrix();
    
    // Render other game elements as needed
    // This is where terrain, stars, etc. would be rendered in immediate mode
}

void cleanup_old_renderer() {
    // Disable lighting and other fixed pipeline features
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_COLOR_MATERIAL);
    
    printf("Old school renderer cleaned up\n");
}