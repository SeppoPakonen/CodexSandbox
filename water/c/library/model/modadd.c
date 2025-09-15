/*****************************************************************
 * Project		: Model Library
 *****************************************************************
 * File			: modadd.c
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 30 May 1998
 * Last Update	: 30 May 1998
 *****************************************************************
 * Description	: Add to model functions
 *****************************************************************/

/*
 * includes
 */

#include <stdio.h>
#include <math.h>
#include <text/text.h>
#include <memory/memory.h>
#include <tga/tga.h>
#include <triangle/triangle.h>

#include "model.h"
#include "modmsg.h"

/*
 * macros
 */

/*
 * typedefs
 */

/*
 * prototypes
 */

Point_f3d modCalcNormal( ModModel *model, int p1, int p2, int p3 );
Bool modCalcTextureCoords( ModModel *model, Text layer );

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
 * modAddLayer -- add or update a layer in a model
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool modAddLayer (
	ModModel    *model,
	Text		 layer, 
	int			 r,
	int			 g,
	int			 b, 
	Bool		 singlesided,
	Bool		 mirrored,
	int			 mirroraxis
	)
{	
	ModLayer	*lay;
	Bool		 layexists;
	int			 i;

	/* validate */
	if ( model == NULL || layer == NULL ) return( FALSE );

	/* does layer exist? */
	lay = modFindLayer( model, layer );
	layexists = (Bool)lay;
	
	/* fail if layer exists */
	if ( layexists ) {
		modSetErrorString( "duplicate layer name not allowed" );
		return( FALSE );
	} 

	/* init if necessary */
	if ( ! layexists ) {
		if ( model->maxlayers == 0 ) {
			model->maxlayers = 16;
			model->layers = (ModLayer *)memAlloc( sizeof(ModLayer) * model->maxlayers );
			//memClear( model->layers, sizeof(ModLayer) * model->maxlayers );
		}

		/* allocate more memory if necessary */
		if ( model->nlayers == model->maxlayers ) {
			model->maxlayers *= 2;
			model->layers = memReAlloc( model->layers, sizeof(ModLayer) * model->maxlayers );
			//memClear( (model->layers + model->nlayers), sizeof(ModLayer) * (model->maxlayers - model->nlayers) );
		}

		lay = &model->layers[model->nlayers];
	} else {
		/* free memory */
		memFree( lay->name );
	}

	/* add new or update existing layer */
	lay->name = txtDupl( layer );
	lay->r = r;
	lay->g = g;
	lay->b = b;
	lay->uvee    = NULL;
	lay->maxuvee = 0;
	lay->nuvee   = 0;
	lay->singlesided  = singlesided;
	lay->transparency = 0.0;
	lay->mirrored     = mirrored;
	lay->mirroraxis   = mirroraxis;
	lay->limits.min.x = 0.0;
	lay->limits.min.y = 0.0;
	lay->limits.min.z = 0.0;
	lay->limits.max.x = 0.0;
	lay->limits.max.y = 0.0;
	lay->limits.max.z = 0.0;

	/* init if new */
	if ( ! layexists ) {
		lay->tri     = NULL;
		lay->ntri    = 0;
		lay->maxtri  = 0;
		for ( i=0; i<2; i++ ) {
			lay->image[i]   = NULL;
			lay->idata[i]   = NULL;
			lay->iwidth[i]  = 0;
			lay->iheight[i] = 0;
		}
		lay->xaxis = MOD_X_AXIS;
		lay->yaxis = MOD_Y_AXIS;
		model->nlayers++;
	}

	return( TRUE );
}


/******************************************************************************
 * Function:
 * modAddChild -- add or update a child in a model
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool modAddChild (
	ModModel    *model,
	Text		 childname, 
	Point_f3d	 position,
	Point_f3d	 scale,
	Point_f3d	 angle
	)
{	
	ModChild	*child;

	/* validate */
	if ( model == NULL || childname == NULL ) return( FALSE );

	/* init memory if necessary */
	if ( model->maxchildren == 0 ) {
		model->maxchildren = 16;
		model->children = (ModChild *)memAlloc( sizeof(ModChild) * model->maxchildren );
		//memClear( model->children, sizeof(ModChild) * model->maxchildren );
	}

	/* allocate more memory if necessary */
	if ( model->nchildren == model->maxchildren ) {
		model->maxchildren *= 2;
		model->children = memReAlloc( model->children, sizeof(ModChild) * model->maxchildren );
		//memClear( (model->children + model->nchildren), sizeof(ModChild) * (model->maxchildren - model->nchildren) );
	}

	/* add new child */
	child = &model->children[model->nchildren];
	child->name     = txtDupl( childname );
	child->model    = NULL;
	child->position = position;
	child->scale    = scale;
	child->angle    = angle;

	model->nchildren++;

	return( TRUE );
}


