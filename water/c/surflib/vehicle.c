/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		vehicle.c
 * Author:		Mark Theyer
 * Created:		15 Jul 1999
 **********************************************************
 * Description:	Vehicle related functions. The surfboard(s)
 *				are a type of surf vehicle.
 **********************************************************
 * Functions:
 *	surfInitVehicle() 
 *				Initialise the surf vehicle.
 *  surfUpdateVehiclePosition()
 *				Update the position of the vehicle.
 *	surfNextXYPosition()
 *				Get a relative x,y position based on
 *				speed and direction.
 *	surfVehiclePitchFlatten()
 *				Flatten the pitch of the vehicle.
 *	surfPushVehicleNose()
 *				Push the nose of the vehicle down the wave
 *				face.
 *	surfChangeVehicleDirection()
 *				Change the direction of the vehicle.
 *	surfChangeVehiclePitch()
 *				Change the pitch angle of the vehicle.
 *	surfChangeVehicleRoll()
 *				Change the roll angle of the vehicle.
 *	surfVehicleStraighten()
 *				Straighten the vehicle by a rate
 *	surfUpdateVehicle()
 *				Position the vehicle in 3D space.
 *	surfVehiclePitch()
 *				Get the current vehicle pitch.
 *	surfVehicleRoll()
 *				Get the current vehicle roll.
 *	surfVehicleInPosition()
 *				Is the vehicle in position?
 *	surfVehicleNextPosition()
 *				Set the vehicle position based on the rider's
 *				position.
 *	surfDrawVehicle()
 *				Draw the vehicle.
 *	surfVehicleNextAirPosition()
 *				Get the next air position for the vehicle.
 *	surfNextAirPosition()
 *				Get a relative z position based on
 *				pitch and angle.
 *	surfUpdateVehicleAirPosition()
 *				Set the vehicle's z position based on water
 *				level at this point, speed, direction, etc.
 *	surfVehicleSetState()
 *				Set the state of a vehicle (air/onwater/sub).
 *	surfUpdateVehicleSpeed()
 *				Update the speed of the vehicle.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 24-Jul-98	Theyer	Initial coding
 * 15-Jul-99	Theyer	Initial update for new game
 * 15-Jul-99	Theyer	Disabled surfPushVehicleNose() so
 *						vehicle does not turn down the wave.
 * 04-Jun-01	Theyer	New port for PS2
 *
 **********************************************************/

/*
 * includes
 */

#include <surfutil.h>
#include <gfx/gfx.h>
#include <vehicle.h>

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

/*
 * functions
 */


/**********************************************************
 * Function:	surfInitVehicle
 **********************************************************
 * Description: Initialise a surfing vehicle
 * Inputs:		veh    - vehicle data pointer to initialise
 *				type   - vehicle type
 *				object - graphical object
 *				data   - data pointer to save with vehicle
 *				weight - weight of vehicle and rider
 *				terminal_velocity - maximum speed possible
 *				slowdown     - slow down value (to simulate drag)
 *				update       - update vehicle callback function
 *				inposition   - vehicle in position callback function
 *				nextposition - vehicle next position callback function
 *				draw         - draw vehicle callback function
 * Notes:		Callbacks allow different vehicle types
 * Returns:		initialised vehicle data
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

void surfInitVehicle (
    SurfVehicle						*veh,
	int								 type,
	GfxObject						*object,
	void							*craft,
	void							*rider,
	float							 terminal_velocity,
	float							 slowdown,
	SurfVehicleUpdateCallback		*update,
	SurfVehicleInPositionCallback	*inposition,
	SurfVehicleNextPositionCallback *nextposition,
	SurfVehicleDrawCallback			*draw
    )
{
	/* clear vehicle memory */
	memClear( veh, sizeof(SurfVehicle) );

	/* create dummy object for positioning rider and craft */
    veh->posobj = gfxBeginObject( "vehicle", NULL, FALSE );
    gfxEndObject( veh->posobj );

	/* init */
	veh->type			    = type;
	veh->object				= object;
	veh->mat				= gfxGetObjectMatrix( veh->posobj );
	veh->craft				= craft;
	veh->rider				= rider;
	veh->state				= VEHICLE_STATE_ONWATER;
	veh->rate				= 1.0f;
	veh->terminal_velocity	= terminal_velocity;
	veh->update				= update;
	veh->inposition			= inposition;
	veh->nextposition		= nextposition;
	veh->draw				= draw;
	veh->slowdown			= slowdown;
	veh->sail_side			= TRUE;

	/* init object as a child */
	gfxObjectParent( object, veh->posobj );

#if USE_WAVE_POSITION
	surfInitWavePos( &veh->wavepos );
#endif
}


