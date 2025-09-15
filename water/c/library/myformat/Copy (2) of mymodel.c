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

/*
 * typedefs
 */

/*
 * prototypes
 */

#ifdef MMF_FIXEDPOINT_API
private void mmfAddTri( GfxObject *object, Point_i3d *limits, Point_i3d *pts, int *normals, Point_fx2d *uvee, Bool quad, char xaxis, char yaxis );
#else
private void mmfAddTri( GfxObject *object, Point_f3d *limits, Point_f3d *pts, int *normals, Point_f2d *uvee, Bool quad, char xaxis, char yaxis );
#endif

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
	//MmfModel	*model;
	//MmfLayer	*layer;
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
	Text		 image;
	Bool		 doublesided;
	char		 modelname[32];
	char		 imagestr[32];
	int			 len;
	static Bool	 flip = FALSE;
	//char		 childname[32];
	Byte		*childdata;
	Text		 childname;
	int			 nchildren;
	//Bool		 binary;
	int			 nuvee;
	Byte		*data;
	int			 uv;
	int			 uvpt;
	int			 nnormals;
	int			 normali[4];
	int			*norm_ptr;

#ifdef MMF_FIXEDPOINT_API
	fixed		 r,g,b;
	float		 version;
	fixed		 transparency;
	fixed		 scalex;
	fixed		 scaley;
	fixed		 scalez;
	int			 positionx;
	int			 positiony;
	int			 positionz;
	fixed		 anglex;
	fixed		 angley;
	fixed		 anglez;
	Point_i3d	 pts[4];
	Point_fx2d	 uvee[4];
	Point_fx2d	*uvee_ptr;
	Point_i3d	 saved;
	Point_i3d	 limits_a[2];
	Point_i3d	 limits_b[2];
	Point_fx3d	 norm;	
	Point_fx3d	 normals[4];
	Point_i3d	 pt;
#else
	float		 r,g,b;
	float		 version;
	float		 transparency;
	float		 scalex;
	float		 scaley;
	float		 scalez;
	float		 positionx;
	float		 positiony;
	float		 positionz;
	float		 anglex;
	float		 angley;
	float		 anglez;
	Point_f3d	 pts[4];
	Point_f2d	 uvee[4];
	Point_f2d	*uvee_ptr;
	Point_f3d	 saved;
	Point_f3d	 limits_a[2];
	Point_f3d	 limits_b[2];
	Point_f3d	 norm;	
	Point_f3d	 normals[4];
	Point_f3d	 pt;
#endif

#define B2F(val)	((float)((float)val/255.0f))

	/* validate */
	if ( filename == NULL || object == NULL ) return( 0 );

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
	//printf( "load model: %s\n", modelname );
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
	if ( npoints % 2 )
		pad = 2;
	else
		pad = 0;
	/* get number of normals (must equal number of points or 0) */
	nnormals = binReadInt( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad) );
	if ( nnormals ) {
#if 1
		if ( nnormals != npoints ) {
			mmf_error_string = "invalid num. of normals";
			return( 0 );
		}
#endif
		norm_ptr = normali;
	} else
		norm_ptr = NULL;
	nlayers  = binReadInt( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad) + 4 + (nnormals * MMB_SIZEOF_NORMAL) );
	layerdata = (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (nnormals * MMB_SIZEOF_NORMAL) + 4);

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
#ifdef MMF_FIXEDPOINT_API
		pt.x = (fixed)binReadShort( (data + 12 + (i * MMB_SIZEOF_POINT)) );
		pt.y = (fixed)binReadShort( (data + 12 + (i * MMB_SIZEOF_POINT) + 2) );
		pt.z = (fixed)binReadShort( (data + 12 + (i * MMB_SIZEOF_POINT) + 4) );
#else
		pt.x = (float)binReadShort( (data + 12 + (i * MMB_SIZEOF_POINT)) );
		pt.y = (float)binReadShort( (data + 12 + (i * MMB_SIZEOF_POINT) + 2) );
		pt.z = (float)binReadShort( (data + 12 + (i * MMB_SIZEOF_POINT) + 4) );
