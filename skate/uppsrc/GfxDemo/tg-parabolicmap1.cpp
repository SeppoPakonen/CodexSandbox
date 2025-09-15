/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     tg-spheremap1.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/gxdemo/src/TexGen/tg-parabolicmap1.c $
    
    1     7/19/00 9:41a Dante
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <demo.h>
#include <math.h>

#define ENVIRON_MAP_SIZE  256
#define ENVIRON_MAP_FMT   GX_TF_RGBA8

#define PI 3.14159265358979323846F

/*>*******************************(*)*******************************<*/
static Mtx v, m, mv;
static Mtx44 proj;

static TEXPalettePtr tpl0 = 0; // cube map files
static TEXPalettePtr tpl1 = 0;

static GXTexObj CubeMap[2][6];	// a couple of pre-made cube maps
static GXTexObj EnvironMap[2];	// the dual paraboloid maps (Front, Back)
static u32 CurrentTexture = 0;

/*>*******************************(*)*******************************<*/
// Coord Structure
typedef struct
{
	// Position
	f32 x, y, z;
	// Tex Coord
	f32 s, t;
} Coord;

/*>*******************************(*)*******************************<*/
void        main                ( void );
static void Init2DCamera          ( void );
static void Init3DCamera          ( void );
static void DrawInit            ( void );
static void DrawTick            ( void );
static void AnimTick            ( void );
static void PrintIntro          ( void );

static void DrawCubeMap( GXTexObj * pCubeMaps );
static void DrawTexQuad( GXTexObj * pTexObj );
//void DrawTorus (f32 rc, f32 rt, u8 numc, u8 numt);
void DrawTorus (f32 rc, f32 rt, u8 numc, u8 numt, Vec * pvView);
static void LoadReflectionTexGenMtx ( u32 uTexMapID );
void SphereCubeFaceInit( void );
void SphereCubeFaceCoord(u32 i, u32 j, u32 ijMax, Coord * pCoord);
void DrawSphereCube( GXTexObj * pCubeMaps, Mtx mView );
static void DrawSphereCubes( GXTexObj * pCubeMaps, Mtx mView );
void InitDualParaboloidEnvMaps( GXTexObj* pFrontMap, GXTexObj* pBackMap, 
		u32 uMapSize, u32 uMapFormat);
void GetDualParaboloidEnvMaps( GXTexObj* pFrontMap, GXTexObj* pBackMap, 
		GXTexObj* pCubeMaps);

/*>*******************************(*)*******************************<*/
void main ( void )
{
	DEMOInit(NULL);

	DrawInit();
	PrintIntro();

	while (!(DEMOPadGetButton(0) & PAD_BUTTON_MENU))
	{
		DEMOPadRead();

		AnimTick();

		DEMOBeforeRender();
		DrawTick();
		DEMODoneRender();

	}

	OSHalt("End of test");
}

/*>*******************************(*)*******************************<*/
static void Init2DCamera ( void )
{
	Vec CamLoc = {0.0F, 0.0F, 6.0F};
	Vec Up     = {0.0F, 1.0F, 0.0F};
	Vec ObjPt  = {0.0F, 0.0F, 0.0F};
	Mtx44 p;

	// set viewport to square
	GXSetViewport(0.0F, 0.0F, 400.0F, 400.0F, 0.0F, 1.0F);

	// set matrices
	MTXOrtho(p, 4.0f, -4.0f, -4.0f, 4.0f, 1.0f, 100.0f);
	GXSetProjection(p, GX_ORTHOGRAPHIC);
	MTXLookAt(v, &CamLoc, &Up, &ObjPt);
}

/*>*******************************(*)*******************************<*/
static void Init3DCamera ( void )
{
	Vec CamLoc = {0.0F, 0.0F, 6.0F};
	Vec Up     = {0.0F, 1.0F, 0.0F};
	Vec ObjPt  = {0.0F, 0.0F, 0.0F};
	Mtx44 p;

	// set viewport to square
	GXSetViewport(0.0F, 0.0F, 640.0F, 448.0F, 0.0F, 1.0F);

	// set matrices
	MTXPerspective(p, 33, 640.0F/448.0F, 1.0f, 100.0f);
	GXSetProjection(p, GX_PERSPECTIVE);
	MTXLookAt(v, &CamLoc, &Up, &ObjPt);
}

