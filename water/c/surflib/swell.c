/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		swell.c
 * Author:		Mark Theyer
 * Created:		7 Jul 2001
 **********************************************************
 * Description:	Swell modifier for water level
 **********************************************************
 * Functions:
 *	surfInitSwell() 
 *				Initialise the water swell.
 *	surfGetSwellHeight()
 *				Get the swell height at this x,y position
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

#include <swell.h>
#include <surfutil.h>
#include <gfx/gfxps2.h>
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
 * Function:	surfInitSwell
 **********************************************************
 * Description: Initialise swell modifier
 * Inputs:		swell - pointer to a swell data structure
 * Notes:		nil
 * Returns:		initialised data structure 
 **********************************************************/

void surfInitSwell ( 
	WaveSwell   *swell
	)
{
	WaveSwellData	*sw;
	//int			 angle;

	// manual init for testing
	memClear( swell, sizeof(WaveSwell) );

#if 0
	swell->nswells =  1;

	sw = &swell->data[0];
	sw->axis 	  =  WAVE_SWELL_AXIS_X;
	sw->position  =       0;
	sw->length    =  0x2000;
	sw->mask      =  (sw->length - 1);
	sw->normalise =  (0x40000000 / sw->length);
	sw->peak      =  100.0f;
	sw->trough    =  -50.0f;
	sw->height    = (sw->peak - sw->trough);
	sw->speed     =   10;
	//for ( angle=0; angle<=180; angle++ )
	//	sw->saved[angle] = (sw->peak - (triSinFAST(angle) * sw->height));
#else
	swell->nswells = 1;

	sw = &swell->data[0];
	sw->axis 	  =  WAVE_SWELL_AXIS_X;
	sw->position  =       0;
	sw->length    =  0x2000;
	sw->mask      =  (sw->length - 1);
	sw->normalise =  (0x40000000 / sw->length);
	sw->peak      =  100.0f;
	sw->trough    =  -50.0f;
	sw->height    = (sw->peak - sw->trough);
	sw->speed     =   10;
	//for ( angle=0; angle<=180; angle++ )
	//	sw->saved[angle] = (sw->peak - (triSinFAST(angle) * sw->height));

	sw = &swell->data[1];
	sw->axis 	  =  WAVE_SWELL_AXIS_X;
	sw->position  =       0;
	sw->length    =  0x1000;
	sw->mask      =  (sw->length - 1);
	sw->normalise =  (0x40000000 / sw->length);
	sw->peak      =   30.0f;
	sw->trough    =  -30.0f;
	sw->height    = (sw->peak - sw->trough);
	sw->speed     =  -20;
	//for ( angle=0; angle<=180; angle++ )
	//	sw->saved[angle] = (sw->peak - (triSinFAST(angle) * sw->height));

	sw = &swell->data[2];
	sw->axis 	  =  WAVE_SWELL_AXIS_Y;
	sw->position  =       0;
	sw->length    =  0x2000;
	sw->mask      =  (sw->length - 1);
	sw->normalise =  (0x40000000 / sw->length);
	sw->peak      =  100.0f;
	sw->trough    =  -50.0f;
	sw->height    = (sw->peak - sw->trough);
	sw->speed     =  -5;
	//for ( angle=0; angle<=180; angle++ )
	//	sw->saved[angle] = (sw->peak - (triSinFAST(angle) * sw->height));
#endif
}


/**********************************************************
 * Function:	surfUpdateSwell
 **********************************************************
 * Description: Move/update the swell modifier
 * Inputs:		swell - pointer to a swell data structure
 * Notes:		nil
 * Returns:		updated data structure 
 **********************************************************/

void surfUpdateSwell ( 
	WaveSwell   *swell
	)
{
	WaveSwellData	*sw;
	int				 i;

	// manual update for testing
	for ( i=0; i<swell->nswells; i++ ) {
		//sw = (WaveSwellData *)PS2_UNCACHED(&swell->data[i]);
		sw = (WaveSwellData *)&swell->data[i];
		sw->position += sw->speed;
		if ( sw->position >= sw->length )
			sw->position -= sw->length;
		if ( sw->position < 0 )
			sw->position += sw->length;
	}
}