/**********************************************************
 * Function:	surfUpdateVehiclePosition
 **********************************************************
 * Description: Update a surfing vehicle's position
 * Inputs:		veh     - vehicle data pointer
 *				contour - surface contour (unused to cube backgrounds)
 *				tick	- game timer tick
 * Notes:		nil
 * Returns:		updates vehicle position (x,y)
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

float surfUpdateVehiclePosition (
	SurfVehicle		*veh,
	SurfContour		*contour,
	float			 tick
	)
{
	float			 speedx;
	float			 speedy;
	float			 pitch;
	float			 rate;

	/* save last */
	veh->last.posx = veh->posx;
	veh->last.posy = veh->posy;

	/* no change if no speed */
	if ( veh->speed < 1.0f ) {
		veh->surface_speed = 0.0f;
		return( 0.0f );
	}

	/* change direction depending on the roll */
	rate = (veh->roll * 0.0833333f);		// divide by 12...
	surfChangeVehicleDirection( veh, rate, tick );

	/* update position */
	switch ( veh->state ) {
	case VEHICLE_STATE_AIRTIME:
		pitch = veh->takeoffangle;
		break;
	case VEHICLE_STATE_SUBMERGED:
		pitch = veh->pitch;
		break;
	default:
	case VEHICLE_STATE_ONWATER:
		pitch = veh->pitch;
		if ( veh->onwave ) 
			pitch += veh->wave_pitch;
		break;
	}

	/* get next x,y position */
	if ( veh->crash && (veh->state != VEHICLE_STATE_AIRTIME) ) {
		/* slow to a quick stop when crashing into water */
		speedx = (veh->last.speedx * 0.5f);
		speedy = (veh->last.speedy * 0.5f);
		veh->speed = (veh->speed * 0.5f);
		/* update roll, pitch and yaw */
		veh->roll  *= 0.5f;
		veh->pitch *= 0.5f;
		veh->yaw   *= 0.5f;
	} else
		surfNextXYPosition( veh->speed, tick, veh->direction, pitch, &speedx, &speedy );

	/* update position */
	veh->posx += speedx;
	veh->posy += speedy;

	/* calculate surface speed */
	veh->surface_speed = (gfxCos((int)veh->direction) * speedx) + (gfxSin((int)veh->direction) * speedy) * tick;
	//printf2( "dir=%d, speedx=%d, speedy=%d, veh->speed=%0.2f\n", veh->direction, (int)speedx, (int)speedy, veh->speed );

	/* save speed */
	veh->last.speedx = speedx;
	veh->last.speedy = speedy;

	return( rate );
}


/**********************************************************
 * Function:	surfNextXYPosition
 **********************************************************
 * Description: Get the next x,y position for the vehicle
 * Inputs:		speed     - speed of vehicle
 *				tick      - game timer tick
 *				direction - direction vehicle is heading
 *				pitch     - pitch angle of vehicle
 *				x         - returned new x position
 *				y         - returned new y position
 * Notes:		nil
 * Returns:		new x and y 
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

void surfNextXYPosition (
	float	 speed,
	float	 tick,
	float	 direction,
	float	 pitch,
	float	*x,
	float	*y
	)
{
	float	 ratio;

	//printf2( "speed=%d(%.2f), dir=%d, pitch=%d\n", speed, float_TO_FLOAT(speed), direction, pitch );
	/* force into range -90 to +90 indicating pitch angle */
	if ( pitch > 90.0f )
		pitch = (180.0f - pitch);
	else if ( pitch < -90.0f )
		pitch = (-180.0f - pitch);

	/* if vertical align say 80 for some forward movement */
	if ( gfxAbs(pitch) >= 90.0f )
		pitch = 80.0f;

	/* work out ratio */
	ratio = 1.0f;
	if ( pitch != 0.0f )
		ratio = (90.0f - gfxAbs(pitch)) * 0.011111f;

	/* new position */
	*x = -(gfxCos((int)direction) * speed * ratio * tick);
	*y = gfxSin((int)direction) * speed * ratio * tick;
}


/**********************************************************
 * Function:	surfVehiclePitchFlatten
 **********************************************************
 * Description: Flatten vehicle pitch...
 * Inputs:		veh - vehicle pointer
 * Notes:		nil
 * Returns:		updated vehicle
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

void surfVehiclePitchFlatten (
	SurfVehicle		*veh
	)
{
	/* flatten */
	if ( veh->pitch != 0.0f ) {
		//printf( "pitch = %d,", veh->pitch );
		if ( veh->pitch > 0.0f ) {
			if ( veh->pitch < 15.0f )
				veh->pitch -= 1.0f;
			else if ( veh->pitch < 45.0f ) 
				veh->pitch -= 5.0f;
			else if ( veh->pitch < 90.0f )
				veh->pitch -= 15.0f;
			else 
				veh->pitch -= 25.0f;
		} else {
			/* must be negative */
			if ( veh->pitch > -15.0f )
				veh->pitch += 1.0f;
			else if ( veh->pitch > -45.0f ) 
				veh->pitch += 5.0f;
			else if ( veh->pitch > -90.0f )
				veh->pitch += 15.0f;
			else 
				veh->pitch += 25.0f;
		}
		//printf( " after flatten = %d\n", veh->pitch );
	}
}


