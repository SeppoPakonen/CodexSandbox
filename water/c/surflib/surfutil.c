/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		surfutil.c
 * Author:		Mark Theyer
 * Created:		26 Dec 1998
 **********************************************************
 * Description:	Surf game miscellaneous supporting functions.
 **********************************************************
 * Functions:
 *	noprint()
 *				Used to disable debug printf statements() 
 *	surfExit()
 *				Exit from the surf game.
 *	adjustToMax()
 *				Adjust a movement to a limited maximum.
 *	memEqual()
 *				Check if 2 memory blocks are the same.
 *	adjustValue()
 *				Adjust a position angle value.
 *	surfKeepInRange()
 *				Force a value to be in a specified range.
 *	surfWait()
 *				Wait for n seconds before continuing...
 *	surfSetResolution()
 *				Set high or low screen resolution.
 *	surfBlankScreen()
 *				Draw a blank (black) screen.
 *	surfLoadMap()
 *				Set current PlayStation texture map file.
 *	surfLoadPos()
 *				Load position animation file for an object
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 26-Dec-97	Theyer	Initial Coding
 * 15-Jul-99	Theyer	Initial conversion to new game
 * 31-May-01	Theyer	Initial conversion to PS2
 *
 **********************************************************/

/*
 * includes
 */

#include <surfutil.h>
#include <waverr.h>

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

/*
 * functions
 */


/**********************************************************
 * Function:	noprint
 **********************************************************
 * Description: Used to disable debug printf statements
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void noprint( 
	char *format, 
	... 
	)
{
	;
}


/**********************************************************
 * Function:	surfExit
 **********************************************************
 * Description: Exit the game
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfExit (
	int exitcode 
	)
{ 
	printf( "surf error: %d\n", exitcode );
	gfxClose();
	exit(1);
}


/**********************************************************
 * Function:	adjustToMax
 **********************************************************
 * Description: Adjust a movement to a limited maximum 
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void adjustToMax( int *diff, int max )
{ 
    if ( *diff > 0 ) {
        if ( *diff > max )
		    *diff = max;
    } else { 
        if ( *diff < -(max) )
		    *diff = -(max);
    }
}


/**********************************************************
 * Function:	memEqual
 **********************************************************
 * Description: Check if 2 memory blocks are the same
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

public Bool memEqual (
    int		*ptr1,
    int		*ptr2,
    int		 size
    )
{
    register    int 	i, *p1, *p2, sz;

    /* init */
    p1 = ptr1;
    p2 = ptr2;
    sz = size;
    for ( i=0; i<sz; i++ ) {
    	if ( *p1 != *p2 )
            return( FALSE );
        p1++;
        p2++;
    }
    return( TRUE );
}


/**********************************************************
 * Function:	adjustValue
 **********************************************************
 * Description: Adjust a position angle value
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void adjustValue (
    int   *current,
    int   *wanted,
    int    max
    )
{
    int 	diff;

    /* adjust value */
    if ( *current != *wanted ) {
		diff = *wanted - *current;
        adjustToMax( &diff, max );
		*(current) += diff;
    }
}


/**********************************************************
 * Function:	surfKeepInRange
 **********************************************************
 * Description: Keep value in the range 0..359
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfKeepInRange (
	float	*value,
	int		 range
	)
{
	switch( range ) {
	case 180:
		while( *value > 180.0f )
			*value -= 360.0f;
		while( *value < -179.0f )
			*value += 360.0f;
		break;
	case 360:
		while( *value > 359.0f )
			*value -= 360.0f;
		while( *value < 0.0f )
			*value += 360.0f;
		break;
	default:
		//printf("KeepInRange: bad range value (%d)\n", range );
		surfExit( UTIL_ERROR_BAD_RANGE_VALUE );
	}
}


/**********************************************************
 * Function:	surfWait
 **********************************************************
 * Description: Wait for n seconds before continuing...
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

Bool surfWait (
	int		nsecs,
	Bool	interrupt
	)
{
	float	tick;
	float	timeout;

	/* init */
	tick    = 0.0f;
	timeout = (nsecs * GFX_TICKS_PER_SECOND);

	gfxResetTick();
	while( tick < timeout ) {
		/* check for interrupt */
		if ( interrupt ) {
			padRead();
			if ( padAnyButtonPressed( (PAD_BUTTON_A|PAD_BUTTON_SELECT|PAD_BUTTON_START) ) != PAD_NONE )
				return( TRUE );
		}
		/* update time count */
		tick += gfxTick();
		//gfxResetTick();
	}

	return( FALSE );
}