/******************************************************************************
 * Function:
 * modAddUVee -- add texture uvee's to a model layer
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool modAddUVee (
	ModModel	*model, 
	Text		 layer, 
	int			 pt,
	float		 u,
	float		 v
	)
{
	ModLayer	*lay;
	int			 i;

	/* validate */
	if ( layer == NULL ) return( FALSE );
	if ( pt < 0 || pt >= model->npts ) {
		modSetErrorString( "uvee point out of range" );
		return( FALSE );
	} 

	/* find layer in model */
	lay = modFindLayer( model, layer );
	if ( lay == NULL ) return( FALSE );

	/* init memory if necessary */
	if ( lay->maxuvee == 0 ) {
		lay->maxuvee = 16;
		lay->uvee = (ModUVee *)memAlloc( sizeof(ModUVee) * lay->maxuvee );
	}

	/* find point index */
	pt = modFindPointReference( model, (pt+1) );

	/* do we already have this uvee? */
	for ( i=0; i<lay->nuvee; i++ ) {
		/* overwrite point if it exists */
		if ( lay->uvee[i].pt == pt ) {
			lay->uvee[i].u  = u;
			lay->uvee[i].v  = v;
			return( TRUE );
		}
	}

	/* allocate more memory if necessary */
	if ( lay->nuvee == lay->maxuvee ) {
		lay->maxuvee *= 2;
		lay->uvee = memReAlloc( lay->uvee, sizeof(ModUVee) * lay->maxuvee );
	}	
	
	/* add uvee */
	i = lay->nuvee;
	lay->uvee[i].pt = pt;
	lay->uvee[i].u  = u;
	lay->uvee[i].v  = v;
	lay->nuvee++;

	return( TRUE );
}


/******************************************************************************
 * Function:
 * modAddImage -- add an image to a model layer
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool modAddImage (
	ModModel	*model, 
	Text		 layer, 
	int			 i,
	Text		 image, 
	int			 xaxis, 
	int			 yaxis 
	)
{
	ModLayer	*lay;
	char		 name[2048];

	/* validate */
	if ( layer == NULL || image == NULL ) return( FALSE );
	if ( i < 0 || i > 1 ) return( FALSE );

	/* find lay in model */
	lay = modFindLayer( model, layer );
	if ( lay == NULL ) return( FALSE );

	/* free current image if defined */
	memFree( lay->image[i] );
	memFree( lay->idata[i] );

	/* init */
	if ( model->path != NULL )
		sprintf( name, "%s%s", model->path, image );
	else
		txtCopy( image, name );

	/* update layer */
	lay->idata[i] = modImageLoad( name, &lay->iwidth[i], &lay->iheight[i] );
	lay->image[i] = txtDupl( image );

	if ( i == 0 ) {
		lay->xaxis = xaxis;
		lay->yaxis = yaxis;	
		/* calculate texture points */
		if ( ! modCalcTextureCoords( model, layer ) )
			return( FALSE );
	}

	return( TRUE );
}


/******************************************************************************
 * Function:
 * modAddImageFromPak -- add an image to a model layer from a pak file
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool modAddImageFromPak (
	Byte		*data,
	ModModel	*model, 
	Text		 layer, 
	int			 i,
	Text		 image, 
	int			 xaxis, 
	int			 yaxis 
	)
{
	ModLayer	*lay;

	/* validate */
	if ( layer == NULL || image == NULL ) return( FALSE );
	if ( i < 0 || i > 1 ) return( FALSE );

	/* find lay in model */
	lay = modFindLayer( model, layer );
	if ( lay == NULL ) return( FALSE );

	/* free current image if defined */
	memFree( lay->image[i] );
	memFree( lay->idata[i] );

	/* update layer */
	lay->idata[i] = modImageLoadFromPak( data, image, &lay->iwidth[i], &lay->iheight[i] );
	lay->image[i] = txtDupl( image );

	if ( i == 0 ) {
		lay->xaxis = xaxis;
		lay->yaxis = yaxis;	
		/* calculate texture points */
		if ( ! modCalcTextureCoords( model, layer ) )
			return( FALSE );
	}

	return( TRUE );
}


