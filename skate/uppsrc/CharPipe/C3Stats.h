/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3Stats.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/include/charPipeline/c3/C3Stats.h $
    
    6     2/18/00 5:50p John
    No longer suffixes stats files with "-Stats".
    
    5     2/04/00 6:06p John
    Untabified code.
    
    4     2/01/00 6:49p John
    Added stats for position matrix cache performance.
    
    3     1/20/00 1:13p John
    Added stitching (work in progress).
    
    2     1/04/00 1:12p John
    Added better support for second texture channel.
    
    7     11/18/99 1:12p John
    Calculates stripping/fan efficiency.
    
    6     10/20/99 11:04a John
    Added vertex number and vertex call stats.
    
    5     9/29/99 4:26p John
    Changed header to make them all uniform with logs.
    Added stats for normals.
    
    4     9/20/99 7:00p John
    Added code to analyze indirect vs. direct mode for
    positions (but commented out).
    
    3     9/17/99 9:33a John
    Added C3Options and cleaned up code.
    
    2     9/14/99 6:44p John
    Added stats per object.
    
  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _C3STATS_H
#define _C3STATS_H

#if 0
extern "C" {
#endif

#define C3_EXT_STATS        "txt"
//#define C3_STATS_FILESUFFIX "-Stats"
    
typedef struct
{
    DSLink  link;

    // Should correspond to the C3GeomObject's identifier
    char*   identifier;

    u32     numPoint;
    u32     numLine;
    u32     numLineStrip;
    u32     numTri;
    u32     numQuad;
    u32     numStrip;
    u32     numFan;

    u32     numPositions;
    u16     numUniquePositions;
    u32     numTCoords[C3_MAX_TEXTURES];
    u16     numUniqueTCoords[C3_MAX_TEXTURES];
    u32     numColors;
    u16     numUniqueColors;
    u32     numNormals;
    u16     numUniqueNormals;
    u32     numPrimitives;
    u32     numNullPrimitives;
    u32     numVertices;
    u32     numVerticesForStripping;
    u32     numVertexCallsForStripping;

    u16     numPosWelded;
    u16     numTexWelded[C3_MAX_TEXTURES];
    u16     numColWelded;

    u32     posMatrixCacheMiss;

    // Size in bytes of display object in gpl
    u32     displayObjectHeaderSize; 
    u32     positionDataSize;
    u32     colorDataSize;
    u32     texCoordDataSize;
    u32     displayDataSize;
    u32     lightingDataSize;
    u32     texPalStrDataSize;
    u32     size;

///*
    // Size of indices within the displayDataSize
    u32     numStateEntries;
    u32     posMatrixIndexDataSize;
    u32     positionIndexDataSize;
    u32     normalIndexDataSize;
    u32     texCoordIndexDataSize[C3_MAX_TEXTURES];
    u32     colorIndexDataSize;
//*/

} C3StatsObj;

typedef struct
{
    DSList  statsObjList;  // list of C3StatsObj for each object

    u32     gplHeaderSize;
    u32     gplStringBankSize;

} C3Stats;

void        C3MakeStatsFileName( char* path, char* name, char** fileNamePath );
void        C3WriteStatsFile( char* path, char* name );

#if 0
}
#endif

#endif  // _C3STATS_H
