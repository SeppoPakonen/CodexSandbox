/*****************************************************************
 * Project		: Model Library
 *****************************************************************
 * File			: modpak.c
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
#include <pak/pak.h>
#include <binary/binary.h>
#include <memory/memory.h>
#include <myformat/myformat.h>
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

private ModModel *modLoadFromPak( Text name, Byte *data );
private void modParseInfoFile( void *user_data );
private void modLoadPakChildren( ModModel *model, Byte *data );

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
 * modLoadPak -- load up a model from a PAK input data stream (internet capable)
 * 
 * Description:
 *
 * Returns:
 * 
 */

ModModel	*modLoadPak (
	Byte		*data
	)
{
	ModModel	*model;
	char		 model_name[512];
	Byte		*info;

	/* validate data */
	if ( ! pakValidate( data ) )
		return( NULL );

	/* get info file from pak */
	info = pakGetFile( "pak.map", data );
	if ( info == NULL ) return( NULL );

	/* parse info file */
	model_name[0] = '\0';
	pseParseData( info, "#", 1, modParseInfoFile, model_name );
	if ( model_name[0] == '\0' ) return( NULL );

	//sprintf( model_name, "%s.mmb", name );
	model = modLoadFromPak( model_name, data );
	if ( model == NULL ) return( NULL );

	/* validate */
	if ( model->nnormals > 0 && model->npts != model->nnormals ) {
		modSetErrorString( MOD_MSG_BAD_NUM_NORMALS );
		modDestroy( model );
		return( NULL );
	}

	/* load children models */
	modLoadPakChildren( model, data );

	return( model );
}


/******************************************************************************
 * Function:
 * modLoadPakChildren -- load up a children models from a PAK input data stream
 * 
 * Description:
 *
 * Returns:
 * 
 */

void modLoadPakChildren (
	ModModel	*model,
	Byte		*data
	)
{
	int		 	 i;
	char		 child_name[512];

	/* validate */
	if ( model == NULL || data == NULL ) return;

	/* load children */
	for ( i=0; i<model->nchildren; i++ ) {
		/* create file name */
		sprintf( child_name, "%s.mmb", model->children[i].name );
		model->children[i].model = modLoadFromPak( child_name, data );
		/* check for fail and clean up */
		if ( model->children[i].model == NULL ) {
			modDestroy( model );
			return;
		}
		/* re-entrant to add children to this model */
		modLoadPakChildren( model->children[i].model, data );
	}
}


/******************************************************************************
 * Function:
 * modLoadFromPak -- load up a model from a PAK input data stream (internet capable)
 * 
 * Description:
 *
 * Returns:
 * 
 */

