/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     texPrev2.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/charPipeline/src/texPrev2.c $    
    
    15    8/31/00 6:12p Dante
    Added WIN32 directive
    
    14    7/14/00 2:35p Dante
    
    13    7/07/00 8:47p Dante
    PC Compatibility
    
    12    6/22/00 9:45a John
    Fixed a bug where DVDRead was not receiving a 32-byte aligned filename
    address.
    
    11    5/10/00 5:01p Carl
    Fixed screen size problem that made text hard to read.
    Also changed alphaMode check for HW version.
    
    10    4/12/00 6:29a Ryan
    removed reference to filelib.h
    
    9     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
    8     3/17/00 4:36p John
    Texture previewer now loads using the preview.bat system and not the
    MAC menu system.  On the PC, execute "preview.bat [name of database w/o
    three letter extension] tpl".  Then make sure dolphin folder and
    texPrev2 binary is on MAC desktop.  Then execute texPrev2.
    
    7     2/24/00 11:23p Hirose
    updated pad control functions to match actual HW1 gamepad
    
    6     2/24/00 7:06p Yasu
    Rename gamepad key to match HW1
    
    5     1/18/00 7:38p Hirose
    updated to work with new GXInit defaults
    
    4     1/13/00 8:53p Danm
    Added GXRenderModeObj * parameter to DEMOInit()
    
    3     12/10/99 4:47p Carl
    Fixed Z compare stuff.
    
    13    11/17/99 1:24p Alligator
    removed instances of 'near' and 'far' for PC emulator port
    
    12    11/12/99 5:00p Yasu
    Add GXSetNumTexGens
    
    11    11/03/99 7:38p Yasu
    Replace GX*1u8,u16 to GX*1x8,x16
    
    10    10/12/99 10:02a Hirose
    some bug fixes and enhancements
    
    9     9/30/99 10:33p Yasu
    Renamed some functions and enums
    
    8     9/20/99 11:37a Ryan
    update to use new DEMO calls
    
    7     9/17/99 6:38p Hirose
    changed GXInitTlutObj and GXLoadTlut parameters
    
    6     9/16/99 8:02p Hirose
    arbitrary TLUT size support
    some interface changes
    
    5     9/15/99 3:13p Ryan
    update to fix compiler warnings
    
    4     9/01/99 5:45p Ryan
    
    3     8/27/99 2:45p Yasu
    Change parameter of GXSetZMode()
    
    2     8/18/99 11:22a Alligator
    changed GXLoadTlut and GXInitTexObjCI for new api changes
    
    1     8/16/99 11:31p Hirose
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
   Header files
 *---------------------------------------------------------------------------*/
#include <demo.h>
#include <charPipeline.h>

#ifdef flagWIN32
#include <CharPipe/FileLib.h>
#endif

#include <stdio.h>
#include <string.h>

/*---------------------------------------------------------------------------*
   Macro definitions
 *---------------------------------------------------------------------------*/
#define Clamp(val,min,max) \
    ((val) = (((val) < (min)) ? (min) : ((val) > (max)) ? (max) : (val)))

/*---------------------------------------------------------------------------*
   Structure definitions
 *---------------------------------------------------------------------------*/
// for camera
typedef struct
{
    Vec    location;
    Vec    up;
    Vec    target;
    f32    left;
    f32    top;
    f32    znear;
    f32    zfar;
} CameraConfig;

typedef struct
{
    CameraConfig  cfg; 
    Mtx           view;
    Mtx44         proj;
} MyCameraObj;

// for texture
typedef struct
{
    u32           number;
    GXTexObj      texObj;
    u16           width;
    u16           height;
    s32           maxLOD;
    s32           level;
    char*         information;
} MyTextureObj;

// for entire scene control
typedef struct
{
    MyCameraObj   cam;
    MyTextureObj  texture;
    Mtx           modelCtrl;
    f32           modelScale;
    u32           alphaMode;
    u32           borderMode;
    u32           window;
    u16           sWidth;
    u16           sHeight;
} MySceneCtrlObj;

