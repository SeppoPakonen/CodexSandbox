#include "Game.h"
#include "../Id/Id.h"

NAMESPACE_UPP

namespace Game {

bool Resources::LoadTilesPal8(const char* filename) {
    return Assets::LoadUserImagePAL8(filename, tiles_);
}

bool Resources::DrawTile(Video::Screen& s, int index, int dx, int dy) const {
    if (!tiles_.IsValid()) return false;
    int tw = TileW(), th = TileH();
    int tilesPerRow = tiles_.w / tw;
    if (tilesPerRow <= 0) return false;
    if (index < 0) return false;
    int sy = (index / tilesPerRow) * th;
    int sx = (index % tilesPerRow) * tw;
    s.SetPalette(tiles_.palette.data(), (int)tiles_.palette.size());
    s.BlitPAL8SubrectMask(tiles_.indices.data(), tiles_.w, tiles_.h, sx, sy, tw, th, dx, dy, 0);
    return true;
}

}

END_UPP_NAMESPACE

