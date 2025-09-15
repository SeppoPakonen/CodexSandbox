#include "DolphinRE.h"


/*---------------------------------------------------------------------------*
  Project: matrix vector Library
  File:    mtx.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.     They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.


  $Log: /Dolphin/build/libraries/mtx/src/mtx.c $NoKeywords: $
    
    17    7/31/00 2:07p Carl
    Removed redundant verify in MTXRotRad.
    Minor potential optimization in VECNormalize.
    
    16    7/21/00 4:19p Carl
    Removed as many divides as possible.
    
    15    7/21/00 2:58p Carl
    Removed unnecessary VECNormalize in MTXLookAt.
    
    14    7/12/00 4:41p John
    Substitues MTXConcat and MTXMultVecArray with their paired-singles
    equivalent for Gekko nondebug builds.
    
    13    7/07/00 7:09p Dante
    PC Compatibility
    
    12    5/10/00 1:50p Hirose
    added radian-based rotation matrix functions
    moved paired-single matrix stuff into an another source file
    
    11    4/13/00 11:26a Danm
    Restore hgh performance version of ROMulVecArray.
    
    10    4/10/00 11:56a Danm
    Added 2 matrix skinning support.
    
    9     3/27/00 6:38p Carl
    Fixed other comments.
    
    8     3/27/00 6:20p Carl
    Changed comment regarding z scale in MTXFrustum.
    
    7     3/22/00 2:01p John
    Added VECSquareDistance and VECDistance.
    
    6     2/14/00 2:04p Mikepc
    detabbed code.
    
    5     2/11/00 4:22p Tian
    Cleaned up reordered paired single code
    
    4     1/27/00 5:21p Tian
    Documented paired single ops.
    
    3     1/27/00 4:56p Tian
    Implemented fast paired single matrix ops : PSMTXReorder, PSMTXConcat,
    PSMTXROMultVecArray, PSMTXMultVecArray, PSMTXMultS16VecArray,
    PSMTXROMultS16VecArray
    
    2     12/06/99 12:40p Alligator
    modify proj mtx to scale/translate z to (0, w) range for hardware
    
    20    11/17/99 4:32p Ryan
    update to fin win32 compiler warnings
    
    19    11/12/99 12:08p Mikepc
    changed #include<Dolphin/mtx/mtxAssert.h> to #include"mtxAssert.h" to
    reflect new location of mtxAssert.h file.
    
    18    11/10/99 7:21p Hirose
    added singular case check for VECHalfAngle
    
    17    11/10/99 4:40p Alligator
    added MTXReflect
    
    16    10/06/99 12:28p Yasu
    Appended MTXMultVecSR and MTXMultVecArraySR
    
    15    8/31/99 5:41p Shiki
    Revised to use sqrtf() instead of sqrt().

    14      8/31/99 5:18p Shiki
    Revised to use sinf()/cosf() instead of sin()/cos().

    13      7/28/99 3:38p Ryan

    12      7/28/99 11:30a Ryan
    Added Texture Projection functions

    11      7/02/99 12:53p Mikepc
    changed mtx row/col

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
/*#include <Dolphin/platform.h>
#include <math.h>
#include <Dolphin/mtx.h>
#include "MtxAssert.h"*/

#include "DolphinRE.h"



/*---------------------------------------------------------------------*





                            GENERAL SECTION





*---------------------------------------------------------------------*/





/*---------------------------------------------------------------------*

Name:           MTXIdentity

Description:    sets a matrix to identity

Arguments:      m :  matrix to be set

Return:         none

*---------------------------------------------------------------------*/
void MTXIdentity ( Mtx m )
{

    ASSERTMSG( (m != 0), MTX_IDENTITY_1 );


    m[0][0] = 1.0f;     m[0][1] = 0.0f;  m[0][2] = 0.0f;  m[0][3] = 0.0f;

    m[1][0] = 0.0f;     m[1][1] = 1.0f;  m[1][2] = 0.0f;  m[1][3] = 0.0f;

    m[2][0] = 0.0f;     m[2][1] = 0.0f;  m[2][2] = 1.0f;  m[2][3] = 0.0f;

}

/*---------------------------------------------------------------------*

Name:           MTXCopy

Description:    copies the contents of one matrix into another

Arguments:      src        source matrix for copy
                dst        destination matrix for copy


Return:         none

*---------------------------------------------------------------------*/
void MTXCopy ( MtxPtr src, MtxPtr dst )
{

    ASSERTMSG( (src != 0) , MTX_COPY_1 );
    ASSERTMSG( (dst != 0) , MTX_COPY_2 );

    if( src == dst )
    {
        return;
    }


    dst[0][0] = src[0][0];    dst[0][1] = src[0][1];    dst[0][2] = src[0][2];    dst[0][3] = src[0][3];

    dst[1][0] = src[1][0];    dst[1][1] = src[1][1];    dst[1][2] = src[1][2];    dst[1][3] = src[1][3];

    dst[2][0] = src[2][0];    dst[2][1] = src[2][1];    dst[2][2] = src[2][2];    dst[2][3] = src[2][3];

}

/*---------------------------------------------------------------------*

Name:           MTXConcat

Description:    concatenates two matrices.
                order of operation is A x B = AB.
                ok for any of ab == a == b.

                saves a MTXCopy operation if ab != to a or b.


Arguments:      a        first matrix for concat.
                b        second matrix for concat.
                ab       resultant matrix from concat.


Return:         none

*---------------------------------------------------------------------*/
#if defined(_DEBUG) || !defined(GEKKO)  // Use PSMTXConcat code instead for Gekko nondebug builds
void MTXConcat ( Mtx a, Mtx b, Mtx ab )
{
    Mtx mTmp;
    MtxPtr m;


    ASSERTMSG( (a  != 0), MTX_CONCAT_1 );
    ASSERTMSG( (b  != 0), MTX_CONCAT_2 );
    ASSERTMSG( (ab != 0), MTX_CONCAT_3 );


    if( (ab == a) || (ab == b) )
    {
        m = mTmp;
    }

    else
    {
        m = ab;
    }


    // compute (a x b) -> m

    m[0][0] = a[0][0]*b[0][0] + a[0][1]*b[1][0] + a[0][2]*b[2][0];
    m[0][1] = a[0][0]*b[0][1] + a[0][1]*b[1][1] + a[0][2]*b[2][1];
    m[0][2] = a[0][0]*b[0][2] + a[0][1]*b[1][2] + a[0][2]*b[2][2];
    m[0][3] = a[0][0]*b[0][3] + a[0][1]*b[1][3] + a[0][2]*b[2][3] + a[0][3];

    m[1][0] = a[1][0]*b[0][0] + a[1][1]*b[1][0] + a[1][2]*b[2][0];
    m[1][1] = a[1][0]*b[0][1] + a[1][1]*b[1][1] + a[1][2]*b[2][1];
    m[1][2] = a[1][0]*b[0][2] + a[1][1]*b[1][2] + a[1][2]*b[2][2];
    m[1][3] = a[1][0]*b[0][3] + a[1][1]*b[1][3] + a[1][2]*b[2][3] + a[1][3];

    m[2][0] = a[2][0]*b[0][0] + a[2][1]*b[1][0] + a[2][2]*b[2][0];
    m[2][1] = a[2][0]*b[0][1] + a[2][1]*b[1][1] + a[2][2]*b[2][1];
    m[2][2] = a[2][0]*b[0][2] + a[2][1]*b[1][2] + a[2][2]*b[2][2];
    m[2][3] = a[2][0]*b[0][3] + a[2][1]*b[1][3] + a[2][2]*b[2][3] + a[2][3];


    // overwrite a or b if needed
    if(m == mTmp)
    {
        MTXCopy( mTmp, ab );
    }

}
#endif // defined(_DEBUG) || !defined(GEKKO)

