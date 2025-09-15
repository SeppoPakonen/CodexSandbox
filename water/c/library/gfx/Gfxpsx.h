/**********************************************************
 * Copyright (C) 1997 Mark Theyer
 * All Rights Reserved
 **********************************************************
 * Project:	PSX game
 **********************************************************
 * File:	gfxpsx.h
 * Author:	Mark Theyer
 * Created:	01 May 1997
 **********************************************************
 * Description:	PSX Graphics Interface
 **********************************************************
 * Revision History:
 * 01-May-97	Theyer	Initial Coding
 **********************************************************/

#ifndef THEYER_GFXPSX_H
#define THEYER_GFXPSX_H

/*
 * includes
 */

#include <stdio.h>
#include <type/datatype.h>
#include <gfx/gfx.h>
#include <text/text.h>
#include <gfxparam.h>

/* playstation headers */
#ifdef PSX_FULL_DEV_KIT
/* full kit */
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libhmd.h>
#include <libcd.h>
#include <libsn.h>
#define _LIBPS_H_
#define MODE_NTSC	0
#define MODE_PAL	1
#define getScratchAddr(offset)  ((u_long *)(0x1f800000+offset*4))
#else
/* yaroze kit */
#include <libps.h>
#endif

/*
 * macros
 */

#define GFX_DEBUG					0
#define GFX_VALIDATE				1
#define GFX_USE_EXISTING_POINTS		1
#define GFX_VERTEX_LOCAL			1
#define PSX_USE_ACTIVE_SUBDIV		0

/* limits - set defaults if not already defined */

/* textures */
#ifndef PSX_MAX_TEXTURES
#define PSX_MAX_TEXTURES					  256
#endif
/* sprites */
#ifndef PSX_MAX_SPRITES
#define PSX_MAX_SPRITES						  256
#endif
/* internal file register */
#ifndef PSX_MAX_INTERNAL_FILE_REGISTER
#define PSX_MAX_INTERNAL_FILE_REGISTER		  128
#endif
/* static (full life) objects */
#ifndef PSX_MAX_OBJECT
#define PSX_MAX_OBJECT				     	  200
#endif
#ifndef PSX_PRIMITIVE_BUFFER_SIZE
#define PSX_PRIMITIVE_BUFFER_SIZE		    40000
#endif
#ifndef PSX_MAX_VERTEX
#define PSX_MAX_VERTEX				  		 9000
#endif
#ifndef PSX_MAX_NORMAL	
#define PSX_MAX_NORMAL				   		 9000	// 4000
#endif
/* file buffer - big enough for a 512x256 16bit image */
#ifndef PSX_FILEBUFFER_SIZE
#define PSX_FILEBUFFER_SIZE				   264000
#endif
/* additional object handlers for rendering the same model <n> times each frame */
#ifndef PSX_MAX_EXTRAHANDLERS
#define PSX_MAX_EXTRAHANDLERS		    	 1000
#endif
/* GPU work area packet space size */
#ifndef PSX_GPU_WORK_AREA_SIZE
#define PSX_GPU_WORK_AREA_SIZE			   131072
#endif
/* size of ordering table: 1 << PSX_OT_LENGTH */ 
#ifndef PSX_OT_LENGTH
#define PSX_OT_LENGTH						   14
#endif

/* end of customisable limits */

/* texture map file location name character length */
#define PSX_MAPNAME_SIZE					   32

/* size of ordering table: 1 << PSX_OT_LENGTH */ 
#define PSX_OT_SIZE						(1<<PSX_OT_LENGTH)

#define PSX_NUM_TEMP_BUFFERS			4
#define PSX_TEMP_BUFFER_SIZE			(PSX_OT_SIZE<<1)
#define PSX_TEMP_SHARED_BUFFER			3

#define PSX_HMD_MAX_TYPES_PER_OBJECT	3		// (use temp buffer!)

/* TV resolution modes */
#define PSX_PAL							MODE_PAL
#define PSX_NTSC						MODE_NTSC

/* TMD header constants */
#define PSX_TMD_VERSION					0x00000041
#define PSX_TMD_HAS_REAL_ADDRESSES		0x00000001
#define PSX_TIM_VERSION					0x00000010

/* graphics initialisation options */
#define PSX_DITHER_ON			1
#define PSX_DITHER_OFF			0
#define	PSX_COLOR_16BIT			0
#define PSX_COLOR_24BIT			1

/* primitive types (code bits) */
#define PSX_POLYGON				1
#define PSX_LINE				2
#define PSX_SPRITE				3

/* object handler attributes */
#define PSX_SUBDIV_NONE			0
#define PSX_SUBDIV_2x2			1
#define PSX_SUBDIV_4x4			2
#define PSX_SUBDIV_8x8			3
#define PSX_SUBDIV_16x16		4
#define PSX_SUBDIV_32x32		5

