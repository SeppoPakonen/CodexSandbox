/*****************************************************************
 * Project		: Model Library
 *****************************************************************
 * File			: modlutil.c
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 27 May 1998
 * Last Update	: 27 May 1998
 *****************************************************************
 * Description	: Model library utility functions
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

#define MOD_SAVED_STRING_LENGTH		512

/*
 * prototypes
 */

/*
 * variables
 */


/* 
 * globals variables...
 */

Text	mod_error_string = NULL;
char	mod_saved_string[MOD_SAVED_STRING_LENGTH];

/*
 * functions
 */


/******************************************************************************
 * Function:
 * modSetName -- set the name for a model
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool	modSetName (
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
 * modNumChildren -- get the number of children for a model
 * 
 * Description:
 *
 * Returns:
 * 
 */

int		modNumChildren (
	ModModel *model
    )
{
	int		nkids;
	int		i;

	/* validate */
	if ( model == NULL ) return( 0 );

	/* init */
	nkids = 0;

	for ( i=0; i<model->nchildren; i++ )
		nkids += modNumChildren( model->children[i].model );
	nkids += model->nchildren;

	return( nkids );
}


/******************************************************************************
 * Function:
 * modGetChild -- get the child for a model by offset number
 * 
 * Description:
 *
 * Returns:
 * 
 */

ModChild *modGetChild (
	ModModel *model,
	int		  n
    )
{
	int			 nkids;
	int			 i;

	/* validate */
	if ( model == NULL || (n < 0) ) return( NULL );
	if ( model->nchildren == 0 ) return( NULL );

	/* init */
	nkids = 0;

	/* get child */
	for ( i=0; i<model->nchildren; i++ ) {
		/* got it? */
		if ( n == 0 )
			return( &model->children[i] );
		/* get number of children in this child model */
		nkids = modNumChildren( model->children[i].model );
		/* if desired child is in this model do a recurive call */
		if ( nkids >= n )
			return( modGetChild( model->children[i].model, (n - 1) ) );
		n -= nkids;
	}

	return( NULL );
}


/******************************************************************************
 * Function:
 * modErrorString -- get the error string
 * 
 * Description:
 *
 * Returns:
 * 
 */

Text	modErrorString (
	void
    )
{
	return( mod_error_string );
}


/******************************************************************************
 * Function:
 * modSetErrorString -- set the error string
 * 
 * Description:
 *
 * Returns:
 * 
 */

void	modSetErrorString (
	Text	str
    )
{
	char	errstr[256];
	Text	filename;
	int		linenum;

	/* init - get parse file details */
	filename = pseGetFilename();
	linenum  = pseGetLineNum();
	
	/* clear error string */
	memFree( mod_error_string );
	/* set error string */
	if ( str == NULL )
		mod_error_string = NULL;
	else {
		if ( filename != NULL )
			sprintf( errstr, "%s (file: %s, line: %d)", str, filename, linenum );
		else if ( linenum )
			sprintf( errstr, "%s (line: %d)", str, linenum );
		else
			sprintf( errstr, "%s", str );
		mod_error_string = txtDupl( errstr );
	}
}


/******************************************************************************
 * Function:
 * modCuboidSize -- return the cuboid size a model can fit into around 0,0
 * 
 * Description:
 *
 * Returns:
 * 
 */
 
