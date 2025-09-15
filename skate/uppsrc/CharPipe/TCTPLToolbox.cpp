/*---------------------------------------------------------------------*

Project:  tc library
File:     TCTPLToolbox.cpp

Copyright 1998, 1999 Nintendo.  All rights reserved.

These coded instructions, statements and computer programs contain
proprietary information of Nintendo of America Inc. and/or Nintendo
Company Ltd., and are protected by Federal copyright law.  They may
not be disclosed to third parties or copied or duplicated in any form,
in whole or in part, without the prior written consent of Nintendo.

Change History:

 $Log: /Dolphin/build/charPipeline/tc/src/TCTPLToolbox.cpp $
    
    10    8/15/00 4:45p Mikepc
    fixed bug in TCWriteCachedScriptFile - changed fwrite to run in a loop
    to zero out "imCached and "plCached" blocks.
    
    9     8/10/00 6:03p Mikepc
    
    8     3/17/00 1:19p Mikepc
    change tc to use indices numbered from 0.
    
    7     2/15/00 5:55p Mikepc
    updated tpl version # to from 10141999 to 2142000 to reflect CMPR
    format change.
    
    6     2/11/00 4:43p Mikepc
    
    5     2/11/00 12:51p Mikepc
    
    4     1/20/00 2:00p Mikepc
    change to default wrap mode in TCWriteTplFile:
    power of 2 image is set to REPEAT,
    non-power 2 image is set to CLAMP
    
    3     1/13/00 4:42p Mikepc
    added .tpl partial update code.
    
    2     1/11/00 5:40p Mikepc
    added tpl partial update code.
    
    1     12/03/99 3:45p Ryan
    
   
 $NoKeywords: $

-----------------------------------------------------------------------*/

#include <string.h>         // .tpl partial update name comparison
#include <io.h>				// _open(), _close(), _access(), _chmod()
#include <fcntl.h>			// flags for above functions
#include <sys/types.h>		// file status types
#include <sys/stat.h>		// file status functions
#include <sys/utime.h>		// _utime()
#include <time.h>			// time structures, functions
#include <direct.h>         // _mkDir()

#include <stdio.h>

#include <CharPipe/TCCommon.h>

#include "TCTPLToolbox.h"
#include "TCImageList.h"
#include "TCPaletteList.h"
#include "TCTextureList.h"
#include "TCSrcImageList.h"
#include "TCGXDefs.h"
#include "TCMem.h"
#include "TCLayerInternal.h"
#include "TCMipmap.h"
#include "TCFileInternal.h"

/********************************/

// #define for endian format of .tpl header values:

// this define turns TCFixEndian() on and off.

// note: applies to header values and
//       image/palette/texture descriptor blocks only.
//       all of these values are passed to TCFixEndian()
//       before being written to the .tpl file.

//       data blocks are always packed big-endian
//       in dolphin hw format.

// if TPL_BIG_END is defined,
// TCFixEndian() reverses byte order.
// otherwise, TCFixEndian() does nothing

#define TPL_BIG_END			  1

// uncomment this to pack in little-end format
/*
#ifdef TPL_BIG_END
	#undef TPL_BIG_END
#endif
*/

/********************************/

// size of fixed .tpl components in bytes
#define TPL_HDR_SIZE         12
#define TPL_TEX_DESC_SIZE     8
#define TPL_IMG_DESC_SIZE    36 
#define TPL_PAL_DESC_SIZE    12

// translation of windows file access levels  from _access() ( io.h )
#define FILE_ACCESS_READ_WRITE    6         
#define FILE_ACCESS_READ_ONLY     4			
#define FILE_ACCESS_WRITE_ONLY    2
#define FILE_ACCESS_NONE          0

/********************************/

// cached image data from last-used script file
typedef struct TCCachedImage
{

	u32 colorSrcImage;      // srcImage index for color layer
	u32 alphaSrcImage;      // srcImage index for alpha layer; 0 if no alpha layer
	
	struct tm colorModTime;    // mod. date of the color srcImage file
	struct tm alphaModTime;    // mod. date of the alpha srcImage file

    u32 texelFormat;        // Dolphin texture format ( TPL_IMAGE_TEXEL_FMT_X )

	u32 minLOD;             // minimum LOD level desired relative to srcImage
	u32 maxLOD;             // maximum LOD level desired relative to srcImage
	u32 remapMinLOD;        // remapped (actual) minLOD for .tpl file

    u32 tplImageBankOffset;	// bytes from top of .tpl file to image texel data
    u32 tplBufferSize;      // size of image buffer including padding to 32B tiles

} TCCachedImage, *TCCachedImagePtr;


typedef struct TCCachedPalette
{	 
	 u32 srcImage;              // source image file providing
	                            // both color and alpha components of CLUT

	 struct tm srcModTime;      // mod. date of the srcImage

     u32 entryFormat;           // Dolphin format of CLUT entries

     u32 tplPaletteBankOffset;	// number of bytes from top of file to palette data block    
     u32 tplBufferSize;         // size of palette data block

} TCCachedPalette, *TCCachedPalettePtr;


typedef struct TCCsfHeader
{
	u32       header;
    u32       tplNameOffset;
	struct tm tplModTime;

	u32       numCachedImage;
	u32       cachedImageOffset;
	u32       numCachedPalette;
	u32       cachedPaletteOffset;
	u32       stringBankOffset;
	u32       stringBankSize;

} TCCsfHeader, *TCCsfHeaderPtr;

/***************************************/

/*

	CACHED FILE FORMAT (.csf file)

	u32    header                    // blank 

	u32    tplFileName offset        // bytes from start of string bank 
	struct tm tplModTime ( s32 x 5 ) // mod. time of this .tpl

	u32    TCCachedImage count       // TCCachedImage block array size
	u32    TCCachedImage offset      // bytes from start of .csf file 

	u32    TCCachedPalette count     // TCCachedPalette block array size
	u32    TCCachedPalette offset    // bytes from start of .csf file 

	u32    string bank offset        // bytes from start of .csf file 
	u32    string bank size       

	TCCachedImage block              // array of TCCachedImage structures

	TCCachedPalette block            // array of TCCachedPalette structures

	string bank                      // tpl file name, source image file names, 
							         // source palette file names packed end to end.

*/

/***************************************/

// global number, size, counters for .tpl blocks
u32       NumTex                = 0;
u32       NumImage              = 0;
u32       NumPalette            = 0;

const u32 TplHdrSize            = TPL_HDR_SIZE;
const u32 TexDescSize           = TPL_TEX_DESC_SIZE;
u32       TexDescBlockSize      = 0;

const u32 PaletteDescSize       = TPL_PAL_DESC_SIZE;
u32       PaletteDescBlockSize  = 0;
u32       PaletteDescPad        = 0;
u32       PaletteBankSize       = 0;

const u32 ImageDescSize         = TPL_IMG_DESC_SIZE;
u32       ImageDescBlockSize    = 0;
u32       ImageDescPad          = 0;
u32       ImageBankSize         = 0;
     
const u32 TplVersion			= 2142000;				  // tpl version number:
														  // literally 'Feb. 14, 2000'


const char*      CsfPath        = "C:/Temp";			  // in case C:Temp folder doesn't exist
const char*      CsfName        = "C:/Temp/tplCache.csf"; // hard coded .csf file name

TCCachedImage*   CachedImArray  = NULL;					  // array of cached images from .csf file
u32              NumCachedIm    = 0;

TCCachedPalette* CachedPlArray  = NULL;					  // array of cached palettes from .csf file
u32              NumCachedPl    = 0;

u32*             ImFlags        = NULL;					  // array of image/palette partial update flags.
u32*             PlFlags        = NULL;					  // if flag[n] is 0, full conversion required.
														  // if flag[n] is set, value is the byte offset to
														  // pre-converted data in the existing.tpl file

/*>*******************************(*)*******************************<*/

static void TCSetTplPaletteValues ( void );
static void TCSetTplImageValues	  ( void );
static void TCSetTplTextureValues ( void );

static u32 TCWriteTplPaletteBank ( FILE* fp, u32 total, u8* tplPrev, u32* imFlags );
static u32 TCWriteTplImageBank   ( FILE* fp, u32 total, u8* tplPrev, u32* imFlags );

static u32 TCComputeTplImageBufferSize_4Bit	 ( TCImage* thisImage );
static u32 TCComputeTplImageBufferSize_8Bit	 ( TCImage* thisImage );
static u32 TCComputeTplImageBufferSize_16Bit ( TCImage* thisImage );
static u32 TCComputeTplImageBufferSize_32Bit ( TCImage* thisImage );
static u32 TCComputeTplImageBufferSize_Cmp	 ( TCImage* thisImage );

static u32 TCWritePaletteBlock_R5G6B5 ( FILE* fp, TCPalette* pal, u32 start );
static u32 TCWritePaletteBlock_RGB5A3 ( FILE* fp, TCPalette* pal, u32 start );

static void TCPackTile_I4	   ( TCLayer* srcLayer, u32 x, u32 y, u8* dstPtr );
static void TCPackTile_I8	   ( TCLayer* srcLayer, u32 x, u32 y, u8* dstPtr );

static void TCPackTile_IA4     ( TCLayer* colorLayer, TCLayer* alphaLayer, u32 x, u32 y, u8* dstPtr );
static void TCPackTile_IA8     ( TCLayer* colorLayer, TCLayer* alphaLayer, u32 x, u32 y, u8* dstPtr );

static void TCPackTile_R5G6B5  ( TCLayer* colorLayer, u32 x, u32 y, u8* dstPtr );

static void TCPackTile_RGB5A3  ( TCLayer* colorLayer, TCLayer* alphaLayer, u32 x, u32 y, u8* dstPtr );

static void TCPackTile_RGBA8   ( TCLayer* colorLayer, TCLayer* alphaLayer, u32 x, u32 y, u8* dstPtr );

static void TCPackTile_CI4	   (     TCLayer* srcLayer,   u32 x, u32 y, u8* dstPtr );
static void TCPackTile_CI8	   (     TCLayer* srcLayer,   u32 x, u32 y, u8* dstPtr );
static void TCPackTile_CI14_X2 (     TCLayer* colorLayer, u32 x, u32 y, u8* dstPtr );

static void TCPackTile_CMP	   ( TCLayer* layer, u32 tileX, u32 tileY, u16* dstPtr );
static void TCFixCMPWord ( u16* data );

static void TCGetFileModTime ( const char* tplName, struct tm* refTmPtr );
static void TCGetTime		 ( struct tm* tmPtr );
static s32  TCGetFileAccess	 ( const char* fileName );
static void TCSetFileAccess	 ( const char* fileName, s32 axs );
static void TCSetFileModTime ( const char* fileName, struct tm* refTm );
static s32  TCCompareTime    ( struct tm* time1, struct tm* time2 );
static u32  TCGetTplVersion	 ( const char* tplName );

static u32  TCWriteTplValue	 ( FILE* fp, u32 cursor, void* value, u32 numBytes );
static u32  TCWriteTplBlock	 ( FILE* fp, u32 cursor, void* block, u32 numBytes );

static void TCFixEndian ( u8* src, u32 numBytes );

static u32  TCSetFilterModeByDim ( TCImage* thisImage );

static void TCWriteCachedScriptFile ( const char* tplName,    struct tm* refTmPtr );
static u32  TCReadCachedScriptFile  ( const char* newTplName, u8**       tplPrev  );

static u32  TCCompareToCachedScriptData ( const char* prevTplName, struct tm* prevTplModTime, u8** tplPrev );
static u32  TCCompareImToCachedIm ( struct tm* refTimePtr, TCImage* imPtr,   TCCachedImage*   cImPtr );
static u32  TCComparePlToCachedPl ( struct tm* refTimePtr, TCPalette* plPtr, TCCachedPalette* cPlPtr );

/*>*******************************(*)*******************************<*/ 
// this computes the size of the various blocks within the .tpl file;
// it also computes the offsets (eg image n data offset) for each descriptor
// at this point, the actual data is not yet transformed to hw format
/*>*******************************(*)*******************************<*/ 
void TCComputeTplSizes ( void )
{
	TCImage*   imPtr;
	TCPalette* plPtr;
	TCTexture* txPtr;


	// get the image, palette and texture list sizes
    NumImage = 0;
	imPtr    = ImHead;
	while( imPtr )
	{
		NumImage++;
		imPtr = imPtr->next;
	}

	NumPalette = 0;
	plPtr      = PlHead;
	while( plPtr )
	{
		NumPalette++;
		plPtr = plPtr->next;
	}

	NumTex = 0;
	txPtr  = TxHead;
	while( txPtr )
	{
		NumTex++;
		txPtr = txPtr->next;
	}

	// there must be at least 1 texture and 1 image to create a tpl file
	TCAssertMsg( (NumTex),   "TCComputeTplSizes: NumTex = 0\n"   );
    TCAssertMsg( (NumImage), "TCComputeTplSizes: NumImage = 0\n" );

    // set file descriptor block sizes
    TexDescBlockSize     = NumTex     * TexDescSize; 
    ImageDescBlockSize   = NumImage   * ImageDescSize;
    PaletteDescBlockSize = NumPalette * PaletteDescSize;

    // set .tpl values for images and palettes, compute total bank sizes
    // order is important as palette descriptors and palette bank come before image descriptors, image bank
    // values are stored in the appropriate palette/image structures in ImHead, PlHead lists
	TCSetTplPaletteValues();
    TCSetTplImageValues();

    // compute the actual texture descriptor block offsets for the .tpl file
    // this must be done last as the location of the image descriptors depends on the size of 
    // the palette descriptor block and palette bank
    TCSetTplTextureValues();  
}