#endif
		//printf( "add point %d of %d: %d, %d, %d", i, npoints, pt.x, pt.y, pt.z );
		gfxAddPoints( obj, &pt, 1 );
		//printf( "\n" );
	}

	/* add normal data */
	for ( i=0; i<nnormals; i++ ) {
#ifdef MMF_FIXEDPOINT_API
		norm.x = FLOAT_TO_FIXED( binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (i * MMB_SIZEOF_NORMAL)) ) );
		norm.y = FLOAT_TO_FIXED( binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (i * MMB_SIZEOF_NORMAL) + 4) ) );
		norm.z = FLOAT_TO_FIXED( binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (i * MMB_SIZEOF_NORMAL) + 8) ) );
#else
		norm.x = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (i * MMB_SIZEOF_NORMAL)) );
		norm.y = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (i * MMB_SIZEOF_NORMAL) + 4) );
		norm.z = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (i * MMB_SIZEOF_NORMAL) + 8) );
#endif
		gfxAddNormals( obj, &norm, 1 );
	}

	/* create graphics */
	for ( i=0; i<nlayers; i++ ) {

		/* init */
		ntri       = binReadInt( (layerdata + 8) );
		mirrored   = (Bool)binReadByte( (layerdata + (ntri * MMB_SIZEOF_TRI) + 70) );
		mirroraxis = (char)binReadByte( (layerdata + (ntri * MMB_SIZEOF_TRI) + 71) );
#ifdef MMF_FIXEDPOINT_API
		limits_a[0].x = (int)binReadFloat( (layerdata + 12 + (ntri * MMB_SIZEOF_TRI)) );
		limits_a[0].y = (int)binReadFloat( (layerdata + 12 + (ntri * MMB_SIZEOF_TRI) +  4) );
		limits_a[0].z = (int)binReadFloat( (layerdata + 12 + (ntri * MMB_SIZEOF_TRI) +  8) );
		limits_a[1].x = (int)binReadFloat( (layerdata + 12 + (ntri * MMB_SIZEOF_TRI) + 12) );
		limits_a[1].y = (int)binReadFloat( (layerdata + 12 + (ntri * MMB_SIZEOF_TRI) + 16) );
		limits_a[1].z = (int)binReadFloat( (layerdata + 12 + (ntri * MMB_SIZEOF_TRI) + 20) );
		transparency  = FLOAT_TO_FIXED( binReadFloat( (layerdata + 4) ) );
		r = FIXED_NORMALISE( (int)binReadByte( (layerdata) ),     255 );
		g = FIXED_NORMALISE( (int)binReadByte( (layerdata + 1) ), 255 );
		b = FIXED_NORMALISE( (int)binReadByte( (layerdata + 2) ), 255 );
#else
		limits_a[0].x = binReadFloat( (layerdata + 12 + (ntri * MMB_SIZEOF_TRI)) );
		limits_a[0].y = binReadFloat( (layerdata + 12 + (ntri * MMB_SIZEOF_TRI) +  4) );
		limits_a[0].z = binReadFloat( (layerdata + 12 + (ntri * MMB_SIZEOF_TRI) +  8) );
		limits_a[1].x = binReadFloat( (layerdata + 12 + (ntri * MMB_SIZEOF_TRI) + 12) );
		limits_a[1].y = binReadFloat( (layerdata + 12 + (ntri * MMB_SIZEOF_TRI) + 16) );
		limits_a[1].z = binReadFloat( (layerdata + 12 + (ntri * MMB_SIZEOF_TRI) + 20) );
		transparency  = binReadFloat( (layerdata + 4) );		
		r = B2F( binReadByte( (layerdata) ) );
		g = B2F( binReadByte( (layerdata + 1) ) );
		b = B2f( binReadByte( (layerdata + 2) ) );
#endif
		doublesided = (Bool)binReadByte( (layerdata + 3) );
		xaxis = (char)binReadByte( (layerdata + (ntri * MMB_SIZEOF_TRI) + 68) );
		yaxis = (char)binReadByte( (layerdata + (ntri * MMB_SIZEOF_TRI) + 69) );
		nuvee = binReadInt( (layerdata + (ntri * MMB_SIZEOF_TRI) + 72) );

		/* init limits for mirror */
		if ( mirrored ) {
			/* init limits for mirror */
			limits_b[0] = limits_a[0];
			limits_b[1] = limits_a[1];

			/* flip around the origin */
			switch( mirroraxis ) {
			case MMF_X_AXIS:
				limits_b[0].x = -(limits_b[0].x);
				limits_b[1].x = -(limits_b[1].x);
				break;
			case MMF_Y_AXIS:
				limits_b[0].y = -(limits_b[0].y);
				limits_b[1].y = -(limits_b[1].y);
				break;
			case MMF_Z_AXIS:
				limits_b[0].z = -(limits_b[0].z);
				limits_b[1].z = -(limits_b[1].z);
				break;
			}
		}

		/* set color and doublesided */
		gfxSetColor( r, g, b );
		gfxSetDoubleSided( doublesided );
		gfxSetTransparency( transparency );

		/* load texture(s) */
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
			//printf( "gfxTextureId()..." );
			tex_a = gfxTextureId( imagestr );
			//printf( "done\n" );
			if ( tex_a == GFX_NO_TEXTURE ) {
				//printf( "mifTextureToGfxTexture()..." );
				tex_a = mifTextureToGfxTexture( imagestr, FALSE );
				//printf( "done\n" );
				/* reload model file into file buffer */
				//printf( "load model: %s\n", modelname );
				data = gfxFileLoad( modelname );
				if ( data == NULL ) {
					mmf_error_string = "model load failed";
					return( 0 );
				}
			}
			//printf( "ok\n" );
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
			p0 = binReadUShort( (layerdata + 12 + (j * MMB_SIZEOF_TRI)) );
			p1 = binReadUShort( (layerdata + 12 + (j * MMB_SIZEOF_TRI) + 2) );
			p2 = binReadUShort( (layerdata + 12 + (j * MMB_SIZEOF_TRI) + 4) );
			p3 = binReadUShort( (layerdata + 12 + (j * MMB_SIZEOF_TRI) + 6) );
			quad = (p3 != 0);
#ifdef MMF_FIXEDPOINT_API
			pts[0].x = binReadShort( (data + 12 + (p0 * MMB_SIZEOF_POINT)) );
			pts[0].y = binReadShort( (data + 12 + (p0 * MMB_SIZEOF_POINT) + 2) );
			pts[0].z = binReadShort( (data + 12 + (p0 * MMB_SIZEOF_POINT) + 4) );
			pts[1].x = binReadShort( (data + 12 + (p1 * MMB_SIZEOF_POINT)) );
			pts[1].y = binReadShort( (data + 12 + (p1 * MMB_SIZEOF_POINT) + 2) );
			pts[1].z = binReadShort( (data + 12 + (p1 * MMB_SIZEOF_POINT) + 4) );
			pts[2].x = binReadShort( (data + 12 + (p2 * MMB_SIZEOF_POINT)) );
			pts[2].y = binReadShort( (data + 12 + (p2 * MMB_SIZEOF_POINT) + 2) );
			pts[2].z = binReadShort( (data + 12 + (p2 * MMB_SIZEOF_POINT) + 4) );
			if ( quad ) {
				pts[3].x = binReadShort( (data + 12 + (p3 * MMB_SIZEOF_POINT)) );
				pts[3].y = binReadShort( (data + 12 + (p3 * MMB_SIZEOF_POINT) + 2) );
				pts[3].z = binReadShort( (data + 12 + (p3 * MMB_SIZEOF_POINT) + 4) );
			}
#else
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
#endif
			/* get uvee points */
			if ( nuvee ) {
				uv = 0;
				while( uv < nuvee ) {
					uvpt = binReadInt( (layerdata + (ntri * MMB_SIZEOF_TRI) + 76 + (uv * MMB_SIZEOF_UVEE)) );
#ifdef MMF_FIXEDPOINT_API
					if ( uvpt == p0 ) {
						uvee[0].x = FLOAT_TO_FIXED( binReadFloat( (layerdata + (ntri * MMB_SIZEOF_TRI) + 76 + (uv * MMB_SIZEOF_UVEE) + 4) ) );
						uvee[0].y = FLOAT_TO_FIXED( binReadFloat( (layerdata + (ntri * MMB_SIZEOF_TRI) + 76 + (uv * MMB_SIZEOF_UVEE) + 8) ) );
					} else if ( uvpt == p1 ) {
						uvee[1].x = FLOAT_TO_FIXED( binReadFloat( (layerdata + (ntri * MMB_SIZEOF_TRI) + 76 + (uv * MMB_SIZEOF_UVEE) + 4) ) );
						uvee[1].y = FLOAT_TO_FIXED( binReadFloat( (layerdata + (ntri * MMB_SIZEOF_TRI) + 76 + (uv * MMB_SIZEOF_UVEE) + 8) ) );
					} else if ( uvpt == p2 ) {
						uvee[2].x = FLOAT_TO_FIXED( binReadFloat( (layerdata + (ntri * MMB_SIZEOF_TRI) + 76 + (uv * MMB_SIZEOF_UVEE) + 4) ) );
						uvee[2].y = FLOAT_TO_FIXED( binReadFloat( (layerdata + (ntri * MMB_SIZEOF_TRI) + 76 + (uv * MMB_SIZEOF_UVEE) + 8) ) );
					} else if ( uvpt == p3 ) {
						uvee[3].x = FLOAT_TO_FIXED( binReadFloat( (layerdata + (ntri * MMB_SIZEOF_TRI) + 76 + (uv * MMB_SIZEOF_UVEE) + 4) ) );
						uvee[3].y = FLOAT_TO_FIXED( binReadFloat( (layerdata + (ntri * MMB_SIZEOF_TRI) + 76 + (uv * MMB_SIZEOF_UVEE) + 8) ) );
					}
#else
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
#endif
					uv++;
				}
				uvee_ptr = uvee;
			} else
				uvee_ptr = NULL;

			/* normals */
			if ( nnormals ) {
				normali[0] = p0;
				normali[1] = p1;
				normali[2] = p2;
				normali[3] = p3;
			}

			/* flip points if necessary (for negative scaled single sided models) */
			if ( ! doublesided && flip ) {
				saved  = pts[1];
				pts[1] = pts[2];
				pts[2] = saved;
				if ( nnormals ) {
					k = normali[1];
					normali[1] = normali[2];
					normali[2] = k;
				}
			}

			/* add triangle */
			gfxSetTexture( tex_a );
			mmfAddTri( obj, limits_a, pts, norm_ptr, uvee_ptr, quad, xaxis, yaxis );

			/* do mirrored triangle */
			if ( mirrored ) {

				if ( nnormals ) {
					/* get normals */
#ifdef MMF_FIXEDPOINT_API
					normals[0].x = FLOAT_TO_FIXED( binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p0 * MMB_SIZEOF_NORMAL)) ) );
					normals[0].y = FLOAT_TO_FIXED( binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p0 * MMB_SIZEOF_NORMAL) + 4) ) );
					normals[0].z = FLOAT_TO_FIXED( binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p0 * MMB_SIZEOF_NORMAL) + 8) ) );
					normals[1].x = FLOAT_TO_FIXED( binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p1 * MMB_SIZEOF_NORMAL)) ) );
					normals[1].y = FLOAT_TO_FIXED( binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p1 * MMB_SIZEOF_NORMAL) + 4) ) );
					normals[1].z = FLOAT_TO_FIXED( binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p1 * MMB_SIZEOF_NORMAL) + 8) ) );
					normals[2].x = FLOAT_TO_FIXED( binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p2 * MMB_SIZEOF_NORMAL)) ) );
					normals[2].y = FLOAT_TO_FIXED( binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p2 * MMB_SIZEOF_NORMAL) + 4) ) );
					normals[2].z = FLOAT_TO_FIXED( binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p2 * MMB_SIZEOF_NORMAL) + 8) ) );
					if ( quad ) {
						normals[3].x = FLOAT_TO_FIXED( binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p3 * MMB_SIZEOF_NORMAL)) ) );
						normals[3].y = FLOAT_TO_FIXED( binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p3 * MMB_SIZEOF_NORMAL) + 4) ) );
						normals[3].z = FLOAT_TO_FIXED( binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p3 * MMB_SIZEOF_NORMAL) + 8) ) );
					}
