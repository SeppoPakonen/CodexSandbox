/*****************************************************************
 * Project		: Model Library
 *****************************************************************
 * File			: modfile.c
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 9 May 1998
 * Last Update	: 17 May 1998
 *****************************************************************
 * Description	: Model library file functions
 *****************************************************************/

/*
 * includes
 */

#include <stdio.h>
#include <text/text.h>
#include <parse/parse.h>
#include <file/file.h>
#include <memory/memory.h>
#include "model.h"
#include "modmsg.h"
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

private ModModel	*modFileLoadUp( Text filename, ModModel *parent );

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
 * modFileLoad -- load up a model file
 * 
 * Description:
 *
 * Returns:
 * 
 */

ModModel	*modFileLoad ( 
	Text	filename
	)
{
	ModModel	*model;

	model = modFileLoadUp( filename, NULL );

	return( model );
}


/******************************************************************************
 * Function:
 * modFileLoadUp -- load up a model file
 * 
 * Description:
 *
 * Returns:
 * 
 */

ModModel	*modFileLoadUp ( 
	Text		 filename,
	ModModel	*parent
	)
{
	ModModel	*model;
	int			 i, j;
	char		 childname[256];
	Text		 pname;
	int			 parent_index;
	Text		 path;

	/* create a model */
	model = modCreate( filename, parent );
	modParseReset( model );
	if ( ! pseParseFile( filename, "#", 1, &modParse, NULL ) ) {
		modDestroy( model );
		return( NULL );
	}

	/* set parent info for skinned models */
	for ( i=0; i<model->njts; i++ ) {
		/* world (-1) by default or parent find */
		pname = model->jts[i].pname;
		if ( txtMatch( pname, "world" ) )
			parent_index = -1;
		else {
			parent_index = 0;
			while( parent_index < model->njts ) {
				if ( txtMatch( model->jts[parent_index].name, pname ) )
					break;
				parent_index++;
			}
			/* set to world (-1) if cannot find */
			if ( parent_index == model->njts )
				parent_index = -1;
		}
		/* validate */
		if ( parent_index == i ) {
			modSetErrorString( MOD_MSG_BAD_PARENT );
			modDestroy( model );
			return( NULL );
		}
		/* update */
		model->jts[i].parent = parent_index;
	}

	/* validate */
	if ( model->nnormals > 0 && model->npts != model->nnormals ) {
		modSetErrorString( MOD_MSG_BAD_NUM_NORMALS );
		modDestroy( model );
		return( NULL );
	}

	/* load children */
	for ( i=0; i<model->nchildren; i++ ) {
		/* create file name */
		if ( model->path != NULL )
			sprintf( childname, "%s%s.mod", model->path, model->children[i].name );
		else
			sprintf( childname, "%s.mod", model->children[i].name );
		model->children[i].model = modFileLoadUp( childname, model );
		/* check for fail and clean up */
		if ( model->children[i].model == NULL ) {
			//for ( j=0; j<(i - 1); j++ )
			//	modDestroy( model->children[j].model );
			modDestroy( model );
			return( NULL );
		}
	}

	return( model );
}


