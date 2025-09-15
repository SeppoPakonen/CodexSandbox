/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3GeoCnv.c]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/build/charPipeline/c3/src/C3GeoCnv.c $
    
    27    12/04/00 8:28p John
    Fixed a memory deallocation bug.
    
    26    12/04/00 7:55p John
    Added multitexturing ability (work in progress).
    
    25    11/13/00 1:37p John
    Fixed a position matrix cache bug that arises with stitched geometry
    attached to less than 10 bones.
    
    24    6/27/00 6:05p John
    Substituted quantize.h macros.
    Added optimization to not include color indices if there is only one
    color for a geometry object (assumes runtime will set register color).
    Added optimization to use GX_REPLACE as Tev op if there is texture
    data, no lighting, and all the vertex colors are white.
    Fixed bug in C3GroupMatrixLoads.
    
    23    6/07/00 2:24p John
    Fixed subtle bug with C3GroupMatrixLoads.  FIFO Matrix cache is not
    necessarily sequential between each individual matrix load.
    
    22    6/05/00 4:21p John
    Now groups together matrix loads as much as possible in FIFO matrix
    cache to increase efficiency of graphics pipeline.  Added
    C3GroupMatrixLoads.
    
    21    3/23/00 2:11p John
    Modified function names to runtime library changes.
    
    20    3/23/00 11:12a John
    Removed transparency from primitive structure since never used.
    
    19    3/15/00 2:32p John
    Fixed color quantization bug (incorrect component size was calculated).
    
    18    3/14/00 1:32p John
    Moved display priority from GPL to ACT.
    
    17    2/29/00 7:21p John
    Now converts primitives to strips/fans in C3OptimizeBeforeOutput.
    
    16    2/18/00 5:56p John
    Changed test output filenames to 8.3 format.
    
    15    2/18/00 4:54p John
    Now assigns vertex stitching information after hierarchy extraction
    process.
    
    14    2/16/00 2:35p John
    Cleaned up code.
    
    13    2/15/00 4:23p John
    Fixed bug in primitive sort to minimize texture state change.
    
    12    2/14/00 6:05p John
    Commented out debug code (was writing out a debug file).
    
    11    2/04/00 6:06p John
    Untabified code.
    
    10    2/04/00 2:08p John
    Minor code change.
    
    9     2/01/00 6:52p John
    Added stats for position matrix cache performance.
    Removed static local variables since initialization can be skipped upon
    running from 3D Studio MAX after first time.
    Added sorting primitives in display list for primitive type.
    
    8     1/31/00 4:15p John
    Added pruning of unnecessary bones and animations.
    Finished stitching with FIFO matrix cache and stripping.
    
    7     1/20/00 1:14p John
    Added stitching (work in progress).
    Added C3 string table for efficient management of strings.
    
    6     1/04/00 1:12p John
    Added better support for second texture channel.
    
    5     12/20/99 11:21a John
    Added second texture channel.
    
    4     12/09/99 5:07p John
    Fixed code within C3_GENERATE_NORMAL_TABLE
    to export normals as code in .c file instead of binary data.
    
    3     12/06/99 3:24p John
    Added function comments and fixed memory leaks.
    
    2     12/02/99 2:23p John
    Added sorting of primitives in display list.
    
    26    11/18/99 1:15p John
    Does not send texture coordinate array or header if no tex coords.
    Pads display list to 32 byte boundary.
    
    25    11/11/99 1:15p John
    New Strip/Fan code. Added option to view strip/fan effectiveness.
    
    24    10/21/99 6:22p John
    Added drawing priority.
    
    23    10/20/99 11:05a John
    Added vertex number and vertex call stats.
    C3GeomObject.positionList is now processed through
    C3CompressIndexedList.
    
    22    10/13/99 4:03p John
    Fixed mipmap mag filter bug.
    
    21    10/13/99 3:31p John
    Modified code to accomodate new PositionHeader and LightingHeader.
    
    20    10/13/99 2:52p John
    Added Default Normal Table.
    
    19    10/01/99 11:15a John
    Can quantize color two ways (with and without alpha)
    Added position and tex. coordinate quantization per object.
    
    18    9/29/99 4:29p John
    Changed header to make them all uniform with logs.
    Added position and normal quantization.
    Added external normal table and a #define to create them.
    
    17    9/21/99 6:41p John
    Fixed memory leaks.
    
    16    9/20/99 7:03p John
    Renamed C3TextCoord to C3TexCoord for consistency.
    To have 8 bit indices, count must be <= 255 rather than 256
    since 256 is reserved.
    
    15    9/17/99 9:34a John
    Added C3Options and cleaned up code.
    
    14    9/14/99 6:48p John
    Correctly sets texture filters.
    Compensated code in response to C3Stats and C3StatsObj.
    
    13    9/03/99 2:02p John
    Added quantization of color and indices.  
    Added removal of null primitives.
    Changed data structure for tiling to 8 bits.  
    Work in progress for adding an option for tex. filters.

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#include "C3Private.h"

#define C3_STATE_NONE               0x0000  // Only counts bottom 12 bits in
#define C3_STATE_TEXTURE_COMBINE    0x0001  //  C3AssembleDispList(...) 
#define C3_STATE_VCD                0x0002
#define C3_STATE_TEXTURE0           0x0010  
#define C3_STATE_TEXTURE1           0x0020  
#define C3_STATE_TEXTURE2           0x0040  
#define C3_STATE_TEXTURE3           0x0080  
#define C3_STATE_TEXTURE4           0x0100  
#define C3_STATE_TEXTURE5           0x0200  
#define C3_STATE_TEXTURE6           0x0400  
#define C3_STATE_TEXTURE7           0x0800  
#define C3_STATE_MTXLOAD            0x1000  

#define C3_NUM_POS_MATRIX           10

typedef struct
{
    DSLink          link;

    u16             state;                  // State of the node
    
    C3Texture*      tex[C3_MAX_TEXTURES];   // Textures
    u32             vtxCompDesc;            // Vertex component descriptor

    DSList          primList;               // Display list (C3Primitive)
    u32             primListSize;           // Size of the primitive list

    u8              numMatrixLoads;         // Number of position matrix loads
    u16             matrixCache[C3_NUM_POS_MATRIX]; // current matrix cache

    C3Bool          colorAllWhite;          // true if all vertex colors are white
    C3Bool          lighting;               // true if we want to light the model

} C3StateNode;

typedef struct
{
    DSLink          link;
    u32             index;                  // Index of starting primitive for new 
                                            //  set of matrices
    u16             lastLoaded;
    u16             matrixCache[C3_NUM_POS_MATRIX];

} C3StateMatrixCache;

// External functions ********************************************************

DOLayout*    C3ConvertToDOLayout( 
                        C3GeomObject*  geomObj, 
                        char**         strBank,
                        u32*           strBankSize );

void         C3FreeDOLayout( DOLayout** doLayout );

// External variables ********************************************************

extern DOLightingHeader c3externalNormalHeader;
extern DSList           C3externalNormalList;

// Local functions ***********************************************************

static void         C3ConvertPositionData( C3GeomObject* geomObj, DOPositionHeader** posHeader );
static void         C3ConvertColorData( C3GeomObject* geomObj, DOColorHeader** colorHeader, DOLayout *doLayout );
static u8           C3GetColorCompCount( u8 quant );
static C3Bool       C3ConvertTextureData( C3GeomObject* geomObj, DOTextureDataHeader* texHeader, u8 channel );
static void         C3ConvertLightingData( C3GeomObject* geomObj, DOLightingHeader** litHeader );
static void         C3GetTexPaletteName( C3GeomObject* geomObj, char** texPaletteName );
static void         C3SetStringBank( DOLayout* doLayout, char** strBank, u32* strBankSize );
static void         C3ConvertDisplayData( C3GeomObject* geomObj, DODisplayHeader** displayHeader );
static void         C3SortPrimList( C3GeomObject *geomObj, DSList *matrixCacheList );
static s32          C3ComparePrimForDispListOnTexture( const void* prim1, const void* prim2 );
static s32          C3ComparePrimForDispListOnPrimType( const void* prim1, const void* prim2 );
static u32          C3FindNextTextureStateChange( C3Primitive* primArray, u32 primCount, u32 startIndex );
static void         C3SortPrimByPosMatrix( C3Primitive *primArray, u32 start, u32 end, u16 *matrixCache, DSList *matrixCacheList, u8 *lastLoaded, C3StatsObj *stats );
static void         C3GroupMatrixLoads( C3Primitive* primArray, u32 primCount, DSList *matrixCacheList );
static u32          C3FindBestPrimitive( C3Primitive *primArray, u32 current, u32 end, u16 *matrixCache, u32 *numMatrixLoads, u8 *lastLoaded );
static u32          C3LoadPosMatrices( C3Primitive *prim, u16 *matrixCache, u8 *lastLoaded );
static u8           C3LoadPosMatrix( u16 *matrixCache, u16 matrixId, C3Bool *matrixCacheUsed, u8 *lastLoaded );
static void         C3AssembleDispList( C3GeomObject* geomObj, DSList* stateList,
                                        u32* stateCount, u32* primBankSize, DSList* matrixCacheList );
static u16          C3GetPrimSize( C3GeomObject* obj, C3Primitive* prim );
static C3StateNode* C3GetStateChange( C3GeomObject* obj, C3Primitive* prevPrim, 
                                      C3Primitive* prim, C3StateNode* stateNode, DSList* matrixCacheList );
static C3StateNode* C3AllocStateNodeFromPrim( C3GeomObject* obj, C3Primitive* prim, C3StateMatrixCache* matrixCacheState );
static u16          C3GetState( C3Primitive* prev, C3Primitive* next, DSList* matrixCacheList, C3StateMatrixCache **matrixCacheState );
static void         C3CreateDispList( C3GeomObject* obj, DSList* stateList, 
                                      DODisplayHeader* displayData,
                                      u32 primBankSize, u32 stateCount );
static void         C3ConvertPrimList( C3GeomObject* obj, C3StateNode* stateNode, 
                                       void** primBank, u32* primListLength );
static C3Bool       C3SameType( C3Primitive* p1, C3Primitive* p2 );
static C3Bool       C3SingleType( u8 type );
static u8           C3GetConvertPrimType( C3Primitive* prim );
static void         C3ConvertVtxIndexed( C3GeomObject* obj, C3Vertex* vtx, void** primBank, u16 *matrixCache );
static void         C3ConvertIndex( C3GeomObject* obj, u32 index, void** ptr, u32 target, u8 channel );
static void         C3ConvertState( C3StateNode* stateNode, DODisplayState** stateArray, 
                                    void* primBank, u32 primListLength );
static C3Bool       C3CompareTexturesMore( void* t1, void* t2 );
static u32          C3FindNormalTableIndex( C3Normal* normal );
static u8           C3GetGxPosMtxIndex( u8 index );


