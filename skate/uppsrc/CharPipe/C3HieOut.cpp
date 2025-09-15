/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3HieOut.c]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/build/charPipeline/c3/src/C3HieOut.c $
    
    12    12/06/00 7:26p John
    Fixed a bug in removing bones w/o a control.
    
    11    12/04/00 8:28p John
    Fixed a potential call to allocate 0 memory.
    
    10    11/13/00 1:45p John
    Animation and hierarchy controls are written to files using separate
    functions.
    
    9     8/14/00 6:15p John
    Added user-defined data to ANM, ACT, and GPL formats.
    
    8     3/14/00 1:33p John
    Moved display priority from GPL to ACT.
    
    7     2/29/00 7:23p John
    Cleaned up header and added comments.
    Does not write ACT file if there are no bones.
    
    6     2/04/00 6:07p John
    Untabified code.
    
    5     1/31/00 4:15p John
    Added pruning of unnecessary bones and animations.
    Finished stitching with FIFO matrix cache and stripping.
    
    4     1/20/00 1:14p John
    Added stitching (work in progress).
    Added C3 string table for efficient management of strings.
    
    3     12/15/99 1:19p John
    Added ACT version information.
    
    2     12/08/99 7:26p John
    Added animation quantization.
    
    10    11/11/99 1:22p John
    Changed C3_CTRL_SIZE to sizeof(CTRLControl)
    
    9     9/29/99 4:32p John
    Changed header to make them all uniform with logs.
    
    8     9/17/99 9:34a John
    Added C3Options and cleaned up code.

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#include "C3Private.h"

#define C3_HEADER_SIZE  sizeof(ACTLayout)
#define C3_BONE_SIZE    sizeof(ACTBoneLayout)
#define C3_CTRL_SIZE    sizeof(CTRLControl)

// Local functions
static void C3WriteActorHeader    ( C3Actor*    actor, 
                                    u32         stringOffset, 
                                    FILE*       outFile );
static void C3WriteActorBoneArray ( C3Actor*    actor, 
                                    u32         ctrlOffset,
                                    FILE*       outFile );
static void C3WriteActorBone      ( C3HierNode* hNode, 
                                    u32         ctrlOffset,
                                    void**      buffer );
static void C3WriteCtrlArray      ( C3Actor* actor, FILE* outFile );
static void C3WriteHierControl    ( C3Control *control, void *array );

static void C3RemoveUnusedBones   ( DSTree *hierarchy, C3HierNode* bone, C3Actor* actor );
static void C3PrintHierarchy      ( C3HierNode* node, FILE* outFile );

extern u32   ActUserDataSize;
extern void *ActUserData;

