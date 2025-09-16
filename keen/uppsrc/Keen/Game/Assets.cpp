#include "Game.h"
#include "../Id/Id.h"

NAMESPACE_UPP

namespace Game {

bool Assets::LoadUserImageARGB32(const char* filename, Image& out) {
    FS::File f = FS::File::OpenUser(filename);
    if (!f.IsValid())
        return false;
    FS::Reader r(f);
    uint16_t wh[2] = {0,0};
    if (r.ReadInt16LE(wh, 2) != 2)
        return false;
    int w = wh[0], h = wh[1];
    if (w <= 0 || h <= 0 || w > 4096 || h > 4096)
        return false;
    out.w = w; out.h = h;
    out.pixels.assign((size_t)w * (size_t)h, 0);
    // Read raw ARGB32 little-endian
    // Reuse File::Read directly for speed here
    if (f.Read(out.pixels.data(), sizeof(uint32_t), out.pixels.size()) != out.pixels.size()) {
        out = Image{};
        return false;
    }
    return true;
}

bool Assets::LoadUserImagePAL8(const char* filename, Pal8& out) {
    FS::File f = FS::File::OpenUser(filename);
    if (!f.IsValid())
        return false;
    FS::Reader r(f);
    uint16_t wh[2] = {0,0};
    if (r.ReadInt16LE(wh, 2) != 2)
        return false;
    int w = wh[0], h = wh[1];
    if (w <= 0 || h <= 0 || w > 4096 || h > 4096)
        return false;
    out.w = w; out.h = h;
    out.palette.assign(256, 0);
    if (f.Read(out.palette.data(), sizeof(uint32_t), out.palette.size()) != out.palette.size()) {
        out = Pal8{}; return false;
    }
    out.indices.assign((size_t)w * (size_t)h, 0);
    if (f.Read(out.indices.data(), 1, out.indices.size()) != out.indices.size()) {
        out = Pal8{}; return false;
    }
    return true;
}

}

END_UPP_NAMESPACE
