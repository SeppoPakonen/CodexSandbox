/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		camera.c
 * Author:		Mark Theyer
 * Created:		14 Jul 1999
 **********************************************************
 * Description:	Camera related functions for surf game.
 **********************************************************
 * Functions:
 *	surfInitCamera() 
 *				Initialise camera.
 *	surfUpdateCamera()
 *				Update camera eye and reference based on
 *				current position.
 *	surfSetCamera()
 *				Apply camera settings.
 *	surfNextCamera()
 *				Set next or previous camera position as the
 *				new position to use.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 14-Jul-99	Theyer	Initial coding from original game
 * 31-May-01	Theyer	New version for PS2
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
 * global variables
 */

extern SurfData	surf;

/*
 * functions
 */
          

/**********************************************************
 * Function:	surfInitCamera
 **********************************************************
 * Description: Initialise surf camera
 * Inputs:		camera - pointer to a camera data structure
 * Notes:		nil
 * Returns:		updated data structure
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 14-Jul-99	Theyer	Initial coding
 *
 **********************************************************/

void surfInitCamera (
    SurfCamera	*camera
    )
{
	/* default position is low front centre... */
	camera->pos		   = SURF_CAMERA_POS_LOW_FRONT_CENTRE;
	camera->delay      = SURF_CAMERA_DELAY;
	camera->lookat.x   =   0.0f;
	camera->lookat.y   =   0.0f;
	camera->lookat.z   = 100.0f;
	camera->angle      = SURF_CAMERA_ANGLE_LOW_FRONT_CENTRE;
	camera->pitch      = SURF_CAMERA_PITCH_LOW_FRONT_CENTRE;
	camera->dist       = SURF_CAMERA_DIST_LOW_FRONT_CENTRE;
	camera->zoom       =   0.0f;
	camera->focus      = camera->dist;
	camera->angle_inc  =   0.0f;
	camera->pitch_inc  =   0.0f;
	camera->dist_inc   =   0.0f;
	camera->inposition = TRUE;
	camera->transition =   0;
}        


/**********************************************************
 * Function:	surfUpdateCamera
 **********************************************************
 * Description: Update camera eye and focus points based
 *				on current position.
 * Inputs:	
 * Notes:	
 * Returns:		translation for wave
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 14-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

void surfUpdateCamera (
	SurfCamera		*camera,	 
	float			 posz
	)
{
	/* decrement camera delay counter */
	if ( camera->delay )
		camera->delay--;
	
#if 0
	/* update lookat position */
	camera->lookat.z = 100.0f + posz;
	if ( camera->lookat.z < 300.0f )
		camera->lookat.z = 300.0f;
	camera->lookat.z -= 200.0f;
#else
	camera->lookat.z = 50.0f + posz;
#endif

	// override
	//camera->lookat.z = 10.0f;

	/* update camera angle and pitch */
	if ( ! camera->inposition ) {
		/* update transition */
		camera->transition -= 1;
		/* reposition */
		camera->pitch += camera->pitch_inc;
		camera->angle += camera->angle_inc;
		camera->dist  += camera->dist_inc;
		/* in position ? */
		if ( camera->transition == 0 )
			camera->inposition = TRUE;
		//printf( "pos %d,p=%.2f, a=%.2f, d=%.2f\n", camera->pos, camera->pitch, camera->angle, camera->dist );
	}

	/* zoom */
	if ( posz > 500.0f ) {
		camera->zoom = ((posz - 500.0f) * 4.0f);
		if ( camera->zoom > 4000.0f )
			camera->zoom = 4000.0f;
	} else
		camera->zoom = 0.0f;
	camera->focus = (camera->dist - camera->zoom);

	// override
	//camera->focus = (camera->dist);
}


