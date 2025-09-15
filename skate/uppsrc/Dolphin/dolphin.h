/*---------------------------------------------------------------------------*
  Project:  Dolphin OS
  File:     dolphin.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/dolphin.h $
    
    5     12/06/00 4:27p Billyjack
    added ax.h
    
    4     8/24/00 5:29p Shiki
    Included "card.h".

    3     6/06/00 9:25p Eugene
    Added header files for AI, AR, ARQ, and DSP drivers.

    2     12/23/99 4:05p Tian
    Added PPCArch.h

    5     7/23/99 12:11p Ryan
    removed demo.h

    4     7/20/99 6:03p Alligator
    demo library

    3     6/07/99 4:34p Alligator
    add vi.h

    2     6/03/99 7:43p Tianli01
    Added GX and MTX

    1     5/11/99 4:41p Shiki

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __DOLPHIN_H__
#define __DOLPHIN_H__




//#define GX_REV 2

#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <iostream>
#include <stdint.h>

#ifdef flagWIN32
	#include <windows.h>
#endif

#ifdef flagPOSIX
	#include <unistd.h>
	#include <errno.h>
	#include <sys/stat.h>
#endif


#include <Graphics/Graphics.h>


#undef min
#undef max


#include "Defs.h"

#include "Fifo.h"

#include "GXEnum.h"

#include "PPCArch.h"
#include "db.h"
#include "os.h"
#include "dvd.h"
#include "pad.h"
#include "mtx.h"
#include "gx.h"
#include "vi.h"
#include "ai.h"
#include "ar.h"
#include "ax.h"
#include "arq.h"
#include "dsp.h"
#include "card.h"

#include "hio.h"
#include "GDBase.h"
#include "GDTev.h"
#include "GDLight.h"
#include "GDGeometry.h"
#include "GDTexture.h"
#include "GDPixel.h"
#include "GDTransform.h"
#include "GXTransform.h"



#include "Compat.h"



#endif


#ifndef __SDKVER_SYMBOL__
#define __SDKVER_SYMBOL__

#define __SDKVER__ "12Dec2001"

#endif
