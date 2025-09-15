/*---------------------------------------------------------------------*

Project:  tc library
File:     TCImageList.h

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

   $Log: /Dolphin/build/charPipeline/tc/include/TCImageList.h $
    
    2     3/17/00 1:15p Mikepc
    change tc to use indices numbered from 0:
    added TCFindImagePos prototype.
    
    1     12/03/99 3:44p Ryan
    
    10    10/08/99 2:44p Mikepc
    update for tplConv portability: altered data structures, replaced
    references to 'read tga code' with ' *fileFn, removed redundant
    functions.
    
    9     9/17/99 12:25p Mikepc
    arbitrary palette size is gone- took RemapColors() from reduceColor.cpp
    ( file is now removed), renamed it to RemapImageColors and placed it
    here.
    
    8     9/16/99 8:48p Mikepc
    
    7     8/26/99 4:56p Mikepc
    added namespace protection to remove potential name collisions with
    tool code.  Exceptions are CreateTplFile and QuickConvert.  These are
    extern "C" linked.
    
    6     8/26/99 11:37a Mikepc
    
    5     8/26/99 11:01a Mikepc
    tplConv rewrite for memory usage efficiency, batch file processing
    ability.
    
  
  $NoKeywords: $

-----------------------------------------------------------------------*/


#ifndef __TCIMAGELIST_H__
#define __TCIMAGELIST_H__


/********************************/
#include <CharPipe/TCLayer.h>

/********************************/
typedef struct TCImage
{
    u32 index;              // image index from script file.  This is also the .tpl bank location
	 
	u32 colorSrcImage;      // srcImage index for color layer
	u32 alphaSrcImage;      // srcImage index for alpha layer; 0 if no alpha layer
	
    u32 texelFormat;        // Dolphin texture format from script file

	u32 minLOD;             // minimum LOD level desired relative to srcImage
	u32 maxLOD;             // maximum LOD level desired relative to srcImage
	u32 remapMinLOD;        // remapped (actual) minLOD for .tpl file

    //-------------------

	TCLayer lyColor;        // color layer
	TCLayer lyAlpha;        // alpha layer

    u32 tplImageBankOffset;	// bytes from top of .tpl file to image texel data
    u32 tplBufferSize;      // size of image buffer including padding to 32B tiles

	struct TCImage* prev;
	struct TCImage* next;
	
}TCImage, *TCImagePtr;

/********************************/
extern TCImage* ImHead;

/*>*******************************(*)*******************************<*/
void     TCSetImageValues	   ( void );
TCImage* TCNewImage			   ( void );
void     TCSortImageByIndex	   ( void );
void     TCCopyImage		   ( TCImage* src,  TCImage* dst );
TCImage* TCFindImageByIndex	   ( u32      index              );
void     TCSetImageIndex	   ( TCImage* im,   u32 index    );                        

void     TCSetImageLayerAtt	   ( TCImage* im,   u32 colorLayer, u32 alphaLayer );

void     TCSetImageTexelFormat ( TCImage* im,   u32 texelFmt );

void     TCSetImageMipMap	   ( TCImage* im,   u32 minLOD, u32 maxLOD, u32 baseLOD );

u32      TCFindImagePos        ( TCImage* im );

/*>*******************************(*)*******************************<*/

#endif  // __TCIMAGELIST_H__