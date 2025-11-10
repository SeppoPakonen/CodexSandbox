#pragma once

#include <memory>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "ball.h"
#include "paddle.h"
#include "camera.h"

class SimpleRenderer {
public:
    SimpleRenderer(int width, int height);
    ~SimpleRenderer();
    
    void initialize();
    void cleanup();
    
    void update(float deltaTime);
    void render();
    
    // Physics simulation
    void updatePhysics(float deltaTime);
    void checkCollisions();
    
    // Getters/setters
    GLFWwindow* getWindow() const { return window; }
    bool shouldClose() const { return glfwWindowShouldClose(window); }
    
private:
    GLFWwindow* window;
    int width, height;
    
    // Scene objects
    std::vector<Ball> balls;
    std::vector<Paddle> paddles;  // These will be represented differently in the simple renderer
    std::unique_ptr<Camera> camera;
    
    // Physics parameters
    glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);
    float restitution = 0.8f;  // Bounciness
    
    // Pinball table boundaries
    float tableWidth = 2.0f;
    float tableHeight = 4.0f;
    float tableDepth = 1.0f;
    
    void initOpenGL();
    void createDefaultScene();
    void renderTable();
    void renderBall(const Ball& ball);
    void renderPaddles();
    void renderObstacles();
    
    // Physics functions
    void updateBall(Ball& ball, float deltaTime);
    void handleWallCollisions(Ball& ball);
    void handlePaddleCollision(Ball& ball, Paddle& paddle);
};