/*---------------------------------------------------------------------*

Name:           MTXMultVec

Description:    multiplies a vector by a matrix.
                m x src = dst.


Arguments:      m         matrix.
                src       source vector for multiply.
                dst       resultant vector from multiply.

                note:      ok if src == dst.


Return:         none

*---------------------------------------------------------------------*/
void MTXMultVec ( Mtx m, VecPtr src, VecPtr dst )
{
    Vec vTmp;


    ASSERTMSG( (m   != 0), MTX_MULTVEC_1 );
    ASSERTMSG( (src != 0), MTX_MULTVEC_2 );
    ASSERTMSG( (dst != 0), MTX_MULTVEC_3 );


    // a Vec has a 4th implicit 'w' coordinate of 1
    vTmp.x = m[0][0]*src->x + m[0][1]*src->y + m[0][2]*src->z + m[0][3];
    vTmp.y = m[1][0]*src->x + m[1][1]*src->y + m[1][2]*src->z + m[1][3];
    vTmp.z = m[2][0]*src->x + m[2][1]*src->y + m[2][2]*src->z + m[2][3];


    // copy back
    dst->x = vTmp.x;
    dst->y = vTmp.y;
    dst->z = vTmp.z;
}

/*---------------------------------------------------------------------*

Name:           MTXMultVecArray

Description:    multiplies an array of vectors by a matrix.


Arguments:      m         matrix.
                srcBase   start of source vector array.
                dstBase   start of resultant vector array.

                note:     ok if srcBase == dstBase.

                count     number of vectors in srcBase, dstBase arrays
                          note:      cannot check for array overflow


Return:         none

*---------------------------------------------------------------------*/
#if defined(_DEBUG) || !defined(GEKKO)  // Use PSMTXMultVecArray code instead for Gekko nondebug builds
void MTXMultVecArray ( Mtx m, VecPtr srcBase, VecPtr dstBase, u32 count )
{
    u32 i;
    Vec vTmp;


    ASSERTMSG( (m       != 0),    MTX_MULTVECARRAY_1    );
    ASSERTMSG( (srcBase != 0),    MTX_MULTVECARRAY_2    );
    ASSERTMSG( (dstBase != 0),    MTX_MULTVECARRAY_3    );


    for(i=0; i< count; i++)
    {
        // Vec has a 4th implicit 'w' coordinate of 1
        vTmp.x = m[0][0]*srcBase->x + m[0][1]*srcBase->y + m[0][2]*srcBase->z + m[0][3];
        vTmp.y = m[1][0]*srcBase->x + m[1][1]*srcBase->y + m[1][2]*srcBase->z + m[1][3];
        vTmp.z = m[2][0]*srcBase->x + m[2][1]*srcBase->y + m[2][2]*srcBase->z + m[2][3];

        // copy back
        dstBase->x = vTmp.x;
        dstBase->y = vTmp.y;
        dstBase->z = vTmp.z;

        srcBase++;
        dstBase++;
    }

}
#endif // defined(_DEBUG) || !defined(GEKKO) 

/*---------------------------------------------------------------------*

Name:         MTXMultVecSR

Description:  multiplies a vector by a matrix 3x3 (Scaling and Rotation)
              component.
        
              m x src = dst.

Arguments:    m       matrix.
              src     source vector for multiply.
              dst     resultant vector from multiply.

              note:   ok if src == dst.

Return:       none

*---------------------------------------------------------------------*/
void MTXMultVecSR ( MtxPtr m, VecPtr src, VecPtr dst )
{
    Vec vTmp;

    ASSERTMSG(m  , MTX_MULTVECSR_1);
    ASSERTMSG(src, MTX_MULTVECSR_2);
    ASSERTMSG(dst, MTX_MULTVECSR_3);

    // a Vec has a 4th implicit 'w' coordinate of 1
    vTmp.x = m[0][0]*src->x + m[0][1]*src->y + m[0][2]*src->z;
    vTmp.y = m[1][0]*src->x + m[1][1]*src->y + m[1][2]*src->z;
    vTmp.z = m[2][0]*src->x + m[2][1]*src->y + m[2][2]*src->z;

    // copy back
    dst->x = vTmp.x;
    dst->y = vTmp.y;
    dst->z = vTmp.z;
}

/*---------------------------------------------------------------------*

Name:           MTXMultVecArraySR

Description:    multiplies an array of vectors by a matrix 3x3
                (Scaling and Rotation) component.

Arguments:      m        matrix.
                srcBase  start of source vector array.
                dstBase  start of resultant vector array.

                note:    ok if srcBase == dstBase.

                count    number of vectors in srcBase, dstBase arrays
                note:    cannot check for array overflow
 
Return:         none

*---------------------------------------------------------------------*/
void MTXMultVecArraySR ( Mtx m, VecPtr srcBase, VecPtr dstBase, u32 count )
{
    u32 i;
    Vec vTmp;

    ASSERTMSG( (m       != 0), MTX_MULTVECARRAYSR_1 );
    ASSERTMSG( (srcBase != 0), MTX_MULTVECARRAYSR_2 );
    ASSERTMSG( (dstBase != 0), MTX_MULTVECARRAYSR_3 );

    for ( i = 0; i < count; i ++ )
    {
        // Vec has a 4th implicit 'w' coordinate of 1
        vTmp.x = m[0][0]*srcBase->x + m[0][1]*srcBase->y + m[0][2]*srcBase->z;
        vTmp.y = m[1][0]*srcBase->x + m[1][1]*srcBase->y + m[1][2]*srcBase->z;
        vTmp.z = m[2][0]*srcBase->x + m[2][1]*srcBase->y + m[2][2]*srcBase->z;

        // copy back
        dstBase->x = vTmp.x;
        dstBase->y = vTmp.y;
        dstBase->z = vTmp.z;

        srcBase++;
        dstBase++;
    }
}

/*---------------------------------------------------------------------*

Name:           MTXTranspose

Description:    computes the transpose of a matrix.
                As matrices are 3x4, fourth column (translation component) is
                lost and becomes (0,0,0).

                This function is intended for use in computing an
                inverse-transpose matrix to transform normals for lighting.
                In this case, lost translation component doesn't matter.


Arguments:      src       source matrix.
                xPose     destination (transposed) matrix.
                          ok if src == xPose.



Return:         none

*---------------------------------------------------------------------*/
void MTXTranspose ( Mtx src, Mtx xPose )
{
    Mtx mTmp;
    MtxPtr m;


    ASSERTMSG( (src   != 0), MTX_TRANSPOSE_1  );
    ASSERTMSG( (xPose != 0), MTX_TRANSPOSE_2  );


    if(src == xPose)
    {
        m = mTmp;
    }
    else
    {
        m = xPose;
    }


    m[0][0] = src[0][0];   m[0][1] = src[1][0];      m[0][2] = src[2][0];     m[0][3] = 0.0f;
    m[1][0] = src[0][1];   m[1][1] = src[1][1];      m[1][2] = src[2][1];     m[1][3] = 0.0f;
    m[2][0] = src[0][2];   m[2][1] = src[1][2];      m[2][2] = src[2][2];     m[2][3] = 0.0f;


    // copy back if needed
    if( m == mTmp )
    {
        MTXCopy( mTmp, xPose );
    }
}

