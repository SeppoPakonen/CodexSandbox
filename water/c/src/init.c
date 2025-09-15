/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		init.c
 * Author:		Mark Theyer
 * Created:		04 May 2001
 **********************************************************
 * Description:	
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

//#include <surftxt.h>

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
 * Function:	surfInit
 **********************************************************
 * Description: Init stuff for startup
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

void surfInit (
    void
    )
{   
	/* clear memory */
	memClear( &surf, sizeof(SurfData) );

//#define HOME "host0:/theyer/projects/theyergfx/bragg_surf/data"
#define HOME "host0:/theyernet1/Projects/TheyerGFX/California_Surfing_PS2/bragg_surf/data"

#if SURF_PROTOTYPE_VERSION
	/* init demo data files */
	surfInitDemo();
	gfxFileRoot( HOME );
#else
//
#ifdef GFX_USE_CDROM
	/* CD-ROM/DVD data files */
	gfxFileRoot( "cdrom0:" );
#else
	/* local HARD DRIVE files */
	gfxFileRoot( HOME );
#endif
//
#endif
}


/**********************************************************
 * Function:	surfInitFonts
 **********************************************************
 * Description: Load and init game fonts
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 * 28-Jun-01	Theyer	New version for PS2
 *
 **********************************************************/

void surfInitFonts (
	void
	)
{
#ifdef SURF_USE_PAKS
	/* load package */
	gfxFilePath( SURF_PATH_IMAGES );
	gfxFileLoad( SURF_PATH_FONTS_PAK );
#endif

	/* set path */
	gfxFilePath( SURF_PATH_IMAGES_FONTS );

	/* number font */
	surfInitFont( &surf.font.num1, surf.font.num1_chars, surf.font.num1_sizes, 48, 57, 0.03f, 0.1f );
	surfLoadNumFont( &surf.font.num1 );

	/* font1 */
	surfInitFont( &surf.font.font1, surf.font.font1_chars, surf.font.font1_sizes, 44, 122, 0.03f, 0.1f );
	surfLoadFont1( &surf.font.font1 );

#if 0
	/* load and init loading bar */
	texid = mifTextureToGfxTexture( "loading.mib",  FALSE );
	surf.loading.sprite = gfxSpriteFromTexture( texid );
	gfxSetSpritePosition( surf.loading.sprite, 1024, 3800 );
#endif
}


/**********************************************************
 * Function:	surfInitGame
 **********************************************************
 * Description: Load and init game depending on selected options
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

void surfInitGame (
	int		beachnum
    )
{
	/* reset the graphics system */
	gfxReset( GFX_RESET_ALL );

	/* re-init controllers */
	surfInitControl();

#if SURF_TESTMODE
	/* test mode stuff */
	surfInitTestMode();
#endif

	/* load main map */
	gfxFilePath( SURF_PATH_MAPS );
	surfLoadMap( "main.map", TRUE );

	/* load fonts */
	surfInitFonts();

	/* in-game graphics */
	surfInitInGame( &surf.ingame );

	/* set flag */
	surf.abort_game = FALSE;

	/* init lighting */
	gfxSetAmbient( 0.5f, 0.5f, 0.5f );
	//gfxSetFlatLight( 1, 0.0f, 0.0f, -1.0f, 0.8f, 0.3f, 0.3f );

	if ( beachnum ) {	
		/* beach */
		//surfInitBeach( &surf.beach, 1 );	// test beach
		surfInitBeach( &surf.beach, 2 );	// old CWS beach 2
		//surfInitBottom( &surf.bottom, 1 );
		/* init wave stuff */
		//surfInitSwell( &surf.swell );
		//surfInitAlpha( &surf.alpha );
		//surfInitWaveGenerator( &surf.wavegen, beachnum );
		surfInitWaveAttributes( &surf.waveattr, beachnum );	
		surfInitWaveZone( &surf.zone, &surf.waveattr );
		surfInitContour( &surf.contour, beachnum );	
		surfInitWave( &surf.wave_render_data, &surf.waveattr, &surf.savedpts );
		surf.wave = surfGetViewableWaves( &surf.zone, 0, surf.waves, &surf.nwaves, SURF_MAX_WAVES );
	}
}


/**********************************************************
 * Function:	surfInitPlayer
 **********************************************************
 * Description: Init game for player
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfInitPlayer (
	int		player,
	int		rider,
	int		diff,
	int		board,
	Bool	robot
	)
{
	SurfPlayer	*current;
	GfxObject	*object;
	char		 buff[32];
	int			 i;

	/* init */
	i = player - 1;
	surf.current = &surf.player[i];
	current = surf.current;

	/* clear memory */
	memClear( &current->player, sizeof(PersonInfo) );

	/* robot player */
	current->robot = robot;

	/* init camera */
	surfInitCamera( &current->camera );

