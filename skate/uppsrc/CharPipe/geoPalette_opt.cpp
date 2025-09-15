/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     geoPalette.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/geoPalette/src/geoPalette.c $
    
    8     12/04/00 7:58p John
    Added multitexturing ability (work in progress).
    
    7     8/14/00 6:25p John
    Added user-defined data to ANM, ACT, and GPL formats.
    
    6     8/04/00 5:09p John
    Set the default visibility to be ON.
    
    5     7/19/00 4:18p Ryan
    update to work with precompiled shaders
    
    4     4/12/00 6:28a Ryan
    Update for color/alpha and texgen shader revision
    
    3     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <Dolphin/dvd.h>
#include <Dolphin/os.h>

#include <CharPipe/GeoPalette.h>

/*>*******************************(*)*******************************<*/
static void LoadGeoPalette		     ( GEOPalettePtr *pal, char *name );
static void GeoPaletteFree		     ( GEOPalettePtr *pal );
static void InitDisplayObjWithLayout ( DODisplayObjPtr dispObj, DOLayoutPtr layout );

/*>*******************************(*)*******************************<*/
void GEOGetPalette ( GEOPalettePtr *pal, char *name )
{
	void *p = GeoPaletteFree;
	
	if(DOCacheInitialized) *pal = (GEOPalettePtr)DSGetCacheObj(&DODisplayCache, name);
	if(!*pal)	//file was not found in cache so load it!!!
	{
		LoadGeoPalette(pal, name);
		if(DOCacheInitialized) 
		{
			DSAddCacheNode(&DODisplayCache, name, (Ptr)(*pal), (Ptr)p);
			DSGetCacheObj(&DODisplayCache, name); //Increment reference count
		}
	}
}

/*>*******************************(*)*******************************<*/
void GEOReleasePalette ( GEOPalettePtr *pal )
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
u32 GEOGetUserDataSize      ( GEOPalettePtr pal )
{
    ASSERT( pal );
    return pal->userDataSize;
}

/*>*******************************(*)*******************************<*/
Ptr GEOGetUserData          ( GEOPalettePtr pal )
{
    ASSERT( pal );
    return pal->userData;
}

/*>*******************************(*)*******************************<*/
static void LoadGeoPalette ( GEOPalettePtr *pal, char *name )
{
	DVDFileInfo dfi;
    GEOPalettePtr geoPal;
	DOLayoutPtr layout;
	u16 i, j;

    // Load the file into memory
	if(!DVDOpen(name, &dfi))
    {
        OSReport("LoadGeoPalette: Could not find file %s", name );
        OSHalt("");
    }
	geoPal = (GEOPalettePtr)OSAlloc(OSRoundUp32B(dfi.length));
	DVDRead(&dfi, geoPal, (s32)OSRoundUp32B(dfi.length), 0);
	DVDClose(&dfi);
	
    // Check the version number
	if(geoPal->versionNumber != GPL_VERSION)
    {
        OSReport("LoadGeoPalette: Incompatible version number %d for %s, since\n", geoPal->versionNumber, name);
        OSReport("                the library version number is %d.\n", GPL_VERSION );
        OSHalt("");
    }

    // Unpack the format
    if(geoPal->userDataSize > 0 && geoPal->userData)
        geoPal->userData = (Ptr)((u32)(geoPal->userData) + (u32)geoPal);

	geoPal->descriptorArray = (GEODescriptorPtr)((u32)(geoPal->descriptorArray) + (u32)geoPal);

	for(i = 0; i < geoPal->numDescriptors; i++)
	{
		geoPal->descriptorArray[i].layout = (DOLayoutPtr)((u32)(geoPal->descriptorArray[i].layout) + (u32)geoPal);
		geoPal->descriptorArray[i].name = (Ptr)((u32)(geoPal->descriptorArray[i].name) + (u32)geoPal);
		
		layout = geoPal->descriptorArray[i].layout;
		
		if(layout->positionData)
		{
			layout->positionData = (DOPositionHeaderPtr)((u32)(layout->positionData) + (u32)layout);
			if(layout->positionData->positionArray)
				layout->positionData->positionArray = (Ptr)((u32)(layout->positionData->positionArray) + (u32)layout);
		}
			
		if(layout->colorData)
		{
			layout->colorData = (DOColorHeaderPtr)((u32)(layout->colorData) + (u32)layout);
			if(layout->colorData->colorArray)
				layout->colorData->colorArray = (Ptr)((u32)(layout->colorData->colorArray) + (u32)layout);
		}
			
		if(layout->textureData)
		{
    	    layout->textureData = (DOTextureDataHeaderPtr)((u32)(layout->textureData) + (u32)layout);
            for(j = 0; j < layout->numTextureChannels; j++)
            {
			    if(layout->textureData[j].textureCoordArray)
                {
				    layout->textureData[j].textureCoordArray = (Ptr)((u32)(layout->textureData[j].textureCoordArray) + (u32)layout);
                }
			    if(layout->textureData[j].texturePaletteName)
			    {
				    layout->textureData[j].texturePaletteName = (Ptr)((u32)(layout->textureData[j].texturePaletteName) + (u32)layout);		
				    TEXGetPalette(&(layout->textureData[j].texturePalette), layout->textureData[j].texturePaletteName);
			    }
		    }
        }

        if(layout->lightingData)
		{
			layout->lightingData = (DOLightingHeaderPtr)((u32)(layout->lightingData) + (u32)layout);
			if(layout->lightingData->normalArray)
				layout->lightingData->normalArray = (Ptr)((u32)(layout->lightingData->normalArray) + (u32)layout);	
		}
		
		if(layout->displayData)
		{
			layout->displayData = (DODisplayHeaderPtr)((u32)(layout->displayData) + (u32)layout);
			if(layout->displayData->primitiveBank)
				layout->displayData->primitiveBank = (Ptr)((u32)(layout->displayData->primitiveBank) + (u32)layout);	
			if(layout->displayData->displayStateList)
				layout->displayData->displayStateList = (DODisplayStatePtr)((u32)(layout->displayData->displayStateList) + (u32)layout);	
			for(j = 0; j < layout->displayData->numStateEntries; j++) 
			{
				if(layout->displayData->displayStateList[j].primitiveList)
					layout->displayData->displayStateList[j].primitiveList = (Ptr)((u32)(layout->displayData->displayStateList[j].primitiveList) + (u32)layout);
			}
		}
	}

    *pal = geoPal;
}

