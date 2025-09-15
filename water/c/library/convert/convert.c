/**********************************************************
 * Copyright (C) 1998 Mark Theyer
 * All Rights Reserved
 **********************************************************
 * Project:	Binary library
 **********************************************************
 * File:	convert.c
 * Author:	Mark Theyer
 * Created:	27 Dec 1998
 **********************************************************
 * Description:	Convert from one basic type to another
 **********************************************************
 * Revision History:
 * 27-Dec-98	Theyer	Initial Coding
 **********************************************************/

/*
 * includes
 */

#include <compile.h>
#include <type/datatype.h>
#include <text/text.h>
#include <binary/binary.h>

/*
 * typedefs
 */

/*
 * macros
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
   
           
/******************************************************************************
 * Function:
 * cnvIntToString -- convert an integer type to a string
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool cnvIntToString (
	int		 i,
	Text	 buffer
	)
{
	char	 temp[50];
	int		 j, t;
	int		 r, m;

	/* init */
	j = 0;
	buffer[0] = '\0';

	/* shortcut for zero */
	if ( i == 0 ) {
		buffer[0] = '0';
		buffer[1] = '\0';
		return( TRUE );
	}

	/* negative? */
	if ( i < 0 ) {
		buffer[j] = '-';
		j++;
		/* change to positive now... */
		i = -(i);
	}

	/* reverse save numbers in another string */
	t = 0;
	r = 0;
	m = i;
	while( m > 0 ) {
		r = m % 10;
		m -= r;
		if ( m > 0 ) 
			m /= 10;
		temp[t] = 48 + r;
		t++;
	}

	/* put reverse string into buffer */
	while ( t > 0 ) {
		t--;
		buffer[j] = temp[t];
		j++;
	}

	/* terminate */
	buffer[j] = '\0';

	return( TRUE );
}


 