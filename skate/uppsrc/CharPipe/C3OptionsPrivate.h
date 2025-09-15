/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3OptionsPrivate.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/build/charPipeline/c3/include/C3OptionsPrivate.h $
    
    8     11/13/00 1:29p John
    Removed texture filter option since it does not belong within C3
    library (should be controlled from C3 library caller).
    
    7     2/04/00 6:05p John
    Untabified code.
    
    6     1/20/00 1:12p John
    Added stitching (work in progress).
    
    5     1/04/00 6:58p John
    Added little/big endian output  option
    
    4     1/04/00 12:07p John
    Added better support for second texture channel.
    
    3     12/20/99 11:20a John
    Added second texture channel.
    
    2     12/08/99 7:21p John
    Added animation quantization.
    
    6     11/11/99 1:09p John
    Added option to view strip/fan effectiveness.
    
    5     10/13/99 2:55p John
    Changed position quantization back to a power of 2 scale with a given
    ruler.  
    Added lighting with ambient percentage.
    Added default normal table.
    
    4     10/01/99 11:12a John
    Added quantization of positions and tex. coordinates per object.
    
    3     9/29/99 4:20p John
    Changed header to make them all uniform with logs.
    Added position and normal quantization.
    Added external normal table.
    
    2     9/17/99 9:32a John
    Added C3Options and cleaned up code.
    
  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _C3OPTIONSPRIVATE_H
#define _C3OPTIONSPRIVATE_H

#if 0
extern "C" {
#endif
    
// This is meant as a global options structure
typedef struct 
{
    // Reporting options
    C3ReportStatusFunc  reportStatusFunc;
    C3ReportErrorFunc   reportErrorFunc;

    // Extraction options
    u8      vtxOrder;

    C3Bool  enableStitching;
    C3Bool  enableLighting;
    f32     ambientPercentage;

    // Conversion and optimization options
    C3Bool  enableStripFan;
    C3Bool  enableStripFanView;

    u16     compressFlag;

    f32     positionRange;
    u8      positionQuantization;                   // only holds global type info
    u8      texCoordQuantization[C3_MAX_TEXTURES];  // only holds global type info
    u8      keyFrameQuantization;                   // only holds global type info

    u8      colorQuantization;
    u8      colorAlphaQuantization;
    u8      normalQuantization;

    f32     positionWeldRadius;
    f32     texCoordWeldRadius;
    f32     normalWeldRadius;

    // Output options   
    u32     fileExportFlag;

    C3Bool  useDefaultNormalTable;
    C3Bool  useExternalNormalTable;
    char    externalNormalTablePath[ C3_MAX_PATH ];

    u8      outputEndian;

    // Options per object (C3OptionObj)
    DSList  optionObjList;

} C3Options;

#if 0
}
#endif

#endif // _C3OPTIONSPRIVATE_H
