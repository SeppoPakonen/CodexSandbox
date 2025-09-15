/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     raceModels.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/charPipeline/src/raceModels.c $
    
    6     11/10/00 1:10a John
    
    5     9/16/00 12:26p Ryan
    shader optimization update
    
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

#include "raceModels.h"
#include "racePad.h"
#include "raceCamera.h"
#include "raceShaders.h"
#include "raceSphereMap.h"

/****************************************************************************/

typedef struct
{
    f32 distance;
    Mtx transform;

    CTRLControlPtr *actorControl;
    CTRLControlPtr lightControl;

    void * (*drawFunction) (void);


} DistanceTracker;

static DistanceTracker  TrackerArray[20];
static DistanceTracker  *TrackerSort[20];

/****************************************************************************/

static ACTActorPtr Viper[2] = { 0, 0 };
static CTRLControlPtr ViperControl[2];

static ACTActorPtr ViperFloor = 0;
static CTRLControlPtr ViperFloorControl;

static ACTActorPtr Ferarri[2] = { 0, 0 };
static CTRLControlPtr FerarriControl[2];

static ACTActorPtr FerarriFloor = 0;
static CTRLControlPtr FerarriFloorControl;

static ACTActorPtr Lotus[2] = { 0, 0 };
static CTRLControlPtr LotusControl[2];

static ACTActorPtr LotusFloor = 0;
static CTRLControlPtr LotusFloorControl;

static ACTActorPtr SpecularGeometry = 0;
static CTRLControlPtr SpecularControl;

static ACTActorPtr BigFloor = 0;
static CTRLControlPtr BigFloorControl;

static ACTActorPtr BigFloor5 = 0;
static CTRLControlPtr BigFloor5Control;

static ACTActorPtr BigFloor8 = 0;
static CTRLControlPtr BigFloor8Control;

static ACTActorPtr BigFloor12 = 0;
static CTRLControlPtr BigFloor12Control;

static u8 CarDisplayMode = 0;
u8 CubeMapDisplay = 0;
static u8 CurrentCar = 0;
static float CarRotation = 0;
static float RotationSpeed = 1;

LITLightPtr FerarriLight, ViperLight, LotusLight;
static CTRLControlPtr FerarriLightControl;
static CTRLControlPtr ViperLightControl;
static CTRLControlPtr LotusLightControl; 

static u8 LOD = 0;

/****************************************************************************/

static void SetCarShader    ( SHDRCompiled *shader, DODisplayObjPtr dispObj, u32 combineSetting, GXTexObj *texture, BOOL colorChanUsed, Ptr data );
static void ViperDraw       ( void );
static void FerarriDraw     ( void );
static void LotusDraw       ( void );

static void InitTracker ( Mtx transform, CTRLControl **actorControl, 
                          CTRLControl *lightControl, void *drawFunction, u8 index );

static void DrawModelList ( u8 numModels );