/*>*******************************(*)*******************************<*/
static void DrawInit    ( void )
{
	u32              i;
	TEXDescriptorPtr tdp;

	// no zbuffer needed to create the sphere map
	GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
	GXSetCullMode(GX_CULL_BACK);

	// read cube map textures
	OSReport("Opening gxTests/tg-cube.tpl\n");
	TEXGetPalette(&tpl0, "gxTests/tg-cube.tpl");
	for (i = 0; i < 6; i++)
	{
		tdp = TEXGet(tpl0, i);
		GXInitTexObj(&CubeMap[0][i], 
				tdp->textureHeader->data, 
				tdp->textureHeader->width, 
				tdp->textureHeader->height, 
				(GXTexFmt)tdp->textureHeader->format,
				GX_CLAMP, 
				GX_CLAMP, 
				GX_FALSE); 

		// alpha should be zero on edges, clamp so sphere outside
		// projected texture is not overwritten
		GXInitTexObjLOD(&CubeMap[0][i], 
				tdp->textureHeader->minFilter, 
				tdp->textureHeader->magFilter, 
				tdp->textureHeader->minLOD, 
				tdp->textureHeader->maxLOD, 
				tdp->textureHeader->LODBias, 
				GX_FALSE,
				tdp->textureHeader->edgeLODEnable,
				GX_ANISO_1); 
	}

	// read pre-made cube map textures
	// order of loading: right, front, left, back, top, bottom
	OSReport("Opening gxTests/tg-cube1.tpl\n");
	TEXGetPalette(&tpl1, "gxTests/tg-cube1.tpl");
	for (i = 0; i < 6; i++)
	{
		tdp = TEXGet(tpl1, i);
		GXInitTexObj(&CubeMap[1][i], 
				tdp->textureHeader->data, 
				tdp->textureHeader->width, 
				tdp->textureHeader->height, 
				(GXTexFmt)tdp->textureHeader->format,
				GX_CLAMP, 
				GX_CLAMP, 
				GX_FALSE); 

		// alpha should be zero on edges, clamp so sphere outside
		// projected texture is not overwritten
		GXInitTexObjLOD(&CubeMap[1][i], 
				tdp->textureHeader->minFilter, 
				tdp->textureHeader->magFilter, 
				tdp->textureHeader->minLOD, 
				tdp->textureHeader->maxLOD, 
				tdp->textureHeader->LODBias, 
				GX_FALSE,
				tdp->textureHeader->edgeLODEnable,
				GX_ANISO_1); 
	}

	// initialize maps
	InitDualParaboloidEnvMaps( &EnvironMap[0], &EnvironMap[1], ENVIRON_MAP_SIZE, ENVIRON_MAP_FMT );
	// get first map
	GetDualParaboloidEnvMaps( &EnvironMap[0], &EnvironMap[1], CubeMap[0] );
}

