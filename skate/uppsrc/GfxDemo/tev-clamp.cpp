/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     tev-clamp.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/gxdemo/src/Tev/tev-clamp.c $
    
    10    10/30/00 4:49p Hirose
    added warinig message for execution on GX revision >= 2 platforms
    
    9     7/07/00 5:57p Dante
    PC Compatibility
    
    8     5/17/00 8:46p Hirose
    changed parameter for Z mode
    
    7     3/26/00 7:32p Hirose
    centered display object more considering H/V overscan again
    
    6     3/24/00 6:00p Carl
    Adjusted screen height for overscan.
    
    5     3/24/00 3:45p Hirose
    changed to use DEMOPad library
    
    4     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
    3     3/22/00 3:13p Carl
    Adjusted for overscan.
    
    2     3/06/00 4:33p Hirose
    fixed demo names referenced in the source code
    
    1     3/06/00 12:12p Alligator
    move from tests/gx and rename
    
    6     3/01/00 12:47p Hirose
    fixed caption bug on EPPC mode
    
    5     2/24/00 8:12p Hirose
    updated pad control functions to match actual gamepad
    
    4     2/24/00 7:05p Yasu
    Rename gamepad key to match HW1
    
    3     2/15/00 6:17p Hirose
    added single frame test
    
    2     2/14/00 8:16p Hirose
    
    1     2/11/00 7:51p Hirose
    initial prototype
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
    tev-clamp
        TEV clamp mode test
 *---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
   Header files
 *---------------------------------------------------------------------------*/
#include <demo.h>

#include "tev-emu.h"

/*---------------------------------------------------------------------------*
   Macro definitions
 *---------------------------------------------------------------------------*/
// This test uses 640x480 screen space configuration
// even if the video mode is set to 640x240.
// (From the height, subtract 32 lines for overscan.
//  This is done in DEMOInit.)
#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   448

#define TEX_WIDTH       64
#define TEX_HEIGHT      64

#define NUM_TEVCLAMPS   4
#define NUM_TEVSCALES   4
#define NUM_TEVBIASES   17

#define NUM_TEXTURES    3

// for display location
#define ALI_COL(x)      (x * 104 + 192) 

/*---------------------------------------------------------------------------*
   Structure definitions
 *---------------------------------------------------------------------------*/
// for input arg display mode
typedef struct
{
    GXTevColorArg  arg;
    GXTevMode      mode;
    u8             ciOffset;
    u8             ciStep;
} MyColorArgDispObj;

// for tev state storage
typedef struct
{
    u8             biasMode;
    u8             scaleMode;
    u8             clampMode;
    GXBool         clampSw;
} MyTevStateObj;

// for entire scene control
typedef struct
{
    u32            cur;
    GXTexObj       texture[NUM_TEXTURES];
    u32            currentTex;
    MyTevStateObj  tevState;
} MySceneCtrlObj;

/*---------------------------------------------------------------------------*
   Forward references
 *---------------------------------------------------------------------------*/
void        main               ( void );
static void DrawInit           ( MySceneCtrlObj* sc );
static void DrawTick           ( MySceneCtrlObj* sc );
static void AnimTick           ( MySceneCtrlObj* sc );
static void MyDrawCaption        ( MySceneCtrlObj* sc );
static void DrawQuad           ( void );
static void DrawResult         ( GXTexObj* tx );
static void SetTev             ( MyTevStateObj* ts );
static void PrintIntro         ( void );


#ifdef __SINGLEFRAME  // single frame tests for checking hardware
static void SingleFrameSetUp( MySceneCtrlObj* sc );
#endif

/*---------------------------------------------------------------------------*
   Model Data
 *---------------------------------------------------------------------------*/
static s16 ModelVertices[8] ATTRIBUTE_ALIGN(32) = 
{
    // Used for drawing quads
     0,  0, 64,  0,
    64, 64,  0, 64,
};

static s8 QuadTexCoords[2*4] ATTRIBUTE_ALIGN(32) =
{
      0,   0,
      1,   0,
      1,   1,
      0,   1
};

/*---------------------------------------------------------------------------*
   Other data tables
 *---------------------------------------------------------------------------*/
