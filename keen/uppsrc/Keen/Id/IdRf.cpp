#include "Id.h"

NAMESPACE_UPP

namespace RF {

State::State() {
    dirtyBlocks.resize(3);
    for (auto& page : dirtyBlocks)
        page.assign((size_t)Const::BUFFER_SIZE, 0);
}

void State::MarkBlockDirty(int x, int y, uint8_t val, int page) {
    if (x < 0 || y < 0 || x >= Const::BUFFER_WIDTH_TILES || y >= Const::BUFFER_HEIGHT_TILES)
        return;
    if (page < 0) {
        for (size_t p = 0; p < dirtyBlocks.size(); ++p)
            MarkBlockDirty(x, y, val, (int)p);
        return;
    }
    if (dirtyBufferOffset < 0)
        dirtyBufferOffset += Const::BUFFER_SIZE;
    size_t offset = ((y * Const::BUFFER_WIDTH_TILES + x) + dirtyBufferOffset) % (Const::BUFFER_SIZE);
    if ((size_t)page < dirtyBlocks.size())
        dirtyBlocks[(size_t)page][offset] = val;
}

uint8_t State::IsBlockDirty(int x, int y, int page) const {
    if (x < 0 || y < 0 || x >= Const::BUFFER_WIDTH_TILES || y >= Const::BUFFER_HEIGHT_TILES)
        return 0;
    if (page < 0) page = 0; // active buffer not tracked; default 0
    int dbo = dirtyBufferOffset;
    if (dbo < 0) dbo += Const::BUFFER_SIZE;
    size_t offset = ((y * Const::BUFFER_WIDTH_TILES + x) + dbo) % (Const::BUFFER_SIZE);
    if ((size_t)page < dirtyBlocks.size())
        return dirtyBlocks[(size_t)page][offset];
    return 0;
}

void State::ForceRefresh() {
    for (auto& page : dirtyBlocks)
        std::fill(page.begin(), page.end(), 1);
}

void State::SetScrollBlock(int tileX, int tileY, bool vertical) {
    if (!vertical) {
        horzScrollBlocks.push_back(tileX);
    } else {
        vertScrollBlocks.push_back(tileY);
    }
}

void State::Reposition(int xUnit, int yUnit) {
    // Adjust offset based on tile delta
    int oldTileX = scrollXUnit >> 8;
    int oldTileY = scrollYUnit >> 8;
    scrollXUnit = xUnit;
    scrollYUnit = yUnit;
    int newTileX = scrollXUnit >> 8;
    int newTileY = scrollYUnit >> 8;
    int dxTiles = newTileX - oldTileX;
    int dyTiles = newTileY - oldTileY;
    if (dxTiles || dyTiles) {
        int rowStride = Const::BUFFER_WIDTH_TILES;
        dirtyBufferOffset += dyTiles * rowStride + dxTiles;
        // Mark whole screen dirty on reposition
        ForceRefresh();
    }
}

void State::SmoothScroll(int dxUnit, int dyUnit) {
    scrollXUnit += dxUnit;
    scrollYUnit += dyUnit;
}

void State::SetBuffers(int n) {
    if (n < 1) n = 1; if (n > 3) n = 3;
    numBuffers = n;
    activeBuffer = 0;
}

void State::SwitchBuffer() {
    if (numBuffers > 1) {
        activeBuffer = (activeBuffer + 1) % numBuffers;
        if ((size_t)activeBuffer < dirtyBlocks.size())
            std::fill(dirtyBlocks[(size_t)activeBuffer].begin(), dirtyBlocks[(size_t)activeBuffer].end(), 0);
    }
}

} // namespace RF

END_UPP_NAMESPACE
