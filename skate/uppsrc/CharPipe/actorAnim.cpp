/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     actorAnim.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/actor/src/actorAnim.c $
    
    2     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <CharPipe/actor.h>

#include "boneAnim.h"

/*>*******************************(*)*******************************<*/
void	ACTSetAnimation	( ACTActorPtr actor, ANIMBankPtr animBank, char *sequenceName, u16 seqNum, float time )
{
	u16 i;
	ANIMSequencePtr animSeq = ANIMGetSequence(animBank, sequenceName, seqNum);
	ANIMTrackPtr track;

	for(i = 0; i < actor->totalBones; i++)
	{
		track = ANIMGetTrackFromSeq(animSeq, actor->boneArray[i]->boneID);
		ACTSetBoneTrack(actor->boneArray[i], track, time);
	}
}

/*>*******************************(*)*******************************<*/
void	ACTSetTime		( ACTActorPtr actor, float time )
{
	u16 i;

	for(i = 0; i < actor->totalBones; i++)
		ACTSetBoneTime(actor->boneArray[i], time);
}

/*>*******************************(*)*******************************<*/
void	ACTSetSpeed		( ACTActorPtr actor, float speed )
{
	u16 i;

	for(i = 0; i < actor->totalBones; i++)
		ACTSetBoneSpeed(actor->boneArray[i], speed);
}

/*>*******************************(*)*******************************<*/
void	ACTTick			( ACTActorPtr actor )
{
	u16 i;

	for(i = 0; i < actor->totalBones; i++)
		ACTTickBone(actor->boneArray[i]);
}