/* texture modes */
#define PSX_TEXTURE_16BIT		2
#define PSX_TEXTURE_8BIT		1
#define PSX_TEXTURE_4BIT		0

/* transparency modes */
#define PSX_TRANSPARENCY_25		2 
#define PSX_TRANSPARENCY_50		0  
#define PSX_TRANSPARENCY_75		3
#define PSX_TRANSPARENCY_90		1

/* conversion macros (for the PSX fixed decimal format 4096 == 1.0) */
// 91 = 1 degree, 360 degrees = 32768, convert to 4096 = 1.0 (>>3)
#define DEGREES_TO_PSXANGLE(angle)		(((int)angle * 91)>>3)
#define ONE_TO_PSXONE(value)			(value * 4096);

/* fixed point macros (4096 = 1.0f) */
#define FIXED_TO_COLOR(fixed)			((Byte)(fixed>>4))

/* object state */
#define PSX_OBJ_NOTDRAWN		0
#define PSX_OBJ_DRAWN			1

/*
 * typedefs
 */

/* loaded colormap info */
typedef struct {
	void			*data;
	GfxTextureId	 texid;
} PsxColorMap;

/* primitive types */
typedef enum {
	/* quad types */
	PSX_QUAD_FLAT = 1000,
	PSX_QUAD_FLAT_LIT,
	PSX_QUAD_FLAT_TEX,
    PSX_QUAD_FLAT_TEX_LIT,
	PSX_QUAD_GOURAUD_LIT,
    PSX_QUAD_GOURAUD_TEX_LIT,
	/* triangle types */
	PSX_TRI_FLAT,
    PSX_TRI_FLAT_LIT,
	PSX_TRI_FLAT_TEX,
    PSX_TRI_FLAT_TEX_LIT,
	PSX_TRI_GOURAUD_LIT,
	PSX_TRI_GOURAUD_TEX_LIT,
	/* line (unsupported at the moment) */
    PSX_LINE_FLAT,
	/* quad types */
	PSX_HMD_QUAD_FLAT,
	PSX_HMD_QUAD_FLAT_LIT,
	PSX_HMD_QUAD_FLAT_TEX,
    PSX_HMD_QUAD_FLAT_TEX_LIT,
	PSX_HMD_QUAD_GOURAUD_LIT,
    PSX_HMD_QUAD_GOURAUD_TEX_LIT,
	/* triangle types */
	PSX_HMD_TRI_FLAT,
    PSX_HMD_TRI_FLAT_LIT,
	PSX_HMD_TRI_FLAT_TEX,
    PSX_HMD_TRI_FLAT_TEX_LIT,
	PSX_HMD_TRI_GOURAUD_LIT,
	PSX_HMD_TRI_GOURAUD_TEX_LIT,
	/* HMD shared data types */
	PSX_HMD_SHARED_DATA,
	PSX_HMD_SHARED_BLOCK,
	/* HMD Mesh data types */
	PSX_HMD_MESH_GOURAUD_TEX_LIT,
	PSX_HMD_MESH_GOURAUD_TEX_LIT_ENTRY,
} PsxObjType;

/* PSX texture point */
typedef struct {
	Byte	 x;
	Byte	 y;
} PsxTexPoint;

/* TIM format */
typedef struct {
    u_long	 id;
    u_long 	 flag;
    u_long	 zero;
    u_short	 x;
    u_short	 y;
    u_short	 width;
    u_short	 height;
    u_short	 data[2];
} PsxTimData;

/* TMD header */
typedef struct {
    unsigned long	version;
    unsigned long	flags;
    unsigned long	nobjects;
} PsxTmdHeader;

/* TMD object */
typedef struct {
    u_long	*vert_top;
    u_long	 n_vert;
    u_long	*normal_top;
    u_long	 n_normal;
    u_long	*primitive_top;
    u_long	 n_primitive;
    long	 scale;
} PsxTmdObject;

/* PSX mode */
typedef struct {
    u_char	texture_as_is:1,
    		transparency:1,
            has_texture:1,
            quad:1,
			gouraud:1,
    		type:3;
} PsxMode;

/* PSX primitive header */
typedef struct {
    u_char	olen;		/* used by PSX 		*/
    u_char	ilen;		/* length (in words) of data */
    u_char	lightingoff:1,	/* lighting off flag    */
            doublesided:1,  /* double sided polygon */
    		gradation:1,    /* graded color polygon */
            unused:5;    	/* unused part of byte  */
    PsxMode	mode;		/* more primitive info  */
} PsxPrimitive;

