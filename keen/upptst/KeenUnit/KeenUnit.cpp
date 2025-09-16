#include "KeenUnit.h"

NAMESPACE_UPP

static void TestFSReaderWriter() {
    // Write some values
    const char* fname = "keen_unittest.bin";
    FS::File f = FS::File::CreateUser(fname);
    ASSERT(f.IsValid());
    FS::Writer w(f);
    uint8_t b8[3] = {1,2,3};
    uint16_t b16[3] = {0x0102, 0xA0B0, 0xFFFF};
    uint32_t b32[2] = {0x11223344u, 0xAABBCCDDu};
    bool bb[3] = {true, false, true};
    ASSERT(w.WriteInt8(b8, 3) == 3);
    ASSERT(w.WriteInt16LE(b16, 3) == 3);
    ASSERT(w.WriteInt32LE(b32, 2) == 2);
    ASSERT(w.WriteBool16LE(bb, 3) == 3);
    f.Close();

    // Read back
    FS::File f2 = FS::File::OpenUser(fname);
    ASSERT(f2.IsValid());
    FS::Reader r(f2);
    uint8_t rb8[3] = {0};
    uint16_t rb16[3] = {0};
    uint32_t rb32[2] = {0};
    bool rbb[3] = {false};
    ASSERT(r.ReadInt8(rb8, 3) == 3);
    ASSERT(rb8[0]==1 && rb8[1]==2 && rb8[2]==3);
    ASSERT(r.ReadInt16LE(rb16, 3) == 3);
    ASSERT(rb16[0]==0x0102 && rb16[1]==0xA0B0 && rb16[2]==0xFFFF);
    ASSERT(r.ReadInt32LE(rb32, 2) == 2);
    ASSERT(rb32[0]==0x11223344u && rb32[1]==0xAABBCCDDu);
    ASSERT(r.ReadBool16LE(rbb, 3) == 3);
    ASSERT(rbb[0]==true && rbb[1]==false && rbb[2]==true);
    f2.Close();
}

static void TestConfigStore() {
    const char* cfgname = "keen_unittest.cfg";
    FS::File out = FS::File::CreateUser(cfgname);
    ASSERT(out.IsValid());
    out.PrintF("num = 42\n");
    out.PrintF("str = \"hello\"\n");
    out.PrintF("flag = true\n");
    out.Close();

    Config::Store::Instance().Load(cfgname);
    ASSERT(Config::Store::Instance().GetInt("num", 0) == 42);
    ASSERT(String(Config::Store::Instance().GetString("str", "")) == "hello");
    ASSERT(Config::Store::Instance().GetBool("flag", false) == true);
}

static void TestVideoPrimitives() {
    Video::Screen s;
    s.SetSize(8, 8);
    s.Clear(0xff000000);
    // Draw a 2x2 white square at (3,3)
    s.DrawRect(3,3,2,2, 0xffffffff);
    const uint32_t* px = s.Data();
    auto idx = [&](int x,int y){ return (size_t)y*8 + (size_t)x; };
    ASSERT(px[idx(3,3)] == 0xffffffff);
    ASSERT(px[idx(4,3)] == 0xffffffff);
    ASSERT(px[idx(3,4)] == 0xffffffff);
    ASSERT(px[idx(4,4)] == 0xffffffff);
    ASSERT(px[idx(0,0)] == 0xff000000);

    // Subrect blit: copy 2x2 into (0,0)
    s.BlitSubrect(px, 8, 8, 3,3,2,2, 0,0);
    ASSERT(px[idx(0,0)] == 0xffffffff);
}

static void TestAssetsAndPaletted() {
    // Create a small paletted image 4x4 with palette[0]=transparent, [1]=red
    const char* palname = "keen_unittest_pal8.img";
    {
        FS::File f = FS::File::CreateUser(palname);
        ASSERT(f.IsValid());
        uint16_t wh[2] = {4,4};
        f.Write(wh, sizeof(uint16_t), 2);
        uint32_t pal[256] = {};
        pal[1] = 0xFFFF0000u; // ARGB
        f.Write(pal, sizeof(uint32_t), 256);
        uint8_t idx[16] = {1,1,0,0,
                           1,1,0,0,
                           0,0,1,1,
                           0,0,1,1};
        f.Write(idx, 1, 16);
        f.Close();
    }

    Game::Assets::Pal8 pal;
    ASSERT(Game::Assets::LoadUserImagePAL8(palname, pal));
    ASSERT(pal.IsValid());

    Video::Screen s;
    s.SetSize(4,4);
    s.SetPalette(pal.palette.data(), (int)pal.palette.size());
    s.Clear(0xff000000);
    s.BlitPAL8Mask(pal.indices.data(), 4, 4, 0, 0, 0);
    const uint32_t* px = s.Data();
    auto idxpx = [&](int x,int y){ return (size_t)y*4 + (size_t)x; };
    ASSERT(px[idxpx(0,0)] == 0xFFFF0000u);
    ASSERT(px[idxpx(1,1)] == 0xFFFF0000u);
    ASSERT(px[idxpx(2,0)] == 0xff000000u); // transparent index left as clear colour
}

extern void RunKeenSpriteTests();
extern void RunKeenCacheTests();
extern void RunKeenVhTests();
extern void RunKeenRfTests();
extern void RunKeenInputTimeTests();
extern void RunKeenPlayerTests();
extern void RunKeenFrameRateTests();
extern void RunKeenStateTests();
extern void RunKeenBufferSyncTests();
extern void RunKeenHudTests();
extern void RunKeenStateCounterTests();
extern void RunKeenDemoRecordTests();
extern void RunKeenDemoAutoTests();
extern void RunKeenDemoPauseTests();
extern void RunKeenRfTests();

END_UPP_NAMESPACE

CONSOLE_APP_MAIN {
	using namespace Upp;
    StdLogSetup(LOG_FILE|LOG_COUT);
    TestFSReaderWriter();
    TestConfigStore();
    TestVideoPrimitives();
    TestAssetsAndPaletted();
    RunKeenSpriteTests();
    RunKeenCacheTests();
    RunKeenVhTests();
    RunKeenRfTests();
    RunKeenInputTimeTests();
    RunKeenPlayerTests();
    RunKeenFrameRateTests();
    RunKeenStateTests();
    RunKeenBufferSyncTests();
    RunKeenHudTests();
    RunKeenStateCounterTests();
    RunKeenDemoRecordTests();
    RunKeenDemoAutoTests();
    RunKeenDemoPauseTests();
    LOG("KeenUnit: OK");
}