static GXTevClampMode ClampModeTbl[NUM_TEVCLAMPS] =
{
    GX_TC_LINEAR, GX_TC_GE, GX_TC_EQ, GX_TC_LE
};

static GXTevScale ScaleModeTbl[NUM_TEVSCALES] =
{
    GX_CS_SCALE_1, GX_CS_SCALE_2, GX_CS_SCALE_4, GX_CS_DIVIDE_2
};

/*---------------------------------------------------------------------------*
   Strings data for captions
 *---------------------------------------------------------------------------*/
static char* ClampModeStr[NUM_TEVCLAMPS] =
{
    "GX_TC_LINEAR", "GX_TC_GE", "GX_TC_EQ", "GX_TC_LE"
};

static char* GXBoolStr[2] =
{
    "GX_FALSE", "GX_TRUE"
};

static char* ScaleModeStr[NUM_TEVSCALES] =
{
    "x1", "x2", "x4", "/2"
};

/*---------------------------------------------------------------------------*
   Global variables
 *---------------------------------------------------------------------------*/
static MySceneCtrlObj   SceneCtrl;                 // scene control parameters
static TEXPalettePtr    MyTplObj = NULL;           // texture palette

/*---------------------------------------------------------------------------*
   Application main loop
 *---------------------------------------------------------------------------*/
void main ( void )
{
    DEMOInit(NULL);  // Init the OS, game pad, graphics and video.


#if ( GX_REV != 1 ) // GX revision 2 or later
    // This demo is only available on GX revision 1 platforms
    // (MAC, HW1, etc.)
    OSReport("Sorry. The feature shown in this demo is only\n");
    OSReport("available on platforms with GX revision 1.\n");
    OSReport("This feature won't be available on the final product\n");
    OSReport("If you want to execute, please try MAC/HW1/HW1_DRIP.\n");

#else // GX revision 1

    DrawInit(&SceneCtrl); // Initialize vertex formats, array pointers
                          // and default scene settings.

    PrintIntro();    // Print demo directions

#ifdef __SINGLEFRAME  // single frame tests for checking hardware
    SingleFrameSetUp(&SceneCtrl);
    DEMOBeforeRender();
    DrawTick(&SceneCtrl);
    DEMODoneRender();
#else
    while(!(DEMOPadGetButton(0) & PAD_BUTTON_MENU))
    {   
		DEMOBeforeRender();
        DrawTick(&SceneCtrl);    // Draw the model.
        DEMODoneRender();
        DEMOPadRead();           // Read controller
        AnimTick(&SceneCtrl);    // Do animation
    }
#endif

#endif

    OSHalt("End of test");
}

