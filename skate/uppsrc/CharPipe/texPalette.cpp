/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     texPalette.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/texPalette/src/texPalette.c $
    
    4     7/07/00 7:09p Dante
    PC Compatibility
    
    3     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <CharPipe/TexPalette.h>
#include <CharPipe/GeoPalette.h>
#include <CharPipe/fileCache.h>

#include <CharPipe/texPalette/tplAssert.h>

/*>*******************************(*)*******************************<*/
static void LoadTexPalette		( TEXPalettePtr *pal, char *name );
static void UnpackTexPalette	( TEXPalettePtr pal );
static void TexFreeFunc	        ( TEXPalettePtr pal );

/*>*******************************(*)*******************************<*/
void TEXGetPalette ( TEXPalettePtr *pal, char *name )
{
	void *p = TexFreeFunc;

	if(DOCacheInitialized) *pal = (TEXPalettePtr)(DSGetCacheObj(&DODisplayCache, name));
	if(!*pal)	//file was not found in cache so load it!!!
	{
		LoadTexPalette(pal, name);
		if(DOCacheInitialized) 
		{
			DSAddCacheNode(&DODisplayCache, name, (Ptr)(*pal), (Ptr)p);
			DSGetCacheObj(&DODisplayCache, name); //Increment reference count
		}
	}
}

/*>*******************************(*)*******************************<*/
static void LoadTexPalette ( TEXPalettePtr *pal, char *name )
{
	DVDFileInfo dfi;

	DVDOpen(name, &dfi);
	(*pal) = (TEXPalettePtr)OSAlloc(OSRoundUp32B(dfi.length));
	DVDRead(&dfi, (*pal), (s32)OSRoundUp32B(dfi.length), 0);
	DVDClose(&dfi);

	UnpackTexPalette(*pal);
}

/*>*******************************(*)*******************************<*/
static void UnpackTexPalette ( TEXPalettePtr pal )
{
	u16 i;

#ifdef flagWIN32
	EndianSwap32(&pal->numDescriptors);
	EndianSwap32(&pal->versionNumber);
	EndianSwap32(&pal->descriptorArray);
#endif

	if(pal->versionNumber != 2142000 )
		OSHalt("invalid version number for texture palette");
	
	pal->descriptorArray = (TEXDescriptorPtr)(((u32)(pal->descriptorArray)) + ((u32)pal));
	
	for ( i = 0; i < pal->numDescriptors; i++ )
	{

#ifdef flagWIN32
		EndianSwap32(&pal->descriptorArray[i].textureHeader);
#endif

		if(pal->descriptorArray[i].textureHeader)
		{
			pal->descriptorArray[i].textureHeader = (TEXHeaderPtr)(((u32)(pal->descriptorArray[i].textureHeader)) + ((u32)pal));	

#ifdef flagWIN32
			EndianSwap16(&pal->descriptorArray[i].textureHeader->height);
			EndianSwap16(&pal->descriptorArray[i].textureHeader->width);
			EndianSwap32(&pal->descriptorArray[i].textureHeader->format);
			EndianSwap32(&pal->descriptorArray[i].textureHeader->data);
			EndianSwap32(&pal->descriptorArray[i].textureHeader->wrapS);
			EndianSwap32(&pal->descriptorArray[i].textureHeader->wrapT);
			EndianSwap32(&pal->descriptorArray[i].textureHeader->minFilter);
			EndianSwap32(&pal->descriptorArray[i].textureHeader->magFilter);
			EndianSwap32(&pal->descriptorArray[i].textureHeader->LODBias);
#endif

			if(!(pal->descriptorArray[i].textureHeader->unpacked))
			{
				pal->descriptorArray[i].textureHeader->data = (Ptr)((u32)(pal->descriptorArray[i].textureHeader->data) + (u32)pal);
				pal->descriptorArray[i].textureHeader->unpacked = 1;
			}
		}

#ifdef flagWIN32
			EndianSwap32(&pal->descriptorArray[i].CLUTHeader);
#endif
		
		if(pal->descriptorArray[i].CLUTHeader)
		{
			pal->descriptorArray[i].CLUTHeader = (CLUTHeaderPtr)((u32)(pal->descriptorArray[i].CLUTHeader) + (u32)pal);		

#ifdef flagWIN32
			EndianSwap16(&pal->descriptorArray[i].CLUTHeader->numEntries);
			EndianSwap32(&pal->descriptorArray[i].CLUTHeader->format);
			EndianSwap32(&pal->descriptorArray[i].CLUTHeader->data);
#endif

			if(!(pal->descriptorArray[i].CLUTHeader->unpacked))
			{
				pal->descriptorArray[i].CLUTHeader->data = (Ptr)((u32)(pal->descriptorArray[i].CLUTHeader->data) + (u32)pal);
				pal->descriptorArray[i].CLUTHeader->unpacked = 1;
			}
		}
		
	}
}

