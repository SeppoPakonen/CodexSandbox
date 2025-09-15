/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		events.c
 * Author:		Mark Theyer
 * Created:		04 Jul 1997
 **********************************************************
 * Description:	Surf game event handling
 **********************************************************
 * Functions:
 *	fname() 
 *				func desc.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 04-Jul-97	Theyer	Initial Coding
 * 15-Jul-99	Theyer	Initial update for new game
 * 31-May-01	Theyer	Port to PS2
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

extern SurfData		surf;

/*
 * functions
 */


/**********************************************************
 * Function:	surfProcessEvents
 **********************************************************
 * Description: High level event handler
 * Inputs:	control - state of controller(s)
 * Notes:	
 * Returns:
 **********************************************************/

void surfProcessEvents (
    void
    )
{ 
#if SURF_TESTMODE
	/* test modes */
    switch( surf.test.mode ) {
    case SURF_TESTMODE_VIEW:
		/* next test mode */
		if ( surfPadPressed( PAD_BUTTON_START ) )
			surf.test.mode = SURF_TESTMODE_PLAY;
		/* camera view mode */
		surf.paused = TRUE;
        surfViewModeProcess();
        break;            
    case SURF_TESTMODE_PLAY:
		/* quit game */
		if ( surfPadPressed( PAD_BUTTON_SELECT ) ) {
			gfxClose();
			gfxAbort();
		}
		/* next test mode */
		if ( surfPadPressed( PAD_BUTTON_START ) ) {
			/* copy current camera */
			surf.test.camera = surf.current->camera;
			surf.test.mode = SURF_TESTMODE_VIEW;
		}
		/* play the game */
		surf.paused = FALSE;
        surfPlayModeProcess();
        break;
    }
#else
   	surfPlayModeProcess();	
#endif
}



/**********************************************************
 * Function:	surfPlayModeProcess
 **********************************************************
 * Description: Handle events for play mode
 * Inputs:
 *        
 * Notes:	
 * Returns:
 **********************************************************/

void surfPlayModeProcess (
	void
    )
{
	/* process events */
	personProcessEvents( &surf.current->player );

#if SURF_SSX_STYLE_CONTROLS
	/* update camera */
	if ( ! surf.replay ) {
		/* camera positions */
		if ( surfPadPressed( PAD_BUTTON_C ) )
			surfNextCamera( &surf.current->camera, TRUE );
	}
#else
	/* update camera */
	if ( ! surf.replay ) {
		/* camera positions */
		if ( surfPadPressed( PAD_BUTTON_LEFT_1 ) )
			surfNextCamera( &surf.current->camera, FALSE );
		else if ( surfPadPressed( PAD_BUTTON_LEFT_2 ) )
			surfNextCamera( &surf.current->camera, TRUE );
	}
#endif
}


