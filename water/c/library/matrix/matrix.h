
#ifndef THEYER_MATRIX_H
#define THEYER_MATRIX_H

#include <type/datatype.h>

#if 0
typedef struct {
	float	x[3];
	float	y[3];
	float	z[3];
	float	t[3];
} Matrix;
#else
typedef struct {
	float	x[4];
	float	y[4];
	float	z[4];
	float	w[4];
} Matrix;
#endif

extern void matrixIdentity( Matrix *mat );
extern void matrixCopy( Matrix *src, Matrix *dest );
extern void matrixTranslate( Matrix *mat, float x, float y, float z );
extern void matrixScale( Matrix *mat, float x, float y, float z );
extern void matrixRotate( Matrix *mat, float x, float y, float z );
extern void matrixMultiply( Matrix *first, Matrix *second, Matrix *result );
extern void matrixApply( Matrix *mat, Point_f3d *s, Point_f3d *d );
extern void matrixPrint( Matrix *mat );
extern void matrixFromAngles( Matrix *mat, char *a1, char *a2, char *a3 );
extern float *matrixToGL( Matrix *mat );

#endif