/* PSX object attribute */
typedef struct {
    u_long	unused:6,
    		lightingoff:1,
            unused1:2,
            subdivision:3,
            unused2:18,
            transparency:1,
            displayoff:1;
} PsxObjAttribute;

/* PSX vector */
typedef struct {
    long	x; 
    long	y;
    long	z;
    long	unused;
} PsxVector;

/* PSX vertex */
typedef struct {
    short	x;
    short	y;
    short	z;
    short	unused;
} PsxVertex, PsxNormal;

/* PSX positional data */
typedef struct {
    PsxVector	scale;
    PsxVertex	rotate;
    PsxVector   translate;
} PsxPositionInfo;

/* PSX matrix */
typedef MATRIX	PsxMatrix;

/* PSX object position */
typedef struct PsxObjPositionStruct PsxObjPosition;
struct PsxObjPositionStruct {
    u_long			 flag;
    PsxMatrix		 coord;
    PsxMatrix		 work;
    PsxPositionInfo	*info;
    PsxObjPosition 	*parent;
    PsxObjPosition  *unused;
};

/* PSX object handler */
typedef struct {
    PsxObjAttribute	 attribute;
    PsxObjPosition	*position;
    u_long			*model;
    u_long			 id;
} PsxObjHandler;

/* PSX object handle list */
typedef struct PsxObjHandleListStruct PsxObjHandleList;
struct PsxObjHandleListStruct {
	PsxObjHandler					 handler;	
    PsxObjPosition					 position;
	Bool							 linked;
	struct PsxObjHandleListStruct	*next;
};

/* PSX object data */
typedef struct {
    PsxTmdHeader	  header;
    PsxTmdObject	  object;
    u_long	          primitive_size;
    u_long	 	 	  state;
	PsxObjHandleList *current;
	PsxObjHandleList  handle_list;
} PsxObject;

/* PSX color */
typedef struct {
    Byte	red;
    Byte	green;
    Byte	blue;
    Byte	alpha;
} PsxColor;

/* PSX texture info */
typedef struct {
    u_short	id:5,
    		transparency_mode:2,
            texture_mode:2,
            unused:7;
} PsxTextureInfo;

/* PSX Colour Look up table location info */
typedef struct {
    u_short		clx:6,
    			cly:9,
				unused:1;
} PsxClutLocation;

/* PSX line - no gradation */
typedef struct {
    PsxPrimitive 	prim;
    Byte	 		red; 
    Byte			green;
    Byte			blue;
    PsxMode			mode;
    u_short			v0;
    u_short			v1;
} PsxLineFlat;   

/* PSX triangle - flat shaded, no texture, lighting supported */
typedef struct {
    PsxPrimitive 	prim;
    Byte	 		red; 
    Byte			green;
    Byte			blue;
    PsxMode			mode;
    u_short			n0;
    u_short			v0;
    u_short			v1;
    u_short			v2;
} PsxTriFlatLit;

/* PSX triangle - flat shaded, no texture, no lighting */
typedef struct {
    PsxPrimitive 	prim;
    Byte	 		red; 
    Byte			green;
    Byte			blue;
    PsxMode			mode; 
    u_short			v0;
    u_short			v1;
    u_short			v2;
    u_short			unused;
} PsxTriFlat;

/* PSX triangle - gouraud shaded, no texture, lighting supported */
typedef struct {
    PsxPrimitive 	prim;
    Byte	 		red; 
    Byte			green;
    Byte			blue;
    PsxMode			mode;
    u_short			n0;
    u_short			v0;
    u_short			n1;
    u_short			v1;
    u_short			n2;
    u_short			v2;
} PsxTriGourLit;

/* PSX triangle - flat shaded, with texture, lighting supported */
typedef struct {
    PsxPrimitive 	prim;
    Byte			tx0;
    Byte			ty0;
    PsxClutLocation	clut;
    Byte			tx1;
    Byte			ty1;
    PsxTextureInfo	texture; 
    Byte			tx2;
    Byte			ty2;
    u_short			unused;
    u_short			n0;
    u_short			v0;
    u_short			v1;
    u_short			v2;
} PsxTriFlatTexLit;
	 
/* PSX triangle - flat shaded, with texture, no lighting */
typedef struct {
    PsxPrimitive 	prim;
    Byte			tx0;
    Byte			ty0;
    PsxClutLocation	clut;
    Byte			tx1;
    Byte			ty1;
    PsxTextureInfo	texture; 
    Byte			tx2;
    Byte			ty2;
    u_short			unused0;
    Byte	 		red; 
    Byte			green;
    Byte			blue;
	Byte			unused1;
    u_short			v0;
    u_short			v1;
    u_short			v2;
	u_short			unused2;
} PsxTriFlatTex;

