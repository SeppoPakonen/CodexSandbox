#include "KeenUnit.h"

NAMESPACE_UPP

void RunKeenSpriteTests() {
    // ARGB subrect via Queue
    uint32_t src[16];
    for (int i=0;i<16;++i) src[i]=0xff000000;
    src[0]=src[1]=src[4]=src[5]=0xffffffff; // 2x2 white at (0,0)
    Video::Screen s;
    s.SetSize(6,6);
    s.Clear(0xff000000);
    Video::Queue q;
    q.AddARGB(src, 4,4, 0,0, 2,2, 2,2);
    q.Render(s);
    const uint32_t* px = s.Data();
    auto idx = [&](int x,int y){ return (size_t)y*6 + (size_t)x; };
    ASSERT(px[idx(2,2)] == 0xffffffff);
    ASSERT(px[idx(3,3)] == 0xffffffff);
    ASSERT(px[idx(0,0)] == 0xff000000);

    // Paletted masked subrect via Queue
    uint8_t palidx[9] = {0,1,0,
                         1,1,1,
                         0,1,0};
    uint32_t pal[256] = {};
    pal[1]=0xff00ff00; // green
    s.SetPalette(pal, 256);
    s.Clear(0xff000000);
    q.Clear();
    q.AddPAL8Mask(palidx, 3,3, 0,0,3,3, 1,1, 0);
    q.Render(s);
    ASSERT(px[idx(2,2)] == 0xff00ff00);
    ASSERT(px[idx(1,1)] == 0xff00ff00);
    ASSERT(px[idx(0,0)] == 0xff000000);
}

END_UPP_NAMESPACE