/**********************************************************
 * Function:	surfPushVehicleNose
 **********************************************************
 * Description: Push nose down wave
 * Inputs:		veh - vehicle data pointer
 * Notes:		nil
 * Returns:		updated vehicle
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 * 15-Jul-99	Theyer	Disabled.
 *
 **********************************************************/

void surfPushVehicleNose (
	SurfVehicle		*veh
	)
{
	/* DISABLED... (Mark T. 15/07/99) */
	;
}


/**********************************************************
 * Function:	surfChangeVehicleDirection
 **********************************************************
 * Description: Change the direction of the vehicle
 * Inputs:		veh    - vehicle pointer
 *				angle  - direction angle to change
 *				tick   - game timer tick
 * Notes:		nil
 * Returns:		updated vehicle
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

void surfChangeVehicleDirection (
	SurfVehicle		*veh,
	float			 angle,
	float			 tick
	)
{
	/* adjust to tick */
	angle = angle * tick;

	switch( veh->state ) {
	case VEHICLE_STATE_AIRTIME:
		/* yaw changes but direction does not */
		veh->yaw += angle;
		/* keep in range -179..180 degrees */
		surfKeepInRange( &veh->yaw, 180 );
		break;
	case VEHICLE_STATE_SUBMERGED:
		/* allow half mobility turning under water */
		angle *= 0.5f;
		/* pass thru */
	case VEHICLE_STATE_ONWATER:
		veh->direction += angle;
		/* no yaw - only direction as we have grip */
		if ( veh->yaw != 0.0f ) {
			/* remove yaw component */
			veh->direction += veh->yaw;
			veh->last.yaw = veh->yaw;
			veh->yaw = 0.0f;
		} else
			veh->last.yaw = 0.0f;
		/* ensure we stay in the range 0 - 359 degrees */
		surfKeepInRange( &veh->direction, 360 );
		break;
	}
	/* save */
	veh->last.turn = angle;
}


/**********************************************************
 * Function:	surfChangeVehiclePitch
 **********************************************************
 * Description: Update the pitch of the surfing vehicle
 * Inputs:		veh    - vehicle pointer
 *				angle  - pitch angle to change
 *				tick   - game timer tick
 * Notes:		nil
 * Returns:		updated vehicle
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

void surfChangeVehiclePitch (
	SurfVehicle		*veh,
	float			 angle,
	float			 tick
	)
{
	switch( veh->state ) {
	case VEHICLE_STATE_SUBMERGED:
		break;
	case VEHICLE_STATE_ONWATER:
	case VEHICLE_STATE_AIRTIME:
		/* save */
		veh->last.pitch = veh->pitch;
		/* update pitch */
		angle = angle * tick;
		veh->pitch += angle;
		/* keep in range -179..180 degrees */
		surfKeepInRange( &veh->pitch, 180 );
		break;
	}
}


/**********************************************************
 * Function:	surfChangeVehicleRoll
 **********************************************************
 * Description: Update the pitch of the surfing vehicle
 * Inputs:		veh    - vehicle pointer
 *				angle  - pitch angle to change
 *				limit  - optional limiter (0 = no limit)
 *				tick   - game timer tick
 * Notes:		nil	
 * Returns:		updated vehicle
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

void surfChangeVehicleRoll (
	SurfVehicle		*veh,
	float			 angle,
	float			 limit,
	float			 tick
	)
{
	switch( veh->state ) {
	case VEHICLE_STATE_SUBMERGED:
		break;
	case VEHICLE_STATE_ONWATER:
	case VEHICLE_STATE_AIRTIME:
		//printf( "roll was=%d,angle=%d", veh->roll, angle );
		angle = angle * tick;
		veh->roll += angle;
		/* use limiter if necessary */
		//printf( ",tangle=%d,is=%d", angle, veh->roll );
		if ( limit != 0.0f ) {
			if ( veh->roll < -(limit) )
				veh->roll = -(limit);
			if ( veh->roll > limit )
				veh->roll = limit;
		}
		//printf( ",limit=%d,limited=%d", limit, veh->roll );
		/* keep in range -179..180 degrees */
		surfKeepInRange( &veh->roll, 180 );
		//printf( ",range=%d\n", veh->roll );
		/* save */
		veh->last.roll = angle;
		break;
	}
}


