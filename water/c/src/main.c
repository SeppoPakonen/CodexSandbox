/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		main.c
 * Author:		Mark Theyer
 * Created:		27 Sep 1999
 **********************************************************
 * Description: main menu and game loops
 **********************************************************
 * Functions:
 *	surfMainMenu() 
 *				game main menu.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 27-Sep-99	Theyer	Initial coding from wave test
 * 31-May-01	Theyer	New version for PS2
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

extern SurfData		surf;

/*
 * functions
 */

/**********************************************************
 * Function:	surfGameMainLoop
 **********************************************************
 * Description: game main menu loop
 * Inputs:		nil
 * Notes:		game starts here... called from main() in 
 *				surfgame.c
 * Returns:		void
 **********************************************************/

void surfGameMainLoop( void )
{
	/* determine state of controllers */
	surfInitControl();

	/* logo splash screen */
	gfxFilePath( SURF_PATH_MAPS );
	surfLoadMap( "intro.map", TRUE );
	//surfDisplayScreen( "gfxlogo.p2i", TRUE, TRUE );

	/* do a freeride game */
	while(1) 
		surfGameFreeRide();
}


/**********************************************************
 * Function:	surfGameFreeRide
 **********************************************************
 * Description: Free surfing mode game play
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 25-Oct-99	Theyer	Initial coding.
 *
 **********************************************************/

Bool surfGameFreeRide (
    void
	)
{
	SurfPlayer			*current;
	int					 i;

	/* test.... */
	surf.nplayers = 1;

	/* set playmode */
	surf.playmode = SURF_PLAYMODE_FREERIDE;

	/* load game */
	printf( "init game\n" );
	surfInitGame( 1 );
	
	/* load players */
	printf( "init player\n" );
	for ( i=0; i<surf.nplayers; i++ )
		surfInitPlayer( (i+1), 9, 0, 20, FALSE );

	/* select player 1 */
	surf.current = &surf.player[0];
	surf.active_player  = 1;
	surf.control.active = surfPlayerControlPad(surf.active_player);

	/* init */
	surf.paused = FALSE;

	/* start game */
	printf( "start game\n" );
	surfStartGame( surf.current );

	/* init for game play */
	current = surf.current;
	gfxResetTick();
	printf( "loop\n" );

	while(1) 
	{	
		/* update game */
		//printf( "update\n" );
		//FlushCache(0);
		if ( ! surf.paused )
			surfUpdateGame( current );

		/* swap graphics buffers */
		//printf( "swap\n" );
		//ushCache(0);
		gfxBufSwap();

		/* draw game objects */
		//printf( "draw\n" );
		//FlushCache(0);
		surfDrawObjects();

		/* update tick */
		surf.tick = gfxTick() * 60.0f;

		/* read the controller */
		//printf( "pad read\n" );
		//FlushCache(0);
		surfPadRead();

		/* pause */
		//if ( surfPadPause() ) {
		//	return(TRUE);
		//}

		/* abort test */
		if ( surf.abort_game ) break;

		/* process input events */
		//printf( "pr_events\n" );
		//FlushCache(0);
		surfProcessEvents();
	}

	return(TRUE);
}


