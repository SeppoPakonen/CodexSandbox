/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		pilot.c
 * Author:		Mark Theyer
 * Created:		6 Dec 1997
 **********************************************************
 * Description: jetski pilot related functions	
 **********************************************************
 * Revision History:
 * 06-Dec-97	Theyer	Initial Coding
 * 31-May-01	Theyer	New version for PS2
 **********************************************************/

/*
 * includes
 */

#include <surfgame.h>

/*
 * typedefs
 */

/*
 * macros
 */

/*
 * prototypes
 */

/*
 * variables
 */

extern SurfData    	surf;

/*
 * functions
 */


/**********************************************************
 * Function:	surfResetPilot
 **********************************************************
 * Description: Prepare to play with jetski pilot dude
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfResetPilot (
	PersonInfo	*pilot
	)
{
	pilot->state      = PILOT_STATE_START;
	pilot->target     = PILOT_STATE_READY;
	pilot->current    = PILOT_POS_BEHIND_IN_WATER;
	pilot->last       = PILOT_POS_BEHIND_IN_WATER;
	pilot->interpol   = 0.0f;
	pilot->move_speed = 0.2f;
	pilot->crash      = FALSE;
	pilot->onvehicle  = TRUE;
	gfxSetPosition( &pilot->object, 1, pilot->last, pilot->current, pilot->interpol );
	personPutOnVehicle( pilot, surfVehicleObject(pilot->vehicle) );
}


/**********************************************************
 * Function:	surfLoadPilot
 **********************************************************
 * Description: Load a jetski pilot dude
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

GfxObject	*surfLoadPilot (
    PersonInfo	*pilot,
	int			 num,
	int			 player
    )
{
	char		 buff[64];
	GfxObject	*object;

#ifdef SURF_USE_PAKS
	/* load package */
	gfxFilePath( "models\\riders" );
	sprintf( buff, "js%d.pak", num );
	gfxFileLoad( buff );
#endif

#if 0
	/* locate models */
	sprintf( buff, "models/riders/js%d", num );
	gfxFilePath( buff );

	/* load texture map locations */
	sprintf( buff, "js%dp%d.map", num, player );
	surfLoadMap( buff, FALSE );

    /* create model */
	sprintf( buff, "js%d", num );
#else
	/* locate models */
	sprintf( buff, "models/riders/s%d", num );
	gfxFilePath( buff );

	/* load texture map locations */
	sprintf( buff, "s%dp%d.map", num, player );
	surfLoadMap( buff, FALSE );

    /* create model */
	sprintf( buff, "s%d", num );
#endif 
	if ( ! p2mModelToGfxObject( buff, (GfxObject **)&object, NULL ) ) {
		printf( p2mModelError() );
		surfExit(SURF_ERROR_PILOT_LOAD_FAILED);
	}

	/* ok */
	return( object );
}


/**********************************************************
 * Function:	surfPilotTurn
 **********************************************************
 * Description: Update the roll for the pilot
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfPilotTurn (
    SurfVehicle	*veh,
	float		 rate
	)
{
	float		 limit;
	float		 slow;

	/* cancel trick turn */
	//if ( rate == 0.0f )
	//	surfTrickTurn( &surf.current->trick, 0, 0 );

#if 0
	/* adjust for faster turning at slower speeds */
	if ( veh->speed < veh->thrust )
		rate += rate * (1.0f - (veh->speed / veh->terminal_velocity));
#else
	/* adjust so turns are slower at high speeds */
	slow = (1.0f - (veh->speed * SURF_JETSKI_THRUST_RATE * SURF_JETSKI_TURN_FRICTION));
	//printf( "rate=%.2f,speed=%0.2f,adjust=%.2f,result=%.2f\n", rate, veh->speed, limit, (rate * limit) );
	rate *= slow;
