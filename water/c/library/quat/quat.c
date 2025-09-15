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

/*
 * includes
 */

#include <type/datatype.h>
#include <triangle/triangle.h>
#include <matrix/matrix.h>
#include "quat.h"

#ifndef QUAT_FIXEDPOINT_API
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#endif

/*
 * macros
 */

#define PI			3.1415926535f
#define RAD2DEG(r)	((r/PI)*360.0f)
#define DEG2RAD(d)	((float)((float)(((float)d)/360.0f))*((float)PI))
#define fmax(a,b)	((a>b)?a:b)

/*
 * typedefs
 */

/*
 * prototypes
 */

/*
 * variables
 */

/*
 * functions
 */

/*
VECTOR is a standard {x,y,z} struct
MATRIX is a 4x4 matrix
  [11 12 13 14]
  [21 22 23 24]
  [31 32 33 34]
  [41 42 43 44]
fmax(a,b) returns the floating point maximum of two values
PI is 3.1415926535
*/

#ifdef QUAT_FIXEDPOINT_API 

/**********************************************************
 * Function:	qatRotToQuat
 **********************************************************
 * Description: Create a quaternion from a rotation
 * Inputs:		q - quaternion to create
 *				v - rotation vector
 *				angle - rotation angle (degrees)
 * Notes:	
 * Returns:		updated q
 **********************************************************/

void qatRotToQuat (
	Quatern		*q,
	fixed		 x,
	fixed		 y,
	fixed		 z,
	fixed		 angle
	)
{
	int		angle2;

	/* init - half angle */
	angle2 = HALF(HALF(angle));
	angle2 = FIXED_TO_INT(angle2);

    q->x = FIXED_MULTIPLY( triSin(angle2), x );
    q->y = FIXED_MULTIPLY( triSin(angle2), y );
    q->z = FIXED_MULTIPLY( triSin(angle2), z );
    q->w = triCos(angle2);
}
 

/**********************************************************
 * Function:	qatQuatToRot
 **********************************************************
 * Description: Create a rotation from a quaternion
 * Inputs:		q - quaternion
 *				v - rotation vector to create
 *				angle - rotation angle (degrees) to create
 * Notes:		
 * Returns:		updated v and angle
 **********************************************************/

void qatQuatToRot (
	Quatern		*q,
	fixed		*x,
	fixed		*y,
	fixed		*z,
	fixed		*angle
	)
{
	fixed	angle2;
	fixed	acos;
	fixed	sin;

	acos = triACos(q->w);
	if ( acos == 0 ) {
		*x = 0;
		*y = 0;
		*z = FIXED_ONE;
		*angle = 0;
		return;
	}

    *angle = DOUBLE(acos);
	angle2 = FIXED_TO_INT( HALF(HALF( *angle )) );
	sin = triSin( angle2 );
    *x = FIXED_NORMALISE( q->x, sin );
    *y = FIXED_NORMALISE( q->y, sin );
    *z = FIXED_NORMALISE( q->z, sin );
}

 
/**********************************************************
 * Function:	qatAnglesToQuat
 **********************************************************
 * Description: Create a quaternion from rotation angles
 * Inputs:		q - quaternion
 *				v - rotation vector to create
 *				angle - rotation angle (degrees) to create
 * Notes:		
 * Returns:		updated v and angle
 **********************************************************/

void qatAnglesToQuat (
	Quatern		*q,
	int			 yaw,
	int			 pitch,
	int			 roll
	)
{
	fixed		sinyaw;
	fixed		sinpitch;
	fixed		sinroll;
	fixed		cosyaw;
	fixed		cospitch;
	fixed		cosroll;

	yaw   = HALF(yaw);
	pitch = HALF(pitch);
	roll  = HALF(roll);

	sinyaw   = triSin(yaw);
	sinpitch = triSin(pitch);
	sinroll  = triSin(roll);
	cosyaw   = triCos(yaw);
	cospitch = triCos(pitch);
	cosroll  = triCos(roll);

	q->x = sinroll * cospitch * cosyaw - cosroll * sinpitch * sinyaw;
    q->y = cosroll * sinpitch * cosyaw + sinroll * cospitch * sinyaw;
    q->z = cosroll * cospitch * sinyaw - sinroll * sinpitch * cosyaw;
    q->w = cosroll * cospitch * cosyaw + sinroll * sinpitch * sinyaw;
}
 
