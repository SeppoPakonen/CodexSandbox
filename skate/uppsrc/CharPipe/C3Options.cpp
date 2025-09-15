/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3Options.c]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/c3/src/C3Options.c $
    
    16    11/13/00 1:48p John
    Added C3 options initialization which were omitted earlier.  All
    options are initialized now.  (Keyframe quantization set to float for
    runtime speed efficiency).
    Min and max positions are calculated later since positions can be
    transformed due to C3SetPivotOffset API.  Also C3SetOptionQuantization
    for positions moved to C3ProcessOptionsAfterCompression.
    Added code to not forget Euler XYZ rotation information when
    calculating quantization shift bits.
    Removed texture filter option since it does not belong within C3
    library (should be controlled from C3 library caller).
    
    15    7/18/00 7:28p John
    Modified to use new CTRL library.
    
    14    6/27/00 6:07p John
    Substituted quantize.h macros.
    
    13    6/06/00 3:21p John
    Changed default filter to be Trilerp/Mipmap instead of Bilerp.
    
    12    2/29/00 7:24p John
    Added code to initialize all options to some default value.
    Renamed C3ProcessOptionsAfterOptimization to
    C3ProcessOptionsAfterCompression.
    
    11    2/18/00 4:55p John
    Now assigns vertex stitching information after hierarchy extraction
    process
    but before actor pruning in C3ProcessOptionsAfterCompression.
    
    10    2/04/00 6:07p John
    Untabified code.
    
    9     2/04/00 3:57p John
    If Geometry is not output, then all hierarchy bones can be pruned.
    Fixed.  But now if we only output Hierarchy, then some bones that
    should be pruned will not be because Geometry was not also output.  To
    get accurate Hierarchy,
    should also export Geometry.
    
    8     1/31/00 4:16p John
    Added pruning of unnecessary bones and animations.
    Finished stitching with FIFO matrix cache and stripping.
    
    7     1/20/00 1:14p John
    Added stitching (work in progress).
    Added C3 string table for efficient management of strings.
    
    6     1/04/00 6:58p John
    Added little/big endian output  option
    
    5     1/04/00 1:12p John
    Added better support for second texture channel.
    
    4     12/20/99 11:22a John
    Added second texture channel.
    
    3     12/08/99 7:27p John
    Added animation quantization.
    
    2     12/06/99 3:26p John
    Removed some commented code.
    
    9     11/11/99 1:22p John
    Added option to view strip/fan effectiveness.
    
    8     10/19/99 11:20a John
    Added more accurate position range calculation.
    
    7     10/13/99 2:54p John
    Changed position quantization back to a power of 2 scale with a given
    ruler.  
    Added lighting with ambient percentage.
    Added default normal table.
    
    6     10/01/99 11:16a John
    Can quantize color two ways (with and without alpha)
    Added position and tex. coordinate quantization per object.
    
    5     9/29/99 4:32p John
    Changed header to make them all uniform with logs.
    Added position and normal quantization
    Added external normal table.
    
    4     9/21/99 6:42p John
    Changed functionality and name of C3ComputeQuantizationShift.
    
    3     9/20/99 7:04p John
    Renamed C3TextCoord to C3TexCoord for consistency.
    
    2     9/17/99 9:34a John
    Added C3Options and cleaned up code.
    
  $NoKeywords: $
 
 *---------------------------------------------------------------------------*/

#include "C3Private.h"

// Global variable
C3Options C3options;


