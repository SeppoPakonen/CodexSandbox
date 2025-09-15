/*---------------------------------------------------------------------------*
  Project: Matrix Vector Library
  File:    mtx.h

  Copyright 1998 - 2001 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.


  $Log: /Dolphin/include/dolphin/mtx.h $
    
    17    8/27/01 3:54p Hirose
    Added some PSQUAT functions.
    
    16    8/03/01 1:19a Hirose
    Added quaternion functions.
    
    15    7/30/01 10:17p Hirose
    Changes for function definitions.
    
    14    7/24/01 6:03p Hirose
    
    13    7/23/01 8:44p Hirose
    Added some more PS functions again.
    
    12    7/09/01 11:15p Hirose
    added general 4x4 matrix functions.
    
    11    7/07/01 7:28p Hirose
    added some more PS matrix functions.
    
    10    3/29/01 3:06p Hirose
    added MTXInvXpose
    
    9     3/16/01 11:44p Hirose
    added PSMTXInverse
    
    8     2/22/01 11:45p Hirose
    Added some more PS functions. Updated function binding rule.
    
    7     7/12/00 4:40p John
    Substitues MTXConcat and MTXMultVecArray with their paired-singles
    equivalent for Gekko nondebug builds.
    
    6     5/11/00 2:15p Hirose
    used macros instead of inline functions in order to avoid error on VC++
    build
    
    5     5/10/00 1:53p Hirose
    added radian-based rotation matrix functions
    
    4     4/10/00 11:56a Danm
    Added 2 matrix skinning support.
    
    3     3/22/00 2:01p John
    Added VECSquareDistance and VECDistance.
    
    2     1/27/00 4:54p Tian
    Detabbed.  Added optimized paired-singles matrix ops.
    
    17    11/10/99 4:39p Alligator
    added MTXReflect
    
    16    10/06/99 5:14p Yasu
    Change char* to void*
    
    15    10/06/99 12:27p Yasu
    Appended MTXMultVecSR and MTXMultVecArraySR
    
    14    9/15/99 5:45p Mikepc
    changed MtxStack member declaration from 'int numMtx' to 'u32 numMtx'
    
    13    7/28/99 3:38p Ryan
    
    12    7/28/99 11:30a Ryan
    Added Texture Projection functions
    
    11    7/02/99 12:55p Mikepc
    changed Mtx row/col
    
    10    6/29/99 10:59p Martho01
    Changed MTXRowCol() macro argument names to r, c. Added comment.
    
    9     6/29/99 10:25p Martho01
    Added 'old-style' MTXRowCol macro for referencing matrix elements.
    'new-style' will come soon.
    
    8     6/24/99 12:51p Mikepc
    added $log keyword.
    changed file name from MTxVec.h to mtx.h in header section.
    changed __MTXVEC_H__ to __MTX_H__ for #ifndef.
  
  
  
  Change History:
  
  [05/28/1999]  [Mike Ockenden] [ changed 'Point' and 'PointPtr' to 'Point3d' , Point3dPtr'
                                  to avoid conflict with MacOS ]
  
  [05/27/1999]	[Mike Ockenden]	[ 1st check-in to source safe.
                                  changed file name from MtxVec.h to mtx.h ]
    
  $NoKeywords: $
  
  
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
    Matrix-Vector Library
 *---------------------------------------------------------------------------*/

#ifndef __MTX_H__
#define __MTX_H__



#include "os.h"
#include "GeoTypes.h"


