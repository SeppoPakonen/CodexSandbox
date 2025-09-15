#ifndef _Graphics_Primitives_h_
#define _Graphics_Primitives_h_

NAMESPACE_GFX_BEGIN

#ifdef flagHAVE_DIRECT_OPENGL
typedef unsigned char	Bool;
#else
typedef bool			Bool;
#endif

typedef float			Float;
typedef float			Clamp;
typedef int				Int;
typedef unsigned int	Uint;
typedef unsigned char	Byte;
typedef void			Void;




NAMESPACE_GFX_END

typedef Gfx::Float		Gxf;
typedef Gfx::Bool		Gxb;
typedef Gfx::Byte		Gxby;
typedef Gfx::Int		Gxi;
typedef Gfx::Uint		Gxu;
typedef Gfx::Uint		Gxbf;


#endif
