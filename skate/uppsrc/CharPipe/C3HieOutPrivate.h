/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3HieOutPrivate.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/build/charPipeline/c3/include/C3HieOutPrivate.h $
    
    4     11/13/00 1:28p John
    Added separate constant for bones that are unused in an actor bone.
    
    3     2/04/00 6:05p John
    Untabified code.
    
    2     1/31/00 4:15p John
    Added pruning of unnecessary bones and animations.
    Finished stitching with FIFO matrix cache and stripping.
    
    2     9/29/99 4:19p John
    Changed header to make them all uniform with logs.
    
  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _C3HIEOUTPRIVATE_H
#define _C3HIEOUTPRIVATE_H


#if 0
extern "C" {
#endif

#define C3_HIERNODE_NO_GEOM     0xFFFF
#define C3_HIERNODE_NOT_USED    0xFFFE

void C3WriteHierarchy( char* path, char* name );
void C3ConvertActor( C3Actor* actor );

#if 0
}
#endif


#endif // _C3HIEOUTPRIVATE_H