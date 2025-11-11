#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include "../Keen.h"
#include <vector>
#include <string>

// Structure to represent a bitmap/image
struct Bitmap {
    int width;
    int height;
    int bytesPerRow;
    std::vector<byte> data;
    byte palette[768]; // 256 colors * 3 bytes (RGB)
    
    Bitmap() : width(0), height(0), bytesPerRow(0), data() {
        memset(palette, 0, sizeof(palette));
    }
    
    void Resize(int w, int h) {
        width = w;
        height = h;
        bytesPerRow = w; // For now, assuming 1 byte per pixel
        data.resize(w * h);
    }
};

class GraphicsSystem {
private:
    static bool initialized;
    static std::vector<Bitmap> loadedBitmaps;
    
public:
    static bool Initialize();
    static void Shutdown();
    
    // Load a bitmap from file
    static bool LoadBitmap(const std::string& filename, Bitmap& bitmap);
    
    // Save a bitmap to file
    static bool SaveBitmap(const std::string& filename, const Bitmap& bitmap);
    
    // Extract bitmap from original Commander Keen data files
    static bool ExtractBitmapFromKeenData(const std::string& dataFile, int bitmapId, Bitmap& bitmap);
    
    // Create a new bitmap
    static Bitmap* CreateBitmap(int width, int height);
    
    // Free a bitmap
    static void FreeBitmap(Bitmap* bitmap);
    
    // Convert legacy Keen bitmap format to modern format
    static bool ConvertKeenBitmap(Bitmap& bitmap);
    
    static bool IsInitialized() { return initialized; }
};

// Include texture management for OpenGL rendering
#include "TextureManager.h"

// Include resource pooling system
#include "ResourcePool.h"

// Include sprite and animation system
#include "SpriteAnimation.h"

#endif // GRAPHICS_SYSTEM_H