ModModel	*modLoadFromPak (
	Text		 name,
	Byte		*data
	)
{
	ModModel	*model;
	int			 i, j, x, y, z;
	int			 len;
	Bool		 quad;
	int			 p0, p1, p2, p3;
	int			 nlayers;
	int			 npoints;
	int			 pad;
	Text		 fileid;
	int			 ntri;
	Bool		 mirrored;
	char		 mirroraxis;
	char		 xaxis;
	char		 yaxis;
	Bool		 doublesided;
	Byte		*childdata;
	Text		 childname;
	char		 imagestr[256];
	Text		 image;
	int			 nchildren;
	int			 nuvee;
	int			 uv;
	int			 uvpt;
	int			 nnormals;
	int			 normali[4];
	int			*norm_ptr;
	int			 njoints;
	char		 lname[32];
	int			 r,g,b;
	float		 fx, fy;
	Byte		*layerdata;
	float		 version;
	float		 transparency;
	Point_f3d	 norm;	
	Point_f3d	 position;
	Point_f3d	 scale;
	Point_f3d	 angle;
	Byte		*pak;

#define B2F(val)	((float)((float)val/255.0f))

/* offsets */
#define ID_OFFSET			(data)
#define VERSION_OFFSET		(data + 4)
#define NJOINTS_OFFSET		(data + 8)
#define NPOINTS_OFFSET		(data + 12 + (njoints * MMB_SIZEOF_JOINT))
#define NNORMALS_OFFSET		(data + 12 + (njoints * MMB_SIZEOF_JOINT) + 4 + (npoints * MMB_SIZEOF_POINT) + pad)
#define JOINTS_OFFSET		(NJOINTS_OFFSET  + 4)
#define POINTS_OFFSET		(NPOINTS_OFFSET  + 4)
#define NORMALS_OFFSET		(NNORMALS_OFFSET + 4)
#define NLAYERS_OFFSET		(NORMALS_OFFSET  + (nnormals * MMB_SIZEOF_NORMAL))
#define FIRST_LAYER_OFFSET	(NLAYERS_OFFSET  + 4)

	/* get file from PAK */
	pak = data;
	data = pakGetFile( name, data );
	if ( data == NULL ) return( NULL );

	/* validate binary file */
	fileid = binReadString( 4, ID_OFFSET );
	if ( ! txtMatch( fileid, MMB_HEADER ) )
		return( NULL );
	version = binReadFloat( VERSION_OFFSET );
	if ( version != MMB_LATEST_VERSION )
		return( NULL );
	/* number of joints */
	njoints = binReadInt( NJOINTS_OFFSET );
	npoints = binReadInt( NPOINTS_OFFSET );
	if ( npoints % 2 )
		pad = 2;
	else
		pad = 0;
	/* get number of normals (must equal number of points or 0) */
	nnormals = binReadInt( NNORMALS_OFFSET );
	if ( nnormals ) {
		if ( nnormals != npoints )
			return( NULL );
		norm_ptr = normali;
	} else
		norm_ptr = NULL;
	nlayers   = binReadInt( NLAYERS_OFFSET );
	layerdata = FIRST_LAYER_OFFSET;

	/* create object */
	model = modCreate( name, NULL );

	/* add point data */
	for ( i=0; i<npoints; i++ ) {
		x = (fixed)binReadShort( (POINTS_OFFSET + (i * MMB_SIZEOF_POINT)) );
		y = (fixed)binReadShort( (POINTS_OFFSET + (i * MMB_SIZEOF_POINT) + 2) );
		z = (fixed)binReadShort( (POINTS_OFFSET + (i * MMB_SIZEOF_POINT) + 4) );
		modAddPoint( model, (i+1), x, y, z );
	}

	/* add normal data */
	for ( i=0; i<nnormals; i++ ) {
		norm.x = binReadFloat( (NORMALS_OFFSET + (i * MMB_SIZEOF_NORMAL)) );
		norm.y = binReadFloat( (NORMALS_OFFSET + (i * MMB_SIZEOF_NORMAL) + 4) );
		norm.z = binReadFloat( (NORMALS_OFFSET + (i * MMB_SIZEOF_NORMAL) + 8) );
		modAddNormal( model, (i+1), norm.x, norm.y, norm.z );
	}

	/* create graphics */
	for ( i=0; i<nlayers; i++ ) {

		/* init */
		ntri       = binReadInt( (layerdata + 8) );
		mirrored   = (Bool)binReadByte( (layerdata + (ntri * MMB_SIZEOF_TRI) + 70) );
		mirroraxis = (char)binReadByte( (layerdata + (ntri * MMB_SIZEOF_TRI) + 71) );
		transparency  = binReadFloat( (layerdata + 4) );		
		r = (int)binReadByte( (layerdata) );
		g = (int)binReadByte( (layerdata + 1) );
		b = (int)binReadByte( (layerdata + 2) );
		doublesided = (Bool)binReadByte( (layerdata + 3) );
		xaxis = (char)binReadByte( (layerdata + (ntri * MMB_SIZEOF_TRI) + 68) );
		yaxis = (char)binReadByte( (layerdata + (ntri * MMB_SIZEOF_TRI) + 69) );
		nuvee = binReadInt( (layerdata + (ntri * MMB_SIZEOF_TRI) + 72) );

		/* add layer to model */
		sprintf( lname, "layer%d\n", i );
		if ( ! modAddLayer( model, lname, r, g, b, !doublesided, mirrored, mirroraxis ) ) {
			modDestroy( model );
			return( NULL );
		}

		modAddTransparency( model, lname, transparency );

		/* add triangles */
		for ( j=0; j<ntri; j++ ) {

			/* init */
			p0 = binReadUShort( (layerdata + 12 + (j * MMB_SIZEOF_TRI)) );
			p1 = binReadUShort( (layerdata + 12 + (j * MMB_SIZEOF_TRI) + 2) );
			p2 = binReadUShort( (layerdata + 12 + (j * MMB_SIZEOF_TRI) + 4) );
			p3 = binReadUShort( (layerdata + 12 + (j * MMB_SIZEOF_TRI) + 6) );
			quad = (p3 != 0);

			/* get uvee points */
			if ( nuvee ) {
				uv = 0;
				while( uv < nuvee ) {
					uvpt = binReadInt( (layerdata + (ntri * MMB_SIZEOF_TRI) + 76 + (uv * MMB_SIZEOF_UVEE)) );
					fx = binReadFloat( (layerdata + (ntri * MMB_SIZEOF_TRI) + 76 + (uv * MMB_SIZEOF_UVEE) + 4) );
					fy = binReadFloat( (layerdata + (ntri * MMB_SIZEOF_TRI) + 76 + (uv * MMB_SIZEOF_UVEE) + 8) );
					modAddUVee( model, lname, uvpt, fx, fy );
					uv++;
				}
			}
			/* add triangle */
			modAddTriangle( model, lname, quad, (p0 + 1), (p1 + 1), (p2 + 1), (p3 + 1) );
		}

		/* load texture(s) */

		/* get image name */
		image = binReadString( 16, (layerdata + 36 + (ntri * MMB_SIZEOF_TRI)) );
		//printf( "image a string: %s\n", image );
		len = txtLength( image );
		if ( len > 0 ) {
			txtCopy( image, imagestr );
			/* add extension */
			txtCopy( ".mib", &imagestr[len] );
			if ( ! modAddImageFromPak( pak, model, lname, 0, imagestr, xaxis, yaxis ) ) {
				//modDestroy( model );
				//return( NULL );
			}
		}

		/* get image name */
		image = binReadString( 16, (layerdata + 52 + (ntri * MMB_SIZEOF_TRI)) );
		//printf( "image b string: %s\n", image );
		len = txtLength( image );
		if ( len > 0 ) {
			txtCopy( image, imagestr );
			/* add extension */
			txtCopy( ".mib", &imagestr[len] );
			//printf( "image b: %s\n", imagestr );
			if ( ! modAddImageFromPak( pak, model, lname, 1, imagestr, xaxis, yaxis ) ) {
				//modDestroy( model );
				//return( NULL );
			}
		}

		/* update layer binary data pointer */
		layerdata += (76 + (ntri * MMB_SIZEOF_TRI) + (nuvee * MMB_SIZEOF_UVEE));
	}

	/* create child objects */
	childdata = layerdata + 4;
	nchildren = binReadInt( (layerdata) );

	j = 1;
	for ( i=0; i<nchildren; i++ ) {
		childname = binReadString( 16, (childdata) );
		scale.x = binReadFloat( (childdata + 28) );
		scale.y = binReadFloat( (childdata + 32) );
		scale.z = binReadFloat( (childdata + 36) );
		position.x = binReadFloat( (childdata + 16) );
		position.y = binReadFloat( (childdata + 20) );
		position.z = binReadFloat( (childdata + 24) );
		angle.x = binReadFloat( (childdata + 40) );
		angle.y = binReadFloat( (childdata + 44) );
		angle.z = binReadFloat( (childdata + 48) );
		/* add to model */
		modAddChild( model, childname, position, scale, angle );
		/* reinit for next child */
		childdata += 52;			
	}

	return( model );
}


