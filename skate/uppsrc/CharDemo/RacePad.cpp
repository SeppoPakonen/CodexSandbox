/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     racePad.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/charPipeline/src/racePad.c $
    
    2     8/05/00 8:42a Ryan
    20-car update
    
    1     7/29/00 11:17a Ryan
    initial checkin
 
  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <charPipeline.h>
#include <demo.h>

#include "racePad.h"

/****************************************************************************/

static PADStatus   JoyPS[PAD_MAX_CONTROLLERS]; // Game pad state
static PADStatus   OldJoyPS[PAD_MAX_CONTROLLERS]; // Game pad state

/*---------------------------------------------------------------------------*
    Name:			JoyInit
    
    Description:	Initialize the joystick.
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
void JoyInit ( void )
{      
	JoyPS[0].button = 0;
}

/*---------------------------------------------------------------------------*
    Name:			JoyTick
    
    Description:	Reads the current state of the joystick.
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
void JoyTick ( void )
{
	OldJoyPS[0] = JoyPS[0];
    PADRead(JoyPS);	// Read the controller pad
}

/*---------------------------------------------------------------------------*
    Name:			JoyReadButtons
    
    Description:	Returns the current joystick button mask.
    				
    Arguments:		none
    
    Returns:		The current joystick button mask.
 *---------------------------------------------------------------------------*/
u16 JoyReadButtons ( void )
{
	u16 temp = JoyPS[0].button;

	if(OldJoyPS[0].button & PAD_BUTTON_X)
		temp &= (0xFFFF ^ PAD_BUTTON_X);

	if(OldJoyPS[0].button & PAD_BUTTON_B)
		temp &= (0xFFFF ^ PAD_BUTTON_B);

    if(OldJoyPS[0].button & PAD_BUTTON_A)
		temp &= (0xFFFF ^ PAD_BUTTON_A);

    if(OldJoyPS[0].button & PAD_BUTTON_Y)
		temp &= (0xFFFF ^ PAD_BUTTON_Y);

    return temp;
}

/*---------------------------------------------------------------------------*
    Name:			JoyReadStickX
    
    Description:	Returns the current joystick X value.
    				
    Arguments:		none
    
    Returns:		The current joystick X value.
 *---------------------------------------------------------------------------*/
s8 JoyReadStickX ( void )
{      
    return JoyPS[0].stickX;
}

/*---------------------------------------------------------------------------*
    Name:			JoyReadStickY
    
    Description:	Returns the current joystick Y value.
    				
    Arguments:		none
    
    Returns:		The current joystick Y value.
 *---------------------------------------------------------------------------*/
s8 JoyReadStickY ( void )
{      
    return JoyPS[0].stickY;
}

/*---------------------------------------------------------------------------*
    Name:			JoyReadSubstickX
    
    Description:	Returns the current sub-joystick X value.
    				
    Arguments:		none
    
    Returns:		The current sub-joystick X value.
 *---------------------------------------------------------------------------*/
s8 JoyReadSubstickX ( void )
{      
    return JoyPS[0].substickX;
}

/*---------------------------------------------------------------------------*
    Name:			JoyReadSubstickY
    
    Description:	Returns the current sub-joystick Y value.
    				
    Arguments:		none
    
    Returns:		The current sub-joystick Y value.
 *---------------------------------------------------------------------------*/
s8 JoyReadSubstickY ( void )
{      
    return JoyPS[0].substickY;
}

/*---------------------------------------------------------------------------*
    Name:			JoyReadTriggerL
    
    Description:	Returns the current left trigger value.
    				
    Arguments:		none
    
    Returns:		The current left trigger value.
 *---------------------------------------------------------------------------*/
u8 JoyReadTriggerL ( void )
{      
//	if(OldJoyPS[0].triggerLeft > 170)
//		return 0;

    return JoyPS[0].triggerLeft;
}

/*---------------------------------------------------------------------------*
    Name:			JoyReadTriggerR
    
    Description:	Returns the current right trigger value.
    				
    Arguments:		none
    
    Returns:		The current right trigger value.
 *---------------------------------------------------------------------------*/
u8 JoyReadTriggerR ( void )
{      
	if(OldJoyPS[0].triggerRight > 170)
		return 0;

    return JoyPS[0].triggerRight;
}