/**********************************************************
 * Function:	surfNextCamera
 **********************************************************
 * Description: Go to the next camera position
 * Inputs:		camera - camera to update position value
 *				next   - TRUE = next position, FALSE = previous
 * Notes:		
 * Returns:		updated camera
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 14-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

void surfNextCamera (
	SurfCamera *camera,
	Bool		next
	)
{
	int			last;

	/* check for delay */
	if ( camera->delay ) return;

	/* init */
	last = camera->pos;
	if ( next ) {
		/* go to next camera */
		if ( camera->pos < SURF_CAMERA_MAX_POS )
			camera->pos++;
		else
			camera->pos = SURF_CAMERA_MIN_POS;
	} else {
		/* go to previous camera */
		if ( camera->pos > SURF_CAMERA_MIN_POS )
			camera->pos--;
		else
			camera->pos = SURF_CAMERA_MAX_POS;
	}

	/* update */
	camera->delay      = SURF_CAMERA_DELAY;
	camera->inposition = FALSE;
	camera->transition = SURF_CAMERA_TRANSITION_STEPS;

	/* calculate increments for smooth transition */
	switch( camera->pos ) {
	case SURF_CAMERA_POS_HIGH_FRONT_RIGHT:
		camera->pitch_inc = (SURF_CAMERA_PITCH_HIGH_FRONT_RIGHT - camera->pitch)/SURF_CAMERA_TRANSITION_STEPS;
		camera->angle_inc = (SURF_CAMERA_ANGLE_HIGH_FRONT_RIGHT - camera->angle)/SURF_CAMERA_TRANSITION_STEPS;
		camera->dist_inc  = (SURF_CAMERA_DIST_HIGH_FRONT_RIGHT  - camera->dist)/SURF_CAMERA_TRANSITION_STEPS;
		break;
	case SURF_CAMERA_POS_LOW_FRONT_RIGHT:		
		camera->pitch_inc = (SURF_CAMERA_PITCH_LOW_FRONT_RIGHT - camera->pitch)/SURF_CAMERA_TRANSITION_STEPS;
		camera->angle_inc = (SURF_CAMERA_ANGLE_LOW_FRONT_RIGHT - camera->angle)/SURF_CAMERA_TRANSITION_STEPS;
		camera->dist_inc  = (SURF_CAMERA_DIST_LOW_FRONT_RIGHT  - camera->dist)/SURF_CAMERA_TRANSITION_STEPS;
		break;
	case SURF_CAMERA_POS_LOW_FRONT_CENTRE:		
		camera->pitch_inc = (SURF_CAMERA_PITCH_LOW_FRONT_CENTRE - camera->pitch)/SURF_CAMERA_TRANSITION_STEPS;
		camera->angle_inc = (SURF_CAMERA_ANGLE_LOW_FRONT_CENTRE - camera->angle)/SURF_CAMERA_TRANSITION_STEPS;
		camera->dist_inc  = (SURF_CAMERA_DIST_LOW_FRONT_CENTRE  - camera->dist)/SURF_CAMERA_TRANSITION_STEPS;
		break;	
	case SURF_CAMERA_POS_LOW_FRONT_LEFT:		
		camera->pitch_inc = (SURF_CAMERA_PITCH_LOW_FRONT_LEFT - camera->pitch)/SURF_CAMERA_TRANSITION_STEPS;
		camera->angle_inc = (SURF_CAMERA_ANGLE_LOW_FRONT_LEFT - camera->angle)/SURF_CAMERA_TRANSITION_STEPS;
		camera->dist_inc  = (SURF_CAMERA_DIST_LOW_FRONT_LEFT  - camera->dist)/SURF_CAMERA_TRANSITION_STEPS;
		break;
	case SURF_CAMERA_POS_HIGH_FRONT_LEFT:		
		camera->pitch_inc = (SURF_CAMERA_PITCH_HIGH_FRONT_LEFT - camera->pitch)/SURF_CAMERA_TRANSITION_STEPS;
		camera->angle_inc = (SURF_CAMERA_ANGLE_HIGH_FRONT_LEFT - camera->angle)/SURF_CAMERA_TRANSITION_STEPS;
		camera->dist_inc  = (SURF_CAMERA_DIST_HIGH_FRONT_LEFT  - camera->dist)/SURF_CAMERA_TRANSITION_STEPS;
		break;
	case SURF_CAMERA_POS_LOW_ONWAVE_LEFT:
		camera->pitch_inc = (SURF_CAMERA_PITCH_LOW_ONWAVE_LEFT - camera->pitch)/SURF_CAMERA_TRANSITION_STEPS;
		camera->angle_inc = (SURF_CAMERA_ANGLE_LOW_ONWAVE_LEFT - camera->angle)/SURF_CAMERA_TRANSITION_STEPS;
		camera->dist_inc  = (SURF_CAMERA_DIST_LOW_ONWAVE_LEFT  - camera->dist)/SURF_CAMERA_TRANSITION_STEPS;
		break;
	case SURF_CAMERA_POS_HIGH_ONWAVE_LEFT:
		camera->pitch_inc = (SURF_CAMERA_PITCH_HIGH_ONWAVE_LEFT - camera->pitch)/SURF_CAMERA_TRANSITION_STEPS;
		camera->angle_inc = (SURF_CAMERA_ANGLE_HIGH_ONWAVE_LEFT - camera->angle)/SURF_CAMERA_TRANSITION_STEPS;
		camera->dist_inc  = (SURF_CAMERA_DIST_HIGH_ONWAVE_LEFT  - camera->dist)/SURF_CAMERA_TRANSITION_STEPS;
		break;
	case SURF_CAMERA_POS_HIGH_ONWAVE_OVERHEAD:
		camera->pitch_inc = (SURF_CAMERA_PITCH_HIGH_ONWAVE_OVERHEAD - camera->pitch)/SURF_CAMERA_TRANSITION_STEPS;
		camera->angle_inc = (SURF_CAMERA_ANGLE_HIGH_ONWAVE_OVERHEAD - camera->angle)/SURF_CAMERA_TRANSITION_STEPS;
		camera->dist_inc  = (SURF_CAMERA_DIST_HIGH_ONWAVE_OVERHEAD  - camera->dist)/SURF_CAMERA_TRANSITION_STEPS;
		break;
	case SURF_CAMERA_POS_HIGH_ONWAVE_RIGHT:
		camera->pitch_inc = (SURF_CAMERA_PITCH_HIGH_ONWAVE_RIGHT - camera->pitch)/SURF_CAMERA_TRANSITION_STEPS;
		camera->angle_inc = (SURF_CAMERA_ANGLE_HIGH_ONWAVE_RIGHT - camera->angle)/SURF_CAMERA_TRANSITION_STEPS;
		camera->dist_inc  = (SURF_CAMERA_DIST_HIGH_ONWAVE_RIGHT  - camera->dist)/SURF_CAMERA_TRANSITION_STEPS;
		break;
	case SURF_CAMERA_POS_LOW_ONWAVE_RIGHT:
		camera->pitch_inc = (SURF_CAMERA_PITCH_LOW_ONWAVE_RIGHT - camera->pitch)/SURF_CAMERA_TRANSITION_STEPS;
		camera->angle_inc = (SURF_CAMERA_ANGLE_LOW_ONWAVE_RIGHT - camera->angle)/SURF_CAMERA_TRANSITION_STEPS;
		camera->dist_inc  = (SURF_CAMERA_DIST_LOW_ONWAVE_RIGHT  - camera->dist)/SURF_CAMERA_TRANSITION_STEPS;
		break;
	}
}



#if 0
	switch( camera->pos ) {
	case SURF_CAMERA_POS_HIGH_FRONT_RIGHT:
		break;
	case SURF_CAMERA_POS_LOW_FRONT_RIGHT:
		break;
	case SURF_CAMERA_POS_LOW_FRONT_CENTRE:		
		break;	
	case SURF_CAMERA_POS_LOW_FRONT_LEFT:		
		break;
	case SURF_CAMERA_POS_HIGH_FRONT_LEFT:		
		break;
	case SURF_CAMERA_POS_LOW_ONWAVE_LEFT:
		break;
	case SURF_CAMERA_POS_HIGH_ONWAVE_LEFT:
		break;
	case SURF_CAMERA_POS_HIGH_ONWAVE_OVERHEAD:
		break;
	case SURF_CAMERA_POS_HIGH_ONWAVE_RIGHT:
		break;
	case SURF_CAMERA_POS_LOW_ONWAVE_RIGHT:
		break;
	}
#endif