/**********************************************************
 * Function:	surfVehicleStraighten
 **********************************************************
 * Description: Straighten the vehicle by a rate
 * Inputs:		veh  - vehicle pointer
 *				rate - straighten rate (angle)
 *				tick - game timer tick
 * Notes:		nil
 * Returns:		update vehicle
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

void surfVehicleStraighten (
	SurfVehicle *veh, 
	float		 rate,
	float		 tick
	)
{
	/* no auto straightening of craft when in the air... */
	if ( veh->state == VEHICLE_STATE_AIRTIME ) return;

	/* adjust for tick */
	rate = rate * tick;

	//*state = PILOT_STATE_THROTTLE_A;
	if ( veh->roll >= rate ) {
		//printf( "roll -= TR\n" );
		//surfChangeVehicleRoll( veh, -(rate), 0 );
		veh->roll -= rate;
	} else if ( veh->roll <= -(rate) ) {
		//surfChangeVehicleRoll( veh, rate, 0 );
		//printf("roll += TR\n");
		veh->roll += rate;
	} else {
		//printf("roll = 0\n");
		veh->roll = 0.0f;
	}
}


/**********************************************************
 * Function:	surfUpdateVehicle
 **********************************************************
 * Description: Position the vehicle in 3D space
 * Inputs:		veh - vehicle pointer
 * Notes:		nil
 * Returns:		updated vehicle
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

public void surfUpdateVehicle (
	SurfVehicle	*veh
    )
{
	/* call update callback */
	if ( veh->update )
		veh->update( veh->craft, veh, veh->rider );
}


/**********************************************************
 * Function:	surfVehiclePitch
 **********************************************************
 * Description: Get the pitch of the vehicle
 * Inputs:		veh - vehicle pointer
 * Notes:	
 * Returns:		pitch angle
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

public float surfVehiclePitch (
	SurfVehicle		*veh
    )
{
	return( veh->pitch );
}


/**********************************************************
 * Function:	surfVehicleRoll
 **********************************************************
 * Description: Get the roll of the vehicle
 * Inputs:		veh - vehicle pointer
 * Notes:	
 * Returns:		roll angle of vehicle
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

public float surfVehicleRoll (
	SurfVehicle		*veh
    )
{
	return( veh->roll );
}


/**********************************************************
 * Function:	surfVehicleInPosition
 **********************************************************
 * Description: Is this vehicle in position?
 * Inputs:		veh - vehicle pointer
 * Notes:	
 * Returns:		TRUE if in position, FALSE if not
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

public Bool surfVehicleInPosition (
	SurfVehicle		*veh
    )
{
	Bool			    result;

	/* init */
	result = TRUE;

	if ( veh->inposition )
		result = (Bool)veh->inposition( veh->craft );

	return( result );
}


/**********************************************************
 * Function:	surfVehicleNextPosition
 **********************************************************
 * Description: Set the vehicle position based on the rider's
 *				position.
 * Inputs:		veh - vehicle pointer
 *				riderpos - rider's position
 * Notes:	
 * Returns:		nil
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

void surfVehicleNextPosition (
	SurfVehicle		*veh,
	int				 riderpos
    )
{
	if ( veh->nextposition )
		veh->nextposition( veh->craft );
}


/**********************************************************
 * Function:	surfDrawVehicle
 **********************************************************
 * Description: Draw the vehicle 
 * Inputs:		veh - vehicle pointer
 * Notes:	
 * Returns:		nil
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

void surfDrawVehicle (
	SurfVehicle		*veh
    )
{
	if ( veh->draw )
		veh->draw( veh->craft );
}


/**********************************************************
 * Function:	surfVehicleNextAirPosition
 **********************************************************
 * Description: Get the next air position for the vehicle
 * Inputs:		veh - vehicle pointer
 * Notes:	
 * Returns:		updates the nextz value in the vehicle
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

void surfVehicleNextAirPosition (
	SurfVehicle		*veh,
	float			 tick
	)
{
	float		angle;
	float		zchange;

	switch ( veh->state ) {
	case VEHICLE_STATE_AIRTIME:
		//veh->gravity -= (2 * (tick>>2));
		veh->gravity -= (tick * 0.5f);
		//printf( "air gravity=%d\n", veh->gravity );
		angle = veh->takeoffangle;
		break;
	case VEHICLE_STATE_SUBMERGED:
		//veh->gravity += (3 * tick>>2);
		veh->gravity += (tick * 0.75f);
		//printf( "sub gravity=%d\n", veh->gravity );
		angle = veh->pitch;
		/* facing up but coming down so reverse sign */
		if ( angle > 0.0f && veh->vertical_speed < 0.0f )
			angle = -(angle);
		break;
	default:
	case VEHICLE_STATE_ONWATER:
		angle = veh->pitch;
		if ( veh->onwave ) 
			angle += veh->wave_pitch;
		/* facing up but coming down so reverse sign */
		if ( angle > 0.0f && veh->vertical_speed < 0.0f )
			angle = -(angle);
		break;
	}	

	/* get proposed next z position */
	zchange = surfNextAirPosition( veh->speed, angle );
	//printf( "zchange=%d,pz=%d\n", zchange, veh->posz );

	/* this gravity thing may cause problems and need replacing? */
	//veh->nextz = veh->posz + float_TO_INT( ((zchange + veh->gravity) * tick) );
	veh->nextz = (int)(veh->posz + zchange + (veh->gravity * tick));
	//printf( "nz=%d,%d\n", veh->nextz, float_TO_INT( ((zchange + veh->gravity) * surf.tick) ) );

	/* save takeoff angle and roll... */
	if ( veh->state != VEHICLE_STATE_AIRTIME ) {
		veh->takeoffdir   = veh->direction;
		veh->takeoffz     = veh->posz;
		veh->takeoffangle = angle;
		veh->takeoffroll  = veh->wave_roll;
	}
}


