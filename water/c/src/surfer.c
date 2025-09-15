/**********************************************************
 * Copyright (C) 1999,2000 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:	PlayStation surf game
 **********************************************************
 * File:	surfer.c
 * Author:	Mark Theyer
 * Created:	6 Dec 1997
 **********************************************************
 * Description: Surf game surfer related functions	
 **********************************************************
 * Functions:
 *	surfInitSurfer() 
 *				Initialise surfer dude.
 *	surfBuildSurfer()
 *				Build the surfer model and initialise.
 *	surfDrawSurfer()
 *				Draw the surfer dude.
 *	surfSurferProcessKeyEvent()
 *				Surfer event handling.
 *	surfSurferTurn()
 *				Turn the surfboard (roll angle)
 *	surfSurferStraighten()
 *				Straighten the surfboard (done when no turning
 *				events are being performed).
 *	surfSurferNextPosition()
 *				Select next position to go to if necessary.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 06-Dec-97	Theyer	Initial Coding
 * 13-Jul-99	Theyer	Initial update for new game
 *
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

void surfSurferTurn( SurfVehicle *veh, float rate );

/*
 * variables
 */

extern SurfData    	surf;

/*
 * functions
 */


/**********************************************************
 * Function:	surfResetSurfer
 **********************************************************
 * Description: Prepare to play with surfer dude
 * Inputs:		surfer - surfer person pointer
 * Notes:	
 * Returns:
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

void surfResetSurfer (
    PersonInfo	*surfer
    )
{
	surfer->state      = SURFER_STATE_LAYONBOARD;
	surfer->target     = SURFER_STATE_LAYONBOARD;
	surfer->current    = SURFER_POS_PADDLE0;
	surfer->last       = SURFER_POS_PADDLE0;
	surfer->interpol   = 0.0f;
	surfer->move_speed = 0.1f;
	surfer->crash      = FALSE;
	surfer->onvehicle  = TRUE;
	gfxSetPosition( &surfer->object, 1, surfer->last, surfer->current, surfer->interpol );
	personPutOnVehicle( surfer, surfVehicleObject(surfer->vehicle) );
}


/**********************************************************
 * Function:	surfLoadSurfer
 **********************************************************
 * Description: Create a surfer dude
 * Inputs:		surfer - surfer person pointer
 *				name - name of surfer model file to load
 * Notes:	
 * Returns:
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

GfxObject	*surfLoadSurfer (
    PersonInfo	*surfer,
	int			 num,
	int			 player
    )
{
	char		 buff[64];
	GfxObject	*object;

#ifdef SURF_USE_PAKS
	gfxFilePath( "models/riders" );
	sprintf( buff, "s%d.pak", num );
	gfxFileLoad( buff );
#endif

	/* locate models */
	sprintf( buff, "models/riders/s%d", num );
	gfxFilePath( buff );

	/* load texture map locations */
	sprintf( buff, "s%dp%d.map", num, player );
	surfLoadMap( buff, FALSE );

    /* create model */
	sprintf( buff, "s%d", num );
    if ( ! p2mModelToGfxObject( buff, (GfxObject **)&object, NULL ) ) {
		printf( p2mModelError() );
		surfExit(SURF_ERROR_SURFER_LOAD_FAILED);
	}

    /* ok */
	return( object );
}


