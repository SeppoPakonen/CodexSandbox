/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3HieExt.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/charPipeline/c3/C3HieExt.h $
    
    14    11/13/00 1:59p John
    Hierarchy transformation is no longer a mtx in the layout, but a
    CTRLControl structure of translation, rotation, and scale.
    Added C3SetPivotOffset API so that rotation and scale pivot points do
    not have to be the same as the translation.
    
    13    8/14/00 6:14p John
    Added user-defined data to ANM, ACT, and GPL formats.
    
    12    7/18/00 7:36p John
    Modified to use new CTRL library.
    
    11    4/06/00 3:22p John
    Need to use C3SetParent for re-parenting of biped tails.  See
    ProcessINode and ConvertINodeKeyedAnimationRotation.
    
    10    3/14/00 1:32p John
    Moved display priority from GPL to ACT.
    
    9     2/18/00 4:53p John
    Cleaned up code.
    
    8     2/15/00 8:07p John
    Changed version date to omit preceding 0 so that it is not confused as
    an octal number.
    
    7     2/04/00 6:06p John
    Untabified code.
    
    6     1/31/00 4:15p John
    Added pruning of unnecessary bones and animations.
    Finished stitching with FIFO matrix cache and stripping.
    
    5     1/20/00 1:13p John
    Added stitching (work in progress).
    
    4     12/15/99 1:19p John
    Added ACT version information.
    
    3     12/08/99 7:24p John
    Commented out inheritance code (not fully supported).
    
    2     12/06/99 3:22p John
    Commented out unused/unsupported functions.
    
    8     11/11/99 1:10p John
    Moved C3Control structure here from C3HieExtPrivate.h
    
    7     10/13/99 2:59p John
    Added instancing.
    
    6     9/29/99 4:24p John
    Changed header to make them all uniform with logs.

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _C3HIEEXT_H
#define _C3HIEEXT_H

#if 0
extern "C" {
#endif

/********************************/

#define C3_ACT_VERSION              8092000 // Must be compatible with Previewer

#define C3_INHERIT_ALL              0x01
#define C3_INHERIT_SCALE            0x02    // Currently, only C3_INHERIT_ALL is supported   
#define C3_INHERIT_ROTATION         0x04    // ...
#define C3_INHERIT_TRANSLATION      0x08    // ...

/********************************/

typedef struct
{
  DSLink        link;
  u32           index;

  CTRLControl   c;

} C3Control, *C3ControlPtr;

/********************************/

void C3BeginHierarchyNode   ( char* name );
void C3EndHierarchyNode     ( );

void C3SetParent            ( char* name );
//void C3AddChild           ( char* name ); // Use hierarchical extraction instead
//void C3AddSibling         ( char* name );

void C3SetObject            ( char* identifier );
void C3SetObjectSkin        ( char* identifier );

//void C3SetMatrix            ( MtxPtr matrix );
void C3SetHierControl       ( u8 controlType, f32 x, f32 y, f32 z, f32 w );
void C3SetPivotOffset       ( f32 x, f32 y, f32 z );

void C3SetDisplayPriority   ( u8 priority );

//void C3SetObjectInNode    ( char* geomIdentifier, char* hierIdentifier );
void C3SetACTUserData       ( u32 size, void *data );
void C3GetACTUserData       ( u32 *size, void **data );

#if 0
}
#endif


#endif // _C3HIEEXT_H
