/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     animPipe.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/charPipeline/anim/animPipe.h $
    
    5     11/13/00 1:58p John
    Modified so that animation can replace hierarchy transformation (with
    an option enabled in the track).
    
    4     8/14/00 6:23p John
    Added user-defined data to ANM, ACT, and GPL formats.
    
    3     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
    2     12/08/99 2:49p Ryan
    added function descriptions.
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  animPipe provides a binding between the data from an animation bank and the 
  control structure the character pipeline uses to generate matrices.
 *---------------------------------------------------------------------------*/

#ifndef ANIMPIPE_H
#define ANIMPIPE_H

/********************************/
#if 0
extern "C" {
#endif

/********************************/
#include <CharPipe/control.h>

/********************************/
typedef struct ANIMPipe
{
	float			time;
	float			speed;

	ANIMTrackPtr	currentTrack;

	CTRLControlPtr	control;

    u8              replaceHierarchyCtrl;

}ANIMPipe, *ANIMPipePtr;

/*>*******************************(*)*******************************<*/
/*---------------------------------------------------------------------------*
  ANIMBind - Binds the given animation pipe to the given control using the 
  specified track at the given time.  In other words, after a call to this 
  function the control will be animated by the animation pipe through the 
  animation track starting at the given time.
 *---------------------------------------------------------------------------*/
void	ANIMBind		( ANIMPipePtr animPipe, CTRLControlPtr control, 
						  ANIMTrackPtr animTrack, float time );

/*---------------------------------------------------------------------------*
  ANIMSetTime - Sets the time in an animation track at which the animation 
  pipe currently exists.
 *---------------------------------------------------------------------------*/
void	ANIMSetTime		( ANIMPipePtr animPipe, float time );

/*---------------------------------------------------------------------------*
  ANIMSetSpeed - Sets the speed of the animation being played through the 
  animation pipe.
 *---------------------------------------------------------------------------*/
void	ANIMSetSpeed	( ANIMPipePtr animPipe, float speed );

/*---------------------------------------------------------------------------*
  ANIMTick - Causes the animation pipe to animate its corresponding control
  to the next frame in the animation.
 *---------------------------------------------------------------------------*/
void	ANIMTick		( ANIMPipePtr animPipe );	

/********************************/
#if 0
}
#endif

#endif