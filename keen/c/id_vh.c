/*
Omnispeak: A Commander Keen Reimplementation
Copyright (C) 2012 David Gow <david@ingeniumdigital.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "id_vh.h"
#include "id_ca.h"
#include "id_mm.h"
#include "id_rf.h"
#include "id_vl.h"

//TODO: Should these functions cache the bitmap tables?
VH_BitmapTableEntry *VH_GetBitmapTableEntry(int bitmapNumber)
{
	VH_BitmapTableEntry *bitmapTable = (VH_BitmapTableEntry *)(ca_graphChunks[ca_gfxInfoE.hdrBitmaps]);
	return &bitmapTable[bitmapNumber];
}

static VH_BitmapTableEntry *VH_GetMaskedBitmapTableEntry(int bitmapNumber)
{
	VH_BitmapTableEntry *maskedTable = (VH_BitmapTableEntry *)(ca_graphChunks[ca_gfxInfoE.hdrMasked]);
	return &maskedTable[bitmapNumber];
}

VH_SpriteTableEntry *VH_GetSpriteTableEntry(int spriteNumber)
{
	VH_SpriteTableEntry *spriteTable = (VH_SpriteTableEntry *)(ca_graphChunks[ca_gfxInfoE.hdrSprites]);
	return &spriteTable[spriteNumber];
}

VH_ShiftedSprite *VH_GetShiftedSprite(int chunk)
{
	VH_ShiftedSprite *shifted = (VH_ShiftedSprite *)CA_GetGrChunk(chunk, 0, "Sprite", true);
	return shifted;
}

int VH_GetShiftedSpriteWidth(VH_ShiftedSprite *shifted, int shift)
{
	return shifted->sprShiftByteWidths[shift] * 8;
}

void VH_Plot(int x, int y, int colour)
{
	VL_ScreenRect(x, y, 1, 1, colour);
}

void VH_HLine(int x1, int x2, int y, int colour)
{
	VL_ScreenRect(x1, y, x2 - x1 + 1, 1, colour);
}

void VH_VLine(int y1, int y2, int x, int colour)
{
	VL_ScreenRect(x, y1, 1, y2 - y1 + 1, colour);
}

void VH_Bar(int x, int y, int w, int h, int colour)
{
	VL_ScreenRect(x, y, w, h, colour);
}

void VH_DrawTile8(int x, int y, int tile)
{
	char *ptr = (char *)CA_GetGrChunk(ca_gfxInfoE.offTiles8, 0, "Tile8", true) + (tile * 32);
	VL_UnmaskedToScreen(ptr, x, y, 8, 8);
}

void VH_DrawTile8M(int x, int y, int tile)
{
	char *ptr = (char *)CA_GetGrChunk(ca_gfxInfoE.offTiles8m, 0, "Tile8m", true) + (tile * 40);
	VL_MaskedBlitToScreen(ptr, x, y, 8, 8);
}

void VH_DrawTile16(int x, int y, int tile)
{
	VL_UnmaskedToScreen(CA_GetGrChunk(ca_gfxInfoE.offTiles16, tile, "Tile16", true), x, y, 16, 16);
}

void VH_DrawTile16M(int x, int y, int tile)
{
	if (!ca_graphChunks[ca_gfxInfoE.offTiles16m + tile])
		return;
	VL_MaskedBlitToScreen(ca_graphChunks[ca_gfxInfoE.offTiles16m + tile], x, y, 16, 16);
}

void VH_DrawBitmap(int x, int y, int chunk)
{
	int bitmapNumber = chunk - ca_gfxInfoE.offBitmaps;

	VH_BitmapTableEntry *dimensions = VH_GetBitmapTableEntry(bitmapNumber);

	VL_UnmaskedToScreen(CA_GetGrChunk(chunk, 0, "Bitmap", true), x, y, dimensions->width * 8, dimensions->height);
}

void VH_DrawMaskedBitmap(int x, int y, int chunk)
{
	int bitmapNumber = chunk - ca_gfxInfoE.offMasked;

	VH_BitmapTableEntry *dim = VH_GetMaskedBitmapTableEntry(bitmapNumber);

	VL_MaskedBlitToScreen(CA_GetGrChunk(chunk, 0, "MaskedBitmap", true), x, y, dim->width * 8, dim->height);
}

void VH_DrawSprite(int x, int y, int chunk)
{
	int spriteNumber = chunk - ca_gfxInfoE.offSprites;

	VH_SpriteTableEntry *spr = VH_GetSpriteTableEntry(spriteNumber);

	VH_ShiftedSprite *shifted = VH_GetShiftedSprite(chunk);

	int shift = (x & 7) / 2;

	uint8_t *data = &shifted->data[shifted->sprShiftOffset[shift]];

	int width = shifted->sprShiftByteWidths[shift] * 8;

	VL_MaskedBlitToScreen(data, x & ~7, y, width, spr->height);
}

void VH_DrawSpriteMask(int x, int y, int chunk, int colour)
{
	int spriteNumber = chunk - ca_gfxInfoE.offSprites;

	VH_SpriteTableEntry *spr = VH_GetSpriteTableEntry(spriteNumber);

	VH_ShiftedSprite *shifted = VH_GetShiftedSprite(chunk);

	int shift = (x & 7) / 2;

	uint8_t *data = &shifted->data[shifted->sprShiftOffset[shift]];

	int width = shifted->sprShiftByteWidths[shift] * 8;

	VL_1bppInvBlitToScreen(data, x & ~7, y, width, spr->height, colour);
}

void VH_DrawShiftedSprite(int x, int y, int chunk, int shift)
{
	int spriteNumber = chunk - ca_gfxInfoE.offSprites;

	VH_SpriteTableEntry *spr = VH_GetSpriteTableEntry(spriteNumber);

	VH_ShiftedSprite *shifted = VH_GetShiftedSprite(chunk);

	uint8_t *data = &shifted->data[shifted->sprShiftOffset[shift]];

	int width = shifted->sprShiftByteWidths[shift] * 8;

	VL_MaskedBlitToScreen(data, x, y, width, spr->height);
}

void VH_DrawShiftedSpriteMask(int x, int y, int chunk, int shift, int colour)
{
	int spriteNumber = chunk - ca_gfxInfoE.offSprites;

	VH_SpriteTableEntry *spr = VH_GetSpriteTableEntry(spriteNumber);

	VH_ShiftedSprite *shifted = VH_GetShiftedSprite(chunk);

	uint8_t *data = &shifted->data[shifted->sprShiftOffset[shift]];

	int width = shifted->sprShiftByteWidths[shift] * 8;

	VL_1bppInvBlitToScreen(data, x, y, width, spr->height, colour);
}

void VH_DrawPropChar(int x, int y, int chunk, unsigned char c, int colour)
{
	VH_Font *fnt = (VH_Font *)CA_GetGrChunk(chunk, 3, "Font", true);

	uint8_t *chardata = (uint8_t *)fnt + fnt->location[c];

	VL_1bppXorWithScreen(chardata, x, y, fnt->width[c], fnt->height, colour);
}

void VH_MeasureString(const char *string, uint16_t *width, uint16_t *height, VH_Font *fnt)
{
	*height = fnt->height;

	for (*width = 0; *string; string++)
	{
		*width += fnt->width[(uint8_t)*string];
	}
}

void VH_MeasurePropString(const char *string, uint16_t *width, uint16_t *height, int16_t chunk)
{
	VH_MeasureString(string, width, height, (VH_Font *)CA_GetGrChunk(chunk, 3, "Font", true));
}

// TODO: More arguments passed than in the original code?
void VH_DrawPropString(const char *string, int x, int y, int chunk, int colour)
{
	int w = 0;
	VH_Font *font = (VH_Font *)CA_GetGrChunk(chunk, 3, "Font", true);
	for (w = 0; *string; string++)
	{
		// FIXME: Bad cast to unsigned char, even if it seems to make sense
		VH_DrawPropChar(x + w, y, chunk, (unsigned)(*string), colour);
		w += font->width[(uint8_t)*string];
	}
}

// "Buffer" drawing routines.
// These routines (VHB_*) mark the tiles they draw over as 'dirty', so that
// id_rf can redraw them next frame.

void VHB_DrawTile8(int x, int y, int tile)
{
	x += VL_GetScrollX() & 8;
	y += VL_GetScrollY();

	if (VH_MarkUpdateBlock(x, y, x + 7, y + 7))
		VH_DrawTile8(x, y, tile);
}

void VHB_DrawTile8M(int x, int y, int tile)
{
	x += VL_GetScrollX() & 8;
	y += VL_GetScrollY();

	if (VH_MarkUpdateBlock(x, y, x + 7, y + 7))
		VH_DrawTile8M(x, y, tile);
}

void VHB_DrawTile16(int x, int y, int tile)
{
	x += VL_GetScrollX() & 8;
	y += VL_GetScrollY();

	if (VH_MarkUpdateBlock(x, y, x + 15, y + 15))
		VH_DrawTile8(x, y, tile);
}

void VHB_DrawTile16M(int x, int y, int tile)
{
	x += VL_GetScrollX() & 8;
	y += VL_GetScrollY();

	if (VH_MarkUpdateBlock(x, y, x + 15, y + 15))
		VH_DrawTile8M(x, y, tile);
}

void VHB_DrawBitmap(int x, int y, int chunk)
{
	int bitmapNumber = chunk - ca_gfxInfoE.offBitmaps;
	x += VL_GetScrollX() & 8;
	y += VL_GetScrollY();

	VH_BitmapTableEntry *dimensions = VH_GetBitmapTableEntry(bitmapNumber);

	if (VH_MarkUpdateBlock(x, y, x + dimensions->width * 8, y + dimensions->height))
		VL_UnmaskedToScreen(CA_GetGrChunk(chunk, 0, "Bitmap", true), x, y, dimensions->width * 8, dimensions->height);
}

void VHB_DrawMaskedBitmap(int x, int y, int chunk)
{
	int bitmapNumber = chunk - ca_gfxInfoE.offMasked;
	x += VL_GetScrollX() & 8;
	y += VL_GetScrollY();

	VH_BitmapTableEntry *dim = VH_GetMaskedBitmapTableEntry(bitmapNumber);

	if (VH_MarkUpdateBlock(x, y, x + dim->width * 8, y + dim->height))
		VL_MaskedBlitToScreen(CA_GetGrChunk(chunk, 0, "Bitmap", true), x, y, dim->width * 8, dim->height);
}

void VHB_DrawSprite(int x, int y, int chunk)
{
	int spriteNumber = chunk - ca_gfxInfoE.offSprites;
	x += VL_GetScrollX() & 8;
	y += VL_GetScrollY();

	VH_SpriteTableEntry *spr = VH_GetSpriteTableEntry(spriteNumber);

	VH_ShiftedSprite *shifted = (VH_ShiftedSprite *)CA_GetGrChunk(chunk, 0, "Sprite", true);

	int shift = (x & 7) / 2;

	uint8_t *data = &shifted->data[shifted->sprShiftOffset[shift]];

	int width = shifted->sprShiftByteWidths[shift] * 8;

	int realX = (x + RF_UnitToPixel(spr->originX)) & ~7;
	int realY = y + RF_UnitToPixel(spr->originY);

	if (VH_MarkUpdateBlock(realX, realY, realX + spr->width * 8, realY + spr->height))
	{
		VL_MaskedBlitToScreen(data,
			realX, realY,
			width, spr->height);
	}
}

void VHB_Plot(int x, int y, int colour)
{
	x += VL_GetScrollX() & 8;
	y += VL_GetScrollY();
	if (VH_MarkUpdateBlock(x, y, x + 1, y + 1))
		VL_ScreenRect(x, y, 1, 1, colour);
}

void VHB_HLine(int x1, int x2, int y, int colour)
{
	x1 += VL_GetScrollX() & 8;
	x2 += VL_GetScrollX() & 8;
	y += VL_GetScrollY();
	if (VH_MarkUpdateBlock(x1, y, x2 + 1, y + 1))
		VL_ScreenRect(x1, y, x2 - x1 + 1, 1, colour);
}

void VHB_VLine(int y1, int y2, int x, int colour)
{
	x += VL_GetScrollX() & 8;
	y1 += VL_GetScrollY();
	y2 += VL_GetScrollY();
	if (VH_MarkUpdateBlock(x, y1, x + 1, y2 + 1))
		VL_ScreenRect(x, y1, 1, y2 - y1 + 1, colour);
}

void VHB_Bar(int x, int y, int w, int h, int colour)
{
	x += VL_GetScrollX() & 8;
	y += VL_GetScrollY();
	if (VH_MarkUpdateBlock(x, y, x + w, y + h))
		VL_ScreenRect(x, y, w, h, colour);
}

void VHB_DrawPropString(const char *string, int x, int y, int chunk, int colour)
{
	uint16_t w, h;
	x += VL_GetScrollX() & 8;
	y += VL_GetScrollY();
	// Keen Dreams just marks from (x,y) to the bottom-right of the buffer.
	// Wolf3D+ mark based on the width of the string, and to the bottom of the screen.
	// Both of these mark the updated blocks _after_ rendering. This all seems to be
	// an attempt to avoid looping over the string to measure its length.
	VH_MeasurePropString(string, &w, &h, chunk);
	if (VH_MarkUpdateBlock(x, y, x + w, y + h))
		VH_DrawPropString(string, x, y, chunk, colour);
}

// Mark a block (in pixels) as dirty. Returns true if any tiles were dirtied, false otherwise.
bool VH_MarkUpdateBlock(int x1px, int y1px, int x2px, int y2px)
{
	// Convert pixel coords to tile coords.
	int x1tile = x1px >> 4;
	int y1tile = y1px >> 4;
	int x2tile = x2px >> 4;
	int y2tile = y2px >> 4;

	if (x1tile >= RF_BUFFER_WIDTH_TILES)
		return false;
	x1tile = (x1tile < 0) ? 0 : x1tile;

	if (y1tile >= RF_BUFFER_HEIGHT_TILES)
		return false;
	y1tile = (y1tile < 0) ? 0 : y1tile;

	if (x2tile < 0)
		return false;
	x2tile = (x2tile < RF_BUFFER_WIDTH_TILES - 1) ? x2tile : (RF_BUFFER_WIDTH_TILES - 2);

	if (y2tile < 0)
		return false;
	y2tile = (y2tile < RF_BUFFER_HEIGHT_TILES) ? y2tile : (RF_BUFFER_HEIGHT_TILES - 1);

	for (int y = y1tile; y <= y2tile; y++)
	{
		for (int x = x1tile; x <= x2tile; x++)
		{
			RFL_MarkBlockDirty(x, y, 1, -1);
		}
	}
	return true;
}


void VH_UpdateScreen()
{
	int dirtyTileRun = 0;
	for (int y = 0; y < RF_BUFFER_HEIGHT_TILES; ++y)
	{
		for (int x = 0; x < RF_BUFFER_WIDTH_TILES; ++x)
		{
			if (RFL_IsBlockDirty(x, y, VL_GetActiveBuffer()))
			{
				dirtyTileRun++;
			}
			else if (dirtyTileRun)
			{
				VL_UpdateRect((x-dirtyTileRun)*16,y*16,dirtyTileRun*16,16);
				dirtyTileRun = 0;
			}
			// Note that we don't clear the dirty block buffer here, as we need
			// ID_RF to clear it up when we return to the game.
		}
		if (dirtyTileRun)
			VL_UpdateRect((RF_BUFFER_WIDTH_TILES-dirtyTileRun)*16,y*16,dirtyTileRun*16,16);
		dirtyTileRun = 0;
	}
	VL_Present();
}
