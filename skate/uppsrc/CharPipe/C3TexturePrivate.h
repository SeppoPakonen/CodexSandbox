/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3TexturePrivate.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/c3/include/C3TexturePrivate.h $
    
    3     2/04/00 6:05p John
    Untabified code.
    
    2     1/31/00 4:15p John
    Added pruning of unnecessary bones and animations.
    Finished stitching with FIFO matrix cache and stripping.
    
    7     9/29/99 4:22p John
    Changed header to make them all uniform with logs.
    
    6     9/20/99 6:55p John
    Changed filterType name.
    
    5     9/14/99 6:42p John
    Added LOD parameters for mipmapping.
    
    4     9/03/99 1:58p John
    Modified tiling to 8 bit data structure.
    Added filter data structure to C3Texture (Tex. filter is work in
    progress)

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _C3_TEXTUREPRIVATE_H
#define _C3_TEXTUREPRIVATE_H

#if 0
extern "C"
{
#endif

typedef struct 
{
    DSLink  link;
    u32     index;

    char*   fileName;

}   C3SrcImage;


typedef struct 
{
    DSLink      link;
    u32         index;

    C3SrcImage* colorMap;
    C3SrcImage* alphaMap;

    C3TexFmt    texFmt;

    u8          minLOD;
    u8          maxLOD;
    u8          baseLOD;

}   C3Image;


typedef struct 
{
    DSLink      link;
    u32         index;

    C3SrcImage* colorMap;
    C3SrcImage* alphaMap;

    C3TexFmt    texFmt;

}   C3Palette;


typedef struct 
{
    DSLink      link;
    u32         index;

    C3Image*    img;
    C3Palette*  pal;
    
    u8          tiling; // T is 4 upper bits S is 4 lower bits
    u8          filterType; 

}   C3Texture;

void    C3InitializeTextures    ( void );
C3Bool  C3CompareTextures       ( void* t1, void* t2 );
void    C3WriteTextures         ( char* path, char* name );
void    C3CompressTextureData   ( void );

#if 0
}
#endif

#endif  // _C3_TEXTUREPRIVATE_H
