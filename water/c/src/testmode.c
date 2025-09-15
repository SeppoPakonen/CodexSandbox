/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		testmode.c
 * Author:		Mark Theyer
 * Created:		12 Oct 1998
 **********************************************************
 * Description:	
 **********************************************************
 * Functions:
 *	fname() 
 *				func desc.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 12-Oct-98	Theyer	Initial coding.
 * 03-Aug-99	Theyer	Initial update for new game.
 * 03-Aug-99	Theyer	New version for PS2
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
 * Function:	surfInitTestMode
 **********************************************************
 * Description: Initialisation
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfInitTestMode (
    void
    )
{
#if SURF_TESTMODE
	/* init */
    surf.test.mode = SURF_TESTMODE_PLAY;

	/* initialise camera */
	surfInitCamera( &surf.test.camera );
#endif
}


/**********************************************************
 * Function:	surfTestModePrintData
 **********************************************************
 * Description: Initialisation
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfTestModePrintData (
	void 
	)
{
#if SURF_TESTMODE
	;
#endif
}


/**********************************************************
 * Function:	surfModeString
 **********************************************************
 * Description: Return string for current mode
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

Text surfModeString (
	int		mode
	)
{
#if SURF_TESTMODE
	switch( mode ) {
    case SURF_TESTMODE_VIEW:
		return( "View mode" );
        break;
    case SURF_TESTMODE_PLAY:
		return( "Play mode" );
		break;
	}
	return( "UNKNOWN" );
#else
	return(0);
#endif
}


/**********************************************************
 * Function:	surfViewModeProcess
 **********************************************************
 * Description: Handle events for view mode
 * Inputs:	pad - pad state
 *          padconfig - custom pad configuration
 * Notes:	
 * Returns:
 **********************************************************/

void surfViewModeProcess (
	void
    )
{
#if SURF_TESTMODE
	SurfCamera	*camera;

	/* init */
	camera = &surf.test.camera;

	/* update camera focus position */
	if ( surfPadDown( PAD_BUTTON_B ) )
		camera->lookat.x -= 10.0f;
	if ( surfPadDown( PAD_BUTTON_D ) )
		camera->lookat.x += 10.0f;
	if ( surfPadDown( PAD_BUTTON_A ) )
		camera->lookat.y -= 10.0f;
	if ( surfPadDown( PAD_BUTTON_C ) )
		camera->lookat.y += 10.0f;
	if ( surfPadDown( PAD_BUTTON_RIGHT_1 ) )
		camera->lookat.z += 10.0f;
	if ( surfPadDown( PAD_BUTTON_RIGHT_2 ) )
		camera->lookat.z -= 10.0f;

	/* update camera focus distance */
	if ( surfPadDown( PAD_BUTTON_LEFT_1 ) )
		camera->focus += 100.0f;
	if ( surfPadDown( PAD_BUTTON_LEFT_2 ) )
		camera->focus -= 100.0f;

	/* out the back */
	if ( surfPadDown( PAD_BUTTON_LEFT_1 ) ) {
		surf.current->vehicle.posx = surf.waveattr.drop_point_x;
		surf.current->vehicle.posy = surf.waveattr.drop_point_y;
	}

	/* update camera pitch and angle */
	if ( surfPadDown( PAD_BUTTON_UP ) )
		camera->pitch++;
	if ( surfPadDown( PAD_BUTTON_DOWN ) )
		camera->pitch--;
	if ( surfPadDown( PAD_BUTTON_LEFT ) )
		camera->angle--;
	if ( surfPadDown( PAD_BUTTON_RIGHT ) )
		camera->angle++;

	/* limit */
	if ( camera->angle >= 360.0f ) camera->angle -= 360.0f;
	if ( camera->angle < 0.0f ) camera->angle += 360.0f;

#if SURF_ENABLE_SAVESCREEN
	/* dump screen */
	if ( surfPadPressed( PAD_BUTTON_SELECT ) )
		surfTestModeDumpScreen();
#else
	if ( surfPadPressed( PAD_BUTTON_SELECT ) ) {
		gfxClose();
		gfxAbort();
	}
#endif

	//printf( "viewmode camera p=%.2f, a=%.2f, d=%.2f\n", camera->pitch, camera->angle, camera->dist );

#endif
}



/**********************************************************
 * Function:	surfTestModeDumpScreen
 **********************************************************
 * Description: Dump screen if signalled
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jun-00	Theyer	Initial coding.
 *
 **********************************************************/

void surfTestModeDumpScreen (
	void
	)
{
#if 0 //SURF_ENABLE_SAVESCREEN
	static int	delay = 60;

	if ( delay ) {
		delay--;
	} else {
		if ( padAnyButtonDown( PAD_BUTTON_SELECT ) != PAD_NONE ) {
			/* save screen */
			gfxFilePath( "saves" );
			gfxSaveScreen( "screen", 0 );
			delay = 60;
		}
	}
#else
	/* draw to cleared display buffer */
	surfDrawObjects();
	gfxBufSwap();
	surfDrawObjects();
	/* save screen */
	gfxFilePath( "saves" );
	gfxSaveScreen( "screen", 0 );
#endif
}

