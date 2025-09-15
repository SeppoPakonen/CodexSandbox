/*
	Matrix Functions
*/

#include	"matpsx.h"

#include    "sintbl.h"
//extern short sin_tbl[];

#define	FIX_BIT	12
#define	FIX_1	(1<<FIX_BIT)
#define	FIX(a)	((a)>>FIX_BIT)
#define	RSIN(a)	sin_tbl[(a)&(FIX_1-1)]
#define	RCOS(a)	RSIN((a)+(FIX_1/4))

/*
	2つの行列の積
	m2 = m0 x m1
*/
MATRIX *MulMatrix0(MATRIX *m0,MATRIX *m1,MATRIX *m2)
{
#if 0
	/* m0==m2またはm1==m2の時やばいっすね */
#define	M0	m0->m
#define	M1	m1->m
#define	M2	m2->m

	M2[0][0]=FIX(M0[0][0]*M1[0][0] + M0[0][1]*M1[1][0] + M0[0][2]*M1[2][0]);
	M2[0][1]=FIX(M0[0][0]*M1[0][1] + M0[0][1]*M1[1][1] + M0[0][2]*M1[2][1]);
	M2[0][2]=FIX(M0[0][0]*M1[0][2] + M0[0][1]*M1[1][2] + M0[0][2]*M1[2][2]);
	M2[1][0]=FIX(M0[1][0]*M1[0][0] + M0[1][1]*M1[1][0] + M0[1][2]*M1[2][0]);
	M2[1][1]=FIX(M0[1][0]*M1[0][1] + M0[1][1]*M1[1][1] + M0[1][2]*M1[2][1]);
	M2[1][2]=FIX(M0[1][0]*M1[0][2] + M0[1][1]*M1[1][2] + M0[1][2]*M1[2][2]);
	M2[2][0]=FIX(M0[2][0]*M1[0][0] + M0[2][1]*M1[1][0] + M0[2][2]*M1[2][0]);
	M2[2][1]=FIX(M0[2][0]*M1[0][1] + M0[2][1]*M1[1][1] + M0[2][2]*M1[2][1]);
	M2[2][2]=FIX(M0[2][0]*M1[0][2] + M0[2][1]*M1[1][2] + M0[2][2]*M1[2][2]);
#else
	/* これでもm0==m2の時ヤバイ */
	int vx,vy,vz;
	MATRIX tmp;
	/* のでm0をtmpにコピー */
	if (m0==m2) {
		tmp = *m0; m0 = &tmp;
	}

	vx = m1->m[0][0];
	vy = m1->m[1][0];
	vz = m1->m[2][0];
	m2->m[0][0]=FIX(m0->m[0][0]*vx + m0->m[0][1]*vy + m0->m[0][2]*vz);
	m2->m[1][0]=FIX(m0->m[1][0]*vx + m0->m[1][1]*vy + m0->m[1][2]*vz);
	m2->m[2][0]=FIX(m0->m[2][0]*vx + m0->m[2][1]*vy + m0->m[2][2]*vz);
	vx = m1->m[0][1];
	vy = m1->m[1][1];
	vz = m1->m[2][1];
	m2->m[0][1]=FIX(m0->m[0][0]*vx + m0->m[0][1]*vy + m0->m[0][2]*vz);
	m2->m[1][1]=FIX(m0->m[1][0]*vx + m0->m[1][1]*vy + m0->m[1][2]*vz);
	m2->m[2][1]=FIX(m0->m[2][0]*vx + m0->m[2][1]*vy + m0->m[2][2]*vz);
	vx = m1->m[0][2];
	vy = m1->m[1][2];
	vz = m1->m[2][2];
	m2->m[0][2]=FIX(m0->m[0][0]*vx + m0->m[0][1]*vy + m0->m[0][2]*vz);
	m2->m[1][2]=FIX(m0->m[1][0]*vx + m0->m[1][1]*vy + m0->m[1][2]*vz);
	m2->m[2][2]=FIX(m0->m[2][0]*vx + m0->m[2][1]*vy + m0->m[2][2]*vz);
#endif
	return m2;
}

