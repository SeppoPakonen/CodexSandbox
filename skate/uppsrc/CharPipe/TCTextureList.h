/*---------------------------------------------------------------------*

Project:  tc library
File:     TCTextureList.h

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

   $Log: /Dolphin/build/charPipeline/tc/include/TCTextureList.h $
    
    3     3/17/00 1:18p Mikepc
    
    2     3/17/00 1:17p Mikepc
    change tc to use indices numbered from 0:
    
    1     12/03/99 3:44p Ryan
    
    6     8/26/99 4:56p Mikepc
    added namespace protection to remove potential name collisions with
    tool code.  Exceptions are CreateTplFile and QuickConvert.  These are
    extern "C" linked.
    
    5     8/26/99 11:37a Mikepc
    
    4     8/26/99 11:01a Mikepc
    tplConv rewrite for memory usage efficiency, batch file processing
    ability.
    
  
  $NoKeywords: $

-----------------------------------------------------------------------*/

#ifndef __TCTEXTURELIST_H__
#define __TCTEXTURELIST_H__

/********************************/
#include <Dolphin/types.h>

/********************************/
typedef struct TCTexture
{
    u32 index;              // .tpl Texture Descriptor index

	u32 image;              // indices of member components.  0 indicates no component present
	u32 palette;

    //-----------------

    u32 tplImageOffset;     // offset from top of file to Image Desriptor in .tpl file
    u32 tplPaletteOffset;	// offset from top of file to Palette Descriptor in .tpl file 
 
	struct TCTexture* prev;
	struct TCTexture* next;

}TCTexture, *TCTexturePtr; 

/********************************/
extern TCTexture* TxHead;

/*>*******************************(*)*******************************<*/
TCTexture* TCNewTexture			 ( void );
void       TCSetTextureAttributes( TCTexture* tx, u32 index, u32 image, u32 palette );

void TCSortTextureByIndex( void );

/*>*******************************(*)*******************************<*/

#endif  // __TCTEXTURELIST_H__
