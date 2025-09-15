
/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     previewer.c

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <charPipeline.h>
#include <demo.h>

#ifdef flagWIN32
#include <CharPipe/FileLib.h>
#endif

#include <string.h>

/*---------------------------------------------------------------------------*
   Forward references
 *---------------------------------------------------------------------------*/

void        main            ( void ); 

static void CameraInit      ( float scale );
static void DrawInit        ( void );
static void LightInit       ( void );

static void DrawTick        ( Mtx v );
static void MakeModelMtx    ( Vec xAxis, Vec yAxis, Vec zAxis, Mtx m );
static void AnimSetSpeed    ( OSStopwatch *swAnim, f32 animHz );
static void AnimTick        ( void );
static void DrawCube        ( void );

static void LoadFiles       ( void );

static void PrintMenu       ( void );
static void WriteStatsData  ( void );
static void DumpStatsFile   ( void );

/*---------------------------------------------------------------------------*
   Constants
 *---------------------------------------------------------------------------*/

#define NUM_FRAME_CAPTURES  256

/*---------------------------------------------------------------------------*
   Local structures
 *---------------------------------------------------------------------------*/
typedef struct
{
    u8  whichCamera;
    u8  animFile;

    Vec CamX;   
    Vec CamY;
    Vec CamZ;

    Vec FreeCamPos;

    float   CameraLocScale;

    u32     CurrentDispObj;

    Vec     LightPoint;
    float   LightDistance;

    float   animTime;

} SingleFrameInfo;

typedef struct
{
    char            DataFileName[255];
    SingleFrameInfo FileOutput[NUM_FRAME_CAPTURES];

} DataDump;

/*---------------------------------------------------------------------------*
   Global variables
 *---------------------------------------------------------------------------*/

DataDump    SaveData;

u8  OutputFileState = 0;
u32 CurrentOutputFrame = 0;

Vec CamX = {1.0F, 0.0F, 0.0F};  
Vec CamY = {0.0F, 1.0F, 0.0F};
Vec CamZ = {0.0F, 0.0F, 1.0F};

Mtx v, m;

// Scale for the camera's distance from the object
float CameraLocScale = 16.0F;

DODisplayObjPtr dispObj;
GEOPalettePtr pal;

u32 CurrentDispObj;

ACTActorPtr actor = 0;
CTRLControlPtr actorControl;

ANIMBankPtr animBank;

u32 refreshRate = 2;

LITLightPtr light1;

Vec LightPoint = {0.0F, 0.0F, 128.0F};
float LightDistance = 1.0F;

Mtx CubeWorldMatrix;

u8 CurrentControl = 0;
u8 CurrentCamera = 0;

Vec FreeCamX = {1.0F, 0.0F, 0.0F};  
Vec FreeCamY = {0.0F, 1.0F, 0.0F};
Vec FreeCamZ = {0.0F, 0.0F, 1.0F};
float FreeCameraLocScale = 16.0F;
Vec FreeCamPos = {0.0F, 0.0F, -128.0F};

Vec *currentX = &CamX, *currentY = &CamY;

char *DataFileName = NULL;
u8   Quit = 0;

/*---------------------------------------------------------------------------*
   Application main loop
 *---------------------------------------------------------------------------*/
void main ( void )
{
    OSStopwatch swAnim;                      // stopwatch for single frame animation time

    DEMOInit(NULL);

    CameraInit(CameraLocScale);

    LightInit();
           
    DrawInit();

    LoadFiles();

    PrintMenu();

    OSInitStopwatch(&swAnim, "anim");       // time between frames

    // While the quit button is not pressed
    while( !Quit )    
    {   
        OSStartStopwatch(&swAnim); // swAnim stopwatch runs once per frame;
                                   // includes time for VIWaitForRetrace().

        AnimTick();                // Do animation based on input
        
        DEMOBeforeRender();

        DrawTick(v);               // Draw the model.

        DEMODoneRender();

        AnimSetSpeed(&swAnim, (f32)60.0f / refreshRate);

        OSStopStopwatch(&swAnim);  // reset swAnim for next frame
        OSResetStopwatch(&swAnim);
    }

    OSReport( "End of previewer.\n" );
}

