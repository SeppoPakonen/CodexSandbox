// Video facade (null backend for now)
#pragma once

namespace Video {

class Screen {
public:
    static void Startup();
    static void Shutdown();

    Screen() { SetSize(320, 200); }
    void SetSize(int w, int h);
    int Width() const { return width_; }
    int Height() const { return height_; }

    void Clear(uint32_t colour);
    void Present();

    // Primitive drawing
    void DrawRect(int x, int y, int w, int h, uint32_t colour);
    void Blit(const uint32_t* src, int srcW, int srcH, int dstX, int dstY);
    void BlitPAL8(const uint8_t* src, int srcW, int srcH, int dstX, int dstY);
    void BlitPAL8Mask(const uint8_t* src, int srcW, int srcH, int dstX, int dstY, uint8_t transparent_index = 0);
    void BlitSubrect(const uint32_t* src, int srcW, int srcH, int sx, int sy, int sw, int sh, int dstX, int dstY);
    void BlitPAL8SubrectMask(const uint8_t* src, int srcW, int srcH, int sx, int sy, int sw, int sh, int dstX, int dstY, uint8_t transparent_index = 0);

    // Palette control (for paletted blits)
    void SetPalette(const uint32_t* argb, int count);

    // Tiny diagnostics text (5x7 font, digits and ':')
    void DrawChar5x7(int x, int y, char ch, uint32_t colour);
    void DrawText5x7(int x, int y, const char* text, uint32_t colour);

    // Buffering (ring) helpers
    void SetNumBuffers(int n);
    int GetNumBuffers() const { return num_buffers_; }
    int GetActiveBufferId() const { return active_buffer_; }
    void SwapOnNextPresent();
    void UpdateRect(int x, int y, int w, int h); // no-op placeholder; kept for API parity

    uint32_t* Data() { return pixels_.empty() ? nullptr : pixels_.data(); }

private:
    int width_ = 0;
    int height_ = 0;
    std::vector<uint32_t> pixels_;
    std::vector<uint32_t> palette_; // up to 256 entries
    int num_buffers_ = 2;
    int active_buffer_ = 0;
    bool swap_next_ = false;
};

}
