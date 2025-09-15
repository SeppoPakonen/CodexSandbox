/*---------------------------------------------------------------------*

Project:  tc library
File:     TCFile.cpp

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

 $Log: /Dolphin/build/charPipeline/tc/src/TCFile.cpp $
    
    3     8/10/00 6:02p Mikepc
    removed redundant #include<assert.h>, 
    changed any remaining asserts to TCAssertMsg
    
    2     3/17/00 1:19p Mikepc
    change tc to use indices numbered from 0.
    
    1     12/03/99 3:45p Ryan
    
    7     10/08/99 2:45p Mikepc
    update for tplConv portability: altered data structures, replaced
    references to 'read tga code' with ' *fileFn, removed redundant
    functions.  Changed some file conversion paths.
    
    6     10/01/99 12:20p Mikepc
    Integrated s3.lib code to generate s3 textures 'on the fly' as direct
    draw surfaces.  Removed .dds file reading code.  Changed CMP texture
    generation 'order of operations' to- rgb layer->s3->CMPR format per
    mipmap LOD.
    
    5     9/02/99 11:12a Mikepc
    some code re-organization between files.
    added code (verify.cpp) to invoke s3tc.exe from within tc program.
    changed some routines to accommodate the new texture creation path.
    
    4     8/26/99 4:59p Mikepc
    renamed file extensions from .c to .cpp.
    .cpp extension allows addition of namespace protection to remove
    potential name collisions with tool code.  Exceptions are CreateTplFile
    and QuickConvert.  These are extern "C" linked.
    
    3     8/26/99 11:38a Mikepc
    
    2     8/26/99 11:03a Mikepc
    tplCon rewrite for efficient memory usage, batch file processing
    ability.
    
  
 $NoKeywords: $


-----------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <CharPipe/TCCommon.h>

#include "TCFileInternal.h"
#include "TCMem.h"

/********************************/
// global file cache
u32      DFSize = 1;
TCFile*  DF     = NULL;       // cache for decoded files

u32 (*FileFn)( u32 rawSize, u8* rawBits, TCFile* dfPtr ) = NULL;      // ptr to current file-reading function
TCFileNode* FileTable                                    = NULL;      // head of file type/fn table

/*>*******************************(*)*******************************<*/
void TCSetFileCacheSize ( u32 size )
{
	// safety check
	if( !size )
	{
		size = 1;
	}

	DFSize = size;

	DF = (TCFile*)TCCalloc( size, sizeof(TCFile) );
}

/*>*******************************(*)*******************************<*/
// add a new file type to FileTable; install a pointer to the 
// file-reading function 'ext' must be a 3-letter null-terminated 
// string giving the extension of the new file type.
/*>*******************************(*)*******************************<*/
void TCInstallFileReadFn ( char* ext, u32 (*fileFn)( u32 rawSize, 
						   u8* rawBits, TCFile* dfPtr ))
{
    u32         i           = 0;
    u32         found       = 0;
	TCFileNode *fileNode;   
    TCFileNode *newFile;
    char        pExt[4];


    TCAssertMsg( (ext != NULL), "TCInstallFileReadFn: null file extension\n" );
    TCAssertMsg( (strlen(ext) == 3), "TCInstallFileReadFn: extension is not 3 characters\n" );
    TCAssertMsg( (fileFn != NULL), "TCInstallFileReadFn: null file pointer\n" );


	// if the user forgot to set the file cache, default the size to 1
	if( DF == NULL )
	{
		TCSetFileCacheSize( 1 );
	}

  
	// convert the file extension to upper case
    strcpy( pExt, ext );
    TCStrToUpper( pExt );

	// if FileTable is empty, register the type
	if( FileTable == NULL )
	{
		newFile = (TCFileNode*)TCCalloc(1, sizeof(TCFileNode));

		FileTable = newFile;

		strcpy( newFile->ext, pExt );
		newFile->fileFn = fileFn;
		return;
	}

	else
	{
		// search FileTable for this type- if already registered, replace fileFn*
		// otherwise, add a new TCFileNode to the table
		fileNode = FileTable;
		while( fileNode )
		{
			if( (strcmp( fileNode->ext, pExt )) == 0 )
			{
				fileNode->fileFn = fileFn;  // replace fileFn*
				found = 1;
				break;
			}
			fileNode = fileNode->next;
		}

		// file type was not already registered- add a new node
		if( found == 0 )
		{
			fileNode = FileTable;
			while( fileNode->next != NULL )
				fileNode = fileNode->next;

			newFile = (TCFileNode*)TCCalloc( 1, sizeof(TCFileNode) );

			fileNode->next = newFile;
			newFile->prev  = fileNode;

			strcpy( newFile->ext, pExt );
			newFile->fileFn = fileFn;
			return;
		}

	} // end else
}