/**********************************************************
 * Function:	surfSurferProcessKeyEvent
 **********************************************************
 * Description: Change the state of the surfer
 * Inputs:		surfer - surfer person pointer
 *				key - key event code
 * Notes:	
 * Returns:
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

void surfSurferProcessKeyEvent (
	PersonInfo	*surfer,
    int			 key,
	float		 analog
    )
{
	int				*state;
	int				 oldstate;
	SurfVehicle		*veh;
	float			 rate;

#define	ANALOG_ADJUST(rate)		((analog != 0.0f)?(rate * gfxAbs(analog)):(rate))

    /* init */
    state    = &surfer->state;
	oldstate = *state;
	veh      = (SurfVehicle *)surfer->vehicle;

	//printf( "key=%d,state=%d,pos=%d\n", key, *state, surfer->current );

    /* change state of surfer for direction key event */
    switch( *state ) {
    case SURFER_STATE_LAYONBOARD:
	case SURFER_STATE_PADDLING:
		surfSurferSpin( veh, 0 );
		/* surfer is paddling */
        switch( key ) {
		case KEY_DUCKDIVE:
			/* duck-dive */
            *state = SURFER_STATE_DUCKDIVING;
   			break;
        case KEY_PADDLEFORWARD:
			/* paddle left */
			*state = SURFER_STATE_PADDLING;
			/* ride surfboard */
			if ( veh->speed >= SURFER_STANDUP_SPEED )
				*state = SURFER_STATE_STANDING;
			break;
		case KEY_PADDLERIGHT:
			/* paddle right */
			if ( *state != SURFER_STATE_PADDLING )
				*state = SURFER_STATE_PADDLERIGHT;
			if ( veh->speed )
				surfChangeVehicleDirection( veh, SURF_SURFER_TURNRATE, surf.tick );
			/* ride surfboard */
			if ( veh->speed >= SURFER_STANDUP_SPEED )
				*state = SURFER_STATE_STANDING;
			break;
        case KEY_PADDLELEFT:
			/* paddle left */
			*state = SURFER_STATE_PADDLING;
			if ( veh->speed )
				surfChangeVehicleDirection( veh, -(SURF_SURFER_TURNRATE), surf.tick );
			/* ride surfboard */
			if ( veh->speed >= SURFER_STANDUP_SPEED )
				*state = SURFER_STATE_STANDING;
            break;
		case KEY_NOTPADDLING:
			if ( veh->speed >= SURFER_STANDUP_SPEED )
				/* ride surfboard */
				*state = SURFER_STATE_STANDING;
			else
				/* just lay on board if we stop paddling */
				*state = SURFER_STATE_LAYONBOARD;
			break;
        }
		surfVehicleStraighten( veh, SURF_SURFER_STRAIGHTENRATE, surf.tick );
		break;
	case SURFER_STATE_DUCKDIVING:		
        switch( key ) {
		case KEY_DUCKDIVE:
		case KEY_STANDUP:
			break;
		default:
			*state = SURFER_STATE_LAYONBOARD;
			break;
		}
		break;
	case SURFER_STATE_STANDING:
		switch( key ) {
		case KEY_TURNRIGHT:
		case KEY_HARDRIGHT:
			/* turn right */
			surfSurferTurn( veh, (SURF_SURFER_TURNRATE*0.5f) );
			break;
		case KEY_TURNLEFT:
		case KEY_HARDLEFT:
			/* turn left */
			surfSurferTurn( veh, -(SURF_SURFER_TURNRATE*0.5f) );
			break;	
		}
	    return;
		break;
	default:
		switch( key ) {
		case KEY_NO_UP_OR_DOWN:
			if ( *state == SURFER_STATE_STALL )
				*state = SURFER_STATE_RIDING;
			break;
		case KEY_NO_LEFT_OR_RIGHT:
			if ( *state != SURFER_STATE_STALL )
				*state = SURFER_STATE_RIDING;
			surfSurferSpin( veh, 0 );
			break;
		}
	}

	switch( veh->state ) {
	case VEHICLE_STATE_ONWATER:
        switch( key ) {			
		case KEY_TURNRIGHT:
			/* turn right */
			*state = SURFER_STATE_TURNRIGHT;
			rate = SURF_SURFER_TURNRATE;
			rate = ANALOG_ADJUST(rate);
			if ( rate != 0.0f )
				surfSurferTurn( veh, rate );
			 break;
		 case KEY_TURNLEFT:
			/* turn left */
			*state = SURFER_STATE_TURNLEFT;
			rate = SURF_SURFER_TURNRATE;
			rate = ANALOG_ADJUST(rate);
			if ( rate != 0.0f )
				surfSurferTurn( veh, -(rate) );
			break;
		case KEY_HARDRIGHT:
			/* turn right */
			*state = SURFER_STATE_HARDTURNRIGHT;
			rate = SURF_SURFER_HARDTURNRATE;
			rate = ANALOG_ADJUST(rate);
			if ( rate != 0.0f )
				surfSurferTurn( veh, rate );
			break;
		case KEY_HARDLEFT:
			/* turn left */
			*state = SURFER_STATE_HARDTURNLEFT;
			rate = SURF_SURFER_HARDTURNRATE;
			rate = ANALOG_ADJUST(rate);
			if ( rate != 0.0f )
				surfSurferTurn( veh, -(rate) );
			break;
		case KEY_NOSEUP:
			veh->glue_to_wave   = FALSE;
			veh->stay_with_wave = TRUE;
			/* degrade speed to simulate extra drag if heading down the face */
			if ( veh->speed > 0 && veh->direction > 180 ) {
				veh->glue_to_wave   = TRUE;
				*state = SURFER_STATE_STALL;
				rate = (veh->slowdown*2.0f);
				rate = ANALOG_ADJUST(rate);
				veh->speed -= (veh->speed * rate * surf.tick);
				if ( veh->speed < 0 )
					veh->speed = 0;
			} else
				surfChangeSurferPitch( veh, SURF_SURFER_PITCHRATE );			
			break;
		case KEY_NOSEDOWN:
			veh->glue_to_wave   = TRUE;
			//veh->stay_with_wave = TRUE;//FALSE;
			/* don't allow the pitch to go under zero when riding on the water */
			if ( veh->pitch > 0 ) {
				if ( veh->pitch > SURF_SURFER_PITCHRATE )
					surfChangeSurferPitch( veh, -(SURF_SURFER_PITCHRATE) );
				else
					veh->pitch = 0;
			} else
				surfChangeSurferPitch( veh, 0 );
			break;
		case KEY_NO_UP_OR_DOWN:
			if ( veh->speed > 0 && veh->direction > 180 )
				veh->glue_to_wave = TRUE;
			else
				veh->glue_to_wave = ( veh->speed < SURF_AIRTIME_SPEED );
			//veh->glue_to_wave   = TRUE;
			veh->stay_with_wave = TRUE;
			surfChangeSurferPitch( veh, 0 );
			break;
		case KEY_NO_LEFT_OR_RIGHT:
			surfVehicleStraighten( veh, SURF_SURFER_STRAIGHTENRATE, surf.tick );
			//if ( *state != SURFER_STATE_STANDING )
			//	*state = SURFER_STATE_RIDING;
			surfSurferSpin( veh, 0 );
			break;
		case KEY_BOOST:
			surfSetVehicleThrust( veh, SURF_BOARD_TERMINAL_VELOCITY );
			break;
		}
		break;
	case VEHICLE_STATE_AIRTIME:
        switch( key ) {						
		case KEY_TURNRIGHT:
			/* turn right */
			rate = SURF_SURFER_TURNRATE;
			rate = ANALOG_ADJUST(rate);
			if ( rate != 0.0f )
				surfSurferSpin( veh, rate );
			break;
		 case KEY_TURNLEFT:
			/* turn left */
			rate = SURF_SURFER_TURNRATE;
			rate = ANALOG_ADJUST(rate);
			if ( rate != 0.0f )
				surfSurferSpin( veh, -(rate) );
			break;
		case KEY_HARDRIGHT:
			/* turn right */
			rate = SURF_SURFER_HARDSPINRATE;
			rate = ANALOG_ADJUST(rate);
			if ( rate != 0.0f ) {
				surfSurferSpin( veh, rate );
				//surfTriggerSound( &surf.sound, SURF_SFX_ID_BOARD_SPIN_WHIP, 800, 800 );
			}
		    break;
		case KEY_HARDLEFT:
			/* turn left */
			rate = SURF_SURFER_HARDSPINRATE;
			rate = ANALOG_ADJUST(rate);
			if ( rate != 0.0f ) {
				surfSurferSpin( veh, -(rate) );
				//surfTriggerSound( &surf.sound, SURF_SFX_ID_BOARD_SPIN_WHIP, 800, 800 );
			}
			break;
		case KEY_NOSEUP:
			rate = SURF_SURFER_PITCHRATE;
			rate = ANALOG_ADJUST(rate);
			if ( rate != 0.0f )
				surfChangeSurferPitch( veh, rate );
			break;
		case KEY_NOSEDOWN:
			rate = SURF_SURFER_PITCHRATE;
			rate = ANALOG_ADJUST(rate);
			if ( rate != 0.0f )
				surfChangeSurferPitch( veh, -(rate) );
			break;
		case KEY_EXTREME_NOSEUP:
			rate = (SURF_SURFER_PITCHRATE*2.0f);
			rate = ANALOG_ADJUST(rate);
			if ( rate != 0.0f ) {
				surfChangeSurferPitch( veh, rate );
				//surfTriggerSound( &surf.sound, SURF_SFX_ID_BOARD_SPIN_WHIP, 1500, 1500 );
			}
			break;
		case KEY_EXTREME_NOSEDOWN:
			rate = (SURF_SURFER_PITCHRATE*2.0f);
			rate = ANALOG_ADJUST(rate);
			if ( rate != 0.0f ) {
				surfChangeSurferPitch( veh, -(rate) );
				//surfTriggerSound( &surf.sound, SURF_SFX_ID_BOARD_SPIN_WHIP, 1500, 1500 );
			}
			break;
		case KEY_AIRWALK:
			*state = SURFER_STATE_TRICK_AIRWALK;
			break;
		}
		break;
	}
}


