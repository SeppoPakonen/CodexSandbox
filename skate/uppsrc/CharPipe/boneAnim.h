/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     boneAnim.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/actor/include/boneAnim.h $
    
    1     3/23/00 9:56a Ryan
    
    2     12/08/99 12:54p Ryan
    added function descriptions

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  The purpose of boneAnim is to reflect the features of actorAnim at the 
  individual bone level.
 *---------------------------------------------------------------------------*/

#ifndef BONEANIM_H
#define BONEANIM_H

/********************************/
#if 0
extern "C" {
#endif

/*>*******************************(*)*******************************<*/
/*---------------------------------------------------------------------------*
  ACTSetBoneTime - Sets the time in an animation track at which the bone 
  currently exists.
 *---------------------------------------------------------------------------*/
void	ACTSetBoneTime			( ACTBonePtr bone, float time );

/*---------------------------------------------------------------------------*
  ACTSetBoneSpeed - Sets the playback speed for the specified bone's animation.
 *---------------------------------------------------------------------------*/
void	ACTSetBoneSpeed			( ACTBonePtr bone, float speed );

/*---------------------------------------------------------------------------*
  ACTTickBone - Advances bone to the next frame of animation.
 *---------------------------------------------------------------------------*/
void	ACTTickBone				( ACTBonePtr bone );

/*---------------------------------------------------------------------------*
  ACTSetBoneTrack - Binds the specified bone to the requested animation 
  track at the given track time.
 *---------------------------------------------------------------------------*/
void	ACTSetBoneTrack			( ACTBonePtr bone, ANIMTrackPtr track, float time);

/********************************/
#if 0
}
#endif

#endif