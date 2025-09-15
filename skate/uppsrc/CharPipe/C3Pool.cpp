/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3Pool.c]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/c3/src/C3Pool.c $
    
    4     2/04/00 6:07p John
    Untabified code.
    
    3     1/20/00 1:14p John
    Added stitching (work in progress).
    Added C3 string table for efficient management of strings.
    
    2     12/06/99 3:21p John
    Added numerous other pools for efficiency.
    
    10    10/01/99 11:16a John
    Added pools for stats and options per object.
    
    9     9/21/99 6:39p John
    Added pools for Image, SrcImage, and Palette.
    Changed constants to use less initial memory.
    
    8     9/20/99 7:05p John
    Renamed C3TextCoord to C3TexCoord for consistency.
    
    7     6/08/99 1:40p Rod
    Completed all function headers
    
    6     6/02/99 11:30a Rod
    Added a new pool for PtrLink structures

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#include "C3Private.h"

#define C3_NUM_POSITIONS    4096
#define C3_NUM_NORMAL       4096
#define C3_NUM_COLOR         256
#define C3_NUM_TEXCOORD      256
#define C3_NUM_TEXTURE       128
#define C3_NUM_VERTEX       4096
#define C3_NUM_PRIMITIVE    4096
#define C3_NUM_IMAGE         128
#define C3_NUM_SRCIMAGE      128 
#define C3_NUM_PALETTE       128
#define C3_NUM_STATSOBJ      128
#define C3_NUM_OPTIONOBJ     128
#define C3_NUM_SEQTRACK       32
#define C3_NUM_TRACK          32
#define C3_NUM_KEYFRAME      128
#define C3_NUM_CONTROL        64
#define C3_NUM_HIERNODE       64
#define C3_NUM_GEOMOBJECT     32
#define C3_NUM_STRING         32


typedef struct
{
    struct C3Pool*  expansion;  // expansion pool if needed 
    u16             numElem;    // max number of element in pool
    u16             elemSize;   // size of the elements
    u16             cursor;     // current available position in the pool
    Ptr             data;       // data of the pool
} C3Pool;


/*---------------------------------------------------------------------------*
  Name:         C3InitPool
  
  Description:  Initialize the specified pool with the number of element and
                the proper size.

  Arguments:    pool     - pointer to the pool to initialize
                numElem  - number of elements to allocate
                elemSize - size of the element to allocate
                  
  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3InitPool( C3Pool* pool, u16 numElem, u16 elemSize )
{
    C3_ASSERT( pool );

    pool->expansion = NULL;
    pool->numElem   = numElem;
    pool->elemSize  = elemSize;
    pool->cursor = 0;
    pool->data = C3_CALLOC( numElem, elemSize ); 
    C3_ASSERT( pool->data );
}


/*---------------------------------------------------------------------------*
  Name:         C3ClearPool

  Description:  Deallocate the memory used for the specified pool

  Arguments:    pool - pointer to pool to free
                  
  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3ClearPool( C3Pool* pool )
{
    C3_ASSERT( pool );

    if( pool->expansion )
    {
        C3ClearPool( (C3Pool*)pool->expansion );
        C3_FREE( pool->expansion );
    }

    pool->numElem  = 0;
    pool->elemSize = 0;
    pool->cursor   = 0;
    C3_FREE( pool->data );
}


/*---------------------------------------------------------------------------*
  Name:         C3GetNextPoolElem

  Description:  Returns the next available element in the pool allocating a 
                sub pool if needed

  Arguments:    pool - pool to get the next element from
                  
  Returns:      pointer to the next 
/*---------------------------------------------------------------------------*/
Ptr
C3GetNextPoolElem( C3Pool* pool )
{
    Ptr nextElem = NULL;

    C3_ASSERT( pool );

    // if the current pool is filled
    if( (pool->cursor >= pool->numElem) &&
        (pool->expansion != NULL ) )
    {
        // get the next element from the expansion
        return C3GetNextPoolElem( (C3Pool*)pool->expansion );
    }

    // get the next element
    *((u32*)&nextElem) = (u32)pool->data + 
                         (u32)pool->cursor * 
                         (u32)pool->elemSize;

    // Increment the cursor 
    pool->cursor++;
    // If the current pool is filled
    if( pool->cursor >= pool->numElem )
    {
        // Create an expansion
        pool->expansion = (struct C3Pool*)C3_CALLOC( 1, sizeof( C3Pool ));
        C3_ASSERT( pool->expansion );
        C3InitPool( (C3Pool*)pool->expansion, pool->numElem, pool->elemSize );
    }  

    return nextElem;
}

