
#include <stdio.h>
#include <math/math.h>
#include <memory/memory.h>
#include "matrix.h"

/* private */
float matrixMultiplyRow( float row[], float col[] );

#define FABS(f)		((f>0)?(f):(-f))

Matrix work;
Matrix identity = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
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
#if 1
    matrixIdentity( &work );

    work.x[3] = x;
    work.y[3] = y;
    work.z[3] = z;

    matrixMultiply( &work, mat, mat );
#else
	mat->x[3] += x;
	mat->y[3] += y;
	mat->z[3] += z;
#endif
}

void matrixScale( Matrix *mat, float x, float y, float z )
{
    matrixIdentity( &work );

    work.x[0] = x;
    work.y[1] = y;
    work.z[2] = z;

    matrixMultiply( &work, mat, mat );
}

void matrixRotate( Matrix *mat, float x, float y, float z )
{
    double cosx, sinx, cosy, siny, cosz, sinz;
    double xradians, yradians, zradians;

    if ( x != 0.0 ) {
		xradians = DEGREES_TO_RADIANS(x);

		cosx = cos( xradians );
		sinx = sin( xradians );

		/* windows cos/sin DEG(-90) bug causes this fix... */
		if ( FABS(cosx) > 1.0 ) cosx = -(cos( -(xradians) ));
		if ( FABS(sinx) > 1.0 ) sinx = -(sin( -(xradians) ));

        matrixIdentity( &work );

		work.y[1] = (float)cosx;
		work.y[2] = (float)-sinx;
		work.z[1] = (float)sinx;
		work.z[2] = (float)cosx;

		matrixMultiply( &work, mat, mat );
    }

    if ( y != 0.0 ) {
        yradians = DEGREES_TO_RADIANS(y);
		cosy = cos( yradians );
		siny = sin( yradians );

		/* windows cos/sin DEG(-90) bug causes this fix... */
		if ( FABS(cosy) > 1.0 ) cosy = -(cos( -(yradians) ));
		if ( FABS(siny) > 1.0 ) siny = -(sin( -(yradians) ));

        matrixIdentity( &work );

		work.x[0] = (float)cosy;
		work.x[2] = (float)siny;
		work.z[0] = (float)-siny;
		work.z[2] = (float)cosy;

		matrixMultiply( &work, mat, mat );
    }

    if ( z != 0.0 ) {
		zradians = DEGREES_TO_RADIANS(z);
		cosz = cos( zradians );
		sinz = sin( zradians );

		/* windows cos/sin DEG(-90) bug causes this fix... */
		if ( FABS(cosz) > 1.0 ) cosz = -(cos( -(zradians) ));
		if ( FABS(sinz) > 1.0 ) sinz = -(sin( -(zradians) ));

        matrixIdentity( &work );

		work.x[0] = (float)cosz;
		work.x[1] = (float)-sinz;
		work.y[0] = (float)sinz;
		work.y[1] = (float)cosz;

		matrixMultiply( &work, mat, mat );
    }
}

void matrixApply( Matrix *mat, Point_f3d *s, Point_f3d *d )
{
    float w;

    w = (mat->w[0] + mat->w[1] + mat->w[2] + mat->w[3]);
    d->x = ((s->x * mat->x[0]) + (s->y * mat->x[1]) +
            (s->z * mat->x[2]) + mat->x[3]) / w;
    d->y = ((s->x * mat->y[0]) + (s->y * mat->y[1]) +
            (s->z * mat->y[2]) + mat->y[3]) / w;
    d->z = ((s->x * mat->z[0]) + (s->y * mat->z[1]) +
            (s->z * mat->z[2]) + mat->z[3]) / w;
}

float matrixMultiplyRow( float row[], float col[] )
{
    float result;

    result = (row[0] * col[0]) + ( row[4] * col[1]) +
	     (row[8] * col[2]) + (row[12] * col[3]);

    return( result );
}

void matrixMultiply( Matrix *first, Matrix *second, Matrix *result )
{
    Matrix  m;
    float   *row1 = &first->x[0];
    float   *row2 = &first->x[1];
    float   *row3 = &first->x[2];
    float   *row4 = &first->x[3];

    m.x[0] = matrixMultiplyRow( row1, second->x );
    m.x[1] = matrixMultiplyRow( row2, second->x );
    m.x[2] = matrixMultiplyRow( row3, second->x );
    m.x[3] = matrixMultiplyRow( row4, second->x );

    m.y[0] = matrixMultiplyRow( row1, second->y );
    m.y[1] = matrixMultiplyRow( row2, second->y );
    m.y[2] = matrixMultiplyRow( row3, second->y );
    m.y[3] = matrixMultiplyRow( row4, second->y );

    m.z[0] = matrixMultiplyRow( row1, second->z );
    m.z[1] = matrixMultiplyRow( row2, second->z );
    m.z[2] = matrixMultiplyRow( row3, second->z );
    m.z[3] = matrixMultiplyRow( row4, second->z );

    m.w[0] = matrixMultiplyRow( row1, second->w );
    m.w[1] = matrixMultiplyRow( row2, second->w );
    m.w[2] = matrixMultiplyRow( row3, second->w );
    m.w[3] = matrixMultiplyRow( row4, second->w );

    matrixCopy( &m, result );
}

void matrixPrint( Matrix *mat )
{
    printf("%5.3f, %5.3f, %5.3f, %5.3f,\n", mat->x[0], mat->y[0], mat->z[0], mat->w[0] );
    printf("%5.3f, %5.3f, %5.3f, %5.3f,\n", mat->x[1], mat->y[1], mat->z[1], mat->w[1] );
    printf("%5.3f, %5.3f, %5.3f, %5.3f,\n", mat->x[2], mat->y[2], mat->z[2], mat->w[2] );
    printf("%5.3f, %5.3f, %5.3f, %5.3f\n",  mat->x[3], mat->y[3], mat->z[3], mat->w[3] );
    getchar();
}

void matrixFromAngles( Matrix *mat, char *a1, char *a2, char *a3 )
{
	char	 axis;
	char	*str;
	float	 angle;
	int		 i;

    matrixIdentity( mat );

	for ( i=0; i<3; i++ ) {
		switch(i){
		case 0:
			str = a1;
			break;
		case 1:
			str = a2;
			break;
		case 2:
			str = a3;
			break;
		}
		axis = str[0];
		if ( axis == '0' ) continue;
		angle = (float)atof( &str[1] );
		switch( axis ) {
		case 'x':
			matrixRotate( mat, angle, 0, 0 );
			break;
		case 'y':
			matrixRotate( mat, 0, angle, 0 );
			break;
		case 'z':
			matrixRotate( mat, 0, 0, angle );
			break;
		}
	}
}

float	*matrixToGL ( Matrix *mat ) 
{
	static	float	gl[16];

	gl[0] = mat->x[0];
	gl[1] = mat->y[0];
	gl[2] = mat->z[0];
	gl[3] = mat->w[0];
	gl[4] = mat->x[1];
	gl[5] = mat->y[1];
	gl[6] = mat->z[1];
	gl[7] = mat->w[1];
	gl[8] = mat->x[2];
	gl[9] = mat->y[2];
	gl[10] = mat->z[2];
	gl[11] = mat->w[2];
	gl[12] = mat->x[3];
	gl[13] = mat->y[3];
	gl[14] = mat->z[3];
	gl[15] = mat->w[3];

	return( gl );
}


	
