/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3Stats.c]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/c3/src/C3Stats.c $
    
    12    11/13/00 1:48p John
    Fixed statistics dealing with stripping efficiency when extracting
    quads.
    
    11    8/14/00 6:15p John
    Added user-defined data to ANM, ACT, and GPL formats.
    
    10    6/27/00 6:07p John
    Substituted quantize.h macros.
    
    9     2/29/00 7:25p John
    Doesn't report that C3 is writing stats using C3ReportStatus.
    
    8     2/18/00 5:50p John
    No longer suffixes stats files with "-Stats".
    
    7     2/04/00 6:08p John
    Untabified code.
    
    6     2/01/00 6:49p John
    Added stats for position matrix cache performance.
    
    5     1/20/00 1:14p John
    Added stitching (work in progress).
    Added C3 string table for efficient management of strings.
    
    4     1/04/00 1:12p John
    Added better support for second texture channel.
    
    3     12/20/99 11:22a John
    Added second texture channel.
    
    2     12/13/99 3:39p John
    Moved quantization of hierarchy transform to C3WriteControl.
    
    13    11/18/99 1:17p John
    Calculates stripping/fan efficiency.
    
    12    10/20/99 11:04a John
    Added vertex number and vertex call stats.
    
    11    10/18/99 2:29p John
    
    10    10/14/99 6:33p John
    Added output for position, but commented out.
    
    9     10/14/99 4:33p John
    Writes the GPL version number in the stats file.
    
    8     10/13/99 2:54p John
    Changed position quantization back to a power of 2 scale with a given
    ruler.  
    
    7     10/01/99 11:17a John
    Can quantize color two ways (with and without alpha)
    Added position and tex. coordinate quantization per object.
    
    6     9/29/99 4:33p John
    Added stats for normals.
    
    5     9/21/99 6:40p John
    Changed function names.
    
    4     9/20/99 7:05p John
    Added code to analyze indirect vs. direct mode for
    positions, but code is commented out.
    
    3     9/17/99 9:34a John
    Added C3Options and cleaned up code.
    
    2     9/14/99 6:50p John
    Added stats per object.

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#include "C3Private.h"


// Global variables
static C3Stats  C3stats;    


