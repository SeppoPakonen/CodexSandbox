/*---------------------------------------------------------------------*

Project:  tc library
File:     TCTPLToolbox.h

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:


   $Log: /Dolphin/build/charPipeline/tc/include/TCTPLToolbox.h $
    
    1     12/03/99 3:44p Ryan
    
    2     11/23/99 2:52p Mikepc
    changed WriteTplImageBank, WriteTplPaletteBankto include partial update
    flags
    
    14    10/14/99 5:47p Mikepc
    increased TPL_HDR_SIZE to 12B ( was 8 ) for new version field.
    
    13    9/16/99 8:48p Mikepc
    
    12    8/26/99 4:56p Mikepc
    added namespace protection to remove potential name collisions with
    tool code.  Exceptions are CreateTplFile and QuickConvert.  These are
    extern "C" linked.
    
    11    8/26/99 11:37a Mikepc
    
    10    8/26/99 11:01a Mikepc
    tplConv rewrite for memory usage efficiency, batch file processing
    ability.
    
  
  $NoKeywords: $

-----------------------------------------------------------------------*/

#ifndef __TCTPLTOOLBOX_H__
#define __TCTPLTOOLBOX_H__

/********************************/
#include <stdio.h>

#include <CharPipe/TCLayer.h>

#include "TCImageList.h"
#include "TCPaletteList.h"

/*>*******************************(*)*******************************<*/
void TCComputeTplSizes			( void );
void TCWriteTplFile				( char* tplFile );

u32 TCComputeTplImageBufferSize	( TCImage* thisImage );

// write images as tiles
void TCWriteTplImage_I4		( TCLayer* colorLayer, u8* tplBuffer );
void TCWriteTplImage_I8		( TCLayer* colorLayer, u8* tplBuffer );

void TCWriteTplImage_IA4	( TCLayer* colorLayer, TCLayer* alphaLayer, 
							  u8* tplBuffer );

void TCWriteTplImage_IA8	( TCLayer* colorLayer, TCLayer* alphaLayer, 
							  u8* tplBuffer );

void TCWriteTplImage_R5G6B5	( TCLayer* colorLayer, u8* tplBuffer );

void TCWriteTplImage_RGB5A3	( TCLayer* colorLayer, TCLayer* alphaLayer, 
							  u8* tplBuffer );
                                                                                                                      
void TCWriteTplImage_RGBA8	( TCLayer* colorLayer, TCLayer* alphaLayer, 
							  u8* tplBuffer );  
                              
void TCWriteTplImage_CI4	( TCLayer* colorLayer, u8* tplBuffer );
void TCWriteTplImage_CI8	( TCLayer* colorLayer, u8* tplBuffer );                               
void TCWriteTplImage_CI14_X2( TCLayer* colorLayer, u8* tplBuffer );
void TCWriteTplImage_CMP	( TCLayer* colorLayer, u8* tplBuffer );

#endif  // __TCTPLTOOLBOX_H__