/******************************************************************************
 * Function:
 * modLoadMibFromPak -- add an image from a PAK file into a model layer
 * 
 * Description:
 *
 * Returns:
 * 
 */

MibFile *modLoadMibFromPak (
	Byte	 *data,
	Text	  image, 
	char	**errstr
	)
{
	MibFile		 pic;
	MibFile		*rpic;
	Byte		*idata;
	Byte		*cdata;
	Byte		*cptr;
	int			 x, y, i;
	int			 format;
	int			 size;
	Byte		 r[256];
	Byte		 g[256];
	Byte		 b[256];
	Text		 cmapname;
	char		 cmapnamestr[64];
	int			 width;
	int			 height;
	MifPixel	*pix;
#define BIT5_TO_BIT8(val)			(val<<3)

	/* get file from pak */
	idata = pakGetFile( image, data ); 
	if ( idata == NULL ) return( NULL );

	/* only support 8 bit images */
	format = binReadInt( (idata + 4) );
	if ( (format != MIF_TEXTURE_8BIT) && (format != MIF_TEXTURE_16BIT) )
		return( NULL );

	if ( format == MIF_TEXTURE_8BIT ) {
		/* get colormap name from image and load it first */
		cmapname = binReadString( 16, (idata + 12) );
		//printf( "colormap name = %s\n", cmapname );
		if ( cmapname[0] == '\0' ) 
			return( NULL );

		/* add binary colormap (CMB) file extension */
		txtCopy( cmapname, cmapnamestr );
		i = txtLength( cmapnamestr );
		txtCopy( ".cmb", &cmapnamestr[i] );

		/* load colormap */
		cdata = pakGetFile( cmapnamestr, data ); 
		if ( cdata == NULL ) return( NULL );
		width  = binReadShort( (cdata + 8) );
		height = binReadShort( (cdata + 10) );
		if ( (width < 1) || (width > 256) ) return( NULL );
		if ( height != 1 ) return( NULL );

		/* read in colormap */
		pix = (MifPixel *)(cdata + 28);
		for ( i=0; i<width; i++, pix++ ) {
			r[i] = BIT5_TO_BIT8(pix->r);
			g[i] = BIT5_TO_BIT8(pix->g);
			b[i] = BIT5_TO_BIT8(pix->b);
		}

		/* get the image data */
		width  = binReadShort( (idata + 8) );
		height = binReadShort( (idata + 10) );
		if ( (width  < 1) || (width  > 512) ) return( NULL );
		if ( (height < 1) || (height > 512) ) return( NULL );
		size  = (width * height * 3);
		cdata = (Byte *)memAlloc(size);

		/* read in colormap indexed image to create the rgb version */
		cptr = cdata;
		for ( y=0; y<height; y++ ) {
			for ( x=0; x<width; x++ ) {
				/* get index from image */
				i = *(idata + 28 + (y*width) + x);
				if ( (i < -1) && (i > 256 ) ) {
					memFree( cdata );
					return( NULL );
				}
				*(cptr) = r[i]; cptr++;
				*(cptr) = g[i]; cptr++;
				*(cptr) = b[i]; cptr++;
			}
		}
	} else {
		/* 16 bit */

		/* get the image data */
		width  = binReadShort( (idata + 8) );
		height = binReadShort( (idata + 10) );
		if ( (width  < 1) || (width  > 512) ) return( NULL );
		if ( (height < 1) || (height > 512) ) return( NULL );
		size  = (width * height * 3);
		cdata = (Byte *)memAlloc(size);

		/* read in 16bit rgb (1:5:5:5) image to create the rgb version */
		cptr = cdata;
		for ( y=0; y<height; y++ ) {
			for ( x=0; x<width; x++ ) {
				/* get pixel from image (2 bytes per pixel) */
				pix = (MifPixel *)(idata + 28 + (y*width*2) + (x*2));
				*(cptr) = BIT5_TO_BIT8(pix->r); cptr++;
				*(cptr) = BIT5_TO_BIT8(pix->g); cptr++;
				*(cptr) = BIT5_TO_BIT8(pix->b); cptr++;
			}
		}
	}

	/* create return data */
	pic.data   = cdata;
	pic.width  = width;
	pic.height = height;

	/* copy to new memory  */	
	rpic  = memNew(MibFile);
	*rpic = pic;

	return( rpic );
}


/******************************************************************************
 * Function:
 * modParseInfoFile -- parse info file from PAK
 * 
 * Description:
 *
 * Returns:
 * 
 */

void modParseInfoFile (
    void	*user_data
    )
{
	Text	 modname;
	Text	 str;

	/* init */
	modname = (Text)user_data;

	/* no error checking! */
	if ( pseArgValidate( "model", 0 ) ) {
		str = pseGetStringValue(1);
		if ( str != NULL )
			sprintf( modname, "%s.mmb", str );
	}
}


