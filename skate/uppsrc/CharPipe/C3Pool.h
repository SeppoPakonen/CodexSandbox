/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3Pool.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/c3/include/C3Pool.h $
    
    4     2/04/00 6:05p John
    Untabified code.
    
    3     1/20/00 1:13p John
    Added stitching (work in progress).
    
    2     12/06/99 3:20p John
    Added numerous other pools for efficiency.
    
    8     10/01/99 11:12a John
    Added pools for stats and options per geom. object.
    
    7     9/29/99 4:21p John
    Changed header to make them all uniform with logs.
    
    6     9/21/99 6:39p John
    Added pools for Image, SrcImage, and Palette.
    
    5     9/20/99 6:58p John
    Renamed C3TextCoord to C3TexCoord.

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _C3POOL_H
#define _C3POOL_H

#if 0
extern "C" {
#endif

void            C3InitPools();
void            C3ClearPools();

C3Position*     C3PoolPosition();
C3Normal*       C3PoolNormal();
C3Color*        C3PoolColor();
C3TexCoord*     C3PoolTexCoord();
C3Texture*      C3PoolTexture();
C3Vertex*       C3PoolVertex();
C3PtrLink*      C3PoolPtrLink();
C3Primitive*    C3PoolPrimitive();
C3Image*        C3PoolImage();
C3SrcImage*     C3PoolSrcImage();
C3Palette*      C3PoolPalette();
C3StatsObj*     C3PoolStatsObj();
C3OptionObj*    C3PoolOptionObj();
C3SeqTrack*     C3PoolSeqTrack();
C3Track*        C3PoolTrack();
C3KeyFrame*     C3PoolKeyFrame();
C3Control*      C3PoolControl();
C3HierNode*     C3PoolHierNode();
C3GeomObject*   C3PoolGeomObject();
C3String*       C3PoolString();

#if 0
}
#endif

#endif  // _C3POOL_H
