#include "TextureManager.h"
#include "../core/FileSystem.h"

// Static member definitions
bool TextureManager::initialized = false;
std::map<std::string, Texture> TextureManager::textures;

bool TextureManager::Initialize() {
    if (initialized) return false;
    
    // Initialize OpenGL texture-related functionality
    // In a real implementation, we might need to ensure an OpenGL context exists
    
    initialized = true;
    return true;
}

void TextureManager::Shutdown() {
    if (!initialized) return;
    
    // Destroy all loaded textures
    for (auto& pair : textures) {
        pair.second.Destroy();
    }
    textures.clear();
    
    initialized = false;
}

bool TextureManager::LoadTextureFromBitmap(const std::string& name, const Bitmap& bitmap) {
    if (!initialized) return false;
    
    // Create a new texture or update existing one
    Texture texture;
    
    // Generate OpenGL texture ID
    glGenTextures(1, &texture.id);
    if (texture.id == 0) {
        return false; // Failed to generate texture ID
    }
    
    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, texture.id);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // Upload texture data
    // For now, we'll assume palette-based data and convert it to RGB
    std::vector<byte> rgbData;
    rgbData.reserve(bitmap.width * bitmap.height * 3);
    
    for (int i = 0; i < bitmap.width * bitmap.height; i++) {
        byte paletteIndex = bitmap.data[i];
        rgbData.push_back(bitmap.palette[paletteIndex * 3]);      // R
        rgbData.push_back(bitmap.palette[paletteIndex * 3 + 1]);  // G
        rgbData.push_back(bitmap.palette[paletteIndex * 3 + 2]);  // B
    }
    
    glTexImage2D(
        GL_TEXTURE_2D,    // target
        0,                // level
        GL_RGB,           // internal format
        bitmap.width,     // width
        bitmap.height,    // height
        0,                // border
        GL_RGB,           // format
        GL_UNSIGNED_BYTE, // type
        rgbData.data()    // data
    );
    
    // Check for OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        glDeleteTextures(1, &texture.id);
        return false;
    }
    
    // Set texture properties
    texture.width = bitmap.width;
    texture.height = bitmap.height;
    texture.format = GL_RGB;
    texture.loaded = true;
    
    // Store the texture
    textures[name] = texture;
    
    return true;
}

bool TextureManager::LoadTextureFromFile(const std::string& name, const std::string& filename) {
    if (!initialized) return false;
    
    // This would load a texture from various image formats
    // For now, we'll implement a simple approach using our bitmap system
    
    Bitmap bitmap;
    if (!GraphicsSystem::LoadBitmap(filename, bitmap)) {
        // Try to load the file as a bitmap through our graphics system
        // This assumes we have some way to load the file
        return false;
    }
    
    return LoadTextureFromBitmap(name, bitmap);
}

bool TextureManager::CreateTexture(const std::string& name, int width, int height, GLenum format) {
    if (!initialized) return false;
    
    Texture texture;
    
    // Generate OpenGL texture ID
    glGenTextures(1, &texture.id);
    if (texture.id == 0) {
        return false; // Failed to generate texture ID
    }
    
    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, texture.id);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // Create empty texture data
    glTexImage2D(
        GL_TEXTURE_2D,    // target
        0,                // level
        format,           // internal format
        width,            // width
        height,           // height
        0,                // border
        format,           // format
        GL_UNSIGNED_BYTE, // type
        nullptr           // data (empty)
    );
    
    // Check for OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        glDeleteTextures(1, &texture.id);
        return false;
    }
    
    // Set texture properties
    texture.width = width;
    texture.height = height;
    texture.format = format;
    texture.loaded = true;
    
    // Store the texture
    textures[name] = texture;
    
    return true;
}

bool TextureManager::BindTexture(const std::string& name) {
    if (!initialized) return false;
    
    auto it = textures.find(name);
    if (it == textures.end()) {
        return false; // Texture not found
    }
    
    if (!it->second.loaded) {
        return false; // Texture not properly loaded
    }
    
    glBindTexture(GL_TEXTURE_2D, it->second.id);
    return true;
}

Texture* TextureManager::GetTexture(const std::string& name) {
    if (!initialized) return nullptr;
    
    auto it = textures.find(name);
    if (it == textures.end()) {
        return nullptr; // Texture not found
    }
    
    return &it->second;
}

void TextureManager::DestroyTexture(const std::string& name) {
    if (!initialized) return;
    
    auto it = textures.find(name);
    if (it != textures.end()) {
        it->second.Destroy();
        textures.erase(it);
    }
}

bool TextureManager::GenerateMipmaps(const std::string& name) {
    if (!initialized) return false;
    
    auto it = textures.find(name);
    if (it == textures.end()) {
        return false; // Texture not found
    }
    
    if (!it->second.loaded) {
        return false; // Texture not properly loaded
    }
    
    glBindTexture(GL_TEXTURE_2D, it->second.id);
    
    // For now, we'll just set appropriate filtering that will use mipmaps
    // if they were already generated. In a real implementation we'd either:
    // 1. Ensure proper OpenGL headers are available
    // 2. Implement manual mipmap generation
    // 3. Use a graphics library like SOIL or stb_image
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    GLenum error = glGetError();
    return error == GL_NO_ERROR;
}