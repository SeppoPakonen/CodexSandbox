/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     racePad.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/charPipeline/include/racePad.h $
    
    1     7/29/00 11:18a Ryan

  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#ifndef __RACEPAD_H__
#define __RACEPAD_H__

#if 0
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
void	JoyInit 			( void );
void	JoyTick 			( void );
u16 	JoyReadButtons 		( void );
s8 	    JoyReadStickX 		( void );
s8 	    JoyReadStickY 		( void );
s8 	    JoyReadSubstickX 	( void );
s8 	    JoyReadSubstickY 	( void );
u8      JoyReadTriggerL		( void );
u8      JoyReadTriggerR		( void );
/*---------------------------------------------------------------------------*/
#if 0
}
#endif

#endif