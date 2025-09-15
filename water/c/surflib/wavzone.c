/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		wavzone.c
 * Author:		Mark Theyer
 * Created:		13 Jul 1999
 **********************************************************
 * Description:	A wave has a description which is in a
 *				zone. A zone is a group of waves which is
 *				managed and updated by a wave generator. A
 *				wave generator describes a wave environment
 *			    and attributes, etc.
 **********************************************************
 * Functions:
 *	surfInitWaveZone() 
 *				Initialise a wave zone using a wave generator.
 *  surfUpdateWaveZone()
 *				Update a wave zone using a wave generator.
 *  surfGetViewableWave()
 *				Get the viewable wave in a zone.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 13-Jul-99	Theyer	Initial coding.
 * 31-May-01	Theyer	Port to PS2
 *
 **********************************************************/

/*
 * includes
 */

#include <wave.h>
#include <surfutil.h>
#include <waverr.h>

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

/*
 * functions
 */


/**********************************************************
 * Function:	surfInitWaveZone
 **********************************************************
 * Description: Initialise a wave zone
 * Inputs:		zone    - pointer to an uninitialised wave 
 *						  zone data structure
 *				wavegen - pointer to initialised wave generator 
 *						  data
 * Notes:		nil
 * Returns:		initialised zone data structure 
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 13-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

void surfInitWaveZone ( 
	WaveZone			*zone, 
	WaveAttributes		*attr
	)
{
	WaveTypeData	*wave;
	int				 i;

	/* validate */
	if ( attr->nwaves > WAVE_MAX_WAVES )
		surfExit( WAVE_ERROR_TOO_MANY_WAVES );

	zone->nwaves       = attr->nwaves;
	zone->wave_speed   = (float)attr->speed;
	zone->end_ypos     = (float)attr->end_ypos;
	zone->back         = 0.0f;
	zone->out_the_back = (float)((attr->nflat + 1) * attr->nwaves * attr->length);
	for ( i=0; i<zone->nwaves; i++ ) {
		wave = &zone->wave[i];
		wave->position = zone->out_the_back - (float)((attr->nflat + 1) * i * attr->length);
	}
}


/**********************************************************
 * Function:	surfUpdateWaveZone
 **********************************************************
 * Description: Initialise a wave zone
 * Inputs:		zone    - pointer to an uninitialised wave zone data structure
 *				wavegen - pointer to initialised wave generator data
 *				tick    - game tick timer value
 * Notes:		nil
 * Returns:		initialised zone data structure 
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 13-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

void surfUpdateWaveZone ( 
	WaveZone			*zone, 
	float				 tick
	)
{
	WaveTypeData	*wave;
	int				 i;
	float			 shift;

	shift = (zone->wave_speed * tick);
	//printf( "st=%d,", shift );
	for ( i=0; i<zone->nwaves; i++ ) {
		wave = &zone->wave[i];
		wave->position -= shift;
		if ( wave->position < zone->end_ypos ) {
			/* put wave back out the back... */
			wave->position += zone->out_the_back;
			zone->back = i;
		}
		//printf( "%d:%d,", i, wave->position );
	}
	//printf( "\n" );
}


/**********************************************************
 * Function:	surfGetViewableWave
 **********************************************************
 * Description: Get the viewable wave in a wave zone
 * Inputs:		zone - pointer to initialised wave zone data
 *				posy - y position of focus point
 * Notes:		nil
 * Returns:		NULL or viewable wave
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 13-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

WaveTypeData *surfGetViewableWave (
	WaveZone	*zone, 
	float		 posy 
	)
{
	WaveTypeData	*wave;
	WaveTypeData	*closest;
	WaveTypeData	*infront[WAVE_MAX_WAVES];
	int				 i, n_infront;

	/* assume we want the next wave in front of us (if there is one) */
	n_infront = 0;
	for ( i=0; i<zone->nwaves; i++ ) {
		wave = &zone->wave[i];
		if ( wave->position > posy ) {
			infront[n_infront] = wave;
			n_infront++;
		}
	}	

	/* no waves viewable? */
	if ( n_infront == 0 ) 
		return( NULL );

	/* get and return closest */
	closest = infront[0];
	for ( i=1; i<n_infront; i++ ) {
		if ( infront[i]->position < closest->position )
			closest = infront[i];
	}

#if 0
	/* temp debug */
	for ( i=0; i<zone->nwaves; i++ ) {
		wave = &zone->wave[i];
		if ( wave == closest )
			printf( "closest=%d,pos=%d\n", i, closest->position );
	}
#endif

	return( closest );
}


/**********************************************************
 * Function:	surfGetViewableWaves
 **********************************************************
 * Description: Get the viewable waves in a wave zone
 * Inputs:		zone - pointer to initialised wave zone data
 *				posy - y position of focus point
 * Notes:		nil
 * Returns:		NULL or viewable wave
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 13-Jul-99	Theyer	Initial coding from existing game
 *
 **********************************************************/

WaveTypeData *surfGetViewableWaves (
	WaveZone	  *zone, 
	float		   posy, 
	WaveTypeData **waves,
	int			  *nwaves,
	int			   max
	)
{
	WaveTypeData	*wave;
	int				 closest;
	int				 infront[WAVE_MAX_WAVES];
	int				 i, n_infront;

	/* init */
	*nwaves = 0;

	/* assume we want the next wave in front of us (if there is one) */
	n_infront = 0;
	for ( i=0; i<zone->nwaves; i++ ) {
		wave = &zone->wave[i];
		if ( wave->position > posy ) {
			infront[n_infront] = i;
			n_infront++;
		}
	}	

	/* no waves viewable? */
	if ( n_infront == 0 ) 
		return( NULL );

	/* get closest */
	closest = infront[0];
	for ( i=1; i<n_infront; i++ ) {
		if ( zone->wave[(infront[i])].position < zone->wave[closest].position )
			closest = infront[i];
	}

	/* save closest wave for return */
	wave = &zone->wave[closest];
	i = closest;

	/* get max waves in view */
	while( *nwaves < max ) {
		/* set next closest wave */
		waves[*nwaves] = &zone->wave[closest];
		(*nwaves)++;
		/* check if at the back */
		if ( closest == zone->back )
			break;
		/* get next closest wave */
		closest--;
		if ( closest < 0 ) 
			closest = (zone->nwaves - 1);
	}

	//printf( "nwaves=%d:%d (back=%d)\n", *nwaves, i, zone->back );
	return( wave );
}

