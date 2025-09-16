#include "Game.h"
#include "../Id/Id.h"

NAMESPACE_UPP

namespace Game {

// Dummy format for now: [u16 width][u16 height][width*height u16 tiles]
bool Map::LoadUser(const char* filename) {
    FS::File f = FS::File::OpenUser(filename);
    if (!f.IsValid())
        return false;
    FS::Reader r(f);
    uint16_t wh[2] = {0,0};
    if (r.ReadInt16LE(wh, 2) != 2)
        return false;
    width_ = wh[0];
    height_ = wh[1];
    if (width_ <= 0 || height_ <= 0 || width_ > 4096 || height_ > 4096)
        return false;
    tiles_.assign((size_t)width_ * (size_t)height_, 0);
    if (r.ReadInt16LE(tiles_.data(), tiles_.size()) != tiles_.size())
        return false;
    return true;
}

}

END_UPP_NAMESPACE

