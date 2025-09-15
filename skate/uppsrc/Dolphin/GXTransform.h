/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:     GXTransform.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/dolphin/gx/GXTransform.h $
    
    8     5/08/01 5:56p Carl
    GXSetScissorBoxOffset now takes signed arguments.
    
    7     4/17/01 5:29p Tian
    Changed all inlines to static inline
    
    6     12/11/00 5:54p Stevera
    Added conditional #include "win32/win32.h" for pcemu
    
    5     11/06/00 4:06p Carl
    Added GXSetClipMode.
    
    4     10/18/00 4:05p Carl
    Latest round of HW2 modifications and minor updates.
    
    3     6/15/00 1:17p Carl
    Fixed indexed normal matrix load issues.
    
    2     3/06/00 11:54a Alligator
    change GXSetDefaultMtx to GXSetCurrentMtx
    
    8     11/18/99 2:32p Alligator
    added defines for projection and viewport vector sizes
    
    7     11/12/99 1:22p Alligator
    added GXProject, GXGetViewportv, GXGetProjectionv
    
    6     10/13/99 4:32p Alligator
    change GXSetViewport, GXSetScissor to use xorig, yorig, wd, ht
    
    5     9/30/99 10:40p Yasu
    Renamed some GX functions and enums
    
    4     9/17/99 11:24a Ryan
    changed GXJitterViewport to GXSetViewportJitter
    
    3     9/01/99 4:27p Ryan
    Added GXJitterViewport
    
    1     7/14/99 4:20p Alligator
    split gx.h into individual header files for each major section of API
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GXTRANSFORM_H__
#define __GXTRANSFORM_H__

#if 0
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
#include "types.h"
#include "GXEnum.h"
#include "GXStruct.h"

#ifdef flagWIN32
#include "win32.h"
#endif

#define GX_PROJECTION_SZ  7
#define GX_VIEWPORT_SZ    6

/*---------------------------------------------------------------------------*/
void GXSetProjection ( const Mtx44 mtx, GXProjectionType type );
void GXSetProjectionv ( const f32* ptr );
void GXLoadPosMtxImm ( MtxPtr mtx_ptr, u32 id );
void GXLoadPosMtxIndx ( u16 mtx_indx, u32 id );
void GXLoadNrmMtxImm ( MtxPtr mtx_ptr, u32 id );
void GXLoadNrmMtxImm3x3 ( f32 mtx[3][3], u32 id );
void GXLoadNrmMtxIndx3x3 ( u16 mtx_indx, u32 id );
void GXSetCurrentMtx ( u32 id );
void GXLoadTexMtxImm ( f32 mtx[][4], u32 id, GXTexMtxType type );
void GXLoadTexMtxIndx ( u16 mtx_indx, u32 id, GXTexMtxType type );

void GXProject ( 
	f32  x,          // model coordinates
	f32  y,
	f32  z,
	const f32  mtx[3][4],  // model-view matrix
	const f32* pm,         // projection matrix, as returned by GXGetProjectionv
	const f32* vp,         // viewport, as returned by GXGetViewportv
	f32* sx,         // screen coordinates
	f32* sy,
	f32* sz );

void GXSetViewport(
         f32 left, 
         f32 top, 
         f32 wd, 
         f32 ht, 
         f32 nearz, 
         f32 farz );

static inline void GXSetViewportv( f32 *vp )
{   // Note: doesn't check for NULL ptr
    GXSetViewport(vp[0], vp[1], vp[2], vp[3], vp[4], vp[5]);
}

void GXSetViewportJitter(
         f32 left, 
         f32 top, 
         f32 wd, 
         f32 ht, 
         f32 nearz, 
         f32 farz,
	 u32 field );

#if ( GX_REV != 1 )
void GXSetScissorBoxOffset( s32 x_off, s32 y_off );
#endif

void GXSetClipMode( GXClipMode mode );

/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif // __GXTRANSFORM_H__
