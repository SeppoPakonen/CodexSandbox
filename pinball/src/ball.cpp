#include "ball.h"

Ball::Ball(glm::vec3 position, float radius) 
    : position(position), radius(radius), velocity(0.0f, 0.0f, 0.0f) {
}

void Ball::update(float deltaTime) {
    // Apply basic physics - this would be handled in the renderer now
    // The ball class is just a data container now
}

void Ball::setVelocity(glm::vec3 velocity) {
    this->velocity = velocity;
}