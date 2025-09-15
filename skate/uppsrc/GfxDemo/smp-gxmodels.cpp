/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     smp-gxmodels.c

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/gxdemo/src/Simple/smp-gxmodels.c $
    
    7     10/25/00 8:59p Hirose
    A flag fix MAC -> MACOS
    
    6     8/30/00 6:27p Hirose
    removed MAC only instructions from other targets
    
    5     3/27/00 3:24p Hirose
    changed to use DEMOPad library
    
    4     3/23/00 8:05p Hirose
    fixed instruction message
    
    3     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
    2     3/17/00 2:36p Alligator
    added SINGLEFRAME capability
    
    1     3/16/00 7:24p Alligator
    rename to match other demos conventions
    
    1     3/06/00 12:11p Alligator
    move from gxdemos to gxdemos/Simple
    
    7     3/02/00 3:00p Ryan
    changed call to DVDSetRoot to DVDChangeDir
    
    6     2/29/00 5:54p Ryan
    update for dvddata folder rename
    
    5     2/28/00 7:36p Hirose
    removed hardcoded magic number in JoyReadButtons
    
    4     2/24/00 7:06p Yasu
    Rename gamepad key to match HW1
    
    3     2/21/00 3:32p Ryan
    changed to 8.3 filenames
    
    2     1/13/00 8:53p Danm
    Added GXRenderModeObj * parameter to DEMOInit()
    
    18    11/12/99 4:57p Yasu
    
    17    10/15/99 12:03p Ryan
    renamed mecha files
    
    16    10/11/99 1:07p Ryan
    update to add lighting functionality to character pipeline.
    
    15    9/30/99 2:13p Ryan
    sweep to remove gxmodels libs
    
    14    9/17/99 5:26p Ryan
    added new DEMO calls
    
    13    9/15/99 2:48p Ryan
    update to fix compiler warnings
    
    12    7/30/99 1:39p Ryan
    changed demo so that it loads a tpl file for textures
    
    11    7/23/99 2:51p Ryan
    changed dolphinDemo.h to demo.h
    
    10    7/23/99 12:15p Ryan
    included dolphinDemo.h
    
    9     7/20/99 6:03p Alligator
    demo library
    
    8     7/06/99 11:52a Ryan
    changed to match new texture init API
    
    7     7/02/99 1:00p Ryan
    changed Mtx row/col
    
    6     6/16/99 2:43p Ryan
    Changed the car mosels and added new poly counts for each.
    
    5     6/15/99 12:54p Ryan
    Fixed Pad inout to match new version of the library.
    
    4     6/14/99 2:57p Ryan
    added PrintIntro function which prints demo directions
    
    3     6/14/99 12:14p Ryan
    changed code to display all four models in turn.
    
    2     6/11/99 12:39a Ryan
    
    2     6/10/99 2:36p Ryan
    added joystick module
    added comments about external variables
    
    1     6/09/99 2:58p Ryan
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <demo.h>

/*---------------------------------------------------------------------------*
   Forward references
 *---------------------------------------------------------------------------*/

void        main            ( void );

static void CameraInit      ( Mtx v );
static void ViewInit        ( Mtx v );
static void DrawInit        ( void );

static void ReportStatistics( void );

static void DrawTick        ( Mtx v, Mtx m );
static void MakeModelMtx    ( Vec xAxis, Vec yAxis, Vec zAxis, Mtx m );
static void AnimTick        ( Mtx v, Mtx m );

static void MechaDrawInit   ( void );
static void MechaTexDrawInit( void );
static void CarDrawInit     ( void );
static void CarTexDrawInit  ( void );

static void PrintIntro      ( void );

/*---------------------------------------------------------------------------*
   Global variables
 *---------------------------------------------------------------------------*/
// Vectors to keep track of the object's coordinate system orientation
Vec ObjY;    
Vec ObjX;
Vec ObjZ;

s8 CurrentModel = 0;

OSStopwatch   FrameRate;

Mtx  ViewMtx, ModelMtx;

// Scale for the camera's distance from the object
float CameraLocScale;

DODisplayObjPtr MechaDispObj;
GEOPalettePtr MechaPal;

DODisplayObjPtr MechaTexDispObj;
GEOPalettePtr MechaTexPal;

