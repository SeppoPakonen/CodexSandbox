#include "KeenUnit.h"

NAMESPACE_UPP

void RunKeenVhTests() {
    // Create a small paletted tileset 8x8 with a single 8x8 tile (green cross)
    const char* fname = "keen_unittest_vh_tiles.img";
    {
        FS::File f = FS::File::CreateUser(fname);
        ASSERT(f.IsValid());
        uint16_t wh[2] = {8,8};
        f.Write(wh, sizeof(uint16_t), 2);
        uint32_t pal[256] = {};
        pal[1] = 0xff00ff00; // ARGB green
        f.Write(pal, sizeof(uint32_t), 256);
        uint8_t idx[64] = {};
        for (int i=0;i<8;i++){ idx[i*8 + 4] = 1; idx[4*8 + i] = 1; }
        f.Write(idx, 1, 64);
        f.Close();
    }

    VH::Tileset ts;
    ASSERT(ts.LoadPAL8(fname));
    ASSERT(ts.IsValid());

    Video::Screen s;
    s.SetSize(8,8);
    s.Clear(0xff000000);
    ASSERT(ts.DrawTile(s, 0, 0, 0));
    const uint32_t* px = s.Data();
    auto at=[&](int x,int y){ return px[(size_t)y*8 + (size_t)x]; };
    ASSERT(at(4,0) == 0xff00ff00);
    ASSERT(at(0,4) == 0xff00ff00);
}

END_UPP_NAMESPACE