/*---------------------------------------------------------------------------*
  Name:         C3ConvertToGeoPalette

  Description:  Converts the list of objects to a geo palette

  Arguments:    geomList - list of all the geom objects
                c3GeoPal - pointer where to allocate the new C3GeoPalette
                palName  - name of the geo palette

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3ConvertToGeoPalette( DSList* geomList, C3GeoPalette** c3GeoPal, char* palName )
{
    GEOPalette*     geoPal      = NULL;
    C3GeomObject*   cursor      = NULL;
    char*           strCursor   = NULL;
    GEODescriptor*  desc        = NULL;
    u32             i           = 0;
    u32*            strBankSize = NULL;    
    C3DOLayout*     layout      = NULL;
    char            texPalName[256];


    // For the moment always compress the texture data
    C3CompressTextureData();

    sprintf( texPalName, "%s.%s", palName, C3_EXT_TEXTURE_PAL );

    // Allocate memory for the geo palette
    *c3GeoPal = (C3GeoPalette*)C3_CALLOC( 1, sizeof( C3GeoPalette ));
    C3_ASSERT( *c3GeoPal );

    (*c3GeoPal)->geoPal = (GEOPalette*)C3_CALLOC( 1, sizeof( GEOPalette ));
    C3_ASSERT( (*c3GeoPal)->geoPal );

    // Set the local varibles
    geoPal      = (*c3GeoPal)->geoPal;
    strBankSize = &(*c3GeoPal)->strBankSize;

    // First pass to count the number of geom object and calculate the size of
    // the string bank
    cursor = (C3GeomObject*)geomList->Head;
    while( cursor )
    {
        geoPal->numDescriptors++;
        *strBankSize += (strlen( cursor->identifier ) + 1); // Complete path ?
        cursor = (C3GeomObject*)cursor->link.Next;
    }

    // Allocate memory for the descriptors
    (*c3GeoPal)->layoutArray = (C3DOLayout*)C3_CALLOC( geoPal->numDescriptors,
                                                       sizeof( C3DOLayout ));
    C3_ASSERT( (*c3GeoPal)->layoutArray );

    geoPal->descriptorArray = (GEODescriptor*)C3_CALLOC( geoPal->numDescriptors, 
                                                         sizeof(GEODescriptor) );
    C3_ASSERT( geoPal->descriptorArray );

    // Allocate memory for the strings
    (*c3GeoPal)->strBank = (char*)C3_CALLOC( *strBankSize, 1 );
    C3_ASSERT( (*c3GeoPal)->strBank );

    layout    = (*c3GeoPal)->layoutArray;
    desc      = geoPal->descriptorArray;
    strCursor = (*c3GeoPal)->strBank;
    cursor    = (C3GeomObject*)geomList->Head;
    i         = 0;
    while( cursor )
    {
        cursor->texPaletteName = texPalName;
        // Write display objects
        C3_ASSERT( cursor->index == i );
        layout[i].layout = 
        desc[i].layout = C3ConvertToDOLayout( cursor,
                                              &layout[i].strBank,
                                              &layout[i].strBankSize ); 
        desc[i++].name = strCursor ;    
        
        // Write the name in the string bank    
        sprintf( strCursor, "%s", cursor->identifier  );
        strCursor = strCursor + strlen(cursor->identifier) + 1;

        cursor = (C3GeomObject*)cursor->link.Next;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3FreeGeoPalette

  Description:  Frees the memory allocated with the geo palette specified

  Arguments:    geoPal - geo palette to free

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3FreeGeoPalette( C3GeoPalette** geoPal )
{
    GEODescriptor*  desc = NULL;
    u32             i    = 0;


    desc = (*geoPal)->geoPal->descriptorArray;
    for( i = 0; i < (*geoPal)->geoPal->numDescriptors; i++ )
    {
        C3FreeDOLayout( &desc[i].layout );
    }

    C3_FREE( (*geoPal)->layoutArray );
    C3_FREE( (*geoPal)->geoPal->descriptorArray );
    C3_FREE( (*geoPal)->geoPal );
    C3_FREE( (*geoPal)->strBank );
    C3_FREE( *geoPal );
}


/*---------------------------------------------------------------------------*
  Name:         C3ConvertToDOLayout

  Description:  Converts a geomObject to a display object layout 
                (from runtime lib).  Allocates the memory for the DOLayout.

  Arguments:    geomObj     - geomobject to convert
                strBank     - pointer to contain the string bank
                strBankSize - pointer to contain the string bank size

  Returns:      the converted DOLayout object
/*---------------------------------------------------------------------------*/
DOLayout* 
C3ConvertToDOLayout( C3GeomObject* geomObj, char** strBank, u32* strBankSize )
{
    DOLayout*  doLayout = NULL;
    u32        i, channel;
    C3Bool     texDataExist = C3_FALSE;

    C3_ASSERT( geomObj && strBank && strBankSize );

    // Allocate a new DisplayObjLayout
    doLayout = (DOLayout*)C3_CALLOC( 1, sizeof(DOLayout) );

    // Create the position data
    C3ConvertPositionData( geomObj, &doLayout->positionData );
    
    // Create the texture data

    // Find the number of texture coordinate channels
    channel = 0;
    for( i = 0; i < C3_MAX_TEXTURES; i++ )
    {
        if( geomObj->texCoordList[i].Head )
            channel++;
    }

    if( channel > 0 )
    {
        doLayout->textureData = (DOTextureDataHeaderPtr)C3_CALLOC( channel, sizeof(DOTextureDataHeader) );
        doLayout->numTextureChannels = channel;
        for( i = 0; i < channel; i++ )
            C3ConvertTextureData( geomObj, &doLayout->textureData[i], i );
    }
   
    // Create the string bank, which is the texture palette name
    C3SetStringBank( doLayout, strBank, strBankSize );

    // Create the lighting data ( must come before converting display data )
    C3ConvertLightingData( geomObj, &doLayout->lightingData );

    // Create the color data (convert after texture and lighting)
    C3ConvertColorData( geomObj, &doLayout->colorData, doLayout );

    // Create the display data
    C3ConvertDisplayData( geomObj, &doLayout->displayData );

    return doLayout;
}


/*---------------------------------------------------------------------------*
  Name:         C3ConvertPositionData

  Description:  Converts the position data in the geomObject to a 
                position header. Allocates memory for the header.

  Arguments:    geomObject  - the geomObject to convert from 
                posHeader   - position header to convert to

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3ConvertPositionData( C3GeomObject* geomObj, DOPositionHeader** posHeader )
{
    DOPositionHeader* pos     = NULL;
    void*             array   = NULL;
    C3Position*       cursor  = NULL;
    u8                size    = 0;
    u32               count   = 0;
    FILE*             nFile   = NULL;
#ifdef C3_GENERATE_NORMAL_TABLE
    u8                normalQuantInfo;
    u8                normalCompSize;
#endif

    C3_ASSERT( geomObj && posHeader && geomObj->positionList.Head );
    
    // Allocate a new header
    pos = (DOPositionHeader*)C3_CALLOC( 1, sizeof(DOPositionHeader) );
    C3_ASSERT( pos );

    pos->compCount = 3;
    pos->numPositions = geomObj->stats->numUniquePositions;
    pos->quantizeInfo = C3GetOptionQuantization( C3_TARGET_POSITION, 0 ); // we only look at type

    // Calculate the size of a position component
    size = C3GetComponentSize( C3_TARGET_POSITION, 0 );

    // Allocate the memory for the array
    pos->positionArray = array = C3_CALLOC( pos->numPositions, 
                                            pos->compCount * size );
    C3_ASSERT( array );

#ifdef C3_GENERATE_NORMAL_TABLE
    // Special normal table hack -- comment out for normal usage
    // Using this code, export only one object with all positions a distance of 1 
    // from origin.  From MAX, used Icosahedron (or any geodesic sphere) with 
    // 252 vertices (for 8-bit indices).
    nFile = fopen( C3GetOptionExternalNormalTablePath() , "wt" );

    // Compute the shift
    normalQuantInfo = C3GetOptionQuantization( C3_TARGET_NORMAL, 0 );
    normalCompSize  = C3GetComponentSize( C3_TARGET_NORMAL, 0 );

    fprintf( nFile, "#include <Dolphin/types.h>\n" );
    fprintf( nFile, "#include <Dolphin/GXEnum.h>\n\n" );
    fprintf( nFile, "#define MAKE_QUANT(a,b)(((a)<<4)|((b)&0x0F))\n\n" );
    fprintf( nFile, "u8 normalTableQuantizeInfo = MAKE_QUANT( GX_F32, 0 );\n" );
    fprintf( nFile, "u8 normalTableNumComponents = 3;\n" );
    fprintf( nFile, "u16 normalTableNumNormals = %d;\n\n", pos->numPositions );

    fprintf( nFile, "f32 normalTable[%d][3] = \n{\n", pos->numPositions );
#endif

    // Fill the array (Positions are assumed to have a unique index)
    cursor = (C3Position*)(geomObj->positionList.Head);
    while(cursor)
    {
        C3QuantizeFloat( array, pos->quantizeInfo, cursor->x );
        array = (void*)((u8*)array + size);
        C3QuantizeFloat( array, pos->quantizeInfo, cursor->y );
        array = (void*)((u8*)array + size);
        C3QuantizeFloat( array, pos->quantizeInfo, cursor->z );
        array = (void*)((u8*)array + size);
        count++;

        // NOTE: Special hack to create normal table
#ifdef C3_GENERATE_NORMAL_TABLE
        fprintf( nFile, "%.15fF, %.15fF, %.15fF,\n", cursor->x, cursor->y, cursor->z );
#endif

        cursor = (C3Position*)(cursor->link.Next);
    }

    *posHeader = pos;

    // Special normal table hack
#ifdef C3_GENERATE_NORMAL_TABLE
    fprintf( nFile, "};\n" );
    fclose( nFile );
#endif

    C3_ASSERT( count == geomObj->stats->numUniquePositions );

    // Set the index quantization depending on the quant
    // NOTE: 0xFF or 0xFFFF index is reserved for positions by GX API
    if ( count <= 255 ) 
        C3SetVCDDataType( geomObj, C3_TARGET_POSITION, C3_VCD_INDEX8, 0 );
    else
        C3SetVCDDataType( geomObj, C3_TARGET_POSITION, C3_VCD_INDEX16, 0 );
}


/*---------------------------------------------------------------------------*
  Name:         C3ConvertColorData

  Description:  Converts the color data in the geomObject to a color header.
                Allocates memory for the header.

  Arguments:    geomObject  - the geomObject to convert from 
                colorHeader - color header to convert to
                doLayout    - used to check if texture and lighting data exist

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3ConvertColorData( C3GeomObject* geomObj, DOColorHeader** colorHeader, DOLayout *doLayout )
{
    DOColorHeader* color = NULL;
    void*          array   = NULL;
    u32            prev    = -1;
    C3Color*       cursor  = NULL;
    u8             size    = 0;
    u32            count   = 0;
    u8             allWhite = 1;


    C3_ASSERT( geomObj && colorHeader && geomObj->colorList.Head );

    if( doLayout->textureData && !doLayout->lightingData && geomObj->colorAllWhite )
    {
        // if all vertex colors are white and texture data exist and no lighting,
        // then do not send colors since we will use GX_REPLACE
        C3SetVCDDataType( geomObj, C3_TARGET_COLOR, C3_VCD_NONE, 0 );
        *colorHeader = NULL;
        return;
    }

    // Allocate a new header
    color = (DOColorHeader*)C3_CALLOC( 1, sizeof(DOColorHeader) );
    C3_ASSERT( color );

    // If this object is using vertex alpha, then we should 
    // quantize the colors with an alpha component
    color->numColors = geomObj->stats->numUniqueColors;
    if( geomObj->useVertexAlpha )
        color->quantizeInfo = C3GetOptionQuantization( C3_TARGET_COLORALPHA, 0 );
    else
        color->quantizeInfo = C3GetOptionQuantization( C3_TARGET_COLOR, 0 );
        
    color->compCount = C3GetColorCompCount( color->quantizeInfo ); 

    // Get the size of the component (works with color w/ alpha
    // even though the first argument uses a flag which infers color w/o alpha)
    size = C3GetComponentSizeFromQuantInfo( C3_TARGET_COLOR, color->quantizeInfo );

    // Allocate the array of colors
    color->colorArray = array = C3_CALLOC( color->numColors, size );
    C3_ASSERT( array );

    // Fill the array
    cursor = (C3Color*)(geomObj->colorList.Head);
    prev = -1;
    while( cursor )
    {
        if( cursor->index != prev ) // if the index is not the same as the precedent
        {
            C3QuantizeColor( array, color->quantizeInfo, cursor );
            array = (void*)((u8*)array + size);
            count++;
        }
        prev = cursor->index;
        cursor = (C3Color*)(cursor->link.Next);
    }

    *colorHeader = color;

    C3_ASSERT( count == geomObj->stats->numUniqueColors );

    // Set the index quantization depending on the quant
    if( count == 1 )
        // do not send color indices in display list since runtime will set
        // the color in a material register
        C3SetVCDDataType( geomObj, C3_TARGET_COLOR, C3_VCD_NONE, 0 );
    else if( count <= 256 ) 
        C3SetVCDDataType( geomObj, C3_TARGET_COLOR, C3_VCD_INDEX8, 0 );
    else
        C3SetVCDDataType( geomObj, C3_TARGET_COLOR, C3_VCD_INDEX16, 0 );
}


/*---------------------------------------------------------------------------*
  Name:         C3GetColorCount

  Description:  Return the size of a color unit according to the quantization
                type for the color.

  Arguments:    quant - type of quantization applied to the color

  Returns:      size of a color unit
/*---------------------------------------------------------------------------*/
static u8
C3GetColorCompCount( u8 quant )
{
    u8  count = 0;


    switch( C3_QUANT_TYPE( quant ) )
    {
        case GX_RGB565:
        case GX_RGB8:
        case GX_RGBX8:  count = 3; break;
        case GX_RGBA4:
        case GX_RGBA6:
        case GX_RGBA8:  count = 4; break;
        default:
            C3_ASSERT( C3_FALSE );
            break;
    }
    return count;
}


