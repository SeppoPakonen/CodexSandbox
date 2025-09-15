/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     animBank.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/charPipeline/anim/animBank.h $
    
    6     11/13/00 1:57p John
    Modified so that animation can replace hierarchy transformation (with
    an option enabled in the track).
    
    5     8/14/00 6:23p John
    Added user-defined data to ANM, ACT, and GPL formats.
    
    4     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
    3     12/15/99 12:04p Ryan
    Added version number and checking
    
    2     12/08/99 2:49p Ryan
    added function descriptions.
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  The purpose of animBank is to provide an interface between the character 
  pipeline and .anm files on disk.  
 *---------------------------------------------------------------------------*/

#ifndef ANIMBANK_H
#define ANIMBANK_H

/********************************/
#if 0
extern "C" {
#endif

/********************************/
#include <Dolphin/types.h>

/********************************/
#define ANM_VERSION     8092000   
    
// Types of animation
#define ANIM_TRANS      0x01
#define ANIM_SCALE      0x02
#define ANIM_ROT        0x04
#define ANIM_QUAT       0x08
#define ANIM_MTX        0x10

// Combinations of above
#define ANIM_ST         0x03
#define ANIM_RT         0x05
#define ANIM_RS         0x06
#define ANIM_SRT        0x07
#define ANIM_QT         0x09
#define ANIM_SQ         0x0A
#define ANIM_SQT        0x0B

// Types of interpolation
#define ANIM_NONE       0x00
#define ANIM_LINEAR     0x01
#define ANIM_BEZIER     0x02
#define ANIM_HERMITE    0x03
#define ANIM_SQUAD      0x04
#define ANIM_SQUADEE    0x05
#define ANIM_SLERP      0x06

/********************************/
typedef struct  
{
    float           time;
    Ptr             setting;
    Ptr             interpolation;
    
} ANIMKeyFrame, *ANIMKeyFramePtr;

/********************************/
typedef struct ANIMAnimTrack
{
    float           animTime;

    ANIMKeyFramePtr keyFrames;

    u16             totalFrames;
    u16             trackID;

    u8              quantizeInfo;
    u8              animType;
    u8              interpolationType;
    u8              replaceHierarchyCtrl;

} ANIMTrack, *ANIMTrackPtr;

/********************************/
typedef struct 
{
    char            *sequenceName;

    ANIMTrackPtr    trackArray;

    u16             totalTracks;    
    u16             pad16;
    
} ANIMSequence, *ANIMSequencePtr;

/********************************/
typedef struct  
{
    u32             versionNumber;

    ANIMSequencePtr animSequences;  

    u16             bankID;
    u16             numSequences;
    u16             numTracks;
    u16             numKeyFrames;

    u32             userDataSize;
    void*           userData;

} ANIMBank, *ANIMBankPtr;

/*>*******************************(*)*******************************<*/
/*---------------------------------------------------------------------------*
  ANIMGet - Loads and unpacks the specified .anm file.  This system
  loads files from disk in a cached manner if the display cache has been 
  turned on with a call to the DOInitDisplayCache function.
 *---------------------------------------------------------------------------*/
void    ANIMGet     ( ANIMBankPtr *animBank, char *name );

/*---------------------------------------------------------------------------*
  ANIMRelease - If the display cache is initialized, this function will
  decrement the reference count of the specified animation bank and free its 
  memory if the reference count is 0.  If the display cache is not initialized, 
  the specified animation bank is simply freed.
 *---------------------------------------------------------------------------*/
void    ANIMRelease ( ANIMBankPtr *animBank );

/*---------------------------------------------------------------------------*
  ANIMGetKeyFrameFromTrack - Finds the two closest keyframes in a track given
  some time mark within the track.  The user may then interpolate between the 
  two tracks.
 *---------------------------------------------------------------------------*/
void    ANIMGetKeyFrameFromTrack ( ANIMTrackPtr animTrack, float time, 
                                   ANIMKeyFramePtr *currentFrame, ANIMKeyFramePtr *nextFrame );

/*---------------------------------------------------------------------------*
  ANIMGetSequence - Returns a requested sequence from a given bank.
 *---------------------------------------------------------------------------*/
ANIMSequencePtr ANIMGetSequence     ( ANIMBankPtr animBank, char *sequenceName, u16 seqNum );

/*---------------------------------------------------------------------------*
  ANIMGetTrackFromSeq - Returns a requested track from a given sequence.
 *---------------------------------------------------------------------------*/
ANIMTrackPtr    ANIMGetTrackFromSeq ( ANIMSequencePtr animSeq, u16 animTrackID );

/*---------------------------------------------------------------------------*
  ANIMGetUserDataSize - Returns the size in bytes of the user-defined data.
 *---------------------------------------------------------------------------*/
u32     ANIMGetUserDataSize ( ANIMBankPtr animBank );

/*---------------------------------------------------------------------------*
  ANIMGetUserData - Returns a pointer to the user-defined data.
 *---------------------------------------------------------------------------*/
Ptr     ANIMGetUserData     ( ANIMBankPtr animBank );

/********************************/
#if 0
}
#endif

#endif