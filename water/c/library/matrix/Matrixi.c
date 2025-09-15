
#include "compile.h"

#include <stdio.h>
#include <math/math.h>
#include <triangle/triangle.h>
#include <memory/memory.h>
#include "matrix.h"

#ifdef OsTypeIsPsx
#define printf	printf2
#endif

#define F2I(val)	((float)(int)(val*4096))

/* private */
float matrixMultiplyRow( float row[], float col[] );
void matrixMul( Matrix *first, Matrix *second, Matrix *result );

Matrix work;
Matrix identity = {
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	0.0, 0.0, 0.0,
	};

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
#if 0
    matrixIdentity( &work );

    work.t[0] = (float)(int)x;
    work.t[1] = (float)(int)y;
    work.t[2] = (float)(int)z;

    matrixMultiply( &work, mat, mat );
#else
	mat->t[0] += (int)x;
	mat->t[1] += (int)y;
	mat->t[2] += (int)z;
#endif
}

void matrixScale( Matrix *mat, float x, float y, float z )
{
#if 1
    matrixIdentity( &work );

    work.x[0] = x;
    work.y[1] = y;
    work.z[2] = z;

    matrixMultiply( &work, mat, mat );
#else
	/* PSX definition */
	mat->x[0] *= x;
	mat->x[1] *= y;
	mat->x[2] *= z;
	mat->y[0] *= x;
	mat->y[1] *= y;
	mat->y[2] *= z;
	mat->z[0] *= x;
	mat->z[1] *= y;
	mat->z[2] *= z;
	//
	//mat->t[0] *= x;
	//mat->t[1] *= y;
	//mat->t[2] *= z;
#endif
}

void matrixRotate( Matrix *mat, float x, float y, float z )
{
#define USE_FAST_COSINE		0
#if USE_FAST_COSINE
    float   cosx, sinx, cosy, siny, cosz, sinz;
	int		ix, iy, iz;
#else
    double	cosx, sinx, cosy, siny, cosz, sinz;
    double	xradians, yradians, zradians;
#endif

    if ( x != 0.0 ) {

#if USE_FAST_COSINE
		/* init' and put into range */
		ix = (int)x;
		if ( ix < 0 ) ix += 360;
		/* fail if out of range */
		if ( ix < 0 || ix > 359 ) return;
		cosx = triCos( ix );
		sinx = triSin( ix );
#else
		xradians = DEGREES_TO_RADIANS(x);
		cosx = cos( xradians );
		sinx = sin( xradians );
#endif
        matrixIdentity( &work );

		work.y[1] = (float)cosx;
		work.y[2] = (float)-sinx;
		work.z[1] = (float)sinx;
		work.z[2] = (float)cosx;

		matrixMul( &work, mat, mat );
    }

    if ( y != 0.0 ) {

#if USE_FAST_COSINE
		/* init' and put into range */
		iy = (int)y;
		if ( iy < 0 ) iy += 360;
		/* fail if out of range */
		if ( iy < 0 || iy > 359 ) return;
		cosy = triCos( iy );
		siny = triSin( iy );
#else
		yradians = DEGREES_TO_RADIANS(y);
		cosy = cos( yradians );
		siny = sin( yradians );
#endif

        matrixIdentity( &work );

		work.x[0] = (float)cosy;
		work.x[2] = (float)-siny;
		work.z[0] = (float)siny;
		work.z[2] = (float)cosy;

		matrixMul( &work, mat, mat );
    }

    if ( z != 0.0 ) {
		
#if USE_FAST_COSINE
		/* init' and put into range */
		iz = (int)z;
		if ( iz < 0 ) iz += 360;
		/* fail if out of range */
		if ( iz < 0 || iz > 359 ) return;
		cosz = triCos( iz );
		sinz = triSin( iz );
#else
		zradians = DEGREES_TO_RADIANS(z);
		cosz = cos( zradians );
		sinz = sin( zradians );
#endif

        matrixIdentity( &work );

		work.x[0] = (float)cosz;
		work.x[1] = (float)-sinz;
		work.y[0] = (float)sinz;
		work.y[1] = (float)cosz;

		matrixMul( &work, mat, mat );
    }
}

void matrixApply( Matrix *mat, Point_f3d *s, Point_f3d *d )
{
#if 0
    float w;

    w = (mat->w[0] + mat->w[1] + mat->w[2] + mat->w[3]);
    d->x = ((s->x * mat->x[0]) + (s->y * mat->x[1]) +
            (s->z * mat->x[2]) + mat->x[3]) / w;
    d->y = ((s->x * mat->y[0]) + (s->y * mat->y[1]) +
            (s->z * mat->y[2]) + mat->y[3]) / w;
    d->z = ((s->x * mat->z[0]) + (s->y * mat->z[1]) +
            (s->z * mat->z[2]) + mat->z[3]) / w;
#endif
}

