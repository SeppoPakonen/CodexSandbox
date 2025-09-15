//===========================================================================
// -- The Globe Dolphin Demo -- Dante Treglia II --
//	-- The Cave -- Nintendo of America, Inc. --
//===========================================================================

// When Sparks Hit the ground they burst into Sparkies

#include <Dolphin/dolphin.h>
#include <math.h>
#include <stdlib.h>
#include "list.h"
#include "particle.h"

u32 partCnt;
Particle * parts;			// Static Node Array
DLNode * nodes;
DLList * freeListP;			// Free Nodes
DLList * sparkListP;		// Used Nodes (Spaeks)
DLList * sparkieListP;		// Used Nodes (Sparkies)
DLList * lightListP;		// Used Nodes (Sparkies)

Vec sparkGravOrig = {0, -9.8F, 0.0};
Vec sparkieGravOrig = {0.0, -5.0F, 0.0};
Vec wind = {0.0, 0.0, 0.0};
Vec sparkGrav = {0, -9.8F, 0.0};
Vec sparkieGrav = {0.0, -5.0F, 0.0};

// From where to shoot sparks!
Vec currPos = {0.0, 0.0, 0.0};
// Which way to shoot the sparks!
Vec currVel = {0.0, 0.0, 0.0};

f32 texCoords[] = {
	0.0, 1.0,
	0.0, 0.0,
	1.0, 0.0,
	1.0, 1.0,
};

u32 sparkDiff[] = {0xFFFFFFFF, 0xFF40FF40, 0xFF4040FF, 0xFFFF4020};
u32 sparkFade[] = {0x20202020, 0x20042004, 0x20040420, 0x20200402};
u32 clrIndx;

f32 currTime;

u32 turboOn;

#define PI 3.14159265358979323846
#define SPARK_VEL 7.0F
#define SPARK_TURBO_VEL 9.0F
#define SPARKIE_VEL 1.5F
#define PART_RAD 0.045F
#define LIGHT_RAD 0.17F

//---------------------------------------------------------------------------
// Util (dst = scale * srcScale + src)
//---------------------------------------------------------------------------
void VECAddScale(float scale, Vec * srcScale, Vec * src, Vec * dst) {
	dst->x = srcScale->x * scale + src->x;
	dst->y = srcScale->y * scale + src->y;
	dst->z = srcScale->z * scale + src->z;
}

//---------------------------------------------------------------------------
// Util (dst = scale * srcScale + src)
//---------------------------------------------------------------------------
void VECCopy(Vec * src, Vec * dst) {
	dst->x = src->x;
	dst->y = src->y;
	dst->z = src->z;
}

//---------------------------------------------------------------------------
// Util 
//---------------------------------------------------------------------------
void VECInverseTransformation(Mtx mtx, Vec * src, Vec * dst) {
	Mtx m;
	
	MTXCopy(mtx, m);

	m[0][3] = 0;
	m[1][3] = 0;
	m[2][3] = 0;

	MTXInverse(m, m);
	MTXMultVec(m, src, dst);
}

//-------------------------------------------------------------------------
// Util An asymptotic effect returns a float that approaches targetX from currX 
//-------------------------------------------------------------------------
float dampType2(float currX, float targetX) {
	return currX + ((targetX - currX) / 16.0F);
}

//---------------------------------------------------------------------------
// Initialize Particles
//---------------------------------------------------------------------------
void ParticleInit(u32 cnt) {
	int i;

	// Save
	partCnt = cnt;

	// Init Lists
	freeListP = DLListConstructor();
	sparkListP = DLListConstructor();
	sparkieListP = DLListConstructor();
	lightListP = DLListConstructor();

	// Alloc Particles
	parts = (Particle *) OSAlloc(cnt * sizeof(Particle));
	memset(parts, 0, cnt * sizeof(Particle));

	// Alloc Nodes
	nodes = (DLNode *) OSAlloc(cnt * sizeof(DLNode));
	memset(nodes, 0, cnt * sizeof(DLNode));

	// Create List
	for (i = 0; i < cnt; i++) {
		nodes[i].infoP = &parts[i];
		nodes[i].index = i;
		parts[i].nodeP = &nodes[i];
		DLAddNode(freeListP, &nodes[i]);
	}

	srand(54321);

	currTime =(float) OSGetTime() / 1000.0F;
}

