/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		contour.c
 * Author:		Mark Theyer
 * Created:		13 Jul 1999
 **********************************************************
 * Description:	Surface contour related functions for
 *				wave generation. Contours are used to
 *				store wave height information in a
 *				grid. Wave height information is returned
 *				provided an x,y position within the grid.
 **********************************************************
 * Functions:
 *	surfInitContour() 
 *				Initialise a surface contour.
 *	surfGetWaveHeight()
 *				Get the wave height at this x,y position
 *				within a surface contour grid.
 **********************************************************
 *
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 13-Jul-99	Theyer	Initial coding from existing game
 * 31-May-01	Theyer	Initial coding for PS2
 *
 **********************************************************/

/*
 * includes
 */

#include <contour.h>
#include <surfutil.h>
#include <gfx/gfxps2.h>
#include <wave.h>
#include <memory/memory.h>
#include <waverr.h>

/*
 * macros
 */

/*
 * typedefs
 */

typedef struct {
	SurfContour		*contour;
	int				 y;
} SurfParseContourInfo;

/*
 * prototypes
 */

private void surfParseContour( void *user_data );

/*
 * global variables
 */

/*
 * functions
 */

/**********************************************************
 * Function:	surfInitContour
 **********************************************************
 * Description: Initialise contour surface
 * Inputs:		contour - pointer to a contour data structure
 * Notes:		nil
 * Returns:		initialised data structure 
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 13-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

void surfInitContour ( 
	SurfContour *contour,
	int			 num
	)
{
	char					 filename[32];
	Byte					*data;
	SurfParseContourInfo	 info;

	/* set path */
	gfxFilePath( "waves" );

	/* init name */
	sprintf( filename, "wave%d.con", num );

	/* load file */
	data = gfxFileLoad( filename );

	/* validate */
	if ( data == NULL ) goto fail;

	/* init contour */
	memClear( contour, sizeof(SurfContour) );

	/* init parse info */
	info.contour = contour;
	info.y = 0;

	/* parse data file */
	pseParseData( data, "#", 1, surfParseContour, &info );

	/* init */
	contour->width     = (SURF_CONTOUR_STEP_X * contour->size_x);
	contour->normalise = 0x40000000 / (WAVE_MAX_PROFILES - 1);
	
	/* decrement for useful values */
	contour->size_x--;
	contour->size_y--;

	contour->limit_x   = (SURF_CONTOUR_STEP_X * contour->size_x);
	contour->limit_y   = (SURF_CONTOUR_STEP_Y * contour->size_y);

	/* validate */
	if ( (info.y > 0) && (info.y == (contour->size_y + 1)) )
		return;

fail:
	/* failed */
	surfExit( WAVE_ERROR_CONTOUR_LOAD_FAILED );
}


/**********************************************************
 * Function:	surfParseContour
 **********************************************************
 * Description: Parse a contour file
 *
 * sample format...
 *
 *		# my contour
 *		# size <x> <y>
 *		size 2 2
 *		# data (range 0-120)
 *		1 2
 *		2 2
 *
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

private void surfParseContour (
    void	*user_data
    )
{
	SurfParseContourInfo	*cinfo;
	SurfContour				*contour;
	int						 x;

	/* init */
	cinfo = (SurfParseContourInfo *)user_data;
	contour = cinfo->contour;

	/* contour size */
	if ( pseArgValidate( "size", 0 ) ) {
		contour->size_x = pseGetIntValue(1);
		contour->size_y = pseGetIntValue(2);
		/* validate */
		if (( contour->size_x > SURF_CONTOUR_MAX_X ) || ( contour->size_y > SURF_CONTOUR_MAX_Y ))
			surfExit( WAVE_ERROR_CONTOUR_TOO_BIG );
		return;
	}

	/* read contour */
	if ( (contour->size_x > 0) && (pseNumArgs() == contour->size_x) ) {
		for ( x=0; x<contour->size_x; x++ ) {
			/* validate */
			if ( (pseGetIntValue(x) < 0) || (pseGetIntValue(x) > 120) )
				surfExit( WAVE_ERROR_BAD_CONTOUR_LAYOUT );
			//contour->zval[cinfo->y][x] = (short)(pseGetIntValue(x)<<2); // (4 x multiply for range 0->480)
			contour->zval[cinfo->y][x] = (short)(pseGetIntValue(x) * 3);  // (put into range 0->360)
		}
		cinfo->y++;
	}
}