/*---------------------------------------------------------------------------*
  Name:         C3WriteHierarchy

  Description:  Writes the hierarchy in the specified path/file

  Arguments:    path - path for the file
                name - name of the file

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3WriteHierarchy( char* path, char* name )
{
    char*       fileName        = NULL;
    char        geoPalName[256] = {0x00};
    u32         stringOffset    = 0;
    u32         ctrlOffset      = 0;
    FILE*       outFile         = NULL;
    C3Actor*    actor           = NULL;
    void*       cursor          = NULL;
    u16         numControls     = 0;

    C3_ASSERT( path && name );

    // Get the actor
    actor = C3GetActor( );
    C3_ASSERT( actor );

    // Don't write actor file if there are no bones
    if( actor->numBones <= 0 )
        return;

    C3MakePath( path, name, C3_EXT_HIERARCHY, &fileName );
    sprintf( geoPalName,"%s.%s", name, C3_EXT_GEO_PALETTE );

    // Open file
    outFile = fopen( fileName, "wb");
    C3_ASSERT( outFile );

    // Calculate offsets
    ctrlOffset   = C3_HEADER_SIZE + actor->numBones * C3_BONE_SIZE;
    stringOffset = ctrlOffset + actor->numControls * C3_CTRL_SIZE;
    if( actor->numControls <= 0 )
    {
        ctrlOffset = 0;
    }

    // Write the actor layout header
    C3WriteActorHeader( actor, stringOffset, outFile );

    // Write the bone array
    C3WriteActorBoneArray( actor, ctrlOffset, outFile );

    // Write the control array
    if( actor->numControls > 0 )
        C3WriteCtrlArray( actor, outFile );

    // Write the string array
    fwrite( (Ptr)geoPalName, 1, strlen(geoPalName) + 1, outFile );

    if( ActUserDataSize > 0 )
    {
        // Write the user defined data starting at 32 byte boundary
        C3WritePadded32byte( 0, 0, outFile );
        C3WritePadded32bit( ActUserData, ActUserDataSize, outFile );
    }

    // Close file
    fclose( outFile );

    // Free the file names
    C3_FREE( fileName );
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteActorHeader

  Description:  Writes the actor header 

  Arguments:    actor        - actor to write the information from
                stringOffset - offset, in bytes, to the string bank in the 
                                file.
                outFile      - file to output to.

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3WriteActorHeader
( 
    C3Actor*    actor, 
    u32         stringOffset, 
    FILE*       outFile 
)
{  
    C3GeomObject*   geomObj = NULL;
    u32             offset  = 0;
    u16             id      = 0;

    // write the version date
    C3WriteU32F( C3_ACT_VERSION, outFile );

    // actor ID
    C3WriteU16F( id , outFile);

    // # bones in actor
    C3WriteU16F( actor->numBones, outFile );
  
    // branch offset (tree.offset)
    C3WriteU32F( actor->hierarchy.Offset , outFile);

    // Root bone offset 
    offset = C3_HEADER_SIZE + ((C3HierNodePtr)actor->hierarchy.Root)->index * C3_BONE_SIZE;
    C3WriteU32F( offset, outFile );
    
    // GeoPalette file name offset
    C3WriteU32F( stringOffset, outFile );
      
    // write the skin file ID within GeoPalette
    offset = C3_HIERNODE_NO_GEOM;
    geomObj = C3GetObjectFromIdentifier( actor->skinMeshIdentifier );
    if( geomObj )
    {
        offset = geomObj->index;
    }
    C3WriteU16F( (u16)offset, outFile );

    // Write 16 bit pad
    C3WriteU16F( 0, outFile );

    // Write the user defined data size
    C3WriteU32F( ActUserDataSize, outFile );

    // Write the offset to the user defined data
    if( ActUserDataSize > 0 )
    {
        C3_PAD32_BYTES( stringOffset );
    }
    else
    {
        stringOffset = 0;
    }
    C3WriteU32F( stringOffset, outFile );
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteActorBoneArray

  Description:  Write the actor bone array in file

  Arguments:    actor       - actor to take the information from
                ctrlOffset  - offset in bytes from the file start to the 
                                ctrlOffset
                outFile     -

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3WriteActorBoneArray
( 
    C3Actor*    actor, 
    u32         ctrlOffset, 
    FILE*       outFile 
)
{
    void* array       = NULL;
    void* arrayCursor = NULL;
    void* cursor      = NULL;
    u32 size        = 0;


    size = actor->numBones * C3_BONE_SIZE;

    // allocate an array
    array = arrayCursor = C3_CALLOC( 1, size );
    C3_ASSERT( array );
  
    cursor = actor->boneList.Head;
    while( cursor )
    {
        C3WriteActorBone( (C3HierNodePtr)cursor, 
                          ctrlOffset, 
                          &arrayCursor );
        cursor = ((C3HierNodePtr)cursor)->link.Next;
    }

    // write array
    fwrite( array, size, 1, outFile );

    // free array
    C3_FREE( array );
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteActorBone

  Description:  Writes the hierarchy node specified

  Arguments:    hNode       - node to write
                ctrlOffset  - offset in bytes from the file start to the 
                                ctrlOffset
                buffer      - buffer in which to write, will be incremented
                              by the size written

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3WriteActorBone
( 
    C3HierNode* hNode, 
    u32         ctrlOffset,
    void**      buffer 
)
{
    C3GeomObject*   geomObj = NULL;
    u8*             tBuffer = NULL;
    u8              count   = 0;
    u32             val     = 0;


    tBuffer = (u8*)*buffer;
    // Jump over the section we are writing
    *buffer = (void*)(tBuffer + C3_BONE_SIZE);

    // Write the control offset
    if( hNode->control )
    {
        C3_ASSERT( hNode->control->index != 0xFFFFFFFF );
        val = ctrlOffset + hNode->control->index * C3_CTRL_SIZE;
    }
    count += C3WriteU32( val, (void*)(tBuffer + count ));
    val = 0;

    // previous sibling
    if( hNode->branch.Prev)
    {
        val = C3_HEADER_SIZE + 
              ((C3HierNodePtr)hNode->branch.Prev)->index * C3_BONE_SIZE;
    }
    count += C3WriteU32( val, (void*)(tBuffer + count) );
    val = 0;

    // next sibling
    if( hNode->branch.Next)
    {
        val = C3_HEADER_SIZE + 
              ((C3HierNodePtr)hNode->branch.Next)->index * C3_BONE_SIZE;
    }
    count += C3WriteU32( val, (void*)(tBuffer + count) );
    val = 0;

    // parent
    if( hNode->branch.Parent)
    {
        val = C3_HEADER_SIZE + 
              ((C3HierNodePtr)hNode->branch.Parent)->index * C3_BONE_SIZE;
    }
    count += C3WriteU32( val, (void*)(tBuffer + count) );
    val = 0;
    
    // child
    if( hNode->branch.Children)
    {
        val = C3_HEADER_SIZE + 
              ((C3HierNodePtr)hNode->branch.Children)->index * C3_BONE_SIZE;
    }
    count += C3WriteU32( val, (void*)(tBuffer + count) );

    // geo file ID within GeoPalette
    val = C3_HIERNODE_NO_GEOM;
    geomObj = C3GetObjectFromIdentifier( hNode->geomObjName );
    if( geomObj )
    {
        val = geomObj->index;
    }
    count += C3WriteU16( (u16)val, (void*)(tBuffer + count) );  
    val = 0;

    // bone ID
    count += C3WriteU16( hNode->id, (void*)(tBuffer + count) );  

    // inheritance flag
    tBuffer[count++] = hNode->inheritanceFlag;

    // display priority
    tBuffer[count++] = hNode->displayPriority;
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteCtrlArray

  Description:  Writes the control array

  Arguments:    actor   - actor to take the control information from
                outFile - file to output the control to

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3WriteCtrlArray( C3Actor* actor, FILE* outFile )
{
    void*       array       = NULL;
    u8*         arrayCursor = NULL;
    void*       cursor      = NULL;
    C3Control*  ctrl        = NULL;

    arrayCursor = (u8*)array = C3_CALLOC( actor->numControls, C3_CTRL_SIZE );
    C3_ASSERT( array );

    cursor = actor->controlList.Head;
    while(cursor)
    {
        ctrl = (C3Control*)cursor;
        C3WriteHierControl( ctrl, arrayCursor );
        arrayCursor += sizeof(CTRLControl); 
        cursor = ((C3Control*)cursor)->link.Next;
    }

    // write array
    fwrite( array, C3_CTRL_SIZE, actor->numControls, outFile );

    // free array
    C3_FREE( array );
}

static void
C3WriteHierControl( C3Control *control, void *array )
{
    u8   *typePtr     = NULL;
    u8   *ptr;
    u32   i;
    u8    floatSize;

//    u8    quantInfo;
//    u32     qx, qy, qz;
//    f64     xInt, yInt, zInt;
//    f32     xFrac, yFrac, zFrac;

    floatSize = sizeof(f32);

    // Write out a header for CTRLControl
    typePtr  = array;
    *typePtr = control->c.type;
    ptr = ((u8*)array) + 4; // type + padding

    // Quantize the fractional portion of the translation values to the position fixed grid
    // if the matrix is for a hierarchy
    /*
    quantInfo = C3GetOptionQuantization( C3_TARGET_POSITION, 0 );
    if ( C3_QUANT_TYPE( quantInfo ) != GX_F32 )
    {
        // Split the translation into integer and fractional components
        xFrac = (f32)modf( MTXRowCol(control->matrix,0,3), &xInt );
        yFrac = (f32)modf( MTXRowCol(control->matrix,1,3), &yInt );
        zFrac = (f32)modf( MTXRowCol(control->matrix,2,3), &zInt );

        // Quantize the fractional portion
        C3QuantizeFloat( &qx, quantInfo, xFrac );
        C3QuantizeFloat( &qy, quantInfo, yFrac );
        C3QuantizeFloat( &qz, quantInfo, zFrac );
        
        // Then unquantize to get the snapped translation
        MTXRowCol(control->matrix,0,3) = C3FloatFromQuant( &qx, quantInfo ) + (s32)xInt;
        MTXRowCol(control->matrix,1,3) = C3FloatFromQuant( &qy, quantInfo ) + (s32)yInt;
        MTXRowCol(control->matrix,2,3) = C3FloatFromQuant( &qz, quantInfo ) + (s32)zInt;

        // TESTING: Print out positions of matrix to test out quantization
//              f = fopen( "D:\\temp\\positions.txt", "at" );
//              fprintf( f, "Matrix: x: %.10f y: %.10f z: %.10f\n", control->matrix[0][3], control->matrix[1][3], control->matrix[2][3] );
//              fclose( f );
    }
    */

    if( control->c.type == CTRL_MTX )
    {
        // Write the matrix
        for( i = 0; i < 12; i++ )
        {
            C3WriteFloat( ((f32*)(control->c.controlParams.mtx.m))[i], ptr );
            ptr += floatSize;
        }
    }
    else
    {
        // Write the scale
        C3WriteFloat( control->c.controlParams.srt.s.x, ptr );
        ptr += floatSize;
        C3WriteFloat( control->c.controlParams.srt.s.y, ptr );
        ptr += floatSize;
        C3WriteFloat( control->c.controlParams.srt.s.z, ptr );
        ptr += floatSize;

        // Write the rotation
        C3WriteFloat( control->c.controlParams.srt.r.x, ptr );
        ptr += floatSize;
        C3WriteFloat( control->c.controlParams.srt.r.y, ptr );
        ptr += floatSize;
        C3WriteFloat( control->c.controlParams.srt.r.z, ptr );
        ptr += floatSize;
        C3WriteFloat( control->c.controlParams.srt.r.w, ptr );
        ptr += floatSize;

        // Write the translation
        C3WriteFloat( control->c.controlParams.srt.t.x, ptr );
        ptr += floatSize;
        C3WriteFloat( control->c.controlParams.srt.t.y, ptr );
        ptr += floatSize;
        C3WriteFloat( control->c.controlParams.srt.t.z, ptr );
        ptr += floatSize;
    } 
}