/******************************************************************************
 * Function:
 * modCalcTextureCoords -- calculate texture coordinates for a layer
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool modCalcTextureCoords (
	ModModel	*model, 
	Text		 layer 
	)
{
	ModLayer	*lay;
	Point_f3d	 min;
	Point_f3d	 max;
	float		 xsize;
	float		 ysize;
	float		 zsize;
	int			 i, j, k;
	int			 p;
	Point_f3d	*pt;
	Point_f3d	 limits[2];
	Point_i3d	 ipt;
	Point_fx2d	 fxpt;
	Point_i3d	 ilimits[2];
	Bool		 founduvee;

	/* validate */
	if ( model == NULL || layer == NULL ) 
		return( FALSE );
	
	/* find lay in model */
	lay = modFindLayer( model, layer );
	if ( lay == NULL ) return( FALSE );

	/* validate we have an image and triangles to use */
	if ( lay->idata == NULL ) return( FALSE );
	if ( lay->ntri == 0 ) return( FALSE );

#if 1
	/* calculate limits for layer */
	p = lay->tri[0].pts[0];
	pt = &model->pts[p];
	/* init limits */
	min = *pt;
	max = *pt;
	for ( i=0; i<lay->ntri; i++ ) {
		for ( j=0; j<4; j++ ) {
			/* only do quad point if necessary */
			if ( j == 3 && ! lay->tri[i].quad ) continue;
			p = lay->tri[i].pts[j];
			pt = &model->pts[p];
			/* update limits */
			if ( pt->x < min.x ) min.x = pt->x;
			if ( pt->x > max.x ) max.x = pt->x;
			if ( pt->y < min.y ) min.y = pt->y;
			if ( pt->y > max.y ) max.y = pt->y;
			if ( pt->z < min.z ) min.z = pt->z;
			if ( pt->z > max.z ) max.z = pt->z;
		}
	}

	/* init limits */
	limits[0] = min;
	limits[1] = max;
#endif

#if 1
	/* calculate lengths for each axis */
	xsize = max.x - min.x;
	ysize = max.y - min.y;
	zsize = max.z - min.z;
#endif

	/* calculate texture points */
	for ( i=0; i<lay->ntri; i++ ) {
		for ( j=0; j<4; j++ ) {
			/* only do quad point if necessary */
			if ( j == 3 && ! lay->tri[i].quad ) continue;
			p = lay->tri[i].pts[j];
			pt = &model->pts[p];
			if ( lay->uvee != NULL ) {
				/* uvees in layer */
				founduvee = FALSE;
				/* find the point for the uv */
				for ( k=0; k<lay->nuvee; k++ ) {
					if ( lay->uvee[k].pt == p ) {
						/* found point uvee */
						founduvee = TRUE;
						lay->tri[i].texpts[j].x = lay->uvee[k].u;
						lay->tri[i].texpts[j].y = lay->uvee[k].v;
					}
				}
				if ( ! founduvee )
					return( FALSE );
			} else {
				/* automatic uv creation */
#ifdef MMF_FIXEDPOINT_API
				ipt.x = (int)pt->x;
				ipt.y = (int)pt->y;
				ipt.z = (int)pt->z;
				ilimits[0].x = (int)lay->limits.min.x;
				ilimits[0].y = (int)lay->limits.min.y;
				ilimits[0].z = (int)lay->limits.min.z;
				ilimits[1].x = (int)lay->limits.max.x;
				ilimits[1].y = (int)lay->limits.max.y;
				ilimits[1].z = (int)lay->limits.max.z;
				fxpt = triCalcTextureCoord( ipt, (Point_i3d *)&lay->limits, (char)lay->xaxis, (char)lay->yaxis );
				lay->tri[i].texpts[j].x = (float)fxpt.x;
				lay->tri[i].texpts[j].y = (float)fxpt.y;
#else
				lay->tri[i].texpts[j] = triCalcTextureCoord( *pt, (Point_f3d *)&lay->limits, (char)lay->xaxis, (char)lay->yaxis );
#endif
			}
		}
	}

	/* ok */
	return( TRUE );
}


