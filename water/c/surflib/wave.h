/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		wave.h
 * Author:		Mark Theyer
 * Created:		13 Jul 1999
 **********************************************************
 * Description:	Wave header file for wave engine.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 13-Jul-99	Theyer	Initial coding from new wave system
 * 04-Jun-01	Theyer	Port for PS2
 *
 **********************************************************/

#ifndef WAVE_H
#define WAVE_H

/* 
 * includes
 */

#include <type/datatype.h>
#include <gfx/gfx.h>
#include <contour.h>
#include <swell.h>
#include <alpha.h>

/* 
 * macros 
 */

// make this value (less 1) divisible by 3
#define WAVE_MAX_PROFILES			   271			// maximum saved wave profiles

// may be more than double buffering of models
#define WAVE_MAX_DOUBLE_BUFFER		    2			// number of doubled buffers

// filename size limit
#define WAVE_FILENAME_LEN				16
 
// limits
#define WAVE_MAX_WAVES					64			// maximum waves in a zone
#define WAVE_MAX_WHITE					15			// maximum white water models
#define WAVE_MAX_LIPSPRAY				14			// maximum lip spray models

// wave speed up and slow down factors
#define WAVE_FFACE_SPEEDUP				 1.051f
#define WAVE_FFACE_SPEEDHIT				 0.995f

// number of points in a model section
#define WAVE_NPTS						40

// static math values
#define WAVE_ONE_OVER_NINETY			0.0111111f
#define WAVE_ONE_OVER_ONE_EIGHTY		0.0055556f

// number of wave models
#define WAVE_MAX_MODELS					  6

// temp
#define WAVE_MAX_LODS					   3
#define WAVE_MAXPTS_FFACE_SEC			  11
#define WAVE_MAXPTS_BFACE_SEC			   5
#define WAVE_MAX_SECTIONS				  17	

/*
 * typedefs
 */

// wave rendering data
typedef struct {
	GfxObject		*models[WAVE_MAX_DOUBLE_BUFFER][WAVE_MAX_MODELS];	// geometry
	GfxObject		*amodel;											// animation model
	GfxTextureId	 texture_id[2];										// wave water textures
	int				 dbuffer;											// double buffer flag for model switching
	float			 xtrans;											// x translation of wave model
	float			 ytrans;											// y translation of wave model
	int				 yscroll;											// y texture scroll for wave model
} WaveRenderData;

// saved wave points for fast access
typedef struct {
	int				 nprofiles;											// number of profiles (- 1)
	float			 confactor;											// index conversion factor
	Point_f3d		 points[WAVE_MAX_PROFILES][WAVE_NPTS];				// pre-saved front face vertex points
	float			 highest_z[WAVE_MAX_PROFILES];						// pre-saved highest z on profile
	float			 highest_y[WAVE_MAX_PROFILES];						// pre-saved highest y on profile
	float			 longest_y[WAVE_MAX_PROFILES];						// pre-saved longest y on profile
} WaveSavedPoints;

// wave description
typedef struct {
	float			 height;											// height factor of this wave
	float			 position;											// y position of wave (how far away from beach)
	int				 count;												// count flag for set detection
	int				 unused;											// unused
} WaveTypeData;

// wave zone - a group of waves for a wave environment
typedef struct {	
	WaveTypeData	 wave[WAVE_MAX_WAVES];								// wave descriptions
	int				 nwaves;											// total waves described
	float			 wave_speed;										// wave speed
	float			 out_the_back;										// distance to wave starting position
	float			 end_ypos;											// wave end position
	float			 back;												// the wave farthest out the back
} WaveZone;

// positional information for objects on a wave
typedef struct {
	float			 water_level;					// water level
	float			 rate;							// rate of pitch angle
	float			 speed_adjust;					// speed up
	float			 size;							// size of wave
	float			 pitch;							// wave pitch
	float			 roll;							// wave roll
} WavePositionData;

