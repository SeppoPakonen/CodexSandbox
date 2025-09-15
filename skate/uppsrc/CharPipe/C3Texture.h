/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3Texture.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/include/charPipeline/c3/C3Texture.h $
    
    5     2/29/00 7:19p John
    Moved filter constants here from C3GeoExt.h.
    Renamed C3BeginTex to C3BeginTexture and C3EndTex to C3EndTexture.
    
    4     2/14/00 6:05p John
    Changed texture dimension check.  Width and height must be a power of
    two when mipmapping.
    
    3     2/04/00 6:06p John
    Untabified code.
    
    2     1/04/00 1:12p John
    Added better support for second texture channel.
    
    9     9/20/99 6:56p John
    Formatted and restructured function prototypes.
    
    8     9/14/99 6:44p John
    Changed C3TexFmt enum constant names.
    Added function to set LOD parameters for mipmapping.
    
    7     9/03/99 2:00p John
    Changed texture formats (in accordance with GxEnum.h.
    
    6     7/19/99 2:20p Rod
    Updated the texture type to match the order in the tplconv library
    
    5     7/09/99 2:43p Rod
    Changed the define flag for the tiling information
    
    
    4     7/01/99 3:29p Rod
    Moved the Initialization and clean up routine from the public header to
    the private header
    
    Added function and defines for the texture wrapping states
    
    3     6/02/99 12:03p Rod
    Added functions to write and compress textures

  $NoKeywords: $

 *---------------------------------------------------------------------------*/


#ifndef __C3_TEXTURE_H_
#define __C3_TEXTURE_H_


#if 0
extern "C"
{
#endif

#define C3_MAX_TEXTURE_DIMENSION    1024

// IMPORTANT: These constants must match perfectly with GxEnum.h
#define C3_FILTER_NEAR           0 //GX_NEAR
#define C3_FILTER_LINEAR         1 //GX_LINEAR
#define C3_FILTER_LIN_MIP_LIN    5 //GX_LIN_MIP_LIN

typedef enum
{
    I4 = 0,
    I8,
    IA4,
    IA8,
    CI4,
    CI8,
    CI14_X2, 
    RGB565,
    RGB5A3,
    RGBA8,
    CMPR,
    DONT_KNOW = 0xF
}   C3TexFmt;

typedef enum
{
    C3_CLAMP_S  = 0x00,
    C3_REPEAT_S = 0x01,
    C3_MIRROR_S = 0x02,

    C3_CLAMP_T  = 0x00,
    C3_REPEAT_T = 0x10,
    C3_MIRROR_T = 0x20

} C3Wrap;

void     C3BeginTexture         ( u8 channel );
void     C3SetImage             ( char* fileName );
void     C3SetImageAlpha        ( char* fileName );
void     C3SetPalImage          ( char* fileName );
void     C3SetPalAlpha          ( char* fileName );
void     C3SetTexFmt            ( C3TexFmt texFmt );
void     C3SetTexTiling         ( u8 wrap );
void     C3SetTexFilterType     ( u8 method );
void     C3SetImgLOD            ( u8 minLOD, u8 maxLOD, u8 baseLOD );
void     C3EndTexture           ( void );

C3TexFmt C3GetTexFmt            ( char* str );
char*    C3GetTexFmtStr( C3TexFmt texFmt );

#if 0
}
#endif

#endif  // __C3_TEXTURE_H