#define	APPLYMATRIX(m,v0,v1)	{\
	int vx = v0->vx;\
	int vy = v0->vy;\
	int vz = v0->vz;\
	v1->vx = FIX(m->m[0][0]*vx + m->m[0][1]*vy + m->m[0][2]*vz );\
	v1->vy = FIX(m->m[1][0]*vx + m->m[1][1]*vy + m->m[1][2]*vz );\
	v1->vz = FIX(m->m[2][0]*vx + m->m[2][1]*vy + m->m[2][2]*vz );\
}

/*
	行列とベクトルの積
	v1 = m x v0
*/
VECTOR *ApplyMatrix(MATRIX *m,SVECTOR *v0,VECTOR *v1)
{
	APPLYMATRIX(m,v0,v1)
	return v1;
}

/*
	行列とベクトルの積
	v1 = m x v0
*/
SVECTOR *ApplyMatrixSV(MATRIX *m,SVECTOR *v0,SVECTOR *v1)
{
	APPLYMATRIX(m,v0,v1)
	return v1;
}

/*
	行列とベクトルの積
	v1 = m x v0
*/
VECTOR *ApplyMatrixLV(MATRIX *m,VECTOR *v0,VECTOR *v1)
{
	APPLYMATRIX(m,v0,v1)
	return v1;
}

#if 0
/*
	x,y,z軸回りの回転による変換行列を求める
	m = [r->vx] x [r->vy] x [r->vz]
*/
MATRIX* RotMatrix(SVECTOR *r,MATRIX *m)
{
	int c0,c1,c2;
	int s0,s1,s2;

	c0=RCOS(r->vx);
	c1=RCOS(r->vy);
	c2=RCOS(r->vz);
	s0=RSIN(r->vx);
	s1=RSIN(r->vy);
	s2=RSIN(r->vz);

	m->m[0][0]=  FIX(c2*c1);
	m->m[1][0]=  FIX(s2*c0) + FIX(FIX(c2*s1)*s0);
	m->m[2][0]=  FIX(s2*s0) - FIX(FIX(c2*s1)*c0);
	m->m[0][1]= -FIX(s2*c1);
	m->m[1][1]=  FIX(c2*c0) - FIX(FIX(s2*s1)*s0);
	m->m[2][1]=  FIX(c2*s0) + FIX(FIX(s2*s1)*c0);
	m->m[0][2]=  s1;
	m->m[1][2]= -FIX(c1*s0);
	m->m[2][2]=  FIX(c1*c0);

	return m;
}
#else
/* 積和公式で乗算を減らす */
MATRIX *RotMatrix(SVECTOR *r,MATRIX *m)
{
	int c0,c1,c2;
	int s0,s1,s2;
	int s2p0,s2m0,c2p0,c2m0;
	int	s2c0,s2s0,c2c0,c2s0;

	c0=RCOS(r->vx);
	c1=RCOS(r->vy);
	c2=RCOS(r->vz);
	s0=RSIN(r->vx);
	s1=RSIN(r->vy);
	s2=RSIN(r->vz);
	s2p0=RSIN( r->vz + r->vx );
	s2m0=RSIN( r->vz - r->vx );
	c2p0=RCOS( r->vz + r->vx );
	c2m0=RCOS( r->vz - r->vx );
	s2c0 =	(s2p0+s2m0)/2;
	c2s0 =	(s2p0-s2m0)/2;
	s2s0 =	(c2m0-c2p0)/2;
	c2c0 =	(c2m0+c2p0)/2;

	m->m[0][0]=  FIX(c2*c1);
	m->m[1][0]=  s2c0 + FIX(c2s0*s1);
	m->m[2][0]=  s2s0 - FIX(c2c0*s1);
	m->m[0][1]= -FIX(s2*c1);
	m->m[1][1]=  c2c0 - FIX(s2s0*s1);
	m->m[2][1]=  c2s0 + FIX(s2c0*s1);
	m->m[0][2]=  s1;
	m->m[1][2]= -FIX(c1*s0);
	m->m[2][2]=  FIX(c1*c0);

	return m;
}
#endif

