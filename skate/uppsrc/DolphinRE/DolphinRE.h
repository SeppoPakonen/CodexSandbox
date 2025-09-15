#ifndef _DolphinRE_DolphinRE_h_
#define _DolphinRE_DolphinRE_h_

//#include <stdint.h>
#include <assert.h>
#include <math.h>

#include <algorithm>

#include <Dolphin/dolphin.h>

using namespace Gfx;
using namespace Gfx::Static;

#ifdef flagWIN32
extern PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
extern PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;
extern PFNGLMULTITEXCOORD4FARBPROC glMultiTexCoord4fARB;
#endif

#include "Translate.h"
#include "TransMtx.h"

#include "ClipCull.h"
#include "DispList.h"
#include "EmVert.h"
#include "FrameBuf.h"
#include "GeoVert.h"
#include "Light.h"
#include "Misc.h"
#include "TexConv.h"
#include "PixlProc.h"
#include "TexEnv.h"
#include "Texture.h"
#include "Texture2.h"
#include "Draw.h"
#include "FrameBuf2.h"
#include "Init.h"
#include "Transform.h"
#include "Mtx.h"

#endif
