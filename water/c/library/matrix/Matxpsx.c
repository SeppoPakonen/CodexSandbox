
#include <stdio.h>
#include <math/math.h>
#include <triangle/triangle.h>
#include <memory/memory.h>

#include "matrix.h"
#include "matpsx.h"

#define MAT2PSX(mm,pm) { \
	pm.m[0][0] = FLOAT_TO_FIXED(mm->x[0]); \
	pm.m[1][0] = FLOAT_TO_FIXED(mm->x[1]); \
	pm.m[2][0] = FLOAT_TO_FIXED(mm->x[2]); \
	pm.m[0][1] = FLOAT_TO_FIXED(mm->y[0]); \
	pm.m[1][1] = FLOAT_TO_FIXED(mm->y[1]); \
	pm.m[2][1] = FLOAT_TO_FIXED(mm->y[2]); \
	pm.m[0][2] = FLOAT_TO_FIXED(mm->z[0]); \
	pm.m[1][2] = FLOAT_TO_FIXED(mm->z[1]); \
	pm.m[2][2] = FLOAT_TO_FIXED(mm->z[2]); \
	pm.t[0] = FLOAT_TO_FIXED(mm->x[3]); \
	pm.t[1] = FLOAT_TO_FIXED(mm->y[3]); \
	pm.t[2] = FLOAT_TO_FIXED(mm->z[3]); \
	}
#define PSX2MAT(pm,mm) { \
	mm->x[0] = FIXED_TO_FLOAT(pm.m[0][0]); \
	mm->x[1] = FIXED_TO_FLOAT(pm.m[1][0]); \
	mm->x[2] = FIXED_TO_FLOAT(pm.m[2][0]); \
	mm->y[0] = FIXED_TO_FLOAT(pm.m[0][1]); \
	mm->y[1] = FIXED_TO_FLOAT(pm.m[1][1]); \
	mm->y[2] = FIXED_TO_FLOAT(pm.m[2][1]); \
	mm->z[0] = FIXED_TO_FLOAT(pm.m[0][2]); \
	mm->z[1] = FIXED_TO_FLOAT(pm.m[1][2]); \
	mm->z[2] = FIXED_TO_FLOAT(pm.m[2][2]); \
	mm->x[3] = FIXED_TO_FLOAT(pm.t[0]); \
	mm->y[3] = FIXED_TO_FLOAT(pm.t[1]); \
	mm->z[3] = FIXED_TO_FLOAT(pm.t[2]); \
	mm->w[0] = 0.0f; \
	mm->w[1] = 0.0f; \
	mm->w[2] = 0.0f; \
	mm->w[3] = 1.0f; \
	}


#ifdef OsTypeIsPsx
#define printf	printf2
#endif

MATRIX work;
#if 0
MATRIX identity = {
	{ 
		4096, 0, 0,
		0, 4096, 0,
		0, 0, 4096
	}
	{ 
		0, 0, 0 }
	};
#else
Matrix identity = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};
#endif

void matrixIdentity( Matrix *mat )
{
    matrixCopy( &identity, mat );
}

void matrixCopy( Matrix *src, Matrix *dest )
{
    memCopy( src, dest, sizeof(Matrix) );
}

void matrixTranslate( Matrix *mat, float x, float y, float z )
{
	mat->x[3] += x;
	mat->y[3] += y;
	mat->z[3] += z;
}

void matrixScale( Matrix *mat, float x, float y, float z )
{
	MATRIX	m;
	VECTOR	v;

	return;

	MAT2PSX(mat,m);

	v.vx = FLOAT_TO_FIXED(x);
	v.vy = FLOAT_TO_FIXED(y);
	v.vz = FLOAT_TO_FIXED(z);

	ScaleMatrix( &m, &v );

	PSX2MAT(m,mat);
}

void matrixRotate( Matrix *mat, float x, float y, float z )
{
	MATRIX	m;
	long	fx,fy,fz;

	return;

	MAT2PSX(mat,m);

	fx = FLOAT_TO_FIXED(x);
	if ( fx )
		RotMatrixX(fx,&m);

	fy = FLOAT_TO_FIXED(y);
	if ( fy )
		RotMatrixY(fy,&m);

	fz = FLOAT_TO_FIXED(z);
	if ( fz )
		RotMatrixZ(fz,&m);

	PSX2MAT(m,mat);
}

void matrixApply( Matrix *mat, Point_f3d *s, Point_f3d *d )
{
	;
}

void matrixMultiply( Matrix *first, Matrix *second, Matrix *result )
{
	MATRIX	m0,m1,m2;

	MAT2PSX(first,m0);
	MAT2PSX(second,m1);

	CompMatrix(&m0,&m1,&m2);

	PSX2MAT(m2,result);
}

void matrixPrint( Matrix *mat )
{
	;
}