#endif

	/* limit only in the water */
	limit = SURF_JETSKI_ROLL_LIMIT * slow;
	if ( veh->state == VEHICLE_STATE_AIRTIME )
		limit = 0.0f;

	/* 
	 * Behaviour is: 
	 *		- straighten quick (back to roll == 0)
	 *		- roll to 30 degrees at normal rate and...
	 *		- from 30 to 60 degrees at half the normal rate.
	 */
	if ( rate < 0.0f ) {
		if ( veh->roll > 0.0f ) {
			rate *= 2.0f;
			surfChangeVehicleRoll( veh, rate, limit, surf.tick );
			surfChangeVehicleDirection( veh, rate, surf.tick );
			//surfTrickTurn( &surf.current->trick, rate, surf.tick );
		} else if ( veh->roll > -35.0f ) {
			surfChangeVehicleRoll( veh, rate, limit, surf.tick );
			surfChangeVehicleDirection( veh, rate, surf.tick );
			//surfTrickTurn( &surf.current->trick, rate, surf.tick );
		} else if ( veh->roll > -70.0f ) {
			rate *= 0.5f;
			surfChangeVehicleRoll( veh, rate, limit, surf.tick );
			surfChangeVehicleDirection( veh, rate, surf.tick );
			//surfTrickTurn( &surf.current->trick, rate, surf.tick );
		}
	} else {
		if ( veh->roll < 0.0f ) {
			rate *= 2.0f;
			surfChangeVehicleRoll( veh, rate, limit, surf.tick );
			surfChangeVehicleDirection( veh, rate, surf.tick );
			//surfTrickTurn( &surf.current->trick, rate, surf.tick );
		} else if ( veh->roll < 35.0f ) {
			surfChangeVehicleRoll( veh, rate, limit, surf.tick );
			surfChangeVehicleDirection( veh, rate, surf.tick );
			//surfTrickTurn( &surf.current->trick, rate, surf.tick );
		} else if ( veh->roll < 70.0f ) {
			rate *= 0.5f;
			surfChangeVehicleRoll( veh, rate, limit, surf.tick );
			surfChangeVehicleDirection( veh, rate, surf.tick );
			//surfTrickTurn( &surf.current->trick, rate, surf.tick );
		}
	}
}


/**********************************************************
 * Function:	surfPilotSpin
 **********************************************************
 * Description: Update the spin for the pilot's jetski
 * Inputs:		veh - vehicle pointer
 *				rate - spin rate (angle)
 * Notes:	
 * Returns:
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 07-Sep-99	Theyer	Initial coding.
 *
 **********************************************************/

void surfPilotSpin (
    SurfVehicle	*veh,
	float		 rate
	)
{
	SurfJetSki		*jetski;
	float			 spin;

	/* init */
	jetski = (SurfJetSki *)veh->craft;

	if ( veh->state == VEHICLE_STATE_AIRTIME ) {
#if 0
		/* degrade pitchrate */
		if ( jetski->pitchrate != 0.0f ) {
			if ( gfxFabs(jetski->pitchrate) <= 0.21f )
				jetski->pitchrate = 0.0f;
			else if ( jetski->pitchrate > 0.0f ) 
				jetski->pitchrate -= 0.2f;
			else if ( jetski->pitchrate < 0.0f ) 
				jetski->pitchrate += 0.2f;
		}
#endif
		if ( ! veh->crash ) {
			/* add spin */
			spin = rate * surf.tick;
			if ( veh->airz < 30.0f ) spin *= (veh->airz * 0.03333f);
			veh->tick.spin += spin;
			veh->yaw += spin;
			surfKeepInRange( &veh->yaw, 180 );
		}
	} else {
		/* cancel spin */
		surfChangeVehicleDirection( veh, rate, surf.tick );
	}

	/* update vehicle value */
	veh->spin = jetski->spin;
}


/**********************************************************
 * Function:	surfChangePilotPitch
 **********************************************************
 * Description: Update the pitch for the pilot
 * Inputs:		veh - vehicle pointer
 *				angle - pitch angle
 * Notes:	
 * Returns:
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 07-Sep-99	Theyer	Initial coding.
 *
 **********************************************************/

void surfChangePilotPitch (
    SurfVehicle	*veh,
	float		 angle
	)
{
	SurfJetSki	*jetski;
	float		*pitch;

	/* init */
	jetski = (SurfJetSki *)veh->craft;
	pitch = &veh->tick.pitch;

	//printf( "a:a=%d,jsp=%d,t=%d,", angle, jetski->pitch, *pitch );
	if ( veh->state == VEHICLE_STATE_AIRTIME ) {
		*pitch = angle * surf.tick;
		jetski->pitch += *pitch;
		surfKeepInRange( &jetski->pitch, 180 );
	} else {
		if ( jetski->pitch != 0.0f ) {
			veh->pitch += jetski->pitch;
			jetski->pitch = 0.0f;
		}
		if ( angle != 0.0f )
			surfChangeVehiclePitch( veh, angle, surf.tick );
	}
	//printf( "b:jsp=%d,t=%d,", jetski->pitch, *pitch );
}


/**********************************************************
 * Function:	surfChangePilotRoll
 **********************************************************
 * Description: Update the roll for the pilot
 * Inputs:		veh - vehicle pointer
 *				angle - roll angle
 * Notes:	
 * Returns:
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 07-Sep-99	Theyer	Initial coding.
 *
 **********************************************************/