/*>*******************************(*)*******************************<*/
static void DrawTick    ( void )
{
	static float ang = 0.0F;
	Mtx t, mvi;
	Vec viewVec = {0.0f, 0.0f, -1.0f};

	// disable z
	GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);

	// set 2D camera 
	Init2DCamera();

	MTXIdentity(m);
	MTXTrans(m, 0, 1, 0);
	MTXConcat(v, m, mv);
	GXLoadPosMtxImm(mv, GX_PNMTX0);

	MTXIdentity(m);
	MTXTrans(m, 0.0F, -3.0F, 0.0F);
	MTXConcat(v, m, mv);
	GXLoadPosMtxImm(mv, GX_PNMTX0);
	DrawCubeMap(CubeMap[CurrentTexture]);

	MTXScale(m, 4.0F, 4.0F, 4.0F);
	MTXConcat(v, m, mv);
	GXLoadPosMtxImm(mv, GX_PNMTX0);
	DrawTexQuad(&EnvironMap[0]);

	MTXTrans(t, -4.0F, 0.0F, 0.0F);
	MTXScale(m, 4.0F, 4.0F, 4.0F);
	MTXConcat(t, m, m);
	MTXConcat(v, m, mv);
	GXLoadPosMtxImm(mv, GX_PNMTX0);
	DrawTexQuad(&EnvironMap[1]);

	// set 3D camera
	Init3DCamera();

	// state
	GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_NRM, GX_TEXMTX0);
	GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GXSetNumTevStages(1);
	GXSetNumTexGens(1);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0,
			GX_FALSE,		 // enable Channel
			GX_SRC_VTX,		 // amb source       (Don't care)
			GX_SRC_VTX,		 // mat source
			GX_LIGHT0,		 // light mask       (Don't care)
			GX_DF_NONE,		 // diffuse function (Don't care)
			GX_AF_NONE);	 // atten   function (Don't care)


	// matrix
	MTXRotDeg(m, 'y', ang += 1.0f);
	MTXConcat(v, m, mv);
	GXLoadPosMtxImm(mv, GX_PNMTX0);
    MTXInverse(mv, mvi); 
    MTXTranspose(mvi, mvi); 
	GXLoadNrmMtxImm(mvi, GX_PNMTX0);

	// load texture matrix
	LoadReflectionTexGenMtx(GX_TEXMTX0);

	// enable Z
	GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);

	// draw
	MTXMultVec(mvi, &viewVec, &viewVec);
	DrawTorus(0.5F, 1.0F, 32, 32, &viewVec);
}

/*>*******************************(*)*******************************<*/
static void AnimTick    ( void )
{
	u16 buttons = DEMOPadGetButtonDown(0);

	if (buttons & PAD_BUTTON_A)
	{
		CurrentTexture ^= 1;
		GetDualParaboloidEnvMaps( &EnvironMap[0], &EnvironMap[1], CubeMap[CurrentTexture] );
	}
}


/*---------------------------------------------------------------------------*
	Name:           PrintIntro
	
	Description:    Prints the directions on how to use this demo.
					
	Arguments:      none
	
	Returns:        none
 *---------------------------------------------------------------------------*/
static void PrintIntro( void )
{
}

/*---------------------------------------------------------------------------*
	Name:           DrawCubeMap
	
	Description:    Draws a Cross with a cube map on it
					
	Arguments:      pCubeMaps: an array of 6 cube texture maps
	
	Returns:        none
 *---------------------------------------------------------------------------*/