/*>*******************************(*)*******************************<*/
void DOGet ( DODisplayObjPtr *dispObj, GEOPalettePtr pal, u16 id, char *name )
{
	DOLayoutPtr layout;
	u32 i;

	if(name) 
	{
		for ( i = 0; i < pal->numDescriptors; i++ )
		{
			if(!(Strcmp(pal->descriptorArray[i].name, name)))
			{
				layout = pal->descriptorArray[i].layout;
				(*dispObj) = OSAlloc(sizeof(DODisplayObj));
				InitDisplayObjWithLayout(*dispObj, layout);
				return;
			}
		}
		ASSERTMSG(0, "display object not found in Geometry Palette");
	}	
	else
	{
		if(id >= pal->numDescriptors)
			ASSERTMSG(0, "bad display object id");

		layout = pal->descriptorArray[id].layout;
	}

	(*dispObj) = OSAlloc(sizeof(DODisplayObj));
	InitDisplayObjWithLayout(*dispObj, layout);
}

/*>*******************************(*)*******************************<*/
static void GeoPaletteFree	( GEOPalettePtr *pal )
{
	OSFree(*pal);
	(*pal) = 0;
}

/*>*******************************(*)*******************************<*/
static void InitDisplayObjWithLayout ( DODisplayObjPtr dispObj, DOLayoutPtr layout )
{
    u8 i;

	if(layout->positionData)
	{
		dispObj->positionData = (DOPositionHeaderPtr)OSAlloc(sizeof(DOPositionHeader));
		*(dispObj->positionData) = *(layout->positionData);
	}
	else
		dispObj->positionData = 0;

	if(layout->colorData)
	{
		dispObj->colorData = (DOColorHeaderPtr)OSAlloc(sizeof(DOColorHeader));
		*(dispObj->colorData) = *(layout->colorData);
	}
	else
		dispObj->colorData = 0;

	if(layout->textureData && layout->numTextureChannels)
	{
        dispObj->textureData = (DOTextureDataHeaderPtr)OSAlloc(sizeof(DOTextureDataHeader) * layout->numTextureChannels);
        dispObj->numTextureChannels = layout->numTextureChannels;
        for( i = 0; i < layout->numTextureChannels; i++ )
        {
		    dispObj->textureData[i] = layout->textureData[i];
	    }
    }
	else
    {
		dispObj->textureData = 0;
        dispObj->numTextureChannels = 0;
    }

    if(layout->lightingData)
	{
		dispObj->lightingData = (DOLightingHeaderPtr)OSAlloc(sizeof(DOLightingHeader));
		*(dispObj->lightingData) = *(layout->lightingData);
	}
	else
		dispObj->lightingData = 0;

	if(layout->displayData)
	{
		dispObj->displayData = (DODisplayHeaderPtr)OSAlloc(sizeof(DODisplayHeader));
		*(dispObj->displayData) = *(layout->displayData);
	}
	else
		dispObj->displayData = 0;

	dispObj->visibility = DO_DISPLAY_ON;

	MTXIdentity(dispObj->worldMatrix);

	dispObj->shaderFunc = 0;
    dispObj->shaderData = 0;

#if defined(flagWIN32) && defined(OPENGL_OPTIMIZE)
	dispObj->displayList = 0;
	dispObj->displayInit = OPENGL_STATE_NOT_OPTIMIZED;
#endif
}

/*>*******************************(*)*******************************<*/