#include "DolphinRE.h"

float				CurrentProjection[7];
Mtx					GXPosTexMatrix[TEXMAT_ROW_COUNT];
Mtx					GXNrmMatrix[MAT_LIMIT];
GXTexMtxType		GXTexMatrixType[TEXMAT_ROW_COUNT];
uint				DefaultPosMatrix;
float				CurrentLeft;
float				CurrentTop;
float				CurrentWd;
float				CurrentHt;
float				CurrentNear;
float				CurrentFar;


void GXSetProjection(const Mtx44 mtx, GXProjectionType type) {
	uint i, j;
	GLMtx44 mtx_trans;
	
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			mtx_trans[i][j] = mtx[j][i];
	
	if (type == GX_PERSPECTIVE) {
		CurrentProjection[0] = 0.0;
		CurrentProjection[1] = mtx[0][0];
		CurrentProjection[2] = mtx[0][2];
		CurrentProjection[3] = mtx[1][1];
		CurrentProjection[4] = mtx[1][2];
		CurrentProjection[5] = mtx[2][2];
		CurrentProjection[6] = mtx[2][3];
		ProjMtxCheck(mtx_trans, 0, 1,  0.0);
		ProjMtxCheck(mtx_trans, 0, 3,  0.0);
		ProjMtxCheck(mtx_trans, 1, 0,  0.0);
		ProjMtxCheck(mtx_trans, 1, 3,  0.0);
		ProjMtxCheck(mtx_trans, 2, 0,  0.0);
		ProjMtxCheck(mtx_trans, 2, 1,  0.0);
		ProjMtxCheck(mtx_trans, 3, 0,  0.0);
		ProjMtxCheck(mtx_trans, 3, 1,  0.0);
		ProjMtxCheck(mtx_trans, 3, 2, -1.0);
		ProjMtxCheck(mtx_trans, 3, 3,  0.0);
	}
	else if (type == GX_ORTHOGRAPHIC) {
		CurrentProjection[0] = 1.0;
		CurrentProjection[1] = mtx[0][0];
		CurrentProjection[2] = mtx[0][3];
		CurrentProjection[3] = mtx[1][1];
		CurrentProjection[4] = mtx[1][3];
		CurrentProjection[5] = mtx[2][2];
		CurrentProjection[6] = mtx[2][3];
		ProjMtxCheck(mtx_trans, 0, 1, 0.0);
		ProjMtxCheck(mtx_trans, 0, 2, 0.0);
		ProjMtxCheck(mtx_trans, 1, 0, 0.0);
		ProjMtxCheck(mtx_trans, 1, 2, 0.0);
		ProjMtxCheck(mtx_trans, 2, 0, 0.0);
		ProjMtxCheck(mtx_trans, 2, 1, 0.0);
		ProjMtxCheck(mtx_trans, 3, 0, 0.0);
		ProjMtxCheck(mtx_trans, 3, 1, 0.0);
		ProjMtxCheck(mtx_trans, 3, 2, 0.0);
		ProjMtxCheck(mtx_trans, 3, 3, 1.0);
	}
	else {
		PANIC("Invalid project");
	}
	
	MatrixMode(PROJECTION);
	LoadMatrix(&mtx_trans[0][0]);
}


void ProjMtxCheck(GLMtx44 mtx, int x, int y, Gxf f) {
	if (mtx[y][x] != f) {
		OSReport("GXSetProjection: m[%d][%d] component should be %f\n", x, y, f);
		PANIC("");
	}
}


void GXSetProjectionv(const f32 *ptr) {
	Gxf mat [16];
	
	for(int i = 0; i < 7; i++)
		CurrentProjection[i] = ptr[i];
	
	if (*ptr == 0.0) {
		mat[0] = ptr[1];
		mat[4] = 0.0;
		mat[8] = ptr[2];
		mat[12] = 0.0;
		mat[1] = 0.0;
		mat[5] = ptr[3];
		mat[9] = ptr[4];
		mat[13] = 0.0;
		mat[2] = 0.0;
		mat[6] = 0.0;
		mat[10] = ptr[5];
		mat[14] = ptr[6];
		mat[3] = 0.0;
		mat[7] = 0.0;
		mat[11] = -1.0;
		mat[15] = 0.0;
	}
	else if (*ptr == 1.0) {
		mat[0] = ptr[1];
		mat[4] = 0.0;
		mat[8] = 0.0;
		mat[12] = ptr[2];
		mat[1] = 0.0;
		mat[5] = ptr[3];
		mat[9] = 0.0;
		mat[13] = ptr[4];
		mat[2] = 0.0;
		mat[6] = 0.0;
		mat[10] = ptr[5];
		mat[14] = ptr[6];
		mat[3] = 0.0;
		mat[7] = 0.0;
		mat[11] = 0.0;
		mat[15] = 1.0;
	}
	else {
		PANIC("Invalid project");
	}
	
	MatrixMode(PROJECTION);
	LoadMatrix(mat);
}


void GXGetProjectionv(f32 *ptr) {
	for(int i = 0; i < 7; i++)
		ptr[i] = CurrentProjection[i];
}


void GXLoadPosMtxImm(MtxPtr mtx_ptr, u32 id) {
	ASSERTMSG(id < TEXMAT_ROW_COUNT, "out of matrix memory area");
	
	MTXCopy(mtx_ptr, GXPosTexMatrix[id]);
}


