#include "GraphicsSystem.h"
#include "../core/FileSystem.h"

// Static member definitions
bool GraphicsSystem::initialized = false;
std::vector<Bitmap> GraphicsSystem::loadedBitmaps;

bool GraphicsSystem::Initialize() {
    if (initialized) return false;
    
    initialized = true;
    return true;
}

void GraphicsSystem::Shutdown() {
    if (!initialized) return;
    
    // Clean up all loaded bitmaps
    loadedBitmaps.clear();
    
    initialized = false;
}

bool GraphicsSystem::LoadBitmap(const std::string& filename, Bitmap& bitmap) {
    if (!initialized) return false;
    
    std::vector<byte> fileData;
    if (!FileSystem::ReadFile(filename, fileData)) {
        return false;
    }
    
    // This is a simplified implementation - in a real scenario, we would parse
    // different image formats (PCX, BMP, etc.) based on the file extension
    // For now, we'll assume it's raw data with a simple header
    
    if (fileData.size() < 8) {
        return false; // File too small to be valid
    }
    
    // Simple format: first 4 bytes = width, next 4 bytes = height
    int width = *(int*)&fileData[0];
    int height = *(int*)&fileData[4];
    
    bitmap.Resize(width, height);
    
    // Copy pixel data (skip header)
    size_t pixelDataSize = width * height;
    if (fileData.size() >= 8 + pixelDataSize) {
        std::copy(fileData.begin() + 8, fileData.begin() + 8 + pixelDataSize, bitmap.data.begin());
    }
    
    return true;
}

bool GraphicsSystem::SaveBitmap(const std::string& filename, const Bitmap& bitmap) {
    if (!initialized) return false;
    
    std::vector<byte> fileData;
    
    // Create header: width and height as 4-byte integers
    int width = bitmap.width;
    int height = bitmap.height;
    
    fileData.resize(8 + bitmap.data.size());
    
    // Write width and height
    memcpy(&fileData[0], &width, sizeof(int));
    memcpy(&fileData[4], &height, sizeof(int));
    
    // Write pixel data
    std::copy(bitmap.data.begin(), bitmap.data.end(), fileData.begin() + 8);
    
    return FileSystem::WriteFile(filename, fileData);
}

bool GraphicsSystem::ExtractBitmapFromKeenData(const std::string& dataFile, int bitmapId, Bitmap& bitmap) {
    if (!initialized) return false;
    
    // This function would extract bitmaps from original Keen data files
    // such as EGAGRAPH.CK? or MAPHEAD.CK?
    // The actual implementation would depend on the specific format of 
    // the original Commander Keen data files
    
    std::vector<byte> fileData;
    if (!FileSystem::ReadFile(dataFile, fileData)) {
        return false;
    }
    
    // Placeholder implementation - needs to be implemented based on actual Keen data format
    // This is where we would parse the original data file format
    
    // For now, create a simple test bitmap
    bitmap.Resize(32, 32);
    for (int i = 0; i < 32 * 32; i++) {
        bitmap.data[i] = i % 256; // Fill with some pattern
    }
    
    // Create a simple palette (this would be extracted from the original game)
    for (int i = 0; i < 256; i++) {
        bitmap.palette[i * 3] = (i * 4) % 256;       // Red
        bitmap.palette[i * 3 + 1] = (i * 2) % 256;   // Green
        bitmap.palette[i * 3 + 2] = (i * 3) % 256;   // Blue
    }
    
    return true;
}

Bitmap* GraphicsSystem::CreateBitmap(int width, int height) {
    if (!initialized) return nullptr;
    
    // Create a new bitmap and add it to our collection
    loadedBitmaps.emplace_back();
    Bitmap& newBitmap = loadedBitmaps.back();
    newBitmap.Resize(width, height);
    
    return &newBitmap;
}

void GraphicsSystem::FreeBitmap(Bitmap* bitmap) {
    if (!initialized || !bitmap) return;
    
    // In a real implementation, we'd need a more sophisticated way to manage bitmaps
    // For now, we're using a simple vector approach, so we can't easily remove specific bitmaps
    // without invalidating pointers
}

bool GraphicsSystem::ConvertKeenBitmap(Bitmap& bitmap) {
    // Convert legacy Commander Keen bitmap format to modern format
    // This would handle things like palette remapping, format conversion, etc.
    
    // Placeholder implementation
    return true;
}