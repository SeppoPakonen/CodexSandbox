// Minimal configuration manager (C API preserved)
#pragma once

#include <stdbool.h>

typedef struct CFG_Variable {
    const char *name;
    const char *str_value;
    int int_value;
    bool is_boolean;
    bool saved;
} CFG_Variable;

// Note: C CFG_* API removed in favor of Config::Store class below.

// C++ wrapper API
namespace Config {
class Store {
public:
    static Store& Instance();

    void Load(const char* filename);
    void Save(const char* filename);

    bool Exists(const char* name) const;
    int GetInt(const char* name, int def) const;
    const char* GetString(const char* name, const char* def) const;
    bool GetBool(const char* name, bool def) const;
    int GetEnum(const char* name, const char** strings, int def) const;

    void SetInt(const char* name, int value);
    void SetString(const char* name, const char* value);
    void SetBool(const char* name, bool value);
    void SetEnum(const char* name, const char** strings, int value);

    void Startup();
    void Shutdown();
};
}
