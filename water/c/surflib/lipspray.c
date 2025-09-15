/**********************************************************
 * Copyright (C) 1999,2000 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:	PlayStation surf game
 **********************************************************
 * File:	lipspray.c
 * Author:	Mark Theyer
 * Created:	09 Aug 1999
 **********************************************************
 * Description:	Lip Spray related functions
 **********************************************************
 * Functions:
 *	surfInitLipSpray()
 *				Initialise lip spray.
 *	surfCreateLipSpray()
 *				Update lip spray objects for this wave.
 *	surfDrawLipSpray()
 *				Draw lip spray objects.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 09-Aug-99	Theyer	Initial coding.
 *
 **********************************************************/

/*
 * includes
 */

#include <myformat/myformat.h>
#include <gfx/gfx.h>
#include <gfx/psx_il.h>
#include <wave.h>
#include <random.h>

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
 * Function:	surfInitLipSpray
 **********************************************************
 * Description: Initialise lip spray
 * Inputs:		
 * Notes:		nil
 * Returns:		
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 09-Aug-99	Theyer	Initial coding.
 *
 **********************************************************/

void surfInitLipSpray ( 
	WaveAttributes	*waveattr,
	SurfLipSpray	*spray
	)
{
	int			i;

	/* load models */
	gfxFilePath( "Models\\Splash" );
	for ( i=0; i<14; i++ ) {
		if ( ! mmfModelToGfxObject( "lipspray", (GfxObject **)&spray->object[0][i], NULL ) ) {
			printf( "failed to load lip spray model\n" );
			exit(1);
		}
		if ( ! mmfModelToGfxObject( "lipspry1", (GfxObject **)&spray->object[1][i], NULL ) ) {
			printf( "failed to load lip spray model\n" );
			exit(1);
		}
		if ( ! mmfModelToGfxObject( "lipspry2", (GfxObject **)&spray->object[2][i], NULL ) ) {
			printf( "failed to load lip spray model\n" );
			exit(1);
		}
		gfxObjectSubDivide( spray->object[0][i], GFX_SUBDIV_2x2 );
		gfxObjectSubDivide( spray->object[1][i], GFX_SUBDIV_2x2 );
		gfxObjectSubDivide( spray->object[2][i], GFX_SUBDIV_2x2 );
		spray->draw[i] = FALSE;
	}
	
	/* save matrix */
	spray->mat = gfxGetObjectMatrix( spray->object[0][0] );
	
	/* init */
	spray->xtrans  = 0;
	spray->ytrans  = 0;
	spray->swapper = 0;
	spray->scale   = FIXED_NORMALISE( (waveattr->length[10]<<1), 1000 );
}


/**********************************************************
 * Function:	surfCreateLipSpray
 **********************************************************
 * Description: Update lip spray objects for this wave
 * Inputs:		wave - wave render data
 *				spray - lip spray data
 * Notes:		update spray object points
 * Returns:		nil
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 09-Aug-99	Theyer	Initial coding.
 *
 **********************************************************/

