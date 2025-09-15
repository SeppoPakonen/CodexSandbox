/**********************************************************
 * Copyright (C) 1999,2000 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:	PlayStation surf game
 **********************************************************
 * File:	splash.h
 * Author:	Mark Theyer
 * Created:	15 Jul 1997
 **********************************************************
 * Description: Key event codes
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 15-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

#ifndef SURF_SPLASH_H
#define SURF_SPLASH_H

/*
 * includes
 */

#include <type/datatype.h>
#include <gfx/gfx.h>
#include <vehicle.h>
#include <wave.h>

/*
 * macros
 */

#define SURF_MAX_SPLASH_TRAIL				20

/* arg values for surfMakeSplash() */
#define SURF_SPLASH_LANDED					 0
#define SURF_SPLASH_TAKEOFF					 1
#define SURF_SPLASH_MOVING					 2

/*
 * typedefs
 */

typedef struct {
	int					 cnt;
	int					 n;
	Bool				 draw_behind_frontface;
	GfxObject			*splash1;		/* splash model	   */
	GfxObject			*submerge;		/* splash model	   */
	GfxObject			*bksplash;		/* splash model	   */
	GfxObject			*wash;			/* splash model	   */
	GfxObject			*flat;			/* splash model	   */
	GfxObject			*hole1;			/* splash model	   */
	GfxObject			*hole2;			/* splash model	   */
	GfxObject			*landed;		/* splash model	   */
	GfxObject			*idle;			/* idle model	   */
	GfxMatrix			 spmat;
	GfxMatrix			 savemat[SURF_MAX_SPLASH_TRAIL];
	int					 posx[SURF_MAX_SPLASH_TRAIL];
	int					 posy[SURF_MAX_SPLASH_TRAIL];
	int					 posz[SURF_MAX_SPLASH_TRAIL];
	Bool				 drawcnt[SURF_MAX_SPLASH_TRAIL];
	GfxObject			*draw[SURF_MAX_SPLASH_TRAIL];
	GfxTextureId		 texid;
	WavePositionData	 posdata[SURF_MAX_SPLASH_TRAIL];
	struct {							/* big splash		*/
		Bool				 draw;		/* draw flag		*/
		GfxObject			*splash;	/* splash model		*/
		int					 posx;		/* x position		*/
		int					 posy;
		int					 posz;
		fixed				 drawcnt;
	} big;
	struct {							/* big splash		*/
		Bool				 draw;		/* draw flag		*/
		GfxObject			*splash;	/* splash model		*/
		int					 posx;		/* x position		*/
		int					 posy;
		int					 posz;
		fixed				 drawcnt;
	} smal;
} SurfSplashData;

/*
 * prototypes
 */

extern void surfInitSplash( SurfSplashData *splash );
extern void surfUpdateSplash( SurfVehicle *veh, SurfSplashData *splash );
extern void surfDrawSplash( SurfSplashData *splash, int posx, int posy );
extern void surfResetSplash( SurfSplashData *splash );
extern void surfMakeSplash( SurfVehicle *veh, int type, SurfSplashData *splash );
extern void surfMakeSmallSplash( SurfSplashData *splash, int posx, int posy );
extern void surfMakeBigSplash( SurfSplashData *splash, int posx, int posy );
extern void surfUpdateSplashTextures( SurfSplashData *splash );

#endif	// SURF_SPLASH_H
