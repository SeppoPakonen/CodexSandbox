/*---------------------------------------------------------------------*

Project:  tc library
File:     TCCommon.cpp

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

 $Log: /Dolphin/build/charPipeline/tc/src/TCCommon.cpp $
    
    4     8/10/00 6:01p Mikepc
    removed redundant #include<assert.h>, 
    changed any remaining asserts to TCAssertMsg
    
    3     3/17/00 1:19p Mikepc
    change tc to use indices numbered from 0.
    
    2     12/06/99 1:32p Mikepc
    removed #ifdef INTRACTIVE from ErrorMsg() code so the user always sees
    the error message before the program quits.
    
    1     12/03/99 3:45p Ryan
    
    14    11/16/99 12:01p Yasu
    Add #ifdef INTERACTIVE
    
    13    11/16/99 9:41a Yasu
    Delete waiting key input in ErrorMsg
    
    12    10/12/99 8:00p Mikepc
    changed include paths to vcc (tools/options) relative.
    
    11    10/08/99 2:45p Mikepc
    update for tplConv portability: altered data structures, replaced
    references to 'read tga code' with ' *fileFn, removed redundant
    functions.  Changed some file conversion paths.
    
    10    9/16/99 8:47p Mikepc
    updated code for auto-palette generation
    
    9     8/26/99 4:59p Mikepc
    renamed file extensions from .c to .cpp.
    .cpp extension allows addition of namespace protection to remove
    potential name collisions with tool code.  Exceptions are CreateTplFile
    and QuickConvert.  These are extern "C" linked.
    
    8     8/26/99 11:38a Mikepc
    
    7     8/26/99 11:03a Mikepc
    tplCon rewrite for efficient memory usage, batch file processing
    ability.
    
  
 $NoKeywords: $

-----------------------------------------------------------------------*/


// common functions referenced by a number of source files

#include <stdio.h>
#include <stdlib.h>

#include <CharPipe/TCCommon.h>

#include "TCImageList.h"
#include "TCPaletteList.h"
#include "TCSrcImageList.h"
#include "TCTextureList.h"
#include "TCMem.h"

/*>*******************************(*)*******************************<*/
void TCErrorMsg( char* msg, ... )
{
	va_list argPtr;
	char dummy[255];


	printf("error:\n");

	va_start( argPtr, msg    );
	vprintf(  msg,    argPtr );
	va_end(   argPtr         );

	// free all of the app's allocated memory
	TCFreeMem();

	// give the user a chance to read the error message
	printf( "press <enter> to quit\n" );
	gets( dummy );

	exit(1);
}

/*>*******************************(*)*******************************<*/
// if 'test' is false, 
// print a 'printf' style message, free all of the app's allocated
// memory and quit the program.
/*>*******************************(*)*******************************<*/
void TCAssertMsg( int exp, char* msg, ... )
{
	va_list argPtr;
	char    dummy[255];


    if( !exp )
    {
    	printf("error:\n");

    	va_start( argPtr, msg    );
    	vprintf(  msg,    argPtr );
    	va_end(   argPtr         );

    	// free all of the app's allocated memory
    	TCFreeMem();

    	// give the user a chance to read the error message
    	printf( "press <enter> to quit\n" );
    	gets( dummy );

    	exit(1);
    }
}

/*>*******************************(*)*******************************<*/
// transform all characters of a string to uppercase
/*>*******************************(*)*******************************<*/
void TCStrToUpper( char* str )
{
    int c;
    char* cPtr;


    cPtr = str;
    while( *cPtr != '\0' )
    {
        c = toupper( (int)(*cPtr) );

        *cPtr++ = (char)c;
    }
}

/*>*******************************(*)*******************************<*/