/*>*******************************(*)*******************************<*/ 
// note: header values are packed according to status of TPL_BIG_END flag;
//       data blocks are always packed big-end for Dolphin hw
/*>*******************************(*)*******************************<*/ 
void TCWriteTplFile ( char* tplFile )
{
	FILE*       fp;
	TCTexture*  texPtr;
	TCPalette*  palPtr;
	TCImage*    imgPtr;
	u32         total;
	u8          cTmp;
	u16         sTmp;
	u32         iTmp,  iTmp1;
    
	struct tm   tplModTime;							    // modification date of the new .tpl
	u8*         tplPrev       = NULL;				    // buffer for previous .tpl file
	u32         partialUpdate = 0;

    u32         pad[8]        = { 0,0,0,0,0,0,0,0 };	// up to 32B pad between header blocks	


    // tpl file must contain at least 1 texture, image.
    TCAssertMsg( (NumTex),   "TCWriteTplFile: NumTex = 0\n" );
    TCAssertMsg( (NumImage), "TCWriteTplFile: NumImage = 0\n" );


	// read the cached .csf file, compare cached to current data,
	// initialize ImFlags, PlFlags arrays.
	// if any partial update is possible, the previous .tpl file will be stored
	// in a buffer allocated to tplPrev.
	partialUpdate = TCReadCachedScriptFile( tplFile, &tplPrev );

	//---------------------------------------------------------------------------

	// create/overwrite the .tpl file 
	TCSetFileAccess ( tplFile, FILE_ACCESS_READ_WRITE );	// in case the srcTree locked it.	 

	fp = fopen(tplFile, "wb");
	TCAssertMsg( (fp != NULL), "TCWriteTplFile: couldn't create .tpl file %s for write\n", tplFile );

	//-------------------------------------------------------------------------------------------

	// number of bytes written to new .tpl file
	total = 0;  

																// TPL HEADER BLOCK
																// (12B)


																// VERSION NUMBER
																// (4B)
	total += TCWriteTplValue( fp, total, (void*)&TplVersion, 4 );

	
																// NUMBER OF TEXTURE DESCRIPTORS
																// (4B)
	total += TCWriteTplValue( fp, total, (void*)&NumTex, 4 );

	
																// OFFSET TO TEXTURE DESCRIPTOR BLOCK
																// (4B, VALUE = 12)
	total += TCWriteTplValue( fp, total, (void*)&TplHdrSize, 4 );
				
	//-------------------------------------------------------------------------------------------

                                                                // TEXTURE DESCRIPTOR BLOCK
                                                                // (8B x NumTex)                   
	texPtr = TxHead;                                         
	while( texPtr )                                                         
	{                                                     	
																// OFFSET TO IMAGE DESCRIPTOR
																// (4B)
		total += TCWriteTplValue( fp, total, (void*)&texPtr->tplImageOffset, 4 );
				

																// OFFSET TO CLUT DESCRIPTOR
																// (4B)
		total += TCWriteTplValue( fp, total, (void*)&texPtr->tplPaletteOffset, 4 );


		texPtr = texPtr->next;
	}	

	//-------------------------------------------------------------------------------------------
	
					                                            // PALETTE DESCRIPTOR BLOCK
				                                                // (12B x NumPalette)                   
	
	
	// note:  if no palettes are used by textures, 
	//        there will not be a palette descriptor block in the file
	
	
	palPtr = PlHead;                                          
	while( palPtr )                                                
	{
																// NUMBER OF PALETTE ENTRIES
																// (2B)		
		total += TCWriteTplValue( fp, total, (void*)&palPtr->palPtr->numEntry, 2 );


																// PAD
																// (2B)
		total += TCWriteTplValue( fp, total, (void*)pad, 2 );


																// PALETTE ENTRY FORMAT
																// (4B)
		total += TCWriteTplValue( fp, total, (void*)&palPtr->entryFormat, 4 );


		
																// OFFSET TO PALETTE DATA
																// (4B)
		total += TCWriteTplValue( fp, total, (void*)&palPtr->tplPaletteBankOffset, 4 );

				
		palPtr = palPtr->next;
	}	
	
	//------------------------------------------------------
	
	                                                         // PALETTE DESCRIPTOR BLOCK PAD
	                                                         // (ROUND OUT TO 32B)

	// if there is no palette descriptor block, pad will be 0
	if(PaletteDescPad != 0)							
	{                                            																					 
		total += TCWriteTplBlock( fp, total, (void*)pad, PaletteDescPad );
	}
	
	//-------------------------------------------------------------------------------------------

		
															// PALETTE DATA BLOCK

	total += TCWriteTplPaletteBank( fp, total, tplPrev, PlFlags );


	//-------------------------------------------------------------------------------------------

															// IMAGE DESCRIPTOR BLOCK
															// (36B x NumImage)                    
		
	imgPtr = ImHead;									
	while( imgPtr )										                
	{                                                     
		// note: height and width refer to the original 
		//       unpadded image dimensions.
		//       however, if image->minLOD is not 0, width and height
		//       must be remapped to reflect the size of the 1st LOD 
		//       actually stored in the .tpl file.
		//       this is achieved by shifting sTmp by imgPtr->minLOD.


		// image height:  convert to 2-byte value			// IMAGE HEIGHT
		sTmp   = (u16)( (imgPtr->lyColor).height );			// (2B)
		sTmp >>= imgPtr->minLOD;
		total += TCWriteTplValue( fp, total, (void*)&sTmp, 2 );


		// image width:  convert to 2-byte value
		sTmp   = (u16)( (imgPtr->lyColor).width );			// IMAGE WIDTH
		sTmp >>= imgPtr->minLOD;							// (2B)
		total += TCWriteTplValue( fp, total, (void*)&sTmp, 2 );


															// IMAGE PIXEL FORMAT
															// (4B)
		total += TCWriteTplValue( fp, total, (void*)&imgPtr->texelFormat, 4 );


															// OFFSET TO IMAGE DATA
															// (4B)
		total += TCWriteTplValue( fp, total, (void*)&imgPtr->tplImageBankOffset, 4 );

	
		iTmp = TCSetFilterModeByDim( imgPtr );				// FILTER MODES:
		                                                    // TPL_WRAP_MODE_REPEAT for power of 2 images
		                                                    // TPL_WRAP_MODE_CLAMP for non-power 2 images

															// WRAP S
															// (4B)
		total += TCWriteTplValue( fp, total, (void*)&iTmp, 4 );

															// WRAP T
															// (4B)
		total += TCWriteTplValue( fp, total, (void*)&iTmp, 4 );

		                                                        
															// MIN, MAG FILTER MODES
															// BASED ON IMAGE TYPE, NUM LOD

		// 4 byte each for default tex filter min and mag modes.
		switch( imgPtr->texelFormat )
		{		
		case TPL_IMAGE_TEXEL_FMT_CI4:								// palettized image
		case TPL_IMAGE_TEXEL_FMT_CI8:
		case TPL_IMAGE_TEXEL_FMT_CI14_X2:

			iTmp  = TPL_TEX_FILTER_LINEAR;							// min		
			iTmp1 = TPL_TEX_FILTER_LINEAR;							// mag
			break; 

		default:													// true color image

			if( ( imgPtr->maxLOD - imgPtr->minLOD + 1 ) == 1 )		// single LOD	
			{
				iTmp  = TPL_TEX_FILTER_LINEAR;						// min
				iTmp1 = TPL_TEX_FILTER_LINEAR;						// mag
			}
			else													// mip mapped									
			{
				iTmp  = TPL_TEX_FILTER_LIN_MIP_LIN;					// min
				iTmp1 = TPL_TEX_FILTER_LINEAR;						// mag
			}
			break;
		}

															// MIN TEX FILTER MODE
															// (4B) 
		total += TCWriteTplValue( fp, total, (void*)&iTmp, 4 );
	

															// MAG TEX FILTER MODE
															// (4B)
		total += TCWriteTplValue( fp, total, (void*)&iTmp1, 4 );


		iTmp = 0;											// LOD BIAS
															// (4B, DEFAULT = 0) 
		total += TCWriteTplValue( fp, total, (void*)&iTmp, 4 );


		cTmp   = 0;											// LOD EDGE ENABLE 
															// (1B, DEFAULT = 0)
		total += TCWriteTplValue( fp, total, (void*)&cTmp, 1 );


															// IMAGE MIN. LOD
		cTmp   = (u8)(imgPtr->remapMinLOD);					// (1B)
		total += TCWriteTplValue( fp, total, (void*)&cTmp, 1 );


															// IMAGE MAX. LOD
															// (1B)
		// new max LOD is ( remapped minLOD + numLOD - 1 )
		cTmp   = (u8)( imgPtr->remapMinLOD + (imgPtr->maxLOD - imgPtr->minLOD) );  
		total += TCWriteTplValue( fp, total, (void*)&cTmp, 1 );


													    	// PAD
		              										// (1B, DEFAULT = 0)
		total += TCWriteTplValue( fp, total, (void*)pad, 1 );

		
		imgPtr = imgPtr->next;
	}
		
	//------------------------------------------------------

															// IMAGE DESCRIPTOR BLOCK PAD
															// (ROUND OUT TO 32B)		
	if(ImageDescPad != 0)								
	{														
		total += TCWriteTplBlock( fp, total, (void*)pad, ImageDescPad );
	}
	
	//-------------------------------------------------------------------------------------------
	
	                                                            // IMAGE DATA BLOCK

	total += TCWriteTplImageBank( fp, total, tplPrev, ImFlags );


	//------------------------------------------------------

	fclose(fp);

	// explicitly set/update the .tpl modification date.
	// do this before calling TCWriteCachedScriptFile
	TCGetTime( &tplModTime );
	TCSetFileModTime( tplFile, &tplModTime );

	// cache the new .tpl file's script data in a .csf file
	TCWriteCachedScriptFile( tplFile, &tplModTime );

	//------------------------------

	// free memory, reset variables related to .csf cache

	if( tplPrev != NULL )
	{
		TCFree( (void**)(&tplPrev) );
		tplPrev = NULL;
	}

	if( CachedImArray != NULL )
	{
		TCFree( (void**)( &CachedImArray ) );
		CachedImArray = NULL;
	}
	NumCachedIm   = 0;

	if( CachedPlArray != NULL )
	{
		TCFree( (void**)( &CachedPlArray ) );
		CachedPlArray = NULL;
	}
	NumCachedPl   = 0;

	if( ImFlags != NULL )
	{
		TCFree( (void**)( &ImFlags ) );
		ImFlags = NULL;
	}

	if( PlFlags != NULL )
	{
		TCFree( (void**)( &PlFlags ) );
		PlFlags = NULL;
	}

	//------------------------------
}	
		
/*>*******************************(*)*******************************<*/ 
// traverse the sorted palette list and compute the 'tpl' values including the
// data buffer sizes
/*>*******************************(*)*******************************<*/ 
static void TCSetTplPaletteValues ( void )
{
    TCPalette* thisPalette;
    u32        bankOffset;
    u32        paletteEntrySize;
    u32        size;


	// if a palette descriptor block is present, compute the pad needed to round it out to 32B alignment.
	// if not present, pad will remain 0.
	PaletteDescPad = 0;
	if(PaletteDescBlockSize != 0)
	{
		size = TplHdrSize + TexDescBlockSize + PaletteDescBlockSize;
		
		if(size < 32)
		{
			PaletteDescPad = 32 - size;
		}
		else if( (size % 32) != 0 )
		{
			PaletteDescPad = 32 - (size % 32);
		}	
	} 
    
    // offset to this palette's data from the top of the file
    bankOffset = TplHdrSize + TexDescBlockSize + PaletteDescBlockSize + PaletteDescPad; 
	
	PaletteBankSize = 0;

    thisPalette = PlHead;  
    while( thisPalette )
    {                  
   
        thisPalette->tplPaletteBankOffset = bankOffset;
        
		// compute the size of the required tpl buffer for this palette
		// note: IA8 formatis not supported in this version.
        switch(thisPalette->entryFormat)
        {
        case TPL_PALETTE_ENTRY_FMT_R5G6B5:  
			paletteEntrySize = 2;		
			break;			
        case TPL_PALETTE_ENTRY_FMT_RGB5A3:  
			paletteEntrySize = 2;		
			break;
        default: 
			TCErrorMsg( "TCSetTplPaletteValues: unknown entry format for palette %d\n", thisPalette->index );
			return;      
			break;
        }
               
        // compute buffer size for palette.
        // all of these will align to 32B automatically
        // in final data conversion, unused entries will be zeroed out.
		// maximum palette size is 16k entries
        if( (thisPalette->palPtr->numEntry == 0) || (thisPalette->palPtr->numEntry > 16384) )
        {
			TCErrorMsg( "TCSetTplPaletteValues: num entries out of range for palette %d\n", thisPalette->index );
        	return;
        }
        else
        {
        	thisPalette->tplBufferSize = ((thisPalette->palPtr->numEntry + 15) & 0xFFF0) * paletteEntrySize;
        }
          
		// bank offset and total bank size update continuously
        bankOffset      += thisPalette->tplBufferSize;
        PaletteBankSize += thisPalette->tplBufferSize;
        
        thisPalette = thisPalette->next;

    } // end while
}

/*>*******************************(*)*******************************<*/ 
// traverse the sorted image list and compute the 'tpl' values including the
// data buffer sizes
/*>*******************************(*)*******************************<*/ 
static void TCSetTplImageValues ( void )
{
    TCImage* thisImage;
    u32      bankOffset;
	u32      size;
	

    // compute the pad needed to round the image descriptor block out to 32B alignment.
	ImageDescPad = 0;
	
	size =   TplHdrSize     + TexDescBlockSize + PaletteDescBlockSize
           + PaletteDescPad + PaletteBankSize  + ImageDescBlockSize; 
                 
	if(size < 32)
	{
		ImageDescPad = 32 - size;
	}
	else if( (size % 32) != 0 )
	{
		ImageDescPad = 32 - (size % 32);
	}	
	
    // offset to image data from the top of the file
    bankOffset =   TplHdrSize     + TexDescBlockSize + PaletteDescBlockSize
                 + PaletteDescPad + PaletteBankSize  + ImageDescBlockSize + ImageDescPad; 
                                     
	ImageBankSize = 0;

    thisImage = ImHead;
    while( thisImage )
    {               
        // note:  image buffer size is determined by colorLayer->height, colorLayer->width,
        //        final pixel format, mipmaps and padding requirements.

        thisImage->tplImageBankOffset = bankOffset;
        thisImage->tplBufferSize      = TCComputeTplMipMapImageBufferSize( thisImage );

		// bank offset and total bank size update continuously
        bankOffset    += thisImage->tplBufferSize;
        ImageBankSize += thisImage->tplBufferSize;
        
        thisImage = thisImage->next;
	}
}

