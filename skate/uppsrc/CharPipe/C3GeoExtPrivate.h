/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3GeoExtPrivate.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/c3/include/C3GeoExtPrivate.h $
    
    13    11/13/00 1:27p John
    Added pivot point offsets for rotation and scale since they may not
    necessarily be the same as the translation.
    
    12    6/27/00 5:59p John
    Added optimization to not include color indices if there is only one
    color for a geometry object (assumes runtime will set register color).
    Added optimization to use GX_REPLACE as Tev op if there is texture
    data, no lighting, and all the vertex colors are white.
    
    11    3/22/00 4:51p John
    Removed transparency field from C3Primitive.
    
    10    3/14/00 1:31p John
    Moved display priority from GPL to ACT.
    
    9     2/07/00 3:02p John
    Removed gx.h include.
    
    8     2/04/00 6:05p John
    Untabified code.
    
    7     1/31/00 4:15p John
    Added pruning of unnecessary bones and animations.
    Finished stitching with FIFO matrix cache and stripping.
    
    6     1/20/00 1:12p John
    Added stitching (work in progress).
    
    5     1/04/00 12:07p John
    Added better support for second texture channel.
    
    4     12/20/99 11:20a John
    Added second texture channel.
    
    3     12/08/99 7:21p John
    Got rid of colorCombineFunc.
    
    2     12/02/99 2:20p John
    Removed unused material and skinning code.
    
    16    10/21/99 6:21p John
    Added drawing priority.
    
    15    10/01/99 11:11a John
    Can quantize color two ways (with and without alpha)
    
    14    9/29/99 4:19p John
    Changed header to make them all uniform with logs.
    Added position and normal quantization.
    
    13    9/20/99 6:58p John
    Renamed C3TextCoord to C3TexCoord for consistency and
    removed unused variables.
    
    12    9/17/99 9:32a John
    Added C3Options and cleaned up code.
    
    11    9/14/99 6:40p John
    Added stats per object.
    Moved some #define constants that were also in another file.
    
    10    9/03/99 1:56p John
    Added quantization of color and indices.
    Added removal of null primitives.
    
    9     8/23/99 4:00p John
    
    8     8/19/99 3:46p John
    Added analysis and welding of texture coordinates.
    Added analysis and welding of texture coordinates.
    
    7     8/15/99 10:28p John
    Changed minPosition and maxPosition to be of type C3Position instead of
    f32 so that positions can be hashed by distance from origin.

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _C3GEOEXTPRIVATE_H
#define _C3GEOEXTPRIVATE_H

#if 0
extern "C" {
#endif // __cplusplus


/********************************/

typedef struct
{
    DSLink  link;
    u32     index;
    void*   ptr;

} C3PtrLink, *C3PtrLinkPtr;

/********************************/

typedef struct
{
    DSLink  link;
    u32     index;
    float   x;
    float   y;
    float   z; 

} C3Position, *C3PositionPtr, 
  C3Normal, *C3NormalPtr,
  C3Vector, *C3VectorPtr;

/********************************/

typedef struct
{
    DSLink  link;
    u32     index;
    u8      r;
    u8      g;
    u8      b;
    u8      a;

} C3Color, *C3ColorPtr;

/********************************/

typedef struct
{
    DSLink  link;
    u32     index;
    float   s;
    float   t;

} C3TexCoord, *C3TexCoordPtr;

/********************************/

typedef struct
{
    DSLink        link;
    u32           index;

    C3Position*   position;
    C3Color*      color;
    C3Normal*     normal;
    C3TexCoord*   tCoord[C3_MAX_TEXTURES];

    u16*          boneIndex;        // pointer to the index of C3HierNode

} C3Vertex, *C3VertexPtr;

/********************************/

typedef struct
{
    DSLink      link;
    u32         index;

    u8          type;

    u8          numVertex;
    DSList      vertexPtrList;

    C3Texture*  texture[C3_MAX_TEXTURES];
    C3Normal*   normal;
    C3Color*    color;
    
    u16         texWrap[C3_MAX_TEXTURES];

}   C3Primitive, *C3PrimitivePtr;

/********************************/

typedef struct
{
    DSLink          link;
    u32             index;  // Index in the geoPaletteFile

    char*           identifier;

    char*           texPaletteName;

    DSList          positionList;
    C3Position      minPosition;
    C3Position      maxPosition;
    f32             maxPosDistance;
    f32             minPosDistance;
    char*           pivotTransformedBy; // Error check when transforming w/ pivots

    DSList          texCoordList[C3_MAX_TEXTURES];
    f32             minTexCoord[C3_MAX_TEXTURES];
    f32             maxTexCoord[C3_MAX_TEXTURES];
    f32             minTexDistance[C3_MAX_TEXTURES];
    f32             maxTexDistance[C3_MAX_TEXTURES];

    DSList          colorList;
    f32             maxColDistance;
    f32             minColDistance;
    C3Bool          useVertexAlpha;
    C3Bool          colorAllWhite;

    DSList          normalList;
    f32             minNormalX;
    f32             maxNormalX;

    DSList          primitiveList;

    DSList          vertexList;

    C3Texture*      textures[C3_MAX_TEXTURES];
    C3Color*        color;

    u16             indexColQuant;
    u16             indexPosQuant;
    u16             indexTexQuant[C3_MAX_TEXTURES];
    u16             indexNorQuant;
    u16             indexPosMatQuant;

    C3StatsObj*     stats;
    C3OptionObj*    options;

} C3GeomObject, *C3GeomObjectPtr;

/********************************/


/*>*******************************(*)*******************************<*/
// Internal functions
/*>*******************************(*)*******************************<*/

void            C3InitGeometryExtraction    ( );

C3GeomObject*   C3GetNextObject             ( C3GeomObject* obj );
C3GeomObject*   C3GetObjectFromIdentifier   ( char* identifier );
DSList*         C3GetGeomObjList            ( );
u32             C3GetGeomObjCount           ( );

C3Primitive*    C3GetCurrentPrim            ( );
C3GeomObject*   C3GetCurrentGeomObj         ( );

#if 0
}
#endif // __cplusplus


#endif // _C3GEOEXTPRIVATE_H