//---------------------------------------------------------------------------
// Internal Init Functions
//---------------------------------------------------------------------------
static SparkInit(Particle * partP, float currTime) {
	f32 rand1;
	f32 rand2;

	// Clear It
	memset(partP, 0, sizeof(Particle));

	// Position
	partP->pos0.x = currPos.x;
	partP->pos0.y = currPos.y;
	partP->pos0.z = currPos.z;

	// Velocity
	rand1 = ((float) rand() / (float) RAND_MAX) * PI * 2.0f;
	rand2 = ((float) rand() / (float) RAND_MAX) * PI * 0.25f;
	partP->vel0.x = cosf(rand1) * sinf(rand2) * 2.5f + 2.0 * currVel.x;
	partP->vel0.z = sinf(rand1) * sinf(rand2) * 2.5f + 2.0 * currVel.z;
	partP->vel0.y = cosf(rand2);
	partP->vel0.y *= ((float) rand() / (float) RAND_MAX) * (turboOn ? SPARK_TURBO_VEL : SPARK_VEL);

	// Time
	partP->time0 = currTime;

	// Color 
	partP->clrDiff = sparkDiff[clrIndx];
	partP->clrFade = sparkFade[clrIndx];

	// Frame
	partP->frames = 0;
}

//---------------------------------------------------------------------------
// Internal Init Functions
//---------------------------------------------------------------------------
static SparkieInit(Particle * partP, Particle * sparkP, float currTime) {
	f32 rand1;
	f32 rand2;

	// Clear It
	memset(partP, 0, sizeof(Particle));

	// Position
	partP->pos0.x = sparkP->pos.x;
	partP->pos0.y = PART_RAD;
	partP->pos0.z = sparkP->pos.z;

	// Velocity
	rand1 = ((float) rand() / (float) RAND_MAX) * PI * 2.0f;
	rand2 = ((float) rand() / (float) RAND_MAX) * PI * 0.25f;
	partP->vel0.x = sparkP->vel.x * 0.25F + cosf(rand1) * sinf(rand2);
	partP->vel0.z = sparkP->vel.z * 0.25F + sinf(rand1) * sinf(rand2);
	partP->vel0.y = cosf(rand2);
	partP->vel0.y *= ((float) rand() / (float) RAND_MAX) * SPARKIE_VEL;

	// Init
	VECCopy(&partP->pos0, &partP->pos);
	VECCopy(&partP->vel0, &partP->vel);

	// Time
	partP->time0 = currTime;

	// Color 
	partP->clrDiff = sparkP->clrDiff;
	partP->clrFade = sparkP->clrFade;

	// Frame
	partP->frames = 0;
}

