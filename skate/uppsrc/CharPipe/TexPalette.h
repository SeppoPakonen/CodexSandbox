/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     texPalette.h

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/charPipeline/texPalette.h $
    
    4     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
    3     2/07/00 3:15p John
    Changed gx.h include to GXTexture.h
    
    2     12/08/99 12:16p Ryan
    added function descriptions
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  The texPalette library provides an interface between the character pipeline
  and .tpl files on disk.  Also, it provides methods to retrieve texture data
  from a texture palette.
 *---------------------------------------------------------------------------*/

#ifndef TEXPALETTE_H
#define TEXPALETTE_H

/********************************/
#if 0
extern "C" {
#endif

/********************************/
#include <Dolphin/os.h>
#include <Dolphin/GXTexture.h>
#include <Dolphin/dvd.h>
#include <CharPipe/fileCache.h>

/********************************/
typedef struct
{
	u16				numEntries;
	u8				unpacked;
	u8				pad8;

	GXTlutFmt 		format;
	Ptr				data;

}CLUTHeader, *CLUTHeaderPtr;

/********************************/
typedef struct
{
	u16				height;
	u16				width;

	u32				format;
	Ptr				data;

	GXTexWrapMode   wrapS; 
    GXTexWrapMode   wrapT;

	GXTexFilter	    minFilter;
	GXTexFilter	    magFilter;

	float 			LODBias;

	u8				edgeLODEnable;
	u8				minLOD;
	u8				maxLOD;
	u8				unpacked;

}TEXHeader, *TEXHeaderPtr;

/********************************/
typedef struct
{
	TEXHeaderPtr	    textureHeader;
	CLUTHeaderPtr		CLUTHeader;

}TEXDescriptor, *TEXDescriptorPtr;

/********************************/
typedef struct
{
	u32					versionNumber;

	u32					numDescriptors;
	TEXDescriptorPtr	descriptorArray;

}TEXPalette, *TEXPalettePtr;

/*>*******************************(*)*******************************<*/
/*---------------------------------------------------------------------------*
  GetTexPalette - loads and unpacks the specified .tpl file into the given 
  texture palette structure.  This system loads files from disk in a cached 
  manner if the display cache has been turned on with a call to the 
  DOInitDisplayCache function.
 *---------------------------------------------------------------------------*/
void TEXGetPalette ( TEXPalettePtr *pal, char *name );

/*---------------------------------------------------------------------------*
  GetTexture - returns the texture descriptor of the requested texture.  
  Returns 0 if the requested texture is not found.
 *---------------------------------------------------------------------------*/
TEXDescriptorPtr TEXGet	( TEXPalettePtr pal, u32 id );

/*---------------------------------------------------------------------------*
  ReleaseTexPalette - If the display cache is initialized, this function will
  decrement the reference count of the specified texture palette and free its 
  memory if the reference count is 0.  If the display cache is not 
  initialized, the specified texture palette is simply freed.
 *---------------------------------------------------------------------------*/
void TEXReleasePalette ( TEXPalettePtr *pal );

/*---------------------------------------------------------------------------*
  GetTexObjFromPalette - initializes a GXTexObj structure with the data from
  the requested texture.
 *---------------------------------------------------------------------------*/
void TEXGetGXTexObjFromPalette ( TEXPalettePtr pal, GXTexObj *to, u32 id );

/*---------------------------------------------------------------------------*
  GetTexObjFromPaletteCI - initializes a GXTexObj structure with the data from
  the requested structure.  Also initializes the given GXTlutObj structure with
  tlut information.
 *---------------------------------------------------------------------------*/
void TEXGetGXTexObjFromPaletteCI ( TEXPalettePtr pal, GXTexObj *to, 
                                   GXTlutObj *tlo, GXTlut tluts, u32 id );

/********************************/
#if 0
}
#endif

#endif