static void DrawCubeMap( GXTexObj * pCubeMaps ) {
	GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
	GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetNumTevStages(1);
	GXSetNumTexGens(1);
	GXSetNumChans(0);

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

	GXInvalidateTexAll();

	// Left
	GXLoadTexObj(&pCubeMaps[0], GX_TEXMAP0);    
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(0.0F, 2.0F, 0.0F);
	GXTexCoord2f32(0.0F, 0.0F);
	GXPosition3f32(1.0F, 2.0F, 0.0F);
	GXTexCoord2f32(1.0F, 0.0F);
	GXPosition3f32(1.0F, 1.0F, 0.0F);
	GXTexCoord2f32(1.0F, 1.0F);
	GXPosition3f32(0.0F, 1.0F, 0.0F);
	GXTexCoord2f32(0.0F, 1.0F);
	GXEnd();
	// Front
	GXLoadTexObj(&pCubeMaps[1], GX_TEXMAP0);    
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(1.0F, 2.0F, 0.0F);
	GXTexCoord2f32(0.0F, 0.0F);
	GXPosition3f32(2.0F, 2.0F, 0.0F);
	GXTexCoord2f32(1.0F, 0.0F);
	GXPosition3f32(2.0F, 1.0F, 0.0F);
	GXTexCoord2f32(1.0F, 1.0F);
	GXPosition3f32(1.0F, 1.0F, 0.0F);
	GXTexCoord2f32(0.0F, 1.0F);
	GXEnd();
	// Right
	GXLoadTexObj(&pCubeMaps[2], GX_TEXMAP0);    
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(2.0F, 2.0F, 0.0F);
	GXTexCoord2f32(0.0F, 0.0F);
	GXPosition3f32(3.0F, 2.0F, 0.0F);
	GXTexCoord2f32(1.0F, 0.0F);
	GXPosition3f32(3.0F, 1.0F, 0.0F);
	GXTexCoord2f32(1.0F, 1.0F);
	GXPosition3f32(2.0F, 1.0F, 0.0F);
	GXTexCoord2f32(0.0F, 1.0F);
	GXEnd();
	// Back
	GXLoadTexObj(&pCubeMaps[3], GX_TEXMAP0);    
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(3.0F, 2.0F, 0.0F);
	GXTexCoord2f32(0.0F, 0.0F);
	GXPosition3f32(4.0F, 2.0F, 0.0F);
	GXTexCoord2f32(1.0F, 0.0F);
	GXPosition3f32(4.0F, 1.0F, 0.0F);
	GXTexCoord2f32(1.0F, 1.0F);
	GXPosition3f32(3.0F, 1.0F, 0.0F);
	GXTexCoord2f32(0.0F, 1.0F);
	GXEnd();
	// Top
	GXLoadTexObj(&pCubeMaps[4], GX_TEXMAP0);    
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(1.0F, 3.0F, 0.0F);
	GXTexCoord2f32(0.0F, 0.0F);
	GXPosition3f32(2.0F, 3.0F, 0.0F);
	GXTexCoord2f32(1.0F, 0.0F);
	GXPosition3f32(2.0F, 2.0F, 0.0F);
	GXTexCoord2f32(1.0F, 1.0F);
	GXPosition3f32(1.0F, 2.0F, 0.0F);
	GXTexCoord2f32(0.0F, 1.0F);
	GXEnd();
	// Bottom
	GXLoadTexObj(&pCubeMaps[5], GX_TEXMAP0);    
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(1.0F, 1.0F, 0.0F);
	GXTexCoord2f32(0.0F, 0.0F);
	GXPosition3f32(2.0F, 1.0F, 0.0F);
	GXTexCoord2f32(1.0F, 0.0F);
	GXPosition3f32(2.0F, 0.0F, 0.0F);
	GXTexCoord2f32(1.0F, 1.0F);
	GXPosition3f32(1.0F, 0.0F, 0.0F);
	GXTexCoord2f32(0.0F, 1.0F);
	GXEnd();
}

/*---------------------------------------------------------------------------*
	Name:           DrawTexQuad
	
	Description:    Draws a Quad with one texture
					
	Arguments:      pTexObj: The desired texture
	
	Returns:        none
 *---------------------------------------------------------------------------*/
static void DrawTexQuad( GXTexObj * pTexObj ) {
	GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
	GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetNumTevStages(1);
	GXSetNumTexGens(1);
	GXSetNumChans(0);

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

	GXInvalidateTexAll();

	// Left
	GXLoadTexObj(pTexObj, GX_TEXMAP0);    
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition3f32(0.0F, 1.0F, 0.0F);
	GXTexCoord2f32(0.0F, 0.0F);
	GXPosition3f32(1.0F, 1.0F, 0.0F);
	GXTexCoord2f32(1.0F, 0.0F);
	GXPosition3f32(1.0F, 0.0F, 0.0F);
	GXTexCoord2f32(1.0F, 1.0F);
	GXPosition3f32(0.0F, 0.0F, 0.0F);
	GXTexCoord2f32(0.0F, 1.0F);
	GXEnd();
}

/*---------------------------------------------------------------------------*
	Name:           DrawTorus
	
	Description:    Draws a Torus
					
	Arguments:      rc: radius 1
					rt: radius 2
					numc: number of samples in radius 1
					numt: number of samples in radius 2 
	
	Returns:        none
 *---------------------------------------------------------------------------*/
