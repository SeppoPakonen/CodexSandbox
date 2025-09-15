/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     animBank.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/anim/src/animBank.c $
    
    7     8/14/00 6:22p John
    Added user-defined data to ANM, ACT, and GPL formats.
    
    6     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <CharPipe/anim.h>
#include <CharPipe/actor.h>

#include <Dolphin/os.h>
#include <Dolphin/dvd.h>

/*>*******************************(*)*******************************<*/
void    ANIMGet     ( ANIMBankPtr *animBank, char *name )
{
    ANIMBankPtr     anmBank;
    ANIMSequencePtr seq;
    ANIMTrackPtr    track;
    ANIMKeyFramePtr frame;
    DVDFileInfo     dfi;
    u32             i;

    if(!DVDOpen(name, &dfi))
    {
        OSReport("ANIMGet: Could not find file %s", name );
        OSHalt("");
    }
    anmBank = (ANIMBankPtr)OSAlloc(OSRoundUp32B(dfi.length));
    DVDRead(&dfi, anmBank, (s32)OSRoundUp32B(dfi.length), 0);
    DVDClose(&dfi);

    if(anmBank->versionNumber != ANM_VERSION)
    {
        OSReport("ANIMGet: Incompatible version number %d for %s, since\n", anmBank->versionNumber, name);
        OSReport("         the library version number is %d.\n", ANM_VERSION );
        OSHalt("");
    }

    if(anmBank->userDataSize > 0 && anmBank->userData)
        anmBank->userData = (Ptr)((u32)anmBank->userData + (u32)anmBank);

    seq   = (ANIMSequencePtr)((u32)anmBank + sizeof(ANIMBank));
    track = (ANIMTrackPtr)((u32)seq + (anmBank->numSequences * sizeof(ANIMSequence)));
    frame = (ANIMKeyFramePtr)((u32)track + (anmBank->numTracks * sizeof(ANIMTrack)));

    //unpack bank
    anmBank->animSequences = (ANIMSequencePtr)((u32)(anmBank->animSequences) + (u32)anmBank);

    //unpack sequences
    for(i = 0; i < anmBank->numSequences; i ++)
    {
        seq[i].sequenceName = (char*)((u32)(seq[i].sequenceName) + (u32)anmBank);
        seq[i].trackArray = (ANIMTrackPtr)((u32)(seq[i].trackArray) + (u32)anmBank);
    }

    //unpack tracks
    for(i = 0; i < anmBank->numTracks; i ++)
    {
        track[i].keyFrames = (ANIMKeyFramePtr)((u32)(track[i].keyFrames) + (u32)anmBank);
    }

    //unpack keyFrames
    for(i = 0; i < anmBank->numKeyFrames; i ++)
    {
        frame[i].setting = (Ptr)((u32)(frame[i].setting) + (u32)anmBank);
        frame[i].interpolation = (Ptr)((u32)(frame[i].interpolation) + (u32)anmBank);
    }

    *animBank = anmBank;
}

/*>*******************************(*)*******************************<*/
void    ANIMRelease ( ANIMBankPtr *animBank )
{
    OSFree(*animBank);
    *animBank = 0;
}

/*>*******************************(*)*******************************<*/
ANIMSequencePtr ANIMGetSequence     ( ANIMBankPtr animBank, char *sequenceName, u16 seqNum )
{
    u32 i;

    if(sequenceName)
    {
        for(i = 0; i < animBank->numSequences; i ++)
        {
            if(!(Strcmp(animBank->animSequences[i].sequenceName, sequenceName)))
                return &(animBank->animSequences[i]);
        }
    }
    else
    {
        ASSERTMSG(seqNum < animBank->numSequences, "Specified sequence ID too large for the animBank");
        return &(animBank->animSequences[seqNum]);
    }

    return 0;   
}

/*>*******************************(*)*******************************<*/
ANIMTrackPtr    ANIMGetTrackFromSeq ( ANIMSequencePtr animSeq, u16 animTrackID )
{
    u32 i;

    for(i = 0; i < animSeq->totalTracks; i ++)
    {
        if(animTrackID == (animSeq->trackArray)[i].trackID)
            return &((animSeq->trackArray)[i]);
    }

    return 0;
}

/*>*******************************(*)*******************************<*/
void    ANIMGetKeyFrameFromTrack ( ANIMTrackPtr animTrack, float time, ANIMKeyFramePtr *currentFrame, ANIMKeyFramePtr *nextFrame )
{
    u16 i;

    for(i = 0; i < animTrack->totalFrames; i++)
    {
        if(animTrack->keyFrames[i].time <= time)
        {
            if(i < animTrack->totalFrames - 1)
            {
                if(animTrack->keyFrames[i + 1].time > time)
                {
                    if(currentFrame) *currentFrame = &(animTrack->keyFrames[i]);
                    if(nextFrame) *nextFrame = &(animTrack->keyFrames[i + 1]);
                    return;
                }
            }
            else
            {
                if(currentFrame) *currentFrame = &(animTrack->keyFrames[i]);
                if(nextFrame) *nextFrame = &(animTrack->keyFrames[0]);
                return;
            }
        }
    }

    ASSERTMSG(0, "Keyframes not found");
}

/*>*******************************(*)*******************************<*/
u32 ANIMGetUserDataSize ( ANIMBankPtr animBank )
{
    ASSERT( animBank );
    return animBank->userDataSize;
}

/*>*******************************(*)*******************************<*/
Ptr ANIMGetUserData     ( ANIMBankPtr animBank )
{
    ASSERT( animBank );
    return animBank->userData;
}
