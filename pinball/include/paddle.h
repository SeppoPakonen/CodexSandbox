#pragma once

#include <glm/glm.hpp>

class Paddle {
public:
    Paddle(glm::vec3 position, glm::vec3 rotation);
    
    void update(float deltaTime);
    void setRotation(float rotation);
    glm::vec3 getPosition() const { return position; }
    float getRotation() const { return rotation; }
    
    void setPosition(glm::vec3 pos) { position = pos; }
    
private:
    glm::vec3 position;
    float rotation;
    float maxRotation = 0.5f;  // Max rotation in radians
    float rotationSpeed = 3.0f; // Rotation speed in radians per second
};