/**********************************************************
 * Function:	surfSetResolution
 **********************************************************
 * Description: Set high or low resolution
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfSetResolution (
	Bool	high
	)
{
	if ( gfxGetVideoMode() == GFX_VIDEO_MODE_PAL ) {
		/* PAL */
		if ( high ) 
			gfxSetResolution( 512, 256 );
		else
			gfxSetResolution( 384, 256 );
	} else {
		/* NTSC */
		if ( high )
			gfxSetResolution( 512, 240 );
		else
			gfxSetResolution( 384, 240 );
	}
}


/**********************************************************
 * Function:	surfBlankScreen
 **********************************************************
 * Description: Draw a blank (black) screen
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfBlankScreen (
	void 
	)
{
	gfxSetBackgroundColor( 0, 0, 0 );
	gfxBufSwap();
	gfxBufSwap();
}


/**********************************************************
 * Function:	surfHeightToUnit
 **********************************************************
 * Description: convert height value to string in feet or metres
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfHeightToUnit ( 
	int		 height, 
	char	*buff, 
	Bool	 metres
	)
{
	int		 tmp;

	/* check for zero */
	if ( height <= 0 ) {
		txtCopy( " 0.00", buff );
		return;
	}

	if ( metres ) {
		//printf( "metres\n" );
		// metres conversion (110 height units = 1.00m)
		tmp = (height / 110);
		sprintf( buff, "%2d", tmp );
		buff[2] = '.';
		tmp = (height % 110);
		if ( tmp ) {
			tmp -= (tmp / 11);
			sprintf( &buff[3], "%2d", tmp );
			if ( buff[3] == ' ' ) buff[3] = '0';
			buff[5] = '\0';
		} else {
			sprintf( &buff[3], "00" );
		}
	} else {
		//printf( "feet\n" );
		// feet conversion (34 height units = 1ft)
		tmp = (height / 34);
		sprintf( buff, "%2d", tmp );
		buff[2] = '.';
		tmp = (height % 34);
		if ( tmp ) {
			tmp -= 2;
			sprintf( &buff[3], "%2d", tmp );
			if ( buff[3] == ' ' ) buff[3] = '0';
			buff[5] = '\0';
		} else {
			sprintf( &buff[3], "00" );
		}
	}
}


/**********************************************************
 * Function:	surfFloatString
 **********************************************************
 * Description: Convert a fixed value to a floating point 
 *				character string
 * Inputs:		value (range expected 0..10)
 * Notes:	
 * Returns:
 **********************************************************/

void surfFloatString ( 
	fixed	 value,
	char	*buff
	)
{
	int		 tmp;

	/* check for zero */
	if ( value <= 0 ) {
		txtCopy( " 0.00", buff );
		return;
	}

	// conversion (fixed point value 4096 = float value 1.0)
	tmp = (value / 4096);
	sprintf( buff, "%2d", tmp );
	buff[2] = '.';
	tmp = (value % 4096);
	if ( tmp ) {
		tmp /= 41;
		sprintf( &buff[3], "%2d", tmp );
		if ( buff[3] == ' ' ) buff[3] = '0';
		buff[5] = '\0';
	} else {
		sprintf( &buff[3], "00" );
	}
}


/**********************************************************
 * Function:	surfLoadMap
 **********************************************************
 * Description: Set current PlayStation texture map file
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfLoadMap (
    Text	mapname,
	Bool	clear
	)
{
	Byte	*mapfile;

	/* set file path */
	//gfxFilePath( "maps" );

	/* load file */
	mapfile = gfxFileLoad( mapname );
	/* validate */
	if ( mapfile == NULL ) {
		//printf( "Failed to load texture map location file: %s\n", mapname );
		surfExit( UTIL_ERROR_MAP_LOAD_FAILED );
	}
	/* set playstation specific info */
	gfxLoadMap( mapfile, clear );
	//printf( "Loaded texture map location file: %s\n", mapname );
}


/**********************************************************
 * Function:	surfLoadPos
 **********************************************************
 * Description: Load a position data file
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void surfLoadPos (
	GfxObject	*obj,
    Text		 posname
	)
{
	Byte	*posfile;
	//int	 nframes;
	int		 size;

	/* set file path */
	gfxFilePath( "pos" );

	/* load file */
	posfile = gfxFileLoad( posname );
	/* validate */
	if ( posfile == NULL ) {
		//printf( "Failed to load position file: %s\n", posname );
		surfExit( UTIL_ERROR_POSITION_LOAD_FAILED );
	}

	/* get nframes */
	//nframes  = binReadInt( (posfile + 4) );
	//nentries = binReadInt( (posfile + 8) );
	//size = gfxFileSize() - 16;
	size = gfxFileSize();

	/* set playstation animation data for this object (skipo header) */
	//gfxPs2AddPositions( obj, (posfile + 16), nframes, size );
	//printf( "Loaded position file: %s\n", posname );
	gfxAddObjectData( obj, posfile, size, GFX_PS2_ANIMATION_DATA );
}

