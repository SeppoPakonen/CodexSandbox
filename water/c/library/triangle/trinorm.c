/*****************************************************************
 * Project		: Triangle Library
 *****************************************************************
 * File			: trinormal.c
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 30 Jun 1998
 * Last Update	: 30 Jun 1998
 *****************************************************************
 * Description	: Triangle normal calculation functions
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
 * triCalcNormal -- calculate the normal for a triangle
 * 
 * Description:
 *
 * Returns:
 * 
 */

Point_fx3d	triCalcNormal (
	Point_i3d	*pts
    )
{
	Point_i3d	*pt0;
	Point_i3d	*pt1;
	Point_i3d	*pt2;
	Point_fx3d	 normal;
	Point_i3d	 v1,v2;
	fixed		 max;
#define FABS(v)	 ((v>0)?v:-(v))

	/* init */
	normal.x = FIXED_ZERO;
	normal.y = FIXED_ZERO;
	normal.z = FIXED_ONE;

	/* validate */
	if ( pts == NULL ) return( normal );

	/* no validation... */
	pt0 = &pts[0];
	pt1 = &pts[1];
	pt2 = &pts[2];

    /* calculate normal */
    v1.x = pt0->x - pt2->x;
    v1.y = pt0->y - pt2->y;
    v1.z = pt0->z - pt2->z;
    v2.x = pt1->x - pt0->x;
    v2.y = pt1->y - pt0->y;
    v2.z = pt1->z - pt0->z;
	if ( (! v1.x) && (! v2.x) ) {			// (v1.x == 0 && v2.x == 0)
		normal.x = FIXED_ONE;
		normal.y = FIXED_ZERO;
		normal.z = FIXED_ZERO;
		if ( ! triClockWise( pt0->y, pt0->z, pt1->y, pt1->z, pt2->y, pt2->z ) )
			normal.x = -(FIXED_ONE);
		return( normal );
	} else if ( (! v1.y) && (! v2.y) ) {	// (v1.y == 0 && v2.y == 0)
		normal.x = FIXED_ZERO;
		normal.y = FIXED_ONE;
		normal.z = FIXED_ZERO;
		if ( triClockWise( pt0->x, pt0->z, pt1->x, pt1->z, pt2->x, pt2->z ) )
			normal.y = -(FIXED_ONE);
		return( normal );
	} else if ( (! v1.z) && (! v2.z) ) {	// (v1.z == 0 && v2.z == 0)
		normal.x = FIXED_ZERO;
		normal.y = FIXED_ZERO;
		normal.z = FIXED_ONE;
		if ( ! triClockWise( pt0->x, pt0->y, pt1->x, pt1->y, pt2->x, pt2->y ) )
			normal.z = -(FIXED_ONE);
		return( normal );
	} else {
		normal.x = (v1.y * v2.z) - (v1.z * v2.y);
		normal.y = (v1.z * v2.x) - (v1.x * v2.z);
		normal.z = (v1.x * v2.y) - (v1.y * v2.x);
		/* get maximum */
		if ( (FABS( normal.x )) > (FABS( normal.y )) )
			max = FABS( normal.x );
		else
			max = FABS( normal.y );
		if ( (FABS( normal.z )) > max )
			max = FABS( normal.z);
		/* normalize */
		normal.x = -(FIXED_NORMALISE(normal.x,max));
		normal.y = -(FIXED_NORMALISE(normal.y,max));
		normal.z = -(FIXED_NORMALISE(normal.z,max));
	}

	return( normal );
}


/******************************************************************************
 * Function:
 * triClockWise -- identify if the 3 passed points are clockwise
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool	triClockWise (
	int		x0,
	int		y0,
	int		x1,
	int		y1,
	int		x2,
	int		y2
	)
{
	int		x[2], y[2];
	float	angle[2];

	x[0] = x1 - x0;
	y[0] = y1 - y0;
	x[1] = x2 - x1;
	y[1] = y2 - y1;
	
	angle[0] = (float)atan2( (double)y[0], (double)x[0] );
	angle[1] = (float)atan2( (double)y[1], (double)x[1] );

	if ( ( angle[1] < angle[0] ) && ( angle[1] > ( angle[0] - 3.141592654f ) ) )
		return( TRUE );

	if ( ( angle[0] < 0 ) && ( angle[1] > ( angle[0] + 3.141592654f ) ) )
		return( TRUE );
			
	return( FALSE );
}

#else

/******************************************************************************
 * Function:
 * triCalcNormal -- calculate the normal for a triangle
 * 
 * Description:
 *
 * Returns:
 * 
 */

