#ifndef _MATRIX_H_
#define	_MATRIX_H_

typedef struct {
	long	vx,vy,vz,pad;
} VECTOR;

typedef struct {
	short	vx,vy,vz,pad;
} SVECTOR;

typedef struct {
	unsigned char r,g,b,pad;
} CVECTOR;

typedef struct {
	short m[3][3];
	long t[3];
} MATRIX;

#ifdef __cplusplus
extern "C" {
#endif

MATRIX *MulMatrix0(MATRIX *m0,MATRIX *m1,MATRIX *m2);
VECTOR *ApplyMatrix(MATRIX *m,SVECTOR *v0,VECTOR *v1);
SVECTOR *ApplyMatrixSV(MATRIX *m,SVECTOR *v0,SVECTOR *v1);
VECTOR *ApplyMatrixLV(MATRIX *m,VECTOR *v0,VECTOR *v1);
MATRIX* RotMatrix(SVECTOR *r,MATRIX *m);
MATRIX *RotMatrixX(long r,MATRIX *m);
MATRIX *RotMatrixY(long r,MATRIX *m);
MATRIX *RotMatrixZ(long r,MATRIX *m);
MATRIX *TransMatrix(MATRIX *m,VECTOR *v);
MATRIX *ScaleMatrix(MATRIX *m,VECTOR *v);
MATRIX *TransposeMatrix(MATRIX *m0,MATRIX *m1);
MATRIX *CompMatrix(MATRIX *m0,MATRIX *m1,MATRIX *m2);
void gteMIMefunc(SVECTOR *otp, SVECTOR *dfp, long n, long p);

#ifdef __cplusplus
}
#endif

#endif
