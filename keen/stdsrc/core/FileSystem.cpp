#include "FileSystem.h"
#include <sys/stat.h>
#include <dirent.h>

// Static member definitions
bool FileSystem::initialized = false;
std::vector<std::string> FileSystem::searchPaths;

bool FileSystem::Initialize() {
    if (initialized) return false;
    
    initialized = true;
    return true;
}

void FileSystem::Shutdown() {
    searchPaths.clear();
    initialized = false;
}

void FileSystem::AddSearchPath(const std::string& path) {
    if (initialized) {
        // Check if path already exists in search paths
        for (const auto& existingPath : searchPaths) {
            if (existingPath == path) {
                return; // Path already exists
            }
        }
        searchPaths.push_back(path);
    }
}

std::string FileSystem::FindFile(const std::string& filename) {
    if (!initialized) return "";
    
    // First check the current directory
    if (FileExists(filename)) {
        return filename;
    }
    
    // Search in all search paths
    for (const auto& path : searchPaths) {
        std::string fullPath = path + "/" + filename;
        if (FileExists(fullPath)) {
            return fullPath;
        }
    }
    
    return ""; // File not found
}

bool FileSystem::FileExists(const std::string& filepath) {
    if (!initialized) return false;
    
    struct stat buffer;
    return (stat(filepath.c_str(), &buffer) == 0);
}

size_t FileSystem::GetFileSize(const std::string& filepath) {
    if (!initialized) return 0;
    
    struct stat buffer;
    if (stat(filepath.c_str(), &buffer) == 0) {
        return static_cast<size_t>(buffer.st_size);
    }
    return 0;
}

bool FileSystem::ReadFile(const std::string& filepath, std::vector<byte>& buffer) {
    if (!initialized) return false;
    
    if (!FileExists(filepath)) {
        return false;
    }
    
    size_t fileSize = GetFileSize(filepath);
    buffer.resize(fileSize);
    
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    file.close();
    
    return true;
}

bool FileSystem::WriteFile(const std::string& filepath, const std::vector<byte>& buffer) {
    if (!initialized) return false;
    
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
    file.close();
    
    return true;
}

bool FileSystem::DirectoryExists(const std::string& dirpath) {
    if (!initialized) return false;
    
    struct stat buffer;
    return (stat(dirpath.c_str(), &buffer) == 0 && S_ISDIR(buffer.st_mode));
}

bool FileSystem::CreateDirectory(const std::string& dirpath) {
    if (!initialized) return false;
    
    if (DirectoryExists(dirpath)) {
        return true; // Directory already exists
    }
    
    // Create directory with system-specific function
    int result = mkdir(dirpath.c_str(), 0755); // 0755 gives rwxr-xr-x permissions
    return (result == 0);
}