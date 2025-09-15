/*****************************************************************
 * Project		: Myformat Library
 *****************************************************************
 * File			: mymodel.c
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 27 Jun 1998
 * Last Update	: 27 Jun 1998
 *****************************************************************
 * Description	: my model format (MMF) library functions
 *****************************************************************/

/*
 * includes
 */

#include <stdio.h>
#include <gfx/gfx.h>
#include <triangle/triangle.h>
#include <memory/memory.h>
#include <text/text.h>
#include <binary/binary.h>
#include "myformat.h"

/*
 * macros
 */

#define BINARY_ONLY		TRUE

/*
 * typedefs
 */

/*
 * prototypes
 */

private void mmfAddTri( GfxObject *object, Point_f3d *limits, Point_f3d *pts, Point_f2d *uvee, Bool quad, char xaxis, char yaxis );

/*
 * variables
 */


/* 
 * globals variables...
 */

Text	mmf_error_string = NULL;

/*
 * functions
 */


/******************************************************************************
 * Function:
 * mmfModelToGfxObject -- create a GFX object from an MMF model
 * 
 * Description:
 *
 * Returns:
 * 
 */

public int	mmfModelToGfxObject ( 
	Text		  filename,
	GfxObject	**object,
	GfxTextureId *textures
	)
{
	int			 t, i, j, k, n;//, x;
	MmfModel	*model;
	//MmfLayer	*layer;
	Point_f3d	 pts[4];
	Point_f2d	 uvee[4];
	Point_f2d	 uvee_ptr;
	Point_f3d	 saved;
	Point_f3d	 limits_a[2];
	Point_f3d	 limits_b[2];
	GfxTextureId tex_a, tex_b;
	Bool		 quad;
	int			 p0, p1, p2, p3;
	int			 nobjects;
	int			 ntextures;
	Bool		 inlist;
	//MmfChild	*child;
	GfxObject	*obj;
	int			 nlayers;
	int			 npoints;
	int			 pad;
	Text		 fileid;
	Byte		*layerdata;
	int			 ntri;
	Bool		 mirrored;
	char		 mirroraxis;
	char		 xaxis;
	char		 yaxis;
	Byte		 r,g,b;
	Text		 image;
	Bool		 doublesided;
	float		 transparency;
	char		 modelname[32];
	char		 imagestr[32];
	int			 len;
	static Bool	 flip = FALSE;
	//char		 childname[32];
	float		 scalex;
	float		 scaley;
	float		 scalez;
	float		 positionx;
	float		 positiony;
	float		 positionz;
	float		 anglex;
	float		 angley;
	float		 anglez;
	Byte		*childdata;
	Text		 childname;
	int			 nchildren;
	Bool		 binary;
	float		 version;
	int			 nuvee;
	Byte		*data;
	int			 uv;
	int			 uvpt;
	Point_f3d	 pt;
	int			 nnormals;
	Point_f3d	 norm;	
	Point_f3d	 normals[4];
	Point_f3d	*norm_ptr;
#define B2F(val)	((float)((float)val/255.0f))

	/* validate */
	if ( filename == NULL || object == NULL ) return( 0 );

#if BINARY_ONLY
	binary = TRUE;
#endif

	/* init */
	if ( binary ) {
		/* copy name */
		for ( i=0; i<27 && filename[i] != '\0'; i++ )
			modelname[i] = filename[i];
		/* add extension */
		modelname[i] = '.'; i++;
		modelname[i] = 'm'; i++;
		modelname[i] = 'm'; i++;
		modelname[i] = 'b'; i++;
		modelname[i] = '\0';
		/* open file */
		data = gfxFileLoad( modelname );
		if ( data == NULL ) return( 0 );
		/* validate binary file */
		fileid = binReadString( 4, data );
		if ( ! txtMatch( fileid, MMB_HEADER ) ) {
			mmf_error_string = "not an mmb file";
			return( 0 );
		}
		version = binReadFloat( (data + 4) );
		if ( version != MMB_LATEST_VERSION ) {
			mmf_error_string = "not latest version";
			return( 0 );
		}
		npoints = binReadInt( (data + 8) );
		//printf( "points = %d\n", npoints );
		if ( npoints % 2 )
			pad = 2;
		else
			pad = 0;
		/* get number of normals (must equal number of points or 0) */
		nnormals = binReadInt( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad) );
		if ( nnormals ) {
			if ( nnormals != npoints ) {
				mmf_error_string = "number of normals do not equal number of points";
				return( 0 );
			}
			norm_ptr = &normals;
		} else
			norm_ptr = NULL;
		nlayers  = binReadInt( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad) + 4 + (nnormals * MMB_SIZEOF_NORMAL) );
		//printf( "layers = %d\n", nlayers );
		layerdata = (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (nnormals * MMB_SIZEOF_NORMAL) + 4);