void surfCreateLipSpray ( 
	WaveRenderData	*wave,
	SurfLipSpray	*spray
	)
{
	Point_i3d	pt[8];
	int			i, p;
	int			random;
	fixed		size[2];
	fixed		size_less_half[2];
	int			rand_300;
	int			rand_150;
	int			obji;

	/* update lip spray object swapper */
	spray->swapper = (!spray->swapper);

	/* init */
	obji = (1+spray->swapper);
#if 0
	random = FIXED_TO_INT(gfxRandom( -102400, 307200 )); // fixed value 102400 = 25.0f, 307200 = 75.0f
#else
	random = surfGetRandom( SURF_RANDOM_SET_N25_75 );
#endif
	for ( i=0; i<14; i++ ) {
		if (( wave->size[i]     < 2048 || wave->size[i]     > 8192 ) &&
			( wave->size[(i+1)] < 2048 || wave->size[(i+1)] > 8192 )) { // fixed value 2048 = 0.5f
			/* do not draw unless over half size */
			spray->draw[i] = FALSE;
		} else {
			/* draw */
			spray->draw[i] = TRUE;
			/* init */
			size[0] = wave->size[i];
			size[1] = wave->size[(i+1)];
			/* adjust so wave size 0->1->2->3 becomes lip spray size 0->1->2->0 */
			/* formula following: size[0] = 0.05f + (2.0f * (1.0f - (size[0] - 2.0f))); */
			if ( size[0] > 8192 ) // 2.0f
				size[0] = 205 + ((FIXED_ONE - (size[0] - 8192))<<1);
			if ( size[1] > 8192 )
				size[1] = 205 + ((FIXED_ONE - (size[1] - 8192))<<1);
			/* handle wave size 3-4 */
			if ( size[0] < 205 ) size[0] = 205;
			if ( size[1] < 205 ) size[1] = 205;
			/* init points */
			p = (11 * i) + 10;
			pt[0] = wave->front_face_pts[p];
			pt[1] = wave->front_face_pts[(p+11)];
			pt[2] = pt[0];
			pt[3] = pt[1];
			pt[4] = pt[0];
			pt[5] = pt[1];
			pt[6] = pt[0];
			pt[7] = pt[1];
			pt[2].z += 2;
			pt[3].z += 2;
			/* save multiple calculations */
			size_less_half[0] = FIXED_MULTIPLY( (size[0] - 2048), spray->scale );
			size_less_half[1] = FIXED_MULTIPLY( (size[1] - 2048), spray->scale );
			rand_300 = random + 300;
			rand_150 = random + 150;
			/* update points */
			pt[2].z += FIXED_TO_INT(((rand_300) * size_less_half[0]));
			pt[6].z += FIXED_TO_INT(((rand_150) * size_less_half[0]));
			/* random here connects joining spray */
#if 0
			random = FIXED_TO_INT(gfxRandom( -102400, 307200 )); // fixed value 102400 = 25.0f, 307200 = 75.0f
#else
			random = surfGetRandom( SURF_RANDOM_SET_N25_75 );
#endif
			pt[3].z += FIXED_TO_INT(((rand_300) * size_less_half[1]));
			pt[7].z += FIXED_TO_INT(((rand_150) * size_less_half[1]));
			/* limit */
			if ( pt[2].z < (pt[0].z + 10) ) pt[2].z = (pt[0].z + 10);
			if ( pt[3].z < (pt[1].z + 10) ) pt[3].z = (pt[1].z + 10);
			if ( pt[6].z < (pt[4].z + 10) ) pt[6].z = (pt[4].z + 10);
			if ( pt[7].z < (pt[5].z + 10) ) pt[7].z = (pt[5].z + 10);
			/* set points in model */
			for ( p=0; p<4; p++ )
				SURF_SET_OBJECT_POINT( spray->object[0][i], (p+1), &pt[p] );
			if ( wave->size[i] < 8192 )
				pt[4].z -= FIXED_TO_INT((60 * size_less_half[0]));
			if ( wave->size[(i+1)] < 8192 )
				pt[5].z -= FIXED_TO_INT((60 * size_less_half[1]));
			/* set points in model */
			for ( p=0; p<4; p++ )
				SURF_SET_OBJECT_POINT( spray->object[obji][i], (p+1), &pt[(p+4)] );
		}
	}
	/* save position */ 
	spray->xtrans = wave->xtrans;
	spray->ytrans = wave->ytrans;
}


/**********************************************************
 * Function:	surfDrawLipSpray
 **********************************************************
 * Description: Draw lip spray objects
 * Inputs:		spray - lip spray data
 * Notes:		
 * Returns:		nil
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 09-Aug-99	Theyer	Initial coding.
 *
 **********************************************************/

void surfDrawLipSpray (
	SurfLipSpray	*spray
	)
{
	int		i;
	int		obji;

#ifdef OsTypeIsWin95
	return;
#endif

	/* init */
	obji = (1+spray->swapper);
	i = 13;
	while( i ) {
		if ( spray->draw[i] ) {
			gfxSetObjectMatrix( spray->object[0][i], &spray->mat );
			gfxTranslateObject( spray->object[0][i], -(spray->xtrans), -(spray->ytrans), 0 );
			//gfxDrawObject( spray->object[0][i] );
			gfxSetObjectMatrix( spray->object[obji][i], &spray->mat );
			gfxTranslateObject( spray->object[obji][i], -(spray->xtrans), -(spray->ytrans), 0 );
			gfxDrawObject( spray->object[obji][i] );
		}
		i--;
	}
}

