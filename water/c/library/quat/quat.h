/**********************************************************
 * Copyright (C) 1999 TheyerGFX
 * All Rights Reserved
 **********************************************************
 * Project:	library
 **********************************************************
 * File:	quat.c
 * Author:	Mark Theyer
 * Created:	26 Nov 1999
 **********************************************************
 * Description:	Quaternion supporting functions
 **********************************************************
 * Notes:		
 **********************************************************
 * Functions:
 *
 **********************************************************
 * Revision History:
 * 26-Nov-99	Theyer	Initial coding.
 **********************************************************/

#ifndef THEYER_QUAT_H
#define THEYER_QUAT_H

/*
 * includes...
 */

#include <type/datatype.h>
#include <matrix/matrix.h>

#ifdef QUAT_FIXEDPOINT_API
typedef struct {
	fixed		x, y, z, w;
} Quatern;
#else
typedef struct {
	float		x, y, z, w;
} Quatern;
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef QUAT_FIXEDPOINT_API
extern void qatRotToQuat( Quatern *q, fixed  x, fixed  y, fixed  z, fixed  angle );
extern void qatQuatToRot( Quatern *q, fixed *x, fixed *y, fixed *z, fixed *angle );
extern void qatAnglesToQuat( Quatern *q, fixed yaw, fixed pitch, fixed roll );
extern void qatInterpolate( Quatern *qa, Quatern *qb, Quatern *q, fixed ratio );
extern void qatXyzToXyza( int *x, int *y, int *z, int *a );
#else
extern void qatRotToQuat( Quatern *q, float  x, float  y, float  z, float  angle );
extern void qatQuatToRot( Quatern *q, float *x, float *y, float *z, float *angle );
extern void qatAnglesToQuat( Quatern *q, float yaw, float pitch, float roll );
extern void qatInterpolate( Quatern *qa, Quatern *qb, Quatern *q, float ratio );
extern void qatXyzToXyza( float *x, float *y, float *z, float *a );
extern void qatQuatToMat( Matrix *m, float x, float y, float z, float w );
extern void qatMatToQuat( Matrix *m, float *x, float *y, float *z, float *w );
#endif

#ifdef __cplusplus
}
#endif

#endif

