/*---------------------------------------------------------------------*

Project:  tc library
File:     TCScriptFile.cpp

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

 $Log: /Dolphin/build/charPipeline/tc/src/TCScriptFile.cpp $
    
    3     8/10/00 6:03p Mikepc
    
    2     3/17/00 1:19p Mikepc
    change tc to use indices numbered from 0.
    
    1     12/03/99 3:45p Ryan
    
    20    10/13/99 4:19p Mikepc
    
    18    10/08/99 2:45p Mikepc
    update for tplConv portability: altered data structures, replaced
    references to 'read tga code' with ' *fileFn, removed redundant
    functions.  Changed some file conversion paths.
    
    17    9/17/99 12:57p Mikepc
    palette entry format defaults to rgb565
    
    16    9/17/99 12:21p Mikepc
    - arbitrary palette size has been removed- changed ReadTplTxtFile() to
    reject CI4 and CI14_X2 as conversion formats.
    - palette alpha layer has been removed- changed ProcessPlKey() to
    default this to 0.
    
    15    9/16/99 8:47p Mikepc
    updated code for auto-palette generation
    
    14    9/13/99 5:03p Mikepc
    changed ProcessPlKey() to look for palette format RGB565 instead of
    R5G6B5.
    
    13    9/09/99 7:02p Mikepc
    in ProcessImKey, changed 2 conversion format values to match latest
    gxEnum.h:  R5G6B5 to RGB565, and CMP to CMPR
    
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
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <CharPipe/TCCommon.h>

#include "TCSrcImageList.h"
#include "TCImageList.h"
#include "TCPaletteList.h"
#include "TCTextureList.h"
#include "TCGXDefs.h"

// path name and ptr for script file keys-
// was 'static', made global so it could be cleared
// each time for batch processing
char  PathName[NAME_SIZE];       // stores current file path name
char* PathPtr = NULL;

/*>*******************************(*)*******************************<*/
static void TCProcessKeyName( char* lineBuff );
static void TCProcessSiKey	( u32 index, char* pathName, char* fileName );
static void TCProcessImKey	( u32 index, char* attList );
static void TCProcessPlKey	( u32 index, char* attList );
static void TCProcessTxKey	( u32 index, char* attList );

static void TCGetKeyTokens	( char* lineBuff, char* leftSide, 
							  char* rightSide );

static void TCGetKeyNum		( char* srcStr, u32* num );
static void TCGetKeyString	( char* srcStr, char* dstStr );
static void TCStripBlanks	( char* srcStr, u32 leading, u32 trailing );

/*>*******************************(*)*******************************<*/
// parse an ini-style text file
/*>*******************************(*)*******************************<*/
void TCReadTplTxtFile ( char* fileName )
{
	FILE* fp;
	char  lineBuff[NAME_SIZE];
	char* buffPtr;

 
	fp = fopen(fileName, "r");
	TCAssertMsg( (fp != NULL), "TCReadTplTextFile: couldn't open %s for read\n", fileName );
						
	// read one line at a time until EOF is reached.
	// trim leading blanks while reading.
	while( (buffPtr = fgets(lineBuff, NAME_SIZE, fp)) != NULL )
	{		
		// ensure that the .txt file line was not greater than (NAME_SIZE-1) characters
		// if the string is of size (NAME_SIZE-1), check if EOF or a newline was encountered at position 254	
		if( (strlen(lineBuff)) == (NAME_SIZE - 1) )
		{
			if( (feof(fp)) == 0 )            // this is not the end of the file
			{
				if( lineBuff[ (NAME_SIZE - 2) ] != '\n' )  // fgets should always include a newline at a line's end
				                                           // except at the end of the file
				{
					TCErrorMsg( "TCReadTplTextFile: line exceeded %d characters in %s", (NAME_SIZE-1), fileName );
				}                           
			}
		}
					 
		TCStripBlanks( lineBuff, 1, 1 );
		
		// identify the line type by its first character
		
		if( lineBuff[0] == '\0' )                   // '\n' after StripBlanks()
		{
			continue;
		}
			
		else if( lineBuff[0] == ';' )               // start of a comment
		{                                   		
			continue; 		
		}				

		else                                        // start of a possible key name
		{
			// take action depending on the type of this key
			TCProcessKeyName(lineBuff);						
		}
	}
	
	fclose(fp);
}