#if ! BINARY_ONLY
	} else {
		/* init model pointer for MMF type data */
		model = (MmfModel *)data;
		nlayers = model->nlayers;
#endif
	}

	/* create object */
	obj = gfxBeginObject( filename, NULL, GFX_OBJ_STATIC );
	if ( obj == NULL ) {
		mmf_error_string = "gfxBeginObject() failed";
		return( 0 );
	}
	object[0] = obj;
	nobjects  = 1;
	ntextures = 0;

	/* add point data */
	for ( i=0; i<npoints; i++ ) {
		pt.x = (float)binReadShort( (data + 12 + (i * MMB_SIZEOF_POINT)) );
		pt.y = (float)binReadShort( (data + 12 + (i * MMB_SIZEOF_POINT) + 2) );
		pt.z = (float)binReadShort( (data + 12 + (i * MMB_SIZEOF_POINT) + 4) );
		gfxAddPoints( obj, &pt, 1 );
	}

	/* add normal data */
	for ( i=0; i<nnormals; i++ ) {
		norm.x = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad) + 4 + (i * MMB_SIZEOF_NORMAL)) );
		norm.y = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad) + 4 + (i * MMB_SIZEOF_NORMAL) + 4) );
		norm.z = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad) + 4 + (i * MMB_SIZEOF_NORMAL) + 8) );
		gfxAddNormals( obj, &norm, 1 );
	}

	/* create graphics */
	for ( i=0; i<nlayers; i++ ) {

		/* init */
		if ( binary ) {
			ntri       = binReadInt( (layerdata + 8) );
			mirrored   = (Bool)binReadByte( (layerdata + (ntri * MMB_SIZEOF_TRI) + 70) );
			mirroraxis = (char)binReadByte( (layerdata + (ntri * MMB_SIZEOF_TRI) + 71) );
			limits_a[0].x = binReadFloat( (layerdata + 12 + (ntri * MMB_SIZEOF_TRI)) );
			limits_a[0].y = binReadFloat( (layerdata + 12 + (ntri * MMB_SIZEOF_TRI) +  4) );
			limits_a[0].z = binReadFloat( (layerdata + 12 + (ntri * MMB_SIZEOF_TRI) +  8) );
			limits_a[1].x = binReadFloat( (layerdata + 12 + (ntri * MMB_SIZEOF_TRI) + 12) );
			limits_a[1].y = binReadFloat( (layerdata + 12 + (ntri * MMB_SIZEOF_TRI) + 16) );
			limits_a[1].z = binReadFloat( (layerdata + 12 + (ntri * MMB_SIZEOF_TRI) + 20) );
			doublesided = (Bool)binReadByte( (layerdata + 3) );
			transparency = binReadFloat( (layerdata + 4) );
			r = binReadByte( (layerdata) );
			g = binReadByte( (layerdata + 1) );
			b = binReadByte( (layerdata + 2) );
			xaxis = (char)binReadByte( (layerdata + (ntri * MMB_SIZEOF_TRI) + 68) );
			yaxis = (char)binReadByte( (layerdata + (ntri * MMB_SIZEOF_TRI) + 69) );
			nuvee = binReadInt( (layerdata + (ntri * MMB_SIZEOF_TRI) + 72) );
#if ! BINARY_ONLY
		} else {
			layer      = model->layers[i];
			ntri       = layer->ntri;
			mirrored   = layer->mirrored;
			mirroraxis = layer->mirroraxis;
			limits_a[0].x = (float)layer->min[0];
			limits_a[0].y = (float)layer->min[1];
			limits_a[0].z = (float)layer->min[2];
			limits_a[1].x = (float)layer->max[0];
			limits_a[1].y = (float)layer->max[1];
			limits_a[1].z = (float)layer->max[2];
			doublesided = layer->doublesided;
			r = layer->r;
			g = layer->g;
			b = layer->b;
			xaxis = layer->xaxis;
			yaxis = layer->yaxis;
#endif
		}

		/* init limits for mirror */
		if ( mirrored ) {
			/* init limits for mirror */
			limits_b[0] = limits_a[0];
			limits_b[1] = limits_a[1];

			/* flip around the origin */
			switch( mirroraxis ) {
			case MMF_X_AXIS:
				limits_b[0].x *= -1;
				limits_b[1].x *= -1;
				break;
			case MMF_Y_AXIS:
				limits_b[0].y *= -1;
				limits_b[1].y *= -1;
				break;
			case MMF_Z_AXIS:
				limits_b[0].z *= -1;
				limits_b[1].z *= -1;
				break;
			}
		}

		/* set color and doublesided */
		gfxSetColor( B2F(r), B2F(g), B2F(b) );
		gfxSetDoubleSided( doublesided );
		gfxSetTransparency( transparency );

		/* load texture(s) */
		if ( binary ) {
			/* get image name */
			image = binReadString( 16, (layerdata + 36 + (ntri * MMB_SIZEOF_TRI)) );
			//printf( "image a string: %s\n", image );
			len = txtLength( image );
			tex_a = GFX_NO_TEXTURE;
			if ( len > 0 ) {
				txtCopy( image, imagestr );
				/* add extension */
				txtCopy( ".mib", &imagestr[len] );
				//printf( "image a: %s\n", imagestr );
				tex_a = gfxTextureId( imagestr );
				if ( tex_a == GFX_NO_TEXTURE ) {
					tex_a = mifTextureToGfxTexture( imagestr, FALSE );
					/* reload model file into file buffer */
					//printf( "load model: %s\n", modelname );
					data = gfxFileLoad( modelname );
					if ( data == NULL ) {
						mmf_error_string = "model load failed";
						return( 0 );
					}
				}
			}
			image = binReadString( 16, (layerdata + 52 + (ntri * MMB_SIZEOF_TRI)) );
			//printf( "image b string: %s\n", image );
			len = txtLength( image );
			tex_b = GFX_NO_TEXTURE;
			if ( len > 0 ) {
				txtCopy( image, imagestr );
				/* add extension */
				txtCopy( ".mib", &imagestr[len] );
				//printf( "image b: %s\n", imagestr );
				tex_b = gfxTextureId( imagestr );
				if ( tex_b == GFX_NO_TEXTURE ) {
					tex_b = mifTextureToGfxTexture( imagestr, FALSE );
					/* reload model file into file buffer */
					//printf( "load model: %s\n", modelname );
					data = gfxFileLoad( modelname );
					if ( data == NULL ) {
						mmf_error_string = "model load failed";
						return( 0 );
					}
				}
			}
#if ! BINARY_ONLY
		} else {
			tex_a = mifTextureToGfxTexture( (Byte *)layer->image_a, binary );
			tex_b = mifTextureToGfxTexture( (Byte *)layer->image_b, binary );
#endif
		}

		/* return textures may be NULL */
		if ( textures != NULL ) {
			/* store texture a in return array */
			if ( tex_a != GFX_NO_TEXTURE ) {
				inlist = FALSE;
				for ( t=0; t<ntextures; t++ ) {
					if ( textures[t] == tex_a ) 
						inlist = TRUE;	
				}
				if ( ! inlist ) {
					textures[ntextures] = tex_a;
					ntextures++;
				}
			}
			/* store texture b in return array */
			if ( tex_b != GFX_NO_TEXTURE ) {
				inlist = FALSE;
				for ( t=0; t<ntextures; t++ ) {
					if ( textures[t] == tex_b ) 
						inlist = TRUE;	
				}
				if ( ! inlist ) {
					textures[ntextures] = tex_b;
					ntextures++;
				}
			}			
		}

		/* add triangles */
		for ( j=0; j<ntri; j++ ) {

			/* init */
			if ( binary ) {
				p0 = binReadUShort( (layerdata + 12 + (j * MMB_SIZEOF_TRI)) );
				p1 = binReadUShort( (layerdata + 12 + (j * MMB_SIZEOF_TRI) + 2) );
				p2 = binReadUShort( (layerdata + 12 + (j * MMB_SIZEOF_TRI) + 4) );
				p3 = binReadUShort( (layerdata + 12 + (j * MMB_SIZEOF_TRI) + 6) );
				quad = (p3 != 0);
				pts[0].x = (float)binReadShort( (data + 12 + (p0 * MMB_SIZEOF_POINT)) );
				pts[0].y = (float)binReadShort( (data + 12 + (p0 * MMB_SIZEOF_POINT) + 2) );
				pts[0].z = (float)binReadShort( (data + 12 + (p0 * MMB_SIZEOF_POINT) + 4) );
				pts[1].x = (float)binReadShort( (data + 12 + (p1 * MMB_SIZEOF_POINT)) );
				pts[1].y = (float)binReadShort( (data + 12 + (p1 * MMB_SIZEOF_POINT) + 2) );
				pts[1].z = (float)binReadShort( (data + 12 + (p1 * MMB_SIZEOF_POINT) + 4) );
				pts[2].x = (float)binReadShort( (data + 12 + (p2 * MMB_SIZEOF_POINT)) );
				pts[2].y = (float)binReadShort( (data + 12 + (p2 * MMB_SIZEOF_POINT) + 2) );
				pts[2].z = (float)binReadShort( (data + 12 + (p2 * MMB_SIZEOF_POINT) + 4) );
				if ( quad ) {
					pts[3].x = (float)binReadShort( (data + 12 + (p3 * MMB_SIZEOF_POINT)) );
					pts[3].y = (float)binReadShort( (data + 12 + (p3 * MMB_SIZEOF_POINT) + 2) );
					pts[3].z = (float)binReadShort( (data + 12 + (p3 * MMB_SIZEOF_POINT) + 4) );
				}
				/* get uvee points */
				if ( nuvee ) {
					uv = 0;
					while( uv < nuvee ) {
						uvpt = binReadInt( (layerdata + (ntri * MMB_SIZEOF_TRI) + 76 + (uv * MMB_SIZEOF_UVEE)) );
						if ( uvpt == p0 ) {
							uvee[0].x = binReadFloat( (layerdata + (ntri * MMB_SIZEOF_TRI) + 76 + (uv * MMB_SIZEOF_UVEE) + 4) );
							uvee[0].y = binReadFloat( (layerdata + (ntri * MMB_SIZEOF_TRI) + 76 + (uv * MMB_SIZEOF_UVEE) + 8) );
						} else if ( uvpt == p1 ) {
							uvee[1].x = binReadFloat( (layerdata + (ntri * MMB_SIZEOF_TRI) + 76 + (uv * MMB_SIZEOF_UVEE) + 4) );
							uvee[1].y = binReadFloat( (layerdata + (ntri * MMB_SIZEOF_TRI) + 76 + (uv * MMB_SIZEOF_UVEE) + 8) );
						} else if ( uvpt == p2 ) {
							uvee[2].x = binReadFloat( (layerdata + (ntri * MMB_SIZEOF_TRI) + 76 + (uv * MMB_SIZEOF_UVEE) + 4) );
							uvee[2].y = binReadFloat( (layerdata + (ntri * MMB_SIZEOF_TRI) + 76 + (uv * MMB_SIZEOF_UVEE) + 8) );
						} else if ( uvpt == p3 ) {
							uvee[3].x = binReadFloat( (layerdata + (ntri * MMB_SIZEOF_TRI) + 76 + (uv * MMB_SIZEOF_UVEE) + 4) );
							uvee[3].y = binReadFloat( (layerdata + (ntri * MMB_SIZEOF_TRI) + 76 + (uv * MMB_SIZEOF_UVEE) + 8) );
						}
						uv++;
					}
					uvee_ptr = &uvee;
				} else
					uvee_ptr = NULL;
				/* normals */
				if ( nnormals ) {
					normals[0].x = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad) + 4 + (p0 * MMB_SIZEOF_NORMAL)) );
					normals[0].y = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad) + 4 + (p0 * MMB_SIZEOF_NORMAL) + 4) );
					normals[0].z = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad) + 4 + (p0 * MMB_SIZEOF_NORMAL) + 8) );
					normals[1].x = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad) + 4 + (p1 * MMB_SIZEOF_NORMAL)) );
					normals[1].y = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad) + 4 + (p1 * MMB_SIZEOF_NORMAL) + 4) );
					normals[1].z = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad) + 4 + (p1 * MMB_SIZEOF_NORMAL) + 8) );
					normals[2].x = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad) + 4 + (p2 * MMB_SIZEOF_NORMAL)) );
					normals[2].y = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad) + 4 + (p2 * MMB_SIZEOF_NORMAL) + 4) );
					normals[2].z = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad) + 4 + (p2 * MMB_SIZEOF_NORMAL) + 8) );
					if ( quad ) {
						normals[3].x = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad) + 4 + (p3 * MMB_SIZEOF_NORMAL)) );
						normals[3].y = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad) + 4 + (p3 * MMB_SIZEOF_NORMAL) + 4) );
						normals[3].z = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad) + 4 + (p3 * MMB_SIZEOF_NORMAL) + 8) );
					}
				}