/* PSX triangle - gouraud shaded, with texture, lighting supported */
typedef struct {
    PsxPrimitive 	prim;
    Byte			tx0;
    Byte			ty0;
    PsxClutLocation	clut;
    Byte			tx1;
    Byte			ty1;
    PsxTextureInfo	texture; 
    Byte			tx2;
    Byte			ty2;
    u_short			unused;
    u_short			n0;
    u_short			v0;
    u_short			n1;
    u_short			v1;
    u_short			n2;
    u_short			v2;
} PsxTriGourTexLit;

/* PSX quad - flat shaded, no texture, no lighting */
typedef struct {
    PsxPrimitive 	prim;
    Byte	 		red; 
    Byte			green;
    Byte			blue;
    PsxMode			mode;
    u_short			v0;
    u_short			v1;
    u_short			v2;
    u_short			v3;
} PsxQuadFlat;
 
/* PSX quad - flat shaded, with texture, no lighting */
typedef struct {
    PsxPrimitive 	prim;
    Byte			tx0;
    Byte			ty0;
    PsxClutLocation	clut;
    Byte			tx1;
    Byte			ty1;
    PsxTextureInfo	texture;
    Byte			tx2;
    Byte			ty2;
    u_short			unused0;  
    Byte			tx3;
    Byte			ty3;
    u_short			unused1;
    Byte	 		red; 
    Byte			green;
    Byte			blue;
	Byte			unused2;
    u_short			v0;
    u_short			v1;
    u_short			v2;
    u_short			v3;
} PsxQuadFlatTex;

/* PSX quad - flat shaded, no texture, lighting supported */
typedef struct {
    PsxPrimitive 	prim;
    Byte	 		red; 
    Byte			green;
    Byte			blue;
    PsxMode			mode;
    u_short			n0;
    u_short			v0;
    u_short			v1;
    u_short			v2;
    u_short			v3;
    u_short			unused;
} PsxQuadFlatLit;
 
/* PSX quad - flat shaded, with texture, lighting supported */
typedef struct {
    PsxPrimitive 	prim;
    Byte			tx0;
    Byte			ty0;
    PsxClutLocation	clut;
    Byte			tx1;
    Byte			ty1;
    PsxTextureInfo	texture; 
    Byte			tx2;
    Byte			ty2;
    u_short			unused0;  
    Byte			tx3;
    Byte			ty3;
    u_short			unused1;
    u_short			n0;
    u_short			v0;
    u_short			v1;
    u_short			v2;
    u_short			v3;
    u_short			unused2;
} PsxQuadFlatTexLit;

/* PSX quad - gouraud shaded, no texture, lighting supported */
typedef struct {
    PsxPrimitive 	prim;
    Byte	 		red; 
    Byte			green;
    Byte			blue;
    PsxMode			mode;
    u_short			n0;
    u_short			v0;
    u_short			n1;
    u_short			v1;
    u_short			n2;
    u_short			v2;
    u_short			n3;
    u_short			v3;
} PsxQuadGourLit;
 
/* PSX quad - gouraud shaded, with texture, lighting supported */
typedef struct {
    PsxPrimitive 	prim;
    Byte			tx0;
    Byte			ty0;
    PsxClutLocation	clut;
    Byte			tx1;
    Byte			ty1;
    PsxTextureInfo	texture; 
    Byte			tx2;
    Byte			ty2;
    u_short			unused0;  
    Byte			tx3;
    Byte			ty3;
    u_short			unused1;
    u_short			n0;
    u_short			v0;
    u_short			n1;
    u_short			v1;
    u_short			n2;
    u_short			v2;
    u_short			n3;
    u_short			v3;
} PsxQuadGourTexLit;

/* PSX background cell */
typedef struct {
	Byte			xoffset;
	Byte			yoffset;
	u_short			clutid;
	u_short			invert;
	u_short			texpage;
} PsxCell;

/* PSX background map */
typedef struct {
	Byte			cell_width;
	Byte			cell_height;
	u_short			cell_size_x;
	u_short 		cell_size_y;
	PsxCell		   *cells;
	u_short		   *index;
} PsxMap;

/* PSX background hander */
typedef struct {
	unsigned long	attribute;
	short			x, y;
	short			width;
	short			height;
	short			scroll_x;
	short			scroll_y;
	Byte			r, g, b;
	PsxMap		   *map;
	short			origin_x;
	short			origin_y;
	short			scale_x;
	short			scale_y;
	long			angle;
} PsxBackground;

