/*---------------------------------------------------------------------------*
  Project:  Dolphin OS Reset API
  File:     OSReset.h

  Copyright 2001 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/dolphin/os/OSReset.h $
    
    8     9/15/01 3:31a Hashida
    Added OS_RESET_SHUTDOWN
    
    7     9/07/01 5:30a Hashida
    Added OSSet/GetSaveRegion.
    
    6     7/18/01 10:55p Hashida
    Changed definitions.
    
    5     7/11/01 10:19p Hashida
    Added code definition for restarting.
    
    4     5/17/01 8:20p Shiki
    Modified priority from s32 to u32.

    3     5/17/01 7:58p Shiki
    Revised.

    2     01/04/23 16:31 Shiki
    Added menu param to OSResetSystem().

    1     01/04/09 13:52 Shiki
    Initial check-in.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __OSRESET_H__
#define __OSRESET_H__

#include "types.h"

#if 0
extern "C" {
#endif

#define OS_RESETCODE_RESTART    0x80000000      // not by cold/hot reset

#define OS_RESET_RESTART        0
#define OS_RESET_HOTRESET       1
#define OS_RESET_SHUTDOWN       2

#define OS_RESET_PRIO_CARD  127     // CARD API
#define OS_RESET_PRIO_PAD   127     // PAD API

typedef BOOL (* OSResetFunction )(BOOL final);
typedef struct OSResetFunctionInfo OSResetFunctionInfo;

struct OSResetFunctionInfo
{
    // public
    OSResetFunction      func;
    u32                  priority;

    // private
    OSResetFunctionInfo* next;
    OSResetFunctionInfo* prev;
};

void OSRegisterResetFunction  ( OSResetFunctionInfo* info );
void OSUnregisterResetFunction( OSResetFunctionInfo* info );
void OSResetSystem            ( int reset, u32 resetCode, BOOL forceMenu );
u32  OSGetResetCode           ( void );
void OSGetSaveRegion          ( void** start, void** end );
void OSSetSaveRegion          ( void* start, void* end );
    
#if 0
}
#endif

#endif  // __OSRESET_H__
