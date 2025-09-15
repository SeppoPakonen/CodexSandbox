
#ifndef THEYER_GFXPROTO_H
#define THEYER_GFXPROTO_H

#include <type/datatype.h>
#include <compile.h>
#include <gfx/gfxdefs.h>

/* macros */
#define gfxAbs(n)	((n > 0) ? n : -(n))
#define gfxFabs(f)	((f > 0.0f)?(f):((f == 0.0f)?(f):-(f)))

/* PlayStation2 debug break */
#define	ps2BreakPoint()		asm("break")
#define ps2DumpVu1()		gfxPs2MemoryDump( "saves", "vu1mem.dat", GFX_PS2_VU1_MEM, -1 );

/* initialisation stuff */
extern Bool		  gfxInit( GfxInitArgs args[], int nargs );
extern void		  gfxClose( void );
extern void		  gfxAbort( void );
extern int		  gfxGetVideoMode( void );
extern void		  gfxSetResolution( int x, int y );
extern void		  gfxSplitScreen( Bool split, int mode );
extern void		  gfxSelectSplitScreen( int screen );
extern int		  gfxCurrentSplitScreen( void );
extern void		  gfxAdjustScreen( int x, int y, int w, int h );
extern void		  gfxSaveScreen( Text name, int buffer );
extern void		  gfxReset( int level );
/* file services */
extern void		  gfxFileRoot( Text rootpath );
extern void		  gfxFilePath( Text filepath );
extern Bool		  gfxFileExists( Text filename );
extern Byte		 *gfxFileLoad( Text filename );
extern void		  gfxFileSave( Text filename, Byte *data, int size );
extern int		  gfxFileSize( void );
extern void		  gfxFileLocation( Text filename, Byte *location, int size );
/* double buffered graphics functions */
extern void		  gfxBufSwap( void );
extern void		  gfxBufSwapNoClear( void );
extern int		  gfxCurrentBuffer( void );
/* object functinos */
extern GfxObject *gfxBeginObject( Text name, GfxObject *parent, Bool skinned );
extern void		  gfxEndObject( GfxObject *object );
extern void		  gfxDrawObject( GfxObject *object );
extern GfxMatrix  gfxGetObjectMatrix( GfxObject *object );                  
extern int		  gfxGetObjectNumPoints( GfxObject *object );
extern void       gfxSetObjectMatrix( GfxObject *object, GfxMatrix *mat );
extern void       gfxResetMatrix( GfxObject *object );
extern void		  gfxObjectParent( GfxObject *object, GfxObject *parent );
extern void		  gfxSetPivot( GfxObject *object, int pivot );
/* R3 PlayStation 1 functions */
extern void		  gfxDepthStep( int step );
extern void		  gfxLoadMap( Byte *map, Bool clear );
extern void		  gfxObjectSubDivide( GfxObject *object, int subdiv );
extern void		 *gfxMemoryBuffer( int type, int *size );
/* PlayStation2 functions */
extern void		  gfxPs2SetTextureInfo( GfxTextureId id, void *data );
extern void		  gfxPs2AddPivotOffset( GfxObject *object, int joint, int offset );
extern void		  gfxPs2AddPositions( GfxObject *object, void *data, int nframes, int size );
extern int		  gfxPs2AddData( GfxObject *object, void *data, int size, int type );
extern void		  gfxPs2MemoryDump( Text path, Text filename, int mem, int size );
extern void		  gfxPs2CopyToScratchPad( void *data, int size );
/* attribute functions */
extern void		  gfxSetDoubleSided( Bool set );
extern void		  gfxLightingOn( Bool on );
/* timer and frame rate functions */
extern int		  gfxFrameRate( void );
extern void		  gfxResetTick( void );
/* statistics reporting */
extern Text		  gfxUsageDetails( void );
/* colormap */
extern GfxColorMapId gfxLoadColorMap( Text name, unsigned char *data, int size );
/* texture functions */
extern void			 gfxSetTexture( GfxTextureId id );
extern GfxTextureId  gfxLoadTexture( Text name, void *data, GfxTextureFormat format, int width, int height, GfxColorMapId cmapid );
extern void			 gfxScrollTexture( GfxTextureId id, int y );
extern GfxTextureId  gfxTextureId( Text name );
/* sprite functions */
extern GfxSpriteId   gfxSpriteFromTexture( GfxTextureId id );
extern GfxSpriteId   gfxSpriteFromSubTexture( GfxTextureId id, int x, int y, int width, int height );
/* viewpoint functions */
extern void			 gfxSetViewParent( GfxObject *parent );

