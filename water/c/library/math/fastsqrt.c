/*****************************************************************
 * Project		: Math Library
 *****************************************************************
 * File			: fastsqrt.c
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 23 Aug 1999
 * Last Update	: 23 Aug 1999
 *****************************************************************
 * Description	: Fast table based square root function
 *****************************************************************/

/*
 * includes
 */

#include <math/sqrtable.h>
#include "math.h"

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


/******************************************************************************
 * Function:
 * mthFastSqrt -- return the approximate square root for a value
 * 
 * Description: Works with values from 1 to 65535
 *
 * Returns: 0 (zero) if out of range
 * 
 */

int mthFastSqrt (
	int		value
	)
{
	register int	i;

	/* validate */
	if ( value < 1 || value > 65535 ) {
		printf( "mthFastSqrt: sqrt out of range (%d)\n", value );
		return( 0 );
	}

	/* start in the middle or end? */
	if ( value < 16384 )
		i = 126;
	else
		i = 255;

	/* find the sqrt in the table */
	while( value < (int)sqrtable[i] )			
		i--;

	return( i );
}