/*>*******************************(*)*******************************<*/ 
// compute the actual .tpl file offsets for each texture descriptor.
// this must be called after .tpl values have been set for images and 
// palettes.  all lists must already be sorted
/*>*******************************(*)*******************************<*/ 
static void TCSetTplTextureValues ( void )
{
    TCTexture* thisTex;
    TCImage*   thisImage;
    TCPalette* thisPalette;
    u32        paletteDescOffset, imageDescOffset;
    u32        pos;


	// offsets from top of file to the start of each descriptor block
    paletteDescOffset     = TplHdrSize     + TexDescBlockSize;
    
    imageDescOffset       = TplHdrSize     + TexDescBlockSize + PaletteDescBlockSize + 
                            PaletteDescPad + PaletteBankSize;

    thisTex = TxHead;
    while( thisTex )
    {
       // compute offsets from the top of the file 
        // to the given image/palette descriptor (NOT to the actual data)
                 
        thisImage = TCFindImageByIndex( thisTex->image );
        TCAssertMsg( (thisImage != NULL), "TCSetTplTextureValues: no matching image for texture %d\n", thisTex->index );

        // find thisImage's position in the list
        pos = TCFindImagePos( thisImage );
        thisTex->tplImageOffset = imageDescOffset + ( pos * ImageDescSize);

        // note: a texture must have an image, but doesn't require a palette
        thisTex->tplPaletteOffset = 0;
        if( thisTex->palette != TC_UNUSED )
        {
            thisPalette = TCFindPaletteByIndex( thisTex->palette );      
            pos         = TCFindPalettePos( thisPalette );
            thisTex->tplPaletteOffset = paletteDescOffset + ( pos * PaletteDescSize);       
		}
			       
        thisTex = thisTex->next;
	}
}

/*>*******************************(*)*******************************<*/ 
// compute the size in bytes of a .tpl file image buffer
// include row and column padding to 32 byte alignment
// return the required buffer size on success or 0 on failure
/*>*******************************(*)*******************************<*/ 
u32 TCComputeTplImageBufferSize ( TCImage* thisImage )
{
	u32 size;


    switch(thisImage->texelFormat)
    {           
    case TPL_IMAGE_TEXEL_FMT_I4:
    	size = TCComputeTplImageBufferSize_4Bit(thisImage);		break;
    case TPL_IMAGE_TEXEL_FMT_I8:
        size = TCComputeTplImageBufferSize_8Bit(thisImage);		break;
    case TPL_IMAGE_TEXEL_FMT_IA4:
        size = TCComputeTplImageBufferSize_8Bit(thisImage);		break;            	
    case TPL_IMAGE_TEXEL_FMT_IA8:
        size = TCComputeTplImageBufferSize_16Bit(thisImage);	break;          	
    case TPL_IMAGE_TEXEL_FMT_R5G6B5:
        size = TCComputeTplImageBufferSize_16Bit(thisImage);	break;                 	      
    case TPL_IMAGE_TEXEL_FMT_RGB5A3:
        size = TCComputeTplImageBufferSize_16Bit(thisImage);	break;          	
    case TPL_IMAGE_TEXEL_FMT_RGBA8:
        size = TCComputeTplImageBufferSize_32Bit(thisImage);	break;           	
    case TPL_IMAGE_TEXEL_FMT_CI4:
        size = TCComputeTplImageBufferSize_4Bit(thisImage);		break;           	
    case TPL_IMAGE_TEXEL_FMT_CI8:
        size = TCComputeTplImageBufferSize_8Bit(thisImage);		break;          	
	case TPL_IMAGE_TEXEL_FMT_CI14_X2:
        size = TCComputeTplImageBufferSize_16Bit(thisImage);	break;          		
    case TPL_IMAGE_TEXEL_FMT_CMP:
        size = TCComputeTplImageBufferSize_Cmp(thisImage);		break;	
    default:
        TCErrorMsg( "TCComputeTplImageBufferSize: unknown output format for image %d\n", thisImage->index );
        return 0;
        break;          	
    }

    return size;  
}

/*>*******************************(*)*******************************<*/ 
// 4-bit format = 8x8 texel block / 32B cache line
/*>*******************************(*)*******************************<*/ 
static u32 TCComputeTplImageBufferSize_4Bit	( TCImage* thisImage )
{
	u32 tileCols, tileRows, size;
	u32 width, height;

	width  = (thisImage->lyColor).width;
	height = (thisImage->lyColor).height;
 
 	// need to pad tile columns out to 8 texels
 	tileCols  = ((width + 7) >> 3);

	// need to pad total # rows out to 8 texels
	tileRows  = ((height + 7) >> 3);

	//   = total # of tiles * 32B per tile
	size = tileCols * tileRows * 32;
	return size;
}

/*>*******************************(*)*******************************<*/ 
// 8-bit format     = 4x8 texel block / 32B cache line
/*>*******************************(*)*******************************<*/ 
static u32 TCComputeTplImageBufferSize_8Bit ( TCImage* thisImage )
{
	u32 tileCols, tileRows, size;
	u32 width, height;


	width  = (thisImage->lyColor).width;
	height = (thisImage->lyColor).height;
 
 	// need to pad tile columns out to 8 texels
 	tileCols  = ((width + 7) >> 3);

	// need to pad total # rows out to 4 texels
	tileRows  = ((height + 3) >> 2);

	//   = total # of tiles * 32B per tile
	size = tileCols * tileRows * 32;
	return size;
}

/*>*******************************(*)*******************************<*/ 
// 16-bit format    = 4x4 texel block / 32B cache line
/*>*******************************(*)*******************************<*/ 
static u32 TCComputeTplImageBufferSize_16Bit ( TCImage* thisImage )
{
	u32 tileCols, tileRows, size;
	u32 width, height;


	width  = (thisImage->lyColor).width;
	height = (thisImage->lyColor).height;
 
 	// need to pad final tile out to 4 texels
 	tileCols = ((width + 3) >> 2);

	// need to pad total # rows out to 4 texels
	tileRows = ((height + 3) >> 2);

	//   = total # of tiles * 32B per tile
	size = tileCols * tileRows * 32;
	return size;
}

/*>*******************************(*)*******************************<*/ 
// 32-bit format     = 4x4 texel block, 32B cache line, 2 lines needed
/*>*******************************(*)*******************************<*/ 
static u32 TCComputeTplImageBufferSize_32Bit (TCImage* thisImage )
{
	u32 tileCols, tileRows, size;
	u32 width, height;


	width  = (thisImage->lyColor).width;
	height = (thisImage->lyColor).height;
 
 	// need to pad final tile out to 4 texels 
 	tileCols = ((width + 3) >> 2);

	// need to pad total # rows out to 4 texels
	tileRows = ((height + 3) >> 2);

	//   = total # of tiles in the padded image * 32B per tile * 2 blocks of data
	size = tileCols * tileRows * 32 * 2;
	return size;
}

/*>*******************************(*)*******************************<*/ 
// CMP format       = 8x8 texel block / 32B cache line
// breakdown:       1 4x4 texel block = 32-bits for texel indices, + 32-bits for 2 16-bit colors
//                  total = 64b or 8B per 4x4 texel block.
/*>*******************************(*)*******************************<*/ 
static u32 TCComputeTplImageBufferSize_Cmp ( TCImage* thisImage )
{
	u32 tileRows, tileCols, size;
	u32 width, height;


	width  = (thisImage->lyColor).width;
	height = (thisImage->lyColor).height;
 
	// must pad any image < 8 texels out to 8 texel boundary

	// these are the 8x8 tpl tiles
	tileCols = ((width  + 7) >> 3);
	tileRows = ((height + 7) >> 3);

	size = (tileRows * tileCols * 32);
	return size;
}

/*>*******************************(*)*******************************<*/ 
// palettes are stored at 16-entry granularity, 32B word 
// (entries 0x0 to 0xF) per line with 0x00 at address 0.
/*>*******************************(*)*******************************<*/ 
static u32 TCWriteTplPaletteBank ( FILE* fp, u32 total, u8* tplPrev, u32* plFlags )
{
	u32         i;
	u32         numBlocks;
	u32         bytesWritten = 0;
	TCPalette*  plPtr;
	u32         blockSize    = 32;	// ( 16 entry/block x 2B/entry )
	void*       dataPtr;
	u32        (*palFn)( FILE* fp, TCPalette* pal, u32 start ) = NULL; 
    u32         pos;
    
	
    TCAssertMsg( (fp != NULL), "TCWriteTplPaletteBank: NULL file ptr\n" );


	// this list is already sorted from 1->n
	plPtr = PlHead;	
	while( plPtr )
	{	
		// if this palette's data can be copied over from the existing .tpl file,
		// use the offset stored in tplFlags
		if( tplPrev && plFlags )
		{
            pos = TCFindPalettePos( plPtr );

            if( plFlags[pos] )
            {
			    dataPtr = (void*)( tplPrev + plFlags[pos] );
			    fwrite( dataPtr, plPtr->tplBufferSize, 1, fp );                     
			    bytesWritten += plPtr->tplBufferSize;

			    plPtr = plPtr->next;
			    continue;
            }
		}

		// compute # of 16-entry blocks needed for actual entries
 		numBlocks = ((plPtr->palPtr->numEntry + 15) >> 4);

		// set the palette function depending on entry format
        switch( plPtr->entryFormat )
        {      
        case TPL_PALETTE_ENTRY_FMT_R5G6B5:
			palFn = TCWritePaletteBlock_R5G6B5;
			break;

		case TPL_PALETTE_ENTRY_FMT_RGB5A3:    // note: if no alpha layer, all alphas are set to max.     
        	palFn = TCWritePaletteBlock_RGB5A3;
        	break;
		default:
			TCErrorMsg( "TCWritePaletteBank: unknown entry format for palette %d\n", plPtr->index );
			return 0;
			break;
		}

 		for(i=0; i<numBlocks; i++)
 		{		
        	bytesWritten += palFn( fp, plPtr, (i*16) );						
		} 
			
    	plPtr = plPtr->next;
    	
    }  // end while( plPtr )

	return bytesWritten;
}

/*>*******************************(*)*******************************<*/ 
static u32 TCWritePaletteBlock_R5G6B5 ( FILE* fp, TCPalette* pal, u32 start )
{	
	u8  r, g, b;
	u8* tplPtr;
	u16 u16Tmp;
	u32 entry, realEntries;
	u32 bytesWritten = 0, count = 0;


	realEntries = pal->palPtr->numEntry - start;
	if( realEntries > 16)
	{
		realEntries = 16;
	}

	tplPtr = (u8*)(&u16Tmp);
	
	for(entry=0; entry< realEntries; entry++)
	{
		TCGetPalTableValue( pal->palPtr, (start + entry), &r, &g, &b, NULL );
		
		// pack entries in big-endian format
		*tplPtr       = ( ( r & 0xF8)       | (g >> 5) );  // byte 0 is 5 bits red, upper 3 bits of green
		*(tplPtr + 1) = ( ((g & 0x1C) << 3) | (b >> 3) );  // byte 1 is lower 3 bits of green, 5 bits blue
		
		if( (count = fwrite( &u16Tmp, 2, 1, fp )) != 1 )
		{
			TCErrorMsg( "WritePaletteBlockRGB565:  error in fwrite for palette %d\n", pal->index );
			return 0;
		}

		bytesWritten += 2;
	}

	// pad unused entries with zeros
	u16Tmp = 0;
	for(entry; entry<16; entry++)
	{
		if( (count = fwrite( &u16Tmp, 2, 1, fp )) != 1 )
		{
			TCErrorMsg( "WritePaletteBlockRGB565:  error in fwrite for palette %d\n", pal->index );
			return 0;
		}
		bytesWritten += 2;
	}

	return bytesWritten;
}
	
/*>*******************************(*)*******************************<*/ 
static u32 TCWritePaletteBlock_RGB5A3 ( FILE* fp, TCPalette* pal, u32 start )
{	
	u8  r, g, b, a;
	u8* tplPtr;
	u32 entry, realEntries;
	u16 u16Tmp;
	u32 bytesWritten = 0, count = 0;


	realEntries = pal->palPtr->numEntry - start;
	if( realEntries > 16)
	{
		realEntries = 16;
	}
		
	tplPtr   = (u8*)(&u16Tmp);

	for(entry=0; entry< realEntries; entry++)
	{
		TCGetPalTableValue( pal->palPtr, (start + entry), &r, &g, &b, &a );
					
		if( a == 0xFF )	// fully opaque texel- set alpha to 1-bit format
		{
			// pack in 5551 format, alpha bit is set to 1
			*tplPtr       = (  (a & 0x80)       | ((r & 0xF8) >> 1) | (g >> 6) );  // byte0 is 1 bit alpha, upper 5-bits 		                                                                             
			*(tplPtr + 1) = ( ((g & 0x38) << 2) |  (b >>   3)                  );  // byte1 is bits 3-5 of green, upper 5 of blue
		}				
		
		else // 03444 format
		{
			a = (a >> 1) & 0x70; // keep 3 msbs shifted over by 1, msb set to 0
		
			*tplPtr       = (  a         | ((r & 0xF0) >> 4) );  // byte0 is msb = 0, 3 bits alpha, 4 bits red		                                                                              
			*(tplPtr + 1) = ( (g & 0xF0) |  ( b >> 4)        );  // byte1 is 4 bits green, 4 bits blue	
		}

		if( (count = fwrite( &u16Tmp, 2, 1, fp )) != 1 )
		{
			TCErrorMsg( "WritePaletteBlockRGB5A3:  error in fwrite for palette %d\n", pal->index );
			return 0;
		}
		bytesWritten += 2;	
	}

	// pad unused entries with zeros
	u16Tmp = 0;
	for(entry; entry<16; entry++)
	{
		if( (count = fwrite( &u16Tmp, 2, 1, fp )) != 1 )
		{
			TCErrorMsg( "WritePaletteBlockRGB5A3:  error in fwrite for palette %d\n", pal->index );
			return 0;
		}
		bytesWritten += 2;
	}
	
	return bytesWritten;
}
	