/**********************************************************
 * Function:	surfNextAirPosition
 **********************************************************
 * Description: Estimate the change in z position
 * Inputs:		speed - speed
 *				angle - pitch angle
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

float surfNextAirPosition (
	float		speed,
	float		angle
	)
{
	//float		speedrate;
	float	zchange;

	//printf( "in angle = %d, ", angle );

	/* force into range 0..180 */
	surfKeepInRange( &angle, 180 );

	/* force into range -90 to +90 indicating pitch angle */
	if ( angle > 90.0f )
		angle = (180.0f - angle);
	else if ( angle < -90.0f )
		angle = (-180.0f - angle);

	//printf( "z angle = %d\n", angle );

	/* get next z position */
	if ( angle > 0.0f )
		zchange = (gfxSin(angle) * speed);
	else 
		zchange = -(gfxSin( -(angle) ) * speed);

	return( zchange );
}


/**********************************************************
 * Function:	surfUpdateVehicleAirPosition
 **********************************************************
 * Description: Update a surfing vehicle's position
 * Inputs:		veh    - vehicle pointer
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

void surfUpdateVehicleAirPosition (
	SurfVehicle		*veh,
	WaveSwell		*swell,
	SurfSplashData	*splash,
	float			 tick
	)
{
	int			water_level;
	float		pitch;
	Bool		sub;

	/* record this state */
	veh->last.state = veh->state;

	/* init */
	water_level = veh->water_level;
#if 0
	if ( veh->onwave ) {
		/* if under curl limit height to top of wave curl */
		if ( ! veh->crash ) {
			if ( veh->wavepos.size > 1.22f && veh->wavepos.size < 2.5f ) {
				/* if going up and above curl but is near curl limit height to curl height */
				if ( (veh->nextz > (int)veh->last.posz) && (veh->nextz > veh->wavepos.highest_z) && (veh->nextz < (veh->wavepos.highest_z + 100)) )
					veh->nextz = veh->wavepos.highest_z;
			}
		}
	}