/*>*******************************(*)*******************************<*/
// check the first few characters of a non-comment line for one of
// 'file', 'image', 'palette', 'texture'
// leading blanks will already have been stripped out when fgets was 
// called for lineBuff so keyPtr should point to lineBuff on success
//
// note:  if a key can't be identified, the entire line is ignored
//        
/*>*******************************(*)*******************************<*/
static void TCProcessKeyName ( char* lineBuff )
{
	char  leftSide[NAME_SIZE], rightSide[NAME_SIZE];
	char* keyPtr;
	u32   index;

				
	// split this key into left/right tokens, trim leading/trailing blanks
	TCGetKeyTokens( lineBuff, leftSide, rightSide);
					
	// trim blanks and comments from the right side string
	TCGetKeyString( rightSide, rightSide );

    // convert everything to upper case
    TCStrToUpper( leftSide  );
    TCStrToUpper( rightSide );

	// determine which key this is and get its number if needed	

	if( (keyPtr = strstr( leftSide, "PATH" )) == leftSide )          // path name key
	{
		
		// no number component in path key name
		
		// check for null path name
		if( ((strcmp(rightSide, "0")) == 0) || ((strcmp(rightSide, "NULL")) == 0) )
		{
			PathPtr = NULL;
		}
		else		// set a new path name
		{
			PathPtr = PathName;
			strcpy( PathPtr, rightSide );
		}		
	}
    else if( (keyPtr = strstr( leftSide, "FILE" )) == leftSide )      // source image (file name) key
	{		
		TCGetKeyNum( (leftSide + strlen("FILE")), &index );
		TCProcessSiKey( index, PathPtr, rightSide );								
	}

	else if( (keyPtr = strstr( leftSide, "IMAGE" )) == leftSide )     // image key
	{
		TCGetKeyNum( (leftSide + strlen("IMAGE")), &index );	
		TCProcessImKey(index, rightSide);		
	}		
	else if( (keyPtr = strstr( leftSide, "PALETTE" )) == leftSide )   // palette key
	{		
		TCGetKeyNum( (leftSide + strlen("PALETTE")), &index );		
	    TCProcessPlKey(index, rightSide);			
	}
	else if( (keyPtr = strstr( leftSide, "TEXTURE" )) == leftSide )   // texture key
	{		
		TCGetKeyNum( (leftSide + strlen("TEXTURE")), &index );
		TCProcessTxKey(index, rightSide);				
	}
	else                                                          
	{
		;	   // unknown key- treat as a comment and ignore the whole line
	}
}
	
/*>*******************************(*)*******************************<*/
// fetch source image data using this key as file name.
// create a new source image, identify layers, create layer names 
// based on index and file name
/*>*******************************(*)*******************************<*/
static void TCProcessSiKey ( u32 index, char* pathName, char* fileName )
{
	char        fullName[NAME_SIZE];
	u32         len;
	TCSrcImage* newSi;


    // 'X'ed out index? ignore this key
    if( index == TC_UNUSED )
    {
        return;
    }

    TCAssertMsg( (fileName  != NULL), "TCProcessSiKey: NULL fileName for file %d\n", index    );
    TCAssertMsg( (*fileName != '\0'), "TCProcessSiKey: NULL fileName for file %d\n", index    );

		
	len = 0;
	if( pathName != NULL )
	{
		len = strlen(pathName);
	}
		
	len += strlen(fileName);
    TCAssertMsg( (len <= (NAME_SIZE-1)), "TCProcessSiKey: length of (%s + path) > %d characters\n", (NAME_SIZE-1), fileName );
	
	// build the source image name from the current path and file name	
	if(pathName)
	{
		strcpy(fullName, pathName);
		strcat(fullName, fileName);
	}
	else
	{
		strcpy(fullName, fileName);
	}

	// create a new source image and set its attributes
	newSi = TCNewSrcImage();
	TCAssertMsg( (newSi != NULL),"ProcessSiKey: couldn't allocate memory for new source image %d\n", index );

	// set source image attributes
	// valid source file types depend on user-defined file reading functions
	// set by InstallFileReadFn() - invalid types will be caught by ReadFile()
	TCSetSrcImageFromFile( newSi, fullName, index );	
}
		