/*---------------------------------------------------------------------------*
   Forward references
 *---------------------------------------------------------------------------*/
void        main            ( void );
static void DrawInit        ( MySceneCtrlObj* sc );
static void DrawTick        ( MySceneCtrlObj* sc );
static void AnimTick        ( MySceneCtrlObj* sc, PADStatus* pad );
static void StickTick       ( PADStatus* pad );
static u16  StickDone       ( PADStatus* pad );
static void LoadTplFile     ( void );
static void SetTplTexture   ( MyTextureObj* to );
static void DrawBox         ( void );
static void SetCamera       ( MyCameraObj* cam );
static void Message         ( void );

/*---------------------------------------------------------------------------*
  Model Data
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
   Data arrays for indexed primitives must be 32B aligned.  Normally, memory
   for the arrays would be OSAlloc'd (which returns 32B aligned pointers) and
   the data would be loaded from ROM.  The pragma variable_align provides a
   convenient way to align initialized arrays.
 *---------------------------------------------------------------------------*/

static s16 TopVerts[] ATTRIBUTE_ALIGN(32) = 
{
    -1, -1,  1, // 0
     1, -1,  1, // 1
     1,  1,  1, // 2
    -1,  1,  1, // 3
    -1, -1, -1, // 4
     1, -1, -1, // 5
     1,  1, -1, // 6
    -1,  1, -1  // 7
};

static u8 Colors[] ATTRIBUTE_ALIGN(32) =
{
    0x80, 0x00, 0x00, 0x80,
    0x00, 0x80, 0x00, 0x80,
    0x00, 0x00, 0x80, 0x80,
    0x80, 0x80, 0x00, 0x80,
    0x80, 0x00, 0x80, 0x80,
    0x00, 0x80, 0x80, 0x80,
};

/*---------------------------------------------------------------------------*
   Strings Data
 *---------------------------------------------------------------------------*/
static char* TxFmtStr[] =
{
    "I4",
    "I8",
    "IA4",
    "IA8",
    "RGB565",
    "RGB5A3",
    "RGBA8",
    "?",
    "CI4",
    "CI8",
    "CI14X2",
    "?",
    "?",
    "?",
    "Compressed"
};

static char* TxWrapStr[] =
{
    "Clamp",
    "Repeat",
    "Mirror"
};

static char* TxFilterStr[] =
{
    "Near",
    "Linear",
    "Near/Near",
    "Linear/Near",
    "Near/Linear",
    "Linear/Linear"
};

static char* TxSwStr[] =
{
    "OFF",
    "ON"
};

/*---------------------------------------------------------------------------*
   Camera configuration
 *---------------------------------------------------------------------------*/
static CameraConfig DefaultCamera =
{
    {   0.0F, -5000.0F, 0.0F }, // location
    {   0.0F,     0.0F, 1.0F }, // up
    {   0.0F,     0.0F, 0.0F }, // target
    320.0F,  // left
    240.0F,  // top
    0.0F,    // near
    10000.0F // far
};

/*---------------------------------------------------------------------------*
   Global variables
 *---------------------------------------------------------------------------*/
static MySceneCtrlObj   SceneCtrl;                // scene control parameters
static PADStatus        Pad[PAD_MAX_CONTROLLERS]; // game pad status
static TEXPalettePtr    TplObj = NULL;            // texture palette
static u32              Tmax;                     // number of textures in tpl

/*---------------------------------------------------------------------------*
   Application main loop
 *---------------------------------------------------------------------------*/
