/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3HieExtPrivate.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/c3/include/C3HieExtPrivate.h $
    
    8     11/13/00 1:28p John
    Added pivot point offsets for rotation and scale since they may not
    necessarily be the same as the translation.
    
    7     3/14/00 1:31p John
    Moved display priority from GPL to ACT.
    
    6     2/29/00 7:15p John
    Removed unnecesary geomObject data field.
    
    5     2/04/00 6:05p John
    Untabified code.
    
    4     1/31/00 4:15p John
    Added pruning of unnecessary bones and animations.
    Finished stitching with FIFO matrix cache and stripping.
    
    3     1/20/00 1:12p John
    Added stitching (work in progress).
    
    2     12/06/99 3:20p John
    Commented out unused/unsupported functions.
    
    5     11/11/99 1:08p John
    Moved C3Control structure out.
    
    4     7/01/99 3:25p Rod
    Moved the Initialization and clean up routine from the public header to
    the private header
    
    3     6/02/99 11:46a Rod
    Changed NQuat to Quaternion

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _C3HIEEXTPRIVATE_H
#define _C3HIEEXTPRIVATE_H

#if 0
extern "C" {
#endif

/********************************/

typedef struct 
{
    DSLink          link;     // used for the stack and conversion to array
    u16             index;    // used for the array conversion on output

    u8              displayPriority;
    u8              inheritanceFlag;

    DSBranch        branch;

    char*           identifier;

    C3ControlPtr    control;

    char*           geomObjName;
    u32             geomIdOffset; // for output

    Vec             pivotOffset;

    u16             id;

    C3Bool          usedForStitching;


} C3HierNode, *C3HierNodePtr;

/********************************/

typedef struct
{
    DSLink  link;

    char*   identifier;

    u16     numBones;
    DSList  boneList;

    u16     numControls;
    DSList  controlList;

    Ptr     animBank;

    DSTree  hierarchy;

    Ptr     boundingSphere;

    char*   skinMeshIdentifier;
    u32     skinIdOffset;

} C3Actor, *C3ActorPtr;


/*>*******************************(*)*******************************<*/

void            C3InitHierarchyExtraction ( void );
void            C3CleanHierarchyExtraction( void );

C3ActorPtr      C3GetActor( void );
C3HierNodePtr   C3GetHierNodeFromIdentifier( char *identifier );
C3HierNodePtr   C3GetCurrentHierNode( void );

//void          C3NameActor( C3ActorPtr actor, char* name );

#if 0
}
#endif


#endif // C3HIEEXTPRIVATE_H
