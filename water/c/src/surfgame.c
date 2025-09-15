/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		Jetracer2 PlayStation2 jetski racing game
 **********************************************************
 * File:		jetracer2.c
 * Author:		Mark Theyer
 * Created:		23 Apr 2001
 **********************************************************
 * Description:	
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 23-Apr-01	Theyer	Initial coding.
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

SurfData	surf ALIGN128;

/*
 * functions
 */

/**********************************************************
 * Function:	main
 **********************************************************
 * Description: Main program
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

/* standard PlayStation entry */
int main( void )
{
    GfxInitArgs		  args[3];
    int	    		  nargs;
	int				  mode;

	/* set up args and init graphics system */
	nargs = 0;
	args[0].arg  = GFX_INITARG_SETVIDEOMODE;
#if SURF_VIDEO_MODE_NTSC
	mode = GFX_VIDEO_MODE_NTSC;
#else
	mode = GFX_VIDEO_MODE_PAL;
#endif
	args[0].data = (void *)&mode;
	nargs++;

	/* init */
	//memCardInit(0);
    padInit();
	gfxInit( args, nargs );

#if 0
	/* local HARD DRIVE files */
	gfxFileRoot( "host0:/theyernet1/Projects/TheyerGFX/California_Surfing_PS2/bragg_surf/data" );

	/* load main map */
	gfxFilePath( SURF_PATH_MAPS );
	surfLoadMap( "main.map", TRUE );
	/* set path */
	gfxFilePath( SURF_PATH_IMAGES_FONTS );
	/* font1 */
	surfInitFont( &surf.font.font1, surf.font.font1_chars, surf.font.font1_sizes, 44, 122, 0.02f, 0.1f );
	surfLoadFont2( &surf.font.font1 );
	
	/* run game */
	rider_test();
	//summer_test();
	//wave_test2();
	//wave_test3();
#else
	/* init */
	surfInit();
	surfGameMainLoop();
#endif

	/* stop all sound effects */
	//surfResetSound( &surf.sound );

    /* clean up */
	//sndClose();
    gfxClose();

	/* done */
    return(0);
}