void main ( void )
{
    GXRenderModeObj *rm;

    DEMOInit(NULL);  // Init the OS, game pad, graphics and video.

    rm = DEMOGetRenderModeObj();
    SceneCtrl.sWidth = rm->fbWidth;
    SceneCtrl.sHeight = rm->efbHeight;

    DrawInit(&SceneCtrl); // Define vertex formats and scene etc.
    Message();
    LoadTplFile();
    SetTplTexture(&SceneCtrl.texture);

    while(!StickDone(Pad))
    {
        DEMOBeforeRender();
        DrawTick(&SceneCtrl);       // Draw the model.
        DEMODoneRender();
        StickTick(Pad);             // Read controller
        AnimTick(&SceneCtrl, Pad);  // Do animation
    }

    OSHalt("End of demo");
}

/*---------------------------------------------------------------------------*
   Functions
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
    Name:           DrawInit
    
    Description:    Initializes vertex attribute formats, and sets
                    the array pointers and strides for the indexed data.
                    Also sets some default state for this app.
                    
    Arguments:      sc : pointer to the structure of scene control parameters
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void DrawInit( MySceneCtrlObj* sc )
{
    //  Vertex Attribute Format

    //  The format 0 is used by DEMOPuts library and
    //  So the main drawing of this demo use the format 1
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);   
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    
    //  Set Array Pointers and Strides
    // stride = 3 elements (x,y,z) each of type s16
    GXSetArray(GX_VA_POS, TopVerts, 3*sizeof(s16));
    // stride = 4 elements (r,g,b,a) each of type u8
    GXSetArray(GX_VA_CLR0, Colors, 4*sizeof(u8));


    //  Scene parameters

    // camera
    sc->cam.cfg = DefaultCamera;
   
    // texture number
    sc->texture.number = 0;

    // texture level control option (default off)
    sc->texture.level = -1;
    
    // texture information strings
    sc->texture.information = OSAlloc(256);
    
    // model rotation and scale
    sc->modelScale = 1.0F;
    MTXIdentity(sc->modelCtrl);

    // alpha check mode, border mode and window status
    sc->alphaMode  = 0;
    sc->borderMode = 0;
    sc->window     = 1;
}

/*---------------------------------------------------------------------------*
    Name:           DrawTick
    
    Description:    Draw the model by using given scene parameters 
                    
    Arguments:      sc : pointer to the structure of scene control parameters
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void DrawTick( MySceneCtrlObj* sc )
{
    Mtx        ms; // Model scale matrix.
    Mtx        mv; // Modelview matrix.
    f32        s;

    SetCamera(&sc->cam);

    s = sc->borderMode ? ( sc->modelScale * 1.30F ) : sc->modelScale;

    // Set Z mode (It will be modified by DEMOPuts lib.)
    GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
    
    // Set modelview matrix
    MTXConcat(sc->cam.view, sc->modelCtrl, mv); 
    MTXScale(ms, s * sc->texture.width / 2, s * sc->texture.height / 2, s * sc->texture.height / 2);
    MTXConcat(mv, ms, mv);
    GXLoadPosMtxImm(mv, GX_PNMTX0);

    // Set texture coordinate
    MTXIdentity(mv);
    if ( sc->borderMode )
    {
        MTXScale(ms, 1.30F, 1.30F, 0.0F);
        MTXConcat(ms, mv, mv);
        MTXTrans(ms, -0.15F, -0.15F, 0.0F);
        MTXConcat(ms, mv, mv);
    }
    GXLoadTexMtxImm(mv, GX_TEXMTX0, GX_MTX2x4);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);

    // Tev mode = one color / one texture
    GXSetNumChans(1);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    switch(sc->alphaMode) 
    {
      case 0:
        GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
        break;
      case 1:
#if defined(MAC) || defined(flagWIN32)
        GXSetTevOp(GX_TEVSTAGE0, GX_DECAL);
#else
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
                        GX_CC_TEXA);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
                        GX_CA_TEXA);
#endif
        break;
      default:
        ASSERTMSG(0, "Bad alpha mode");
    }

    // load texture obj
    GXLoadTexObj( &sc->texture.texObj, GX_TEXMAP0 );
    
    // draw the textured box
    DrawBox();


    // draw the texture status window by using DEMOPuts library
    if ( sc->window )
    {
        DEMOInitCaption( DM_FT_OPQ, sc->sWidth, sc->sHeight );
        DEMOPrintf( 0, 0, 0, "Alpha Check : %s", TxSwStr[sc->alphaMode]);
        DEMOPrintf( 0, 8, 0, "Wrap  Check : %s", TxSwStr[sc->borderMode]);
        DEMOPuts( 0, 32, 0, sc->texture.information );
        
        if ( sc->texture.level > 0 )
        {
            DEMOPrintf( 0, 16, 0, "View Level  : +%d", sc->texture.level);
        }
    }

}

/*---------------------------------------------------------------------------*
    Name:           AnimTick
    
    Description:    Changes scene parameters according to the pad status.
                    
    Arguments:      pad : pad status
                    sc  : pointer to the structure of scene control parameters
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void AnimTick( MySceneCtrlObj* sc, PADStatus* pad )
{
    static u16 button = 0;
    
    u16  down;
    f32  zc;
    Mtx  mrx, mry;
    
    down = PADButtonDown(button, pad->button);
    button = pad->button;

    // Reset the view
    if ( down & PAD_BUTTON_B )
    {
        if ( sc->texture.level > 0 )
        {
            sc->texture.level  = 0;
            SetTplTexture(&sc->texture);
        }
        sc->modelScale = 1.0F;
        MTXIdentity(sc->modelCtrl);
    }
    
    // Model Rotation Calculation
    MTXRotDeg(mry, 'x', -(f32)(pad->stickY)/64.0F);
    MTXRotDeg(mrx, 'z', -(f32)(pad->stickX)/64.0F);
    MTXConcat(mry, sc->modelCtrl, sc->modelCtrl);
    MTXConcat(mrx, sc->modelCtrl, sc->modelCtrl);

    // Model Scale Control
    zc = 1.0F + (f32)(pad->substickY / 64) * 0.1F;
    sc->modelScale *= zc;

    Clamp(sc->modelScale, 0.0001F, 4.0F);
    
    if ( (down & PAD_TRIGGER_R) || (down & PAD_TRIGGER_L) )
    {
        // Load new TPL file
        if ( ( button & PAD_TRIGGER_R ) &&
             ( button & PAD_TRIGGER_L ) &&
             ( button & PAD_BUTTON_A ) )
        {
            LoadTplFile();
            sc->texture.number = 0;
        }
        // Mipmap level control
        else if ( button & PAD_BUTTON_B )
        {
            if ( down & PAD_TRIGGER_R )
            {
                ++(sc->texture.level);
            }
            if ( down & PAD_TRIGGER_L )
            {
                sc->texture.level += sc->texture.maxLOD;
            }
            sc->texture.level = sc->texture.level % ( sc->texture.maxLOD + 1 );
	    }
	    // Change texture number
        else
        {
            if ( down & PAD_TRIGGER_R )
            {
                sc->texture.number = ( sc->texture.number + 1 ) % Tmax;
            }
            if ( down & PAD_TRIGGER_L )
            {
                sc->texture.number = ( sc->texture.number + Tmax - 1 ) % Tmax;
            }
            sc->texture.level = 0;
        }   
    
        SetTplTexture(&sc->texture);
        sc->modelScale = 1.0F;
        MTXIdentity(sc->modelCtrl);
    }

    // Border mode
    if ( down & PAD_BUTTON_X )
    {
        sc->borderMode = (u32)(sc->borderMode ? 0 : 1);
    }

    // Tev mode (for alpha check)
    if ( down & PAD_BUTTON_Y )
    {
        sc->alphaMode = (u32)(sc->alphaMode ? 0 : 1);
    }

    // Hide status window
    sc->window = (u32)(( button & PAD_BUTTON_A ) ? 0 : 1);

}

/*---------------------------------------------------------------------------*
    Name:           LoadTplFile
    
    Description:    loads tpl file from MAC window
                    
    Arguments:      none
    
    Returns:        none
 *---------------------------------------------------------------------------*/
