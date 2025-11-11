#include "SpriteAnimation.h"
#include "TextureManager.h"
#include "ResourcePool.h"  // Include the resource pool
#include <cmath>

// Static member definitions
bool AnimationSystem::initialized = false;
std::vector<Animation> AnimationSystem::animations;
std::unordered_map<std::string, size_t> AnimationSystem::animationMap;

Sprite::Sprite(const std::string& spriteName) 
    : name(spriteName), x(0), y(0), rotation(0), scaleX(1), scaleY(1), alpha(1) {
    // Initialize transformation properties
}

Sprite::~Sprite() {
    // No specific cleanup needed - resources are managed by the resource pool
}

void Sprite::Render() {
    // In a real implementation, this would use OpenGL to render the sprite
    // For now, we'll just output debug info
    
    // If we had a texture resource, we would bind it here
    if (!textureResourceName.empty()) {
        // Acquire the resource from the pool for rendering
        auto textureResource = ResourcePool::AcquireResource(textureResourceName);
        
        if (textureResource) {
            // In a real implementation, we would:
            // 1. Activate the texture
            // 2. Set up shader uniforms for position, rotation, scale, alpha
            // 3. Draw a quad with the texture applied
            // 4. Handle the transformation matrix
            
            // For now, just print some debug info
            printf("Rendering sprite '%s' with texture '%s' at (%.2f, %.2f) with rotation %.2f\n", 
                   name.c_str(), textureResourceName.c_str(), x, y, rotation);
        }
    }
}

bool AnimationSystem::Initialize() {
    if (initialized) return false;
    
    initialized = true;
    return true;
}

void AnimationSystem::Shutdown() {
    if (!initialized) return;
    
    animations.clear();
    animationMap.clear();
    
    initialized = false;
}

bool AnimationSystem::RegisterAnimation(const Animation& animation) {
    if (!initialized || animation.frames.empty()) return false;
    
    // Check if animation with this name already exists
    if (animationMap.find(animation.name) != animationMap.end()) {
        return false; // Animation already exists
    }
    
    // Add to animations vector
    animations.push_back(animation);
    size_t index = animations.size() - 1;
    
    // Map the name to the index
    animationMap[animation.name] = index;
    
    // Calculate duration for the new animation
    animations.back().CalculateDuration();
    
    return true;
}

bool AnimationSystem::PlayAnimation(Sprite* sprite, const std::string& animationName, float time) {
    if (!initialized || !sprite) return false;
    
    auto it = animationMap.find(animationName);
    if (it == animationMap.end()) {
        return false; // Animation not found
    }
    
    const Animation& anim = animations[it->second];
    
    // Get the current frame based on time
    const SpriteFrame& frame = anim.GetFrame(time);
    
    // Apply frame properties to sprite
    sprite->SetPosition(frame.x, frame.y);
    sprite->SetTextureResourceName(frame.textureName);
    
    return true;
}

void AnimationSystem::UpdateAnimation(Sprite* sprite, float deltaTime) {
    if (!initialized || !sprite) return;
    
    // In a real implementation, this would advance the animation
    // time and update the sprite's appearance accordingly
    (void)sprite;
    (void)deltaTime;
}

const Animation* AnimationSystem::GetAnimation(const std::string& name) {
    if (!initialized) return nullptr;
    
    auto it = animationMap.find(name);
    if (it == animationMap.end()) {
        return nullptr;
    }
    
    return &animations[it->second];
}