void surfChangePilotRoll (
    SurfVehicle	*veh,
	float		 angle,
	float		 limit
	)
{
	SurfJetSki	*jetski;
	float		*roll;

	/* init */
	jetski = (SurfJetSki *)veh->craft;
	roll = &veh->tick.roll;

	if ( veh->state == VEHICLE_STATE_AIRTIME ) {
		*roll = angle * surf.tick;
		jetski->roll += *roll;
		surfKeepInRange( &jetski->roll, 180 );
	} else {
		if ( jetski->roll )
			jetski->roll = 0.0f;
		surfChangeVehicleRoll( veh, angle, limit, surf.tick );
	}
}


/**********************************************************
 * Function:	surfPlayJetSkiingProcess
 **********************************************************
 * Description: Handle events for jetskiing play mode
 * Inputs:	pad - pad state
 *          padconfig - custom pad configuration
 * Notes:	
 * Returns:
 **********************************************************/

void surfPlayJetSkiingProcess ( 
	PersonInfo	*pilot
	)
{
	float		 analog[2];
	float		 fabs[2];
	SurfVehicle *veh;
	float		 left;
	float		 right;
	float		 up;
	float		 down;
	float		 press;
	float		 throttle;

#if SURF_SSX_STYLE_CONTROLS
// SSX controls
#define TURNLEFT		(surfPadDown( PAD_BUTTON_LEFT )  || (left  > SURF_ANALOG_MIN_INPUT))
#define TURNRIGHT		(surfPadDown( PAD_BUTTON_RIGHT ) || (right > SURF_ANALOG_MIN_INPUT))
#define NOSEUP			(surfPadDown( PAD_BUTTON_DOWN )  || (up    > SURF_ANALOG_MIN_INPUT))
#define NOSEDOWN		(surfPadDown( PAD_BUTTON_UP )    || (down  > SURF_ANALOG_MIN_INPUT))
#define TRICK			(surfPadDown( PAD_BUTTON_LEFT_1 ) && surfPadDown( PAD_BUTTON_RIGHT_1 ))
#define THROTTLE		(surfPadDown( PAD_BUTTON_A ))
#else
// original controls
#define TURNLEFT		(surfPadDown( PAD_BUTTON_LEFT )  || (left  > SURF_ANALOG_MIN_INPUT))
#define TURNRIGHT		(surfPadDown( PAD_BUTTON_RIGHT ) || (right > SURF_ANALOG_MIN_INPUT))
#define NOSEUP			(surfPadDown( PAD_BUTTON_DOWN )  || (up    > SURF_ANALOG_MIN_INPUT))
#define NOSEDOWN		(surfPadDown( PAD_BUTTON_UP )    || (down  > SURF_ANALOG_MIN_INPUT))
#define EJECT			(surfPadDown( PAD_BUTTON_C ))
#define TRICK			(surfPadDown( PAD_BUTTON_B ))
#define EXTREME			(surfPadDown( PAD_BUTTON_A ))
#define THROTTLE		(surfPadDown( PAD_BUTTON_RIGHT_1 ))
#endif

	/* no player input handling when crashing... */
	if ( pilot->crash ) return;

	/* init */
	veh = (SurfVehicle *)pilot->vehicle;

	if ( pilot->onvehicle ) {
		/* get analog values (will be 0 if not supported) */
		analog[0] = padAnalogX( surf.control.active, 0 );
		analog[1] = padAnalogY( surf.control.active, 0 );
		fabs[0] = gfxFabs(analog[0]);
		fabs[1] = gfxFabs(analog[1]);

		/* discard if values too low */
		if ( fabs[0] < SURF_ANALOG_MIN_INPUT ) analog[0] = 0.0f;
		if ( fabs[1] < SURF_ANALOG_MIN_INPUT ) analog[1] = 0.0f;

		/* init */
		left  = (analog[0] < 0.0f)?(fabs[0]):(0.0f);
		right = (analog[0] > 0.0f)?(fabs[0]):(0.0f);
		up    = (analog[1] < 0.0f)?(fabs[1]):(0.0f);
		down  = (analog[1] > 0.0f)?(fabs[1]):(0.0f);

		/* left turn analog override */
		if ( left == 0.0f ) {
			/* test pressure mode */
			press = surfPadPressure( PAD_BUTTON_LEFT );
			if ( press != 0.0f )
				left = press;
		}

		/* right turn analog override */
		if ( right == 0.0f ) {
			press = surfPadPressure( PAD_BUTTON_RIGHT );
			if ( press != 0.0f ) 
				right = press;
		}

		/* up analog override */
		if ( up == 0.0f ) {
			press = surfPadPressure( PAD_BUTTON_DOWN );
			if ( press != 0.0f ) 
				up = press;
		}

		/* down analog override */
		if ( down == 0.0f ) {
			press = surfPadPressure( PAD_BUTTON_UP );
			if ( press != 0.0f ) 
				down = press;
		}

		/* throttle */
#if SURF_SSX_STYLE_CONTROLS
		throttle = surfPadPressure( PAD_BUTTON_A );
#else
		throttle = surfPadPressure( PAD_BUTTON_RIGHT_1 );
#endif
		if ( throttle > SURF_ANALOG_MIN_INPUT ) 
			surfPilotProcessKeyEvent( pilot, KEY_THROTTLE, throttle );
		else 
			surfPilotProcessKeyEvent( pilot, KEY_IDLE_THROTTLE, 0 );
		
		switch( veh->state ) {
		case VEHICLE_STATE_AIRTIME:
			/* superman */
#if SURF_SSX_STYLE_CONTROLS
			if ( TRICK ) {
#else
			if ( TRICK && NOSEDOWN ) {
#endif
				/* no turns or pitch */
				surfPilotProcessKeyEvent( pilot, KEY_NO_UP_OR_DOWN, 0 );
				surfPilotProcessKeyEvent( pilot, KEY_NO_LEFT_OR_RIGHT, 0 );
				surfPilotProcessKeyEvent( pilot, KEY_SUPERMAN, 0 );
			} else {
				/* pitch */
				if ( NOSEUP )
					surfPilotProcessKeyEvent( pilot, KEY_NOSEUP, up );
				else if ( NOSEDOWN ) 
					surfPilotProcessKeyEvent( pilot, KEY_NOSEDOWN, down );
				else 
					surfPilotProcessKeyEvent( pilot, KEY_NO_UP_OR_DOWN, 0 );
				/* normal mode */
				if ( TURNRIGHT ) 
					surfPilotProcessKeyEvent( pilot, KEY_TURNRIGHT, right );
				else if ( TURNLEFT ) 
					surfPilotProcessKeyEvent( pilot, KEY_TURNLEFT, left );
				else if ( NOSEDOWN ) {
					surfPilotProcessKeyEvent( pilot, KEY_NO_LEFT_OR_RIGHT, 0 );
					surfPilotProcessKeyEvent( pilot, KEY_LEANFORWARD, 0 );
				} else
					surfPilotProcessKeyEvent( pilot, KEY_NO_LEFT_OR_RIGHT, 0 );
			}
			break;
		case VEHICLE_STATE_ONWATER:
			/* pitch */
			if ( NOSEUP )
				surfPilotProcessKeyEvent( pilot, KEY_NOSEUP, up );
			else if ( NOSEDOWN ) 
				surfPilotProcessKeyEvent( pilot, KEY_NOSEDOWN, down );
			else 
				surfPilotProcessKeyEvent( pilot, KEY_NO_UP_OR_DOWN, 0 );
			/* bunny hop */
			if ( TRICK && NOSEUP ) {
				/* no turns */
				surfPilotProcessKeyEvent( pilot, KEY_NO_LEFT_OR_RIGHT, 0 );
				/* trick mode */
				surfPilotProcessKeyEvent( pilot, KEY_BUNNY_HOP, 0 );
			} else {
				/* normal mode */
				if ( TURNRIGHT ) 
					surfPilotProcessKeyEvent( pilot, KEY_TURNRIGHT, right );
				else if ( TURNLEFT ) 
					surfPilotProcessKeyEvent( pilot, KEY_TURNLEFT, left );
#if 0
				else if ( NOSEDOWN ) {
					surfPilotProcessKeyEvent( pilot, KEY_NO_LEFT_OR_RIGHT, 0 );
					surfPilotProcessKeyEvent( pilot, KEY_LEANFORWARD, 0 );
				} else
#else
				else
#endif
					surfPilotProcessKeyEvent( pilot, KEY_NO_LEFT_OR_RIGHT, 0 );
			}
			break;
		}
	}
}


/**********************************************************
 * Function:	surfPilotProcessKeyEvent
 **********************************************************
 * Description: Change the state of the jetski pilot
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfPilotProcessKeyEvent (
	PersonInfo	*pilot,
    int			 key,
	float		 analog
    )
{
    int				*state;
    int				*target;
	SurfVehicle		*veh;
	SurfJetSki		*jetski;
	float			 rate;

    /* init */
    state  = &pilot->state;
    target = &pilot->target;
	veh    = (SurfVehicle*)pilot->vehicle;
	jetski = (SurfJetSki *)veh->craft;

	/* clear pitch and turn rates if not in the air... */
	if ( veh->state != VEHICLE_STATE_AIRTIME ) {
		jetski->turnrate  = 0.0f;
		jetski->pitchrate = 0.0f;
	}

	/* debug */
	//printf( "key = %d\n", key );

	switch( veh->state ) {
	case VEHICLE_STATE_AIRTIME:
        switch( key ) {
		case KEY_NOSEUP:
			*state = PILOT_STATE_CROUCH;
			rate = SURF_JETSKI_PITCHRATE;
			if ( analog != 0.0f )
				rate *= analog;
			if ( rate != 0.0f ) {
				surfChangePilotPitch( veh, rate );
				if ( jetski->pitchrate < SURF_JETSKI_PITCHLAG )
					jetski->pitchrate += 0.02f;
			}
			break;
		case KEY_NOSEDOWN:
			*state = PILOT_STATE_LEANFORWARD;
			rate = SURF_JETSKI_PITCHRATE;
			if ( analog != 0.0f )
				rate *= analog;
			if ( rate != 0.0f ) {
				surfChangePilotPitch( veh, -(rate) );
				if ( jetski->pitchrate > -(SURF_JETSKI_PITCHLAG) )
					jetski->pitchrate -= 0.02f;
			}
			break;
		case KEY_NO_UP_OR_DOWN:
			*state = PILOT_STATE_RIDING;
			surfChangePilotPitch( veh, jetski->pitchrate );
			break;
        case KEY_TURNRIGHT:
			/* turn right */
			*target = PILOT_STATE_HARDTURNRIGHT;
			rate = SURF_JETSKI_TURNRATE;
			if ( analog != 0.0f )
				rate *= analog;
			if ( rate != 0.0f ) {
				surfPilotSpin( veh, rate );
				if ( jetski->turnrate < SURF_JETSKI_TURNRATE )
					jetski->turnrate += 0.2f;
			}
            break;
        case KEY_TURNLEFT:
			/* turn left */
			*target = PILOT_STATE_HARDTURNLEFT;
			rate = SURF_JETSKI_TURNRATE;
			if ( analog != 0.0f )
				rate *= analog;
			if ( rate != 0.0f ) {
				surfPilotSpin( veh, -(rate) );
				if ( jetski->turnrate > -(SURF_JETSKI_TURNRATE) )
					jetski->turnrate -= 0.2f;
			}
            break;
		case KEY_LEANRIGHT:
			*state = PILOT_STATE_LEANRIGHT;
			surfPilotSpin( veh, SURF_JETSKI_TURNRATE );
			break;
		case KEY_LEANLEFT:
			*state = PILOT_STATE_LEANLEFT;
			surfPilotSpin( veh, -(SURF_JETSKI_TURNRATE) );
			break;
		case KEY_NO_LEFT_OR_RIGHT:
			//surfPilotTurn( veh, turnrate );
			surfPilotSpin( veh, jetski->turnrate );
			break;
		case KEY_RICKTA:
			/* go for the rickta */
			rate = SURF_JETSKI_RICKTA_RATE;
			if ( analog != 0.0f )
				rate *= analog;
			if ( rate != 0.0f )
				surfChangePilotPitch( veh, rate );
			//surfChangePilotPitch( veh, SURF_JETSKI_RICKTA_RATE );
			*state = PILOT_STATE_RICKTA;
			break;
		case KEY_TABLETOP_RIGHT:
			*state = PILOT_STATE_TABLETOPRIGHT;
			break;
		case KEY_CROUCH:
			if ( veh->posz > (veh->wavepos.water_level + 30) )
				*state = PILOT_STATE_CROUCH;
			break;
		case KEY_LEANFORWARD:
			*state = PILOT_STATE_LEANFORWARD;
			break;
		case KEY_SUPERMAN:
			*state = PILOT_STATE_SUPERMAN;
			/* neutralise NOSEDOWN key effects */
			//surfChangePilotPitch( veh, SURF_JETSKI_PITCHRATE );
			//if ( jetski->pitchrate < SURF_JETSKI_PITCHRATE )
			//	jetski->pitchrate += 1;
			break;
		case KEY_HEEL_CLICK:
			*state = PILOT_STATE_HEELCLICK;
			break;
		case KEY_HAND_PUNCH_RIGHT:
			*state = PILOT_STATE_HANDPUNCHRIGHT;
			break;
		case KEY_HAND_PUNCH_LEFT:
			*state = PILOT_STATE_HANDPUNCHLEFT;
			break;
		case KEY_HAND_PUNCH_BOTH:
			*state = PILOT_STATE_HANDPUNCHBOTH;
			break;
		case KEY_BARREL_ROLL:
			*state = PILOT_STATE_BARRELROLL;
			rate = SURF_JETSKI_RICKTA_RATE;
			if ( analog )
				rate *= gfxFabs(analog);
			if ( rate != 0.0f )
				surfChangePilotPitch( veh, -(rate) );
			break;
		case KEY_BARREL_ROLL_NO_HANDS:
			*state = PILOT_STATE_BARRELROLLNOHANDS;
			break;
        }
		break;
	case VEHICLE_STATE_ONWATER:		
        surfPilotSpin( veh, 0.0f );
		switch( key ) {
		case KEY_NOSEUP:
			if ( veh->speed > SURF_JETSKI_GLUE_SPEED ) {
				veh->glue_to_wave   = FALSE;
				veh->stay_with_wave = TRUE;
				rate = SURF_JETSKI_PITCHRATE;
				if ( analog != 0.0f )
					rate *= analog;
				if ( rate != 0.0f )
					surfChangePilotPitch( veh, rate );
			}
			break;
		case KEY_NOSEDOWN:
			veh->glue_to_wave   = TRUE;
			veh->stay_with_wave = FALSE;
			/* don't allow the pitch to go under zero when riding on the water */
			if ( veh->pitch > 0.0f ) {
				if ( veh->pitch > SURF_JETSKI_PITCHRATE ) {
					rate = SURF_JETSKI_PITCHRATE;
					if ( analog != 0.0f )
						rate *= analog;
					if ( rate != 0.0f )
						surfChangePilotPitch( veh, -(rate) );
				} else {
					surfChangePilotPitch( veh, 0.0f );
					veh->pitch = 0.0f;
				}
			} else
				surfChangePilotPitch( veh, 0.0f );
			break;
		case KEY_NO_UP_OR_DOWN:
			surfChangePilotPitch( veh, 0.0f );			
			veh->glue_to_wave   = (veh->speed < SURF_JETSKI_GLUE_SPEED);
			veh->stay_with_wave = FALSE;
			break;
		case KEY_NO_LEFT_OR_RIGHT:
			surfVehicleStraighten( veh, SURF_JETSKI_STRAIGHTENRATE, surf.tick );
			break;
		case KEY_SUBMARINE:
			if ( *state != PILOT_STATE_SUBMARINE ) {
				*target = PILOT_STATE_SUBMARINE;
				surfChangePilotPitch( veh, -35.0f );
			}
			break;
		case KEY_BUNNY_HOP:
			if ( *state != PILOT_STATE_BUNNYHOP ) {
				*target = PILOT_STATE_BUNNYHOP;
				surfChangePilotPitch( veh, 10.0f );
				veh->speed *= 0.85f;
			}
			break;
		case KEY_CROUCH:
			*target = PILOT_STATE_CROUCH;
			break;
		case KEY_LEANFORWARD:
			*target = PILOT_STATE_LEANFORWARD;
			break;
		}
		if ( veh->speed < SURF_JETSKI_SLOW_SPEED ) {
			switch( key ) {
			case KEY_TURNRIGHT:
			case KEY_HARDRIGHT:
				 if ( gfxFabs(veh->roll) > SURF_JETSKI_SLOWTURN )
					surfVehicleStraighten( veh, SURF_JETSKI_STRAIGHTENRATE, surf.tick );
				 if ( veh->roll < SURF_JETSKI_SLOWTURN )
					surfPilotTurn( veh, SURF_JETSKI_SLOWTURN );
				 break;
			case KEY_TURNLEFT:
			case KEY_HARDLEFT:
				 if ( gfxFabs(veh->roll) > SURF_JETSKI_SLOWTURN )
					surfVehicleStraighten( veh, SURF_JETSKI_STRAIGHTENRATE, surf.tick );
				 if ( veh->roll > -(SURF_JETSKI_SLOWTURN) )
					surfPilotTurn( veh, -(SURF_JETSKI_SLOWTURN) );
				break;
			}
		} else {
			switch( key ) {
			case KEY_TURNRIGHT:
				/* only turn if thrust is enough (50% of speed or better) */
				if ( veh->thrust < (veh->speed * 0.5f) )
					break;
				/* turn right */
				if ( gfxFabs(veh->roll) > 20.0f )
					*target = PILOT_STATE_HARDTURNRIGHT;
				else
					*target = PILOT_STATE_TURNRIGHT;
				rate = SURF_JETSKI_TURNRATE;
				if ( analog != 0.0f )
					rate *= analog;
				if ( rate != 0.0f )
					surfPilotTurn( veh, rate );
				break;
			 case KEY_TURNLEFT:
				/* only turn if thrust is enough (50% of speed or better) */
				if ( veh->thrust < (veh->speed * 0.5f) )
					break;
				/* turn left */
				if ( gfxFabs(veh->roll) > 20.0f )
					*target = PILOT_STATE_HARDTURNLEFT;
				else
					*target = PILOT_STATE_TURNLEFT;
				rate = SURF_JETSKI_TURNRATE;
				if ( analog != 0.0f )
					rate *= analog;
				if ( rate != 0.0f )
					surfPilotTurn( veh, -(rate) );
				break;
			}
		}
		break;
	case VEHICLE_STATE_SUBMERGED:
		veh->glue_to_wave = FALSE;
        switch( key ) {
		case KEY_NO_LEFT_OR_RIGHT:
			//*state = PILOT_STATE_RIDING;
			surfVehicleStraighten( veh, SURF_JETSKI_STRAIGHTENRATE, surf.tick );
			break;
		}
	}

	switch( key ) {
	case KEY_NO_THROTTLE:
		/* lay down if going slow (at idle speed) */
		if ( veh->speed <= SURF_JETSKI_IDLE_SPEED ) {
			*target = PILOT_STATE_ON_TUMMY;
			return;
		}
		/* no throttle (engine off or stalled) */
		surfJetSkiSetThrust( veh, 0.0f );
		break;
	case KEY_IDLE_THROTTLE:
		/* lay down if going slow (at idle speed) */
		if ( veh->speed <= SURF_JETSKI_IDLE_SPEED ) {
			*target = PILOT_STATE_ON_TUMMY;
			return;
		}
		/* idle throttle */
		surfJetSkiSetThrust( veh, SURF_JETSKI_IDLE_THRUST );
		break;
	case KEY_THROTTLE:
		/* apply throttle */
		rate = (SURF_JETSKI_FULL_THROTTLE_THRUST * analog);
		if ( rate < 0.5f )
			*target = PILOT_STATE_HALFTHROTTLE;
		if ( rate > 0.5f )
			*target = PILOT_STATE_FULLTHROTTLE;
		surfJetSkiSetThrust( veh, rate );
	    break;
	}
}


