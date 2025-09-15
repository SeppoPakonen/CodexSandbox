/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     smp-gxmodels-perf.c

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/gxdemo/src/Simple/smp-gxmodels-perf.c $
    
    3     8/08/00 4:25p Hirose
    updated to call DEMOSwapBuffers instead of using own routine
    
    2     7/18/00 4:01p Tian
    Added pre- and post- draw functions.  Fixes for optimizations made to
    displayobject.c
    
    1     7/06/00 5:08p Tian
    Initial checkin.  Based on smp-gxmodels
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  This simple demo show show to quickly integrate the performance 
  visualization library into an application.
 *---------------------------------------------------------------------------*/


#include <demo.h>
#include <Dolphin/perf.h>

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

static void MyDoneRender    ( void );
static void MyPerfInit      ( void );


/*---------------------------------------------------------------------------*
   Global variables
 *---------------------------------------------------------------------------*/
// Vectors to keep track of the object's coordinate system orientation
Vec ObjY;    
Vec ObjX;
Vec ObjZ;

s8 CurrentModel = 0;
static u32 framecount;

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


// performance viewer stuff

// maximum number of samples to take per measurement frame
#define NUM_SAMPLES 100
// number of measurement frames.  There is currently no point in using 
// more than 1
#define NUM_FRAMES  1
// total number of measurement events
#define NUM_TYPES   3

// IDs for each event
#define RENDER_EVENT 0
#define ANIM_EVENT   1
#define GRAPH_EVENT  2

#define WHITE            {255, 255, 255, 255}
#define BLACK            {00, 00, 00, 255}
#define YELLOW           {200, 200, 0, 200}


// The performance graph will be rendered into a display list.
// In multi-fifo mode, the display list should be multi-buffered
// to avoid changing the DL while the GP is rendering
static u8*      DL;
static u32      DLSize;

static Mtx           v;       // view matrix

// default display list size for displaying graphs
#define         DEFAULT_DL_SIZE (48*1024)

/*---------------------------------------------------------------------------*
   Application main loop
 *---------------------------------------------------------------------------*/
void main ( void )
{
    DEMOInit(NULL);  // init os, pad, gx, vi
    MyPerfInit();
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

        PERFStartFrame();
        PERFStartAutoSampling(1.0f);

        DEMOPadRead(); // Update pad status (using DEMOPad library)

        PERFEventStart(ANIM_EVENT);
        AnimTick(ViewMtx, ModelMtx);    
        PERFEventEnd(ANIM_EVENT);


        // begin render timing
        PERFEventStart(RENDER_EVENT);
        DEMOBeforeRender();

        DrawTick(ViewMtx, ModelMtx);    // Draw the model.
        MyDoneRender();  // Wait for vertical retrace.

        OSStopStopwatch(&FrameRate); 
#ifndef __SINGLEFRAME
    }
#endif // __SINGLEFRAME

    ReportStatistics();
}


/*---------------------------------------------------------------------------*
   Functions
 *---------------------------------------------------------------------------*/

// wrappers around OSAlloc and OSFree for MyPerfInit
static void DoFree(void* block)
{
    OSFree(block);
}

static void* MemAlloc( u32 size )
{
    void* pNew;

    pNew = OSAlloc( size );
    if( !pNew )
    {
        OSReport( "error: MemAlloc: failed to allocate %d contiguous bytes.\n", size );
        OSReport( "                 free memory left: %d\n", OSCheckHeap(0) );
        OSHalt( "" );
    }

    return pNew;
}


/*---------------------------------------------------------------------------*
    Name:           FastDrawInit
    
    Description:    restores GXinit defaults for channel control, blending
                    z mode and TEV
                    
    Arguments:      none
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void FastDrawInit( void )
{ 
    GXColor white = WHITE;
    GXColor black = BLACK;
    GXSetChanCtrl(
        GX_COLOR0A0,
        GX_DISABLE,
        GX_SRC_REG,
        GX_SRC_VTX,
        GX_LIGHT_NULL,
        GX_DF_NONE,
        GX_AF_NONE );

    // important to set these to non-transparent colors, otherwise
    // performance will take a hit.
    GXSetChanAmbColor(GX_COLOR0A0, black);
    GXSetChanMatColor(GX_COLOR0A0, white);

    GXSetBlendMode( GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR );
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetZCompLoc(GX_TRUE);
    GXSetNumTexGens(1);
    GXSetNumChans(1);  // Enable light channel; by default = vertex color
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);

    GXSetTevOp(GX_TEVSTAGE0, GX_DECAL);        
}


/*---------------------------------------------------------------------------*
    Name:           MyPerfInit
    
    Description:    Sets up the PERF library and events.  Also allocates
                    memory for display lists for the graph.
                    
    Arguments:      none
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void MyPerfInit()
{
    GXColor yellow = YELLOW;
    
    PERFInit(NUM_SAMPLES, NUM_FRAMES, NUM_TYPES, MemAlloc, DoFree, FastDrawInit);
    PERFSetEvent(RENDER_EVENT,  "RENDER",   PERF_CPU_GP_EVENT);
    PERFSetEvent(ANIM_EVENT,    "ANIM",     PERF_CPU_EVENT);
    PERFSetEvent(GRAPH_EVENT,   "GRAPH",    PERF_CPU_GP_EVENT);
    PERFSetEventColor(GRAPH_EVENT, yellow);

    PERFSetDrawBWBarKey(TRUE);

    // create display list buffer
    DL = MemAlloc(DEFAULT_DL_SIZE);
}


/*---------------------------------------------------------------------------*
    Name:           MyDoneRender
    
    Description:    Supplants DEMODoneRender, as we want to render a new 
                    display list for the graph
                    
    Arguments:      none
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void MyDoneRender( void )
{
    extern void*  DemoFrameBuffer1;   // static global variables from DEMOInit.c
    extern void*  DemoFrameBuffer2;
    extern void*  DemoCurrentBuffer;
    BOOL          enabled;

    PERFEventEnd(RENDER_EVENT);


    PERFEventStart(GRAPH_EVENT);    

    if (framecount > 0)
    {
        PERFPreDraw();
        // if not first frame, we have a display list to show.
        GXCallDisplayList(DL,DLSize);
        PERFPostDraw();
        // since not all state will be restored by a display list,
        // call our draw init again
        FastDrawInit();
        GXClearVtxDesc();        // update shadow state
        GXInvalidateVtxCache();
    }
    PERFEventEnd(GRAPH_EVENT);    

    framecount++;

    GXCopyDisp(DemoCurrentBuffer, GX_TRUE);

    GXDrawDone();        
    // wait for copy out
    // this lets us capture the end of the render event
    enabled = OSDisableInterrupts();
    PERFEndFrame(); 
    PERFStopAutoSampling();
    
    // draw graph for next frame's DL
    GXBeginDisplayList(DL, DEFAULT_DL_SIZE);
    PERFDumpScreen();
    DLSize = GXEndDisplayList();
    OSRestoreInterrupts(enabled);
    
    // Wait for vertical retrace and set the next frame buffer
    // Display the buffer which was just filled by GXCopyDisplay
    DEMOSwapBuffers();

}


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
}

/*===========================================================================*/
