/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     racedemo.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/charPipeline/src/racedemo.c $
    
    4     8/11/00 10:42a Ryan
    final checkin for spaceworld
    
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

#include "racePad.h"
#include "raceInit.h"
#include "raceModels.h"
#include "raceSphereMap.h"
#include "raceCamera.h"

/*---------------------------------------------------------------------------*
   Forward references
 *---------------------------------------------------------------------------*/

void 		main 			( void ); 

/*---------------------------------------------------------------------------*
   Global variables
 *---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*
   Application main loop
 *---------------------------------------------------------------------------*/
void main ( void )
{
    RaceInit();

	// While the quit button is not pressed
	while(!(JoyReadButtons() & PAD_BUTTON_MENU))	
	{	
		JoyTick();			// Read the joystick for this frame
        CarAnim();
        CameraAnim();

        DEMOBeforeRender();

        RenderCubeMaps();
        DrawSphereMap();

        if(VideoModeAA) // draw antialiased version
        {      
            SetMainCameraTop();

            CarDraw();

            if(CubeMapDisplay == 1)
                DisplaySphereMap();

            // Copy out the top half
            GXCopyDisp(DEMOGetCurrentBuffer(), GX_TRUE);

            ////////////////////////////////
            //draw bottom

            SetMainCameraBottom();

            CarDraw();

            // Copy out (and clear) the bottom half
            CopyBottom();

            // Swap buffers, wait for retrace, etc.
            GXDrawDone();
            DEMOSwapBuffers();
        }
        else    //draw interlaced deflickered version
        {
            SetMainCamera();

            CarDraw();

            if(CubeMapDisplay == 1)
                DisplaySphereMap();

            DEMODoneRender();
        }
	}
    OSHalt("End of demo");
}








