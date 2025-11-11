#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "../Keen.h"
#include <map>
#include <string>

class ConfigManager {
private:
    static bool initialized;
    static std::map<std::string, std::string> configValues;
    
public:
    static bool Initialize();
    static void Shutdown();
    
    static bool LoadConfig(const std::string& filename);
    static bool SaveConfig(const std::string& filename);
    
    // Set configuration values
    static void SetValue(const std::string& key, const std::string& value);
    static void SetValue(const std::string& key, int value);
    static void SetValue(const std::string& key, float value);
    static void SetValue(const std::string& key, bool value);
    
    // Get configuration values with default fallbacks
    static std::string GetString(const std::string& key, const std::string& defaultValue = "");
    static int GetInt(const std::string& key, int defaultValue = 0);
    static float GetFloat(const std::string& key, float defaultValue = 0.0f);
    static bool GetBool(const std::string& key, bool defaultValue = false);
    
    static bool IsInitialized() { return initialized; }
};

#endif // CONFIG_MANAGER_H