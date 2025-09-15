/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     raceInit.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/charPipeline/src/raceInit.c $
    
    5     11/10/00 1:10a John
    
    4     9/25/00 5:54p Hashida
    Changed API name from DVDInitStream to DVDPrepareStream (synchronous
    function).
    
    3     8/08/00 12:30p Ryan
    update to fix errors/warnings
    
    2     8/05/00 8:42a Ryan
    20-car update
    
    1     7/29/00 11:17a Ryan
    initial checkin
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <charPipeline.h>
#include <demo.h>

#include "raceInit.h"
#include "raceModels.h"
#include "raceShaders.h"
#include "raceCamera.h"
#include "racePad.h"
#include "raceSphereMap.h"

/****************************************************************************/

static DVDFileInfo AudioInfo;

/****************************************************************************/

static void DrawInit	    ( void );
static void InitFileSystem  ( void );
static void AudioInit	    ( void );

/****************************************************************************/
/*---------------------------------------------------------------------------*
    Name:			
    
    Description:	
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
void RaceInit ( void )
{      
	//DEMOInit(NULL);
    DEMOInit(&GXNtsc480IntAa);

    // Initialize the game pad
    JoyInit();

    // Initialize the camera
	CameraInit();
           
    // Initialize Draw Parameters
    DrawInit();

    // Initialize file system
    InitFileSystem();

    //Initialize shaders
    InitShaders();

    //Initialize the Garage
    InitModels();

    // Initialize the sphere mapping information
    InitSphereMap();

    //Initialize per-vertex lights
    LightInit();

    //Initialize Audio
    AudioInit();

    VIWaitForRetrace();
}

/*---------------------------------------------------------------------------*
    Name:			DrawInit
    
    Description:	Sets up draw state  
    				
    Arguments:		
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static void DrawInit	( void )
{
    GXColor black = {0, 0, 0, 0};

	GXSetZCompLoc(GX_FALSE);

	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, 
                       GX_LO_CLEAR);

	GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_GREATER, 0);

    GXSetCopyClear(black, 0x00FFFFFF);

    GXSetDispCopyYScale(1.0F); 

    GXSetDispCopyGamma(GX_GM_1_0);

    GXSetDither(GX_ENABLE);
}


/*---------------------------------------------------------------------------*
    Name:           InitFileSystem			
    
    Description:	Initializes file stuff
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static void InitFileSystem  ( void )
{
    // initialize the Character Pipeline file cache
    CSHInitDisplayCache();

    DVDChangeDir("gxDemos/cardemo");
}

/*>*******************************(*)*******************************<*/
void InitTextureMap ( void **data, u16 width, u16 height, GXTexObj *texObj, GXTexFmt fmt )
{
    (*data) = OSAlloc(GXGetTexBufferSize(width,
                                         height,
                                         fmt,
                                         GX_FALSE,
                                         0 ));

    GXInitTexObj(texObj, (*data), width, height,
                 fmt, GX_CLAMP, GX_CLAMP, GX_FALSE);

    GXInitTexObjLOD(texObj, GX_LINEAR, GX_LINEAR, 0.0F,
                    0.0F, 0.0F, GX_FALSE, GX_FALSE, GX_ANISO_1);
}

/*---------------------------------------------------------------------------*
    Name:			AudioInit
    
    Description:	Sets up audio state  
    				
    Arguments:		
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static void AudioInit	( void )
{
#if defined(HW1) || defined(HW2)
    AIInit   (NULL);

    // set volume
    AISetStreamVolLeft(255);
    AISetStreamVolRight(255);

    // set sample rate
    AISetStreamSampleRate(AI_SAMPLERATE_48KHZ);

    if (FALSE == DVDOpen("cardemo.adp", &AudioInfo))
    {
        OSReport("Unable to open cardemo.adp\n");
    }

    // start streaming!
    DVDPrepareStream(&AudioInfo, 0, 0);
    AISetStreamPlayState(AI_STREAM_START);
#endif
}
