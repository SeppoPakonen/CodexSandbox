/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     sketchdemo.c

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/charPipeline/src/sketchdemo.c $
    
    5     11/03/00 11:28p Hirose
    added codes for GX revision 2 (HW2)
    
    4     9/25/00 5:54p Hashida
    Changed API name from DVDInitStream to DVDPrepareStream (synchronous
    function).
    
    3     8/21/00 7:44p Hirose
    fixed a VAT initialization bug
    
    2     8/21/00 12:03p Hirose
    fixed compile error on MAC build
    
    1     8/18/00 11:48a Hirose
    initial check in
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <demo.h>
#include <charPipeline.h>

#include <string.h>
#include <math.h>

/*---------------------------------------------------------------------------*
   Forward references
 *---------------------------------------------------------------------------*/
void        main                ( void ); 

static void AudioInit           ( void );

static void CameraInit          ( f32 scale );
static void DrawInit            ( void );
static void LightInit           ( void );

static void DrawTick            ( void );
static void MakeModelMtx        ( Vec xAxis, Vec yAxis, Vec zAxis, Mtx m );
static void AnimSetSpeed        ( OSStopwatch *swAnim, f32 animHz );
static void AnimTick            ( void );
static void DrawModel           ( void );
static void DrawCartoonOutline  ( void );
static void DrawBackGround      ( void );

static void LoadFiles           ( void );

static void SetShader0  ( SHDRCompiled *shader, DODisplayObjPtr dispObj, Ptr data );

/*---------------------------------------------------------------------------*
   Macros
 *---------------------------------------------------------------------------*/
#define PI    3.14159265F

#define Clamp(val,min,max) \
    ((val) = (((val) < (min)) ? (min) : ((val) > (max)) ? (max) : (val)))

#define Modulo(val,min,max) \
    ((val) = (((val) < (min)) ? ((val)+(max)-(min)) : ((val) > (max)) \
    ? ((val)-(max)+(min)) : (val)))


/*---------------------------------------------------------------------------*
   Local structures
 *---------------------------------------------------------------------------*/
// for outlining
typedef struct
{
    GXTexObj    tex;
    void*       texData;
    u16         areaWd;
    u16         areaHt;
} MyOutlineObj;

/*---------------------------------------------------------------------------*
   Data
 *---------------------------------------------------------------------------*/
static f32 TransTbl[8] =
{ 0.0F, 0.10F, -0.30F, 0.30F, 0.20F, -0.30F, 0.15F, -0.45F };

static f32 OTransTbl[8] =
{ 1.0F, 3.0F, -2.0F, 1.0F, 0.0F, -1.0F, 3.0F, -2.0F };

/*---------------------------------------------------------------------------*
   Global variables
 *---------------------------------------------------------------------------*/
static Vec CamXDef = { -1.0F, 0.0F, 0.0F };  
static Vec CamYDef = {  0.0F, 0.0F, 1.0F };
static Vec CamZDef = {  0.0F, 1.0F, 0.0F };
#define CAMLOC_SCALE_DEF  12.5F

static Vec CamX, CamY, CamZ;
static f32 CameraLocScale;
static Mtx ViewMtx;

DODisplayObjPtr dispObj;
GEOPalettePtr pal = NULL;

u32 CurrentDispObj;

ACTActorPtr actor0 = NULL;
CTRLControlPtr actorControl0;

ACTActorPtr actor1 = NULL;
CTRLControlPtr actorControl1;
ANIMBankPtr animBank = NULL;

LITLightPtr light1;

Vec LightPoint = {0.0F, 0.0F, 128.0F};
float LightDistance = 1.0F;

Mtx CubeWorldMatrix;

s32 LightTheta, LightPhi;

static GXTexObj         MyMonotoneTex;
static GXTexObj         MySketchTex;
static GXTexObj         MyBGTex;

static u32              FrameCount;
static u32              FlickerMode = 1;
static u32              AutoCamera = 1;
static u32              Quit = 0;

static u16              ScrWidth;
static u16              ScrHeight;

static MyOutlineObj     Outline;
static TEXPalettePtr    MyTplObj = 0;    // texture palette file

static DVDFileInfo      AudioInfo;       // for audio streaming


/*---------------------------------------------------------------------------*
   Application main loop
 *---------------------------------------------------------------------------*/
