/*****************************************************************
 * Project		: Triangle Library
 *****************************************************************
 * File			: tricosin.c
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 30 Jun 1998
 * Last Update	: 30 Jun 1998
 *****************************************************************
 * Description	: Triangle cosine and sine fast calculation functions
 *****************************************************************/

/*
 * includes
 */

#ifdef TRI_FIXEDPOINT_API
#include <triangle/fixed.h>
#else
#include <triangle/tables.h>
#endif

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
 * triCos -- return the cosine for an angle in degrees
 * 
 * Description:
 *
 * Returns:
 * 
 */

fixed	triCos (
	int		angle
    )
{
	return( (fixed)fixed_cos_table[angle] );
}


/******************************************************************************
 * Function:
 * triACos -- return the arccosine angle in degrees
 * 
 * Description:
 *
 * Returns:
 * 
 */

fixed	triACos (
	fixed	value
    )
{
	int		index;

	// 1 = 0, 0=180, -1 = 360
	if ( value == 0 )
		return( INT_TO_FIXED(180) );
	if ( value >= FIXED_ONE )
		return( 0 );
	if ( value <= -(FIXED_ONE) )
		return( INT_TO_FIXED(360) );

	// grab from table
	if ( value < 0 ) {
		//index = (1024 - (ABS(value)>>2));
		index = (4096 - ABS(value));
		return( fixed_acos_table[index] );
	} else {
		//index = (1024 + (ABS(value)>>2));
		index = (4096 + ABS(value));
		return( fixed_acos_table[index] );
	}

	return( 0 );
}


/******************************************************************************
 * Function:
 * triSin -- return the sine for an angle in degrees
 * 
 * Description:
 *
 * Returns:
 * 
 */

fixed	triSin (
	int		angle
    )
{
	return( (fixed)fixed_sin_table[angle] );
}

#else

/******************************************************************************
 * Function:
 * triCos -- return the cosine for an angle in degrees
 * 
 * Description:
 *
 * Returns:
 * 
 */

float	triCos (
	int		angle
    )
{
	return( costable[angle] );
}


/******************************************************************************
 * Function:
 * triSin -- return the sine for an angle in degrees
 * 
 * Description:
 *
 * Returns:
 * 
 */

float	triSin (
	int		angle
    )
{
	return( sintable[angle] );
}

#endif