void DrawTorus (f32 rc, f32 rt, u8 numc, u8 numt, Vec * pvView)
{
	s32 i, j, k;
	f32 s, t;
	f32 x, y, z;
	f32 twopi = 2.0F * PI;

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_NRM, GX_DIRECT);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);

	for (i = 0; i < numc; i++)
	{
		GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (u16)((numt+1) * 2));
		for (j = 0; j <= numt; j++)
		{
			for (k = 0; k <= 1; k++)
			{
				s = (i + k) % numc + 0.5F;
				t = (f32)(j % numt);

				x = (rt + rc * cosf(s*twopi/numc)) * cosf(t*twopi/numt);
				y = (rt + rc * cosf(s*twopi/numc)) * sinf(t*twopi/numt);
				z = rc * sinf(s*twopi/numc);
				GXPosition3f32(x, y, z);

				x = cosf(t*twopi/numt) * cosf(s*twopi/numc);
				y = sinf(t*twopi/numt) * cosf(s*twopi/numc);
				z = sinf(s*twopi/numc);
#if 1
				{
					Vec tmp;
					tmp.x = x;
					tmp.y = y;
					tmp.z = z;
					VECReflect(pvView, &tmp, &tmp);
					GXNormal3f32(tmp.x, tmp.y, tmp.z);
				}
#else
				GXNormal3f32(x, y, z);
#endif

				GXColor1u32(0xFFFFFFFF);

			}
		}
		GXEnd();
	}
}

/*---------------------------------------------------------------------------*
	Name:           LoadReflectionTexGenMtx
	
	Description:    Load Reflection Texture Coord Generations Matrix
					
	Arguments:      uTexMapID: texture map to load matrix in
	
	Returns:        none
 *---------------------------------------------------------------------------*/
static void LoadReflectionTexGenMtx ( u32 uTexMapID )
{
    Mtx ry, rz, mv, t, s;

    //MTXRotDeg(ry, 'Y', (float)rot);
    //MTXRotDeg(rz, 'Z', (float)rot);
    MTXScale(s, -0.5F, -0.5F, 0.0F);
    MTXTrans(t, 0.5F, 0.5F, 1.0F);

    //MTXConcat(rz, ry, mv);
    //MTXInverse(mv, mv);
    //MTXTranspose(mv, mv);
	MTXIdentity(mv);
    MTXConcat(s, mv, mv);
    MTXConcat(t, mv, mv);

    GXLoadTexMtxImm(mv, uTexMapID, GX_MTX3x4);
}

/*---------------------------------------------------------------------------*
 *---------------------------------------------------------------------------*/
/*
// Coord Structure
typedef struct {
	// Position
	f32 x, y, z;
	// Tex Coord
	f32 s, t;
} Coord;
*/

// Stores the Geometry information for one cube face projected
// onto a sphere called a "Sphere Cube Face" in this demo
Coord SphereCubeFace[64+1][64+1];

// Texture Sample in each (s,t) direction. The higher the number the 
// better the sample.
u32 TextureSample = 32;

// Used to rotate the projection for each face of the cube
static f32 CubeAngle[6] = {270.0F, 0.0F, 90.0F, 180.0F, -90.0F, 90.0F};
static char CubeAxis[6] = { 'y',    'y',    'y',  'y',   'x',    'x'};

/*---------------------------------------------------------------------------*
	Name:           SphereCubeFaceCoord
	
	Description:    Maps a coord on a cube face (i,j) to a point on a sphere
					then stores the resulting position and texture coord.
					
	Arguments:      i: [0, ijMax] current step in x direction
					j: [0, ijMax] current step in y direction
					ijMax: The maximum steps of i and j
					pCoord: pointer to coord structure. stores result.
	
	Returns:        none
 *---------------------------------------------------------------------------*/
void SphereCubeFaceCoord(u32 i, u32 j, u32 ijMax, Coord * pCoord)
{
	f32 *x; f32 *y; f32 *z;
	x = &pCoord->x;
	y = &pCoord->y;
	z = &pCoord->z;

	// projection
	*x = (f32) 2.0f*i/ijMax - 1.0f;
	*y = (f32) 2.0f*j/ijMax - 1.0f;
	*z = 1/sqrt(*x**x + *y**y + 1);
	*x *= *z;
	*y *= *z;

	// texture coords
	pCoord->s = (f32)i/ijMax;
	pCoord->t = 1.0F - (f32)j/ijMax;
}

/*---------------------------------------------------------------------------*
	Name:           SphereCubeFaceInit
	
	Description:    Generates the Geometry information for one Cube Face.
					Only one is needed since it may be rotated to represent
					all the faces of the cube.
					
	Arguments:      none
	
	Returns:        none
 *---------------------------------------------------------------------------*/
