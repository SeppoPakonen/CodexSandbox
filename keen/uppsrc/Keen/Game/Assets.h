// Simple asset helpers for early bring-up
#pragma once

#include <cstdint>
#include <vector>

namespace Game {

struct Image {
    int w = 0;
    int h = 0;
    std::vector<uint32_t> pixels; // ARGB32
    bool IsValid() const { return w > 0 && h > 0 && !pixels.empty(); }
};

class Assets {
public:
    // Dummy format: [u16 w][u16 h][u32 ARGB pixels...]
    static bool LoadUserImageARGB32(const char* filename, Image& out);
    // Dummy paletted format: [u16 w][u16 h][256*u32 palette ARGB][u8 indices w*h]
    struct Pal8 { int w=0,h=0; std::vector<uint32_t> palette; std::vector<uint8_t> indices; bool IsValid() const { return w>0&&h>0&&!indices.empty()&&!palette.empty(); } };
    static bool LoadUserImagePAL8(const char* filename, Pal8& out);
};

}