#endif

	//printf( "water level = %d\n", water_level );
	if ( veh->nextz > water_level ) {
		//printf("a: ");
		/* want air */
		switch( veh->state ) {
		case VEHICLE_STATE_AIRTIME:
			/* still in the air */
			veh->wave_pitch   = 0.0f;
			veh->wave_roll    = 0.0f;
			break;
		case VEHICLE_STATE_ONWATER:
			/* keep on water if speed < 3.0 */
			if ( veh->speed < 3.0f ) {
				veh->nextz = water_level;
				break;
			}
			/* pass thru */
		case VEHICLE_STATE_SUBMERGED:
#if 0
			/* do not allow air from back face of wave for jetski only */
			if ( veh->type == VEHICLE_TYPE_JETSKI || veh->type == VEHICLE_TYPE_PWC ) {
				if ( veh->onwave && ! veh->frontface && (veh->facepos < 0.9277f) ) {
					surfVehicleSetState( veh, VEHICLE_STATE_ONWATER, splash );
					veh->nextz = water_level;
					break;
				}
			}
#endif
			/* do not allow any air after a crash */
			if ( veh->crash ) {
				surfVehicleSetState( veh, VEHICLE_STATE_ONWATER, splash );
				veh->nextz = water_level;
				break;
			}
			/* stay on wave for glue to wave when on the front face and going slow */
			if ( (!veh->crash) && veh->glue_to_wave && veh->onwave && veh->frontface ) {
				surfVehicleSetState( veh, VEHICLE_STATE_ONWATER, splash );
				veh->nextz = water_level;
				break;
			}
			/* got air */
			surfVehicleSetState( veh, VEHICLE_STATE_AIRTIME, splash );
			//printf( "airtime a\n" );
			veh->wave_pitch = 0.0f;
			veh->wave_roll  = 0.0f;
			break;
		}
	} else {
		//printf("s: ");
		if ( veh->nextz < water_level ) {
			/* force no submersion for crash */
			if ( (!((veh->crash) && (veh->nextz > (int)veh->posz))) ) {
				/* want submerge */
				sub = TRUE;
				switch( veh->state ) {
				case VEHICLE_STATE_AIRTIME:
					if ( veh->onwave ) 
						pitch = (veh->pitch - veh->wave_pitch);
					else
						pitch = veh->pitch;
					sub = ( gfxAbs(pitch) > 10.0f && veh->vertical_speed <= -10.0f );
					//printf( "want submerged: vertical speed = %d\n", veh->vertical_speed );
					//if ( sub ) 
					//	printf( "sub pitch=%d, vs=%d\n", gfxAbs(pitch), (int)veh->vertical_speed );
					if ( (!sub) ) {
						/* stay on top of water */
						surfVehicleSetState( veh, VEHICLE_STATE_ONWATER, splash );
						//if ( veh->onwave )
						veh->nextz = water_level;
					}
					break;
				case VEHICLE_STATE_ONWATER:
					/* check for passing over the lip */
					if ( (veh->takeoffangle > 0.0f) && (veh->wave_pitch < 0.0f) &&
						 (veh->onwave) && (veh->last.face != veh->frontface) ) {				
						/* got air off lip */
						//printf( "airtime b\n" );
						surfVehicleSetState( veh, VEHICLE_STATE_AIRTIME, splash );
						veh->wave_pitch = 0.0f;
						veh->wave_roll  = 0.0f;
						sub = FALSE;
					} else {
						if ( veh->onwave ) {
							sub = ( gfxAbs(veh->pitch) > 10.0f && veh->vertical_speed <= -30.0f );
							// move from here to...
						} else {
							sub = ( gfxAbs(veh->pitch) > 10.0f && veh->vertical_speed <= -30.0f );
							if ( ! sub )
								sub = ( veh->nextz < (water_level - 30) );
						}
						// here... new for swell
						if ( ! sub ) 
							veh->nextz = water_level;
					}
					break;
				case VEHICLE_STATE_SUBMERGED:
					/* already done */
					sub = FALSE;
					veh->wave_pitch = 0.0f;
					veh->wave_roll  = 0.0f;
					break;
				}
				/* force no submersion for anything other than a jetski or sailboards */
				if ( veh->type == VEHICLE_TYPE_JETSKI || veh->type == VEHICLE_TYPE_PWC || veh->type == VEHICLE_TYPE_SAILBOARD ) {
#if 0
					/* don't allow submerge at top of peaking wave */
					if ( veh->wavepos.onwave && veh->wavepos.frontface && 
						(veh->wavepos.size > 0.5f) && (veh->wavepos.pos > 0.5f) ) {
						sub = FALSE;
						/* stay on top of water */
						surfVehicleSetState( veh, VEHICLE_STATE_ONWATER, splash );
						veh->nextz = water_level;
					}
#endif
				} else {
					sub = FALSE;
					/* stay on top of water */
					surfVehicleSetState( veh, VEHICLE_STATE_ONWATER, splash );
					veh->nextz = water_level;
				}
				/* do submersion if necessary */
				if ( sub ) {
					//printf("submerge: abspitch=%d, vspeed=%d\n", gfxAbs(pitch), veh->vertical_speed );
					surfVehicleSetState( veh, VEHICLE_STATE_SUBMERGED, splash );
					//printf("(wave pitch = %d, pitch = %d)...OK\n", veh->wave_pitch, veh->pitch );
					//printf( "submerge!\n" );
					veh->wave_pitch = 0.0f;
					veh->wave_roll  = 0.0f;
				}
			}
		} else {
			//printf("w: ");
			/* on top of the water */
			switch( veh->state ) {
			case VEHICLE_STATE_AIRTIME:
				/* force no submersion for crash */
				if ( veh->crash ) {
					//printf( "crash wants onwater z=%d, wl=%d\n", veh->nextz, water_level );
					sub = FALSE;
					veh->nextz = (water_level + 10);
				}
			case VEHICLE_STATE_SUBMERGED:
				surfVehicleSetState( veh, VEHICLE_STATE_ONWATER, splash );
				break;
			case VEHICLE_STATE_ONWATER:
				if ( veh->takeoffangle > 0.0f && veh->wave_pitch < 0.0f ) {
					/* got air over the lip */
					surfVehicleSetState( veh, VEHICLE_STATE_AIRTIME, splash );
					//printf( "airtime c\n" );
					veh->wave_pitch = 0.0f;
					veh->wave_roll  = 0.0f;
				} else {
					// new for swell...
					veh->nextz = water_level;
				}
			}
		}
	}

	/* update pitch to go flat */
	if ( (veh->state != VEHICLE_STATE_AIRTIME) || (((int)veh->posz) < veh->water_level + 30) ) 
		surfVehiclePitchFlatten( veh );

	/* save last */
	veh->last.posz = veh->posz;
	veh->last.vertical_speed = veh->vertical_speed;

	/* update z position */
	veh->vertical_speed = (((float)veh->nextz) - veh->posz) * tick;
	veh->posz = veh->nextz;

	/* update airz value */
	if ( veh->state == VEHICLE_STATE_AIRTIME ) {
		veh->airz = (veh->posz - veh->takeoffz);
		if ( veh->airz < 0.0f ) veh->airz = 0.0f;
	} else
		veh->airz = 0.0f;

	//printf("vehicle state=%d,posz=%.2f,onwave=%d,ff=%d,sh=%d\n", veh->state, veh->posz, veh->onwave, veh->frontface, (int)surfGetSwellHeight( swell, veh->posx, veh->posy ) );
}


