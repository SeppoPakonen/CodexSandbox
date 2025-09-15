/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		surfbrd.c
 * Author:		Mark Theyer
 * Created:		8 Dec 1997
 **********************************************************
 * Description:	Surfboard related functions.
 **********************************************************
 * Functions:
 *	surfInitSurfboard() 
 *				Initialise surfboard.
 *	surfBuildSurfboard()
 *				Load surfboard model and initialise.
 *	surfDrawSurfboard()
 *				Draw the surfboard model.
 *	surfBoardInPosition()
 *				Is the surfboard in position?
 *	surfBoardSetPosition()
 *				Set the position for the surfboard.
 *	surfBoardUpdate()
 *				Update the surfboard in 3D space.
 *	surfBoardNextPosition()
 *				Set the position for the surfboard depending
 *				on the position of the rider.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 08-Dec-97	Theyer	Initial Coding
 * 13-Jul-99	Theyer	Initial update for new game
 * 31-Jul-01	Theyer	PS2 port.
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
 * Function:	surfResetSurfboard
 **********************************************************
 * Description: Reset the surf board
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 27-Jul-00	Theyer	Initial coding.
 *
 **********************************************************/

void surfResetSurfboard (
	SurfBoard	*board
	)
{
	board->state      = SURF_BOARD_STATE_AS_PER_RIDER;
	board->current    = 0;
	board->last		  = 0;
	board->interpol   = 0.0f;
	board->move_speed = 0.1f;
	board->target     = SURF_BOARD_STATE_READY;
	board->spin       = 0.0f;
	board->roll       = 0.0f;
	board->pitch      = 0.0f;
}


/**********************************************************
 * Function:	surfLoadSurfboard
 **********************************************************
 * Description: Create surf board object
 * Inputs:	
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

void surfLoadSurfboard (
    SurfBoard	*board,
	int			 num,
	int			 player
    )
{
	char	buff[64];

#ifdef SURF_USE_PAKS
	gfxFilePath( "models/boards" );
	sprintf( buff, "s%d.pak", num );
	gfxFileLoad( buff );
#endif

	/* locate models */
	sprintf( buff, "models/boards/s%d", num );
	gfxFilePath( buff );

	/* load texture locations map */
	sprintf( buff, "s%dp%d.map", num, player );
	surfLoadMap( buff, FALSE );

	sprintf( buff, "b%d", num );
    /* create model */
    if ( ! p2mModelToGfxObject( buff, (GfxObject **)&board->object, NULL ) )
		surfExit(SURF_ERROR_SURFBOARD_LOAD_FAILED);

	//gfxRotateObject( board->object, GFX_Z_AXIS, 90 );
    //board->mat = gfxGetObjectMatrix( board->object );
}


/**********************************************************
 * Function:	surfDrawSurfboard
 **********************************************************
 * Description: Draw surfboard object
 * Inputs:	
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

void surfDrawSurfboard (
    SurfBoard	*board
    )
{         
	gfxDrawObject( board->object );
}


/**********************************************************
 * Function:	surfBoardInPosition
 **********************************************************
 * Description: Check if surfboard is in desired position
 * Inputs:	
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

public Bool surfBoardInPosition(
    SurfBoard	*board
    )
{
	//if ( board->state == SURF_BOARD_STATE_AS_PER_RIDER )
	//	return( TRUE );
    return( ((board->current == board->last) && (board->interpol == 0.0f)) );
}


/**********************************************************
 * Function:	surfBoardUpdate
 **********************************************************
 * Description: Update board position
 * Inputs:	
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

public void surfBoardUpdate (
	SurfBoard	*board,
	SurfVehicle	*veh,
	PersonInfo	*rider
	)
{
	float		 roll;
	float		 pitch;

	/* position as per rider? */
	if ( board->state == SURF_BOARD_STATE_AS_PER_RIDER ) {
		/* set position */
		gfxSetPosition( &board->object, 1, rider->last, rider->current, rider->interpol );
	}

	/* init values */
	if ( veh->state == VEHICLE_STATE_AIRTIME ) {
		pitch = veh->takeoffangle;
		roll  = veh->takeoffroll;
	} else {
		pitch = veh->wave_pitch;
		roll  = veh->wave_roll;
	}

	/* position vehicle */
	gfxResetMatrix( veh->posobj );
	gfxScaleObject( veh->posobj, 0.1f, 0.1f, 0.1f );
	//printf( "br=%d,bp=%d,y=%d,spin=%d,vr=%d,r=%d,vp=%d,p=%d,vd=%d\n",
	//	(int)board->roll, (int)board->pitch, (int)veh->yaw, (int)board->spin,
	//	(int)veh->roll, (int)roll, (int)veh->pitch, (int)pitch, (int)veh->direction );
	gfxRotateObject( veh->posobj, GFX_Y_AXIS,  (board->roll) );
	gfxRotateObject( veh->posobj, GFX_X_AXIS, -(board->pitch) );
	gfxRotateObject( veh->posobj, GFX_Z_AXIS,  (veh->yaw + board->spin) );
	gfxRotateObject( veh->posobj, GFX_Y_AXIS,  (veh->roll  + roll) );
	gfxRotateObject( veh->posobj, GFX_X_AXIS, -(veh->pitch + pitch) );
	gfxRotateObject( veh->posobj, GFX_Z_AXIS,  (veh->direction + 90.0f) );
	if ( veh->posz != 0.0f )
		gfxTranslateObject( veh->posobj, 0.0f, 0.0f, veh->posz );
}


/**********************************************************
 * Function:	surfBoardNextPosition
 **********************************************************
 * Description: Check if surfboard is in desired position
 * Inputs:	
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

public void surfBoardNextPosition(
    SurfBoard	*board
    )
{
	;
}
