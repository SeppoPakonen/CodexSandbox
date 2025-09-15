/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:     GXDispList.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/dolphin/gx/GXDispList.h $
    
    3     6/11/01 9:08p Tian
    Made GXFastCallDisplayList static inline
    
    2     5/22/01 8:08p Carl
    Added GXFastCallDisplayList.
    
    4     11/10/99 3:59p Tian
    Updated GXEndDisplayList API
    
    3     9/02/99 11:04a Ryan
    
    2     7/28/99 4:07p Alligator
    update header files and emulator for hw changes
    
    1     7/14/99 4:20p Alligator
    split gx.h into individual header files for each major section of API
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GXDISPLIST_H__
#define __GXDISPLIST_H__

#if 0
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
#include "types.h"

/*---------------------------------------------------------------------------*/
void GXBeginDisplayList		( byte* list, u32 size );
u32  GXEndDisplayList		( void );
void GXCallDisplayList		( const void *list, u32 nbytes );

#ifndef flagEMU
// Note: GXCommandList.h and GXVert.h must be #included first

// Note: This function can only be used in certain situations.
//       There must be no dirty state pending.  Thus this function
//       is safe when called after a GXCallDisplayList or GXEnd call
//       (without any other GX calls in between).  It is also safe to
//       call this function multiple times in a row.
//       Please refer to the online man page for complete details.

// Note: No error checking is done; be careful!

static inline void GXFastCallDisplayList ( void *list, u32 nbytes )
    {
        GXWGFifo.u8  = (u8) GX_CMD_CALL_DL;
        // XXX UNSAFE: Assumes GP ignores all extra padding/unused bits.
        GXWGFifo.ptr = (size_t) list;
        GXWGFifo.u32 = (u32) nbytes;
    }
#else
static inline void GXFastCallDisplayList ( void *list, u32 nbytes )
    { GXCallDisplayList( list, nbytes ); }
#endif

/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif // __GXDISPLIST_H__