DODisplayObjPtr CarDispObj;
GEOPalettePtr CarPal;

DODisplayObjPtr CarTexDispObj;
GEOPalettePtr CarTexPal;

/*---------------------------------------------------------------------------*
   Application main loop
 *---------------------------------------------------------------------------*/
void main ( void )
{
    DEMOInit(NULL);  // init os, pad, gx, vi
   
    DVDChangeDir("gxDemos");

    MechaDrawInit();
    MechaTexDrawInit();
    CarDrawInit();
    CarTexDrawInit();
    DrawInit();  // Define my vertex formats and set array pointers.

    OSInitStopwatch(&FrameRate, "frame rate stopwatch");

#ifndef __SINGLEFRAME
    PrintIntro();

    // While the quit button is not pressed
    while(!(DEMOPadGetButton(0) & PAD_BUTTON_MENU))    
    {    
#endif // __SINGLEFRAME
        OSStartStopwatch(&FrameRate);

        DEMOPadRead(); // Update pad status (using DEMOPad library)

        // Do animation based on input
        AnimTick(ViewMtx, ModelMtx);    

        DEMOBeforeRender();
        
        DrawTick(ViewMtx, ModelMtx);    // Draw the model.

        DEMODoneRender();  // Wait for vertical retrace.

        OSStopStopwatch(&FrameRate); 
#ifndef __SINGLEFRAME
    }
#endif // __SINGLEFRAME

    ReportStatistics();
}

