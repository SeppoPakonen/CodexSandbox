/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:     GXVerify.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/dolphin/gx/GXVerify.h $
    
    5     8/07/01 3:08p Carl
    Fixed typo.
    
    4     11/14/00 11:33p Hirose
    added stubs for GXVerify stuff of NDEBUG build
    
    3     10/06/00 6:20p Carl
    Updated warning system to print messages through callback (part 2)
    
    2     8/27/00 5:45p Alligator
    print warning messages through a callback function
    
    1     1/28/00 3:55p Alligator
    integrate library from ArtX
    
    1     8/30/99 11:06a Vimal
    
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GXVERIFY_H
#define __GXVERIFY_H

#if 0
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
#include "types.h"

typedef enum {
    GX_WARN_NONE,       // no warnings reported
    GX_WARN_SEVERE,     // reports only severest warnings
    GX_WARN_MEDIUM,     // reports severe and medium warnings
    GX_WARN_ALL         // reports any and all warning info
} GXWarningLevel;

#ifdef _DEBUG
extern void GXSetVerifyLevel( GXWarningLevel level );
#else
#define GXSetVerifyLevel(level)     ((void)0)
#endif

// Note: warning IDs are subject to change.
//       warning messages are subject to change as well.

typedef void (*GXVerifyCallback)(GXWarningLevel level, 
                                 u32            id, 
                                 char*          msg);

#ifdef _DEBUG
GXVerifyCallback GXSetVerifyCallback( GXVerifyCallback cb );
#else
#define GXSetVerifyCallback(cb)     ((GXVerifyCallback)0)
#endif

/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif // __GXVERIFY_H
