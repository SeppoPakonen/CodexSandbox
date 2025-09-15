/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3GeoOut.c]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/build/charPipeline/c3/src/C3GeoOut.c $
    
    19    12/04/00 7:56p John
    Added multitexturing ability (work in progress).
    
    18    8/14/00 6:15p John
    Added user-defined data to ANM, ACT, and GPL formats.
    
    17    7/18/00 7:27p John
    Removed unnecessary configuration argument.
    Removed C3Text code (which was commented out).
    
    16    6/27/00 6:07p John
    Added optimization to not include color indices if there is only one
    color for a geometry object (assumes runtime will set register color).
    
    15    5/22/00 9:00p John
    Fixed bug that used to include zero-length display lists.
    
    14    3/23/00 2:11p John
    Modified function names to runtime library changes.
    
    13    3/23/00 11:12a John
    Changed version date for updated GPL format.
    
    12    3/15/00 2:32p John
    Fixed color quantization bug (incorrect component size was calculated).
    
    11    3/14/00 1:33p John
    Moved display priority from GPL to ACT.
    
    10    2/15/00 4:23p John
    Added ability to export colors always in big endian.
    
    9     2/04/00 6:07p John
    Untabified code.
    
    8     1/31/00 4:15p John
    Added pruning of unnecessary bones and animations.
    Finished stitching with FIFO matrix cache and stripping.
    
    7     1/20/00 1:14p John
    Added stitching (work in progress).
    Added C3 string table for efficient management of strings.
    
    6     1/04/00 6:58p John
    Added little/big endian output  option
    
    5     1/04/00 1:12p John
    Added better support for second texture channel.
    
    4     12/20/99 11:21a John
    Added second texture channel.
    
    3     12/06/99 3:25p John
    Fixed memory leak.
    
    2     11/22/99 6:31p Ryan
    
    20    11/19/99 4:10p John
    
    27    11/18/99 1:17p John
    Does not send texture coordinate array or header if no tex coords.
    Pads display list to 32 byte boundary.
    
    26    11/11/99 1:16p John
    Removed some commented out code.
    
    25    10/21/99 6:23p John
    Added drawing priority.
    
    24    10/14/99 5:42p John
    Doesn't have creation date anymore.
    
    23    10/14/99 4:36p John
    
    22    10/14/99 4:32p John
    Added GPL version and creation date.
    
    21    10/13/99 3:32p John
    Modified code to accomodate new PositionHeader and LightingHeader.
    
    20    10/13/99 2:53p John
    Added lighting and default normal table.
    
    19    9/29/99 4:31p John
    Changed header to make them all uniform with logs.
    Added position and normal quantization.
    Added external normal table.
    
    18    9/21/99 6:41p John
    Minor cleaning up of code.
    
    17    9/20/99 7:04p John
    Gets stats for indirect vs. direct mode for positions, 
    but code is commented out.
    
    16    9/17/99 9:34a John
    Added C3Options and cleaned up code.
    
    15    9/14/99 6:50p John
    Added stats per object.  Computes them too.

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#include "C3Private.h"

#define C3_GEOPALETTE_HEADER_SIZE       sizeof(GEOPalette)
#define C3_DISPLAYOBJECT_HEADER_SIZE    sizeof(DOLayout)
#define C3_POSITION_HEADER_SIZE         sizeof(DOPositionHeader)
#define C3_LIGHTING_HEADER_SIZE         sizeof(DOLightingHeader)
#define C3_COLOR_HEADER_SIZE            sizeof(DOColorHeader)
#define C3_TEXTURE_HEADER_SIZE          sizeof(DOTextureDataHeader)
#define C3_DISPLAY_DATA_HEADER_SIZE     sizeof(DODisplayHeader)
#define C3_DISPLAY_STATE_SIZE           sizeof(DODisplayState)
#define C3_GEODESCRIPTOR_SIZE           sizeof(GEODescriptor)

#define TARGET_COUNT        12
#define TARGET_POS_MATRIX   0
#define TARGET_POSITION     1
#define TARGET_NORMAL       2
#define TARGET_COLOR_0      3
#define TARGET_TCOORD       4   // There are 8 texture channels 

// Local functions
static void C3WriteGeoPalette( C3GeoPalette* geoPalette, char* path, char* name );
static u32  C3WriteDOLayout( 
                DOLayout*   dObj, 
                char*       fileNamePath,
                char*       strBank,
                u32         strBankSize,
                FILE*       outFile,
                char*       identifier );
static void C3WriteDOLayoutHeader( 
                FILE*       outFile,
                u32         offset,
                DOLayout*   dObj );
static void C3WriteTexPalStrOffset( FILE* outFile, u32 offset );
static void C3WritePalNameStr( FILE* outFile, u32* offset, DOLayout* dObj );
static u32  C3WritePosition( 
                FILE*       outFile,
                u32*        offset, 
                DOLayout*   doLayout );
