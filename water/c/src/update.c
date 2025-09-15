/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		update.c
 * Author:		Mark Theyer
 * Created:		30 Dec 1998
 **********************************************************
 * Description:	Game update related functions
 **********************************************************
 * Functions:
 *	fname() 
 *				func desc.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 30-Dec-98	Theyer	Initial coding.
 * 06-Aug-99	Theyer	Initial update for new game.
 * 31-May-01	Theyer	Port to PS2
 *
 **********************************************************/

/*
 * includes
 */

#include <surfgame.h>

/*
 * macros
 */

/*
 * typedefs
 */

/*
 * prototypes
 */

/*
 * global variables
 */

extern	SurfData	surf;

/*
 * functions
 */


/**********************************************************
 * Function:	surfUpdateGame
 **********************************************************
 * Description: Update game for next frame
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 06-Aug-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

void surfUpdateGame (
	SurfPlayer *current
    )
{
	SurfVehicle *veh;
	float		 rate;

	/* init */
	veh = &current->vehicle;

	/* update the wave */
	//surfUpdateSwell( &surf.swell );
	//surfUpdateAlpha( &surf.alpha );
	surfUpdateWaveZone( &surf.zone, surf.tick );
	surf.wave = surfGetViewableWaves( &surf.zone, (current->posy - 3000), surf.waves, &surf.nwaves, SURF_MAX_WAVES );
	surfUpdateWave( &surf.wave_render_data, &surf.contour, current->posx, current->posy, surf.wave->position );
	surfScrollWave( &surf.wave_render_data );

	/* update rider */
	if ( personInPosition( &current->player ) ) 
		personNextPosition( &current->player );
	personUpdate( &current->player, surf.tick );

	/* update speed */
	surfUpdateVehicleSpeed( veh, surf.tick );

	/* update vehicle if it is on this wave */		
	//if ( surf.wave ) 
	//	surfVehicleOnWave( &surf.waveattr, surf.wave, &surf.wave_render_data, &surf.contour, &surf.swell, &surf.savedpts, veh, surf.zone.wave_speed, surf.tick );
	/* adjust the position */
	surfUpdateVehicleAirPosition( veh, &surf.swell, NULL, surf.tick );
	
	/* update vehicle */
	if ( surfVehicleInPosition( veh ) )
		surfVehicleNextPosition( veh, current->player.current );
	//surfUpdateVehicle( veh );

	/* put vehicle in new position */
	rate = surfUpdateVehiclePosition( veh, NULL, surf.tick );
	
	/* limit x,y position */
	if ( veh->posx < 0.0f ) veh->posx += surfContourWidth(&surf.contour);
	if ( veh->posx > surfContourWidth(&surf.contour) ) veh->posx -= surfContourWidth(&surf.contour);
	if ( veh->posy > 114688.0f ) veh->posy -= 16384.0f;
	if ( veh->posy < 0.0f ) veh->posy = 0.0f;
	
	/* get next z position in 3D space */
	surfVehicleNextAirPosition( veh, surf.tick );

	/* update position */
	current->posx = veh->posx;
	current->posy = veh->posy;
	//printf( "x=%d,y=%d\n", (int)veh->posx, (int)veh->posy );

	// temp
	veh->posz = surfWaterLevel( surf.wave, &surf.contour, &surf.savedpts, current->posx, current->posy );

	/* update camera (do this last for latest posz) */
	surfUpdateCamera( &current->camera, veh->posz );

#if 0
	// handle sync for landings (change of vehicle state)
	switch( veh->type ) {
	case VEHICLE_TYPE_JETSKI:
	case VEHICLE_TYPE_PWC:
		surfChangePilotPitch( veh, 0 );
		surfChangePilotRoll( veh, 0, 0 );
		surfPilotSpin( veh, 0 );
		break;
#if 0
	case VEHICLE_TYPE_SAILBOARD:
		surfChangeSailorPitch( veh, 0 );
		surfSailorSpin( veh, 0 );
		break;
	case VEHICLE_TYPE_BODYBOARD:
		surfChangeRiderPitch( veh, 0 );
		surfChangeRiderRoll( veh, 0, 0 );
		if ( (veh->state == VEHICLE_STATE_AIRTIME) || (veh->last.state == VEHICLE_STATE_AIRTIME) )
			surfRiderSpin( veh, 0 );
		break;
	case VEHICLE_TYPE_SURFBOARD:
		surfChangeSurferPitch( veh, 0 );
		surfSurferSpin( veh, 0 );
		break;
#endif
	}
#endif
	surfUpdateVehicle( veh );
}


/**********************************************************
 * Function:	surfStartGame
 **********************************************************
 * Description: Update game for first 2 display frames
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 22-Oct-99	Theyer	Initial coding.
 *
 **********************************************************/

void surfStartGame (
	SurfPlayer *current
    )
{
	Bool	saved;

	/* save paused state */
	saved = surf.paused;
	surf.paused = FALSE;
	
	/* out the back */
	current->vehicle.posx = surf.waveattr.drop_point_x;
	current->vehicle.posy = surf.waveattr.drop_point_y;

	/* update position */
	current->posx = current->vehicle.posx;
	current->posy = current->vehicle.posy;

	/* set and reset the camera */
	surfResetCamera( &current->camera );

	/* restore paused flag */
	surf.paused = saved;

	/* init last wave position */
	current->last_wave_position = 0x0FFFFFFF;
}

