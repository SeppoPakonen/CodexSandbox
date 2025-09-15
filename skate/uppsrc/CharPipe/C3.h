/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/include/charPipeline/C3.h $
    
    4     7/18/00 7:35p John
    Added control include and removed C3Camera include.
    
    3     6/27/00 6:00p John
    Removed quantize.h dependancy.  Moved macros to C3Util.h
    
    2     2/04/00 6:05p John
    Untabified code.
    
    10    11/18/99 1:11p John
    Added math include
    
    9     11/11/99 1:09p John
    Moved C3HieExt.h above C3AnmExt.h
    
    8     9/29/99 4:23p John
    Changed header to make them all uniform with logs.
    
  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _C3_H
#define _C3_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <float.h>
#include <assert.h>
#include <stdarg.h>
#include <time.h>

#include <Dolphin/types.h>
#include <Dolphin/GeoTypes.h>
#include <CharPipe/structures.h>
#include <CharPipe/control.h>

#include <CharPipe/C3Defines.h>
#include <CharPipe/C3Util.h>
#include <CharPipe/C3Math.h>
#include <CharPipe/C3HieExt.h>
#include <CharPipe/C3AnmExt.h>
#include <CharPipe/C3CnvOpz.h>
#include <CharPipe/C3Debug.h>
#include <CharPipe/C3GeoExt.h> 
#include <CharPipe/C3Options.h>
#include <CharPipe/C3Out.h> 
#include <CharPipe/C3Stats.h>
#include <CharPipe/C3Texture.h>

#endif