/*---------------------------------------------------------------------------*
  Name:         C3ConvertTextureData

  Description:  Converts the Texture data in the geomObject to a Texture header
                for the specified texture coordinate channel.
                NOTE: As opposed to the other C3Convert* functions, it assumes
                that the header has already been allocated.

  Arguments:    geomObject  - the geomObject to convert from 
                texHeader   - texture header to convert to
                channel     - texture coordinate channel

  Returns:      true if the channel was converted (channel exists)
/*---------------------------------------------------------------------------*/
static C3Bool
C3ConvertTextureData( C3GeomObject* geomObj, DOTextureDataHeader* texHeader, u8 channel )
{
    DOTextureDataHeader *tex           = NULL;
    C3TexCoord*         cursor         = NULL;
    void*               array          = NULL;
    u32                 prev           = -1;
    u8                  size           = 0;
    u32                 count          = 0;

    C3_ASSERT( geomObj && texHeader );

    // Make sure we have a list to work with
    if( !geomObj->texCoordList[channel].Head )
    {
        C3SetVCDDataType( geomObj, C3_TARGET_TEXCOORD, C3_VCD_NONE, channel );
        return C3_FALSE;
    }

    tex = texHeader;

    C3GetTexPaletteName( geomObj, &tex->texturePaletteName );
    tex->texturePalette = 0;

    tex->compCount = 2;
    tex->numTextureCoords = geomObj->stats->numUniqueTCoords[channel];
    // Get the quantization info for object w/ previously computed shift
    tex->quantizeInfo = geomObj->options->texCoordQuantization[channel];
    
    // Allocate memory
    size = C3GetComponentSize( C3_TARGET_TEXCOORD, channel );
    array = C3_CALLOC( tex->numTextureCoords, size * tex->compCount );
    C3_ASSERT( array );
    tex->textureCoordArray = array;

    // fill the array
    cursor = (C3TexCoord*)(geomObj->texCoordList[channel].Head);
    prev = -1;
    while( cursor )
    {
        if( cursor->index != prev ) // if the index is not the same as the precedent
        {
            C3QuantizeFloat( array, tex->quantizeInfo, cursor->s );
            array = (void*)((u8*)array + size);
            C3QuantizeFloat( array, tex->quantizeInfo, cursor->t );
            array = (void*)((u8*)array + size);
            count++;
        }
        prev = cursor->index;
        cursor = (C3TexCoord*)(cursor->link.Next);
    }

    C3_ASSERT( count == geomObj->stats->numUniqueTCoords[channel] );

    // Set the index quantization depending on the quant
    if ( count <= 256 ) 
        C3SetVCDDataType( geomObj, C3_TARGET_TEXCOORD, C3_VCD_INDEX8, channel );
    else
        C3SetVCDDataType( geomObj, C3_TARGET_TEXCOORD, C3_VCD_INDEX16, channel );

    return C3_TRUE;
}


/*---------------------------------------------------------------------------*
  Name:         C3ConvertLightingData

  Description:  Converts the lighting data in the geomObject to a lighting header.
                Allocates memory for the header.

  Arguments:    geomObject  - the geomObject to convert from 
                litHeader   - lighting header to convert to

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3ConvertLightingData( C3GeomObject* geomObj, DOLightingHeader** litHeader )
{
    DOLightingHeader* lit     = NULL;
    void*             array   = NULL;
    u32               prev    = -1;
    C3Normal*         cursor  = NULL;
    u8                size    = 0;
    u32               count   = 0;

    C3_ASSERT( geomObj && litHeader );

    // See if we will send normal indices in the display list
    if ( !C3GetOptionEnableLighting() || !geomObj->normalList.Head ) 
    {
        C3SetVCDDataType( geomObj, C3_TARGET_NORMAL, C3_VCD_NONE, 0 );
        *litHeader = NULL;
        return;
    }

    // Allocate a new header
    lit = (DOLightingHeader*)C3_CALLOC( 1, sizeof(DOLightingHeader) );
    C3_ASSERT( lit );
    
    lit->compCount = 3;
    lit->numNormals = geomObj->stats->numUniqueNormals;
    lit->quantizeInfo = C3GetOptionQuantization( C3_TARGET_NORMAL, 0 );
    lit->ambientPercentage = C3GetOptionAmbientPercentage();
    lit->normalArray = NULL;

    if ( C3GetOptionUseExternalNormalTable() )
    {
        lit->compCount = c3externalNormalHeader.compCount;
        lit->numNormals = c3externalNormalHeader.numNormals;
        lit->quantizeInfo = c3externalNormalHeader.quantizeInfo;
        count = lit->numNormals;
    }
    else if ( C3GetOptionUseDefaultNormalTable() )
    {
        lit->compCount = normalTableNumComponents;
        lit->numNormals = normalTableNumNormals;
        lit->quantizeInfo = normalTableQuantizeInfo;
        count = lit->numNormals;
    }
    else
    {
        // Calculate the size of a position component
        size = C3GetComponentSize( C3_TARGET_NORMAL, 0 );

        // Allocate the memory for the array
        lit->normalArray = array = C3_CALLOC( lit->numNormals, 
                                              lit->compCount * size );
        C3_ASSERT( array );

        // Fill the array
        cursor = (C3Normal*)(geomObj->normalList.Head);
        prev = -1;
        while(cursor)
        {
            if(cursor->index != prev) // if the index is not the same as the precedent
            {
                C3QuantizeFloat( array, lit->quantizeInfo, cursor->x );
                array = (void*)((u8*)array + size);
                C3QuantizeFloat( array, lit->quantizeInfo, cursor->y );
                array = (void*)((u8*)array + size);
                C3QuantizeFloat( array, lit->quantizeInfo, cursor->z );
                array = (void*)((u8*)array + size);
                count++;
            }
            prev = cursor->index;
            cursor = (C3Normal*)(cursor->link.Next);
        }
        C3_ASSERT( count == geomObj->stats->numUniqueNormals );
    }

    *litHeader = lit;

    // Set the index quantization depending on the quant
    if ( count <= 256 ) 
        C3SetVCDDataType( geomObj, C3_TARGET_NORMAL, C3_VCD_INDEX8, 0 );
    else 
        C3SetVCDDataType( geomObj, C3_TARGET_NORMAL, C3_VCD_INDEX16, 0 );
}


/*---------------------------------------------------------------------------*
  Name:         C3GetTexPaletteName

  Description:  Retrieves the palette name for the geom object or create a
                new one if it does not exist.
  
  Arguments:    geomObj         - geom object the 
                texPaletterName - location where to allocate the string 
                                  for the palette name.

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3GetTexPaletteName( C3GeomObject* geomObj, char** texPaletteName )
{
    // If a texture palette has been assigned
    if( geomObj->texPaletteName )
    {
        // Copy the name
        *texPaletteName = C3AddStringInTable( geomObj->texPaletteName );
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3SetStringBank

  Description:  Copies the texture palette name to the string bank.
  
  Arguments:    doLayout    - doLayout object to generate the string bank from
                strBank     - location for the string bank
                strBankSize - location for the string bank size

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3SetStringBank( DOLayout* doLayout, char** strBank, u32* strBankSize )
{
    *strBank     = NULL;
    *strBankSize = 0;
    
    if( doLayout->textureData )
    {
        if( doLayout->textureData[0].texturePaletteName )
        {
            *strBank = C3AddStringInTable( doLayout->textureData[0].texturePaletteName );
            *strBankSize = strlen( *strBank );
        }
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3ConvertDisplayData

  Description:  Converts the Display data in the geomObject to a Display header.
                Allocates memory for the header.

  Arguments:    geomObject      - the geomObject to convert from 
                displayHeader   - Display header to convert to

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3ConvertDisplayData( C3GeomObject* geomObj, DODisplayHeader** displayHeader )
{
    DODisplayHeader*   display      = NULL;
    DSList             stateList;
    DSList             matrixCacheList;
    C3StateNode        stateNode;
    C3StateNode*       stateNodePtr, *cursor;
    C3StateMatrixCache *matrixCacheState, *nextMatrixCacheState;
    u32                primBankSize = 0;
    u32                stateCount   = 0;


    // Allocate a new header
    display = (DODisplayHeader*)C3_CALLOC( 1, sizeof(DODisplayHeader) );
    C3_ASSERT( display );

    // Initialize the state list
    DSInitList( &stateList, (Ptr)&stateNode, &stateNode.link );
    DSInitList( &matrixCacheList, (Ptr)&stateNode, &stateNode.link );

    C3ReportStatus( "Sorting primitives for %s...", geomObj->identifier );

    // Sort the primitives so that state change is minimized
    C3SortPrimList( geomObj, &matrixCacheList );

    // Assemble the display data
    C3AssembleDispList( geomObj, &stateList, &stateCount, &primBankSize, &matrixCacheList );

    // Create the display list
    C3CreateDispList( geomObj, &stateList, display, primBankSize, stateCount );

    // Free the state list
    cursor = (C3StateNode*)stateList.Head;
    while( cursor )
    {   
        stateNodePtr = cursor;
        cursor = (C3StateNode*)cursor->link.Next;
        C3_FREE( stateNodePtr );
    }

    // Free the matrix cache list
    nextMatrixCacheState = (C3StateMatrixCache*)matrixCacheList.Head;
    while( nextMatrixCacheState )
    {
        matrixCacheState = nextMatrixCacheState;
        nextMatrixCacheState = (C3StateMatrixCache*)nextMatrixCacheState->link.Next;
        C3_FREE( matrixCacheState );
    }

    *displayHeader = display;
}


/*---------------------------------------------------------------------------*
  Name:         C3AssignBoneIndices

  Description:  If geomObj is stitched, converts all of the boneNames in 
                all of the vertices (currently stored in C3Vertex.boneIndex, 
                even though it is of type u16*--yes, it's a hack to save
                space) to boneIndices.

  Arguments:    geomObj - geom object

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3AssignBoneIndices( C3GeomObject *geomObj )
{
    C3Primitive *prim;
    C3PtrLink   *vtxPtr;
    C3Vertex    *vtx;
    C3HierNode  *hNode;

    // Step through each vertex in each primitive of the geometry object
    prim = (C3Primitive*)geomObj->primitiveList.Head;
    while( prim )
    {
        vtxPtr = (C3PtrLink*)prim->vertexPtrList.Head;
        while( vtxPtr )
        {
            vtx = (C3Vertex*)vtxPtr->ptr;
            if( vtx->boneIndex )
            {
                hNode = C3GetHierNodeFromIdentifier( (char*)vtx->boneIndex );
                if( !hNode )
                {        
                    C3ReportError( "Could not find bone %s to which vertex was to be attached", (char*)vtx->boneIndex );
                    vtx->boneIndex = NULL;                    
                }
                else
                {
                    vtx->boneIndex = &hNode->index;

                    // Mark so that this bone is not pruned in C3RemoveUnusedBones
                    hNode->usedForStitching = C3_TRUE;
                }
            }

            vtxPtr = (C3PtrLink*)vtxPtr->link.Next;
        }

        prim = (C3Primitive*)prim->link.Next;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3SortPrimList

  Description:  Sorts a list of primitives so that state change will be
                minimized.  Sorts by texture first, then by primitive type.

  Arguments:    geomObj      - geom object that holds the primitive list
                matrixCacheList - list to hold matrix loads for

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3SortPrimList( C3GeomObject *geomObj, DSList *matrixCacheList )
{
    u32                 primCount = 0;
    u32                 i, j;
    C3Primitive         *cursor;
    C3Primitive         *primArray;
    DSLink              link;
    u32                 start, end;
    u16                 matrixCache[C3_NUM_POS_MATRIX]; // Keep track of matrices in cache
    C3PtrLink           *ptrLink;
    C3Vertex            *vertex, *unStitchedVertex;
    C3Bool              objectUsesPosMatrices = C3_FALSE;
    u8                  lastLoaded;
    C3StateMatrixCache  *stateMatrixCache;
//  FILE        *f;

    // TESTING
    /*
    f = fopen( "D:\\temp\\SrtBefor.txt", "at" );
    cursor = (C3Primitive*)geomObj->primitiveList.Head;
    i = 0;
    fprintf( f, "======> %s:\n", geomObj->identifier);
    while( cursor )
    {
        fprintf( f, "%d: Type: %d Texture: %s TextureID: %d NumVertices: %d\n", 
                 i, cursor->type, 
                 (cursor->texture[0])?"yes":"no", (cursor->texture[0])?cursor->texture[0]->index:-1, 
                 cursor->numVertex );
//        fprintf( f, "\tMatrix Ids: " );
//        ptrLink = (C3PtrLink*)cursor->vertexPtrList.Head;
//        while( ptrLink )
//        {
//            fprintf( f, "%2d, ", (((C3Vertex*)ptrLink->ptr)->boneIndex)?(*((C3Vertex*)ptrLink->ptr)->boneIndex):-1 );
//            ptrLink = (C3PtrLink*)ptrLink->link.Next;
//        }
//        fprintf( f, "\n" );

        i++;
        cursor = (C3Primitive*)cursor->link.Next;
    }
    fclose( f );
//  */

    // Find the number of primitives
    cursor = (C3Primitive*)geomObj->primitiveList.Head;
    while( cursor )
    {
        primCount++;
        cursor = (C3Primitive*)cursor->link.Next;
    }

    // Allocate an array of primitives, copying them to an array
    primArray = C3_CALLOC( primCount, sizeof(C3Primitive) );
    i = 0;
    cursor = (C3Primitive*)geomObj->primitiveList.Head;
    while( cursor )
    {
        memcpy( &primArray[i], cursor, sizeof(C3Primitive) );

        i++;
        cursor = (C3Primitive*)cursor->link.Next;
    }

    // Sort the primitive array by texture ids first (qsort only works with arrays)
    if( geomObj->stats->numUniqueTCoords[0] > 0 )
        qsort( primArray, primCount, sizeof(C3Primitive), C3ComparePrimForDispListOnTexture );

    // Within each texture bucket, sort for position matrices
    if( C3GetOptionEnableStitching() )
    {
        // See if all vertices in all primitives are attached to a bone
        objectUsesPosMatrices = C3_TRUE;
        for( i = 0; i < primCount && objectUsesPosMatrices; i++ )
        {
            ptrLink = (C3PtrLink*)primArray[i].vertexPtrList.Head;
            while( ptrLink )
            {
                vertex = (C3Vertex*)ptrLink->ptr;
                if( !vertex->boneIndex )
                {
                    objectUsesPosMatrices = C3_FALSE;
                    unStitchedVertex = vertex;
                }
                else if( !objectUsesPosMatrices && vertex->boneIndex )
                {
                    C3ReportError( "A vertex is not attached to a bone at (%.5f, %.5f, %.5f)",
                                   unStitchedVertex->position->x, unStitchedVertex->position->y, unStitchedVertex->position->z );

                    // Go through all the primitives and make all vertices unstitched
                    for( j = 0; j < primCount; j++ )
                    {
                        ptrLink = (C3PtrLink*)primArray[j].vertexPtrList.Head;
                        while( ptrLink )
                        {
                            ((C3Vertex*)ptrLink->ptr)->boneIndex = NULL;
                            ptrLink = (C3PtrLink*)ptrLink->link.Next;
                        }
                    }

                    break;
                }

                ptrLink = (C3PtrLink*)ptrLink->link.Next;
            }
        }

        // Only sort if all vertices in all primitives are stitched
        if( objectUsesPosMatrices )
        {
            // Initialize the matrix cache with 0xFFFF to signal no matrix loaded
            memset( matrixCache, 0xFF, sizeof(u16) * C3_NUM_POS_MATRIX );
            lastLoaded = C3_NUM_POS_MATRIX - 1; // So it will start loading at 0

            // Then sort to minimize matrix loads
            start = 0;
            end = C3FindNextTextureStateChange( primArray, primCount, start ) - 1;
            while( start < primCount )
            {
                C3SortPrimByPosMatrix( primArray, start, end, matrixCache, matrixCacheList, &lastLoaded, geomObj->stats );

                start = end + 1;
                end = C3FindNextTextureStateChange( primArray, primCount, start ) - 1;
            }

            // Group as much of the matrix loads together as possible for efficiency
            // NOTE: Check by looking at SrtAfter.txt at the end of this function
            C3GroupMatrixLoads( primArray, primCount, matrixCacheList );
        }
    }

    // Sort by primitive type
    if( objectUsesPosMatrices )
    {
        // Within each position matrix bucket 
        stateMatrixCache = (C3StateMatrixCache*)matrixCacheList->Head;
        start = 0;
        do
        {
            stateMatrixCache = (C3StateMatrixCache*)stateMatrixCache->link.Next;
            if( stateMatrixCache )
                end = stateMatrixCache->index - 1;
            else
                end = primCount - 1;

            C3_ASSERT( start <= end );

            qsort( &primArray[start], (end - start + 1), sizeof(C3Primitive), C3ComparePrimForDispListOnPrimType );

            start = end + 1;

        } while( stateMatrixCache );
    }
    else
    {
        // Within each texture bucket
        start = 0;
        end = C3FindNextTextureStateChange( primArray, primCount, start ) - 1;
        while( start < primCount )
        {
            qsort( &primArray[start], (end - start + 1), sizeof(C3Primitive), C3ComparePrimForDispListOnPrimType );

            start = end + 1;
            end = C3FindNextTextureStateChange( primArray, primCount, start ) - 1;
        }       
    }

    // In order, copy over the primitives back to the primitiveList
    // without altering the link information
    cursor = (C3Primitive*)geomObj->primitiveList.Head;
    i = 0;
    while( cursor )
    {
        link = cursor->link;
        memcpy( cursor, &primArray[i], sizeof(C3Primitive) );
        cursor->link = link;
        
        cursor->index = i;

        i++;
        cursor = (C3Primitive*)cursor->link.Next;
    }

    C3_FREE( primArray );

    // TESTING
    /*
    f = fopen( "D:\\temp\\SrtAfter.txt", "at" );
    cursor = (C3Primitive*)geomObj->primitiveList.Head;
    fprintf( f, "======> %s:\n", geomObj->identifier);
    stateMatrixCache = (C3StateMatrixCache*)matrixCacheList->Head;
    while( cursor )
    {
        if( stateMatrixCache && stateMatrixCache->index == cursor->index )
        {
            fprintf( f, "===================================================\n" );
            fprintf( f, "New Matrix Cache: " );
            for( i = 0; i < C3_NUM_POS_MATRIX; i++ )
                fprintf( f, "%2d, ", ((stateMatrixCache->matrixCache[i] == 0xFFFF)?-1:stateMatrixCache->matrixCache[i]) );
            fprintf( f, "\n" );
            fprintf( f, "Last loaded: %d\n", stateMatrixCache->lastLoaded );
            fprintf( f, "===================================================\n" );
            stateMatrixCache = (C3StateMatrixCache*)stateMatrixCache->link.Next;
        }

        fprintf( f, "%d: Type: %d Texture: %s ",
                 cursor->index, cursor->type, 
                 (cursor->texture[0])?"yes":"no" );
        for( i = 0; i < C3_MAX_TEXTURES; i++ )
        {
            fprintf( f, "Tex%dID: %d ", i, (cursor->texture[i])?cursor->texture[i]->index:-1 );
        }
        fprintf( f, "NumVertices: %d\n", cursor->numVertex );

        // Print out the matrices these vertices are attached to
        fprintf( f, "      " );
        ptrLink = (C3PtrLink*)cursor->vertexPtrList.Head;
        while( ptrLink )
        {
            vertex = (C3Vertex*)ptrLink->ptr;
            C3_ASSERT( vertex );
            if( vertex->boneIndex )
                fprintf( f, "%2d, ", *vertex->boneIndex );
            else
                fprintf( f, "-1, " );
            ptrLink = (C3PtrLink*)ptrLink->link.Next;
        }
        fprintf( f, "\n" );

        cursor = (C3Primitive*)cursor->link.Next;
    }
    fclose( f );
//  */
}