Point_f3d	triCalcNormal (
	Point_f3d	*pts
    )
{
	Point_f3d	*pt0;
	Point_f3d	*pt1;
	Point_f3d	*pt2;
	Point_f3d	 normal;
	Point_f3d	 v1,v2;
	float		 max;

	/* init */
	normal.x = 0.0f;
	normal.y = 0.0f;
	normal.z = 1.0f;

	/* validate */
	if ( pts == NULL ) return( normal );

	/* no validation... */
	pt0 = &pts[0];
	pt1 = &pts[1];
	pt2 = &pts[2];

    /* calculate normal */
    v1.x = pt0->x - pt2->x;
    v1.y = pt0->y - pt2->y;
    v1.z = pt0->z - pt2->z;
    v2.x = pt1->x - pt0->x;
    v2.y = pt1->y - pt0->y;
    v2.z = pt1->z - pt0->z;
	if ( v1.x == 0.0f && v2.x == 0.0f ) {
		normal.x = 1.0f;
		normal.y = 0.0f;
		normal.z = 0.0f;
		if ( ! triClockWise( pt0->y, pt0->z, pt1->y, pt1->z, pt2->y, pt2->z ) )
			normal.x = -1.0f;
		return( normal );
	} else if ( v1.y == 0.0f && v2.y == 0.0f ) {
		normal.x = 0.0f;
		normal.y = 1.0f;
		normal.z = 0.0f;
		if ( triClockWise( pt0->x, pt0->z, pt1->x, pt1->z, pt2->x, pt2->z ) )
			normal.y = -1.0f;
		return( normal );
	} else if ( v1.z == 0.0f && v2.z == 0.0f ) {
		normal.x = 0.0f;
		normal.y = 0.0f;
		normal.z = 1.0f;
		if ( ! triClockWise( pt0->x, pt0->y, pt1->x, pt1->y, pt2->x, pt2->y ) )
			normal.z = -1.0f;
		return( normal );
	} else {
		normal.x = (v1.y * v2.z) - (v1.z * v2.y);
		normal.y = (v1.z * v2.x) - (v1.x * v2.z);
		normal.z = (v1.x * v2.y) - (v1.y * v2.x);
#define FABS(v)		((v>0)?v:-(v))

		/* get maximum */
		if ( (FABS( normal.x )) > (FABS( normal.y )) )
			max = (float)FABS( normal.x );
		else
			max = (float)FABS( normal.y );
		if ( (FABS( normal.z )) > max )
			max = (float)FABS( normal.z);

		/* normalize */
		normal.x /= max;
		normal.y /= max;
		normal.z /= max;
		normal.x *= -1;
		normal.y *= -1;
		normal.z *= -1;
	}

	return( normal );
}


/******************************************************************************
 * Function:
 * triClockWise -- identify if the 3 passed points are clockwise
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool	triClockWise (
	float	x0,
	float	y0,
	float	x1,
	float	y1,
	float	x2,
	float	y2
	)
{
	float	x[2], y[2];
	float	angle[2];
	//char	buf[256];

	x[0] = x1 - x0;
	y[0] = y1 - y0;
	x[1] = x2 - x1;
	y[1] = y2 - y1;
	
	angle[0] = (float)atan2( (double)y[0], (double)x[0] );
	angle[1] = (float)atan2( (double)y[1], (double)x[1] );

	if ( ( angle[1] < angle[0] ) && ( angle[1] > ( angle[0] - 3.141592654f ) ) )
		return( TRUE );

	if ( ( angle[0] < 0 ) && ( angle[1] > ( angle[0] + 3.141592654f ) ) )
		return( TRUE );

	return( FALSE );
}

#endif
