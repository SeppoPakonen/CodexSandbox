/*****************************************************************
 * Project		: ModelR Application
 *****************************************************************
 * File			: model.h
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 9 May 1998
 * Last Update	: 17 May 1998
 *****************************************************************
 * Description	: Modeller application header
 *****************************************************************/

#ifndef THEYER_MODEL_H
#define THEYER_MODEL_H

/*
 * includes
 */

#include <type/datatype.h>
#include <matrix/matrix.h>

/* 
 * macros
 */

#define MOD_X_AXIS				1
#define MOD_Y_AXIS				2
#define MOD_Z_AXIS				3
#define MOD_NEG_X_AXIS			4
#define MOD_NEG_Y_AXIS			5
#define MOD_NEG_Z_AXIS			6

/*
 * typedefs
 */

typedef struct {
	Point_f3d	min;
	Point_f3d	max;
} ModLimits;

typedef struct {
	Bool		 quad;
	int			 pts[4];
	Point_f3d	 normal[2];
	Point_f2d	 texpts[4];
} ModTriangle;

typedef struct {
	int			 pt;
	float		 u;
	float		 v;
} ModUVee;

typedef struct {
	Text		 name;
	int			 r,g,b;
	Bool		 singlesided;
	float		 transparency;
	ModTriangle	*tri;
	int			 ntri;
	int			 maxtri;
	ModUVee		*uvee;
	int			 nuvee;
	int			 maxuvee;
	ModLimits	 limits;	
	Text		 image[2];
	int			 iwidth[2];
	int			 iheight[2];
	void		*idata[2];
	int			 xaxis;
	int			 yaxis;
	Bool		 mirrored;
	int			 mirroraxis;
} ModLayer;

typedef struct ModModelStruct ModModel;

typedef struct {
	Text					 name;
	struct ModModelStruct	*model;
	Point_f3d				 position;
	Point_f3d				 scale;
	Point_f3d				 angle;
} ModChild;

typedef struct {
	Text					 name;
	Text					 pname;
	Point_f3d				 pt;
	int						 parent;
	int						 npts;
} ModJoint;

typedef struct {
	Byte		r;
	Byte		g;
	Byte		b;
	Byte		pad;
} ModRGB;

struct ModModelStruct {
	struct ModModelStruct	*parent;
	Text					 path;
	Text					 name;
	Point_f3d				*pts;
	int						*ptsi;
	int						 npts;
	int						 maxpts;
	ModJoint				*jts;
	int						 njts;
	int						 maxjts;
	Point_f3d				*normals;
	int						 nnormals;
	int						 maxnormals;
	ModRGB					*rgb;
	int						 nrgb;
	int						 maxrgb;
	ModLayer				*layers;
	int						 nlayers;
	int						 maxlayers;
	ModLimits				 limits;
	ModChild				*children;
	int						 nchildren;
	int						 maxchildren;
};

typedef struct {
	Byte		*data;
	int			 width;
	int			 height;
} MibFile;

/*
 * prototypes
 */

/* modcreate.c */
extern ModModel * modCreate( Text name, ModModel *parent );
extern Bool modDestroy( ModModel *model );

/* modfile.c */
extern ModModel * modFileLoad( Text filename );
extern Bool modFileSave( ModModel *model );

/* modpak.c */
extern ModModel * modLoadPak( Byte *data );
extern MibFile *modLoadMibFromPak( Byte *data, Text image, char **errstr );

/* modadd.c */
extern Bool modAddLayer( ModModel *model, Text layer, int r, int g, int b, Bool singlesided, Bool mirrored, int mirroraxis );
extern Bool modAddImage( ModModel *model, Text layer, int i, Text image, int xaxis, int yaxis );
extern Bool modAddImageFromPak( Byte *data, ModModel *model, Text layer, int i, Text image, int xaxis, int yaxis );
extern Bool modAddJoint( ModModel *model, Text name, int x, int y, int z, Text parent, int npts );
extern Bool modAddPoint( ModModel *model, int n, int x, int y, int z );
extern Bool modAddNormal( ModModel *model, int n, float nx, float ny, float nz );
extern Bool modAddGoraud( ModModel *model, int n, Byte r, Byte g, Byte b );
extern Bool modAddTriangle( ModModel *model, Text layer, Bool quad, int p1, int p2, int p3, int p4 );
extern Bool modAddTriangleStrip( ModModel *model, Text layer, Bool npts, int *p );
extern Bool modAddUVee( ModModel *model, Text layer, int pt, float u, float v );
extern Bool modAddTransparency( ModModel *model, Text layer, float trans );
extern Bool modAddChild( ModModel *model, Text childname, Point_f3d position, Point_f3d scale, Point_f3d angle );
	
/* modimage.c */
extern void *modImageLoad( Text name, int *width, int *height );
extern void *modImageLoadFromPak( Byte *data, Text name, int *width, int *height );
extern Bool  modImageDestroy( Text name );

/* modfind.c */
extern ModLayer * modFindLayer( ModModel *model, Text layer );
extern ModJoint * modFindJoint( ModModel *model, Text joint );
extern ModChild * modFindChild( ModModel *model, Text childname );
extern int        modFindPointReference( ModModel *model, int n );

/* modutil.c */
extern float modCuboidSize( ModModel *model, Matrix *mat );
extern int	 modNumChildren( ModModel *model );
extern Bool  modSetName( ModModel *model, Text name );
extern Text  modErrorString( void );
extern void  modSetErrorString( Text str );
extern Text  modAxisString( int axis );
extern Text	 modExtractFilename( Text filename );
extern Text	 modExtractPathname( Text filename );

/* modedit.c */
extern Bool modRename( ModModel *model, Text name );
extern Bool modScale( ModModel *model, float scalex, float scaley, float scalez );
extern Bool modSwapXY( ModModel *model );
extern Bool modTessellate( ModModel *model );
extern Bool modSwapQuadZigZag( ModModel *model );

#endif