/**********************************************************
 * Function:	surfSurferTurn
 **********************************************************
 * Description: Update the roll for the surfer
 * Inputs:		veh - vehicle pointer
 *				rate - turn rate (angle)
 * Notes:	
 * Returns:
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

void surfSurferTurn (
    SurfVehicle	*veh,
	float		 rate
	)
{
	float		 limit;
	//float		 slow;

	/* cancel trick turn */
	//if ( rate == 0.0f )
	//	surfTrickTurn( &surf.current->trick, 0, 0 );

	/* adjust so turns are slower at high speeds */
	//slow = (1.0f - (veh->speed * SURF_BOARD_TURN_FRICTION));
	//printf( "rate=%.2f,speed=%0.2f,adjust=%.2f,result=%.2f\n", rate, veh->speed, limit, (rate * limit) );
	//rate *= slow;

	/* limit only in the water */
	limit = SURF_BOARD_ROLL_LIMIT;// * slow;
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
 * Function:	surfSurferSpin
 **********************************************************
 * Description: Update the spin for the surfer
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

void surfSurferSpin (
    SurfVehicle	*veh,
	float		 rate
	)
{
	SurfBoard	*board;
	float		 spin;

	/* no turning under planing speed */
	if ( veh->speed < SURF_BOARD_PLANING_SPEED ) return;

	/* init */
	board = (SurfBoard *)veh->craft;

	/* remove spin if landed */
	if ( (veh->state != VEHICLE_STATE_AIRTIME) && (board->spin != 0.0f) )
		rate = 0.0f;

	if ( rate != 0.0f ) {
		/* add spin */
		spin = rate * surf.tick;
		board->spin += spin;
		veh->tick.spin += spin;
		/* keep in range */
		surfKeepInRange( &board->spin, 180 );
	} else {
		/* 0=no spin */
		veh->yaw += board->spin;
		surfKeepInRange( &veh->yaw, 180 );
		surfChangeVehicleDirection( veh, 0.0f, surf.tick );	
		board->spin = 0.0f;
	}
}


