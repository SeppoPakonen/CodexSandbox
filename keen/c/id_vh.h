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

#ifndef ID_VH_H
#define ID_VH_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "ck_cross.h"

typedef CK_PACKED_STRUCT(VH_BitmapTableEntry
{
	uint16_t width;
	uint16_t height;
}) VH_BitmapTableEntry;

typedef CK_PACKED_STRUCT(VH_SpriteTableEntry
{
	uint16_t width, height;
	int16_t originX, originY;
	int16_t xl, yl, xh, yh;
	uint16_t shifts;
}) VH_SpriteTableEntry;

typedef CK_PACKED_STRUCT(VH_Font
{
	uint16_t height;
	uint16_t location[256];
	uint8_t width[256];
}) VH_Font;

VH_BitmapTableEntry *VH_GetBitmapTableEntry(int bitmapNumber);
VH_SpriteTableEntry *VH_GetSpriteTableEntry(int spriteNumber);

#define VH_MAXSPRSHIFTS 4

typedef struct VH_ShiftedSprite
{
	size_t sprShiftOffset[VH_MAXSPRSHIFTS];
	int sprShiftByteWidths[VH_MAXSPRSHIFTS];
	uint8_t data[];
} VH_ShiftedSprite;

VH_ShiftedSprite *VH_GetShiftedSprite(int chunk);
int VH_GetShiftedSpriteWidth(VH_ShiftedSprite *shifted, int shift);

void VH_Plot(int x, int y, int colour);
void VH_HLine(int x1, int x2, int y, int colour);
void VH_VLine(int y1, int y2, int x, int colour);
void VH_Bar(int x, int y, int w, int h, int colour);
void VH_DrawTile8(int x, int y, int tile);
void VH_DrawTile8M(int x, int y, int tile);
void VH_DrawTile16(int x, int y, int tile);
void VH_DrawTile16M(int x, int y, int tile);
void VH_DrawBitmap(int x, int y, int chunk);
void VH_DrawMaskedBitmap(int x, int y, int chunk);
void VH_DrawSprite(int x, int y, int chunk);
void VH_DrawSpriteMask(int x, int y, int chunk, int colour);
void VH_DrawShiftedSprite(int x, int y, int chunk, int shift);
void VH_DrawShiftedSpriteMask(int x, int y, int chunk, int shift, int colour);
void VH_DrawPropChar(int x, int y, int chunk, unsigned char c, int colour);
void VH_MeasurePropString(const char *string, uint16_t *width, uint16_t *height, int16_t chunk);
void VH_DrawPropString(const char *string, int x, int y, int chunk, int colour);

bool VH_MarkUpdateBlock(int x1px, int y1px, int x2px, int y2px);

void VHB_DrawTile8(int x, int y, int tile);
void VHB_DrawTile8M(int x, int y, int tile);
void VHB_DrawTile16(int x, int y, int tile);
void VHB_DrawTile16M(int x, int y, int tile);
void VHB_DrawBitmap(int x, int y, int chunk);
void VHB_DrawMaskedBitmap(int x, int y, int chunk);
void VHB_DrawSprite(int x, int y, int chunk);
void VHB_Plot(int x, int y, int colour);
void VHB_HLine(int x1, int x2, int y, int colour);
void VHB_VLine(int y1, int y2, int x, int colour);
void VHB_Bar(int x, int y, int w, int h, int colour);
void VHB_DrawPropString(const char *string, int x, int y, int chunk, int colour);
void VH_UpdateScreen();
#endif
