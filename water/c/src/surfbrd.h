/**********************************************************
 * Copyright (C) 1999,2000 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:	PlayStation surf game
 **********************************************************
 * File:	surfbrd.h
 * Author:	Mark Theyer
 * Created:	15 Jul 1999
 **********************************************************
 * Description:	Header file for surfboard functions.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game.
 *
 **********************************************************/

#ifndef SURF_SURFBRD_H
#define SURF_SURFBRD_H

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

/* settings */
#define SURF_BOARD_ROLL_LIMIT				 70.0f
#define SURF_BOARD_SLOWDOWN					  0.01465f
/* fastest speed limit */
#define SURF_BOARD_TERMINAL_VELOCITY		 45.0f
/* speed settings */
#define SURF_BOARD_AIRTIME_SPEED			  8.0f
#define SURF_BOARD_STANDUP_SPEED			  4.0f
#define SURF_BOARD_PLANING_SPEED			  4.0f
#define SURF_BOARD_LAYDOWN_SPEED			  3.0f
#define SURF_BOARD_DUCKDIVE_SPEED			  2.0f
/* thrust values */
#define SURF_PADDLE_THRUST					  3.0f
#define SURF_BOARD_TURN_FRICTION			  0.5f			// turn slows when going faster by this factor
/* rates */
#define SURF_BOARD_STRAIGHTENRATE			  2.0f
#define SURF_PADDLE_TURNRATE		 		  2.0f
#define SURF_BOARD_TURNRATE					  4.0f
#define SURF_BOARD_PITCHRATE				  6.0f

/* surfboard states */
#define SURF_BOARD_STATE_AS_PER_RIDER			   1
#define SURF_BOARD_STATE_READY					   1
#define SURF_BOARD_STATE_START					   1

/*
 * typedefs
 */

typedef struct {
   	GfxObject			*object;		/* surfboard object		*/
	GfxObject			*fin[3];		/* fins					*/
    int					 current;		/* position index		*/
    int					 last;			/* last position		*/
    int					 state;			/* current state		*/                               	
	int					 target;		/* airtime spin angle	*/
	float				 move_speed;	/* position move speed	*/
	float				 interpol;		/* current interpolation*/
	float				 spin;			/* airtime spin angle	*/
	float				 roll;			/* airtime roll value	*/
	float				 pitch;			/* airtime pitch value  */
	//float				 wet;			/* jet water (0=dry)    */
	//float				 pitchrate;		/* saved pitch rate     */
	//float				 turnrate;		/* saved turn rate      */
} SurfBoard;

/*
 * prototypes
 */

extern void surfResetSurfboard( SurfBoard *board );
extern void surfLoadSurfboard( SurfBoard *board, int num, int player );
extern void surfDrawSurfboard( SurfBoard *board );
extern void surfBoardUpdate( SurfBoard *board, SurfVehicle *veh, PersonInfo *rider );
extern Bool surfBoardInPosition( SurfBoard *board );
extern void surfBoardNextPosition( SurfBoard *board );

#endif	// SURF_SURFBRD_H