/*---------------------------------------------------------------------*

Name:           MTXInverse

Description:    computes a fast inverse of a matrix.
                this algorithm works for matrices with a fourth row of
                (0,0,0,1).

                for a matrix
                M =  |     A      C      |  where A is the upper 3x3 submatrix,
                     |     0      1      |        C is a 1x3 column vector


                INV(M)     =    |  inv(A)      (inv(A))*(-C)    |
                                |     0               1         |



Arguments:      src       source matrix.
                inv       destination (inverse) matrix.
                          ok if src == inv.



Return:         0 if src is not invertible.
                1 on success.

*---------------------------------------------------------------------*/
u32 MTXInverse ( Mtx src, Mtx inv )
{
    Mtx mTmp;
    MtxPtr m;
    f32 det;


    ASSERTMSG( (src != 0), MTX_INVERSE_1 );
    ASSERTMSG( (inv != 0), MTX_INVERSE_2 );


    if( src == inv )
    {
        m = mTmp;
    }
    else
    {
        m = inv;
    }


    // compute the determinant of the upper 3x3 submatrix
    det =   src[0][0]*src[1][1]*src[2][2] + src[0][1]*src[1][2]*src[2][0] + src[0][2]*src[1][0]*src[2][1]
          - src[2][0]*src[1][1]*src[0][2] - src[1][0]*src[0][1]*src[2][2] - src[0][0]*src[2][1]*src[1][2];


    // check if matrix is singular
    if( det == 0.0f )
    {
        return 0;
    }


    // compute the inverse of the upper submatrix:

    // find the transposed matrix of cofactors of the upper submatrix
    // and multiply by (1/det)

    det = 1.0f / det;


    m[0][0] =  (src[1][1]*src[2][2] - src[2][1]*src[1][2]) * det;
    m[0][1] = -(src[0][1]*src[2][2] - src[2][1]*src[0][2]) * det;
    m[0][2] =  (src[0][1]*src[1][2] - src[1][1]*src[0][2]) * det;

    m[1][0] = -(src[1][0]*src[2][2] - src[2][0]*src[1][2]) * det;
    m[1][1] =  (src[0][0]*src[2][2] - src[2][0]*src[0][2]) * det;
    m[1][2] = -(src[0][0]*src[1][2] - src[1][0]*src[0][2]) * det;

    m[2][0] =  (src[1][0]*src[2][1] - src[2][0]*src[1][1]) * det;
    m[2][1] = -(src[0][0]*src[2][1] - src[2][0]*src[0][1]) * det;
    m[2][2] =  (src[0][0]*src[1][1] - src[1][0]*src[0][1]) * det;


    // compute (invA)*(-C)
    m[0][3] = -m[0][0]*src[0][3] - m[0][1]*src[1][3] - m[0][2]*src[2][3];
    m[1][3] = -m[1][0]*src[0][3] - m[1][1]*src[1][3] - m[1][2]*src[2][3];
    m[2][3] = -m[2][0]*src[0][3] - m[2][1]*src[1][3] - m[2][2]*src[2][3];


    // copy back if needed
    if( m == mTmp )
    {
        MTXCopy( mTmp,inv );
    }


    return 1;
}



/*---------------------------------------------------------------------*





                             MODEL SECTION





*---------------------------------------------------------------------*/





/*---------------------------------------------------------------------*

Name:           MTXRotDeg

Description:    sets a rotation matrix about one of the X, Y or Z axes

Arguments:      m       matrix to be set

                axis    major axis about which to rotate.
                        axis is passed in as a character.
                        it must be one of 'X', 'x', 'Y', 'y', 'Z', 'z'

                deg     rotation angle in degrees.

                        note:  counter-clockwise rotation is positive.

Return:         none

*---------------------------------------------------------------------*/
// The function is defined as inlined in mtx.h
/*
void MTXRotDeg ( Mtx m, char axis, f32 deg )
{
    MTXRotRad( m, axis, MTXDegToRad( deg ) );
}
*/

/*---------------------------------------------------------------------*

Name:           MTXRotRad

Description:    sets a rotation matrix about one of the X, Y or Z axes

Arguments:      m       matrix to be set

                axis    major axis about which to rotate.
                        axis is passed in as a character.
                        it must be one of 'X', 'x', 'Y', 'y', 'Z', 'z'

                deg     rotation angle in radians.

                        note:  counter-clockwise rotation is positive.

Return:         none

*---------------------------------------------------------------------*/
void MTXRotRad ( Mtx m, char axis, f32 rad )
{

    f32 sinA, cosA;

    ASSERTMSG( (m != 0), MTX_ROTDEG_1 );

    // verification of "axis" will occur in MTXRotTrig

    sinA = sinf(rad);
    cosA = cosf(rad);

    MTXRotTrig( m, axis, sinA, cosA );
}

/*---------------------------------------------------------------------*

Name:           MTXRotTrig

Description:    sets a rotation matrix about one of the X, Y or Z axes
                from specified trig ratios

Arguments:      m       matrix to be set

                axis    major axis about which to rotate.
                        axis is passed in as a character.
                        It must be one of 'X', 'x', 'Y', 'y', 'Z', 'z'

                sinA    sine of rotation angle.

                cosA    cosine of rotation angle.

                        note:  counter-clockwise rotation is positive.

Return:         none

*---------------------------------------------------------------------*/
void MTXRotTrig ( Mtx m, char axis, f32 sinA, f32 cosA )
{


    ASSERTMSG( (m != 0), MTX_ROTTRIG_1 );


    switch(axis)
    {

    case 'x':
    case 'X':
        m[0][0] =  1.0f;  m[0][1] =  0.0f;    m[0][2] =  0.0f;  m[0][3] = 0.0f;
        m[1][0] =  0.0f;  m[1][1] =  cosA;    m[1][2] = -sinA;  m[1][3] = 0.0f;
        m[2][0] =  0.0f;  m[2][1] =  sinA;    m[2][2] =  cosA;  m[2][3] = 0.0f;
        break;                      
                                    
    case 'y':                       
    case 'Y':                       
        m[0][0] =  cosA;  m[0][1] =  0.0f;    m[0][2] =  sinA;  m[0][3] = 0.0f;
        m[1][0] =  0.0f;  m[1][1] =  1.0f;    m[1][2] =  0.0f;  m[1][3] = 0.0f;
        m[2][0] = -sinA;  m[2][1] =  0.0f;    m[2][2] =  cosA;  m[2][3] = 0.0f;
        break;                      
                                    
    case 'z':                       
    case 'Z':                       
        m[0][0] =  cosA;  m[0][1] = -sinA;    m[0][2] =  0.0f;  m[0][3] = 0.0f;
        m[1][0] =  sinA;  m[1][1] =  cosA;    m[1][2] =  0.0f;  m[1][3] = 0.0f;
        m[2][0] =  0.0f;  m[2][1] =  0.0f;    m[2][2] =  1.0f;  m[2][3] = 0.0f;
        break;

    default:
        ASSERTMSG( 0, MTX_ROTTRIG_2 );
        break;

    }

}

/*---------------------------------------------------------------------*

Name:           MTXRotAxisDeg

Description:    sets a rotation matrix about an arbitrary axis


Arguments:      m       matrix to be set

                axis    ptr to a vector containing the x,y,z axis
                        components.
                        axis does not have to be a unit vector.

                deg     rotation angle in degrees.

                        note:  counter-clockwise rotation is positive.

Return:         none

*---------------------------------------------------------------------*/
// The function is defined as inlined in mtx.h
/*
void MTXRotAxisDeg ( Mtx m, VecPtr axis, f32 deg )
{
    MTXRotAxisRad( m, axis, MTXDegToRad( deg ) );
}
*/