/**********************************************************
 * Function:	surfChangeSurferPitch
 **********************************************************
 * Description: Update the pitch for the surfer
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

void surfChangeSurferPitch (
    SurfVehicle	*veh,
	float		 angle
	)
{
	SurfBoard	*board;
	float		*pitch;

	/* init */
	board = (SurfBoard *)veh->craft;
	pitch = &veh->tick.pitch;

	if ( veh->state == VEHICLE_STATE_AIRTIME ) {
		*pitch = angle * surf.tick;
		board->pitch += *pitch;
		surfKeepInRange( &board->pitch, 180 );
	} else {
		if ( board->pitch != 0.0f ) {
			/* change direction if pitch is pointing the board in the opposite direction */
			if ( gfxAbs( board->pitch ) > 90.0f ) {
				surfChangeVehicleDirection( veh, 180.0f, 1.0f );
			}
			board->pitch = 0.0f;
		}
		if ( angle != 0.0f )
			surfChangeVehiclePitch( veh, angle, surf.tick );
	}
}


/**********************************************************
 * Function:	surfSurferNextPosition
 **********************************************************
 * Description: Change the position of the surfer
 * Inputs:		surfer - surfer person pointer
 * Notes:	
 * Returns:
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

void surfSurferNextPosition (
    PersonInfo	*surfer
    )
{
	SurfVehicle	*veh;
	//SurfBoard	*board;

	/* init */
	veh = surfer->vehicle;

	/* handle crash */
	if ( veh->crash ) {
		/* do a crash */
		personMoveSpeed( surfer, PERSON_MOVESPEED );
		personSetPosition( surfer, SURFER_POS_GR_R );
		return;
	}

#if 0
	/* grab rail if air off lip */
	if ( veh->state == VEHICLE_STATE_AIRTIME && veh->posz > (veh->water_level+50)) {
	    /* do a rail grab... */
		personMoveSpeed( surfer, (PERSON_MOVESPEED<<1) );
		personSetPosition( surfer, SURFER_POS_RAILGRAB );
		return;
	}