void SphereCubeFaceInit( void )
{
	u32 i, j;

	for (i=0; i <= TextureSample; i++)
		for (j=0; j <= TextureSample; j++)
		{
			SphereCubeFaceCoord(i, j, TextureSample, &SphereCubeFace[i][j]);
		}
}

/*---------------------------------------------------------------------------*
	Name:           DrawSphereCube
	
	Description:    Draws the SphereCubeFace 6 times for each face of a cube
					by rotating the stored primitive data. Each face has the
					corresponding cube map.
					
	Arguments:      pCubeMaps: the cube map. an array of 6 textures.
					mView: the current view matrix.
	
	Returns:        none
 *---------------------------------------------------------------------------*/
void DrawSphereCube( GXTexObj * pCubeMaps, Mtx mView )
{
	u32 i, j, face;
	Coord * pC;
	Mtx mFace, mModelView;

	GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
	GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetNumTevStages(1);
	GXSetNumTexGens(1);
	GXSetNumChans(0);

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

	GXInvalidateTexAll();

	for (face=0; face< 6; face++)
	{
		// load cube map corresponding to face
		GXLoadTexObj(&pCubeMaps[face], GX_TEXMAP0);    

		// rotate gemoetry to face the cube face
		MTXRotDeg(mFace, CubeAxis[face], CubeAngle[face]);
		MTXConcat(mView, mFace, mModelView);
		GXLoadPosMtxImm(mModelView, GX_PNMTX0);

		for (i=0; i < TextureSample; i++)
		{
#if 1
			GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (u16)(TextureSample * 2 + 2));
#else
			GXBegin(GX_POINTS, GX_VTXFMT0, TextureSample*2 + 2);
#endif

			for (j=0; j <= TextureSample; j++)
			{
				pC = &SphereCubeFace[i][j];
				GXPosition3f32(pC->x, pC->y, pC->z);
				GXTexCoord2f32(pC->s, pC->t);
				pC = &SphereCubeFace[i+1][j];
				GXPosition3f32(pC->x, pC->y, pC->z);
				GXTexCoord2f32(pC->s, pC->t);
			}

			GXEnd();
		}
	}
}

/*---------------------------------------------------------------------------*
	Name:           DrawSphereCubes
	
	Description:    Draws the both Front and Back Sphere Cubes 
					
	Arguments:      pCubeMaps: the cube map. an array of 6 textures.
					mView: the current view matrix.
	
	Returns:        none
 *---------------------------------------------------------------------------*/
void DrawSphereCubes( GXTexObj * pCubeMaps, Mtx mView ) {
	Mtx mModel;
	Mtx mModelView;

	MTXIdentity(mModel);
	MTXConcat(mView, mModel, mModelView);
	DrawSphereCube(pCubeMaps, mModelView);

	MTXTrans(mModel, 2, 0, 0);
	mModel[0][0] = -1.0f;
	mModel[1][1] = 1.0f;
	mModel[2][2] = -1.0f;
	//mTran[2][3] = -1.0f;
	MTXConcat(mView, mModel, mModelView);
	DrawSphereCube(pCubeMaps, mModelView);
}

/*---------------------------------------------------------------------------*
	Name:           GetDualParaboloidEnvMaps
	
	Description:    Initializes Two Environment Maps (Front and Back).
					Note: Should only be called once! Data for texture
					is allocated in this function. This function also
					initializes the sphere cube geometry.
					
	Arguments:      pFrontMap: pointer to a TexObj structure
					pBackMap: pointer to a TexObj structure
					uMapSize: size of env map
					uMapFormat: Format of env map
	
	Returns:        none
 *---------------------------------------------------------------------------*/