/*---------------------------------------------------------------------------*
  Name:         C3InitOptions

  Description:  Sets default options
    
  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void    
C3InitOptions( )
{
    C3OptionObj optionObj;
    u8          i;  
    u32         flag;
    u16         compressFlags;

    // Initialize the global C3Options structure
    memset( &C3options, 0, sizeof( C3options ) );

    // Initialize the object options list
    DSInitList( &C3options.optionObjList, (void*)&optionObj, &optionObj.link );

    // Vertex order should be extracted in counter-clockwise fashion as front-facing
    C3SetOptionSrcVertexOrder( C3_CCW );

    // Quantize all vertex attributes to save format space and memory space at runtime
    C3SetOptionQuantization( C3_TARGET_POSITION, 0, C3_MAKE_QUANT(GX_S16, 0) );
    for( i = 0; i < C3_MAX_TEXTURES; i++ )
        C3SetOptionQuantization( C3_TARGET_TEXCOORD, i, C3_MAKE_QUANT(GX_S16, 0) );
    C3SetOptionQuantization( C3_TARGET_COLOR, 0, C3_MAKE_QUANT(GX_RGB565, 0) );
    C3SetOptionQuantization( C3_TARGET_COLORALPHA, 0, C3_MAKE_QUANT(GX_RGBA4, 0) );
    C3SetOptionQuantization( C3_TARGET_NORMAL, 0, C3_MAKE_QUANT(GX_S16, 14) );
    C3SetOptionQuantization( C3_TARGET_KEYFRAME, 0, C3_MAKE_QUANT(GX_F32, 0) );

    // Enable triangle stripping/fanning
    C3SetOptionEnableStripFan( C3_TRUE );

    // Set default output to stdout
    C3SetOptionReportStatusFunc( printf );
    C3SetOptionReportErrorFunc ( printf );

    // Output to big endian format (Hardware and MAC Emulator format), but set to
    // little endian if outputting to x86 format (Win32 Emulator)
    C3SetOptionOutputEndian( C3_OUTPUT_LITTLE_ENDIAN );

    // Output all files
    flag = C3_FILE_GEOMETRY | C3_FILE_HIERARCHY | C3_FILE_ANIMATION |
           C3_FILE_TEXTURE | C3_FILE_STATS;
    C3SetOptionFileExportFlag( flag );

    // Do not enable stitching (skinning where a vertex is rigidly attached to one bone)
    C3SetOptionEnableStitching( C3_TRUE );

    // Enable lighting so normals are exported per vertex
    C3SetOptionEnableLighting( C3_TRUE );

    // Set ambient lighting to be 25 percent of vertex color
    C3SetOptionAmbientPercentage( 25.0f );

    // Compress all vertex attributes
    compressFlags = C3_TARGET_POSITION | C3_TARGET_COLOR |
                    C3_TARGET_TEXCOORD | C3_TARGET_NORMAL;
    C3SetOptionCompress( compressFlags );
}


/*---------------------------------------------------------------------------*
  Name:         C3ProcessOptionsAfterCompression

  Description:  This function must be called after object data has been optimized
                since it computes the optimal number of quantization shift bits
                based on object data.
    
  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3ProcessOptionsAfterCompression( )
{
    C3Actor      *actor;
    C3GeomObject *obj;
    C3Track      *trackCursor;
    C3KeyFrame   *keyFrameCursor;
    u32           maxIntegerValue;
    f32           largestCoordinate;
    u8            quantInfo;
    u8            newQuantInfo;
    u8            type;
    u8            shift;
    u32           i;
    
    // ========== Compute for position quantization
    quantInfo = C3GetOptionQuantization( C3_TARGET_POSITION, 0 );
    if( C3_QUANT_TYPE( quantInfo ) != GX_F32 )
    {
        largestCoordinate = -FLT_MAX;
        obj = C3GetNextObject( NULL );
        while ( obj )
        {
            // Now find absolute largest value for positions across all objects
            // (one pos. quantization for global fixed grid of positions
            if( fabs(obj->maxPosition.x) > largestCoordinate )
                largestCoordinate = (f32)fabs(obj->maxPosition.x);
            if( fabs(obj->minPosition.x) > largestCoordinate )
                largestCoordinate = (f32)fabs(obj->minPosition.x);

            if( fabs(obj->maxPosition.y) > largestCoordinate )
                largestCoordinate = (f32)fabs(obj->maxPosition.y);
            if( fabs(obj->minPosition.y) > largestCoordinate )
                largestCoordinate = (f32)fabs(obj->minPosition.y);

            if( fabs(obj->maxPosition.z) > largestCoordinate )
                largestCoordinate = (f32)fabs(obj->maxPosition.z);
            if( fabs(obj->minPosition.z) > largestCoordinate )
                largestCoordinate = (f32)fabs(obj->minPosition.z);

            obj = C3GetNextObject( obj );
        }

        // Make sure that the largest position coordinate can be properly represented
        // with the current position range.  If no range is set, then set it automatically.
        if( C3GetOptionPositionRange() <= 0 )
        {
            // Recompute position quantization shift bits, by resetting range
            C3SetOptionPositionRange( largestCoordinate );
            quantInfo = C3ComputeQuantizationShift( quantInfo, 0, largestCoordinate );
            C3SetOptionQuantization( C3_TARGET_POSITION, 0, quantInfo );
        }
        else 
        {
            // Position quantization was set by user using C3SetOptionPositionRange/C3SetOptionQuantization w/ position.
            // So now, just check the position range and warn if it isn't sufficient
            quantInfo = C3ComputeQuantizationShift( quantInfo, 0, C3GetOptionPositionRange() );
            C3SetOptionQuantization( C3_TARGET_POSITION, 0, quantInfo );
            type = C3_QUANT_TYPE( quantInfo );
            shift = C3_QUANT_SHIFT( quantInfo );
            if( type == GX_S8 || type == GX_S16 )
            {
                if( type == GX_S8 )
                    maxIntegerValue = (1 << (7 - shift));
                if( type == GX_S16 )
                    maxIntegerValue = (1 << (15 - shift));
            } 
            else if( type == GX_U8 || type == GX_U16 )
            {
                if( type == GX_U8 )
                    maxIntegerValue = (1 << (8 - shift));
                if( type == GX_U16 )
                    maxIntegerValue = (1 << (16 - shift));
            }

            if( largestCoordinate > 0 && floor(largestCoordinate) > maxIntegerValue )
            {
                C3ReportError( "Position Range does not encompass all positions. Largest Position Coordinate: %f ", largestCoordinate );
            }
        }
    }

    // NOTE: Normal shift bit quantization is done in C3SetOptionQuantization
 
    // ========== Compute for texture coordinate quantization
    // Get the global type of quantization without the shift bits
    // Then set the shift bits per object
    if( C3GetOptionFileExportFlag() & C3_FILE_TEXTURE )
    {
        obj = C3GetNextObject( NULL );
        while ( obj )
        {
            // We can only compute min and max if there are any texture coordinates
            for( i = 0; i < C3_MAX_TEXTURES; i++ )
            {
                if( obj->stats->numUniqueTCoords[i] <= 0 )
                    continue;

                quantInfo = (u8)( C3GetOptionQuantization( C3_TARGET_TEXCOORD, i ) & 0xF0 );

                if( C3_QUANT_TYPE( quantInfo ) != GX_F32 )
                {
                    newQuantInfo = C3ComputeQuantizationShift( quantInfo, 
                                                               obj->minTexCoord[i],
                                                               obj->maxTexCoord[i] );
                    obj->options->texCoordQuantization[i] = newQuantInfo;
                }
                else
                {
                    obj->options->texCoordQuantization[i] = quantInfo;
                }
            }

            obj = C3GetNextObject( obj );
        }
    }

    // ========== Compute for animation quantization
    // Calculate the shift bits per track for animation
    actor = C3GetActor();     
    if( (C3GetOptionFileExportFlag() & C3_FILE_ANIMATION) && actor && actor->animBank )
    {
        quantInfo = (u8)( C3GetOptionQuantization( C3_TARGET_KEYFRAME, 0 ) & 0xF0 );

        // For each track, find the min and max range of translation and scale values
        // as well as their in and out controls
        trackCursor = (C3Track*)((C3AnimBank*)actor->animBank)->trackList.Head;
        while( trackCursor )
        {
            if( C3_QUANT_TYPE( quantInfo ) != GX_F32 )
            {           
                largestCoordinate = -FLT_MAX;

                keyFrameCursor = (C3KeyFrame*)trackCursor->keyList.Head;
                while( keyFrameCursor )
                {
                    // If the controltype is a matrix
                    if( keyFrameCursor->control.type & CTRL_MTX )
                    {
                        for( i = 0; i < 12; i++ )
                            if( fabs(((float*)keyFrameCursor->control.controlParams.mtx.m)[i]) > largestCoordinate )
                                largestCoordinate = (f32)fabs(((float*)keyFrameCursor->control.controlParams.mtx.m)[i]);

                        keyFrameCursor = (C3KeyFrame*)keyFrameCursor->link.Next;
                        continue;
                    }

                    // If there is euler rotation, check for largest coordinate
                    if( keyFrameCursor->control.type & CTRL_ROT_EULER )
                    {
                        if( fabs(keyFrameCursor->control.controlParams.srt.r.x) > largestCoordinate )
                            largestCoordinate = (f32)fabs(keyFrameCursor->control.controlParams.srt.r.x);
                        if( fabs(keyFrameCursor->control.controlParams.srt.r.y) > largestCoordinate )
                            largestCoordinate = (f32)fabs(keyFrameCursor->control.controlParams.srt.r.y);
                        if( fabs(keyFrameCursor->control.controlParams.srt.r.z) > largestCoordinate )
                            largestCoordinate = (f32)fabs(keyFrameCursor->control.controlParams.srt.r.z);

                        // If in and out controls will be used, check for largest coordinate
                        if( C3_INTERPTYPE_ROT(trackCursor->interpolationType) == C3_INTERPTYPE_BEZIER || 
                            C3_INTERPTYPE_ROT(trackCursor->interpolationType) == C3_INTERPTYPE_HERMITE )
                        {
                            if( fabs(keyFrameCursor->controlInterpRot.inControl.x) > largestCoordinate )
                                largestCoordinate = (f32)fabs(keyFrameCursor->controlInterpRot.inControl.x);
                            if( fabs(keyFrameCursor->controlInterpRot.inControl.y) > largestCoordinate )
                                largestCoordinate = (f32)fabs(keyFrameCursor->controlInterpRot.inControl.y);
                            if( fabs(keyFrameCursor->controlInterpRot.inControl.z) > largestCoordinate )
                                largestCoordinate = (f32)fabs(keyFrameCursor->controlInterpRot.inControl.z);
                            if( fabs(keyFrameCursor->controlInterpRot.outControl.x) > largestCoordinate )
                                largestCoordinate = (f32)fabs(keyFrameCursor->controlInterpRot.outControl.x);
                            if( fabs(keyFrameCursor->controlInterpRot.outControl.y) > largestCoordinate )
                                largestCoordinate = (f32)fabs(keyFrameCursor->controlInterpRot.outControl.y);
                            if( fabs(keyFrameCursor->controlInterpRot.outControl.z) > largestCoordinate )
                                largestCoordinate = (f32)fabs(keyFrameCursor->controlInterpRot.outControl.z);
                        }
                    }

                    // If there is translation, check for largest coordinate
                    if( keyFrameCursor->control.type & CTRL_TRANS )
                    {
                        if( fabs(keyFrameCursor->control.controlParams.srt.t.x) > largestCoordinate )
                            largestCoordinate = (f32)fabs(keyFrameCursor->control.controlParams.srt.t.x);
                        if( fabs(keyFrameCursor->control.controlParams.srt.t.y) > largestCoordinate )
                            largestCoordinate = (f32)fabs(keyFrameCursor->control.controlParams.srt.t.y);
                        if( fabs(keyFrameCursor->control.controlParams.srt.t.z) > largestCoordinate )
                            largestCoordinate = (f32)fabs(keyFrameCursor->control.controlParams.srt.t.z);

                        // If in and out controls will be used, check for largest coordinate
                        if( C3_INTERPTYPE_TRANS(trackCursor->interpolationType) == C3_INTERPTYPE_BEZIER || 
                            C3_INTERPTYPE_TRANS(trackCursor->interpolationType) == C3_INTERPTYPE_HERMITE )
                        {
                            if( fabs(keyFrameCursor->controlInterpTrans.inControl.x) > largestCoordinate )
                                largestCoordinate = (f32)fabs(keyFrameCursor->controlInterpTrans.inControl.x);
                            if( fabs(keyFrameCursor->controlInterpTrans.inControl.y) > largestCoordinate )
                                largestCoordinate = (f32)fabs(keyFrameCursor->controlInterpTrans.inControl.y);
                            if( fabs(keyFrameCursor->controlInterpTrans.inControl.z) > largestCoordinate )
                                largestCoordinate = (f32)fabs(keyFrameCursor->controlInterpTrans.inControl.z);
                            if( fabs(keyFrameCursor->controlInterpTrans.outControl.x) > largestCoordinate )
                                largestCoordinate = (f32)fabs(keyFrameCursor->controlInterpTrans.outControl.x);
                            if( fabs(keyFrameCursor->controlInterpTrans.outControl.y) > largestCoordinate )
                                largestCoordinate = (f32)fabs(keyFrameCursor->controlInterpTrans.outControl.y);
                            if( fabs(keyFrameCursor->controlInterpTrans.outControl.z) > largestCoordinate )
                                largestCoordinate = (f32)fabs(keyFrameCursor->controlInterpTrans.outControl.z);
                        }
                    }

                    // If there is scale, check for largest coordinate
                    if( keyFrameCursor->control.type & CTRL_SCALE )
                    {
                        if( fabs(keyFrameCursor->control.controlParams.srt.s.x) > largestCoordinate )
                            largestCoordinate = (f32)fabs(keyFrameCursor->control.controlParams.srt.s.x);
                        if( fabs(keyFrameCursor->control.controlParams.srt.s.y) > largestCoordinate )
                            largestCoordinate = (f32)fabs(keyFrameCursor->control.controlParams.srt.s.y);
                        if( fabs(keyFrameCursor->control.controlParams.srt.s.z) > largestCoordinate )
                            largestCoordinate = (f32)fabs(keyFrameCursor->control.controlParams.srt.s.z);

                        // If in and out controls will be used, check for largest coordinate
                        if( C3_INTERPTYPE_SCALE(trackCursor->interpolationType) == C3_INTERPTYPE_BEZIER || 
                            C3_INTERPTYPE_SCALE(trackCursor->interpolationType) == C3_INTERPTYPE_HERMITE )
                        {
                            if( fabs(keyFrameCursor->controlInterpScale.inControl.x) > largestCoordinate )
                                largestCoordinate = (f32)fabs(keyFrameCursor->controlInterpScale.inControl.x);
                            if( fabs(keyFrameCursor->controlInterpScale.inControl.y) > largestCoordinate )
                                largestCoordinate = (f32)fabs(keyFrameCursor->controlInterpScale.inControl.y);
                            if( fabs(keyFrameCursor->controlInterpScale.inControl.z) > largestCoordinate )
                                largestCoordinate = (f32)fabs(keyFrameCursor->controlInterpScale.inControl.z);
                            if( fabs(keyFrameCursor->controlInterpScale.outControl.x) > largestCoordinate )
                                largestCoordinate = (f32)fabs(keyFrameCursor->controlInterpScale.outControl.x);
                            if( fabs(keyFrameCursor->controlInterpScale.outControl.y) > largestCoordinate )
                                largestCoordinate = (f32)fabs(keyFrameCursor->controlInterpScale.outControl.y);
                            if( fabs(keyFrameCursor->controlInterpScale.outControl.z) > largestCoordinate )
                                largestCoordinate = (f32)fabs(keyFrameCursor->controlInterpScale.outControl.z);
                        }
                    }

                    keyFrameCursor = (C3KeyFrame*)keyFrameCursor->link.Next;
                }
            }

            // Compute the number of shift bits and set the quantization type
            newQuantInfo = C3ComputeQuantizationShift( quantInfo, 0, largestCoordinate );
            trackCursor->paramQuantizeInfo = newQuantInfo;

            trackCursor = (C3Track*)trackCursor->link.Next;
        }
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3NewOptionObj

  Description:  Allocates a new C3OptionObj from the pool and returns it 
                initialized with the identifier.  The identifier is not copied,
                so the string must be persistent.

  Arguments:    identifier - Identifier of the geometry Object (C3GeomObject)

  Returns:      New option object
/*---------------------------------------------------------------------------*/
C3OptionObj*
C3NewOptionObj( char* identifier )
{
    C3OptionObj* optionObj;

    C3_ASSERT( identifier );

    optionObj = C3PoolOptionObj();

    // Zero out the entire object
    memset( optionObj, 0, sizeof(C3OptionObj) );

    optionObj->identifier = C3AddStringInTable( identifier );

    return optionObj;
}