/* sprite */
typedef struct {
	u_long			unused:6,
					nobrightness:1,
					unused2:17,
					format:2,
					unused3:1,
					draw_as_is:1,
					transparency:2,
					transparency_on:1,
					displayoff:1;
	short			x, y;
	u_short			width;
	u_short			height;
	u_short			texpage;
	unsigned char	xoffset;
	unsigned char	yoffset;
	short			clutx;
	short			cluty;
	Byte			r, g, b;
	short			originx;
	short			originy;
	short			scalex;
	short			scaley;
	long			angle;	
} PsxSprite;

/* texture */
typedef struct {
	Text			 name;
	u_short			 x, y;	
    u_char			 texpage;
	u_char			 format;
    u_short			 xoffset;
    u_short			 yoffset;
    u_short			 width;
    u_short			 height;
	GfxColorMapId	 cmapid;
} PsxTexture;

/* file register */
typedef struct {
	char		 filename[67];
	Byte		*location;
} PsxFileRegister;

/* texture map file name location */
typedef struct {
	char		 name[PSX_MAPNAME_SIZE];
	short		 x;
	short		 y;
} PsxMapLocation;

#ifdef PSX_FULL_DEV_KIT
/* HMD format structures */
typedef struct {
	u_long		 size;
	u_long		*poly_data_ptr;
	u_long		*vertex_data_ptr;
	u_long		*normal_data_ptr;
} PsxHmdPrimHdr;

typedef struct {
	u_long		 size;
	u_long		*poly_data_ptr;
	u_long		*vertex_data_ptr;
	u_long		*calc_vertex_data_ptr;
	u_long		*normal_data_ptr;
	u_long		*calc_normal_data_ptr;
} PsxHmdSharedPrimHdr;

typedef struct {
	u_long							*driver;
	u_short							 size;
	u_short							 poly_count;
	u_long							 poly_offset:24,
									 poly_subdiv:8;
} PsxHmdTypeInfo;

typedef struct PsxHmdPrimBlockStruct PsxHmdPrimBlock;
struct PsxHmdPrimBlockStruct {
	struct PsxHmdPrimBlockStruct	*next;
	PsxHmdPrimHdr					*prim_hdr_ptr;
	u_long							 type_count;
	PsxHmdTypeInfo					 type[PSX_HMD_MAX_TYPES_PER_OBJECT];
};

typedef struct {
	//u_long							*ptr_to_next;
	//PsxHmdPrimHdr						*unused_prim_hdr_ptr;
	//u_long							 unused_type_count;
	u_long								*next;
	PsxHmdSharedPrimHdr					*shared_prim_hdr_ptr;
	u_long								 type_count;
	u_long								*driver;
	u_short								 size;
	u_short								 poly_count;
	u_long								 vertex_count;
	u_long								 src_vertex_offset;
	u_long								 dst_vertex_offset;
	u_long								 normal_count;
	u_long								 src_normal_offset;
	u_long								 dst_normal_offset;
} PsxHmdSharedPrimBlock;

/* PSX hmd handler list */
typedef struct PsxHmdHandleListStruct PsxHmdHandleList;
struct PsxHmdHandleListStruct {
	GsUNIT							 unit;
    GsCOORDUNIT						 coord;
	struct PsxHmdHandleListStruct	*next;
};

typedef struct {
	PsxHmdSharedPrimBlock			 block;
	int								 cx,cy,cz;
	int								 parent;
	//int							 nslots;
    //u_long	 					 state;
	PsxHmdHandleList				*current;
	PsxHmdHandleList				 handle_list;
} PsxHmdSharedBlock;

typedef struct {
	PsxHmdSharedPrimHdr				 prim_hdr;
	int								 nblocks;
	int								 current;
	PsxHmdSharedBlock			     blocks[1];
} PsxHmdSharedData;

typedef struct {
	PsxHmdPrimHdr					 prim_hdr;
	PsxHmdPrimBlock					 block;
	PsxHmdSharedData				*shared;
    u_long					         primitive_size;
    u_long	 	 					 state;
	PsxHmdHandleList				*current;
	PsxHmdHandleList				 handle_list;	
    u_long							*vert_top;
    u_long							 n_vert;
    u_long							*normal_top;
    u_long							 n_normal;
    u_long							*primitive_top;
    u_long							 n_primitive;
} PsxHmdObject;

/* PSX triangle - flat shaded, no texture, lighting supported */
typedef struct {
    Byte	 		red; 
    Byte			green;
    Byte			blue;
    Byte			mode;
    u_short			n0;
    u_short			v0;
    u_short			v1;
    u_short			v2;
} PsxHmdTriFlatLit;

/* PSX triangle - flat shaded, no texture, no lighting */
typedef struct {
    Byte	 		red; 
    Byte			green;
    Byte			blue;
    Byte			mode;
    u_short			v0;
    u_short			v1;
    u_short			v2;
    u_short			unused;
} PsxHmdTriFlat;

