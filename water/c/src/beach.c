/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		beach.c
 * Author:		Mark Theyer
 * Created:		02 Sep 1999
 **********************************************************
 * Description:	Beach background related functions
 **********************************************************
 * Functions:
 *	surfInitBeach() 
 *				func desc.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 02-Sep-99	Theyer	Initial coding.
 * 25-Jul-01	Theyer	PS2 port.
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
 * Function:	surfInitBeach
 **********************************************************
 * Description: Initialise beach
 * Inputs:		beach - beach data pointer
 *				name  - name of beach file to load
 * Notes:		nil
 * Returns:		initialised data structure
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 02-Sep-99	Theyer	Initial coding.
 *
 **********************************************************/

void surfInitBeach (
	SurfBeach	*beach,
	int			 num
	)
{
	char		 buff[32];

	/* set active beach */
	//surf.active_beach = num;

	/* temporary override */
	//num = 4;

#ifdef SURF_USE_PAKS
	gfxFilePath( "models\\beaches" );
	sprintf( buff, "b%d.pak", num );
	gfxFileLoad( buff );
#endif

	/* locate model */
	sprintf( buff, "models/beaches/b%d", num );
	gfxFilePath( buff );

	/* load texture location information */
	sprintf( buff, "beach%d.map", num );
	surfLoadMap( buff, FALSE );

	/* init beach cube name */
	//sprintf( buff, "beach%d", num );

    /* create model */
    if ( ! p2mModelToGfxObject( "east", (GfxObject **)&beach->east, NULL ) )
		surfExit(SURF_ERROR_BEACH_LOAD_FAILED);
    /* create model */
    if ( ! p2mModelToGfxObject( "north", (GfxObject **)&beach->north, NULL ) )
		surfExit(SURF_ERROR_BEACH_LOAD_FAILED);
    /* create model */
    if ( ! p2mModelToGfxObject( "west", (GfxObject **)&beach->west, NULL ) )
		surfExit(SURF_ERROR_BEACH_LOAD_FAILED);

	/* init front view does not want east and west beach drawn */
	beach->draw_east = TRUE;
	beach->draw_west = TRUE;
}


/**********************************************************
 * Function:	surfDrawBeach
 **********************************************************
 * Description: Draw the beach background
 * Inputs:		beach - beach data pointer
 *				angle - camera angle
 * Notes:		nil
 * Returns:		nil
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 02-Sep-99	Theyer	Initial coding.
 *
 **********************************************************/

void surfDrawBeach ( 
	SurfBeach	*beach
	)
{
	gfxDrawObject( beach->north );
	if ( beach->draw_east )
		gfxDrawObject( beach->east );
	if ( beach->draw_west )
		gfxDrawObject( beach->west );
}

#if 0
/**********************************************************
 * Function:	surfDrawBeachEast
 **********************************************************
 * Description: Draw the east beach background
 * Inputs:		beach - beach data pointer
 *				angle - camera angle
 * Notes:		nil
 * Returns:		nil
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 02-Sep-99	Theyer	Initial coding.
 *
 **********************************************************/

void surfBeachDrawEast ( 
	SurfBeach	*beach,
	Bool		 draw
	)
{
	beach->draw_east = draw;
}


/**********************************************************
 * Function:	surfDrawBeachWest
 **********************************************************
 * Description: Draw the east beach background
 * Inputs:		beach - beach data pointer
 *				angle - camera angle
 * Notes:		nil
 * Returns:		nil
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 02-Sep-99	Theyer	Initial coding.
 *
 **********************************************************/

void surfBeachDrawWest ( 
	SurfBeach	*beach,
	Bool		 draw
	)
{
	beach->draw_west = draw;
}
#endif