/******************************************************************************
 * Function:
 * modAddJoint -- add a joint to a model
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool modAddJoint (
	ModModel   *model,
	Text		name,
	int			x,
	int			y,
	int			z,
	Text		parent,
	int			npts
	)
{
	int			i;
	int			parent_index;

	/* validate */
	if ( model == NULL || name == NULL ) return( FALSE );

	/* init if necessary */
	if ( model->maxjts == 0 ) {
		model->maxjts = 16;
		model->jts = (ModJoint *)memAlloc( sizeof(ModJoint) * model->maxjts );
	}

	/* allocate more memory if necessary */
	if ( model->njts == model->maxjts ) {
		model->maxjts *= 2;
		model->jts = (ModJoint *)memReAlloc( model->jts, sizeof(ModJoint) * model->maxjts );
	}

	/* add point */
	i = model->njts;
	model->jts[i].name  = txtDupl( name );
	model->jts[i].pname = txtDupl( parent );
	model->jts[i].pt.x  = (float)x;
	model->jts[i].pt.y  = (float)y;
	model->jts[i].pt.z  = (float)z;
	model->jts[i].npts  = npts;
	
	/* world (-1) by default or parent find */
	if ( txtMatch( parent, "world" ) )
		parent_index = -1;
	else {
		parent_index = 0;
		while( parent_index < model->njts ) {
			if ( txtMatch( model->jts[parent_index].name, parent ) )
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
		return( FALSE );
	}

	/* update */
	model->jts[i].parent = parent_index;
	model->njts++;

	return( TRUE );
}


/******************************************************************************
 * Function:
 * modAddPoint -- add a point to a model
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool modAddPoint (
	ModModel   *model,
	int			n,
	int			x, 
	int			y,
	int			z 
	)
{
	int			i;
	float		fx, fy, fz;

	/* validate */
	if ( model == NULL ) return( FALSE );

	/* init if necessary */
	if ( model->maxpts == 0 ) {
		model->maxpts = 16;
		model->pts  = (Point_f3d *)memAlloc( sizeof(Point_f3d) * model->maxpts );
		model->ptsi = (int *)memAlloc( sizeof(int) * model->maxpts );
	}

	/* allocate more memory if necessary */
	if ( model->npts == model->maxpts ) {
		model->maxpts *= 2;
		model->pts  = (Point_f3d *)memReAlloc( model->pts, sizeof(Point_f3d) * model->maxpts );
		model->ptsi = (int *)memReAlloc( model->ptsi, sizeof(int) * model->maxpts );
	}

	/* add point */
	i = model->npts;
	model->pts[i].x = (float)x;
	model->pts[i].y = (float)y;
	model->pts[i].z = (float)z;
	model->ptsi[i]  = n;
	model->npts++;

	/* update limits */
	fx = (float)x;
	fy = (float)y;
	fz = (float)z;
	if ( fx < model->limits.min.x ) model->limits.min.x = fx;
	if ( fx > model->limits.max.x ) model->limits.max.x = fx;
	if ( fy < model->limits.min.y ) model->limits.min.y = fy;
	if ( fy > model->limits.max.y ) model->limits.max.y = fy;
	if ( fz < model->limits.min.z ) model->limits.min.z = fz;
	if ( fz > model->limits.max.z ) model->limits.max.z = fz;

	return( TRUE );
}


/******************************************************************************
 * Function:
 * modAddNormal -- add a point normal to a model
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool modAddNormal (
	ModModel   *model,
	int			n,
	float		nx, 
	float		ny,
	float		nz 
	)
{
	int			i;

	/* validate */
	if ( model == NULL ) return( FALSE );

	/* init if necessary */
	if ( model->maxnormals == 0 ) {
		model->maxnormals = 16;
		model->normals = (Point_f3d *)memAlloc( sizeof(Point_f3d) * model->maxnormals );
	}

	/* find point number */
	i = modFindPointReference( model, n );

	/* allocate more memory if necessary */
	if ( (model->nnormals >= model->maxnormals) || ((i+1) >= model->maxnormals) ) {
		while ( (model->nnormals >= model->maxnormals) || ((i+1) >= model->maxnormals) )
			model->maxnormals *= 2;
		model->normals = (Point_f3d *)memReAlloc( model->normals, sizeof(Point_f3d) * model->maxnormals );
	}

	/* add point normal */
	//i = model->nnormals;
	model->normals[i].x = (float)nx;
	model->normals[i].y = (float)ny;
	model->normals[i].z = (float)nz;
	model->nnormals++;

	return( TRUE );
}