//---------------------------------------------------------------------------
// Particles Animation Tick
//---------------------------------------------------------------------------
void ParticleTick(u32 newPartLimit, PADStatus * padP) {
	DLNode * nodeP;
	DLNode * nextP;
	Particle * partP;
	f32 dTime = 0.0;
	Vec pos;
	u32 i;
	u32 color;
	u8 * rgba1P;
	u8 * rgba2P;
	static u32 prev = 0;
	u32 down;
	f32 targetWindX;
	f32 targetWindZ;
	static float tempAng = 0;

	//currPos.x = cos(tempAng += 0.08F);
	//currPos.z = sin(tempAng);

	currTime =(float) OSGetTime() / 1000.0F;

	// ----------------------------
	// Kill All Lights
	while (lightListP->cnt) {
		nodeP = DLPopNode(lightListP);
		DLAddNode(freeListP, nodeP);
	}

	// ----------------------------
	// Emit New Sparks
	while (sparkListP->cnt < newPartLimit) {
		// Manipulate Lists
		nodeP = DLPopNode(freeListP);
		DLAddNode(sparkListP, nodeP);

		// Check for node
		if (nodeP) {
			SparkInit((Particle *)(nodeP->infoP), currTime);
		}
	}

	// ----------------------------
	// Update Physics
	DLTraverseList(sparkListP, nodeP) {
		// Grab Info
		if (nodeP) partP = ((Particle *)(nodeP->infoP));

		// Time 
		dTime = currTime - partP->time0;

		// Math
		partP->pos.x = 0.5F * dTime * dTime * sparkGrav.x + dTime * partP->vel0.x + partP->pos0.x;
		partP->pos.y = 0.5F * dTime * dTime * sparkGrav.y + dTime * partP->vel0.y + partP->pos0.y;
		partP->pos.z = 0.5F * dTime * dTime * sparkGrav.z + dTime * partP->vel0.z + partP->pos0.z;
		partP->vel.x = dTime * sparkGrav.x + partP->vel0.x;
		partP->vel.y = dTime * sparkGrav.y + partP->vel0.y;
		partP->vel.z = dTime * sparkGrav.z + partP->vel0.z;

		// Frames
		partP->frames ++;
	}
	DLTraverseList(sparkieListP, nodeP) {
		// Grab Info
		if (nodeP) partP = ((Particle *)(nodeP->infoP));

		// Time 
		dTime = currTime - partP->time0;

		// Math
		partP->pos.x = 0.5F * dTime * dTime * sparkieGrav.x + dTime * partP->vel0.x + partP->pos0.x;
		partP->pos.y = 0.5F * dTime * dTime * sparkieGrav.y + dTime * partP->vel0.y + partP->pos0.y;
		partP->pos.z = 0.5F * dTime * dTime * sparkieGrav.z + dTime * partP->vel0.z + partP->pos0.z;
		partP->vel.x = dTime * sparkieGrav.x + partP->vel0.x;
		partP->vel.y = dTime * sparkieGrav.y + partP->vel0.y;
		partP->vel.z = dTime * sparkieGrav.z + partP->vel0.z;

		// Color Fade
		if (partP->frames = 0);
		else if (partP->clrFade == 0);
		else {
			rgba1P = (u8 *) &partP->clrDiff;
			rgba2P = (u8 *) &partP->clrFade;
			for (i = 0; i < 4; i++) {
				if (*rgba1P > *rgba2P) *rgba1P -= *rgba2P;
				else *rgba1P = 0;
				rgba1P++;
				rgba2P++;
			}
		}

		// Frames
		partP->frames ++;
	}

	// ----------------------------
	// Kill Sparks (Into Sparkies)
	nodeP = sparkListP->headP;
	while (nodeP) {
		// Grab Info
		partP = ((Particle *)(nodeP->infoP));

		if (partP->pos.y < 0.0) {
			nextP = nodeP->nextP;
			DLDeleteNode(sparkListP, nodeP);
			DLAddNode(lightListP, nodeP);

			// ----------------------------
			// Create Sparkies
			pos.x = partP->pos.x;
			pos.y = partP->pos.y;
			pos.z = partP->pos.z;
			color = partP->clrDiff;

			for (i = 0; i < 4; i++) {
				// Manipulate Lists
				nodeP = DLPopNode(freeListP);
				DLAddNode(sparkieListP, nodeP);

				// No Nodes Left!
				if (!nodeP) break;

				// Init
				SparkieInit((Particle *)(nodeP->infoP), partP, currTime);
			}
			// ----------------------------

			nodeP = nextP;
		}
		else {
			nodeP = nodeP->nextP;
		}
	}

	// ----------------------------
	// Kill Sparkies
	nodeP = sparkieListP->headP;
	while (nodeP) {
		// Grab Info
		partP = ((Particle *)(nodeP->infoP));

		if (partP->pos.y < 0.0) {
			nextP = nodeP->nextP;
			DLDeleteNode(sparkieListP, nodeP);
			DLAddNode(freeListP, nodeP);
			nodeP = nextP;
		}
		else {
			nodeP = nodeP->nextP;
		}
	}

	// ----------------------------
	// Controler
	if (!padP) return; 
	down = PADButtonDown(prev, padP->button);
	switch (down) {
		case PAD_BUTTON_A:
			clrIndx++;
			clrIndx %= 3;
		break;
	}
	prev = padP->button;

	// Turbo
//	if (padP->button & PAD_BUTTON_X) turboOn = 1;
//	else
		turboOn = 0;

	// Wind
	targetWindX = 0.0;
//	if (padP->button & WINKEY_NUMPAD6) targetWindX = 3.0;
//	if (padP->button & WINKEY_NUMPAD4) targetWindX = -3.0;
	wind.x = dampType2(wind.x, targetWindX);
	targetWindZ = 0.0;
//	if (padP->button & WINKEY_NUMPAD8) targetWindZ = 3.0;
//	if (padP->button & WINKEY_NUMPAD2) targetWindZ = -3.0;
	wind.z = dampType2(wind.z, targetWindZ);
	VECAdd(&sparkGravOrig, &wind, &sparkGrav);
	VECAdd(&sparkieGravOrig, &wind, &sparkieGrav);
}