/*---------------------------------------------------------------------------*
   Functions
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
    Name:           DrawInit
    
    Description:    Initializes the vertex attribute format and sets up
                    the array pointer for the indexed data.
                    This function also initializes scene control parameters.
                    
    Arguments:      sc : pointer to the structure of scene control parameters
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void DrawInit( MySceneCtrlObj* sc )
{
    GXColor           bgColor = { 0x00, 0x00, 0x40, 0x00 };
    TEXDescriptorPtr  tdp;
    u32               i;
    
    // Vertex Attribute (VTXFMT0 is used by DEMOPuts library)
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_POS, GX_POS_XY, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_TEX0, GX_TEX_ST, GX_S8, 0);

    // Array pointers
    GXSetArray(GX_VA_POS,  ModelVertices, 2 * sizeof(s16));
    GXSetArray(GX_VA_TEX0, QuadTexCoords, 2 * sizeof(s8));

    // Load TPL file and initialize texture objects
    TEXGetPalette(&MyTplObj, "gxTests/tev-01.tpl");
    for ( i = 0 ; i < NUM_TEXTURES ; ++i )
    {
        tdp = TEXGet(MyTplObj, i);
        
        GXInitTexObj(
            &sc->texture[i],
            tdp->textureHeader->data,
            tdp->textureHeader->width,
            tdp->textureHeader->height,
            (GXTexFmt)tdp->textureHeader->format,
            tdp->textureHeader->wrapS, // s
            tdp->textureHeader->wrapT, // t
            GX_FALSE ); // Mipmap
        
        GXInitTexObjLOD(
            &sc->texture[i],
            GX_NEAR,
            GX_NEAR,
            tdp->textureHeader->minLOD,
            tdp->textureHeader->maxLOD,
            tdp->textureHeader->LODBias,
            GX_FALSE,
            tdp->textureHeader->edgeLODEnable,
            GX_ANISO_1 );
    }
    

    // Default scene control parameter settings

    // current texture number
    sc->currentTex = 0;
    
    // tev state
    sc->tevState.biasMode  = 0;
    sc->tevState.scaleMode = 0;
    
    // cursor
    sc->cur   = 0;
}

/*---------------------------------------------------------------------------*
    Name:           DrawTick
    
    Description:    Draw the model by using given scene parameters 
                    
    Arguments:      sc : pointer to the structure of scene control parameters
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void DrawTick( MySceneCtrlObj* sc )
{
    Mtx  mt;
    u32  x, y;

    // set projection to match screen space coordinate system
    DEMOSetupScrnSpc(SCREEN_WIDTH, SCREEN_HEIGHT, 100.0F);
    
    // set up Z mode (which actually doesn't matter)
    GXSetZMode(GX_ENABLE, GX_ALWAYS, GX_ENABLE);
    
    // set up TEV rendering mode (no color / one texture)
    GXSetNumTevStages(1);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

    // draw result quads
    for ( x = 0 ; x < NUM_TEVCLAMPS ; ++x )
    {
        for ( y = 0 ; y < 2 ; ++y )
        {
            // TEV detail
            sc->tevState.clampMode = (u8)x;
            sc->tevState.clampSw   = y ? GX_TRUE : GX_FALSE;
            SetTev(&sc->tevState);
            
            // Draw a quad for result color
            MTXTrans(mt, ALI_COL(x), (f32)(y*80+68), 0);
            GXLoadPosMtxImm(mt, GX_PNMTX0);
            GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_COPY);
            DrawResult(&sc->texture[sc->currentTex]);
            
            // Draw a quad for result alpha
            MTXTrans(mt, ALI_COL(x), (f32)(y*80+288), 0);
            GXLoadPosMtxImm(mt, GX_PNMTX0);
            GXSetBlendMode(GX_BM_BLEND, GX_BL_ZERO, GX_BL_INVSRCALPHA, GX_LO_COPY);
            DrawResult(&sc->texture[sc->currentTex]);
        }
    }
    
    // Reset TEV clamp mode
    GXSetTevClampMode(GX_TEVSTAGE0, GX_TC_LINEAR);

    // Captions
    MyDrawCaption(sc);
}

/*---------------------------------------------------------------------------*
    Name:           AnimTick
    
    Description:    Changes scene parameters according to the pad status.
                    
    Arguments:      sc  : pointer to the structure of scene control parameters
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void AnimTick( MySceneCtrlObj* sc )
{
    u16  down = DEMOPadGetButtonDown(0);

    // move cursor
    if ( down & PAD_BUTTON_X )
    {
        sc->cur = ( sc->cur + 1 ) % 3;
    }
    if ( down & PAD_BUTTON_Y )
    {
        sc->cur = ( sc->cur + 2 ) % 3;
    }

    // change parameter
    if ( down & PAD_TRIGGER_R )
    {
        switch(sc->cur)
        {
            case 0:
                sc->currentTex += 1;
                break;
            case 1:
                sc->tevState.biasMode += 1;
                break;
            case 2:
                sc->tevState.scaleMode += 1;
                break;
        }
    }
    if ( down & PAD_TRIGGER_L )
    {
        switch(sc->cur)
        {
            case 0:
                sc->currentTex += NUM_TEXTURES - 1;
                break;
            case 1:
                sc->tevState.biasMode += NUM_TEVBIASES - 1;
                break;
            case 2:
                sc->tevState.scaleMode += NUM_TEVSCALES - 1;
                break;
        }
    }
    sc->currentTex         %= NUM_TEXTURES;
    sc->tevState.biasMode  %= NUM_TEVBIASES;
    sc->tevState.scaleMode %= NUM_TEVSCALES;

}

/*---------------------------------------------------------------------------*
    Name:           MyDrawCaption
    
    Description:    Draw captions
    
    Arguments:      sc : pointer to the structure of scene control parameters

    Returns:        none
 *---------------------------------------------------------------------------*/
