//===========================================================================
// -- The Globe Dolphin Demo -- Dante Treglia II --
//	-- The Cave -- Nintendo of America, Inc. --
//===========================================================================
#include <Dolphin/dolphin.h>
#include <demo.h>
#include "fly.h"
#include "plane.h"
#include "particle.h"
#include "list.h"
#include "spline.h"

//---------------------------------------------------------------------------
// Prototypes
//---------------------------------------------------------------------------
static void DrawInit();
static void DrawTick();
static void AnimTick(PADStatus * pad);
static void CameraInit();
static void PrintIntro() {
OSReport("-=- Dolphin Spark Demo -=-\n");
OSReport("Controls:\n");
OSReport("  Pad Button B   -- Control Toggle (Fly/Edit)\n");
OSReport("  Pad Button Y   -- Reflection\n");
OSReport("  Pad Button X   -- Ground Lights\n");
OSReport("  Pad Button A   -- Color\n");
//OSReport("  Numpad 1   -- Turbo\n");
}

//---------------------------------------------------------------------------
// Globals
//---------------------------------------------------------------------------
PADStatus pad[4];
Mtx viewMtx;
Mtx modelViewMtx[2];
Plane * planeP;
Spline * splineP;
GXTexObj texObj[2];
u32 controlMode = 1;
u32 reflectOn = 1;
u32 lightsOn = 1;
char path[256];

#define FLY_MODE 0
#define EDIT_MODE 1

//---------------------------------------------------------------------------
// FPS
//---------------------------------------------------------------------------
f32 fps;
u32 frmCnt;
OSTime fpsStartTime;
OSTime fpsEndTime;
#define FPS_FRAME_CHECK 30
#define FPS() fps
#define FPS_INIT() fpsStartTime = OSGetTime();
#define FPS_FRAME() \
	if (++frmCnt >= FPS_FRAME_CHECK) { \
		fpsEndTime = OSGetTime(); \
		fps = (FPS_FRAME_CHECK * 1000.0) / (float)(fpsEndTime - fpsStartTime); \
		fpsStartTime = OSGetTime(); \
		frmCnt = 0; \
		FLYSetUnitsPerFrame(fps); \
	}

#define PI 3.14159265358979323846

// For HW simulations, use a smaller viewport.
#if __HWSIM
extern GXRenderModeObj  GXRmHW;
GXRenderModeObj *hrmode = &GXRmHW;
#else
GXRenderModeObj *hrmode = NULL;
#endif

//---------------------------------------------------------------------------
// Main
//---------------------------------------------------------------------------
void main ( void )
{
	pad[0].button = 0;

	//GetCurrentDirectory(256, path);
	DEMOInit( hrmode );
	//strcat(path, "\\art\\");
	strcpy(path, installPath);
	strcat(path, "\\build\\demos\\spark\\art");
	DVDSetRoot(path);

	DrawInit();

	PrintIntro();

	CameraInit();

	FPS_INIT();


	while(1)
	{
		DEMOBeforeRender();
		DrawTick();
		DEMODoneRender();
		PADRead(pad);
		AnimTick(pad);

		FPS_FRAME();
	}

	OSHalt("End of test");
}

//---------------------------------------------------------------------------
// Camera
//---------------------------------------------------------------------------
static void CameraInit () {
	Vec eye = {0.0, 8.0, 8.0};
	Vec at = {0.0, 1.0, 0.0};
	Vec up = {0.0, 1.0, 0.0};
	Mtx44 p;

	MTXPerspective(p, 33, 640.0F/480.0F, 0, 3000);
	GXSetProjection(p, GX_PERSPECTIVE);

	FLYInit(viewMtx, &eye, &up, &at);
}

//---------------------------------------------------------------------------
// Draw Init
//---------------------------------------------------------------------------
static void DrawInit() {
	GXColor clear = {0.0, 0.0, 0.0, 0.0};
	TEXPalettePtr tpl = 0;
	int i;

    //  Load the texture palette
    TEXGetPalette(&tpl, "spark.tpl");
	for (i = 0; i < 2; i++) {
	    TEXGetGXTexObjFromPalette(tpl, &texObj[i], i);
	    GXLoadTexObj(&texObj[i], GX_TEXMAP0);
	}

	// Modes
	GXSetCopyClear(clear, GX_MAX_Z24);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_COPY);
	GXSetCullMode(GX_CULL_NONE);
	GXSetZMode(FALSE, GX_NEVER, FALSE);

	// Particles
	ParticleInit(1000);

	planeP = PlaneConstructor('Y', 1, 1, 20, 20, 4, 4, 0xE0828282); 

	// Spline
	splineP = SplineConstructor(4, 100);
}

