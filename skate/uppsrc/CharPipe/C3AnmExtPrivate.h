/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3AnmExtPrivate.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/build/charPipeline/c3/include/C3AnmExtPrivate.h $
    
    7     11/13/00 1:25p John
    Modified so that animation can replace hierarchy transformation (with
    an option enabled in the track).
    Removed C3SetTrackTransform function.
    
    6     7/18/00 7:23p John
    Modified to use new CTRL library.
    
    5     2/29/00 7:14p John
    Sorts keyframes by time.
    
    4     2/04/00 6:05p John
    Untabified code.
    
    3     1/31/00 4:14p John
    Added pruning of unnecessary bones and animations.
    Finished stitching with FIFO matrix cache and stripping.
    
    2     12/08/99 7:21p John
    Added animation quantization.
    
    6     11/11/99 1:06p John
    New animation format.  Added linear, bezier, slerp, and squad
    extraction.
    
    5     10/26/99 2:44p John
    Adding animation (work in progress).
    
    4     9/29/99 4:17p John
    Changed header to make them all uniform with logs.
    
  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _C3ANMEXTPRIVATE_H
#define _C3ANMEXTPRIVATE_H

#if 0
extern "C" {
#endif // __cplusplus

#define C3_INTERPTYPE_MASK_TRANS    0x03    // Proper order for retrieving interpolation type
#define C3_INTERPTYPE_MASK_SCALE    0x03    // is to bit-shift first, then use mask.
#define C3_INTERPTYPE_MASK_ROT      0x07
#define C3_INTERPTYPE_SHIFT_TRANS   0
#define C3_INTERPTYPE_SHIFT_SCALE   2
#define C3_INTERPTYPE_SHIFT_ROT     4

#define C3_INTERPTYPE_TRANS(type)   ((type >> C3_INTERPTYPE_SHIFT_TRANS) & C3_INTERPTYPE_MASK_TRANS)
#define C3_INTERPTYPE_SCALE(type)   ((type >> C3_INTERPTYPE_SHIFT_SCALE) & C3_INTERPTYPE_MASK_SCALE)
#define C3_INTERPTYPE_ROT(type)     ((type >> C3_INTERPTYPE_SHIFT_ROT)   & C3_INTERPTYPE_MASK_ROT)

#define C3_QUATERNION_QUANTINFO     0x3E //C3_MAKE_QUANT( GX_S16, 14 )
#define C3_QUATERNION_SIZE          2
#define C3_EASE_QUANTINFO           0x3E //C3_MAKE_QUANT( GX_S16, 14 )
#define C3_EASE_SIZE                2

/********************************/

typedef struct
{
    Quaternion  inControl;      // Quaternion can be used as a Vec structure as well
    Quaternion  outControl;     // for Bezier/Hermite control points/tangents.
    f32         inEase;         
    f32         outEase;

} C3ControlInterp;

/********************************/

typedef struct
{
    DSLink          link;
    u32             index;
  
    f32             time;
    CTRLControl     control;
    C3ControlInterp controlInterpTrans;
    C3ControlInterp controlInterpScale; 
    C3ControlInterp controlInterpRot; 

} C3KeyFrame, *C3KeyFramePtr;

/********************************/

typedef struct
{
    DSLink          link;
    u32             index;

    char*           owner;      // name of the owner of the track (for the id)
    u16             numKeys;    // number of keys
    DSList          keyList;    // keyframe list
    u16             numUniqueKeys;

    f32             startTime;
    f32             endTime;

    u8              paramQuantizeInfo;  // Quantization info for Keyframe settings (ignore shift for quats)
    u8              animType;           // Combination of Trans, Scale, (Quat or Euler), or Matrix
    u8              interpolationType;  // Combination of Linear, Bezier, Hermite, Squad, SquadEE, Slerp
    u8              replaceHierarchyCtrl; // At runtime, should animation replace hierarchy control or be relative to it

    C3Bool          sortKeyFramesNeeded;

} C3Track, *C3TrackPtr;

/********************************/

typedef struct 
{
    DSLink      link;
    u32         index;

    C3Track*    track;

} C3SeqTrack, *C3SeqTrackPtr;

/********************************/

typedef struct
{
    DSLink      link;

    C3String    name;

    u16         numTracks;
    DSList      trackList;  // C3SeqTrack

} C3Sequence, *C3SequencePtr;

/********************************/

typedef struct
{
    DSList  sequenceList;
    DSList  trackList;

    u16     id;
  
    u16     numSequences;
    u16     numTracks;
    u16     numKeyframes;
    u16     numUniqueKeys; 

} C3AnimBank, *C3AnimBankPtr;

/********************************/

typedef struct
{
    C3KeyFrame* keyFrame;
    C3Track*    track;
    C3Sequence* sequence;
    C3AnimBank* bank;

    char*       owner;

    C3Actor*    actor;

} C3AnimationState, *C3AnimationStatePtr;

/********************************/

void    C3InitAnimationExtraction ( void );
void    C3CleanAnimationExtraction( void );
void    C3SetCurrentAnim( C3AnimationState* currAnim );
void    C3RestoreCurrentAnim( void );

void    C3SortKeyFrames( C3Track *track );
void    C3ComputeTrackWithTransform( C3Track *track );
void    C3ComputeTrackInOutControl( u8 controlType, u8 interpType, C3Track* track );

#if 0
}
#endif // __cplusplus

#endif  // _C3ANMEXTPRIVATE_H