void LoadTplFile( void )
{
#ifdef flagWIN32
	if(WINGetFile("Dolphin Files (*.tpl)\0*.tpl\0")) {
		DVDSetRoot(FilePath);
	}

    // release texture palette if we currently have one
    if ( TplObj != 0 )
    {
        TEXReleasePalette(&TplObj);
    }
    // get the tpl file
    TEXGetPalette(&TplObj, FileName);

    Tmax = TplObj->numDescriptors;
    
    ASSERTMSG( TplObj != NULL, "TPL get failed\n");
#else
    DVDFileInfo dfi;
    char *fileName;
    char *cursor;

    DVDChangeDir("preview");
	DVDOpen("prevload.txt", &dfi);
	fileName = (char*)OSAlloc(OSRoundUp32B(dfi.length));
	DVDRead(&dfi, fileName, (s32)OSRoundUp32B(dfi.length), 0);
	DVDClose(&dfi);

    // find the last dot (looking for 3-letter extension)
    cursor = fileName;
    while(*cursor != '.')
    {
        cursor++;
        ASSERTMSG(*cursor, "file name has no extension");
    }
    cursor++;
    cursor[3] = '\0';
    
    // make sure that there is a tpl extension
    ASSERTMSG(!Strcmp( cursor, "tpl"), "not TPL file\n");

    // release texture palette if we currently have one
    if ( TplObj != 0 )
    {
        TEXReleasePalette(&TplObj);
    }
    // get the tpl file
    TEXGetPalette(&TplObj, fileName);
    
    ASSERTMSG( TplObj != NULL, "TPL get failed\n");
    
    Tmax = TplObj->numDescriptors;
    OSFree(fileName);
#endif
}

