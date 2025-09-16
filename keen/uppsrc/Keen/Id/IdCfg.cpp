#include "Id.h"

NAMESPACE_UPP

namespace {
    struct CfgVar {
        std::string name;
        std::string str_value; // empty if integer/bool
        int int_value = 0;
        bool is_boolean = false;
        bool saved = false;
    };

    static std::unordered_map<std::string, CfgVar>& Vars() {
        static std::unordered_map<std::string, CfgVar> v;
        return v;
    }

    static inline int StrCaseCmp(const char* a, const char* b) {
    #if defined(_WIN32)
        return _stricmp(a, b);
    #else
        return strcasecmp(a, b);
    #endif
    }

    static CfgVar& GetOrCreate(const char* name) {
        auto& m = Vars();
        auto it = m.find(name);
        if (it != m.end()) return it->second;
        CfgVar v; v.name = name; v.str_value.clear(); v.int_value = 0; v.is_boolean = false; v.saved = false;
        auto res = m.emplace(v.name, std::move(v));
        return res.first->second;
    }
}

namespace Config {

Store& Store::Instance() { static Store s; return s; }

bool Store::Exists(const char *name) const { return Vars().find(name) != Vars().end(); }

int Store::GetInt(const char *name, int defValue) const {
    auto it = Vars().find(name);
    if (it == Vars().end()) return defValue;
    return it->second.int_value;
}

const char *Store::GetString(const char *name, const char *defValue) const {
    auto it = Vars().find(name);
    if (it == Vars().end()) return defValue;
    if (it->second.str_value.empty()) return defValue;
    return it->second.str_value.c_str();
}

bool Store::GetBool(const char *name, bool defValue) const {
    return GetInt(name, defValue ? 1 : 0) != 0;
}

int Store::GetEnum(const char *name, const char **strings, int defValue) const {
    const char* s = GetString(name, nullptr);
    if (!s) return defValue;
    for (int i = 0; strings[i]; ++i) {
        if (StrCaseCmp(strings[i], s) == 0)
            return i;
    }
    return defValue;
}

void Store::SetInt(const char *name, int value) {
    auto& v = GetOrCreate(name);
    v.str_value.clear();
    v.int_value = value;
    v.is_boolean = false;
}

void Store::SetString(const char *name, const char *value) {
    auto& v = GetOrCreate(name);
    if (!v.str_value.size() || v.str_value != value)
        v.str_value = value ? value : "";
    v.int_value = 0;
    v.is_boolean = false;
}

void Store::SetBool(const char *name, bool value) {
    auto& v = GetOrCreate(name);
    v.str_value.clear();
    v.int_value = value ? 1 : 0;
    v.is_boolean = true;
}

void Store::SetEnum(const char *name, const char **strings, int value) {
    SetString(name, strings[value]);
}

static bool ParseConfigLine(Str::Parser& parser) {
    auto nameTok = parser.GetToken();
    if (nameTok.inner.tokenType == STR_TOK_EOF)
        return false;
    char name[ID_STR_MAX_TOKEN_LENGTH];
    Str::GetStringValue(nameTok, name, ID_STR_MAX_TOKEN_LENGTH);
    if (!parser.Expect("="))
        return false;
    auto value = parser.GetToken();
    if (value.inner.tokenType == STR_TOK_EOF)
        return false;
    else if (value.inner.tokenType == STR_TOK_String) {
        char tokenBuf[ID_STR_MAX_TOKEN_LENGTH];
        Str::GetStringValue(value, tokenBuf, ID_STR_MAX_TOKEN_LENGTH);
        Store::Instance().SetString(name, tokenBuf);
    }
    else if (!StrCaseCmp("false", value.inner.valuePtr))
        Store::Instance().SetBool(name, false);
    else if (!StrCaseCmp("true", value.inner.valuePtr))
        Store::Instance().SetBool(name, true);
    else
        Store::Instance().SetInt(name, Str::GetIntegerValue(value));
    return true;
}

void Store::Load(const char *filename) {
    int numVarsParsed = 0;
    char* data = nullptr;
    int size = 0;
    if (!FS::ReadAllUser(filename, (mm_ptr_t*)(&data), &size))
        return;
    Str::Parser parser(data, size);
    while (ParseConfigLine(parser))
        numVarsParsed++;
    MM_FreePtr((mm_ptr_t*)&data);
    (void)numVarsParsed;
}

void Store::Save(const char *filename) {
    FS::File out = FS::File::CreateUser(filename);
    if (!out.IsValid())
        return;
    for (auto &kv : Vars()) {
        const auto& v = kv.second;
        if (!v.str_value.empty())
            out.PrintF("%s = \"%s\"\n", v.name.c_str(), v.str_value.c_str());
        else if (v.is_boolean)
            out.PrintF("%s = %s\n", v.name.c_str(), v.int_value ? "true" : "false");
        else
            out.PrintF("%s = %d\n", v.name.c_str(), v.int_value);
    }
}

void Store::Startup() {
    static bool started = false;
    if (started) return;
    Load("OMNISPK.CFG");
    started = true;
}

void Store::Shutdown() {
    static bool started = true;
    if (!started) return;
    Save("OMNISPK.CFG");
    started = false;
}

} // namespace Config

END_UPP_NAMESPACE