/*>*******************************(*)*******************************<*/
static void TCProcessImKey( u32 index, char* attList )
{
	TCImage* newIm;
	char*  icPtr       = NULL, *iaPtr      = NULL, *fmtPtr     = NULL;
	char*  minLODPtr   = NULL, *maxLODPtr  = NULL, *baseLODPtr = NULL;
	u32    icNum       = 0, iaNum          = 0,    fmt         = 0;
	u32    minLOD      = 0, maxLOD         = 0,    baseLOD     = 0;	


    // 'X'ed out index? ignore this key
    if( index == TC_UNUSED )
    {
        return;
    }

    TCAssertMsg( (attList  != NULL), "TCProcessImKey: missing attribute list for image %d in script file\n", index );
    TCAssertMsg( (*attList != '\0'), "TCProcessImKey: missing attribute list for image %d in script file\n", index );
 
				
	// split the attList string into tokens using commas as separators.
	// remove whitespace from around tokens.	
	icPtr  = strtok( attList, "," );
	TCAssertMsg( (icPtr != NULL), "ProcessImKey: problem reading color index for image %d in script file\n", index );

	iaPtr  = strtok( NULL,    "," );
	TCAssertMsg( (iaPtr != NULL), "ProcessImKey: problem reading alpha index for image %d in script file\n", index );

	fmtPtr = strtok( NULL,    ","  );
	TCAssertMsg( (fmtPtr != NULL), "ProcessImKey: problem reading output format for image %d in script file\n", index );
	
	// mip map information may is optional
	// if present, there must be 3 more comma-separated values
	if( (minLODPtr = strtok( NULL,    "," )) != 0 )
	{	
		maxLODPtr = strtok( NULL,    ","  );
		TCAssertMsg( (maxLODPtr != NULL), "ProcessImKey: problem reading max LOD for image %d in script file\n", index );
		
		baseLODPtr = strtok( NULL,    ""  );
		TCAssertMsg( (baseLODPtr != NULL), "ProcessImKey: problem reading remap base LOD for image %d in script file\n", index );
	}
			
	// strip whitespace from these tokens
	TCStripBlanks( icPtr,  1,1 );
	TCStripBlanks( iaPtr,  1,1 );
	TCStripBlanks( fmtPtr, 1,1 );
		
	TCStripBlanks( minLODPtr,   1,1 );	
	TCStripBlanks( maxLODPtr,   1,1 );	
	TCStripBlanks( baseLODPtr,  1,1 );


    // set the color and alpha layer indices:

    // color layer index is mandatory    
    TCGetKeyNum ( icPtr, &icNum );
    TCAssertMsg( (icNum != TC_UNUSED), "TCProcessImKey: invalid color index for image %d in script file\n", index );

    // alpha layer index is optional.
    // if no alpha layer, iaNum will be set to TC_UNUSED
    TCGetKeyNum ( iaPtr, &iaNum );


	// set the final hw pixel format
	if( (strcmp(fmtPtr, "I4")) == 0 )
		fmt = TPL_IMAGE_TEXEL_FMT_I4;
	else if( (strcmp(fmtPtr, "I8")) == 0 )
		fmt = TPL_IMAGE_TEXEL_FMT_I8;
	else if( (strcmp(fmtPtr, "IA4")) == 0 )
		fmt = TPL_IMAGE_TEXEL_FMT_IA4;
	else if( (strcmp(fmtPtr, "IA8")) == 0 )
		fmt = TPL_IMAGE_TEXEL_FMT_IA8;
		
	else if( (strcmp(fmtPtr, "RGB565")) == 0 )
		fmt = TPL_IMAGE_TEXEL_FMT_R5G6B5;
		
	else if( (strcmp(fmtPtr, "RGB5A3")) == 0 )
		fmt = TPL_IMAGE_TEXEL_FMT_RGB5A3;
	else if( (strcmp(fmtPtr, "RGBA8")) == 0 )
		fmt = TPL_IMAGE_TEXEL_FMT_RGBA8;

	else if( (strcmp(fmtPtr, "CI8")) == 0 )
		fmt = TPL_IMAGE_TEXEL_FMT_CI8;

    //=========================================================================================
	// note: input palette sizes other than 8-bit/ @256 entry are not supported in this version.
    //       if output palette is CI4, tc will use the low 4-bits of each index.
    //       if output palette is CI14_X2, tc will only create up to 256 'real' entries;
    //       the remaining palette entries will be padded with zeroes.

	else if( (strcmp(fmtPtr, "CI4")) == 0 )
		fmt = TPL_IMAGE_TEXEL_FMT_CI4;
	else if( (strcmp(fmtPtr, "CI14_X2")) == 0 )
		fmt = TPL_IMAGE_TEXEL_FMT_CI14_X2;

    //==========================================================================================

	else if( (strcmp(fmtPtr, "CMPR")) == 0 )
		fmt = TPL_IMAGE_TEXEL_FMT_CMP;
		
	else
		TCErrorMsg( "ProcessImKey: unknown output format %s for image %d in script file \n", fmtPtr, index );


	// if mipmap numbers are included, get their values
	if( minLODPtr )
		minLOD   = atoi( minLODPtr );
	if( maxLODPtr )
		maxLOD   = atoi( maxLODPtr );
	if( baseLODPtr )
		baseLOD  = atoi( baseLODPtr );
		
	// create a new Image structure and set its attributes.
	// 'index' is also used as the final tpl bank location
	newIm = TCNewImage();
    TCAssertMsg( (newIm != NULL), "ProcessImKey: couldn't allocate new image %d\n", index );

	// set new image attributes
	TCSetImageIndex(       newIm, index                   );                        
	TCSetImageLayerAtt(    newIm, icNum, iaNum            );
    TCSetImageTexelFormat( newIm, fmt                     );
    TCSetImageMipMap(      newIm, minLOD, maxLOD, baseLOD );
}
	
