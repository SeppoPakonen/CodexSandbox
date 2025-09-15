/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		object.c
 * Author:		Mark Theyer
 * Created:		01 May 1997
 **********************************************************
 * Description: Surf game object related functions
 **********************************************************
 * Functions:
 *	fname() 
 *				func desc.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 01-May-97	Theyer	Initial coding.
 * 06-Aug-99	Theyer  Initial update for new game.
 * 31-May-01	Theyer  Ported to PS2
 *
 **********************************************************/

/*
 * includes
 */

#include <surfgame.h>
#include <gfx/gfxps2.h>

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

extern SurfData	surf;
extern Ps2Data  ps2;

/*
 * functions
 */


/**********************************************************
 * Function:	surfDrawObjects
 **********************************************************
 * Description: Draw surf objects
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfDrawObjects (
	void
    )
{
	SurfPlayer	*current;

	/* init */
	current = surf.current;

	/* validate */
	if ( current == NULL )
		surfExit( SURF_ERROR_CURRENT_IS_NULL );

	/* set the camera */
	surfSetCamera( &current->camera );

#if SURF_TESTMODE
	/* override camera? */
	if ( surf.test.mode == SURF_TESTMODE_VIEW )
		surfSetCamera( &surf.test.camera );
#endif

	/* beach */
	//surfDrawBottom( &surf.bottom );
	surfDrawBeach( &surf.beach );

#if 1
	/* draw rider and craft */
	surfDrawVehicle( &current->vehicle );
	personDraw( &current->player );
#endif

#if 1
	surfDrawWave( &surf.wave_render_data );
#endif

	// in-game graphics
	surfDrawInGame( &surf.ingame );
}