/******** Pools specific to the converter **************/

C3Pool  positionPool;
C3Pool  normalPool;
C3Pool  colorPool;
C3Pool  texCoordPool;
C3Pool  texPool;
C3Pool  vertexPool;
C3Pool  ptrLinkPool;
C3Pool  primitivePool;
C3Pool  imagePool;
C3Pool  srcImagePool;
C3Pool  palettePool;
C3Pool  statsObjPool;
C3Pool  optionObjPool;
C3Pool  seqTrackPool;
C3Pool  trackPool;
C3Pool  keyFramePool;
C3Pool  controlPool;
C3Pool  hierNodePool;
C3Pool  geomObjectPool;
C3Pool  stringPool;


/*---------------------------------------------------------------------------*
  Name:         C3InitPools
  
  Description:  Initialize all the pools specific to the converter

  Arguments:    NONE
                  
  Returns:      NONE
/*---------------------------------------------------------------------------*/
void    
C3InitPools( void )
{
    C3InitPool( &positionPool, C3_NUM_POSITIONS, sizeof(C3Position) );
    C3InitPool( &normalPool, C3_NUM_NORMAL, sizeof(C3Normal) );
    C3InitPool( &colorPool, C3_NUM_COLOR, sizeof(C3Color) );
    C3InitPool( &texCoordPool, C3_NUM_TEXCOORD, sizeof(C3TexCoord) );
    C3InitPool( &texPool, C3_NUM_TEXTURE, sizeof(C3Texture) );
    C3InitPool( &vertexPool, C3_NUM_VERTEX, sizeof(C3Vertex) );
    C3InitPool( &ptrLinkPool, C3_NUM_VERTEX, sizeof(C3PtrLink) );
    C3InitPool( &primitivePool, C3_NUM_PRIMITIVE, sizeof(C3Primitive) );
    C3InitPool( &imagePool, C3_NUM_IMAGE, sizeof(C3Image) );
    C3InitPool( &srcImagePool, C3_NUM_SRCIMAGE, sizeof(C3SrcImage) );
    C3InitPool( &palettePool, C3_NUM_PALETTE, sizeof(C3Palette) );
    C3InitPool( &statsObjPool, C3_NUM_STATSOBJ, sizeof(C3StatsObj) );
    C3InitPool( &optionObjPool, C3_NUM_OPTIONOBJ, sizeof(C3OptionObj) );
    C3InitPool( &seqTrackPool, C3_NUM_SEQTRACK, sizeof(C3SeqTrack) );
    C3InitPool( &trackPool, C3_NUM_TRACK, sizeof(C3Track) );
    C3InitPool( &keyFramePool, C3_NUM_KEYFRAME, sizeof(C3KeyFrame) );
    C3InitPool( &controlPool, C3_NUM_CONTROL, sizeof(C3Control) );
    C3InitPool( &hierNodePool, C3_NUM_HIERNODE, sizeof(C3HierNode) );
    C3InitPool( &geomObjectPool, C3_NUM_GEOMOBJECT, sizeof(C3GeomObject) );
    C3InitPool( &stringPool, C3_NUM_STRING, sizeof(C3String) );
}


/*---------------------------------------------------------------------------*
  Name:         C3ClearPools
  
  Description:  Clear all the pools specific to the converter

  Arguments:    NONE
                  
  Returns:      NONE
/*---------------------------------------------------------------------------*/
void    
C3ClearPools( void )
{
    C3ClearPool( &positionPool );
    C3ClearPool( &normalPool );
    C3ClearPool( &colorPool );
    C3ClearPool( &texCoordPool );
    C3ClearPool( &texPool );
    C3ClearPool( &vertexPool );
    C3ClearPool( &ptrLinkPool );
    C3ClearPool( &primitivePool );
    C3ClearPool( &imagePool );
    C3ClearPool( &srcImagePool );
    C3ClearPool( &palettePool );
    C3ClearPool( &statsObjPool );
    C3ClearPool( &optionObjPool );
    C3ClearPool( &seqTrackPool );
    C3ClearPool( &trackPool );
    C3ClearPool( &keyFramePool );
    C3ClearPool( &controlPool );
    C3ClearPool( &hierNodePool );
    C3ClearPool( &geomObjectPool );
    C3ClearPool( &stringPool );
}