/*---------------------------------------------------------------------------*
    Name:           SetTplTexture
    
    Description:    specifies current texture to be displayed
                    
    Arguments:      to : pointer to MyTextureObj structure
    
    Returns:        none
 *---------------------------------------------------------------------------*/
void SetTplTexture( MyTextureObj* to )
{
    TEXDescriptorPtr tdp;
    GXBool           mipMapFlag;
    u32              fmt, num;
    u32              data;
    u16              width, height;
    s32              levCnt;
    char*            sp;

    num = to->number;
    if ( num >= TplObj->numDescriptors )
    {
        num = 0;
    }

    tdp = TEXGet(TplObj, num);
    
    mipMapFlag =
        ( tdp->textureHeader->minLOD == tdp->textureHeader->maxLOD ) ?
        GX_FALSE : GX_TRUE;

    fmt = (u32)tdp->textureHeader->format;
    width  = to->width  = tdp->textureHeader->width;
    height = to->height = tdp->textureHeader->height;
    data   = (u32)tdp->textureHeader->data;
    to->maxLOD = tdp->textureHeader->maxLOD;

    // mipmap level control option
    if ( mipMapFlag )
    {
        for ( levCnt = 0 ; levCnt < to->level ; ++levCnt )
        {
            if ( levCnt >= tdp->textureHeader->minLOD )
            {
                data += GXGetTexBufferSize(width, height, fmt, GX_FALSE, 0);
            }
        
            width  = (u16)(( width  > 1 ) ? ( width  / 2 ) : 1);
            height = (u16)(( height > 1 ) ? ( height / 2 ) : 1);
        }
    }

    if ( fmt == GX_TF_C4 || fmt == GX_TF_C8 )
    {
        GXTlutObj tlutObj;
    
        GXInitTexObjCI(
            &to->texObj,
            (void*)data,
            width,
            height,
            (GXCITexFmt)fmt,
            GX_REPEAT, // s
            GX_REPEAT, // t
            mipMapFlag, // Mipmap
            GX_TLUT0 );
    
        GXInitTlutObj(
            &tlutObj,
            tdp->CLUTHeader->data,
            (GXTlutFmt)tdp->CLUTHeader->format,
            tdp->CLUTHeader->numEntries );
        GXLoadTlut(&tlutObj, GX_TLUT0);
    }
    else if ( fmt == GX_TF_C14X2 )
    {
        GXTlutObj tlutObj;
    
        GXInitTexObjCI(
            &to->texObj,
            (void*)data,
            width,
            height,
            (GXCITexFmt)fmt,
            GX_REPEAT, // s
            GX_REPEAT, // t
            mipMapFlag, // Mipmap
            GX_BIGTLUT0 );
    
        ASSERTMSG( tdp->CLUTHeader->numEntries <= 1024,
                   "CLUT size is too big to preview\n" );
    
        GXInitTlutObj(
            &tlutObj,
            tdp->CLUTHeader->data,
            (GXTlutFmt)tdp->CLUTHeader->format,
            tdp->CLUTHeader->numEntries );
        GXLoadTlut(&tlutObj, GX_BIGTLUT0);
    }
    else
    {
        GXInitTexObj(
            &to->texObj,
            (void*)data,
            width,
            height,
            (GXTexFmt)fmt,
            GX_REPEAT, // s
            GX_REPEAT, // t
            mipMapFlag ); // Mipmap
    }

    GXInitTexObjLOD(
        &to->texObj,
        tdp->textureHeader->minFilter, 
        ( to->level > 0 ) ? GX_NEAR : tdp->textureHeader->magFilter, 
        tdp->textureHeader->minLOD, 
        to->maxLOD - levCnt,
        tdp->textureHeader->LODBias,
        GX_DISABLE,
        tdp->textureHeader->edgeLODEnable,
        GX_ANISO_1 );

    GXInvalidateTexAll();


    // generate information strings
    sp = to->information;
    sp += sprintf(sp, "Number : %04d\n", to->number); 
    sp += sprintf(sp, "Width  : %04d\n", tdp->textureHeader->width); 
    sp += sprintf(sp, "Height : %04d\n", tdp->textureHeader->height);
    sp += sprintf(sp, "Format : %s\n", TxFmtStr[tdp->textureHeader->format]);
    sp += sprintf(sp, "Wrap_s : %s\n", TxWrapStr[tdp->textureHeader->wrapS]);
    sp += sprintf(sp, "Wrap_t : %s\n", TxWrapStr[tdp->textureHeader->wrapT]);
    sp += sprintf(sp, "MinFil : %s\n", TxFilterStr[tdp->textureHeader->minFilter]);
    sp += sprintf(sp, "MagFil : %s\n", TxFilterStr[tdp->textureHeader->magFilter]);
    sp += sprintf(sp, "MinLOD : %d\n", tdp->textureHeader->minLOD); 
    sp += sprintf(sp, "MaxLOD : %d\n", tdp->textureHeader->maxLOD); 
    sp += sprintf(sp, "LODBias: %.3f\n", tdp->textureHeader->LODBias);  
    sp += sprintf(sp, "MipMap : %s\n", TxSwStr[mipMapFlag]);
    *sp = '\0';

}