/*---------------------------------------------------------------------------*
  Name:         C3InitStats

  Description:  Initialize the statistics

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3InitStats( void )
{
    C3StatsObj  statsObj;

    DSInitList(&C3stats.statsObjList, (void*)&statsObj, &(statsObj.link));
}


/*---------------------------------------------------------------------------*
  Name:         C3MakeStatsFileName

  Description:  Returns the name of the Stats file given the path, and the
                name of the export file int fileNamePath.

  Arguments:    path - path of export file
                name - name of export file
                fileNamePath - will point to stats filename.

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3MakeStatsFileName( char* path, char* name, char** fileNamePath )
{
//    C3MakePathSuffix( path, name, C3_EXT_STATS, C3_STATS_FILESUFFIX, fileNamePath );

    C3MakePath( path, name, C3_EXT_STATS, fileNamePath );
}


/*---------------------------------------------------------------------------*
  Name:         C3NewStatsObj

  Description:  Allocates a new C3StatsObj from the pool and returns it.
                Since the identifier is not copied, the string must be
                persistent.

  Arguments:    identifier - Identifier of the geometry Object (C3GeomObject)

  Returns:      new stats object
/*---------------------------------------------------------------------------*/
C3StatsObj*
C3NewStatsObj( char* identifier )
{
    C3StatsObj* statsObj;

    C3_ASSERT( identifier );

    statsObj = C3PoolStatsObj();

    // Zero out the entire object
    memset( statsObj, 0, sizeof(statsObj) );

    statsObj->identifier = C3AddStringInTable( identifier );

    return statsObj;
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteStatsFile

  Description:  Writes the stats summary file.  The stats filename is figured 
                out using name as a basis.

  Arguments:    path - path of the output file
                name - root name of the export file 

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3WriteStatsFile( char* path, char* name )
{
    char*       fileNamePath;
    FILE*       outFile;
    u32         counter = 0;
    C3GeomObject* geomObj;
    C3StatsObj* cursor;
    u32         gplTotalSize = 0;
    u32         numTotal;
    u32         temp;
    u8          type;
    u8          shift;
    u32         totalNumVertices = 0;
    u32         totalNumVerticesForStripping = 0;
    u32         totalNumVertexCallsForStripping = 0;
    u8          i;
    /*
    C3Position* pos;
    C3Position  unqPos;
    u32         qx, qy, qz;
    FILE*       testPos;
//  */
    /*
    FILE*       testTex;
    u32         prev;
    C3TexCoord* tex;
    C3TexCoord  unqTex;
//  */

    C3MakeStatsFileName( path, name, &fileNamePath );
    outFile = fopen( fileNamePath, "wb" );
    C3_ASSERT( outFile );

    fprintf( outFile, "GPL Stats\n\n");
    fprintf( outFile, "Header Size: %d\n", C3stats.gplHeaderSize );
    fprintf( outFile, "StringBank Size: %d\n", C3stats.gplStringBankSize );
    fprintf( outFile, "Version date: %d\n", C3_GPL_VERSION );
    
    // Print the breakdown of each object
    geomObj = C3GetNextObject( NULL );
    while ( geomObj )
    {
        cursor = geomObj->stats;

        fprintf( outFile, "\n-----------------------------------------------------------\n" );
        fprintf( outFile, "%d:", counter++ );
        if ( cursor->identifier )
            fprintf( outFile, " %s", cursor->identifier );
        fprintf( outFile, "\n\n" );

        // Print the number of prims of each type
        fprintf( outFile, "\t         Num Vertices: %d\n", cursor->numVertices );
        if( cursor->numPoint )
            fprintf( outFile, "\t           Num Points: %d\n", cursor->numPoint );
        if( cursor->numLine )
            fprintf( outFile, "\t            Num Lines: %d\n", cursor->numLine );
        if( cursor->numLineStrip )
            fprintf( outFile, "\t      Num Line Strips: %d\n", cursor->numLineStrip );
        if( cursor->numTri )
            fprintf( outFile, "\t        Num Triangles: %d\n", cursor->numTri );
        if( cursor->numQuad )
            fprintf( outFile, "\t            Num Quads: %d\n", cursor->numQuad );
        if( cursor->numStrip )
            fprintf( outFile, "\t  Num Triangle Strips: %d\n", cursor->numStrip );
        if( cursor->numFan )
            fprintf( outFile, "\t    Num Triangle Fans: %d\n", cursor->numFan );

        // Print vertex per triangle stats to measure stripping performance
        if( cursor->numVertexCallsForStripping > 0 )
        {
            fprintf( outFile, "\n" );
            fprintf( outFile, "\t    Num Tri/Quad/Strip/Fan Vertices: %d\n", cursor->numVerticesForStripping );
            fprintf( outFile, "\tNum Tri/Quad/Strip/Fan Vertex Calls: %d\n", cursor->numVertexCallsForStripping );
            fprintf( outFile, "\t              Vertices per Triangle: %f\n", (float)cursor->numVertexCallsForStripping / (cursor->numVerticesForStripping / 3.0f) );
        }

        totalNumVertices += cursor->numVertices;
        totalNumVerticesForStripping += cursor->numVerticesForStripping;
        totalNumVertexCallsForStripping += cursor->numVertexCallsForStripping;
        
        // Print display object stats
        fprintf( outFile, "\n" );
        fprintf( outFile, "\t        Num Positions: %d\n", cursor->numUniquePositions );
        fprintf( outFile, "\t           Num Colors: %d\n", cursor->numUniqueColors );
        for( temp = 0; temp < C3_MAX_TEXTURES; temp++ )
        {
            fprintf( outFile, "\t     Num Tex Coords %d: %d\n", temp, cursor->numUniqueTCoords[temp] );
        }
        fprintf( outFile, "\t          Num Normals: %d\n", cursor->numUniqueNormals );

        fprintf( outFile, "\n" );
        fprintf( outFile, "\t   Object Header Size: %d\n", cursor->displayObjectHeaderSize );
        fprintf( outFile, "\t   Position Data Size: %d\n", cursor->positionDataSize );
        fprintf( outFile, "\t      Color Data Size: %d\n", cursor->colorDataSize );
        fprintf( outFile, "\t  Tex Coord Data Size: %d\n", cursor->texCoordDataSize );
        fprintf( outFile, "\t   Lighting Data Size: %d\n", cursor->lightingDataSize );
        fprintf( outFile, "\t    Display Data Size: %d\n", cursor->displayDataSize );
        fprintf( outFile, "\tTex Pal Str Data Size: %d\n", cursor->texPalStrDataSize );
        fprintf( outFile, "\t----------------------------\n" );
        fprintf( outFile, "\t          Object Size: %d\n", cursor->size );

        // Print display list stats
        fprintf( outFile, "\n" );
        fprintf( outFile, "\t          Num State Entries: %d\n", cursor->numStateEntries );
        fprintf( outFile, "\t Pos Matrix Index Data Size: %d\n", cursor->posMatrixIndexDataSize );
        fprintf( outFile, "\t   Position Index Data Size: %d\n", cursor->positionIndexDataSize );
        fprintf( outFile, "\t     Normal Index Data Size: %d\n", cursor->normalIndexDataSize );
        for( temp = 0; temp < C3_MAX_TEXTURES; temp++ )
        {
            fprintf( outFile, "\tTex Coord %d Index Data Size: %d\n", temp, cursor->texCoordIndexDataSize[temp] );
        }
        fprintf( outFile, "\t      Color Index Data Size: %d\n", cursor->colorIndexDataSize );
/*
        // Make Direct mode vs. Indirect mode Calculations
        fprintf( outFile, "\tObject Position Indirect Size: %d\n", cursor->positionDataSize + cursor->positionIndexDataSize );
        // Find the size of each index
        if ( cursor->numUniquePositions <= 255 )
            temp = 1;
        else
            temp = 2;
        temp = C3GetComponentSize( C3_TARGET_POSITION, 0 ) * (cursor->positionIndexDataSize / temp) * 3;
        fprintf( outFile, "\tObject Position Direct   Size: %d\n", temp );
*/

        // Print welding stats
        fprintf( outFile, "\n" );
        if( cursor->numPosWelded )
            fprintf( outFile, "\t   Num Positions Welded: %d\n", cursor->numPosWelded );
        for( i = 0; i < C3_MAX_TEXTURES; i++ )
            if( cursor->numTexWelded[i] )
                fprintf( outFile, "\tNum Tex Coords %d Welded: %d\n", i, cursor->numTexWelded[i] );
        if( cursor->numColWelded )
            fprintf( outFile, "\t      Num Colors Welded: %d\n", cursor->numColWelded );
        if( cursor->numNullPrimitives )
            fprintf( outFile, "\t    Num Null Primitives: %d\n", cursor->numNullPrimitives );

        // Print Position Matrix Cache Performance Info
        if( cursor->posMatrixCacheMiss )
        {
            fprintf( outFile, "\n" );
            fprintf( outFile, "\tPos Matrix Cache Misses: %d\n", cursor->posMatrixCacheMiss );
            fprintf( outFile, "\t       Cache Miss Ratio: %.5f\n", (float)cursor->posMatrixCacheMiss / cursor->numVertexCallsForStripping );
        }

        // Print Quantization information
        fprintf( outFile, "\n" );
        for( i = 0; i < C3_MAX_TEXTURES; i++ )
        {
            if( cursor->numUniqueTCoords[i] > 0 )
            {
                type = C3_QUANT_TYPE( geomObj->options->texCoordQuantization[i] );
                shift = C3_QUANT_SHIFT( geomObj->options->texCoordQuantization[i] );
                fprintf( outFile, "\t  TexCoord %d Quantization: %s, %d\n", i,
                                  C3GetOptionQuantizationString( C3_TARGET_TEXCOORD, type ),
                                  shift );
            }
        }
        if( geomObj->useVertexAlpha )
            type = C3_QUANT_TYPE( C3GetOptionQuantization( C3_TARGET_COLORALPHA, 0 ) );
        else
            type = C3_QUANT_TYPE( C3GetOptionQuantization( C3_TARGET_COLOR, 0 ) );
        if( cursor->colorDataSize > 0 )
            fprintf( outFile, "\t       Color Quantization: %s\n", C3GetOptionQuantizationString( C3_TARGET_COLOR, type ) );

        // TESTING: Print out the positions
        /*
        pos = (C3Position*)geomObj->positionList.Head;
        testPos = fopen( "D:\\temp\\positions.txt", "at" );
        fprintf( testPos, "After quantization:\n" );
        while ( pos )
        {
            C3QuantizeFloat( &qx, C3GetOptionQuantization( C3_TARGET_POSITION, 0 ), pos->x );
            C3QuantizeFloat( &qy, C3GetOptionQuantization( C3_TARGET_POSITION, 0 ), pos->y );
            C3QuantizeFloat( &qz, C3GetOptionQuantization( C3_TARGET_POSITION, 0 ), pos->z );
            unqPos.x = C3FloatFromQuant( &qx, C3GetOptionQuantization( C3_TARGET_POSITION, 0 ) );
            unqPos.y = C3FloatFromQuant( &qy, C3GetOptionQuantization( C3_TARGET_POSITION, 0 ) );
            unqPos.z = C3FloatFromQuant( &qz, C3GetOptionQuantization( C3_TARGET_POSITION, 0 ) );
            fprintf( testPos, "\ti: %d x: %.8f y: %.8f z: %.8f\n", pos->index, unqPos.x, unqPos.y, unqPos.z );

            pos = (C3Position*)pos->link.Next;
        }
        fclose( testPos );
//      */

        // TESTING: Print out the texture coordinates
        /*
        testTex = fopen( "D:\\temp\\texCoords.txt", "at" );
        if( cursor->identifier )
            fprintf( testTex, "%s ", cursor->identifier );
        fprintf( testTex, "after quantization:\n" );
        tex = (C3TexCoord*)geomObj->texCoordList.Head;
        prev = -1;
        while( tex )
        {
            if( tex->index != prev )
            {
                C3QuantizeFloat( &qx, geomObj->options->texCoordQuantization, tex->s );
                C3QuantizeFloat( &qy, geomObj->options->texCoordQuantization, tex->t );
                unqTex.s = C3FloatFromQuant( &qx, geomObj->options->texCoordQuantization );
                unqTex.t = C3FloatFromQuant( &qy, geomObj->options->texCoordQuantization );
                fprintf( testTex, "\ti: %d s: %.8f t: %.8f\n", tex->index, unqTex.s, unqTex.t );
            }

            prev = tex->index;
            tex = (C3TexCoord*)tex->link.Next;
        }
        fclose( testTex );
//      */

        gplTotalSize += cursor->size;

        geomObj = C3GetNextObject( geomObj );
    }

    fprintf( outFile, "\n===========================================================\n\n" );
    type = C3_QUANT_TYPE( C3GetOptionQuantization( C3_TARGET_POSITION, 0 ) );
    shift = C3_QUANT_SHIFT( C3GetOptionQuantization( C3_TARGET_POSITION, 0 ) );
    fprintf( outFile, "\tPosition Quantization: %s, %d\n", C3GetOptionQuantizationString( C3_TARGET_POSITION, type ), shift );
    type = C3_QUANT_TYPE( C3GetOptionQuantization( C3_TARGET_NORMAL, 0 ) );
    shift = C3_QUANT_SHIFT( C3GetOptionQuantization( C3_TARGET_NORMAL, 0 ) );
    fprintf( outFile, "\t  Normal Quantization: %s, %d\n", 
                      C3GetOptionQuantizationString( C3_TARGET_NORMAL, type ), shift );

    fprintf( outFile, "\n" );
    fprintf( outFile, "              Total Vertices: %d\n", totalNumVertices );
    numTotal = C3StatsGetNumPrim( C3_PRIM_POINT );
    if ( numTotal )
        fprintf( outFile, "            Total Num Points: %d\n", numTotal );
    numTotal = C3StatsGetNumPrim( C3_PRIM_LINE );
    if ( numTotal )
        fprintf( outFile, "             Total Num Lines: %d\n", numTotal );
    numTotal = C3StatsGetNumPrim( C3_PRIM_LINE_STRIP );
    if ( numTotal )
        fprintf( outFile, "       Total Num Line Strips: %d\n", numTotal );
    numTotal = C3StatsGetNumPrim( C3_PRIM_TRI );
    if ( numTotal )
        fprintf( outFile, "         Total Num Triangles: %d\n", numTotal );
    numTotal = C3StatsGetNumPrim( C3_PRIM_QUAD );
    if ( numTotal )
        fprintf( outFile, "             Total Num Quads: %d\n", numTotal );
    numTotal = C3StatsGetNumPrim( C3_PRIM_STRIP );
    if ( numTotal )
        fprintf( outFile, "   Total Num Triangle Strips: %d\n", numTotal );
    numTotal = C3StatsGetNumPrim( C3_PRIM_FAN );
    if ( numTotal )
        fprintf( outFile, "     Total Num Triangle Fans: %d\n", numTotal );
    if ( totalNumVertexCallsForStripping > 0 )
    {
        fprintf( outFile, "\t    Total Num Tri/Quad/Strip/Fan Vertices: %d\n", totalNumVerticesForStripping );
        fprintf( outFile, "\tTotal Num Tri/Quad/Strip/Fan Vertex Calls: %d\n", totalNumVertexCallsForStripping );
        fprintf( outFile, "\t              Total Vertices per Triangle: %f\n", (float)totalNumVertexCallsForStripping / (totalNumVerticesForStripping / 3 ) );
    }

    fprintf( outFile, "\n" );
    numTotal = C3StatsGetTotal( C3_TARGET_POSITION, 0 );
    fprintf( outFile, "             Total Positions: %d\n", numTotal );
    for( i = 0; i < C3_MAX_TEXTURES; i++ )
    {
        numTotal = C3StatsGetTotal( C3_TARGET_TEXCOORD, i );
        fprintf( outFile, "          Total Tex Coords %d: %d\n", i, numTotal );
    }
    numTotal = C3StatsGetTotal( C3_TARGET_COLOR, 0 );
    fprintf( outFile, "            Total Num Colors: %d\n", numTotal );
    numTotal = C3StatsGetTotal( C3_TARGET_NORMAL, 0 );
    fprintf( outFile, "           Total Num Normals: %d\n", numTotal );

    fprintf( outFile, "\n" );
    numTotal = C3StatsGetTotalWelded( C3_TARGET_POSITION, 0 );
    if ( numTotal )
        fprintf( outFile, "   Total Num Positions Welded: %d\n", numTotal );
    for( i = 0; i < C3_MAX_TEXTURES; i++ )
    {
        numTotal = C3StatsGetTotalWelded( C3_TARGET_TEXCOORD, i );
        if ( numTotal )
            fprintf( outFile, "Total Num Tex Coords %d Welded: %d\n", i, numTotal );
    }
    numTotal = C3StatsGetTotalWelded( C3_TARGET_COLOR, 0 );
    if ( numTotal )
        fprintf( outFile, "      Total Num Colors Welded: %d\n", numTotal );
    numTotal = C3StatsGetNumNullPrim();
    if ( numTotal )
        fprintf( outFile, "    Total Num Null Primitives: %d\n", numTotal );
    
    gplTotalSize += C3stats.gplHeaderSize + C3stats.gplStringBankSize;
    fprintf( outFile, "\nTotal GPL Size: %d (Bytes)\n", gplTotalSize );

    fclose( outFile );
    C3_FREE( fileNamePath );
}


/*---------------------------------------------------------------------------*
  Name:         C3AddStatsObj

  Description:  Adds the object to the global stats list.

  Arguments:    obj - stats object

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void         
C3AddStatsObj( C3StatsObj* obj )
{
    C3_ASSERT( obj );

    DSInsertListObject( &C3stats.statsObjList, NULL, (void*) obj );
}


/*---------------------------------------------------------------------------*
  Name:         C3GetStats

  Description:  Returns the global stats.

  Arguments:    NONE

  Returns:      global stats
/*---------------------------------------------------------------------------*/
C3Stats* 
C3GetStats( void )
{
    return &C3stats;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetStatsObjList

  Description:  Returns a list of all stats objects.

  Arguments:    NONE

  Returns:      list of all stats objects.
/*---------------------------------------------------------------------------*/
DSList*
C3GetStatsObjList( void )
{
    return &C3stats.statsObjList;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetStatsObjFromId

  Description:  Returns the appropriate stats object using identifier to locate
                it.  Returns NULL if no matching stats object with the exact same
                identifier is found.

  Arguments:    identifier - identifier of the geometry object

  Returns:      stats object if found.  NULL otherwise.
/*---------------------------------------------------------------------------*/
C3StatsObj* 
C3GetStatsObjFromId( char* identifier )
{
    // Look through the statsObjList and find the one that corresponds with identifier
    C3StatsObj* obj    = NULL;
    C3StatsObj* cursor = NULL;

    cursor = (C3StatsObj*)C3stats.statsObjList.Head;
    while ( cursor )
    {
        // for each object compare to the identifier
        if( !strcmp( cursor->identifier, identifier ) )
        {
            return cursor;
        }
        cursor = C3GetNextStatsObj( cursor );
    }

    return NULL;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetNextStatsObj

  Description:  Gets the next stats object in the global stats list.  If argument
                obj is NULL, the head of the list is returned.

  Arguments:    obj - object which precedes the object we want.  

  Returns:      the next stats object.
/*---------------------------------------------------------------------------*/
C3StatsObj*
C3GetNextStatsObj( C3StatsObj* obj )
{
    C3StatsObj* next = NULL;

    if( obj == NULL )
    {
        next = (C3StatsObj*)C3stats.statsObjList.Head;
    }
    else
    {
        next = (C3StatsObj*)obj->link.Next;
    }

    return next;
}


/*---------------------------------------------------------------------------*
  Name:         C3StatsGetNumPrim

  Description:  Returns the number of primitives of the type specified are in 
                all the geomobject at the current time.
    
  Arguments:    primType - primitive type to count

  Returns:      number of primitive of the specified type
/*---------------------------------------------------------------------------*/
u32
C3StatsGetNumPrim( u8 primType )
{
    u32             numPrim = 0;
    C3GeomObject*   geomObj;

    // for all the object in the list
    geomObj = C3GetNextObject( NULL );
    while( geomObj )
    {
        switch( primType )
        {
            case C3_PRIM_POINT:
                numPrim += geomObj->stats->numPoint;
                break;

            case C3_PRIM_LINE:
                numPrim += geomObj->stats->numLine;
                break;

            case C3_PRIM_LINE_STRIP:
                numPrim += geomObj->stats->numLineStrip;
                break;

            case C3_PRIM_TRI:
                numPrim += geomObj->stats->numTri;
                break;

            case C3_PRIM_QUAD:
                numPrim += geomObj->stats->numQuad;
                break;

            case C3_PRIM_STRIP:
                numPrim += geomObj->stats->numStrip;
                break;

            case C3_PRIM_FAN:
                numPrim += geomObj->stats->numFan;
                break;
        }

        geomObj = C3GetNextObject( geomObj );
    }

    return numPrim;
}


/*---------------------------------------------------------------------------*
  Name:         C3StatsGetNumNullPrim

  Description:  Returns the number of primitives removed since they were
                zero area triangles
    
  Arguments:    NONE

  Returns:      number of null primitives
/*---------------------------------------------------------------------------*/
u32  
C3StatsGetNumNullPrim( void )
{
    u32             numPrim = 0;
    C3GeomObject*   geomObj;

    // for all the object in the list
    geomObj = C3GetNextObject( NULL );
    while( geomObj )
    {
        numPrim += geomObj->stats->numNullPrimitives;
        geomObj = C3GetNextObject( geomObj );
    }

    return numPrim;
}


/*---------------------------------------------------------------------------*
  Name:         C3StatsGetTotal

  Description:  Returns the total number of positions, colors, or texture
                coordinates in the given channel
    
  Arguments:    target  - primitive type to count
                channel - texture coordinate channel if target is C3_TARGET_TEXCOORD

  Returns:      number of primitive of the specified type
/*---------------------------------------------------------------------------*/
u32
C3StatsGetTotal( u16 target, u8 channel )
{
    u32             numTotal = 0;
    C3GeomObject*   geomObj;
    
    // for all the object in the list
    geomObj = C3GetNextObject( NULL );
    while( geomObj )
    {
        switch( target )
        {
            case C3_TARGET_POSITION:
                numTotal += geomObj->stats->numUniquePositions;
                break;

            case C3_TARGET_TEXCOORD:
                numTotal += geomObj->stats->numUniqueTCoords[channel];
                break;

            case C3_TARGET_COLOR:
                numTotal += geomObj->stats->numUniqueColors;
                break;

            case C3_TARGET_NORMAL:
                numTotal += geomObj->stats->numUniqueNormals;
                break;
        }

        geomObj = C3GetNextObject( geomObj );
    }

    return numTotal;
}


/*---------------------------------------------------------------------------*
  Name:         C3StatsGetTotalWelded

  Description:  Returns the number of primitives of the type specified are in 
                all the geomobject at the current time.
    
  Arguments:    target  - primitive type to count
                channel - texture coordinate channel if target is C3_TARGET_TEXCOORD

  Returns:      number of primitive of the specified type
/*---------------------------------------------------------------------------*/
u32
C3StatsGetTotalWelded( u16 target, u8 channel )
{
    u32             numTotal = 0;
    C3GeomObject*   geomObj;
    
    // for all the object in the list
    geomObj = C3GetNextObject( NULL );
    while( geomObj )
    {
        switch( target )
        {
            case C3_TARGET_POSITION:
                numTotal += geomObj->stats->numPosWelded;
                break;

            case C3_TARGET_TEXCOORD:
                numTotal += geomObj->stats->numTexWelded[channel];
                break;

            case C3_TARGET_COLOR:
                numTotal += geomObj->stats->numColWelded;
                break;
        }

        geomObj = C3GetNextObject( geomObj );
    }

    return numTotal;
}