float matrixMultiplyRow( float row[], float col[] )
{
    float result;

#if 0
    result = (row[0] * col[0]) + ( row[4] * col[1]) +
	     (row[8] * col[2]) + (row[12] * col[3]);
#else
	/* ignore translation component */
    result = (row[0] * col[0]) + ( row[3] * col[1]) +
	     (row[6] * col[2]);
#endif

    return( result );
}

void matrixMul( Matrix *first, Matrix *second, Matrix *result )
{
    Matrix   m;
	Matrix	 tm;
	int		 i, x, y;

	/* multiply the 3x3 */
#if 1
	for ( i=0; i<3; i++ ) {
		m.x[i] = (first->x[0] * second->x[i]) + (first->x[1] * second->y[i]) + (first->x[2] * second->z[i]);
		m.y[i] = (first->y[0] * second->x[i]) + (first->y[1] * second->y[i]) + (first->y[2] * second->z[i]);
		m.z[i] = (first->z[0] * second->x[i]) + (first->z[1] * second->y[i]) + (first->z[2] * second->z[i]);
		//m.t[i] = (first->t[0] * second->x[i]) + (first->t[1] * second->y[i]) + (first->t[2] * second->z[i]);
	}
#else
	matrixIdentity(&m);
	for ( i=0; i<3; i++ ) {
		/* x */
		if ( first->x[i] == 0.0f )
			m.x[i] = second->x[i];
		else if ( second->x[i] == 0.0 )
			m.x[i] = 0.0f;
		else
			m.x[i] = first->x[i] * second->x[i];
		/* y */
		if ( first->y[i] == 0.0f )
			m.y[i] = second->y[i];
		else if ( second->y[i] == 0.0 )
			m.y[i] = 0.0f;
		else
			m.y[i] = first->y[i] * second->y[i];
		/* z */
		if ( first->z[i] == 0.0f )
			m.z[i] = second->z[i];
		else if ( second->z[i] == 0.0 )
			m.z[i] = 0.0f;
		else
			m.z[i] = first->z[i] * second->z[i];
	}
#endif

	m.t[0] = first->t[0] + second->t[0];
	m.t[1] = first->t[1] + second->t[1];
	m.t[2] = first->t[2] + second->t[2];

    matrixCopy( &m, result );
}

void matrixMultiply( Matrix *first, Matrix *second, Matrix *result )
{
    Matrix   m;
	Matrix	 tm;
	int		 i;

	/* multiply the 3x3 */
	for ( i=0; i<3; i++ ) {
		m.x[i] = (first->x[0] * second->x[i]) + (first->x[1] * second->y[i]) + (first->x[2] * second->z[i]);
		m.y[i] = (first->y[0] * second->x[i]) + (first->y[1] * second->y[i]) + (first->y[2] * second->z[i]);
		m.z[i] = (first->z[0] * second->x[i]) + (first->z[1] * second->y[i]) + (first->z[2] * second->z[i]);
		//m.t[i] = (first->t[0] * second->x[i]) + (first->t[1] * second->y[i]) + (first->t[2] * second->z[i]);
	}

#if 0
	m.t[0] = first->t[0] + second->t[0];
	m.t[1] = first->t[1] + second->t[1];
	m.t[2] = first->t[2] + second->t[2];
#else
	/* PSX definition */
	for ( i=0; i<3; i++ ) {
		//m.t[0] = (first->t[0] + second->t[0])
		//m.t[i] = (first->x[i] * (first->t[i] + second->t[i])) + 
		//		 (first->y[i] * (first->t[i] + second->t[i])) +
		//		 (first->z[i] * (first->t[i] + second->t[i]));
		//m.t[i] = (first->x[i] * (first->t[0] + second->t[0])) + 
		//		 (first->y[i] * (first->t[1] + second->t[1])) +
		//		 (first->z[i] * (first->t[2] + second->t[2]));
		m.t[i] = (first->x[i] * second->t[i]) + 
				 (first->y[i] * second->t[i]) +
				 (first->z[i] * second->t[i]) + 
				  first->t[i];	
	}
#endif

    matrixCopy( &m, result );
}

void matrixPrint( Matrix *mat )
{
#if 0
    printf("%5.3f, %5.3f, %5.3f, %5.3f,\n", mat->x[0], mat->y[0], mat->z[0], mat->w[0] );
    printf("%5.3f, %5.3f, %5.3f, %5.3f,\n", mat->x[1], mat->y[1], mat->z[1], mat->w[1] );
    printf("%5.3f, %5.3f, %5.3f, %5.3f,\n", mat->x[2], mat->y[2], mat->z[2], mat->w[2] );
    printf("%5.3f, %5.3f, %5.3f, %5.3f\n",  mat->x[3], mat->y[3], mat->z[3], mat->w[3] );
#else
    printf("%.2f %.2f %.2f, ", mat->x[0], mat->x[1], mat->x[2] );
    printf("%.2f %.2f %.2f, ", mat->y[0], mat->y[1], mat->y[2] );
    printf("%.2f %.2f %.2f, ", mat->z[0], mat->z[1], mat->z[2] );
    printf("%.2f %.2f %.2f\n", mat->t[0], mat->t[1], mat->t[2] );
#endif
    getchar();
}
