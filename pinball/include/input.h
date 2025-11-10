#pragma once

#include <GLFW/glfw3.h>

class Input {
public:
    static bool isKeyPressed(GLFWwindow* window, int key);
    static bool isMouseButtonPressed(GLFWwindow* window, int button);
    static void getMousePosition(GLFWwindow* window, double& xpos, double& ypos);
    
private:
    // This class is made up of static methods only
};