/**********************************************************
 * Function:	surfVehicleSetState
 **********************************************************
 * Description: Update a surfing vehicle's state
 * Inputs:		veh    - vehicle pointer
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

void surfVehicleSetState (
	SurfVehicle		*veh,
	int				 state,
	SurfSplashData	*splash
	)
{
	float			 landing_pitch;

	/* validate */
	if ( state == veh->state ) return;

	switch( state ) {
	case VEHICLE_STATE_AIRTIME:
		switch( veh->state ) {
		case VEHICLE_STATE_SUBMERGED:
			veh->takeoffangle = veh->pitch;
			/* pass thru */
		case VEHICLE_STATE_ONWATER:
			veh->pitch = 0.0f;
			//surfMakeSplash( veh, SURF_SPLASH_TAKEOFF, splash );
			break;
		}
		break;
	case VEHICLE_STATE_SUBMERGED:
		switch( veh->state ) {
		case VEHICLE_STATE_AIRTIME:
			//surfMakeSplash( veh, SURF_SPLASH_LANDED, splash );
			/* pass thru */
		case VEHICLE_STATE_ONWATER:
			/* calc landing pitch */
			landing_pitch = surfVehicleLandingPitch( veh );
			veh->pitch += (landing_pitch - veh->wave_pitch);
			veh->takeoffangle = veh->pitch;
			//if ( veh->onwave ) {
			//	veh->pitch -= veh->wave_pitch;
			//	/* keep in range -179..180 degrees */
			//	surfKeepInRange( &veh->pitch, 180 );
			//}
			//veh->wave_pitch = 0;
			break;
		}
		break;
	case VEHICLE_STATE_ONWATER:
		switch( veh->state ) {
		case VEHICLE_STATE_AIRTIME:
			//surfMakeSplash( veh, SURF_SPLASH_LANDED, splash );
			/* pass thru */
		case VEHICLE_STATE_SUBMERGED:
			/* calc landing pitch */
			landing_pitch = surfVehicleLandingPitch( veh );
			veh->pitch += (landing_pitch - veh->wave_pitch);
			//veh->wave_roll = 0;	// test for sailboard....
			//if ( veh->onwave ) {
			//	veh->pitch -= veh->wave_pitch;
			//	/* keep in range -179..180 degrees */
			//	surfKeepInRange( &veh->pitch, 180 );
			//}
			//veh->wave_pitch = 0;
			break;
		}
		break;
	}

	/* set state */
	veh->state = state;
	if ( state == VEHICLE_STATE_SUBMERGED )
		veh->gravity *= 0.33333f; // divide by 3
	else
		veh->gravity = 0.0f;
	veh->gravity_rate = 0.0f;
}


/**********************************************************
 * Function:	surfVehicleLandingPitch
 **********************************************************
 * Description: Spit a vehicle
 * Inputs:		veh    - vehicle pointer
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

float surfVehicleLandingPitch ( 
	SurfVehicle		*veh
	)
{
	int		turn;
	int		abs_yaw;
	float	f;

	/* no turn so no change for landing angle */
	if ( veh->yaw == 0.0f )
		return( veh->takeoffangle );

	/* init */
	abs_yaw = gfxAbs((int)veh->yaw);
	turn = (abs_yaw % 90);

	/* 90 degree turn so no angle */
	if ( abs_yaw == 90 )
		return( 0.0f );

	/* 0=takeoffangle,90=0,179=-(takeoffangle) */
	if ( abs_yaw < 90 )
		turn = (90 - turn);
	f = ((float)turn) * 0.011111f * veh->takeoffangle;

	/* negate if turned more than 90 deg */
	if ( abs_yaw > 90 )
		f = -(f);

	//printf( "toa=%d,ayaw=%d,turn=%d,landa=%d\n", veh->takeoffangle, abs_yaw, turn, i );

	/* ok */
	return( f );
}