void main ( void )
{
    //OSStopwatch swAnim;                      // stopwatch for single frame animation time

    DEMOInit(NULL);

    LightInit();
    DrawInit();
    LoadFiles();
    AudioInit();
    
    OSReport("************************\n");
    OSReport("    Sketch demo\n");
    OSReport("************************\n");
    
    //OSInitStopwatch(&swAnim, "anim");       // time between frames

    // While the quit button is not pressed
    while( !Quit )    
    {   
        //OSStartStopwatch(&swAnim); // swAnim stopwatch runs once per frame;
                                   // includes time for VIWaitForRetrace().

        AnimTick();                // Do animation based on input
        
        DEMOBeforeRender();

        DrawTick();                // Draw the model.

        DEMODoneRender();

        //AnimSetSpeed(&swAnim, (f32)60.0f); // 60Hz
        //OSStopStopwatch(&swAnim);  // reset swAnim for next frame
        //OSResetStopwatch(&swAnim);
    }

    OSReport( "End of the demo.\n" );
}

/*---------------------------------------------------------------------------*
   Functions
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
    Name:           AudioInit
    
    Description:    Initialize audio streaming
                    
    Arguments:      
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void AudioInit ( void )
{
#if ( defined(HW1) || defined(HW2) )
    AIInit(NULL);

    // set volume
    AISetStreamVolLeft(255);
    AISetStreamVolRight(255);

    // set sample rate
    AISetStreamSampleRate(AI_SAMPLERATE_48KHZ);

    DVDChangeDir("gxDemos/sketch");
    if (FALSE == DVDOpen("sketch.adp", &AudioInfo))
    {
        OSReport("Unable to open sketch.adp\n");
    }

    // start streaming!
    DVDPrepareStream(&AudioInfo, 0, 0);
    AISetStreamPlayState(AI_STREAM_START);
    
    DVDChangeDir("../..");
#endif
}

/*---------------------------------------------------------------------------*
    Name:           CameraInit
    
    Description:    Initialize the projection matrix and load into hardware.
                    
    Arguments:      ViewMtx   view matrix to be passed to ViewInit
                    cameraLocScale  scale for the camera's distance from the 
                                    object - to be passed to ViewInit
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void CameraInit ( f32 scale )
{
    Mtx44 p;

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
static void DrawInit ( void )
{
    GXRenderModeObj* rmp;

    // get screen size
    rmp = DEMOGetRenderModeObj();
    ScrWidth = rmp->fbWidth;
    ScrHeight = rmp->efbHeight;

    // BG format / background color
    GXSetPixelFmt(GX_PF_RGBA6_Z24, GX_ZC_LINEAR);
    GXSetCopyClear((GXColor){ 0xFF, 0xFF, 0xFF, 0x00 } , 0x00FFFFFF);
    GXCopyDisp(DEMOGetCurrentBuffer(), GX_TRUE);
    GXSetDispCopyGamma(GX_GM_1_0);

    GXSetZCompLoc(GX_FALSE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_SET);
    
    // outline setting
    Outline.texData = OSAlloc(GXGetTexBufferSize(ScrWidth, ScrHeight, GX_TF_I8, GX_FALSE, 0));
    ASSERTMSG( Outline.texData != NULL, "Not enough memory\n" );
    GXInitTexObj(
        &Outline.tex,
        Outline.texData,
        ScrWidth,
        ScrHeight,
        GX_TF_I8,
        GX_CLAMP,
        GX_CLAMP,
        GX_FALSE );
    GXInitTexObjLOD(&Outline.tex, GX_NEAR, GX_NEAR, 0, 0, 0, 0, 0, GX_ANISO_1);

    // camera
    CamX = CamXDef;
    CamY = CamYDef;
    CamZ = CamZDef;
    CameraLocScale = CAMLOC_SCALE_DEF;

    // light
    LightTheta = 45;
    LightPhi   = 15;
}

/*---------------------------------------------------------------------------*
    Name:           DrawTick
    
    Description:    Draw the current model once.  
                    
    Arguments:      ViewMtx       view matrix
                    m       model matrix
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void DrawTick ( void )
{
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_GREATER, 0);

    // Background
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
    DrawBackGround();

    // Main object
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    DrawModel();

    // Outlines
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
    DrawCartoonOutline();
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

    if(actor1 && animBank)
        ACTSetSpeed(actor1, animStep);
}

/*---------------------------------------------------------------------------*
    Name:           AnimTick
    
    Description:    Animates the camera and object based on the joystick's 
                    state.
                    
    Arguments:      m   model matrix
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

    // Move camera
    if ( buttons & PAD_TRIGGER_L )
    {
        CameraLocScale *= .95F;
    }
    if ( buttons & PAD_TRIGGER_R )
    {
        CameraLocScale *= 1.05F;        
    }
    Clamp(CameraLocScale, 0.01F, 1024.0F);


    // Rotate camera
    if ( AutoCamera && stickX == 0 && stickY == 0 )
    {
        MTXRotAxis(rot, &CamY, -0.5F);
        MTXMultVec(rot, &CamX, &CamX);
        MTXMultVec(rot, &CamZ, &CamZ);
    }

    if ( stickX > 16 || stickX < -16 )
    {
        MTXRotAxis(rot, &CamY, -(f32)stickX/24);
        MTXMultVec(rot, &CamX, &CamX);
        MTXMultVec(rot, &CamZ, &CamZ);
    }

    if ( stickY > 16 || stickY < -16 )
    {
        MTXRotAxis(rot, &CamX, -(f32)stickY/24);
        MTXMultVec(rot, &CamY, &CamY);
        MTXMultVec(rot, &CamZ, &CamZ);          
    }
            
    MakeModelMtx(CamX, CamY, CamZ, ViewMtx);  // Make a new model matrix
    MTXTranspose(ViewMtx, ViewMtx);
    MTXTrans(rot, 0.0F, 0.0F, -8.0F * CameraLocScale);
    MTXConcat(rot, ViewMtx, ViewMtx);


    // Move light
    LightTheta += subStickX / 24;
    LightPhi   += subStickY / 24;
    Modulo(LightTheta, 0, 360);
    Clamp(LightPhi, -90, 90);


    // Flickering animation switch
    if ( buttonsDown & PAD_BUTTON_A )
    {
        FlickerMode = FlickerMode ? 0U : 1U;
    }

    // Auto camera pilot
    if ( buttonsDown & PAD_BUTTON_B )
    {
        AutoCamera = AutoCamera ? 0U : 1U;
        if ( AutoCamera )
        {
            CamX = CamXDef;
            CamY = CamYDef;
            CamZ = CamZDef;
            CameraLocScale = CAMLOC_SCALE_DEF;
        }
    }


    if ( buttonsDown & PAD_BUTTON_X )
    {
        OSReport("CamX : %f, %f, %f\n", CamX.x, CamX.y, CamX.z);
        OSReport("CamY : %f, %f, %f\n", CamY.x, CamY.y, CamY.z);
        OSReport("CamZ : %f, %f, %f\n", CamZ.x, CamZ.y, CamZ.z);
        OSReport("Scale : %f\n", CameraLocScale);
        OSReport("Light : %d, %d\n", LightTheta, LightPhi);
    }

/*
    if(animBank)
        ACTTick(actor1);
*/
        
    ++FrameCount;
}

