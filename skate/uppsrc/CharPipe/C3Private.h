/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3Private.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/build/charPipeline/c3/include/C3Private.h $
    
    6     7/18/00 7:24p John
    Removed camera include.
    
    5     2/29/00 7:16p John
    Removed C3Analysis.* code (unused and out of date).
    
    4     2/07/00 3:03p John
    Added GXCommandList.h include, and removed C3Txt.h include.
    
    3     2/04/00 6:05p John
    Untabified code.
    
    2     1/04/00 1:11p John
    
    15    10/13/99 3:01p John
    
    14    10/01/99 11:12a John
    Had to move C3Pool.h to the end.
    
    13    9/29/99 4:21p John
    Changed header to make them all uniform with logs.
    
    12    9/24/99 10:32a John
    
    11    9/17/99 9:32a John
    Added C3Options and cleaned up code.
    
    10    9/14/99 6:41p John
    Added stats include.
    
    9     8/15/99 10:27p John
    Added analysis include.

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _C3PRIVATE_H
#define _C3PRIVATE_H

#include <CharPipe/C3.h>
#include <CharPipe/actor.h>
#include <CharPipe/normalTable.h>

#include <Dolphin/GXCommandList.h>

// Texture
#include "C3TexturePrivate.h"

// Geometry
#include "C3GeoExtPrivate.h" // extraction
#include "C3GeoCnvPrivate.h" // conversion
#include "C3GeoOutPrivate.h" // output
 
// Hierarchy
#include "C3HieExtPrivate.h" // extraction
#include "C3HieOutPrivate.h" // output

// Animation
#include "C3AnmExtPrivate.h"
#include "C3AnmOutPrivate.h"

// Optimization
#include "C3CnvOpzPrivate.h"

// Information and statistics about conversion
#include "C3StatsPrivate.h"

// Options
#include "C3OptionsPrivate.h"

// Memory management (Pool)
#include "C3Pool.h" 

#endif  //_C3PRIVATE_H