static void MyDrawCaption( MySceneCtrlObj* sc )
{
    u32  i, j;

    DEMOInitCaption(DM_FT_OPQ, SCREEN_WIDTH, SCREEN_HEIGHT);

    DEMOPuts(48, 132, 0, "COLOR");
    DEMOPuts(48, 352, 0, "ALPHA");

    for ( j = 0 ; j < 2 ; ++j )
    {
        for ( i = 0 ; i < NUM_TEVCLAMPS ; ++i )
        {
            DEMOPuts((s16)ALI_COL(i), (s16)(52+j*220), 0, ClampModeStr[i]);
        }

        for ( i = 0 ; i < 2 ; ++i )
        {
            DEMOPuts(96, (s16)(i*80+j*220+96), 0, GXBoolStr[i]);
        }
    }

    // Parameters
    DEMOPrintf(48, 32, 0, " TEXTURE: %d", sc->currentTex);
    DEMOPrintf(48, 40, 0, " BIAS   : -%d/%d",
               sc->tevState.biasMode, NUM_TEVBIASES-1);
    DEMOPrintf(48, 48, 0, " SCALE  : %s",
               ScaleModeStr[sc->tevState.scaleMode]);

    // Cursor
    DEMOPrintf(40, (s16)(sc->cur*8+32), 0, "%c", 0x7F);

}

/*---------------------------------------------------------------------------*
    Name:           DrawQuad
    
    Description:    Draw a textured quad.
    
    Arguments:      none

    Returns:        none
 *---------------------------------------------------------------------------*/
static void DrawQuad( void )
{
    u8   i;

    // set vertex descriptor
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);

    // draw the quad
    GXBegin(GX_QUADS, GX_VTXFMT1, 4);
        for ( i = 0 ; i < 4 ; ++i )
        {
            GXPosition1x8(i);
            GXTexCoord1x8(i);
        }
    GXEnd();
}

/*---------------------------------------------------------------------------*
    Name:           DrawResult
    
    Description:    Draw a quad which shows color combination result
    
    Arguments:      tx : pointer to a texture object to be used

    Returns:        none
 *---------------------------------------------------------------------------*/