#if 0
	/* load and initialise jetski */
	surfLoadJetSki( &current->jetski, board, player );
	/* load jetski positional data */
	sprintf( buff, "jetski%d.p2a", board );
	surfLoadPos( current->jetski.ski, buff );
	surfResetJetSki( &current->jetski );

	/* load pilot */
	object = surfLoadPilot( &current->player, rider, player );

	/* create person */
    personCreate( &current->player, object, &current->vehicle, 
			(PersonEventCallback *)surfPlayJetSkiingProcess, (PersonNextPositionCallback *)surfPilotNextPosition );

	/* create vehicle */
	surfInitVehicle( &current->vehicle, VEHICLE_TYPE_JETSKI, current->jetski.ski, 
			(void*)&current->jetski, (void *)&current->player, 
			SURF_JETSKI_TERMINAL_VELOCITY, SURF_JETSKI_SLOWDOWN, 
			(SurfVehicleUpdateCallback *)surfJetSkiUpdate,
			(SurfVehicleInPositionCallback *)surfJetSkiInPosition,
			(SurfVehicleNextPositionCallback *)surfJetSkiNextPosition,
			(SurfVehicleDrawCallback *)surfDrawJetSki );
	
	/* load pilot positional data */
	sprintf( buff, "pilot%d.p2a", board );
	surfLoadPos( personObject(&current->player), buff );
	surfResetPilot( &current->player );
#else
	/* load and initialise surfboard */
	printf( "load board\n" );
	surfLoadSurfboard( &current->board, board, player );
	/* load surfboard positional data */
	//sprintf( buff, "surfbd6.p2a" );
	printf( "load board animation\n" );
	sprintf( buff, "b8.p2a" );
	surfLoadPos( current->board.object, buff );
	surfResetSurfboard( &current->board );

	/* load surfer */
	printf( "load surfer\n" );
	object = surfLoadSurfer( &current->player, rider, player );

	/* create person */
	printf( "create person\n" );
    personCreate( &current->player, object, &current->vehicle, 
			(PersonEventCallback *)surfPlaySurfingProcess, (PersonNextPositionCallback *)surfSurferNextPosition );

	/* create vehicle */
	printf( "init vehicle\n" );
	surfInitVehicle( &current->vehicle, VEHICLE_TYPE_SURFBOARD, current->board.object, 
			(void*)&current->board, (void *)&current->player, 
			SURF_BOARD_TERMINAL_VELOCITY, SURF_BOARD_SLOWDOWN, 
			(SurfVehicleUpdateCallback *)surfBoardUpdate,
			(SurfVehicleInPositionCallback *)surfBoardInPosition,
			(SurfVehicleNextPositionCallback *)surfBoardNextPosition,
			(SurfVehicleDrawCallback *)surfDrawSurfboard );
	
	/* load surfer positional data */
	//sprintf( buff, "tania6.p2a" );
	printf( "load surfer animation\n" );
	sprintf( buff, "s8.p2a" );
	surfLoadPos( personObject(&current->player), buff );
	surfResetSurfer( &current->player );
#endif

	/* update position */
	current->posx = current->vehicle.posx;
	current->posy = current->vehicle.posy;

	/* init crash flag */
	current->crash = FALSE;
}


/**********************************************************
 * Function:	surfLoadScreen
 **********************************************************
 * Description: Load a full res title screen or backdrop
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

GfxSpriteId surfLoadScreen (
	Text	name
	)
{	
	GfxTextureId		texid;
	GfxSpriteId		    spid;

	/* set path */
	gfxFilePath( SURF_PATH_IMAGES_SCREENS );

	/* load image */
	texid = p2iTextureToGfxTexture( name, TRUE );
	if ( texid == GFX_NO_TEXTURE )
		surfExit(SURF_ERROR_SCREEN_LOAD_FAILED);
	spid = gfxSpriteFromTexture( texid );

	/* locate */
	gfxSetSpritePosition( spid, 0.0f, 0.0f );
	gfxSetSpriteScale( spid, 1.25f, 0.875f );
	gfxSetSpriteTransparency( spid, 0.0f );

	return( spid );
}


/**********************************************************
 * Function:	surfDisplayScreen
 **********************************************************
 * Description: Display a full res title screen
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

Bool surfDisplayScreen (
	Text	name,
	Bool	fade_in,
	Bool	interrupt
	)
{	
	GfxSpriteId		 spid;
	float			 f;

	/* load */
	spid = surfLoadScreen( name );

	if ( fade_in ) {
		for ( f=0.0f; f<1.0f; f+=0.02f ) {
			gfxSetSpriteBrightness( spid, f );
			gfxDrawSprite( spid, 0.0f );
			gfxBufSwap();
			/* interrupt? */
			if ( interrupt ) {
				padRead();
				if ( padAnyButtonDown( (PAD_BUTTON_A|PAD_BUTTON_START) ) != PAD_NONE )
					return( TRUE );
			}
		}
		gfxSetSpriteBrightness( spid, 1.0f );
		gfxDrawSprite( spid, 0.0f );
		gfxBufSwap();
	} else {
		/* splat screen onto both frame buffers */
		gfxDrawSprite( spid, 0.0f );
		gfxBufSwap();
		gfxDrawSprite( spid, 0.0f );
		gfxBufSwap();
	}

	return( FALSE );
}

