#pragma once


extern Vec CurrentTexCoords[VTXARR_SIZE];
extern Vec CurrentNormals[VTXARR_SIZE];
extern Vec CurrentTangents[8]; // added
extern Vec CurrentBinormals[8]; // added
extern Vec CurrentPosition;

extern bool DisableCurrent;
extern uint TexMtxIndex[8];
extern uint PosMtxIndex;

extern Vec FinalTangent;
extern Vec FinalPosition;
extern Vec FinalBinormal;
extern GXColor FinalColor;
extern Vec FinalTexCoord[8];

extern GXColor QuadColor;
extern Vec QuadTexCoord[8];
extern Vec QuadPosition;
extern CompStruct* GfxTracker;


void GXCheckApi32(int major, int i, int j, int k);
void CheckVtx16(ushort bitfield);
void CheckVtx32(uint bitfield);
void GXCheckApi16(uint major, short i);
void VecGetF32FromF32(float *x, float *y, float *z, f32 *vtx);
void VecGetF32FromS16(float *x, float *y, float *z, s16 *vtx);
void VecGetF32FromU16(float *x, float *y, float *z, u16 *vtx);
void VecGetF32FromS8(float *x, float *y, float *z, s8 *vtx);
void VecGetF32FromU8(float *x, float *y, float *z, u8 *vtx);
void StopByMismatch(int i);
void GetRGBA6(u8 *r, u8 *g, u8 *b, u8 *a, uint *rgb);
void GetRGBA8(u8 *r, u8 *g, u8 *b, u8 *a, uint *rgb);
void EmGXColor4u8(u8 r, u8 g, u8 b, u8 a);
void GetRGB565(u8 *r, u8 *g, u8 *b, ushort *out);
void GetRGBA4(byte *r, byte *g, byte *b, byte *a, u8 *src);
void GetRGB8(u8 *r, u8 *g, u8 *b, u8 *rgb);
void DumpVertex();
void TransformVertex();
void TransformNormal();
void SelectColorChannel();
void WriteGLVertex(Vec *vec, GXColor *clr, Vec vtx[8]);
void GXVECNormalize(Vec *a,Vec *b);
void TexGenMtx2x4(int i);
void TexGenMtx3x4(int i);
void TexGenBump(int i, int j);
void TexGenSRTG(int i);
void MTXMultVec2x4(MtxPtr m,float srcbase_x,float srcbase_y,float srcbase_z,float *src,float *dst);
void GXPerformLighting();

void GXPositionIndex_Internal(int idx);
void GXColorIndex_Internal(int idx);
void GXPosition3f32_Internal(float x,float y,float z);
void GXColor4u8_Internal(u8 r, u8 g, u8 b, u8 a);
void EmGXNormal3f32_Internal(f32 x,f32 y,f32 z);
void GXNormalIndex_Internal(uint idx);
void GXTexCoord2f32_Internal(float s,float t);
void GXNormal3f32_Internal(float x, float y, float z, float n);
void GXTexCoordIndex_Internal(int idx);
void GXDoTexGen();

/*
void EmGXCmd1u8(u8 x);
void EmGXCmd1u8(u8 x);
void EmGXCmd1u32(u32 x);
void EmGXParam1u8(u8 x);
void EmGXParam1u16(u16 x);
void EmGXParam1u32(u32 x);
void EmGXParam1s8(s8 x);
void EmGXParam1s16(s16 x);
void EmGXParam1s32(s32 x);
void EmGXParam1f32(f32 x);
void EmGXParam3f32(f32 x,f32 y,f32 z);
void EmGXParam4f32(f32 x,f32 y,f32 z,f32 w);
void EmGXPosition3u8(u8 x,u8 y,u8 z);
void EmGXPosition3s8(s8 x,s8 y,s8 z);
void EmGXPosition3u16(u16 x,u16 y,u16 z);
void EmGXPosition3s16(s16 x,s16 y,s16 z);
void EmGXPosition3f32(float x,float y,float z);
void EmGXPosition2u8(u8 x,u8 y);
void EmGXPosition2s8(s8 x,s8 y);
void EmGXPosition2u16(u16 x,u16 y);
void EmGXPosition2s16(s16 x,s16 y);
void EmGXPosition2f32(float x,float y);
void EmGXPosition1x8(u8 idx);
void EmGXPosition1x16(u16 idx);
void EmGXColor3u8(u8 r,u8 g,u8 b);
void EmGXColor1u32(u32 rgba);
void EmGXColor1u16(u16 rgba);
void EmGXColor1x8(u8 idx);
void EmGXColor1x16(u16 idx);
void EmGXNormal3s8(s8 x,s8 y,s8 z);
void EmGXNormal3s16(s16 x,s16 y,s16 z);
void EmGXNormal1x8(u8 idx);
void EmGXNormal1x16(u16 idx);
void EmGXTexCoord2u8(u8 s,u8 t);
void EmGXTexCoord2s8(s8 s,s8 t);
void EmGXTexCoord2u16(u16 s,u16 t);
void EmGXTexCoord2s16(s16 s,s16 t);
void EmGXTexCoord2f32(f32 s,f32 t);
void EmGXTexCoord1u8(u8 s);
void EmGXTexCoord1s8(s8 s);
void EmGXTexCoord1u16(u16 s);
void EmGXTexCoord1s16(s16 s);
void EmGXTexCoord1f32(f32 s);
void EmGXTexCoord1x8(u8 idx);
void __fastcall EmGXTexCoord1x16(u16 idx);
void EmGXMatrixIndex1u8(u32 idx);
undefined __chkesp()
u32 EndianSwapReturn32(void * buffer)
ushort EndianSwapReturn16(void * value)
int TranslateGXAttr_Array(GXAttr attr)
void OSPanic(char * file, int line, char * msg)
int TranslateGXAttr_TexMtxIndex(GXAttr attr)
int TranslateGXTexCoord(GXTexCoordID coord)
void MTXMultVec(f32 * m, VecPtr src, VecPtr dst)
int TranslateGXAttr_VCD(GXAttr attr)
void MTXMultVecSR(Mtx * m, VecPtr src, VecPtr dst)
int TranslateGXTexGenSrc_TexCoord(GXTexGenSrc in)
GXAttr UntranslateGXAttr_TexMtxIndex(int i)
int TranslateGXTexGenSrc_BumpTexCoord(GXTexGenSrc i)
f32 VECDotProduct(Vec * a, Vec * b)
double _sqrt(double __x)
int _sprintf(char * __s, char * __format, ...)
char * TranslateGXCompType_Name(GXChannelID chan, GXCompType type)
char * TranslateGXCompCnt_Name(GXAttr attr, GXCompCnt cnt)
char * TranslateGXAttrType_Name(GXAttrType type)
char * TranslateGXAttr_Name(GXAttr attr)
char * TranslateGXApiID_Name(ApiID api)
*/