#endif

	if ( (surfer->state != SURFER_STATE_PADDLING    ) && 
		 (surfer->state != SURFER_STATE_PADDLERIGHT ) && 
		 (surfer->state != SURFER_STATE_DUCKDIVING  ) ) {
		/* thrust only when paddling */
		//surfSetVehicleThrust( veh, 0 );		
		/* if we are moving too slowly go back to paddling */
		if ( veh->speed < SURFER_LAYDOWN_SPEED )
			surfer->state = SURFER_STATE_LAYONBOARD;
#if 0
		/* add random pitch for realistic(ish) water */
		if ( (!veh->wavepos.onwave) && veh->state == VEHICLE_STATE_ONWATER ) 
			surfChangeVehiclePitch( veh, (int)surfGetRandom( SURF_RANDOM_SET_N1_2 ), surf.tick );
#endif
	}

    /* determine next position to go to */
    switch( surfer->state ) {
    case SURFER_STATE_LAYONBOARD:
		/* surfer is laying on the surfboard */
		switch( surfer->current ) {
		case SURFER_POS_LAY_ON_BOARD0:
		case SURFER_POS_LAY_ON_BOARD1:
			personSetPosition( surfer, (surfer->current + 1) );
			personMoveSpeed( surfer, (PERSON_MOVESPEED*0.25f) );
			break;
		case SURFER_POS_LAY_ON_BOARD2:
			personSetPosition( surfer, SURFER_POS_LAY_ON_BOARD0 );
			personMoveSpeed( surfer, (PERSON_MOVESPEED*0.25f) );
			break;
		case SURFER_POS_PADDLE0:
			personSetPosition( surfer, SURFER_POS_LAY_ON_BOARD0 );
			personMoveSpeed( surfer, (PERSON_MOVESPEED*0.5f) );
			break;
		case SURFER_POS_PADDLE1:
		case SURFER_POS_PADDLE2:
		case SURFER_POS_PADDLE3:
		case SURFER_POS_PADDLE4:
		case SURFER_POS_PADDLE5:
			personSetPosition( surfer, SURFER_POS_PADDLE0 );
			personMoveSpeed( surfer, (PERSON_MOVESPEED*2.0f) );
			break;
		case SURFER_POS_STAND5:
		case SURFER_POS_STAND4:
		case SURFER_POS_STAND3:
		case SURFER_POS_STAND2:
		case SURFER_POS_STAND1:
		case SURFER_POS_STAND0:
			personSetPosition( surfer, (surfer->current - 1) );
			personMoveSpeed( surfer, (PERSON_MOVESPEED*2.0f) );
            break;
		default:
			personSetPosition( surfer, SURFER_POS_STAND5 );
			personMoveSpeed( surfer, (PERSON_MOVESPEED*2.0f) );
            break;
        }
		return;
        break;
	case SURFER_STATE_PADDLERIGHT:
	case SURFER_STATE_PADDLING:
		/* surfer is paddling */
		surfer->state = SURFER_STATE_PADDLING;
        switch( surfer->current ) {
		case SURFER_POS_PADDLE_END:
			personSetPosition( surfer, SURFER_POS_PADDLE_START );
			break;
        case SURFER_POS_PADDLE_START:
			if ( veh->onwave && veh->frontface && veh->direction > 180 ) {
				surfSetVehicleThrust( veh, (SURF_PADDLE_THRUST*2.0f) );
				personMoveSpeed( surfer, (PERSON_MOVESPEED*2.0f) );
			} else {
				surfSetVehicleThrust( veh, SURF_PADDLE_THRUST );
				personMoveSpeed( surfer, PERSON_MOVESPEED );
			}
			/* pass thru */
        case SURFER_POS_PADDLE1:
        case SURFER_POS_PADDLE2:
        case SURFER_POS_PADDLE3:
        case SURFER_POS_PADDLE4:
			personSetPosition( surfer, (surfer->current + 1) );
			//if ( surfer->current == SURFER_POS_PADDLE1 )
			//	surfTriggerSound( &surf.sound, SURF_SFX_ID_SURFER_PADDLE_RIGHT, 1024, 3072 );
			//if ( surfer->current == SURFER_POS_PADDLE4 )
			//	surfTriggerSound( &surf.sound, SURF_SFX_ID_SURFER_PADDLE_LEFT, 3072, 1024 );
			break;
		default:
			personSetPosition( surfer, SURFER_POS_PADDLE_START );
			personMoveSpeed( surfer, PERSON_MOVESPEED );
			break;
        }
		return;
        break;
	case SURFER_STATE_TURNRIGHT:
		personSetPosition( surfer, SURFER_POS_TURNRIGHT );
		if ( bitOn(surfer->extrainfo,SURFER_EXTRA_LOWSTANCE) )
			personSetPosition( surfer, SURFER_POS_LOW_TR );
		if ( bitOn(surfer->extrainfo,SURFER_EXTRA_GRAB_RIGHT) )
			personSetPosition( surfer, SURFER_POS_GR_R_TR );
		if ( bitOn(surfer->extrainfo,SURFER_EXTRA_GRAB_LEFT) )
			personSetPosition( surfer, SURFER_POS_GR_L_TR );
		personMoveSpeed( surfer, PERSON_MOVESPEED );
		break;		
	case SURFER_STATE_HARDTURNRIGHT:
		personSetPosition( surfer, SURFER_POS_LOW_TR );
		if ( bitOn(surfer->extrainfo,SURFER_EXTRA_GRAB_RIGHT) )
			personSetPosition( surfer, SURFER_POS_GR_R_TR );
		if ( bitOn(surfer->extrainfo,SURFER_EXTRA_GRAB_LEFT) )
			personSetPosition( surfer, SURFER_POS_GR_L_TR );
		personMoveSpeed( surfer, PERSON_MOVESPEED );
		break;		
	case SURFER_STATE_TURNLEFT:	
		personSetPosition( surfer, SURFER_POS_TURNLEFT );
		if ( bitOn(surfer->extrainfo,SURFER_EXTRA_LOWSTANCE) )
			personSetPosition( surfer, SURFER_POS_LOW_TL );
		if ( bitOn(surfer->extrainfo,SURFER_EXTRA_GRAB_RIGHT) )
			personSetPosition( surfer, SURFER_POS_GR_R_TL );
		if ( bitOn(surfer->extrainfo,SURFER_EXTRA_GRAB_LEFT) )
			personSetPosition( surfer, SURFER_POS_GR_L_TL );
		personMoveSpeed( surfer, PERSON_MOVESPEED );
		break;
	case SURFER_STATE_HARDTURNLEFT:
		personSetPosition( surfer, SURFER_POS_LOW_TL );
		if ( bitOn(surfer->extrainfo,SURFER_EXTRA_GRAB_RIGHT) )
			personSetPosition( surfer, SURFER_POS_GR_R_TL );
		if ( bitOn(surfer->extrainfo,SURFER_EXTRA_GRAB_LEFT) )
			personSetPosition( surfer, SURFER_POS_GR_L_TL );
		personMoveSpeed( surfer, PERSON_MOVESPEED );
		break;
	case SURFER_STATE_STANDING:
		/* thrust only when paddling */
		surfSetVehicleThrust( veh, 0 );		
        switch( surfer->current ) {		
		case SURFER_POS_RIDING:
			if ( personInPosition( surfer ) )
				surfer->state = SURFER_STATE_RIDING;
			personMoveSpeed( surfer, PERSON_MOVESPEED );
			break;
		case SURFER_POS_STAND1:
			//surfTriggerSound( &surf.sound, SURF_SFX_ID_SURFER_STAND_UP, 2048, 2048 );
		case SURFER_POS_STAND0:
		case SURFER_POS_STAND2:
		case SURFER_POS_STAND3:
		case SURFER_POS_STAND4:
		case SURFER_POS_STAND5:
			personSetPosition( surfer, (surfer->current + 1) );
			personMoveSpeed( surfer, PERSON_MOVESPEED );
			break;
		default:
			personSetPosition( surfer, SURFER_POS_STAND0 );
			personMoveSpeed( surfer, PERSON_MOVESPEED );
			break;
		}
		return;
		break;
	case SURFER_STATE_RIDING:
	case SURFER_STATE_STALL:
		personSetPosition( surfer, SURFER_POS_RIDING );
		if ( bitOn(surfer->extrainfo,SURFER_EXTRA_LOWSTANCE) )
			personSetPosition( surfer, SURFER_POS_LOW );
		if ( bitOn(surfer->extrainfo,SURFER_EXTRA_GRAB_RIGHT) )
			personSetPosition( surfer, SURFER_POS_GR_R );
		if ( bitOn(surfer->extrainfo,SURFER_EXTRA_GRAB_LEFT) )
			personSetPosition( surfer, SURFER_POS_GR_L );
		personMoveSpeed( surfer, PERSON_MOVESPEED );
		break;		
	case SURFER_STATE_CRASH_LIP:
		break;
#if 0
	// now same as riding (above)
	case SURFER_STATE_STALL:
		//if ( veh->wavepos.size > 6000 ) {
		/* in barrel */
		if ( veh->direction > 270 || veh->direction < 90 )
			personSetPosition( surfer, SURFER_POS_GR_L );
		else
			personSetPosition( surfer, SURFER_POS_GR_R );
		//} else
		//	personSetPosition( surfer, SURFER_POS_RAILGRAB );
		personMoveSpeed( surfer, PERSON_MOVESPEED );
		break;
#endif
	case SURFER_STATE_TRICK_AIRWALK:
        switch( surfer->current ) {
		case SURFER_POS_GR_R_AIR_WALK:
			break;
		case SURFER_POS_GR_R:
			personSetPosition( surfer, SURFER_POS_GR_R_AIR );
			personMoveSpeed( surfer, PERSON_MOVESPEED );
		case SURFER_POS_GR_R_AIR:
			personSetPosition( surfer, SURFER_POS_GR_R_AIR_WALK );
			personMoveSpeed( surfer, PERSON_MOVESPEED );
			break;
		default:
			personSetPosition( surfer, SURFER_POS_GR_R );
			personMoveSpeed( surfer, PERSON_MOVESPEED );
			break;
		}
		break;
    }
}


