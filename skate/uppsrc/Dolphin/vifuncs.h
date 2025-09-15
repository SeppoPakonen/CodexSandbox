/*---------------------------------------------------------------------------*
  Project:  Dolphin VI library
  File:     vifuncs.h

  Copyright 1998 - 2001 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/dolphin/vi/vifuncs.h $
    
    7     7/11/01 5:10p Hashida
    Updated header.
    
    6     6/25/01 3:29p Hashida
    Added VIGetDTVStatus.
    
    5     4/28/00 4:26p Carl
    Changed set callback routines to return old callback.
    
    4     2/25/00 11:30a Hashida
    Added VISetPreRetraceCallback and VISetPostRetraceCallback
    
    3     2/17/00 4:17p Hashida
    Added VIGetTvFormat
    
    2     1/28/00 11:01p Hashida
    Added VIFlush().
    
    1     1/26/00 3:51p Hashida
    initial revision
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __VIFUNCS_H
#define __VIFUNCS_H

#if 0
extern "C" {
#endif

#include "types.h"
#include "vitypes.h"
#include "GXStruct.h"     // for GXRenderModeObj structure


#define VIPadFrameBufferWidth(width)     ((u16)(((u16)(width) + 15) & ~15))

void VIInit                  ( void );
void VIFlush                 ( void );
void VIWaitForRetrace        ( void );

void VIConfigure             ( GXRenderModeObj* rm );
void VIConfigurePan          ( u16 PanPosX, u16 PanPosY,
                               u16 PanSizeX, u16 PanSizeY );
void VISetNextFrameBuffer    ( void *fb );

VIRetraceCallback VISetPreRetraceCallback  (VIRetraceCallback callback);
VIRetraceCallback VISetPostRetraceCallback (VIRetraceCallback callback);

void VISetBlack              ( BOOL black );
u32  VIGetRetraceCount       ( void );
u32  VIGetNextField          ( void );
u32  VIGetCurrentLine        ( void );
u32  VIGetTvFormat           ( void );

u32  VIGetDTVStatus          ( void );

/********************************/
#if 0
}
#endif

#endif