#if ! BINARY_ONLY
			} else {
				x = j * 5;
				quad = layer->tri[x];
				p0 = layer->tri[x+1];
				p1 = layer->tri[x+2];
				p2 = layer->tri[x+3];
				p3 = layer->tri[x+4];

				/* init points */
				pts[0].x = (float)model->pts[p0*3];
				pts[0].y = (float)model->pts[(p0*3)+1];
				pts[0].z = (float)model->pts[(p0*3)+2];
				pts[1].x = (float)model->pts[p1*3];
				pts[1].y = (float)model->pts[(p1*3)+1];
				pts[1].z = (float)model->pts[(p1*3)+2];
				pts[2].x = (float)model->pts[p2*3];
				pts[2].y = (float)model->pts[(p2*3)+1];
				pts[2].z = (float)model->pts[(p2*3)+2];
				pts[3].x = (float)model->pts[p3*3];
				pts[3].y = (float)model->pts[(p3*3)+1];
				pts[3].z = (float)model->pts[(p3*3)+2];
#endif
			}

			/* flip points if necessary (for negative scaled single sided models) */
			if ( ! doublesided && flip ) {
				saved  = pts[1];
				pts[1] = pts[2];
				pts[2] = saved;
			}

			/* add triangle */
			gfxSetTexture( tex_a );
			mmfAddTri( obj, limits_a, pts, norm_ptr, uvee_ptr, quad, xaxis, yaxis );

			/* do mirrored triangle */
			if ( mirrored ) {

				/* flip around the origin */
				switch( mirroraxis ) {
				case MMF_X_AXIS:
					for ( k=0; k<4; k++ )
						pts[k].x *= -1;
					break;
				case MMF_Y_AXIS:
					for ( k=0; k<4; k++ )
						pts[k].y *= -1;
					break;
				case MMF_Z_AXIS:
					for ( k=0; k<4; k++ )
						pts[k].z *= -1;
					break;
				}

				/* switch points for singlesided model */
				if ( ! doublesided ) {
					saved  = pts[1];
					pts[1] = pts[2];
					pts[2] = saved;
				}

				/* add the mirrored triangle */
				if ( tex_b != GFX_NO_TEXTURE )
					gfxSetTexture( tex_b );
				mmfAddTri( obj, limits_b, pts, norm_ptr, uvee_ptr, quad, xaxis, yaxis );
			}
		}
		/* update layer binary data pointer */
		if ( binary )
			layerdata += (76 + (ntri * MMB_SIZEOF_TRI) + (nuvee * MMB_SIZEOF_UVEE));
	}
	gfxEndObject();

	/* create child objects */
	if ( binary ) {
		childdata = layerdata + 4;
		nchildren = binReadInt( (layerdata) );
	} else
		nchildren = model->nchildren;

	j = 1;
	for ( i=0; i<nchildren; i++ ) {
		if ( binary ) {
			childname = binReadString( 16, (childdata) );
			scalex = binReadFloat( (childdata + 28) );
			scaley = binReadFloat( (childdata + 32) );
			scalez = binReadFloat( (childdata + 36) );
			positionx = binReadFloat( (childdata + 16) );
			positiony = binReadFloat( (childdata + 20) );
			positionz = binReadFloat( (childdata + 24) );
			anglex = binReadFloat( (childdata + 40) );
			angley = binReadFloat( (childdata + 44) );
			anglez = binReadFloat( (childdata + 48) );
			/* reinit for next child */
			childdata += 52;			
#if ! BINARY_ONLY
		} else {
			child = model->children[i];
			scalex = child->scale.x;
			scaley = child->scale.y;
			scalez = child->scale.z;
			positionx = child->position.x;
			positiony = child->position.y;
			positionz = child->position.z;
#endif
		}
		/* set flip flag depending on scale (gets reset so will be FALSE here) */
		if (( scalex * scaley * scalez ) < 0 )
			flip = TRUE;
		if ( binary ) {
			n = mmfModelToGfxObject( childname, &object[j], textures );
			/* reload file */
			data = gfxFileLoad( modelname );
			if ( data == NULL ) {
				mmf_error_string = "model load failed";
				return( 0 );
			}
#if ! BINARY_ONLY
		} else {
			n = mmfModelToGfxObject( (Byte *)child->model, &object[j], textures, FALSE );
#endif
		}
		/* check for failure */
		if ( n == 0 ) return( 0 );
		obj = object[j];
		/* set parent for this object and the children */
		gfxObjectParent( obj, object[0] );
		/* set position, scale and angle */
		gfxTranslateObject( obj, positionx, positiony, positionz );
		gfxScaleObject( obj, scalex, scaley, scalez );
		gfxRotateObject( obj, GFX_X_AXIS, anglex );
		gfxRotateObject( obj, GFX_Y_AXIS, angley );
		gfxRotateObject( obj, GFX_Z_AXIS, anglez );
		/* update object offset */
		nobjects += n;
		j += n;
	}

	/* reset flip flag */
	flip = FALSE;

	/* debug output */
	printf( "object loaded: %s, points = %d, layers = %d, children = %d\n", txtToUpper(modelname), npoints, nlayers, nchildren );

	return( nobjects );
}
		

