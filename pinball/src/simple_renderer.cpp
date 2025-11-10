#include "simple_renderer.h"
#include <iostream>
#include <memory>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

SimpleRenderer::SimpleRenderer(int width, int height) 
    : width(width), height(height) {
    
    // Initialize GLFW
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);  // Using older OpenGL for fixed-function pipeline
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    
    // Create window
    window = glfwCreateWindow(width, height, "Pinball Game - Simple Renderer", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    
    glfwMakeContextCurrent(window);
    
    // Set up callback functions
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int w, int h) {
        SimpleRenderer* renderer = static_cast<SimpleRenderer*>(glfwGetWindowUserPointer(window));
        glViewport(0, 0, w, h);
        renderer->width = w;
        renderer->height = h;
    });
}

SimpleRenderer::~SimpleRenderer() {
    cleanup();
}

void SimpleRenderer::initialize() {
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Failed to initialize GLEW");
    }
    
    initOpenGL();
    createDefaultScene();
}

void SimpleRenderer::cleanup() {
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

void SimpleRenderer::initOpenGL() {
    // Enable depth testing for 3D
    glEnable(GL_DEPTH_TEST);
    
    // Enable blending for transparency if needed
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Set up projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / height, 0.1, 100.0);
    
    // Set up modelview matrix
    glMatrixMode(GL_MODELVIEW);
    
    // Set clear color
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    
    // Enable lighting for better visuals
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    GLfloat light_pos[] = { 5.0f, 5.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
}

void SimpleRenderer::createDefaultScene() {
    // Create a ball
    balls.emplace_back(glm::vec3(0.0f, 0.5f, 0.0f), 0.05f);
    balls[0].setVelocity(glm::vec3(0.5f, 2.0f, 0.0f));
    
    // Create paddles
    paddles.emplace_back(glm::vec3(-0.8f, -1.5f, 0.0f), glm::vec3(0.0f));
    paddles.emplace_back(glm::vec3(0.8f, -1.5f, 0.0f), glm::vec3(0.0f));
    
    // Set up camera
    camera = std::make_unique<Camera>(glm::vec3(0.0f, 2.0f, 3.0f));
}

void SimpleRenderer::update(float deltaTime) {
    // Update physics only
    updatePhysics(deltaTime);
    
    // Poll events
    glfwPollEvents();
}

void SimpleRenderer::render() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set up camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Apply camera transformations
    gluLookAt(
        camera->getPosition().x, camera->getPosition().y, camera->getPosition().z,
        0.0, 0.0, 0.0,  // Look at center
        0.0, 1.0, 0.0   // Up vector
    );
    
    // Render the scene
    renderTable();
    renderPaddles();
    renderObstacles();
    
    for (auto& ball : balls) {
        renderBall(ball);
    }
    
    // Swap buffers
    glfwSwapBuffers(window);
}

void SimpleRenderer::updatePhysics(float deltaTime) {
    // Update all balls
    for (auto& ball : balls) {
        updateBall(ball, deltaTime);
    }
    
    // Check for collisions
    checkCollisions();
}

void SimpleRenderer::updateBall(Ball& ball, float deltaTime) {
    // Apply gravity
    glm::vec3 acceleration = gravity;
    
    // Update velocity
    glm::vec3 newVelocity = ball.getVelocity() + acceleration * deltaTime;
    ball.setVelocity(newVelocity);
    
    // Update position
    glm::vec3 newPosition = ball.getPosition() + ball.getVelocity() * deltaTime + 
                            0.5f * acceleration * deltaTime * deltaTime;
    ball.setPosition(newPosition);
    
    // Handle wall collisions
    handleWallCollisions(ball);
}