/*---------------------------------------------------------------------------*
    Name:           StickTick
    
    Description:    computes analog tilt x, y 
                    
    Arguments:      pad : pointer to pad status
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void StickTick( PADStatus* pad )
{
    PADRead(pad);
}

/*---------------------------------------------------------------------------*
    Name:           StickDone
    
    Description:    return true when START button pressed
                    
    Arguments:      pad : pointer to pad status
    
    Returns:        u16
 *---------------------------------------------------------------------------*/
static u16 StickDone( PADStatus* pad )
{
    return((u16)(pad->button & PAD_BUTTON_MENU));
}

/*---------------------------------------------------------------------------*
    Name:           VertexT
    
    Description:    Draw a vertex with texture, direct data
                    
    Arguments:      v        8-bit position index
                    s, t     16-bit tex coord
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static inline void VertexT( u8 v, f32 s, f32 t, u8 c )
{
    GXPosition1x8(v);
    GXColor1x8(c);
    GXTexCoord2f32(s, t);
}

/*---------------------------------------------------------------------------*
    Name:           DrawTexQuad
    
    Description:    Draw a textured quad.  Map extends to corners of the quad.
                    MAX_SCOORD is the value of 1.0 in the fixed point format.

    Arguments:      v0        8-bit position
                    v1        8-bit position
                    v2        8-bit position
                    v3        8-bit position
                    c         color index
                    b         border mode
 
    Returns:        none
 *---------------------------------------------------------------------------*/
