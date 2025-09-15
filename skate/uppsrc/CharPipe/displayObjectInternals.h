/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     displayObjectInternals.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/charPipeline/geoPalette/displayObjectInternals.h $
    
    6     12/04/00 7:59p John
    Added multitexturing ability (work in progress).
    
    5     8/04/00 5:14p John
    Detabified.
    
    4     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef DISPLAYOBJECTINTERNALS_H
#define DISPLAYOBJECTINTERNALS_H

/********************************/
#if 0
extern "C" {
#endif

/********************************/
#include <CharPipe/TexPalette.h>

/********************************/
extern DSCache  DODisplayCache;
extern u8       DOCacheInitialized;

/********************************/
#define DO_MAX_TEXTURES     1

/********************************/
#define DO_DISPLAY_ON       TRUE
#define DO_DISPLAY_OFF      FALSE

/********************************/
#define DISPLAY_STATE_TEXTURE           1
#define DISPLAY_STATE_VCD               3
#define DISPLAY_STATE_TEXTURE_COMBINE   4
#define DISPLAY_STATE_MTXLOAD           5

/********************************/
#define DO_CLAMP            0
#define DO_REPEAT           1
#define DO_MIRROR           2

/********************************/
#define DO_NEAR             0
#define DO_LINEAR           1
#define DO_NEAR_MIP_NEAR    2
#define DO_LIN_MIP_NEAR     3
#define DO_NEAR_MIP_LIN     4
#define DO_LIN_MIP_LIN      5

/*Texture state setting layout
byte 4                          byte 0
magFilter(4 bits) minFilter(4 bits) wrapT(4 bits) wrapS(4 bits) textureIndex(16 bits)
*/

/********************************/
typedef struct
{
    Ptr positionArray;
    u16 numPositions;
    u8  quantizeInfo;
    u8  compCount;

} DOPositionHeader, *DOPositionHeaderPtr;

/********************************/
typedef struct
{
    Ptr colorArray;
    u16 numColors;
    u8  quantizeInfo;
    u8  compCount;

} DOColorHeader, *DOColorHeaderPtr;

/********************************/
typedef struct
{
    Ptr textureCoordArray;
    u16 numTextureCoords;
    u8  quantizeInfo;
    u8  compCount;

    char            *texturePaletteName;
    TEXPalettePtr   texturePalette;

} DOTextureDataHeader, *DOTextureDataHeaderPtr;

/********************************/
typedef struct
{
    Ptr normalArray;
    u16 numNormals;
    u8  quantizeInfo;
    u8  compCount;
    f32 ambientPercentage;

} DOLightingHeader, *DOLightingHeaderPtr;

/********************************/
typedef struct
{
    u8  id;

    u8  pad8;
    u16 pad16;

    u32 setting;

    Ptr primitiveList;
    u32 listSize;

} DODisplayState, *DODisplayStatePtr;

/********************************/
typedef struct
{
    Ptr                 primitiveBank;

    DODisplayStatePtr   displayStateList;
    u16                 numStateEntries;

    u16                 pad16;

} DODisplayHeader, *DODisplayHeaderPtr;

/********************************/
typedef struct
{
    DOPositionHeaderPtr     positionData;
    DOColorHeaderPtr        colorData;
    DOTextureDataHeaderPtr  textureData;
    DOLightingHeaderPtr     lightingData;

    DODisplayHeaderPtr      displayData;

    u8                      numTextureChannels;
    u8                      pad8;
    u16                     pad16;
} DOLayout, *DOLayoutPtr;

/********************************/
typedef struct
{
    DOLayoutPtr layout;
    char        *name;

} GEODescriptor, *GEODescriptorPtr;

/********************************/
#if 0
}
#endif

#endif