/*---------------------------------------------------------------------------*
    Name:           
    
    Description:    
                    
    Arguments:      
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void LightInit ( void )
{
    LITAlloc(&light1);  

    LITInitAttn(light1, 1.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F);
    LITInitPos(light1, 0.0F, 0.0F, 0.0F);
    LITInitColor(light1, (GXColor){ 0, 0, 0, 0 });       

    LITAttach(light1, (Ptr)CubeWorldMatrix, PARENT_MTX);
}

/*---------------------------------------------------------------------------*
    Name:           
    
    Description:    
                    
    Arguments:      
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void DrawModel( void )
{
    CameraInit(CameraLocScale);

    LITXForm(light1, ViewMtx);

    // Viewport
    GXSetViewport(80.0F, 64.0F, (f32)(ScrWidth-160), (f32)(ScrHeight-128), 0.0F, 1.0F);
    GXSetScissor(80, 64, (u32)(ScrWidth-160), (u32)(ScrHeight-128));
    
    ACTBuildMatrices(actor0);
    ACTRender(actor0, ViewMtx, 1, light1);  
}

/*---------------------------------------------------------------------------*
    Name:           DrawCartoonOutline
    
    Description:    Draws outline
                    
    Arguments:      
    
    Returns:        none
 *---------------------------------------------------------------------------*/