/**********************************************************
 * Function:	surfPlaySurfingProcess
 **********************************************************
 * Description: Handle events for surfing play mode
 * Inputs:	pad - pad state
 *          padconfig - custom pad configuration
 * Notes:	
 * Returns:
 **********************************************************/

void surfPlaySurfingProcess ( 
	PersonInfo	*surfer
	)
{
	SurfVehicle *veh;
	//int		 i;
	float		 analog[4];

#define PADDLEFORWARD	  (surfPadDown( PAD_BUTTON_UP )    || (analog[1] >   SURF_ANALOG_MIN_INPUT))
#define PADDLERIGHT		  (surfPadDown( PAD_BUTTON_RIGHT ) || (analog[0] >   SURF_ANALOG_MIN_INPUT))
#define PADDLELEFT		  (surfPadDown( PAD_BUTTON_LEFT )  || (analog[0] < -(SURF_ANALOG_MIN_INPUT)))
#define TURNLEFT		  (surfPadDown( PAD_BUTTON_LEFT )  || (analog[0] < -(SURF_ANALOG_MIN_INPUT)))
#define TURNRIGHT		  (surfPadDown( PAD_BUTTON_RIGHT ) || (analog[0] >   SURF_ANALOG_MIN_INPUT))
#define NOSEUP			  (surfPadDown( PAD_BUTTON_DOWN )  || (analog[1] < -(SURF_ANALOG_MIN_INPUT)))
#define NOSEDOWN		  (surfPadDown( PAD_BUTTON_UP )    || (analog[1] >   SURF_ANALOG_MIN_INPUT))
#define STALL			  (surfPadDown( PAD_BUTTON_DOWN )  || (analog[1] < -(SURF_ANALOG_MIN_INPUT)))
#define LOW				  (surfPadDown( PAD_BUTTON_A ))
#define BOOST			  (surfPadDown( PAD_BUTTON_B ))
#define GRAB_LEFT		  (surfPadDown( PAD_BUTTON_LEFT_1 ))
#define GRAB_RIGHT		  (surfPadDown( PAD_BUTTON_RIGHT_1 ))
#define TRICK_LEFT		  (surfPadDown( PAD_BUTTON_LEFT_2 ))
#define TRICK_RIGHT		  (surfPadDown( PAD_BUTTON_RIGHT_2 ))
#define POP				  (surfPadReleased( PAD_BUTTON_A ))

	/* no player input handling when crashing... */
	if ( surfer->crash ) return;

	if ( surfer->onvehicle ) {
		veh = (SurfVehicle *)surfer->vehicle;

		/* hack for crash */
		//if ( FALLOFF )
		//	personPutOnVehicle( surfer, FALSE );

		/* get analog values (will be 0 if not supported) */
		analog[0] = padAnalogX( surf.control.active, 0 );
		analog[1] = padAnalogY( surf.control.active, 0 );
		//printf( "analog(%d) x=%d, y=%d\n", surf.control.active, analog[0], analog[1] );
		//analog[2] = padAnalogX( surf.control.active, 1 );
		//analog[3] = padAnalogY( surf.control.active, 1 );

		/* discard if values too low */
		if ( gfxAbs(analog[0]) < SURF_ANALOG_MIN_INPUT ) analog[0] = 0;
		if ( gfxAbs(analog[1]) < SURF_ANALOG_MIN_INPUT ) analog[1] = 0;

		switch( surfer->state ) {
		case SURFER_STATE_STANDING:
			if ( TURNRIGHT )
				surfSurferProcessKeyEvent( surfer, KEY_TURNRIGHT, analog[0] );
			else if ( TURNLEFT ) 
				surfSurferProcessKeyEvent( surfer, KEY_TURNLEFT, analog[0]  );
			else
				surfSurferProcessKeyEvent( surfer, KEY_NO_LEFT_OR_RIGHT, 0 );
			break;
		case SURFER_STATE_PADDLING:
		case SURFER_STATE_PADDLERIGHT:
		case SURFER_STATE_LAYONBOARD:
		case SURFER_STATE_DUCKDIVING:
			/* paddling? */
			if ( PADDLERIGHT )
				surfSurferProcessKeyEvent( surfer, KEY_PADDLERIGHT, analog[0] );
			else if ( PADDLELEFT )
				surfSurferProcessKeyEvent( surfer, KEY_PADDLELEFT, analog[0] );
			else if ( PADDLEFORWARD )
				surfSurferProcessKeyEvent( surfer, KEY_PADDLEFORWARD, analog[1] );
			else
				surfSurferProcessKeyEvent( surfer, KEY_NOTPADDLING, 0 );
			break;
		default:
			/* modifiers */
			if ( LOW )
				surfer->extrainfo |= SURFER_EXTRA_LOWSTANCE;
			else
				surfer->extrainfo &= ~(SURFER_EXTRA_LOWSTANCE);
			if ( GRAB_LEFT )
				surfer->extrainfo |= SURFER_EXTRA_GRAB_LEFT;
			else
				surfer->extrainfo &= ~(SURFER_EXTRA_GRAB_LEFT);
			if ( GRAB_RIGHT )
				surfer->extrainfo |= SURFER_EXTRA_GRAB_RIGHT;
			else
				surfer->extrainfo &= ~(SURFER_EXTRA_GRAB_RIGHT);
			if ( veh->state == VEHICLE_STATE_AIRTIME ) {
				/* in air ... */
				if ( NOSEUP )
					if ( bitOn(surfer->extrainfo,SURFER_EXTRA_LOWSTANCE) )
						surfSurferProcessKeyEvent( surfer, KEY_EXTREME_NOSEUP, analog[1] );
					else
						surfSurferProcessKeyEvent( surfer, KEY_NOSEUP, analog[1] );
				else if ( NOSEDOWN )
					if ( bitOn(surfer->extrainfo,SURFER_EXTRA_LOWSTANCE) )
						surfSurferProcessKeyEvent( surfer, KEY_EXTREME_NOSEDOWN, analog[1] );
					else
						surfSurferProcessKeyEvent( surfer, KEY_NOSEDOWN, analog[1] );
				else 
					surfSurferProcessKeyEvent( surfer, KEY_NO_UP_OR_DOWN, 0 );
				if ( TURNRIGHT ) {
					if ( bitOn(surfer->extrainfo,SURFER_EXTRA_LOWSTANCE) )
						surfSurferProcessKeyEvent( surfer, KEY_HARDRIGHT, analog[0] );
					else
						surfSurferProcessKeyEvent( surfer, KEY_TURNRIGHT, analog[0] );
				} else if ( TURNLEFT ) {
					if ( bitOn(surfer->extrainfo,SURFER_EXTRA_LOWSTANCE) )
						surfSurferProcessKeyEvent( surfer, KEY_HARDLEFT, analog[0] );
					else
						surfSurferProcessKeyEvent( surfer, KEY_TURNLEFT, analog[0]  );
				} else
					surfSurferProcessKeyEvent( surfer, KEY_NO_LEFT_OR_RIGHT, 0 );
				if ( TRICK_LEFT )
					surfSurferProcessKeyEvent( surfer, KEY_AIRWALK, 0 );
				break;
			} else {
				if ( BOOST )
					surfSurferProcessKeyEvent( surfer, KEY_BOOST, 0 );
				else
					surfSetVehicleThrust( veh, 0 );
				/* riding on water... */
				if ( NOSEUP )
					surfSurferProcessKeyEvent( surfer, KEY_NOSEUP, analog[1] );
				else if ( NOSEDOWN ) 
					surfSurferProcessKeyEvent( surfer, KEY_NOSEDOWN, analog[1] );
				else 
					surfSurferProcessKeyEvent( surfer, KEY_NO_UP_OR_DOWN, 0 );
				/* turn */
				if ( TURNRIGHT ) {
					if ( bitOn(surfer->extrainfo,SURFER_EXTRA_LOWSTANCE) )
						surfSurferProcessKeyEvent( surfer, KEY_HARDRIGHT, analog[0] );
					else
						surfSurferProcessKeyEvent( surfer, KEY_TURNRIGHT, analog[0] );
				} else if ( TURNLEFT ) {
					if ( bitOn(surfer->extrainfo,SURFER_EXTRA_LOWSTANCE) )
						surfSurferProcessKeyEvent( surfer, KEY_HARDLEFT, analog[0] );
					else
						surfSurferProcessKeyEvent( surfer, KEY_TURNLEFT, analog[0] );
				} else {
					surfSurferProcessKeyEvent( surfer, KEY_NO_LEFT_OR_RIGHT, 0 );
				}
			}
		}
	} else {
		surfSurferProcessKeyEvent( surfer, KEY_NO_UP_OR_DOWN, 0 );
		surfSurferProcessKeyEvent( surfer, KEY_NO_LEFT_OR_RIGHT, 0 );
	}
}
