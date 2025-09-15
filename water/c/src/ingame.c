/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		ingame.c
 * Author:		Mark Theyer
 * Created:		22 Aug 1999
 **********************************************************
 * Description:	In game graphics related functions
 **********************************************************
 * Functions:
 *	surfInitWave() 
 *				func desc.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 23-Jul-01	Theyer	Initial coding.
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
 * Function:	surfInitInGame
 **********************************************************
 * Description: Initialise on screen location map
 * Inputs:		map - map data
 * Notes:		nil
 * Returns:		initialised data structure
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 10-Aug-99	Theyer	Initial coding.
 *
 **********************************************************/

void surfInitInGame (
	SurfInGame		*ingame
	)
{
	//GfxTextureId	 texid;
	//GfxSpriteId	 sprite;

	/* init */
	memClear( ingame, sizeof(SurfInGame) );

#if 0 //def SURF_USE_PAKS
	/* load package */
	gfxFilePath( SURF_PATH_IMAGES );
	gfxFileLoad( SURF_PATH_INGAME_PAK );
#endif

#if 0
	/* locate images */
	//gfxFilePath( SURF_PATH_IMAGES_INGAME );
	gfxFilePath( "images/logos" );

	/* load and init logo */
	texid = p2iTextureToGfxTexture( "gfxlogo.p2i",  FALSE );
	ingame->gfxlogo = gfxSpriteFromTexture( texid );
	gfxSetSpriteScale( ingame->gfxlogo, 2.0f, 0.5f );
	gfxSetSpritePosition( ingame->gfxlogo, 0.55f, 0.85f );
	gfxSetSpriteTransparency( ingame->gfxlogo, 0.35f );
#endif
}


/**********************************************************
 * Function:	surfDrawInGame
 **********************************************************
 * Description: Draw the ingame graphics
 * Inputs:		wave     - pointer to a wave render data
 * Notes:		nil
 * Returns:		nil
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 10-Aug-99	Theyer	Initial coding.
 *
 **********************************************************/

void surfDrawInGame ( 
	SurfInGame	*ingame
	)
{
	//char		 str[1024];

	/* draw TheyerGFX logo */
	//gfxDrawSprite( ingame->gfxlogo, 0.0f );

#if 0
	// display vehicle position
	surfFontScale( &surf.font.num1, 0.7f, 0.7f );
	surfFontPosition( &surf.font.num1, 0.6f, 0.05f );
	if ( surf.current->vehicle.posz >= 0.0f )
		surfFontColor( &surf.font.num1, 1.0f, 1.0f, 1.0f );
	else
		surfFontColor( &surf.font.num1, 1.0f, 0.1f, 0.1f );
	sprintf( str, "%d %d %d\n", (int)surf.current->vehicle.posx, (int)surf.current->vehicle.posy, (int)surf.current->vehicle.posz );
	surfPrintFont( &surf.font.num1, str );
#endif

#if 0
	// display vehicle state with colour attribute
	switch( surf.current->vehicle.state ) {
	case 1: // air
		surfFontColor( &surf.font.num1, 0.0f, 1.0f, 0.0f );
		break;
	case 2: // on water
		surfFontColor( &surf.font.num1, 1.0f, 1.0f, 1.0f );
		break;
	case 3: // subed
		surfFontColor( &surf.font.num1, 1.0f, 0.0f, 0.0f );
		break;
	}
	sprintf( str, "%3d\n", surf.current->vehicle.state );
	surfPrintFont( &surf.font.num1, str );
#endif

#if 0
#if 1
	// display vehicle speed
	surfFontColor( &surf.font.num1, 1.0f, 1.0f, 1.0f );
	sprintf( str, "%d ", (int)surf.current->vehicle.speed );
	surfPrintFont( &surf.font.num1, str );
#endif

#if 1
	// display wave pitch
	surfFontColor( &surf.font.num1, 1.0f, 1.0f, 1.0f );
	sprintf( str, "%d ", (int)surf.current->vehicle.wave_pitch );
	surfPrintFont( &surf.font.num1, str );
#endif

#if 1
	// display wave size
	surfFontColor( &surf.font.num1, 1.0f, 1.0f, 1.0f );
	sprintf( str, " %.2f ", (float)surf.current->vehicle.wavepos.size );
	surfPrintFont( &surf.font.num1, str );
#endif
#endif
}

