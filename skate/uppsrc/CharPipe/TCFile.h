/*---------------------------------------------------------------------*

Project:  tc library
File:     TCFile.h

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

   $Log: /Dolphin/include/charPipeline/tc/TCFile.h $
    
    1     12/03/99 4:15p Ryan
   
  $NoKeywords: $

-----------------------------------------------------------------------*/

#ifndef __TCFILE_H__
#define __TCFILE_H__

/********************************/
#include <Dolphin/types.h>

#include <CharPipe/TCLayer.h>
#include <CharPipe/TCPalTable.h>

/********************************/
typedef struct
{
	char      name[255];

	TCLayer*    lyColor;
	TCLayer*    lyAlpha;
  
	TCPalTable* palPtr;
	
} TCFile, *TCFilePtr;

/*>*******************************(*)*******************************<*/
void TCSetFileCacheSize	( u32 size );

void TCInstallFileReadFn( char* ext, u32 (*fileFn)( u32 rawSize, 
						  u8* rawBits, TCFile* fPtr ) );

/*>*******************************(*)*******************************<*/

#endif  // __TCFILE_H__