/****************************************************************************/
/*---------------------------------------------------------------------------*
    Name:           InitModels
    
    Description:	Initializes the models for the demo
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
void InitModels  ( void )
{   
    ACTGet(&Viper[0], "viper.act");
    ACTShow(Viper[0]);
    ViperControl[0] = ACTGetControl(Viper[0]);
    CTRLInit(ViperControl[0]);
    ACTSetEffectsShader(Viper[0], (Ptr)SetCarShader, 0); 
    ACTSetAmbientPercentage(Viper[0], 35.0F);

    ACTGet(&Viper[1], "vip_low.act");
    ACTShow(Viper[1]);
    ViperControl[1] = ACTGetControl(Viper[1]);
    CTRLInit(ViperControl[1]);
    ACTSetEffectsShader(Viper[1], (Ptr)SetCarShader, 0); 
    ACTSetAmbientPercentage(Viper[1], 35.0F);

    ACTGet(&ViperFloor, "vip_flr.act");
    ACTShow(ViperFloor);
    ViperFloorControl = ACTGetControl(ViperFloor);
    CTRLInit(ViperFloorControl);
    ACTSetAmbientPercentage(ViperFloor, 100.0F);

    ///////////////////////////////
#if defined(HW1) || defined(HW2)
    ACTGet(&Ferarri[0], "f50.act");
    ACTShow(Ferarri[0]);
    FerarriControl[0] = ACTGetControl(Ferarri[0]);
    CTRLInit(FerarriControl[0]);
    ACTSetEffectsShader(Ferarri[0], (Ptr)SetCarShader, 0); 
    ACTSetAmbientPercentage(Ferarri[0], 35.0F);

    ACTGet(&Ferarri[1], "f50_low.act");
    ACTShow(Ferarri[1]);
    FerarriControl[1] = ACTGetControl(Ferarri[1]);
    CTRLInit(FerarriControl[1]);
    ACTSetEffectsShader(Ferarri[1], (Ptr)SetCarShader, 0); 
    ACTSetAmbientPercentage(Ferarri[1], 35.0F);

    ACTGet(&FerarriFloor, "f50_flr.act");
    ACTShow(FerarriFloor);
    FerarriFloorControl = ACTGetControl(FerarriFloor);
    CTRLInit(FerarriFloorControl);
    ACTSetAmbientPercentage(FerarriFloor, 100.0F);

    //turn off shader for Ferarri
    DOSetEffectsShader(((Ferarri[0])->boneArray[1])->dispObj , 0, 0); 

    ///////////////////////////////

    ACTGet(&Lotus[0], "lotus.act");
    ACTShow(Lotus[0]);
    LotusControl[0] = ACTGetControl(Lotus[0]);
    CTRLInit(LotusControl[0]);
    ACTSetEffectsShader(Lotus[0], (Ptr)SetCarShader, 0); 
    ACTSetAmbientPercentage(Lotus[0], 35.0F);

    ACTGet(&Lotus[1], "lot_low.act");
    ACTShow(Lotus[1]);
    LotusControl[1] = ACTGetControl(Lotus[1]);
    CTRLInit(LotusControl[1]);
    ACTSetEffectsShader(Lotus[1], (Ptr)SetCarShader, 0); 
    ACTSetAmbientPercentage(Lotus[1], 35.0F);

    ACTGet(&LotusFloor, "lot_flr.act");
    ACTShow(LotusFloor);
    LotusFloorControl = ACTGetControl(LotusFloor);
    CTRLInit(LotusFloorControl);
    ACTSetAmbientPercentage(LotusFloor, 100.0F);

    ///////////////////////////////
    ACTGet(&BigFloor, "big_flr.act");
    ACTShow(BigFloor);
    BigFloorControl = ACTGetControl(BigFloor);
    CTRLInit(BigFloorControl);
    ACTSetAmbientPercentage(BigFloor, 100.0F);

    ///////////////////////////////
    ACTGet(&BigFloor5, "big_flr5.act");
    ACTShow(BigFloor5);
    BigFloor5Control = ACTGetControl(BigFloor5);
    CTRLInit(BigFloor5Control);
    ACTSetAmbientPercentage(BigFloor5, 100.0F);

    ///////////////////////////////
    ACTGet(&BigFloor8, "big_flr8.act");
    ACTShow(BigFloor8);
    BigFloor8Control = ACTGetControl(BigFloor8);
    CTRLInit(BigFloor8Control);
    ACTSetAmbientPercentage(BigFloor8, 100.0F);

    ///////////////////////////////
    ACTGet(&BigFloor12, "big_fl12.act");
    ACTShow(BigFloor12);
    BigFloor12Control = ACTGetControl(BigFloor12);
    CTRLInit(BigFloor12Control);
    ACTSetAmbientPercentage(BigFloor12, 100.0F);

#endif
    ///////////////////////////////

    ACTGet(&SpecularGeometry, "flares.act");
    ACTShow(SpecularGeometry);
    SpecularControl = ACTGetControl(SpecularGeometry);
    CTRLInit(SpecularControl);
    ACTSetAmbientPercentage(SpecularGeometry, 100.0F);
}

/*---------------------------------------------------------------------------*
    Name:			ViperDraw
    
    Description:	Draw the Viper model once.  
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static void ViperDraw ( void )
{
    LITXForm(ViperLight, cameraMatrix);

    ACTBuildMatrices(Viper[LOD]);
    ACTRender(Viper[LOD], cameraMatrix, 1, ViperLight); 
}

/*---------------------------------------------------------------------------*
    Name:			ViperDraw
    
    Description:	Draw the Viper model once.  
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static void FerarriDraw ( void )
{
    LITXForm(FerarriLight, cameraMatrix);
   
    ACTBuildMatrices(Ferarri[LOD]);
    ACTRender(Ferarri[LOD], cameraMatrix, 1, FerarriLight); 
}

/*---------------------------------------------------------------------------*
    Name:			ViperDraw
    
    Description:	Draw the Viper model once.  
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static void LotusDraw ( void )
{
    LITXForm(LotusLight, cameraMatrix);

    ACTBuildMatrices(Lotus[LOD]);
    ACTRender(Lotus[LOD], cameraMatrix, 1, LotusLight); 
}

/*---------------------------------------------------------------------------*
    Name:			ViperDraw
    
    Description:	Draw the Viper model once.  
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
void SpecularGeometryDraw ( Mtx m )
{
	ACTBuildMatrices(SpecularGeometry);

    ACTRender(SpecularGeometry, m, 0); 
}

/*---------------------------------------------------------------------------*
    Name:			CarDraw
    
    Description:	Draws the Current Car
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
void CarDraw ( void )
{
#if defined(HW1) || defined(HW2)
    Mtx trans, rot, rot1;
#endif

	switch(CurrentCar)
    {
#if defined(HW1) || defined(HW2)
        case 2:
            MTXRotDeg(rot, 'z', (float)CarRotation);
            InitTracker(rot, ViperControl, ViperLightControl, ViperDraw, 0);

            CTRLSetRotation(ViperFloorControl, 0.0F, 0.0F, (float)CarRotation);    
            ACTBuildMatrices(ViperFloor);
            ACTRender(ViperFloor, cameraMatrix, 0); 

            DrawModelList(1);
            break;
        case 0:
            MTXRotDeg(rot, 'z', (float)CarRotation);
            InitTracker(rot, FerarriControl, FerarriLightControl, FerarriDraw, 0);

            CTRLSetRotation(FerarriFloorControl, 0.0F, 0.0F, (float)CarRotation);    
            ACTBuildMatrices(FerarriFloor);
            ACTRender(FerarriFloor, cameraMatrix, 0); 

            DrawModelList(1);
            break;
        case 1:
            MTXRotDeg(rot, 'z', (float)CarRotation);
            InitTracker(rot, LotusControl, LotusLightControl, LotusDraw, 0);

            CTRLSetRotation(LotusFloorControl, 0.0F, 0.0F, (float)CarRotation);    
            ACTBuildMatrices(LotusFloor);
            ACTRender(LotusFloor, cameraMatrix, 0); 

            DrawModelList(1);
            break;
        //3 car case
        case 3:
            MTXRotDeg(rot1, 'z', (float)CarRotation);

            //set up Ferarri
            CTRLSetRotation(FerarriFloorControl, 0.0F, 0.0F, (float)CarRotation);    

            InitTracker(rot1, FerarriControl, FerarriLightControl, FerarriDraw, 0);

            //make viper matrix
            MTXTrans(trans, -600.0F, 100.0F, 0.0F);
            MTXRotDeg(rot, 'z', -30.0F);
            MTXConcat(trans, rot, rot);
            MTXConcat(rot1, rot, rot);
            InitTracker(rot, ViperControl, ViperLightControl, ViperDraw, 1);

            //make lotus Mtx
            MTXTrans(trans, 600.0F, 100.0F, 0.0F);
            MTXRotDeg(rot, 'z', 30.0F);
            MTXConcat(trans, rot, rot);
            MTXConcat(rot1, rot, rot);
            InitTracker(rot, LotusControl, LotusLightControl, LotusDraw, 2);

            CTRLSetRotation(BigFloorControl, 0.0F, 0.0F, (float)CarRotation);
            ACTBuildMatrices(BigFloor);
            ACTRender(BigFloor, cameraMatrix, 0); 

            DrawModelList(3);
            break;
        //5 car case
        case 4:           
            MTXTrans(trans, 0.0F, -700.0F, 0.0F);

            //make Ferarri matrix
            MTXRotDeg(rot, 'z', (float)CarRotation);
            MTXConcat(rot, trans, rot);
            InitTracker(rot, FerarriControl, FerarriLightControl, FerarriDraw, 0);

            //make viper matrix
            MTXRotDeg(rot, 'z', (float)CarRotation + 72.0F);
            MTXConcat(rot, trans, rot);
            InitTracker(rot, ViperControl, ViperLightControl, ViperDraw, 1);

            //make lotus Mtx
            MTXRotDeg(rot, 'z', (float)CarRotation + 144.0F);
            MTXConcat(rot, trans, rot);
            InitTracker(rot, LotusControl, LotusLightControl, LotusDraw, 2);

            //make second ferarri Mtx
            MTXRotDeg(rot, 'z', (float)CarRotation + 216.0F);
            MTXConcat(rot, trans, rot);
            InitTracker(rot, FerarriControl, FerarriLightControl, FerarriDraw, 3);

            //make lotus matrix
            MTXRotDeg(rot, 'z', (float)CarRotation + 288.0F);
            MTXConcat(rot, trans, rot);
            InitTracker(rot, LotusControl, LotusLightControl, LotusDraw, 4);

            CTRLSetRotation(BigFloor5Control, 0.0F, 0.0F, (float)CarRotation);
            ACTBuildMatrices(BigFloor5);
            ACTRender(BigFloor5, cameraMatrix, 0); 

            DrawModelList(5);
            break;

        //8 car case
        case 5:
            MTXRotDeg(rot, 'z', (float)CarRotation);

            //make viper matrix
            MTXRotDeg(rot1, 'z', -40.0F);
            MTXTrans(trans, -950.0F, 500.0F, 0.0F);
            MTXConcat(trans, rot1, rot1);
            MTXConcat(rot, rot1, rot1);
            InitTracker(rot1, ViperControl, ViperLightControl, ViperDraw, 0);

            //make viper matrix
            MTXRotDeg(rot1, 'z', 40.0F);
            MTXTrans(trans, 950.0F, 500.0F, 0.0F);
            MTXConcat(trans, rot1, rot1);
            MTXConcat(rot, rot1, rot1);
            InitTracker(rot1, ViperControl, ViperLightControl, ViperDraw, 1);

            //make viper matrix
            MTXRotDeg(rot1, 'z', 10.0F);
            MTXTrans(trans, 350.0F, -900.0F, 0.0F);
            MTXConcat(trans, rot1, rot1);
            MTXConcat(rot, rot1, rot1);
            InitTracker(rot1, ViperControl, ViperLightControl, ViperDraw, 2);

            //make lotus matrix
            MTXRotDeg(rot1, 'z', -90.0F);
            MTXTrans(trans, 0.0F, 900.0F, 0.0F);
            MTXConcat(trans, rot1, rot1);
            MTXConcat(rot, rot1, rot1);
            InitTracker(rot1, LotusControl, LotusLightControl, LotusDraw, 3);

            //make lotus matrix
            MTXRotDeg(rot1, 'z', 40.0F);
            MTXTrans(trans, 950.0F, -400.0F, 0.0F);
            MTXConcat(trans, rot1, rot1);
            MTXConcat(rot, rot1, rot1);
            InitTracker(rot1, LotusControl, LotusLightControl, LotusDraw, 4);

            //make lotus matrix
            MTXRotDeg(rot1, 'z', -40.0F);
            MTXTrans(trans, -950.0F, -400.0F, 0.0F);
            MTXConcat(trans, rot1, rot1);
            MTXConcat(rot, rot1, rot1);
            InitTracker(rot1, LotusControl, LotusLightControl, LotusDraw, 5);

            //make Ferarri matrix
            MTXRotDeg(rot1, 'z', -90.0F);
            MTXTrans(trans, 0.0F, 100.0F, 0.0F);
            MTXConcat(trans, rot1, rot1);
            MTXConcat(rot, rot1, rot1);
            InitTracker(rot1, FerarriControl, FerarriLightControl, FerarriDraw, 6);

            //make Ferarri matrix
            MTXRotDeg(rot1, 'z', -10.0F);
            MTXTrans(trans, -350.0F, -900.0F, 0.0F);
            MTXConcat(trans, rot1, rot1);
            MTXConcat(rot, rot1, rot1);
            InitTracker(rot1, FerarriControl, FerarriLightControl, FerarriDraw, 7);

            CTRLSetRotation(BigFloor8Control, 0.0F, 0.0F, (float)CarRotation);
            ACTBuildMatrices(BigFloor8);
            ACTRender(BigFloor8, cameraMatrix, 0); 

            DrawModelList(8);
            break;

        //12 car case
        case 6:
            MTXRotDeg(rot, 'z', (float)CarRotation);

            //make viper matrix
            MTXRotDeg(rot1, 'z', 0.0F);
            MTXTrans(trans, 0.0F, 400.0F, 0.0F);
            MTXConcat(trans, rot1, rot1);
            MTXConcat(rot, rot1, rot1);
            InitTracker(rot1, ViperControl, ViperLightControl, ViperDraw, 0);

            //make viper matrix
            MTXRotDeg(rot1, 'z', -20.0F);
            MTXTrans(trans, -750.0F, -1100.0F, 0.0F);
            MTXConcat(trans, rot1, rot1);
            MTXConcat(rot, rot1, rot1);
            InitTracker(rot1, ViperControl, ViperLightControl, ViperDraw, 1);

            //make viper matrix
            MTXRotDeg(rot1, 'z', 50.0F);
            MTXTrans(trans, -1200.0F, 1100.0F, 0.0F);
            MTXConcat(trans, rot1, rot1);
            MTXConcat(rot, rot1, rot1);
            InitTracker(rot1, ViperControl, ViperLightControl, ViperDraw, 2);

            //make viper matrix
            MTXRotDeg(rot1, 'z', -50.0F);
            MTXTrans(trans, 1200.0F, 1100.0F, 0.0F);
            MTXConcat(trans, rot1, rot1);
            MTXConcat(rot, rot1, rot1);
            InitTracker(rot1, ViperControl, ViperLightControl, ViperDraw, 3);

            //make lotus matrix
            MTXRotDeg(rot1, 'z', 0.0F);
            MTXTrans(trans, 0.0F, -900.0F, 0.0F);
            MTXConcat(trans, rot1, rot1);
            MTXConcat(rot, rot1, rot1);
            InitTracker(rot1, LotusControl, LotusLightControl, LotusDraw, 4);

            //make lotus matrix
            MTXRotDeg(rot1, 'z', -35.0F);
            MTXTrans(trans, 1300.0F, -300.0F, 0.0F);
            MTXConcat(trans, rot1, rot1);
            MTXConcat(rot, rot1, rot1);
            InitTracker(rot1, LotusControl, LotusLightControl, LotusDraw, 5);

            //make lotus matrix
            MTXRotDeg(rot1, 'z', 30.0F);
            MTXTrans(trans, -750.0F, 100.0F, 0.0F);
            MTXConcat(trans, rot1, rot1);
            MTXConcat(rot, rot1, rot1);
            InitTracker(rot1, LotusControl, LotusLightControl, LotusDraw, 6);

            //make lotus matrix
            MTXRotDeg(rot1, 'z', -20.0F);
            MTXTrans(trans, 400.0F, 1500.0F, 0.0F);
            MTXConcat(trans, rot1, rot1);
            MTXConcat(rot, rot1, rot1);
            InitTracker(rot1, LotusControl, LotusLightControl, LotusDraw, 7);

            //make Ferarri matrix
            MTXRotDeg(rot1, 'z', 20.0F);
            MTXTrans(trans, 750.0F, -1100.0F, 0.0F);
            MTXConcat(trans, rot1, rot1);
            MTXConcat(rot, rot1, rot1);
            InitTracker(rot1, FerarriControl, FerarriLightControl, FerarriDraw, 8);

            //make Ferarri matrix
            MTXRotDeg(rot1, 'z', -30.0F);
            MTXTrans(trans, 700.0F, 100.0F, 0.0F);
            MTXConcat(trans, rot1, rot1);
            MTXConcat(rot, rot1, rot1);
            InitTracker(rot1, FerarriControl, FerarriLightControl, FerarriDraw, 9);

            //make Ferarri matrix
            MTXRotDeg(rot1, 'z', 20.0F);
            MTXTrans(trans, -400.0F, 1500.0F, 0.0F);
            MTXConcat(trans, rot1, rot1);
            MTXConcat(rot, rot1, rot1);
            InitTracker(rot1, FerarriControl, FerarriLightControl, FerarriDraw, 10);

            //make Ferarri matrix
            MTXRotDeg(rot1, 'z', 35.0F);
            MTXTrans(trans, -1300.0F, -300.0F, 0.0F);
            MTXConcat(trans, rot1, rot1);
            MTXConcat(rot, rot1, rot1);
            InitTracker(rot1, FerarriControl, FerarriLightControl, FerarriDraw, 11);

            CTRLSetRotation(BigFloor12Control, 0.0F, 0.0F, (float)CarRotation);
            ACTBuildMatrices(BigFloor12);
            ACTRender(BigFloor12, cameraMatrix, 0); 

            DrawModelList(12);
            break;
#else
        case 0:
            CTRLSetRotation(ViperControl[0], 0.0F, 0.0F, (float)CarRotation);
            CTRLSetRotation(ViperFloorControl, 0.0F, 0.0F, (float)CarRotation);    
            ViperDraw();
            break;
        case 1:
            CTRLSetRotation(FerarriFloorControl, 0.0F, 0.0F, (float)CarRotation);
            CTRLSetRotation(FerarriControl[0], 0.0F, 0.0F, (float)CarRotation);
            FerarriDraw();
            break;
        case 2:
            CTRLSetRotation(LotusFloorControl, 0.0F, 0.0F, (float)CarRotation);
            CTRLSetRotation(LotusControl[0], 0.0F, 0.0F, (float)CarRotation);
            LotusDraw();
            break;
#endif
    }
}

/*---------------------------------------------------------------------------*
    Name:			CarAnim
    
    Description:	Animates the car and object based on the joystick's 
					state.
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
void CarAnim ( void )
{
	u16 buttons = JoyReadButtons();
    s8 subStickX = JoyReadSubstickX();
    s8 subStickY = JoyReadSubstickY();
    u8 triggerL = JoyReadTriggerL();
    
#if defined(HW1) || defined(HW2)
    u8 camIndex;
    Mtx rot;

    if(buttons & PAD_BUTTON_Y)
    {
        CurrentCar ++;
        if(CurrentCar > 6)
            CurrentCar = 0;

        if(CurrentCar == 3)
        {
            ACTHide(ViperFloor);
            ACTHide(LotusFloor);
            ACTHide(FerarriFloor);
        }
        if(CurrentCar == 0)
        {
            MTXIdentity(rot);
            CTRLSetMatrix(ViperLightControl, rot);
            CTRLSetMatrix(LotusLightControl, rot);
            CTRLSetMatrix(FerarriLightControl, rot);

            ACTShow(ViperFloor);
            ACTShow(LotusFloor);
            ACTShow(FerarriFloor);
        }

        //reset the camera
        switch(CurrentCar)
        {
            case 0:
            case 1:
            case 2:
                camIndex = 0;                
                break;
            case 3:
                camIndex = 1;    
                break;
            case 4:
                camIndex = 2;    
                break;
            case 5:
                camIndex = 3;    
                break;
            case 6:
                camIndex = 4;    
                break;
        }

        CamX = CameraKeyArray[camIndex].x;	
        CamY = CameraKeyArray[camIndex].y;
        CamZ = CameraKeyArray[camIndex].z;
        CameraLocScale = CameraKeyArray[camIndex].scale;

        //reset car rotation
        CarRotation = 0;

        //reset video mode
        switch(CurrentCar)
        {
            case 0:
            case 1:
            case 2:
            case 3:            
                VideoModeAA = TRUE;
                SetUpAA();
                currentRenderMode = &GXNtsc480IntAa;
                break;
            case 4:
            case 5:
            case 6:
                VideoModeAA = FALSE;
                SetUpIntDF();
                currentRenderMode = &GXNtsc480IntDf;
                break;
        }

        //set LOD
        if(CurrentCar == 6)
            LOD = 1;
        else
            LOD = 0;
    }
#endif
    
    if(buttons & PAD_BUTTON_B)
    {
        CubeMapDisplay ++;
        if(CubeMapDisplay > 1)  CubeMapDisplay = 0;
    }
    
    if(buttons & PAD_BUTTON_X)
    {
        CarDisplayMode ++;
        if(CarDisplayMode > 4) CarDisplayMode = 0;

        switch(CarDisplayMode)
        {
            case 0:
                OSReport("     Normal Car Display\n");
                break;
            case 1:
                OSReport("     diffuse Only Car Display\n");
                break;
            case 2:
                OSReport("     Specular Only Car Display\n");
                break;
            case 3:
                OSReport("     Vertex Color Only Car Display\n");
                break;
            case 4:
                OSReport("     Gloss Map Car Display\n");
                break;
        }
    }

    if(subStickX > 75)
    {
        if(RotationSpeed < 2.0F)
            RotationSpeed += .01;        
    }
    else if(subStickX < -75)
    {
        if(RotationSpeed > -2.0F)
            RotationSpeed -= .01;
    }

    //don't rotate the car if left trigger is down
    if(triggerL < 170)
    {
        CarRotation += RotationSpeed;

        if(CarRotation > 359.0F)
            CarRotation -= 360.0F;

        if(CarRotation < -359.0F)
            CarRotation += 360.0F;
    }
}

/*---------------------------------------------------------------------------*
    Name:	LightInit		
    
    Description:    Initializes per-vertex lights in the scene
    				
    Arguments:		
    
    Returns:		none
 *---------------------------------------------------------------------------*/
