#include "ManiaCore.h"

NAMESPACE_UPP

ManiaPrefs Prefs;

void ManiaPrefs::Clear() {
    keys.Clear();
    values.Clear();
}

void ManiaPrefs::Set(const String& key, const String& val) {
    int i = keys.Find(key);
    if (i < 0) {
        keys.Add(key);
        values.Add(val);
    }
    else
        values[i] = val;
}

void ManiaPrefs::Set(const String& key, int val) {
    Set(key, AsString(val));
}

void ManiaPrefs::Set(const String& key, double val) {
    Set(key, DblStr(val));
}

bool ManiaPrefs::Has(const String& key) const {
    return keys.Find(key) >= 0;
}

String ManiaPrefs::Get(const String& key, const String& def) const {
    int i = keys.Find(key);
    return i < 0 ? def : values[i];
}

int ManiaPrefs::GetInt(const String& key, int def) const {
    int out;
    if (!ScanInt(Get(key), out))
        return def;
    return out;
}

double ManiaPrefs::GetDouble(const String& key, double def) const {
    double out;
    if (!ScanDouble(Get(key), out))
        return def;
    return out;
}

void ManiaPrefs::Load(const String& path) {
    Clear();
    String s;
    if (!LoadFile(path, s))
        return;
    Ini::Data ini;
    if (!Ini::Load(ini, s))
        return;
    for (int i = 0; i < ini.GetSectionCount(); ++i) {
        const Ini::Section& sec = ini.GetSection(i);
        for (int k = 0; k < sec.GetCount(); ++k) {
            Set(sec.GetKey(k), sec.GetValue(k));
        }
    }
}

void ManiaPrefs::Save(const String& path) const {
    Ini::Data ini;
    Ini::Section& sec = ini.AddSection("prefs");
    for (int i = 0; i < keys.GetCount(); ++i)
        sec.Add(keys[i], values[i]);
    String out;
    Ini::Save(ini, out);
    SaveFile(path, out);
}

END_UPP_NAMESPACE
