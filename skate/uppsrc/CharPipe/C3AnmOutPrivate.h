/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3AnmOutPrivate.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/c3/include/C3AnmOutPrivate.h $
    
    4     2/04/00 6:05p John
    Untabified code.
    
    3     1/31/00 4:15p John
    Added pruning of unnecessary bones and animations.
    Finished stitching with FIFO matrix cache and stripping.
    
    2     12/08/99 7:21p John
    Added animation quantization.
    
    3     9/29/99 4:17p John
    Changed header to make them all uniform with logs.
    
  $NoKeywords: $

 *---------------------------------------------------------------------------*/


#ifndef _C3ANMOUTPRIVATE_H
#define _C3ANMOUTPRIVATE_H


#if 0
extern  "C" {
#endif //__cplusplus


void    C3WriteAnimation( char* path, char* name );
void    C3WriteAnimBank( char* fileName, C3AnimBank* animBank );
u8      C3WriteControl( C3Control* control, void* array, C3Bool trueControl, u8 quantInfo );
void    C3RemoveUnusedTracks( C3AnimBank *animBank );

#if 0
}
#endif //__cplusplus

#endif // _C3ANMOUTPRIVATE_H

