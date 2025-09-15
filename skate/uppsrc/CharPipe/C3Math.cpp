/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3Math.c]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/build/charPipeline/c3/src/C3Math.c $
    
    8     7/24/00 12:16p John
    Moved EPSILON and COS_EPSILON out of header file and into C3Math.c
    
    7     7/19/00 12:05p John
    Added OSPanic for DEBUG build since we are now including MTX library.
    
    6     7/18/00 7:43p John
    Added C3MtxToQuat again.
    
    5     7/18/00 7:28p John
    Now uses MTX functions instead of C3Mtx functions.
    Changed QUAT prefix to C3Quat and commented function headers.
    
    4     3/15/00 3:45p John
    Cleaned up code.
    
    3     2/29/00 7:23p John
    Added C3QuatNormalize.
    
    2     2/04/00 6:07p John
    Untabified code.
    
    2     11/18/99 1:17p John
    Initial math file to support animation extraction.
    
  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#include "C3Private.h"

#define EPSILON               0.0001f
#define COS_EPSILON           0.000001f

/*---------------------------------------------------------------------------*
  Name:         OSPanic

  Description:  Reports an assertion failure.  This function only exists
                since c3 library includes the matrix library, and OSPanic
                is needed to print assertion failure messages during DEBUG
                builds.

  Arguments:    file - filename where assertion failed
                line - line number
                msg  - message to display

  Returns:      NONE
/*---------------------------------------------------------------------------*/
#ifdef _DEBUG
void
OSPanic( const char* file, int line, const char* msg, ... )
{
    C3ReportError( "Assert failure in %s line %d", file, line );
}
#endif

/*---------------------------------------------------------------------------*
  Name:         C3MtxToQuat

  Description:  Converts a matrix to a unit quaternion.

  Arguments:    m - the matrix
                dst - result quaternion

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3MtxToQuat( Mtx m, Quaternion *dst )
{
    f32 tr,s;
    s32 i,j,k;
    s32 nxt[3] = {1,2,0};
    f32 q[3];

    C3_ASSERT( m && dst );

    tr = m[0][0] + m[1][1] + m[2][2];
    if( tr > 0.0f )
    {
        s = (f32)sqrt(tr + 1.0f);
        dst->w = s * 0.5f;
        s = 0.5f / s;
        dst->x = (m[1][2] - m[2][1]) * s;
        dst->y = (m[2][0] - m[0][2]) * s;
        dst->z = (m[0][1] - m[1][0]) * s;
    }
    else 
    {
        i = 0;
        if (m[1][1] > m[0][0]) i = 1;
        if (m[2][2] > m[i][i]) i = 2;
        j = nxt[i];
        k = nxt[j];
        s = (f32)sqrt( (m[i][i] - (m[j][j] + m[k][k])) + 1.0f );
        q[i] = s * 0.5f;
        if (s!=0.0f) s = 0.5f / s;
        dst->w = (m[j][k] - m[k][j]) * s;
        q[j] = (m[i][j] + m[j][i]) * s;
        q[k] = (m[i][k] + m[k][i]) * s;

        dst->x = q[0];
        dst->y = q[1];
        dst->z = q[2];
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3QuatNormalize

  Description:  Normalizes a quaternion

  Arguments:    src - the source quaternion
                unit - resulting unit quaternion

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3QuatNormalize( Quaternion *src, Quaternion *unit )
{
    f32 mag;

    C3_ASSERT( src && unit );

    mag = (f32)sqrt((src->x * src->x) + (src->y * src->y) + (src->z * src->z) + (src->w * src->w));

    if ( mag < -C3_FLT_EPSILON || mag > C3_FLT_EPSILON )
    {
        unit->x = src->x / mag;
        unit->y = src->y / mag;
        unit->z = src->z / mag;
        unit->w = src->w / mag;
    }    
}


/*---------------------------------------------------------------------------*
  Name:         C3QuatDot

  Description:  Returns the dot product of the two quaternions.

  Arguments:    p - first quaternion
                q - second quaternion

  Returns:      Dot product
/*---------------------------------------------------------------------------*/
f32 
C3QuatDot( Quaternion p, Quaternion q) 
{
    return (q.x*p.x + q.y*p.y + q.z*p.z + q.w*p.w);
}


/*---------------------------------------------------------------------------*
  Name:         C3QuatInverse

  Description:  Returns the inverse of the quaternion.

  Arguments:    p - quaternion

  Returns:      Inverse
/*---------------------------------------------------------------------------*/
Quaternion 
C3QuatInverse( Quaternion q )
{
    f32 l, norminv;

    Quaternion qq;
    l = ( q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w );
    if ( l == 0.0f ) 
        l = 1.0f;
    norminv = (f32)1.0f / l;
    qq.x = -q.x * norminv;
    qq.y = -q.y * norminv;
    qq.z = -q.z * norminv;
    qq.w =  q.w * norminv;

    return qq;
}


/*---------------------------------------------------------------------------*
  Name:         C3QuatMultiply

  Description:  Returns the product of two quaternions.  The order of 
                multiplication is important p*q.

  Arguments:    p - left quaternion
                q - right quaternion

  Returns:      Product p*q
/*---------------------------------------------------------------------------*/
Quaternion 
C3QuatMultiply( Quaternion p, Quaternion q ) 
{
    Quaternion qp;

    qp.w = p.w*q.w - p.x*q.x - p.y*q.y - p.z*q.z;
    qp.x = p.w*q.x + p.x*q.w + p.y*q.z - p.z*q.y;
    qp.y = p.w*q.y + p.y*q.w + p.z*q.x - p.x*q.z;
    qp.z = p.w*q.z + p.z*q.w + p.x*q.y - p.y*q.x;

    return qp;
}