/*>*******************************(*)*******************************<*/
static void TCProcessPlKey ( u32 index, char* attList )
{
	char*      psPtr, *fmtPtr;
	u32        psNum,  fmt;
	TCPalette* newPl;


    // 'X'ed out index? ignore this key
    if( index == TC_UNUSED )
    {
        return;
    }
    
    TCAssertMsg( (attList  != NULL), "TCProcessPlKey: missing attribute list for palette %d in script file\n", index );
    TCAssertMsg( (*attList != '\0'), "TCProcessPlKey: missing attribute list for palette %d in script file\n", index );


	// split the attList string into tokens using commas as separators.
	// remove whitespace from around tokens.	
	psPtr  = strtok( attList, "," );
	TCAssertMsg( (psPtr != NULL), "ProcessPlKey: problem reading file index for palette %d in script file\n", index );
	
	fmtPtr = strtok( NULL,    ""  );
	TCAssertMsg( (fmtPtr != NULL), "ProcessPlKey: problem reading output format for palette %d in script file\n", index );

		
	// strip whitespace from these tokens
	TCStripBlanks( psPtr,    1,1 );
	TCStripBlanks( fmtPtr,   1,1 );

	// set the palette color layer
    // (color and alpha must come from the same source image)
    TCGetKeyNum ( psPtr, &psNum );
    TCAssertMsg( (psNum != TC_UNUSED), "TCProcessPlKey: invalid color index for palette %d in script file\n", index );	
    
	// set the palette entry format
	// note: IA8 format is not supported yet.
	if( (strcmp(fmtPtr, "RGB565")) == 0 )
	{
		fmt = TPL_PALETTE_ENTRY_FMT_R5G6B5;	
	}
	else if( (strcmp(fmtPtr, "RGB5A3")) == 0 )
	{
		fmt = TPL_PALETTE_ENTRY_FMT_RGB5A3;
	}
	else if( (strcmp(fmtPtr, "IA8")) == 0 )
	{
		TCErrorMsg( "ProcessPlKey: tc does not support output format %s. (palette %d in script file)\n", fmtPtr, index );
	}
	else
	{
		TCErrorMsg( "ProcessPlKey: unknown output format %s for palette %d in script file\n", fmtPtr, index );
	}

	// create a new Palette structure and set its attributes.
	// 'index' is also used as the implied bank location
	newPl = TCNewPalette();
	TCAssertMsg( (newPl != NULL), "ProcessPlKey: couldn't allocate palette %d\n", index );
	
	// set palette values
	TCSetPaletteIndex(       newPl, index );
    TCSetPaletteSrcImage(    newPl, psNum );
    TCSetPaletteEntryFormat( newPl, fmt   );
}
	