/**********************************************************
 * Function:	surfGetSwellHeights
 **********************************************************
 * Description: Get the height of the swell at this x y point
 * Inputs:		swell - pointer to the swell data structure
 *				x - x position
 *				y - y position
 * Notes:		
 * Returns:		swell height
 **********************************************************/

void surfGetSwellHeights ( 
	WaveSwell		*swell,
	float			 x, 
	float			 y, 
	Point_f3d		*pts,
	int				 npts,
	float			*highest_z,
	float			*highest_y
	)
{
#if 1
	int				 ix, iy;
	int				 pos;
	float			 height;
	WaveSwellData	*sw;
	int				 i;
	int				 w;
	int				 nswells;

	// init
	if ( highest_z ) {
		*highest_z = -10000.0f;
		*highest_y = -10000.0f;
	}
	ix      = (int)x;
	nswells = swell->nswells;

	for ( w=0; w<npts; w++, pts++ ) {
		// init
		height = 0.0f;
		iy = (int)(y + pts->y);

		for ( i=0; i<nswells; i++ ) {

			// init
			sw = &swell->data[i];

			// get value of 0 to 0x40000000
			if ( sw->axis == WAVE_SWELL_AXIS_X )
				pos = ((ix + sw->position) & sw->mask) * sw->normalise;
			else
				pos = ((iy + sw->position) & sw->mask) * sw->normalise;
			pos >>= 12;
			pos *= 180;
			pos >>= 18;

			// OK what is z value?
			if ( pos < 0 || pos > 180 ) {
				printf( "pos error=%d\n", pos );
				surfExit(1);
			}
			//height += *(sw->saved + pos);
			height += (sw->peak - (triSinFAST(pos) * sw->height));
		}

		// update z
		pts->z += height;

		// update wave height if this point is higher and peak point location
		if ( highest_z ) {
			if ( pts->z > *highest_z ) {
				*highest_z = pts->z;
				*highest_y = pts->y;
			}
		}
	}
#endif
}


/**********************************************************
 * Function:	surfGetSwellHeight
 **********************************************************
 * Description: Get the height of the swell at this x y point
 * Inputs:		swell - pointer to the swell data structure
 *				x - x position
 *				y - y position
 * Notes:		
 * Returns:		swell height
 **********************************************************/

float surfGetSwellHeight ( 
	WaveSwell		*swell,
	float			 fx, 
	float			 fy 
	)
{
#if 1
	int				 ix, iy;
	int				 pos;
	float			 height;
	WaveSwellData	*sw;
	int				 i;
	int				 nswells;

	// init
	ix      = (int)fx;
	iy      = (int)fy;
	height  = 0.0f;
	nswells = swell->nswells;

	// manual update for testing
	for ( i=0; i<nswells; i++ ) {

		// init
		sw = &swell->data[i];

		// get value of 0 to 0x40000000
		if ( sw->axis == WAVE_SWELL_AXIS_X )
			pos = ((ix + sw->position) & sw->mask) * sw->normalise;
		else
			pos = ((iy + sw->position) & sw->mask) * sw->normalise;
		pos >>= 12;
		pos *= 180;
		pos >>= 18;

		// OK what is z value?
		if ( pos < 0 || pos > 180 ) {
			printf( "pos error=%d\n", pos );
			surfExit(1);
		}
		//height += *(sw->saved + pos);
		height += (sw->peak - (triSinFAST(pos) * sw->height));
	}

	return( height );
#else
	return( 0.0f );
#endif
}



#if 0
void surfCalcSwellHeights (
	WaveAttributes	*attr, 
	WaveSwell		*swell, 
	int				 type,
	int				 nsec, 
	Point_f3d		*f_pts, 
	float			 x, 
	float			 xtrans, 
	float			 y 
	)
{

}
#endif

