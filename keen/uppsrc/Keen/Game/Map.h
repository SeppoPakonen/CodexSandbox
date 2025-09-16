// Simple map skeleton for gameplay porting
#pragma once

#include <cstdint>
#include <vector>

namespace Game {

class Map {
public:
    bool LoadUser(const char* filename);
    int Width() const { return width_; }
    int Height() const { return height_; }
    const std::vector<uint16_t>& Tiles() const { return tiles_; }
private:
    int width_ = 0;
    int height_ = 0;
    std::vector<uint16_t> tiles_;
};

}