/*>*******************************(*)*******************************<*/ 
// do this after all .tpl values have been set.
// allocate a (tpl) data buffer, perform image conversion
// using colorLayer + alphaLayer to specific .tpl format
/*>*******************************(*)*******************************<*/ 
static u32 TCWriteTplImageBank ( FILE* fp, u32 total, u8* tplPrev, u32* imFlags )
{
	TCImage*         imPtr;
	TCSrcImage*      siPtr;
	TCFilePtr        dfPtr;
	u32              bytesWritten = 0;
	void*            dataPtr;
    u32              pos;


    TCAssertMsg( (fp != NULL), "TCWriteTplImageBank: NULL file ptr\n" );


	// this list is already sorted in ascending order
	imPtr = ImHead;	
	while( imPtr )
	{
		// this data block can be copied over from the existing .tpl file
		// using the offset stored in tplFlags
		if( tplPrev && imFlags )
		{
            pos = TCFindImagePos( imPtr );
            if( imFlags[pos] )
            {
			    dataPtr = (void*)( tplPrev + imFlags[pos] );
			    fwrite( dataPtr, imPtr->tplBufferSize, 1, fp );                     
			    bytesWritten += imPtr->tplBufferSize;

			    imPtr = imPtr->next;
			    continue;
            }
		}

		// otherwise, if the .tpl file is new or if a partial update is required,
		// convert and write the image block


	    // if alpha layer comes from a different srcImage, read another file.
		// note: do this first so that dfPtr is read second for color layer.
		//       this allows the same dfPtr to be used in the switch statements
		//       below for layer conversion .
		if( imPtr->alphaSrcImage != TC_UNUSED )
		{
			siPtr = TCFindSrcImageByIndex( imPtr->alphaSrcImage );
			dfPtr = TCReadFile( siPtr->fileName );

			TCMakeImAlphaLayer( dfPtr, &(imPtr->lyAlpha) );
		}

        // if color index == alpha index, ReadFile will simply
		// return the cached file
		siPtr = TCFindSrcImageByIndex( imPtr->colorSrcImage );
		dfPtr = TCReadFile( siPtr->fileName );

		// make the color layer from the decoded file.
		TCMakeImColorLayer( dfPtr, &(imPtr->lyColor) );

		// perform any required layer conversions:
		// there are only 2 color layer types at this point: RGB24 and CI16
        // color info. must be present; alpha may not be		
		switch( imPtr->lyColor.type )
		{
	    // if layer format is CI16 but conversion format is
		// truecolor, convert the CI layer to an rgb format.
		case LY_IMAGE_COLOR_CI16:

			switch( imPtr->texelFormat )
			{
			case TPL_IMAGE_TEXEL_FMT_I4:		// all fall through
			case TPL_IMAGE_TEXEL_FMT_I8:

			case TPL_IMAGE_TEXEL_FMT_IA4:
			case TPL_IMAGE_TEXEL_FMT_IA8:

			case TPL_IMAGE_TEXEL_FMT_R5G6B5:

			case TPL_IMAGE_TEXEL_FMT_RGB5A3:
			case TPL_IMAGE_TEXEL_FMT_RGBA8:

			case TPL_IMAGE_TEXEL_FMT_CMP:  

				TCConvertCI_To_RGB( dfPtr, imPtr );
				break;
			}
			break;

		// if layer format is RGB24, and destination format is CI,
		// tc cannot convert it.
		case LY_IMAGE_COLOR_RGB24:

			switch( imPtr->texelFormat )
			{
			case TPL_IMAGE_TEXEL_FMT_CI4:		// all fall through
			case TPL_IMAGE_TEXEL_FMT_CI8:
			case TPL_IMAGE_TEXEL_FMT_CI14_X2:

				TCErrorMsg( "WriteTplImageBank: attempt to convert an RGB image %s to CI format\n", dfPtr->name );
				return 0;
				break;
			}
			break;

		} // end switch( imPtr->lyColor.type )

		// once pre-conversion is complete, convert and write the image
		bytesWritten += TCWriteTplImageMipMaps( fp, imPtr );

		// free up image layer memory for the next pass
		if( imPtr->lyColor.data != NULL )
		{
			TCFree( (void**)(&imPtr->lyColor.data) );
			imPtr->lyColor.data = NULL;
		}
		if( imPtr->alphaSrcImage != 0 )
		{
			if( (imPtr->lyAlpha).data != NULL )
			{
				TCFree( (void**)(&imPtr->lyAlpha.data) );
				imPtr->lyAlpha.data = NULL;
			}
		}

    	imPtr = imPtr->next;
    	
    }  // end while

    return bytesWritten;  
}

/*>*******************************(*)*******************************<*/ 
// WriteTplImage_I4
//
// convert from layer to final hw format
// 8x8 texel tiles @ 4B per row, 32B per tile
//
// note: intensity comes in 3 flavors:
//       LY_IMAGE_COLOR_I8, LY_IMAGE_COLOR_RGB24, LY_IMAGE_COLOR_CI16
//       for color-indexed, use the indices directly as intensity values
/*>*******************************(*)*******************************<*/ 
void TCWriteTplImage_I4 ( TCLayer* colorLayer, u8* tplBuffer )
{
	u32 numTileRows, tileRow;	
	u32 numTileCols, tileCol;
	u8* dstPtr;
	u32 width, height;


	width  = colorLayer->width;
	height = colorLayer->height;
 		
	// number of 8x8 texel tile cols, rows including any partial tiles
	numTileCols = ((width  + 7) >> 3);
	numTileRows = ((height + 7) >> 3);

	dstPtr = tplBuffer;
	
	// numTileRows, numTileCols includes any partial tiles
	for( tileRow=0; tileRow<numTileRows; tileRow++ )
	{		
		for(tileCol=0; tileCol<numTileCols; tileCol++)
		{						
			TCPackTile_I4( colorLayer, (tileCol * 8), (tileRow * 8), dstPtr);
			dstPtr += 32;                  // next 32B cache line
		}			
	} 
}
	
/*>*******************************(*)*******************************<*/ 
// TCPackTile_I4
//
// 8x8 tile
// x and y represent starting texel position of this tile
/*>*******************************(*)*******************************<*/ 
static void TCPackTile_I4 ( TCLayer* srcLayer, u32 x, u32 y, u8* dstPtr )
{
	u16 ria;
	u8  g, b;
	u32 row, col;
	u32 realRows, realCols;
	u8* tilePtr;

    
	// dstPtr is already zeroed out, so this will take care of padding issue
	// 'realRows', 'realCols' represent actual source image texels remaining
	realRows = srcLayer->height - y;
	realCols = srcLayer->width  - x;
	
	if( realRows > 8)    
		realRows = 8;
		
	if(realCols > 8)
		realCols = 8;
		
	// pack 32B tile from most-sig. texel to least sig. texel
	for(row=0; row<realRows; row++)
	{	
		tilePtr = dstPtr + (row * 4);                       // move 4 bytes (8 4-bit texels) per row
		                                                    // need to reset ptr each row to account for
		                                                    // column padding
		for(col=0; col<realCols; col++)
		{
			
			TCGetLayerValue( srcLayer, (x+col), (y+row), &ria, &g, &b );     
			
			// for LY_IMAGE_COLOR_CI16, use low 4 bits of ria (0 to 15).
			// for LY_IMAGE_COLOR_RGB24, average the 3 color values
			if( srcLayer->type == LY_IMAGE_COLOR_RGB24 )
			{
				ria = ( ( ria + g + b ) / 3 );
			}
			else if( srcLayer->type == LY_IMAGE_COLOR_CI16 )
			{
				ria <<= 4;
			}

			if( (col % 2) == 0 )                            // on even iterations, pack the high 4-bits
			{
				*tilePtr  = (ria & 0x00F0);	
			}
			else                                            // on odd iterations, pack the high 4-bits
			{                                           // and move to the next byte
				*tilePtr |= ((ria & 0x00F0) >> 4);
				tilePtr++;
			}
		}				
	}				
}
	
	
/*>*******************************(*)*******************************<*/ 
// WriteTplImage_I8
//
// convert from layer to final hw format
// 4x8 texel tiles @ 8B per row, 32B per tile
//
// note:  color layer comes in 3 formats: 
//        LY_IMAGE_COLOR_I8, LY_IMAGE_COLOR_RGB24, LY_IMAGE_COLOR_CI16.
//        for indexed color, use the index directly as the 
//		  intensity value
/*>*******************************(*)*******************************<*/ 
void TCWriteTplImage_I8 ( TCLayer* colorLayer, u8* tplBuffer )
{
	u32 numTileRows, tileRow;	
	u32 numTileCols, tileCol;
	u8* dstPtr;
	u32 width, height;


	width  = colorLayer->width;
	height = colorLayer->height;
 		
	// number of 4x8 texel tile cols, rows including any partial tiles
	numTileCols = ((width  + 7) >> 3);
	numTileRows = ((height + 3) >> 2);
	
	dstPtr = tplBuffer;
	
	// numTileRows, numTileCols includes any partial tiles
	for( tileRow=0; tileRow<numTileRows; tileRow++ )
	{		
		for(tileCol=0; tileCol<numTileCols; tileCol++)
		{						
			TCPackTile_I8( colorLayer, (tileCol * 8), (tileRow * 4), dstPtr);
			dstPtr += 32;                  // next 32B cache line
		}			
	}
}
	
/*>*******************************(*)*******************************<*/ 
// TCPackTile_I8
//
// 4x8 tile
// x and y represent starting texel position of this tile
/*>*******************************(*)*******************************<*/ 
static void TCPackTile_I8 ( TCLayer* srcLayer, u32 x, u32 y, u8* dstPtr )
{
	u16 ria;
	u8 g, b;
	u32 row, col;
	u32 realRows, realCols;
	u8* tilePtr;

    
	// dstPtr is already zeroed out, so this will take care of padding issue
	// 'realRows', 'realCols' represent actual source image texels remaining
	realRows = srcLayer->height - y;
	realCols = srcLayer->width  - x;
	
	if( realRows > 4)    
		realRows = 4;
		
	if(realCols > 8)
		realCols = 8;
		
	// pack 32B tile 
	for(row=0; row<realRows; row++)
	{	
		tilePtr = dstPtr + (row * 8);                       // move 8 bytes (8 8-bit texels) per row
		                                                    // need to reset ptr each row to account for
		                                                    // column padding
		for(col=0; col<realCols; col++)
		{			
			TCGetLayerValue( srcLayer, (x+col), (y+row), &ria, &g, &b );     // fetch an 8-bit intensity value		

			// for LY_IMAGE_COLOR_CI16, use ria (index) directly as intensity value.
			// for LY_IMAGE_COLOR_RGB24, average the 3 color values
			if( srcLayer->type == LY_IMAGE_COLOR_RGB24 )
			{
				ria = ( ( ria + g + b ) / 3 );
			}

			*tilePtr = (unsigned char)ria;
			tilePtr++;
		}				
	}				
}
	
/*>*******************************(*)*******************************<*/ 
// WriteTplImage_IA4
//
// convert from layer to final hw format
// 4x8 texel tiles @ 4B per row, 32B per tile
//
// note:  source color layer must be in the format LY_IMAGE_COLOR_I8
//        if no alpha layer provided, all alphas set to max. value
/*>*******************************(*)*******************************<*/ 
void TCWriteTplImage_IA4 ( TCLayer* colorLayer, TCLayer* alphaLayer, u8* tplBuffer )
{	
	u32 numTileRows, tileRow;	
	u32 numTileCols, tileCol;
	u8* dstPtr;
	u32 width, height;


	width  = colorLayer->width;
	height = colorLayer->height;
 					
	// number of 4x8 texel tile cols, rows including any partial tiles
	numTileCols = ((width  + 7) >> 3);
	numTileRows = ((height + 3) >> 2);
	
	dstPtr = tplBuffer;
	
	// numTileRows, numTileCols includes any partial tiles
	for( tileRow=0; tileRow<numTileRows; tileRow++ )
	{		
		for(tileCol=0; tileCol<numTileCols; tileCol++)
		{						
			TCPackTile_IA4( colorLayer, alphaLayer, (tileCol * 8), (tileRow * 4), dstPtr);
			dstPtr += 32;                  // next 32B cache line
		}			
	} 	
}
	
/*>*******************************(*)*******************************<*/ 
// TCPackTile_IA4
//
// 4x8 tile
// x and y represent starting texel position of this tile
/*>*******************************(*)*******************************<*/ 
static void TCPackTile_IA4 ( TCLayer* colorLayer, TCLayer* alphaLayer, u32 x, u32 y, u8* dstPtr )
{
	u16 ria, a;
	u8 g, b;
	u32 row, col;
	u32 realRows, realCols;
	u8* tilePtr;
	

	// dstPtr is already zeroed out, so this will take care of padding issue
	// 'realRows', 'realCols' represent actual source image texels remaining
	realRows = colorLayer->height - y;
	realCols = colorLayer->width  - x;
	
	if( realRows > 4)    
		realRows = 4;
		
	if(realCols > 8)
		realCols = 8;
		
	// pack 32B tile 
	for(row=0; row<realRows; row++)
	{	
		tilePtr = dstPtr + (row * 8);                       // move 8 bytes (8 8-bit texels) per row
		                                                    // need to reset ptr each row to account for
		                                                    // column padding
		for(col=0; col<realCols; col++)
		{		
			if(alphaLayer)
			{
				TCGetLayerValue( alphaLayer, (x+col), (y+row), &a, 0, 0 );				
			}		
			else  // set high 4 bits to max. alpha
			{
				a = 0x00F0;
			}
			
			TCGetLayerValue( colorLayer, (x+col), (y+row), &ria, &g, &b );

			// for LY_IMAGE_COLOR_CI16, use low 4 bits of ria.
			// for LY_IMAGE_COLOR_RGB24, average the 3 color values
			if( colorLayer->type == LY_IMAGE_COLOR_RGB24 )
			{
				ria = ( ( ria + g + b ) / 3 );
			}
			else if( colorLayer->type == LY_IMAGE_COLOR_CI16 )
			{
				ria <<= 4;
			}
			
			*tilePtr  = ( (a & 0xF0) | ((ria & 0x00F0) >> 4) );	// high 4-bits are alpha, low 4-bits are intensity  
			tilePtr++;				
		} // end for col loop		
	} // end for row loop					
}
	