#ifdef GFX_FIXEDPOINT_API	// fixed point API
/* window clipping */
extern void		 gfxSetClip( fixed x, fixed y, fixed width, fixed height );
/* timer and frame rate functions */
extern fixed	 gfxTick( void );
/* maths and random functions */
extern fixed	 gfxCos( int angle );
extern fixed	 gfxSin( int angle );
extern fixed	 gfxRandom( fixed min, fixed max );
/* add functions */
extern void		 gfxAddPoints( GfxObject *object, Point_i3d pts[], int npts );
extern int		 gfxAddNormals( GfxObject *object, Point_fx3d normals[], int nnormals );
extern void		 gfxAddTri( GfxObject *object, Point_i3d pts[], Point_fx2d txy[], int normals[] );
extern void		 gfxAddQuad( GfxObject *object, Point_i3d pts[], Point_fx2d txy[], int normals[] );
extern int		 gfxAddPivot( GfxObject *object, Point_i3d pt, int parent, int npts );
/* object matrix functions */
extern void		 gfxRotateObject( GfxObject *object, char axis, int angle );
extern void		 gfxTranslateObject( GfxObject *object, int x, int y, int z );
extern void		 gfxScaleObject( GfxObject *object, fixed x, fixed y, fixed z );
extern void		 gfxApplyQuaternion( GfxObject *object, fixed x, fixed y, fixed z, fixed w );
/* query and update functions */
extern Point_i3d gfxGetObjectPoint( GfxObject *object, int v );
extern void      gfxSetObjectPoint( GfxObject *object, int v, Point_i3d *pt );
extern void      gfxSetObjectNormal( GfxObject *object, int n, Point_fx3d *normal );
/* viewpoint functions */
extern void		 gfxSetViewPoint( Point_i3d *eye, Point_i3d *ref, fixed proj );
/* attribute functions */
extern void		 gfxSetColor( fixed red, fixed green, fixed blue );
extern void		 gfxSetBackgroundColor( fixed red, fixed green, fixed blue );
extern void		 gfxSetTransparency( fixed trans );
extern void		 gfxSetFog( fixed factor, fixed red, fixed green, fixed blue );
/* sprite functions */
extern void		 gfxDrawSprite( GfxSpriteId id, fixed depth );
extern void		 gfxSetSpriteOrigin( GfxSpriteId id, fixed originx, fixed originy );
extern void		 gfxSetSpriteRotation( GfxSpriteId id, int angle );
extern void		 gfxSetSpriteScale( GfxSpriteId id, fixed scalex, fixed scaley );
extern void		 gfxSetSpritePosition( GfxSpriteId id, fixed x, fixed y );
//extern void	 gfxSetSpriteWorldPosition( GfxSpriteId id, GfxObject *object, Point_i3d wxyz );
extern void		 gfxSetSpriteBrightness( GfxSpriteId id, fixed br );
extern void		 gfxSetSpriteColor( GfxSpriteId id, fixed r, fixed g, fixed b );
extern void		 gfxSetSpriteTransparency( GfxSpriteId id, fixed trans );
/* lighting */
extern void		 gfxSetFlatLight( int light, fixed x, fixed y, fixed z, fixed red, fixed green, fixed blue ); 
extern void		 gfxSetAmbient( fixed red, fixed green, fixed blue );
#else	// float API follows
/* window clipping */
extern void		 gfxSetClip( float x, float y, float width, float height );
/* timer and frame rate functions */
extern float	  gfxTick( void );
/* maths and random functions */
extern float	 gfxCos( int angle );
extern float	 gfxSin( int angle );
extern float	 gfxRandom( float min, float max );
/* add functions */
extern void		 gfxAddPoints( GfxObject *object, Point_f3d pts[], int npts );
extern int		 gfxAddNormals( GfxObject *object, Point_f3d normals[], int nnormals );
extern void		 gfxAddTri( GfxObject *object, Point_f3d pts[], Point_f2d txy[], int normals[] );
extern void		 gfxAddQuad( GfxObject *object, Point_f3d pts[], Point_f2d txy[], int normals[] );
extern int		 gfxAddPivot( GfxObject *object, Point_f3d pt, int parent, int npts );
/* object matrix functions */
extern void		 gfxRotateObject( GfxObject *object, char axis, float angle );
extern void		 gfxTranslateObject( GfxObject *object, float x, float y, float z );
extern void		 gfxScaleObject( GfxObject *object, float x, float y, float z );
extern void		 gfxApplyQuaternion( GfxObject *object, float x, float y, float z, float w );
/* query and update functions */
extern Point_f3d gfxGetObjectPoint( GfxObject *object, int v );
extern void      gfxSetObjectPoint( GfxObject *object, int v, Point_f3d *pt );
extern void      gfxSetObjectPoints( GfxObject *object, Point_f3d *pts, int npts );
extern void      gfxSetObjectPointsAndRGBAs( GfxObject *object, Point_f3d *pts, GfxColor *col, int npts );
extern void      gfxSetObjectNormal( GfxObject *object, int n, Point_f3d *normal );
extern void      gfxSetObjectNormals( GfxObject *object, Point_f3d *normals, int nnormals );
extern void      gfxSetObjectRGBAs( GfxObject *object, GfxColor *col, int ncol );
extern void		 gfxScrollObjectTextures( GfxObject *object, float x, float y );
/* viewpoint functions */
extern void		 gfxSetViewPoint( Point_f3d *eye, Point_f3d *ref, float proj );
extern void		 gfxSetCamera( Point_f3d *eye, float angle, float pitch, float dist );
/* attribute functions */
extern void		 gfxSetColor( float red, float green, float blue );
extern void		 gfxSetBackgroundColor( float red, float green, float blue );
extern void		 gfxSetTransparency( float trans );
extern void		 gfxSetFog( float factor, float red, float green, float blue );
/* sprite functions */
extern void		 gfxDrawSprite( GfxSpriteId id, float depth );
extern void		 gfxSetSpriteOrigin( GfxSpriteId id, float originx, float originy );
extern void		 gfxSetSpriteScale( GfxSpriteId, float scalex, float scaley );
extern void		 gfxSetSpritePosition( GfxSpriteId id, float x, float y );
//extern void	 gfxSetSpriteWorldPosition( GfxSpriteId id, GfxObject *object, Point_f3d wxyz );
extern void		 gfxSetSpriteBrightness( GfxSpriteId id, float br );
extern void		 gfxSetSpriteColor( GfxSpriteId id, float r, float g, float b );
extern void		 gfxSetSpriteTransparency( GfxSpriteId id, float trans );
/* lighting */
extern void		 gfxSetFlatLight( int light, float x, float y, float z, float red, float green, float blue ); 
extern void		 gfxSetAmbient( float red, float green, float blue );
/* animation */
extern void		 gfxSetPosition( GfxObject **object, int nobjects, int source, int target, float interpol );
#endif	// end float API

#if 0
/* unsupported */
extern void		 gfxStartTimer( void );
extern float	 gfxStopTimer( void );
extern Point_f2d gfxWorldToScreen( Point_f3d *pt );
extern void		 gfxDrawLine( Point_f3d *start, Point_f3d *end );
extern void		 gfxAddLine( GfxObject *object, Point_f3d pts[] );
extern void		 gfxSetBackground( float x, float y, float width, float height, int tiles_x, int tiles_y, GfxTextureId *ids );
extern void		 gfxScrollBackground( float x, float y );
extern void		 gfxDrawBackground( void );

/* superceded - not allowed in code anymore... */
#ifdef OsTypeIsPsx
#define gfxPrintf		FntPrint
#define gfxFontFlush()	FntFlush(-1)
#else
#define gfxPrintf		printf
#define	gfxFontFlush()	;
#endif
#endif

#endif	// THEYER_GFXPROTO_H

