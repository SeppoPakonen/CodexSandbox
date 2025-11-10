#pragma once

#include <glm/glm.hpp>

class Ball {
public:
    Ball(glm::vec3 position, float radius);
    
    void update(float deltaTime);
    void setVelocity(glm::vec3 velocity);
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getVelocity() const { return velocity; }
    float getRadius() const { return radius; }
    
    void setPosition(glm::vec3 pos) { position = pos; }
    void setVelocity(float x, float y, float z) { velocity = glm::vec3(x, y, z); }
    void setVelocityX(float x) { velocity.x = x; }
    void setVelocityY(float y) { velocity.y = y; }
    void setVelocityZ(float z) { velocity.z = z; }
    
private:
    glm::vec3 position;
    glm::vec3 velocity;
    float radius;
};