/*****************************************************************
 * Project		: Model Library
 *****************************************************************
 * File			: modedit.c
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 01 Feb 2000
 * Last Update	: 01 Feb 2000
 *****************************************************************
 * Description	: Model library editing functions
 *****************************************************************/

/*
 * includes
 */

#include <stdio.h>
#include <text/text.h>
#include <memory/memory.h>
#include <math/math.h>
#include <parse/parse.h>
#include <matrix/matrix.h>
#include "model.h"
#include "model_int.h"

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
 * modRename -- rename a model
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool	modRename (
	ModModel *model,
	Text	  name
    )
{
	/* validate */
	if ( model == NULL || name == NULL ) return( FALSE );

	memFree( model->name );
	model->name = txtDupl( name );

	return( TRUE );
}


/******************************************************************************
 * Function:
 * modScale -- scale a models points
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool	modScale (
	ModModel	*model,
	float		 scalex,
	float		 scaley,
	float		 scalez
	)
{
	int			 i;
	ModChild	*child;

	/* validate */
	if ( model == NULL || scalex == 0.0f || scaley == 0.0f || scalez == 0.0f ) 
		return( FALSE );

	/* scale points */
	for ( i=0; i<model->npts; i++ ) {
		model->pts[i].x *= scalex;
		model->pts[i].y *= scaley;
		model->pts[i].z *= scalez;
	}

	/* scale child translations */
	for ( i=0; i<model->nchildren; i++ ) {
		child = (ModChild *)&model->children[i];
		child->position.x *= scalex;
		child->position.y *= scaley;
		child->position.z *= scalez;
	}

	return( TRUE );
}


/******************************************************************************
 * Function:
 * modSwapXY -- swap x and y axis points and flip triangle orientation
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool	modSwapXY (
	ModModel	*model
	)
{
	int			 i, j, itmp;
	float		 ftmp;
	Point_f2d	 f2dtmp;
	ModTriangle *tri;
	ModLayer	*layer;
	ModChild	*child;

	/* validate */
	if ( model == NULL ) 
		return( FALSE );

	/* scale points */
	for ( i=0; i<model->npts; i++ ) {
		ftmp = model->pts[i].x;
		model->pts[i].x = model->pts[i].y;
		model->pts[i].y = ftmp;
	}

	/* swap triangle orientations and mirror orientation if necessary */
	for ( i=0; i<model->nlayers; i++ ) {
		layer = &model->layers[i];
		/* mirror orientation */
		if ( layer->mirrored ) {
			switch( layer->mirroraxis ) {
			case MOD_X_AXIS:
				layer->mirroraxis = MOD_Y_AXIS;
				break;
			case MOD_NEG_X_AXIS:
				layer->mirroraxis = MOD_NEG_Y_AXIS;
				break;
			case MOD_Y_AXIS:
				layer->mirroraxis = MOD_X_AXIS;
				break;
			case MOD_NEG_Y_AXIS:
				layer->mirroraxis = MOD_NEG_X_AXIS;
				break;
			}
		}
		/* change triangle orientation */
		for ( j=0; j<layer->ntri; j++ ) {
			tri = &layer->tri[j];
			/* swap triangle points */
			itmp = tri->pts[1];
			tri->pts[1] = tri->pts[2];
			tri->pts[2] = itmp;
			/* swap texture points to match */
			f2dtmp = tri->texpts[1];
			tri->texpts[1] = tri->texpts[2];
			tri->texpts[2] = f2dtmp;
		}
	}

	/* swap child x,y location, scale and rotation values */
	for ( i=0; i<model->nchildren; i++ ) {
		child = (ModChild *)&model->children[i];
		ftmp = child->position.x;
		child->position.x = child->position.y;
		child->position.y = ftmp;
		ftmp = child->scale.x;
		child->scale.x = child->scale.y;
		child->scale.y = ftmp;
		ftmp = child->angle.x;
		child->angle.x = child->angle.y;
		child->angle.y = ftmp;
	}

	return( TRUE );
}


/******************************************************************************
 * Function:
 * modTessellate -- convert all primitives to triangles
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool	modTessellate (
	ModModel	*model
	)
{
	int			 i, j, itmp;
	float		 ftmp;
	Point_f2d	 f2dtmp;
	ModTriangle *tri;
	ModLayer	*layer;
	ModChild	*child;
	int			 p[4];

	/* validate */
	if ( model == NULL ) 
		return( FALSE );

	/* convert triangles in all layers */
	for ( i=0; i<model->nlayers; i++ ) {
		layer = &model->layers[i];
		/* convert quads to a triangle and add another */
		for ( j=0; j<layer->ntri; j++ ) {
			tri = &layer->tri[j];
			if ( tri->quad ) {
				/* save second triangle points */
				p[0] = tri->pts[2] + 1;
				p[1] = tri->pts[1] + 1;
				p[2] = tri->pts[3] + 1;
				p[3] = 0;
				/* convert to single triangle */
				tri->pts[3] = 0;
				tri->quad = FALSE;
				/* add new triangle taken from quad */
				modAddTriangle( model, layer->name, FALSE, p[0], p[1], p[2], p[3] );
			}
		}
	}

	return( TRUE );
}


/******************************************************************************
 * Function:
 * modSwapQuadZigZag -- swap quads zig zag orientation
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool	modSwapQuadZigZag (
	ModModel	*model
	)
{
	int			 i, j, itmp;
	float		 ftmp;
	Point_f2d	 f2dtmp;
	ModTriangle *tri;
	ModLayer	*layer;
	ModChild	*child;
	int			 p[4];

	/* validate */
	if ( model == NULL ) 
		return( FALSE );

	/* convert triangles in all layers */
	for ( i=0; i<model->nlayers; i++ ) {
		layer = &model->layers[i];
		/* convert quads to a triangle and add another */
		for ( j=0; j<layer->ntri; j++ ) {
			tri = &layer->tri[j];
			if ( tri->quad ) {
				/* swap zig-zag by changing points (keep front facing orientation) */
				p[0] = tri->pts[0];
				p[1] = tri->pts[1];
				p[2] = tri->pts[2];
				p[3] = tri->pts[3];
				tri->pts[0] = p[2];
				tri->pts[1] = p[0];
				tri->pts[2] = p[3];
				tri->pts[3] = p[1];
			}
		}
	}

	return( TRUE );
}