/*---------------------------------------------------------------------*

Name:           MTXRotAxisRad

Description:    sets a rotation matrix about an arbitrary axis


Arguments:      m       matrix to be set

                axis    ptr to a vector containing the x,y,z axis
                        components.
                        axis does not have to be a unit vector.

                deg     rotation angle in radians.

                        note:  counter-clockwise rotation is positive.

Return:         none

*---------------------------------------------------------------------*/
void MTXRotAxisRad( Mtx m, VecPtr axis, f32 rad )
{
    Vec vN;
    f32 s, c;             // sinTheta, cosTheta
    f32 t;                // ( 1 - cosTheta )
    f32 x, y, z;          // x, y, z components of normalized axis
    f32 xSq, ySq, zSq;    // x, y, z squared


    ASSERTMSG( (m    != 0), MTX_ROTAXIS_1  );
    ASSERTMSG( (axis != 0), MTX_ROTAXIS_2  );


    s = sinf(rad);
    c = cosf(rad);

    t = 1.0f - c;


    VECNormalize( axis, &vN );

    x = vN.x;
    y = vN.y;
    z = vN.z;

    xSq = x * x;
    ySq = y * y;
    zSq = z * z;


    m[0][0] = ( t * xSq )   + ( c );
    m[0][1] = ( t * x * y ) - ( s * z );
    m[0][2] = ( t * x * z ) + ( s * y );
    m[0][3] =    0.0f;

    m[1][0] = ( t * x * y ) + ( s * z );
    m[1][1] = ( t * ySq )   + ( c );
    m[1][2] = ( t * y * z ) - ( s * x );
    m[1][3] =    0.0f;

    m[2][0] = ( t * x * z ) - ( s * y );
    m[2][1] = ( t * y * z ) + ( s * x );
    m[2][2] = ( t * zSq )   + ( c );
    m[2][3] =    0.0f;


}

/*---------------------------------------------------------------------*

Name:           MTXTrans

Description:    sets a translation matrix.


Arguments:       m        matrix to be set

                xT        x component of translation.

                yT        y component of translation.

                zT        z component of translation.

Return:         none

*---------------------------------------------------------------------*/
void MTXTrans ( Mtx m, f32 xT, f32 yT, f32 zT )
{

    ASSERTMSG( (m != 0), MTX_TRANS_1 );


    m[0][0] = 1.0f;     m[0][1] = 0.0f;  m[0][2] = 0.0f;  m[0][3] =  xT;
    m[1][0] = 0.0f;     m[1][1] = 1.0f;  m[1][2] = 0.0f;  m[1][3] =  yT;
    m[2][0] = 0.0f;     m[2][1] = 0.0f;  m[2][2] = 1.0f;  m[2][3] =  zT;

}

/*---------------------------------------------------------------------*

Name:            MTXScale

Description:     sets a scaling matrix.


Arguments:       m        matrix to be set

                xS        x scale factor.

                yS        y scale factor.

                zS        z scale factor.

Return:         none

*---------------------------------------------------------------------*/
void MTXScale ( Mtx m, f32 xS, f32 yS, f32 zS )
{

    ASSERTMSG( (m != 0), MTX_SCALE_1 );


    m[0][0] = xS;      m[0][1] = 0.0f;  m[0][2] = 0.0f;  m[0][3] = 0.0f;
    m[1][0] = 0.0f;    m[1][1] = yS;    m[1][2] = 0.0f;  m[1][3] = 0.0f;
    m[2][0] = 0.0f;    m[2][1] = 0.0f;  m[2][2] = zS;    m[2][3] = 0.0f;

}

/*---------------------------------------------------------------------*

Name:            MTXQuat

Description:     sets a rotation matrix from a quaternion.


Arguments:       m        matrix to be set

                 q        ptr to quaternion.

Return:          none

*---------------------------------------------------------------------*/
void MTXQuat ( Mtx m, QuaternionPtr q )
{

    f32 s,xs,ys,zs,wx,wy,wz,xx,xy,xz,yy,yz,zz;


    ASSERTMSG( (m != 0),                         MTX_QUAT_1     );
    ASSERTMSG( (q != 0),                         MTX_QUAT_2     );
    ASSERTMSG( ( q->x || q->y || q->z || q->w ), MTX_QUAT_3     );


    s = 2.0f / ( (q->x * q->x) + (q->y * q->y) + (q->z * q->z) + (q->w * q->w) );


    xs = q->x *  s;     ys = q->y *  s;  zs = q->z *  s;
    wx = q->w * xs;     wy = q->w * ys;  wz = q->w * zs;
    xx = q->x * xs;     xy = q->x * ys;  xz = q->x * zs;
    yy = q->y * ys;     yz = q->y * zs;  zz = q->z * zs;


    m[0][0] = 1.0f - (yy + zz);
    m[0][1] = xy   - wz;
    m[0][2] = xz   + wy;
    m[0][3] = 0.0f;

    m[1][0] = xy   + wz;
    m[1][1] = 1.0f - (xx + zz);
    m[1][2] = yz   - wx;
    m[1][3] = 0.0f;

    m[2][0] = xz   - wy;
    m[2][1] = yz   + wx;
    m[2][2] = 1.0f - (xx + yy);
    m[2][3] = 0.0f;

}

/*---------------------------------------------------------------------*

Name:           MTXReflect

Description:    reflect a rotation matrix with respect to a plane.


Arguments:      m        matrix to be set

                p        point on the planar reflector.

                n       normal of the planar reflector.

Return:         none

*---------------------------------------------------------------------*/
void MTXReflect ( Mtx m,  VecPtr p, VecPtr n )
{
    f32 vxy, vxz, vyz, pdotn;

    vxy   = -2.0f * n->x * n->y;
    vxz   = -2.0f * n->x * n->z;
    vyz   = -2.0f * n->y * n->z;
    pdotn = 2.0f * VECDotProduct(p, n);

    m[0][0] = 1.0f - 2.0f * n->x * n->x;
    m[0][1] = vxy;
    m[0][2] = vxz;
    m[0][3] = pdotn * n->x;

    m[1][0] = vxy;
    m[1][1] = 1.0f - 2.0f * n->y * n->y;
    m[1][2] = vyz;
    m[1][3] = pdotn * n->y;

    m[2][0] = vxz;
    m[2][1] = vyz;
    m[2][2] = 1.0f - 2.0f * n->z * n->z;
    m[2][3] = pdotn * n->z;
}


/*---------------------------------------------------------------------*





                             VIEW SECTION





*---------------------------------------------------------------------*/






/*---------------------------------------------------------------------*

Name:           MTXLookAt

Description:    compute a matrix to transform points to camera coordinates.


Arguments:      m        matrix to be set

                camPos   camera position.

                camUp    camera 'up' direction.

                target   camera aim point.


Return:         none

*---------------------------------------------------------------------*/
void MTXLookAt ( Mtx m, Point3dPtr camPos, VecPtr camUp, Point3dPtr target )
{

    Vec vLook,vRight,vUp;


    ASSERTMSG( (m != 0),      MTX_LOOKAT_1    );
    ASSERTMSG( (camPos != 0), MTX_LOOKAT_2    );
    ASSERTMSG( (camUp  != 0), MTX_LOOKAT_3    );
    ASSERTMSG( (target != 0), MTX_LOOKAT_4    );


    // compute unit target vector
    // use negative value to look down (-Z) axis
    vLook.x = camPos->x - target->x;
    vLook.y = camPos->y - target->y;
    vLook.z = camPos->z - target->z;
    VECNormalize( &vLook,&vLook );


    // vRight = camUp x vLook
    VECCrossProduct    ( camUp, &vLook, &vRight );
    VECNormalize( &vRight,&vRight );


    // vUp = vLook x vRight
    VECCrossProduct( &vLook, &vRight, &vUp );
    // Don't need to normalize vUp since it should already be unit length
    // VECNormalize( &vUp, &vUp );


    m[0][0] = vRight.x;
    m[0][1] = vRight.y;
    m[0][2] = vRight.z;
    m[0][3] = -( camPos->x * vRight.x + camPos->y * vRight.y + camPos->z * vRight.z );

    m[1][0] = vUp.x;
    m[1][1] = vUp.y;
    m[1][2] = vUp.z;
    m[1][3] = -( camPos->x * vUp.x + camPos->y * vUp.y + camPos->z * vUp.z );

    m[2][0] = vLook.x;
    m[2][1] = vLook.y;
    m[2][2] = vLook.z;
    m[2][3] = -( camPos->x * vLook.x + camPos->y * vLook.y + camPos->z * vLook.z );


}

