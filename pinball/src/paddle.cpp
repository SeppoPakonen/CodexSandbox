#include "paddle.h"

Paddle::Paddle(glm::vec3 position, glm::vec3 rotation) 
    : position(position), rotation(0.0f) {
}

void Paddle::update(float deltaTime) {
    // Update paddle state based on input or game logic
    // This is a simplified implementation
}

void Paddle::setRotation(float rotation) {
    this->rotation = rotation;
}