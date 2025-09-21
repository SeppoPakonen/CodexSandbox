#ifndef _Game_Core_Math_Math_h_
#define _Game_Core_Math_Math_h_

#if 1
#include "../../Game.h"
#else
// Minimal Core dependencies needed by math headers/impl
#include "../Types.h"
#include "../Defines.h"
#include "../Checks.h"
#include "../Debug.h"
#include "../Support2.h"

// Platform math intrinsics used by legacy NsVector/NsMatrix
#include <Dolphin/dolphin.h>

// Aggregate public math headers in this package
#include "Rot90.h"
#include "MathCore.h"
#include "Vector.h"
#include "Vector2.h"
#include "VectorPair.h"
#include "Matrix.h"
#include "Matrix2.h"
#include "Quat.h"
#include "Quat2.h"
// Inlines are still provided by Core package for now
#include "Rect.h"
#include "BBox.h"
#include "BBox2.h"
#include "Geometry.h"
#endif

#endif