/*---------------------------------------------------------------------------*
  Name:         C3QuatDivide

  Description:  Returns the ratio of two quaternions.  Creates a result 
                r = p/q such that q*r=p (order of multiplication is important).

  Arguments:    p - left quaternion
                q - right quaternion

  Returns:      Ratio p/q
/*---------------------------------------------------------------------------*/
Quaternion 
C3QuatDivide( Quaternion p, Quaternion q )
{
    return ( C3QuatMultiply( C3QuatInverse( q ), p ) );
}


/*---------------------------------------------------------------------------*
  Name:         C3QuatLogN

  Description:  Returns the natural logarithm of a UNIT quaternion

  Arguments:    q - unit quaternion

  Returns:      Natural log
/*---------------------------------------------------------------------------*/
Quaternion 
C3QuatLogN( Quaternion q )
{
    f32 theta,scale;
    Quaternion qq;

    scale = (f32)sqrt( q.x*q.x + q.y*q.y + q.z*q.z );
    theta = (f32)atan2( scale, q.w );
    if ( scale > 0.0f )
        scale = theta/scale;
    qq.x = scale*q.x;
    qq.y = scale*q.y;
    qq.z = scale*q.z;
    qq.w = 0.0f;

    return qq;
}


/*---------------------------------------------------------------------------*
  Name:         C3QuatLnDif

  Description:  Returns the natural log of p/q.

  Arguments:    p - left quaternion
                q - right quaternion

  Returns:      ln(p/q)
/*---------------------------------------------------------------------------*/
Quaternion 
C3QuatLnDif( Quaternion p, Quaternion q )
{
    Quaternion r;

    r = C3QuatDivide( q, p );

    return C3QuatLogN( r );
}


/*---------------------------------------------------------------------------*
  Name:         C3QuatScale

  Description:  Scales a quaternion.

  Arguments:    q     - quaternion 
                scale - float to scale by

  Returns:      Scaled quaternion
/*---------------------------------------------------------------------------*/
Quaternion 
C3QuatScale( Quaternion q, f32 scale )
{
    Quaternion r;

    r.x = q.x * scale;
    r.y = q.y * scale;
    r.z = q.z * scale;
    r.w = q.w * scale;

    return r;
}


/*---------------------------------------------------------------------------*
  Name:         C3QuatAdd

  Description:  Returns the sum of two quaternions.

  Arguments:    p - first quaternion
                q - second quaternion

  Returns:      Sum p+q
/*---------------------------------------------------------------------------*/
Quaternion 
C3QuatAdd( Quaternion p, Quaternion q )
{
    Quaternion r;

    r.x = p.x + q.x;
    r.y = p.y + q.y;
    r.z = p.z + q.z;
    r.w = p.w + q.w;

    return r;
}


/*---------------------------------------------------------------------------*
  Name:         C3QuatMinus

  Description:  Returns the difference of two quaternions p-q.

  Arguments:    p - left quaternion
                q - right quaternion

  Returns:      Difference p-q
/*---------------------------------------------------------------------------*/
Quaternion 
C3QuatMinus( Quaternion p, Quaternion q )
{
    Quaternion r;

    r.x = p.x - q.x;
    r.y = p.y - q.y;
    r.z = p.z - q.z;
    r.w = p.w - q.w;

    return r;
}


/*---------------------------------------------------------------------------*
  Name:         C3QuatExp

  Description:  Exponentiate quaternion (where q.w == 0).

  Arguments:    q - quaternion

  Returns:      Exponentiate quaternion
/*---------------------------------------------------------------------------*/
Quaternion 
C3QuatExp( Quaternion q ) 
{
    f32 theta, scale;
    Quaternion qq;

    theta = (f32)sqrt( q.x*q.x + q.y*q.y + q.z*q.z );
    scale = 1.0f;
    if ( theta > EPSILON ) 
        scale = (f32)sin(theta)/theta;
    qq.x = scale * q.x;
    qq.y = scale * q.y;
    qq.z = scale * q.z;
    qq.w = (f32)cos(theta);

    return qq;
}


/*---------------------------------------------------------------------------*
  Name:         C3QuatMakeClosest

  Description:  Modify q so it is on the same side of the hypersphere as qto

  Arguments:    q   - quaternion (also return value)
                qto - quaternion to be close to

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3QuatMakeClosest( Quaternion *q, Quaternion qto ) 
{
    f32 dot =  q->x*qto.x + q->y*qto.y + q->z*qto.z+ q->w*qto.w;
    if ( dot < 0.0f ) 
    {
        q->x = -q->x; q->y = -q->y; q->z = -q->z; q->w = -q->w;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3QuatCompA

  Description:  Compute a, the term used in Boehm-type interpolation
                a[n] = q[n]* qexp(-(1/4)*( ln(qinv(q[n])*q[n+1]) +
                                           ln( qinv(q[n])*q[n-1] )))

  Arguments:    qprev - previous quaternion
                q     - current quaternion
                qnext - next quaternion

  Returns:      Quaternion A
/*---------------------------------------------------------------------------*/
Quaternion 
C3QuatCompA( Quaternion qprev, Quaternion q, Quaternion qnext) 
{
    Quaternion qm, qp, r;

    qm = C3QuatLnDif( q, qprev );
    qp = C3QuatLnDif( q, qnext );
    r = C3QuatScale( C3QuatAdd( qm, qp ), -0.25f );

    return C3QuatMultiply( q, C3QuatExp(r) );
}
