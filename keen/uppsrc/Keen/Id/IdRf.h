// Minimal RF (render-frame) utilities: dirty blocks, scrolling, sprite draw entry
#pragma once


namespace RF {

struct SpriteDrawEntry {
    int chunk = 0;
    int zLayer = 0;
    int x = 0, y = 0;    // unit coordinates
    int sx = 0, sy = 0;  // source coords (pixels) for subrect
    int sw = 0, sh = 0;  // source size
    bool maskOnly = false;
    int updateCount = 0;
    int shift = 0;
};

struct Const {
    static constexpr int BUFFER_WIDTH_TILES  = 21;
    static constexpr int BUFFER_HEIGHT_TILES = 14;
    static constexpr int BUFFER_SIZE = BUFFER_WIDTH_TILES * BUFFER_HEIGHT_TILES;
    static constexpr int SCREEN_WIDTH_TILES  = 20;
    static constexpr int SCREEN_HEIGHT_TILES = 13;
};

class State {
public:
    State();
    // Dirty blocks
    void MarkBlockDirty(int x, int y, uint8_t val, int page);
    uint8_t IsBlockDirty(int x, int y, int page) const;
    void ForceRefresh();

    // Scroll control
    void SetScrollBlock(int tileX, int tileY, bool vertical);
    void Reposition(int scrollXunit, int scrollYunit);
    void SmoothScroll(int dxUnit, int dyUnit);
    void SetBuffers(int n);
    int GetNumBuffers() const { return numBuffers; }
    int GetActiveBuffer() const { return activeBuffer; }
    void SwitchBuffer();

    // Accessors
    int ScrollXUnit() const { return scrollXUnit; }
    int ScrollYUnit() const { return scrollYUnit; }

private:
    int scrollXUnit = 0;
    int scrollYUnit = 0;
    int dirtyBufferOffset = 0; // for wrap-around
    int numBuffers = 2;
    int activeBuffer = 0;
    // Support up to 3 buffers
    std::vector<std::vector<uint8_t>> dirtyBlocks; // [page][Const::BUFFER_SIZE]
    std::vector<int> horzScrollBlocks;
    std::vector<int> vertScrollBlocks;
};

} // namespace RF