void LightInit ( void )
{
	GXColor white = {255, 255, 255, 255};

    /////////////////////
	LITAlloc(&ViperLight);	

	LITInitAttn(ViperLight, 1.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F);

	LITInitPos(ViperLight, 0.0F, 0.0F, 500.0F);
	LITInitColor(ViperLight, white);  

    ViperLightControl = LITGetControl(ViperLight);

    CTRLInit(ViperLightControl);

    /////////////////////
	LITAlloc(&FerarriLight);	

	LITInitAttn(FerarriLight, 1.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F);

	LITInitPos(FerarriLight, 0.0F, 0.0F, 500.0F);
	LITInitColor(FerarriLight, white);  

    FerarriLightControl = LITGetControl(FerarriLight);

    CTRLInit(FerarriLightControl);

    /////////////////////
	LITAlloc(&LotusLight);	

	LITInitAttn(LotusLight, 1.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F);

	LITInitPos(LotusLight, 0.0F, 0.0F, 500.0F);
	LITInitColor(LotusLight, white);  

    LotusLightControl = LITGetControl(LotusLight);

    CTRLInit(LotusLightControl);
}

/*---------------------------------------------------------------------------*
    Name:	SetCarShader		
    
    Description:    Sets the car's shader
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static void SetCarShader ( SHDRCompiled *shader, DODisplayObjPtr dispObj, u32 combineSetting, GXTexObj *texture, BOOL colorChanUsed, Ptr data )
{
    #pragma unused (data)

    #pragma unused (combineSetting)
    #pragma unused (texture)
    #pragma unused (colorChanUsed)

    Mtx mv, t, s;

    // set up reflection map matrix
    MTXScale(s, 0.5F, -0.5F, 0.0F);
    MTXTrans(t, 0.5F, 0.5F, 1.0F);

    MTXConcat(cameraMatrix, dispObj->worldMatrix, mv);
    MTXInverse(mv, mv);
    MTXTranspose(mv, mv);
    MTXConcat(s, mv, mv);
    MTXConcat(t, mv, mv);

    switch(CarDisplayMode)
    {
    case 0:
        SHDRBindTexGenMtx(CarShader_REF_DIFF, SHADER_MTX0, mv);
        SHDRBindTexture(CarShader_REF_DIFF, SHADER_TEX0, texture);
        SHDRBindTexture(CarShader_REF_DIFF, SHADER_TEX1, &SphereMap);
        //SHDRBindComplexInput(CarShader_REF_DIFF, SHADER_COMPLEX0, shader);
        SHDRBindRasterized(CarShader_REF_DIFF, SHADER_RAS0, GX_COLOR0A0);
        SHDRExecute(CarShader_REF_DIFF);
        break;
    case 2:
        SHDRBindTexGenMtx(CarShader_REF, SHADER_MTX0, mv);
        SHDRBindTexture(CarShader_REF, SHADER_TEX0, &SphereMap);
        SHDRBindComplexInput(CarShader_REF, SHADER_COMPLEX0, shader);
        SHDRBindRasterized(CarShader_REF, SHADER_RAS0, GX_COLOR0A0);
        SHDRExecute(CarShader_REF);
        break;
    case 1:
        SHDRBindComplexInput(CarShader_DIFF, SHADER_COMPLEX0, shader);
        SHDRBindRasterized(CarShader_DIFF, SHADER_RAS0, GX_COLOR0A0);
        SHDRExecute(CarShader_DIFF);
        break;
    case 3:
        SHDRBindRasterized(CarShader, SHADER_RAS0, GX_COLOR0A0);
        SHDRExecute(CarShader);
        break;
    case 4:
        SHDRBindComplexInput(CarShader_GLOSS, SHADER_COMPLEX0, shader);
        SHDRExecute(CarShader_GLOSS);
        break;
    }
}

/****************************************************************************/
static void InitTracker ( Mtx transform, CTRLControl **actorControl, 
                          CTRLControl *lightControl, void *drawFunction, u8 index )
{
    DistanceTracker *current = &(TrackerArray[index]);
    Vec location = {0.0F, 0.0F, 0.0F};

    MTXCopy(transform, current->transform);
    current->actorControl = actorControl;
    current->lightControl = lightControl;
    current->drawFunction = drawFunction;

    MTXMultVec(transform, &location, &location);
    MTXMultVec(cameraMatrix, &location, &location);
    current->distance = VECMag(&location);

    //add it to the list
    TrackerSort[index] = current;

    //sort the list with the car of the greatest distance being first
    while(index)
    {
        //move this node up in the list if its distance is greater
        if(TrackerSort[index]->distance > TrackerSort[index - 1]->distance)
        {
            current = TrackerSort[index - 1];
            TrackerSort[index - 1] = TrackerSort[index];
            TrackerSort[index] = current;
        }
        else
            break;
    }
}

/****************************************************************************/
static void DrawModelList ( u8 numModels )
{
    u8 i;

    for ( i = 0; i < numModels; i ++ )
    {
        CTRLSetMatrix(TrackerSort[i]->actorControl[0], TrackerSort[i]->transform);
        CTRLSetMatrix(TrackerSort[i]->actorControl[1], TrackerSort[i]->transform);
        CTRLSetMatrix(TrackerSort[i]->lightControl, TrackerSort[i]->transform);

        (TrackerSort[i]->drawFunction)();

    }
}