/*>*******************************(*)*******************************<*/
// scan the file cache for the file; if not there, use the extension to
// locate the file type in the FileTable and call the user-installed 
// file-reading function to decode the file into a free cache element
/*>*******************************(*)*******************************<*/
TCFile* TCReadFile ( char* fileName )
{
	char        ext[4];
	u32         i, len, found;
	u32         rawSize = 0;
	s32         tmp;	
	u8*         rawBits = NULL, *cPtr = NULL;
	FILE*       fp;
	TCFilePtr   dfPtr;
	TCFileNode* fileNode;


    TCAssertMsg( (fileName != NULL),  "TCReadFile: NULL fileName\n" );
    TCAssertMsg( (*fileName != '\0'), "TCReadFile: NULL fileName\n" );
 

	// scan file cache for file
	// if found, return DFPtr
	for( i=0; i< DFSize; i++ )
	{
		if( (strcmp( fileName, DF[i].name )) == 0 ) // file is already in the cache
		{
			return &( DF[i] );
		}
	}

	// otherwise, scan the FileTable for a matching extension,
	// and set the file-read fn ptr

	// get the 3-letter uppercase extension after the last '.'
	len = strlen( fileName ) - 3;
	strcpy( ext, ( fileName + len ) );
    TCStrToUpper( ext );

	fileNode = FileTable;
	found    = 0;
	while( fileNode != NULL )
	{
		if( (strcmp( ext, fileNode->ext )) == 0 )
		{
			found = 1;
			break;
		}

		fileNode = fileNode->next;
	}

	if( found == 0 )
	{
		TCErrorMsg( "ReadFile: file %s is an unregistered type\n", fileName );
		return NULL;
	}

	// set current file-read fn ptr
	FileFn = fileNode->fileFn;

	// look for a clear entry in the file cache; if none is
	// available, clear and use entry 0
	dfPtr = &( DF[0] );
	for( i=0; i<DFSize; i++)
	{
		if( DF[i].name[0] == '\0' )
		{
			dfPtr = &( DF[i] );
			break;
		}
	}

	// clear and zero all fields prior to use
	dfPtr->name[0] = '\0';

	if( dfPtr->lyColor != NULL )
	{
		if( dfPtr->lyColor->data != NULL )
		{
			TCFree( (void**)( &(dfPtr->lyColor->data) ) );
		}
		TCFree( (void**)(&(dfPtr->lyColor)) );
	}

	if( dfPtr->lyAlpha != NULL )
	{
		if( dfPtr->lyAlpha->data != NULL )
		{
			TCFree( (void**)( &(dfPtr->lyAlpha->data) ) );
		}
		TCFree( (void**)( &(dfPtr->lyAlpha)) );
	}

	if( dfPtr->palPtr != NULL )
	{
		if( dfPtr->palPtr->rgba != NULL )
		{
			TCFree( (void**)( &(dfPtr->palPtr->rgba) ) );
		}
		dfPtr->palPtr->numEntry = 0;
		TCFree( (void**)(&(dfPtr->palPtr)) );
	}

	// open the file, get its raw size, and read it to a buffer
	if( (fp = fopen( fileName, "rb" )) == 0 )
	{
		TCErrorMsg( "ReadFile: unable to open %s for read\n", fileName );
		return NULL;
	}

	// copy the file name to dfPtr
	strcpy( dfPtr->name, fileName );

	// get the file size
	rawSize = 0;
	while( 1 )
	{
		fgetc( fp );
		if( (feof( fp )) != 0 )
		{
			break;
		}
		rawSize++;
	}
	rewind( fp );

	// allocate a file buffer
	rawBits = (u8*)TCMalloc( rawSize );

	// copy the raw file to the buffer
	cPtr = rawBits;
	for( i=0; i< rawSize; i++ )
	{
		tmp = getc( fp );
		*cPtr++ = (u8)tmp;
	}
	fclose( fp );

	// call file reading function
	(*FileFn)( rawSize, rawBits, dfPtr );

	// free up the raw file memory
	TCFree( (void**)(&rawBits) );

	// return a ptr to the decoded file in the file cache
	return dfPtr;
}

/*>*******************************(*)*******************************<*/