/*
void MatrixFromQuaternion(MATRIX& m, FLOAT x, FLOAT y, FLOAT z, FLOAT w)
{
    FLOAT xx = x * x,   xy = x * y,   xz = x * z,   xw = x * w;
    FLOAT yy = y * y,   yz = y * z,   yw = y * w;
    FLOAT zz = z * z,   zw = z * w;
    FLOAT ww = w * w;
 
    m.11 = xx - yy - zz + ww;
    m.12 = 2.0f * (xy - zw);
    m.13 = 2.0f * (xz + yw);
 
    m.21 = 2.0f * (xy + zw);
    m.22 = -xx + yy - zz + ww;
    m.23 = 2.0f * (yz - xw);
 
    m.31 = 2.0f * (xz - yw);
    m.32 = 2.0f * (yz + xw);
    m.33 = -xx - yy + zz + ww;
 
    m.14 = mat._41 = 0.0f;
    m.24 = mat._42 = 0.0f;
    m.34 = mat._43 = 0.0f;
    m.44 = xx + yy + zz + ww;
}
 
void QuaternionFromMatrix(FLOAT& x, FLOAT& y, FLOAT& z, FLOAT& w, MATRIX& m)
{
    FLOAT diag1 =  m.11 - m.22 - m.33 + m.44;
    FLOAT diag2 = -m.11 + m.22 - m.33 + m.44;
    FLOAT diag3 = -m.11 - m.22 + m.33 + m.44;
    FLOAT diag4 =  m.11 + m.22 + m.33 + m.44;
    
    FLOAT max = fmax(diag1, fmax(diag2, fmax(diag3, diag4)));
    FLOAT d = 2.0f * (FLOAT)sqrt(max);
 
    if(diag1 == max)
    {
        x = max / d;
        y = (m.21 + m.12) / d;
        z = (m.13 + m.31) / d;
        w = (m.32 - m.23) / d;
    }
    else if(diag2 == max)
    {
        x = (m.21 + m.12) / d;
        y = max / d;
        z = (m.32 + m.23) / d;
        w = (m.13 - m.31) / d;
    }
    else if(diag3 == max)
    {
        x = (m.13 + m.31) / d;
        y = (m.32 + m.23) / d;
        z = max / d;
        w = (m.21 - m.12) / d;
    }
    else if(diag4 == max)
    {
        x = (m.32 - m.23) / d;
        y = (m.13 - m.31) / d;
        z = (m.21 - m.12) / d;
        w = max / d;
    }
 
    if(w < 0.0f)
    {
        x = -x;   y = -y;   z = -z;   w = -w;
    }
}
 
void QuaternionMultiply(FLOAT& Qx, FLOAT& Qy, FLOAT& Qz, FLOAT& Qw,
                        FLOAT  Ax, FLOAT  Ay, FLOAT  Az, FLOAT  Aw,
                        FLOAT  Bx, FLOAT  By, FLOAT  Bz, FLOAT  Bw)
{
    FLOAT Dx =  Ax * Bw + Ay * Bz - Az * By + Aw * Bx;
    FLOAT Dy = -Ax * Bz + Ay * Bw + Az * Bx + Aw * By;
    FLOAT Dz =  Ax * By - Ay * Bx + Az * Bw + Aw * Bz;
    FLOAT Dw = -Ax * Bx - Ay * By - Az * Bz + Aw * Bw;
 
    Qx = Dx; Qy = Dy; Qz = Dz; Qw = Dw;
}
*/

 
/**********************************************************
 * Function:	qatInterpolate
 **********************************************************
 * Description: Create a quaternion from rotation angles
 * Inputs:		qa - quaternion a
 *				qb - quaternion b
 *				q  - quaternion to create
 *				ratio - interpolation ratio (0->4096) (0.0f->1.0f)
 * Notes:		0=qa, 1=qb
 * Returns:		updated q
 **********************************************************/

