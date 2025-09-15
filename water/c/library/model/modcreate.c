/*****************************************************************
 * Project		: Model Library
 *****************************************************************
 * File			: modcreate.c
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 9 May 1998
 * Last Update	: 17 May 1998
 *****************************************************************
 * Description	: Model create and destroy functions
 *****************************************************************/

/*
 * includes
 */

#include <stdio.h>
#include <text/text.h>
#include <parse/parse.h>
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
 * modCreate -- create and initialise a model structure
 * 
 * Description:
 *
 * Returns:
 * 
 */

ModModel	*modCreate ( 
	Text		 name,
	ModModel	*parent
	)
{
	ModModel	*model;
	Text		 str;

	/* validate */
	if ( name == NULL ) return( NULL );

	/* allocate memory for new model */
	model = memNew( ModModel );
	memClear( model, sizeof(ModModel) );

	/* init */
	model->parent = parent;
	/* get name */
	str = modExtractFilename( name );
	if ( str )
		model->name = txtDupl( str );
	/* get path */
	str = modExtractPathname( name );
	if ( str != NULL )
		model->path = txtDupl( str );

#if 0
	/*
	model->jts       = NULL;
	model->njts      = 0;
	model->maxjts    = 0;
	model->pts       = NULL;
	model->npts      = 0;
	model->maxpts    = 0;
	model->normals    = NULL;
	model->nnormals   = 0;
	model->maxnormals = 0;
	model->rgb       = NULL;
	model->nrgb      = 0;
	model->maxrgb    = 0;
	model->layers    = NULL;
	model->nlayers   = 0;
	model->maxlayers = 0;
	model->limits.min.x = 0.0;
	model->limits.min.y = 0.0;
	model->limits.min.z = 0.0;
	model->limits.max.x = 0.0;
	model->limits.max.y = 0.0;
	model->limits.max.z = 0.0;
	model->children  = NULL;
	model->nchildren = 0;
	model->maxchildren = 0;
	*/
#endif

	return( model );
}


/******************************************************************************
 * Function:
 * modDestroy -- clean memory for a model
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool	modDestroy ( 
	ModModel	*model
	)
{
	ModLayer	*layer;
	int			 i, j;
	char		 iname[2048];

	/* validate */
	if ( model == NULL ) return( FALSE );

	/* destroy children */
	for ( i=0; i<model->nchildren; i++ )
		modDestroy( model->children[i].model );

	/* destroy model */
	memFree( model->pts );
	memFree( model->name );
	for ( i=0; i<model->njts; i++ ) {
		memFree( model->jts[i].name );
		memFree( model->jts[i].pname );
	}
	memFree( model->jts );
	for ( i=0; i<model->nlayers; i++ ) {
		layer = &model->layers[i];
		memFree( layer->tri );
		memFree( layer->name );
		for ( j=0; j<2; j++ ) {
			if ( model->path != NULL ) {
				sprintf( iname, "%s%s", model->path, layer->image[j] );
				modImageDestroy( iname );
			} else 
				modImageDestroy( layer->image[j] );
			memFree( layer->image[j] );
			//memFree( layer->idata[j] );
		}
	}
	memFree( model->layers );
	memFree( model->path );
	memFree( model );

	return( TRUE );
}