/* PSX triangle - gouraud shaded, no texture, lighting supported */
typedef struct {
    Byte	 		red; 
    Byte			green;
    Byte			blue;
    Byte			mode;
    u_short			n0;
    u_short			v0;
    u_short			n1;
    u_short			v1;
    u_short			n2;
    u_short			v2;
} PsxHmdTriGourLit;

/* PSX triangle - flat shaded, with texture, lighting supported */
typedef struct {
    Byte			tx0;
    Byte			ty0;
    PsxClutLocation	clut;
    Byte			tx1;
    Byte			ty1;
    PsxTextureInfo	texture; 
    Byte			tx2;
    Byte			ty2;
    u_short			unused;
    u_short			n0;
    u_short			v0;
    u_short			v1;
    u_short			v2;
} PsxHmdTriFlatTexLit;
	 
/* PSX triangle - flat shaded, with texture, no lighting */
typedef struct {	
    Byte	 		red; 
    Byte			green;
    Byte			blue;
    Byte			mode;
    Byte			tx0;
    Byte			ty0;
    PsxClutLocation	clut;
    Byte			tx1;
    Byte			ty1;
    PsxTextureInfo	texture; 
    Byte			tx2;
    Byte			ty2;
    u_short			v0;
    u_short			v1;
    u_short			v2;
} PsxHmdTriFlatTex;

/* PSX triangle - gouraud shaded, with texture, lighting supported */
typedef struct {
    Byte			tx0;
    Byte			ty0;
    PsxClutLocation	clut;
    Byte			tx1;
    Byte			ty1;
    PsxTextureInfo	texture; 
    Byte			tx2;
    Byte			ty2;
    u_short			unused;
    u_short			n0;
    u_short			v0;
    u_short			n1;
    u_short			v1;
    u_short			n2;
    u_short			v2;
} PsxHmdTriGourTexLit;

/* PSX quad - flat shaded, no texture, no lighting */
typedef struct {
    Byte	 		red; 
    Byte			green;
    Byte			blue;
    Byte			mode;
    u_short			v0;
    u_short			v1;
    u_short			v2;
    u_short			v3;
} PsxHmdQuadFlat;
 
/* PSX quad - flat shaded, with texture, no lighting */
typedef struct {
    Byte			tx0;
    Byte			ty0;
    PsxClutLocation	clut;
    Byte			tx1;
    Byte			ty1;
    PsxTextureInfo	texture;
    Byte			tx2;
    Byte			ty2;
    u_short			unused0;  
    Byte			tx3;
    Byte			ty3;
    u_short			unused1;
    Byte	 		red; 
    Byte			green;
    Byte			blue;
	Byte			unused2;
    u_short			v0;
    u_short			v1;
    u_short			v2;
    u_short			v3;
} PsxHmdQuadFlatTex;

/* PSX quad - flat shaded, no texture, lighting supported */
typedef struct {
    Byte	 		red; 
    Byte			green;
    Byte			blue;
    Byte			mode;
    u_short			n0;
    u_short			v0;
    u_short			v1;
    u_short			v2;
    u_short			v3;
    u_short			unused;
} PsxHmdQuadFlatLit;
 
/* PSX quad - flat shaded, with texture, lighting supported */
typedef struct {
    Byte			tx0;
    Byte			ty0;
    PsxClutLocation	clut;
    Byte			tx1;
    Byte			ty1;
    PsxTextureInfo	texture; 
    Byte			tx2;
    Byte			ty2;
    u_short			unused0;  
    Byte			tx3;
    Byte			ty3;
    u_short			unused1;
    u_short			n0;
    u_short			v0;
    u_short			v1;
    u_short			v2;
    u_short			v3;
    u_short			unused2;
} PsxHmdQuadFlatTexLit;

/* PSX quad - gouraud shaded, no texture, lighting supported */
typedef struct {
    Byte	 		red; 
    Byte			green;
    Byte			blue;
    Byte			mode;
    u_short			n0;
    u_short			v0;
    u_short			n1;
    u_short			v1;
    u_short			n2;
    u_short			v2;
    u_short			n3;
    u_short			v3;
} PsxHmdQuadGourLit;
 
/* PSX quad - gouraud shaded, with texture, lighting supported */
typedef struct {
    Byte			tx0;
    Byte			ty0;
    PsxClutLocation	clut;
    Byte			tx1;
    Byte			ty1;
    PsxTextureInfo	texture; 
    Byte			tx2;
    Byte			ty2;
    u_short			unused0;  
    Byte			tx3;
    Byte			ty3;
    u_short			unused1;
    u_short			n0;
    u_short			v0;
    u_short			n1;
    u_short			v1;
    u_short			n2;
    u_short			v2;
    u_short			n3;
    u_short			v3;
} PsxHmdQuadGourTexLit;

