/*---------------------------------------------------------------------*

Project:  tc library
File:     TCPaletteList.h

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

   $Log: /Dolphin/build/charPipeline/tc/include/TCPaletteList.h $
    
    2     3/17/00 1:16p Mikepc
    change tc to use indices numbered from 0:
    add TCFindPalettePos prototype.
    
    1     12/03/99 3:44p Ryan
    
    9     10/08/99 2:44p Mikepc
    update for tplConv portability: altered data structures, replaced
    references to 'read tga code' with ' *fileFn, removed redundant
    functions.
    
    8     9/17/99 12:24p Mikepc
    arbitrary palette size is gone- removed CreatePalette() prototype
    
    7     9/16/99 8:48p Mikepc
    
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

#ifndef __TCPALETTELIST_H__
#define __TCPALETTELIST_H__

/********************************/
#include <CharPipe/TCPalTable.h>

/********************************/
typedef struct TCPalette
{
     u32 index;                 // index from script file.  This is also the .tpl bank location
	 
	 u32 srcImage;              // index of SrcImage providing
	                            // both color and alpha components of CLUT

     u32 entryFormat;           // Dolphin format of CLUT entries

     //-------------------

	 TCPalTable* palPtr;        // array of palette entries 


     u32 tplPaletteBankOffset;	// number of bytes from top of file to palette data block    
     u32 tplBufferSize;         // size of palette data block

	 struct TCPalette* prev;
	 struct TCPalette* next;

     //-------------------

}TCPalette, *TCPalettePtr;

/********************************/
extern TCPalette* PlHead;

/*>*******************************(*)*******************************<*/

void       TCSetPalettes		   ( void );
TCPalette* TCNewPalette			   ( void );
void       TCSetPaletteIndex	   ( TCPalette* pl, u32 index );
void       TCSetPaletteSrcImage	   ( TCPalette* pl, u32 srcImage );
void       TCSetPaletteEntryFormat ( TCPalette* pl, u32 entryFmt );

void       TCSortPaletteByIndex( void );
TCPalette* TCFindPaletteByIndex( u32        index ); 
u32        TCFindPalettePos(     TCPalette* pl    );

/*>*******************************(*)*******************************<*/

#endif  // __TCPALETTELIST_H__

