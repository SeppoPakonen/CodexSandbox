/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     boneAnim.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/actor/src/boneAnim.c $
    
    5     11/13/00 1:21p John
    Modified so that animation can replace hierarchy transformation (with
    an option enabled in the track).
    Hierarchy transformation is no longer a mtx in the layout, but a
    CTRLControl structure of translation, rotation, and scale. (Unanimated
    hierarchy information must be moved into animation control if
    animating).
    
    4     8/14/00 6:22p John
    Added user-defined data to ANM, ACT, and GPL formats.
    
    3     8/04/00 5:04p John
    Renamed animationControl to animationCtrl.
    
    2     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <CharPipe/actor.h>
#include <CharPipe/control.h>
#include <Dolphin/os.h>

#include "boneAnim.h"

/*>*******************************(*)*******************************<*/
void ACTSetBoneTime          ( ACTBonePtr bone, float time )
{
    ANIMSetTime(bone->animPipe, time);
}

/*>*******************************(*)*******************************<*/
void ACTSetBoneSpeed     ( ACTBonePtr bone, float speed )
{
    ANIMSetSpeed(bone->animPipe, speed);
}

/*>*******************************(*)*******************************<*/
void ACTTickBone         ( ACTBonePtr bone )
{
    ANIMTick(bone->animPipe);
}

/*>*******************************(*)*******************************<*/
void    ACTSetBoneTrack     ( ACTBonePtr bone, ANIMTrackPtr track, float time)
{
    u8   hierControlType;
    Qtrn param;

    if(!track)
        return;

    if(!bone->animPipe)
    {
        bone->animPipe = OSAlloc(sizeof(ANIMPipe));
        bone->animPipe->time = 0.0F;
        bone->animPipe->speed = 1.0F;
        bone->animPipe->currentTrack = NULL;
        bone->animPipe->control = NULL;
        bone->animPipe->replaceHierarchyCtrl = 0;
    }

    ANIMBind(bone->animPipe, &bone->animationCtrl, track, time);

    // Add in unanimated hierarchy information into animation control,
    // but hierarchy control cannot be of a matrix type
    hierControlType = CTRLGetType( &bone->orientationCtrl );

    if( hierControlType & CTRL_TRANS && !(track->animType & ANIM_TRANS))
    {
        CTRLGetTranslation( &bone->orientationCtrl, &param.x, &param.y, &param.z );
        CTRLSetTranslation( &bone->animationCtrl, param.x, param.y, param.z );
    }

    if( hierControlType & CTRL_SCALE && !(track->animType & ANIM_SCALE))
    {
        CTRLGetScale( &bone->orientationCtrl, &param.x, &param.y, &param.z );
        CTRLSetScale( &bone->animationCtrl, param.x, param.y, param.z );
    }

    if( hierControlType & CTRL_ROT_EULER && !(track->animType & ANIM_ROT))
    {
        CTRLGetRotation( &bone->orientationCtrl, &param.x, &param.y, &param.z );
        CTRLSetRotation( &bone->animationCtrl, param.x, param.y, param.z );
    }
    else if( hierControlType & CTRL_ROT_QUAT && !(track->animType & ANIM_QUAT))
    {
        CTRLGetQuat( &bone->orientationCtrl, &param.x, &param.y, &param.z, &param.w );
        CTRLSetQuat( &bone->animationCtrl, param.x, param.y, param.z, param.w );
    }
}