void qatInterpolate (
	Quatern		*qa,
	Quatern		*qb,
	Quatern		*q,
	fixed		 ratio
	)
{
	fixed	scale1;
	fixed	scale2;
	fixed	costheta;
	fixed	sintheta;
	fixed	theta;
	fixed	tmp;

	// if both quat's are the same copy into result
	if ( qa->x == qb->x && qa->y == qb->y && qa->z == qb->z && qa->w == qb->w ) {
		*q = *qa;
		return;
	}

    // Compute dot product, aka cos(theta):
    costheta = FIXED_MULTIPLY( qa->x, qb->x ) + FIXED_MULTIPLY( qa->y, qb->y ) + 
			   FIXED_MULTIPLY( qa->z, qb->z ) + FIXED_MULTIPLY( qa->w, qb->w );

    if ( costheta < 0 ) {
		// Flip the Start Quaternion
        qa->x = -(qa->x);
        qa->y = -(qa->y);
        qa->z = -(qa->z);
        qa->w = -(qa->w);
        costheta = -(costheta);
    }
 
    if ( (costheta + FIXED_ONE) > 205 ) { // (205 = 0.05f)
        //if( (FIXED_ONE - costheta) < 205 ) {
	        // If the quaternions are close, use linear interploation
            scale1 = FIXED_ONE - ratio;
            scale2 = ratio;
        //} else { 
#if 0
			// Otherwise, do spherical interpolation
            theta = triACos( costheta );
            theta = HALF(theta);
            sintheta = triSin( FIXED_TO_INT(theta) );
			//printf( "costheta=%.4f,theta=%.4f,sintheta=%.4f\n",
			//	FIXED_TO_FLOAT(costheta),FIXED_TO_FLOAT(theta),
			//	FIXED_TO_FLOAT(sintheta));
			tmp = FIXED_TO_INT( FIXED_MULTIPLY( theta, (FIXED_ONE - ratio) ) );
			tmp = triSin(tmp);
            scale1 = FIXED_NORMALISE( tmp, sintheta );
			tmp = FIXED_TO_INT( FIXED_MULTIPLY( theta, ratio ) );
			tmp = triSin(tmp);
            scale2 = FIXED_NORMALISE( tmp, sintheta );
#endif
        //}
    } else {
        qb->x = -(qa->y);
        qb->y = qa->x;
        qb->z = -(qa->w);
        qb->w = qa->z;
        scale1 = triSin( FIXED_TO_INT((360 * (2048 - ratio))) );
        scale2 = triSin( FIXED_TO_INT((360 * ratio)) );
    }
    
	// Set the resultant quaternion
    q->x = FIXED_MULTIPLY( scale1, qa->x ) + FIXED_MULTIPLY( scale2, qb->x );
    q->y = FIXED_MULTIPLY( scale1, qa->y ) + FIXED_MULTIPLY( scale2, qb->y );
    q->z = FIXED_MULTIPLY( scale1, qa->z ) + FIXED_MULTIPLY( scale2, qb->z );
    q->w = FIXED_MULTIPLY( scale1, qa->w ) + FIXED_MULTIPLY( scale2, qb->w );
}


/**********************************************************
 * Function:	qatXyzToXyza
 **********************************************************
 * Description: Modify an xyz to an xyza
 * Inputs:		x - x angle
 *				y - y angle
 *				z - z angle
 *				a - angle value to return
 * Notes:		
 * Returns:		updated x,y,z,a
 **********************************************************/

void qatXyzToXyza (
	int		*x,
	int		*y,
	int		*z,
	int		*a
	)
{
	int		 max;

	/* init */
	*x = INT_TO_FIXED(*x);
	*y = INT_TO_FIXED(*y);
	*z = INT_TO_FIXED(*z);
	*a = 0;

	/* get maximum */
	max = 0;
	if ( *x > max ) max = *x;
	if ( *y > max ) max = *y;
	if ( *z > max ) max = *z;

	/* normalise */
	*x = FIXED_NORMALISE( *x, max );
	*y = FIXED_NORMALISE( *y, max );
	*z = FIXED_NORMALISE( *z, max );
	*a = max;
}

