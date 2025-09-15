//===========================================================================
// -- The Globe Dolphin Demo -- Dante Treglia II --
//    -- The Cave -- Nintendo of America, Inc. --
//===========================================================================
#ifndef _PARTICLE_H
#define _PARTICLE_H
#include "list.h"
#include <Dolphin/dolphin.h>

typedef struct Particle_Str {
	BOOL spark;			// Spark? or real particle?

	Vec pos;			// Current position
	Vec vel;			// Current velocity

	Vec pos0;			// Initial position
	Vec vel0;			// Initial velocity
	f32 time0;			// Time of creation

	u32 clrDiff;		// Initial diffuse color
	u32 clrFade;		// Faded diffuse color
	f32 fade;			// Fade progression

	u32 frames;

	u8 draw;			// Draw flag

	struct DLNode_Str * nodeP;
} Particle;

extern Vec currPos;
extern Vec currVel;

void ParticleInit(u32 cnt);
void ParticleDrawPoints();
void ParticleDraw(Mtx mtx);
void ParticleDrawLights();
void ParticleTick(u32 newPartsLimit, PADStatus * padP);
#endif