float	modCuboidSize (
    ModModel *model,
	Matrix	 *mat
	)
{
	ModLimits	*limits;
	float		 max;
	float		 cube;
	Matrix		 matrix;
	Matrix		 saved;
	ModChild	*child;
	Point_f3d	 limit_min;
	Point_f3d	 limit_max;
	int			 c;
	static		 rlevel = 0;

	/* init */
	if ( mat == NULL ) {
		mat = &matrix;
		matrixIdentity( mat );
	}

	/* validate */
	if ( model == NULL ) return( (float)0.0 );

	/* update recursion level */
	rlevel++;

	/* transform points */
	limits = &model->limits;
	matrixApply( mat, &limits->min, &limit_min );
	matrixApply( mat, &limits->max, &limit_max );

	/* get maximum limit in any axis */
	max = absF(limit_min.x);
	if ( absF(limit_min.y) > max ) max = absF(limit_min.y);
	if ( absF(limit_min.z) > max ) max = absF(limit_min.z);
	if ( absF(limit_max.x) > max ) max = absF(limit_max.x);
	if ( absF(limit_max.y) > max ) max = absF(limit_max.y);
	if ( absF(limit_max.z) > max ) max = absF(limit_max.z);

	/* save a copy of the matrix */
	saved = *mat;

	/* get limits of children */
	for ( c=0; c<model->nchildren; c++ ) {
		/* init */
		child = &model->children[c];
		/* update matrix */
		matrixTranslate( mat, child->position.x, child->position.y, child->position.z );
		matrixScale( mat, child->scale.x, child->scale.y, child->scale.z );
		matrixRotate( mat, child->angle.x, child->angle.y, child->angle.z );
		/* get limit cube value */
		cube = modCuboidSize( child->model, mat );
		if ( cube > max ) max = cube;
		/* restore matrix */
		*mat = saved;
	}

	/* this is the maximum value from 0 so double it to make cube size */
	if ( rlevel == 1 ) 
		max *= 2;

	/* decrement recursion level */
	rlevel--;

	return( max );
}


/******************************************************************************
 * Function:
 * modAxisString -- return the string for an axis number
 * 
 * Description:
 *
 * Returns:
 * 
 */
 
Text	modAxisString (
	int		axis
	)
{
	Text	str;

	switch( axis ) {
		case MOD_X_AXIS:
			str = "x";
			break;
		case MOD_Y_AXIS:
			str = "y";
			break;
		case MOD_Z_AXIS:
			str = "z";
			break;
		case MOD_NEG_X_AXIS:
			str = "-x";
			break;
		case MOD_NEG_Y_AXIS:
			str = "-y";
			break;
		case MOD_NEG_Z_AXIS:
			str = "-z";
			break;
		default:
			str = "x";
			break;
	}

	return( str );
}


/******************************************************************************
 * Function:
 * modExtractName -- extract name of file from full path
 * 
 * Description:
 *
 * Returns:
 * 
 */

Text	modExtractFilename (
	Text	filename
	)
{
	int			flen;
	Text		name;
	int			i, lastdir;

	/* validate */
	if ( filename == NULL || filename[0] == '\0' ) 
		return( NULL );
	if ( txtLength( filename ) >= MOD_SAVED_STRING_LENGTH )
		return( NULL );

	/* init */
	txtCopy( filename, mod_saved_string );
	flen = txtLength( mod_saved_string );

	/* remove file extension */
	flen -= 4;
	if ( flen <= 0 ) return( NULL );
	if ( mod_saved_string[flen] == '.' )
		mod_saved_string[flen] = '\0';

	/* remove leading path */
	i = 0;
	lastdir = 0;
	for ( i=0; i<flen; i++ ) {
		if ( mod_saved_string[i] == '\\' ) 
			lastdir = i+1;
	}
	name = &mod_saved_string[lastdir];
	txtToLower( name );

	return( name );
}


/******************************************************************************
 * Function:
 * modExtractPath -- extract path for file from full name
 * 
 * Description:
 *
 * Returns:
 * 
 */

Text	modExtractPathname (
	Text	filename
	)
{
	int			flen;
	Text		name;
	int			i, lastdir;

	/* validate */
	if ( filename == NULL || filename[0] == '\0' ) 
		return( NULL );
	if ( txtLength( filename ) >= MOD_SAVED_STRING_LENGTH )
		return( NULL );

	/* init */
	txtCopy( filename, mod_saved_string );
	flen = txtLength( mod_saved_string );

	/* keep path with last slash */
	i = 0;
	lastdir = 0;
	for ( i=0; i<flen; i++ ) {
		if ( mod_saved_string[i] == '\\' ) 
			lastdir = i+1;
	}
	mod_saved_string[lastdir] = '\0';
	name = mod_saved_string;
	txtToLower( name );

	return( name );
}