void GXLoadPosMtxIndx(u16 mtxIndx, u32 id) {
	int i = TranslateGXAttr_Array(GX_POS_MTX_ARRAY);
	MtxPtr src = (MtxPtr)((byte*)GXVtxArray[i].arr + (size_t)GXVtxArray[i].stride * (size_t)mtxIndx);
	
	ASSERTMSG(id < TEXMAT_ROW_COUNT, "out of matrix memory area");
	
	MTXCopy(src, GXPosTexMatrix[id]);
}


void GXLoadNrmMtxImm(Mtx mtx_ptr, u32 id) {
	ASSERTMSG(id < MAT_LIMIT, "out of matrix memory area");
	   
	MTXCopy(mtx_ptr, GXNrmMatrix[id]);
}


void GXLoadNrmMtxIndx3x3(u16 mtxIndx, u32 id) {
	int i = TranslateGXAttr_Array(GX_NRM_MTX_ARRAY);
	MtxPtr mat = (MtxPtr)((byte*)GXVtxArray[i].arr + (size_t)GXVtxArray[i].stride * (size_t)mtxIndx);
	ASSERTMSG(id < MAT_LIMIT, "out of matrix memory area");
	
	Mtx src;
	src[0][0] = mat[0][0];
	src[0][1] = mat[0][1];
	src[0][2] = mat[0][2];
	src[0][3] = 0.0;
	src[1][0] = mat[0][3];
	src[1][1] = mat[1][0];
	src[1][2] = mat[1][1];
	src[1][3] = 0.0;
	src[2][0] = mat[1][2];
	src[2][1] = mat[1][3];
	src[2][2] = mat[2][0];
	src[2][3] = 0.0;
	
	MTXCopy(src, GXNrmMatrix[id]);
}


void GXLoadTexMtxImm(MtxPtr mtx_ptr, u32 id, GXTexMtxType type) {
	if (type == GX_MTX3x4) {
		ASSERTMSG(id < MAT_LIMIT, "out of matrix memory area");
		MTXCopy(mtx_ptr, GXPosTexMatrix[id]);
	}
	else if (type == GX_MTX2x4) {
		ASSERTMSG(id < TEXMAT_ROW_COUNT, "out of matrix memory area");
		MTXCopy2x4(mtx_ptr, GXPosTexMatrix[id]);
	}
	int base = id / 3;
	ASSERT(base >= 0 && base < TEXMAT_ROW_COUNT);
	GXTexMatrixType[base] = type;
}


void GXLoadTexMtxIndx(u16 mtx_idx, u32 id, GXTexMtxType type) {
	int trans_arr = TranslateGXAttr_Array(GX_TEX_MTX_ARRAY);
	MtxPtr src = (MtxPtr)((byte*)GXVtxArray[trans_arr].arr +
				  (size_t)GXVtxArray[trans_arr].stride * (size_t)mtx_idx);
	
	if (type == GX_MTX3x4) {
		ASSERTMSG(id < TEXMAT_ROW_COUNT, "out of matrix memory area");
		MTXCopy(src, GXPosTexMatrix[id]);
	}
	else if (type == GX_MTX2x4) {
		ASSERTMSG(id < TEXMAT_ROW_COUNT, "out of matrix memory area");
		MTXCopy2x4(src, GXPosTexMatrix[id]);
	}
	
	id /= 3;
	ASSERT(id >= 0 && id < TEXMAT_ROW_COUNT);
	GXTexMatrixType[id] = type;
}


void GXSetCurrentMtx(u32 id) {
	ASSERTMSG(id < TEXMAT_ROW_COUNT, "out of matrix memory area");
	DefaultPosMatrix = id;
}


void GXSetViewport(f32 xOrig, f32 yOrig, f32 wd, f32 ht, f32 nearZ, f32 farZ) {
	int height = ht;
	int width = wd;
	int x = xOrig;
	int y = yOrig;
	
	Viewport(x, y, width, height);
	DepthRange(nearZ, farZ);
	
	CurrentLeft = xOrig;
	CurrentTop = yOrig;
	CurrentWd = wd;
	CurrentHt = ht;
	CurrentNear = nearZ;
	CurrentFar = farZ;
}


// This function sets the viewport and adjusts the viewport's line offset for interlaced field
// rendering.  Depending on whether the viewport starts on an even or odd line, and whether the next
// field to be rendered is even (0) or odd (1), the viewport may be adjusted by half a line.  This
// has the same effect as slightly tilting the camera down and is necessary for interlaced
// rendering. No other camera adjustment (i.e. don't change the projection matrix) is needed for
// interlaced field rendering.
void GXSetViewportJitter(f32 xOrig, f32 yOrig, f32 wd, f32 ht, f32 nearZ, f32 farZ, u32 field) {
	
	// Not implemented
	
	GXSetViewport(xOrig, yOrig, wd, ht, nearZ, farZ);
}


void GXGetViewportv(f32 *vp) {
	vp[0] = CurrentLeft;
	vp[1] = CurrentTop;
	vp[2] = CurrentWd;
	vp[3] = CurrentHt;
	vp[4] = CurrentNear;
	vp[5] = CurrentFar;
}


void MTXCopy2x4(MtxPtr src, MtxPtr dst) {
	dst[0][0] = src[0][0];
	dst[0][1] = src[0][1];
	dst[0][2] = src[0][2];
	dst[0][3] = src[0][3];
	dst[1][0] = src[1][0];
	dst[1][1] = src[1][1];
	dst[1][2] = src[1][2];
	dst[1][3] = src[1][3];
}


