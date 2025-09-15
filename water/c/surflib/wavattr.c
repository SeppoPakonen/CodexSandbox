/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		wavattr.c
 * Author:		Mark Theyer
 * Created:		02 Sep 1999
 **********************************************************
 * Description:	Wave attribute functions.
 **********************************************************
 * Functions:
 *	surfInitWaveAttributes() 
 *				Initialise wave attribute data.
 *	surfParseWavAttr()
 *				File parser for a wave animation file.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 02-Sep-99	Theyer	Initial coding.
 * 04-Jun-01	Theyer	Port for PS2
 *
 **********************************************************/

/*
 * includes
 */

#include <wave.h>
#include <waverr.h>
#include <surfutil.h>
#include <parse/parse.h>

/*
 * macros
 */

/*
 * typedefs
 */

typedef struct {
	WaveAttributes *wavattr;
	int				read;
} SurfParseWavAttrInfo;

/*
 * prototypes
 */

void surfParseWavAttr( void *user_data );

/*
 * global variables
 */

/*
 * functions
 */

/**********************************************************
 * Function:	surfInitWaveAttributes
 **********************************************************
 * Description: Initialise wave attribute data
 * Inputs:		waveattr - uninitialised wave attributes
 *				beachnum - beach number to use for init'
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

void surfInitWaveAttributes (
	WaveAttributes *waveattr,
	int				beachnum
	)
{
	char					 filename[32];
	Byte					*data;
	SurfParseWavAttrInfo	 info;
	int						 i, j;
	float					 x, xinc;

	/* set path */
	gfxFilePath( "waves" );

	/* init name */
	sprintf( filename, "wave%d.att", beachnum );

	/* load file */
	data = gfxFileLoad( filename );

	/* validate */
	if ( data == NULL ) goto fail;

	/* parse data file */
	info.read = 0;
	info.wavattr = waveattr;
	pseParseData( data, "#", 1, surfParseWavAttr, &info );

	/* validate */
	xinc = 0.0f;
	if ( info.read >= 14 ) {
		/* init static info */
		waveattr->half_width  = (int)(waveattr->width * 0.5f);
		waveattr->half_length = (waveattr->length>>1);
		for ( i=0; i<WAVE_MAX_LODS; i++ ) {
			waveattr->fface_npts[i] = (waveattr->nsections[i] * waveattr->fface_nsecpts);
			waveattr->bface_npts[i] = (waveattr->nsections[i] * waveattr->bface_nsecpts);
			/* create x section values */
			x    = -(waveattr->width * 0.5f);
			xinc =  (waveattr->width / (float)(waveattr->nsections[i] - 1));
			for ( j=0; j<waveattr->nsections[i]; j++, x += xinc )
				waveattr->xsection[i][j] = x;
		}
		return;
	}

fail:
	/* failed */
	surfExit( WAVE_ERROR_ATTR_LOAD_FAILED );
}


/**********************************************************
 * Function:	surfParseWavAttr
 **********************************************************
 * Description: File parser for a wave animation file
 * Inputs:		user_data - data pointer and parse read counter
 * Notes:		nil
 * Returns:		nil
 **********************************************************/

void surfParseWavAttr (
    void *user_data
    )
{
	SurfParseWavAttrInfo	*info;
	WaveAttributes			*wavattr;
	int						 i;

	/* init */
	info = (SurfParseWavAttrInfo *)user_data;
	wavattr = info->wavattr;

	/* increment read value */
	info->read++;

	switch( info->read ) {
	case 1:
		// section points front and back
		wavattr->fface_nsecpts = pseGetIntValue(0);
		wavattr->bface_nsecpts = pseGetIntValue(1);
		/* validate */
		if ( wavattr->fface_nsecpts > WAVE_MAXPTS_FFACE_SEC )
			surfExit( WAVE_ERROR_BAD_FFACE_NPTS );
		if ( wavattr->bface_nsecpts > WAVE_MAXPTS_BFACE_SEC )
			surfExit( WAVE_ERROR_BAD_BFACE_NPTS );
		break;
	case 2:
		// front face lengths 
		for ( i=0; i<wavattr->fface_nsecpts; i++ )
			wavattr->fface_length[i] = pseGetFloatValue(i);
		break;
	case 3:
		// front face pitch angles
		for ( i=0; i<wavattr->fface_nsecpts; i++ )
			wavattr->fface_angles[i] = pseGetFloatValue(i);
		break;
	case 4:
		// back face lengths
		for ( i=0; i<wavattr->bface_nsecpts; i++ )
			wavattr->bface_length[i] = pseGetFloatValue(i);
		//for ( i=0; i<wavattr->bface_nsecpts; i++ )
		//	printf( "bfl[%d]=%.2Lf, %.2Lf, [%s]\n", i, (long double)wavattr->bface_length[i], (long double)pseGetFloatValue(i), pseGetStringValue(i) );
		break;
	case 5:
		// back face heights
		for ( i=0; i<wavattr->bface_nsecpts; i++ )
			wavattr->bface_height[i] = pseGetFloatValue(i);
		//for ( i=0; i<wavattr->bface_nsecpts; i++ )
		//	printf( "bfh[%d]=%.2Lf, %.2Lf, [%s]\n", i, (long double)wavattr->bface_height[i], (long double)pseGetFloatValue(i), pseGetStringValue(i) );
		break;
	case 6:
		// textures high and low
		txtCopy( pseGetStringValue(0), wavattr->high_res_image );
		txtCopy( pseGetStringValue(1), wavattr->low_res_image  );
		break;
	case 7:
		// background colour
		wavattr->r = pseGetFloatValue(0);
		wavattr->g = pseGetFloatValue(1);
		wavattr->b = pseGetFloatValue(2);
		break;
	case 8:
		// wave width, length and number of flat sections inbetween each wave
		wavattr->width    = pseGetIntValue(0);
		wavattr->length   = pseGetIntValue(1);
		wavattr->nflat    = pseGetIntValue(2);
		break;
	case 9:
		// number of waves (total), end termination position, wave speed
		wavattr->nwaves   = pseGetIntValue(0);
		wavattr->end_ypos = pseGetIntValue(1);
		wavattr->speed    = pseGetIntValue(2);
		break;
	case 10:
		// wave number of profiles for each model
		for ( i=0; i<WAVE_MAX_LODS; i++ ) {
			wavattr->nsections[i] = pseGetFloatValue(i);
			if ( wavattr->nsections[i] > WAVE_MAX_SECTIONS )
				surfExit( WAVE_ERROR_BAD_NSECTIONS );
		}
		break;
	case 11:
		// front face file names 
		for ( i=0; i<WAVE_MAX_LODS; i++ )
			txtCopy( pseGetStringValue(i), wavattr->fface_file[i] );
		break;
	case 12:
		// back face file names 
		for ( i=0; i<WAVE_MAX_LODS; i++ )
			txtCopy( pseGetStringValue(i), wavattr->bface_file[i] );
		break;
	case 13:
		// flat water file names 
		for ( i=0; i<WAVE_MAX_LODS; i++ )
			txtCopy( pseGetStringValue(i), wavattr->flat_file[i] );
		break;
	case 14:
		// drop point for rider
		wavattr->drop_point_x = pseGetFloatValue(0);
		wavattr->drop_point_y = pseGetFloatValue(1);
		break;
	}
}

