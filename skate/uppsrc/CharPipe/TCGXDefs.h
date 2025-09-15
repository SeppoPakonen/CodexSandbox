/*---------------------------------------------------------------------*

Project:  tc library
File:     TCGXDefs.h

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

   $Log: /Dolphin/build/charPipeline/tc/include/TCGXDefs.h $
    
    1     12/03/99 3:44p Ryan
   
  $NoKeywords: $

-----------------------------------------------------------------------*/

#ifndef __TCGXDEFS_H__
#define __TCGXDEFS_H__

/********************************/
#include<Dolphin/gxEnum.h>

/********************************/
// .tpl file image texel formats
// mapped to GX_TF formats from gxEnum.h    
#define TPL_IMAGE_TEXEL_FMT_I4            GX_TF_I4  
#define TPL_IMAGE_TEXEL_FMT_I8            GX_TF_I8

#define TPL_IMAGE_TEXEL_FMT_IA4           GX_TF_IA4
#define TPL_IMAGE_TEXEL_FMT_IA8           GX_TF_IA8

#define TPL_IMAGE_TEXEL_FMT_CI4           GX_TF_C4
#define TPL_IMAGE_TEXEL_FMT_CI8           GX_TF_C8
#define TPL_IMAGE_TEXEL_FMT_CI14_X2       GX_TF_C14X2

#define TPL_IMAGE_TEXEL_FMT_R5G6B5        GX_TF_RGB565

#define TPL_IMAGE_TEXEL_FMT_RGB5A3        GX_TF_RGB5A3
#define TPL_IMAGE_TEXEL_FMT_RGBA8         GX_TF_RGBA8

#define TPL_IMAGE_TEXEL_FMT_CMP           GX_TF_CMPR  


// wrap, clamp, mirror modes
#define TPL_WRAP_MODE_CLAMP			      GX_CLAMP
#define TPL_WRAP_MODE_REPEAT              GX_REPEAT
#define TPL_WRAP_MODE_MIRROR              GX_MIRROR


// filtering modes
#define TPL_TEX_FILTER_NEAR               GX_NEAR
#define TPL_TEX_FILTER_LINEAR             GX_LINEAR
#define TPL_TEX_FILTER_NEAR_MIP_NEAR      GX_NEAR_MIP_NEAR
#define TPL_TEX_FILTER_LIN_MIP_NEAR       GX_LIN_MIP_NEAR
#define TPL_TEX_FILTER_NEAR_MIP_LIN       GX_NEAR_MIP_LIN
#define TPL_TEX_FILTER_LIN_MIP_LIN        GX_LIN_MIP_LIN

// possible .tpl palette entry formats
#define TPL_PALETTE_ENTRY_FMT_R5G6B5       GX_TL_RGB565   
#define TPL_PALETTE_ENTRY_FMT_RGB5A3       GX_TL_RGB5A3

/********************************/

#endif  // __TCGXDEFS_H__