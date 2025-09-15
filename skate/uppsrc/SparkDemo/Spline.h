
#define MAX_CONTROL_PNT 100
#define MAX_CURVE_PNT 1000

#define B_SPLINE(u, u_2, u_3, cntrl0, cntrl1, cntrl2, cntrl3) \
	( \
		( \
			(-1*u_3 + 3*u_2 - 3*u + 1) * (cntrl0) + \
			( 3*u_3 - 6*u_2 + 0*u + 4) * (cntrl1) + \
			(-3*u_3 + 3*u_2 + 3*u + 1) * (cntrl2) + \
			( 1*u_3 + 0*u_2 + 0*u + 0) * (cntrl3)   \
		) / 6 \
	) 

#define B_TANGENT(u, u_2, cntrl0, cntrl1, cntrl2, cntrl3) \
	( \
		( \
			(-1*u_2 + 2*u - 1) * (cntrl0) + \
			( 3*u_2 - 4*u + 0) * (cntrl1) + \
			(-3*u_2 + 2*u + 1) * (cntrl2) + \
			( 1*u_2 + 0*u + 0) * (cntrl3)   \
		) / 2 \
	) 

#define CATMULL_ROM_SPLINE(u, u_2, u_3, cntrl0, cntrl1, cntrl2, cntrl3) \
	( \
		( \
			(-1*u_3 + 2*u_2 - 1*u + 0) * (cntrl0) + \
			( 3*u_3 - 5*u_2 + 0*u + 2) * (cntrl1) + \
			(-3*u_3 + 4*u_2 + 1*u + 0) * (cntrl2) + \
			( 1*u_3 - 1*u_2 + 0*u + 0) * (cntrl3)   \
		) / 2 \
	) 

#define CATMULL_ROM_TANGENT(u, u_2, cntrl0, cntrl1, cntrl2, cntrl3) \
	( \
		( \
			(-3*u_2 +  4*u - 1) * (cntrl0) + \
			( 9*u_2 - 10*u + 0) * (cntrl1) + \
			(-9*u_2 +  8*u + 1) * (cntrl2) + \
			( 3*u_2 -  2*u + 0) * (cntrl3)   \
		) / 2 \
	) 


#define CONTROL_NORMAL_ROT 1
#define CONTROL_NORMAL_GIVEN 2
#define CONTROL_PHYSICS_NO_GRAVITY 1
#define CONTROL_PHYSICS_BREAK 2
#define DISPLAY_MODE_OFF 0
#define DISPLAY_MODE_POINTS 1
#define DISPLAY_MODE_LINES 2

typedef struct SplineControlPnt_Str {
	Vec pos;
} SplineControlPnt;

typedef struct SplineCurvePnt_Str {
	Vec pos;
	Vec tan;
	float distance;
} SplineCurvePnt;

typedef struct Spline_Str {

	// The control points
	SplineControlPnt * controlPnt;

	// The number of control points
	int controlCnt;

	// The Current Control Pnt
	int controlIndx;

	// The Curve
	SplineCurvePnt * curvePnt;

	// Subdivision between each control point 
	int curveSubD;

	// The number of Points on the final curve
	int curveCnt;

	// The Current Control Pnt
	int curveIndx;

	// Type of curve
	int curveType;

	// Distance
	float shortestDistance;
} Spline;

Spline * SplineConstructor(int controlCnt, float curveSubD);
void SplineBuild(Spline * sP);
int SplineDistanceIndex(Spline * sP, float distance);
