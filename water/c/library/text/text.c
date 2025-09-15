/**********************************************************
 * Copyright (C) 1998 Mark Theyer
 * All Rights Reserved
 **********************************************************
 * Project:	Nil
 **********************************************************
 * File:	text.c
 * Author:	Mark Theyer
 * Created:	27 Jun 1998
 **********************************************************
 * Description:	text string functions
 **********************************************************
 * Revision History:
 * 27-Jun-98	Theyer	Initial Coding
 **********************************************************/

/*
 * includes
 */

#include <stdio.h>
#include <text/text.h>
#include <memory/memory.h>

/*
 * typedefs
 */

/*
 * macros
 */

#ifdef OsTypeIsPs2
#define toupper(i)		(((i>96)&&(i<123))?(i-30):(i))
#define tolower(i)		(((i>64)&&(i<91))?(i+30):(i))
#endif

/*
 * prototypes
 */

/*
 * global variables
 */

/*
 * functions
 */


#ifndef OsTypeIsPsx
/******************************************************************************
 * Function:
 * txtDupl -- duplicate a text string
 * 
 * Description:
 *
 * Returns:
 * 
 */

Text txtDupl(
    Text 	str
    )
{
    int		i;
    Text	cpy;

	if ( str == NULL ) return( NULL );

    i = txtLength( str ) + 1;
    cpy = (Text)memAlloc( i );
    txtCopy( str, cpy );

    return( cpy );
}
#endif


/******************************************************************************
 * Function:
 * txtToUpper -- convert a text string to upper case
 * 
 * Description:
 *
 * Returns:
 * 
 */

Text txtToUpper (
	Text	str
	)
{
	int		i;

	/* validate */
	if ( str == NULL ) return( NULL );

	i = 0;
	while( str[i] != '\0' ) {
		str[i] = (char)toupper( (int)str[i] );
		i++;
	}

	return( str );
}


/******************************************************************************
 * Function:
 * txtToLower -- convert a text string to lower case
 * 
 * Description:
 *
 * Returns:
 * 
 */

Text txtToLower (
	Text	str
	)
{
	int		i;

	/* validate */
	if ( str == NULL ) return( NULL );

	i = 0;
	while( str[i] != '\0' ) {
		str[i] = (char)tolower( (int)str[i] );
		i++;
	}

	return( str );
}


/******************************************************************************
 * Function:
 * txtCopy -- copy a text string into another buffer
 * 
 * Description:
 *
 * Returns:
 * 
 */

void txtCopy (
	Text	source,
	Text	destination
	)
{
	if ((source == NULL)||(source == '\0')) 
		destination[0] = '\0';
	else
		strcpy( destination, source );
}


/******************************************************************************
 * Function:
 * txtSwapChars -- swap characters in a text string
 * 
 * Description:
 *
 * Returns:
 * 
 */

Text txtSwapChars (
	Text	str,
	char	change_this,
	char	to_that
	)
{
	int		i;

	/* validate */
	if ( str == NULL ) return( NULL );

	i = 0;
	while( str[i] != '\0' ) {
		if ( str[i] == change_this )
			str[i] = to_that;
		i++;
	}

	return( str );
}

