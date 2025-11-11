#include "ConfigManager.h"

// Static member definitions
bool ConfigManager::initialized = false;
std::map<std::string, std::string> ConfigManager::configValues;

bool ConfigManager::Initialize() {
    if (initialized) return false;
    
    initialized = true;
    return true;
}

void ConfigManager::Shutdown() {
    configValues.clear();
    initialized = false;
}

bool ConfigManager::LoadConfig(const std::string& filename) {
    if (!initialized) return false;
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    configValues.clear();
    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#' || line[0] == ';') {
            continue;
        }
        
        // Find the '=' delimiter
        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos) {
            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);
            
            // Trim whitespace
            key.erase(0, key.find_first_not_of(" \t\r\n"));
            key.erase(key.find_last_not_of(" \t\r\n") + 1);
            value.erase(0, value.find_first_not_of(" \t\r\n"));
            value.erase(value.find_last_not_of(" \t\r\n") + 1);
            
            configValues[key] = value;
        }
    }
    
    file.close();
    return true;
}

bool ConfigManager::SaveConfig(const std::string& filename) {
    if (!initialized) return false;
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    for (const auto& pair : configValues) {
        file << pair.first << "=" << pair.second << std::endl;
    }
    
    file.close();
    return true;
}

void ConfigManager::SetValue(const std::string& key, const std::string& value) {
    if (initialized) {
        configValues[key] = value;
    }
}

void ConfigManager::SetValue(const std::string& key, int value) {
    if (initialized) {
        configValues[key] = std::to_string(value);
    }
}

void ConfigManager::SetValue(const std::string& key, float value) {
    if (initialized) {
        configValues[key] = std::to_string(value);
    }
}

void ConfigManager::SetValue(const std::string& key, bool value) {
    if (initialized) {
        configValues[key] = value ? "true" : "false";
    }
}

std::string ConfigManager::GetString(const std::string& key, const std::string& defaultValue) {
    if (!initialized) return defaultValue;
    
    auto it = configValues.find(key);
    return (it != configValues.end()) ? it->second : defaultValue;
}

int ConfigManager::GetInt(const std::string& key, int defaultValue) {
    if (!initialized) return defaultValue;
    
    auto it = configValues.find(key);
    if (it != configValues.end()) {
        try {
            return std::stoi(it->second);
        } catch (...) {
            return defaultValue;
        }
    }
    return defaultValue;
}

float ConfigManager::GetFloat(const std::string& key, float defaultValue) {
    if (!initialized) return defaultValue;
    
    auto it = configValues.find(key);
    if (it != configValues.end()) {
        try {
            return std::stof(it->second);
        } catch (...) {
            return defaultValue;
        }
    }
    return defaultValue;
}

bool ConfigManager::GetBool(const std::string& key, bool defaultValue) {
    if (!initialized) return defaultValue;
    
    auto it = configValues.find(key);
    if (it != configValues.end()) {
        const std::string& value = it->second;
        if (value == "true" || value == "TRUE" || value == "1") {
            return true;
        } else if (value == "false" || value == "FALSE" || value == "0") {
            return false;
        }
    }
    return defaultValue;
}