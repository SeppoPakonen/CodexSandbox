/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:	pilot.h
 * Author:	Mark Theyer
 * Created:	15 Jul 1999
 **********************************************************
 * Description:	Header file for jetski pilot functions.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game.
 * 31-May-01	Theyer	New version for PS2
 *
 **********************************************************/

#ifndef SURF_PILOT_H
#define SURF_PILOT_H

/* 
 * includes
 */

#include <compile.h>
#include <type/datatype.h>
#include <gfx/gfx.h>
//#include <vehicle.h>
#include <person.h>

/* 
 * macros 
 */

/* pilot states */
#define PILOT_STATE_BEHIND_IN_WATER		  1
#define PILOT_STATE_IDLETHROTTLE		  2	  
#define PILOT_STATE_HALFTHROTTLE		  3  
#define PILOT_STATE_FULLTHROTTLE		  4
#define PILOT_STATE_TURNRIGHT			  5
#define PILOT_STATE_TURNLEFT			  6
#define PILOT_STATE_HARDTURNRIGHT		  7
#define PILOT_STATE_HARDTURNLEFT		  8
#define PILOT_STATE_SUBMARINE			  9
#define PILOT_STATE_CROUCH				 10
#define PILOT_STATE_BUNNYHOP			 11
#define PILOT_STATE_SUPERMAN			 12
#define PILOT_STATE_HEELCLICK			 13
#define PILOT_STATE_BARRELROLL			 14
#define PILOT_STATE_BARRELROLLNOHANDS	 15
#define PILOT_STATE_HANDPUNCHLEFT		 16
#define PILOT_STATE_HANDPUNCHRIGHT		 17
#define PILOT_STATE_HANDPUNCHBOTH		 18
#define PILOT_STATE_RICKTA				 19
#define PILOT_STATE_TABLETOPRIGHT		 20
#define PILOT_STATE_RIDING				 21
#define PILOT_STATE_CRASHING			 22
#define PILOT_STATE_LEANFORWARD			 23
#define PILOT_STATE_LEANRIGHT			 24
#define PILOT_STATE_LEANLEFT			 25
#define PILOT_STATE_CRASH_LIP			 26
#define PILOT_STATE_ON_TUMMY			 27

/* pilot states */
#define PILOT_STATE_START				  PILOT_STATE_BEHIND_IN_WATER
#define PILOT_STATE_READY				  PILOT_STATE_ON_TUMMY

/* special position indicating we are not on the vehicle */
#define SURF_POS_NOT_ON_VEHICLE			 -1

/* pilot positions */
#define PILOT_POS_BEHIND_IN_WATER		  0
#define PILOT_POS_CLIMB_ON_1			  1
#define PILOT_POS_CLIMB_ON_2			  2
#define PILOT_POS_CLIMB_ON_3			  3
#define PILOT_POS_CLIMB_ON_4			  4
#define PILOT_POS_ON_TUMMY				  5
#define PILOT_POS_STAND_UP_1			  6
#define PILOT_POS_STAND_UP_2			  7
#define PILOT_POS_STAND_UP_3			  8
#define PILOT_POS_STAND_UP_4			  9
#define PILOT_POS_STAND_UP_5			 10
#define PILOT_POS_STAND_UP_6			 11
#define PILOT_POS_STAND_UP_7			 12
#define PILOT_POS_RIDING				 13
#define PILOT_POS_RIDING_BUMP			 14
#define PILOT_POS_LEFT_TURN_READY		 15
#define PILOT_POS_LEFT_TURN_SLOW		 16
#define PILOT_POS_LEFT_TURN_FAST		 17
#define PILOT_POS_LEFT_TURN_FAST_BUMP	 18
#define PILOT_POS_BOTH_FEET_FORWARD		 19
#define PILOT_POS_RIDING_GOOFY			 20
#define PILOT_POS_RIGHT_TURN_READY		 21
#define PILOT_POS_RIGHT_TURN_SLOW		 22
#define PILOT_POS_RIGHT_TURN_FAST		 23
#define PILOT_POS_RIGHT_TURN_FAST_BUMP	 24

#if 0
// OLD STUFF
#define PILOT_POS_LYONPAD				  1
#define PILOT_POS_IDLETHROTTLE			  2
#define PILOT_POS_HALFTHROTTLE			  3
#define PILOT_POS_FULLTHROTTLE			  4
#define PILOT_POS_TURNRIGHT				  5
#define PILOT_POS_TURNLEFT				  6
#define PILOT_POS_HARDTURNRIGHT			  7
#define PILOT_POS_HARDTURNLEFT			  8
#define PILOT_POS_SWAPFEET				  9
#define PILOT_POS_GOOFY					 10
#define PILOT_POS_SUBMARINE				 11
#define PILOT_POS_RICKTA				 12
#define PILOT_POS_TABLETOPRIGHT			 13
#define PILOT_POS_CROUCH				 14
#define PILOT_POS_RIDING				 15
#define PILOT_POS_SUPERMANA				 16	
#define PILOT_POS_HEELCLICK              17
#define PILOT_POS_HANDPUNCHRIGHT		 18
#define PILOT_POS_HANDPUNCHLEFT			 19
#define PILOT_POS_HANDPUNCHBOTH			 20
#define PILOT_POS_BARRELROLL			 21
#define PILOT_POS_BARRELROLLNOHANDS		 22
#define PILOT_POS_SUPERMANB				 23	
#define PILOT_POS_SUPERMANC				 24	
#define PILOT_POS_RIDINGA				 25	
#define PILOT_POS_RIDINGB				 26	
#define PILOT_POS_RIDINGC				 27	
#define PILOT_POS_LEANFORWARD			 28
#define PILOT_POS_CRASH_LIP				 29
#define NPILOTPOSITIONS					 30
#endif

/*
 * typedefs
 */

/*
 * prototypes
 */

extern void			 surfResetPilot( PersonInfo *pilot );
extern GfxObject	*surfLoadPilot( PersonInfo *pilot, int num, int player );
extern void			 surfDrawPilot( PersonInfo *pilot );
extern void			 surfPlayJetSkiingProcess( PersonInfo *pilot );
extern void			 surfPilotProcessKeyEvent( PersonInfo *pilot, int key, float analog );
extern void			 surfPilotNextPosition( PersonInfo *pilot );

extern void surfPilotTurn( SurfVehicle *veh, float rate );
extern void surfPilotSpin( SurfVehicle *veh, float rate );
extern void surfChangePilotPitch( SurfVehicle *veh, float angle );
extern void surfChangePilotRoll( SurfVehicle *veh, float angle, float limit );

#endif	// SURF_PILOT_H

