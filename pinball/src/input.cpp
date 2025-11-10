#include "input.h"

bool Input::isKeyPressed(GLFWwindow* window, int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool Input::isMouseButtonPressed(GLFWwindow* window, int button) {
    return glfwGetMouseButton(window, button) == GLFW_PRESS;
}

void Input::getMousePosition(GLFWwindow* window, double& xpos, double& ypos) {
    glfwGetCursorPos(window, &xpos, &ypos);
}