#include "ResourcePool.h"
#include "TextureManager.h"  // Include TextureManager for the Texture type and TextureManager functions

// Static member definitions
bool ResourcePool::initialized = false;
std::unordered_map<std::string, std::unique_ptr<GraphicsResource>> ResourcePool::resourceMap;
std::unordered_map<GraphicsResource*, int> ResourcePool::referenceCounts;

bool ResourcePool::Initialize() {
    if (initialized) return false;
    
    initialized = true;
    return true;
}

void ResourcePool::Shutdown() {
    if (!initialized) return;
    
    // Unload and remove all resources
    for (auto& pair : resourceMap) {
        if (pair.second->IsLoaded()) {
            pair.second->Unload();
        }
    }
    
    resourceMap.clear();
    referenceCounts.clear();
    
    initialized = false;
}

bool ResourcePool::RegisterResource(const std::string& name, GraphicsResource* resource) {
    if (!initialized || !resource) return false;
    
    // Add the resource to the map
    resourceMap[name] = std::unique_ptr<GraphicsResource>(resource);
    
    // Initialize reference count to 0
    referenceCounts[resource] = 0;
    
    return true;
}

ResourceHandle<GraphicsResource> ResourcePool::AcquireResource(const std::string& name) {
    if (!initialized) return ResourceHandle<GraphicsResource>(nullptr, nullptr);
    
    auto it = resourceMap.find(name);
    if (it == resourceMap.end()) {
        return ResourceHandle<GraphicsResource>(nullptr, nullptr);
    }
    
    GraphicsResource* resource = it->second.get();
    
    // Increment reference count
    referenceCounts[resource]++;
    
    // Create a handle with a release callback
    auto releaseCallback = [](GraphicsResource* res) {
        if (ResourcePool::initialized) {
            auto refIt = referenceCounts.find(res);
            if (refIt != referenceCounts.end()) {
                refIt->second--;
                // Resource isn't destroyed here, just reference count decremented
            }
        }
    };
    
    return ResourceHandle<GraphicsResource>(resource, releaseCallback);
}

void ResourcePool::ReleaseResource(GraphicsResource* resource) {
    if (!initialized || !resource) return;
    
    auto it = referenceCounts.find(resource);
    if (it != referenceCounts.end()) {
        if (it->second > 0) {
            it->second--;
        }
    }
}

ResourceHandle<GraphicsResource> ResourcePool::CreateTextureResource(const std::string& name, int width, int height, GLenum format) {
    if (!initialized) return ResourceHandle<GraphicsResource>(nullptr, nullptr);
    
    // For texture resource, we can't directly create TextureResource because
    // it contains Texture type which is defined in TextureManager.h
    // Instead, we'll create a custom texture resource class that doesn't store
    // the Texture object directly but manages it through TextureManager
    class PooledTextureResource : public GraphicsResource {
    private:
        int width, height;
        GLenum format;
        
    public:
        PooledTextureResource(const std::string& n, int w, int h, GLenum fmt)
            : GraphicsResource(n), width(w), height(h), format(fmt) {}
        
        void Load() override {
            if (TextureManager::CreateTexture(name, width, height, format)) {
                loaded = true;
            }
        }
        
        void Unload() override {
            if (loaded) {
                TextureManager::DestroyTexture(name);
                loaded = false;
            }
        }
    };
    
    PooledTextureResource* textureRes = new PooledTextureResource(name, width, height, format);
    
    if (!RegisterResource(name, textureRes)) {
        delete textureRes;
        return ResourceHandle<GraphicsResource>(nullptr, nullptr);
    }
    
    // Load the texture resource
    textureRes->Load();
    
    return AcquireResource(name);
}

ResourceHandle<GraphicsResource> ResourcePool::CreateBitmapResource(const std::string& name, int width, int height) {
    if (!initialized) return ResourceHandle<GraphicsResource>(nullptr, nullptr);
    
    BitmapResource* bitmapRes = new BitmapResource(name, width, height);
    
    if (!RegisterResource(name, bitmapRes)) {
        delete bitmapRes;
        return ResourceHandle<GraphicsResource>(nullptr, nullptr);
    }
    
    // Load the bitmap resource
    bitmapRes->Load();
    
    return AcquireResource(name);
}

bool ResourcePool::RemoveResource(const std::string& name) {
    if (!initialized) return false;
    
    auto it = resourceMap.find(name);
    if (it == resourceMap.end()) {
        return false;
    }
    
    GraphicsResource* resource = it->second.get();
    auto refIt = referenceCounts.find(resource);
    
    // Don't remove if there are active references
    if (refIt != referenceCounts.end() && refIt->second > 0) {
        return false; // Resource is still in use
    }
    
    // Unload the resource if it was loaded
    if (resource->IsLoaded()) {
        resource->Unload();
    }
    
    // Remove from maps
    resourceMap.erase(it);
    if (refIt != referenceCounts.end()) {
        referenceCounts.erase(refIt);
    }
    
    return true;
}

bool ResourcePool::HasResource(const std::string& name) {
    if (!initialized) return false;
    
    return resourceMap.find(name) != resourceMap.end();
}

// BitmapResource implementation
void BitmapResource::Load() {
    // Create a basic bitmap structure
    bitmap.Resize(width, height);
    
    // Initialize with some default data
    for (int i = 0; i < width * height; i++) {
        bitmap.data[i] = i % 256; // Simple pattern
    }
    
    // Set up a basic palette
    for (int i = 0; i < 256; i++) {
        bitmap.palette[i * 3] = (i * 2) % 256;     // R
        bitmap.palette[i * 3 + 1] = (i * 3) % 256; // G
        bitmap.palette[i * 3 + 2] = (i * 4) % 256; // B
    }
    
    loaded = true;
}

void BitmapResource::Unload() {
    if (loaded) {
        // Clear the bitmap data
        bitmap.data.clear();
        loaded = false;
    }
}