static inline void DrawTexQuad(
    u8  v0,
    u8  v1,
    u8  v2,
    u8  v3,
    u8  c,
    u32 b )
{
    f32 st0, st1;

    if(b)
    {
        st0 = -0.125F;
        st1 = 1.125F;
    }
    else
    {
        st0 = 0.0F;
        st1 = 1.0F;
    }

    VertexT(v0, st0, st0, c);
    VertexT(v1, st1, st0, c);
    VertexT(v2, st1, st1, c);
    VertexT(v3, st0, st1, c);
}

/*---------------------------------------------------------------------------*
    Name:           DrawBox
       
    Description:    Draw a textured box.

    Arguments:      none
 
    Returns:        none
 *---------------------------------------------------------------------------*/
static void DrawBox( void )
{
    // set vertex descriptor
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    // draw the box
    GXBegin(GX_QUADS, GX_VTXFMT1, 4*6);
        DrawTexQuad(0, 1, 2, 3, 0, 0);
        DrawTexQuad(7, 6, 5, 4, 1, 0);
        DrawTexQuad(0, 3, 7, 4, 2, 0);
        DrawTexQuad(1, 0, 4, 5, 3, 0);
        DrawTexQuad(2, 1, 5, 6, 4, 0);
        DrawTexQuad(3, 2, 6, 7, 5, 0);
    GXEnd();
}

/*---------------------------------------------------------------------------*
    Name:           SetCamera
    
    Description:    set view matrix and load projection matrix into hardware
                    
    Arguments:      cam : pointer to the MyCameraObj structure
                    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void SetCamera( MyCameraObj* cam )
{
    MTXLookAt(
        cam->view,
        &cam->cfg.location,
        &cam->cfg.up,
        &cam->cfg.target );

    MTXOrtho(
        cam->proj,
        cam->cfg.top,
        - (cam->cfg.top),
        cam->cfg.left,
        - (cam->cfg.left),
        cam->cfg.znear,
        cam->cfg.zfar );
    GXSetProjection(cam->proj, GX_ORTHOGRAPHIC);
}

/*---------------------------------------------------------------------------*
    Name:           Message
    
    Description:    Output message for debugging
                    
    Arguments:      none
    
    Returns:        none
 *---------------------------------------------------------------------------*/
void Message(void)
{
    OSReport("***************************************\n");
    OSReport("Instructions:\n");
    OSReport("  Menu         : quit this program\n");
    OSReport("  Main stick   : rotate the box\n");
    OSReport("  Sub stick Y  : change scale of the box\n");
    OSReport("  L/R triggers : select the texture\n");
    OSReport("  A button     : hide status window\n");
    OSReport("  B button     : reset the view\n");
    OSReport("  Y button     : alpha map check ON/OFF\n");
    OSReport("  X button     : wrap mode check ON/OFF\n");
    OSReport("  A + L + R    : open another tpl file\n");
    OSReport("***************************************\n\n");
}

/*============================================================================*/

