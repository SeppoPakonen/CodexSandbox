/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3CnvOpz.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/include/charPipeline/c3/C3CnvOpz.h $
    
    7     2/29/00 7:16p John
    Removed arguments from C3CompressData.
    
    6     2/04/00 6:06p John
    Untabified code.
    
    5     1/20/00 1:13p John
    Added stitching (work in progress).
    
    4     1/04/00 1:11p John
    Added better support for second texture channel.
    
    3     12/20/99 11:20a John
    Added second texture channel.
    Fixed stripping bug when joining primitives w/ different textures.
    
    2     12/08/99 7:23p John
    Added animation quantization.
    
    17    10/01/99 11:13a John
    Can quantize color two ways (with and without alpha)
    
    16    9/29/99 4:23p John
    Changed header to make them all uniform with logs.
    
  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _C3CNVOPZ_H
#define _C3CNVOPZ_H

#if 0
extern "C" {
#endif // __cplusplus

#define C3_TARGET_POSITION   0x0001
#define C3_TARGET_TEXCOORD   0x0002
#define C3_TARGET_NORMAL     0x0004
#define C3_TARGET_ALL        0x0007 // Only includes position, texcoords, and normals
#define C3_TARGET_COLOR      0x0010
#define C3_TARGET_COLORALPHA 0x0020
#define C3_TARGET_KEYFRAME   0x0040
#define C3_TARGET_POS_MATRIX 0x0080

void    C3CompressData              ( );
u8      C3ComputeQuantizationShift  ( u8 quantInfo, f32 min, f32 max );

#if 0
}
#endif // __cplusplus

#endif // _C3CNVOPZ_H
