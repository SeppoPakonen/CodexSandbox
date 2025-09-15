#ifndef _Graphics_Graphics_h_
#define _Graphics_Graphics_h_


#define NAMESPACE_GFX_BEGIN namespace Gfx {
#define NAMESPACE_GFX_END }
#define NAMESPACE_GFXSTATIC_BEGIN namespace Gfx::Static {
#define NAMESPACE_GFXSTATIC_END }


#include "Primitives.h"
#include "Enums.h"

//#define flagHAVE_DIRECT_OPENGL

#ifdef flagHAVE_DIRECT_OPENGL


#include "OpenGL.h"
#include <Core/Core.h>


#else

#include <Core/Core.h>



#include "GlobalApi.h"



#endif

#endif
