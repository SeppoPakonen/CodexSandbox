#include "Id.h"

NAMESPACE_UPP

namespace Video {

void Queue::Clear() { items.clear(); }

void Queue::AddARGB(const uint32_t* src, int srcW, int srcH, int sx, int sy, int sw, int sh, int dx, int dy) {
    Sprite s; s.type = Sprite::Type::ARGB; s.argb = src; s.srcW = srcW; s.srcH = srcH; s.sx=sx; s.sy=sy; s.sw=sw; s.sh=sh; s.dx=dx; s.dy=dy; items.push_back(s);
}

void Queue::AddPAL8Mask(const uint8_t* src, int srcW, int srcH, int sx, int sy, int sw, int sh, int dx, int dy, uint8_t transparent) {
    Sprite s; s.type = Sprite::Type::PAL8_MASK; s.pal8 = src; s.palW = srcW; s.palH = srcH; s.sx=sx; s.sy=sy; s.sw=sw; s.sh=sh; s.dx=dx; s.dy=dy; s.transparent=transparent; items.push_back(s);
}

void Queue::Render(Screen& screen) {
    for (const Sprite& s : items) {
        switch (s.type) {
        case Sprite::Type::ARGB:
            screen.BlitSubrect(s.argb, s.srcW, s.srcH, s.sx, s.sy, s.sw, s.sh, s.dx, s.dy);
            break;
        case Sprite::Type::PAL8_MASK:
            screen.BlitPAL8SubrectMask(s.pal8, s.palW, s.palH, s.sx, s.sy, s.sw, s.sh, s.dx, s.dy, s.transparent);
            break;
        }
    }
}

}

END_UPP_NAMESPACE

