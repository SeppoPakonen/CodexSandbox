/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:     GXCull.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/include/dolphin/gx/GXCull.h $
    
    5     10/13/99 4:32p Alligator
    change GXSetViewport, GXSetScissor to use xorig, yorig, wd, ht
    
    4     9/30/99 10:40p Yasu
    Renamed some GX functions and enums
    
    3     1/05/98 2:48p Ryan
    routine updated of header files to match man pages
    
    2     7/28/99 4:07p Alligator
    update header files and emulator for hw changes
    
    1     7/14/99 4:20p Alligator
    split gx.h into individual header files for each major section of API
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GXCULL_H__
#define __GXCULL_H__

#if 0
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
#include "types.h"
#include "GXEnum.h"

/*---------------------------------------------------------------------------*/
void GXSetScissor 	( u32 left, u32 top, u32 wd, u32 ht );
void GXSetCullMode	( GXCullMode mode );
void GXSetCoPlanar	( GXBool enable );

/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif // __GXCULL_H__