/* PSX mesh entry - gouraud shaded, with texture, lighting supported */
typedef struct {
    Byte			tx0;
    Byte			ty0;
    PsxClutLocation	clut;
    Byte			tx1;
    Byte			ty1;
    PsxTextureInfo	texture; 
    Byte			tx2;
    Byte			ty2;
    u_short			unused0;
	u_short			n0;
	u_short			n1;
	u_short			n2;
	u_short			v0;
} PsxHmdMeshGourTexLitEntry;

/*
	Gouraud Texture Mesh
	0x0000001d; DRV(0)|PRIM_TYPE(MESH|IIP|TME); GsUMGT3	H(num); H(0);
	B(u0); B(v0); H(cba);
	B(u1); B(v1); H(tsb);
	B(u2); B(v2); H(0);
	H(norm0); H(vert0);
	H(norm1); H(vert1);
	H(norm2); H(vert2);
	* * *
	B(u1a); B(v1a); H(cba);
	B(u2a); B(v2a); H(tsb);
	B(u3); B(v3); H(0);
	H(norm1); H(norm2);
	H(norm3); H(vert3);
*/

/* PSX mesh - gouraud shaded, with texture, lighting supported */
typedef struct {
	u_short			ntri;
	u_short			unused0;
    Byte			tx0;
    Byte			ty0;
    PsxClutLocation	clut;
    Byte			tx1;
    Byte			ty1;
    PsxTextureInfo	texture; 
    Byte			tx2;
    Byte			ty2;
    u_short			unused1;  
    u_short			n0;
    u_short			v0;
    u_short			n1;
    u_short			v1;
    u_short			n2;
    u_short			v2;
	PsxHmdMeshGourTexLitEntry	next[1];
} PsxHmdMeshGourTexLit;

#endif

