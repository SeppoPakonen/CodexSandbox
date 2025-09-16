#include "Id.h"

NAMESPACE_UPP

namespace CA {

void Cache::Clear() { chunks_.clear(); }

bool Cache::LoadUser(const char* name, const char* filename) {
    if (!name || !filename) return false;
    mm_ptr_t buf = nullptr;
    int size = 0;
    if (!FS::ReadAllUser(filename, &buf, &size))
        return false;
    Chunk c;
    c.data.resize((size_t)size);
    std::memcpy(c.data.data(), buf, (size_t)size);
    MM_FreePtr(&buf);
    chunks_[name] = std::move(c);
    return true;
}

const Chunk* Cache::Get(const char* name) const {
    if (!name) return nullptr;
    auto it = chunks_.find(name);
    if (it == chunks_.end()) return nullptr;
    return &it->second;
}

}

END_UPP_NAMESPACE

