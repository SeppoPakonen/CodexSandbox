/**********************************************************
 * Copyright (C) 1999,2000 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:	PlayStation surf game
 **********************************************************
 * File:	random.c
 * Author:	Mark Theyer
 * Created:	120 June 2000
 **********************************************************
 * Description:	Random store related functions
 **********************************************************
 * Functions:
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 20-Jun-00	Theyer	Initial coding.
 *
 **********************************************************/

/*
 * includes
 */

#include <random.h>

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
 * global variables
 */

static SurfRandom	*random = NULL;

/*
 * functions
 */


/**********************************************************
 * Function:	surfInitRandom
 **********************************************************
 * Description: Initialise random number store
 * Inputs:		random - random data
 * Notes:		nil
 * Returns:		initialised data structure
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 20-Jun-00	Theyer	Initial coding.
 *
 **********************************************************/

void surfInitRandom ( 
	SurfRandom	*data,
	Bool		 init
	)
{
	int			 i;
	fixed		 rand;

	/* init */
	random = data;

	if ( init ) {
		random->n = 0;
		for ( i=0; i<SURF_RANDOM_MAX_SET; i++ ) {
			/* range -1..2 */
			rand = gfxRandom( INT_TO_FIXED(-1), INT_TO_FIXED(2) );
			random->set_n1_2[i] = (short)FIXED_TO_INT(rand);
			/* range -15..15 */
			rand = gfxRandom( INT_TO_FIXED(-15), INT_TO_FIXED(15) );
			random->set_n15_15[i] = (short)FIXED_TO_INT(rand);
			/* range -25..75 */
			rand = gfxRandom( INT_TO_FIXED(-25), INT_TO_FIXED(75) );
			random->set_n25_75[i] = (short)FIXED_TO_INT(rand);
			/* range -164..164 */
			rand = gfxRandom( INT_TO_FIXED(-164), INT_TO_FIXED(164) );
			random->set_n164_164[i] = (short)FIXED_TO_INT(rand);
		}
	}
}


/**********************************************************
 * Function:	surfGetRandom
 **********************************************************
 * Description: Get a random number from the store
 * Inputs:		random - random data
 *				type   - number type to get
 * Notes:		nil
 * Returns:		random number or 0
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 19-Aug-99	Theyer	Initial coding.
 *
 **********************************************************/

int surfGetRandom ( 
	int			 type
	)
{
	int			 num;

	/* init */
	num = 0;

#if 1
	if ( random ) {
		switch( type ) {
		case SURF_RANDOM_SET_N1_2:
			num = (int)random->set_n1_2[random->n];
			break;
		case SURF_RANDOM_SET_N15_15:
			num = (int)random->set_n15_15[random->n];
			break;
		case SURF_RANDOM_SET_N25_75:
			num = (int)random->set_n25_75[random->n];
			break;
		case SURF_RANDOM_SET_N164_164:
			num = (int)random->set_n164_164[random->n];
			break;
		}
	}

	/* cycle selector */
	random->n++;
	if ( random->n == SURF_RANDOM_MAX_SET )
		random->n = 0;
#endif

	return( num );
}