/*---------------------------------------------------------------------------*
   Functions
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    Name:           CameraInit
    
    Description:    Initialize the projection matrix and load into hardware.
                    
    Arguments:      v      view matrix    to be passed to ViewInit
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void CameraInit         ( Mtx v )
{
    Mtx44 p;
    
    MTXFrustum(p, .24F * CameraLocScale,-.24F * CameraLocScale,
               -.32F * CameraLocScale, .32F * CameraLocScale, 
               .5F * CameraLocScale, 20.0F * CameraLocScale);

    GXSetProjection(p, GX_PERSPECTIVE);
    
    ViewInit(v);    
}

/*---------------------------------------------------------------------------*
    Name:           ViewInit
    
    Description:    Initialize the view matrix.
                    
    Arguments:      v      view matrix
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void ViewInit ( Mtx v )
{
    Vec camPt = {0.0F, 0.0F, 8.0F};
    Vec up = {0.0F, 1.0F, 0.0F};
    Vec origin = {0.0F, 0.0F, 0.0F};
    
    camPt.x *= CameraLocScale;    // Scale camPt by cameraLocScale 
    camPt.y *= CameraLocScale; 
    camPt.z *= CameraLocScale;

    MTXLookAt(v, &camPt, &up, &origin);        
}

/*---------------------------------------------------------------------------*
    Name:           ReportStatistics
    
    Description:    Prints the frame rate and polygons per second for the 
                    current model.
                    
    Arguments:      none
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void ReportStatistics( void )
{   
    OSReport("\n\n********************************\n\n");
    
    switch(CurrentModel)
    {
        case 0:
            OSReport("Gouraud shaded Mecha \n");
            break;
        case 1:
            OSReport("Textured Mecha \n");
            break;
        case 2:
            OSReport("Gouraud shaded Car \n");
            break;
        case 3:
            OSReport("Textured Car \n");
            break;
    }
    
    // Figure and print frames per second and polygons per second
    OSReport("Frames per second = %f \n", 1.0 /
    (((float)OSTicksToMicroseconds(FrameRate.total/FrameRate.hits)) /
    1000000.0) );

    OSResetStopwatch(&FrameRate);
}

/*---------------------------------------------------------------------------*
    Name:           DrawInit
    
    Description:    Calls the correct initialization function for the current 
                    model.
                    
    Arguments:      none
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void DrawInit( void )
{
    ObjY.x = 0.0F;  ObjY.y = 1.0F; ObjY.z = 0.0F;
    ObjX.x = 1.0F;  ObjX.y = 0.0F; ObjX.z = 0.0F;
    ObjZ.x = 0.0F;  ObjZ.y = 0.0F; ObjZ.z = 1.0F;

    switch(CurrentModel)
    {
        case 0: 
        case 1: 
            CameraLocScale = 10.0F;
            break;
        case 2:  
        case 3: 
            CameraLocScale = 2.0F;
            break;
    }

    CameraInit(ViewMtx);    // Re-Initialize the camera.
}

/*---------------------------------------------------------------------------*
    Name:           DrawTick
    
    Description:    Draw the current model once.  
                    
    Arguments:      v        view matrix
                    m        model matrix
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void DrawTick(Mtx v, Mtx m)
{
    switch(CurrentModel)
    {
      case 0:
        DOSetWorldMatrix(MechaDispObj, m);
        DORender(MechaDispObj, v, 0);
        break;
      case 1:
        DOSetWorldMatrix(MechaTexDispObj, m);
        DORender(MechaTexDispObj, v, 0);
        break;
      case 2:
        DOSetWorldMatrix(CarDispObj, m);
        DORender(CarDispObj, v, 0);
        break;
      case 3:
        DOSetWorldMatrix(CarTexDispObj, m);
        DORender(CarTexDispObj, v, 0);
        break;
    }
}

/*---------------------------------------------------------------------------*
    Name:           MechaTexDrawInit
    
    Description:    Loads the textured Mecha model 
                    
    Arguments:      none
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void MechaTexDrawInit( void )
{
    GEOGetPalette(&MechaTexPal, "MechaTex.gpl");
    
    DOGet(&MechaTexDispObj, MechaTexPal, 0, 0); 
    DOShow(MechaTexDispObj);
}

/*---------------------------------------------------------------------------*
    Name:           MechaDrawInit
    
    Description:    Loads the non-textured Mecha model 
                    
    Arguments:      none
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void MechaDrawInit( void )
{
    GEOGetPalette(&MechaPal, "Mecha.gpl");
    
    DOGet(&MechaDispObj, MechaPal, 0, 0);
    DOShow(MechaDispObj);
}

/*---------------------------------------------------------------------------*
    Name:           CarDrawInit
    
    Description:    Loads the non-textured car model 
                    
    Arguments:      none
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void CarDrawInit( void )
{
    GEOGetPalette(&CarPal, "Car.gpl");
    
    DOGet(&CarDispObj, CarPal, 0, 0); 
    DOShow(CarDispObj);
}

/*---------------------------------------------------------------------------*
    Name:           CarTexDrawInit
    
    Description:    Loads the textured car model 
                    
    Arguments:      none
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void CarTexDrawInit( void )
{
    GEOGetPalette(&CarTexPal, "CarTex.gpl");
    
    DOGet(&CarTexDispObj, CarTexPal, 0, 0); 
    DOShow(CarTexDispObj);
}

/*---------------------------------------------------------------------------*
    Name:           MakeModelMtx
    
    Description:    computes a model matrix from 3 vectors representing an 
                    object's coordinate system.
                    
    Arguments:      xAxis    vector for the object's X axis
                    yAxis    vector for the object's Y axis
                    zAxis    vector for the object's Z axis
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void MakeModelMtx ( Vec xAxis, Vec yAxis, Vec zAxis, Mtx m )
{
    VECNormalize(&xAxis,&xAxis);
    VECNormalize(&yAxis,&yAxis);
    VECNormalize(&zAxis, &zAxis);

    m[0][0] = xAxis.x;
    m[0][1] = xAxis.y;
    m[0][2] = xAxis.z;
    m[0][3] = 0.0F;

    m[1][0] = yAxis.x;
    m[1][1] = yAxis.y;
    m[1][2] = yAxis.z;
    m[1][3] = 0.0F;

    m[2][0] = zAxis.x;
    m[2][1] = zAxis.y;
    m[2][2] = zAxis.z;
    m[2][3] = 0.0F;

    MTXInverse(m, m);
}

/*---------------------------------------------------------------------------*
    Name:           AnimTick
    
    Description:    Animates the camera and object based on the joystick's 
                    state.
                    
    Arguments:      v    view matrix
                    m    model matrix
                    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void AnimTick ( Mtx v, Mtx m )
{
    Mtx rot;

    // Get controller status through DEMOPad library
    u16 buttons = DEMOPadGetButton(0);
    u16 downs   = DEMOPadGetButtonDown(0);
    s8 stickX = DEMOPadGetStickX(0);
    s8 stickY = DEMOPadGetStickY(0);

    if(downs & PAD_BUTTON_X)
    {
        ReportStatistics();
        CurrentModel ++;
        if(CurrentModel > 3)
            CurrentModel = 0;
    
        DrawInit();
    }
    else if (downs & PAD_BUTTON_B)
    {
        ReportStatistics();
        CurrentModel --;
        if(CurrentModel < 0)
            CurrentModel = 3;
        
        DrawInit();
    }

    // Move camera
    if(buttons & PAD_BUTTON_Y)
    {
        CameraLocScale *= .95F;
        if(CameraLocScale < 0.001F)
        {
            CameraLocScale = 0.001F;
        }
        ViewInit(v);
        
    }
    if(buttons & PAD_BUTTON_A)
    {
        CameraLocScale *= 1.05F;         
        ViewInit(v);
    }

    // Rotate object
    if(stickX || stickY)
    {
        if(stickX)
        {
            if(stickX > 0) 
            {
                MTXRotDeg(rot, 'Y', 3.0F);
            }
            else 
            {
                MTXRotDeg(rot, 'Y', -3.0F);
            }

            MTXMultVec(rot, &ObjX, &ObjX);
            MTXMultVec(rot, &ObjY, &ObjY);
            MTXMultVec(rot, &ObjZ, &ObjZ); 
        }
    
        if(stickY)
        {
            if(stickY > 0)
            {
                MTXRotDeg(rot, 'X', 3.0F);
            }
            else 
            {
                MTXRotDeg(rot, 'X', -3.0F);
            }

            MTXMultVec(rot, &ObjX, &ObjX);
            MTXMultVec(rot, &ObjY, &ObjY);
            MTXMultVec(rot, &ObjZ, &ObjZ);            
        }
    }
        
#ifdef __SINGLEFRAME
    CurrentModel = __SINGLEFRAME;
    if (CurrentModel < 2) 
    {
        CameraLocScale *= .5F;
        ViewInit(v);
        MTXRotDeg(rot, 'X', -90.0F);
        MTXMultVec(rot, &ObjX, &ObjX);
        MTXMultVec(rot, &ObjY, &ObjY);
        MTXMultVec(rot, &ObjZ, &ObjZ);            
        MTXRotDeg(rot, 'Y', 180.0F);
        MTXMultVec(rot, &ObjX, &ObjX);
        MTXMultVec(rot, &ObjY, &ObjY);
        MTXMultVec(rot, &ObjZ, &ObjZ);
    } 
    else
    {
        CameraLocScale *= .4F;
        ViewInit(v);
        MTXRotDeg(rot, 'X', -45.0F);
        MTXMultVec(rot, &ObjX, &ObjX);
        MTXMultVec(rot, &ObjY, &ObjY);
        MTXMultVec(rot, &ObjZ, &ObjZ);            
        MTXRotDeg(rot, 'Y', 45.0F);
        MTXMultVec(rot, &ObjX, &ObjX);
        MTXMultVec(rot, &ObjY, &ObjY);
        MTXMultVec(rot, &ObjZ, &ObjZ);
    }
#endif // __SINGLEFRAME
    
    MakeModelMtx(ObjX, ObjY, ObjZ, m);    // Make a new model matrix

}

/*---------------------------------------------------------------------------*
    Name:            PrintIntro
    
    Description:    Prints the directions on how to use this demo.
                    
    Arguments:        none
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void PrintIntro( void )
{
    
    OSReport("\n\n********************************\n");
    OSReport("Stick - rotate object\n");
    OSReport("A/Y buttons - zoom in / zoom out\n");
    OSReport("X/B buttons - next object / previous object\n");
    OSReport("menu - quit\n");
    OSReport("********************************\n");
#ifdef MACOS
    OSReport("to quit:\n");
    OSReport("     hit the menu button\n");
    OSReport("     click on the text output window\n");
    OSReport("     select quit from the menu or hit 'command q'\n");
    OSReport("     select 'don't save'\n");
    OSReport("********************************\n");
#endif // MACOS
}

/*===========================================================================*/
