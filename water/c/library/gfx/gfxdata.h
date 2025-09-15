
#ifndef THEYER_GFXDATA_H
#define THEYER_GFXDATA_H

#include <compile.h>
#include <text/text.h>
#include <type/datatype.h>

typedef struct {
    Byte		r,g,b,a;
} GfxColor;

typedef struct {
    int		 arg;
    void	*data;
} GfxInitArgs;

typedef enum {
    GFX_VIEW_MATRIX,
    GFX_MODEL_MATRIX
} GfxMatrixMode;

typedef struct {
	Text	 name;
    Bool	 dynamic;
    int		 id;
    void	*data;
} GfxObject;

typedef struct {
	Byte		data[64];
} GfxMatrix;

typedef struct {
	Text			 name;
	unsigned short	 x, y;
} GfxPsxTexInfo;

typedef struct {
	GfxPsxTexInfo	*texinfo;
	int				 ntexinfo;
} GfxPsxInfo;

typedef int				GfxTextureId;
typedef int				GfxSpriteId;
typedef GfxTextureId	GfxColorMapId;
typedef int				GfxTextureFormat;

#endif
