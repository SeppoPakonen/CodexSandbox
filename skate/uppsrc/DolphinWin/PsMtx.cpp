/*---------------------------------------------------------------------------*
  Project: matrix vector Library
  File:    psmtx.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.     They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.


  $Log: /Dolphin/build/libraries/mtx/src/psmtx.c $NoKeywords: $
    
    2     7/12/00 4:41p John
    Substitues MTXConcat and MTXMultVecArray with their paired-singles
    equivalent for Gekko nondebug builds.
    
    1     5/10/00 1:48p Hirose
    moved paired-single matrix stuff into an another source file
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <Dolphin/platform.h>

#include <math.h>
#include <Dolphin/mtx.h>