/*---------------------------------------------------------------------------*
   Functions
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
    Name:           CameraInit
    
    Description:    Initialize the projection matrix and load into hardware.
                    
    Arguments:      v   view matrix to be passed to ViewInit
                    cameraLocScale  scale for the camera's distance from the 
                                    object - to be passed to ViewInit
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void CameraInit      ( float scale )
{
    Mtx44 p;

    GXSetViewport(0.0F, 640.0F, 0.0F, 480.0F, 0.0F, 1.0F);
    
    MTXFrustum(p, .24F * scale,-.24F * scale,
               -.32F * scale, .32F * scale, 
               .5F * scale, 20.0F * scale);

    GXSetProjection(p, GX_PERSPECTIVE);
}

/*---------------------------------------------------------------------------*
    Name:           DrawInit
    
    Description:    Initialize a geo Palette and display object.  
                    
    Arguments:      
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void DrawInit    ( void )
{
    GXSetZCompLoc(GX_FALSE);
    
    CSHInitDisplayCache();

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, 
                       GX_LO_CLEAR);

    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_GREATER, 0);
}

/*---------------------------------------------------------------------------*
    Name:           DrawTick
    
    Description:    Draw the current model once.  
                    
    Arguments:      v       view matrix
                    m       model matrix
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void DrawTick ( Mtx v )
{
    DrawCube();
    LITXForm(light1, v);

    if(dispObj)
        DORender(dispObj, v, 1, light1);

    if(actor)
    {
        ACTBuildMatrices(actor);
        ACTRender(actor, v, 1, light1);  
    }
}

/*---------------------------------------------------------------------------*
    Name:           MakeModelMtx
    
    Description:    computes a model matrix from 3 vectors representing an 
                    object's coordinate system.
                    
    Arguments:      xAxis   vector for the object's X axis
                    yAxis   vector for the object's Y axis
                    zAxis   vector for the object's Z axis
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void MakeModelMtx ( Vec xAxis, Vec yAxis, Vec zAxis, Mtx m )
{
    VECNormalize(&xAxis,&xAxis);
    VECNormalize(&yAxis,&yAxis);
    VECNormalize(&zAxis,&zAxis);

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
    Name:           AnimSetSpeed
    
    Description:    Sets the speed of the animation rate depending on the frame
                    rate.
                    
    Arguments:      swAnim - current animation stopwatch
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void AnimSetSpeed( OSStopwatch *swAnim, f32 animHz )
{
    OSTime      swTime;                      // current stopwatch time    
    f32         animStep;                    // animation step adjusted per frame to
                                             // produce fixed rate animation
    f32         frameTime;                   // stopwatch time in seconds

    swTime    = OSCheckStopwatch( swAnim );
    frameTime = (f32)OSTicksToSeconds( ((f32)swTime) );  
    animStep  = frameTime * animHz;

    if(actor && animBank)
        ACTSetSpeed(actor, animStep);
}

/*---------------------------------------------------------------------------*
    Name:           AnimTick
    
    Description:    Animates the camera and object based on the joystick's 
                    state.
                    
    Arguments:      m   model matrix
                    v   view matrix
                    cameraLocScale  scale value for the camera's distance
                                    to the object.
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void AnimTick ( void )
{
    u16       buttons;
    u16       buttonsDown;
    s8        stickX;
    s8        stickY;
    s8        subStickX;
    s8        subStickY;
    u8        triggerL;
    u8        triggerR;
    Mtx       rot;

    DEMOPadRead();

    buttons     = DEMOPadGetButton(0);
    buttonsDown = DEMOPadGetButtonDown(0);
    stickX      = DEMOPadGetStickX(0);
    stickY      = DEMOPadGetStickY(0);
    subStickX   = DEMOPadGetSubStickX(0);
    subStickY   = DEMOPadGetSubStickY(0);
    triggerL    = DEMOPadGetTriggerL(0);
    triggerR    = DEMOPadGetTriggerR(0);

    if(buttonsDown & PAD_BUTTON_MENU)
    {
        Quit = 1;
    }

    if(triggerL > 170 && triggerR > 170)
    {
        OutputFileState = 1;
    }
    else
    {
        if(buttonsDown & PAD_TRIGGER_L)
            LoadFiles();    

        // Change animation speed
        if(buttonsDown & PAD_TRIGGER_R)
        {
            refreshRate++;

            if (refreshRate > 6)
                refreshRate = 1;
        
            OSReport( "Animation rate %i Hz\n", 60 / refreshRate );
        }
    }

    if(CurrentControl == 0)
    {
        if(CurrentCamera == 0)
        {
            // Move camera
            if(buttons & PAD_BUTTON_Y)
            {
                CameraLocScale *= .95F;
                if(CameraLocScale < 0.001F)
                    CameraLocScale = 0.001F;
                CameraInit(CameraLocScale);
            }
            if(buttons & PAD_BUTTON_A)
            {
                CameraLocScale *= 1.05F;        
                CameraInit(CameraLocScale);
            }

            // Rotate camera
            if(stickX > 30 || stickX < -30)
             {
                if(stickX > 30) 
                    MTXRotAxis(rot, &CamY, -3.0F);
                else if(stickX < -30)
                    MTXRotAxis(rot, &CamY, 3.0F);

                MTXMultVec(rot, &CamX, &CamX);
                MTXMultVec(rot, &CamZ, &CamZ); 
            }

            if(stickY > 30 || stickY < -30)
            {
                if(stickY > 30)
                    MTXRotAxis(rot, &CamX, -3.0F);
                else if(stickY < -30)
                    MTXRotAxis(rot, &CamX, 3.0F);

                MTXMultVec(rot, &CamY, &CamY);
                MTXMultVec(rot, &CamZ, &CamZ);          
            }
            
            MakeModelMtx(CamX, CamY, CamZ, v);  // Make a new model matrix
            MTXTranspose(v, v);
            MTXTrans(rot, 0.0F, 0.0F, -8.0F * CameraLocScale);
            MTXConcat(rot, v, v);
        }
        else if(CurrentCamera == 1)
        {
            if(subStickX > 30 || subStickX < -30)
             {
                if(subStickX > 30) 
                    MTXRotAxis(rot, &FreeCamZ, -3.0F);
                else if(subStickX < -30) 
                    MTXRotAxis(rot, &FreeCamZ, 3.0F);

                MTXMultVec(rot, &FreeCamX, &FreeCamX);
                MTXMultVec(rot, &FreeCamY, &FreeCamY); 
            }

            if(subStickY > 30 || subStickY < -30)
            {
                if(subStickY > 30)
                {
                    FreeCameraLocScale *= .95F;
                    if(FreeCameraLocScale < 0.001F)
                        FreeCameraLocScale = 0.001F;
                    CameraInit(FreeCameraLocScale);
                }
                else if(subStickY < -30)
                {
                    FreeCameraLocScale *= 1.05F;        
                    CameraInit(FreeCameraLocScale);
                }           
            }
        
            // Move camera in or out
            if(buttons & PAD_BUTTON_Y)
            {
                FreeCamPos.x += FreeCamZ.x;
                FreeCamPos.y += FreeCamZ.y;
                FreeCamPos.z += FreeCamZ.z;
            }
            if(buttons & PAD_BUTTON_A)
            {
                FreeCamPos.x -= FreeCamZ.x;
                FreeCamPos.y -= FreeCamZ.y;
                FreeCamPos.z -= FreeCamZ.z;
            }
            
            // Rotate camera
            if(stickX > 30 || stickX < -30)
             {
                if(stickX > 30) 
                    MTXRotAxis(rot, &FreeCamY, -3.0F);
                else if(stickX < -30) 
                    MTXRotAxis(rot, &FreeCamY, 3.0F);

                MTXMultVec(rot, &FreeCamX, &FreeCamX);
                MTXMultVec(rot, &FreeCamZ, &FreeCamZ); 
            }

            if(stickY > 30 || stickY < -30)
            {
                if(stickY > 30)
                    MTXRotAxis(rot, &FreeCamX, -3.0F);
                else if(stickY < -30) 
                    MTXRotAxis(rot, &FreeCamX, 3.0F);

                MTXMultVec(rot, &FreeCamY, &FreeCamY);
                MTXMultVec(rot, &FreeCamZ, &FreeCamZ);          
            }
        
            MakeModelMtx(FreeCamX, FreeCamY, FreeCamZ, v);  // Make a new model matrix
            MTXTranspose(v, v);
            MTXTrans(rot, FreeCamPos.x, FreeCamPos.y, FreeCamPos.z);
            MTXConcat(v, rot, v);
        }
    }
    else if(CurrentControl == 1)
    {
        // Rotate light
        if(stickX > 30 || stickX < -30)
         {
            if(stickX > 30) 
                MTXRotAxis(rot, currentY, 3.0F);
            else if(stickX < -30) 
                MTXRotAxis(rot, currentY, -3.0F);

            MTXMultVec(rot, &LightPoint, &LightPoint);
        }

        if(stickY > 30 || stickY < -30)
        {
            if(stickY > 30)
                MTXRotAxis(rot, currentX, 3.0F);
            else if(stickY < -30)
                MTXRotAxis(rot, currentX, -3.0F);

            MTXMultVec(rot, &LightPoint, &LightPoint);          
        }
        
        if(buttons & PAD_BUTTON_Y)
        {
            LightDistance *= .95F;
            if(LightDistance < 0.001F)
                LightDistance = 0.001F;
        }
        if(buttons & PAD_BUTTON_A)
            LightDistance *= 1.05F;
    }

    if(buttonsDown & PAD_BUTTON_B && buttonsDown & PAD_BUTTON_X)
    {
        if(pal)
        {
            if(CurrentDispObj < pal->numDescriptors - 1)
                CurrentDispObj++;
            else
                CurrentDispObj = 0;
        
            if(dispObj)
                DORelease(&dispObj);
            DOGet(&dispObj, pal, (u16)CurrentDispObj, 0);    
        }

        if(animBank)
        {
            if(CurrentDispObj < animBank->numSequences - 1)
                CurrentDispObj++;
            else
                CurrentDispObj = 0;
        
            if(actor)
                ACTSetAnimation(actor, animBank, 0, (u16)CurrentDispObj, 0.0F);
        }
    }
    else
    {
        if( buttonsDown & PAD_BUTTON_B )
        {
            CurrentControl ++;
            if(CurrentControl > 1)
                CurrentControl = 0;

            PrintMenu();
        }

        if( buttonsDown & PAD_BUTTON_X )
        {
            CurrentCamera ++;
            if(CurrentCamera > 1)
                CurrentCamera = 0;

            switch(CurrentCamera)
            {
                case 0:
                    CameraInit(CameraLocScale);
                    MakeModelMtx(CamX, CamY, CamZ, v);  // Make a new model matrix
                    MTXTranspose(v, v);
                    MTXTrans(rot, 0.0F, 0.0F, -8.0F * CameraLocScale);
                    MTXConcat(rot, v, v);
                    currentX = &CamX;
                    currentY = &CamY;
                    break;
                case 1:
                    CameraInit(FreeCameraLocScale);
                    MakeModelMtx(FreeCamX, FreeCamY, FreeCamZ, v);  // Make a new model matrix
                    MTXTranspose(v, v);
                    MTXTrans(rot, FreeCamPos.x, FreeCamPos.y, FreeCamPos.z);
                    MTXConcat(v, rot, v);
                    currentX = &FreeCamX;
                    currentY = &FreeCamY;
                    break;
            }

            PrintMenu();
        }
    }

    if(OutputFileState)
    {
        WriteStatsData();
        OSReport("Capturing frame %d\n", CurrentOutputFrame);
        CurrentOutputFrame ++;

        if(CurrentOutputFrame == NUM_FRAME_CAPTURES)
        {
            DumpStatsFile();
            CurrentOutputFrame = 0;
            OutputFileState = 0;
        }
    }

    if(animBank)
        ACTTick(actor);
}

/*---------------------------------------------------------------------------*
    Name:           
    
    Description:    
                    
    Arguments:      
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void LightInit ( void )
{
    GXColor white = {255, 255, 255, 255};

    LITAlloc(&light1);  

    LITInitAttn(light1, 1.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F);

    LITInitPos(light1, 0.0F, 0.0F, 0.0F);
    LITInitColor(light1, white);       

    LITAttach(light1, (Ptr)CubeWorldMatrix, PARENT_MTX);
}

/*---------------------------------------------------------------------------*
    Name:           
    
    Description:    
                    
    Arguments:      
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void DrawCube ( void )
{
    Mtx m;
    GXColor red = {255, 0, 0, 255};

    //disable lighting
    GXSetNumChans(1);
    GXSetChanMatColor(GX_COLOR0, red);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);

    //pass the material color
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);

    MTXTrans(CubeWorldMatrix, LightPoint.x * LightDistance, 
             LightPoint.y * LightDistance, LightPoint.z * LightDistance);

    MTXConcat(v, CubeWorldMatrix, m);
    GXLoadPosMtxImm(m, GX_PNMTX0);

    GXDrawCube();    
}

/*---------------------------------------------------------------------------*
    Name:           LoadFiles           
    
    Description:    Loads the prevload.txt file and all of the data files
                    specified therein
                    
    Arguments:      none
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void LoadFiles  ( void )
{
    DVDFileInfo dfi;
    char *cursor;
    Mtx rot;

    if(pal)
    {
        GEOReleasePalette(&pal);
        pal = 0;
    }

    if(actor)

    {
        ACTRelease(&actor);
        actor = 0;
    }

    if(animBank)
    {
        ANIMRelease(&animBank);
        animBank = 0;
    }

#ifdef flagWIN32
	if(WINGetFile("Dolphin Files (*.gpl; *.act; *.anm)\0*.gpl;*.act;*.anm\0")) {
		DVDSetRoot(FilePath);
	}

    cursor = FileName;
	DataFileName = FileName;
#else
    if( DataFileName )
        OSFree( DataFileName );
    DVDChangeDir("preview");
    DVDOpen("prevload.txt", &dfi);
    DataFileName = (char *)OSAlloc(OSRoundUp32B(dfi.length));
    DVDRead(&dfi, DataFileName, (s32)OSRoundUp32B(dfi.length), 0);
    DVDClose(&dfi);

    //determine type of file to load
    cursor = DataFileName;
#endif

    while(*cursor != '.')
    {
        cursor ++;
        ASSERTMSG(*cursor, "LoadFiles: Filename has no extension");
    }

    cursor[4] = '\0'; //terminate the string with a \0

    OSReport( "Loading %s...\n", DataFileName );

    if(cursor[1] == 'g' || cursor[1] == 'G') // we know it's a GPL file!
    {
        GEOGetPalette(&pal, DataFileName);
        
        DOGet(&dispObj, pal, (u16)CurrentDispObj, 0); 
        MTXIdentity(rot);
        DOSetWorldMatrix(dispObj, rot);
    }
    else if(cursor[1] == 'a' || cursor[1] == 'A')
    {
        if(cursor[2] == 'c' || cursor[2] == 'C') // we know it's an actor file!
        {
            ACTGet(&actor, DataFileName);
            
            actorControl = ACTGetControl(actor);
            MTXIdentity(rot);
            CTRLSetMatrix(actorControl, rot);
        }
        else if(cursor[2] == 'n' || cursor[2] == 'N') // we know it's an anim file!
        {
            ANIMGet(&animBank, DataFileName);
        
            cursor[2] = 'c';
            cursor[3] = 't';

            ACTGet(&actor, DataFileName);
            
            actorControl = ACTGetControl(actor);
            CTRLInit(actorControl);

            ACTSetAnimation(actor, animBank, 0, (u16)CurrentDispObj, 0.0F);

            //set File Name back for data capture possibilities
            cursor[2] = 'n';
            cursor[3] = 'm';
        }
    }

    DVDChangeDir("..");
}

/*---------------------------------------------------------------------------*
    Name:           PrintMenu
    
    Description:    prints the menu for current controls
                    
    Arguments:      none
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void PrintMenu( )
{
    char* str1, *str2;

    if( CurrentCamera )
    {
        str1 = "Free-floating";
        str2 = "Trackball";
    }
    else
    {
        str1 = "Trackball";
        str2 = "Free-floating";
    }

    OSReport( "-------------------------------\n" );

    if( CurrentControl )
    {
        // Light control
        OSReport( "Light Control (Viewing with %s camera)\n\n", str1 );
        OSReport( "primary stick      : rotate light about origin\n" );
        OSReport( "pad button Y, A    : zoom light to/from origin\n" );
        OSReport( "pad button B       : change to camera control\n" );
    }
    else
    {
        // Camera control
        OSReport( "%s Camera Control\n\n", str1 );
        OSReport( "primary stick      : rotate camera\n" );
        if( CurrentCamera )
        {
            OSReport( "substick left/right: roll the camera\n" );
            OSReport( "substick   up/down : scales camera frustum\n" );
        }
        OSReport( "pad button Y, A    : move camera forwards/backwards\n" );
        OSReport( "pad button B       : change to light control\n" );
    }

    OSReport( "pad button X       : change to %s camera\n", str2 );
    if( pal )
        OSReport( "pad button X AND B : change to next display object\n" );
    else if( animBank )
        OSReport( "pad button X AND B : change to next animation sequence\n" );
    OSReport( "trigger    L       : reloads file\n" );
    OSReport( "trigger    R       : change animation rate\n" );
    OSReport( "pad button menu    : quit\n" );
    OSReport( "\n" );
    OSReport( "-------------------------------\n" );
}

/*---------------------------------------------------------------------------*
    Name:           WriteStatsData
    
    Description:    stores one frame worth of stats data
                    
    Arguments:      none
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void WriteStatsData  ( void )
{
    u32 i;

    SaveData.FileOutput[CurrentOutputFrame].whichCamera = CurrentCamera;
    
    if(CurrentCamera == 0)
    {
        SaveData.FileOutput[CurrentOutputFrame].CamX = CamX;    
        SaveData.FileOutput[CurrentOutputFrame].CamY = CamY;
        SaveData.FileOutput[CurrentOutputFrame].CamZ = CamZ;
        SaveData.FileOutput[CurrentOutputFrame].CameraLocScale = CameraLocScale;
    }
    else
    {
        SaveData.FileOutput[CurrentOutputFrame].CamX = FreeCamX;    
        SaveData.FileOutput[CurrentOutputFrame].CamY = FreeCamY;
        SaveData.FileOutput[CurrentOutputFrame].CamZ = FreeCamZ;
        SaveData.FileOutput[CurrentOutputFrame].CameraLocScale = FreeCameraLocScale;
    }

    SaveData.FileOutput[CurrentOutputFrame].FreeCamPos = FreeCamPos;

    

    SaveData.FileOutput[CurrentOutputFrame].CurrentDispObj = CurrentDispObj;

    SaveData.FileOutput[CurrentOutputFrame].LightPoint = LightPoint;
    SaveData.FileOutput[CurrentOutputFrame].LightDistance = LightDistance;
    
    if(animBank)
    {
        for(i = 0; i < actor->totalBones; i++)
        {
            if(actor->boneArray[i]->animPipe)
            {
                SaveData.FileOutput[CurrentOutputFrame].animTime = actor->boneArray[i]->animPipe->time;
                break;
            }
        }
        SaveData.FileOutput[CurrentOutputFrame].animFile = 1;
    }
    else
    {
        SaveData.FileOutput[CurrentOutputFrame].animTime = 0.0F;
        SaveData.FileOutput[CurrentOutputFrame].animFile = 0;
    }
}

/*---------------------------------------------------------------------------*
    Name:           DumpStatsFile
    
    Description:    writes stats data to disk
                    
    Arguments:      none
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void DumpStatsFile   ( void )
{
#ifdef MAC
    FILE *fp = fopen("Preview.da0", "w+b"); // for SPRUCE_MARLIN 8.3 limit

    strcpy(SaveData.DataFileName, DataFileName);
    
    fwrite(&SaveData, sizeof(DataDump), 1, fp);
    
    fclose(fp);
#endif
}


        
