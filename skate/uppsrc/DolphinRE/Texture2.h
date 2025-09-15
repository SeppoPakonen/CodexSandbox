#pragma once

u32 GXGetTexBufferSize(u16 width,u16 height,GXTexFmt format,GXBool mipmap,u8 max_lod);
void GXGetTexTileShift(GXTexFmt fmt,uint *x_shift,uint *y_shift);


