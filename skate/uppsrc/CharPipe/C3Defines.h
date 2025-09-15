/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3Defines.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/include/charPipeline/c3/C3Defines.h $
    
    6     6/27/00 6:01p John
    Uncommented C3_QUADS_TO_FANS define.
    
    5     2/16/00 6:34p John
    Cleaned up header.
    
    4     2/15/00 4:22p John
    Added define to export colors always in big endian.
    Moved quads to fans define here.
    
    3     2/04/00 6:06p John
    Untabified code.
    
    2     1/04/00 6:58p John
    Added little/big endian output  option
    
    2     9/29/99 4:24p John
    Changed header to make them all uniform with logs.
    Added C3_MAX_PATH
    
  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _C3DEFINES_H
#define _C3DEFINES_H

// Defining the following constant will enable debugging
// compilation for the module C3 
// - Memory allocation tracking
// - Custom assertion
//#define C3_DEBUG

// Special code to generate normal table in C3ConvertPositionData
//#define C3_GENERATE_NORMAL_TABLE

// GX requires colors to be in PPC format (big endian) even
// when outputting for PC format (little endian).
#define C3_COLOR_ALWAYS_TO_BIG_ENDIAN

// Convert all quads to fans.  Should uncomment when the target is hardware,
// since non-coplanar quads do not have backface rejection issues and also
// since quads are faster than fans.  Should define C3_QUADS_TO_FANS if target
// is other than hardware.
//#define C3_QUADS_TO_FANS

#define C3_FALSE                0
#define C3_TRUE                 1

#define C3_OUTPUT_BIG_ENDIAN    0   // i.e. PowerPC destination
#define C3_OUTPUT_LITTLE_ENDIAN 1   // i.e. Intel destination

#define C3_MAX_PATH             256

#endif  // _C3DEFINES_H
