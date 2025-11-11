#ifndef SPRITE_ANIMATION_H
#define SPRITE_ANIMATION_H

#include "../Keen.h"
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <cmath>

// Forward declaration of GraphicsResource
class GraphicsResource;

// Structure to represent a sprite frame
struct SpriteFrame {
    float x, y;        // Position offset
    float width, height;  // Dimensions
    std::string textureName;  // Name of the texture resource
    float duration;    // Duration in seconds for this frame
    
    SpriteFrame() : x(0), y(0), width(32), height(32), duration(0.1f) {}
    SpriteFrame(float posX, float posY, float w, float h, const std::string& texName, float dur = 0.1f)
        : x(posX), y(posY), width(w), height(h), textureName(texName), duration(dur) {}
};

// Structure to represent an animation sequence
struct Animation {
    std::string name;
    std::vector<SpriteFrame> frames;
    float totalDuration;
    bool loop;
    
    Animation() : totalDuration(0), loop(true) {}
    
    void CalculateDuration() {
        totalDuration = 0;
        for (const auto& frame : frames) {
            totalDuration += frame.duration;
        }
    }
    
    const SpriteFrame& GetFrame(float time) const {
        if (frames.empty()) {
            static SpriteFrame emptyFrame;
            return emptyFrame;
        }
        
        if (loop) {
            time = fmodf(time, totalDuration);
        } else {
            if (time > totalDuration) time = totalDuration;
        }
        
        float currentTime = 0;
        for (const auto& frame : frames) {
            if (time < currentTime + frame.duration) {
                return frame;
            }
            currentTime += frame.duration;
        }
        
        // Return the last frame if time exceeds total duration
        return frames.back();
    }
};

// Class to manage sprites
class Sprite {
private:
    std::string name;
    std::string textureResourceName;  // Name of texture resource instead of handle
    float x, y;                    // Position
    float rotation;               // Rotation in degrees
    float scaleX, scaleY;         // Scale factors
    float alpha;                  // Transparency
    
public:
    Sprite(const std::string& spriteName);
    ~Sprite();
    
    // Set position
    void SetPosition(float newX, float newY) { x = newX; y = newY; }
    void SetX(float newX) { x = newX; }
    void SetY(float newY) { y = newY; }
    
    // Get position
    float GetX() const { return x; }
    float GetY() const { return y; }
    void GetPosition(float& outX, float& outY) const { outX = x; outY = y; }
    
    // Set transformation properties
    void SetRotation(float degrees) { rotation = degrees; }
    void SetScale(float xScale, float yScale) { scaleX = xScale; scaleY = yScale; }
    void SetAlpha(float alphaValue) { alpha = (alphaValue < 0.0f) ? 0.0f : ((alphaValue > 1.0f) ? 1.0f : alphaValue); }
    
    // Get transformation properties
    float GetRotation() const { return rotation; }
    void GetScale(float& outX, float& outY) const { outX = scaleX; outY = scaleY; }
    float GetAlpha() const { return alpha; }
    
    // Set/Get the texture resource name
    void SetTextureResourceName(const std::string& resourceName) { textureResourceName = resourceName; }
    const std::string& GetTextureResourceName() const { return textureResourceName; }
    
    // Render the sprite
    void Render();
    
    const std::string& GetName() const { return name; }
};

// Class to manage animations
class AnimationSystem {
private:
    static bool initialized;
    static std::vector<Animation> animations;
    static std::unordered_map<std::string, size_t> animationMap;
    
public:
    static bool Initialize();
    static void Shutdown();
    
    // Register an animation
    static bool RegisterAnimation(const Animation& animation);
    
    // Play an animation on a sprite
    static bool PlayAnimation(Sprite* sprite, const std::string& animationName, float time = 0.0f);
    
    // Update animation state
    static void UpdateAnimation(Sprite* sprite, float deltaTime);
    
    // Get an animation by name
    static const Animation* GetAnimation(const std::string& name);
    
    static bool IsInitialized() { return initialized; }
};

#endif // SPRITE_ANIMATION_H