void SimpleRenderer::handleWallCollisions(Ball& ball) {
    glm::vec3 pos = ball.getPosition();
    float radius = ball.getRadius();
    
    // Left and right walls
    if (pos.x - radius <= -tableWidth/2) {
        pos.x = -tableWidth/2 + radius;
        ball.setVelocity(glm::vec3(-ball.getVelocity().x * restitution, ball.getVelocity().y, ball.getVelocity().z));
    } else if (pos.x + radius >= tableWidth/2) {
        pos.x = tableWidth/2 - radius;
        ball.setVelocity(glm::vec3(-ball.getVelocity().x * restitution, ball.getVelocity().y, ball.getVelocity().z));
    }
    
    // Front and back walls
    if (pos.z - radius <= -tableDepth/2) {
        pos.z = -tableDepth/2 + radius;
        ball.setVelocity(glm::vec3(ball.getVelocity().x, ball.getVelocity().y, -ball.getVelocity().z * restitution));
    } else if (pos.z + radius >= tableDepth/2) {
        pos.z = tableDepth/2 - radius;
        ball.setVelocity(glm::vec3(ball.getVelocity().x, ball.getVelocity().y, -ball.getVelocity().z * restitution));
    }
    
    // Floor (bottom)
    if (pos.y - radius <= -tableHeight/2) {
        pos.y = -tableHeight/2 + radius;
        ball.setVelocity(glm::vec3(ball.getVelocity().x, -ball.getVelocity().y * restitution, ball.getVelocity().z));
    }
    
    // Update position after collision handling
    ball.setPosition(pos);
}

void SimpleRenderer::checkCollisions() {
    // Simple implementation - balls with walls only
    // In a full implementation, you'd check ball-to-ball, ball-to-obstacle, ball-to-paddle collisions
    for (auto& ball : balls) {
        handleWallCollisions(ball);
    }
}

void SimpleRenderer::renderTable() {
    // Draw the table as a simple rectangle
    glPushMatrix();
    glTranslatef(0.0f, -tableHeight/4, 0.0f);  // Position the table
    glScalef(tableWidth, 0.1f, tableDepth);    // Scale to table size
    
    // Set material properties for the table
    GLfloat tableColor[] = {0.1f, 0.4f, 0.1f, 1.0f};  // Dark green
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, tableColor);
    
    // Draw the table surface
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
    glEnd();
    glPopMatrix();
    
    // Draw table sides/borders
    glPushMatrix();
    glTranslatef(0.0f, -tableHeight/4 + 0.05f, 0.0f);  // Slightly above the table surface
    
    // Draw left border
    glPushMatrix();
    glTranslatef(-tableWidth/2 - 0.05f, 0.0f, 0.0f);
    glScalef(0.1f, 0.2f, tableDepth + 0.2f);
    GLfloat borderColor[] = {0.7f, 0.7f, 0.7f, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, borderColor);
    glBegin(GL_QUADS);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
    glEnd();
    glPopMatrix();
    
    // Draw right border
    glPushMatrix();
    glTranslatef(tableWidth/2 + 0.05f, 0.0f, 0.0f);
    glScalef(0.1f, 0.2f, tableDepth + 0.2f);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, borderColor);
    glBegin(GL_QUADS);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
    glEnd();
    glPopMatrix();
    
    // Draw front border
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -tableDepth/2 - 0.05f);
    glScalef(tableWidth + 0.2f, 0.2f, 0.1f);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, borderColor);
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
    glEnd();
    glPopMatrix();
    
    // Draw back border
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, tableDepth/2 + 0.05f);
    glScalef(tableWidth + 0.2f, 0.2f, 0.1f);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, borderColor);
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
    glEnd();
    glPopMatrix();
    
    glPopMatrix();
}

void SimpleRenderer::renderBall(const Ball& ball) {
    glPushMatrix();
    glm::vec3 pos = ball.getPosition();
    glTranslatef(pos.x, pos.y, pos.z);
    
    // Set material properties for the ball
    GLfloat ballColor[] = {0.8f, 0.1f, 0.1f, 1.0f};  // Red
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ballColor);
    
    // Draw sphere using quadrics for simple renderer
    GLUquadric* quadric = gluNewQuadric();
    gluSphere(quadric, ball.getRadius(), 16, 16);
    gluDeleteQuadric(quadric);
    
    glPopMatrix();
}

