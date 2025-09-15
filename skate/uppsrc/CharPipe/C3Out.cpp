/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3Out.c]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/c3/src/C3Out.c $
    
    8     11/13/00 1:48p John
    Added C3TransformData to transform geometry, hierarchy, and animation
    to compensate for new API: C3SetPivotOffset, where rotate and scale
    pivot points are not the same as the translation.
    
    7     7/18/00 7:29p John
    Removed commented code.
    Changed C3_FILE_ALL define.
    
    6     2/29/00 7:24p John
    Added C3OptimizeBeforeOutput.
    
    5     2/04/00 6:07p John
    Untabified code.
    
    4     1/31/00 4:16p John
    Added pruning of unnecessary bones and animations.
    Finished stitching with FIFO matrix cache and stripping.
    
    3     1/20/00 1:14p John
    Added stitching (work in progress).
    Added C3 string table for efficient management of strings.
    
    2     12/08/99 7:27p John
    Commented out unused code.
    
    10    10/14/99 4:33p John
    Minor code change.
    
    9     9/17/99 9:34a John
    Added C3Options and cleaned up code.

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#include "C3Private.h"


/*---------------------------------------------------------------------------*
  Name:         C3OptimizeBeforeOutput

  Description:  Optimizes data and must be performed before output.
                Welds and Removes duplicate vertex attribute data.
                Also removes null triangles.
                Computes optimal number of quantization shift bits.
                Converts vertex stitching information to bone indices.
                Converts triangles to quads/strips/fans.
                Prunes unnecessary actor bones.

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3OptimizeBeforeOutput( )
{
    C3GeomObject *geomObj;

    C3TransformData();

    // Compress (remove duplicates) and weld geometry data
    if( C3GetOptionCompress() )
        C3CompressData( );

    // Compute quantization shift bits now that we have final vertex
    // attribute arrays
    C3ProcessOptionsAfterCompression();

    // Now that we have extracted all hierarchy information, convert
    // all vertex stitching information from bone names to bone indices.
    // We have to do this before C3ConvertActor so that the bones we 
    // need for stitching are not pruned.
    if( C3GetOptionEnableStitching() )
    {
        geomObj = C3GetNextObject( NULL );
        while ( geomObj )
        {
            C3AssignBoneIndices( geomObj );

            geomObj = C3GetNextObject( geomObj );
        }
    }

    // Convert triangles to quads/strips/fans
    if( C3GetOptionEnableStripFan() )
    {
        geomObj = C3GetNextObject( NULL );
        while( geomObj )
        {
            C3ConvertToStripFan( geomObj );

            geomObj = C3GetNextObject( geomObj );
        }
    }

    // Convert the actor
    C3ConvertActor( C3GetActor() );
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteFile

  Description:  Writes the proper files according to the type of files
                specified

  Arguments:    path - path where to put the files
                name - common name for the files

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3WriteFile( char* path, char* name )
{
    u32 type = C3GetOptionFileExportFlag();

    // Write actor
    if( type & C3_FILE_HIERARCHY )
    {
        C3ReportStatus("Writing Hierarchy...");
        C3WriteHierarchy( path, name );
    }

    // Write geometry
    if( type & C3_FILE_GEOMETRY )
    {
        C3ReportStatus("Writing Geometry...");
        C3WriteGeometry( path, name );
    }

    // Write animation
    if( type & C3_FILE_ANIMATION )
    {
        C3ReportStatus("Writing Animation...");
        C3WriteAnimation( path, name );
    }

    // Write TCS script file and call TexConv to generate TPL
    if( type & C3_FILE_TEXTURE )
    {
        C3ReportStatus("Writing Texture...");
        C3WriteTextures( path, name );
    }

    // Write the stats file
    if( type & C3_FILE_STATS )
    {
        C3ReportStatus("Writing Stats...");
        C3WriteStatsFile( path, name );
    }
}