/******************************************************************************
 * Function:
 * modAddGoraud -- add a goraud rgb value to a model
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool modAddGoraud (
	ModModel   *model,
	int			n,
	Byte		r, 
	Byte		g, 
	Byte		b
	)
{
	int			i;

	/* validate */
	if ( model == NULL ) 
		return( FALSE );

	/* init if necessary */
	if ( model->maxrgb == 0 ) {
		model->maxrgb = 16;
		model->rgb = (ModRGB *)memAlloc( sizeof(ModRGB) * model->maxrgb );
	}

	/* find point number */
	i = modFindPointReference( model, n );

	/* allocate more memory if necessary */
	if ( (model->nrgb >= model->maxrgb) || ((i+1) >= model->maxrgb) ) {
		while( (model->nrgb >= model->maxrgb) || ((i+1) >= model->maxrgb) )
			model->maxrgb *= 2;
		model->rgb = (ModRGB *)memReAlloc( model->rgb, sizeof(ModRGB) * model->maxrgb );
	}

	/* add goraud rgb value */
	//i = model->nrgb;
	model->rgb[i].r = r;
	model->rgb[i].g = g;
	model->rgb[i].b = b;
	model->rgb[i].pad = 0;	// (unused)
	model->nrgb++;

	return( TRUE );
}


/******************************************************************************
 * Function:
 * modAddTriangle -- add a triangle to a model layer
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool modAddTriangle (
	ModModel	*model,
	Text		 layer, 
	Bool		 quad,
	int			 p1, 
	int			 p2,
	int			 p3,
	int			 p4
	)
{
	ModLayer	*lay;
	int			 i, j;
	int			 p;
	Point_f3d	*pt;
	Point_f3d	 min;
	Point_f3d	 max;

	/* validate */
	if ( model == NULL || layer == NULL ) return( FALSE );

	/* find lay in model */
	lay = modFindLayer( model, layer );
	if ( lay == NULL ) return( FALSE );

	/* init if necessary */
	if ( lay->maxtri == 0 ) {
		lay->maxtri = 16;
		lay->tri = (ModTriangle *)memAlloc( sizeof(ModTriangle) * lay->maxtri );
	}

	/* allocate more memory if necessary */
	if ( lay->ntri == lay->maxtri ) {
		lay->maxtri *= 2;
		lay->tri = (ModTriangle *)memReAlloc( lay->tri, sizeof(ModTriangle) * lay->maxtri );
	}

	/* add triangle */
	i = lay->ntri;
	lay->tri[i].quad   = quad;
#if 1
	lay->tri[i].pts[0] = modFindPointReference( model, p1 );
	lay->tri[i].pts[1] = modFindPointReference( model, p2 );
	lay->tri[i].pts[2] = modFindPointReference( model, p3 );
	lay->tri[i].pts[3] = modFindPointReference( model, p4 );
#else
	lay->tri[i].pts[0] = p1 - 1;
	lay->tri[i].pts[1] = p2 - 1;
	lay->tri[i].pts[2] = p3 - 1;
	lay->tri[i].pts[3] = p4 - 1;
#endif
	lay->tri[i].normal[0] = modCalcNormal( model, lay->tri[i].pts[0], lay->tri[i].pts[1], lay->tri[i].pts[2] );
	if ( quad )
		lay->tri[i].normal[1] = modCalcNormal( model, lay->tri[i].pts[2], lay->tri[i].pts[1], lay->tri[i].pts[3] );
	else {
		lay->tri[i].normal[1].x = 0.0;
		lay->tri[i].normal[1].y = 0.0;
		lay->tri[i].normal[1].z = 1.0;
	}

	lay->ntri++;

	/* update limits for layer */
	if ( lay->ntri == 1 ) {
		p = lay->tri[0].pts[0];
		pt = &model->pts[p];
		/* init limits */
		min = *pt;
		max = *pt;
	} else {
		min = lay->limits.min;
		max = lay->limits.max;
	}

	for ( j=0; j<4; j++ ) {
		/* only do quad point if necessary */
		if ( j == 3 && ! lay->tri[i].quad ) continue;
		p = lay->tri[i].pts[j];
		pt = &model->pts[p];
		/* update limits */
		if ( pt->x < min.x ) min.x = pt->x;
		if ( pt->x > max.x ) max.x = pt->x;
		if ( pt->y < min.y ) min.y = pt->y;
		if ( pt->y > max.y ) max.y = pt->y;
		if ( pt->z < min.z ) min.z = pt->z;
		if ( pt->z > max.z ) max.z = pt->z;
	}

	/* update layer limits */
	lay->limits.min = min;
	lay->limits.max = max;

	return( TRUE );
}


