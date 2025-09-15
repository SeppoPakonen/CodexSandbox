/*---------------------------------------------------------------------------*
  Project:  Dolphin OS Expansion Interface API
  File:     OSSerial.h

  Copyright 2001 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/dolphin/os/OSExpansion.h $
    
    1     10/18/01 13:49 Shiki
    Initial check-in.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __OS_EXPANSION_H__
#define __OS_EXPANSION_H__

#include "types.h"
#include "os.h"

#if 0
extern "C" {
#endif

#define EXI_MEMORY_CARD_59      0x00000004
#define EXI_MEMORY_CARD_123     0x00000008
#define EXI_MEMORY_CARD_251     0x00000010
#define EXI_MEMORY_CARD_507     0x00000020

#define EXI_USB_ADAPTER         0x01010000
#define EXI_NPDP_GDEV           0x01020000

#define EXI_MODEM               0x02020000

#define EXI_MARLIN              0x03010000

BOOL  EXIProbe        ( s32 chan );
s32   EXIProbeEx      ( s32 chan );

s32   EXIGetType      ( s32 chan, u32 dev, u32* type );
char* EXIGetTypeString( u32 type );

#if 0
}
#endif

#endif  // __OS_EXPANSION_H__
