// Minimal resources helper: paletted tileset loader
#pragma once

//#include "Assets.h"

namespace Game {

class Resources {
public:
    bool LoadTilesPal8(const char* filename);
    const Assets::Pal8& Tiles() const { return tiles_; }
    int TileW() const { return 8; }
    int TileH() const { return 8; }
    bool DrawTile(Video::Screen& s, int index, int dx, int dy) const;
private:
    Assets::Pal8 tiles_;
};

}

