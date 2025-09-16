// Simple sprite queue for drawing
#pragma once

namespace Video { class Screen; }

namespace Video {

struct Sprite {
    enum class Type { ARGB, PAL8_MASK } type = Type::ARGB;
    // Common
    int sx=0, sy=0, sw=0, sh=0;
    int dx=0, dy=0;
    // ARGB source
    const uint32_t* argb = nullptr; int srcW=0, srcH=0;
    // PAL8 source
    const uint8_t* pal8 = nullptr; int palW=0, palH=0; uint8_t transparent = 0;
};

class Queue {
public:
    void Clear();
    void AddARGB(const uint32_t* src, int srcW, int srcH, int sx, int sy, int sw, int sh, int dx, int dy);
    void AddPAL8Mask(const uint8_t* src, int srcW, int srcH, int sx, int sy, int sw, int sh, int dx, int dy, uint8_t transparent=0);
    void Render(Screen& screen);
private:
    std::vector<Sprite> items;
};

}

