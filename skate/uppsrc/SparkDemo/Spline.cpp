#include <Dolphin/dolphin.h>
#include "spline.h"

#define STATE_BEGIN 0
#define STATE_SPLINE 1
#define STATE_POINTS 3
static int state = STATE_BEGIN;

#define COLOR_CONTROL 0xFF0000FF
#define COLOR_SPLINE_NO_GRAVITY 0xFFFFC000
#define COLOR_SPLINE_BREAK 0xFF0000E0
#define COLOR_SPLINE 0xFF00FF00
#define COLOR_TANGENT 0xFF0080FF
#define COLOR_NORMAL 0xFFFF00FF
#define COLOR_SIDE 0xFF00FF00
#define COLOR_ON 0xFFFFFFFF

static Vec defualtNrm = {0.0, 1.0, 0.0};

#define PI 3.14159265358979323846

//---------------------------------------------------------------------------
// Util 
//---------------------------------------------------------------------------
static void VECCopy(Vec * src, Vec * dst) {
	dst->x = src->x;
	dst->y = src->y;
	dst->z = src->z;
}
#define VECIsZero(src) ((src.x == 0 && src.y == 0 && src.z ==0)) 

//---------------------------------------------------------------------------
// Build a Spline
//---------------------------------------------------------------------------
void SplineBuild(Spline * sP) {
	float u, u_2, u_3;
	int i, j;
	int index;
	Vec distVec;
	Vec origin = {0.0, 0.0, 0.0};

	index = 0;
	// For each control Point (Minus the last three)
	for (i = 0; i < sP->controlCnt - 3; i++) {

		// For each subdivision
		for(j = 0; j < sP->curveSubD; j++) {

			u = (float)j / sP->curveSubD;
			u_2 = u * u;
			u_3 = u_2 * u;

			if (sP->curveType == 0) {
				// Position
				sP->curvePnt[index].pos.x = B_SPLINE(u, u_2, u_3, 
									sP->controlPnt[i  ].pos.x,
									sP->controlPnt[i+1].pos.x,
									sP->controlPnt[i+2].pos.x,
									sP->controlPnt[i+3].pos.x);

				sP->curvePnt[index].pos.y = B_SPLINE(u, u_2, u_3, 
									sP->controlPnt[i  ].pos.y,
									sP->controlPnt[i+1].pos.y,
									sP->controlPnt[i+2].pos.y,
									sP->controlPnt[i+3].pos.y);

				sP->curvePnt[index].pos.z = B_SPLINE(u, u_2, u_3, 
									sP->controlPnt[i  ].pos.z,
									sP->controlPnt[i+1].pos.z,
									sP->controlPnt[i+2].pos.z,
									sP->controlPnt[i+3].pos.z);

				// Tangent
				sP->curvePnt[index].tan.x = B_TANGENT(u, u_2,
									sP->controlPnt[i  ].pos.x,
									sP->controlPnt[i+1].pos.x,
									sP->controlPnt[i+2].pos.x,
									sP->controlPnt[i+3].pos.x);

				sP->curvePnt[index].tan.y = B_TANGENT(u, u_2,
									sP->controlPnt[i  ].pos.y,
									sP->controlPnt[i+1].pos.y,
									sP->controlPnt[i+2].pos.y,
									sP->controlPnt[i+3].pos.y);

				sP->curvePnt[index].tan.z = B_TANGENT(u, u_2,
									sP->controlPnt[i  ].pos.z,
									sP->controlPnt[i+1].pos.z,
									sP->controlPnt[i+2].pos.z,
									sP->controlPnt[i+3].pos.z);
			}
			else {
				sP->curvePnt[index].pos.x = CATMULL_ROM_SPLINE(u, u_2, u_3, 
									sP->controlPnt[i  ].pos.x,
									sP->controlPnt[i+1].pos.x,
									sP->controlPnt[i+2].pos.x,
									sP->controlPnt[i+3].pos.x);

				sP->curvePnt[index].pos.y = CATMULL_ROM_SPLINE(u, u_2, u_3, 
									sP->controlPnt[i  ].pos.y,
									sP->controlPnt[i+1].pos.y,
									sP->controlPnt[i+2].pos.y,
									sP->controlPnt[i+3].pos.y);

				sP->curvePnt[index].pos.z = CATMULL_ROM_SPLINE(u, u_2, u_3, 
									sP->controlPnt[i  ].pos.z,
									sP->controlPnt[i+1].pos.z,
									sP->controlPnt[i+2].pos.z,
									sP->controlPnt[i+3].pos.z);

				// Tangent
				sP->curvePnt[index].tan.x = CATMULL_ROM_TANGENT(u, u_2,
									sP->controlPnt[i  ].pos.x,
									sP->controlPnt[i+1].pos.x,
									sP->controlPnt[i+2].pos.x,
									sP->controlPnt[i+3].pos.x);

				sP->curvePnt[index].tan.y = CATMULL_ROM_TANGENT(u, u_2,
									sP->controlPnt[i  ].pos.y,
									sP->controlPnt[i+1].pos.y,
									sP->controlPnt[i+2].pos.y,
									sP->controlPnt[i+3].pos.y);

				sP->curvePnt[index].tan.z = CATMULL_ROM_TANGENT(u, u_2,
									sP->controlPnt[i  ].pos.z,
									sP->controlPnt[i+1].pos.z,
									sP->controlPnt[i+2].pos.z,
									sP->controlPnt[i+3].pos.z);
			}

			if (!(sP->curvePnt[index].tan.x == 0
				&& sP->curvePnt[index].tan.x == 0
				&& sP->curvePnt[index].tan.x == 0
				)) {
				VECNormalize(&sP->curvePnt[index].tan, &sP->curvePnt[index].tan);
			}

			// Distance
			if (j == 0 && i == 0) {
				sP->curvePnt[index].distance = 0;
				sP->shortestDistance = 65536.0;
			}
			else {
				VECSubtract(
					&sP->curvePnt[index].pos,
					&sP->curvePnt[index - 1].pos,
					&distVec);
				sP->curvePnt[index].distance = sP->curvePnt[index - 1].distance +
					VECMag(&distVec);
				if (sP->curvePnt[index].distance < sP->shortestDistance)
					sP->shortestDistance = sP->curvePnt[index].distance;
			}
								
			index++;
		}
	}
	
	sP->curveCnt = index;
}