/*>*******************************(*)*******************************<*/
static void TCProcessTxKey ( u32 index, char* attList )
{
	char*      imPtr, *plPtr;
	u32        imNum,  plNum;
	TCTexture* newTx;


    // 'X'ed out index? ignore this key
    if( index == TC_UNUSED )
    {
        return;
    }
    
    TCAssertMsg( (attList  != NULL), "TCProcessTxKey: missing attribute list for texture %d in script file\n", index );
    TCAssertMsg( (*attList != '\0'), "TCProcessTxKey: missing attribute list for texture %d in script file\n", index );


	// split the attList string into tokens using commas as separators.
	// remove whitespace from around tokens.	
	imPtr  = strtok( attList, "," );
	TCAssertMsg( (imPtr != NULL), "ProcessTxKey: problem reading image index for texture %d in script file\n", index );

	plPtr  = strtok( NULL,    "," );
	TCAssertMsg( (plPtr != NULL), "ProcessTxKey: problem reading palette index for texture %d in script file\n", index );
	
	// strip whitespace from these tokens
	TCStripBlanks( imPtr, 1,1 );
	TCStripBlanks( plPtr, 1,1 );

    // image index is mandatory    
    TCGetKeyNum ( imPtr, &imNum );
    TCAssertMsg( (imNum != TC_UNUSED), "TCProcessTxKey: invalid image index for texture %d in script file\n", index );

    // palette index is optional.
    // if no palette, plNum will be set to TC_UNUSED
    TCGetKeyNum ( plPtr, &plNum );
					
	// create a new Texture structure and set its attributes.
	// 'index' is also used as the implied bank location
	newTx = TCNewTexture();
    TCAssertMsg( (newTx != NULL), "ProcessTxKey(): couldn't allocate new Texture %d\n", index );

    // set texture values
	TCSetTextureAttributes( newTx, index, imNum, plNum );
}
	
/*>*******************************(*)*******************************<*/
// split lineBuff into two tokens separated by an '=' sign,
// strip away any leading/trailing whitespace
//
// note: while there is guaranteed to be at least a one character 
//		 non-whitespace string for leftSide, rightSide may end up 
//		 as NULL if no '=' was found.  this could happen if the 
//		 'key' was actually just a line of text without a ';' comment 
//		 delineator.  The problem will be caught by TCProcessKeyName().
/*>*******************************(*)*******************************<*/
static void TCGetKeyTokens ( char* lineBuff, char* leftSide, char* rightSide )
{
	char* left, *right;


    // safety check
	if( (lineBuff == NULL) || (leftSide == NULL) || (rightSide == NULL) )
	{
		return;
	}
		
	*leftSide  = '\0';
	*rightSide = '\0';
	
	if( *lineBuff == '\0')
	{
		return;
	}

	// use the '=' sign as leftSide/rightSide separator
	if( (left = strtok( lineBuff, "=" )) == 0 )
	{
		return;
	}
	
	strcpy(leftSide, left);
	TCStripBlanks( leftSide, 1, 1 );
		
	if( (right = strtok( NULL,     ""  )) == 0 )
	{
		return;
	}	

	strcpy(rightSide, right);
	TCStripBlanks( rightSide, 1, 1 );
}
	