#if 0
extern "C" {
#endif

/*---------------------------------------------------------------------------*
    Default function binding configuration
 *---------------------------------------------------------------------------*/
// [Binding Rule]
//
// "MTX_USE_PS" -> When this flag is specified, it uses PS* (Paired-Single
//                 assembler code) functions for non-prefixed function calls.
// "MTX_USE_C " -> When this flag is specified, it uses C_* (C code) functions
//                 for non-prefixed function calls.
//
// If both are specified, it will be treated as MTX_USE_PS.
// If nothing is specified, NDEBUG build refers PS* functions and
// DEBUG build uses C_* functions.

// For non-Gekko HW (e.g. emulator)
#ifndef GEKKO
#define MTX_USE_C
#undef  MTX_USE_PS
#endif

#if ( !defined(MTX_USE_PS) && !defined(MTX_USE_C) )
#ifndef _DEBUG
#define MTX_USE_PS
#endif
#endif

/*---------------------------------------------------------------------------*
    Macro definitions
 *---------------------------------------------------------------------------*/

// MtxPtr offset to access next Mtx of an array
#define MTX_PTR_OFFSET      3

// Mtx44Ptr offset to access next Mtx44 of an array
#define MTX44_PTR_OFFSET    4   


// matrix stack
typedef struct				    
{            

    u32    numMtx;
    MtxPtr stackBase;
    MtxPtr stackPtr;
	
} MtxStack, *MtxStackPtr;

 
// degree <--> radian conversion macros
#define MTXDegToRad(a)   ( (a) *  0.01745329252f )
#define MTXRadToDeg(a)   ( (a) * 57.29577951f )


// Matrix-element-referencing macro.
// Insulates user from changes from row-major to column-major and vice-versa.
// Fully documents which index is row, which index is column.
// XXX this version will change once matrices are transposed.

#define MTXRowCol(m,r,c) ((m)[(r)][(c)])


/*---------------------------------------------------------------------------*
    GENERAL MATRIX SECTION
 *---------------------------------------------------------------------------*/
// C version
void    C_MTXIdentity           ( Mtx m );
void    C_MTXCopy               ( MtxPtr src, MtxPtr dst );
void    C_MTXConcat             ( Mtx a, Mtx b, Mtx ab );
void    C_MTXTranspose          ( Mtx src, Mtx xPose );
u32     C_MTXInverse            ( Mtx src, Mtx inv );
u32     C_MTXInvXpose           ( Mtx src, Mtx invX );

// PS assembler version
#ifdef GEKKO
void    PSMTXIdentity           ( Mtx m );
void    PSMTXCopy               ( Mtx src, Mtx dst );
void    PSMTXConcat             ( Mtx a, Mtx b, Mtx ab );
void    PSMTXTranspose          ( Mtx src, Mtx xPose );
u32     PSMTXInverse            ( Mtx src, Mtx inv );
u32     PSMTXInvXpose           ( Mtx src, Mtx invX );
#endif

// Bindings
#ifdef MTX_USE_PS
#define MTXIdentity             PSMTXIdentity
#define MTXCopy                 PSMTXCopy
#define MTXConcat               PSMTXConcat
#define MTXTranspose            PSMTXTranspose
#define MTXInverse              PSMTXInverse
#define MTXInvXpose             PSMTXInvXpose
#else // MTX_USE_C
#define MTXIdentity             C_MTXIdentity
#define MTXCopy                 C_MTXCopy
#define MTXConcat               C_MTXConcat
#define MTXTranspose            C_MTXTranspose
#define MTXInverse              C_MTXInverse
#define MTXInvXpose             C_MTXInvXpose
#endif

/*---------------------------------------------------------------------------*
    MATRIX-VECTOR SECTION
 *---------------------------------------------------------------------------*/
// C version
void    C_MTXMultVec            ( MtxPtr m, VecPtr src, VecPtr dst );
void    C_MTXMultVecArray       ( MtxPtr m, VecPtr srcBase, VecPtr dstBase, u32 count );
void    C_MTXMultVecSR          ( MtxPtr m, VecPtr src, VecPtr dst );
void    C_MTXMultVecArraySR     ( MtxPtr m, VecPtr srcBase, VecPtr dstBase, u32 count );

// PS assembler version
#ifdef GEKKO
void    PSMTXMultVec            ( Mtx m, VecPtr src, VecPtr dst );
void    PSMTXMultVecArray       ( Mtx m, VecPtr srcBase, VecPtr dstBase, u32 count );
void    PSMTXMultVecSR          ( Mtx m, VecPtr src, VecPtr dst );
void    PSMTXMultVecArraySR     ( Mtx m, VecPtr srcBase, VecPtr dstBase, u32 count );
#endif


void    MTXMultVec            ( Mtx m, VecPtr src, VecPtr dst );
void    MTXMultVecArray       ( Mtx m, VecPtr srcBase, VecPtr dstBase, u32 count );
void    MTXMultVecSR          ( Mtx m, VecPtr src, VecPtr dst );
void    MTXMultVecArraySR     ( Mtx m, VecPtr srcBase, VecPtr dstBase, u32 count );


// Bindings
/*#ifdef MTX_USE_PS
#define MTXMultVec              PSMTXMultVec
#define MTXMultVecArray         PSMTXMultVecArray
#define MTXMultVecSR            PSMTXMultVecSR
#define MTXMultVecArraySR       PSMTXMultVecArraySR
#else // MTX_USE_C
#define MTXMultVec              C_MTXMultVec
#define MTXMultVecArray         C_MTXMultVecArray
#define MTXMultVecSR            C_MTXMultVecSR
#define MTXMultVecArraySR       C_MTXMultVecArraySR
#endif*/


/*---------------------------------------------------------------------------*
    MODEL MATRIX SECTION
 *---------------------------------------------------------------------------*/
// C version
void    C_MTXQuat             ( Mtx m, QuaternionPtr q );
void    C_MTXReflect          ( Mtx m, VecPtr p, VecPtr n );

void    C_MTXTrans            ( Mtx m, f32 xT, f32 yT, f32 zT );
void    C_MTXTransApply       ( Mtx src, Mtx dst, f32 xT, f32 yT, f32 zT );
void    C_MTXScale            ( Mtx m, f32 xS, f32 yS, f32 zS );
void    C_MTXScaleApply       ( Mtx src, Mtx dst, f32 xS, f32 yS, f32 zS );

void    C_MTXRotRad           ( Mtx m, char axis, f32 rad );
void    C_MTXRotTrig          ( Mtx m, char axis, f32 sinA, f32 cosA );
void    C_MTXRotAxisRad       ( Mtx m, VecPtr axis, f32 rad );

// PS assembler version
#ifdef GEKKO
void    PSMTXQuat             ( Mtx m, QuaternionPtr q );
void    PSMTXReflect          ( Mtx m, VecPtr p, VecPtr n );

void    PSMTXTrans            ( Mtx m, f32 xT, f32 yT, f32 zT );
void    PSMTXTransApply       ( Mtx src, Mtx dst, f32 xT, f32 yT, f32 zT );
void    PSMTXScale            ( Mtx m, f32 xS, f32 yS, f32 zS );
void    PSMTXScaleApply       ( Mtx src, Mtx dst, f32 xS, f32 yS, f32 zS );

void    PSMTXRotRad           ( Mtx m, char axis, f32 rad );
void    PSMTXRotTrig          ( Mtx m, char axis, f32 sinA, f32 cosA );
void    PSMTXRotAxisRad       ( Mtx m, VecPtr axis, f32 rad );
#endif

// Bindings
#ifdef MTX_USE_PS
#define MTXQuat                 PSMTXQuat
#define MTXReflect              PSMTXReflect
#define MTXTrans                PSMTXTrans
#define MTXTransApply           PSMTXTransApply
#define MTXScale                PSMTXScale
#define MTXScaleApply           PSMTXScaleApply
#define MTXRotRad               PSMTXRotRad
#define MTXRotTrig              PSMTXRotTrig
#define MTXRotAxisRad           PSMTXRotAxisRad

#define MTXRotDeg( m, axis, deg ) \
    PSMTXRotRad( m, axis, MTXDegToRad(deg) )
#define MTXRotAxisDeg( m, axis, deg ) \
    PSMTXRotAxisRad( m, axis, MTXDegToRad(deg) )

#else // MTX_USE_C
#define MTXQuat                 C_MTXQuat
#define MTXReflect              C_MTXReflect
#define MTXTrans                C_MTXTrans
#define MTXTransApply           C_MTXTransApply
#define MTXScale                C_MTXScale
#define MTXScaleApply           C_MTXScaleApply
#define MTXRotRad               C_MTXRotRad
#define MTXRotTrig              C_MTXRotTrig
#define MTXRotAxisRad           C_MTXRotAxisRad

#define MTXRotDeg( m, axis, deg ) \
    C_MTXRotRad( m, axis, MTXDegToRad(deg) )
#define MTXRotAxisDeg( m, axis, deg ) \
    C_MTXRotAxisRad( m, axis, MTXDegToRad(deg) )

#endif


// Obsoleted. Don't use this if possible.
#define MTXRotAxis              MTXRotAxisDeg


/*---------------------------------------------------------------------------*
    VIEW MATRIX SECTION
 *---------------------------------------------------------------------------*/
// C version only so far
void    C_MTXLookAt         ( Mtx           m, 
                              Point3dPtr    camPos, 
                              VecPtr        camUp, 
                              Point3dPtr    target );

// Bindings
#define MTXLookAt               C_MTXLookAt


/*---------------------------------------------------------------------------*
    PROJECTION MATRIX SECTION
 *---------------------------------------------------------------------------*/
// C version only so far
void    C_MTXFrustum        ( Mtx44 m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f );
void    C_MTXPerspective    ( Mtx44 m, f32 fovY, f32 aspect, f32 n, f32 f );
void    C_MTXOrtho          ( Mtx44 m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f );

// Bindings
#define MTXFrustum              C_MTXFrustum
#define MTXPerspective          C_MTXPerspective
#define MTXOrtho                C_MTXOrtho


/*---------------------------------------------------------------------------*
    TEXTURE PROJECTION MATRIX SECTION
 *---------------------------------------------------------------------------*/
// C version only so far
void    C_MTXLightFrustum       ( Mtx m, f32 t, f32 b, f32 l, f32 r, f32 n, 
                                  f32 scaleS, f32 scaleT, f32 transS, 
                                  f32 transT );

void    C_MTXLightPerspective   ( Mtx m, f32 fovY, f32 aspect, f32 scaleS, 
                                  f32 scaleT, f32 transS, f32 transT );

void    C_MTXLightOrtho         ( Mtx m, f32 t, f32 b, f32 l, f32 r, f32 scaleS, 
                                  f32 scaleT, f32 transS, f32 transT );

// Bindings
#define MTXLightFrustum         C_MTXLightFrustum
#define MTXLightPerspective     C_MTXLightPerspective
#define MTXLightOrtho           C_MTXLightOrtho


/*---------------------------------------------------------------------------*
    VECTOR SECTION
 *---------------------------------------------------------------------------*/
// C version
void    C_VECAdd              ( VecPtr a, VecPtr b, VecPtr ab );
void    C_VECSubtract         ( VecPtr a, VecPtr b, VecPtr a_b );
void    C_VECScale            ( VecPtr src, VecPtr dst, f32 scale );
void    C_VECNormalize        ( VecPtr src, VecPtr unit );
f32     C_VECSquareMag        ( VecPtr v );
f32     C_VECMag              ( VecPtr v );
f32     C_VECDotProduct       ( VecPtr a, VecPtr b );
void    C_VECCrossProduct     ( VecPtr a, VecPtr b, VecPtr axb );
f32     C_VECSquareDistance   ( VecPtr a, VecPtr b );
f32     C_VECDistance         ( VecPtr a, VecPtr b );
void    C_VECReflect          ( VecPtr src, VecPtr normal, VecPtr dst );
void    C_VECHalfAngle        ( VecPtr a, VecPtr b, VecPtr half );

// PS assembler version
#ifdef GEKKO
void    PSVECAdd              ( VecPtr a, VecPtr b, VecPtr ab );
void    PSVECSubtract         ( VecPtr a, VecPtr b, VecPtr a_b );
void    PSVECScale            ( VecPtr src, VecPtr dst, f32 scale );
void    PSVECNormalize        ( VecPtr src, VecPtr unit );
f32     PSVECSquareMag        ( VecPtr v );
f32     PSVECMag              ( VecPtr v );
f32     PSVECDotProduct       ( VecPtr a, VecPtr b );
void    PSVECCrossProduct     ( VecPtr a, VecPtr b, VecPtr axb );
f32     PSVECSquareDistance   ( VecPtr a, VecPtr b );
f32     PSVECDistance         ( VecPtr a, VecPtr b );
#endif

void    VECAdd              ( VecPtr a, VecPtr b, VecPtr ab );
void    VECSubtract         ( VecPtr a, VecPtr b, VecPtr a_b );
void    VECScale            ( VecPtr src, VecPtr dst, f32 scale );
void    VECNormalize        ( VecPtr src, VecPtr unit );
f32     VECSquareMag        ( VecPtr v );
f32     VECMag              ( VecPtr v );
f32     VECDotProduct       ( VecPtr a, VecPtr b );
void    VECCrossProduct     ( VecPtr a, VecPtr b, VecPtr axb );
f32     VECSquareDistance   ( VecPtr a, VecPtr b );
f32     VECDistance         ( VecPtr a, VecPtr b );
void    VECReflect          ( VecPtr src, VecPtr normal, VecPtr dst );
void    VECHalfAngle        ( VecPtr a, VecPtr b, VecPtr half );


// Bindings
/*#ifdef MTX_USE_PS
#define VECAdd                  PSVECAdd
#define VECSubtract             PSVECSubtract
#define VECScale                PSVECScale
#define VECNormalize            PSVECNormalize
#define VECSquareMag            PSVECSquareMag
#define VECMag                  PSVECMag
#define VECDotProduct           PSVECDotProduct
#define VECCrossProduct         PSVECCrossProduct
#define VECSquareDistance       PSVECSquareDistance
#define VECDistance             PSVECDistance
#else // MTX_USE_C
#define VECAdd                  C_VECAdd
#define VECSubtract             C_VECSubtract
#define VECScale                C_VECScale
#define VECNormalize            C_VECNormalize
#define VECSquareMag            C_VECSquareMag
#define VECMag                  C_VECMag
#define VECDotProduct           C_VECDotProduct
#define VECCrossProduct         C_VECCrossProduct
#define VECSquareDistance       C_VECSquareDistance
#define VECDistance             C_VECDistance
#endif

#define VECReflect              C_VECReflect
#define VECHalfAngle            C_VECHalfAngle
*/

/*---------------------------------------------------------------------------*
    QUATERNION SECTION
 *---------------------------------------------------------------------------*/
// C version
void C_QUATAdd              ( QuaternionPtr p, QuaternionPtr q, QuaternionPtr r );
void C_QUATSubtract         ( QuaternionPtr p, QuaternionPtr q, QuaternionPtr r );
void C_QUATMultiply         ( QuaternionPtr p, QuaternionPtr q, QuaternionPtr pq );
void C_QUATDivide           ( QuaternionPtr p, QuaternionPtr q, QuaternionPtr r );
void C_QUATScale            ( QuaternionPtr q, QuaternionPtr r, f32 scale );
f32  C_QUATDotProduct       ( QuaternionPtr p, QuaternionPtr q );
void C_QUATNormalize        ( QuaternionPtr src, QuaternionPtr unit );
void C_QUATInverse          ( QuaternionPtr src, QuaternionPtr inv );
void C_QUATExp              ( QuaternionPtr q, QuaternionPtr r );
void C_QUATLogN             ( QuaternionPtr q, QuaternionPtr r );

void C_QUATMakeClosest      ( QuaternionPtr q, QuaternionPtr qto, QuaternionPtr r );
void C_QUATRotAxisRad       ( QuaternionPtr r, VecPtr axis, f32 rad );
void C_QUATMtx              ( QuaternionPtr r, Mtx m );

void C_QUATLerp             ( QuaternionPtr p, QuaternionPtr q, QuaternionPtr r, f32 t );
void C_QUATSlerp            ( QuaternionPtr p, QuaternionPtr q, QuaternionPtr r, f32 t );
void C_QUATSquad            ( QuaternionPtr p, QuaternionPtr a, QuaternionPtr b,
                              QuaternionPtr q, QuaternionPtr r, f32 t );
void C_QUATCompA            ( QuaternionPtr qprev, QuaternionPtr q,
                              QuaternionPtr qnext, QuaternionPtr a );


// PS assembler version
#ifdef GEKKO
void PSQUATAdd              ( QuaternionPtr p, QuaternionPtr q, QuaternionPtr r );
void PSQUATSubtract         ( QuaternionPtr p, QuaternionPtr q, QuaternionPtr r );
void PSQUATMultiply         ( QuaternionPtr p, QuaternionPtr q, QuaternionPtr pq );
void PSQUATDivide           ( QuaternionPtr p, QuaternionPtr q, QuaternionPtr r );
void PSQUATScale            ( QuaternionPtr q, QuaternionPtr r, f32 scale );
f32  PSQUATDotProduct       ( QuaternionPtr p, QuaternionPtr q );
void PSQUATNormalize        ( QuaternionPtr src, QuaternionPtr unit );
void PSQUATInverse          ( QuaternionPtr src, QuaternionPtr inv );
#endif


// Bindings
#ifdef MTX_USE_PS
#define QUATAdd                 PSQUATAdd
#define QUATSubtract            PSQUATSubtract
#define QUATMultiply            PSQUATMultiply
#define QUATDivide              PSQUATDivide
#define QUATScale               PSQUATScale
#define QUATDotProduct          PSQUATDotProduct
#define QUATNormalize           PSQUATNormalize
#define QUATInverse             PSQUATInverse
#else // MTX_USE_C
#define QUATAdd                 C_QUATAdd
#define QUATSubtract            C_QUATSubtract
#define QUATMultiply            C_QUATMultiply
#define QUATDivide              C_QUATDivide
#define QUATScale               C_QUATScale
#define QUATDotProduct          C_QUATDotProduct
#define QUATNormalize           C_QUATNormalize
#define QUATInverse             C_QUATInverse
#endif

#define QUATExp                 C_QUATExp
#define QUATLogN                C_QUATLogN
#define QUATMakeClosest         C_QUATMakeClosest
#define QUATRotAxisRad          C_QUATRotAxisRad
#define QUATMtx                 C_QUATMtx
#define QUATLerp                C_QUATLerp
#define QUATSlerp               C_QUATSlerp
#define QUATSquad               C_QUATSquad
#define QUATCompA               C_QUATCompA


/*---------------------------------------------------------------------------*
    SPECIAL PURPOSE MATRIX SECTION
 *---------------------------------------------------------------------------*/
// Only PS assembler versions are available
#ifdef GEKKO
void    PSMTXReorder          ( Mtx src, ROMtx dest );
void    PSMTXROMultVecArray   ( ROMtx m, VecPtr srcBase, VecPtr dstBase, u32 count );
void    PSMTXROSkin2VecArray  ( ROMtx m0, ROMtx m1, f32* wtBase, VecPtr srcBase, VecPtr dstBase, u32 count);
void    PSMTXMultS16VecArray  ( Mtx m, S16Vec* srcBase, VecPtr dstBase, u32 count );
void    PSMTXROMultS16VecArray( ROMtx m, S16Vec* srcBase, VecPtr dstBase, u32 count );
#endif


/*---------------------------------------------------------------------------*
    MATRIX STACK SECTION
 *---------------------------------------------------------------------------*/
void    MTXInitStack          ( MtxStackPtr sPtr, u32 numMtx );
MtxPtr  MTXPush               ( MtxStackPtr sPtr, Mtx m );
MtxPtr  MTXPushFwd            ( MtxStackPtr sPtr, Mtx m );
MtxPtr  MTXPushInv            ( MtxStackPtr sPtr, Mtx m );
MtxPtr  MTXPushInvXpose       ( MtxStackPtr sPtr, Mtx m );
MtxPtr  MTXPop                ( MtxStackPtr sPtr );
MtxPtr  MTXGetStackPtr        ( MtxStackPtr sPtr );


// macro to create a matrix stack.
// the macro exposes the  use of OSAlloc() for
// ease of replacement if desired.
#define MTXAllocStack( sPtr, numMtx ) (  ((MtxStackPtr)(sPtr))->stackBase = (MtxPtr)OSAlloc( ( (numMtx) * sizeof(Mtx) ) )  )
 
// macro to free a matrix stack
#define MTXFreeStack( sPtr )    (  OSFree( (void*)( ((MtxStackPtr)(sPtr))->stackBase ) )  )
 

/*---------------------------------------------------------------------------*/


#if 0
}
#endif

#endif // __MTX_H__

/*===========================================================================*/

