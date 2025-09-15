/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Surfing PlayStation2
 **********************************************************
 * File:		wave.c
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
 *	surfInitWave() 
 *				Initialise wave render and saved points data.
 *	surfUpdateWave()
 *				Create new wave for rendering.
 *	surfSyncWave()
 *				Sync double buffered wave models to be the same.
 *	surfDrawWaveFrontFace()
 *				Draw the front face of the wave.
 *	surfDrawWaveBackFace()
 *				Draw the back face of the wave.
 *	surfDrawWaveCurl()
 *				Draw the transparent wave front face curling lip.
 *	surfWaveSwapBuffers()
 *				Switch wave models for double buffering.
 *	surfCalcWavePoints()
 *				Calculate the front or back face points for a wave.
 *	surfCalcWaveRGBA()
 *				Calculate the RGBA colour and alpha values for a wave
 *	load_wave()
 *				Initialise wave models.
 *	init_wave_points()
 *				Init wave points and pre save wave points.
 *	surfVehicleOnWave()
 *				Update vehicle if it is on this wave.
 *	surfWaterLevel()
 *				Get the water level at this x,y position
 *	z_position()
 *				Get the Z position on a wave face.
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

#include <compile.h>
#include <gfx/gfx.h>
#include <gfx/gfxps2.h>
#include <vehicle.h>
#include <wave.h>
#include <ps2/ps2model.h>
#include <ps2/ps2image.h>
#include <triangle/triangle.h>
#include <random.h>
#include <surfutil.h>
#include <waverr.h>
#include <inline.h>

/*
 * macros
 */

/*
 * typedefs
 */

/*
 * prototypes
 */

void load_wave( WaveRenderData *wave, WaveAttributes *attr );
void init_wave_points( WaveRenderData *wave, WaveSavedPoints *save );

/*
 * global variables
 */

/*
 * functions
 */

/**********************************************************
 * Function:	surfInitWave
 **********************************************************
 * Description: Initialise wave render data
 * Inputs:		waveattr - initialised wave attributes data
 *				wave     - pointer to uninitialised wave render data
 *				save	 - pointer to uninitialised wave point data
 * Notes:		nil
 * Returns:		initialised data structures (wave and save)
 **********************************************************/

void surfInitWave (
	WaveRenderData  *wave,
	WaveAttributes  *attr,
	WaveSavedPoints *save
	)
{
	/* init */
	memClear( wave, sizeof(WaveRenderData) );

	/* set appropriate background color for this water */
	gfxSetBackgroundColor( attr->r, attr->g, attr->b );

	/* load */
	load_wave( wave, attr );

	/* init points */
	init_wave_points( wave, save );

	/* init */
	wave->dbuffer = 0;
	wave->yscroll = 0;
}


/**********************************************************
 * Function:	surfScrollWave
 **********************************************************
 * Description: Update wave for rendering
 * Inputs:		wave     - pointer to a wave render data
 * Notes:		nil
 * Returns:		Updated wave textures
 **********************************************************/

void surfScrollWave ( 
	WaveRenderData *wave	
	)
{
	/* update scroll */
	wave->yscroll--;
	if ( wave->yscroll < 0 )
		wave->yscroll = 127;
	gfxScrollTexture( wave->texture_id[0], wave->yscroll );
	//gfxScrollTexture( wave->texture_id[1], (wave->yscroll>>1) );	
}


/**********************************************************
 * Function:	surfDrawWave
 **********************************************************
 * Description: Draw the wave
 * Inputs:		wave - pointer to a wave render data
 * Notes:		nil
 * Returns:		nil
 **********************************************************/

void surfDrawWave ( 
	WaveRenderData *wave
	)
{
	GfxObject	**draw;
	int			  i;

	/* draw front face */
	draw = wave->models[wave->dbuffer];
	for ( i=0; i<6; i++ )
		gfxDrawObject( draw[i] );
}


/**********************************************************
 * Function:	surfUpdateWave
 **********************************************************
 * Description: Update the wave
 * Inputs:		wave - pointer to a wave render data
 * Notes:		nil
 * Returns:		nil
 **********************************************************/