/**********************************************************
 * Function:	surfGetWaveHeight
 **********************************************************
 * Description: Get the height of wave at this point on 
 *				a contour
 * Inputs:		contour - pointer to a contour data structure
 *				x - x position on contour
 *				y - y position on contour
 * Notes:		A return value of 16384 has been previously used
 *				to indicate a limit to stop the player going out
 *				of bounds. This was used for 3D backgrounds. It
 *				is not used with the environment cube method.
 * Returns:		wave height (range 0..4.0) 
 *				0=flat water, 1.0=peak, 2.0=break, 3.0=flat
 *				again, 4.0=beach
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 13-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

float surfGetWaveHeight ( 
	SurfContour		*contour, 
	float			 fx, 
	float			 fy 
	)
{
	fixed	iheight;
	float	fheight;

	iheight = surfGetWaveIndex( contour, fx, fy );
	fheight = (float)iheight * 0.011111111f; // 90 = 1.0f
	//printf( "height=%f\n", (long double)fheight );

	return( fheight );
}


/**********************************************************
 * Function:	surfGetWaveIndex
 **********************************************************
 * Description: Get the height of wave at this point on 
 *				a contour
 * Inputs:		contour - pointer to a contour data structure
 *				x - x position on contour
 *				y - y position on contour
 * Notes:		A return value of 16384 has been previously used
 *				to indicate a limit to stop the player going out
 *				of bounds. This was used for 3D backgrounds. It
 *				is not used with the environment cube method.
 * Returns:		wave height (range 0..4.0) 
 *				0=flat water, 1.0=peak, 2.0=break, 3.0=flat
 *				again, 4.0=beach
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 13-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

int surfGetWaveIndex ( 
	SurfContour		*contour, 
	float			 fx, 
	float			 fy 
	)
{
	int		heighta;
	int		heightb;
	int		height;
	fixed	xratio;
	fixed	yratio;
	int		i;
	int		xa;
	int		ya, yb;
	int		za, zb;
	int		x, y;
	Bool	y_hit;

	// local "cache"
	static  float	 last_y = -1.0f;
	static  int		 last_ya = 0;
	static  int		 last_yb = 0;
	//static  float	 last_yratio = 0.0f;
	static  fixed	 last_yratio = 0;

#if 1
	// same performance as using SPR? 
	static  short	 yacontour[SURF_CONTOUR_MAX_X];
	static  short	 ybcontour[SURF_CONTOUR_MAX_X];
#else
	static  short	*yacontour = (short *)(ps2ScratchPad | 16032);
	static  short	*ybcontour = (short *)(ps2ScratchPad | 16208);
#endif

	/* check for hit */
	y_hit = (fy == last_y);

	/* init */
	x = (int)fx;
	y = 0;

#if 1
	/* force x into range */
	if ( x > contour->width )
		x -= contour->width;
	if ( x < 0 )
		x += contour->width;
#endif

	if ( y_hit ) {
		/* use previously saved values */
		ya = last_ya;
		yb = last_yb;
		yratio = last_yratio;
	} else {
		/* init */
		y = (int)fy;
		yratio  = 0.0f;
		/* surface contour calculation */
		if ( y <= 0 ) {
			ya = contour->size_y;
			yb = ya;
		} else if ( y >= contour->limit_y ) {
			ya = 0;
			yb = 0;
		} else {
			//i = (y >> SURF_CONTOUR_DIVIDE_Y);
			i  = (y/SURF_CONTOUR_STEP_Y);
			ya = (SURF_CONTOUR_STEP_Y * i);
			yratio = (y - ya) * SURF_CONTOUR_FACTOR_Y;
			ya = contour->size_y - i - 1;
			yb = ya + 1;
		}
		/* copy to "cache" memory */
		memCopy( contour->zval[ya], yacontour, (SURF_CONTOUR_MAX_X<<1) );
		if ( yb == ya )
			memCopy( yacontour, ybcontour, (SURF_CONTOUR_MAX_X<<1) );
		else
			memCopy( contour->zval[yb], ybcontour, (SURF_CONTOUR_MAX_X<<1) );
	}

	if ( x <= 0 ) {
		heighta = *yacontour;
		if ( yb == ya ) {
			height = heighta;
		} else {
			heightb = *ybcontour;
			height = heightb + (((heighta - heightb) * yratio)>>16);
		}
	} else if ( x >= contour->limit_x ) {
		/* interpolate with start of grid for smooth transition */
		i = contour->size_x;
		xa = (SURF_CONTOUR_STEP_X * i);
		xratio = (x - xa) * SURF_CONTOUR_FACTOR_X;
		za = *(yacontour + i);
		zb = *yacontour;
		heighta = za + (((zb - za) * xratio)>>16);
		if ( yb == ya ) {
			height = heighta;
		} else {
			za = *(ybcontour + i);
			zb = *ybcontour;
			heightb = za + (((zb - za) * xratio)>>16);
			height  = heightb + (((heighta - heightb) * yratio)>>16);
		}
	} else {
		//i  = (x >> SURF_CONTOUR_DIVIDE_X);
		i  = (x/SURF_CONTOUR_STEP_X);
		za = *(yacontour + i);
		zb = *(yacontour + i + 1);
		xa = (SURF_CONTOUR_STEP_X * i);
		xratio = (x - xa) * SURF_CONTOUR_FACTOR_X;
		heighta = za + (((zb - za) * xratio)>>16);
		if ( yb == ya ) {
			height = heighta;
		} else {
			za = *(ybcontour + i);
			zb = *(ybcontour + i + 1);
			heightb = za + (((zb - za) * xratio)>>16);
			height  = heightb + (((heighta - heightb) * yratio)>>16);
		}
	}

	// validate 
	if ( height > 360 || height < 0 )
		printf( "height error: %d, x,y=%d,%d, ya,yb=%d,%d\n", height, x, y, ya, yb );

	if ( height >= 270 ) 
		height -= 270;

	// save
	if ( ! y_hit ) {
		last_y  = fy;
		last_ya = ya;
		last_yb = yb;
		last_yratio = yratio;
	}

	return( height );
}
