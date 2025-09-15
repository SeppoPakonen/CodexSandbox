//===========================================================================
// Flight Module
// Dante Treglia
// NOA Developer Support
// support@noa.com
//===========================================================================

#include <string.h>
#include <math.h>
#include "fly.h"

// Global Flight Object
GFX_FlyCam flyCam;

#define PI 3.14159265359F
#define PI_2 6.28318530718F
#define PI_OVER_2 1.57079632679F
#define RAD_TO_DEG(rad) (57.2957795131F * rad)

//-------------------------------------------------------------------------
// An asymptotic effect returns a float that approaches targetX from currX 
//-------------------------------------------------------------------------
float dampType1(float currX, float targetX) {
	return currX + ((targetX - currX) / 16.0F);
}

//-------------------------------------------------------------------------
// Initialize camera
//-------------------------------------------------------------------------
#ifdef OPEN_GL
void FLYInit(Vec * eye, Vec * up, Vec * at) {
#else
void FLYInit(Mtx viewMtx, Vec * eye, Vec * up, Vec * at) {
#endif
	Vec temp;
	Vec temp2;
	Vec orig;
	float cosA;
	float cosE;

	// Flying Cam
	flyCam.eye.x = eye->x;
	flyCam.eye.y = eye->y;
	flyCam.eye.z = eye->z;
	flyCam.at.x = at->x;
	flyCam.at.y = at->y;
	flyCam.at.z = at->z;
	flyCam.up.x = up->x;
	flyCam.up.y = up->y;
	flyCam.up.z = up->z;
	flyCam.unitsPerFrame = 10;

	// Create and Normalize the FWD Vector
	VECSubtract(&flyCam.at, &flyCam.eye, &flyCam.fwd);
	VECNormalize(&flyCam.fwd, &flyCam.fwd);

	// Original Viewing Direction
	orig.x = 0;
	orig.y = 0;
	orig.z = -1;

	// Drop Fwd Vector unto (X, Z) 
	temp.x = flyCam.fwd.x;
	temp.y = 0;
	temp.z = flyCam.fwd.z;
	VECNormalize(&temp, &temp);

	// Compute A
	cosA = VECDotProduct(&temp, &orig);
	flyCam.A = acos(cosA);
	if (temp.x < 0) flyCam.A *= -1.0;

	// Drop Fwd Vector unto (Y, Z) 
	temp2.x = 0;
	temp2.y = flyCam.fwd.y;
	temp2.z = flyCam.fwd.z;
	// If Z is zero then we must use (X, Z) plane
	// but moved to (Y,Z)
	if (temp2.z == 0) {
		temp2.x = 0;
		temp2.y = flyCam.fwd.y;
		temp2.z = flyCam.fwd.x;
	}
	VECNormalize(&temp2, &temp2);

	// Compute E
	// This will give us a value in [0, PI/2]
	if (temp2.z > 0) temp2.z *= -1.0;
	cosE = VECDotProduct(&temp2, &orig);
	flyCam.E = acos(cosE);
	// This will give us a value in [-PI/2, PI/2]
	if (temp2.y < 0) flyCam.E *= -1.0;

	// Calc first matrix
	FLYLookAt(viewMtx);
}

//-------------------------------------------------------------------------
// Parse Controller Data for flight info
//-------------------------------------------------------------------------
void FLYPadParseData(PADStatus * padP) {
	static float damp_x = 0;
	static float damp_y = 0;
	static float target_fwdS = 0;
	static float targetSideS = 0;
	static float target_upS = 0;

	// ------- Proccess Stick Data --------
	damp_x = dampType1(damp_x, flyCam.unitsPerFrame * padP->stickX);
	if (fabs(damp_x) > 2) 	
		flyCam.A += damp_x / 2560.0;

	damp_y = dampType1(damp_y, flyCam.unitsPerFrame * padP->stickY);
	if (fabs(damp_y) > 2) 	
		flyCam.E -= damp_y / 5120.0;

	// Check Bounds
	if (flyCam.A > PI_2 || flyCam.A < -PI_2) flyCam.A = 0;
	if (flyCam.E > PI_2 || flyCam.E < -PI_2) flyCam.E = 0;

	// ------ Forward/Backward Motion -------
//	if (padP->button & WINKEY_NUMPAD8) {
//		target_fwdS = flyCam.unitsPerFrame;
//	}
//	else if (padP->button & WINKEY_NUMPAD2) {
//		target_fwdS = -flyCam.unitsPerFrame;
//	}
//	else {
		target_fwdS = 0;
//	}

	// Apply Damping
	flyCam.fwdS = dampType1(flyCam.fwdS, target_fwdS);
	flyCam.eye.x += flyCam.fwd.x * flyCam.fwdS;
	flyCam.eye.y += flyCam.fwd.y * flyCam.fwdS;
	flyCam.eye.z += flyCam.fwd.z * flyCam.fwdS;

	// ------ Strafe Right/Left -------
//	if (padP->button & WINKEY_NUMPAD6) {
//		targetSideS = flyCam.unitsPerFrame;
//	}
//	else if (padP->button & WINKEY_NUMPAD4) {
//		targetSideS = -flyCam.unitsPerFrame;
//	}
//	else {
		targetSideS = 0;
//	}

	// Apply Damping
	flyCam.sideS = dampType1(flyCam.sideS, targetSideS);
	flyCam.eye.x += flyCam.side.x * flyCam.sideS;
	flyCam.eye.y += flyCam.side.y * flyCam.sideS;
	flyCam.eye.z += flyCam.side.z * flyCam.sideS;

	// ------ Strafe Up/Down ------
//	if (padP->button & WINKEY_NUMPAD9) {
//		target_upS = flyCam.unitsPerFrame;
//	}
//	else if (padP->button & WINKEY_NUMPAD3) {
//		target_upS = -flyCam.unitsPerFrame;
//	}
//	else {
		target_upS = 0;
//	}

	// Apply Damping
	flyCam.upS = dampType1(flyCam.upS, target_upS);
	flyCam.eye.x += flyCam.up.x * flyCam.upS;
	flyCam.eye.y += flyCam.up.y * flyCam.upS;
	flyCam.eye.z += flyCam.up.z * flyCam.upS;	

//	if (padP->button & WINKEY_NUMPAD7) {
//		FLYPrintData();
//	}
}

//-------------------------------------------------------------------------
// Compute viewing matrix according to flight
//-------------------------------------------------------------------------
#ifdef OPEN_GL
void FLYLookAt() {
#else
void FLYLookAt(Mtx viewMtx) {
#endif
	static Vec origin = {0.0, 0.0, 0.0};
	static Vec look;
	float cosA, cosE, cosR;
	float sinA, sinE, sinR;

	// Only Want to Calc these once
	cosA = cosf(flyCam.A);
	cosE = cosf(flyCam.E);
	cosR = cosf(flyCam.R);
	sinA = sinf(flyCam.A);
	sinE = sinf(flyCam.E);
	sinR = sinf(flyCam.R);

	// Fwd Vector
/*
	flyCam.fwd.x = -sinA * cosE;
	flyCam.fwd.y = sinE;
	flyCam.fwd.z = cosE * cosA;
*/
	flyCam.fwd.x = sinA * cosE;
	flyCam.fwd.y = sinE;
	flyCam.fwd.z = cosE * -cosA;

	// Look At Point
	flyCam.at.x = flyCam.fwd.x + flyCam.eye.x;
	flyCam.at.y = flyCam.fwd.y + flyCam.eye.y;
	flyCam.at.z = flyCam.fwd.z + flyCam.eye.z;

	// Up Vector
/*
	flyCam.up.x = cosA * sinR + sinA * sinE * cosR;
	flyCam.up.y = cosE * cosR;
	flyCam.up.z = sinA * sinR - sinE * cosR * cosA;
*/
	flyCam.up.x = -cosA * sinR - sinA * sinE * cosR;
	flyCam.up.y = cosE * cosR;
	flyCam.up.z = -sinA * sinR - sinE * cosR * -cosA;

	// Side Vector (right)
	VECCrossProduct(&flyCam.fwd, &flyCam.up, &flyCam.side);

	// View matrix
#ifdef OPEN_GL
	gluLookAt(
		flyCam.eye.x, flyCam.eye.y, flyCam.eye.z,
		flyCam.at.x, flyCam.at.y, flyCam.at.z,
		flyCam.up.x, flyCam.up.y, flyCam.up.z
	);
#else
	MTXLookAt(viewMtx, &flyCam.eye, &flyCam.up, &flyCam.at);
#endif
}

//-------------------------------------------------------------------------
// Print Data
//-------------------------------------------------------------------------
void FLYPrintData() {
	OSReport("-=- Fly Data -=-\n");
	OSReport("  A: %.2f E:%.2f R:%.2f\n", flyCam.A, flyCam.E, flyCam.R);
	OSReport("  Eye: <%.2f, %.2f, %.2f>\n", flyCam.eye.x, flyCam.eye.y, flyCam.eye.z);
	OSReport("  At : <%.2f, %.2f, %.2f>\n", flyCam.at.x, flyCam.at.y, flyCam.at.z);
	OSReport("  Up : <%.2f, %.2f, %.2f>\n", flyCam.up.x, flyCam.up.y, flyCam.up.z);
}

