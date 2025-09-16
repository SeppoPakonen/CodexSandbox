#include "Id.h"

NAMESPACE_UPP

namespace Video {

void Screen::Startup() {}
void Screen::Shutdown() {}

void Screen::SetSize(int w, int h) {
    if (w < 1) w = 1;
    if (h < 1) h = 1;
    width_ = w;
    height_ = h;
    pixels_.assign((size_t)w * (size_t)h, 0u);
}

void Screen::Clear(uint32_t colour) {
    if (!pixels_.empty())
        std::fill(pixels_.begin(), pixels_.end(), colour);
}

void Screen::Present() {
    // No-op in null backend; swap active buffer index if requested
    if (swap_next_ && num_buffers_ > 1) {
        active_buffer_ = (active_buffer_ + 1) % num_buffers_;
        swap_next_ = false;
    }
}

void Screen::DrawRect(int x, int y, int w, int h, uint32_t colour) {
    if (w <= 0 || h <= 0 || pixels_.empty()) return;
    // Clip
    if (x < 0) { w += x; x = 0; }
    if (y < 0) { h += y; y = 0; }
    if (x >= width_ || y >= height_) return;
    if (x + w > width_) w = width_ - x;
    if (y + h > height_) h = height_ - y;
    for (int row = 0; row < h; ++row) {
        uint32_t* dst = pixels_.data() + (size_t)(y + row) * (size_t)width_ + (size_t)x;
        std::fill(dst, dst + w, colour);
    }
}

void Screen::Blit(const uint32_t* src, int srcW, int srcH, int dstX, int dstY) {
    if (!src || srcW <= 0 || srcH <= 0 || pixels_.empty()) return;
    // Clip source/destination
    int x0 = dstX, y0 = dstY;
    int w = srcW, h = srcH;
    if (x0 < 0) { int dx = -x0; x0 = 0; w -= dx; src += dx; srcW -= dx; }
    if (y0 < 0) { int dy = -y0; y0 = 0; h -= dy; src += (size_t)dy * (size_t)srcW; srcH -= dy; }
    if (x0 >= width_ || y0 >= height_) return;
    if (x0 + w > width_) w = width_ - x0;
    if (y0 + h > height_) h = height_ - y0;
    for (int row = 0; row < h; ++row) {
        const uint32_t* s = src + (size_t)row * (size_t)srcW;
        uint32_t* d = pixels_.data() + (size_t)(y0 + row) * (size_t)width_ + (size_t)x0;
        std::copy(s, s + w, d);
    }
}

void Screen::BlitPAL8(const uint8_t* src, int srcW, int srcH, int dstX, int dstY) {
    if (!src || srcW <= 0 || srcH <= 0 || pixels_.empty()) return;
    if (palette_.empty()) {
        // Default grayscale palette
        palette_.resize(256);
        for (int i = 0; i < 256; ++i) {
            uint32_t v = (uint32_t)i;
            palette_[i] = 0xFF000000u | (v << 16) | (v << 8) | v;
        }
    }
    int x0 = dstX, y0 = dstY;
    int w = srcW, h = srcH;
    if (x0 < 0) { int dx = -x0; x0 = 0; w -= dx; src += dx; srcW -= dx; }
    if (y0 < 0) { int dy = -y0; y0 = 0; h -= dy; src += (size_t)dy * (size_t)srcW; srcH -= dy; }
    if (x0 >= width_ || y0 >= height_) return;
    if (x0 + w > width_) w = width_ - x0;
    if (y0 + h > height_) h = height_ - y0;
    for (int row = 0; row < h; ++row) {
        const uint8_t* s = src + (size_t)row * (size_t)srcW;
        uint32_t* d = pixels_.data() + (size_t)(y0 + row) * (size_t)width_ + (size_t)x0;
        for (int col = 0; col < w; ++col)
            d[col] = palette_[(unsigned)s[col]];
    }
}

void Screen::SetPalette(const uint32_t* argb, int count) {
    if (!argb || count <= 0) return;
    palette_.assign(argb, argb + count);
}

void Screen::BlitPAL8Mask(const uint8_t* src, int srcW, int srcH, int dstX, int dstY, uint8_t transparent_index) {
    if (!src || srcW <= 0 || srcH <= 0 || pixels_.empty()) return;
    if (palette_.empty()) {
        palette_.resize(256);
        for (int i = 0; i < 256; ++i) {
            uint32_t v = (uint32_t)i;
            palette_[i] = 0xFF000000u | (v << 16) | (v << 8) | v;
        }
    }
    int x0 = dstX, y0 = dstY;
    int w = srcW, h = srcH;
    if (x0 < 0) { int dx = -x0; x0 = 0; w -= dx; src += dx; srcW -= dx; }
    if (y0 < 0) { int dy = -y0; y0 = 0; h -= dy; src += (size_t)dy * (size_t)srcW; srcH -= dy; }
    if (x0 >= width_ || y0 >= height_) return;
    if (x0 + w > width_) w = width_ - x0;
    if (y0 + h > height_) h = height_ - y0;
    for (int row = 0; row < h; ++row) {
        const uint8_t* s = src + (size_t)row * (size_t)srcW;
        uint32_t* d = pixels_.data() + (size_t)(y0 + row) * (size_t)width_ + (size_t)x0;
        for (int col = 0; col < w; ++col) {
            uint8_t idx = s[col];
            if (idx != transparent_index)
                d[col] = palette_[idx];
        }
    }
}

void Screen::BlitSubrect(const uint32_t* src, int srcW, int srcH, int sx, int sy, int sw, int sh, int dstX, int dstY) {
    if (!src || srcW <= 0 || srcH <= 0 || sw <= 0 || sh <= 0 || pixels_.empty()) return;
    // Clip src rect to src bounds
    if (sx < 0) { sw += sx; dstX -= sx; sx = 0; }
    if (sy < 0) { sh += sy; dstY -= sy; sy = 0; }
    if (sx + sw > srcW) sw = srcW - sx;
    if (sy + sh > srcH) sh = srcH - sy;
    if (sw <= 0 || sh <= 0) return;
    // Clip to dest
    if (dstX < 0) { int dx = -dstX; sx += dx; sw -= dx; dstX = 0; }
    if (dstY < 0) { int dy = -dstY; sy += dy; sh -= dy; dstY = 0; }
    if (dstX >= width_ || dstY >= height_) return;
    if (dstX + sw > width_) sw = width_ - dstX;
    if (dstY + sh > height_) sh = height_ - dstY;
    for (int row = 0; row < sh; ++row) {
        const uint32_t* s = src + (size_t)(sy + row) * (size_t)srcW + (size_t)sx;
        uint32_t* d = pixels_.data() + (size_t)(dstY + row) * (size_t)width_ + (size_t)dstX;
        std::copy(s, s + sw, d);
    }
}

void Screen::BlitPAL8SubrectMask(const uint8_t* src, int srcW, int srcH, int sx, int sy, int sw, int sh, int dstX, int dstY, uint8_t transparent_index) {
    if (!src || srcW <= 0 || srcH <= 0 || sw <= 0 || sh <= 0 || pixels_.empty()) return;
    if (palette_.empty()) {
        palette_.resize(256);
        for (int i = 0; i < 256; ++i) {
            uint32_t v = (uint32_t)i;
            palette_[i] = 0xFF000000u | (v << 16) | (v << 8) | v;
        }
    }
    if (sx < 0) { sw += sx; dstX -= sx; sx = 0; }
    if (sy < 0) { sh += sy; dstY -= sy; sy = 0; }
    if (sx + sw > srcW) sw = srcW - sx;
    if (sy + sh > srcH) sh = srcH - sy;
    if (sw <= 0 || sh <= 0) return;
    if (dstX < 0) { int dx = -dstX; sx += dx; sw -= dx; dstX = 0; }
    if (dstY < 0) { int dy = -dstY; sy += dy; sh -= dy; dstY = 0; }
    if (dstX >= width_ || dstY >= height_) return;
    if (dstX + sw > width_) sw = width_ - dstX;
    if (dstY + sh > height_) sh = height_ - dstY;
    for (int row = 0; row < sh; ++row) {
        const uint8_t* s = src + (size_t)(sy + row) * (size_t)srcW + (size_t)sx;
        uint32_t* d = pixels_.data() + (size_t)(dstY + row) * (size_t)width_ + (size_t)dstX;
        for (int col = 0; col < sw; ++col) {
            uint8_t idx = s[col];
            if (idx != transparent_index)
                d[col] = palette_[idx];
        }
    }
}

// 5x7 font glyphs for digits, colon, and uppercase A-Z
static const uint8_t kGlyph5x7_Digits[10][5] = {
    {0x7E,0x81,0x81,0x81,0x7E}, // 0
    {0x00,0x82,0xFF,0x80,0x00}, // 1
    {0xE2,0x91,0x89,0x89,0x86}, // 2
    {0x42,0x81,0x89,0x89,0x76}, // 3
    {0x1C,0x12,0x11,0xFF,0x10}, // 4
    {0x4F,0x89,0x89,0x89,0x71}, // 5
    {0x7E,0x89,0x89,0x89,0x72}, // 6
    {0x01,0x01,0xF1,0x09,0x07}, // 7
    {0x76,0x89,0x89,0x89,0x76}, // 8
    {0x4E,0x91,0x91,0x91,0x7E}, // 9
};

static const uint8_t kGlyph5x7_Colon[5] = {0x00,0x00,0x24,0x00,0x00};

static const uint8_t kGlyph5x7_Upper[26][5] = {
    {0xFE,0x11,0x11,0x11,0xFE}, // A
    {0xFF,0x89,0x89,0x89,0x76}, // B
    {0x7E,0x81,0x81,0x81,0x42}, // C
    {0xFF,0x81,0x81,0x42,0x3C}, // D
    {0xFF,0x89,0x89,0x89,0x81}, // E
    {0xFF,0x09,0x09,0x09,0x01}, // F
    {0x7E,0x81,0x91,0x91,0x72}, // G
    {0xFF,0x08,0x08,0x08,0xFF}, // H
    {0x81,0x81,0xFF,0x81,0x81}, // I
    {0x40,0x80,0x81,0x7F,0x01}, // J
    {0xFF,0x18,0x24,0x42,0x81}, // K
    {0xFF,0x80,0x80,0x80,0x80}, // L
    {0xFF,0x02,0x0C,0x02,0xFF}, // M
    {0xFF,0x06,0x18,0x60,0xFF}, // N
    {0x7E,0x81,0x81,0x81,0x7E}, // O
    {0xFF,0x11,0x11,0x11,0x0E}, // P
    {0x7E,0x81,0xA1,0x41,0xBE}, // Q
    {0xFF,0x11,0x31,0x51,0x8E}, // R
    {0x46,0x89,0x89,0x89,0x72}, // S
    {0x01,0x01,0xFF,0x01,0x01}, // T
    {0x7F,0x80,0x80,0x80,0x7F}, // U
    {0x1F,0x60,0x80,0x60,0x1F}, // V
    {0x7F,0x80,0x7C,0x80,0x7F}, // W
    {0xC3,0x24,0x18,0x24,0xC3}, // X
    {0x03,0x0C,0xF0,0x0C,0x03}, // Y
    {0xE1,0x91,0x89,0x85,0x83}, // Z
};

void Screen::DrawChar5x7(int x, int y, char ch, uint32_t colour) {
    const uint8_t* glyph = nullptr;
    if (ch >= '0' && ch <= '9') glyph = kGlyph5x7_Digits[ch - '0'];
    else if (ch == ':') glyph = kGlyph5x7_Colon;
    else if (ch >= 'A' && ch <= 'Z') glyph = kGlyph5x7_Upper[ch - 'A'];
    else if (ch == ' ') return; // space
    if (!glyph) return;
    // Render 5 columns x 7 rows
    for (int col = 0; col < 5; ++col) {
        uint8_t bits = glyph[col];
        for (int row = 0; row < 7; ++row) {
            if (bits & (1 << (6 - row))) {
                int px = x + col;
                int py = y + row;
                if ((unsigned)px < (unsigned)width_ && (unsigned)py < (unsigned)height_)
                    pixels_[(size_t)py * (size_t)width_ + (size_t)px] = colour;
            }
        }
    }
}

void Screen::DrawText5x7(int x, int y, const char* text, uint32_t colour) {
    if (!text) return;
    int cx = x;
    for (const char* p = text; *p; ++p) {
        DrawChar5x7(cx, y, *p, colour);
        cx += 6; // 1px spacing
    }
}

void Screen::SetNumBuffers(int n) {
    if (n < 1) n = 1; if (n > 3) n = 3;
    num_buffers_ = n;
    active_buffer_ = 0;
}

void Screen::SwapOnNextPresent() { swap_next_ = true; }

void Screen::UpdateRect(int, int, int, int) {
    // Placeholder: in a real backend this would mark damaged region
}

}

END_UPP_NAMESPACE
