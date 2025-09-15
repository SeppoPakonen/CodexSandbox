#pragma once


#define TEXMAT_ROW_COUNT		63
#define MAT_LIMIT				0x3f

extern Mtx					GXPosTexMatrix[TEXMAT_ROW_COUNT];
extern Mtx					GXNrmMatrix[MAT_LIMIT];
extern GXTexMtxType			GXTexMatrixType[TEXMAT_ROW_COUNT];
extern uint					DefaultPosMatrix;
extern float				CurrentLeft;
extern float				CurrentTop;
extern float				CurrentWd;
extern float				CurrentHt;
extern float				CurrentNear;
extern float				CurrentFar;




void ProjMtxCheck(GLMtx44 mtx, int x, int y, Gxf f);
void MTXCopy2x4(MtxPtr src, MtxPtr dst);


/*
void GXSetProjection(f32 *mtx,GXProjectionType type);
void GXSetProjectionv(f32 *ptr);
void GXGetProjectionv(f32 *ptr);
void GXLoadPosMtxImm(f32 *mtx_ptr,u32 id);
void GXLoadPosMtxIndx(u16 mtxIndx,u32 id);
void GXLoadNrmMtxImm(f32 *mtx_ptr,u32 id);
void GXLoadNrmMtxIndx3x3(u16 mtxIndx,u32 id);
void GXLoadTexMtxImm(f32 *mtx_ptr,u32 id,GXTexMtxType type);
void GXLoadTexMtxIndx(u16 mtx_idx,u32 id,GXTexMtxType type);
void GXSetCurrentMtx(u32 id);
void GXSetViewport(f32 xOrig,f32 yOrig,f32 wd,f32 ht,f32 nearZ,f32 farZ);
void GXSetViewportJitter(f32 xOrig,f32 yOrig,f32 wd,f32 ht,f32 nearZ,f32 farZ,u32 field);
void GXGetViewportv(f32 *vp);
void MTXCopy(f32 * src, f32 * dst);
*/