/******************************************************************************
 * Function:
 * mmfAddTri -- add a MMF triangle to a GFX object
 * 
 * Description:
 *
 * Returns:
 * 
 */

private void mmfAddTri ( 
	GfxObject	*object,
	Point_f3d	*limits,
	Point_f3d	*pts,
	Point_f3d	*normals,
	Point_f2d	*uvee,
	Bool		 quad,
	char		 xaxis,
	char		 yaxis
	)
{
	Point_f2d	*texpts;
	Point_f3d	 normal;	
	Point_f3d	*norm_ptr;
	Point_f3d	 pts_swapped[3];
	Point_f2d	 uvee_swapped[3];

	/* no validation! */

	/* calc texture coords */
	if ( uvee == NULL )
		texpts = triCalcTextureCoords( pts, limits, xaxis, yaxis );				
	else
		texpts = uvee;
	
	/* calc normal */
	if ( normals == NULL ) {
		normal = triCalcNormal( pts );
		norm_ptr = &normal;
	} else
		norm_ptr = NULL;
	gfxAddTri( object, pts, texpts, norm_ptr );
	
	/* do second triangle if defined (quad) */
	if ( quad ) {

		/* using point 1..3 of 0..3, swapping point 1 & 2 for clockwise normal */
		pts_swapped[0] = pts[2];
		pts_swapped[1] = pts[1];
		pts_swapped[2] = pts[3];

		/* calc texture coords */
		if ( uvee == NULL )
			texpts = triCalcTextureCoords( pts_swapped, limits, xaxis, yaxis );
		else {
			uvee_swapped[0] = uvee[2];
			uvee_swapped[1] = uvee[1];
			uvee_swapped[2] = uvee[3];
			texpts = uvee_swapped;
		}

		/* calc normal */
		if ( normals == NULL ) {
			normal = triCalcNormal( &pts[1] );
			norm_ptr = &normal;
		} else
			norm_ptr = NULL;
		gfxAddTri( object, pts_swapped, texpts, norm_ptr );
	}
}

Text mmfModelError (
	void 
	)
{
	return( mmf_error_string );
}

