/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		alpha.c
 * Author:		Mark Theyer
 * Created:		7 Jul 2001
 **********************************************************
 * Description:	Alpha modifier for water level
 **********************************************************
 * Functions:
 *	surfInitAlpha() 
 *				Initialise the water alpha.
 *	surfGetWaveAlpha()
 *				Get the water transparency modification value 
 *				at this x,y position
 **********************************************************
 *
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 07-Jul-01	Theyer	Initial coding for PS2
 *
 **********************************************************/

/*
 * includes
 */

#include <alpha.h>
#include <surfutil.h>
#include <memory/memory.h>
#include <triangle/triangle.h>
#include <waverr.h>

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

/*
 * functions
 */

/**********************************************************
 * Function:	surfInitAlpha
 **********************************************************
 * Description: Initialise alpha modifier
 * Inputs:		alpha - pointer to a alpha data structure
 * Notes:		nil
 * Returns:		initialised data structure 
 **********************************************************/

void surfInitAlpha ( 
	WaveAlpha   *alpha
	)
{
	WaveAlphaData	*al;
	int				 angle;

	// manual init for testing
	memClear( alpha, sizeof(WaveAlpha) );

#if 0
	alpha->nalpha =  1;

	al = &alpha->data[0];
	al->axis 	  =  WAVE_ALPHA_AXIS_X;
	al->position  =     0;
	al->length    =  8000;
	al->normalise =  1.0f/(((float)al->length) * 0.5f);
	al->peak      =  100.0f;
	al->trough    =  -50.0f;
	al->height    = (al->peak - al->trough);
	al->speed     =   10;
#else
	alpha->nalpha = 1;

	al = &alpha->data[0];
	al->axis 	  =  WAVE_ALPHA_AXIS_X;
	al->position  =       0;
	al->length    =  0x2000;
	al->mask      =  (al->length - 1);
	al->normalise =  (0x40000000 / al->length);
	al->peak      =   40.0f;
	al->trough    =   10.0f;
	al->height    = (al->peak - al->trough);
	al->speed     =    8;
	for ( angle=0; angle<=180; angle++ )
		al->saved[angle] = (int)(al->peak - (triSinFAST(angle) * al->height));

	al = &alpha->data[1];
	al->axis 	  =  WAVE_ALPHA_AXIS_Y;
	al->position  =       0;
	al->length    =  0x2000;
	al->mask      =  (al->length - 1);
	al->normalise =  (0x40000000 / al->length);
	al->peak      =   20.0f;
	al->trough    =  -20.0f;
	al->height    = (al->peak - al->trough);
	al->speed     =   20;
	for ( angle=0; angle<=180; angle++ )
		al->saved[angle] = (int)(al->peak - (triSinFAST(angle) * al->height));
#endif
}


/**********************************************************
 * Function:	surfUpdateAlpha
 **********************************************************
 * Description: Move/update the alpha modifier
 * Inputs:		alpha - pointer to a alpha data structure
 * Notes:		nil
 * Returns:		updated data structure 
 **********************************************************/

void surfUpdateAlpha ( 
	WaveAlpha   *alpha
	)
{
	WaveAlphaData	*al;
	int				 i;

	// manual update for testing
	for ( i=0; i<alpha->nalpha; i++ ) {
		al = &alpha->data[i];
		al->position += al->speed;
		if ( al->position >= al->length )
			al->position -= al->length;
		if ( al->position < 0 )
			al->position += al->length;
	}
}


/**********************************************************
 * Function:	surfGetAlphaHeight
 **********************************************************
 * Description: Get the water transparency (alpha) at this x y point
 * Inputs:		alpha - pointer to the alpha data structure
 *				x - x position
 *				y - y position
 * Notes:		
 * Returns:		alpha height
 **********************************************************/

int surfGetWaveAlpha ( 
	WaveAlpha		*walpha,
	float			 fx, 
	float			 fy 
	)
{
#if 1
	int				 x, y;
	int				 pos;
	int				 alpha;
	WaveAlphaData	*al;
	int				 i;

	// init
	x      = (int)fx;
	y      = (int)fy;
	alpha  = 0;

	// manual update for testing
	for ( i=0; i<walpha->nalpha; i++ ) {

		// init
		al = &walpha->data[i];

		// get value of 0 to 0x40000000
		if ( al->axis == WAVE_ALPHA_AXIS_X )
			pos = ((x + al->position) & al->mask) * al->normalise;
		else
			pos = ((y + al->position) & al->mask) * al->normalise;
		pos >>= 12;
		pos *= 180;
		pos >>= 18;

		// OK what is z value?
		if ( pos < 0 || pos > 180 ) {
			printf( "pos error=%d\n", pos );
			surfExit(1);
		}
			
		// OK what is z value?
		alpha += *(al->saved + pos);
	}

	return( alpha );
#else
	return( 0 );
#endif
}

