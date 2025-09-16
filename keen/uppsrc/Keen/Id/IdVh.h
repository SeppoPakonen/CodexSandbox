// Minimal VH-like tileset interface
#pragma once


namespace Video { class Screen; }

namespace VH {

class Tileset {
public:
    bool LoadPAL8(const char* filename);
    bool IsValid() const { return w > 0 && h > 0 && !indices.empty() && !palette.empty(); }
    bool DrawTile(Video::Screen& s, int tileIndex, int dx, int dy, int tileW = 8, int tileH = 8, uint8_t transparent = 0) const;
    int w = 0, h = 0; // sheet dimensions
    std::vector<uint32_t> palette;
    std::vector<uint8_t> indices;
};

}