/*---------------------------------------------------------------------*





                             VIEW SECTION





*---------------------------------------------------------------------*/







/*---------------------------------------------------------------------*

Name:           MTXFrustum

Description:    compute a 4x4 perspective projection matrix from a
                specified view volume.


Arguments:      m        4x4 matrix to be set

                t        top coord. of view volume at the near clipping plane

                b        bottom coord of view volume at the near clipping plane

                l        left coord. of view volume at near clipping plane

                r        right coord. of view volume at near clipping plane

                n        positive distance from camera to near clipping plane

                f        positive distance from camera to far clipping plane


Return:         none

*---------------------------------------------------------------------*/
void MTXFrustum ( Mtx44 m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f )
{
    f32 tmp;
    

    ASSERTMSG( (m != 0),  MTX_FRUSTUM_1     );
    ASSERTMSG( (t != b),  MTX_FRUSTUM_2     );
    ASSERTMSG( (l != r),  MTX_FRUSTUM_3     );
    ASSERTMSG( (n != f),  MTX_FRUSTUM_4     );


    // NOTE: Be careful about "l" vs. "1" below!!!

    tmp     =  1.0f / (r - l);
    m[0][0] =  (2*n) * tmp;
    m[0][1] =  0.0f;
    m[0][2] =  (r + l) * tmp;
    m[0][3] =  0.0f;

    tmp     =  1.0f / (t - b);
    m[1][0] =  0.0f;
    m[1][1] =  (2*n) * tmp;
    m[1][2] =  (t + b) * tmp;
    m[1][3] =  0.0f;

    m[2][0] =  0.0f;
    m[2][1] =  0.0f;

    tmp     =  1.0f / (f - n);
#ifdef EPPC
    // scale z to (-w, 0) range for Dolphin
    m[2][2] = -(n) * tmp;
    m[2][3] = -(f*n) * tmp;
#else
    // scale z to (-w, w) range
    m[2][2] = -(f + n) * tmp;
    m[2][3] = -(2*f*n) * tmp;
#endif

    m[3][0] =  0.0f;
    m[3][1] =  0.0f;
    m[3][2] = -1.0f;
    m[3][3] =  0.0f;


}

/*---------------------------------------------------------------------*

Name:           MTXPerspective

Description:    compute a 4x4 perspective projection matrix from
                field of view and aspect ratio.


Arguments:      m       4x4 matrix to be set

                fovy    total field of view in in degrees in the YZ plane

                aspect  ratio of view window width:height (X / Y)

                n       positive distance from camera to near clipping plane

                f       positive distance from camera to far clipping plane


Return:         none

*---------------------------------------------------------------------*/
void MTXPerspective    ( Mtx44 m, f32 fovY, f32 aspect, f32 n, f32 f )
{
    f32 angle;
    f32 cot;
    f32 tmp;
    

    ASSERTMSG( (m != 0),                             MTX_PERSPECTIVE_1    );
    ASSERTMSG( ( (fovY > 0.0) && ( fovY < 180.0) ),  MTX_PERSPECTIVE_2    );
    ASSERTMSG( (aspect != 0),                        MTX_PERSPECTIVE_3    );


    // find the cotangent of half the (YZ) field of view
    angle = fovY * 0.5f;
    angle = MTXDegToRad( angle );

    cot = 1.0f / tanf(angle);


    m[0][0] =  cot / aspect;
    m[0][1] =  0.0f;
    m[0][2] =  0.0f;
    m[0][3] =  0.0f;
              
    m[1][0] =  0.0f;
    m[1][1] =   cot;
    m[1][2] =  0.0f;
    m[1][3] =  0.0f;
              
    m[2][0] =  0.0f;
    m[2][1] =  0.0f;

    tmp     = 1.0f / (f - n);
#ifdef EPPC
    // scale z to (-w, 0) range for Dolphin
    m[2][2] = -(n) * tmp;
    m[2][3] = -(f*n) * tmp;
#else
    // scale z to (-w, w) range
    m[2][2] = -(f + n) * tmp;
    m[2][3] = -(2*f*n) * tmp;
#endif

    m[3][0] =  0.0f;
    m[3][1] =  0.0f;
    m[3][2] = -1.0f;
    m[3][3] =  0.0f;
}

/*---------------------------------------------------------------------*

Name:           MTXOrtho

Description:    compute a 4x4 orthographic projection matrix.


Arguments:      m        4x4 matrix to be set

                t        top coord. of parallel view volume

                b        bottom coord of parallel view volume

                l        left coord. of parallel view volume

                r        right coord. of parallel view volume

                n        positive distance from camera to near clipping plane

                f        positive distance from camera to far clipping plane


Return:         none

*---------------------------------------------------------------------*/
void MTXOrtho ( Mtx44 m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f )
{
    f32 tmp;


    ASSERTMSG( (m != 0),  MTX_ORTHO_1  );
    ASSERTMSG( (t != b),  MTX_ORTHO_2  );
    ASSERTMSG( (l != r),  MTX_ORTHO_3  );
    ASSERTMSG( (n != f),  MTX_ORTHO_4  );


    // NOTE: Be careful about "l" vs. "1" below!!!

    tmp     =  1.0f / (r - l);
    m[0][0] =  2.0f * tmp;
    m[0][1] =  0.0f;
    m[0][2] =  0.0f;
    m[0][3] = -(r + l) * tmp;

    tmp     =  1.0f / (t - b);
    m[1][0] =  0.0f;
    m[1][1] =  2.0f * tmp;
    m[1][2] =  0.0f;
    m[1][3] = -(t + b) * tmp;

    m[2][0] =  0.0f;
    m[2][1] =  0.0f;

    tmp     =  1.0f / (f - n);
#ifdef EPPC
    // scale z to (-1, 0) range for Dolphin
    m[2][2] = -(1.0f) * tmp;
    m[2][3] = -(f) * tmp;
#else
    // scale z to (-1, 1) range
    m[2][2] = -(2.0f) * tmp;
    m[2][3] = -(f + n) * tmp;
#endif
    m[3][0] =  0.0f;
    m[3][1] =  0.0f;
    m[3][2] =  0.0f;
    m[3][3] =  1.0f;
}

/*---------------------------------------------------------------------*





                             STACK SECTION





*---------------------------------------------------------------------*/






/*---------------------------------------------------------------------*

Name:           MTXInitStack

Description:    initializes a matrix stack size and stack ptr from
                a previously allocated stack.


Arguments:      sPtr      ptr to MtxStack structure to be initialized.

                numMtx    number of matrices in the stack.

                note:     the stack (array) memory must have been
                          previously allocated.
                          MtxVec.h provides a macro
                          ( MTXAllocStack( sPtr, numMtx ) )
                          to accomplish this using OSAlloc().



Return:         none.

*---------------------------------------------------------------------*/
void MTXInitStack( MtxStackPtr sPtr, u32 numMtx )
{


    ASSERTMSG( (sPtr != 0),              MTX_INITSTACK_1     );
    ASSERTMSG( (sPtr->stackBase != 0),   MTX_INITSTACK_2     );
    ASSERTMSG( (numMtx != 0),            MTX_INITSTACK_3     );


    sPtr->numMtx   = numMtx;
    sPtr->stackPtr = NULL;

}

