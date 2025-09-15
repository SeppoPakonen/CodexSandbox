/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3StatsPrivate.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/c3/include/C3StatsPrivate.h $
    
    3     2/04/00 6:05p John
    Untabified code.
    
    2     1/04/00 1:11p John
    Added better support for second texture channel.
    
    6     10/01/99 11:13a John
    Now with C3StatsPool(), no longer need to clean stats.
    The stats identifier no longer has its own buffer too.
    
    5     9/29/99 4:22p John
    
    4     9/21/99 6:40p John
    Changed function names.
    
    3     9/17/99 9:33a John
    Added C3Options and cleaned up code.
    
    2     9/14/99 6:41p John
    Added stats per object.
    Separated stats from C3GeomObject to C3Stats and C3StatsObj
    
  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _C3STATSPRIVATE_H
#define _C3STATSPRIVATE_H


#if 0
extern "C" {
#endif

void        C3InitStats             ( );
void        C3AddStatsObj           ( C3StatsObj* obj );

C3StatsObj* C3NewStatsObj           ( char* identifier );
C3Stats*    C3GetStats              ( );
DSList*     C3GetStatsObjList       ( );
C3StatsObj* C3GetNextStatsObj       ( C3StatsObj* obj );
C3StatsObj* C3GetStatsObjFromId     ( char* identifier );

u32         C3StatsGetNumPrim       ( u8 primType );
u32         C3StatsGetNumNullPrim   ( void );
u32         C3StatsGetTotal         ( u16 target, u8 channel );
u32         C3StatsGetTotalWelded   ( u16 target, u8 channel );

#if 0
}
#endif


#endif  // _C3STATSPRIVATE_H
