/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:     GXDraw.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/include/dolphin/gx/GXDraw.h $
    
    4     9/23/99 5:37p Alligator
    added GenNormalTable func
    
    3     7/20/99 6:30p Alligator
    added new models, all models are normalized, no vtxfmt needed
    
    2     7/16/99 2:04p Alligator
    
    1     7/16/99 1:06p Alligator
    procedural models used in tests
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GXDRAW_H__
#define __GXDRAW_H__

#if 0
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
#include "types.h"
#include "GXEnum.h"

void GXDrawCylinder(u8 numEdges);
void GXDrawTorus(f32 rc, u8 numc, u8 numt);
void GXDrawSphere(u8 numMajor, u8 numMinor);
void GXDrawCube(void);
void GXDrawDodeca(void);
void GXDrawOctahedron( void );
void GXDrawIcosahedron( void );
void GXDrawSphere1( u8 depth );
u32  GXGenNormalTable( u8 depth, f32* table );

/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif // __GXDRAW_H__
