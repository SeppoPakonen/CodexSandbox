/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		jetski.c
 * Author:		Mark Theyer
 * Created:		04 May 2001
 **********************************************************
 * Description:	Jet ski event handling, init, load, update
 *				and draw functions
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 04-May-01	Theyer	Initial coding.
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

/*
 * variables
 */

extern SurfData    	surf;

/*
 * functions
 */


/**********************************************************
 * Function:	surfResetJetSki
 **********************************************************
 * Description: Prepare to play with the jet ski
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfResetJetSki (
	SurfJetSki	*jetski
	)
{
	jetski->state      = JETSKI_STATE_AS_PER_RIDER;
	jetski->current    = 0;
	jetski->last	   = 0;
	jetski->interpol   = 0.0f;
	jetski->move_speed = 0.1f;
	jetski->target     = JETSKI_STATE_READY;
	jetski->spin       = 0;
	jetski->roll       = 0;
	jetski->pitch      = 0;
	jetski->wet        = 1.0f;
	jetski->pitchrate  = 0;
	jetski->turnrate   = 0;
}


/**********************************************************
 * Function:	surfLoadJetSki
 **********************************************************
 * Description: Load a jet ski model
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfLoadJetSki (
    SurfJetSki	*jetski,
	int			 num,
	int			 player
    )
{
	char		 buff[64];

#ifdef SURF_USE_PAKS
	gfxFilePath( "models/jetskis" );
	sprintf( buff, "s%d.pak", num );
	gfxFileLoad( buff );
#endif

	/* locate models */
	sprintf( buff, "models/jetskis/s%d", num );
	gfxFilePath( buff );

	/* load texture locations map */
	sprintf( buff, "jski%dp%d.map", num, player );
	surfLoadMap( buff, FALSE );

    /* create model */
	sprintf( buff, "ski%d", num );
    if ( ! p2mModelToGfxObject( buff, (GfxObject **)&jetski->ski, NULL ) )
		surfExit(SURF_ERROR_JETSKI_LOAD_FAILED);

	// test...
	//gfxSetViewParent( jetski->ski );
}


/**********************************************************
 * Function:	surfDrawJetSki
 **********************************************************
 * Description: Draw jetski object
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfDrawJetSki (
    SurfJetSki	*jetski
    )
{   
	/* draw objects */
    gfxDrawObject( jetski->ski );
    gfxDrawObject( jetski->pole );
    gfxDrawObject( jetski->bars );
    gfxDrawObject( jetski->jet );
}


/**********************************************************
 * Function:	surfJetSkiUpdate
 **********************************************************
 * Description: Update jetski position
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfJetSkiUpdate (
	SurfJetSki	*jetski,
	SurfVehicle	*veh,
	PersonInfo	*rider
	)
{
	float		 roll;
	float		 pitch;

	/* position as per rider? */
	if ( jetski->state == JETSKI_STATE_AS_PER_RIDER ) {
		/* set position */
		gfxSetPosition( &jetski->ski, 4, rider->last, rider->current, rider->interpol );
		//return;
	}

	/* init values */
	if ( veh->state == VEHICLE_STATE_AIRTIME ) {
		//printf( "pitch=%d,roll=%d\n", (int)pitch, (int)roll );
		//printf( "air dir=%d, yaw=%d, spin=%d\n", (int)veh->direction, (int)veh->yaw, (int)jetski->spin );
		pitch = veh->takeoffangle;
		roll  = veh->takeoffroll;
	} else {
		//printf( "... dir=%d, yaw=%d, spin=%d\n", (int)veh->direction, (int)veh->yaw, (int)jetski->spin );
		pitch = veh->wave_pitch;
		roll  = veh->wave_roll;
	}

	/* position vehicle */
	gfxResetMatrix( veh->posobj );
	gfxScaleObject( veh->posobj, 0.1f, 0.1f, 0.1f );
	gfxRotateObject( veh->posobj, GFX_Y_AXIS,  (jetski->roll) );
	gfxRotateObject( veh->posobj, GFX_X_AXIS, -(jetski->pitch) );
	gfxRotateObject( veh->posobj, GFX_Z_AXIS,  (veh->yaw + jetski->spin) );
	gfxRotateObject( veh->posobj, GFX_Y_AXIS,  (veh->roll  + roll) );
	gfxRotateObject( veh->posobj, GFX_X_AXIS, -(veh->pitch + pitch) );
	gfxRotateObject( veh->posobj, GFX_Z_AXIS,  (veh->direction + 90.0f) );
	if ( veh->posz != 0.0f )
		gfxTranslateObject( veh->posobj, 0.0f, 0.0f, veh->posz );
}


/**********************************************************
 * Function:	surfJetSkiInPosition
 **********************************************************
 * Description: Check if jetski is in desired position
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

public Bool surfJetSkiInPosition(
    SurfJetSki	*jetski
    )
{
    return( ((jetski->current == jetski->last) && (jetski->interpol == 0.0f)) );
}


/**********************************************************
 * Function:	surfJetSkiSetThrust
 **********************************************************
 * Description: Set new desired position for jetski
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfJetSkiSetThrust (
	SurfVehicle	*veh,
    float		 thrust
    )
{
	SurfJetSki	*jetski;

	/* init */
	jetski = (SurfJetSki *)veh->craft;

	switch( veh->state ) {
	case VEHICLE_STATE_AIRTIME:
		if ( ((int)veh->posz) > (veh->wavepos.water_level + 30) ) {
			/* jet is drying out (losing thrust!) */
			if ( jetski->wet > 0.05f )
				jetski->wet -= 0.05f;
			else
				jetski->wet = 0.0f;
		}
		break;
	case VEHICLE_STATE_ONWATER:
	case VEHICLE_STATE_SUBMERGED:
		/* wet jet gradually */
		if ( jetski->wet < 1.0f )
			jetski->wet += 0.05f;
		if ( jetski->wet > 1.0f )
			jetski->wet = 1.0f;
		break;
	}

	if ( thrust > 0.0f ) {
		thrust *= jetski->wet;
		surfSetVehicleThrust( veh, thrust );
	} else
		surfSetVehicleThrust( veh, 0.0f );
}


/**********************************************************
 * Function:	surfJetSkiNextPosition
 **********************************************************
 * Description: 
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfJetSkiNextPosition (
    SurfJetSki	*jetski
    )
{
#if 0
	//printf( "current = %d\n", pilot->current );
	/* get next position for target */
	switch( jetski->target ) {
	case JETSKI_STATE_ON_TUMMY:
		/* are we in the desired position? */
		if ( jetski->current == JETSKI_POS_ON_TUMMY ) {
			/* update the state */
			jetski->state = JETSKI_STATE_ON_TUMMY;
			break;
		}
		/* climb onto jetski */
		if ( (jetski->current >= JETSKI_POS_BEHIND_IN_WATER) && (jetski->current <= JETSKI_POS_CLIMB_ON_12 ) )
			jetski->current++;
		break;
	}
#endif
}

