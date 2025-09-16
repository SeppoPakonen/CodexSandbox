#include "Game.h"
#include "../Id/Id.h" // allowed after main header per policy

NAMESPACE_UPP

namespace Game {

void Bootstrap() {
    Config::Store::Instance().Startup();
    Video::Screen::Startup();
    Input::Device::Default().Startup();
    Audio::Device::Default().Startup();

    // Example: read an optional config
    Config::Store::Instance().Load("OMNISPK.CFG");

    // No-op present to validate linkage
    Video::Screen s;
    s.Clear(0xff000000);
    s.Present();

    // Tiny loop to exercise timing and input polling (no real rendering yet)
    const int frames = 3;
    for (int i = 0; i < frames; ++i) {
        Input::Device::Default().Poll();
        AppTime::Clock::DelayMs(16);
    }

    // Attempt to load a user map (dummy format) to exercise FS::Reader
    Game::Map map;
    map.LoadUser("MAP.DAT");

    // Draw simple diagnostics text
    char buf[64];
    std::snprintf(buf, sizeof(buf), "T:%u", (unsigned)AppTime::Clock::NowMs());
    s.DrawText5x7(2, 2, buf, 0xffffffff);
    s.Present();

    // Attempt to load a test image and blit it
    Game::Image img;
    if (Game::Assets::LoadUserImageARGB32("TEST.IMG", img) && img.IsValid()) {
        s.SetSize(img.w, img.h);
        s.Clear(0xff202020);
        s.Blit(img.pixels.data(), img.w, img.h, 0, 0);
        s.Present();
    }

    // Attempt to load a paletted tileset and draw a few tiles
    Game::Resources res;
    if (res.LoadTilesPal8("TILES.PAL8")) {
        s.SetSize(160, 100);
        s.Clear(0xff000000);
        // Use a sprite queue to draw first 10 tiles in a row
        Video::Queue q;
        s.SetPalette(res.Tiles().palette.data(), (int)res.Tiles().palette.size());
        int tw = res.TileW(), th = res.TileH();
        int tilesPerRow = res.Tiles().w / tw;
        for (int i = 0; i < 10; ++i) {
            int sx = (i % tilesPerRow) * tw;
            int sy = (i / tilesPerRow) * th;
            q.AddPAL8Mask(res.Tiles().indices.data(), res.Tiles().w, res.Tiles().h,
                          sx, sy, tw, th, i * tw, 10, 0);
        }
        q.Render(s);
        s.Present();
    }

    // Run a short demo loop to exercise player + timing
    RunDemoLoop(30);
}

}

END_UPP_NAMESPACE