/*>*******************************(*)*******************************<*/ 
// WriteTplImage_IA8
//
// convert from layer to final hw format
// 4x4 texel tiles @ 8B per row, 32B per tile
//
// note:  source color layer must be in the format LY_IMAGE_COLOR_I8.
//        if no alpha layer found, set all alphas to max. value
/*>*******************************(*)*******************************<*/ 
void TCWriteTplImage_IA8 ( TCLayer* colorLayer, TCLayer* alphaLayer, u8* tplBuffer )
{
	u32 numTileRows, tileRow;	
	u32 numTileCols, tileCol;
	u8* dstPtr;
	u32 width, height;


	width  = colorLayer->width;
	height = colorLayer->height;
 			
	// number of 4x4 texel tile cols, rows including any partial tiles
	numTileCols = ((width  + 3) >> 2);
	numTileRows = ((height + 3) >> 2);

	dstPtr = tplBuffer;
	
	// numTileRows, numTileCols includes any partial tiles
	for( tileRow=0; tileRow<numTileRows; tileRow++ )
	{		
		for(tileCol=0; tileCol<numTileCols; tileCol++)
		{			
			TCPackTile_IA8( colorLayer, alphaLayer, (tileCol * 4), (tileRow * 4), dstPtr);
			dstPtr += 32;                  // next 32B cache line
		}			
	} 	
}
	
/*>*******************************(*)*******************************<*/ 
// TCPackTile_IA8
//
// 4x4 tile, 16-bit texels
// x and y represent starting texel position of this tile
/*>*******************************(*)*******************************<*/ 
static void TCPackTile_IA8 ( TCLayer* colorLayer, TCLayer* alphaLayer, u32 x, u32 y, u8* dstPtr)
{
	u16 ria;
	u8 g, b;
	u32 row, col;
	u32 realRows, realCols;
	u8* tilePtr;
	

	// dstPtr is already zeroed out, so this will take care of padding issue
	// 'realRows', 'realCols' represent actual source image texels remaining
	realRows = colorLayer->height - y;
	realCols = colorLayer->width  - x;
	
	if( realRows > 4)    
		realRows = 4;
		
	if(realCols > 4)
		realCols = 4;
			
	// pack 32B tile 
	for(row=0; row<realRows; row++)
	{	
		tilePtr = dstPtr + (row * 8);                       // move 8 bytes (4 16-bit texels) per row
		                                                    // need to reset ptr each row to account for
		                                                    // column padding
		for(col=0; col<realCols; col++)
		{
			if(alphaLayer)                                  // alpha is byte 0
			{
				TCGetLayerValue( alphaLayer, (x+col), (y+row), &ria, 0, 0 );	
					
				*tilePtr = (u8)ria;			
			}	
			else  // set byte 0 to max. alpha
			{
				*tilePtr = 0xFF;
			}
					                                       // color is byte 1     
			TCGetLayerValue( colorLayer, (x+col), (y+row), &ria, &g, &b );

			// for LY_IMAGE_COLOR_CI16, use ria (index) directly as intensity value.
			// for LY_IMAGE_COLOR_RGB24, average the 3 color values
			if( colorLayer->type == LY_IMAGE_COLOR_RGB24 )
			{
				ria = ( ( ria + g + b ) / 3 );
			}

			*(tilePtr + 1) = (u8)ria;
			
			tilePtr += 2;		
		} // end for col loop			
	} // end for row loop
}
	
/*>*******************************(*)*******************************<*/ 
// WriteTplImage_R5G6B5
//
// convert from layer to final hw format
// 4x4 texel tiles @ 8B per row, 32B per tile
// note:  color layer must be in LY_IMAGE_COLOR_RGB24 format
/*>*******************************(*)*******************************<*/ 
void TCWriteTplImage_R5G6B5 ( TCLayer* colorLayer, u8* tplBuffer )
{	
	u32 numTileRows, tileRow;	
	u32 numTileCols, tileCol;
	u8* dstPtr;
	u32 width, height;


	width  = colorLayer->width;
	height = colorLayer->height;
 				
	// number of 4x4 texel tile cols, rows including any partial tiles
	numTileCols = ((width  + 3) >> 2);
	numTileRows = ((height + 3) >> 2);
	
	dstPtr = tplBuffer;
	
	// numTileRows, numTileCols includes any partial tiles
	for( tileRow=0; tileRow<numTileRows; tileRow++ )
	{	
		for(tileCol=0; tileCol<numTileCols; tileCol++)
		{			
			TCPackTile_R5G6B5( colorLayer, (tileCol * 4), (tileRow * 4), dstPtr);
			dstPtr += 32;                  // next 32B cache line
		}			
	} 	
}
	
/*>*******************************(*)*******************************<*/ 
// TCPackTile_R5G6B5
//
// 4x4 tile, 16-bit texels
// x and y represent starting texel position of this tile
/*>*******************************(*)*******************************<*/ 
static void TCPackTile_R5G6B5 ( TCLayer* colorLayer, u32 x, u32 y, u8* dstPtr)
{
	u16 ria;
	u32 row, col;
	u32 realRows, realCols;
	u8* tilePtr;
	u8 g, b;
	

	// dstPtr is already zeroed out, so this will take care of padding issue
	// 'realRows', 'realCols' represent actual source image texels remaining
	realRows = colorLayer->height - y;
	realCols = colorLayer->width  - x;
		
	if( realRows > 4)    
		realRows = 4;
		
	if(realCols > 4)
		realCols = 4;
		   			
	// pack 32B tile 
	for(row=0; row<realRows; row++)
	{	
		tilePtr = dstPtr + (row * 8);                       // move 8 bytes (4 16-bit texels) per row
		                                                    // need to reset ptr each row to account for	                                                    // column padding
		for(col=0; col<realCols; col++)
		{
			TCGetLayerValue( colorLayer, (x+col), (y+row), &ria, &g, &b );
			
			*tilePtr       = ( (ria & 0x00F8)       | ((g & 0xE0) >> 5) );  // byte0 is upper 5 bits of red, upper 3 of green
			*(tilePtr + 1) = ( ((g  & 0x1C)   << 3) | ( b >> 3)         );  // byte1 is lower 3 bits of green, upper 5 of blue
			
			tilePtr += 2;	
		} 		
	} 
}
	
/*>*******************************(*)*******************************<*/ 
// WriteTplImage_RGB5A3
//
// convert from layer to final hw format
// 4x4 texel tiles @ 8B per row, 32B per tile
//
// note:  source color layer must be in the format LY_IMAGE_COLOR_RGB24.
//        if no alpha layer found, set all alphas to max. value
/*>*******************************(*)*******************************<*/ 
void TCWriteTplImage_RGB5A3 ( TCLayer* colorLayer, TCLayer* alphaLayer, u8* tplBuffer )
{	
	u32 numTileRows, tileRow;	
	u32 numTileCols, tileCol;
	u8* dstPtr;
	u32 width, height;


	width  = colorLayer->width;
	height = colorLayer->height;
 		
	// number of 4x4 texel tile cols, rows including any partial tiles
	numTileCols = ((width  + 3) >> 2);
	numTileRows = ((height + 3) >> 2);
	
	dstPtr = tplBuffer;
	
	// numTileRows, numTileCols includes any partial tiles
	for( tileRow=0; tileRow<numTileRows; tileRow++ )
	{
		for(tileCol=0; tileCol<numTileCols; tileCol++)
		{			
			TCPackTile_RGB5A3( colorLayer, alphaLayer, (tileCol * 4), (tileRow * 4), dstPtr);
			dstPtr += 32;                  // next 32B cache line
		}			
	} 	
}
	
/*>*******************************(*)*******************************<*/ 
// TCPackTile_RGB5A3
//
// 4x4 tile, 16-bit texels
// x and y represent starting texel position of this tile
/*>*******************************(*)*******************************<*/ 
static void TCPackTile_RGB5A3 ( TCLayer* colorLayer, TCLayer* alphaLayer, u32 x, u32 y, u8* dstPtr )
{
	u32 row, col;
	u32 realRows, realCols;
	u8* tilePtr;
	u8 g, b;
	u16 ria, a;
	

	// dstPtr is already zeroed out, so this will take care of padding issue
	// 'realRows', 'realCols' represent actual source image texels remaining
	realRows = colorLayer->height - y;
	realCols = colorLayer->width  - x;
	
	if( realRows > 4)    
		realRows = 4;
		
	if(realCols > 4)
		realCols = 4;
				     			
	// pack 32B tile 
	for(row=0; row<realRows; row++)
	{
		tilePtr = dstPtr + (row * 8);                       // move 8 bytes (4 16-bit texels) per row
		                                                    // need to reset ptr each row to account for
                                                            // column padding
		for(col=0; col<realCols; col++)
		{
            if(alphaLayer)
            {
				TCGetLayerValue( alphaLayer, (x+col), (y+row), &a, 0, 0 );		
			}
			else  
			{
				a = 0x00FF;   
			}
			
			TCGetLayerValue( colorLayer, (x+col), (y+row), &ria, &g, &b );
				
			// check alpha to determine whether to pack color 5551 or 4443 
			
			// since hw replicates msbs, everything >= 224 of original alpha 
			// will unpack as 255 ( 1110 0000 unpacks as 1111 1111 )

			if( a >= 224 ) // pixel is opaque
			{	
				// pack in 5551 format, msb is set to 1
				*tilePtr       = ( (0x0080)          | ((ria & 0xF8) >> 1) | (g >> 6) );  // byte0 is 1 bit alpha, upper 5-bits 
			                                                                              // of red, upper 2-bits of green
				*(tilePtr + 1) = ( ((g & 0x38) << 2) | (b >> 3) );                        // byte1 is bits 3-5 of green, upper 5 of blue		
			}
			else           // pixel is translucent
			{
				// pack in 4443 format,  shift alpha by 1 and set msb to 0					
				*tilePtr       = ( ( (a >> 1) & 0x70 ) | ((ria & 0xF0)   >> 4) );  // byte0 is 1 bit 0, 3 alpha, 4-bits red 
			                                                                     
				*(tilePtr + 1) = ( (g & 0xF0)          | ((b   & 0xF0)   >> 4) );  // 4-bits green, 4-bits blue	
			}
						
			tilePtr += 2;
			
		} // end for col loop		
	} // end for row loop
}
	
/*>*******************************(*)*******************************<*/ 
// WriteTplImage_RGBA8
//
// convert from layer to final hw format
// 2 4x4 texel tiles @ 8B per tile row, 32B per tile, 2 cache lines (64B) total
//
// AR tiles are stored contiguously in the low half of image->tplBuffer;
// GB tiles are stored contiguously in the high half of image->tplBuffer;
//
// note:  source color layer must be in the format LY_IMAGE_COLOR_RGB24.
//        if no alpha layer found, set all alphas to max. value
/*>*******************************(*)*******************************<*/ 
void TCWriteTplImage_RGBA8 ( TCLayer* colorLayer, TCLayer* alphaLayer, u8* tplBuffer )
{	
	u32 numTileRows, tileRow;	
	u32 numTileCols, tileCol;
	u8* dstPtr;
	u32 width, height;


	width  = colorLayer->width;
	height = colorLayer->height;
 	
	// number of 4x4 texel tile cols, rows including any partial tiles
	numTileCols = ((width  + 3) >> 2);
	numTileRows = ((height + 3) >> 2);
	
	dstPtr  = tplBuffer;
	
	// numTileRows, numTileCols includes any partial tiles
	for( tileRow=0; tileRow<numTileRows; tileRow++ )
	{
		for(tileCol=0; tileCol<numTileCols; tileCol++)
		{						
			TCPackTile_RGBA8( colorLayer, alphaLayer, (tileCol * 4), (tileRow * 4), dstPtr );
			dstPtr  += 64;                  // move to next 2 (32B) cache lines
		}			
	} 
}
	
/*>*******************************(*)*******************************<*/
// TCPackTile_RGBA8 
// 
// 4x4 tile, 16-bit texels
// x and y represent starting texel position of this tile
// pack AR in low half, GB in high half dest. buffer 
/*>*******************************(*)*******************************<*/ 
static void TCPackTile_RGBA8 ( TCLayer* colorLayer, TCLayer* alphaLayer, u32 x, u32 y, u8* dstPtr )
{
	u16 ria, a;
	u32 row, col;
	u32 realRows, realCols;
	u8* arPtr, *gbPtr;
	u8 g, b;
	

	// dstPtr is already zeroed out, so this will take care of padding issue
	// 'realRows', 'realCols' represent actual source image texels remaining
	realRows = colorLayer->height - y;
	realCols = colorLayer->width  - x;
	
	if( realRows > 4)    
		realRows = 4;
		
	if(realCols > 4)
		realCols = 4;
			
	// pack 2 32B tiles
	for(row=0; row<realRows; row++)
	{	
															// pack 2 cache lines at once
		arPtr = dstPtr  +      (row * 8);                   // move 8 bytes (4 16-bit texels) per row
		gbPtr = dstPtr  + 32 + (row * 8);                   // need to reset ptr each row to account for
		                                                    // column padding

		for(col=0; col<realCols; col++)
		{               
			TCGetLayerValue( colorLayer, (x+col), (y+row), &ria, &g, &b );
			
			if(alphaLayer)                                  
			{
				TCGetLayerValue( alphaLayer, (x+col), (y+row), &a, 0, 0 );			
			}	
			else  // set to max. alpha
			{
				a = 0xFF;
			}
				
			*arPtr       = (u8)a;                           // alpha is byte 0, red is byte 1
			*(arPtr + 1) = (u8)ria;
			
			*gbPtr       = g;                               // green is byte 0, blue is byte 1
			*(gbPtr + 1) = b;
			
			arPtr += 2;
			gbPtr += 2;

		} // end for col loop			
	} // end for row loop						
}
	
/*>*******************************(*)*******************************<*/ 
// WriteTplImage_CI4
//
// convert from layer to final hw format
// 8x8 texel tiles @ 4B per row, 32B per tile
//
// note:  source color layer must be in the format LY_IMAGE_COLOR_CI8
/*>*******************************(*)*******************************<*/ 
void TCWriteTplImage_CI4 ( TCLayer* colorLayer, u8* tplBuffer )
{	
	u32 numTileRows, tileRow;	
	u32 numTileCols, tileCol;
	u8* dstPtr;
	u32 width, height;


	width  = colorLayer->width;
	height = colorLayer->height;
 		
	// number of 8x8 texel tile cols, rows including any partial tiles
	numTileCols = ((width  + 7) >> 3);
	numTileRows = ((height + 7) >> 3);
	
	dstPtr = tplBuffer;
	
	// numTileRows, numTileCols includes any partial tiles
	for( tileRow=0; tileRow<numTileRows; tileRow++ )
	{
		for(tileCol=0; tileCol<numTileCols; tileCol++)
		{				
			TCPackTile_CI4( colorLayer, (tileCol * 8), (tileRow * 8), dstPtr);
			dstPtr += 32;                  // next 32B cache line
		}			
	} 
}
	