/*>*******************************(*)*******************************<*/
TEXDescriptorPtr TEXGet	( TEXPalettePtr pal, u32 id )
{
	ASSERTMSG(id < pal->numDescriptors, GET_TEXTURE_1);
	
	return &pal->descriptorArray[id];
}

/*>*******************************(*)*******************************<*/
static void TexFreeFunc	( TEXPalettePtr pal )
{
	OSFree(pal);
}

/*>*******************************(*)*******************************<*/
void TEXReleasePalette ( TEXPalettePtr *pal )
{
	if(DOCacheInitialized) 
		DSReleaseCacheObj(&DODisplayCache, (Ptr)(*pal));
	else
	{
		OSFree(*pal);
		*pal = 0;
	}		
}

/*>*******************************(*)*******************************<*/
void TEXGetGXTexObjFromPalette ( TEXPalettePtr pal, GXTexObj *to, u32 id )
{
	TEXDescriptorPtr tdp = TEXGet(pal, id);
	GXBool mipMap;

	//initTexObj with texture values

	if(tdp->textureHeader->minLOD == tdp->textureHeader->maxLOD)
		mipMap = GX_FALSE;
	else
		mipMap = GX_TRUE;

	GXInitTexObj(to, 
				 tdp->textureHeader->data, 
				 tdp->textureHeader->width, 
    			 tdp->textureHeader->height, 
				 (GXTexFmt)tdp->textureHeader->format,
	   			 tdp->textureHeader->wrapS, 
				 tdp->textureHeader->wrapT, 
				 mipMap); 

	
	GXInitTexObjLOD(to, tdp->textureHeader->minFilter, 
					tdp->textureHeader->magFilter, 
					tdp->textureHeader->minLOD, 
					tdp->textureHeader->maxLOD, 
					tdp->textureHeader->LODBias,
					GX_DISABLE,
					tdp->textureHeader->edgeLODEnable,
					GX_ANISO_1);  
}

/*>*******************************(*)*******************************<*/
void TEXGetGXTexObjFromPaletteCI ( TEXPalettePtr pal, GXTexObj *to, 
			    				   GXTlutObj *tlo, GXTlut tluts, u32 id )
{
	GXBool mipMap;
	TEXDescriptorPtr tdp = TEXGet(pal, id);

	//initTexObj with texture values

	if(tdp->textureHeader->minLOD == tdp->textureHeader->maxLOD)
		mipMap = GX_FALSE;
	else
		mipMap = GX_TRUE;

	GXInitTlutObj(tlo,
	              tdp->CLUTHeader->data,
	              (GXTlutFmt)tdp->CLUTHeader->format,
	              tdp->CLUTHeader->numEntries );

	GXInitTexObjCI(	to, 
					tdp->textureHeader->data, 
					tdp->textureHeader->width, 
    			 	tdp->textureHeader->height, 
					(GXCITexFmt)tdp->textureHeader->format,
	   			 	tdp->textureHeader->wrapS, 
					tdp->textureHeader->wrapT, 
				 	mipMap, 
					tluts );
	
	GXInitTexObjLOD(to, 
					tdp->textureHeader->minFilter, 
					tdp->textureHeader->magFilter, 
					tdp->textureHeader->minLOD, 
					tdp->textureHeader->maxLOD, 
					tdp->textureHeader->LODBias,
					GX_DISABLE,
					tdp->textureHeader->edgeLODEnable,
					GX_ANISO_1);  
}

/*>*******************************(*)*******************************<*/
