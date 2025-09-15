/**********************************************************
 * Copyright (C) 1999,2000 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:	PlayStation surf game
 **********************************************************
 * File:	flat.c
 * Author:	Mark Theyer
 * Created:	10 Aug 1999
 **********************************************************
 * Description:	Flat water related functions
 **********************************************************
 * Functions:
 *	surfInitFlatWater()
 *				Initialise flat water.
 *	surfDrawFlatWater()
 *				Draw the flat water around this wave.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 10-Aug-99	Theyer	Initial coding.
 *
 **********************************************************/

/*
 * includes
 */

#include <myformat/myformat.h>
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
 * Function:	surfInitFlatWater
 **********************************************************
 * Description: Initialise flat water
 * Inputs:		waveattr - wave attribute data
 *				flat     - uninitialised flat water data
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

void surfInitFlatWater (
	WaveAttributes  *waveattr,
	SurfFlatWater	*flat
	)
{
	fixed	scale;

	gfxFilePath( "models\\waves" );

	/* flat water */
	if (( ! mmfModelToGfxObject( "flat",  (GfxObject **)&flat->high,  NULL ) ) ||
		( ! mmfModelToGfxObject( "flat2", (GfxObject **)&flat->low,   NULL ) ) ||
	    ( ! mmfModelToGfxObject( "sides", (GfxObject **)&flat->sides, NULL ) ) ) {
		printf( "failed to load flat water models" );
		exit(1);
	}

#if 0
	/* scale depending on wave length (note flat water model is 1000 long) */
	scale = FIXED_NORMALISE( (waveattr->length[10]<<1), 1000 );
	gfxScaleObject( flat->high, FIXED_ONE, scale, FIXED_ONE );
#else
	gfxScaleObject( flat->high, FIXED_ONE, 6144, FIXED_ONE );
#endif

	/* save matrix */
	flat->mat = gfxGetObjectMatrix( flat->high );

	/* set subdivision flag so the models do not clip out on the PSX */
	gfxObjectSubDivide( flat->high,  GFX_SUBDIV_4x4 );
	gfxObjectSubDivide( flat->low,   GFX_SUBDIV_2x2 );
	gfxObjectSubDivide( flat->sides, GFX_SUBDIV_2x2 );

#if 0
	gfxFilePath( "Images\\Back" );
	flat->water_texture_id = mifTextureToGfxTexture( "water1.mib", FALSE );
	
	/* create sprites */
	flat->water_sprite_id = gfxSpriteFromTexture( flat->water_texture_id );
	gfxSetSpriteScale( flat->water_sprite_id, 16588, 8192 ); // fixed value of 4.05f, 2.0f
#endif

	/* draw flag */
	//flat->draw_infront    = TRUE;
	flat->trans           = 1500;//FIXED_TO_INT( (1000 * scale) );
	flat->wave_end        = waveattr->backside[0];
}


/**********************************************************
 * Function:	surfDrawFlatWater
 **********************************************************
 * Description: Draw the flat water around this wave.
 * Inputs:		flat - flat water data structure
 *				wave - pointer to a wave render data
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

void surfDrawFlatWater ( 
	SurfFlatWater	*flat,
	WaveRenderData	*wave,
	int				 posy,
	Bool			 front_only
	)
{
	int			xtrans;
	int			ytrans;
	fixed		x;
	int			i;
	fixed		res;
	GfxMatrix	mat;

	fixed		scale;
	int			size;

#ifdef OsTypeIsWin95
	return;
#endif

	/* init */
	xtrans = -(wave->xtrans);
	if ( posy < 10000 )
		posy = -(posy);
	else
		posy = -10000;

	/* draw flat water in front of this wave */
	ytrans = -(wave->ytrans) + flat->trans;
	gfxSetObjectMatrix( flat->high, &flat->mat );
	gfxTranslateObject( flat->high, xtrans, ytrans, 0 );
	/* init flat sides */
	gfxSetObjectMatrix( flat->sides, &flat->mat );
	gfxTranslateObject( flat->sides, 0, ytrans, 0 );
	while( ytrans > -1500 ) { // was 4000 (27/10)
		gfxTranslateObject( flat->high, 0, -(flat->trans), 0 );
		gfxDrawObject( flat->high );
		/* draw sides */
		gfxTranslateObject( flat->sides, 0, -(flat->trans), 0 );
		gfxDrawObject( flat->sides );
		ytrans -= flat->trans;
	}

	/* exit here on flag */
	if ( front_only )
		return;

//	if ( flat->draw_infront ) {
		/* (ytrans is initialised) */
		size = (flat->trans<<1);
		ytrans -= flat->trans;