void surfUpdateWave (
	WaveRenderData	*wave,
	SurfContour		*contour,
	float			 x, 
	float			 y,
	float			 wavey
	)
{
	GfxObject		**draw;
	float			  shift;
	int				  i;
	float			  times[25];

#define HEIGHT(_offset)		(4.0f - surfGetWaveHeight( contour, (x - shift + _offset), wavey ))

	/* init */
	shift = (float)(((int)x) % 2000);

	/* get wave section heights */
	times[0]  = HEIGHT(-7000.0f);
	times[1]  = HEIGHT(-6000.0f);
	times[2]  = HEIGHT(-5000.0f);
	times[3]  = HEIGHT(-4000.0f);
	times[4]  = HEIGHT(-3000.0f);
	times[5]  = HEIGHT(-2500.0f);
	times[6]  = HEIGHT(-2000.0f);
	times[7]  = HEIGHT(-1500.0f);
	times[8]  = HEIGHT(-1000.0f);
	times[9]  = HEIGHT(-750.0f);
	times[10] = HEIGHT(-500.0f);
	times[11] = HEIGHT(-250.0f);
	times[12] = HEIGHT(0.0f);
	times[13] = HEIGHT(250.0f);
	times[14] = HEIGHT(500.0f);
	times[15] = HEIGHT(750.0f);
	times[16] = HEIGHT(1000.0f);
	times[17] = HEIGHT(1500.0f);
	times[18] = HEIGHT(2000.0f);
	times[19] = HEIGHT(2500.0f);
	times[20] = HEIGHT(3000.0f);
	times[21] = HEIGHT(4000.0f);
	times[22] = HEIGHT(5000.0f);
	times[23] = HEIGHT(6000.0f);
	times[24] = HEIGHT(7000.0f);

	/* init */
	wave->xtrans = -(shift);
	wave->ytrans = (wavey - y);
	//printf( "y=%.1f,wavey=%.1f,ytrans=%.1f\n", y, wavey, wave->ytrans );

	/* update */
	draw = wave->models[wave->dbuffer];
	for ( i=0; i<6; i++ ) {
		ps2ActionAdjustTime( draw[i], &times[(i*4)], 5 );
		gfxResetMatrix( draw[i] );
		gfxSetObjectPosition( draw[i], 3.999f );
	}

	/* position */
	gfxTranslateObject( draw[0], (wave->xtrans - 5000.0f), wave->ytrans, 0.0f );
	gfxTranslateObject( draw[1], (wave->xtrans - 2000.0f), wave->ytrans, 0.0f );
	gfxTranslateObject( draw[2], (wave->xtrans -  500.0f), wave->ytrans, 0.0f );
	gfxTranslateObject( draw[3], (wave->xtrans +  500.0f), wave->ytrans, 0.0f );
	gfxTranslateObject( draw[4], (wave->xtrans + 2000.0f), wave->ytrans, 0.0f );
	gfxTranslateObject( draw[5], (wave->xtrans + 5000.0f), wave->ytrans, 0.0f );
}


/**********************************************************
 * Function:	surfWaveSwapBuffers
 **********************************************************
 * Description: Swap wave models for double buffering
 * Inputs:		wave - pointer to a wave render data
 * Notes:		nil
 * Returns:		nil
 **********************************************************/

void surfWaveSwapBuffers ( 
	WaveRenderData *wave
	)
{
	/* swap wave buffer */
	wave->dbuffer++;
	if ( wave->dbuffer == WAVE_MAX_DOUBLE_BUFFER )
		wave->dbuffer = 0;
}


/**********************************************************
 * Function:	surfCalcWavePoints
 **********************************************************
 * Description: Calculate the points for a wave profile
 * Inputs:		saved	  - saved wave points
 *				size      - size of wave (0=flat,1=peaking,2=broken)
 *				pts       - pointer to points to update
 * Notes:		nil
 * Returns:		nil
 **********************************************************/

