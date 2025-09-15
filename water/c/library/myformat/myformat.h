/*****************************************************************
 * Project		: Surf
 *****************************************************************
 * File			: myformat.h
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 27 Jun 1998
 * Last Update	: 27 Jun 1998
 *****************************************************************
 * Description	: My image, model and colormap formats
 *****************************************************************/

#ifndef THEYER_MYFORMAT_H
#define THEYER_MYFORMAT_H

/*
 * includes
 */

#include <type/datatype.h>
#include <gfx/gfx.h>

/* 
 * macros
 */

/*
 * MMB "My Model Binary format" FORMAT:
 * ------------------------------------
 *
 *		DESCRIPTION			SIZE (in bytes)		DEFINITION OR RANGE
 *      -----------------------------------------------------------
 *		header				4					MMB\0
 *		version				4					version number (float)
 *		number of joints	4					0..n
 *		joint 1
 *			x				2					signed 16bit point
 *			y				2					signed 16bit point
 *			z				2					signed 16bit point
 *			parent			2					signed -1=world, 0..n=joint offset n
 *			npoints			4					number of points used by joint
 *		...
 *		joint <n>
 *		number of points	4					1..n
 *		point data			(6 * npoints) + pad if necessary
 *		number of normals	4					0..n
 *		normal data			(12 * nnormals)
 *		number of layers	4					1..n
 *		layer 1				
 *			r				1					0..255
 *			g				1					0..255
 *			b				1					0..255
 *			dblesided flag	1					0 or 1
 *			transparency	4					0.0 .. 1.0
 *			ntri			4					1..n
 *			triangle data	(8 * ntri)			4th point value non 0 = quad triangle pair
 *			min, max limits 24					?
 *			image a			16					fixed length character string
 *			image b			16					fixed length character string
 *			map xaxis		1					'X', 'Y' or 'Z'
 *			map yaxis		1					'X', 'Y' or 'Z'
 *			mirrored		1					0 or 1
 *			mirroraxis		1					0 or 1
 *			nuvees			4					0..n
 *			uvee data		(12 * nuvees)		format: (pt,u,v) pt=int,u=float,v=float
 *		...
 *		layer <n>
 *		number of children	4					0..n
 *		child 1
 *			model name		16					fixed length character string
 *			position x		4					0.0 ..n
 *			position y		4					0.0 ..n
 *			position z		4					0.0 ..n
 *			scale x         4					no scale = 1.0
 *			scale y			4					no scale = 1.0
 *			scale z			4					no scale = 1.0
 *			angle x			4					0.0 ..n
 *			angle y			4					0.0 ..n
 *			angle z			4					0.0 ..n
 *		...
 *		child <n>
 *
 * MIB "My Image Binary format" FORMAT:
 * ------------------------------------
 *
 *		DESCRIPTION			SIZE (in bytes)		DEFINITION OR RANGE
 *      -----------------------------------------------------------
 *		header				4					MIB\0
 *		format				4					4,8 or 16 bit (refer below)
 *		width				2					1..n
 *		height				2					1..n
 *		colormap filename   16					fixed length character string
 *		image data			size depends on format:
 *							16 bit = (width * height * 2)
 *							 8 bit = (width * height)
 *					 		 4 bit = ((width * height)/2)
 */
 
/* 4 byte binary file header (note the terminator \0) */
#define MMB_HEADER				"MMB"
#define MIB_HEADER				"MIB"
#define MIF_HEADER				"MIF"

#define MMB_LATEST_VERSION		 0.4f

#define MMB_SIZEOF_JOINT		12
#define MMB_SIZEOF_POINT		 6
#define MMB_SIZEOF_NORMAL		12
#define MMB_SIZEOF_TRI			 8
#define MMB_SIZEOF_UVEE			12

#define MMB_DATA				TRUE
#define MMF_DATA				FALSE
#define MIB_DATA				TRUE
#define MIF_DATA				FALSE

#define MMF_X_AXIS				 1
#define MMF_Y_AXIS				 2
#define MMF_Z_AXIS				 3
#define MMF_NEG_X_AXIS			 4  
#define MMF_NEG_Y_AXIS			 5
#define MMF_NEG_Z_AXIS			 6

/* formats */
#define MIF_TEXTURE_1BIT		 1
#define MIF_TEXTURE_4BIT		 4
#define MIF_TEXTURE_8BIT		 8
#define MIF_TEXTURE_16BIT		16
#define MIF_TEXTURE_24BIT		24

/*
 * typedefs
 */

typedef short			MmfPoint;
typedef unsigned short	MmfTriangle; 
typedef unsigned short	MifColorMap;

typedef struct {
	unsigned short	r:5,g:5,b:5,a:1;
} MifPixel;

typedef struct {
	int				 format;
	MifColorMap		*colormap;
	int				 cmap_size;
	unsigned char	*data;
	unsigned short	 width, height;
} MifTexture;

typedef struct {
	unsigned char	 r,g,b;
	char			 doublesided;	//(Bool)
	float			 transparency;
	MmfTriangle	    *tri;
	int				 ntri;
	MmfPoint		 min[3];
	MmfPoint		 max[3];
	MifTexture		*image_a;
	MifTexture		*image_b;
	char			 xaxis;
	char			 yaxis;
	char			 mirrored;		//(Bool)
	char			 mirroraxis;
} MmfLayer;

typedef struct MmfModelStruct MmfModel;

typedef struct {
	struct MmfModelStruct	 *model;
	Point_f3d				  position;
	Point_f3d				  scale;
	Point_f3d				  angle;
} MmfChild;

struct MmfModelStruct {
	MmfPoint		   *pts;
	int					npts;
	MmfLayer		  **layers;
	int					nlayers;
	MmfChild		  **children;
	int					nchildren;
};


/*
 * prototypes
 */

/* mymodel.c */
extern int mmfModelToGfxObject( Text filename, GfxObject **object, GfxTextureId *textures );
extern Text mmfModelError( void );

/* myimage.c */
GfxTextureId mifTextureToGfxTexture( Text filename, Bool reload );

#endif