static u32  C3WriteColor( 
                FILE*       outFile,
                u32*        offset, 
                DOLayout*   doLayout );
static u32  C3WriteTexture( 
                FILE*       outFile,
                u32*        offset, 
                DOLayout*   doLayout );
static u32  C3WriteDisplayData(   
                FILE*       outFile,
                u32*        offset, 
                DOLayout*   doLayout,
                C3StatsObj* statsObj );
static u32  C3WriteLighting( 
                FILE*       outFile,
                u32*        offset, 
                DOLayout*   doLayout );
static void C3WriteDisplayStateList( 
                FILE*             outFile, 
                u32               offset, 
                DODisplayHeader*  header,
                u32*              primBankSize );
static void C3WritePrimBank ( FILE* outFile, DODisplayHeader* header, C3StatsObj* statsObj );
static u32  C3NumVtxOpCode  ( void* ptr );
static u32  C3WriteBinIndex ( FILE* outFile, void** ptr, u8 target );
static void C3SetVCDValues  ( DODisplayState* ds );

// Local variables
static u8  gIndexData[ TARGET_COUNT ];
static u32 c3TexPalStrOffset[C3_MAX_TEXTURES];
            
extern u32   GplUserDataSize;
extern void *GplUserData;

/*---------------------------------------------------------------------------*
  Name:         C3WriteGeometry

  Description:  Write a geometry palette file from the geometry objects
                extracted in the module C3GeoExt

  Arguments:    path    - path to store the file
                name    - name of the palette file

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3WriteGeometry( char* path, char* name )
{
    C3GeoPalette*   geoPalette  = NULL;
    DSList*         geomList    = NULL;

    C3_ASSERT( path && name );

    // get the list of display object
    geomList = C3GetGeomObjList( );
    if(!geomList)
        return;

    // Convert to GeoPalette
    C3ReportStatus("Converting to geo palette...");
    C3ConvertToGeoPalette( geomList, &geoPalette, name );

    // If binary
    C3ReportStatus("Writing geo palette...");
    C3WriteGeoPalette( geoPalette, path, name);

    C3FreeGeoPalette( &geoPalette );
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteGeoPalette

  Description:  Writes the geo palette at the specified location

  Arguments:    geoPalette - geo palette to write
                path       - path at which to write the file
                name       - name of the file

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3WriteGeoPalette( C3GeoPalette* geoPalette, char* path, char* name )
{
    GEOPalette*     geoPal      = NULL;
    FILE*           outFile     = NULL;
    u32*            geomDescp   = NULL;
    u32             loc         = 0;
    u32             i           = 0;
    u32             j           = 0;
    u32             base        = 0;
    u32             length      = 0;
    u32             offset      = 0;
    u32             doSize      = 0;
    C3Stats*        stats;
    char*           fileNamePath  = NULL;

    geoPal = geoPalette->geoPal;
    
    // Set up statistics
    stats = C3GetStats();

    C3MakePath( path, name, C3_EXT_GEO_PALETTE, &fileNamePath );

    // Open a palette file
    outFile = fopen( fileNamePath, "wb" );
    C3_ASSERT( outFile );

    // Write GPL header

    // Write the version number
    // The version number is defined in C3GeoExt.h and this must be
    // consistent with the previewer
    C3WriteU32F( C3_GPL_VERSION, outFile );

    // write the user defined data size
    C3WriteU32F( GplUserDataSize, outFile );

    // write the offset to the user defined data (dummy for now)
    // keep a pointer to the location of geom descriptor in file
    loc = ftell( outFile );
    C3WriteU32F( 0, outFile );

    // Write the number of geometry descriptors
    C3WriteU32F( (u32)geoPal->numDescriptors, outFile ); 

    // Write offset to descriptor array
    C3WriteU32F( C3_GEOPALETTE_HEADER_SIZE, outFile );

    // Allocate memory for the geom descriptor
    geomDescp = (u32*)C3_CALLOC( geoPal->numDescriptors, C3_GEODESCRIPTOR_SIZE );
    C3_ASSERT( geomDescp );

    // Write dummy descriptor array
    fwrite( (void*)geomDescp, geoPal->numDescriptors, C3_GEODESCRIPTOR_SIZE, outFile );

    // Set the base offset for the first doLayout
    offset = C3_GEOPALETTE_HEADER_SIZE + geoPal->numDescriptors * C3_GEODESCRIPTOR_SIZE;

    stats->gplHeaderSize = offset;
    
    // Transform the string offsets in the geom descriptor
    for( i = 0, j = 0; i < geoPal->numDescriptors; i++ )
    {
        // Write display objects
        doSize = C3WriteDOLayout( geoPalette->layoutArray[i].layout, 
                                  fileNamePath, 
                                  geoPalette->layoutArray[i].strBank, 
                                  geoPalette->layoutArray[i].strBankSize,
                                  outFile,
                                  geoPalette->geoPal->descriptorArray[i].name );
        C3WriteU32( offset, (void*)(geomDescp + j)); 
        j++;
        offset += doSize; 
        length = (strlen(geoPalette->geoPal->descriptorArray[i].name) + 1);
        geomDescp[j] = base;
        j++;
        base += length;
    }
    
    // Write the string bank
    doSize = ftell( outFile );
    fwrite( (void*)geoPalette->strBank, 
            geoPalette->strBankSize, 
            sizeof(char), 
            outFile );

    stats->gplStringBankSize = geoPalette->strBankSize;

    // Write the user defined data starting at 32 byte boundary
    if( GplUserDataSize > 0 )
    {
        C3WritePadded32byte( 0, 0, outFile );
        base = ftell( outFile );
        C3WritePadded32bit( GplUserData, GplUserDataSize, outFile );
    }
    else
        base = 0;
    
    // Transform the string offsets in the geom descriptor
    for( i = 0, j = 1; i < geoPal->numDescriptors; i++, j += 2 )
    {
        offset = geomDescp[j] + doSize; 
        C3WriteU32( offset, (void*)(geomDescp + j)); 
    }

    // Go back in the file to the userData pointer
    fseek( outFile, loc, SEEK_SET );

    // Overwrite the offset to the user defined data
    C3WriteU32F( base, outFile );

    // Move to the top of the descriptor array
    fseek( outFile, loc + 12, SEEK_SET );

    // Overwrite geom descriptor array
    fwrite( (void*)geomDescp, geoPal->numDescriptors, C3_GEODESCRIPTOR_SIZE, outFile );

    // Close the file
    fclose( outFile );

    C3_FREE( fileNamePath );
    C3_FREE( geomDescp );
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteDOLayout

  Description:  Writes a display object layout in the specified path or 
                    the specified file.
                Will not create a file is the file pointer specified is non
                    NULL.

  Arguments:    dObj         - display object Layout to write
                fileNamePath - file name with path
                strBank      - string bank of the display object
                strBankSize  - size of the string bank
                outFile      - file to write to (NULL if fileNamePath is not )
                identifier   - the unique name of this object

  Returns:      size in bytes of the doLayout
/*---------------------------------------------------------------------------*/
static u32
C3WriteDOLayout( DOLayout* dObj, char* fileNamePath, char* strBank,
                 u32 strBankSize, FILE* outFile, char* identifier )
{
    u32         offset      = 0;
    DOLayout    header;
    C3Bool      free        = C3_FALSE;
    u32         baseOffset  = 0;
    u32         size;
    u32         channel;

    u32         lastOffset;
    C3StatsObj* statsObj;

//  FILE* f;

    C3_ASSERT( dObj );

    // open the file
    if( !outFile )
    {
        C3_ASSERT( fileNamePath );
        free = C3_TRUE;
        outFile = fopen( fileNamePath, "wb" );
        C3_ASSERT( outFile );
    }

    size = ftell( outFile );

    for( channel = 0; channel < C3_MAX_TEXTURES; channel++ )
        c3TexPalStrOffset[channel] = 0;

    // Get a location for the header
    baseOffset = ftell( outFile );

    // write the place holder for the Header
    fwrite( (void*)dObj, 1, C3_DISPLAYOBJECT_HEADER_SIZE, outFile );
    offset += C3_DISPLAYOBJECT_HEADER_SIZE;

    // get the stats object
    statsObj = C3GetStatsObjFromId( identifier );
    if( !statsObj )
        C3ReportError( "Could not find object %s.", identifier );
    statsObj->displayObjectHeaderSize = ftell( outFile ) - size;

    lastOffset = ftell(outFile);

    // Write position (header + data )
    header.positionData = (DOPositionHeader*)C3WritePosition( outFile, 
                                                              &offset, 
                                                              dObj );

    statsObj->positionDataSize = ftell( outFile ) - lastOffset;
    lastOffset = ftell(outFile);

    // Write color (header + data )
    if( dObj->colorData )
    {
        header.colorData = (DOColorHeader*)C3WriteColor( outFile, 
                                                         &offset, 
                                                         dObj );
    }
    else
        header.colorData = NULL;

    statsObj->colorDataSize = ftell( outFile ) - lastOffset;
    lastOffset = ftell(outFile);

    // Write texture (header + data )
    if( dObj->textureData )
    {
        header.textureData = (DOTextureDataHeader*)C3WriteTexture( outFile,
                                                                   &offset, 
                                                                   dObj );
        header.numTextureChannels = dObj->numTextureChannels;
    }
    else
    {
        header.textureData = NULL;
        header.numTextureChannels = 0;
    }

    statsObj->texCoordDataSize = ftell( outFile ) - lastOffset;
    lastOffset = ftell(outFile);

    // Write lighting (header + data )
    if( dObj->lightingData )
    {
        header.lightingData = (DOLightingHeader*)C3WriteLighting( outFile,
                                                                  &offset,
                                                                  dObj );
    }
    else
        header.lightingData = NULL;
    
    statsObj->lightingDataSize = ftell( outFile ) - lastOffset;
    lastOffset = ftell(outFile);

    // Write display (header + display state + primitive bank )
    header.displayData = (DODisplayHeader*)C3WriteDisplayData( outFile, 
                                                               &offset, 
                                                               dObj,
                                                               statsObj );

    statsObj->displayDataSize = ftell( outFile ) - lastOffset;
    lastOffset = ftell(outFile);

    // write the Header offsets
    C3WriteDOLayoutHeader( outFile, baseOffset, &header ); 

    // write the offset to the texture palette string
    C3WriteTexPalStrOffset( outFile, offset );
    
    // Write the texture palette name string
    C3WritePalNameStr( outFile, &offset, dObj );

    statsObj->texPalStrDataSize = ftell( outFile ) - lastOffset;

    size = ftell( outFile ) - size;

    // close the file
    if( free )
        fclose( outFile );

    statsObj->size = size;

    return size;
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteDOLayoutHeader

  Description:  Writes a DOLayout header at the offset location.
                Assumes that there is a file open.

  Arguments:    outFile - File to write to.
                offset  - Offset to at which to write (begining of dolayout ).
                dObj    - display object (as offsets) to take the info from.

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3WriteDOLayoutHeader( FILE* outFile, u32 offset, DOLayout* dObj )
{
    u32 loc = 0;
    u32 pad = 0;

    C3_ASSERT( outFile && dObj );
    
    loc = ftell( outFile );
    fseek( outFile, offset, SEEK_SET );
    C3WriteU32F( (u32)dObj->positionData, outFile );
    C3WriteU32F( (u32)dObj->colorData, outFile );
    C3WriteU32F( (u32)dObj->textureData, outFile );
    C3WriteU32F( (u32)dObj->lightingData, outFile );
    C3WriteU32F( (u32)dObj->displayData, outFile );
    fwrite( (void*)&dObj->numTextureChannels, sizeof(u8), 1, outFile );
    fwrite( (void*)&pad, sizeof(u8) + sizeof(u16), 1, outFile );

    fseek( outFile, loc, SEEK_SET );
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteTexPalStrOffset

  Description:  Writes the value of the current offset at the location 
                previously set in the global variable c3TexPalStrOffset.

  Arguments:    outFile - File to write to.
                offset  - value of the current Offset 
                channel - texture coordinate channel

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3WriteTexPalStrOffset( FILE* outFile, u32 offset )
{
    u32 channel;
    u32 loc;

    C3_ASSERT( outFile );

    loc = ftell( outFile );
    for( channel = 0; channel < C3_MAX_TEXTURES; channel++ )
    {
        if( c3TexPalStrOffset[channel] )
        {
            fseek( outFile, c3TexPalStrOffset[channel], SEEK_SET );
            C3WriteU32F( (u32)offset, outFile );
        }
    }

    fseek( outFile, loc, SEEK_SET );
}


/*---------------------------------------------------------------------------*
  Name:         C3WritePalNameSt

  Description:  Write the texture palette name if it exists.  Since all
                textures come from the same TPL file per export, let's just
                take the name from the first texture channel.

  Arguments:    outFile  - File to write to.
                offset   - Offset to update.
                doLayout - Display layout to get the info from.

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3WritePalNameStr( FILE* outFile, u32* offset, DOLayout* dObj )
{
    u32 size = 0;

    if( dObj->textureData )
    {
        if( dObj->textureData[0].texturePaletteName )
        {
            size = strlen(dObj->textureData[0].texturePaletteName)+1;
            fwrite( dObj->textureData[0].texturePaletteName, size, 1, outFile );
            *offset += size;
        }
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3WritePosition

  Description:  Write the position header and position data in the file
                    specified.

  Arguments:    outFile  - File to write to.
                offset   - Offset to update.
                doLayout - Display layout to get the info from.

  Returns:      offset of the position information
/*---------------------------------------------------------------------------*/
static u32
C3WritePosition( FILE* outFile, u32* offset, DOLayout* doLayout )
{
    DOPositionHeader* header      = NULL;   
    u32               dataOffset  = 0;
    u32               size        = 0;
    u32               compSize    = 0;
    void*             swap        = NULL;


    C3_ASSERT( outFile && offset && doLayout );

    header = doLayout->positionData;
    if( !header )
        return dataOffset;

    dataOffset = *offset;
    *offset += C3_POSITION_HEADER_SIZE;

    // Write offset to position array
    C3WriteU32F( *offset, outFile );    

    // Write number of position
    C3WriteU16F( header->numPositions, outFile );

    // Write quantize info
    fwrite( (void*)&header->quantizeInfo, sizeof(u8), 1, outFile );

    // Write the number of components
    fwrite( (void*)&header->compCount, sizeof(u8), 1, outFile );

    // Write data padded 32bits
    compSize = C3GetComponentSize( C3_TARGET_POSITION, 0 );
    size = (compSize * header->compCount) * header->numPositions;

    // Swap the bytes for endianness
    if( C3GetOptionOutputEndian() == C3_OUTPUT_BIG_ENDIAN &&
        compSize > 1 )
    {
        C3SwapBytes( &header->positionArray, &swap, size, compSize );
    }

    *offset += C3WritePadded32bit( header->positionArray, size, outFile);

    // If we created a temporary array
    if( swap )
    {
        void* tmp = header->positionArray;
      
        header->positionArray = swap;
        swap = NULL;
        C3_FREE( tmp );
    }

    return dataOffset;
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteColor

  Description:  Write the color header and color data in the file specified.

  Arguments:    outFile  - File to write to.
                offset   - Offset to update.
                doLayout - Display layout to get the info from.

  Returns:      offset of the color information
/*---------------------------------------------------------------------------*/
static u32
C3WriteColor( FILE* outFile, u32* offset, DOLayout* doLayout )
{
    DOColorHeader*    header      = NULL;   
    u32               dataOffset  = 0;
    u32               size        = 0;
    u32               compSize    = 0;
    void*             swap        = NULL;


    C3_ASSERT( outFile && offset && doLayout );

    header = doLayout->colorData;
    if( !header )
        return dataOffset;

    dataOffset = *offset;
    *offset += C3_COLOR_HEADER_SIZE;

    // Write offset to color array
    C3WriteU32F( *offset, outFile );

    // Write number of colors
    C3WriteU16F( header->numColors, outFile );

    // Write quantize info
    fwrite( (void*)&header->quantizeInfo, sizeof(u8), 1, outFile );

    // Write the number of components
    fwrite( (void*)&header->compCount, sizeof(u8), 1, outFile );
        
    // Write data padded 32bits
    // Gets the accurate component size even if we are quantizing to a
    // color w/ alpha format even though the first argument does not suggest so.
    compSize = C3GetComponentSizeFromQuantInfo( C3_TARGET_COLOR, header->quantizeInfo );
    size = compSize * header->numColors;

    // GX requires colors to be in PPC (big) endian even for
    // PC emulator.  The compiler flag is in C3Defines.h
#ifndef C3_COLOR_ALWAYS_TO_BIG_ENDIAN
    if( C3GetOptionOutputEndian() == C3_OUTPUT_BIG_ENDIAN )
#endif
        if( compSize > 1 )
            C3SwapBytes( &header->colorArray, &swap, size, compSize );

    *offset += C3WritePadded32bit( header->colorArray, size, outFile);

    // If we created a temporary array
    if( swap )
    {
        void* tmp = header->colorArray;
        
        header->colorArray = swap;
        swap = NULL;
        C3_FREE( tmp );
    }

    return dataOffset;
}

/*---------------------------------------------------------------------------*
  Name:         C3WriteTexture

  Description:  Write the texture header and texture data in the file
                specified for the given texture coordinate channel.

  Arguments:    outFile  - File to write to
                offset   - Offset to update
                doLayout - Display layout to get the info from

  Returns:      offset of the texture information
/*---------------------------------------------------------------------------*/
static u32
C3WriteTexture( FILE* outFile, u32* offset, DOLayout* doLayout )
{
    DOTextureDataHeader* header;   
    u32                  dataOffset  = 0;
    u32                  size        = 0;
    u32                  compSize    = 0;
    void*                swap        = NULL;
    u32                  headerStartInFile[C3_MAX_TEXTURES];
    u32                  channel;

    C3_ASSERT( outFile && offset && doLayout );

    if( !doLayout->textureData )
        return dataOffset;

    // Write a texture header.
    // We will overwrite the offset to the texture coordinate array when we
    // know them after we write the arrays
    dataOffset = *offset;

    for( channel = 0; channel < doLayout->numTextureChannels; channel++ )
    {
        header = &doLayout->textureData[channel];

        // Write offset to texture coordinate array
        headerStartInFile[channel] = ftell( outFile );
        C3WriteU32F( 0, outFile );    

        // Write number of texture coordinates
        C3WriteU16F( header->numTextureCoords, outFile );

        // Write quantize info
        fwrite( (void*)&header->quantizeInfo, sizeof(u8), 1, outFile );

        // Write the number of components
        fwrite( (void*)&header->compCount, sizeof(u8), 1, outFile );
    
        // Write the offset to the texture palette name
        c3TexPalStrOffset[channel] = ftell( outFile );
        C3WriteU32F( 0, outFile );
        C3WriteU32F( 0, outFile );  // Pointer to the palette

        *offset += C3_TEXTURE_HEADER_SIZE;
    }

    for( channel = 0; channel < doLayout->numTextureChannels; channel++ )
    {
        header = &doLayout->textureData[channel];

        if( header->numTextureCoords > 0 )
        {
            // Write data padded 32bits
            compSize = C3GetComponentSize( C3_TARGET_TEXCOORD, channel );
            size = (compSize * header->compCount) * header->numTextureCoords;

            // Swap the bytes for endianness
            if( C3GetOptionOutputEndian() == C3_OUTPUT_BIG_ENDIAN &&
                compSize > 1 )
            {
                C3SwapBytes( &header->textureCoordArray, &swap, size, compSize );
            }

            // Write the offset back
            fseek( outFile, headerStartInFile[channel], SEEK_SET );
            C3WriteU32F( *offset, outFile );
            fseek( outFile, 0, SEEK_END );
        
            *offset += C3WritePadded32bit( header->textureCoordArray, size, outFile);

            // If we created a temporary array
            if( swap )
            {
                void* tmp = header->textureCoordArray;
    
                header->textureCoordArray = swap;
                swap = NULL;
                C3_FREE( tmp );
            }
        }
    }

    return dataOffset;
}

/*---------------------------------------------------------------------------*
  Name:         C3WriteDisplayData

  Description:  Write the Display Data header and Display Data in the file
                    specified.

  Arguments:    outFile  - File to write to.
                offset   - Offset to update.
                doLayout - Display layout to get the info from.
                statsObj - stats for this object

  Returns:      offset of the Display Data information
/*---------------------------------------------------------------------------*/
static u32
C3WriteDisplayData( FILE* outFile, u32* offset, DOLayout* doLayout, C3StatsObj* statsObj )
{
    DODisplayHeader* header          = NULL;   
    u32              dataOffset      = 0;
    u32              primBankSize    = 0;
    u32              primOffset      = 0;
    u32              dispOffset      = 0;
    u32              temp            = 0;
    u32              primOffsetInFile = 0;
    u32              stateOffsetInFile;
    void*            tempBuffer;

    C3_ASSERT( outFile && offset && doLayout );

    header = doLayout->displayData;
    if( !header )
        return dataOffset;

    statsObj->numStateEntries = header->numStateEntries;

    dataOffset = *offset;
    *offset += C3_DISPLAY_DATA_HEADER_SIZE;
    dispOffset = *offset;
    *offset += (header->numStateEntries * C3_DISPLAY_STATE_SIZE);
    primOffset = *offset;

    // Write offset to primitive bank with 32 byte pad (will write pad right before C3WritePrimBank)
    primOffsetInFile = ftell( outFile ) + C3_DISPLAY_DATA_HEADER_SIZE + 
                       (header->numStateEntries * C3_DISPLAY_STATE_SIZE);
    temp = ( primOffsetInFile % 32 ) ? 32 - ( primOffsetInFile % 32 ) : 0;
    primOffset += temp;
    *offset += temp;
    C3WriteU32F( primOffset, outFile );

    // Write offset to display state list
    C3WriteU32F( dispOffset, outFile );

    // Write number of state list entries
    C3WriteU16F( header->numStateEntries, outFile );

    // Write a 2 byte pad
    temp = 0;
    C3WriteU16F( (u16)temp, outFile );

    // Write a space for the display state list in the outFile
    stateOffsetInFile = ftell( outFile );
    temp = header->numStateEntries * C3_DISPLAY_STATE_SIZE;
    tempBuffer = C3_CALLOC( 1, temp );
    C3_ASSERT( tempBuffer );
    memset( tempBuffer, 0, temp );
    fwrite( tempBuffer, temp, 1, outFile );
    C3_FREE( tempBuffer );

    // Make sure the start of the primbank is 32 BYTE aligned
    C3WritePadded32byte( NULL, 0, outFile );

    // Write primitive bank and get the number of pad bytes written
    C3WritePrimBank( outFile, header, statsObj );

    // Write back the state list with pad information
    fseek( outFile, stateOffsetInFile, SEEK_SET );
    C3WriteDisplayStateList( outFile, primOffset, header, &primBankSize );
    fseek( outFile, 0, SEEK_END );

    *offset += primBankSize;

    return dataOffset;
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteLighting

  Description:  Write the lighting header and data in the file specified.

  Arguments:    outFile  - File to write to.
                offset   - Offset to update.
                doLayout - Display layout to get the info from.

  Returns:      offset of the normal information
/*---------------------------------------------------------------------------*/
static u32
C3WriteLighting( FILE* outFile, u32* offset, DOLayout* doLayout )
{
    DOLightingHeader* header      = NULL;   
    u32               dataOffset  = 0;
    u32               size        = 0;
    u32               compSize    = 0;
    void*             swap        = NULL;
    u8                dummy       = 0;

    C3_ASSERT( outFile && offset && doLayout );

    header = doLayout->lightingData;
    if( !header )
        return dataOffset;

    dataOffset = *offset;
    *offset += C3_LIGHTING_HEADER_SIZE;

    if( C3GetOptionUseExternalNormalTable() || C3GetOptionUseDefaultNormalTable() )
    {
        // Write offset to normal array
        C3WriteU32F( 0, outFile );
        
        // Write number of normals
        C3WriteU16F( header->numNormals, outFile );

        // Write quantize info
        fwrite( (void*)&header->quantizeInfo, sizeof(u8), 1, outFile );

        // Write the number of components
        fwrite( (void*)&header->compCount, sizeof(u8), 1, outFile );

        // Write the ambient
        C3WriteFloatF( header->ambientPercentage, outFile );
    }
    else
    {
        // Write offset to normal array
        C3WriteU32F( *offset, outFile );    

        // Write number of normals
        C3WriteU16F( header->numNormals, outFile );

        // Write quantize info
        fwrite( (void*)&header->quantizeInfo, sizeof(u8), 1, outFile );

        // Write the number of components
        fwrite( (void*)&header->compCount, sizeof(u8), 1, outFile );
        
        // Write the ambient
        C3WriteFloatF( header->ambientPercentage, outFile );

        // Write data padded 32bits
        compSize = C3GetComponentSize( C3_TARGET_NORMAL, 0 );
        size = (compSize * header->compCount) * header->numNormals;

        // Swap the bytes for endianness
        if( C3GetOptionOutputEndian() == C3_OUTPUT_BIG_ENDIAN &&
            compSize > 1 )
        {
            C3SwapBytes( &header->normalArray, &swap, size, compSize );
        }

        *offset += C3WritePadded32bit( header->normalArray, size, outFile );

        // If we created a temporary array
        if( swap )
        {
            void* tmp = header->normalArray;
        
            header->normalArray = swap;
            swap = NULL;
            C3_FREE( tmp );
        }
    }

    return dataOffset;
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteDisplayStateList

  Description:  Write the display state list in the specified file.  Should be
                called after C3WritePrimBank since it will update the 
                primitiveGroup offset with pad information, although the state
                list precedes the display list in the GPL.

  Arguments:    outFile      - File to write to.
                offset       - offset of the primitive bank in the file.
                header       - DODisplayHeader to write.
                primBankSize - Size of the primitive bank (will be added to)

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3WriteDisplayStateList( FILE* outFile, u32 offset, DODisplayHeader* header, u32* primBankSize )
{
    DODisplayState* state       = NULL;
    u16             i           = 0;
    u32             primOffset  = 0;
    u32             padSize     = 0;

    C3_ASSERT( outFile && header && primBankSize );

    // for all state
    state = header->displayStateList;
    for( i = 0; i < header->numStateEntries; i++ )
    {
        // Write ID
        fwrite( (void*)&state[i].id, sizeof(u8), 1, outFile);
    
        // Write pads
        fwrite( (void*)&state[i].pad8, sizeof(u8), 1, outFile);
        C3WriteU16F( state[i].pad16, outFile );

        // Write settings
        C3WriteU32F( state[i].setting, outFile );

        // Write primitive group offset
        if( state[i].primitiveList )
        {
            primOffset = offset + (u32)state[i].primitiveList - (u32)header->primitiveBank;
        }
        else
        {
            primOffset = 0; 
        }

        C3WriteU32F( primOffset, outFile );

        // Write group size
        C3WriteU32F( state[i].listSize, outFile );
        *primBankSize += state[i].listSize;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3WritePrimBank

  Description:  Writes the primitive bank from the display header.
                Assumes that the size of the outFile is currently a multiple
                of 32 bytes.  Alignment is necessary since the display list
                will be sent to the graphics chip.  Since this function updates
                the primitiveGroup offset in the state list
                depending on the padding, the function C3WriteDisplayList should
                be called after this function, even though the state list 
                precedes the display list in the GPL file.

  Arguments:    outFile - file to write to
                header  - display header to take the information from
                statsObj - stats for this object's primitive bank

  Returns:      the size in bytes of padding written to the file
/*---------------------------------------------------------------------------*/
static void
C3WritePrimBank( FILE* outFile, DODisplayHeader* header, C3StatsObj* statsObj )
{
    u32             padSize     = 0;
    u32             size        = 0;
    u32             numVtx      = 0;
    u32             vtxCount    = 0;
    void*           ptr         = NULL;
    u32             i           = 0;
    DODisplayState  ds;
    u16             numVertices = 0;
    u16             j;
    u8              channel;
    u32             written;

    // Assumes that the outFile is aligned to a boundary of 32 bytes

    for( i = 0; i < header->numStateEntries; i++ )
    {
        ds = header->displayStateList[i];

        if( ds.id == DISPLAY_STATE_VCD )
        {
            C3SetVCDValues( &ds );
        }

        ptr = ds.primitiveList;

        if( header->displayStateList[i].primitiveList == 0 )
            continue;

        // Update the size of the display list with the pad alignment to 32 bytes
        header->displayStateList[i].primitiveList += padSize;

        size = numVtx = vtxCount = j = 0;
        while( size < ds.listSize )
        {
            // Write the vertex op code
            numVtx = C3NumVtxOpCode( ptr );
            fwrite( ptr, 1, sizeof( u8 ), outFile );
            ((u8*)ptr)++;

            // Write the number of vertices
            numVertices = *(u16*)ptr;
            C3WriteU16F( *(u16*)ptr, outFile );
            ((u16*)ptr)++;

            size += C3_PRIM_HEADER_SIZE;

            // Write the indices
            for( j = 0; j < numVertices; j++)
            {
                written = C3WriteBinIndex( outFile, &ptr, TARGET_POS_MATRIX );
                size += written;
                statsObj->posMatrixIndexDataSize += written;
                
                written = C3WriteBinIndex( outFile, &ptr, TARGET_POSITION );
                size += written;
                statsObj->positionIndexDataSize += written;

                written = C3WriteBinIndex( outFile, &ptr, TARGET_NORMAL );
                size += written;
                statsObj->normalIndexDataSize += written;

                written = C3WriteBinIndex( outFile, &ptr, TARGET_COLOR_0 );
                size += written;
                statsObj->colorIndexDataSize += written;

                for( channel = 0; channel < C3_MAX_TEXTURES; channel++ )
                {
                    written = C3WriteBinIndex( outFile, &ptr, (u8)(TARGET_TCOORD + channel) );
                    size += written;
                    statsObj->texCoordIndexDataSize[channel] += written;
                }
            }
        }

        // If we wrote a display list to the file,
        // make sure the end of the display list is 32 BYTE aligned
        if( j != 0 )
        {
            written = C3WritePadded32byte( NULL, 0, outFile );
            padSize += written;

            // Update the size of the display list to be a multiple of 32 bytes
            header->displayStateList[i].listSize += written;
        }
    }       
}


/*---------------------------------------------------------------------------*
  Name:         C3NumVtxOpCode

  Description:  Returns the minimum number of vertices per primitive for the 
                specified op code

  Arguments:    opCode - op code from which to get the count from

  Returns:      number of vertices
/*---------------------------------------------------------------------------*/
static u32
C3NumVtxOpCode( void* ptr )
{   
    u32 numVtx  =   0;
    u8  opCode  =   0;
    

    opCode = (*((u8*)ptr) & GX_OPCODE_MASK);
    switch( opCode )
    {
        case GX_DRAW_QUADS:
            numVtx = 4;
            break;


        case GX_DRAW_TRIANGLES:
            numVtx = 3;
            break;

        case GX_DRAW_TRIANGLE_STRIP:
        case GX_DRAW_TRIANGLE_FAN:
        case GX_DRAW_LINE_STRIP:  
            ((u8*)ptr)++;
            numVtx = *((u16*)ptr);
            break;

        case GX_DRAW_LINES:
            numVtx = 2;
            break;

        case GX_DRAW_POINTS:
            numVtx = 1;
            break;

        default:
            C3_ASSERT( C3_FALSE );
            break;
    }

    return numVtx;
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteBinIndex

  Description:  Writes the index of the specified target

  Arguments:    outFile - file to output
                ptr     - destination of the index the pointer will be 
                          incremented by the number of bytes written
                target  - target for which to write the index

  Returns:      number of bytes written
/*---------------------------------------------------------------------------*/
static u32
C3WriteBinIndex( FILE* outFile, void** ptr, u8 target )
{
    u32     written     = 0;

    switch( gIndexData[target] )
    {
        case GX_NONE:
            break;
        
        case GX_INDEX16:
            C3WriteU16F( *((u16*)*ptr), outFile );
            written = 2;
            break;

        case GX_INDEX8:
            fwrite( *ptr, 1, sizeof( u8 ), outFile );
            written = 1;
            break;

        case GX_DIRECT:
            C3_ASSERT( target == TARGET_POS_MATRIX );
            fwrite( *ptr, 1, sizeof( u8 ), outFile );
            written = 1;
            break;
    }
    
    if( written )
    {
        ((u8*)(*ptr)) = ((u8*)(*ptr)) + written;
    }
    return written;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetVCDValues

  Description:  Set the vertex component descriptor values

  Arguments:    ds - display state to set the vcd for

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3SetVCDValues( DODisplayState* ds )
{
    u32 i;

    // position matrix
    gIndexData[TARGET_POS_MATRIX] = (u8)((ds->setting >> C3_VCD_SHIFT_POS_MATRIX) & C3_VCD_MASK);
    
    // position
    gIndexData[TARGET_POSITION] = (u8)((ds->setting >> C3_VCD_SHIFT_POSITION) & C3_VCD_MASK);

    // normal
    gIndexData[TARGET_NORMAL] = (u8)((ds->setting >> C3_VCD_SHIFT_NORMAL) & C3_VCD_MASK);

    // color 0
    gIndexData[TARGET_COLOR_0] = (u8)((ds->setting >> C3_VCD_SHIFT_COLOR_0) & C3_VCD_MASK);

    // texture
    for( i = 0; i < C3_MAX_TEXTURES; i++ )
        gIndexData[TARGET_TCOORD + i] = (u8)((ds->setting >> (C3_VCD_SHIFT_TCOORD_BASE + (i * C3_VCD_SHIFT_TCOORD_INCREMENT))) & C3_VCD_MASK);
}
