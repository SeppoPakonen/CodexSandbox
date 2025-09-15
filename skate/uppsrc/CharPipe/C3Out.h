/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3Out.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/include/charPipeline/c3/C3Out.h $
    
    4     7/18/00 7:37p John
    Removed unnecessary and unused defines (constants).
    Changed C3_FILE_ALL define.
    
    3     2/29/00 7:18p John
    Added C3OptimizeBeforeOutput function to clean up C3 API.
    
    2     2/04/00 6:06p John
    Untabified code.
    
    9     9/29/99 4:25p John
    Changed header to make them all uniform with logs.
    
    8     9/17/99 9:33a John
    Added C3Options and cleaned up code.

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _C3OUT_H
#define _C3OUT_H

#if 0
extern "C" {
#endif

/********************************/

// FILE EXTENSIONS

#define C3_EXT_GEO_PALETTE  "gpl"
#define C3_EXT_HIERARCHY    "act"
#define C3_EXT_ANIMATION    "anm"
#define C3_EXT_TEXTURE_PAL  "tpl"
#define C3_EXT_TEXT         "h"

/********************************/

// FILE TYPE OUTPUT

#define C3_FILE_NONE        0x0000
#define C3_FILE_GEOMETRY    0x0100
#define C3_FILE_HIERARCHY   0x0200
#define C3_FILE_ANIMATION   0x0400
#define C3_FILE_TEXTURE     0x0800
#define C3_FILE_STATS       0x4000
#define C3_FILE_ALL         0X4F00

/*>*******************************(*)*******************************<*/

void C3OptimizeBeforeOutput( );
void C3WriteFile( char* path, char* name );

#if 0
}
#endif

#endif // _C3OUT_H