/**********************************************************
 * Function:	surfVehicleSpit
 **********************************************************
 * Description: Spit a vehicle
 * Inputs:		veh    - vehicle pointer
 *				angle  - angle change
 *				pitch  - pitch change
 *				speed  - speed change (factor)
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

void surfVehicleSpit ( 
	SurfVehicle		*veh, 
	float			 angle, 
	float			 pitch, 
	float			 speed,
	SurfSplashData	*splash
	)
{
	/* spit directional adjustment */
	veh->direction += angle;
	surfKeepInRange( &veh->direction, 360 );

	/* spit pitch adjustment */
	veh->pitch += pitch;
	surfKeepInRange( &veh->pitch, 180 );

	/* spit speed adjustment */
	veh->speed *= speed;

	/* set airtime state */
	veh->takeoffangle = (pitch + veh->wave_pitch);
	surfVehicleSetState( veh, VEHICLE_STATE_AIRTIME, splash );
	veh->gravity      = 0.0f;
	veh->takeoffz     = veh->posz;
	veh->takeoffroll  = veh->wave_roll;
	veh->wave_pitch   = 0.0f;
	veh->wave_roll    = 0.0f;
}


/**********************************************************
 * Function:	surfUpdateVehicleSpeed
 **********************************************************
 * Description: Update a surfing vehicle's speed
 * Inputs:		veh    - vehicle pointer
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

void surfUpdateVehicleSpeed (
	SurfVehicle	*veh,
	float		 tick
	)
{
	int			 water_level;
	Bool		 update;
	float		 size;
	float		 wave_push_speed;

	/* init */
	update = FALSE;
#if 0
	if ( veh->onwave )
		water_level = 0;
	else
		water_level = veh->water_level;
#else
	// new for swell...
	water_level = veh->water_level;
#endif

	/* no change unless on a wave */
	switch( veh->state ) {
	case VEHICLE_STATE_ONWATER:
		update = TRUE;
		break;
	case VEHICLE_STATE_AIRTIME:
		if ( veh->speed > 0 )
			veh->speed -= veh->speed * 0.0075f * tick; // 0.0075f
		if ( ((int)veh->posz) < (water_level + 30) )
			update = TRUE;	
		break;
	case VEHICLE_STATE_SUBMERGED:
		if ( veh->speed > 0.0f )
			veh->speed = veh->speed * 0.85f;
		break;
	}	

	/* update speed if requested */
	if ( update ) {
		if ( veh->onwave && veh->state == VEHICLE_STATE_ONWATER ) {
			/* ON WAVE FACE */
			if ( veh->speed_adjust > 1.0f ) {
				/* do not increase speed if no thrust and at maximum wave push speed */
				if ( (veh->thrust == 0.0f) ) { //&& veh->wavepos.frontface ) {
					size = veh->wavepos.size;
					if ( size >= 3.0f ) size -= 3.0f;
					if ( size >= 2.0f ) size = (3.0f - size);
					if ( size < 0.5f ) {
						wave_push_speed = veh->terminal_velocity * size * 2.0f;
						if ( veh->speed >= wave_push_speed ) {
							//printf( "speed=%.2f,wave push speed=%.2f,size=%.2f (skip)\n", veh->speed, wave_push_speed, size );
							goto skip_speed_update;
						}
					}
				}
				//printf( "spd=%.2f,fpos=%.2f,rate=%.2f,adj=%.2f,tick=%.2f",
				//	veh->speed, veh->facepos, veh->rate, veh->speed_adjust, tick );
				veh->speed += veh->speed * veh->facepos * (veh->rate + 1.0f) * (veh->speed_adjust - 1.0f) * tick;
				//printf( "result=%.2f\n", veh->speed );
skip_speed_update:
				;
			} else {
				/* can do better speed minimum than this (use rate) */
				if ( veh->speed < (veh->thrust * 0.5f) ) 
					veh->speed = (veh->thrust * 0.5f);
			}
		} 

		if ( veh->speed < veh->thrust ) {
			/* increase speed if we have thrust */
			if ( veh->speed < veh->terminal_velocity )
				veh->speed += (0.05f * veh->thrust) * tick;
		} else {
			/* degrade speed to simulate drag */
			if ( veh->speed > 0.0f ) {
				if ( veh->thrust ) {
					/* slowdown under power */
					veh->speed -= veh->speed * veh->slowdown * tick;
				} else {
					/* slowdown under no power */
					if ( veh->onwave ) {
						/* slowdown on wave */
						veh->speed -= veh->speed * veh->slowdown * tick;
					} else {
						/* slowdown not on wave is double */
						veh->speed -= veh->speed * veh->slowdown * 2.0f * tick;
					}
				}
			}
			/* more drag when more pitch */
			;
		}
	}
	
	//printf( "speed=%.2f,thrust=%.2f\n", veh->speed, veh->thrust );
	//printf("ow=%d,ff=%d\n", veh->onwave, veh->frontface );

	/* limit speed to terminal velocity */
	if ( veh->speed > veh->terminal_velocity ) 
		veh->speed = veh->terminal_velocity;
	if ( veh->speed < 0.0f )
		veh->speed = 0.0f;
}