void SimpleRenderer::renderPaddles() {
    for (size_t i = 0; i < paddles.size(); i++) {
        glPushMatrix();
        glm::vec3 pos = paddles[i].getPosition();
        glTranslatef(pos.x, pos.y, pos.z);
        
        // Paddle color - different for left and right
        GLfloat paddleColor[] = {0.4f, 0.4f, 0.8f, 1.0f};  // Blue
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, paddleColor);
        
        // Draw paddle as a rectangular box
        glScalef(0.3f, 0.05f, 0.1f);  // Width, height, depth
        glBegin(GL_QUADS);
            // Top face
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(-0.5f, 0.5f, -0.5f);
            glVertex3f(0.5f, 0.5f, -0.5f);
            glVertex3f(0.5f, 0.5f, 0.5f);
            glVertex3f(-0.5f, 0.5f, 0.5f);
            
            // Bottom face
            glNormal3f(0.0f, -1.0f, 0.0f);
            glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f(-0.5f, -0.5f, 0.5f);
            glVertex3f(0.5f, -0.5f, 0.5f);
            glVertex3f(0.5f, -0.5f, -0.5f);
            
            // Front face
            glNormal3f(0.0f, 0.0f, 1.0f);
            glVertex3f(-0.5f, -0.5f, 0.5f);
            glVertex3f(0.5f, -0.5f, 0.5f);
            glVertex3f(0.5f, 0.5f, 0.5f);
            glVertex3f(-0.5f, 0.5f, 0.5f);
            
            // Back face
            glNormal3f(0.0f, 0.0f, -1.0f);
            glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f(-0.5f, 0.5f, -0.5f);
            glVertex3f(0.5f, 0.5f, -0.5f);
            glVertex3f(0.5f, -0.5f, -0.5f);
            
            // Left face
            glNormal3f(-1.0f, 0.0f, 0.0f);
            glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f(-0.5f, -0.5f, 0.5f);
            glVertex3f(-0.5f, 0.5f, 0.5f);
            glVertex3f(-0.5f, 0.5f, -0.5f);
            
            // Right face
            glNormal3f(1.0f, 0.0f, 0.0f);
            glVertex3f(0.5f, -0.5f, -0.5f);
            glVertex3f(0.5f, 0.5f, -0.5f);
            glVertex3f(0.5f, 0.5f, 0.5f);
            glVertex3f(0.5f, -0.5f, 0.5f);
        glEnd();
        
        glPopMatrix();
    }
}

void SimpleRenderer::renderObstacles() {
    // Render some simple bumpers as spheres
    GLfloat bumperColor[] = {0.8f, 0.8f, 0.2f, 1.0f};  // Yellow
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bumperColor);
    
    // Bumper 1
    glPushMatrix();
    glTranslatef(-0.5f, -0.5f, 0.0f);
    GLUquadric* quadric = gluNewQuadric();
    gluSphere(quadric, 0.15f, 16, 16);
    gluDeleteQuadric(quadric);
    glPopMatrix();
    
    // Bumper 2
    glPushMatrix();
    glTranslatef(0.5f, -0.5f, 0.0f);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bumperColor);
    glPushMatrix();
    glTranslatef(0.5f, -0.5f, 0.0f);
    quadric = gluNewQuadric();
    gluSphere(quadric, 0.15f, 16, 16);
    gluDeleteQuadric(quadric);
    glPopMatrix();
    
    // Bumper 3
    glPushMatrix();
    glTranslatef(0.0f, -0.8f, 0.5f);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bumperColor);
    glPushMatrix();
    glTranslatef(0.0f, -0.8f, 0.5f);
    quadric = gluNewQuadric();
    gluSphere(quadric, 0.15f, 16, 16);
    gluDeleteQuadric(quadric);
    glPopMatrix();
}