/*---------------------------------------------------------------------------*
  Name:         C3ComparePrimForDispListOnTexture

  Description:  Compare two primitives based on texture

  Arguments:    prim1 - first primitive
                prim2 - second primitive

  Returns:      -1 if prim1 < prim2, 0 if prim1 == prim2, 1 if prim1 > prim2
/*---------------------------------------------------------------------------*/
static s32
C3ComparePrimForDispListOnTexture( const void* prim1, const void* prim2 )
{
    C3Primitive *p1 = (C3Primitive*)prim1;
    C3Primitive *p2 = (C3Primitive*)prim2;
    u32         i;

    for( i = 0; i < C3_MAX_TEXTURES; i++ )
    {
        // sort by whether textures exist or not
        if( p1->texture[i] && !p2->texture[i] )
            return -1;
        else if( !p1->texture[i] && p2->texture[i] )
            return 1;

        // sort by texture ids
        if( p1->texture[i] && p2->texture[i] )
        {
            if( p1->texture[i]->index < p2->texture[i]->index )
                return -1;
            else if( p1->texture[i]->index > p2->texture[i]->index )
                return 1;
        }
    }
    
    return 0;   
}


/*---------------------------------------------------------------------------*
  Name:         C3ComparePrimForDispListOnPrimType

  Description:  Compare two primitives based on primitive type

  Arguments:    prim1 - first primitive
                prim2 - second primitive

  Returns:      -1 if prim1 < prim2, 0 if prim1 == prim2, 1 if prim1 > prim2
/*---------------------------------------------------------------------------*/
static s32
C3ComparePrimForDispListOnPrimType( const void* prim1, const void* prim2 )
{
    s32 textureCompare;

    // still sort by texture first since it is not guaranteed that the 
    // primitives within a position matrix bucket all have the same texture
    textureCompare = C3ComparePrimForDispListOnTexture( prim1, prim2 );
    if( textureCompare != 0 )
        return textureCompare;

    // sort by primitive type
    if( ((C3Primitive*)prim1)->type == ((C3Primitive*)prim2)->type )
        return 0;
    else if( ((C3Primitive*)prim1)->type < ((C3Primitive*)prim2)->type )
        return -1;

    return 1;
}


/*---------------------------------------------------------------------------*
  Name:         C3FindNextTextureStateChange

  Description:  Given an array of primitives, returns the index of the 
                primitive after startIndex that has a texture state change.

  Arguments:    primArray - array of primitives
                primCount - length of primArray
                startIndex - index at which to start looking

  Returns:      index of primtive with texture state different than startIndex
                or if none, then primCount;
/*---------------------------------------------------------------------------*/
static u32
C3FindNextTextureStateChange( C3Primitive* primArray, u32 primCount, u32 startIndex )
{
    u32 i, j;

    if( startIndex >= primCount )
        return primCount;

    for( i = startIndex + 1; i < primCount; i++ )
    {
        for( j = 0; j < C3_MAX_TEXTURES; j++ )
            if( (!primArray[startIndex].texture[j] &&  primArray[i].texture[j]) ||
                ( primArray[startIndex].texture[j] && !primArray[i].texture[j]) ||
                ( primArray[startIndex].texture[j] &&  primArray[i].texture[j] &&
                  primArray[startIndex].texture[j]->index != primArray[i].texture[j]->index ) )
            {
                return i;
            }
    }

    return primCount;
}