static void DrawResult( GXTexObj* tx )
{
#ifdef EPPC // Real hardware only have to do like this

    // Draw a quad
    GXLoadTexObj(tx, GX_TEXMAP0);
    DrawQuad();

#else // EPPC

    // Psuedo Tev emulation by tev-emu
    {
        static u16  count = 0;
        void        *rasBuf, *texBuf, *mixBuf;
        u16         wd, ht;
        u32         size;
        GXTexObj    tx2;
        
        GXInvalidateTexAll();
        GXSetPixelFmt(GX_PF_RGBA6_Z24, GX_ZC_LINEAR);

        wd = GXGetTexObjWidth(tx);
        ht = GXGetTexObjHeight(tx);
        ASSERTMSG( wd == TEX_WIDTH,  "Invalid sample texture width");
        ASSERTMSG( ht == TEX_HEIGHT, "Invalid sample texture height");
        
        size   = GXGetTexBufferSize(wd, ht, GX_TF_RGBA8, GX_FALSE, 0);
        texBuf = GXGetTexObjData(tx);
        mixBuf = OSAlloc(size);
        // This test uses no rasterized color
        // so actually rasBuf is not used.
        rasBuf = OSAlloc(size);
        
        // Call tev-emu utility and combine texture.
        TEImgSetSize(wd, ht);
        TEImgSetStageInput(GX_TEVSTAGE0, rasBuf, texBuf);
        TEImgCombine(mixBuf);
        
        // Draw a quad by using combined result texture.
        GXInitTexObj(&tx2, mixBuf, wd, ht, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
        GXLoadTexObj(&tx2, GX_TEXMAP0);
        GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
        DrawQuad();
        
        OSFree(rasBuf);
        OSFree(mixBuf);
    }
    
#endif // EPPC
}

/*---------------------------------------------------------------------------*
    Name:           SetTev
    
    Description:    Set up TEV details.
                    In this function, TEV functions are called via TE*
                    function set which is defined in tev-emu utility.
                    In the case of actual GX (non-emulator), TE* functions
                    will be simply replaced by GX* functions automatically.
                    (See "tev-emu.h")
    
    Arguments:      ts : pointer to a tev state structure

    Returns:        none
 *---------------------------------------------------------------------------*/
static void SetTev( MyTevStateObj* ts )
{
    GXColor  col;
    u8       bias;

    TESetNumTevStages(1);
    
    // Clamp mode
    TESetTevClampMode(GX_TEVSTAGE0, ClampModeTbl[ts->clampMode]);

    // Detailed bias is generated by using register 
    bias = (u8)(255 * ts->biasMode / ( NUM_TEVBIASES - 1 ));
    col.r = col.g = col.b = col.a = bias;
    TESetTevColor(GX_TEVREG0, col);
    
    TESetTevColorIn(
        GX_TEVSTAGE0,
        GX_CC_ZERO,    // a
        GX_CC_C0,      // b
        GX_CC_ONE,     // c
        GX_CC_TEXC );  // d
    
    TESetTevColorOp(
        GX_TEVSTAGE0,
        GX_TEV_SUB,                  // Op (Add/Sub)
        GX_TB_ZERO,                  // Bias
        ScaleModeTbl[ts->scaleMode], // Scale
        ts->clampSw,                 // Clamp
        GX_TEVPREV );                // Output
    
    TESetTevAlphaIn(
        GX_TEVSTAGE0,
        GX_CA_ZERO,    // a
        GX_CA_A0,      // b
        GX_CA_ONE,     // c
        GX_CA_TEXA );  // d
    
    TESetTevAlphaOp(
        GX_TEVSTAGE0,
        GX_TEV_SUB,                  // Op (Add/Sub)
        GX_TB_ZERO,                  // Bias
        ScaleModeTbl[ts->scaleMode], // Scale
        ts->clampSw,                 // Clamp
        GX_TEVPREV );                // Output
}

/*---------------------------------------------------------------------------*
    Name:           PrintIntro
    
    Description:    Prints the directions on how to use this demo.
                    
    Arguments:      none

    Returns:        none
 *---------------------------------------------------------------------------*/
static void PrintIntro( void )
{
    OSReport("\n\n");
    OSReport("************************************************\n");
    OSReport("tev-clamp: TEV clamp mode test\n");
    OSReport("************************************************\n");
    OSReport("to quit hit the menu button\n");
    OSReport("\n");
    OSReport("X/Y buttons : move the cursor\n");
    OSReport("L/R triggers: change parameter\n");
    OSReport("************************************************\n\n");
}

/*---------------------------------------------------------------------------*
    Name:           SingleFrameSetUp
    
    Description:    Sets up parameters to make single frame snapshots.
                    (This function is used for single frame test only.)

    Arguments:      sc : pointer to the structure of scene control parameters
    
    Returns:        none
 *---------------------------------------------------------------------------*/
#ifdef __SINGLEFRAME  // single frame tests for checking hardware
static void SingleFrameSetUp( MySceneCtrlObj* sc )
{
    u32  mode = __SINGLEFRAME;

    switch(mode%4)
    {
      case 0:
        {
            sc->currentTex         = 0;
            sc->tevState.biasMode  = 0;    // -0/16
            sc->tevState.scaleMode = 0;    // x1
        } break;
      case 1:
        {
            sc->currentTex         = 0;
            sc->tevState.biasMode  = 8;    // -8/16
            sc->tevState.scaleMode = 0;    // x1
        } break;
      case 2:
        {
            sc->currentTex         = 0;
            sc->tevState.biasMode  = 10;   // -10/16
            sc->tevState.scaleMode = 2;    // x4
        } break;
      case 3:
        {
            sc->currentTex         = 2;
            sc->tevState.biasMode  = 4;    // -4/16
            sc->tevState.scaleMode = 3;    // /2
        } break;
    }
}
#endif

/*============================================================================*/
