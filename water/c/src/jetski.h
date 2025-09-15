/**********************************************************
 * Copyright (C) 1999,2000 TheyerGFX
 * All Rights Reserved
 **********************************************************
 * Project:	Surf game
 **********************************************************
 * File:	jetski.h
 * Author:	Mark Theyer
 * Created:	06 Sep 1999
 **********************************************************
 * Description:	Header file for jetski functions.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 06-Sep-99	Theyer	Initial coding from existing game.
 *
 **********************************************************/

#ifndef SURF_JETSKI_H
#define SURF_JETSKI_H

/* 
 * includes
 */

#include <compile.h>
#include <type/datatype.h>
#include <gfx/gfx.h>

/* 
 * macros 
 */

/* settings */
#define SURF_JETSKI_ROLL_LIMIT				 70.0f	
#define SURF_JETSKI_SLOWDOWN				  0.0293f
/* fastest speed limit */
#define SURF_JETSKI_TERMINAL_VELOCITY		 60.0f			// 40.0f	
/* speed settings */
#define SURF_JETSKI_AIRTIME_SPEED			 10.0f
#define SURF_JETSKI_SLOW_SPEED				  5.0f
#define SURF_JETSKI_IDLE_SPEED				  3.0f
#define SURF_JETSKI_GLUE_SPEED				 20.0f
/* thrust values */
#define SURF_JETSKI_IDLE_THRUST				  2.0f
#define SURF_JETSKI_HALF_THROTTLE_THRUST	 30.0f			// 15.0f
#define SURF_JETSKI_FULL_THROTTLE_THRUST	 60.0f			// 30.0f
#define SURF_JETSKI_THRUST_RATE				  0.01666667f	//  0.03333333f	// 1.0f/(SURF_JETSKI_FULL_THROTTLE_THRUST)
#define SURF_JETSKI_TURN_FRICTION			  0.5f			// turn slows when going faster by this factor
/* rates */
#define SURF_JETSKI_STRAIGHTENRATE			  5.0f
#define SURF_JETSKI_SLOWTURN				  1.0f
#define SURF_JETSKI_TURNRATE				  4.0f
#define SURF_JETSKI_RICKTA_RATE				  4.0f
#define SURF_JETSKI_PITCHRATE				  6.0f
#define SURF_JETSKI_PITCHLAG				  2.0f

/* states */
#define JETSKI_STATE_BEHIND_IN_WATER		1
#define JETSKI_STATE_ON_TUMMY				2
#define JETSKI_STATE_AS_PER_RIDER			3

#define JETSKI_STATE_START					JETSKI_STATE_BEHIND_IN_WATER
#define JETSKI_STATE_READY					JETSKI_STATE_ON_TUMMY

#if 0 // same as pilot so don't need these...
/* jetski positions */
#define JETSKI_POS_BEHIND_IN_WATER		  0
#define JETSKI_POS_CLIMB_ON_1			  1
#define JETSKI_POS_CLIMB_ON_2			  2
#define JETSKI_POS_CLIMB_ON_3			  3
#define JETSKI_POS_CLIMB_ON_4			  4
#define JETSKI_POS_CLIMB_ON_5			  5
#define JETSKI_POS_CLIMB_ON_6			  6
#define JETSKI_POS_CLIMB_ON_7			  7
#define JETSKI_POS_CLIMB_ON_8			  8
#define JETSKI_POS_CLIMB_ON_9			  9
#define JETSKI_POS_CLIMB_ON_10			 10
#define JETSKI_POS_CLIMB_ON_11			 11
#define JETSKI_POS_CLIMB_ON_12			 12
#define JETSKI_POS_ON_TUMMY				 13
#endif

/*
 * typedefs
 */

typedef struct {
   	GfxObject			*ski;			/* ski object			*/
	GfxObject			*pole;			/* pole object			*/
	GfxObject			*bars;			/* handle bars object	*/
	GfxObject			*jet;			/* jet object			*/
    int					 current;		/* position index		*/
    int					 last;			/* last position		*/
    int					 state;			/* current state		*/                               	
	int					 target;		/* airtime spin angle	*/
	float				 move_speed;	/* position move speed	*/
	float				 interpol;		/* current interpolation*/
	float				 spin;			/* airtime spin angle	*/
	float				 roll;			/* airtime roll value	*/
	float				 pitch;			/* airtime pitch value  */
	float				 wet;			/* jet water (0=dry)    */
	float				 pitchrate;		/* saved pitch rate     */
	float				 turnrate;		/* saved turn rate      */
} SurfJetSki;

/*
 * prototypes
 */

/* jetski.c */
extern void surfResetJetSki( SurfJetSki *jetski );
extern void surfLoadJetSki( SurfJetSki *jetski, int num, int player );
extern void surfJetSkiSetThrust( SurfVehicle *veh, float thrust );
extern void surfDrawJetSki( SurfJetSki *jetski );
extern void surfJetSkiUpdate( SurfJetSki *jetski, SurfVehicle *veh, PersonInfo *rider );
extern void surfJetSkiNextPosition( SurfJetSki *jetski );
extern Bool surfJetSkiInPosition( SurfJetSki *jetski );
//#define surfJetSkiInPosition(ski)		(((ski)->current == (ski)->last) && ((ski)->interpol == 0.0f))

#endif	// SURF_JETSKI_H

