#include "DolphinRE.h"



void GXProject(f32 mx, f32 my, f32 mz, const float mtx[3][4], const float *p, const float *vp, f32 *sx, f32 *sy, f32 *sz) {
	ASSERTMSG(p != 0 && vp != 0 && sx != 0 && sy != 0 && sz != 0, "GXGet*: invalid null pointer");

	float x   = mz * mtx[0][2] + my * mtx[0][1] + mx * mtx[0][0] + mtx[0][3];
	float y   = mz * mtx[1][2] + my * mtx[1][1] + mx * mtx[1][0] + mtx[1][3];
	float mul = mz * mtx[2][2] + my * mtx[2][1] + mx * mtx[2][0] + mtx[2][3];
	
	float z, proj, vport, step;
	if (*p == 0.0) {
		z		= mul * p[2] + x * p[1];
		proj	= mul * p[4] + y * p[3];
		vport	= mul * p[5] +     p[6];
		step	= -1.0 / mul;
	}
	else {
		z		=   x * p[1] + p[2];
		proj	=   y * p[3] + p[4];
		vport	= mul * p[5] + p[6];
		step	= 1.0;
	}
	
	*sx = vp[2] / 2.0 +    z * vp[2] / 2.0 * step + vp[0];
	*sy = vp[3] / 2.0 - proj * vp[3] / 2.0 * step + vp[1];
	*sz = (vp[5] - vp[4]) * vport * step + vp[4];
	
}




