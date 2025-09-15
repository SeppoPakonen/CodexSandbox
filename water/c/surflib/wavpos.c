/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		wavpos.c
 * Author:		Mark Theyer
 * Created:		13 Jul 1999
 **********************************************************
 * Description:	Wave generation and rendering functions.
 *				A wave has a description which is in a
 *				zone. A zone is a group of waves which is
 *				managed and updated by a wave generator. A
 *				wave generator describes a wave environment
 *			    and attributes, etc.
 **********************************************************
 * Functions:
 *	surfWavePosition()
 *				Calculate position on a wave face.
 *	surfCurlWavePos() 
 *				Calculate position on wave curl.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 13-Jul-99	Theyer	Initial coding 
 * 04-Jun-01	Theyer	Port for PS2
 *
 **********************************************************/
/*
 * includes
 */

#include <wave.h>

/*
 * macros
 */

/*
 * typedefs
 */

/*
 * prototypes
 */

float	z_position( Point_f3d *pts, float y );

/*
 * global variables
 */

/*
 * functions
 */

/**********************************************************
 * Function:	surfWavePosition
 **********************************************************
 * Description: get position on this wave
 * Inputs:		waveattr - wave attribute data
 *				waveinfo - wave position data
 *				wavepos  - wave position data to initialise
 *				contour	 - surface contour data
 *				saved	 - saved wave points
 *				veh		 - vehicle data
 *				wave_speed - current wave speed on water
 *				tick     - game timer tick
 * Notes:		nil
 * Returns:		TRUE if on a wave, FALSE if not
 **********************************************************/

void surfWavePosition (
	WaveTypeData	 *waveinfo,
	SurfContour		 *contour,
	WaveSavedPoints  *saved,
	WavePositionData *wavepos,
	float			  x,
	float			  y,
	float			  dir
    )
{         
	float			 water_level;
	float			 pitch;
	float			 roll;
	float			 rate;
	float			 adjust;
	float			 size;
	Point_f3d		*pts;

	/* init */
	surfKeepInRange( &dir, 360 );

	/* get water level */
	size = surfGetWaveHeight( contour, x, waveinfo->position );
	pts  = surfCalcWavePoints( size, saved );
	water_level = z_position( pts, (y - waveinfo->position) );

	/* set rate and adjust values */
	if ( dir <= 90.0f ) {
		rate   = dir * WAVE_ONE_OVER_NINETY;
		adjust = WAVE_FFACE_SPEEDHIT;
	} else if ( dir <= 180.0f ) {
		rate   = 1.0f - ((dir - 90.0f) * WAVE_ONE_OVER_NINETY);
		adjust = WAVE_FFACE_SPEEDHIT;
	} else if ( dir <= 270.0f ) {
		rate   = (dir - 180.0f) * WAVE_ONE_OVER_NINETY;
		adjust = WAVE_FFACE_SPEEDUP;
	} else {
		rate   = 1.0f - ((dir - 270.f) * WAVE_ONE_OVER_NINETY);
		adjust = WAVE_FFACE_SPEEDUP;
	}

	/* wave pitch */
	if ( dir == 0.0f || dir == 180.0f ) {
		pitch = 0.0f;
	} else {
		/* vehicle pitch on wave is variable depending on wave size and angle on wave */
		//wave_pitch( pts, (y - waveinfo->position) );
		//pitch *= rate;
		//if ( dir > 180.0f )	pitch = -(pitch);
		pitch = 0.0f;
	}

	/* wave roll */
	if ( dir == 90.0f || dir == 270.0f ) {
		roll = 0.0f;
	} else {
		/* vehicle roll on wave is variable depending on wave size and angle on wave */
		roll = gfxAbs( (pitch * (1.0f - rate)) );
		//if ( dir > 270.0f || dir < 90.0f ) {
		//	roll = -(roll);
	}

	/* save data */
	wavepos->water_level  = water_level;
	wavepos->rate         = rate;
	wavepos->speed_adjust = adjust;
	wavepos->pitch        = pitch;
	wavepos->roll         = roll;
}


/**********************************************************
 * Function:	surfWaterLevel
 **********************************************************
 * Description: Get the water level at this x,y position
 * Inputs:		waveinfo - wave position data
 *				contour	 - surface contour data
 *				saved	 - saved wave points
 *				x        - current x position
 *				y		 - current y position
 * Notes:		nil
 * Returns:		water level at this position
 **********************************************************/

float surfWaterLevel (
	WaveTypeData	*waveinfo,
	SurfContour		*contour,
	WaveSavedPoints *saved,
	float			 x,
	float			 y
    )
{   
	float		  water_level;
	float		  size;
	Point_f3d	 *pts;

	/* validate */
	if ( waveinfo == NULL ) return( 0 );

	/* get water level */
	size = surfGetWaveHeight( contour, x, waveinfo->position );
	pts  = surfCalcWavePoints( size, saved );
	water_level = z_position( pts, (y - (waveinfo->position - 4000.0f)) );

	return( water_level );
}


/**********************************************************
 * Function:	z_position
 **********************************************************
 * Description: Get the Z position on a wave face
 * Inputs:		face    - front or back face
 *				y       - y position
 *				pts     - wave profile points
 *				npts    - number of points passed
 *				ypeak   - highest point y value
 *				zpeak   - highest point z value
 *				indexpt - returned index point
 *				fract   - returned fractional position
 * Notes:		nil
 * Returns:		z position
 **********************************************************/

float	z_position (
	Point_f3d	*pts,
	float		 y
	)
{
	float		 y1, y2, z1, z2;
	int			 i;
	float		 yfract;
	float		 lip;
	float		 z;

	/* init */
	lip = pts[20].y;
	//y -= 4000.0f;

	/* */
	i = 1;
	y1 = pts[0].y;
	y2 = pts[1].y;
	while ( y2 < y ) {
		i++;
		y1 = y2;
		y2 = pts[i].y;
	}

	/* get fraction between the two points */
	yfract = (y - y1) / (y2 - y1);

	/* calculate z height value */
	z1 = pts[i-1].z;
	z2 = pts[i].z;
	z  = (z1 + ((z2 - z1) * yfract));

#if 0
	// debug
	printf( "y=%.1f", y );
	printf( " i=%d", i );
	printf( " y1=%.1f", y1 );
	printf( " y2=%.1f", y2 );
	printf( " yf=%.1f", yfract );
	printf( " z1=%.1f", z1 );
	printf( " z2=%.1f", z2 );
	printf( " z=%.1f", z );
	printf( "\n" );
#endif

	return( z );
}