/*>*******************************(*)*******************************<*/ 
// TCPackTile_CI4
//
// 8x8 tile
// x and y represent starting texel position of this tile
// assume the 8-bit layer indices only run from 0 to 15,
// so take the low 4 bits as is.
/*>*******************************(*)*******************************<*/ 
static void TCPackTile_CI4 ( TCLayer* srcLayer, u32 x, u32 y, u8* dstPtr )
{
	u16 ria;
	u32 row, col;
	u32 realRows, realCols;
	u8* tilePtr;
	

	// dstPtr is already zeroed out, so this will take care of padding issue
	// 'realRows', 'realCols' represent actual source image texels remaining
	realRows = srcLayer->height - y;
	realCols = srcLayer->width  - x;
	
	if( realRows > 8)    
		realRows = 8;
		
	if(realCols > 8)
		realCols = 8;
		
	// pack 32B tile 
	for(row=0; row<realRows; row++)
	{	
		tilePtr = dstPtr + (row * 4);                       // move 4 bytes (8 4-bit texels) per row
		                                                    // need to reset ptr each row to account for
		                                                    // column padding
		for(col=0; col<realCols; col++)
		{		
			TCGetLayerValue( srcLayer, (x+col), (y+row), &ria, 0, 0 );     // fetch an 8-bit color-index value
			                                                          // and keep just the low 4 bits.
			if( col %2 == 0 )
			{
				*tilePtr = ((ria & 0x000F) << 4);
			}

			else
			{
				*tilePtr |= (ria & 0x000F);
				tilePtr++;
			}	
		}		
	}				
}
	
/*>*******************************(*)*******************************<*/ 
// WriteTplImage_CI8
//
// convert from layer to final hw format
// 4x8 texel tiles @ 8B per row, 32B per tile
//
// note:  source color layer must be in the format LY_IMAGE_COLOR_CI8
/*>*******************************(*)*******************************<*/ 
void TCWriteTplImage_CI8 ( TCLayer* colorLayer, u8* tplBuffer )
{	
	u32 numTileRows, tileRow;	
	u32 numTileCols, tileCol;
	u8* dstPtr;	
	u32 width, height;


	width  = colorLayer->width;
	height = colorLayer->height;
 
	// number of 4x8 texel tile cols, rows including any partial tiles
	numTileCols = ((width  + 7) >> 3);
	numTileRows = ((height + 3) >> 2);
		
	dstPtr = tplBuffer;
	
	// numTileRows, numTileCols includes any partial tiles
	for( tileRow=0; tileRow<numTileRows; tileRow++ )
	{		
		for(tileCol=0; tileCol<numTileCols; tileCol++)
		{				
			TCPackTile_CI8( colorLayer, (tileCol * 8), (tileRow * 4), dstPtr);
			dstPtr += 32;                  // next 32B cache line
		}			
	} 
}
	
/*>*******************************(*)*******************************<*/ 
// TCPackTile_CI8
//
// 4x8 tile
// x and y represent starting texel position of this tile
/*>*******************************(*)*******************************<*/ 
static void TCPackTile_CI8 ( TCLayer* srcLayer, u32 x, u32 y, u8* dstPtr )
{
	u16 ria;
	u32 row, col;
	u32 realRows, realCols;
	u8* tilePtr;
	

	// dstPtr is already zeroed out, so this will take care of padding issue
	// 'realRows', 'realCols' represent actual source image texels remaining
	realRows = srcLayer->height - y;
	realCols = srcLayer->width  - x;
	
	if( realRows > 4)    
		realRows = 4;
		
	if(realCols > 8)
		realCols = 8;
		
	// pack 32B tile 
	for(row=0; row<realRows; row++)
	{	
		tilePtr = dstPtr +      (row * 8);                  // move 8 bytes (8 8-bit texels) per row
		                                                    // need to reset ptr each row to account for
		                                                    // column padding
		for(col=0; col<realCols; col++)
		{			
			TCGetLayerValue( srcLayer, (x+col), (y+row), &ria, 0, 0 );     // fetch an 8-bit color index value
			
			*tilePtr = (unsigned char)ria;
			tilePtr++;
		}				
	}				
}
	
/*>*******************************(*)*******************************<*/ 
// WriteTplImage_CI12_A4
// 
// convert from layer to final hw format
// 4x4 texel tiles @ 8B per row, 32B per tile
//
// note:  source color layer must be in the format LY_IMAGE_COLOR_CI16
/*>*******************************(*)*******************************<*/ 
void TCWriteTplImage_CI14_X2 ( TCLayer* colorLayer, u8* tplBuffer )
{	
	u32 numTileRows, tileRow;	
	u32 numTileCols, tileCol;
	u8* dstPtr;
	u32 width, height;


	width  = colorLayer->width;
	height = colorLayer->height;
 		
	// number of 4x4 texel tile cols, rows including any partial tiles
	numTileCols = ((width  + 3) >> 2);
	numTileRows = ((height + 3) >> 2);
	
	dstPtr = tplBuffer;
	
	// numTileRows, numTileCols includes any partial tiles
	for( tileRow=0; tileRow<numTileRows; tileRow++ )
	{
		for(tileCol=0; tileCol<numTileCols; tileCol++)
		{			
			TCPackTile_CI14_X2( colorLayer, (tileCol * 4), (tileRow * 4), dstPtr);
			dstPtr += 32;                  // next 32B cache line
		}			
	} 
}
	
/*>*******************************(*)*******************************<*/ 
// TCPackTile_CI14_X2
//
// 4x4 tile, 16-bit texels
// x and y represent starting texel position of this tile
//
// note: for color index, although 16-bits are read, we are only 
// expecting indices to reach a max. of 12-bits; no down-shifting 
// of lower 4-bits is used
/*>*******************************(*)*******************************<*/ 
static void TCPackTile_CI14_X2 ( TCLayer* colorLayer, u32 x, u32 y, u8* dstPtr )
{
	u16 ria;
	u32 row, col;
	u32 realRows, realCols;
	u16* tilePtr;
		

	// dstPtr is already zeroed out, so this will take care of padding issue
	// 'realRows', 'realCols' represent actual source image texels remaining
	realRows = colorLayer->height - y;
	realCols = colorLayer->width  - x;
	
	if( realRows > 4)    
		realRows = 4;
		
	if(realCols > 4)
		realCols = 4;
			
	// pack 32B tile 
	for(row=0; row<realRows; row++)
	{
		tilePtr = (u16*)( (u8*)dstPtr + (row * 8) );        // move 8 bytes (4 16-bit texels) per row
		                                                    // need to reset ptr each row to account for
		                                                    // column padding
		for(col=0; col<realCols; col++)
		{															     
			TCGetLayerValue( colorLayer, (x+col), (y+row), &ria, 0, 0 );
			 
			// keep only the low 14 bits
			ria = ria & 0x3FFF;

			// pack in big-endian format
			TCFixEndian( (u8*)(&ria), 2 );   
			*tilePtr++ = ria;             						
		} 				
	} 						
}
	
/*>*******************************(*)*******************************<*/ 
// TCWriteTplImage_CMP
//
/*>*******************************(*)*******************************<*/ 
void TCWriteTplImage_CMP ( TCLayer* colorLayer, u8* tplBuffer )
{
	u32 tileRow, tileCol;
	u32 srcTileRows, srcTileCols;
	u16* dstPtr;

	// each source tile is 4x4 texels, 8B
	srcTileRows   = ((colorLayer->height + 3) >> 2);
	srcTileCols   = ((colorLayer->width  + 3) >> 2);

	dstPtr = (u16*)(tplBuffer);

	// each dst tile is 2x2 source tiles, so move by 2 each iteration
	for(tileRow = 0; tileRow < srcTileRows; tileRow += 2 )
	{
		for(tileCol = 0; tileCol < srcTileCols; tileCol += 2 )
		{
			TCPackTile_CMP( colorLayer, tileCol, tileRow, dstPtr );
			dstPtr += 16; // 32B per dst tile, short ptr
		}
	}
}

/*>*******************************(*)*******************************<*/ 
// TCPackTile_CMP
//
// pack a 2x2 tile block, each tile of 4x4 texels, into a single 
// 32B dst tile note: this assumes s3 algorithm pads out to a minimum 
// block size of 4x4 texels
/*>*******************************(*)*******************************<*/ 
static void TCPackTile_CMP ( TCLayer* layer, u32 tileX, u32 tileY, u16* dstPtr)
{
	u32  x, y;
	u16* srcPtr;
	u16  tmp;
	u32  srcTileOffset;
	u32  subTileRows, subRowShorts;    // number of s3 4x4 tiles
	u32  srcPadWidth, srcPadHeight;
	u16* buffPtr;

	// set the padded size of the s3 source image out to a 4-texel boundary
	srcPadWidth  = ( (layer->width  + 3) >> 2 );
	srcPadHeight = ( (layer->height + 3) >> 2 );

	// number of bytes in a single row of 4x4 texel source tiles
	srcTileOffset = srcPadWidth * 8;

	// number of 4x4 (source) tile rows to copy ( will be 1 or 2 )
	subTileRows = 2;
	if( (srcPadHeight - tileY) < 2 )
		subTileRows = 1;

	// number of 4x4 tile cols to copy translated into number of short values
	// ( will be 4 or 8 )
	subRowShorts = 8;
	if( (srcPadWidth - tileX) < 2 )
		subRowShorts = 4;

	for( y=0; y < subTileRows; y++ )
	{
		srcPtr  = (u16*)( (u8*)(layer->data) + ((tileY + y) * srcTileOffset) + (tileX*8) ); 
		buffPtr = ( dstPtr + (y * 8) );        // 16 bytes per subRow = 8 shorts

		// process one or both 4x4 row tiles at once- 4 short each
		for( x=0; x < subRowShorts; x++ )
		{			
			switch( x )
			{

			// color table entries - switch bytes within a 16-bit world only
			case 0:	
			case 1:
			case 4:
			case 5:
				tmp = *srcPtr++;
				TCFixEndian( (u8*)(&tmp), 2 );
				*buffPtr++ = tmp;
				break;
			
			// 2-bit color tuples;
			// reverse tuple order within bytes of a word
			case 2:
			case 3:
			case 6:
			case 7:
				tmp = *srcPtr++;
				TCFixCMPWord( &tmp );
				*buffPtr++ = tmp;
				break;

			} // end switch
		} // end for( subRowShorts )			
	} // end for( subTileRows )
}

/*>*******************************(*)*******************************<*/
// switch tuple and byte order within 16-bit words of an s3-packed tile
// to match hw.
// 1) switch 2-bit tuple order within bytes
//    from ( 0,1,2,3 ) to ( 3,2,1,0 ).
// 2) leave byte order within the word as is.
/*>*******************************(*)*******************************<*/
static void TCFixCMPWord( u16* data )
{
	u16 tmp;


	tmp = *data;

	// reverse tuple order within bytes
	*data = ( (tmp & 0x3 )   << 6 ) |
			( (tmp & 0xC )   << 2 ) |
			( (tmp & 0x30)   >> 2 ) |
			( (tmp & 0xC0)   >> 6 ) |

            ( (tmp & 0x300 ) << 6 ) |
			( (tmp & 0xC00 ) << 2 ) |
			( (tmp & 0x3000) >> 2 ) |
			( (tmp & 0xC000) >> 6 ) ;
}

/*>*******************************(*)*******************************<*/ 
// get a reference date for the tpl file for use when comparing 
// source image mod. dates refTmPtr is the date structure to be filled
// fp is a ptr to the open tpl file dateType determines which type of 
// date to use 0 indicates file properties ( struct _stat type 
// accessed by _fstat ) 1 indicates a byte stream ( date field ) 
// within the tpl file
/*>*******************************(*)*******************************<*/ 
static void TCGetFileModTime ( const char* fileName, struct tm* modTmPtr )
{
	struct _stat statBuff; 
	struct tm*   localPtr = NULL;
	FILE*        fp;

	
	memset( modTmPtr, 0, sizeof(struct tm) );

	fp = fopen( fileName, "rb" );
	TCAssertMsg( (fp != NULL), "TCGetFileModTime: couldn't open file %s\n", fileName );


	// date is set from file properties
	if( ( _fstat( fp->_file, &statBuff )) != 0 )
	{
		TCErrorMsg( "TCGetFileModTime: error in _fstat() for file %s call\n", fileName );
		return;
	}

	// use file modification date
	localPtr  = localtime( &statBuff.st_mtime );	
	*modTmPtr = *localPtr;

	fclose( fp );
}

/*>*******************************(*)*******************************<*/ 
// set a 'struct tm' variable to the current time
/*>*******************************(*)*******************************<*/ 
static void TCGetTime ( struct tm* tmPtr )
{
	time_t tm;
	struct tm* localPtr = NULL;


	tm = time( NULL );

	localPtr = localtime( &tm );

	*tmPtr = *localPtr;
}

/*>*******************************(*)*******************************<*/ 
// determine the original access permission for a file
/*>*******************************(*)*******************************<*/ 
static s32 TCGetFileAccess ( const char* fileName )
{
	s32 axs = FILE_ACCESS_NONE;


	// check for file existence:
	// if the file doesn't exist, leave access as 0
	if( ( _access( fileName, 0 )) == -1 )
	{
		return FILE_ACCESS_NONE;
	}
	// otherwise, determine access
	else if( ( _access( fileName, 6 )) != -1 )  // read and write permission
	{
		axs = FILE_ACCESS_READ_WRITE;
	}
	else if( ( _access( fileName, 2 )) != -1 )	// write permission
	{
		axs = FILE_ACCESS_WRITE_ONLY;
	}
	else if( ( _access( fileName, 4 )) != -1 )	// read permission
	{
		axs = FILE_ACCESS_READ_ONLY;
	}

	return axs;
}

