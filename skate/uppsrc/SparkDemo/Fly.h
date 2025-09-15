//===========================================================================
//===========================================================================
#ifndef _FLY_H
#define _FLY_H
#include <Dolphin/dolphin.h>

#define UNITS_PER_SEC 40.0F
#define FLYSetUnitsPerFrame(framesPerSecond) \
	(flyCam.unitsPerFrame = UNITS_PER_SEC / framesPerSecond)

// Camera Structure
typedef struct GFX_FlyCam_Str {
	// Position (Point)
	Vec eye;
	// Forward (Point) (IE LookAt)
	Vec at;
	// Up (Vector)
	Vec up;
	// Forward (Vector) (IE View Direction)
	Vec fwd;
	// Side (right)
	Vec side;

	// Orientation data
	// Azimuth, Elevation, and Roll
	float A, E, R;

	// Max Speed
	float unitsPerFrame;

	// Forward Speed
	float fwdS;
	// Side Speed
	float sideS;
	// Up Speed
	float upS;
} GFX_FlyCam;

// Prototypes
#ifdef OPEN_GL
	void FLYInit(Vec * eye, Vec * at, Vec * up);
	void FLYLookAt();
#else
	void FLYInit(Mtx viewMtx, Vec * eye, Vec * at, Vec * up);
	void FLYLookAt(Mtx viewMtx);
#endif
void FLYPrintData();
void FLYPadParseData(PADStatus * padP);
float dampType1(float currX, float targetX);

extern GFX_FlyCam flyCam;
#endif 