/*---------------------------------------------------------------------*

Name:           MTXPush

Description:    copy a matrix to stack pointer + 1.
                increment stack pointer.


Arguments:      sPtr    ptr to MtxStack structure.

                m       matrix to copy into (stack pointer + 1) location.


Return:         stack pointer.

*---------------------------------------------------------------------*/
MtxPtr MTXPush ( MtxStackPtr sPtr, Mtx m )
{


    ASSERTMSG( (sPtr != 0),             MTX_PUSH_1    );
    ASSERTMSG( (sPtr->stackBase != 0),  MTX_PUSH_2    );
    ASSERTMSG( (m != 0),                MTX_PUSH_3    );


    if( sPtr->stackPtr == NULL )
    {
        sPtr->stackPtr = sPtr->stackBase;
        MTXCopy( m, sPtr->stackPtr );
    }

    else
    {
        // check for stack overflow
        if( (u32)((sPtr->stackPtr - sPtr->stackBase) / MTX_PTR_OFFSET) >=
            (sPtr->numMtx - 1) )
        {
            ASSERTMSG( 0,  MTX_PUSH_4  );
        }

    MTXCopy( m, (sPtr->stackPtr + MTX_PTR_OFFSET) );
    sPtr->stackPtr += MTX_PTR_OFFSET;
    }


    return sPtr->stackPtr;
}

/*---------------------------------------------------------------------*

Name:           MTXPushFwd

Description:    concatenate a matrix with the current top of the stack,
                increment the stack ptr and push the resultant matrix to
                the new top of stack.

                this is intended for use in building forward transformations,
                so concatenation is post-order
                ( top of stack x mtx ) = ( top of stack + 1 ).


Arguments:      sPtr    ptr to MtxStack structure.

                m        matrix to concatenate with stack ptr and
                         push to stack ptr + 1.


Return:         stack pointer.

*---------------------------------------------------------------------*/
MtxPtr MTXPushFwd ( MtxStackPtr sPtr, Mtx m )
{

    ASSERTMSG( (sPtr != 0),             MTX_PUSHFWD_1  );
    ASSERTMSG( (sPtr->stackBase != 0),  MTX_PUSHFWD_2  );
    ASSERTMSG( (m != 0),                MTX_PUSHFWD_3  );


    if( sPtr->stackPtr == NULL )
    {
        sPtr->stackPtr = sPtr->stackBase;
        MTXCopy( m, sPtr->stackPtr );
    }

    else
    {
        // check for stack overflow
        if( (u32)((sPtr->stackPtr - sPtr->stackBase) / MTX_PTR_OFFSET) >=
            (sPtr->numMtx - 1) )
        {
            ASSERTMSG( 0,  MTX_PUSHFWD_4  );
        }

        MTXConcat( sPtr->stackPtr, m, ( sPtr->stackPtr + MTX_PTR_OFFSET ) );
        sPtr->stackPtr += MTX_PTR_OFFSET;
    }


    return sPtr->stackPtr;
}

/*---------------------------------------------------------------------*

Name:           MTXPushInv

Description:    take a matrix, compute its inverse and concatenate that
                inverse with the current top of the stack,
                increment the stack ptr and push the resultant matrix to
                the new top of stack.

                this is intended for use in building inverse transformations,
                so concatenation is pre-order
                ( mtx x top of stack ) = ( top of stack + 1 ).


Arguments:      sPtr    ptr to MtxStack structure.

                m       matrix to concatenate with stack ptr and
                        push to stack ptr + 1.

                        m is not modified by this function.


Return:         stack pointer.

*---------------------------------------------------------------------*/
MtxPtr    MTXPushInv ( MtxStackPtr sPtr, Mtx m )
{

    Mtx mInv;


    ASSERTMSG( (sPtr != 0),             MTX_PUSHINV_1  );
    ASSERTMSG( (sPtr->stackBase != 0),  MTX_PUSHINV_2  );
    ASSERTMSG( (m != 0),                MTX_PUSHINV_3  );


    MTXInverse( m, mInv );


    if( sPtr->stackPtr == NULL )
    {
        sPtr->stackPtr = sPtr->stackBase;
        MTXCopy( mInv, sPtr->stackPtr );
    }

    else
    {
        // check for stack overflow
        if( (u32)((sPtr->stackPtr - sPtr->stackBase) / MTX_PTR_OFFSET) >=
            (sPtr->numMtx - 1) )
        {
            ASSERTMSG( 0,  MTX_PUSHINV_4  );
        }

        MTXConcat( mInv, sPtr->stackPtr, ( sPtr->stackPtr + MTX_PTR_OFFSET ) );
        sPtr->stackPtr += MTX_PTR_OFFSET;
    }


    return sPtr->stackPtr;
}

/*---------------------------------------------------------------------*

Name:           MTXPushInvXpose

Description:    take a matrix, compute its inverse-transpose and concatenate it
                with the current top of the stack,
                increment the stack ptr and push the resultant matrix to
                the new top of stack.

                this is intended for use in building an inverse-transpose
                matrix for forward transformations of normals, so
                concatenation is post-order.
                ( top of stack x mtx ) = ( top of stack + 1 ).


Arguments:      sPtr   ptr to MtxStack structure.

                m      matrix to concatenate with stack ptr and 
                       push to stack ptr + 1.

                       m is not modified by this function.


Return:         stack pointer.

*---------------------------------------------------------------------*/
MtxPtr MTXPushInvXpose ( MtxStackPtr sPtr, Mtx m )
{

    Mtx mIT;


    ASSERTMSG( (sPtr != 0),             MTX_PUSHINVXPOSE_1    );
    ASSERTMSG( (sPtr->stackBase != 0),  MTX_PUSHINVXPOSE_2    );
    ASSERTMSG( (m != 0),                MTX_PUSHINVXPOSE_3    );


    MTXInverse(     m, mIT );
    MTXTranspose( mIT, mIT );


    if( sPtr->stackPtr == NULL )
    {
        sPtr->stackPtr = sPtr->stackBase;
        MTXCopy( mIT, sPtr->stackPtr );
    }

    else
    {
        // check for stack overflow
        if( (u32)((sPtr->stackPtr - sPtr->stackBase) / MTX_PTR_OFFSET) >=
            (sPtr->numMtx - 1) )
        {
            ASSERTMSG( 0,  MTX_PUSHINVXPOSE_4  );
        }

        MTXConcat( sPtr->stackPtr, mIT, ( sPtr->stackPtr + MTX_PTR_OFFSET ) );
        sPtr->stackPtr += MTX_PTR_OFFSET;
    }


    return sPtr->stackPtr;
}

/*---------------------------------------------------------------------*

Name:           MTXPop

Description:    decrement the stack pointer


Arguments:      sPtr        pointer to stack structure


Return:         stack pointer.

*---------------------------------------------------------------------*/
MtxPtr MTXPop ( MtxStackPtr sPtr )
{


    ASSERTMSG( (sPtr != 0),               MTX_POP_1  );
    ASSERTMSG( (sPtr->stackBase != 0),    MTX_POP_2  );


    if( sPtr->stackPtr == NULL )
    {
        return NULL;
    }

    else if( sPtr->stackBase == sPtr->stackPtr )
    {
        sPtr->stackPtr = NULL;
        return NULL;
    }
    else
    {
        sPtr->stackPtr -= MTX_PTR_OFFSET;
        return sPtr->stackPtr;
    }

}

/*---------------------------------------------------------------------*

Name:           MTXGetStackPtr

Description:    return the stack pointer


Arguments:      sPtr pointer to stack structure


Return:         stack pointer.

*---------------------------------------------------------------------*/
MtxPtr MTXGetStackPtr( MtxStackPtr sPtr )
{

    ASSERTMSG( (sPtr != 0),               MTX_GETSTACKPTR_1  );
    ASSERTMSG( (sPtr->stackBase != 0),    MTX_GETSTACKPTR_2  );

    return sPtr->stackPtr;

}

/*---------------------------------------------------------------------*





                             VECTOR SECTION





*---------------------------------------------------------------------*/







