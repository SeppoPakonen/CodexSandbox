/*****************************************************************
 * Project		: Triangle Library
 *****************************************************************
 * File			: tritexture.c
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 30 May 1998
 * Last Update	: 30 May 1998
 *****************************************************************
 * Description	: Add to model functions
 *****************************************************************/

/*
 * includes
 */

#include <compile.h>
#include <stdio.h>

#ifdef OsTypeIsPsx
#include <gfx/gfx.h>
#else
#include <math.h>
#endif

#include <memory/memory.h>
#include "triangle.h"

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
 * variables
 */

/* 
 * globals variables...
 */


/*
 * functions
 */


#ifdef TRI_FIXEDPOINT_API

/******************************************************************************
 * Function:
 * triCalcTextureCoords -- calculate texture coordinates for a triangle
 * 
 * Description:
 *
 * Returns:
 * 
 */

Point_fx2d * triCalcTextureCoords (
	Point_i3d	*pts,
	Point_i3d	*limits,
	char		 xaxis,
	char		 yaxis
	)
{
	int					i;
	static Point_fx2d	texpts[3];

	/* validate */
	if ( pts == NULL || limits == NULL ) return( NULL );

	for ( i=0; i<3; i++ )
		texpts[i] = triCalcTextureCoord( pts[i], limits, xaxis, yaxis );

	return( texpts );
}


/******************************************************************************
 * Function:
 * triCalcTextureCoord -- calculate texture coordinate for a point
 * 
 * Description:
 *
 * Returns:
 * 
 */

Point_fx2d triCalcTextureCoord (
	Point_i3d	 pt,
	Point_i3d	*limits,
	char		 xaxis,
	char		 yaxis
	)
{
	Point_i3d	 min;
	Point_i3d	 max;
	int			 xsize;
	int			 ysize;
	int			 zsize;
	Point_fx2d   texpt;

	/* init */
	texpt.x = FIXED_ZERO;
	texpt.y = FIXED_ZERO;

	/* validate */
	if ( limits == NULL ) return( texpt );
	
	/* init limits */
	min = limits[0];
	max = limits[1];

	/* calculate lengths for each axis */
	xsize = max.x - min.x;
	ysize = max.y - min.y;
	zsize = max.z - min.z;

	/* calculate texture point */
	switch( xaxis ) {
	case TRI_X_AXIS:
	case TRI_NEG_X_AXIS:
		texpt.x = FIXED_ONE - FIXED_NORMALISE( (pt.x - min.x), xsize );
		break;
	case TRI_Y_AXIS:
	case TRI_NEG_Y_AXIS:
		texpt.x = FIXED_NORMALISE( (pt.y - min.y), ysize );
		break;
	case TRI_Z_AXIS:
	case TRI_NEG_Z_AXIS:
		texpt.x = FIXED_ONE - FIXED_NORMALISE( (pt.z - min.z), zsize );
		break;
	}
	switch( xaxis ) {
	case TRI_NEG_X_AXIS:
	case TRI_NEG_Y_AXIS:
	case TRI_NEG_Z_AXIS:
		texpt.x = FIXED_ONE - texpt.x;
	}
	switch( yaxis ) {
	case TRI_X_AXIS:
	case TRI_NEG_X_AXIS:
		texpt.y = FIXED_ONE - FIXED_NORMALISE( (pt.x - min.x), xsize );
		break;
	case TRI_Y_AXIS:
	case TRI_NEG_Y_AXIS:
		texpt.y = FIXED_NORMALISE( (pt.y - min.y), ysize );
		break;
	case TRI_Z_AXIS:
	case TRI_NEG_Z_AXIS:
		texpt.y = FIXED_ONE - FIXED_NORMALISE( (pt.z - min.z), zsize );
		break;
	}
	switch( yaxis ) {
	case TRI_NEG_X_AXIS:
	case TRI_NEG_Y_AXIS:
	case TRI_NEG_Z_AXIS:
		texpt.y = FIXED_ONE - texpt.y;
	}

	return( texpt );
}

#else

/******************************************************************************
 * Function:
 * triCalcTextureCoords -- calculate texture coordinates for a triangle
 * 
 * Description:
 *
 * Returns:
 * 
 */

Point_f2d * triCalcTextureCoords (
	Point_f3d	*pts,
	Point_f3d	*limits,
	char		 xaxis,
	char		 yaxis
	)
{
	int					i;
	static Point_f2d	texpts[3];

	/* validate */
	if ( pts == NULL || limits == NULL ) return( NULL );

	for ( i=0; i<3; i++ )
		texpts[i] = triCalcTextureCoord( pts[i], limits, xaxis, yaxis );

	return( texpts );
}

/******************************************************************************
 * Function:
 * triCalcTextureCoord -- calculate texture coordinate for a point
 * 
 * Description:
 *
 * Returns:
 * 
 */

Point_f2d triCalcTextureCoord (
	Point_f3d	 pt,
	Point_f3d	*limits,
	char		 xaxis,
	char		 yaxis
	)
{
	Point_f3d	 min;
	Point_f3d	 max;
	float		 xsize;
	float		 ysize;
	float		 zsize;
	Point_f2d    texpt;

	/* init */
	texpt.x = 0.0f;
	texpt.y = 0.0f;

	/* validate */
	if ( limits == NULL ) return( texpt );
	
	/* init limits */
	min = limits[0];
	max = limits[1];

	/* calculate lengths for each axis */
	xsize = max.x - min.x;
	ysize = max.y - min.y;
	zsize = max.z - min.z;

	/* calculate texture point */
	switch( xaxis ) {
	case TRI_X_AXIS:
	case TRI_NEG_X_AXIS:
		texpt.x = (float)1.0f - ((pt.x - min.x)/xsize);
		break;
	case TRI_Y_AXIS:
	case TRI_NEG_Y_AXIS:
		texpt.x = (pt.y - min.y)/ysize;
		break;
	case TRI_Z_AXIS:
	case TRI_NEG_Z_AXIS:
		texpt.x = (float)1.0f - (pt.z - min.z)/zsize;
		break;
	}
	switch( xaxis ) {
	case TRI_NEG_X_AXIS:
	case TRI_NEG_Y_AXIS:
	case TRI_NEG_Z_AXIS:
		texpt.x = (float)1.0f - texpt.x;
	}
	switch( yaxis ) {
	case TRI_X_AXIS:
	case TRI_NEG_X_AXIS:
		texpt.y = (float)1.0f - ((pt.x - min.x)/xsize);
		break;
	case TRI_Y_AXIS:
	case TRI_NEG_Y_AXIS:
		texpt.y = (pt.y - min.y)/ysize;
		break;
	case TRI_Z_AXIS:
	case TRI_NEG_Z_AXIS:
		texpt.y = (float)1.0f - (pt.z - min.z)/zsize;
		break;
	}
	switch( yaxis ) {
	case TRI_NEG_X_AXIS:
	case TRI_NEG_Y_AXIS:
	case TRI_NEG_Z_AXIS:
		texpt.y = (float)1.0f - texpt.y;
	}

	return( texpt );
}

#endif
