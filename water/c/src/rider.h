/**********************************************************
 * Copyright (C) 1999,2000 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:	PlayStation surf game
 **********************************************************
 * File:	rider.h
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

#ifndef SURF_RIDER_H
#define SURF_RIDER_H

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

#define SURF_RIDER_WEIGHT					 75		

/* speed settings */
#define SURF_RIDER_AIRTIME_SPEED		  32768		// 8
#define SURF_RIDER_PLANING_SPEED		  16384		// 4
#define SURF_RIDER_PADDLE_SPEED			  12288		// 3
	
/* thrust values */
#define SURF_RIDER_PADDLE_THRUST			  3

/* rates */
#define SURF_RIDER_PADDLE_TURNRATE	 		  2
#define SURF_RIDER_TURNRATE					  2
#define SURF_RIDER_SPINRATE					  6
#define SURF_RIDER_HARDTURNRATE				  6
#define SURF_RIDER_PITCHRATE				  1
#define SURF_RIDER_STRAIGHTENRATE			 12

#define SURF_RIDER_ROLL_LIMIT				 70

/* rider states */
#define SURF_RIDER_STATE_LAYONBOARD		      1
#define SURF_RIDER_STATE_PADDLING		      2
#define SURF_RIDER_STATE_PADDLERIGHT	      3
#define SURF_RIDER_STATE_RIDING				  4
#define SURF_RIDER_STATE_TURNRIGHT		      5
#define SURF_RIDER_STATE_TURNLEFT		      6
#define SURF_RIDER_STATE_SPINRIGHT			  7
#define SURF_RIDER_STATE_SPINLEFT			  8
#define SURF_RIDER_STATE_LEANRIGHT			  9
#define SURF_RIDER_STATE_LEANLEFT			 10
#define SURF_RIDER_STATE_TABLETOP			 11
#define SURF_RIDER_STATE_BACKFLIP			 12
#define SURF_RIDER_STATE_STALL				 13
#define SURF_RIDER_STATE_CRASH_LIP			 14

/* special position indicating we are not on the vehicle */
#define SURF_RIDER_POS_NOT_ON_VEHICLE		 -1

/* rider positions */
#define SURF_RIDER_POS_LAYONBOARD			  0
#define SURF_RIDER_POS_PADDLE0			  	  1
#define SURF_RIDER_POS_PADDLE1			  	  2
#define SURF_RIDER_POS_PADDLE2				  3
#define SURF_RIDER_POS_PADDLE3				  4                                          
#define SURF_RIDER_POS_RIDING				  5
#define SURF_RIDER_POS_TURNRIGHT			  6
#define SURF_RIDER_POS_TURNLEFT				  7
#define SURF_RIDER_POS_SPINRIGHT			  8
#define SURF_RIDER_POS_SPINLEFT				  9
#define SURF_RIDER_POS_TABLETOP				 10
#define SURF_RIDER_POS_AIR					 11
#define SURF_RIDER_POS_AIRSPINRIGHT			 12
#define SURF_RIDER_POS_AIRSPINLEFT			 13
#define SURF_RIDER_POS_BACKFLIP				 14
#define SURF_RIDER_POS_STALL				 15
#define SURF_RIDER_POS_CRASH_LIP			 16
#define SURF_RIDER_NPOSITIONS				 17

/* rider start and end positions for sequential actions */
#define SURF_RIDER_POS_PADDLE_START		  SURF_RIDER_POS_PADDLE0
#define SURF_RIDER_POS_PADDLE_END		  SURF_RIDER_POS_PADDLE3

/*
 * typedefs
 */

/*
 * prototypes
 */

extern void surfInitRider( PersonInfo *rider );
extern void surfBuildRider( PersonInfo *rider, int num, int skin, int player );
extern void surfDrawRider( PersonInfo *rider );
extern int  surfRiderProcessKeyEvent( PersonInfo *rider, int key, fixed analog );
extern void surfRiderNextPosition( PersonInfo *rider );
extern int  surfPlayBodyBoardingProcess( PersonInfo *rider );
extern void surfRiderOutTheBack( PersonInfo *rider );

#endif	// SURF_RIDER_H


