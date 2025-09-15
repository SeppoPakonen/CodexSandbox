/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		bottom.c
 * Author:		Mark Theyer
 * Created:		02 Sep 1999
 **********************************************************
 * Description:	Beach seabed/bottom related functions
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
 * Function:	surfInitBottom
 **********************************************************
 * Description: Initialise bottom
 * Inputs:		bottom - bottom data pointer
 *				name  - name of bottom file to load
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

void surfInitBottom (
	SurfBottom	*bottom,
	int			 num
	)
{
	char		 buff[32];

	/* set active bottom */
	//surf.active_bottom = num;

	/* temporary override */
	num = 1;

#ifdef SURF_USE_PAKS
	gfxFilePath( "models\\bottoms" );
	sprintf( buff, "b%d.pak", num );
	gfxFileLoad( buff );
#endif

	/* locate model */
	sprintf( buff, "models/bottoms/b%d", num );
	gfxFilePath( buff );

	/* load texture location information */
	sprintf( buff, "bottom%d.map", num );
	surfLoadMap( buff, FALSE );

	/* init bottom cube name */
	//sprintf( buff, "bottom%d", num );

    /* create model */
    if ( ! p2mModelToGfxObject( "b1", (GfxObject **)&bottom->object, NULL ) )
		surfExit(SURF_ERROR_BEACH_LOAD_FAILED);
}


/**********************************************************
 * Function:	surfDrawBottom
 **********************************************************
 * Description: Draw the bottom background
 * Inputs:		bottom - bottom data pointer
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

void surfDrawBottom ( 
	SurfBottom	*bottom
	)
{
	gfxResetMatrix( bottom->object );
	gfxTranslateObject( bottom->object, -(surf.current->posx) + 40000.0f, -(surf.current->posy) + 60000.0f, 0.0f );
	//gfxScaleObject( bottom->object, 10.0f, 10.0f, 10.0f );
	gfxDrawObject( bottom->object );
}