/*>*******************************(*)*******************************<*/ 
// set the access permission for a file
/*>*******************************(*)*******************************<*/ 
static void TCSetFileAccess ( const char* fileName, s32 axs )
{
	s32 pmode;


	// make sure file exists before calling _chmod
	if( (TCGetFileAccess ( fileName )) == FILE_ACCESS_NONE )
	{
		return;
	}

	switch( axs )
	{
	case FILE_ACCESS_READ_WRITE:
		pmode = ( _S_IREAD | _S_IWRITE );
		break;

	case FILE_ACCESS_READ_ONLY:
		pmode = _S_IREAD;
		break;

	case FILE_ACCESS_WRITE_ONLY:
		pmode = _S_IWRITE;
		break;

	case FILE_ACCESS_NONE:	// if 'no access', 
	default:                // or if unknown axs flag, leave the current permission as is.
		return;
		break;
	}

	if( ( _chmod( fileName, pmode )) != 0 )
	{
		TCErrorMsg( "TCSetFileAccess: couldn't change file %s permissions\n", fileName );
		return;
	}
}

/*>*******************************(*)*******************************<*/ 
// set the modification date of a file after it has been closed.
// we use a closed file so an explicit time can be set without regard to
// the behavior of fclose().
/*>*******************************(*)*******************************<*/ 
static void TCSetFileModTime ( const char* fileName, struct tm* refTm )
{
	s32             saveAxs;
	struct _utimbuf utBuff;
	struct _stat    stBuff;


	// save the original file access level
	if( (saveAxs = TCGetFileAccess( fileName )) == FILE_ACCESS_NONE )
	{
		TCErrorMsg( "TCSetFileModTime: file %s does not exist\n", fileName );
		return;
	}

	// set file access to read/write
	TCSetFileAccess( fileName, FILE_ACCESS_READ_WRITE );

	// get the access, modification stats of this file
	_stat( fileName, &stBuff );

	// save the access time; set the modification time to 'refTm'
	utBuff.actime  = stBuff.st_atime;
	utBuff.modtime = mktime( refTm );

	// update the modification time
	_utime( fileName, &utBuff );

	// restore the original access level
	TCSetFileAccess( fileName, saveAxs );
}

/*>*******************************(*)*******************************<*/ 
// compare two struct tms for year, year-day, hour, min, sec:

// return  1 if time 1 is  > time 2
// return  0 if time 1 is  = time 2
// return -1 if time 1 is  < time 2
/*>*******************************(*)*******************************<*/ 
static s32 TCCompareTime ( struct tm* time1, struct tm* time2 )
{

	if( time1->tm_year > time2->tm_year )
		return 1;
	else if( time1->tm_year < time2->tm_year )
		return -1;

	if( time1->tm_yday > time2->tm_yday )
		return 1;
	else if( time1->tm_yday < time2->tm_yday )
		return -1;

	if( time1->tm_hour > time2->tm_hour )
		return 1;
	else if( time1->tm_hour < time2->tm_hour )
		return -1;

	if( time1->tm_min > time2->tm_min )
		return 1;
	else if( time1->tm_min < time2->tm_min )
		return -1;

	if( time1->tm_sec > time2->tm_sec )
		return 1;
	else if( time1->tm_sec < time2->tm_sec )
		return -1;


	return 0;
}

/*>*******************************(*)*******************************<*/ 
// get the version of a pre-existing tpl file
/*>*******************************(*)*******************************<*/ 
static u32 TCGetTplVersion ( const char* tplName )
{
	FILE* fp;
	u32   version;


	fp = fopen( tplName, "rb");
	TCAssertMsg( (fp != NULL), "TCGetTplVersion: couldn't open existing tpl %s for read\n", tplName );

	// version is the 1st 4 bytes of the file
	if( (fread( &version, 4, 1, fp )) != 1 )
	{
		fclose( fp );
		TCErrorMsg( "TCGetTplVersion: fread failed for %s version number\n", tplName );
		return 0;
	}

	// tpl was packed big-endian
	TCFixEndian( (u8*)&version, 4 );

	return version;
}

/*>*******************************(*)*******************************<*/ 
static u32 TCWriteTplValue ( FILE* fp, u32 cursor, void* value, u32 numBytes )
{
	u8            buff[8];
	u8* buffPtr = buff;


	// copy 'value' to 'buff' based on its 'type'
	switch( numBytes )
	{
	case 1:
		*(u8*)buffPtr = *(u8*)value;
		break;
	case 2:
		*(u16*)buffPtr = *(u16*)value;
		break;
	case 4:
		*(u32*)buffPtr = *(u32*)value;
		break;
	case 8:
		*(f64*)buffPtr = *(f64*)value;
		break;
	default:
		TCErrorMsg("TCWriteTplValue: error in numBytes value\n" );
		return 0;
		break;
	}

	TCFixEndian( buff, numBytes );  		
	if( (fwrite( buff, numBytes, 1, fp )) != 1 )
	{
		fclose( fp );
		TCErrorMsg("TCWriteTplValue: error in fwrite\n" ); 
		return 0;
	}

	return numBytes;
}

/*>*******************************(*)*******************************<*/ 
static u32 TCWriteTplBlock ( FILE* fp, u32 cursor, void* block, u32 numBytes )
{

	if( (fwrite( block, 1, numBytes, fp )) != numBytes )
	{
		fclose( fp );
		TCErrorMsg("TCWriteTplBlock: error in fwrite\n" ); 
		return 0;
	}

	return numBytes;
}

/*>*******************************(*)*******************************<*/
// reverse the byte order within a block of bytes.
// do this only if TPL_BIG_END is defined
/*>*******************************(*)*******************************<*/ 
static void TCFixEndian( u8* src, u32 numBytes )
{

	#ifdef TPL_BIG_END

	u8  tmp[8];  // large enough to hold a double
	u32 max, i;


    TCAssertMsg( (numBytes <= 8), "TCFixEndian: numBytes > 8\n" );

	if( (numBytes == 0) || (numBytes == 1) )
	{
		return;
	}
					
	max = numBytes - 1;
	for(i=0; i< numBytes; i++)
	{
		tmp[(max - i)] = src[i];	
	}
	
	for(i=0; i< numBytes; i++)
	{
		src[i] = tmp[i];
	}

	#endif	
}

/*>*******************************(*)*******************************<*/
// compute wrap filter mode based on image dimensions.
// this function is used by TCWriteTplFile to set filter mode.
// return TPL_WRAP_MODE_REPEAT if image is power of 2.
// return TPL_WRAP_MODE_CLAMP  if image is not power of 2.
/*>*******************************(*)*******************************<*/ 
static u32 TCSetFilterModeByDim( TCImage* thisImage )
{
	u32  i;
	u32  width  = (thisImage->lyColor).width;
	u32  height = (thisImage->lyColor).height;
	u32* dimPtr = &width;


	for( i=0; i<2; i++ )
	{
		switch( *dimPtr )
		{	
		case 1024:		// fall through
		case  512:
		case  256:
		case  128:
		case   64:
		case   32:
		case   16:
		case    8:
		case    4:
		case    2:
		case    1:
	
			*dimPtr = TPL_WRAP_MODE_REPEAT;
			break;
	
		default:
			*dimPtr = TPL_WRAP_MODE_CLAMP;
			break;	
	}
		
		// check both dimensions
		dimPtr = &height;

	} // end for


	// if both dimensions are a power of 2, return TPL_WRAP_MODE_REPEAT.
	// if both dimensions are non-power  2, return TPL_WRAP_MODE_CLAMP.
	if( width == height )
	{
		return width;
	}

	// if dimensions are a mix of power of 2/non-power of 2, 
	// default to TPL_WRAP_MODE_CLAMP.
	return TPL_WRAP_MODE_CLAMP;
}

/*>*******************************(*)*******************************<*/ 
/*>*******************************(*)*******************************<*/ 
// write a .csf file using information in ImHead, PlHead.
// tplName and refTmPtr provide the .tpl file name and mod. time
// use hard-coded 'CsfName' for cached script file name.
// ".csf" extension stands for "cached script file"
/*>*******************************(*)*******************************<*/ 
static void TCWriteCachedScriptFile( const char* tplName, struct tm* refTmPtr )
{
	const char*     csfNamePtr = CsfName;	// use the hard-coded .csf name for now
	FILE*           fp;

	TCImage*        imPtr;
	TCPalette*      plPtr;
	TCSrcImage*     siPtr;
	TCCsfHeader     csfHdr;

	u32             imCount;
	u32             plCount;
	TCCachedImage   imCached;
	TCCachedPalette plCached;

	u32             strSize;
	u32             cursor, strBase, strCursor;
    u32             count;


    TCAssertMsg( (tplName  != NULL), "TCWriteCachedScriptFile: NULL tplName\n"  );
    TCAssertMsg( (*tplName != '\0'), "TCWriteCachedScriptFile: NULL tplName\n"  );
    TCAssertMsg( (refTmPtr != NULL), "TCWriteCachedScriptFile: NULL refTmPtr\n" );

	// zero out the data structures for initial write
	memset( &csfHdr,   0, sizeof(TCCsfHeader)     );
	memset( &imCached, 0, sizeof(TCCachedImage)   );
	memset( &plCached, 0, sizeof(TCCachedPalette) );


	_mkdir( CsfPath );		                                // if this directory already exists, _mkdir does nothing
	TCSetFileAccess ( csfNamePtr, FILE_ACCESS_READ_WRITE );	// in case srcTree locked the previous one
	if( (fp = fopen( csfNamePtr, "wb" )) == NULL )
	{
		// if it doesn't work, the result is just a forced reconvert of this .tpl next time.
		remove( csfNamePtr );
		return;	
	}

	// these are the global counters from TCSetTplValues
	imCount = NumImage;
	plCount = NumPalette;

	//---------------------------

	// compute file header values (the rest default to 0)
	// except string bank size- this isn't known until the end.

	// store the mod. date for the just-written .tpl file
	csfHdr.tplModTime = *refTmPtr;

	if( imCount )
	{
		csfHdr.numCachedImage      = imCount;
		csfHdr.cachedImageOffset   = sizeof(TCCsfHeader);
	}

	if( plCount )
	{
		csfHdr.numCachedPalette    = plCount;
		csfHdr.cachedPaletteOffset = sizeof(TCCsfHeader) + ( imCount * sizeof(TCCachedImage)   );
	}

	csfHdr.stringBankOffset        = sizeof(TCCsfHeader) + ( imCount * sizeof(TCCachedImage)   )
		                                                 + ( plCount * sizeof(TCCachedPalette) );

	//---------------------------

	// write out the header.
	// note: the 'stringBankSize' field will be overwritten at the end
	fwrite( &csfHdr, sizeof(TCCsfHeader), 1, fp );

	// write 0's to the file up to the start of the string bank
	// so we can write the string bank
	// simultaneously with the image and palette banks
	if( imCount )
	{
        for( count = 0; count < imCount; count++ )
        {
		    fwrite( &imCached, sizeof(TCCachedImage), 1, fp );
        }
	}
	if( plCount )
	{
        for( count = 0; count < plCount; count++ )
        {
		    fwrite( &plCached, sizeof(TCCachedPalette), 1, fp );
        }
	}
	rewind(fp);

	// set cursors so we can move the file ptr between 
	// cached image/palette blocks and string bank
	cursor    = sizeof(TCCsfHeader);
	strBase   = csfHdr.stringBankOffset;	// offset to string bank
	strCursor = 0;	                        // local offset within string bank

	// write the .tpl file name as the first string in the string bank
	strSize = strlen( tplName ) + 1;
	fseek( fp, (strBase + strCursor), SEEK_SET );
	fwrite( tplName, strSize, 1, fp );
	strCursor += strSize;


	// write cached images sequentially from ImHead list.
	// write image file names simultaneously to the string bank
	imPtr = ImHead;
	while( imPtr )
	{
		// zero out the cached image structure
		memset( &imCached, 0, sizeof(TCCachedImage) );

		// find imPtr's color src image name
		siPtr = TCFindSrcImageByIndex( imPtr->colorSrcImage );

		strSize = strlen( siPtr->fileName ) + 1;

		// store the mod. date of the color source image
		TCGetFileModTime(  siPtr->fileName, &imCached.colorModTime );

		// store the string bank offset in imCached and
		// write the colorSrcImage string to the string bank
		imCached.colorSrcImage = strCursor;

		fseek( fp, (strBase + strCursor), SEEK_SET );
		fwrite( siPtr->fileName, strSize, 1, fp );
		strCursor += strSize;

		// find imPtr's alpha src image name
        imCached.alphaSrcImage = TC_UNUSED;
		if( imPtr->alphaSrcImage != TC_UNUSED )
		{
			siPtr = TCFindSrcImageByIndex( imPtr->alphaSrcImage );

			strSize = strlen( siPtr->fileName ) + 1;

			// store the mod. date of the alpha source image
			TCGetFileModTime(  siPtr->fileName, &imCached.alphaModTime );

			// store the string bank offset in imCached and
			// write the colorSrcImage string to the string bank
			imCached.alphaSrcImage = strCursor;

			fseek( fp, (strBase + strCursor), SEEK_SET );
			fwrite( siPtr->fileName, strSize, 1, fp );
			strCursor += strSize;
		}

		// set the remainder of the cached image fields
		imCached.texelFormat        = imPtr->texelFormat;        
		imCached.minLOD             = imPtr->minLOD;             
		imCached.maxLOD             = imPtr->maxLOD;             
		imCached.remapMinLOD        = imPtr->remapMinLOD;        
		imCached.tplImageBankOffset = imPtr->tplImageBankOffset;	
		imCached.tplBufferSize      = imPtr->tplBufferSize;      

		// write out the cached image in one block
		fseek( fp, cursor, SEEK_SET );
		fwrite( &imCached, sizeof(TCCachedImage), 1, fp );
		cursor += sizeof(TCCachedImage);

		imPtr = imPtr->next;
	} 

	// write cached palettes sequentially from PlHead list.
	// write palette file names simultaneously to the string bank
	plPtr = PlHead;
	while( plPtr )
	{
		// zero out the cached palette structure
		memset( &plCached, 0, sizeof(TCCachedPalette) );

		// find plPtr's src image name
		siPtr = TCFindSrcImageByIndex( plPtr->srcImage );

		strSize = strlen( siPtr->fileName ) + 1;

		// store the mod. date of the palette source image
		TCGetFileModTime(  siPtr->fileName, &plCached.srcModTime );

		// store the string bank offset in plCached and
		// write the srcImage string to the string bank
		plCached.srcImage = strCursor;

		fseek( fp, (strBase + strCursor), SEEK_SET );
		fwrite( siPtr->fileName, strSize, 1, fp );
		strCursor += strSize;

		// set the remainder of the cached palette fields
		plCached.entryFormat          = plPtr->entryFormat;        
		plCached.tplPaletteBankOffset = plPtr->tplPaletteBankOffset;	
		plCached.tplBufferSize        = plPtr->tplBufferSize;      

		// write out the cached palette in one block
		fseek( fp, cursor, SEEK_SET );
		fwrite( &plCached, sizeof(TCCachedPalette), 1, fp );
		cursor += sizeof(TCCachedPalette);

		plPtr = plPtr->next;
	}

	// write the final string bank size out to the header
	csfHdr.stringBankSize = strCursor;
	fseek( fp, (u32)( (u32)(&csfHdr.stringBankSize) - (u32)(&csfHdr) ), SEEK_SET );
	fwrite( &csfHdr.stringBankSize, 4, 1, fp );

	fseek(fp, 0, SEEK_END);
	fclose(fp);
}

