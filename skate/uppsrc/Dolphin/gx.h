/*---------------------------------------------------------------------------*
  Project:  Dolphin GX library
  File:     gx.h

  Copyright 1998-2001 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/dolphin/gx.h $
    
    9     5/22/01 8:09p Carl
    Changed order for GXDispList.h
    
    8     4/13/01 6:51p John
    Substituted MAC for MAC2 build.
    
    7     11/14/00 11:33p Hirose
    added stubs for GXVerify stuff of NDEBUG build
    
    6     10/05/00 11:49a Hirose
    modified build flag definition scheme
    
    5     9/30/00 5:27p Hirose
    added revision number flag test-and-set
    
    4     6/08/00 1:59p Carl
    Added GXCpu2Efb.h
    
    3     4/17/00 4:24p Alligator
    integrate ArtX update.  Perf monitor api and indirect texture api
    added. Misc bug fixes.
    
    2     2/01/00 7:22p Alligator
    second GX update from ArtX
    
    17    10/06/99 2:25a Yasu
    Add GXCpu2Efb.h in comment
    
    16    9/03/99 2:43p Ryan
    remover GXInit.h
    
    15    8/30/99 11:06a Ryan
    added GXinit.h
    
    14    7/16/99 1:05p Alligator
    use GXDraw functions
    
    13    7/14/99 9:23p Yoshya01
    Move vertex API definitions to GXVert.h.
    
    12    7/14/99 4:21p Alligator
    Split gx.h into separate header file for each major section of the API
    
    11    7/13/99 2:21p Alligator
    added some EPPC-specific enumerations, will be used to test EPPC GX
    library.
    
    10    7/09/99 4:58p Hirose
    
    9     7/06/99 5:34p Ryan
    
    8     7/06/99 12:03p Ryan
    Added GX_TRUE, GX_FALSE, GX_ENABLE, GX_DISABLE
    
    6     7/02/99 12:59p Ryan
    changed Mtx row/col
    
    5     6/30/99 1:27p Ryan
    
    4     6/07/99 8:27p Alligator
    changed Ptr to void *
    
    3     6/04/99 2:47p Ryan
    parameter order changes in SetViewport and SetScissor
    
    2     6/03/99 3:17p Ryan
    
    1     6/01/99 2:04p Ryan

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GX_H__
#define __GX_H__



#include "Defs.h"

#if 0
extern "C" {
#endif

/*---------------------------------------------------------------------------*/

// If the compiler flag GX_REV is not defined,
// set the latest revision number
#ifndef GX_REV

//#if ( defined(HW2) || defined(MAC) )
#define GX_REV  2
//#else  // other targets
//#define GX_REV  1
//#endif // ( defined(HW2) || defined(MAC) )

#endif // GX_REV
    
/*---------------------------------------------------------------------------*
    header file for each major section of the API
 *---------------------------------------------------------------------------*/
#include "types.h"
#include "GXEnum.h"
#include "GXStruct.h"
#include "GXGeometry.h"
#include "GXTransform.h"
#include "GXCull.h"
#include "GXLighting.h"
#include "GXTexture.h"
#include "GXTev.h"
#include "GXBump.h"
#include "GXPixel.h"
#include "GXFrameBuffer.h"
#include "GXCpu2Efb.h"
#include "GXPerf.h"
#include "GXFifo.h"
#include "GXManage.h"
#include "GXGet.h"
#include "GXDraw.h"
#include "GXVert.h"
#include "GXCommandList.h"
#include "GXDispList.h"
#include "GXVerify.h"
/*---------------------------------------------------------------------------*/

#if 0
}
#endif

#endif // __GX_H__
