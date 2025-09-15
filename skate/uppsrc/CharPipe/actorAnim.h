/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     actorAnim.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/charPipeline/actor/actorAnim.h $
    
    2     12/08/99 12:54p Ryan
    added function descriptions

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
  The purpose of actorAnim is to bind the actor structure to the animation
  library.
 *---------------------------------------------------------------------------*/

#ifndef ACTORANIM_H
#define ACTORANIM_H

/********************************/
#if 0
extern "C" {
#endif

/*>*******************************(*)*******************************<*/
/*---------------------------------------------------------------------------*
  ACTSetAnimation - Binds the specified actor to the requested animation 
  sequence at the given track time.
 *---------------------------------------------------------------------------*/
void	ACTSetAnimation		( ACTActorPtr actor, ANIMBankPtr animBank, 
							  char *sequenceName, u16 seqNum, float time );

/*---------------------------------------------------------------------------*
  ACTSetTime - Sets the time in an animation sequence at which the actor 
  currently exists.
 *---------------------------------------------------------------------------*/
void	ACTSetTime			( ACTActorPtr actor, float time );

/*---------------------------------------------------------------------------*
  ACTSetSpeed - Sets the playback speed for the specified actor's animation.
 *---------------------------------------------------------------------------*/
void	ACTSetSpeed			( ACTActorPtr actor, float speed );

/*---------------------------------------------------------------------------*
  ACTTick - Advances actor to the next frame of animation.  This function 
  should be called every frame to achieve smooth animation.
 *---------------------------------------------------------------------------*/
void	ACTTick				( ACTActorPtr actor );

/********************************/
#if 0
}
#endif

#endif