/******************************************************************************
 * Function:
 * modFileSave -- save a model file
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool	modFileSave ( 
	ModModel	*model
	)
{
	char		 fullname[256];
	char		 line[256];
	ModJoint	*jt;
	ModLayer	*layer;
	ModTriangle *tri;
	ModChild	*child;
	int			 i, t, u;
	File		 file;

	/* validate */
	if ( model == NULL ) return( FALSE );

	if ( model->name == NULL ) {
		/* no name for this model! */
		sprintf( fullname, "noname.mod" );
	} else {
		/* add .mod extension if necessary */
		i = txtLength( model->name );
		i -= 4;
		if ( (i > 0) && (model->name[i] == '.') )
			sprintf( fullname, "%s", model->name );
		else
			sprintf( fullname, "%s.mod", model->name );
	}
	
	/* open file */
	file = fileOpen( fullname, "w" );
	if ( file == -1 ) return( FALSE );

	/* print header */
	sprintf( line, "#\n# model: %s\n#\n", fullname );
	fileWrite( file, line, txtLength(line), 1 );

	/* save joint info */
	for ( i=0; i<model->njts; i++ ) {
		jt = &model->jts[i];
		sprintf( line, "joint %s %d %d %d %s %d\n",
			jt->name, (int)jt->pt.x, (int)jt->pt.y,(int)jt->pt.z,
			jt->pname, jt->npts );
		fileWrite( file, line, txtLength(line), 1 );
	}

	/* save point info */
	sprintf( line, "points\n" );
	fileWrite( file, line, txtLength(line), 1 );
	for ( i=0; i<model->npts; i++ ) {
		sprintf( line, "%d   %d %d %d\n", (i+1), (int)model->pts[i].x, (int)model->pts[i].y, (int)model->pts[i].z );
		fileWrite( file, line, txtLength(line), 1 );
	}

	/* save normal info */
	if ( model->nnormals > 0 ) {
		sprintf( line, "normals\n" );
		fileWrite( file, line, txtLength(line), 1 );
		for ( i=0; i<model->nnormals; i++ ) {
			sprintf( line, "%d   %.4f %.4f %.4f\n", (i+1), model->normals[i].x, model->normals[i].y, model->normals[i].z );
			fileWrite( file, line, txtLength(line), 1 );
		}
	}

	/* save layers */
	for ( i=0; i<model->nlayers; i++ ) {
		layer = &model->layers[i];
		/* layer name rgb sided */
		sprintf( line, "layer %s %d %d %d %s",
			layer->name, layer->r, layer->g, layer->b,
			((layer->singlesided)?("singlesided"):("doublesided"))
			);
		fileWrite( file, line, txtLength(line), 1 );
		/* mirrored? */
		sprintf( line, "\n" );
		if ( layer->mirrored )
			sprintf( line, " mirror %s\n", modAxisString(layer->mirroraxis) );
		fileWrite( file, line, txtLength(line), 1 );
		/* transparency? */
		if ( layer->transparency != 0.0f ) {
			sprintf( line, "transparency %.4f\n", layer->transparency );
			fileWrite( file, line, txtLength(line), 1 );
		}
		/* triangles */		
		if ( layer->ntri > 0 ) {
			sprintf( line, "triangles\n" );
			fileWrite( file, line, txtLength(line), 1 );
		}
		for ( t=0; t<layer->ntri; t++ ) {
			tri = &layer->tri[t];
			sprintf( line, "%d %d %d %d %d\n",
				((tri->quad)?(1):(0)),
				(tri->pts[0] + 1),
				(tri->pts[1] + 1),
				(tri->pts[2] + 1),
				((tri->quad)?(tri->pts[3] + 1):(0))
				);
			fileWrite( file, line, txtLength(line), 1 );
		}
		/* uvees */
		if ( layer->nuvee > 0 ) {
			sprintf( line, "uvees\n" );
			fileWrite( file, line, txtLength(line), 1 );
		}
		for ( u=0; u<layer->nuvee; u++ ) {
			sprintf( line, "%d  %.4f  %.4f\n", (layer->uvee[u].pt + 1),
				layer->uvee[u].u, layer->uvee[u].v );
			fileWrite( file, line, txtLength(line), 1 );
		}
		/* image info */
		if ( layer->image[0] != NULL ) {
			sprintf( line, "image %s %s %s\n", layer->image[0], 
				modAxisString(layer->xaxis), modAxisString(layer->yaxis) );
			fileWrite( file, line, txtLength(line), 1 );
			if ( layer->image[1] != NULL ) {
				sprintf( line, "mirrorimage %s\n", layer->image[1] );
				fileWrite( file, line, txtLength(line), 1 );
			}
		}
	}

	/* save children */
	for ( i=0; i<model->nchildren; i++ ) {
		child = &model->children[i];
		sprintf( line, "child %s  %d %d %d  %.4f %.4f %.4f  %d %d %d\n",
			child->name,
			(int)child->position.x, (int)child->position.y, (int)child->position.z,
			child->scale.x, child->scale.y, child->scale.z,
			(int)child->angle.x, (int)child->angle.y, (int)child->angle.z
			);
		fileWrite( file, line, txtLength(line), 1 );
	}

	/* close file */
	fileClose( file );

	return( TRUE );
}

