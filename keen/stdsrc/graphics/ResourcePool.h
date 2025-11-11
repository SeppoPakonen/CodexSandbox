#ifndef RESOURCE_POOL_H
#define RESOURCE_POOL_H

#include "../Keen.h"
#include <unordered_map>
#include <memory>
#include <functional>

// Include OpenGL headers for texture format types
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

// Forward declarations for types from other modules
class GraphicsResource;

// Resource handle for safe access to pooled resources
template<typename T>
class ResourceHandle {
private:
    T* resource;
    std::function<void(T*)> releaseCallback;  // To notify the pool when handle is destroyed
    
public:
    ResourceHandle(T* res, std::function<void(T*)> callback) 
        : resource(res), releaseCallback(callback) {}
    
    ~ResourceHandle() {
        if (resource && releaseCallback) {
            releaseCallback(resource);
        }
    }
    
    // Move constructor
    ResourceHandle(ResourceHandle&& other) noexcept 
        : resource(other.resource), releaseCallback(std::move(other.releaseCallback)) {
        other.resource = nullptr;
    }
    
    // Move assignment operator
    ResourceHandle& operator=(ResourceHandle&& other) noexcept {
        if (this != &other) {
            if (resource && releaseCallback) {
                releaseCallback(resource);
            }
            resource = other.resource;
            releaseCallback = std::move(other.releaseCallback);
            other.resource = nullptr;
        }
        return *this;
    }
    
    // Copy operations are disabled to enforce RAII with raw pointers
    ResourceHandle(const ResourceHandle&) = delete;
    ResourceHandle& operator=(const ResourceHandle&) = delete;
    
    T* operator->() const { return resource; }
    T& operator*() const { return *resource; }
    T* get() const { return resource; }
    
    explicit operator bool() const { return resource != nullptr; }
};

class ResourcePool {
private:
    static bool initialized;
    static std::unordered_map<std::string, std::unique_ptr<GraphicsResource>> resourceMap;
    static std::unordered_map<GraphicsResource*, int> referenceCounts;
    
public:
    static bool Initialize();
    static void Shutdown();
    
    // Register a resource with the pool
    static bool RegisterResource(const std::string& name, GraphicsResource* resource);
    
    // Acquire a resource (increment reference count)
    static ResourceHandle<GraphicsResource> AcquireResource(const std::string& name);
    
    // Release a resource (decrement reference count)
    static void ReleaseResource(GraphicsResource* resource);
    
    // Create a texture resource
    static ResourceHandle<GraphicsResource> CreateTextureResource(const std::string& name, int width, int height, GLenum format = GL_RGB);
    
    // Create a bitmap resource
    static ResourceHandle<GraphicsResource> CreateBitmapResource(const std::string& name, int width, int height);
    
    // Remove a resource from the pool
    static bool RemoveResource(const std::string& name);
    
    // Check if a resource exists in the pool
    static bool HasResource(const std::string& name);
    
    static bool IsInitialized() { return initialized; }
};

// Base class for all graphics resources
class GraphicsResource {
protected:
    std::string name;
    bool loaded;
    
public:
    GraphicsResource(const std::string& n) : name(n), loaded(false) {}
    virtual ~GraphicsResource() = default;
    
    virtual void Load() = 0;
    virtual void Unload() = 0;
    virtual bool IsLoaded() const { return loaded; }
    const std::string& GetName() const { return name; }
};

// Bitmap resource implementation
class BitmapResource : public GraphicsResource {
private:
    Bitmap bitmap;
    int width, height;
    
public:
    BitmapResource(const std::string& name, int w, int h)
        : GraphicsResource(name), width(w), height(h) {}
    
    void Load() override;
    void Unload() override;
    
    Bitmap* GetBitmap() { return &bitmap; }
};

#endif // RESOURCE_POOL_H