// wave attributes
typedef struct {
	// these values are loaded from a file
	int				 width;											// wave width
	int				 length;										// wave length
	int				 nwaves;										// number of waves in the zone
	int				 nflat;											// number of flat sections in between each wave
	int				 end_ypos;										// wave end position
	int				 speed;											// wave speed
	int				 fface_nsecpts;									// number of front face points in profile
	int				 bface_nsecpts;									// number of back face points in profile
	int				 nsections[WAVE_MAX_LODS];						// number of sections for each LOD
	float			 fface_length[WAVE_MAXPTS_FFACE_SEC];			// front face y lengths
	float			 fface_angles[WAVE_MAXPTS_FFACE_SEC];			// front face angles		
	float			 bface_height[WAVE_MAXPTS_BFACE_SEC];			// back face  height factors
	float			 bface_length[WAVE_MAXPTS_BFACE_SEC];			// back face  y lengths
	char			 fface_file[WAVE_MAX_LODS][WAVE_FILENAME_LEN];	// front face wave model filenames for each LOD
	char			 bface_file[WAVE_MAX_LODS][WAVE_FILENAME_LEN];	// back face wave model filenames for each LOD
	char			 flat_file[WAVE_MAX_LODS][WAVE_FILENAME_LEN];	// "flat" water model filenames for each LOD
	char			 high_res_image[WAVE_FILENAME_LEN];				// high res' wave texture
	char			 low_res_image[WAVE_FILENAME_LEN];				// low  res' wave texture
	float			 r,g,b;											// background color
	float			 drop_point_x;									// point to drop rider out the back
	float			 drop_point_y;									// point to drop rider out the back
	// these values are calculated from the above
	int				 half_width;									// half of wave width
	int				 half_length;									// half of wave length
	int				 fface_npts[WAVE_MAX_LODS];						// number of front face points in model
	int				 bface_npts[WAVE_MAX_LODS];						// number of back face points in model
	float			 xsection[WAVE_MAX_LODS][WAVE_MAX_SECTIONS];	// x section point values for each model LOD
} WaveAttributes;

// flat water data
typedef struct {
	GfxObject		*high;						// high detail flat water model
	GfxObject		*low;						// lower detail flat water model
	GfxObject		*sides;						// side water (between cube and wave)
	GfxMatrix		 mat;						// saved matrix
	GfxTextureId	 water_texture_id;			// water texture id
	int				 trans;						// y translation
	int				 wave_end;					//
} SurfFlatWater;

// lip spray data
typedef struct {
	Bool			 draw[WAVE_MAX_LIPSPRAY];		// draw flags
	GfxObject		*object[3][WAVE_MAX_LIPSPRAY];	// object pointers
	GfxMatrix		 mat;							// saved matrix
	int				 xtrans;						// x translation of wave model
	int				 ytrans;						// y translation of wave model
	int				 swapper;						// lip model swapping flag
	float			 scale;							// scale adjustment
} SurfLipSpray;

// white water data
typedef struct {
	Bool			 draw[WAVE_MAX_WHITE];			// draw flags
	int				 xtrans[WAVE_MAX_WHITE];		// x translation of white model
	int				 ytrans[WAVE_MAX_WHITE];		// y translation of white model
	int				 ztrans[WAVE_MAX_WHITE];		// z translation of white model
	float			 scale[WAVE_MAX_WHITE];			// scale
	GfxObject		*object[WAVE_MAX_WHITE];		// object pointers
	GfxMatrix		 mat;							// saved matrix
	int				 swap;							// white water model swapper 	
} SurfWhiteWater;

/*
 * prototypes
 */

extern float	sintable[];

/* wavattr.c */
extern void surfInitWaveAttributes( WaveAttributes *waveattr, int num );

/* wave.c */
extern void surfInitWave( WaveRenderData *wave, WaveAttributes *attr, WaveSavedPoints *save );
extern void surfWaveSwapBuffers( WaveRenderData *wave );
extern void surfUpdateWave( WaveRenderData *wave, SurfContour *contour, float x, float y, float wavey );
extern void surfDrawWave( WaveRenderData *wave );
extern void surfScrollWave( WaveRenderData *wave );
extern Point_f3d *surfCalcWavePoints( float size, WaveSavedPoints *saved );

/* wavzone.c - wave zones */
extern void			 surfInitWaveZone( WaveZone *zone, WaveAttributes *waveattr );
extern void			 surfUpdateWaveZone( WaveZone *zone, float tick );
extern WaveTypeData *surfGetViewableWave( WaveZone *zone, float posy );
extern WaveTypeData *surfGetViewableWaves( WaveZone *zone, float posy, WaveTypeData **waves, int *nwaves, int max );

/* wavpos.c */
extern void  surfWavePosition( WaveTypeData *waveinfo, SurfContour *contour, WaveSavedPoints *saved, WavePositionData *wavepos, float x, float y, float dir );
extern float surfWaterLevel( WaveTypeData *waveinfo, SurfContour *contour, WaveSavedPoints *saved, float x, float y );

/* lipspray.c */
extern void surfInitLipSpray( WaveAttributes *waveattr, SurfLipSpray *spray );
extern void surfCreateLipSpray( WaveRenderData *wave, SurfLipSpray *spray );
extern void surfDrawLipSpray( SurfLipSpray *spray );

/* flat.c */
extern void surfInitFlatWater( WaveAttributes *waveattr, SurfFlatWater *flat );
extern void surfDrawFlatWater( SurfFlatWater *flat, WaveRenderData *wave, int posy, Bool front_only );

/* white.c */
extern void surfInitWhiteWater( WaveAttributes *waveattr, SurfWhiteWater *white );
extern void surfCreateWhiteWater( WaveRenderData *wave, SurfWhiteWater *white );
extern void surfDrawWhiteWater( SurfWhiteWater *white );

#endif	// WAVE_H