/* PSX global info */
typedef struct {
	union {
		struct {
			GsOT	 	worldorder[2];						// ordering table handlers (20x2=40 bytes)
			GsOT_TAG	zsorttable[2][PSX_OT_SIZE];			// ordering tables ((14^2=16384)*4*2=131072 bytes)
		} full;
		struct {
			GsOT	 	worldorder[6];						// ordering table handlers (20x6=120 bytes)
			GsOT_TAG	zsorttable[6][(PSX_OT_SIZE>>2)];	// ordering tables ((14^2=16384)*4*2=131072 bytes)
		} split;
		struct {
			Byte		do_not_touch[120];					// do not touch world order data area
			int			size[PSX_NUM_TEMP_BUFFERS];			// temporary data sizes (4*8=32 bytes)
			Byte		data[PSX_NUM_TEMP_BUFFERS][PSX_TEMP_BUFFER_SIZE]; // temporary work area (when screen not in use) (16384 * 8 = 131072 bytes)
		} temp;
	} screen;
	Bool			screen_lock;							// lock when using temp buffers
	Bool			pak_loaded;								// loaded a file package flag
	Byte			filebuffer[PSX_FILEBUFFER_SIZE];		// file buffer - starts at offset ? (hex=?)
    PACKET	 		gpupacket[2][PSX_GPU_WORK_AREA_SIZE];	// gpu work area
    long	 		mode;									// PAL or NTSC mode
    Bool	 		interlace;								// interlace mode flag
    int 	 		displaypage;							// current display page
    int 	 		drawpage;								// current draw page
    PsxColor 		bkcolor;								// background color
    PsxColor 		color;									// current drawing color
    int 	 		xmax;									// x video resolution
    int 	 		ymax;									// y video resolution
    u_char	 		lightingoff:1,							// current primitive attributes
    		 		textureon:1,							// current primitive attributes
					texture_as_is:1,						// current primitive attributes
					doublesided:1,							// current primitive attributes
					transparency:1,							// current primitive attributes
					gradation:1,							// current primitive attributes
					gouraud:1,								// current primitive attributes
					unused:1;								// current primitive attributes
	char			pad[3];									// padding only
	int				transparency_mode;						// current transparency mode
	PsxMapLocation	texmaps[PSX_MAX_TEXTURES];				// texture map filenames and locations in frame buffer
	int				ntexmaps;								// number of texture maps locations
    GfxTextureId   	textureid;							// currently selected texture id
    u_short	   		texpage;							// current texture page for selected texture id
    u_short	   		pad2;								// more padding
    GfxTextureId	id;									// texture id counter
    PsxTexture     	textures[PSX_MAX_TEXTURES];			// texture data structures
	GfxSpriteId		sid;								// sprite id counter
	PsxSprite		sprites[PSX_MAX_SPRITES];			// sprite data structures
	GfxTextureId	spritetexid[PSX_MAX_SPRITES];		// texture id used for this sprite
#ifdef PSX_USE_HMD_FORMAT
    struct {													
    	GfxObject		 gfxobj[PSX_MAX_OBJECT];				// gfx object
    	PsxHmdObject	 psxobj[PSX_MAX_OBJECT];				// hmd psx object
    	u_long			 primitive[(PSX_PRIMITIVE_BUFFER_SIZE>>2)];	// hmd primitive buffer
    	PsxVertex		 vertex[PSX_MAX_VERTEX];				// hmd vertex buffer
    	PsxNormal		 normal[PSX_MAX_NORMAL];				// hmd normal buffer
    	u_long			 nobjects;								// number of objects
    	u_long			 primitive_size;						// current primitive buffer size in use
        u_long			 n_vert;								// number of points in buffer
        u_long			 n_normal;								// number of normals in buffer
	    GsRVIEWUNIT		 view;									// camera view structure
		PsxHmdHandleList extrahandlers[PSX_MAX_EXTRAHANDLERS];	// extra object handlers
	} hmd;
#else
	struct {													
    	GfxObject		 gfxobj[PSX_MAX_OBJECT];				// gfx object
    	PsxObject		 psxobj[PSX_MAX_OBJECT];				// tmd psx object
    	u_long			 primitive[(PSX_PRIMITIVE_BUFFER_SIZE>>2)];	// tmd primitive buffer
    	PsxVertex		 vertex[PSX_MAX_VERTEX];				// tmd vertex buffer
    	PsxNormal		 normal[PSX_MAX_NORMAL];				// tmd normal buffer
    	u_long			 nobjects;								// number of objects
    	u_long			 primitive_size;						// current primitive buffer size in use
        u_long			 n_vert;								// number of points in buffer
        u_long			 n_normal;								// number of normals in buffer
	    GsRVIEW2		 view;									// camera view structure
		PsxObjHandleList extrahandlers[PSX_MAX_EXTRAHANDLERS];	// extra object handlers
	} tmd;													
#endif
	int		   		 nextrahandlers;					// number of extra object handlers
    unsigned int	 time;								// time in vsync units
	int				 nframes;							// displayed frame counter
	int				 fps;								// number of frames displayed in the last second
	int				 ndraw;								// drawn frame counter
#if 0													// background not supported
	Bool			 has_background;					// background not supported
	PsxBackground	 background;						// background not supported
	PsxMap			 bkmap;								// background not supported
	PsxCell			 cells[PSX_MAX_BACKGROUND_CELLS];	// background not supported
	u_short			 cellinfo[PSX_MAX_BACKGROUND_CELLS];// background not supported
	u_long			*bkwork;							// background not supported
#endif													// background not supported
	char			 fileroot[256];						// file root path
	char			 filepath[128];						// file path relative to root
	PsxFileRegister  fileregister[PSX_MAX_INTERNAL_FILE_REGISTER]; // internal file register
	int				 nregfiles;							// number of registered files
	int				 depth_step;						// depth step for z sorting
	int				 tick;								// tick timer value
	volatile int	 vsync;								// VSync(0) optimisation
	struct {
		Bool		 on;								// split screen enabled flag
		RECT		 clip[3];							// split screen clip regions in display buffer
		int			 offsetx[2];						// split screen drawing x offsets
		int			 offsety[2];						// split screen drawing y offsets
		int			 active_buffer;						// active split screen buffer
	} split_screen;
	struct {
		int				 i;								// temp variables
		void			*void_ptr;						// temp variables
		PsxObject		*psx_obj;						// temp variables
		PsxTmdObject	*psx_tmd_obj;					// temp variables
		PsxVertex		*psx_vertex;					// temp variables
	} tmp;
} PsxData;

/*
 * local prototypes 
 */

extern void gfxError( int code );
extern void psxClearModels( void );
extern void psxAddVertex( GfxObject *gfxobj, Point_i3d pts[], int vcnt, u_short *vindex );
extern void psxAddNormal( GfxObject *gfxobj, Point_fx3d pts[], int ncnt, u_short *nindex );
extern void psxGetTextureLocation( Text name, Text *fullname, int *x, int *y );
extern void psxSetTextureInfo( PsxTextureInfo *texture, PsxClutLocation *clut );
extern PsxTexPoint *psxAdjustTexturePoints( Point_fx2d txy[], int npts );
extern void psxApplyMatrix( GfxObject *object, PsxMatrix *m );
extern void gfxFileInit( void );
extern void gfxFileClose( void );

#endif