void  DrawCartoonOutline( void )
{
#ifndef flagEMU
    Mtx     texMtx1, texMtx2;
    Mtx     mtxUnit;
    Mtx     mtxProj;
    f32     xt0, yt0, xt1, yt1;
    u32     idx;
    
    // Copy out eFB alpha plane as I8 texture format
    GXSetTexCopySrc(0, 0, ScrWidth, ScrHeight);
    GXSetTexCopyDst(ScrWidth, ScrHeight, GX_TF_A8, GX_DISABLE);
    GXCopyTex(Outline.texData, GX_DISABLE);

    // Be sure texture is completely copied out
    GXPixModeSync();

    // texture setting
    GXLoadTexObj(&Outline.tex, GX_TEXMAP0);

    // Setup for outline drawing
    GXSetNumTexGens(2);
    GXSetNumChans(0);
    GXSetNumTevStages(2);
    
    // TEV settings
    GXSetTevOrder( GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL );
    GXSetTevOp( GX_TEVSTAGE0, GX_REPLACE );
    GXSetTevOrder( GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP0, GX_COLOR_NULL );
    GXSetTevColorIn( GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO,
                     GX_CC_ZERO, GX_CC_ZERO );
    GXSetTevColorOp( GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
                     GX_ENABLE, GX_TEVPREV );
    GXSetTevAlphaIn( GX_TEVSTAGE1, GX_CA_TEXA, GX_CA_ZERO,
                     GX_CA_ZERO, GX_CA_APREV );
    GXSetTevAlphaOp( GX_TEVSTAGE1, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1,
                     GX_DISABLE, GX_TEVPREV );

#if ( GX_REV == 1 )
    GXSetTevClampMode( GX_TEVSTAGE0, GX_TC_LINEAR );
    GXSetTevClampMode( GX_TEVSTAGE1, GX_TC_LINEAR );
#endif

    // only write pixels that pass alpha test
    GXSetAlphaCompare( GX_GREATER, 4, GX_AOP_AND, GX_LESS, 256-4 );

    // Z mode
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);


    MTXIdentity(texMtx1);
    MTXRowCol(texMtx1, 0, 3) = 1.0f / (f32)ScrWidth;
    MTXIdentity(texMtx2);
    MTXRowCol(texMtx2, 1, 3) = 1.0f / (f32)ScrHeight;
    GXLoadTexMtxImm(texMtx1, GX_TEXMTX1, GX_MTX2x4);    
    GXLoadTexMtxImm(texMtx2, GX_TEXMTX2, GX_MTX2x4);

    // vertex attribute
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    
    // vertex descriptor settings
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1);
    
    // Viewport
    GXSetViewport(0.0F, 0.0F, (f32)ScrWidth, (f32)ScrHeight, 0.0F, 1.0F);
    GXSetScissor(0, 0, ScrWidth, ScrHeight);

    // load matrices
    MTXOrtho(mtxProj, 0.0F, (f32)ScrHeight, 0.0F, (f32)ScrWidth, 0.0F, -1.0F);
    GXSetProjection(mtxProj, GX_ORTHOGRAPHIC);
    MTXIdentity(mtxUnit);
    GXLoadPosMtxImm(mtxUnit, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    
    idx = FrameCount / 10;
    xt0 = FlickerMode ? OTransTbl[idx%8]     : 0;
    xt1 = FlickerMode ? OTransTbl[(idx+3)%8] : 0;
    yt0 = FlickerMode ? OTransTbl[(idx+4)%8] : 0;
    yt1 = FlickerMode ? OTransTbl[(idx+7)%8] : 0;

    // draw with texture offset in S
    GXBegin(GX_QUADS, GX_VTXFMT1, 4);
        GXPosition3f32(xt0, yt0, 0.5f);
        GXTexCoord2f32(0.0F, 0.0F);
        GXPosition3f32((f32)ScrWidth + xt1, yt1, 0.5f);
        GXTexCoord2f32(1.0F, 0.0F);
        GXPosition3f32((f32)ScrWidth + xt0, (f32)ScrHeight + yt1, 0.5f);
        GXTexCoord2f32(1.0F, 1.0F);
        GXPosition3f32(xt1, (f32)ScrHeight + yt0, 0.5f);
        GXTexCoord2f32(0.0F, 1.0F);
    GXEnd();

    GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX2);

    // draw with texture offset in T
    GXBegin(GX_QUADS, GX_VTXFMT1, 4);
        GXPosition3f32(xt0, yt0, 0.5f);
        GXTexCoord2f32(0.0F, 0.0F);
        GXPosition3f32((f32)ScrWidth + xt1, yt1, 0.5f);
        GXTexCoord2f32(1.0F, 0.0F);
        GXPosition3f32((f32)ScrWidth + xt0, (f32)ScrHeight + yt1, 0.5f);
        GXTexCoord2f32(1.0F, 1.0F);
        GXPosition3f32(xt1, (f32)ScrHeight + yt0, 0.5f);
        GXTexCoord2f32(0.0F, 1.0F);
    GXEnd();