//---------------------------------------------------------------------------
// Draw Particles
//---------------------------------------------------------------------------
void ParticleDrawPoints() {
	u32 color;
	DLNode * nodeP;

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS,  GX_INDEX16);
	GXSetArray(GX_VA_POS, &parts[0].pos, sizeof(Particle));
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS,  GX_POS_XYZ,  GX_F32, 0);

	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);

	glPointSize(2.5);
	if (sparkListP->cnt > 0) {
		GXBegin(GX_POINTS, GX_VTXFMT0, sparkListP->cnt);
		DLTraverseList(sparkListP, nodeP) {
			GXPosition1x16(nodeP->index);
			color = 0xFFFFFFFF;	
			GXColor1u32(color);
		}
		GXEnd();
	}
	glPointSize(2.0);
	if (sparkieListP->cnt > 0) {
		GXBegin(GX_POINTS, GX_VTXFMT0, sparkieListP->cnt);
		DLTraverseList(sparkieListP, nodeP) {
			GXPosition1x16(nodeP->index);
			color = 0xFF0000FF;	
			GXColor1u32(color);
		}
		GXEnd();
	}
}

//---------------------------------------------------------------------------
// Draw Particles
//---------------------------------------------------------------------------
void ParticleDraw(Mtx mtx) {
	DLNode * nodeP;
	Particle * partP;
	Vec pos;
	Vec vel;
	Vec temp;
	f32 mag;
	u32 trails;
	u32 i;
	u32 listCnt = 0;
	Vec vecTL = {-PART_RAD,  PART_RAD,  0.0f};
	Vec vecBL = {-PART_RAD, -PART_RAD,  0.0f};
	Vec vecBR = { PART_RAD, -PART_RAD,  0.0f};
	Vec vecTR = { PART_RAD,  PART_RAD,  0.0f};

	// Tranform Coordinates to Particle Space
	VECInverseTransformation(mtx, &vecTL, &vecTL);
	VECInverseTransformation(mtx, &vecBL, &vecBL);
	VECInverseTransformation(mtx, &vecBR, &vecBR);
	VECInverseTransformation(mtx, &vecTR, &vecTR);

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS,  GX_POS_XYZ,  GX_F32, 0);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
	GXSetArray(GX_VA_TEX0, texCoords, 2*sizeof(f32));
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

	// Make sure there is something to draw
	if (sparkListP->cnt == 0 && sparkieListP->cnt == 0) return;

	nodeP = sparkListP->headP;
	while (nodeP) {
		partP = ((Particle *)(nodeP->infoP));
		VECCopy(&partP->pos, &pos);
		VECCopy(&partP->vel, &vel);

#if 0
		VECScale(&vel, &vel, 0.04F);
		mag = VECMag(&vel);
		trails = (mag  / (PART_RAD * 0.9F)) + 1;
		VECScale(&vel, &vel, 1.0 / (float) trails);
#else
		mag = VECMag(&vel);
		mag *= mag;
		#define SQ(x) ((x) * (x))

		if (mag < SQ(1.0f)) trails = 2;
		else if (mag < SQ(2.0f)) trails = 3;
		else if (mag < SQ(3.0f)) trails = 4;
		else if (mag < SQ(3.5f)) trails = 5;
		else if (mag < SQ(4.0f)) trails = 6;
		else if (mag < SQ(4.5f)) trails = 7;
		else trails = 8;
		VECScale(&vel, &vel, 0.06 / (float) trails);
#endif

		for (i = 0; i < trails; i++) {
			GXBegin(GX_QUADS, GX_VTXFMT0, 4);
				VECAdd(&vecTL, &pos, &temp);
				GXPosition3f32(temp.x, temp.y, temp.z);
				GXColor1u32(partP->clrDiff);
				GXTexCoord1x16(0);

				VECAdd(&vecBL, &pos, &temp);
				GXPosition3f32(temp.x, temp.y, temp.z);
				GXColor1u32(partP->clrDiff);
				GXTexCoord1x16(1);

				VECAdd(&vecBR, &pos, &temp);
				GXPosition3f32(temp.x, temp.y, temp.z);
				GXColor1u32(partP->clrDiff);
				GXTexCoord1x16(2);

				VECAdd(&vecTR, &pos, &temp);
				GXPosition3f32(temp.x, temp.y, temp.z);
				GXColor1u32(partP->clrDiff);
				GXTexCoord1x16(3);
			GXEnd();
			VECAdd(&pos, &vel, &pos);
		}

		nodeP = nodeP->nextP;

		// Switch Lists
		if (nodeP == NULL && listCnt == 0) {
			listCnt ++;
			nodeP = sparkieListP->headP;
		}
	}
}

