/*---------------------------------------------------------------------------*
  Project:  Dolphin GD library
  File:     gd.h

  Copyright 2001 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/dolphin/gd.h $
    
    2     9/19/01 3:55p Carl
    More win32 compatibility defines.
    
    1     9/12/01 1:53p Carl
    Initial revision of GD: Graphics Display List Library.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __GD_H__
#define __GD_H__

/*---------------------------------------------------------------------------*/

#ifdef flagWIN32
#define inline __inline
#define cosf   (float)cos
#define sqrtf  (float)sqrt
#endif

/*---------------------------------------------------------------------------*/

#include "GDBase.h"
#include "GDFile.h"
#include "GDGeometry.h"
#include "GDLight.h"
#include "GDPixel.h"
#include "GDTev.h"
#include "GDTexture.h"
#include "GDTransform.h"
#include "GDVert.h"

/*---------------------------------------------------------------------------*/

#endif // __GD_H__
