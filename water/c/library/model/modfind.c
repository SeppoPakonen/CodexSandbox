/*****************************************************************
 * Project		: Model Library
 *****************************************************************
 * File			: modfind.c
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 30 May 1998
 * Last Update	: 30 May 1998
 *****************************************************************
 * Description	: Model library find functions
 *****************************************************************/

/*
 * includes
 */

#include <stdio.h>
#include <text/text.h>
#include <memory/memory.h>
#include "model.h"

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
 * modFindLayer -- find a layer in a model
 * 
 * Description:
 *
 * Returns:
 * 
 */

ModLayer * modFindLayer (
	ModModel    *model,
	Text		 layer
	)
{	
	int			 i;

	/* validate */
	if ( model == NULL || layer == NULL ) return( NULL );

	for ( i=0; i<model->nlayers; i++ ) {
		if ( txtMatch( layer, model->layers[i].name ) ) 
			return( &model->layers[i] );
	}

	return( NULL );
}


/******************************************************************************
 * Function:
 * modFindJoint -- find a joint in a model
 * 
 * Description:
 *
 * Returns:
 * 
 */

ModJoint * modFindJoint (
	ModModel    *model,
	Text		 joint
	)
{	
	int			 i;

	/* validate */
	if ( model == NULL || joint == NULL ) return( NULL );

	if ( txtMatch( joint, "world" ) )
		return( NULL );

	for ( i=0; i<model->njts; i++ ) {
		if ( txtMatch( joint, model->jts[i].name ) ) 
			return( &model->jts[i] );
	}

	return( NULL );
}


/******************************************************************************
 * Function:
 * modFindChild -- find a child in a model
 * 
 * Description:
 *
 * Returns:
 * 
 */

ModChild * modFindChild (
	ModModel    *model,
	Text		 childname
	)
{	
	int			 i;

	/* validate */
	if ( model == NULL || childname == NULL ) return( NULL );

	for ( i=0; i<model->nchildren; i++ ) {
		if ( txtMatch( childname, model->children[i].name ) ) 
			return( &model->children[i] );
	}

	return( NULL );
}


/******************************************************************************
 * Function:
 * modFindPointReference -- find a point index from a reference number
 * 
 * Description:
 *
 * Returns:
 * 
 */

int		modFindPointReference (
	ModModel    *model,
	int			 n
	)
{	
	int			 i;

	/* validate */
	if ( model == NULL || n <= 0 ) 
		return(0);

	/* find point index */
	for ( i=0; i<model->npts; i++ ) {
		if ( model->ptsi[i] == n )
			return(i);
	}

	return(0);
}