#else

/**********************************************************
 * Function:	qatRotToQuat
 **********************************************************
 * Description: Create a quaternion from a rotation
 * Inputs:		q - quaternion to create
 *				v - rotation vector
 *				angle - rotation angle (degrees)
 * Notes:	
 * Returns:		updated q
 **********************************************************/

void qatRotToQuat (
	Quatern		*q,
	float		 x,
	float		 y,
	float		 z,
	float		 angle
	)
{
	float		 theta2;

	theta2 = DEG2RAD(angle)/2;
    q->x = sinf(theta2) * x;
    q->y = sinf(theta2) * y;
    q->z = sinf(theta2) * z;
    q->w = cosf(theta2);
}
 

/**********************************************************
 * Function:	qatQuatToRot
 **********************************************************
 * Description: Create a rotation from a quaternion
 * Inputs:		q - quaternion
 *				v - rotation vector to create
 *				angle - rotation angle (degrees) to create
 * Notes:		
 * Returns:		updated v and angle
 **********************************************************/

void qatQuatToRot (
	Quatern		*q,
	float		*x,
	float		*y,
	float		*z,
	float		*angle
	)
{
	float		 theta;
	float		 theta2;

    theta2 = acosf(q->w);
	theta  = theta2 * 2.0f;
    *x     = (q->x / sinf(theta2));
    *y     = (q->y / sinf(theta2));
    *z     = (q->z / sinf(theta2));
	*angle = RAD2DEG(theta);
}

 
/**********************************************************
 * Function:	qatAnglesToQuat
 **********************************************************
 * Description: Create a quaternion from rotation angles
 * Inputs:		q - quaternion
 *				v - rotation vector to create
 *				angle - rotation angle (degrees) to create
 * Notes:		
 * Returns:		updated v and angle
 **********************************************************/

void qatAnglesToQuat (
	Quatern		*q,
	float		 yaw,
	float		 pitch,
	float		 roll
	)
{
    float fSinYaw;
    float fSinPitch;
    float fSinRoll;
    float fCosYaw;
    float fCosPitch;
    float fCosRoll;
 
	/* init */
	fSinYaw   = sinf(yaw   * 2.0f);
    fSinPitch = sinf(pitch * 2.0f);
    fSinRoll  = sinf(roll  * 2.0f);
    fCosYaw   = cosf(yaw   * 0.5f);
    fCosPitch = cosf(pitch * 0.5f);
    fCosRoll  = cosf(roll  * 0.5f);

    q->x = fSinRoll * fCosPitch * fCosYaw - fCosRoll * fSinPitch * fSinYaw;
    q->y = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fCosPitch * fSinYaw;
    q->z = fCosRoll * fCosPitch * fSinYaw - fSinRoll * fSinPitch * fCosYaw;
    q->w = fCosRoll * fCosPitch * fCosYaw + fSinRoll * fSinPitch * fSinYaw;
}
 
