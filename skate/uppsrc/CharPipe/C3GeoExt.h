/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3GeoExt.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/include/charPipeline/c3/C3GeoExt.h $
    
    12    12/04/00 7:54p John
    Changed GPL version number to incorporate multitexturing.
    
    11    8/14/00 6:13p John
    Added user-defined data to ANM, ACT, and GPL formats.
    
    10    3/23/00 11:11a John
    Changed version date for updated GPL format.
    
    9     3/14/00 1:32p John
    Moved display priority from GPL to ACT.
    
    8     2/29/00 7:17p John
    Moved filter constants to C3Texture.h.
    
    7     2/17/00 2:34p John
    Cleaned up code.
    
    6     2/04/00 6:06p John
    Untabified code.
    
    5     1/20/00 1:13p John
    Added stitching (work in progress).
    
    4     1/04/00 1:11p John
    Added better support for second texture channel.
    
    3     12/08/99 7:23p John
    Cleaned up code.
    
    2     12/02/99 2:21p John
    Added sorting of primitives in display list.
    
    18    11/18/99 1:12p John
    Removed convex poly (was never used)
    
    17    11/11/99 1:10p John
    Commented out material extraction.
    
    16    10/21/99 6:22p John
    Added drawing priority.
    
    15    10/14/99 4:32p John
    Added GPL version and creation date.
    
    14    9/29/99 4:24p John
    Changed header to make them all uniform with logs.
    
  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _C3GEOEXT_H
#define _C3GEOEXT_H

#if 0
extern "C" {
#endif // __cplusplus

/********************************/

#define C3_GPL_VERSION           12012000  // Must be compatible with Previewer

#define C3_CCW                   0
#define C3_CW                    1

#define C3_MAX_TEXTURES          1 // Number of textures referred per primitive

#define C3_PRIM_TRI              1 // Primitives will be sorted in the display
#define C3_PRIM_QUAD             2 // list in ascending order of these constants.
#define C3_PRIM_STRIP            3
#define C3_PRIM_FAN              4
#define C3_PRIM_POINT            5
#define C3_PRIM_LINE             6
#define C3_PRIM_LINE_STRIP       7

/*>*******************************(*)*******************************<*/
// Extraction functions
/*>*******************************(*)*******************************<*/

void C3BeginObject          ( char* identifier );
void C3EndObject            ( );

void C3BeginPolyPrimitive   ( );
void C3EndPolyPrimitive     ( );

void C3BeginLinePrimitive   ( );
void C3EndLinePrimitive     ( );

void C3BeginVertex          ( );
void C3SetPosition          ( float X, float Y, float Z );
void C3SetTexCoord          ( float s, float t, u8 channel );
void C3SetColor             ( u8 Red, u8 Green, u8 Blue, u8 Alpha );
void C3SetNormal            ( float nX, float nY, float nZ );
void C3SetVertexMatrix      ( char* boneName );
void C3EndVertex            ( );

void C3SetGPLUserData       ( u32 size, void *data );
void C3GetGPLUserData       ( u32 *size, void **data );

#if 0
}
#endif // __cplusplus


#endif // _C3GEOEXT_H
