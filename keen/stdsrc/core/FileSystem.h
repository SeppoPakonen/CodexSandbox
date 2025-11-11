#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include "../Keen.h"
#include <vector>

class FileSystem {
private:
    static bool initialized;
    static std::vector<std::string> searchPaths;
    
public:
    static bool Initialize();
    static void Shutdown();
    
    // Add a directory to the search path
    static void AddSearchPath(const std::string& path);
    
    // Find a file in the search paths
    static std::string FindFile(const std::string& filename);
    
    // File operations
    static bool FileExists(const std::string& filepath);
    static size_t GetFileSize(const std::string& filepath);
    static bool ReadFile(const std::string& filepath, std::vector<byte>& buffer);
    static bool WriteFile(const std::string& filepath, const std::vector<byte>& buffer);
    
    // Directory operations
    static bool DirectoryExists(const std::string& dirpath);
    static bool CreateDirectory(const std::string& dirpath);
    
    static bool IsInitialized() { return initialized; }
};

#endif // FILE_SYSTEM_H