//---------------------------------------------------------------------------
// Draw a Frame
//---------------------------------------------------------------------------
static void DrawTick() {
	// Spark Refection
	if (reflectOn) {
		GXLoadPosMtxImm(modelViewMtx[0], GX_PNMTX0);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_COPY);
	    GXSetNumTevStages(1);
	    GXSetNumTexGens(1);
	    GXLoadTexObj(&texObj[0], GX_TEXMAP0);
	    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
		GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
		ParticleDraw(modelViewMtx[0]);
	}

	// Ground
	GXLoadPosMtxImm(modelViewMtx[1], GX_PNMTX0);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetNumTevStages(1);
	GXSetNumTexGens(0);
    //GXSetNumTexGens(1);
	GXSetNumChans(1);
    GXLoadTexObj(&texObj[1], GX_TEXMAP0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
	PlaneDraw(planeP, 1);		

	// Lights
	if (lightsOn) {
		GXLoadPosMtxImm(modelViewMtx[1], GX_PNMTX0);
		GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_COPY);
	    GXSetNumTevStages(1);
	    GXSetNumTexGens(1);
	    GXLoadTexObj(&texObj[0], GX_TEXMAP0);
	    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
		GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
		ParticleDrawLights();
	}

	// Spark
	GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_COPY);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXLoadTexObj(&texObj[0], GX_TEXMAP0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
	ParticleDraw(modelViewMtx[1]);
}

//---------------------------------------------------------------------------
// Animate a Frame
//---------------------------------------------------------------------------
static void AnimTick(PADStatus * pad) {
	static first_time = 1;
	static u32 prev = 0;
	u32 down;
	Mtx scale;
	int i;
	static int ticks;
	static float sparkDistance = 0.0;
	int index;
	static float sparkAng = 0.0;
	static float sparkDistAng = 0.0;
	static float sparkDistAng2 = 0.0;

	// Spline
	if (first_time == 1) {
		first_time = 0;
		
		for (i = 0; i < 4; i++) {
			splineP->controlPnt[i].pos.x = 3.0 * cos(sparkAng) + ((float)(rand()%2048))/2048.0F * 1.0F - 0.5F;
			splineP->controlPnt[i].pos.y = 0.0F;
			splineP->controlPnt[i].pos.z = 3.0 * sin(sparkAng) + ((float)(rand()%2048))/2048.0F * 1.0F - 0.5F;
			sparkAng += 0.4F;
		}

		SplineBuild(splineP);
	}


	sparkDistance += 0.2F;

	if ((index = SplineDistanceIndex(splineP, sparkDistance)) == -1) {
		for (i = 0; i < 3; i++) {
			splineP->controlPnt[i].pos.x = splineP->controlPnt[i + 1].pos.x;
			splineP->controlPnt[i].pos.y = splineP->controlPnt[i + 1].pos.y;
			splineP->controlPnt[i].pos.z = splineP->controlPnt[i + 1].pos.z;
		}
		splineP->controlPnt[3].pos.x = 4.0 * cos(sparkDistAng) * cos(sparkAng);
		splineP->controlPnt[3].pos.y = 0.0F;
		splineP->controlPnt[3].pos.z = 4.0 * cos(sparkDistAng) * sin(sparkAng);
		SplineBuild(splineP);
		sparkDistance = 0.0;
		sparkAng += 0.378F + 0.3 * cos(sparkDistAng2 += 0.159F);
		sparkDistAng += 0.312F + 0.2 * sin(sparkDistAng2 += 0.083F);
		index = 0;
	}
	currPos.x = splineP->curvePnt[index].pos.x;
	currPos.y = splineP->curvePnt[index].pos.y;
	currPos.z = splineP->curvePnt[index].pos.z;
	currVel.x = -splineP->curvePnt[index].tan.x;
	currVel.y = -splineP->curvePnt[index].tan.y;
	currVel.z = -splineP->curvePnt[index].tan.z;

	// View
	if (controlMode == FLY_MODE) FLYPadParseData(pad);
	FLYLookAt(viewMtx);

	// Origin (Mirror in Y direction
	MTXScale(scale, 1.0, -1.0, 1.0);
	MTXConcat(viewMtx, scale, modelViewMtx[0]);

	// Origin
	MTXCopy(viewMtx, modelViewMtx[1]);

	// Animate
	ParticleTick(200, (controlMode == EDIT_MODE) ? pad : NULL);

	// Controlls
	down = PADButtonDown(prev, pad->button);
	switch (down) {
		case PAD_BUTTON_B:
			controlMode ^= 1;
			OSReport("%s\n", (controlMode == FLY_MODE) ? "Fight Mode" : "Edit Mode");
		break;
		case PAD_BUTTON_Y:
			reflectOn ^= 1;
			OSReport("Reflection %s\n", (reflectOn) ? "On" : "Off");
		break;
		case PAD_BUTTON_X:
			lightsOn ^= 1;
			OSReport("GroundLights %s\n", (lightsOn) ? "On" : "Off");
		break;
	}
	prev = pad->button;
}