/*---------------------------------------------------------------------------*
  Name:         C3AddOptionObj

  Description:  Adds the object to the global option list.

  Arguments:    obj - option object

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void         
C3AddOptionObj( C3OptionObj* obj )
{
    C3_ASSERT( obj );

    DSInsertListObject( &C3options.optionObjList, NULL, (void*) obj );
}


/*---------------------------------------------------------------------------*
  Name:         C3GetOptionObjList

  Description:  Returns a list of all option objects.

  Arguments:    NONE

  Returns:      List of all option objects.
/*---------------------------------------------------------------------------*/
DSList*
C3GetOptionObjList( )
{
    return &C3options.optionObjList;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetOptionObjFromId

  Description:  Returns the appropriate option object using identifier to locate
                it.  Returns NULL if no matching option object with the exact same
                identifier is found.

  Arguments:    identifier - identifier of the geometry object

  Returns:      Option object if found.  NULL otherwise.
/*---------------------------------------------------------------------------*/
C3OptionObj*
C3GetOptionObjFromId( char* identifier )
{
    // Look through the optionObjList and find the one that corresponds with identifier
    C3OptionObj* obj    = NULL;
    C3OptionObj* cursor = NULL;

    cursor = (C3OptionObj*)C3options.optionObjList.Head;
    while ( cursor )
    {
        // for each object compare to the identifier
        if( strcmp( cursor->identifier, identifier ) == 0 )
        {
            return cursor;
        }
        cursor = (C3OptionObj*)cursor->link.Next;
    }

    return NULL;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetOptionReportStatusFunc

  Description:  Sets the report status function pointer.

  Arguments:    func - the new report status function
                  
  Returns:      NONE
/*---------------------------------------------------------------------------*/
void  
C3SetOptionReportStatusFunc( C3ReportStatusFunc func )
{
    C3options.reportStatusFunc = func;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetOptionReportStatusFunc

  Description:  Returns the report status function pointer. The string should not
                contain any newline characters (for Maya compatibility).

  Arguments:    NONE
                  
  Returns:      Report status function pointer
/*---------------------------------------------------------------------------*/
C3ReportStatusFunc
C3GetOptionReportStatusFunc( )
{
    return C3options.reportStatusFunc;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetOptionReportErrorFunc

  Description:  Sets the report error function pointer.  The string should not
                contain any newline characters (for Maya compatibility).

  Arguments:    func - the new report status function
                  
  Returns:      NONE
/*---------------------------------------------------------------------------*/
void  
C3SetOptionReportErrorFunc( C3ReportErrorFunc func )
{
    C3options.reportErrorFunc = func;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetOptionReportErrorFunc

  Description:  Returns the report error function pointer.

  Arguments:    NONE
                  
  Returns:      Report error function pointer
/*---------------------------------------------------------------------------*/
C3ReportStatusFunc
C3GetOptionReportErrorFunc( )
{
    return C3options.reportErrorFunc;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetOptionOutputEndian

  Description:  Sets the endian byte order of C3 library outputs.

  Arguments:    outputEndian - Either C3_OUTPUT_BIG_ENDIAN or C3_OUTPUT_LITTLE_ENDIAN
                  
  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetOptionOutputEndian( u8 outputEndian )
{
    C3_ASSERT( outputEndian == C3_OUTPUT_BIG_ENDIAN ||
               outputEndian == C3_OUTPUT_LITTLE_ENDIAN );

    C3options.outputEndian = outputEndian;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetOptionOutputEndian

  Description:  Returns the endian byte order of C3 library outputs.

  Arguments:    NONE
                  
  Returns:      C3_OUTPUT_BIG_ENDIAN or C3_OUTPUT_LITTLE_ENDIAN
/*---------------------------------------------------------------------------*/
u8
C3GetOptionOutputEndian( )
{
    return C3options.outputEndian;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetOptionSrcVertexOrder

  Description:  Sets the vertex order flag to specify if the primitive received
                are in CCW or CW order.
    
  Arguments:    vtxOrder - either C3_CCW or C3_CW

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3SetOptionSrcVertexOrder( u8 vtxOrder )
{
    C3_ASSERT( (vtxOrder == C3_CW) || (vtxOrder == C3_CCW));
    C3options.vtxOrder = vtxOrder;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetOptionSrcVertexOrder

  Description:  Returns the vertex order.
    
  Arguments:    NONE

  Returns:      Vertex order - either C3_CCW or C3_CW
/*---------------------------------------------------------------------------*/
u8
C3GetOptionSrcVertexOrder( )
{
    return C3options.vtxOrder;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetOptionFileExportFlag

  Description:  Sets the file export flag. 
                
  Arguments:    fileExportflag - output flags.   Must be a combination of:
                C3_FILE_ALL,        C3_FILE_GEOMETRY,       C3_FILE_HIERARCHY,
                C3_FILE_ANIMATION,  C3_FILE_TEXTURE,        C3_FILE_STATS.
                The list of flags is described in C3Out.h.

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void    
C3SetOptionFileExportFlag( u32 fileExportflag )
{
    C3options.fileExportFlag = fileExportflag;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetOptionFileExportFlag

  Description:  Returns the file export flag
    
  Arguments:    NONE

  Returns:      File Export flag
/*---------------------------------------------------------------------------*/
u32 
C3GetOptionFileExportFlag( )
{
    return C3options.fileExportFlag;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetOptionUseDefaultNormalTable

  Description:  Set whether or not to use an default (internal) normal
                table.  If this option is false, the normals generated by
                the artist CG Tool will be exported.
    
  Arguments:    flag - C3_TRUE or C3_FALSE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetOptionUseDefaultNormalTable( C3Bool flag )
{
    C3options.useDefaultNormalTable = flag;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetOptionUseExternalNormalTable

  Description:  Returns true if the option is to use an default normal table.
    
  Arguments:    NONE

  Returns:      Use Default Normal Table option
/*---------------------------------------------------------------------------*/
C3Bool
C3GetOptionUseDefaultNormalTable( )
{
    return C3options.useDefaultNormalTable;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetOptionUseExternalNormalTable

  Description:  Set whether or not to use an external (pre-defined) normal
                table.  If this option is false, the normals generated by
                the artist CG Tool will be exported.
    
  Arguments:    flag - C3_TRUE or C3_FALSE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetOptionUseExternalNormalTable( C3Bool flag )
{
    C3options.useExternalNormalTable = flag;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetOptionUseExternalNormalTable

  Description:  Returns true if the option is to use an external normal table.
    
  Arguments:    NONE

  Returns:      Use External Normal Table option
/*---------------------------------------------------------------------------*/
C3Bool
C3GetOptionUseExternalNormalTable( )
{
    return C3options.useExternalNormalTable;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetOptionExternalNormalTablePath

  Description:  Sets the path to the external normal table path.  No error
                checking, so path and file must exist.
    
  Arguments:    name - full path to the file.  Cannot exceed C3_MAX_PATH in length

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetOptionExternalNormalTablePath( char* name )
{
    strncpy( C3options.externalNormalTablePath, name, C3_MAX_PATH );
}


/*---------------------------------------------------------------------------*
  Name:         C3GetOptionExternalNormalTablePath

  Description:  Returns a pointer to the string buffer with the path to the
                external normal table.
    
  Arguments:    NONE

  Returns:      Path to the external normal table
/*---------------------------------------------------------------------------*/
char*
C3GetOptionExternalNormalTablePath( )
{
    return C3options.externalNormalTablePath;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetOptionEnableStripFan

  Description:  Enables quads/strips/fans for geometry.
    
  Arguments:    flag - C3_TRUE or C3_FALSE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void    
C3SetOptionEnableStripFan( C3Bool flag )
{
    C3options.enableStripFan = flag;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetOptionEnableStripFan

  Description:  Returns whether geometry will be compressed to quads/strips/fans.
    
  Arguments:    NONE

  Returns:      Enable Strip/Fan option
/*---------------------------------------------------------------------------*/
C3Bool
C3GetOptionEnableStripFan( )
{
    return C3options.enableStripFan;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetOptionEnableStripFanView

  Description:  Enables whether you can view different colors for quads/strips/fans 
                to see their effectiveness at output.  Note: Colors cannot be
                compressed when this option is turned on.  So call this function
                after C3SetOptionCompressFlags(...).
    
  Arguments:    flag - C3_TRUE or C3_FALSE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void    
C3SetOptionEnableStripFanView( C3Bool flag )
{
    u16 compressFlags = C3GetOptionCompress();

    // Remove the compress color option if enabling strip/fan view
    if( flag )
    {
        compressFlags &= ~C3_TARGET_COLOR;
        C3SetOptionCompress( compressFlags );   
    }

    C3options.enableStripFanView = flag;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetOptionEnableStripFanView

  Description:  Returns whether quads, strips, and fans can be viewed as
                different colors to measure their effectiveness.
    
  Arguments:    NONE

  Returns:      View Strip/Fan option
/*---------------------------------------------------------------------------*/
C3Bool
C3GetOptionEnableStripFanView( )
{
    return C3options.enableStripFanView;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetOptionCompress

  Description:  Sets the flags of whether or not to remove duplicate positions,
                colors, texture coordinates, and normals.
    
  Arguments:    target - target flags which should be a bitwise OR of
                C3_TARGET_POSITION, C3_TARGET_COLOR,    
                C3_TARGET_TEXCOORD, C3_TARGET_NORMAL.
                Remember that C3_TARGET_ALL does not include C3_TARGET_COLOR
                The targets are defined in C3CnvOpz.h

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void    
C3SetOptionCompress( u16 targets )
{
    // Cannot compress colors when enabling strip/fan view
    if( C3GetOptionEnableStripFanView() )
        targets &= ~C3_TARGET_COLOR;

    C3options.compressFlag = targets;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetOptionCompress

  Description:  Returns the target flags which indicate which targets to compress.
                The return flag can be a bitwise OR of
                C3_TARGET_POSITION, C3_TARGET_COLOR,    
                C3_TARGET_TEXCOORD, C3_TARGET_NORMAL.
                Remember that C3_TARGET_ALL does not include C3_TARGET_COLOR
                The targets are defined in C3CnvOpz.h
    
  Arguments:    NONE

  Returns:      Compress data flag
/*---------------------------------------------------------------------------*/
u16
C3GetOptionCompress( )
{
    return C3options.compressFlag;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetOptionPositionRange

  Description:  Sets the position range for all positions.  Used to compute
                position quantization.  If the range is 0.0f, C3 library will
                automatically calculate the shift bits to maximize precision
                in C3ProcessOptionsAfterCompression.
    
  Arguments:    range - range of position data.

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3SetOptionPositionRange( f32 range )
{
    C3options.positionRange = range;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetOptionPositionRange

  Description:  Returns the position range.
    
  Arguments:    NONE

  Returns:      Position range
/*---------------------------------------------------------------------------*/
f32
C3GetOptionPositionRange( )
{
    return C3options.positionRange;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetOptionQuantization

  Description:  Sets the quantization information for position, texture
                coordinates, normals and color with quantInfo.
                -For position, the range must be set properly by C3SetOptionPositionRange
                 for quantization shift bits to be calculated. 
                -For normals, quantization shift bits will be calculated assuming
                 that all normals are normalized to length 1.
                -For texture coordinates, only should hold global
                 type of quantization--per object variables are stored in 
                 C3OptionObj.
                -For keyframes, only holds the global type of quantization for
                 all tracks.  Only quantizes translation and scale and their
                 in and out controls since quaternions are assumed to be
                 normalized to length 1.
    
  Arguments:    target - which one to set quantization info for.  Must be one of
                C3_TARGET_POSITION, C3_TARGET_TEXCOORD to 1, C3_TARGET_KEYFRAME
                C3_TARGET_NORMAL,   C3_TARGET_COLOR,    C3_TARGET_COLORALPHA
                The list of defines is in C3CnvOpz.h.

                quantInfo - Top 4 MSBs represent type of quantization (see C3Util.h)
                            Bottom 4 LSBs represent shift bits

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void    
C3SetOptionQuantization( u32 target, u8 channel, u8 quantInfo )
{
    C3_ASSERT( target && target != C3_TARGET_ALL );

    if( target & C3_TARGET_POSITION )
    {
        C3options.positionQuantization = quantInfo;
    }
    else if( target & C3_TARGET_NORMAL )
    {
        if( C3_QUANT_TYPE( quantInfo ) != GX_F32 )
        {
            // Normal components are always between 0 and 1
            quantInfo = C3ComputeQuantizationShift( quantInfo, 0, 1 );
        }
        C3options.normalQuantization = quantInfo;
    }
    else if( target & C3_TARGET_COLOR )
    {
        C3options.colorQuantization = quantInfo;
    }
    else if( target & C3_TARGET_COLORALPHA )
    {
        C3options.colorAlphaQuantization = quantInfo;   
    }
    else if( target & C3_TARGET_TEXCOORD )
    {
        C3_ASSERT( channel >= 0 && channel < C3_MAX_TEXTURES );
        C3options.texCoordQuantization[channel] = quantInfo;
    }
    else if( target & C3_TARGET_KEYFRAME )
    {
        C3options.keyFrameQuantization = quantInfo;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3GetOptionQuantization

  Description:  Returns the quantization information for the specified target.
                For texture coordinates, only should hold global
                type of quantization--per object variables are stored in 
                C3OptionObj.

  Arguments:    target - which one to get quantization info for.  Must be one of
                C3_TARGET_POSITION, C3_TARGET_TEXCOORD to 1, C3_TARGET_KEYFRAME
                C3_TARGET_NORMAL,   C3_TARGET_COLOR,    C3_TARGET_COLORALPHA
                The list of defines is in C3CnvOpz.h.

  Returns:      Quantization information
/*---------------------------------------------------------------------------*/
u8      
C3GetOptionQuantization( u32 target, u8 channel )
{
    switch ( target )
    {
    case C3_TARGET_POSITION:
        return C3options.positionQuantization;
    case C3_TARGET_NORMAL:
        return C3options.normalQuantization;
    case C3_TARGET_COLOR:
        return C3options.colorQuantization;
    case C3_TARGET_COLORALPHA:
        return C3options.colorAlphaQuantization;
    case C3_TARGET_TEXCOORD:
        C3_ASSERT( channel >= 0 && channel < C3_MAX_TEXTURES );
        return C3options.texCoordQuantization[channel];
    case C3_TARGET_KEYFRAME:
        return C3options.keyFrameQuantization;
    }

    C3_ASSERT( C3_FALSE );
    return 0;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetOptionQuantizationString

  Description:  Returns the quantization string depending on the type

  Arguments:    type - type of quantization

  Returns:      String
/*---------------------------------------------------------------------------*/
char*
C3GetOptionQuantizationString( u16 target, u8 type )
{
    if( target == C3_TARGET_COLOR || target == C3_TARGET_COLORALPHA )
        switch( type )
        {
        case GX_RGB565:
            return C3_STR(RGB565);
        case GX_RGB8:
            return C3_STR(RGB8);
        case GX_RGBX8:
            return C3_STR(RGBX8);
        case GX_RGBA4:
            return C3_STR(RGBA4);
        case GX_RGBA6:
            return C3_STR(RGBA6);
        case GX_RGBA8:
            return C3_STR(RGBA8);
        default:
            return NULL;
        }
    else
        switch( type )
        {
        case GX_F32:
            return C3_STR(F32);
        case GX_U16:
            return C3_STR(U16);
        case GX_S16:
            return C3_STR(S16);
        case GX_U8:
            return C3_STR(U8);
        case GX_S8:
            return C3_STR(S8);
        default:
            return NULL;
        }
}


/*---------------------------------------------------------------------------*
  Name:         C3SetOptionEnableStitching

  Description:  Set whether or not to enable stitching which will set a matrix
                index per vertex.
    
  Arguments:    flag - C3_TRUE or C3_FALSE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetOptionEnableStitching( C3Bool flag )
{
    C3options.enableStitching = flag;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetOptionEnableStitching

  Description:  Returns true if the option is to enable stitching.

  Arguments:    flag - C3_TRUE or C3_FALSE

  Returns:      Stitching options
/*---------------------------------------------------------------------------*/
C3Bool
C3GetOptionEnableStitching( )
{
    return C3options.enableStitching;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetOptionEnableLighting

  Description:  Set whether or not to enable lighting.  If lighting is enabled,
                normals will be output.  Otherwise, lighting data is not output.
    
  Arguments:    flag - C3_TRUE or C3_FALSE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetOptionEnableLighting( C3Bool flag )
{
    C3options.enableLighting = flag;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetOptionEnableLighting

  Description:  Returns true if the option is to enable lighting.
    
  Arguments:    NONE

  Returns:      Lighting option
/*---------------------------------------------------------------------------*/
C3Bool
C3GetOptionEnableLighting( )
{
    return C3options.enableLighting;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetOptionAmbientPercentage

  Description:  Sets the ambient percentage for lighting.  The ambient colors
                will be this percentage of the vertex color when lighting
                is enabled.

  Arguments:    percent - percentage, must be between 0.0f and 1.0f, inclusive

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetOptionAmbientPercentage( f32 percent )
{
    C3options.ambientPercentage = percent;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetOptionAmbientPercentage

  Description:  Gets the ambient percentage for lighting.

  Arguments:    NONE

  Returns:      Ambient percentage
/*---------------------------------------------------------------------------*/
f32
C3GetOptionAmbientPercentage( )
{
    return C3options.ambientPercentage;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetOptionWeldRadius

  Description:  Sets the weld radius for position or texture coordinate welding.

  Arguments:    target - which one to set weld radius for.  Must be one of
                C3_TARGET_ALL,  C3_TARGET_POSITION,     C3_TARGET_TEXCOORD.
                The list of defines is in C3CnvOpz.h.

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void    
C3SetOptionWeldRadius( u32 target, f32 radius )
{
    C3_ASSERT( target );
    C3_ASSERT( radius >= -C3_FLT_EPSILON );

    if( target & C3_TARGET_POSITION )
        C3options.positionWeldRadius = radius;

    if( target & C3_TARGET_TEXCOORD )
        C3options.texCoordWeldRadius = radius;

    if( target & C3_TARGET_NORMAL )
        C3options.normalWeldRadius = radius;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetOptionWeldRadius

  Description:  Returns the weld radius for position or texture coordinates.
    
  Arguments:    target - which one to get quantization info for.  Must be one of
                C3_TARGET_POSITION,     C3_TARGET_TEXCOORD,
                The list of defines is in C3CnvOpz.h.

  Returns:      Weld radius
/*---------------------------------------------------------------------------*/
f32     
C3GetOptionWeldRadius( u32 target )
{
    switch ( target )
    {
    case C3_TARGET_POSITION:
        return C3options.positionWeldRadius;
    case C3_TARGET_TEXCOORD:
        return C3options.texCoordWeldRadius;
    case C3_TARGET_NORMAL:
        return C3options.normalWeldRadius;
    }

    C3_ASSERT( C3_FALSE );
    return 0;
}