/*---------------------------------------------------------------------*

Name:           VECAdd

Description:    add two vectors.


Arguments:      a    first vector.

                b    second vector.

                ab   resultant vector (a + b).
                     ok if ab == a or ab == b.


Return:         none.

*---------------------------------------------------------------------*/
void VECAdd ( VecPtr a, VecPtr b, VecPtr ab )
{

    ASSERTMSG( ( a    != 0), VEC_ADD_1 );
    ASSERTMSG( ( b    != 0), VEC_ADD_2 );
    ASSERTMSG( ( ab != 0),   VEC_ADD_3 );


    ab->x = a->x + b->x;
    ab->y = a->y + b->y;
    ab->z = a->z + b->z;

}

/*---------------------------------------------------------------------*

Name:           VECSubtract

Description:    subtract one vector from another.


Arguments:      a       first vector.

                b       second vector.

                a_b     resultant vector (a - b).
                        ok if a_b == a or a_b == b.


Return:         none.

*---------------------------------------------------------------------*/
void VECSubtract ( VecPtr a, VecPtr b, VecPtr a_b )
{

    ASSERTMSG( ( a    != 0),    VEC_SUBTRACT_1     );
    ASSERTMSG( ( b    != 0),    VEC_SUBTRACT_2     );
    ASSERTMSG( ( a_b != 0),     VEC_SUBTRACT_3     );


    a_b->x = a->x - b->x;
    a_b->y = a->y - b->y;
    a_b->z = a->z - b->z;

}

/*---------------------------------------------------------------------*

Name:           VECScale

Description:    multiply a vector by a scalar.


Arguments:      src     unscaled source vector.

                dst     scaled resultant vector ( src * scale).
                        ok if dst == src.

                scale   scaling factor.


Return:         none.

*---------------------------------------------------------------------*/
void VECScale ( VecPtr src, VecPtr dst, f32 scale )
{

    ASSERTMSG( ( src  != 0),  VEC_SCALE_1  );
    ASSERTMSG( ( dst  != 0),  VEC_SCALE_2  );


    dst->x = src->x * scale;
    dst->y = src->y * scale;
    dst->z = src->z * scale;

}

/*---------------------------------------------------------------------*

Name:           VECNormalize

Description:    normalize a vector.


Arguments:      src     non-unit source vector.

                unit    resultant unit vector ( src / src magnitude ).
                        ok if unit == src


Return:         none.

*---------------------------------------------------------------------*/
void VECNormalize ( VecPtr src, VecPtr unit )
{
    f32 mag;


    ASSERTMSG( (src != 0 ),     VEC_NORMALIZE_1  );
    ASSERTMSG( (unit != 0),     VEC_NORMALIZE_2  );


    mag = (src->x * src->x) + (src->y * src->y) + (src->z * src->z);

    ASSERTMSG( (mag != 0),      VEC_NORMALIZE_3  );

    mag = 1.0f / sqrtf(mag);

    unit->x = src->x * mag;
    unit->y = src->y * mag;
    unit->z = src->z * mag;

}

/*---------------------------------------------------------------------*

Name:           VECSquareMag

Description:    compute the square of the magnitude of a vector.


Arguments:      v    source vector.


Return:         square magnitude of the vector.

*---------------------------------------------------------------------*/
f32 VECSquareMag ( VecPtr v )
{
    f32 sqmag;

    ASSERTMSG( (v != 0),  VEC_MAG_1 );

    sqmag = (v->x * v->x) + (v->y * v->y) + (v->z * v->z);

    return sqmag;
}

/*---------------------------------------------------------------------*

Name:           VECMag

Description:    compute the magnitude of a vector.


Arguments:      v    source vector.


Return:         magnitude of the vector.

*---------------------------------------------------------------------*/
f32 VECMag ( VecPtr v )
{
    return sqrtf( VECSquareMag(v) );
}

/*---------------------------------------------------------------------*

Name:           VECReflect

Description:    reflect a vector about a normal to a surface.


Arguments:      src        incident vector.

                normal     normal to surface.

                dst        normalized reflected vector.
                           ok if dst == src


Return:         none.

*---------------------------------------------------------------------*/
void VECReflect ( VecPtr src, VecPtr normal, VecPtr dst )
{
    f32 cosA;
    Vec uI, uN;


    ASSERTMSG( (src != 0),     VEC_REFLECT_1  );
    ASSERTMSG( (normal != 0),  VEC_REFLECT_2  );
    ASSERTMSG( (dst != 0),     VEC_REFLECT_3  );


    // assume src is incident to a surface.
    // reverse direction of src so that src and normal
    // sit tail to tail.
    uI.x = -( src->x );
    uI.y = -( src->y );
    uI.z = -( src->z );


    // VECNormalize will catch any zero magnitude vectors
    VECNormalize( &uI,    &uI);
    VECNormalize( normal, &uN);

    // angle between the unit vectors
    cosA = VECDotProduct( &uI, &uN);


    // R = 2N(N.I) - I
    dst->x = (2.0f * uN.x * cosA) - uI.x;
    dst->y = (2.0f * uN.y * cosA) - uI.y;
    dst->z = (2.0f * uN.z * cosA) - uI.z;

    VECNormalize( dst, dst );

}

/*---------------------------------------------------------------------*

Name:           VECDotProduct

Description:    compute the dot product of two vectors.


Arguments:      a    first vector.

                b    second vector.

                note:  input vectors do not have to be normalized.
                       input vectors are not normalized within the function.

                       if direct cosine computation of the angle
                       between a and b is desired, a and b should be
                       normalized prior to calling VECDotProduct.


Return:         dot product value.

*---------------------------------------------------------------------*/
f32 VECDotProduct ( VecPtr a, VecPtr b )
{
    f32 dot;


    ASSERTMSG( (a != 0), VEC_DOTPRODUCT_1 );
    ASSERTMSG( (b != 0), VEC_DOTPRODUCT_2 );


    dot = (a->x * b->x) + (a->y * b->y) + (a->z * b->z);

    return dot;
}

/*---------------------------------------------------------------------*

Name:           VECCrossProduct

Description:    compute the cross product of two vectors.


Arguments:      a       first vector.

                b       second vector.

                note:  input vectors do not have to be normalized.


                axb     resultant vector.
                        ok if axb == a or axb == b.


Return:         none.

*---------------------------------------------------------------------*/
void VECCrossProduct ( VecPtr a, VecPtr b, VecPtr axb )
{
    Vec vTmp;


    ASSERTMSG( (a    != 0),   VEC_CROSSPRODUCT_1    );
    ASSERTMSG( (b    != 0),   VEC_CROSSPRODUCT_2    );
    ASSERTMSG( (axb != 0),    VEC_CROSSPRODUCT_3    );


    vTmp.x =  ( a->y * b->z ) - ( a->z * b->y );
    vTmp.y =  ( a->z * b->x ) - ( a->x * b->z );
    vTmp.z =  ( a->x * b->y ) - ( a->y * b->x );


    axb->x = vTmp.x;
    axb->y = vTmp.y;
    axb->z = vTmp.z;

}

/*---------------------------------------------------------------------*

Name:           VECHalfAngle

Description:    compute the vector halfway between two vectors.
                intended for use in computing specular highlights


Arguments:      a     first vector.
                      this must point FROM the light source (tail) 
                      TO the surface (head).

                b     second vector.
                      this must point FROM the viewer (tail)
                      TO the surface (head).

                note:     input vectors do not have to be normalized.


                half  resultant normalized 'half-angle' vector.
                      ok if half == a or half == b


Return:         none.

*---------------------------------------------------------------------*/
void VECHalfAngle ( VecPtr a, VecPtr b, VecPtr half )
{
    Vec aTmp, bTmp, hTmp;


    ASSERTMSG( (a    != 0),    VEC_HALFANGLE_1  );
    ASSERTMSG( (b    != 0),    VEC_HALFANGLE_2  );
    ASSERTMSG( (half != 0),    VEC_HALFANGLE_3  );


    aTmp.x = -a->x;
    aTmp.y = -a->y;
    aTmp.z = -a->z;

    bTmp.x = -b->x;
    bTmp.y = -b->y;
    bTmp.z = -b->z;

    VECNormalize( &aTmp, &aTmp );
    VECNormalize( &bTmp, &bTmp );

    VECAdd( &aTmp, &bTmp, &hTmp );
    
    if ( VECDotProduct( &hTmp, &hTmp ) > 0.0F )
    {
        VECNormalize( &hTmp, half );
    }
    else    // The singular case returns zero vector
    {
        *half = hTmp;
    }

}

