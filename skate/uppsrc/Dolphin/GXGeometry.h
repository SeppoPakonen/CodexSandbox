/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:     GXGeometry.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/dolphin/gx/GXGeometry.h $
    
    12    4/17/01 5:29p Tian
    Changed all inlines to static inline
    
    11    4/04/01 1:10p Carl
    Latest XF bug fix.
    
    10    3/16/01 7:22p Carl
    Changes for XF stall bug.
    
    9     10/18/00 5:44p Hirose
    modified definition rule of GXSetTexCoordGen
    
    8     10/18/00 4:05p Carl
    Latest round of HW2 modifications and minor updates.
    
    7     10/03/00 7:55p Hirose
    Update for HW2(Rev.B) GX API extensions
    
    6     9/29/00 9:51p Hirose
    replaced "MAC" flag by better alternatives
    
    5     7/07/00 6:00p Dante
    PC Compatibility
    
    4     3/27/00 4:59p Danm
    Made GXEnd inline in non-debug builds
    
    3     3/14/00 1:36p Alligator
    changed GXSetTexCoordGen matrix type to u32 so developers can override
    matrix memory map without spurious warnings.
    
    2     3/03/00 4:21p Alligator
    integrate with ArtX source
    
    7     10/29/99 3:35p Hirose
    added GXSetNumTexGens(u8)
    
    6     10/13/99 4:32p Alligator
    change GXSetViewport, GXSetScissor to use xorig, yorig, wd, ht
    
    5     9/30/99 10:40p Yasu
    Renamed some GX functions and enums
    
    4     1/05/98 2:39p Ryan
    routine update to match header file to man pages
    
    3     8/17/99 1:33p Alligator
    added GXSetTexGenEnables function, modified GXTexCoordID enumeration for
    bitwise OR of enables.
    
    2     7/28/99 4:07p Alligator
    update header files and emulator for hw changes
    
    1     7/14/99 4:20p Alligator
    split gx.h into individual header files for each major section of API
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GXGEOMETRY_H__
#define __GXGEOMETRY_H__

#if 0
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
#include "types.h"
#include "GXEnum.h"
#include "GXStruct.h"

void GXSetVtxDesc           ( GXAttr attr, GXAttrType type );
void GXSetVtxDescv          ( GXVtxDescList *attrPtr );
void GXClearVtxDesc         ( void );

void GXSetVtxAttrFmt(
    GXVtxFmt       vtxfmt,
    GXAttr         attr,
    GXCompCnt      cnt,
    GXCompType     type,
    u8             frac );

void GXSetVtxAttrFmtv       ( GXVtxFmt vtxfmt, GXVtxAttrFmtList *list );
void GXSetArray             ( GXAttr attr, void* base_ptr, u8 stride );
void GXBegin                ( GXPrimitive type, GXVtxFmt vtxfmt, u16 nverts );
void GXGetArray             (GXAttr attr, void** base_ptr, u8* stride);

#if defined(flagEMU) || defined(flagWIN32)
void GXEnd                  ( void );
#else // !EMU
static inline void GXEnd           ( void )
{
#ifdef _DEBUG
    extern GXBool GXinBegin;
    void OSPanic ( const char* file, int line, const char* msg, ... );
    if (!GXinBegin)
        OSPanic(__FILE__, __LINE__, "GXEnd: called without a GXBegin");
    GXinBegin = GX_FALSE;
#endif
}
#endif // EMU

#if ( GX_REV != 1 ) // GX revision 2 or later only
void GXSetTexCoordGen2(
    GXTexCoordID     dst_coord,
    GXTexGenType     func,
    GXTexGenSrc      src_param,
    u32              mtx,
    GXBool           normalize,
    u32              postmtx );
#endif // ( GX_REV != 1 )

#if ( GX_REV == 1 || defined(flagEMU) ) // GX revision 1 or emulator
void GXSetTexCoordGen( 
    GXTexCoordID     dst_coord,
    GXTexGenType     func,
    GXTexGenSrc      src_param,
    u32              mtx );
#else // GX revision 2 or later and real hardware
static inline void GXSetTexCoordGen ( 
    GXTexCoordID    dst_coord,
    GXTexGenType    func,
    GXTexGenSrc     src_param,
    u32             mtx )
{
    GXSetTexCoordGen2(dst_coord, func, src_param, mtx, 
                      GX_FALSE, GX_PTIDENTITY);
}
#endif // ( GX_REV == 1 || defined(flagEMU) )


void GXSetNumTexGens        ( u8 nTexGens );

void GXInvalidateVtxCache   ( void );
void GXSetLineWidth         ( u8 width, GXTexOffset texOffsets );
void GXSetPointSize         ( u8 pointSize, GXTexOffset texOffsets );
void GXEnableTexOffsets	    ( GXTexCoordID coord, GXBool line_enable, 
			      GXBool point_enable );

/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif // __GXGEOMETRY_H__
