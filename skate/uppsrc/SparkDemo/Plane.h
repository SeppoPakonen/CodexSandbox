#ifndef _PLANE_H
#define _PLANE_H
#include <Dolphin/dolphin.h>

//---------------------------------------------------------------------------
// Definitions
//---------------------------------------------------------------------------
typedef struct Plane_Str {
	char axis;
	// W x L of tile + Number of tiles in each direction
	f32 tileX;
	u32 tileXNum;
	f32 tileY;
	u32 tileYNum;
	// Vertex Count
	u32 vrtCnt;
	// Vertices
	f32 * vtx_f32P;
	// Colors
	u32 * clr_u32P;
	// Texture
	f32 * tex_f32P;
	// Face Normals
	f32 * nrm_f32P;
} Plane;

//---------------------------------------------------------------------------
// Prototypes
//---------------------------------------------------------------------------
Plane * PlaneConstructor(char pAxis, f32 tileX, f32 tileY, u16 tileXNum, 
		u16 tileYNum, u16 texXNum, u16 texYNum, u32 color);
void PlaneDraw(Plane * pP, u8 texture);
#endif