/*---------------------------------------------------------------------*

Name:           VECSquareDistance

Description:    Returns the square of the distance between vectors
                a and b.  Distance can be calculated using the
                square root of the returned value.


Arguments:      a     first vector.

                b     second vector.


Return:         square distance of between vectors.

*---------------------------------------------------------------------*/
f32 VECSquareDistance( VecPtr a, VecPtr b )
{
    Vec diff;

    diff.x = a->x - b->x;
    diff.y = a->y - b->y;
    diff.z = a->z - b->z;

    return (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
}

/*---------------------------------------------------------------------*

Name:           VECDistance

Description:    Returns the distance between vectors a and b.


Arguments:      a     first vector.

                b     second vector.


Return:         distance between the two vectors.

*---------------------------------------------------------------------*/
f32 VECDistance( VecPtr a, VecPtr b )
{
    return sqrtf( VECSquareDistance( a, b ) );
}


/*---------------------------------------------------------------------*





                       TEXTURE PROJECTION SECTION





*---------------------------------------------------------------------*/


/*---------------------------------------------------------------------*

Name:           MTXLightFrustum

Description:    Compute a 3x4 projection matrix for texture projection


Arguments:      m        3x4 matrix to be set

                t        top coord. of view volume at the near clipping plane

                b        bottom coord of view volume at the near clipping plane

                l        left coord. of view volume at near clipping plane

                r        right coord. of view volume at near clipping plane

                n        positive distance from camera to near clipping plane

                scaleS   scale in the S direction for projected coordinates
                         (usually 0.5)

                scaleT   scale in the T direction for projected coordinates
                         (usually 0.5)

                transS   translate in the S direction for projected coordinates
                         (usually 0.5)

                transT   translate in the T direction for projected coordinates
                         (usually 0.5)


Return:         none.

*---------------------------------------------------------------------*/
void MTXLightFrustum    ( Mtx m, float t, float b, float l, float r, float n,
                          float scaleS, float scaleT, float transS,
                          float transT )
{
    f32 tmp;
    

    ASSERTMSG( (m != 0),  MTX_LIGHT_FRUSTUM_1  );
    ASSERTMSG( (t != b),  MTX_LIGHT_FRUSTUM_2  );
    ASSERTMSG( (l != r),  MTX_LIGHT_FRUSTUM_3  );

    
    // NOTE: Be careful about "l" vs. "1" below!!!

    tmp     =  1.0f / (r - l);
    m[0][0] =  ((2*n) * tmp) * scaleS;
    m[0][1] =  0.0f;
    m[0][2] =  (((r + l) * tmp) * scaleS) - transS;
    m[0][3] =  0.0f;

    tmp     =  1.0f / (t - b);
    m[1][0] =  0.0f;
    m[1][1] =  ((2*n) * tmp) * scaleT;
    m[1][2] =  (((t + b) * tmp) * scaleT) - transT;
    m[1][3] =  0.0f;

    m[2][0] =  0.0f;
    m[2][1] =  0.0f;
    m[2][2] = -1.0f;
    m[2][3] =  0.0f;
}

/*---------------------------------------------------------------------*

Name:           MTXLightPerspective

Description:    compute a 3x4 perspective projection matrix from
                field of view and aspect ratio for texture projection.


Arguments:      m        3x4 matrix to be set

                fovy     total field of view in in degrees in the YZ plane

                aspect   ratio of view window width:height (X / Y)

                scaleS   scale in the S direction for projected coordinates
                         (usually 0.5)

                scaleT   scale in the T direction for projected coordinates
                         (usually 0.5)

                transS   translate in the S direction for projected coordinates
                         (usually 0.5)

                transT   translate in the T direction for projected coordinates
                         (usually 0.5)


Return:         none

*---------------------------------------------------------------------*/
void MTXLightPerspective    ( Mtx m, f32 fovY, f32 aspect, float scaleS,
                              float scaleT, float transS, float transT )
{
    f32 angle;
    f32 cot;

    ASSERTMSG( (m != 0),                            MTX_LIGHT_PERSPECTIVE_1  );
    ASSERTMSG( ( (fovY > 0.0) && ( fovY < 180.0) ), MTX_LIGHT_PERSPECTIVE_2  );
    ASSERTMSG( (aspect != 0),                       MTX_LIGHT_PERSPECTIVE_3  );

    // find the cotangent of half the (YZ) field of view
    angle = fovY * 0.5f;
    angle = MTXDegToRad( angle );

    cot = 1.0f / tanf(angle);

    m[0][0] =    (cot / aspect) * scaleS;
    m[0][1] =    0.0f;
    m[0][2] =    -transS;
    m[0][3] =    0.0f;

    m[1][0] =    0.0f;
    m[1][1] =    cot * scaleT;
    m[1][2] =    -transT;
    m[1][3] =    0.0f;

    m[2][0] =    0.0f;
    m[2][1] =    0.0f;
    m[2][2] =   -1.0f;
    m[2][3] =    0.0f;
}

/*---------------------------------------------------------------------*

Name:           MTXLightOrtho

Description:    compute a 3x4 orthographic projection matrix.


Arguments:      m        matrix to be set

                t        top coord. of parallel view volume

                b        bottom coord of parallel view volume

                l        left coord. of parallel view volume

                r        right coord. of parallel view volume

                scaleS   scale in the S direction for projected coordinates
                         (usually 0.5)

                scaleT   scale in the T direction for projected coordinates
                         (usually 0.5)

                transS   translate in the S direction for projected coordinates
                         (usually 0.5)

                transT   translate in the T direction for projected coordinates
                         (usually 0.5)


Return:         none

*---------------------------------------------------------------------*/
void MTXLightOrtho ( Mtx m, f32 t, f32 b, f32 l, f32 r, float scaleS,
                              float scaleT, float transS, float transT )
{
    f32 tmp;
    

    ASSERTMSG( (m != 0),  MTX_LIGHT_ORTHO_1     );
    ASSERTMSG( (t != b),  MTX_LIGHT_ORTHO_2     );
    ASSERTMSG( (l != r),  MTX_LIGHT_ORTHO_3     );


    // NOTE: Be careful about "l" vs. "1" below!!!

    tmp     =  1.0f / (r - l);
    m[0][0] =  (2.0f * tmp * scaleS);
    m[0][1] =  0.0f;
    m[0][2] =  0.0f;
    m[0][3] =  ((-(r + l) * tmp) * scaleS) + transS;

    tmp     =  1.0f / (t - b);
    m[1][0] =  0.0f;
    m[1][1] =  (2.0f * tmp) * scaleT;
    m[1][2] =  0.0f;
    m[1][3] =  ((-(t + b) * tmp)* scaleT) + transT;

    m[2][0] =  0.0f;
    m[2][1] =  0.0f;
    m[2][2] =  0.0f;
    m[2][3] =  1.0f;
}

/*===========================================================================*/


u32 MTXInvXpose ( MtxPtr ptr, Mtx m ) {
    Mtx mIT;


    ASSERTMSG( (ptr != 0),              MTX_INVXPOSE_1    );
    ASSERTMSG( (m != 0),                MTX_INVXPOSE_3    );


    MTXInverse(     m, mIT );
    MTXTranspose( mIT, mIT );
    MTXCopy( mIT, ptr );
    return 1;
}