void InitDualParaboloidEnvMaps( GXTexObj* pFrontMap, GXTexObj* pBackMap, 
		u32 uMapSize, u32 uMapFormat) {

	void * tex_buffer;

	// alocate Data
	tex_buffer = (void*)OSAlloc(
			GXGetTexBufferSize((u16)uMapSize, (u16)uMapSize, uMapFormat, GX_FALSE, (u8)0));
	// init TexObj
	GXInitTexObj(
			pFrontMap,
			tex_buffer,
			(u16)uMapSize,
			(u16)uMapSize,
			(GXTexFmt)uMapFormat,
			GX_CLAMP,
			GX_CLAMP,
			GX_FALSE);

	// alocate Data
	tex_buffer = (void*)OSAlloc(
			GXGetTexBufferSize((u16)uMapSize, (u16)uMapSize, uMapFormat, GX_FALSE, 0));
	// init TexObj
	GXInitTexObj(
			pBackMap,
			tex_buffer,
			(u16)uMapSize,
			(u16)uMapSize,
			(GXTexFmt)uMapFormat,
			GX_CLAMP,
			GX_CLAMP,
			GX_FALSE);

	// Initialize the Sphere Cube Geometry Info
	SphereCubeFaceInit();
}

/*---------------------------------------------------------------------------*
	Name:           GetDualParaboloidEnvMaps
	
	Description:    Creates Two Environment Maps (Front and Back).
					
	Arguments:     	pFrontMap: pointer to initialized TexObj structure 
					pBackMap: pointer to initialized TexObj structure 
					pCubeMaps: pointer an array of 6 cube maps
	
	Returns:        none
 *---------------------------------------------------------------------------*/
void GetDualParaboloidEnvMaps( GXTexObj* pFrontMap, GXTexObj* pBackMap, 
		GXTexObj* pCubeMaps) {
	u16        width, height;
	GXTexFmt   fmt;
	void*      frontData;
	void*      backData;
	Mtx44      p;
	Mtx        mModelView;
	Mtx        mView;
	Mtx        mModel;
	Vec lEye = {0.0F, 0.0F, 6.0F};
	Vec lUp  = {0.0F, 1.0F, 0.0F};
	Vec lAt  = {0.0F, 0.0F, 0.0F};
	GXColor    black = {0, 0, 0, 0};
	GXColor    grey = {128, 128, 128, 128};

	// snag texture info 
	// (Note: Assuming Back and Front are the same size and format)
	width  = GXGetTexObjWidth(pFrontMap);
	height = GXGetTexObjHeight(pFrontMap);
	fmt    = GXGetTexObjFmt(pFrontMap);
	frontData   = GXGetTexObjData(pFrontMap);
	backData   = GXGetTexObjData(pBackMap);

	// set projection
	MTXOrtho(p, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 100.0f);
	GXSetProjection(p, GX_ORTHOGRAPHIC);

	// set view matrix
	MTXLookAt(mView, &lEye, &lUp, &lAt);

	// don't draw the back
	GXSetCullMode(GX_CULL_BACK);

	// size the sphere map
	GXSetViewport(0.0F, 0.0F, 
			(f32)width, (f32)height, 
			0.0F, 1.0F);

	// set copy parameters
	GXSetTexCopySrc(0, 0, width, height);
	GXSetTexCopyDst(width, height, fmt, GX_FALSE);

	// set a black background
	GXSetCopyClear(black, GX_MAX_Z24);

	// clear the background
	GXCopyDisp(DEMOGetCurrentBuffer(), GX_TRUE);

	// **** Front Sphere Cube ****

	// set Model View Matrix
	MTXIdentity(mModel);
	MTXConcat(mView, mModel, mModelView);

	// draw Sphere Cube geometry
	DrawSphereCube(pCubeMaps, mModelView);

	// copy frame buffer to texture
	GXCopyTex(frontData, GX_TRUE); // clear old texture
	GXPixModeSync(); // prevent data from being used until copy completes

	// **** Back Sphere Cube ****

	// set Model View Matrix
	mModel[0][0] = -1.0f;
	mModel[1][1] = 1.0f;
	mModel[2][2] = -1.0f;
	MTXConcat(mView, mModel, mModelView);
	DrawSphereCube(pCubeMaps, mModelView);

	// copy frame buffer to texture
	GXCopyTex(backData, GX_TRUE); // clear old texture
	GXPixModeSync(); // prevent data from being used until copy completes

	// set the background to a grey
	GXSetCopyClear(grey, GX_MAX_Z24);
}
