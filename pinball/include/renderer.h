#pragma once

#include <memory>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "camera.h"

enum class RendererType {
    Simple,  // MdyGWG
    Advanced // MdyGDz
};

class Renderer {
public:
    Renderer(int width, int height, const std::string& title);
    ~Renderer();
    
    void initialize();
    void cleanup();
    
    void update(float deltaTime);
    void render();
    
    void setRendererType(RendererType type);
    RendererType getRendererType() const { return currentRendererType; }
    
    void setViewport(int width, int height);
    GLFWwindow* getWindow() const { return window; }
    
    // Shadertoy uniforms mapping
    void setResolution(float x, float y);
    void setTime(float time);
    void setTimeDelta(float deltaTime);
    void setMouse(float x, float y);
    void setFrame(int frame);
    
private:
    GLFWwindow* window;
    int width, height;
    RendererType currentRendererType;
    
    // Shaders for both renderers
    std::unique_ptr<Shader> simpleShader;
    std::unique_ptr<Shader> advancedShader;
    Shader* currentShader;
    
    // Camera
    std::unique_ptr<Camera> camera;
    
    // Uniform values
    float iResolution[2];
    float iTime = 0.0f;
    float iTimeDelta = 0.0f;
    float iMouse[2] = {0.0f, 0.0f};
    int iFrame = 0;
    
    // Quad VAO for full screen shader rendering
    GLuint quadVAO, quadVBO;
    
    void initOpenGL();
    void initShaders();
    void initQuad();
    void renderQuad();
};