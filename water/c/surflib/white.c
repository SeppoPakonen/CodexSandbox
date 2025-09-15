/**********************************************************
 * Copyright (C) 1999,2000 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:	PlayStation surf game
 **********************************************************
 * File:	white.c
 * Author:	Mark Theyer
 * Created:	19 Aug 1999
 **********************************************************
 * Description:	White water related functions
 **********************************************************
 * Functions:
 *	surfInitWhiteWater()
 *				Initialise white water.
 *	surfCreateWhiteWater()
 *				Update white water objects for this wave.
 *	surfDrawWhiteWater()
 *				Draw white water objects.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 19-Aug-99	Theyer	Initial coding.
 *
 **********************************************************/

/*
 * includes
 */

#include <myformat/myformat.h>
#include <gfx/gfx.h>
#include <random.h>
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

/*
 * global variables
 */

/*
 * functions
 */


/**********************************************************
 * Function:	surfInitWhiteWater
 **********************************************************
 * Description: Initialise white water 
 * Inputs:		waveattr - wave attribute data
 *				white    - uninitialised white data
 * Notes:		nil
 * Returns:		initialised white data structure
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 19-Aug-99	Theyer	Initial coding.
 *
 **********************************************************/

void surfInitWhiteWater ( 
	WaveAttributes	*waveattr,
	SurfWhiteWater	*white
	)
{
	int			i;
	fixed		scale;

	/* load model(s) */
	gfxFilePath( "models\\splash" );

	/* scale depending on wave length (note white water model is 1000 high) */
	i = (waveattr->length[10]<<1);
	scale = FIXED_NORMALISE( i, 1000 );

	if ( ! mmfModelToGfxObject( "white1", (GfxObject **)&white->object[0], NULL ) ) {
		printf( "failed to load white water model\n" );
		exit(1);
	}
	if ( ! mmfModelToGfxObject( "white2", (GfxObject **)&white->object[1], NULL ) ) {
		printf( "failed to load white water model\n" );
		exit(1);
	}
	gfxScaleObject( white->object[0], FIXED_ONE, scale, scale );

	/* save matrix */
	white->mat  = gfxGetObjectMatrix( white->object[0] );

	/* swapper */
	white->swap = 0;

	/* init */
	for ( i=0; i<SURF_MAX_WHITE_WATER; i++ ) {
		white->xtrans[i] = 0;
		white->ytrans[i] = 0;
		white->ztrans[i] = 0;
		white->draw[i]   = FALSE;
	}
}


/**********************************************************
 * Function:	surfCreateWhiteWater
 **********************************************************
 * Description: Update white water objects for this wave
 * Inputs:		wave  - wave render data
 *				white - white water data
 * Notes:		nil
 * Returns:		updated white water 
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 19-Aug-99	Theyer	Initial coding.
 *
 **********************************************************/

void surfCreateWhiteWater ( 
	WaveRenderData	*wave,
	SurfWhiteWater	*white
	)
{
	int			i, p;
	int			random;
	fixed		size;
	fixed		scale;

	/* init */
	for ( i=0; i<SURF_MAX_WHITE_WATER; i++ ) {
		if ( wave->size[i] < 8192 || wave->size[i] >= 12288 ) {
			/* do not draw unless breaking */
			white->draw[i] = FALSE;
		} else {
			/* draw */
			white->draw[i] = TRUE;
			/* init */
			size = wave->size[i];
			/* adjust so wave size 2->3 becomes lip spray size 2->2.1 = 0.0f->1.0f and 2.1->3.0 = 1.0->0.0f */
			if ( size <= 8233 ) // 2.01f
				scale = FIXED_NORMALISE((size - 8192),41);
			else
				scale = FIXED_ONE - (41 * FIXED_TO_INT(FIXED_NORMALISE((size - 8192),41)));
			/* add random */
#if 0
			random = FIXED_TO_INT( gfxRandom( -671744, 671744 ) ); // 164
#else
			random = surfGetRandom( SURF_RANDOM_SET_N164_164 );
#endif
			scale += (scale>>2) + random;
			/* init points */
			p = (11 * i) + 10;
			/* save white water info */
			white->xtrans[i] = -(wave->xtrans) + wave->front_face_pts[p].x;
			white->ytrans[i] = -(wave->ytrans) + wave->front_face_pts[p].y;
			white->ztrans[i] = wave->front_face_pts[p].z;
			white->scale[i]  = scale;
		}
	}

	/* update swapper */
	white->swap = (!white->swap);
}


/**********************************************************
 * Function:	surfDrawWhiteWater
 **********************************************************
 * Description: Draw white water objects
 * Inputs:		white - white water data
 * Notes:		
 * Returns:		nil
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 19-Aug-99	Theyer	Initial coding.
 *
 **********************************************************/

void surfDrawWhiteWater (
	SurfWhiteWater	*white
	)
{
	int		i, swap;
	int		obji;

#ifdef OsTypeIsWin95
	return;
#endif

	/* init */
	i = (SURF_MAX_WHITE_WATER - 1);
	swap = white->swap;
	while( i >= 0 ) {
		if ( white->draw[i] && (white->scale[i] > 0) ) {
			gfxSetObjectMatrix( white->object[swap], &white->mat );
			gfxScaleObject( white->object[swap], ((white->scale[i]<2048)?(2048):(white->scale[i])), white->scale[i], white->scale[i] );
			gfxTranslateObject( white->object[swap], white->xtrans[i], white->ytrans[i], white->ztrans[i] );
			gfxDrawObject( white->object[swap] );
		}
		swap = !swap;
		i--;
	}
}

