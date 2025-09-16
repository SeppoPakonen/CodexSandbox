#pragma once

// Minimal preferences API (stub). Engine-agnostic persistent key/value cache.

struct ManiaPrefs {
    void   Clear();
    void   Set(const String& key, const String& val);
    void   Set(const String& key, int val);
    void   Set(const String& key, double val);
    bool   Has(const String& key) const;
    String Get(const String& key, const String& def = String()) const;
    int    GetInt(const String& key, int def = 0) const;
    double GetDouble(const String& key, double def = 0) const;
    void   Load(const String& path);
    void   Save(const String& path) const;

    // Optional namespace prefix and obfuscation
    void   SetNamespace(const String& ns_) { ns = ns_; }
    void   SetObfuscate(bool enabled, const String& salt_ = String()) { obf = enabled; salt = salt_; }
    bool   IsObfuscated() const { return obf; }

private:
    Index<String> keys;
    Vector<String> values;
    String ns;
    bool   obf = false;
    String salt;
};

extern ManiaPrefs Prefs;
