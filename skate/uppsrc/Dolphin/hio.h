/*---------------------------------------------------------------------------*
  Project:  Dolphin OS Host I/O
  File:     hio.h

  Copyright 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/dolphin/hio.h $
    
    4     10/12/01 15:27 Shiki
    Added HIOInitEx().

    3     3/05/01 8:12p Hashida
    Fixed paths.

    2     2/21/01 7:27p Hashida
    Transition phase to incorporate hio library in source tree.

    2     9/11/00 10:30a Ryunos. add;HIOEnumDevices,HIOEnumCallback
    1     7/14/00 4:33p Shiki
    Initial check-in.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __HIO_H__
#define __HIO_H__

#ifdef flagWIN32//host include here
#   include <windows.h>
#   include "types.h"
#else
#   include "types.h"

#ifndef WINAPI
#   define WINAPI
#endif

#endif

#if 0
extern "C" {
#endif

#define HIO_STATUS_TX   0x00000001  // Tx mailbox interrupt
#define HIO_STATUS_RX   0x00000002  // Rx mailbox interrupt
#define HIO_STATUS_ID   0x00000004  // ID interrupt

// CALLBACK PROTOTYPES
// ================
typedef void (WINAPI*HIOCallback)( void /*OSContext* context*/);
typedef BOOL (WINAPI*HIOEnumCallback)( s32 chan );

// FUNCTION PROTOTYPES
// ================
BOOL WINAPI HIOEnumDevices ( HIOEnumCallback callback );
BOOL WINAPI HIOInit        ( s32 chan, HIOCallback callback );
BOOL WINAPI HIOInitEx      ( s32 chan, u32 dev, HIOCallback callback );
BOOL WINAPI HIOReadMailbox ( u32* word );
BOOL WINAPI HIOWriteMailbox( u32  word );
BOOL WINAPI HIORead        ( u32 addr, void* buffer, s32 size );
BOOL WINAPI HIOWrite       ( u32 addr, void* buffer, s32 size );
BOOL WINAPI HIOReadAsync   ( u32 addr, void* buffer, s32 size, HIOCallback callback );
BOOL WINAPI HIOWriteAsync  ( u32 addr, void* buffer, s32 size, HIOCallback callback );
BOOL WINAPI HIOReadStatus  ( u32* status );

#ifdef flagWIN32
void WINAPI HIOExit ( );
#endif

#if 0
}
#endif

#endif  // __HIO_H__