//---------------------------------------------------------------------------
// Draw Particles
//---------------------------------------------------------------------------
void ParticleDrawLights() {
	DLNode * nodeP;
	Particle * partP;
	Vec pos;
	Vec temp;
	Vec vecTL = {-LIGHT_RAD, 0.0F,  LIGHT_RAD};
	Vec vecBL = {-LIGHT_RAD, 0.0F, -LIGHT_RAD};
	Vec vecBR = { LIGHT_RAD, 0.0F, -LIGHT_RAD};
	Vec vecTR = { LIGHT_RAD, 0.0F,  LIGHT_RAD};

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS,  GX_POS_XYZ,  GX_F32, 0);
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
	GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
	GXSetArray(GX_VA_TEX0, texCoords, 2*sizeof(f32));
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

	// Make sure there is something to draw
	if (lightListP->cnt == 0) return;

	DLTraverseList(lightListP, nodeP) {
		partP = ((Particle *)(nodeP->infoP));
		VECCopy(&partP->pos, &pos);
		pos.y = 0;

		GXBegin(GX_QUADS, GX_VTXFMT0, 4);
			VECAdd(&vecTL, &pos, &temp);
			GXPosition3f32(temp.x, temp.y, temp.z);
			GXColor1u32(partP->clrDiff);
			GXTexCoord1x16(0);

			VECAdd(&vecBL, &pos, &temp);
			GXPosition3f32(temp.x, temp.y, temp.z);
			GXColor1u32(partP->clrDiff);
			GXTexCoord1x16(1);

			VECAdd(&vecBR, &pos, &temp);
			GXPosition3f32(temp.x, temp.y, temp.z);
			GXColor1u32(partP->clrDiff);
			GXTexCoord1x16(2);

			VECAdd(&vecTR, &pos, &temp);
			GXPosition3f32(temp.x, temp.y, temp.z);
			GXColor1u32(partP->clrDiff);
			GXTexCoord1x16(3);
		GXEnd();
	}
}
