#include "Id.h"

NAMESPACE_UPP

namespace VH {

bool Tileset::LoadPAL8(const char* filename) {
    FS::File f = FS::File::OpenUser(filename);
    if (!f.IsValid()) return false;
    FS::Reader r(f);
    uint16_t wh[2] = {0,0};
    if (r.ReadInt16LE(wh, 2) != 2) return false;
    w = wh[0]; h = wh[1];
    if (w <= 0 || h <= 0 || w > 8192 || h > 8192) return false;
    palette.assign(256, 0);
    if (f.Read(palette.data(), sizeof(uint32_t), palette.size()) != palette.size()) {
        palette.clear(); return false;
    }
    indices.assign((size_t)w * (size_t)h, 0);
    if (f.Read(indices.data(), 1, indices.size()) != indices.size()) {
        palette.clear(); indices.clear(); return false;
    }
    return true;
}

bool Tileset::DrawTile(Video::Screen& s, int tileIndex, int dx, int dy, int tileW, int tileH, uint8_t transparent) const {
    if (!IsValid() || tileW <= 0 || tileH <= 0) return false;
    int tilesPerRow = w / tileW;
    if (tilesPerRow <= 0 || tileIndex < 0) return false;
    int sy = (tileIndex / tilesPerRow) * tileH;
    int sx = (tileIndex % tilesPerRow) * tileW;
    s.SetPalette(palette.data(), (int)palette.size());
    s.BlitPAL8SubrectMask(indices.data(), w, h, sx, sy, tileW, tileH, dx, dy, transparent);
    return true;
}

}

END_UPP_NAMESPACE