/*>*******************************(*)*******************************<*/ 
// read a .csf file and transfer its data to arrays of 
// TCCachedImage and TCCachedPalette structures.
// if a partial update is possible, allocate *tplPrev and store the cached .tpl file
// return 0 if full conversion is required, 
//        1 if partial update is possible.
/*>*******************************(*)*******************************<*/ 
static u32 TCReadCachedScriptFile( const char* newTplName, u8** tplPrev )
{
	const char*      csfNamePtr = CsfName;	// use the hard-coded cache file name
	FILE*            fp;

	TCCsfHeader      csfHdr;
	TCCachedImage*   cImPtr;
	TCCachedPalette* cPlPtr;
	TCSrcImage*      siPtr;

	char*            strPtr;
	char*            cachedStrBank;

	u32              i;
	u32              found;
	u32              partialUpdate = 0;


	// set array values to defaults
	NumCachedIm   = 0;
	CachedImArray = NULL;
	NumCachedPl   = 0;
	CachedPlArray = NULL;

	// check if the cached .csf file exists.
	// if it doesn't, do a full conversion
	if( (fp = fopen( csfNamePtr, "rb" )) == NULL )
	{
		return 0;
	}

	// fetch the file header
	fread( &csfHdr, sizeof(TCCsfHeader), 1, fp );

	// allocate the TCCachedImage, TCCachedPalette arrays
	// and read their data sequentially
	NumCachedIm = csfHdr.numCachedImage;
	if( NumCachedIm )
	{
		CachedImArray = (TCCachedImage*)TCCalloc( NumCachedIm, sizeof(TCCachedImage) );
		fseek( fp, csfHdr.cachedImageOffset, SEEK_SET );
		fread( CachedImArray, sizeof(TCCachedImage), NumCachedIm, fp );
	}

	NumCachedPl = csfHdr.numCachedPalette;
	if( NumCachedPl )
	{
		CachedPlArray = (TCCachedPalette*)TCCalloc( NumCachedPl, sizeof(TCCachedPalette) );
		fseek( fp, csfHdr.cachedPaletteOffset, SEEK_SET );
		fread( CachedPlArray, sizeof(TCCachedPalette), NumCachedPl, fp );
	}

	// allocate the string bank- keep all of the strings together and set
	// TCImageArray[n]->colorSrcImage etc. to point here.
	if( csfHdr.stringBankSize == 0 )
	{
		fclose( fp );
		return 0;
	}

	cachedStrBank = (char*)TCCalloc( 1, csfHdr.stringBankSize );
	fseek( fp, csfHdr.stringBankOffset, SEEK_SET );
	fread( cachedStrBank, csfHdr.stringBankSize, 1, fp );
	
	// traverse the SrcImage list, compare file name strings to those in the 
	// CachedImArray and CachedPlArray and convert matching offsets to indices

	// CachedImArray
	cImPtr = CachedImArray;
	for( i=0; i< NumCachedIm; i++, cImPtr++ )
	{
		// traverse SiHead twice- once for color, once for alpha

		// match cImPtr->colorSrcImage name to siPtr->fileName
		found  = 0;
		strPtr = cachedStrBank + cImPtr->colorSrcImage;
		siPtr  = SiHead;
		while( siPtr )
		{			
			if( (strcmp( strPtr, siPtr->fileName )) == 0 )
			{
				cImPtr->colorSrcImage = siPtr->index;
				found = 1;
				break;
			}
			siPtr = siPtr->next;
		}

		// no matching filename in SiHead; set index to 0
		if( found == 0 )
		{
			cImPtr->colorSrcImage = TC_UNUSED;
		}

		//------------------------------

		// match cImPtr->alphaSrcImage name to siPtr->fileName
		if( cImPtr->alphaSrcImage != TC_UNUSED )
		{
			found  = 0;
			strPtr = cachedStrBank + cImPtr->alphaSrcImage;
			siPtr  = SiHead;
			while( siPtr )
			{			
				if( (strcmp( strPtr, siPtr->fileName )) == 0 )
				{
					cImPtr->alphaSrcImage = siPtr->index;
					found = 1;
					break;
				}
				siPtr = siPtr->next;
			}

			// no matching filename in SiHead; set index to 0
			if( found == 0 )
			{
				cImPtr->alphaSrcImage = TC_UNUSED;
			}

		} // end if

		//--------------------------------

	} // end for

	//------------------------------------------------------------

	// CachedPlArray
	cPlPtr = CachedPlArray;
	for( i=0; i< NumCachedPl; i++, cPlPtr++ )
	{
		// match cPlPtr->srcImage name to siPtr->fileName
		found  = 0;
		strPtr = cachedStrBank + cPlPtr->srcImage;
		siPtr  = SiHead;
		while( siPtr )
		{			
			if( (strcmp( strPtr, siPtr->fileName )) == 0 )
			{
				cPlPtr->srcImage = siPtr->index;
				found = 1;
				break;
			}
			siPtr = siPtr->next;
		}

		// no matching filename in SiHead; set index to 0
		if( found == 0 )
		{
				cPlPtr->srcImage = TC_UNUSED;
		}

	} // end for

	fclose(fp);

	// compare cached information to current script information
	// including source image modification dates.
	// initialize ImFlags, PlFlags arrays if partial update is possible.
	// return 0 if full conversion is required, 1 if partial update is possible.

	// previous .tpl file name is the 1st string in cachedStrBank.
	// note:    call this fn BEFORE freeing cachedStrBank!
	partialUpdate = TCCompareToCachedScriptData( (char*)(cachedStrBank + csfHdr.tplNameOffset), 
		                                         &csfHdr.tplModTime, tplPrev );

	if( cachedStrBank )
	{
		TCFree( (void**)&cachedStrBank );
		cachedStrBank = NULL;
	}

	return partialUpdate;
}

/*>*******************************(*)*******************************<*/ 
// compare ImHead, PlHead lists to ImCached, PlCached arrays;
// (includes check for source image modification dates)
// set corresponding ImFlags, PlFlags array entries.
//
// array values: 0               = new conversion required for this image/palette;
//               any other value = u32 offset to this image/palette's data block 
//                                 within the pre-existing tpl file. 
//
// return 0 if full conversion is required for .tpl file,
//        1 if partial update is possible
/*>*******************************(*)*******************************<*/ 
static u32 TCCompareToCachedScriptData( const char* prevTplName, struct tm* prevTplModTime, u8** tplPrev )
{
	FILE*            fp;
	s32              fh;
	s32              size; 

	TCImage*         imPtr;
	TCCachedImage*   cImPtr;

	TCPalette*       plPtr;
	TCCachedPalette* cPlPtr;

	struct tm        tplRefTime;
	
	u32              i;
	u32              update = 0;
    u32              pos;


    // initialize both flag arrays to 0
	// (default = full update required for all data blocks)
	if( NumImage )
	{
		ImFlags = (u32*)TCCalloc( (NumImage),   sizeof(u32) );
	}
	if( NumPalette )
	{
		PlFlags = (u32*)TCCalloc( (NumPalette), sizeof(u32) );
	}


	// check if a cached .tpl exists; 
	if( (TCGetFileAccess( prevTplName )) == FILE_ACCESS_NONE )
	{
		return 0;
	}

	// if prev .tpl exists but version number is old, full update is required															
	if( (TCGetTplVersion( prevTplName )) != TplVersion )				    						
	{
		return 0;
	}
	
	// get the reference date for the existing .tpl file.
	// source image files will be checked against this date 
	TCGetFileModTime( prevTplName, &tplRefTime );

	// compare the existing file's reference date against the cached reference date;
	// this protects against accidental renaming of another .tpl file
	// to the name stored in the .csf file
	if( (TCCompareTime( prevTplModTime, &tplRefTime)) != 0 )	// they must be equal
	{
		return 0;
	}

	// flag all image and palette blocks where pre-converted data exists.
	imPtr = ImHead;
    pos   = 0;
	while( imPtr )
	{
		cImPtr = CachedImArray;
		for( i=0; i< NumCachedIm; i++, cImPtr++ )
		{
			// if image is the same as cached image,
			// use ImFlags to save converted data's location in the cached .tpl
			if( (TCCompareImToCachedIm( &tplRefTime, imPtr, cImPtr )) == 1 )
			{
				ImFlags[pos] = cImPtr->tplImageBankOffset;
				update = 1;
				break; // stop once a match is found
			}
		}
        pos++;
		imPtr = imPtr->next;
	}

	//------------------------

	plPtr = PlHead;
    pos   = 0;
	while( plPtr )
	{
		cPlPtr = CachedPlArray;
		for( i=0; i< NumCachedPl; i++, cPlPtr++ )
		{
			// if palette is the same as cached palette;
			// use PlFlags to save converted data's location in 'prevTpl'
			if( (TCComparePlToCachedPl( &tplRefTime, plPtr, cPlPtr )) == 1 )
			{
				PlFlags[pos] = cPlPtr->tplPaletteBankOffset;
				update = 1;
				break; // stop once a match is found
			}
		}
        pos++;
		plPtr = plPtr->next;
	}

	if( update == 1 )
	{
		// if any partial update is possible, read the previous .tpl into a buffer
		if( (fp = fopen( prevTplName, "rb" )) == NULL )
		{
			return 0;	// *tplPrev is still NULL; this forces a full update
		}

		// get the file size and read into a buffer
		fh   = _fileno( fp );
		size = _filelength( fh );

		*tplPrev = (u8*)TCMalloc( size );
		fseek( fp, 0, SEEK_SET );
		fread( *tplPrev, size, 1, fp );
		fclose( fp );
	}

	return update;
}

/*>*******************************(*)*******************************<*/ 
// compare a TCImage to a TCCachedImage:
// this includes checking source image modification dates against .tpl reference date
// return 1 if the image and cached image are the same,
//        0 if they are different.
/*>*******************************(*)*******************************<*/ 
static u32 TCCompareImToCachedIm( struct tm* refTimePtr, TCImage* imPtr, TCCachedImage* cImPtr )
{
	struct tm   siTime;
	TCSrcImage* siPtr;


	// check image fields first
	if( imPtr->colorSrcImage != cImPtr->colorSrcImage )
		return 0; 
    if( imPtr->alphaSrcImage != cImPtr->alphaSrcImage )
		return 0;	
    if( imPtr->texelFormat   != cImPtr->texelFormat   )
		return 0;       
	if( imPtr->minLOD        != cImPtr->minLOD        )
		return 0;            
	if( imPtr->maxLOD        != cImPtr->maxLOD        )
		return 0;         
	if( imPtr->remapMinLOD   != cImPtr->remapMinLOD   )
		return 0;       
    if( imPtr->tplBufferSize != cImPtr->tplBufferSize )
		return 0;     

	// compare the .tpl reference date to the source image modification date.
	// if the source image is newer, force an update
	// note: this covers changes to source content, format and dimensions.

	// compare srcImage to .tpl
	siPtr = TCFindSrcImageByIndex( imPtr->colorSrcImage );

	TCGetFileModTime( siPtr->fileName, &siTime );
	if( (TCCompareTime( &siTime, refTimePtr )) != -1 )	// srcImage is not older than .tpl
		return 0;

	// compare srcImage mod. date to cached srcImage mod. date
	// this protects against accidental renaming of an older source image
	// to the same name as that stored in the .csf file
	if( (TCCompareTime( &siTime, &cImPtr->colorModTime )) != 0 )	// cached mod date is different
		return 0;


	if( imPtr->alphaSrcImage != TC_UNUSED )
	{
		siPtr = TCFindSrcImageByIndex( imPtr->alphaSrcImage );

		TCGetFileModTime ( siPtr->fileName, &siTime );
		if( (TCCompareTime( &siTime, refTimePtr )) != -1 )	// srcImage is not older than .tpl
			return 0;

		// compare srcImage to cached srcImage
		// this protects against accidental renaming of an older source image
		// to the same name as that stored in the .csf file
		if( (TCCompareTime( &siTime, &cImPtr->alphaModTime )) != 0 )	// cached mod date is different
			return 0;
	} 

	return 1;
}

/*>*******************************(*)*******************************<*/ 
// compare a TCPalette against a TCCachedPalette:
// this includes checking source image modification dates against .tpl reference date
// return 1 if the palette and cached palette are the same,
//        0 if they are different.
/*>*******************************(*)*******************************<*/ 
static u32 TCComparePlToCachedPl( struct tm* refTimePtr, TCPalette* plPtr, TCCachedPalette* cPlPtr )
{
	struct tm   siTime;
	TCSrcImage* siPtr;


	if( plPtr->srcImage      != cPlPtr->srcImage )
		return 0; 
     if( plPtr->entryFormat  != cPlPtr->entryFormat   )
		return 0;            
    if( plPtr->tplBufferSize != cPlPtr->tplBufferSize )
		return 0;     

	// compare the .tpl reference date to the source image modification date.
	// if the source image is newer, force an update
	// note: this covers changes to source content, format and dimensions.
	siPtr = TCFindSrcImageByIndex( plPtr->srcImage );

	TCGetFileModTime( siPtr->fileName, &siTime );
	if( (TCCompareTime( &siTime, refTimePtr )) != -1 )	// srcImage is not older than .tpl
		return 0;

	// compare srcImage to cached srcImage
	// this protects against accidental renaming of an older source image
	// to the same name as that stored in the .csf file
	if( (TCCompareTime( &siTime, &cPlPtr->srcModTime )) != 0 )	// cached mod date is different
		return 0;

	return 1;
}

/*>*******************************(*)*******************************<*/ 