//---------------------------------------------------------------------------
// Contstruct a Spline
//---------------------------------------------------------------------------
Spline * SplineConstructor(int controlCnt, float curveSubD) {
	Spline * sP;

	// Alloc Spline
	sP = (Spline *)OSAlloc(sizeof(Spline));
	memset(sP, 0, sizeof(Spline));

	sP->controlCnt = controlCnt;
	sP->curveSubD = curveSubD;

	// Alloc Control Points
	sP->controlPnt = OSAlloc(sP->controlCnt * sizeof(SplineControlPnt));
	memset(sP->controlPnt, 0, sP->controlCnt * sizeof(SplineControlPnt));

	// Alloc Curve Points
	sP->curveCnt = sP->controlCnt * sP->curveSubD;
	sP->curvePnt = OSAlloc(sP->curveCnt * sizeof(SplineCurvePnt));
	memset(sP->curvePnt, 0, sP->curveCnt * sizeof(SplineCurvePnt));

	// Build Spline
	SplineBuild(sP);

	return sP;
}

//---------------------------------------------------------------------------
// Compute a spline index at distance
//---------------------------------------------------------------------------
int SplineDistanceIndex(Spline * sP, float distance) {
	int index = 0;

	if (distance < 0.0) return -1;

	// Skip some distances
	//index = (int)(distance / shortestDistance);

	// Forward Push
	while (index < sP->curveCnt && distance > sP->curvePnt[index].distance) {
		index++;
	}

	if (index >= sP->curveCnt) return -1;

	return index;
}