#else
					normals[0].x = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p0 * MMB_SIZEOF_NORMAL)) );
					normals[0].y = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p0 * MMB_SIZEOF_NORMAL) + 4) );
					normals[0].z = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p0 * MMB_SIZEOF_NORMAL) + 8) );
					normals[1].x = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p1 * MMB_SIZEOF_NORMAL)) );
					normals[1].y = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p1 * MMB_SIZEOF_NORMAL) + 4) );
					normals[1].z = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p1 * MMB_SIZEOF_NORMAL) + 8) );
					normals[2].x = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p2 * MMB_SIZEOF_NORMAL)) );
					normals[2].y = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p2 * MMB_SIZEOF_NORMAL) + 4) );
					normals[2].z = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p2 * MMB_SIZEOF_NORMAL) + 8) );
					if ( quad ) {
						normals[3].x = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p3 * MMB_SIZEOF_NORMAL)) );
						normals[3].y = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p3 * MMB_SIZEOF_NORMAL) + 4) );
						normals[3].z = binReadFloat( (data + 12 + (npoints * MMB_SIZEOF_POINT) + pad + 4 + (p3 * MMB_SIZEOF_NORMAL) + 8) );
					}
#endif
					/* flip around the origin */
					switch( mirroraxis ) {
					case MMF_X_AXIS:
						for ( k=0; k<4; k++ ) {
							pts[k].x     = -(pts[k].x);
							normals[k].x = -(normals[k].x);
						}
						break;
					case MMF_Y_AXIS:
						for ( k=0; k<4; k++ ) {
							pts[k].y     = -(pts[k].y);
							normals[k].y = -(normals[k].y);
						}
						break;
					case MMF_Z_AXIS:
						for ( k=0; k<4; k++ ) {
							pts[k].z     = -(pts[k].z);
							normals[k].z = -(normals[k].z);
						}
						break;
					}

					/* add new, mirrored normals to list */
					for ( k=0; k<3; k++ )
						normali[k] = gfxAddNormals( obj, &normals[k], 1 );
					if ( quad )
						normali[3] = gfxAddNormals( obj, &normals[3], 1 );

				} else {

					/* flip around the origin */
					switch( mirroraxis ) {
					case MMF_X_AXIS:
						for ( k=0; k<4; k++ )
							pts[k].x = -(pts[k].x);
						break;
					case MMF_Y_AXIS:
						for ( k=0; k<4; k++ )
							pts[k].y = -(pts[k].y);
						break;
					case MMF_Z_AXIS:
						for ( k=0; k<4; k++ )
							pts[k].z = -(pts[k].z);
						break;
					}

				}
				
				/* switch points for singlesided model */
				if ( ! doublesided ) {
					saved  = pts[1];
					pts[1] = pts[2];
					pts[2] = saved;
					if ( nnormals ) {
						k = normali[1];
						normali[1] = normali[2];
						normali[2] = k;
					}
				}

				/* add the mirrored triangle */
				if ( tex_b != GFX_NO_TEXTURE )
					gfxSetTexture( tex_b );
				mmfAddTri( obj, limits_b, pts, norm_ptr, uvee_ptr, quad, xaxis, yaxis );
			}
		}
		/* update layer binary data pointer */
		layerdata += (76 + (ntri * MMB_SIZEOF_TRI) + (nuvee * MMB_SIZEOF_UVEE));
	}
	gfxEndObject( obj );

	/* create child objects */
	childdata = layerdata + 4;
	nchildren = binReadInt( (layerdata) );

	j = 1;
	for ( i=0; i<nchildren; i++ ) {
		childname = binReadString( 16, (childdata) );
#ifdef MMF_FIXEDPOINT_API
		scalex = FLOAT_TO_FIXED( binReadFloat( (childdata + 28) ) );
		scaley = FLOAT_TO_FIXED( binReadFloat( (childdata + 32) ) );
		scalez = FLOAT_TO_FIXED( binReadFloat( (childdata + 36) ) );
		positionx = (int)( binReadFloat( (childdata + 16) ) );
		positiony = (int)( binReadFloat( (childdata + 20) ) );
		positionz = (int)( binReadFloat( (childdata + 24) ) );
		anglex = (int)(binReadFloat( (childdata + 40) ));
		angley = (int)(binReadFloat( (childdata + 44) ));
		anglez = (int)(binReadFloat( (childdata + 48) ));
#else
		scalex = binReadFloat( (childdata + 28) );
		scaley = binReadFloat( (childdata + 32) );
		scalez = binReadFloat( (childdata + 36) );
		positionx = binReadFloat( (childdata + 16) );
		positiony = binReadFloat( (childdata + 20) );
		positionz = binReadFloat( (childdata + 24) );
		anglex = binReadFloat( (childdata + 40) );
		angley = binReadFloat( (childdata + 44) );
		anglez = binReadFloat( (childdata + 48) );
#endif
		//printf( "child %d = %s\n", i, childname );
		/* reinit for next child */
		childdata += 52;			
		/* set flip flag depending on scale (gets reset so will be FALSE here) */
		if (( scalex * scaley * scalez ) < 0 )
			flip = TRUE;
		n = mmfModelToGfxObject( childname, &object[j], textures );
		/* reload file */
		data = gfxFileLoad( modelname );
		if ( data == NULL ) {
			mmf_error_string = "model load failed";
			return( 0 );
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
	//printf( "object loaded: %s, points = %d, layers = %d, children = %d\n", txtToUpper(modelname), npoints, nlayers, nchildren );

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

#ifdef MMF_FIXEDPOINT_API

private void mmfAddTri ( 
	GfxObject	*object,
	Point_i3d	*limits,
	Point_i3d	*pts,
	int			*normals,
	Point_fx2d	*uvee,
	Bool		 quad,
	char		 xaxis,
	char		 yaxis
	)
{
	Point_fx2d	*texpts;
	//Point_f3d	 normal;	
	int			*norm_ptr;
	//int		 nnormals;
	Point_i3d	 pts_swapped[3];
	int			 norm_swapped[3];
	Point_fx2d	 uvee_swapped[4];

#else

	private void mmfAddTri ( 
	GfxObject	*object,
	Point_f3d	*limits,
	Point_f3d	*pts,
	int			*normals,
	Point_f2d	*uvee,
	Bool		 quad,
	char		 xaxis,
	char		 yaxis
	)
{
	Point_f2d	*texpts;
	//Point_f3d	 normal;	
	int			*norm_ptr;
	//int		 nnormals;
	Point_f3d	 pts_swapped[3];
	int			 norm_swapped[3];
	Point_f2d	 uvee_swapped[4];

#endif

	/* no validation! */

	/* calc texture coords */
	if ( uvee == NULL )
		texpts = triCalcTextureCoords( pts, limits, xaxis, yaxis );				
	else
		texpts = uvee;
	
#ifdef MMF_CREATE_TRIANGLES_ONLY
	gfxAddTri( object, pts, texpts, normals );
	
	/* do second triangle if defined (quad) */
	if ( quad ) {

		/* using point 1..3 of 0..3, swapping point 1 & 2 for clockwise normal */
		pts_swapped[0] = pts[2];
		pts_swapped[1] = pts[1];
		pts_swapped[2] = pts[3];
		if ( normals != NULL ) {
			norm_swapped[0] = normals[2];
			norm_swapped[1] = normals[1];
			norm_swapped[2] = normals[3];
			norm_ptr = norm_swapped;
		} else
			norm_ptr = NULL;

		/* calc texture coords */
		if ( uvee == NULL )
			texpts = triCalcTextureCoords( pts_swapped, limits, xaxis, yaxis );
		else {
			uvee_swapped[0] = uvee[2];
			uvee_swapped[1] = uvee[1];
			uvee_swapped[2] = uvee[3];
			texpts = uvee_swapped;
		}

		gfxAddTri( object, pts_swapped, texpts, norm_ptr );
	}
#else
	if ( quad )	{
		if ( uvee == NULL ) {
			/* save uvees for first quad triangle */
			uvee_swapped[0] = texpts[0];
			uvee_swapped[1] = texpts[1];
			uvee_swapped[2] = texpts[2];
			/* get texture point for quad triangle */
			pts_swapped[0] = pts[2];
			pts_swapped[1] = pts[1];
			pts_swapped[2] = pts[3];
			/* calc texture coords */
			texpts = triCalcTextureCoords( pts_swapped, limits, xaxis, yaxis );
			uvee_swapped[3] = texpts[2];
			texpts = uvee_swapped;
		}
		gfxAddQuad( object, pts, texpts, normals );
	} else
		gfxAddTri( object, pts, texpts, normals );
#endif
}

Text mmfModelError (
	void 
	)
{
	return( mmf_error_string );
}

