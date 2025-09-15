/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3Math.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/include/charPipeline/c3/C3Math.h $
    
    7     7/24/00 12:16p John
    Moved EPSILON and COS_EPSILON out of header file and into C3Math.c
    
    6     7/18/00 7:43p John
    Added C3MtxToQuat again.
    
    5     7/18/00 7:37p John
    Changed QUAT prefix to C3Quat.
    Removed C3Mtx code since MTX library is included in build.
    
    4     4/06/00 3:22p John
    Added C3MtxRowCol to access matrices using this macro from MTX library.
    
    3     2/29/00 7:18p John
    Added C3QuatNormalize.
    
    2     2/04/00 6:06p John
    Untabified code.
    
    2     11/18/99 1:12p John
    
  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _C3MATH_H
#define _C3MATH_H

#if 0
extern "C" {
#endif // __cplusplus

#define C3_FLT_EPSILON        1.192092896e-06F 
#define C3_FLOAT_EQUAL(a,b)   ( fabs(a-b) <= C3_FLT_EPSILON )

void       C3MtxToQuat        ( Mtx m, Quaternion *dst );
void       C3QuatNormalize    ( Quaternion *src, Quaternion *unit );

f32        C3QuatDot          ( Quaternion p, Quaternion q);
Quaternion C3QuatInverse      ( Quaternion q );
Quaternion C3QuatMultiply     ( Quaternion p, Quaternion q );
Quaternion C3QuatDivide       ( Quaternion p, Quaternion q );
Quaternion C3QuatLogN         ( Quaternion q );
Quaternion C3QuatLnDif        ( Quaternion p, Quaternion q );
Quaternion C3QuatScale        ( Quaternion q, f32 scale );
Quaternion C3QuatAdd          ( Quaternion p, Quaternion q );
Quaternion C3QuatMinus        ( Quaternion p, Quaternion q );
Quaternion C3QuatExp          ( Quaternion q );
void       C3QuatMakeClosest  ( Quaternion *q, Quaternion qto );
Quaternion C3QuatCompA        ( Quaternion qprev, Quaternion q, Quaternion qnext);

#if 0
}
#endif // __cplusplus

#endif // _C3MATH_H
