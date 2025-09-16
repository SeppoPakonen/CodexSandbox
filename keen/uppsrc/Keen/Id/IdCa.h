// Minimal cache/asset loader inspired by id_ca
#pragma once


namespace CA {

struct Chunk {
    std::vector<uint8_t> data;
};

class Cache {
public:
    void Clear();
    bool LoadUser(const char* name, const char* filename);
    const Chunk* Get(const char* name) const;
private:
    std::unordered_map<std::string, Chunk> chunks_;
};

}

