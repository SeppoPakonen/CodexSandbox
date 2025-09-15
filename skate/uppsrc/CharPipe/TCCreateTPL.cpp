/*---------------------------------------------------------------------*

Project:  tc library
File:     TCCreateTPL.cpp

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

 $Log: /Dolphin/build/charPipeline/tc/src/TCCreateTPL.cpp $
    
    2     3/17/00 1:19p Mikepc
    change tc to use indices numbered from 0.
    
    1     12/03/99 3:45p Ryan
    
    16    10/08/99 2:45p Mikepc
    update for tplConv portability: altered data structures, replaced
    references to 'read tga code' with ' *fileFn, removed redundant
    functions.  Changed some file conversion paths.
    
    15    9/16/99 8:47p Mikepc
    updated code for auto-palette generation
    
    14    9/07/99 3:05p Mikepc
    removed default directory restriction from ConvSi2Dds().  tga files can
    now originate in any directory.
    
    13    9/07/99 10:20a Mikepc
    changed a couple of comments to improve clarity.
    
    12    9/02/99 11:12a Mikepc
    some code re-organization between files.
    added code (verify.cpp) to invoke s3tc.exe from within tc program.
    changed some routines to accommodate the new texture creation path.
    
    11    8/26/99 4:59p Mikepc
    renamed file extensions from .c to .cpp.
    .cpp extension allows addition of namespace protection to remove
    potential name collisions with tool code.  Exceptions are CreateTplFile
    and QuickConvert.  These are extern "C" linked.
    
    10    8/26/99 11:38a Mikepc
    
    9     8/26/99 11:03a Mikepc
    tplCon rewrite for efficient memory usage, batch file processing
    ability.
    
  
 $NoKeywords: $

-----------------------------------------------------------------------*/

#include <string.h>

#include <CharPipe/TCCreateTPL.h>
#include <CharPipe/TCCommon.h>

#include "TCScriptFile.h"
#include "TCVerify.h"
#include "TCImageList.h"
#include "TCPaletteList.h"
#include "TCTPLToolbox.h"
#include "TCMem.h"

/*>*******************************(*)*******************************<*/
// wrapper function to perform the major .tpl creation steps
/*>*******************************(*)*******************************<*/
void TCCreateTplFile( char* srcTxtFile, char* dstTplFile )
{
	char* cPtr   = NULL;
    char* tmpTpl = NULL;
    int   len    =    0;


	// read a text file and generate SrcImage, Image, Palette and Texture lists
	TCReadTplTxtFile( srcTxtFile );


	// at this point, all source files have been opened once and header
	// information extracted.
	// SrcImage, Image, Palette and Texture lists have been created.


	// do a 1st pass to verify list info.
	// set image and palette base sizes from SrcImage list
	TCVerifyLists();


	// set image layer values from source files
	// but dont fetch image data yet.
	TCSetImageValues();


	// create or copy palette data from source files,
	// filling in each palette's PalEntry array
	TCSetPalettes();


	// compute all .tpl file, data block sizes; compute all offsets within each .tpl 
	// descriptor block. Information is stored within Image/Palette/Texture structures
	// in ImHead, PlHead, TxHead lists
	TCComputeTplSizes();

    
	// check for a 3-letter extension; if none is present, append one

    len = strlen( dstTplFile ) + 5;  // large enough to add ".tpl" if needed
    TCAssertMsg( (len), "TCCreateTplFile: empty string for .tpl file name\n" );

    tmpTpl = (char*)TCMalloc( len );

    strcpy( tmpTpl, dstTplFile );

	cPtr = tmpTpl + strlen( tmpTpl ) - 4;

    // no 3-letter extension
    if( *cPtr != '.' )
    {
        cPtr += 4;
    }

    // append/overwrite a ".tpl" extension to the filename
    strcpy( cPtr, ".tpl" );

    // create the .tpl file
	TCWriteTplFile( tmpTpl );
	

	// free SrcImage, Image, Palette, Texture lists
	// and all other allocated memory (includes tmpTpl)
	TCFreeMem();

}
	
/*>*******************************(*)*******************************<*/