#if 0
/*
	x軸回りの回転による変換行列を求める
	・・じゃなくてmに回転をかけるという仕様らしい
*/
MATRIX *RotMatrixX(long r,MATRIX *m)
{
	MATRIX tmp;
	int s0 = RSIN(r);
	int c0 = RCOS(r);
	tmp.m[0][0] = FIX_1;
	tmp.m[0][1] = 
	tmp.m[0][2] = 
	tmp.m[1][0] = 
	tmp.m[2][0] = 0;
	tmp.m[1][1] = 
	tmp.m[2][2] = c0;
	tmp.m[1][2] = -s0;
	tmp.m[2][1] =  s0;
	MulMatrix0(&tmp,m,m);
	return m;
}

/*
	y軸回りの回転による変換行列を求める
	・・じゃなくてmに回転をかけるという仕様らしい
*/
MATRIX *RotMatrixY(long r,MATRIX *m)
{
	MATRIX tmp;
	int s0 = RSIN(r);
	int c0 = RCOS(r);
	tmp.m[1][1] = FIX_1;
	tmp.m[1][0] = 
	tmp.m[1][2] = 
	tmp.m[0][1] = 
	tmp.m[2][1] = 0;
	tmp.m[0][0] = 
	tmp.m[2][2] = c0;
	tmp.m[0][2] =  s0;
	tmp.m[2][0] = -s0;
	MulMatrix(&tmp,m,m);
	return m;
}

/*
	z軸回りの回転による変換行列を求める
	・・じゃなくてmに回転をかけるという仕様らしい
*/
MATRIX *RotMatrixZ(long r,MATRIX *m)
{
	MATRIX tmp;
	int s0 = RSIN(r);
	int c0 = RCOS(r);
	tmp.m[2][2] = FIX_1;
	tmp.m[2][0] = 
	tmp.m[2][1] = 
	tmp.m[0][2] = 
	tmp.m[1][2] = 0;
	tmp.m[0][0] = 
	tmp.m[1][1] = c0;
	tmp.m[0][1] = -s0;
	tmp.m[1][0] =  s0;
	MulMatrix(&tmp,m,m);
	return m;
}

#else
MATRIX *RotMatrixX(long r,MATRIX *m)
{
	int s0 = RSIN(r);
	int c0 = RCOS(r);
	int t1,t2;
	t1 = m->m[1][0];
	t2 = m->m[2][0];
	m->m[1][0] = FIX(t1*c0 - t2*s0);
	m->m[2][0] = FIX(t1*s0 + t2*c0);
	t1 = m->m[1][1];
	t2 = m->m[2][1];
	m->m[1][1] = FIX(t1*c0 - t2*s0);
	m->m[2][1] = FIX(t1*s0 + t2*c0);
	t1 = m->m[1][2];
	t2 = m->m[2][2];
	m->m[1][2] = FIX(t1*c0 - t2*s0);
	m->m[2][2] = FIX(t1*s0 + t2*c0);

	return m;
}

MATRIX *RotMatrixY(long r,MATRIX *m)
{
	int s0 = RSIN(r);
	int c0 = RCOS(r);
	int t1,t2;
	t1 = m->m[0][0];
	t2 = m->m[2][0];
	m->m[0][0] = FIX( t1*c0 + t2*s0);
	m->m[2][0] = FIX(-t1*s0 + t2*c0);
	t1 = m->m[0][1];
	t2 = m->m[2][1];
	m->m[0][1] = FIX( t1*c0 + t2*s0);
	m->m[2][1] = FIX(-t1*s0 + t2*c0);
	t1 = m->m[0][2];
	t2 = m->m[2][2];
	m->m[0][2] = FIX( t1*c0 + t2*s0);
	m->m[2][2] = FIX(-t1*s0 + t2*c0);

	return m;
}

MATRIX *RotMatrixZ(long r,MATRIX *m)
{
	int s0 = RSIN(r);
	int c0 = RCOS(r);
	int t1,t2;
	t1 = m->m[0][0];
	t2 = m->m[1][0];
	m->m[0][0] = FIX(t1*c0 - t2*s0);
	m->m[1][0] = FIX(t1*s0 + t2*c0);
	t1 = m->m[0][1];
	t2 = m->m[1][1];
	m->m[0][0] = FIX(t1*c0 - t2*s0);
	m->m[1][1] = FIX(t1*s0 + t2*c0);
	t1 = m->m[0][2];
	t2 = m->m[1][2];
	m->m[0][2] = FIX(t1*c0 - t2*s0);
	m->m[1][2] = FIX(t1*s0 + t2*c0);

	return m;
}


