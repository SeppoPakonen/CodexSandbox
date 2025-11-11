#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "../Keen.h"
#include "GraphicsSystem.h"
#include <map>
#include <string>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

// Structure to represent an OpenGL texture
struct Texture {
    GLuint id;           // OpenGL texture ID
    int width;          // Width of the texture
    int height;         // Height of the texture
    GLenum format;      // Texture format (GL_RGB, GL_RGBA, etc.)
    bool loaded;        // Whether the texture is properly loaded
    
    Texture() : id(0), width(0), height(0), format(GL_RGB), loaded(false) {}
    
    void Destroy() {
        if (id != 0) {
            glDeleteTextures(1, &id);
            id = 0;
            loaded = false;
        }
    }
};

class TextureManager {
private:
    static bool initialized;
    static std::map<std::string, Texture> textures;
    
public:
    static bool Initialize();
    static void Shutdown();
    
    // Load a texture from a bitmap
    static bool LoadTextureFromBitmap(const std::string& name, const Bitmap& bitmap);
    
    // Load a texture from a file
    static bool LoadTextureFromFile(const std::string& name, const std::string& filename);
    
    // Create an empty texture
    static bool CreateTexture(const std::string& name, int width, int height, GLenum format = GL_RGB);
    
    // Bind a texture for rendering
    static bool BindTexture(const std::string& name);
    
    // Get a texture by name
    static Texture* GetTexture(const std::string& name);
    
    // Destroy a texture
    static void DestroyTexture(const std::string& name);
    
    // Generate mipmaps for a texture
    static bool GenerateMipmaps(const std::string& name);
    
    static bool IsInitialized() { return initialized; }
};

#endif // TEXTURE_MANAGER_H