/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:     GXLighting.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/dolphin/gx/GXLighting.h $
    
    9     10/03/01 9:33a Hirose
    fixed GXInitSpecularDirHAv macro again.
    
    8     8/21/00 4:44p Hirose
    fixed GXInitSpecularDirHAv macro
    
    7     8/21/00 11:36a Hirose
    deleted GXInitSpecularHA / added GXInitSpecluarDirHA instead
    
    6     5/12/00 6:04p Hirose
    Changed definition of GXInit*v macros
    
    5     4/10/00 3:09p Alligator
    removed GXInitSpecularHAv macro
    
    4     2/12/00 5:16p Alligator
    Integrate ArtX source tree changes
    
    3     2/01/00 7:22p Alligator
    second GX update from ArtX
    
    2     12/13/99 4:54p Hirose
    added GXInitLightShininess macro
    
    9     11/17/99 4:14p Hirose
    added macros for vector type
    
    8     10/29/99 3:35p Hirose
    replaced GXSetChannels(GXChannelID) to GXSetNumChans(u8)
    
    7     10/28/99 4:20p Hirose
    added GXSetChannels
    
    6     10/18/99 3:49p Hirose
    changed the type of light_mask on GXSetChanCtrl to u32
    
    5     9/30/99 10:40p Yasu
    Renamed some GX functions and enums
    
    4     9/09/99 1:22p Hirose
    added GXInitLightSpot() and GXInitLightDistAttn()
    
    3     1/05/98 3:02p Ryan
    routine update of header file to match man pages
    
    2     7/28/99 4:07p Alligator
    update header files and emulator for hw changes
    
    1     7/14/99 4:20p Alligator
    split gx.h into individual header files for each major section of API
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GXLIGHTING_H__
#define __GXLIGHTING_H__

#if 0
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
#include "types.h"
#include "GXEnum.h"
#include "GXStruct.h"

/*---------------------------------------------------------------------------*/
void GXInitLightAttn(
         GXLightObj*   lt_obj,
         f32           a0,
         f32           a1,
         f32           a2,
         f32           k0,
         f32           k1,
         f32           k2 );

void GXInitLightAttnA ( GXLightObj *lt_obj, f32 a0, f32 a1, f32 a2);
void GXInitLightAttnK ( GXLightObj *lt_obj, f32 k0, f32 k1, f32 k2 );

void GXInitLightSpot(
         GXLightObj*   lt_obj,
         f32           cutoff,
         GXSpotFn      spot_func );

void GXInitLightDistAttn(
         GXLightObj*   lt_obj,
         f32           ref_distance,
         f32           ref_brightness,
         GXDistAttnFn  dist_func );


void GXInitLightPos 	( GXLightObj* lt_obj, f32 x, f32 y, f32 z );
void GXInitLightColor 	( GXLightObj* lt_obj, GXColor color );
void GXLoadLightObjImm 	( const GXLightObj* lt_obj, GXLightID light );
void GXLoadLightObjIndx	( u32 lt_obj_indx, GXLightID light );
void GXSetChanAmbColor 	( GXChannelID chan, GXColor amb_color );
void GXSetChanMatColor	( GXChannelID chan, GXColor mat_color );
void GXSetNumChans      ( u8 nChans );

void GXInitLightDir 	( GXLightObj* lt_obj, f32 nx, f32 ny, f32 nz );

// New functions to set light direction and half-angle.
void GXInitSpecularDir 	( GXLightObj* lt_obj, f32 nx, f32 ny, f32 nz );
void GXInitSpecularDirHA(
        GXLightObj* lt_obj,
        f32         nx,
        f32         ny,
        f32         nz,
        f32         hx,
        f32         hy,
        f32         hz );

void GXSetChanCtrl(
        GXChannelID   chan,
        GXBool        enable,
        GXColorSrc    amb_src,
        GXColorSrc    mat_src,
        uint          light_mask,
        GXDiffuseFn   diff_fn,
        GXAttnFn      attn_fn );

/*---------------------------------------------------------------------------*/
// Convenient Macros

#define GXInitLightPosv(lo,vec) \
    (GXInitLightPos((lo), *(f32*)(vec), *((f32*)(vec)+1), *((f32*)(vec)+2)))

#define GXInitLightDirv(lo,vec) \
    (GXInitLightDir((lo), *(f32*)(vec), *((f32*)(vec)+1), *((f32*)(vec)+2)))

#define GXInitSpecularDirv(lo,vec) \
    (GXInitSpecularDir((lo), *(f32*)(vec), *((f32*)(vec)+1), *((f32*)(vec)+2)))

#define GXInitSpecularDirHAv(lo,vec0,vec1) \
    (GXInitSpecularDirHA((lo), \
    *(f32*)(vec0), *((f32*)(vec0)+1), *((f32*)(vec0)+2), \
    *(f32*)(vec1), *((f32*)(vec1)+1), *((f32*)(vec1)+2)))

#define GXInitLightShininess(lobj, shininess) \
    (GXInitLightAttn(lobj, 0.0F, 0.0F, 1.0F,  \
                    (shininess)/2.0F, 0.0F,   \
                    1.0F-(shininess)/2.0F ))


/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif // __GXLIGHTING_H__