/*---------------------------------------------------------------------------*
  Name:         C3PoolPosition
  
  Description:  Returns a pointer to the next available position

  Arguments:    NONE
                  
  Returns:      pointer to next available
/*---------------------------------------------------------------------------*/
C3Position*     
C3PoolPosition( void )
{
    return (C3Position*)C3GetNextPoolElem( &positionPool );
}


/*---------------------------------------------------------------------------*
  Name:         C3PoolNormal
  
  Description:  Returns a pointer to the next available normal

  Arguments:    NONE
                  
  Returns:      pointer to next available normal
/*---------------------------------------------------------------------------*/
C3Normal*     
C3PoolNormal( void )
{
    return (C3Normal*)C3GetNextPoolElem( &normalPool );
}


/*---------------------------------------------------------------------------*
  Name:         C3PoolColor
  
  Description:  Returns a pointer to the next available color

  Arguments:    NONE
                  
  Returns:      pointer to next available color
/*---------------------------------------------------------------------------*/
C3Color*        
C3PoolColor( void )
{
    return (C3Color*)C3GetNextPoolElem( &colorPool );
}


/*---------------------------------------------------------------------------*
  Name:         C3PoolTexCoord
  
  Description:  Returns a pointer to the next available texture coordinate

  Arguments:    NONE
                  
  Returns:      pointer to next available texture coordinate
/*---------------------------------------------------------------------------*/
C3TexCoord*    
C3PoolTexCoord( void )
{
    return (C3TexCoord*)C3GetNextPoolElem( &texCoordPool );
}


/*---------------------------------------------------------------------------*
  Name:         C3PoolTexture
  
  Description:  Returns a pointer to the next available texture

  Arguments:    NONE
                  
  Returns:      pointer to next available texture
/*---------------------------------------------------------------------------*/
C3Texture*      
C3PoolTexture( void )
{
    return (C3Texture*)C3GetNextPoolElem( &texPool );
}


/*---------------------------------------------------------------------------*
  Name:         C3PoolVertex
  
  Description:  Returns a pointer to the next available vertex

  Arguments:    NONE
                  
  Returns:      pointer to next available vertex
/*---------------------------------------------------------------------------*/
C3Vertex*
C3PoolVertex( void )
{
    return (C3Vertex*)C3GetNextPoolElem( &vertexPool );
}


/*---------------------------------------------------------------------------*
  Name:         C3PoolPtrLink
  
  Description:  Returns a pointer to the next available ptrLink

  Arguments:    NONE
                  
  Returns:      pointer to next available ptr link
/*---------------------------------------------------------------------------*/
C3PtrLink*
C3PoolPtrLink( void )
{
    return (C3PtrLink*)C3GetNextPoolElem( &ptrLinkPool );
}


/*---------------------------------------------------------------------------*
  Name:         C3PoolPrimitive
  
  Description:  Returns a pointer to the next available primitive

  Arguments:    NONE
                  
  Returns:      pointer to next available
/*---------------------------------------------------------------------------*/
C3Primitive*
C3PoolPrimitive( void )
{
    return (C3Primitive*)C3GetNextPoolElem( &primitivePool );
}


/*---------------------------------------------------------------------------*
  Name:         C3PoolImage
  
  Description:  Returns a pointer to the next available image

  Arguments:    NONE
                  
  Returns:      pointer to next available
/*---------------------------------------------------------------------------*/
C3Image*
C3PoolImage( void )
{
    return (C3Image*)C3GetNextPoolElem( &imagePool );
}