/*---------------------------------------------------------------------------*
  Name:         C3SortPrimByPosMatrix

  Description:  Given an array of primitives, sorts between start and end 
                indices to minimize matrix loads.  Returns the matrix cache
                after sorting is done for this portion of the primitive array.

  Arguments:    primArray - array of primitives
                primCount - length of primArray
                startIndex - index at which to start looking
                matrixCache - matrix cache
                stats - stats to maintain for cache hit/miss rate

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3SortPrimByPosMatrix( C3Primitive *primArray, u32 start, u32 end, u16 *matrixCache, DSList *matrixCacheList, u8 *lastLoaded, C3StatsObj *stats )
{
    // Find a primitive between start and end, inclusive that is best used by
    // with the current matrix cache
    u32 current = start;
    u32 best;
    C3Primitive buffer;
    u32 numMatrixLoads;
    C3StateMatrixCache *state;

    while( current <= end )
    {
        // Find best match with current matrix cache
        // Also loads the matrices into matrix cache
        best = C3FindBestPrimitive( primArray, current, end, matrixCache, &numMatrixLoads, lastLoaded );

        // Keep stats on cache hit/miss rate
        stats->posMatrixCacheMiss += numMatrixLoads;
        
        C3_ASSERT( best >= current );

        // Swap the best candidate and current position in array
        if( best != current )
        {
            memcpy( &buffer, &primArray[best], sizeof(C3Primitive) );
            memcpy( &primArray[best], &primArray[current], sizeof(C3Primitive) );
            memcpy( &primArray[current], &buffer, sizeof(C3Primitive) );
        }

        // If the position matrices were loaded, then add a state change
        if( numMatrixLoads > 0 )
        {
            // Don't pool this memory, since we'll deallocate it in 
            // assembling display list
            state = C3_CALLOC( 1, sizeof( C3StateMatrixCache ) );

            // Copy over matrix cache and when matrices should be loaded
            memcpy( state->matrixCache, matrixCache, sizeof(u16) * C3_NUM_POS_MATRIX );
            state->index = current;
            state->lastLoaded = *lastLoaded;

            DSInsertListObject( matrixCacheList, NULL, (Ptr)state );
        }

        current++;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3GroupMatrixLoads

  Description:  Groups as much of the single matrix loads together as possible
                to increase efficiency.

  Arguments:    primArray - array of primitives
                primCount - length of primArray
                matrixCacheList - list of matrix loads

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3GroupMatrixLoads( C3Primitive* primArray, u32 primCount, DSList *matrixCacheList )
{
    u32                 i, start, end, primIndex;
    u32                 matrixID, nextFifoLocation, lastFifoLocation;
    C3StateMatrixCache  *stateMatrixCache, *nextState;
    C3Primitive         *cursor;
    C3PtrLink           *ptrLink;
    C3Vertex            *vertex;
    C3Bool              canCombine;

    // Group the first ten loads (Delete the first nine)
    stateMatrixCache = (C3StateMatrixCache*)matrixCacheList->Head;
    C3_ASSERT( stateMatrixCache );
    nextState = (C3StateMatrixCache*)stateMatrixCache->link.Next;
    for( i = 0; i < C3_NUM_POS_MATRIX - 1 && nextState; i++ )
    {       
        DSRemoveListObject( matrixCacheList, (void*)stateMatrixCache );
        C3_FREE( stateMatrixCache );
        stateMatrixCache = nextState;
        nextState = (C3StateMatrixCache*)stateMatrixCache->link.Next;
    }
    stateMatrixCache = (C3StateMatrixCache*)matrixCacheList->Head;
    stateMatrixCache->index = 0;

    // Try to group as much of the remainder matrix loads as possible by
    // finding if the next FIFO location is unused within the current group
    // of primitives
    stateMatrixCache = (C3StateMatrixCache*)stateMatrixCache->link.Next;
    while( stateMatrixCache )
    {
        nextFifoLocation = (stateMatrixCache->lastLoaded + 1) % C3_NUM_POS_MATRIX;
        nextState = (C3StateMatrixCache*)stateMatrixCache->link.Next;
        if( !nextState )
            break;

        lastFifoLocation = nextState->lastLoaded;
        start = stateMatrixCache->index;
        end = nextState->index - 1;

        // for each of the matrices loaded
        canCombine = C3_TRUE;
        for( i = nextFifoLocation; 
             i != ((lastFifoLocation + 1) % C3_NUM_POS_MATRIX) && canCombine; 
             i = (i + 1) % C3_NUM_POS_MATRIX )
        {
            matrixID = stateMatrixCache->matrixCache[i];
            // Iterate through each primitive in this group of the matrix load
            // to see if any vertices use the next FIFO location of the matrix cache.
            primIndex = start;
            while( primIndex <= end && canCombine )
            {
                cursor = &primArray[primIndex];

                // Iterate through the vertex matrices in the current primitive
                // and see if matrixID is unused
                ptrLink = (C3PtrLink*)cursor->vertexPtrList.Head;
                while( ptrLink )
                {
                    vertex = (C3Vertex*)ptrLink->ptr;
                    C3_ASSERT( vertex && vertex->boneIndex );
                    if( *vertex->boneIndex == matrixID )
                    {
                        // This primitive does, so we cannot combine with next matrix load
                        canCombine = C3_FALSE;
                        break;
                    }
                    ptrLink = (C3PtrLink*)ptrLink->link.Next;
                }
                primIndex++;
            }
        }

        // If we can combine, join with the next state
        if( canCombine )
        {
            i = stateMatrixCache->index;
            DSRemoveListObject( matrixCacheList, (void*)stateMatrixCache );
            C3_FREE( stateMatrixCache );
            nextState->index = i;
        }

        stateMatrixCache = nextState;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3FindBestPrimitive

  Description:  Starting at the current index in the primitive array, finds
                the best primitive which requires the least number of
                matrix loads with the given matrix cache.  Then goes ahead
                and loads the matrices needs by the best primitive.

  Arguments:    primArray - array of primitives
                current - index at which to start searching
                end - index at which to stop searching
                matrixCache - matrix cache

  Returns:      index of primitive which requires least number of matrix loads
/*---------------------------------------------------------------------------*/
static u32
C3FindBestPrimitive( C3Primitive *primArray, u32 current, u32 end, u16 *matrixCache, u32 *numMatrixLoads, u8 *lastLoaded )
{
    u32 minNumLoads = C3_NUM_POS_MATRIX + 1;
    u32 i, numLoads, best;
    u16 tempMatrixCache[C3_NUM_POS_MATRIX];
    u16 bestMatrixCache[C3_NUM_POS_MATRIX];
    u8  tempLastLoaded;
    u8  bestLastLoaded;

    // Copy over the matrix cache to the temp matrix cache
    memcpy( tempMatrixCache, matrixCache, sizeof(u16) * C3_NUM_POS_MATRIX );
    tempLastLoaded = *lastLoaded;

    for( i = current; i <= end; i++ )
    {
        // If this primitive requires less number of matrix loads
        // choose it as best candidate.
        numLoads = C3LoadPosMatrices( &primArray[i], tempMatrixCache, &tempLastLoaded );
        if( numLoads < minNumLoads )
        {
            minNumLoads = numLoads;
            best = i;
            memcpy( bestMatrixCache, tempMatrixCache, sizeof(u16) * C3_NUM_POS_MATRIX );
            bestLastLoaded = tempLastLoaded;

            // Best candidate is one that doesn't require any matrix loads
            if( minNumLoads == 0 )
                break;
        }

        // Copy over the matrix cache to the temp matrix cache
        memcpy( tempMatrixCache, matrixCache, sizeof(u16) * C3_NUM_POS_MATRIX );
        tempLastLoaded = *lastLoaded;
    }

    // Copy over the matrix cache that is used for the best primitive
    memcpy( matrixCache, bestMatrixCache, sizeof(u16) * C3_NUM_POS_MATRIX );
    *lastLoaded = bestLastLoaded;

    C3_ASSERT( minNumLoads != C3_NUM_POS_MATRIX + 1 );
    *numMatrixLoads = minNumLoads;

    return best;
}


/*---------------------------------------------------------------------------*
  Name:         C3LoadPosMatrices

  Description:  Loads the position matrices needed by the given primitive 
                into the matrix cache.  The argument lastLoaded aids in 
                determining where to load the next matrix in a FIFO fashion.

  Arguments:    prim - primitive whose matrices we need to load
                matrixCache - matrix cache to load the matrices
                lastLoaded - index of matrix cache which was loaded most recently

  Returns:      number of matrix loads into matrix cache
/*---------------------------------------------------------------------------*/
static u32
C3LoadPosMatrices( C3Primitive *prim, u16 *matrixCache, u8 *lastLoaded )
{
    C3PtrLink *ptrLink; 
    C3Vertex *vertex;
    u32     i;
    u32     numLoads = 0;
    C3Bool  matrixCacheUsed[C3_NUM_POS_MATRIX];

    // We haven't loaded any matrices yet for this primitive
    memset( matrixCacheUsed, C3_FALSE, sizeof(C3Bool) * C3_NUM_POS_MATRIX );

    ptrLink = (C3PtrLink*)prim->vertexPtrList.Head;
    while( ptrLink )
    {
        vertex = (C3Vertex*)ptrLink->ptr;

        C3_ASSERT( vertex->boneIndex );

        // Search through matrix cache to see if matrix is already there
        for( i = 0; i < C3_NUM_POS_MATRIX; i++ )
        {
            // If so, break
            if( *vertex->boneIndex == matrixCache[i] )
            {
                matrixCacheUsed[i] = C3_TRUE;
                break;
            }
        }
        
        // We know matrix isn't in cache if we reached past the end of matrix cache
        if( i == C3_NUM_POS_MATRIX )
        {
            numLoads++;
            *lastLoaded = C3LoadPosMatrix( matrixCache, *vertex->boneIndex, matrixCacheUsed, lastLoaded );
        }

        // Move to next vertex in primitive
        ptrLink = (C3PtrLink*)ptrLink->link.Next;
    }

    return numLoads;
}


/*---------------------------------------------------------------------------*
  Name:         C3LoadPosMatrix

  Description:  Load one position matrix into the matrix cache.
                The argument lastLoaded aids in determining where to load 
                the next matrix in a FIFO fashion.  MatrixCacheUsed lets
                us know which entries in the matrix cache can be used to
                load the position matrix.

  Arguments:    matrixCache - matrix cache to load the matrices
                matrixId - id of matrix to load
                matrixCacheUsed - tells which entries in matrix cache can be overwritten
                lastLoaded - index of matrix cache which was loaded most recently

  Returns:      index at which the position matrix was loaded
/*---------------------------------------------------------------------------*/
static u8
C3LoadPosMatrix( u16 *matrixCache, u16 matrixId, C3Bool *matrixCacheUsed, u8 *lastLoaded )
{
    u8 i, j;

    C3_ASSERT( matrixCache && matrixCacheUsed );

    // Search for an empty spot first
    for( j = 0, i = (*lastLoaded + 1) % C3_NUM_POS_MATRIX; j < C3_NUM_POS_MATRIX; j++, i = (i + 1) % C3_NUM_POS_MATRIX )
    {
        if( matrixCache[i] == 0xFFFF )
        {
            matrixCache[i] = matrixId;
            matrixCacheUsed[i] = C3_TRUE;
            return i;
        }
    }

    // Need to flush out one matrix to load this one
    // So find a spot which isn't used by the current primitive
    for( j = 0, i = (*lastLoaded + 1) % C3_NUM_POS_MATRIX; j < C3_NUM_POS_MATRIX; j++, i = (i + 1) % C3_NUM_POS_MATRIX )
    {
        if( matrixCacheUsed[i] == C3_FALSE )
        {
            matrixCache[i] = matrixId;
            matrixCacheUsed[i] = C3_TRUE;
            return i;           
        }
    }

    // If we get here, that means there is a primitive that requires
    // more than C3_NUM_POS_MATRIX matrices.
    C3ReportError( "Found a primitive that requires more than %d position matrices.",
                    C3_NUM_POS_MATRIX );
    C3_ASSERT( C3_FALSE );
    return 0;
}


