/**********************************************************
 * Copyright (C) 1999,2000 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:	PlayStation surf game
 **********************************************************
 * File:	surfer.h
 * Author:	Mark Theyer
 * Created:	15 Jul 1999
 **********************************************************
 * Description:	Header file for surfer functions.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game.
 *
 **********************************************************/

#ifndef SURF_SURFER_H
#define SURF_SURFER_H

/* 
 * includes
 */

#include <compile.h>
#include <type/datatype.h>
#include <gfx/gfx.h>
#include <vehicle.h>
#include <person.h>

/* 
 * macros 
 */

#define SURF_SURFER_WEIGHT				 75		

/* speed settings */
#define SURF_AIRTIME_SPEED				  8.0f		// 8
#define SURFER_STANDUP_SPEED			  4.0f		// 4
#define SURFER_LAYDOWN_SPEED			  3.0f		// 3
#define SURFER_DUCKDIVE_SUBMERSE_SPEED	  3.0f		// 3

/* thrust values */
//#define SURF_PADDLE_THRUST			  3
//#define SURF_DUCKDIVE_THRUST			  3

/* rates */
//#define SURF_PADDLE_TURNRATE		 	  2
#define SURF_SURFER_TURNRATE			  2.0f
#define SURF_SURFER_HARDTURNRATE		  4.0f
#define SURF_SURFER_HARDSPINRATE		  6.0f
#define SURF_SURFER_PITCHRATE			  2.0f
#define SURF_SURFER_STRAIGHTENRATE		 12.0f

#define SURF_SURFER_ROLL_LIMIT			 70.0f

/* surfer states */
#define SURFER_STATE_STANDING			  0 
#define SURFER_STATE_LAYONBOARD		      1
#define SURFER_STATE_DUCKDIVING			  2
#define SURFER_STATE_RIDING				  3
#define SURFER_STATE_SITTINGONBOARD		  4 
#define SURFER_STATE_SITTINGONBEACH	      5
#define SURFER_STATE_RUNNING		      6
#define SURFER_STATE_JUMPIN			      7
#define SURFER_STATE_TURNRIGHT		      8
#define SURFER_STATE_TURNLEFT		      9
#define SURFER_STATE_PADDLING		     10
#define SURFER_STATE_PADDLERIGHT	     11
#define SURFER_STATE_HARDTURNRIGHT		 12
#define SURFER_STATE_HARDTURNLEFT		 13
#define SURFER_STATE_LEANRIGHT			 14
#define SURFER_STATE_LEANLEFT			 15
#define SURFER_STATE_CROUCH				 16
#define SURFER_STATE_STALL				 17
#define SURFER_STATE_CRASH_LIP			 18
#define SURFER_STATE_TRICK_AIRWALK		 19

/* special position indicating we are not on the vehicle */
#define SURF_POS_NOT_ON_VEHICLE			 -1

/* surfer positions */
#define SURFER_POS_PADDLE0		  	  1
#define SURFER_POS_PADDLE1		  	  2
#define SURFER_POS_PADDLE2			  3
#define SURFER_POS_PADDLE3			  4                                          
#define SURFER_POS_PADDLE4			  5                                          
#define SURFER_POS_PADDLE5			  6                                          
#define SURFER_POS_STAND0		  	  7
#define SURFER_POS_STAND1		  	  8
#define SURFER_POS_STAND2		  	  9
#define SURFER_POS_STAND3		  	 10
#define SURFER_POS_STAND4		  	 11
#define SURFER_POS_STAND5		  	 12
#define SURFER_POS_RIDING			 13
#define SURFER_POS_TURNRIGHT		 14
#define SURFER_POS_TURNLEFT			 15
#define SURFER_POS_GR_R				 16
#define SURFER_POS_GR_R_TR			 17
#define SURFER_POS_GR_R_TL			 18
#define SURFER_POS_GR_L				 19
#define SURFER_POS_GR_L_TR			 20
#define SURFER_POS_GR_L_TL			 21
#define SURFER_POS_LAY_ON_BOARD0     22
#define SURFER_POS_LAY_ON_BOARD1     23
#define SURFER_POS_LAY_ON_BOARD2     24
#define SURFER_POS_GR_R_AIR			 25
#define SURFER_POS_GR_R_AIR_WALK	 26
#define SURFER_POS_LOW				 27
#define SURFER_POS_LOW_TR			 28
#define SURFER_POS_LOW_TL			 29
//#define NSURFERPOSITIONS			 30

/* surfer start and end positions for sequential actions */
#define SURFER_POS_PADDLE_START		  SURFER_POS_PADDLE0
#define SURFER_POS_PADDLE_END		  SURFER_POS_PADDLE5
#define SURFER_POS_STANDONBEACH_START SURFER_POS_SITONBEACH
#define SURFER_POS_STANDONBEACH_END	  SURFER_POS_STANDING

/* bit flags for extra information for surfer */
#define SURFER_EXTRA_LOWSTANCE		0x0001
#define SURFER_EXTRA_GRAB_LEFT		0x0002
#define SURFER_EXTRA_GRAB_RIGHT		0x0004
#define SURFER_EXTRA_GRAB_BOTH		0x0006

/*
 * typedefs
 */

/*
 * prototypes
 */

extern void			 surfResetSurfer( PersonInfo *surfer );
extern GfxObject	*surfLoadSurfer( PersonInfo *surfer, int num, int player );
extern void			 surfDrawSurfer( PersonInfo *surfer );
extern void			 surfSurferProcessKeyEvent( PersonInfo *surfer, int key, float analog );
extern void			 surfSurferNextPosition( PersonInfo *surfer );
extern void			 surfPlaySurfingProcess( PersonInfo *surfer );
extern void			 surfSurferSpin( SurfVehicle *veh, float rate );
extern void			 surfChangeSurferPitch( SurfVehicle *veh, float angle );

#endif	// SURF_SURFER_H