/*
void qatQuatToMatrix (
	Matrix	MATRIX& m, float x, float y, float z, float w)
{
    float xx = x * x,   xy = x * y,   xz = x * z,   xw = x * w;
    float yy = y * y,   yz = y * z,   yw = y * w;
    float zz = z * z,   zw = z * w;
    float ww = w * w;
 
    m.11 = xx - yy - zz + ww;
    m.12 = 2.0f * (xy - zw);
    m.13 = 2.0f * (xz + yw);
 
    m.21 = 2.0f * (xy + zw);
    m.22 = -xx + yy - zz + ww;
    m.23 = 2.0f * (yz - xw);
 
    m.31 = 2.0f * (xz - yw);
    m.32 = 2.0f * (yz + xw);
    m.33 = -xx - yy + zz + ww;
 
    m.14 = mat._41 = 0.0f;
    m.24 = mat._42 = 0.0f;
    m.34 = mat._43 = 0.0f;
    m.44 = xx + yy + zz + ww;
}
 
void QuaternionFromMatrix(float& x, float& y, float& z, float& w, MATRIX& m)
{
    float diag1 =  m.11 - m.22 - m.33 + m.44;
    float diag2 = -m.11 + m.22 - m.33 + m.44;
    float diag3 = -m.11 - m.22 + m.33 + m.44;
    float diag4 =  m.11 + m.22 + m.33 + m.44;
    
    float max = fmax(diag1, fmax(diag2, fmax(diag3, diag4)));
    float d = 2.0f * (float)sqrt(max);
 
    if(diag1 == max)
    {
        x = max / d;
        y = (m.21 + m.12) / d;
        z = (m.13 + m.31) / d;
        w = (m.32 - m.23) / d;
    }
    else if(diag2 == max)
    {
        x = (m.21 + m.12) / d;
        y = max / d;
        z = (m.32 + m.23) / d;
        w = (m.13 - m.31) / d;
    }
    else if(diag3 == max)
    {
        x = (m.13 + m.31) / d;
        y = (m.32 + m.23) / d;
        z = max / d;
        w = (m.21 - m.12) / d;
    }
    else if(diag4 == max)
    {
        x = (m.32 - m.23) / d;
        y = (m.13 - m.31) / d;
        z = (m.21 - m.12) / d;
        w = max / d;
    }
 
    if(w < 0.0f)
    {
        x = -x;   y = -y;   z = -z;   w = -w;
    }
}
 
void QuaternionMultiply(float& Qx, float& Qy, float& Qz, float& Qw,
                        float  Ax, float  Ay, float  Az, float  Aw,
                        float  Bx, float  By, float  Bz, float  Bw)
{
    float Dx =  Ax * Bw + Ay * Bz - Az * By + Aw * Bx;
    float Dy = -Ax * Bz + Ay * Bw + Az * Bx + Aw * By;
    float Dz =  Ax * By - Ay * Bx + Az * Bw + Aw * Bz;
    float Dw = -Ax * Bx - Ay * By - Az * Bz + Aw * Bw;
 
    Qx = Dx; Qy = Dy; Qz = Dz; Qw = Dw;
}
*/

void qatQuatToMat( Matrix *m, float x, float y, float z, float w )
{
    float xx = x * x,   xy = x * y,   xz = x * z,   xw = x * w;
    float yy = y * y,   yz = y * z,   yw = y * w;
    float zz = z * z,   zw = z * w;
    float ww = w * w;
 
    m->x[0] = xx - yy - zz + ww;
    m->x[1] = 2.0f * (xy - zw);
    m->x[2] = 2.0f * (xz + yw);
	m->x[3] = 0.0f;

    m->y[0] = 2.0f * (xy + zw);
    m->y[1] = -xx + yy - zz + ww;
    m->y[2] = 2.0f * (yz - xw);
	m->y[3] = 0.0f;
 
    m->z[0] = 2.0f * (xz - yw);
    m->z[1] = 2.0f * (yz + xw);
    m->z[2] = -xx - yy + zz + ww;
	m->z[3] = 0.0f;
 
    m->w[0] = 0.0f;
    m->w[1] = 0.0f;
    m->w[2] = 0.0f;
    m->w[3] = xx + yy + zz + ww;
}
 