Point_f3d *surfCalcWavePoints (
	float			  size,
	WaveSavedPoints	 *saved
	)
{
	int		i;

	/* get offset */
	i = (int)(size * saved->confactor);

	//
	//printf( "size=%.1f,i=%d\n", size, i );

	/* validate */
	if ( i > saved->nprofiles )
		i = 0;

	/* point to wave section points */
	return( saved->points[i] );
}


/**********************************************************
 * Function:	load_wave
 **********************************************************
 * Description: Initialise wave models
 * Inputs:		wave     - wave render data to update
 *				waveattr - wave attribute data
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

void load_wave ( 
	WaveRenderData *wave, 
	WaveAttributes *attr 
	)
{
	int			 i;

	/* locate models */
	gfxFilePath( "models/waves" );

	/* load map */
	surfLoadMap( "wave4.map", FALSE );

	/* load geometry */
	printf( "loading wave...\n" );
	for ( i=0; i<WAVE_MAX_DOUBLE_BUFFER; i++ ) {
		p2mModelToGfxObject( "fface7d", (GfxObject **)&wave->models[i][0], NULL );
		p2mModelToGfxObject( "fface7c", (GfxObject **)&wave->models[i][1], NULL );
		p2mModelToGfxObject( "fface7b", (GfxObject **)&wave->models[i][2], NULL );
		p2mModelToGfxObject( "fface7a", (GfxObject **)&wave->models[i][3], NULL );
		p2mModelToGfxObject( "fface7c", (GfxObject **)&wave->models[i][4], NULL );
		p2mModelToGfxObject( "fface7d", (GfxObject **)&wave->models[i][5], NULL );
	}

	/* load section geometry used for calculating independent wave section points */
	p2mModelToGfxObject( "fface7e", (GfxObject **)&wave->amodel, NULL );

	/* load wave textures into PS2 frame buffer for scrolling... */
	wave->texture_id[0] = p2iTextureToGfxTexture( attr->high_res_image, TRUE );
	//wave->texture_id[1] = p2iTextureToGfxTexture( attr->low_res_image,  TRUE );
	if ( wave->texture_id[0] == GFX_NO_TEXTURE ) //|| wave->texture_id[1] == GFX_NO_TEXTURE )
		surfExit( WAVE_ERROR_WATER_TEXTURE_LOAD_FAILED );

	/* load animation data */
	for ( i=0; i<6; i++ ) {
		surfLoadPos( wave->models[0][i], "wave8.p2a" );
		surfLoadPos( wave->models[1][i], "wave8.p2a" );
	}
	surfLoadPos( wave->amodel, "wave8e.p2a" );
}


/**********************************************************
 * Function:	init_wave_points
 **********************************************************
 * Description: Init wave points and pre save wave points
 * Inputs:		wave     - wave render data for update
 *				waveattr - wave attribute data
 *				save     - wave save data for update
 * Notes:		nil
 * Returns:		initialised points in data structures
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 13-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

void init_wave_points ( 
	WaveRenderData	*wave, 
	WaveSavedPoints	*save
	) 
{
	int			  i;
	float		  size;
	float		  f;
	float		  saved;
	Point_f3d	 *p;
	int			  j;

	/* init */
	save->nprofiles = (WAVE_MAX_PROFILES - 1);
	save->confactor = (((float)save->nprofiles)/4.0f);

	/* save points */
	f = 4.0f/((float)save->nprofiles);
	for ( i=0; i<=save->nprofiles; i++ ) {
		size = ((float)i) * f;
		size = (4.0f - size);
		// animate wave section
		ps2ActionAdjustTime( wave->amodel, &size, 1 );
		gfxSetObjectPosition( wave->amodel, 3.999f );
		// save the geometry points
		gfxGetObjectPoints( wave->amodel, save->points[i], WAVE_NPTS );
		// swap coordinate system
		p = save->points[i];
		for ( j=0; j<WAVE_NPTS; j++, p++ ) {
			saved = -(p->y);
			p->y  = p->z + 4000.0f;
			p->z  = saved;
			//if ( i == 20 )
			//	printf( "p %d=%.1f,%.1f,%.1f\n", j, p->x, p->y, p->z );
		}
	}
}


