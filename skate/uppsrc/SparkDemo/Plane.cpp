#include <Dolphin/dolphin.h>
#include <stdio.h>
#include "plane.h"

//---------------------------------------------------------------------------
// Plane Init Function
//---------------------------------------------------------------------------
Plane * PlaneConstructor(char pAxis, f32 tileX, f32 tileY, u16 tileXNum, 
		u16 tileYNum, u16 texXNum, u16 texYNum, u32 color) {
	Plane * pP;
	f32 pW, pL;
	u32 pVerts;
	u32 wOffset, lOffset, hOffset;
	u32 w, l, index;
	Vec faceNorm;
#ifdef DEBUG_PRINT
	u32 v0, v1, v2, v3;
#endif

	// Init Parameters
	pP = (Plane *)OSAlloc(sizeof(Plane));
	pVerts = pP->vrtCnt = (tileXNum + 1) * (tileYNum + 1);
	pP->axis = pAxis;
	pP->tileX = tileX;
	pP->tileXNum = tileXNum;
	pP->tileY = tileY;
	pP->tileYNum = tileYNum;
	pW = tileXNum * tileX;
	pL = tileYNum * tileY;

#ifdef DEBUG_PRINT
	OSReport("Plane Init (%c) <%i, %i> x [%i, %i]\n", 
		pAxis, tileX, tileY, tileXNum, tileYNum);
#endif
	// Alocate Space
	pP->vtx_f32P = (f32 *)OSAlloc(OSRoundUp32B(pVerts * 3 * sizeof(f32)));
	pP->clr_u32P = (u32 *)OSAlloc(OSRoundUp32B(pVerts * sizeof(u32)));
	pP->tex_f32P = (f32 *)OSAlloc(OSRoundUp32B(pVerts * 2 * sizeof(f32)));

	// Initialize the Vertices & Colors
	// Switch Offsets to match Plane desired
	// Set the Face Normal
	switch(pAxis) {
		case 'X':
		case 'x':
			wOffset = 2;
			lOffset = 1;
			hOffset = 0;
			faceNorm.x = 1.0;
			faceNorm.y = 0.0;
			faceNorm.z = 0.0;
		break;
		case 'Y':
		case 'y':
			wOffset = 0;
			lOffset = 2;
			hOffset = 1;
			faceNorm.x = 0.0;
			faceNorm.y = 1.0;
			faceNorm.z = 0.0;
		break;
		case 'Z':
		case 'z':
			wOffset = 0;
			lOffset = 1;
			hOffset = 2;
			faceNorm.x = 0.0;
			faceNorm.y = 0.0;
			faceNorm.z = 1.0;
		break;
	}
	// Party!
	index = 0;
	for (l = 0; l < tileYNum + 1; l++) {
		for (w = 0; w < tileXNum + 1; w++) {
			pP->vtx_f32P[3 * index + wOffset] = w * tileX - (pW / 2.0);
			pP->vtx_f32P[3 * index + lOffset] = (tileYNum - l) * tileY - (pL / 2.0);
			pP->vtx_f32P[3 * index + hOffset] = 0; 
			pP->clr_u32P[index] = color;
			pP->tex_f32P[2 * index    ] = texXNum * (w * tileX) / pW;
			pP->tex_f32P[2 * index + 1] = texYNum * (l * tileY) / pL; 
			index ++;
		}
	}

#ifdef DEBUG_PRINT
	for (l = 0; l < pP->tileYNum; l++) {
		for (w = 0; w < pP->tileXNum; w++) {
			v0 = l * (pP->tileXNum + 1) + w;
			v1 = v0 + 1;
			v2 = (l + 1) * (pP->tileXNum + 1) + (w + 1);
			v3 = v2 - 1;
			OSReport("Quad <%i, %i, %i, %i>", v0, v1, v2, v3);
			OSReport("\t[%i, %i, %i]\n", 
				pP->vtx_s16P[v0*3 + 0],
				pP->vtx_s16P[v0*3 + 1],
				pP->vtx_s16P[v0*3 + 2]);
			OSReport("\t[%i, %i, %i]\n", 
				pP->vtx_s16P[v1*3 + 0],
				pP->vtx_s16P[v1*3 + 1],
				pP->vtx_s16P[v1*3 + 2]);
			OSReport("\t[%i, %i, %i]\n", 
				pP->vtx_s16P[v2*3 + 0],
				pP->vtx_s16P[v2*3 + 1],
				pP->vtx_s16P[v2*3 + 2]);
			OSReport("\t[%i, %i, %i]\n", 
				pP->vtx_s16P[v3*3 + 0],
				pP->vtx_s16P[v3*3 + 1],
				pP->vtx_s16P[v3*3 + 2]);
		}
	}
	fflush(fileOut);
#endif

	return pP;
}


//---------------------------------------------------------------------------
// Plane
//---------------------------------------------------------------------------
void PlaneDraw(Plane * pP, u8 texture) {
	u32 v0, v1, v2, v3;
	u32 l, w;

	// Setup
	GXSetCullMode(GX_CULL_NONE);
	GXClearVtxDesc();

	// Vertices
	GXSetVtxDesc(GX_VA_POS,  GX_INDEX16);
	GXSetArray(GX_VA_POS, pP->vtx_f32P, 3*sizeof(f32));
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS,  GX_POS_XYZ, GX_F32, 0);

	// Colors
	GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
	GXSetArray(GX_VA_CLR0, pP->clr_u32P, 1*sizeof(u32));
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);

	// Textures
	if (texture) {
		GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
		GXSetArray(GX_VA_TEX0, pP->tex_f32P, 2*sizeof(f32));
		GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	}

	// Draw a plane
	for (l = 0; l < pP->tileYNum; l++) {
		for (w = 0; w < pP->tileXNum; w++) {
			v0 = l * (pP->tileXNum + 1) + w;
			v1 = v0 + 1;
			v2 = (l + 1) * (pP->tileXNum + 1) + (w + 1);
			v3 = v2 - 1;
			GXBegin(GX_QUADS, GX_VTXFMT0, 4);
				GXPosition1x16(v0);
				GXColor1x16(v0);
				if (texture) {
					GXTexCoord1x16(v0);
				}
				GXPosition1x16(v1);
				GXColor1x16(v1);
				if (texture) {
					GXTexCoord1x16(v1);
				}
				GXPosition1x16(v2);
				GXColor1x16(v2);
				if (texture) {
					GXTexCoord1x16(v2);
				}
				GXPosition1x16(v3);
				GXColor1x16(v3);
				if (texture) {
					GXTexCoord1x16(v3);
				}
			GXEnd();
		}
	}
}