void qatMatToQuat( Matrix *m, float *x, float *y, float *z, float *w )
{
    float diag1 =  m->x[0] - m->y[1] - m->z[2] + m->w[3];
    float diag2 = -m->x[0] + m->y[1] - m->z[2] + m->w[3];
    float diag3 = -m->x[0] - m->y[1] + m->z[2] + m->w[3];
    float diag4 =  m->x[0] + m->y[1] + m->z[2] + m->w[3];
    
    float max = fmax(diag1, fmax(diag2, fmax(diag3, diag4)));
    float d = 2.0f * (float)sqrtf(max);
 
    if ( diag1 == max )
    {
        *x = max / d;
        *y = (m->y[0] + m->x[1]) / d;
        *z = (m->x[2] + m->z[0]) / d;
        *w = (m->z[1] - m->y[2]) / d;
    }
    else if ( diag2 == max )
    {
        *x = (m->y[0] + m->x[1]) / d;
        *y = max / d;
        *z = (m->z[1] + m->y[2]) / d;
        *w = (m->x[2] - m->z[0]) / d;
    }
    else if ( diag3 == max )
    {
        *x = (m->x[2] + m->z[0]) / d;
        *y = (m->z[1] + m->y[2]) / d;
        *z = max / d;
        *w = (m->y[0] - m->x[1]) / d;
    } 
	else if ( diag4 == max ) 
	{
        *x = (m->z[1] - m->y[2]) / d;
        *y = (m->x[2] - m->z[0]) / d;
        *z = (m->y[0] - m->x[1]) / d;
        *w = max / d;
    }
 
    if ( *w < 0.0f ) {
        *x = -(*x);   
		*y = -(*y);   
		*z = -(*z);   
		*w = -(*w);
    }
}
 
 
/**********************************************************
 * Function:	qatInterpolate
 **********************************************************
 * Description: Create a quaternion from rotation angles
 * Inputs:		qa - quaternion a
 *				qb - quaternion b
 *				q  - quaternion to create
 *				ratio - interpolation ratio (0->4096) (0.0f->1.0f)
 * Notes:		0=qa, 1=qb
 * Returns:		updated q
 **********************************************************/

void qatInterpolate (
	Quatern		*qa,
	Quatern		*qb,
	Quatern		*q,
	float		 ratio
	)
{
// The function below calculates the spherical linear interpolation between
// two quaternions (the interpolation amount is set by fAlpha and is a value
// between 0 and 1 - 0 being fully 'quaternion A' and 1 being fully 'quaternion B'
// Q is the resultant quaternion... that's about it really ;)
    float	 fScale1;
    float	 fScale2;
	float	 fCosTheta;
    float	 fTheta;
    float	 fSinTheta;
    
    // Compute dot product, aka cos(theta):
    fCosTheta = (qa->x * qb->x) + (qa->y * qb->y) + (qa->z * qb->z) + (qa->w * qb->w);
 
    if ( fCosTheta < 0.0f ) {
        // Flip the Start Quaternion
        qa->x = -(qa->x);
        qa->y = -(qa->y);
        qa->z = -(qa->z);
        qa->w = -(qa->w);
        fCosTheta = -fCosTheta;
    }
 
    if ( (fCosTheta + 1.0f) > 0.05f ) {
        if ( (1.0f - fCosTheta) < 0.05f ) {
	        // If the quaternions are close, use linear interploation
            fScale1 = 1.0f - ratio;
            fScale2 = ratio;
        } else {
			// Otherwise, do spherical interpolation
            fTheta    = acosf(fCosTheta);
            fSinTheta = sinf(fTheta);
            fScale1 = sinf(fTheta * (1.0f - ratio)) / fSinTheta;
            fScale2 = sinf(fTheta * ratio) / fSinTheta;
        }
    } else {
        qb->x = -(qa->y);
        qb->y = qa->x;
        qb->z = -(qa->w);
        qb->w = qa->z;
        fScale1 = sinf(PI * (0.5f - ratio));
        fScale2 = sinf(PI * ratio);
    }
    
	// Set the resultant quaternion
    q->x = fScale1 * qa->x + fScale2 * qb->x;
    q->y = fScale1 * qa->y + fScale2 * qb->y;
    q->z = fScale1 * qa->z + fScale2 * qb->z;
    q->w = fScale1 * qa->w + fScale2 * qb->w;
}


/**********************************************************
 * Function:	qatXyzToXyza
 **********************************************************
 * Description: Modify an xyz to an xyza
 * Inputs:		x - x angle
 *				y - y angle
 *				z - z angle
 *				a - angle value to return
 * Notes:		
 * Returns:		updated x,y,z,a
 **********************************************************/

void qatXyzToXyza (
	float	*x,
	float	*y,
	float	*z,
	float	*a
	)
{
	float	 max;

	/* get maximum */
	max = 0;
	if ( *x > max ) max = *x;
	if ( *y > max ) max = *y;
	if ( *z > max ) max = *z;

	/* normalise */
	*x = *x/max;
	*y = *y/max;
	*z = *z/max;
	*a = max;
}

#endif	// QUAT_FIXEDPOINT_API