/*---------------------------------------------------------------------------*
  Name:         C3ConvertActor

  Description:  Prune away unnecessary bones from the hierarchy, as well as
                the controls and animation tracks attached to them.  This 
                should be called after extraction, but before anything is 
                output.

  Arguments:    actor   - actor to convert

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3ConvertActor( C3Actor* actor )
{
    C3HierNode *bone;
    C3Control* control;
    u16 i;

    // TESTING: Print actor hierarchy before pruning
/*
    FILE *f;    
    f = fopen( "D:\\temp\\actorTree.txt", "wt" );
    fprintf( f, "\n\n==========>Before pruning...\n\n" );
    fprintf( f, "Number of bones: %d\n", actor->numBones );
    fprintf( f, "Number of controls: %d\n", actor->numControls );
    bone = (C3HierNode*)actor->hierarchy.Root;
    while( bone )
    {
        C3PrintHierarchy( bone, f );

        bone = (C3HierNode*)bone->branch.Next;
        if( bone )
            fprintf( f, "Moving to next in top level %s\n", bone->identifier );
    }
//*/

    // Pruning of actor can affect position matrix indices in display list
    // and can affect animation with the removal of tracks which were attached
    // to unused bones.  Hence this step should be completed after extraction
    // but before writing the files.  We should only convert the actor
    // (or prune bones) if we are exporting geometry, since without geometry
    // necessary bones can be pruned.  The only way to get an accurate
    // hierarchy output file is to also export geometry.
    if( C3GetOptionFileExportFlag() & C3_FILE_GEOMETRY )
    {
        // Eliminate extraneous (unused bones)
        bone = (C3HierNode*)actor->hierarchy.Root;

        // Return if there are no bones
        if( !bone )
            return;

        while( bone )
        {
            C3RemoveUnusedBones( &actor->hierarchy, bone, actor );

            bone = (C3HierNode*)bone->branch.Next;
        }

        // Make the indices of hierarchy node list sequential (not ids)
        i = 0;
        bone = (C3HierNode*)actor->boneList.Head;
        while( bone )
        {
            bone->index = i++;
            bone = (C3HierNode*)bone->link.Next;
        }

        // Make the indices of controls sequential
        i = 0;
        control = (C3Control*)actor->controlList.Head;
        while( control )
        {
            control->index = i++;
            control = (C3Control*)control->link.Next;
        }

        // Now that bones are removed, remove animations that were
        // attached to unused bones
        if( C3GetOptionFileExportFlag() & C3_FILE_ANIMATION )
            C3RemoveUnusedTracks( (C3AnimBank*)actor->animBank );

/*
        // TESTING: Print actor hierarchy after pruning
        fprintf( f, "\n\n==========>After pruning...\n\n" );
        fprintf( f, "Number of bones: %d\n", actor->numBones );
        fprintf( f, "Number of controls: %d\n", actor->numControls );
        bone = (C3HierNode*)actor->hierarchy.Root;
        while( bone )
        {
            C3PrintHierarchy( bone, f );

            bone = (C3HierNode*)bone->branch.Next;
            if( bone )
                fprintf( f, "Moving to next in top level %s\n", bone->identifier );
        }
        fclose( f );
//*/
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3RemoveUnusedBones

  Description:  Remove unused bones from the hierarchy.  A bone is unused if
                the bone is not skinned and there is no display object attached
                to that bone or any of its children.  Assumes hierarchy is
                an acyclic graph (no loops).

  Arguments:    hierarchy - tree of C3HierNodes to prune

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3RemoveUnusedBones( DSTree *hierarchy, C3HierNode *bone, C3Actor *actor )
{
    C3HierNode *child;

    child = (C3HierNode*)bone->branch.Children;
    while( child )
    {
        C3RemoveUnusedBones( hierarchy, child, actor );

        child = (C3HierNode*)child->branch.Next;
    }

    // If no geometry object 
    if( !bone->geomObjName && !bone->usedForStitching && !bone->branch.Children )
    {
//      C3ReportError( "Removing unused bone %s", bone->identifier );

        // Remove control point and pivot from their lists
        if( bone->control )
        {
            bone->control->index = 0xFFFFFFFF;
            DSRemoveListObject( &actor->controlList, (void*)bone->control );
            actor->numControls--;
        }

        // Signal that the id is no longer used
        bone->id = C3_HIERNODE_NOT_USED;

        DSExtractBranch( hierarchy, (void*)bone );
        DSRemoveListObject( &actor->boneList, (void*)bone );
        actor->numBones--;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3PrintHierarchy

  Description:  Debug function to print the current actor hierarchy.

  Arguments:    node - node to start printing at
                outFile - output file

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3PrintHierarchy( C3HierNode* node, FILE* outFile )
{
    C3HierNode* next;

    fprintf( outFile, "Id: %d Index: %d Name: %s\n", node->id, node->index, node->identifier );
    if( node->branch.Parent )
        fprintf( outFile, "\tParent: %s\n", ((C3HierNode*)node->branch.Parent)->identifier );
    if( node->branch.Prev )
        fprintf( outFile, "\tPrev: %s\n", ((C3HierNode*)node->branch.Prev)->identifier );
    if( node->branch.Next )
        fprintf( outFile, "\tNext: %s\n", ((C3HierNode*)node->branch.Next)->identifier );
    if( node->branch.Children )
    {
        fprintf( outFile, "\tFirst Child: %s\n", ((C3HierNode*)node->branch.Children)->identifier );

        next = (C3HierNode*)node->branch.Children;
        while( next )
        {
            C3PrintHierarchy( next, outFile );

            next = (C3HierNode*)next->branch.Next;
        }
    }
}