#if 0
		while( ytrans > -20000 ) {
			/* clip to cube if necessary */
			if ( (ytrans - size) < -20000 )
				size = (20000 - gfxAbs(ytrans));
#else
		while( ytrans > posy ) { //-20000 ) {
			/* clip to cube if necessary */
			if ( (ytrans - size) < posy ) //-20000 )
				size = (gfxAbs(posy) - gfxAbs(ytrans));
#endif
			scale = FIXED_NORMALISE( size, flat->trans );
			gfxSetObjectMatrix( flat->high, &flat->mat );
			//printf( "ytrans=%d, scale=%d, size=%d ", ytrans, scale, size );
			gfxScaleObject( flat->high, FIXED_ONE, scale, FIXED_ONE );
			//printf( "\n" );
			/* save matrix for use with sides model */
			mat = gfxGetObjectMatrix( flat->high );
			gfxTranslateObject( flat->high, xtrans, ytrans, 0 );
			gfxDrawObject( flat->high );
			/* draw sides */
			gfxSetObjectMatrix( flat->sides, &mat );
			gfxTranslateObject( flat->sides, 0, ytrans, 0 );
			gfxDrawObject( flat->sides );
			/* update */
			ytrans -= size;
			size += flat->trans;
			/* keep to scale limits of PSX1 */
			if ( size > (flat->trans<<3) )
				size = (flat->trans<<3);
			if ( size > 8000 )
				size = 8000;
		}
//	}

	if ( (flat->wave_end - flat->trans) > 0 ) {
		/* draw flat water beside wave front face */
		gfxSetObjectMatrix( flat->sides, &flat->mat );
		gfxTranslateObject( flat->sides, 0, (-(wave->ytrans) + flat->trans), 0 );
		gfxDrawObject( flat->sides );

		/* draw flat water beside wave back face */
		gfxSetObjectMatrix( flat->sides, &flat->mat );
		gfxTranslateObject( flat->sides, 0, (-(wave->ytrans) + flat->wave_end), 0 );
		scale = FIXED_NORMALISE( (flat->wave_end - flat->trans), flat->trans );
		gfxScaleObject( flat->sides, FIXED_ONE, scale, FIXED_ONE );
		gfxDrawObject( flat->sides );
	} else {
		/* draw flat water beside wave */
		gfxSetObjectMatrix( flat->sides, &flat->mat );
		gfxTranslateObject( flat->sides, 0, (-(wave->ytrans) + flat->wave_end), 0 );
		scale = FIXED_NORMALISE( flat->wave_end, flat->trans );
		gfxScaleObject( flat->sides, FIXED_ONE, scale, FIXED_ONE );
		gfxDrawObject( flat->sides );
	}

#if 1
	/* draw flat water behind the wave */
	ytrans = -(wave->ytrans) + flat->wave_end;
	gfxSetObjectMatrix( flat->high, &flat->mat );
	gfxTranslateObject( flat->high, xtrans, ytrans, 0 );
	/* init flat sides */
	gfxSetObjectMatrix( flat->sides, &flat->mat );
	gfxTranslateObject( flat->sides, 0, ytrans, 0 );
	while( ytrans < 3500 ) {
		gfxTranslateObject( flat->high, 0, flat->trans, 0 );
		gfxDrawObject( flat->high );
		/* draw sides */
		gfxTranslateObject( flat->sides, 0, flat->trans, 0 );
		gfxDrawObject( flat->sides );
		ytrans += flat->trans;
	}
#endif

	/* draw flat water behind the wave */
	size = flat->trans;
	//ytrans = -(wave->ytrans) + flat->wave_end;
	while( ytrans < 20000 ) {
		/* clip to cube if necessary */
		if ( (ytrans + size) > 20000 )
			size = (20000 - ytrans);
		ytrans += size;
		scale = FIXED_NORMALISE( size, flat->trans );
		gfxSetObjectMatrix( flat->low, &flat->mat );
		//printf( "ytrans=%d, scale=%d, size=%d ", ytrans, scale, size );
		gfxScaleObject( flat->low, FIXED_ONE, scale, FIXED_ONE );
		//printf( "\n" );
		/* save matrix for use with sides model */
		mat = gfxGetObjectMatrix( flat->low );
		gfxTranslateObject( flat->low, xtrans, ytrans, 0 );
		gfxDrawObject( flat->low );
		/* draw sides */
		gfxSetObjectMatrix( flat->sides, &mat );
		gfxTranslateObject( flat->sides, 0, ytrans, 0 );
		gfxDrawObject( flat->sides );
		/* update */
		size += flat->trans;
		/* keep to scale limits of PSX1 */
		if ( size > (flat->trans<<3) )
			size = (flat->trans<<3);
		if ( size > 8000 )
			size = 8000;
	}
}

