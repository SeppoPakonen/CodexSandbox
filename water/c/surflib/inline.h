/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		inline.h
 * Author:		Mark Theyer
 * Created:		12 Jul 2001
 **********************************************************
 * Description:	Header file for inline fast "functions"
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 12-Jul-01	Theyer	Initial coding for PS2
 *
 **********************************************************/

#ifndef SURF_INLINE_H
#define SURF_INLINE_H

/* 
 * includes
 */

#include <stdio.h>

#include <compile.h>
#include <type/datatype.h>

#include <wave.h>
#include <swell.h>
#include <alpha.h>
#include <surfutil.h>

#include <gfx/ps2_il.h>

/* 
 * macros 
 */

// pre-declared variables required: fpts, spts, w
#define surfCalcFrontPointsFAST(isize,xvalue,nsecpts,save_pts) { \
		/* init */ \
		fpts = (Point_f3d *)(save_pts); \
		spts = (Point_f3d *)(saved->front_pts[isize]); \
		/* set points */  \
		fpts->x = xvalue; \
		fpts->y = 0.0f;   \
		fpts->z = 0.0f;   \
		fpts++; \
		spts++; \
		for ( w=1; w<nsecpts; w++, fpts++, spts++ ) { \
			fpts->x = xvalue;  \
			fpts->y = spts->y; \
			fpts->z = spts->z; \
		} \
	}

// pre-declared variables required: fpts, fy, w
#define surfCalcFlatH1PointsFAST(xvalue,nsecpts,save_pts) { \
		/* init */ \
		fpts = (Point_f3d *)(save_pts); \
		fy   = (float *)(saved->flat_ypts[0]); \
		/* set points */  \
		for ( w=0; w<nsecpts; w++, fpts++, fy++ ) { \
			fpts->x = xvalue; \
			fpts->y = *fy; \
			fpts->z = 0.0f; \
		} \
	}

// pre-declared variables required: fpts, fy, w
#define surfCalcFlatH2PointsFAST(xvalue,nsecpts,save_pts) { \
		/* init */ \
		fpts = (Point_f3d *)(save_pts); \
		fy   = (float *)(saved->flat_ypts[1]); \
		/* set points */  \
		for ( w=0; w<nsecpts; w++, fpts++, fy++ ) { \
			fpts->x = xvalue; \
			fpts->y = *fy; \
			fpts->z = 0.0f; \
		} \
	}

// pre-declared variables required: fpts, spts, w
#define surfCalcBackPointsFAST(isize,xvalue,nsecpts,save_pts) { \
		/* init */ \
		fpts = (Point_f3d *)(save_pts); \
		spts = (Point_f3d *)(saved->back_pts[isize]); \
		/* set points */  \
		for ( w=0; w<nsecpts; w++, fpts++, spts++ ) { \
			fpts->x = xvalue;  \
			fpts->y = spts->y; \
			fpts->z = spts->z; \
		} \
	}

// pre-declared variables required: w, rgba, srgba
#define surfCalcWaveRGBAFAST(nsecpts,save_rgba,saved_rgba) { \
		rgba  = save_rgba; \
		srgba = saved_rgba; \
		for ( w=0; w<nsecpts; w++, rgba++, srgba++ ) \
			*rgba = *srgba; \
	}

// pre-declared variables required: w, rgba
#define surfCalcFlatRGBAFAST(nsecpts,save_rgba) { \
		rgba  = save_rgba; \
		/* normal colour */ \
		for ( w=0; w<nsecpts; w++, rgba++ ) { \
			rgba->r = 127; \
			rgba->g = 127; \
			rgba->b = 127; \
			rgba->a = 0;  \
		} \
	}

// pre-declared variables required: w, i, ix, iy, sw, fpts, height, pos, angle, sine
#define surfCalcSwellFAST(swell,save_pts,npts,xpos,ypos) { \
		int				 ix; \
		int				 iy; \
		int				 ns; \
		float			 height; \
		int				 i; \
		WaveSwellData	*sw; \
		int				 pos; \
		/* init */ \
		fpts = (Point_f3d *)(save_pts); \
		ix = (int)xpos; \
		ns = swell->nswells; \
		for ( w=0; w<npts; w++, fpts++ ) { \
			/* init */ \
			height = 0.0f; \
			iy = (int)(ypos + fpts->y); \
			for ( i=0; i<ns; i++ ) { \
				/* init */ \
				sw = (swell->data + i); \
				/* get value of 0 to 0x40000000 */ \
				if ( sw->axis == WAVE_SWELL_AXIS_X ) \
					pos = ((ix + sw->position) & sw->mask) * sw->normalise; \
				else \
					pos = ((iy + sw->position) & sw->mask) * sw->normalise; \
				pos >>= 12; \
				pos *= 180; \
				pos >>= 18; \
				height += (sw->peak - (triSinFAST(pos) * sw->height)); \
				/* height += gfxRandom( -4.0f, 4.0f ); */ \
			} \
			/* update z */ \
			fpts->z += height; \
		} \
	}
//		float			*sintab;
//		sintab = (float *)(ps2ScratchPad | 9680);
//				height += (sw->peak - (*(sintab + pos) * sw->height));
//				height += (sw->peak - (triSinFAST(pos) * sw->height));

// pre-declared variables required: w, i, ix, iy, sw, fpts, height, pos, angle, sine
#define surfCalcAlphaFAST(alpha,save_pts,save_col,npts,xpos,ypos) { \
		GfxColor		*fcol; \
		int				 ix; \
		int				 iy; \
		int				 na; \
		int				 calpha; \
		int				 i; \
		WaveAlphaData	*al; \
		int				 pos; \
		/* init */ \
		fpts = (Point_f3d *)(save_pts); \
		fcol = (GfxColor *)(save_col); \
		ix = (int)xpos; \
		na = alpha->nalpha; \
		for ( w=0; w<npts; w++, fpts++, fcol++ ) { \
			/* init */ \
			calpha = fcol->a; \
			iy = (int)(ypos + fpts->y); \
			for ( i=0; i<na; i++ ) { \
				/* init */ \
				al = &alpha->data[i]; \
				/* get value of 0 to 0x40000000 */ \
				if ( al->axis == WAVE_ALPHA_AXIS_X ) \
					pos = ((ix + al->position) & al->mask) * al->normalise; \
				else \
					pos = ((iy + al->position) & al->mask) * al->normalise; \
				pos >>= 12; \
				pos *= 180; \
				pos >>= 18; \
				calpha += *(al->saved + pos); \
			} \
			/* limit */ \
			if ( calpha > WAVE_ALPHA_MAX ) calpha = WAVE_ALPHA_MAX; \
			if ( calpha < WAVE_ALPHA_MIN ) calpha = WAVE_ALPHA_MIN; \
			/* update alpha */ \
			fcol->a = 128 - calpha; \
		} \
	}

// pre-declared variables required: (none)
#define surfDrawWaveFAST(wave,object) { \
		GfxObject	*draw; \
		/* draw front face */ \
		draw = object; \
		gfxResetMatrix( draw ); \
		gfxTranslateObject( draw, -(wave->xtrans), -(wave->ytrans), 0.0f ); \
		gfxDrawObject( draw ); \
	}

// pre-declared variables required: (none)
#define surfWaveSwapBuffersFAST(wave) { \
		/* swap wave buffer */ \
		wave->dbuffer++; \
		if ( wave->dbuffer == WAVE_MAX_DOUBLE_BUFFER ) \
			wave->dbuffer = 0; \
	}

/*
 * typedefs
 */

/*
 * prototypes
 */


#endif	// SURF_INLINE_H

