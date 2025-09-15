/*---------------------------------------------------------------------------*
  Project:  Dolphin OS Reset button API
  File:     OSResetSW.h

  Copyright 2000, 2001 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/dolphin/os/OSResetSW.h $
    
    2     8/27/01 10:34 Shiki
    Added OSGetResetButtonState().

    1     6/14/00 11:51p Shiki
    Initial check-in.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __OSRESETSW_H__
#define __OSRESETSW_H__

#include "types.h"
#include "OSContext.h"

#if 0
extern "C" {
#endif

typedef void (*OSResetCallback)(void);

BOOL            OSGetResetButtonState( void );

// obsolete
BOOL            OSGetResetSwitchState( void );
OSResetCallback OSSetResetCallback   ( OSResetCallback callback );

#if 0
}
#endif

#endif  // __OSRESETSW_H__
