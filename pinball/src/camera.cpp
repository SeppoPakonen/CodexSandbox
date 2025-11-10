#include "camera.h"
#include <iostream>

Camera::Camera(glm::vec3 position) 
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), 
      movementSpeed(2.5f), 
      mouseSensitivity(0.1f), 
      zoom(45.0f) {
    
    this->position = position;
    this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    this->yaw = -90.0f;
    this->pitch = 0.0f;
    
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) {
    return glm::perspective(glm::radians(zoom), aspectRatio, 0.1f, 100.0f);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    if (direction == CameraMovement::FORWARD)
        position += front * velocity;
    if (direction == CameraMovement::BACKWARD)
        position -= front * velocity;
    if (direction == CameraMovement::LEFT)
        position -= right * velocity;
    if (direction == CameraMovement::RIGHT)
        position += right * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset) {
    if (zoom >= 1.0f && zoom <= 45.0f)
        zoom -= yoffset;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}

void Camera::updateCameraVectors() {
    // Calculate the new Front vector
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    
    // Also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up = glm::normalize(glm::cross(right, front));
}