#endif // EMU

    return;
}

/*---------------------------------------------------------------------------*
    Name:           DrawBackGround
    
    Description:    Draws background
                    
    Arguments:      
    
    Returns:        none
 *---------------------------------------------------------------------------*/
void  DrawBackGround( void )
{
    Mtx     mtxUnit;
    Mtx     mtxProj;
    
    // texture setting
    GXLoadTexObj(&MyBGTex, GX_TEXMAP0);

    // Setup for outline drawing
    GXSetNumTexGens(1);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    
    // TEV settings
    GXSetTevOrder( GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL );
    GXSetTevOp( GX_TEVSTAGE0, GX_REPLACE );
    GXSetDstAlpha( GX_TRUE, 0 );

    // vertex attribute
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    
    // vertex descriptor settings
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    // Viewport
    GXSetViewport(0.0F, 0.0F, (f32)ScrWidth, (f32)ScrHeight, 0.0F, 1.0F);
    GXSetScissor(0, 0, ScrWidth, ScrHeight);

    // Texcoords
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    
    // load matrices
    MTXOrtho(mtxProj, 0.0F, (f32)ScrHeight, 0.0F, (f32)ScrWidth, 0.0F, -1.0F);
    GXSetProjection(mtxProj, GX_ORTHOGRAPHIC);
    MTXIdentity(mtxUnit);
    GXLoadPosMtxImm(mtxUnit, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    // draw with texture offset in S
    GXBegin(GX_QUADS, GX_VTXFMT1, 4);
        GXPosition3f32(0, 0, 0.5f);
        GXTexCoord2f32(0.0F, 0.0F);
        GXPosition3f32(ScrWidth, 0, 0.5f);
        GXTexCoord2f32(1.0F, 0.0F);
        GXPosition3f32(ScrWidth, ScrHeight, 0.5f);
        GXTexCoord2f32(1.0F, 1.0F);
        GXPosition3f32(0, ScrHeight, 0.5f);
        GXTexCoord2f32(0.0F, 1.0F);
    GXEnd();

    // Resume destination alpha lock
    GXSetDstAlpha( GX_FALSE, 0 );

    return;
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
    TEXDescriptorPtr tdp;
    Mtx              mv; //, mt;


    CSHInitDisplayCache();

    DVDChangeDir("gxDemos/sketch");

    // textures
    TEXGetPalette(&MyTplObj, "sk-01.tpl");
    tdp = TEXGet(MyTplObj, 0);
    GXInitTexObj(&MySketchTex, tdp->textureHeader->data,
                 tdp->textureHeader->width,
                 tdp->textureHeader->height,
                 (GXTexFmt)tdp->textureHeader->format,
                 GX_REPEAT, GX_REPEAT, GX_FALSE );
    GXInitTexObjLOD(&MySketchTex, GX_NEAR, GX_NEAR, 0, 0, 0, 0, 0, GX_ANISO_1);

    tdp = TEXGet(MyTplObj, 1);
    GXInitTexObj(&MyMonotoneTex, tdp->textureHeader->data,
                 tdp->textureHeader->width,
                 tdp->textureHeader->height,
                 (GXTexFmt)tdp->textureHeader->format,
                 GX_CLAMP, GX_CLAMP, GX_FALSE );
    GXInitTexObjLOD(&MyMonotoneTex, GX_NEAR, GX_NEAR, 0, 0, 0, 0, 0, GX_ANISO_1);

    tdp = TEXGet(MyTplObj, 2);
    GXInitTexObj(&MyBGTex, tdp->textureHeader->data,
                 tdp->textureHeader->width,
                 tdp->textureHeader->height,
                 (GXTexFmt)tdp->textureHeader->format,
                 GX_CLAMP, GX_CLAMP, GX_FALSE );
    GXInitTexObjLOD(&MyBGTex, GX_LINEAR, GX_LINEAR, 0, 0, 0, 0, 0, GX_ANISO_1);

    // geometry
    ACTGet(&actor0, "sadgirl.act");
    ACTShow(actor0);
    actorControl0 = ACTGetControl(actor0);
    
    MTXTrans(mv, 0.0F, 0.0F, 0.0F);
    CTRLSetMatrix(actorControl0, mv);
    ACTSetEffectsShader(actor0, (Ptr)SetShader0, 0);


    DVDChangeDir("../..");
}

/*---------------------------------------------------------------------------*
    Name:	        SetShader		
    
    Description:    Sets the shader
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static void SetShader0( SHDRCompiled* shader, DODisplayObjPtr dispObj, Ptr data )
{
    #pragma unused (data)
    #pragma unused (shader)
    u32  indx;
    f32  scale, tx, ty, theta, phi;
    Mtx  mp, mv, mt;
    Vec  lpos;
    GXLightObj  lobj;

    theta = (f32)LightTheta * PI / 180.0F;
    phi   = (f32)LightPhi   * PI / 180.0F;
    lpos.x = 50000.0F * cosf(phi) * sinf(theta);
    lpos.y = 50000.0F * sinf(phi);
    lpos.z = 50000.0F * cosf(phi) * cosf(theta);

    //MTXMultVec(ViewMtx, &lpos, &lpos);
    GXInitLightPos(&lobj, lpos.x, lpos.y, lpos.z);
    GXInitLightColor(&lobj, (GXColor){ 0xB0, 0xB0, 0xB0, 0xFF });
    GXLoadLightObjImm(&lobj, GX_LIGHT0);

    // Lighting channel
    GXSetNumChans(1); // number of active color channels
    GXSetChanCtrl(
        GX_COLOR0A0,
        GX_ENABLE,   // enable channel
        GX_SRC_REG,  // amb source
        GX_SRC_REG,  // mat source
        GX_LIGHT0,   // light mask
        GX_DF_SIGN,  // diffuse function
        GX_AF_NONE);
    // set up ambient color
    GXSetChanAmbColor(GX_COLOR0A0, (GXColor){ 0x20, 0x20, 0x20, 0x20 });
    // set up material color
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });


    // set texture
    GXLoadTexObj(&MySketchTex, GX_TEXMAP0);
#ifndef flagEMU
    GXLoadTexObj(&MyMonotoneTex, GX_TEXMAP1);
#endif
    
    // set Tev mode
#ifndef flagEMU
#if ( GX_REV == 1 )
    //----------------------------------------------------
    //  TEV codes for GX revision 1 (HW1, etc.)
    //----------------------------------------------------
    GXSetNumTevStages(2);
    
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO,
                    GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO,
                    GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO,
                    GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO,
                    GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevClampMode(GX_TEVSTAGE0, GX_TC_LINEAR);
    
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC,
                    GX_CC_ONE, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_SUB, GX_TB_ZERO,
                    GX_CS_DIVIDE_2, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO,
                    GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO,
                    GX_CS_DIVIDE_2, GX_TRUE, GX_TEVREG2);
    GXSetTevClampMode(GX_TEVSTAGE1, GX_TC_LE);

#else // ( GX_REV >= 2 )
    //----------------------------------------------------
    //  TEV codes for GX revision 2 (HW2)
    //----------------------------------------------------
    GXSetNumTevStages(2);
    
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO,
                    GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO,
                    GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC,
                    GX_CC_ONE, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_COMP_R8_GT, GX_TB_ZERO,
                    GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO,
                    GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO,
                    GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

#endif // GX_REV

#else  // EMU
    //----------------------------------------------------
    //  TEV codes for the emulator
    //----------------------------------------------------
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
#endif

    // Object ID
    GXSetTevColor(GX_TEVREG0, (GXColor){ 0x00, 0x00, 0x00, 0x80 });

    // texture coord generation mode for cartoon lighting
    GXSetNumTexGens(2);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX0);
    GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_SRTG, GX_TG_COLOR0, GX_IDENTITY);

    // set up projection
    indx = ( FrameCount / 8 ) % 8;
    scale = CameraLocScale;
    tx = FlickerMode ? TransTbl[indx]       : 0;
    ty = FlickerMode ? TransTbl[(indx+3)%8] : 0;
    
    MTXLightFrustum(mp, -.24F * scale, .24F * scale,
                    -.32F * scale, .32F * scale,
                    .5F * scale, 5.0F, 3.5F, tx, ty);

    // set up texmtx
    MTXConcat(ViewMtx, dispObj->worldMatrix, mv);
    MTXConcat(mp, mv, mt);
    GXLoadTexMtxImm(mt, GX_TEXMTX0, GX_MTX3x4);

}

/*============================================================================*/
       