/*---------------------------------------------------------------------------*
  Name:         C3PoolSrcImage
  
  Description:  Returns a pointer to the next available srcImage

  Arguments:    NONE
                  
  Returns:      pointer to next available
/*---------------------------------------------------------------------------*/
C3SrcImage*
C3PoolSrcImage( void )
{
    return (C3SrcImage*)C3GetNextPoolElem( &srcImagePool );
}


/*---------------------------------------------------------------------------*
  Name:         C3PoolPalette
  
  Description:  Returns a pointer to the next available palette

  Arguments:    NONE
                  
  Returns:      pointer to next available
/*---------------------------------------------------------------------------*/
C3Palette*
C3PoolPalette( void )
{
    return (C3Palette*)C3GetNextPoolElem( &palettePool );
}


/*---------------------------------------------------------------------------*
  Name:         C3PoolStatsObj
  
  Description:  Returns a pointer to the next available object option

  Arguments:    NONE
                  
  Returns:      pointer to next available
/*---------------------------------------------------------------------------*/
C3StatsObj*
C3PoolStatsObj( void )
{
    return (C3StatsObj*)C3GetNextPoolElem( &statsObjPool );
}


/*---------------------------------------------------------------------------*
  Name:         C3PoolOptionObj
  
  Description:  Returns a pointer to the next available object option

  Arguments:    NONE
                  
  Returns:      pointer to next available
/*---------------------------------------------------------------------------*/
C3OptionObj*
C3PoolOptionObj( void )
{
    return (C3OptionObj*)C3GetNextPoolElem( &optionObjPool );
}


/*---------------------------------------------------------------------------*
  Name:         C3PoolSeqTrack
  
  Description:  Returns a pointer to the next available seqtrack

  Arguments:    NONE
                  
  Returns:      pointer to next available
/*---------------------------------------------------------------------------*/
C3SeqTrack*
C3PoolSeqTrack( void )
{
    return (C3SeqTrack*)C3GetNextPoolElem( &seqTrackPool );
}


/*---------------------------------------------------------------------------*
  Name:         C3PoolTrack
  
  Description:  Returns a pointer to the next available track

  Arguments:    NONE
                  
  Returns:      pointer to next available
/*---------------------------------------------------------------------------*/
C3Track*
C3PoolTrack( void )
{
    return (C3Track*)C3GetNextPoolElem( &trackPool );
}


/*---------------------------------------------------------------------------*
  Name:         C3PoolKeyFrame
  
  Description:  Returns a pointer to the next available keyframe

  Arguments:    NONE
                  
  Returns:      pointer to next available
/*---------------------------------------------------------------------------*/
C3KeyFrame*
C3PoolKeyFrame( void )
{
    return (C3KeyFrame*)C3GetNextPoolElem( &keyFramePool );
}


/*---------------------------------------------------------------------------*
  Name:         C3PoolControl
  
  Description:  Returns a pointer to the next available control

  Arguments:    NONE
                  
  Returns:      pointer to next available
/*---------------------------------------------------------------------------*/
C3Control*
C3PoolControl( void )
{
    return (C3Control*)C3GetNextPoolElem( &controlPool );
}


/*---------------------------------------------------------------------------*
  Name:         C3PoolHierNode
  
  Description:  Returns a pointer to the next available hierarchy node

  Arguments:    NONE
                  
  Returns:      pointer to next available
/*---------------------------------------------------------------------------*/
C3HierNode*
C3PoolHierNode( void )
{
    return (C3HierNode*)C3GetNextPoolElem( &hierNodePool );
}


/*---------------------------------------------------------------------------*
  Name:         C3PoolGeomObject
  
  Description:  Returns a pointer to the next available geometry object

  Arguments:    NONE
                  
  Returns:      pointer to next available
/*---------------------------------------------------------------------------*/
C3GeomObject*
C3PoolGeomObject( void )
{
    return (C3GeomObject*)C3GetNextPoolElem( &geomObjectPool );
}


/*---------------------------------------------------------------------------*
  Name:         C3PoolString
  
  Description:  Returns a pointer to the next available C3String structure

  Arguments:    NONE
                  
  Returns:      pointer to next available
/*---------------------------------------------------------------------------*/
C3String*
C3PoolString( void )
{
    return (C3String*)C3GetNextPoolElem( &stringPool );
}