/**********************************************************
 * Function:	surfPilotNextPosition
 **********************************************************
 * Description: Change the position of the surfer
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfPilotNextPosition (
    PersonInfo	*pilot
    )
{
	SurfVehicle	*veh;

	/* init */
	veh = (SurfVehicle *)pilot->vehicle;

	/* cancel vertical thrust for jetski */
	if ( (veh->state != VEHICLE_STATE_AIRTIME) && veh->pitch > 30.0f )
		surfJetSkiSetThrust( veh, 0.0f );

	/* handle crash */
	if ( veh->crash ) {
		// should maybe set pilot position when setting crash for different crash positions?
		//pilot->target = PILOT_POS_CRASH_LIP;
		surfJetSkiSetThrust( veh, 0.0f );
		return;
	}

	//printf( "current = %d\n", pilot->current );
	/* get next position for target */
	switch( pilot->target ) {
	default:
	case PILOT_STATE_RIDING:
		/* are we in the desired position? */
		if ( pilot->current == PILOT_POS_RIDING || pilot->current == PILOT_POS_RIDING_GOOFY ) {
			/* update the state */
			pilot->state = pilot->target;
			break;
		}
		/* transition from right turn */
		if ( (pilot->current > PILOT_POS_RIDING_GOOFY) && (pilot->current <= PILOT_POS_RIGHT_TURN_FAST) ) {
			pilot->current--;
			break;
		}
		/* transition from left turn */
		if ( (pilot->current > PILOT_POS_LEFT_TURN_READY) && (pilot->current <= PILOT_POS_LEFT_TURN_FAST) ) {
			pilot->current--;
			break;
		}
		/* if both feet forward to to goofy position */
		if ( pilot->current == PILOT_POS_BOTH_FEET_FORWARD ) {
			pilot->current = PILOT_POS_RIDING_GOOFY;
			break;
		}
		/* default */
		pilot->current = PILOT_POS_RIDING;
		break;
	case PILOT_STATE_ON_TUMMY:
		/* are we in the desired position? */
		if ( pilot->current == PILOT_POS_ON_TUMMY ) {
			/* update the state */
			pilot->state = pilot->target;
			break;
		}
		/* climb onto jetski */
		if ( (pilot->current >= PILOT_POS_BEHIND_IN_WATER) && (pilot->current < PILOT_POS_ON_TUMMY) ) {
			pilot->current++;
			break;
		}
		/* riding to tummy position */
		if ( pilot->current <= PILOT_POS_RIDING ) {
			pilot->current--;
			break;
		}
		/* transition from right turn */
		if ( (pilot->current > PILOT_POS_BOTH_FEET_FORWARD) && (pilot->current <= PILOT_POS_RIGHT_TURN_FAST) ) {
			pilot->current--;
			break;
		}
		/* transition from left turn */
		if ( (pilot->current > PILOT_POS_LEFT_TURN_READY) && (pilot->current <= PILOT_POS_LEFT_TURN_FAST) ) {
			pilot->current--;
			break;
		}
		/* default */
		pilot->current = PILOT_POS_RIDING;
		break;
	case PILOT_STATE_IDLETHROTTLE:
	case PILOT_STATE_HALFTHROTTLE:
	case PILOT_STATE_FULLTHROTTLE:
		/* are we in the desired position? */
		if ( pilot->current == PILOT_POS_RIDING || pilot->current == PILOT_POS_RIDING_GOOFY ) {		
			/* update the state */
			pilot->state = pilot->target;
			//pilot->current = PILOT_POS_RIDING_BUMP;
			break;
		}
		/* tummy to riding position */
		if ( pilot->current < PILOT_POS_RIDING ) {
			pilot->current++;
			break;
		}
		/* transition from right turn */
		if ( (pilot->current > PILOT_POS_RIDING_GOOFY) && (pilot->current <= PILOT_POS_RIGHT_TURN_FAST) ) {
			pilot->current--;
			break;
		}
		/* transition from left turn */
		if ( (pilot->current > PILOT_POS_LEFT_TURN_READY) && (pilot->current <= PILOT_POS_LEFT_TURN_FAST) ) {
			pilot->current--;
			break;
		}
		// swap if goofy
		//if ( pilot->current == PILOT_POS_RIDING_GOOFY ) {
		//	pilot->current = PILOT_POS_BOTH_FEET_FORWARD;
		//	break;
		//}
		/* if both feet forward go to goofy position */
		if ( pilot->current == PILOT_POS_BOTH_FEET_FORWARD ) {
			//pilot->current = PILOT_POS_RIDING;
			pilot->current = PILOT_POS_RIDING_GOOFY;
			break;
		}
		/* default */
		pilot->current = PILOT_POS_RIDING;
		break;
	case PILOT_STATE_TURNRIGHT:
		if ( pilot->current == PILOT_POS_RIGHT_TURN_SLOW ) {
			/* update the state */
			pilot->state = pilot->target;
			break;
		}
		/* riding to swap feet position */
		if ( pilot->current == PILOT_POS_RIDING ) {
			pilot->current = PILOT_POS_BOTH_FEET_FORWARD;
			break;
		}
		/* transition to right turn */
		if ( (pilot->current >= PILOT_POS_BOTH_FEET_FORWARD) && (pilot->current < PILOT_POS_RIGHT_TURN_SLOW) ) {
			pilot->current++;
			break;
		}
		/* default */
		pilot->current = PILOT_POS_RIDING;
		break;
	case PILOT_STATE_HARDTURNRIGHT:
		if ( pilot->current == PILOT_POS_RIGHT_TURN_FAST ) {
			/* update the state */
			pilot->state = pilot->target;
			break;
		}
		/* riding to swap feet position */
		if ( pilot->current == PILOT_POS_RIDING ) {
			pilot->current = PILOT_POS_BOTH_FEET_FORWARD;
			break;
		}
		/* transition to right turn */
		if ( (pilot->current >= PILOT_POS_BOTH_FEET_FORWARD) && (pilot->current < PILOT_POS_RIGHT_TURN_FAST) ) {
			pilot->current++;
			break;
		}
		/* default */
		pilot->current = PILOT_POS_RIDING;
		break;
	case PILOT_STATE_TURNLEFT:
		if ( pilot->current == PILOT_POS_LEFT_TURN_SLOW ) {
			/* update the state */
			pilot->state = pilot->target;
			break;
		}
		/* riding to swap feet position */
		if ( pilot->current == PILOT_POS_RIDING ) {
			pilot->current = PILOT_POS_LEFT_TURN_READY;
			break;
		}
		/* transition to left turn */
		if ( (pilot->current >= PILOT_POS_LEFT_TURN_READY) && (pilot->current < PILOT_POS_LEFT_TURN_SLOW) ) {
			pilot->current++;
			break;
		}
		/* default */
		pilot->current = PILOT_POS_RIDING;
		break;
	case PILOT_STATE_HARDTURNLEFT:
		if ( pilot->current == PILOT_POS_LEFT_TURN_FAST ) {
			/* update the state */
			pilot->state = pilot->target;
			break;
		}
		/* riding to swap feet position */
		if ( pilot->current == PILOT_POS_RIDING ) {
			pilot->current = PILOT_POS_LEFT_TURN_READY;
			break;
		}
		/* transition to left turn */
		if ( (pilot->current >= PILOT_POS_LEFT_TURN_READY) && (pilot->current < PILOT_POS_LEFT_TURN_FAST) ) {
			pilot->current++;
			break;
		}
		/* default */
		pilot->current = PILOT_POS_RIDING;
		break;
#if 0
	case PILOT_STATE_BEHIND_IN_WATER:
		/* are we in the desired position? */
		if ( pilot->current == PILOT_POS_BEHIND_IN_WATER ) {
			/* update the state */
			pilot->state = PILOT_STATE_IDLETHROTTLE;
			break;
		}
		/* lay down if going slow (at idle speed) */
		if ( (pilot->current > PILOT_POS_BEHIND_IN_WATER) && (pilot->current <= PILOT_POS_ON_TUMMY ) )
			pilot->current--;
		else if ( pilot->current != PILOT_POS_ON_TUMMY )
			pilot->current = PILOT_STATE_ON_TUMMY;
		break;
#endif
	}
}