/*---------------------------------------------------------------------------*
  Name:         C3AssembleDispList

  Description:  Assemble a list of primitives into a list of state node,
                according to their states.
                Assumes that there is a list of primitive.

  Arguments:    geomObj      - geom object to take the primitives from.
                stateList    - List of state node to fill.
                stateCount   - Pointer to state count to fill.
                primBankSize - Pointer to the size of the prim bank to fill.

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3AssembleDispList( C3GeomObject* geomObj, DSList* stateList, u32* stateCount,
                    u32* primBankSize, DSList* matrixCacheList )
{
    C3Primitive*    prim            = NULL;
    C3Primitive*    prevPrim        = NULL;
    DSList*         curList         = NULL;
    C3StateNode*    stateNode       = NULL;
    C3StateNode*    newStateNode    = NULL;
    u16             state           = 0;
    u32             count           = 0;
    

    C3_ASSERT( geomObj && stateList && stateCount && primBankSize );
   
    // Get the first primitive
    prim = (C3Primitive*)(geomObj->primitiveList.Head);
    prevPrim = NULL;

    // For all the primitives
    while( prim )
    {
        // Get the state
        newStateNode = C3GetStateChange( geomObj, prevPrim, prim, stateNode, matrixCacheList );
        if( newStateNode != stateNode )
        {
            // Add the length of the display list to the primbank size
            if( stateNode )
            {
                (*primBankSize) += stateNode->primListSize;
                C3_PAD32_BYTES( (*primBankSize) );
                DSInsertListObject( stateList, NULL, (Ptr)stateNode );
            }

            stateNode = newStateNode;

            // Count the number of state changes 
            // (only bottom byte so C3_STATE_MTXLOAD doesn't count
            for( state = stateNode->state & 0x0FFF;
                 state > 0;
                 state = state >> 1 )
            {
                (*stateCount) += (state & 0x01);
            }

            // Add the number of matrix loads
            (*stateCount) += stateNode->numMatrixLoads;

            count = 0;
        }

        // Add the primitive header size when the list is empty
        if( !stateNode->primList.Head )
        {
            // Always one at the beginning of the list
            stateNode->primListSize += C3_PRIM_HEADER_SIZE;
        }
        else
        {
            // One if the primitive type changes in the list
            if( ((count + prim->numVertex) >= C3_MAXPRIM_VTX) ||
                !C3SameType( prevPrim, prim ) )
            {
                stateNode->primListSize += C3_PRIM_HEADER_SIZE;
                count = 0;
            }
        }

        // Get the size of the primitive
        stateNode->primListSize += C3GetPrimSize( geomObj, prim );
        count += prim->numVertex;

        // Remove the primitive from the current list
        DSRemoveListObject( &geomObj->primitiveList, (Ptr)prim );

        // Incrememt the number of vertices in the primitive
        // so we can compute strip/fan effectiveness in stats
        if ( prim->type == C3_PRIM_TRI || prim->type == C3_PRIM_QUAD ||
             prim->type == C3_PRIM_STRIP || prim->type == C3_PRIM_FAN )
            geomObj->stats->numVertexCallsForStripping += prim->numVertex;

        // Insert in the new list
        DSInsertListObject( &stateNode->primList, NULL, (Ptr)prim );

        prevPrim = prim;    
        prim = (C3Primitive*)(geomObj->primitiveList.Head);
    }

    // Add the last state node to the prim bank size
    if (stateNode) 
    {
        (*primBankSize) += stateNode->primListSize;
        C3_PAD32_BYTES( (*primBankSize) );  

        // Insert the last node in the list
        DSInsertListObject( stateList, NULL, (Ptr)stateNode );
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3GetPrimSize

  Description:  Calculate the size of the specified primitive.

  Arguments:    obj - geometry object which contains index quantization info
                prim - primitive to evaluate.

  Returns:      Size, in byte, of the primitive in the display list.
/*---------------------------------------------------------------------------*/
static u16
C3GetPrimSize( C3GeomObject* obj, C3Primitive* prim )
{
    C3Vertex*   v         = NULL;
    u8          indexSize = 0;
    u16         size      = 0;
    u8          i         = 0;

    C3_ASSERT( prim );

    v = (C3Vertex*)(((C3PtrLink*)(prim->vertexPtrList.Head))->ptr);

    if( v->boneIndex )
        switch( C3GetVCDDataType( obj, C3_TARGET_POS_MATRIX, 0 ) )
        {
            case C3_VCD_DIRECT:
                size += 1;
                break;
            default:
                C3_ASSERT( C3_FALSE );
                break;
        }

    if( v->position )
        switch( C3GetVCDDataType( obj, C3_TARGET_POSITION, 0 ) )
        {
            case C3_VCD_INDEX8:
                size += 1;
                break;
            case C3_VCD_INDEX16:
                size += 2;
                break;
            default:
                C3_ASSERT( C3_FALSE );
                break;
        }

    if( v->normal )
        switch( C3GetVCDDataType( obj, C3_TARGET_NORMAL, 0 ) )
        {
            case C3_VCD_INDEX8:
                size += 1;
                break;
            case C3_VCD_INDEX16:
                size += 2;
                break;
            case C3_VCD_NONE:
                break;
            default:
                C3_ASSERT( C3_FALSE );
                break;
        }

    if( v->color )
        switch( C3GetVCDDataType( obj, C3_TARGET_COLOR, 0 ) )
        {
            case C3_VCD_INDEX8:
                size += 1;
                break;
            case C3_VCD_INDEX16:
                size += 2;
                break;
            case C3_VCD_NONE:
                break;
            default:
                C3_ASSERT( C3_FALSE );
                break;
        }

    for( i = 0; i < C3_MAX_TEXTURES; i++ )
    {
        if( v->tCoord[i] )
            switch( C3GetVCDDataType( obj, C3_TARGET_TEXCOORD, i ) )
            {
                case C3_VCD_INDEX8:
                    size += 1;
                    break;
                case C3_VCD_INDEX16:
                    size += 2;
                    break;
                case C3_VCD_NONE:
                    break;
                default:
                    C3_ASSERT( C3_FALSE );
                    break;
            }
    } 

    size *= prim->numVertex;

    return size;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetStateChange

  Description:  Compares the primitive state with the previous one and
                add the primitive to the state node list if they are the 
                same or create a new state node if they are different.

  Arguments:    obj       - current geometry object
                prevPrim  - previous primitive in the list (can be null)
                prim      - current primitive
                stateNode - Current state node (can be null)  

  Returns:      current state node for the primitive (could be new).
/*---------------------------------------------------------------------------*/
static C3StateNode*
C3GetStateChange( C3GeomObject* obj, C3Primitive* prevPrim, C3Primitive* prim, 
                  C3StateNode* stateNode, DSList* matrixCacheList )
{
    u16             state   = C3_STATE_NONE;
    C3StateNode*    newNode = stateNode;
    C3StateMatrixCache *matrixCacheState;

    C3_ASSERT( prim );

    // Verify if there is a state change
    if( prevPrim != prim )
    {
        state = C3GetState( prevPrim, prim, matrixCacheList, &matrixCacheState );
    }

    // Allocate a state node if needed
    if( state || !stateNode )
    {
        // Allocate
        newNode = C3AllocStateNodeFromPrim( obj, prim, matrixCacheState );

        // Set the state
        newNode->state = state;
    }

    return newNode;
}


/*---------------------------------------------------------------------------*
  Name:         C3NeedToLoadPosMatrices

  Description:  Given the current primitive, checks the matrix cache to 
                see if any matrices need to be loaded before the primitive
                can be drawn.

  Arguments:    prim      - current primitive
                matrixCacheList - list of matrix caches

  Returns:      NULL if no matrices need to be loaded.
                Otherwise, the matrix cache needed for the primitive.
/*---------------------------------------------------------------------------*/
static C3StateMatrixCache*
C3NeedToLoadPosMatrices( C3Primitive* prim, DSList* matrixCacheList )
{
    C3StateMatrixCache *current;

    current = (C3StateMatrixCache*)matrixCacheList->Head;
    while( current )
    {
        if( current->index == prim->index )
            return current;

        current = (C3StateMatrixCache*)current->link.Next;
    }

    return NULL;
}


/*---------------------------------------------------------------------------*
  Name:         C3AllocStateNodeFromPrim

  Description:  Allocates a state node and initialize the state according to
                the specified primitive.

  Arguments:    obj  - current geometry object which contains index quant. info
                prim - base primitive for the state initialization.

  Returns:      new state node allocated.
/*---------------------------------------------------------------------------*/
static C3StateNode*
C3AllocStateNodeFromPrim( C3GeomObject* obj, C3Primitive* prim, C3StateMatrixCache *matrixCacheState )
{
    C3StateNode*    stateNode   = NULL;
    u8              i           = 0;
    C3Vertex*       vtx         = NULL;
    C3PtrLink*      vtxPtrLink  = NULL;
    u32             dataType    = 0;
    C3StateMatrixCache *prev;

    C3_ASSERT( prim );

    vtx = (C3Vertex*)(((C3PtrLink*)(prim->vertexPtrList.Head))->ptr);

    // Allocate the state
    stateNode = (C3StateNode*)C3_CALLOC( 1, sizeof(C3StateNode) );
    C3_ASSERT( stateNode );

    // Set the texture
    for( i = 0; i < C3_MAX_TEXTURES; i++ )
        stateNode->tex[i] = prim->texture[i];

    // Set the flag that tells whether all vertex colors are white
    stateNode->colorAllWhite = obj->colorAllWhite;

    // Set the flag that tells whether or not we are lighting
    if( C3GetVCDDataType( obj, C3_TARGET_NORMAL, 0 ) == C3_VCD_NONE )
        stateNode->lighting = C3_FALSE;
    else
        stateNode->lighting = C3_TRUE;

    // Set the vertex component descriptor
    stateNode->vtxCompDesc = 0;

    if( vtx->boneIndex )
    {
        dataType = C3GetVCDDataType( obj, C3_TARGET_POS_MATRIX, 0 );
        dataType <<= C3_VCD_SHIFT_POS_MATRIX;
        stateNode->vtxCompDesc |= dataType;
    }
    
    if( vtx->position ) 
    {
        dataType = C3GetVCDDataType( obj, C3_TARGET_POSITION, 0 );
        dataType <<= C3_VCD_SHIFT_POSITION;
        stateNode->vtxCompDesc |= dataType;
    }

    if( vtx->normal )
    {
        dataType = C3GetVCDDataType( obj, C3_TARGET_NORMAL, 0 );
        dataType <<= C3_VCD_SHIFT_NORMAL;
        stateNode->vtxCompDesc |= dataType;
    }

    if( vtx->color )
    {
        dataType = C3GetVCDDataType( obj, C3_TARGET_COLOR, 0 );
        dataType <<= C3_VCD_SHIFT_COLOR_0;
        stateNode->vtxCompDesc |= dataType;
    }

    for( i = 0; i < C3_MAX_TEXTURES; i++ )
    {
        if( vtx->tCoord[i] )
        {
            dataType = C3GetVCDDataType( obj, C3_TARGET_TEXCOORD, i );
            dataType <<= (C3_VCD_SHIFT_TCOORD_BASE + (C3_VCD_SHIFT_TCOORD_INCREMENT * i));
            stateNode->vtxCompDesc |= dataType;
        }
    }

    // Set the state for position matrix loads

    // Remap the bone index (source matrix id) to dest matrix id in hardware
    stateNode->numMatrixLoads = 0;
    if( vtx->boneIndex && matrixCacheState )
    {
        memcpy( stateNode->matrixCache, matrixCacheState->matrixCache, sizeof(u16) * C3_NUM_POS_MATRIX );

        // Count the number of matrix loads
        prev = (C3StateMatrixCache*)matrixCacheState->link.Prev;
        if( prev )
        {
            // Count the number of changes since last matrix cache
            for( i = 0; i < C3_NUM_POS_MATRIX; i++ )
                if( prev->matrixCache[i] != matrixCacheState->matrixCache[i] )
                    stateNode->numMatrixLoads++;
        }
        else
        {
            // If first matrix cache
            for( i = 0; i < C3_NUM_POS_MATRIX; i++ )
                if( matrixCacheState->matrixCache[i] != 0xFFFF )
                    stateNode->numMatrixLoads++;
        }
    }

    // Initialize the list
    DSInitList( &stateNode->primList, (Ptr)prim, &(prim->link) );

    return stateNode;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetState

  Description:  Compares two primitives and determine what states have changed.
                Can be used to get the state of one primitive. 

  Arguments:    prev - previous primitive in the list.
                next - Current primitive.

  Returns:      State difference between the two primitives.
/*---------------------------------------------------------------------------*/
static u16
C3GetState( C3Primitive* prev, C3Primitive* next, DSList *matrixCacheList, C3StateMatrixCache **matrixCacheState )
{
    u16         state   = C3_STATE_NONE;
    C3Vertex*   v1      = NULL;
    C3Vertex*   v2      = NULL;
    u8          i       = 0;

    C3_ASSERT( next );

    *matrixCacheState = NULL;

    if( prev )
        v1 = (C3Vertex*)(((C3PtrLink*)(prev->vertexPtrList.Head))->ptr);
    v2 = (C3Vertex*)(((C3PtrLink*)(next->vertexPtrList.Head))->ptr);

    // If first primitive
    if( next && !prev )
    {
        // Set all the flags
        for( i = 0; i < C3_MAX_TEXTURES; i++ )
        {
            if( v2->tCoord[i] )
                state |= C3_STATE_TEXTURE0 << i;
        }        

        if( v2->boneIndex )
        {
            *matrixCacheState = C3NeedToLoadPosMatrices( next, matrixCacheList );
            C3_ASSERT( *matrixCacheState );
            state |= C3_STATE_MTXLOAD;
        }

        state |= (C3_STATE_TEXTURE_COMBINE | C3_STATE_VCD);
    }
    else
    {
        // Verify the textures
        for( i = 0; i < C3_MAX_TEXTURES; i++ )
        {
            // If they have textures
            if( v1->tCoord[i] && v2->tCoord[i] )
            {
                // If they are different
                if( !C3CompareTexturesMore( (void*)prev->texture[i], (void*)next->texture[i]) )
                    state |= C3_STATE_TEXTURE0 << i;
            }
            else if( v1->tCoord[i] && !v2->tCoord[i] )
            {
                state |= C3_STATE_VCD | C3_STATE_TEXTURE_COMBINE;
            }
            else if( !v1->tCoord[i] && v2->tCoord[i] )
            {
                state |= (C3_STATE_TEXTURE0 << i) | C3_STATE_VCD | C3_STATE_TEXTURE_COMBINE;
            }
        }
    
        // Verify the vertex attributes
        if( (v1->boneIndex && !v2->boneIndex) ||    // boneIndex is a pointer
            (!v1->boneIndex && v2->boneIndex) ||
            C3_XOR(v1->position, v2->position) || 
            C3_XOR(v1->normal, v2->normal) ||
            C3_XOR(v1->color, v2->color) )
        {
            state |= C3_STATE_VCD; 
        }

        // Verify the matrices
        if( v2->boneIndex )
        {
            if( (*matrixCacheState = C3NeedToLoadPosMatrices( next, matrixCacheList )) )
                state |= C3_STATE_MTXLOAD;
        }
    }

    return state;
}


/*---------------------------------------------------------------------------*
  Name:         C3CreateDispList

  Description:  Convert a state node list to a display list definded in the
                DOLayout object

  Arguments:    obj          - current geometry object
                stateList    - state node list to convert
                displayData  - display data header to fill (will be filled)
                primBankSize - size of the primitive bank (precomputed size)
                stateCount   - number of states (precomputed number of states)

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3CreateDispList( C3GeomObject* obj, DSList* stateList, DODisplayHeader* displayData,
                  u32 primBankSize, u32 stateCount )
{
    DODisplayState*   stateArray      = NULL;
    void*             primBank        = NULL;
    void*             statePrimBank   = NULL;
    C3StateNode*      stateNode       = NULL;
    u32               primListLength  = 0;


    C3_ASSERT( stateList && displayData );

    // Allocate memory for primitive bank
    primBank = C3_CALLOC( 1, primBankSize );
    C3_ASSERT( primBank );
    memset( primBank, 0, primBankSize );
    displayData->primitiveBank = primBank;

    // Allocate memory for the state array
    stateArray = (DODisplayState*)C3_CALLOC( stateCount, sizeof(DODisplayState) );
    C3_ASSERT( stateArray );
    displayData->displayStateList = stateArray;

    displayData->numStateEntries = (u16)stateCount;

    // Convert the nodes and primitives
    stateNode = (C3StateNode*)stateList->Head;
    while( stateNode )
    {
        statePrimBank = primBank;
        C3ConvertPrimList( obj, stateNode, &primBank, &primListLength );

        C3ConvertState( stateNode, &stateArray, statePrimBank, primListLength );
    
        stateNode = (C3StateNode*)stateNode->link.Next;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3ConvertPrimList

  Description:  Converts the primitive list of the given state into primBank

  Arguments:    obj            - object with primitive list (need VCD info)
                stateNode      - state node which contains primitive list to convert
                primBank       - destination of converted primitive list
                primListLength - returns the length of the primitive list

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3ConvertPrimList( C3GeomObject* obj, C3StateNode* stateNode, void** primBank, 
                   u32* primListLength )
{
    C3Primitive*    prim     = NULL;
    C3Primitive*    prevPrim = NULL;
    u16*            countPtr = NULL;
    u16             count    = 0;
    C3PtrLink*      vtxPtr   = NULL;
    C3Vertex*       vtx      = NULL;
    C3StateNode*    currentStateNode;

    C3_ASSERT( stateNode && primBank && primListLength );

    *primListLength = stateNode->primListSize;

    // If necessary, find the matrix cache used by all of these primitives
    currentStateNode = stateNode;
    while( currentStateNode )
    {
        if( currentStateNode->numMatrixLoads > 0 )
            break;

        currentStateNode = (C3StateNode*)currentStateNode->link.Prev;
    }

    // For all primitives
    prim = (C3Primitive*)stateNode->primList.Head;
    while( prim )
    {
        // Write the header if needed
        if( ((count + prim->numVertex) >= C3_MAXPRIM_VTX) ||
            !C3SameType( prevPrim, prim ) )
        {
            if( countPtr )
            {
                *countPtr = count;
                count = 0;
            }

            // Write the type of primitive, 5 upper bits
            *((u8*)(*primBank)) = C3GetConvertPrimType( prim );
            ((u8*)(*primBank))++;
            
            // Keep track of the place for the count
            countPtr = (u16*)(*primBank);
            ((u16*)(*primBank))++;
        }

        // For each vertex      
        vtxPtr = (C3PtrLink*)prim->vertexPtrList.Head;  
        while( vtxPtr )
        {
            vtx = (C3Vertex*)vtxPtr->ptr;    
            C3ConvertVtxIndexed( obj, vtx, primBank, (currentStateNode)?(currentStateNode->matrixCache):NULL ); // write the components
            vtxPtr = (C3PtrLink*)vtxPtr->link.Next;
            count++;
        }

        prevPrim = prim;
        prim = (C3Primitive*)prim->link.Next;
    }
 
    // Write the last count
    *countPtr = count;
}


/*---------------------------------------------------------------------------*
  Name:         C3SameType

  Description:  Returns true if both primitives can be combined in the
                display list.

  Arguments:    p1 - first primitive to compare
                p2 - first primitive to compare

  Returns:      true if both primitives can be combined in the display list
/*---------------------------------------------------------------------------*/
static C3Bool 
C3SameType( C3Primitive* p1, C3Primitive* p2 )
{
    C3Bool same = C3_FALSE;

    if( p1 && p2 )
    {
        if( p1->type == p2->type &&
            C3SingleType( p1->type ) && 
            C3SingleType( p2->type ) )
        {
            same = C3_TRUE;
        }
    }

    return same;
}


/*---------------------------------------------------------------------------*
  Name:         C3SingleType

  Description:  Returns true if the given type can have multiple primitives in the
                display list per header.  In the display list, multiple triangles,
                quads, points, and lines can have one header, whereas triangle
                strips, fans, and line strips need a header for each primitive.

  Arguments:    type - primitive type

  Returns:      true if given type can be have multiple primitives
                in the display list per header
/*---------------------------------------------------------------------------*/
static C3Bool
C3SingleType( u8 type )
{    
    C3Bool single;

    switch( type )
    {
        case C3_PRIM_TRI:
        case C3_PRIM_QUAD:
        case C3_PRIM_POINT:
        case C3_PRIM_LINE:
            single = C3_TRUE;
            break;

        case C3_PRIM_STRIP:
        case C3_PRIM_LINE_STRIP:
        case C3_PRIM_FAN:
            single = C3_FALSE;
            break;

        default:
            C3_ASSERT( C3_FALSE );
            break;
    }

    return single;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetConvertPrimType

  Description:  Returns the run time primitive type of the specified primitive

  Arguments:    prim - the primitive to get the type from.

  Returns:      run time primitive type.
/*---------------------------------------------------------------------------*/
static u8
C3GetConvertPrimType( C3Primitive* prim )
{
    u8  type = 0;
    u16 temp = prim->type;

    switch( prim->type )
    {
        case C3_PRIM_TRI:
            type = GX_DRAW_TRIANGLES;
            break;
        
        case C3_PRIM_QUAD:
            type = GX_DRAW_QUADS;
            break;

        case C3_PRIM_STRIP:
            type = GX_DRAW_TRIANGLE_STRIP;
            break;

        case C3_PRIM_FAN:
            type = GX_DRAW_TRIANGLE_FAN;
            break;

        case C3_PRIM_POINT:
            type = GX_DRAW_POINTS;
            break;

        case C3_PRIM_LINE:
            type = GX_DRAW_LINES;
            break;
        
        case C3_PRIM_LINE_STRIP:    
            type = GX_DRAW_LINE_STRIP;
            break;

        default:
            C3_ASSERT( C3_FALSE );
            break;
    }

    return type;
}


/*---------------------------------------------------------------------------*
  Name:         C3ConvertVtxIndexed

  Description:  Converts a vertex to an indexed vertex in the specified
                primitive bank.

  Arguments:    obj      - current geometry object
                vtx      - vertex to convert  
                primBank - location to store the vertex (pointer updated) 

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3ConvertVtxIndexed( C3GeomObject* obj, C3Vertex* vtx, void** primBank, u16* matrixCache )
{
    u8  i = 0;
    u32 index;

    // If there is a position matrix
    if( vtx->boneIndex )
    {
        C3_ASSERT( C3GetVCDDataType( obj, C3_TARGET_POS_MATRIX, 0 ) == C3_VCD_DIRECT );
        C3_ASSERT( matrixCache );

        for( i = 0; i < C3_NUM_POS_MATRIX; i++ )
        {
            if( *vtx->boneIndex == matrixCache[i] )
            {
                C3ConvertIndex( obj, i, primBank, C3_TARGET_POS_MATRIX, 0 );
                break;
            }
        }

        if( i == C3_NUM_POS_MATRIX )
        {
            C3ReportError( "Internal error: Could not find matrix %d in cache. Matrix Cache %d %d %d %d %d %d %d %d %d %d", 
                           *vtx->boneIndex, matrixCache[0], matrixCache[1], matrixCache[2], 
                           matrixCache[3], matrixCache[4], matrixCache[5], matrixCache[6], 
                           matrixCache[7], matrixCache[8], matrixCache[9] );
            // For now, just choose 0th matrix
            C3ConvertIndex( obj, 0, primBank, C3_TARGET_POS_MATRIX, 0 );
        }
    }
    
    // If there is a position
    if( vtx->position )
        C3ConvertIndex( obj, vtx->position->index, primBank, C3_TARGET_POSITION, 0 );

    // If there is a normal
    if( vtx->normal && C3GetOptionEnableLighting() )
    {
        if ( C3GetOptionUseDefaultNormalTable() ||
             C3GetOptionUseExternalNormalTable() )
        {
            // Find the closest normal in default or external table and pick it
            index = C3FindNormalTableIndex( vtx->normal );
            C3ConvertIndex( obj, index, primBank, C3_TARGET_NORMAL, 0 );
        }
        else
            C3ConvertIndex( obj, vtx->normal->index, primBank, C3_TARGET_NORMAL, 0 );
    }

    // If there is a color
    if( vtx->color )
        C3ConvertIndex( obj, vtx->color->index, primBank, C3_TARGET_COLOR, 0 );

    for( i = 0; i < C3_MAX_TEXTURES; i++ )
    {
        // If there is a texture coord
        if( vtx->tCoord[i] )
            C3ConvertIndex( obj, vtx->tCoord[i]->index, primBank, C3_TARGET_TEXCOORD, i );
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3ConvertIndex

  Description:  Convert an index to the proper index size.
                Size determined by the vertex descriptor component and the 
                target.

  Arguments:    obj     - current geometry object
                index   - index to convert
                ptr     - location to store the index (pointer updated)
                target  - target for which the index is applied
                channel - texture coordinate channel if target is C3_TARGET_TEXCOORD

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3ConvertIndex( C3GeomObject* obj, u32 index, void** ptr, u32 target, u8 channel )
{
    switch( C3GetVCDDataType( obj, target, channel ) )
    {
        case C3_VCD_NONE:
            break;

        case C3_VCD_INDEX8:
            *((u8*)(*ptr)) = (u8)index;
            ((u8*)(*ptr))++;
            break;

        case C3_VCD_INDEX16:
            *((u16*)(*ptr)) = (u16)index;
            ((u16*)(*ptr))++;
            break;

        case C3_VCD_DIRECT:
            C3_ASSERT( target == C3_TARGET_POS_MATRIX );
            *((u8*)(*ptr)) = (u8)C3GetGxPosMtxIndex( (u8)index );
            ((u8*)(*ptr))++;
            break;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3ConvertState

  Description:  Decompose each state in the state node and create a runtime
                display state for each. Updates the state array pointer.
    
  Arguments:    stateNode       - State node to convert
                stateArray      - array to fill (pointer updated)
                primBank        - primitive bank for the states
                primListLength  - lenght of the primitive list

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3ConvertState( C3StateNode* stateNode, DODisplayState** stateArray, 
                void* primBank, u32 primListLength )
{
    DODisplayState* lastState = NULL;
    u8              magFilter;
    u8              i;
    u8              numMatrixLoads;
    u32             channel;
    C3StateNode     *prevStateNode;
//    FILE* f = fopen( "D:\\temp\\state.txt", "at" );

    // Decompose each state in the state node    
    for( channel = 0; channel < C3_MAX_TEXTURES; channel++ )
    {
        if( stateNode->state & (C3_STATE_TEXTURE0 << channel) )
        {
            (*stateArray)->id = DISPLAY_STATE_TEXTURE;

            // Set the texture index
            // Set min and mag filter to be the same

            // But mag filter can only be GX_NEAR or GX_LINEAR...
            if( stateNode->tex[channel]->filterType == C3_FILTER_NEAR )
                magFilter = C3_FILTER_NEAR;
            else
                magFilter = C3_FILTER_LINEAR; // for mipmap, change to LINEAR

            (*stateArray)->setting = (((magFilter << 4) | stateNode->tex[channel]->filterType) << 24) |
                                      (stateNode->tex[channel]->tiling << 16) |
                                      (channel << 13) |
                                      (stateNode->tex[channel]->index & 0x1FFF);

            lastState = (*stateArray);
            (*stateArray)++;

//            fprintf( f, "State: %s\n", C3_STR(DISPLAY_STATE_TEXTURE) );
//            fprintf( f, "\tSetting: 0x%x\n", lastState->setting );
        }
    }
    
    if( stateNode->state & C3_STATE_TEXTURE_COMBINE )
    {
        (*stateArray)->id = DISPLAY_STATE_TEXTURE_COMBINE;

        // Set the texture combine
        if( stateNode->tex[0] && stateNode->colorAllWhite && !stateNode->lighting )
            (*stateArray)->setting = GX_REPLACE;        
        else if( stateNode->tex[0] )
            (*stateArray)->setting = GX_MODULATE;
        else
            (*stateArray)->setting = GX_PASSCLR;

        lastState = (*stateArray);
        (*stateArray)++;

//        fprintf( f, "State: %s\n", C3_STR(DISPLAY_STATE_TEXTURE_COMBINE) );
//        fprintf( f, "\tSetting: 0x%x\n", lastState->setting );
    }
    
    if( stateNode->state & C3_STATE_VCD )
    {
        (*stateArray)->id      = DISPLAY_STATE_VCD;
        (*stateArray)->pad8    = 0;
        (*stateArray)->pad16   = 0;

        (*stateArray)->setting = stateNode->vtxCompDesc;

        lastState = (*stateArray);
        (*stateArray)++;

//        fprintf( f, "State: %s\n", C3_STR(DISPLAY_STATE_VCD) );
//        fprintf( f, "\tSetting: 0x%x\n", lastState->setting );    
    }

    if( stateNode->state & C3_STATE_MTXLOAD )
    {
        C3_ASSERT( stateNode->numMatrixLoads > 0 && stateNode->numMatrixLoads <= C3_NUM_POS_MATRIX );
        numMatrixLoads = 0;

        // Find a previous state node with the last matrix cache
        prevStateNode = (C3StateNode*)stateNode->link.Prev;
        while( prevStateNode )
        {
            if( prevStateNode->numMatrixLoads > 0 )
                break;

            prevStateNode = (C3StateNode*)prevStateNode->link.Prev;
        }

        // Load matrices
        if( prevStateNode )
        {
            for( i = 0; i < C3_NUM_POS_MATRIX; i++ )
                if( prevStateNode->matrixCache[i] != stateNode->matrixCache[i] )
                {
                    (*stateArray)->id      = DISPLAY_STATE_MTXLOAD;
                    (*stateArray)->pad8    = 0;
                    (*stateArray)->pad16   = 0;

                    (*stateArray)->setting = (stateNode->matrixCache[i] << 16) | i;

                    lastState = (*stateArray);
                    (*stateArray)++;

                    numMatrixLoads++;
                    
//                    fprintf( f, "State: %s\n", C3_STR(DISPLAY_STATE_MTXLOAD) );
//                    fprintf( f, "\tSource Mtx %d at %d\n", lastState->setting >> 16, lastState->setting & 0xFFFF );
                }

            C3_ASSERT( stateNode->numMatrixLoads == numMatrixLoads );
        }
        else
        {
            // If first matrix cache
            for( i = 0; i < C3_NUM_POS_MATRIX; i++ )
                if( stateNode->matrixCache[i] != 0xFFFF )
                {
                    (*stateArray)->id      = DISPLAY_STATE_MTXLOAD;
                    (*stateArray)->pad8    = 0;
                    (*stateArray)->pad16   = 0;

                    (*stateArray)->setting = (stateNode->matrixCache[i] << 16) | i;

                    lastState = (*stateArray);
                    (*stateArray)++;

                    numMatrixLoads++;
                    
//                    fprintf( f, "State: %s\n", C3_STR(DISPLAY_STATE_MTXLOAD) );
//                    fprintf( f, "\tSource Mtx %d at %d\n", lastState->setting >> 16, lastState->setting & 0xFFFF );
                }

            C3_ASSERT( stateNode->numMatrixLoads == numMatrixLoads );
        }
    }

    C3_ASSERT( lastState );
    
    // set the pointers in the last state
    lastState->primitiveList = primBank;
    lastState->listSize      = primListLength;

//    fprintf( f, "\tPrimitiveList: %d, Size: %d\n", primBank, primListLength );
//    fclose( f );
}


/*---------------------------------------------------------------------------*
  Name:         C3FreeDOLayout

  Description:  Frees a DOLayout object allocated in the C3GeoCnv module

  Arguments:    doLayout - object to free

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3FreeDOLayout( DOLayout** doLayout )
{
    u8  channel;

    C3_ASSERT( doLayout );

    if( !(*doLayout) )
        return;

    if( (*doLayout)->positionData )
    {
        C3_FREE( (*doLayout)->positionData->positionArray );
        C3_FREE( (*doLayout)->positionData );
    }

    if( (*doLayout)->colorData )
    {
        C3_FREE( (*doLayout)->colorData->colorArray );
        C3_FREE( (*doLayout)->colorData );
    }

    if( (*doLayout)->textureData )
    {
        for( channel = 0; channel < (*doLayout)->numTextureChannels; channel++ )
        {
            C3_FREE( (*doLayout)->textureData[channel].textureCoordArray );
        }
        C3_FREE( (*doLayout)->textureData );
    }

    if( (*doLayout)->displayData )
    {
        C3_FREE( (*doLayout)->displayData->primitiveBank );
        C3_FREE( (*doLayout)->displayData->displayStateList );
        C3_FREE( (*doLayout)->displayData );
    }

    if( (*doLayout)->lightingData )
    {
        C3_FREE( (*doLayout)->lightingData->normalArray );
        C3_FREE( (*doLayout)->lightingData );
    }

    C3_FREE( *doLayout );
    *doLayout = NULL;
}


/*---------------------------------------------------------------------------*
  Name:         C3CompareTexturesMore

  Description:  Compares not only the image and palette names for the texture,
                but also compares tiling and filter type.

  Arguments:    normal - normal to find index of

  Returns:      index of closest normal in external normal table
/*---------------------------------------------------------------------------*/
static C3Bool
C3CompareTexturesMore( void* t1, void* t2 )
{
    C3_ASSERT( t1 && t2 );

    return ( ( ((C3Texture*)t1)->tiling == ((C3Texture*)t2)->tiling ) &&
             ( ((C3Texture*)t1)->filterType == ((C3Texture*)t2)->filterType ) &&
             C3CompareTextures(t1,t2) );

}


/*---------------------------------------------------------------------------*
  Name:         C3FindNormalTableIndex

  Description:  Don't use the normal's index into normal array, but since we
                are presumably using a normal table, find the closest
                normal in the normal table and return its index.  Can handle
                default or specified normal table.

  Arguments:    normal - normal to find index of

  Returns:      index of closest normal in external normal table
/*---------------------------------------------------------------------------*/
static u32
C3FindNormalTableIndex( C3Normal* normal )
{
    C3Normal* cursor;
    f32 minDistance = FLT_MAX;
    u32 minIndex;
    f32 distance;
    u32 i;
//  FILE* f;

    if ( C3GetOptionUseDefaultNormalTable() )
    {
        for( i = 0; i < normalTableNumNormals; i++ )
        {
            distance = (f32)sqrt(pow(normal->x - normalTable[i][0], 2) +
                                 pow(normal->y - normalTable[i][1], 2) +
                                 pow(normal->z - normalTable[i][2], 2) );
            if ( distance < minDistance )
            {
                minIndex = i;
                minDistance = distance;
            }
        }
    }
    else if ( C3GetOptionUseExternalNormalTable() )
    {
        C3_ASSERT( C3externalNormalList.Head );

        cursor = (C3Normal*)C3externalNormalList.Head;
        while (cursor)
        {
            distance = (f32)sqrt(pow(normal->x - cursor->x, 2) +
                                 pow(normal->y - cursor->y, 2) +
                                 pow(normal->z - cursor->z, 2) );

            if ( distance < minDistance )
            {
                minIndex = cursor->index;
                minDistance = distance;
            }

            cursor = (C3Normal*)cursor->link.Next;
        }
    }

    // TESTING
    /*
    f = fopen("D:\\temp\\nrmlTest.txt", "at");
    fprintf(f, "Normal x: %.15f y: %.15f z: %.15f\n", normal->x, normal->y, normal->z);
    fprintf(f, "\tClosest to index: %d with distance %.15f\n", minIndex, minDistance);
    fclose( f );
    */

    return minIndex;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetGxPosMtxIndex

  Description:  Returns the GX enumeration for the 10 position matrix indices.

  Arguments:    index - zero-based index of position matrix index

  Returns:      GX enumeration of position matrix index
/*---------------------------------------------------------------------------*/
static u8
C3GetGxPosMtxIndex( u8 index )
{
    switch( index )
    {
    case 0: return GX_PNMTX0;
    case 1: return GX_PNMTX1;
    case 2: return GX_PNMTX2;
    case 3: return GX_PNMTX3;
    case 4: return GX_PNMTX4;
    case 5: return GX_PNMTX5;
    case 6: return GX_PNMTX6;
    case 7: return GX_PNMTX7;
    case 8: return GX_PNMTX8;
    case 9: return GX_PNMTX9;
    default:
        C3_ASSERT( C3_FALSE );
    }

    return 0;
}