/******************************************************************************
 * Function:
 * modAddTriangleStrip -- add a triangle strip to a model layer
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool modAddTriangleStrip (
	ModModel	*model,
	Text		 layer, 
	Bool		 npts,
	int			*p
	)
{
	ModLayer	*lay;
	int			 i, j;
	int			 pt[4];
	int			 swap, n;

	/* validate */
	if ( model == NULL || layer == NULL ) return( FALSE );

	/* find lay in model */
	lay = modFindLayer( model, layer );
	if ( lay == NULL ) return( FALSE );

	/* add triangles for now... */
	n = 0;
	swap = 0;
	while( (n+3) <= npts ) {
		if ( swap ) {
			pt[0] = p[n+1];
			pt[1] = p[n];
			pt[2] = p[n+2];
		} else {
			pt[0] = p[n];
			pt[1] = p[n+1];
			pt[2] = p[n+2];
		}
		if ( ! modAddTriangle( model, layer, 0, pt[0], pt[1], pt[2], 0 ) )
			return( FALSE );
		swap = !swap;
		n++;
	}

	return( TRUE );
}


/******************************************************************************
 * Function:
 * modAddTransparency -- set transparency for a model layer
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool modAddTransparency (
	ModModel	*model,
	Text		 layer, 
	float		 trans
	)
{
	ModLayer	*lay;

	/* validate */
	if ( model == NULL || layer == NULL ) return( FALSE );
	if ( trans < 0.0 || trans > 1.0 ) return( FALSE );

	/* find lay in model */
	lay = modFindLayer( model, layer );
	if ( lay == NULL ) return( FALSE );

	/* set transparency */
	lay->transparency = trans;

	return( TRUE );
}


/******************************************************************************
 * Function:
 * modCalcNormal -- calculate the normal for a triangle
 * 
 * Description:
 *
 * Returns:
 * 
 */

Point_f3d	modCalcNormal (
	ModModel	*model,
	int			 p1,
	int			 p2,
	int			 p3
    )
{
	Point_f3d	*pt0;
	Point_f3d	*pt1;
	Point_f3d	*pt2;
	Point_f3d	 normal;
	Point_f3d	 pts[3];
	Point_fx3d	 fxnormal;
	Point_i3d	 ipts[3];
	int			 i;

	/* no validation... */
	pt0 = &model->pts[p1];
	pt1 = &model->pts[p2];
	pt2 = &model->pts[p3];

	pts[0] = *pt0;
	pts[1] = *pt1;
	pts[2] = *pt2;
#ifdef MMF_FIXEDPOINT_API
	for ( i=0; i<3; i++ ) {
		ipts[i].x = (int)pts[i].x;
		ipts[i].y = (int)pts[i].y;
		ipts[i].z = (int)pts[i].z;
	}
	fxnormal = triCalcNormal( ipts );
	normal.x = FIXED_TO_FLOAT( fxnormal.x );
	normal.y = FIXED_TO_FLOAT( fxnormal.y );
	normal.z = FIXED_TO_FLOAT( fxnormal.z );
#else
	normal = triCalcNormal( pts );
#endif
	return( normal );
}


#if 0
/******************************************************************************
 * Function:
 * modClockWise -- identify if the 3 passed points are clockwise
 * 
 * Description:
 *
 * Returns:
 * 
 */

Bool	modClockWise (
	float	x0,
	float	y0,
	float	x1,
	float	y1,
	float	x2,
	float	y2
	)
{
	float	x[2], y[2];
	float	angle[2];
	char	buf[256];

	x[0] = x1 - x0;
	y[0] = y1 - y0;
	x[1] = x2 - x1;
	y[1] = y2 - y1;
	
	angle[0] = (float)atan2( (double)y[0], (double)x[0] );
	angle[1] = (float)atan2( (double)y[1], (double)x[1] );

	if ( ( angle[1] < angle[0] ) && ( angle[1] > ( angle[0] - 3.141592654 ) ) )
		return( TRUE );

	if ( ( angle[0] < 0 ) && ( angle[1] > ( angle[0] + 3.141592654 ) ) )
		return( TRUE );

	return( FALSE );
}
#endif