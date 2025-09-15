#pragma once


typedef void (*PackFunc)(GXImage* src, byte* dst);
typedef void (*UnpackFunc)(byte* src, GXImage* dst, GXTexObj* obj);


extern ushort		VirtualTlut[2080];
extern bool			EfbAlpha;
extern byte			TileSizeTable[0x10];
extern UnpackFunc	UnpackFuncTable[0x10];
extern PackFunc		PackFuncTable[0x10];


void TexConvertMain(GXTexObj *obj, GXImage* img);
void TexPutTileBuffer(GXImage *src, GXImage *dst, int x, int y);
void TexStretchBuffer(GXImage *img, uint width, uint height);
void TexConvertMainMipmap(GXTexObj *obj, GXImage* img);
bool TexIsZFmt(GXTexFmt fmt);
void TexCopyFBImageMain(byte *dst,int x_orig,int y_orig,uint width,uint height,GXTexFmt fmt);
void TexCopyFBImageMainBF(byte *dst,int x_orig,int y_orig,uint width,uint height,GXTexFmt fmt);
void TexCopyFBImageMainZ(byte *dst,int x_orig,int y_orig,uint width,uint height,GXTexFmt fmt);
void TexCopyFBImageMainZB(byte *dst,int x_orig,int y_orig,uint width,uint height,GXTexFmt fmt);
uint TexTileWidth(GXTexFmt fmt);
uint TexTileHeight(GXTexFmt fmt);
void TexLookUpTlut(GXColor clr, u32 region_idx, u32 tlut);
void TexFlipBufferV(GXImage *img);
void TexBoxFilter(GXImage *src, GXImage *dst);
void TexBoxFilterZ(GXImage *src, GXImage *dst);

