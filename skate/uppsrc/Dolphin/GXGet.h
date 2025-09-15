/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:     GXGet.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/dolphin/gx/GXGet.h $
    
    8     6/25/01 7:06p Hirose
    Removed protorype of GXGetArray.
    
    7     1/25/01 2:22p Carl
    Added GXGetTexObjTlut.
    
    6     1/24/01 7:19p Carl
    Fixes for GXGetTexRegionAll.
    
    5     1/24/01 4:28p Carl
    New texture init and get functions added.
    
    4     10/18/00 4:05p Carl
    Latest round of HW2 modifications and minor updates.
    
    3     5/12/00 6:05p Hirose
    Changed definition of GXGetLight*v macros
    
    2     4/11/00 1:03p Alligator
    added GXGetLight* functions
    
    8     11/18/99 2:50p Hirose
    added GXGetTlutObj* functions
    
    7     11/17/99 10:26p Hirose
    added GXGetTexObj* functions
    
    6     11/12/99 1:22p Alligator
    added GXProject, GXGetViewportv, GXGetProjectionv
    
    5     11/08/99 9:14a Alligator
    added GXGetTexObj
    
    4     9/30/99 10:40p Yasu
    Renamed some GX functions and enums
    
    3     9/14/99 10:36a Ryan
    changed GXGetViewport to take all floats
    
    2     7/20/99 6:10p Alligator
    added GXGetVtxDescv, GXGetVtxAttrFmtv
    
    1     7/14/99 4:20p Alligator
    split gx.h into individual header files for each major section of API
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GXGET_H__
#define __GXGET_H__

#if 0
extern "C" {
#endif

#include "types.h"
#include "GXEnum.h"
#include "GXStruct.h"

/*---------------------------------------------------------------------------* 
      GEOMETRY AND VERTEX FUNCTIONS
 *---------------------------------------------------------------------------*/
void GXGetVtxDesc            ( GXAttr attr, GXAttrType *type );
void GXGetVtxDescv           ( GXVtxDescList *vcd );
void GXGetVtxAttrFmtv        ( GXVtxFmt fmt, GXVtxAttrFmtList *vat );

void GXGetLineWidth          ( u8 *width, GXTexOffset *texOffsets );
void GXGetPointSize          ( u8 *pointSize, GXTexOffset *texOffsets );

void GXGetVtxAttrFmt( 
    GXVtxFmt       idx, 
    GXAttr         attr, 
    GXCompCnt*     compCnt, 
    GXCompType*    compType, 
    u8*            shift );

/*---------------------------------------------------------------------------* 
      TRANSFORMATION AND MATRIX FUNCTIONS
 *---------------------------------------------------------------------------*/
void GXGetViewportv( f32* viewport );  // used by GXProject
void GXGetProjectionv( f32* p );       // used by GXProject

/*---------------------------------------------------------------------------* 
      CLIPPING AND CULLING FUNCTIONS
 *---------------------------------------------------------------------------*/
void GXGetScissor( 
    u32*    left, 
    u32*    top, 
    u32*    width, 
    u32*    height );

void GXGetCullMode( GXCullMode *mode );

/*---------------------------------------------------------------------------* 
      LIGHTING OBJECT ACCESS FUNCTIONS
 *---------------------------------------------------------------------------*/
void GXGetLightAttnA(
        GXLightObj* lt_obj,
        f32*        a0,
        f32*        a1,
        f32*        a2 );

void GXGetLightAttnK(
        GXLightObj* lt_obj,
        f32*        k0,
        f32*        k1,
        f32*        k2 );

void GXGetLightPos(
        GXLightObj* lt_obj,
        f32*        x,
        f32*        y,
        f32*        z );

#define GXGetLightPosv(lo, vec) \
 (GXGetLightPos((lo), (f32*)(vec), (f32*)(vec)+1, (f32*)(vec)+2))

void GXGetLightDir(
        GXLightObj* lt_obj,
        f32*        nx,
        f32*        ny,
        f32*        nz );

#define GXGetLightDirv(lo, vec) \
 (GXGetLightDir((lo), (f32*)(vec), (f32*)(vec)+1, (f32*)(vec)+2))

void GXGetLightColor(
        GXLightObj* lt_obj,
        GXColor*    color );

/*---------------------------------------------------------------------------* 
      TEXTURE OBJECT ACCESS FUNCTIONS
 *---------------------------------------------------------------------------*/
void*         GXGetTexObjData  ( GXTexObj* tex_obj );
u16           GXGetTexObjWidth ( GXTexObj* tex_obj );
u16           GXGetTexObjHeight( GXTexObj* tex_obj );
GXTexFmt      GXGetTexObjFmt   ( GXTexObj* tex_obj );
GXTexWrapMode GXGetTexObjWrapS ( GXTexObj* tex_obj );
GXTexWrapMode GXGetTexObjWrapT ( GXTexObj* tex_obj );
GXBool        GXGetTexObjMipMap( GXTexObj* tex_obj );     

void GXGetTexObjAll(
        GXTexObj*       tex_obj, 
        void**          data, 
        u16*            width, 
        u16*            height, 
        GXTexFmt*       format,
        GXTexWrapMode*  wrapS, 
        GXTexWrapMode*  wrapT,
        GXBool*         mipMap );

GXTexFilter   GXGetTexObjMinFilt  ( GXTexObj* tex_obj );
GXTexFilter   GXGetTexObjMagFilt  ( GXTexObj* tex_obj );
f32           GXGetTexObjMinLOD   ( GXTexObj* tex_obj );
f32           GXGetTexObjMaxLOD   ( GXTexObj* tex_obj );
f32           GXGetTexObjLODBias  ( GXTexObj* tex_obj );
GXBool        GXGetTexObjBiasClamp( GXTexObj* tex_obj );
GXBool        GXGetTexObjEdgeLOD  ( GXTexObj* tex_obj );
GXAnisotropy  GXGetTexObjMaxAniso ( GXTexObj* tex_obj );

void GXGetTexObjLODAll (
        GXTexObj*       tex_obj,
        GXTexFilter*    min_filt,
        GXTexFilter*    mag_filt,
        f32*            min_lod,
        f32*            max_lod,
        f32*            lod_bias,
        GXBool*         bias_clamp,
        GXBool*         do_edge_lod,
        GXAnisotropy*   max_aniso );

u32 GXGetTexObjTlut( GXTexObj* tex_obj );

void*         GXGetTlutObjData      ( GXTlutObj* tlut_obj );
GXTlutFmt     GXGetTlutObjFmt       ( GXTlutObj* tlut_obj );
u16           GXGetTlutObjNumEntries( GXTlutObj* tlut_obj );

void GXGetTlutObjAll (
        GXTlutObj*      tlut_obj,
        void**          data,
        GXTlutFmt*      format,
        u16*            numEntries );

void GXGetTexRegionAll (
        GXTexRegion*    region,
        GXBool*         is_cached,
        GXBool*         is_32b_mipmap,
        u32*            tmem_even,
        u32*            size_even,
        u32*            tmem_odd,
        u32*            size_odd );

void GXGetTlutRegionAll (
        GXTlutRegion*   region,
        u32*            tmem_addr,
        GXTlutSize*     tlut_size );

/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif // __GXGET_H__