/*>*******************************(*)*******************************<*/
// get the string equivalent of the key name ( the 'N' portion of 'keyN')	
// and place it in dstStr.  num will hold the converted result.
// note: if *srcStr == TC_BLANK value (TCCommon.h), 
//       *num will be set to TC_UNUSED
/*>*******************************(*)*******************************<*/
static void TCGetKeyNum ( char* srcStr, u32* num )
{
	u32   found  = 0;
	char  numStr[10];
    char* numPtr = NULL;
	char  subStr[NAME_SIZE];
    char* subPtr = NULL;

    
    // initialize *num
    *num = TC_UNUSED;

	// safety check
	if( (srcStr == NULL) || (*srcStr == '\0') )         // no string; return TC_UNUSED
	{
		return;
	}

	strcpy(subStr, srcStr);                             // if no string left after 'TCStripBlanks,
	TCStripBlanks(subStr, 1, 1);                        // while loop will not execute;
                                                        // return TC_UNUSED		
				

	subPtr = subStr;                                    // collect base-10 digits from
	numPtr = numStr;
    while( *subPtr != '\0' )                            // *subStr to the first non-digit value                                                                                    
	{
        if( (isdigit(*subPtr)) != 0 )
        {
            found = 1;
		    *numPtr++ = *subPtr++;
		    continue;
        }

        break;  // reached only if *subStr was not a base-10 digit
	}	
	*numPtr = '\0';	


	if(found)                                          // number found - convert to int value
	{		
	    *num = (u32)atoi(numStr);	
    }
}	
	
/*>*******************************(*)*******************************<*/
// check the right side string for any ';' delineated comments.
// leading/trailing blanks have already been stripped
//
// note: src and dst may be the same address, so a temporary string is used
/*>*******************************(*)*******************************<*/
static void TCGetKeyString( char* srcStr, char* dstStr )
{
	char* name;
	char  tmp[NAME_SIZE];
	u32   count = 0;
	

    // safety check
	if( (srcStr == NULL) || (dstStr == NULL) )
	{
		return;
	}

	// srcStr could be an empty string
	if( *srcStr == '\0')
	{
        *dstStr = '\0';
		return;
	}
		
	// strip away any trailing comment- if the whole string was a comment, return.
	if( (name = strtok(srcStr, ";")) == 0 )
	{
		*dstStr = '\0';
		return;
	}
			
	// trim any trailing blanks between the name and any removed comment
	TCStripBlanks(name, 0, 1 );
	
	// copy out the modified name
	count = 0;
	while(  (tmp[count] = name[count]) != '\0' )
	{
		count++;
	}
	count = 0; 
	while( (dstStr[count] = tmp[count]) != '\0' )
	{
		count++;
	}
}
	
/*>*******************************(*)*******************************<*/
// strip leading and trailing blanks from a string.
// if leading or trailing == 1, strip blanks from the indicated end
// else if leading or trailing == 0, do not strip blanks from the 
// indicated end
/*>*******************************(*)*******************************<*/
static void TCStripBlanks ( char* srcStr, u32 leading, u32 trailing )
{
	u32   i,        len;
	char* startPtr, *endPtr;
	char  tmpStr[NAME_SIZE];
    

	if( srcStr == NULL )
	{
		return;
	}
		
	len = strlen(srcStr);
	if( (len == 0) || (len > (NAME_SIZE-1)) )
	{
        TCErrorMsg( "TCStripBlanks: invalid string length % d for string %s\n", srcStr );
	}

    
	startPtr = srcStr;
	
	// strip leading blanks	
	if( leading == 1 )
	{		
		while( (*startPtr != '\0') && ( (isspace(*startPtr)) != 0 ) )
		{
			startPtr++;
		}	
	}
		
	// strip trailing blanks
	if( trailing == 1 )
	{	
		if( *startPtr != '\0')
		{		
			endPtr = startPtr + (strlen(startPtr)) - 1;
		
			while( (endPtr >= startPtr) && ((isspace(*endPtr)) != 0) )
			{
				endPtr--;
			}
				
			endPtr++;
			*endPtr = '\0';
		}	
	}
			
	// replace the original string with the stripped string
	len = strlen(startPtr);
	for(i=0; i <= len; i++) // include the '\0'
	{
		tmpStr[i] = startPtr[i];
	}	
	for(i=0; i <= len; i++)
	{
		srcStr[i] = tmpStr[i];
	}			
}

/*>*******************************(*)*******************************<*/