#endif

/*
	行列に平行移動量を代入する
*/
MATRIX *TransMatrix(MATRIX *m,VECTOR *v)
{
	m->t[0] = v->vx;
	m->t[1] = v->vy;
	m->t[2] = v->vz;
	return m;
}

/*
	行列に拡大変換を行う
*/
MATRIX *ScaleMatrix(MATRIX *m,VECTOR *v)
{
	m->m[0][0] = FIX(m->m[0][0]*v->vx);
	m->m[0][1] = FIX(m->m[0][1]*v->vx);
	m->m[0][2] = FIX(m->m[0][2]*v->vx);
	m->m[1][0] = FIX(m->m[1][0]*v->vy);
	m->m[1][1] = FIX(m->m[1][1]*v->vy);
	m->m[1][2] = FIX(m->m[1][2]*v->vy);
	m->m[2][0] = FIX(m->m[2][0]*v->vz);
	m->m[2][1] = FIX(m->m[2][1]*v->vz);
	m->m[2][2] = FIX(m->m[2][2]*v->vz);
	return m;
}

/*
	行列に拡大変換を行う
*/
MATRIX *ScaleMatrixL(MATRIX *m,VECTOR *v)
{
	return ScaleMatrix(m,v);
}

/*
	転値行列を求める
*/
MATRIX *TransposeMatrix(MATRIX *m0,MATRIX *m1)
{
#if 0
	/* m0 == m1の時ヤバイっす */
	m1->m[0][0] = m0->m[0][0];
	m1->m[0][1] = m0->m[1][0];
	m1->m[0][2] = m0->m[2][0];
	m1->m[1][0] = m0->m[0][1];
	m1->m[1][1] = m0->m[1][1];
	m1->m[1][2] = m0->m[2][1];
	m1->m[2][0] = m0->m[0][2];
	m1->m[2][1] = m0->m[1][2];
	m1->m[2][2] = m0->m[2][2];
#else
	int t1,t2;
	t1 = m0->m[0][1];
	t2 = m0->m[1][0];
	m1->m[1][0] = t1;
	m1->m[0][1] = t2;
	t1 = m0->m[0][2];
	t2 = m0->m[2][0];
	m1->m[2][0] = t1;
	m1->m[0][2] = t2;
	t1 = m0->m[1][2];
	t2 = m0->m[2][1];
	m1->m[2][1] = t1;
	m1->m[1][2] = t2;
	if (m0!=m1) {
		m1->m[0][0] = m0->m[0][0];
		m1->m[1][1] = m0->m[1][1];
		m1->m[2][2] = m0->m[2][2];
	}
#endif
	return m1;
}

/*
	平行移動を含む行列の積
*/
MATRIX *CompMatrix(MATRIX *m0,MATRIX *m1,MATRIX *m2)
{
	int vx = m1->t[0];
	int vy = m1->t[1];
	int vz = m1->t[2];
	m2->t[0] = FIX(m0->m[0][0]*vx + m0->m[0][1]*vy + m0->m[0][2]*vz)+ m0->t[0];
	m2->t[1] = FIX(m0->m[1][0]*vx + m0->m[1][1]*vy + m0->m[1][2]*vz)+ m0->t[1];
	m2->t[2] = FIX(m0->m[2][0]*vx + m0->m[2][1]*vy + m0->m[2][2]*vz)+ m0->t[2];
	MulMatrix0(m0,m1,m2);
	return m2;
}

void gteMIMefunc(SVECTOR *otp, SVECTOR *dfp, long n, long p)
{
	while(--n>=0) {
		otp->vx += FIX(dfp->vx*p);
		otp->vy += FIX(dfp->vy*p);
		otp->vz += FIX(dfp->vz*p);
		otp++;
		dfp++;
	}
}
