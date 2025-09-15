#pragma once


extern GXPixelFmt CurrentPixFmt;


void ClearFrameBuffer(u32 left,u32 top,u32 width,u32 height);
void TexCopyFBImage(byte *dst, int x_orig, int y_orig, uint width, uint height, GXTexFmt fmt, bool is_bitfield);
