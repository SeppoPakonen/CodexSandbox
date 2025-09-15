/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     perfView.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/charPipeline/src/perfView.c $  
    
    23    9/13/00 2:30p Mikepc
    
    22    9/13/00 12:58p Mikepc
    auto-places 10k spheresto 60hz  on startup;
    cleaned up InitCamera
    
    21    8/08/00 4:25p Hirose
    updated to call DEMOSwapBuffers instead of using own routine
    
    20    8/04/00 5:11p John
    No longer calls ACTBuildMatrices every frame for unanimated actors.
    
    19    7/18/00 9:00p Tian
    Updated menus to reflect changes in performance vis. library
    
    18    7/18/00 7:33p John
    Modified and optimized to use new CTRL library.
    Removed FLASHER_MODE_OBJ to scale and rotate objects uniquely.
    
    17    7/18/00 3:52p Tian
    Added new performance visualization graphs.  New flasher mode for graph
    management.

    16    7/07/00 1:55p John
    Substituted MemZero with memset and MemCopy with memcpy.
    Skips over Object Mode (red flasher) to optimize: does not rotate
    objects within a grid.
    
    15    6/27/00 6:19p John
    Changed the multi-fifo mode to use a single fifo where CPU renders one
    frame ahead of GP using fifo breakpoints.
    Added #define FIFO_IMMEDIATE_MODE to use immediate mode, also in MAC
    build.  Cannot switch between modes at runtime.
    Added MemAlloc to error if we cannot allocate more memory.
    
    14    6/22/00 9:49a John
    Added mult-fifo method to decouple CPU and GP rendering time.
    Added a fourth cursor mode for utilities (check free memory, cull all
    triangles, switch between immediate mode and multi-fifo)
    Now can cycle through no background, and it is okay of bkgd and models
    directory are empty.
    Changed PAD functions to utilize DEMOPad.
    Frame bar now goes entirely across the top of the screen.  (Does not
    draw the 16.67 number).
    
    13    6/08/00 3:49p Mikepc
    added OSHalt for failed OSAlloc calls
    
    12    6/07/00 6:12p Mikepc
    more descriptive error messages.
    
    11    6/07/00 4:39p Mikepc
    updated single frame rate bar to contain both cpu and gx bars,
     improved visibility for tv.
    
    10    6/06/00 4:46p Mikepc
    changed performance bar color to lessen bleed on tv
    
    9     5/25/00 2:21p Ryan
    fixed 30 hz bug
    changed "sandshrw" to "sndShrew"
    
    8     5/21/00 8:18p Tian
    from Ryan - updated controller code to work with analog stick...
    changed immediate mode drawing code to work direct rather than indexed
    to work around indexed hardware bug.
    
    7     5/17/00 2:02p Mikepc
    MyDEMODoneRender():  updated 'extern' references to variables changed
    in DEMOInit.c
    
    6     5/16/00 12:22p Mikepc
    changed static data to align to 32B (line3966)
    
    4     4/06/00 12:20p Mikepc
    changed hard-coded model file SndShrew to SandShrw to use latest
    version.
    
    3     4/05/00 1:57p Mikepc
    fixed 'unused variable' (line 608) warning
    
    2     4/03/00 6:13p Mikepc
    
    1     4/03/00 6:10p Mikepc
    performance viewer
    
    1     4/03/00 6:08p Mikepc
    performance viewer


  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#include <math.h>
#include <string.h>
#include <charPipeline.h>
#include <demo.h>
#include <Dolphin/perf.h>

/*---------------------------------------------------------------------------*
   defines
 *---------------------------------------------------------------------------*/

// number of partitions in x and z of object grid.
// DEF_GRID_SEGS represents initial grid setting.
// MAX_GRID_SEGS, MIN_GRID_SEGS specify partition range.
#define DEF_GRID_SEGS        9 //5

#define MAX_GRID_SEGS        10
#define MIN_GRID_SEGS        1

// grid is a unit square with origin at (0,0).
// this offsets the grid to center it in the world.
// GRID_OFFSET_V jitters the grid in Y so that grid lines and 
// coplanar world polys at the origin won't overlap
#define GRID_OFFSET_H        -0.5f
#define GRID_OFFSET_V         0.0f

// animation frame rate
// note: we can't extract frame rate 'units'
//       from MAX; this assumes that artists
//       animated models at 30fps.
#define ANIM_HZ              30.0f

// an 'Obj' is the data structure of a displayable object.
// it contains a void* to a DODisplayObj or ACTActor;
// this identifies the void* type
#define OBJ_TYPE_NONE        0
#define OBJ_TYPE_DISP_OBJ    1
#define OBJ_TYPE_ACTOR       2
#define OBJ_TYPE_ACTOR_ANIM  3

// data structures are stored as linked lists;
// this identifies a node's data type
#define NODE_TYPE_NONE       0
#define NODE_TYPE_REF_OBJ    1
#define NODE_TYPE_OBJ        2

// an Obj's animation type
#define ANIM_TYPE_NONE       0    // no animation
#define ANIM_TYPE_ANIM_BANK  1    // Obj has an ANIMBank

// cursor attributes
#define FLASHER_MODE_TILE    0    // cursor operates on a tile
#define FLASHER_MODE_CAM     1    // cursor shows camera aim point
#define FLASHER_MODE_UTIL    2    // cursor goes into utility mode
#define FLASHER_MODE_PERF    3    // cursor goes into perf graph mode

// fifo attributes
//#define FIFO_IMMEDIATE_MODE       // use immediate mode instead of 
                                  // CPU one frame ahead of GP
#define FIFO_SIZE            (256*1024)
#define FIFO_DRAWDONE_TOKEN  0xBEEF
#define FIFO_DRAWING_TOKEN   0xCACE

// needed for degree<->radian conversion
#define PI                   3.141592654f

// performance graph defines

// maximum number of samples to take per measurement frame
#define PG_NUM_SAMPLES       70
// number of measurement frames.  There is currently no point in using 
// more than 1
#define PG_NUM_FRAMES        1
// total number of measurement events
#define PG_NUM_TYPES         3

// IDs for each event
#define PG_RENDER_EVENT      0
#define PG_ANIM_EVENT        1
#define PG_WAIT_EVENT        2

// define modes for performance graph
#define PG_BWBARS            0
#define PG_FRAMEBAR          1

// default display list size for displaying perf graph
#define DEFAULT_DL_SIZE      (32*1024)

#define WHITE                {255, 255, 255, 255}
#define BLACK                {00, 00, 00, 255}
#define YELLOW               {200, 200, 0, 200}

/*---------------------------------------------------------------------------*
   types
 *---------------------------------------------------------------------------*/

// generic linked list;
// points to 'Obj' or 'RefObj' structures
typedef struct Node
{
    u32    type;
    void*  data;

    struct Node* prev;
    struct Node* next;

} Node;


// instanced object:
// contains a pointer to either a DODisplayObj or ACTActor
typedef struct Obj
{
    u32         type;      // pDOA type: display object, actor, or 'actor with animation'
    void*       pDOA;      // ptr to display object or actor data

    u32         objId;     // unique id 
    void*       pRefObj;   // ptr to reference object

    u32         hasAnm;    // if 1, this object has an ANIMBank 

    CTRLControl worldCtrl; // world srt values
    Mtx         worldMtx;  // result matrix from worldCtrl
    
    f32         worldRad;  // bounding radius of world model

    u32         row;       // current grid position
    u32         col;

} Obj;

// reference object
typedef struct RefObj
{
    u32         refId;        // unique id
    u32         type;         // display object, actor, actor with animation
    Obj*        pObj;         // contains geometry, connectivity data

    char        objName[255]; // name of .act, .gpl file
    char        anmName[255]; // name of .anm file

    ANIMBank*   pAnmBank;     // ptr to ANIMBank; NULL if no animation

    CTRLControl baseCtrl;     // base srt values; converts from original model space
                              // to normalized model space

    f32         baseRad;      // bounding radius of base model

} RefObj;


// tethered camera;
// rotates about y axis only
typedef struct TCam
{
    f32 rotX;           // 0 to 89  deg. cw  from +z axis ( rot.about +x axis in yz plane )
    f32 rotY;           // 0 to 359 deg. ccw from +z axis ( rot.about +y axis in xz plane )
    f32 tether;         // length of tether ( cam to target )

    f32 tetherMin;      // min, max tether lengths
    f32 tetherMax;
    
    f32 rotSpeedX;      // left-right angular rotation speed ( deg/frame )
    f32 rotSpeedY;      // up-down angular speed ( deg/frame )
    f32 tetherSpeed;    // tether shorten-lengthen speed ( zoom )

    Vec pos;            // world position
    Vec up;             // 'up' vector
    Vec at;             // target

} TCam;

// on-screen cursor
typedef struct Flasher
{
    f32 vtx_f32[12];        // vertex list for cursor quad

    f32 halfWid;            // dimensions

    u32 mode;               // mode determines button actions

    u32 gridX;              // (col,row) indices within grid 
    u32 gridZ;

    Vec pos;                // world position

    u32 frame;              // animation frame for color cycling

} Flasher;


// rectangle for collision detection
typedef struct Rect2
{
    f32 left, right;
    f32 top,  bottom;

} Rect2;

// possible actions resulting from button presses.
// joypad buttons are mapped to these actions in
// ProcessInput()
typedef struct ActionList
{
    s32 quit;            // quit app.

    s32 reset;           // reset action depends on cursor mode

    s32 csrMode;         // cycle to next cursor mode
    s32 csrX;            // move cursor in x & z
    s32 csrZ;

    s32 camRotX;         // move camera
    s32 camRotY;
    s32 camZoom;

    s32 objSelect;       // cycle through RefObj list
 
    s32 targetTransX;    // change camera's aim point
    s32 targetTransZ;

    s32 gridScale;       // add rows, cols to grid

    s32 bkgdSelect;      // cycle through backgrounds

} ActionList;

/*---------------------------------------------------------------------------*
   Forward references
*---------------------------------------------------------------------------*/


// MAIN_FUNCTION
void main ( void ); 

// GENERAL_INIT_FUNCTIONS
static void DrawInit     ( void );
static void InitViewport ( void );
static u32  InitBkgd     ( void );
static void MyPerfInit   ( void );

// FILE_FUNCTIONS
static char** GetDirListing     ( char* dirName, u32* numFiles );
static void   SetFileNameBanks  ( void );
static void   FreeFileNameBanks ( void );

// GENERAL_TICK_FUNCTIONS
static void AnimTick         ( f32 animSpeed );
static void DrawTick         ( Mtx  v );
static void CheckReset       ( void );
static void MyDEMODoneRender ( void );

// REFOBJ_FUNCTIONS
static RefObj* InitRefObj        ( u32      id, char* fileName );
static u32     InitRefObjList    ( void );
static void    InitRefObjBaseSRT ( Node*    pRefObjList );
static void    FreeRefObj        ( RefObj** ppRefObj    );

// OBJ_FUNCTIONS
static Obj* InstanceObj     ( RefObj* pRefObj              );
static f32  ComputeObjRad   ( Obj*    pObj                 );
static void SetObjPos       ( Obj*    pObj,  Point3d* pPos );
static void DrawObj         ( Obj*    pObj,  Mtx         v );
static void FreeObj         ( Obj**   ppObj                );
static void UpdateInstObj   ( void );
static void SetWorldControl ( Obj *pObj );

// FLASHER_FUNCTIONS
static void InitFlasher      ( void );
static void SetFlasherPos    ( void );
static void UpdateFlasher    ( void );
static void DrawFlasher      ( void );
static void SelectSquare     ( void );
static Obj* CollFlasherToObj ( Node* pObjList );

// GRID_FUNCTIONS
static void InitGrid  ( void );
static void ScaleGrid ( void );
static void DrawGrid  ( void );

// CAMERA_FUNCTIONS
static void InitCamera   ( void );
static void ResetCamera  ( void );
static void CameraTick   ( void );
static void UpdateCamera ( void );
static void AimCamera    ( s32 xDir, s32 zDir );

// JOYSTICK_FUNCTIONS
static void ProcessInput ( void );

// LIGHT_FUNCTIONS
static void LightInit ( void );

// GUI_FUNCTIONS
static void DrawFrameBar ( f32 mSecCPU, f32 mSecGX );
static void PrintMenu    ( void );
//static void DrawNum      ( void );

// NODE_FUNCTIONS
static void  InsertNode     ( Node** ppList, Node*  pNode,    Node* pCursor );
static Node* InitNode       ( Node** ppList, Node*  pCursor                 );
static void  AttachData     ( Node*  pNode,  u32    nodeType, void* data    );
static Node* RemoveNode     ( Node** ppList, Node*  pNode                   );
static Node* FreeNode       ( Node** ppList, Node** ppNode                  );
static void  FreeNodeList   ( Node** ppList                                 );
static Node* FindNodeByData ( Node*  pList,  void*  pData                   );

// FIFO_FUNCTIONS
#if !defined(MAC) && !defined(FIFO_IMMEDIATE_MODE)
static void FifoInit           ( void );
static void FifoTick           ( void );
static void CheckRenderingTime ( void );
#endif

// HELPER_FUNCTIONS
static void* MemAlloc       ( u32 size );
static void  DoFree         ( void*  block );
static u32   Rect2InRect2   ( Rect2*        r1,    Rect2* r2 );
static f32   ComputeDObjRad ( DODisplayObj* pDObj );


/*---------------------------------------------------------------------------*
   Global variables
 *---------------------------------------------------------------------------*/

// object lists
static Node* RefObjList  = NULL;   // reference objects
static Node* InstObjList = NULL;   // instanced objects
static Node* BkgdObjList = NULL;   // background objects

// current background for display
static Obj* BkgdObj = NULL;

// mapping of joystick buttons to per-frame actions
static ActionList PadAction;

// lighting
static LITLightPtr light1;
static Mtx         CubeWorldMatrix;

// number of grid rows, cols,
// ratio of 1 grid square to full grid size
static u32 GridSegs   = DEF_GRID_SEGS;
static f32 GridScale  = 1.0f / (f32)DEF_GRID_SEGS;

// on-screen cursor
static Flasher Csr;

// tethered camera and viewing matrix
static TCam Camera;
static TCam ResetCam;
static Mtx  ViewMtx;

// file lists of models, backgrounds:
// get dir listing from each listed directory.

// root directory names
static char* BkgdDirName  = "/gxDemos/perfView/bkgd/";
static char* ModelDirName = "/gxDemos/perfView/models/";

// lists of reference objects, models.
// these will be allocated/filled using DVDReadDir
static u32    NumRefObj   =    0;
static char** RefObjName  = NULL;

static u32    NumBkgdObj  =    0;
static char** RefBkgdName = NULL;

// stopwatch and timing
static OSStopwatch  SwMsec;         // stopwatch for single frame render time
static volatile f32 MSecGX  = 0.0f; // volatile since it is during drawdone callback
static f32          MSecCPU = 0.0f; // stopwatch time in milliseconds

// Performance graph
// The graph display list is double buffered to avoid trampling data
// that the GP is using
static u8*   DL[2]; 
static u32   DLSize[2];
static u8    CurrDL;    // current display list index

static volatile u8    PerfGraphMode = PG_FRAMEBAR; // PG_BWBARS;
static volatile BOOL  PerfChangeMode = FALSE;
static vu32  PGFrameCount;  
static GXVtxDescList AttrList[GX_MAX_VTXDESCLIST_SZ];


static u32 Intro();

/*---------------------------------------------------------------------------*



                             MAIN_FUNCTION    



*----------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
    Name:           main
    
    Description:    application main loop
                
    Arguments:      none
    
    Returns:        none
*---------------------------------------------------------------------------*/
void main ( void )
{
    OSTime swTime;                           // current stopwatch time
    f32    animStep =    1.0f;               // animation step adjusted per frame to
                                             // produce fixed rate animation
    u32 introDone = 0;


    // -------- initialization -----------------------

    DrawInit();                              // init DO display cache, set gx blend modes

    MyPerfInit();

    OSInitStopwatch( &SwMsec, "render" );    // time to render single frame

    // ---------- demo loop ---------------------

    while( 1 )                               // must quit from shell   
    {

        if( !introDone )
        {
            introDone = Intro();
            continue;
        }


        if (PerfGraphMode == PG_BWBARS)
        {
            if (PGFrameCount % 2 == 0)
            {
                // start measurement frame.  it will terminate in MyDEMODoneRender
                PERFStartFrame();            
                PERFStartAutoSampling(1.0f);
            }
            // else, continue measuring the next frame
        }
        
        OSStartStopwatch( &SwMsec ); // SwMsec measure single frame render time for CPU and GP;

#if !defined(MAC) && !defined(FIFO_IMMEDIATE_MODE)
        FifoTick();                  // synchronize so CPU calculates next frame while
                                     // GP processes last frame
                                     // needs to be called at the top of main loop
#endif

        DEMOBeforeRender();

        ProcessInput();              // read the joystick for this frame;
                                     // map button presses to 'PadAction' variables

        CheckReset();                // if 'reset', reset camera, objects, etc.

        UpdateFlasher();             // move the cursor, change modes

        SelectSquare();              // allow user to choose an object

        if (PerfGraphMode == PG_BWBARS)
        {
            PERFEventStart(PG_ANIM_EVENT);
        }

        AnimTick( animStep );        // next frame of animation; animation rate is fixed
                                     // over time

        if (PerfGraphMode == PG_BWBARS)
        {
            PERFEventEnd(PG_ANIM_EVENT);
            PERFEventStart(PG_RENDER_EVENT);
        }

        CameraTick();                // move the camera, update ViewMtx

        ScaleGrid();                 // if 'gridScale', change the number of grid segments    

        DrawTick( ViewMtx );         // draw all of the objects.

        if (PerfGraphMode == PG_BWBARS)
        {
            PERFEventEnd(PG_RENDER_EVENT);
        }
        else
        {
            // draw performance bar using last frame's value.
            DrawFrameBar( MSecCPU, MSecGX ); 

            //DrawNum();                 // draw 16.67 bar
        }
                         
        MyDEMODoneRender();          // wait for rendering to complete.
                                     // check SwMsec after GXDone, before VIWaitForRetrace

                                     // check instantaneous frame rate
                                     // for fixed animation speed.

        swTime   = OSCheckStopwatch( &SwMsec );
        animStep = ((f32)OSTicksToSeconds( ((f32)swTime) )) * ANIM_HZ;  

        OSStopStopwatch(  &SwMsec ); // reset SwMsec for next frame
        OSResetStopwatch( &SwMsec );

    }   // end demo loop


    //----------------- clean up -----------------

    // free memory 
    FreeFileNameBanks();

    FreeNodeList( &RefObjList );
    RefObjList  = NULL;

    FreeNodeList( &InstObjList );
    InstObjList = NULL;

    FreeNodeList( &BkgdObjList );
    BkgdObjList = NULL;

    BkgdObj     = NULL;

} // end main()

/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*



                        GENERAL_INIT_FUNCTIONS    



*----------------------------------------------------------------------------*/



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

    GXSetChanAmbColor(GX_COLOR0A0, black);
    GXSetChanMatColor(GX_COLOR0A0, white);

    GXSetBlendMode( GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR );
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetZCompLoc(GX_TRUE);
    GXSetNumTexGens(1);
    GXSetNumChans(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
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
    
    PERFInit(PG_NUM_SAMPLES, 
             PG_NUM_FRAMES, 
             PG_NUM_TYPES, 
             MemAlloc, 
             DoFree, 
             FastDrawInit);

    PERFSetEvent(PG_RENDER_EVENT,  "RENDER",   PERF_CPU_GP_EVENT);
    PERFSetEvent(PG_ANIM_EVENT,    "ANIM",     PERF_CPU_EVENT);
    PERFSetEvent(PG_WAIT_EVENT,    "WAIT",     PERF_CPU_EVENT);
    PERFSetEventColor(PG_WAIT_EVENT, yellow);

    PERFSetDrawBWBarKey(TRUE);

    // create display list buffers
    DL[0]   = MemAlloc(DEFAULT_DL_SIZE);
    DL[1]   = MemAlloc(DEFAULT_DL_SIZE);
    CurrDL  = 0;
    PGFrameCount = 0;
}


/*---------------------------------------------------------------------------*
    Name:           DrawInit
    
    Description:    set blend modes, initialize display file cache.  
                    
    Arguments:      none    
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void DrawInit( void )
{
    GXColor clearClr = { 0, 0, 0, 255 }; // bkgd clear color

    DEMOInit( NULL );                       // from DEMO lib., calls OSInit, GXInit etc.

    GXSetCopyClear( clearClr, GX_MAX_Z24 ); // override GXInit bkgd clear color

    CSHInitDisplayCache();

    GXSetBlendMode( GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR );

    SetFileNameBanks();                     // get all files in the 'bkgd', 'model' directories.

    InitRefObjList();                       // load actor/disp. obj bank
                                           
    InitViewport();                         // set projection matrix

    InitBkgd();                             // load backrounds, set base srt values

    InitFlasher();                          // initial cursor mode, position

    InitGrid();                             // place an instanced object in ObjGrid[0][0]

    InitCamera();                           // initialize camera parameters, 
                                            // projection matrix, ViewMtx  

    LightInit();                            // initialize light position

#if !defined(MAC) && !defined(FIFO_IMMEDIATE_MODE)
    FifoInit();                             // initialize the fifos
                                            // needs to be called last before main loop
#endif
}

/*---------------------------------------------------------------------------*
    Name:           InitViewport
    
    Description:    set viewport dimensions  
                    
    Arguments:      none        
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void InitViewport()
{
    // note: check video modes 
    GXSetViewport(0.0F, 640.0F, 0.0F, 480.0F, 0.0F, 1.0F);
}

/*---------------------------------------------------------------------------*
    Name:           InitBkgd
    
    Description:    initialize the background list. 
                    bkgds exist as RefObj(s) only; no instancing.

    Arguments:      globals:  reads  BkgdDirName
                              writes GridSegs
                              writes GridScale
                              reads  NumBkgdObj
                              reads  RefBkgdName (file name list)
                              writes BkgdObjList
                              writes BkgdObj 
     
    Returns:        1 on success; 0 on failure
*---------------------------------------------------------------------------*/
static u32 InitBkgd( void )
{
    Node*   pNode;
    RefObj* pRefObj;
    Obj*    pObj;
    char*   namePtr;
    u32     i;


    BkgdObjList = NULL;

    DVDChangeDir( BkgdDirName );

    // initialize number of grid partitions
    GridSegs   = DEF_GRID_SEGS;
    GridScale  = 1.0f / (f32)DEF_GRID_SEGS;

    // create the background list
    for( i=0; i < NumBkgdObj; i++ )
    {    
        namePtr = RefBkgdName[i];

        pNode   = InitNode( &BkgdObjList, NULL );

        pRefObj = InitRefObj( i, namePtr );

        AttachData( pNode, NODE_TYPE_REF_OBJ, (void*)pRefObj );

        pObj    = pRefObj->pObj;

        // set base srt values:
        // note: bkgds must be modelled with correct dimensions to match the
        //       model grid (unit square);  bkgds don't scale.

        // set RefObj's pObj srt values once- bkgd doesn't move after that
        CTRLInit( &pRefObj->baseCtrl );
        CTRLInit( &pObj->worldCtrl );

        // assume bkgd in max. was modelled with z-axis as 'up'
        CTRLSetRotation( &pRefObj->baseCtrl, -90.0f, 0.0f, 0.0f );

        // compute and set the srt matrix for the object
        SetWorldControl( pObj );

        // bkgd bounding box
        pObj->worldRad = pRefObj->baseRad;

    } // end for


    // no backgrounds loaded
    if( BkgdObjList == NULL )
    {
        BkgdObj = NULL;
        return 0;
    }

    // point to 1st background
    pNode   = BkgdObjList;
    pRefObj = (RefObj*)pNode->data;
    BkgdObj = pRefObj->pObj;

    return 1;
}

/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*



                           FILE_FUNCTIONS



/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*
    Name:           GetDirListing
    
    Description:    use DVDReadDir to fetch all of the filenames
                    in a directory.
                    
                    sort filenames into .anm. .act, .gpl file hierarchy.
                    confirm that each .anm has a matching .act, .gpl file.
                    confirm that each .act has a matching .gpl file.
                    produce a combined list of .anm (eliminate matching .act, .gpl), 
                    .act (eliminate matching.gpl) and .gpl filenames.
                                      
    Arguments:      dirName:  name of directory to search
                    numFiles: ptr to receive number of files in final collated file list.
    
    Returns:        list of filenames as a 2-dimensional array
*---------------------------------------------------------------------------*/
static char** GetDirListing( char* dirName, u32* numFiles )
{
    DVDDir      dir;
    DVDDirEntry dirEntry;
    BOOL        ok        = FALSE;
    char**      fileNames = NULL;
    char*       pExt      = NULL;
    u32         i, j;
    u32         count     = 0;
    u32         len       = 0;
    u32         found     = 0;

    char** anm1     = NULL, **act1   = NULL, **gpl1   = NULL;
    u32    numAnm1  =    0, numAct1  =    0, numGpl1  =    0;
    u32    anmCount =    0, actCount =    0, gplCount =    0;
    u32    numAnm2  =    0, numAct2  =    0, numGpl2  =    0;

    char   tmpAct[255], tmpGpl[255];


    // open directory for read
    DVDChangeDir( dirName );
    if( (ok = DVDOpenDir( dirName, &dir )) == FALSE )
    {
//        OSReport( "error: GetDirListing: failed to open directory %s for read\n", dirName );
        return NULL;
    }

    // pass 1:
    // count all files; ignore subdirectories.
    // split files into anm, act, gpl lists
    while( (ok = DVDReadDir( &dir, &dirEntry)) == TRUE )
    {
        // file found; ignore subdirectories
        if( DVDDirEntryIsDir(&dirEntry)  == FALSE )
        {
            // count the number of anm, act, gpl files separately
            pExt = (char*)( DVDGetDirEntryName(&dirEntry) + strlen( DVDGetDirEntryName(&dirEntry) ) - 3 );
            
            if( (strcmp(pExt, "anm") == 0 ) || (strcmp(pExt, "ANM") == 0 ) )
            {
                numAnm1 ++;
            }
            else if( (strcmp(pExt, "act") == 0 ) || (strcmp(pExt, "ACT") == 0 ) )
            {
                numAct1 ++;
            }
            else if( (strcmp(pExt, "gpl") == 0 ) || (strcmp(pExt, "GPL") == 0 ) )
            {
                numGpl1 ++;
            }
        }
    }
    DVDRewindDir( &dir );


    // pass 2:
    // allocate initial full-size string blocks;
    // collect all file names
    if( !numAnm1 && !numAct1 && !numGpl1 )
    {
//        OSReport( "error: empty directory %s\n", dirName );
        return NULL;
    }

    if( numAnm1 )
    {
        if( ( anm1 = (char**)MemAlloc( (numAnm1 * sizeof(char*)) ) ) == NULL )
            OSHalt( "error: GetDirListing: failed to allocate anm1 \n" );
    }
    if( numAct1 )
    {
        if( ( act1 = (char**)MemAlloc( (numAct1 * sizeof(char*)) ) ) == NULL )
            OSHalt( "error: GetDirListing: failed to allocate act1\n" );
    }
    if( numGpl1 )
    {
        if( ( gpl1 = (char**)MemAlloc( (numGpl1 * sizeof(char*)) ) ) == NULL )
            OSHalt( "error: GetDirListing: failed to allocate gpl1\n" );
    }

    while( (ok = DVDReadDir( &dir, &dirEntry)) == TRUE )
    {
        if( DVDDirEntryIsDir( &dirEntry ) == FALSE )
        {
          
            pExt = (char*) ( DVDGetDirEntryName(&dirEntry) + strlen( DVDGetDirEntryName(&dirEntry) ) - 3 );      
            len  = strlen( dirEntry.name ) + 1;

            if( (strcmp(pExt, "anm") == 0 ) || (strcmp(pExt, "ANM") == 0 ) )
            {
                if( ( anm1[anmCount] = (char*)MemAlloc( len ) ) == NULL )
                    OSHalt( "error: GetDirListing: failed to allocate anm1[anmCount]\n" );

                strcpy( anm1[anmCount], DVDGetDirEntryName(&dirEntry) );
                anmCount++;
            }
            else if( (strcmp(pExt, "act") == 0 ) || (strcmp(pExt, "ACT") == 0 ) )
            {
                if( ( act1[actCount] = (char*)MemAlloc( len ) ) == NULL )
                    OSHalt( "error: GetDirListing: failed to allocate act1[actCount]\n" );

                strcpy( act1[actCount], DVDGetDirEntryName(&dirEntry) );
                actCount++;
            }
            else if( (strcmp(pExt, "gpl") == 0 ) || (strcmp(pExt, "GPL") == 0 ) )
            {
                if( ( gpl1[gplCount] = (char*)MemAlloc( len ) ) == NULL )
                    OSHalt( "error: GetDirListing: failed to allocate gpl1[gplCount]\n" );

                strcpy( gpl1[gplCount], DVDGetDirEntryName(&dirEntry) );
                gplCount++;
            }
        }
    }
    DVDRewindDir( &dir );
    DVDChangeDir( "/"  );
    DVDCloseDir(  &dir );


    // pass 3:

    // sort the file lists; each anm file must have a matching act, gpl file;
    // each act file must have a matching gpl file.

    // for each anm file, find and remove the corresponding act, gpl file names
    // for each act file, find and remove the corresponding gpl file name.
    // after this pass, the three lists will contain only unique names

    // anm files - all list entries are assumed valid
    numAnm2 = numAnm1;
    numAct2 = numAct1;
    numGpl2 = numGpl1;
    for( i=0; i < numAnm1; i++ )
    {
        // make a matching act file name
        strcpy( tmpAct, anm1[i] );
        pExt = (char*)( tmpAct + strlen( tmpAct ) - 3 );
        strcpy( pExt, "act" );

        // make a matching gpl file name
        strcpy( tmpGpl, anm1[i] );
        pExt = (char*)( tmpGpl + strlen( tmpGpl ) - 3 );
        strcpy( pExt, "gpl" );


        // traverse the act list and find a matching act file;
        // if found, remove it from the act1 list
        found = 0;
        for( j = 0; j < numAct1; j++ )
        {
            if( (strcmp( tmpAct, act1[j])) == 0 )
            {
                found = 1;
                act1[j][0] = '\0';
                numAct2--;
                break;
            }
        }

        if( found == 0 )
        {
            OSReport( "error: no matching .act file for .anm file %s\n", anm1[i] );
            return NULL;
        }


        // traverse the gpl list and find a matching gpl file;
        // if found, remove it from the gpl1 list
        found = 0;
        for( j = 0; j < numGpl1; j++ )
        {
            if( (strcmp( tmpGpl, gpl1[j])) == 0 )
            {
                found = 1;
                gpl1[j][0] = '\0';
                numGpl2 --;
                break;
            }
        }

        if( found == 0 )
        {
            OSReport( "error: no matching .gpl file for .anm file %s\n", anm1[i] );
            return NULL;
        }


    } // end for( i )

    
    //------------------------------

    // traverse the full act1 list and find a matching gpl file for each valid act entry;
    for( i=0; i < numAct1; i++ )
    {
 
        // after anm pass, some act entries will be invalid
        if( act1[i][0] == '\0' )
            continue;

        // make a matching gpl file name
        strcpy( tmpGpl, act1[i] );
        pExt = (char*)( tmpGpl + strlen( tmpGpl ) - 3 );
        strcpy( pExt, "gpl" );
    
        // traverse the gpl list and find a matching gpl file;
        // if found, remove it from the gpl1 list
        found = 0;
        for( j = 0; j < numGpl1; j++ )
        {
            if( (strcmp( tmpGpl, gpl1[j])) == 0 )
            {
                if( gpl1[j][0] == '\0' )
                    continue;

                found = 1;
                gpl1[j][0] = '\0';
                numGpl2 --;
                break;
            }
        }

        if( found == 0 )
        {
            OSReport( "error: no matching .gpl file for .act file %s\n", act1[i] );
            return NULL;
        }

    } // end for( i )

    
    //------------------------------

    // join the 3 lists; copy over into a single 'fileName' list
    *numFiles = numAnm2 + numAct2 + numGpl2;

    if( *numFiles == 0 )
    {
        OSReport( "error: empty anm/act/gpl file list for directory %s\n", dirName );
        return NULL;
    }

    if( ( fileNames = (char**)MemAlloc( (*numFiles * sizeof(char*)) ) ) == NULL )
        OSHalt( "error: GetDirListing: failed to allocate fileNames\n" );

    count = 0;
    for( i=0; i<numAnm2; i++ )
    {
        if( anm1[i][0] != '\0' )
        {
            len = strlen( anm1[i] ) + 1;

            if( ( fileNames[count] = (char*)MemAlloc( len ) ) == NULL )
                OSHalt( "error: GetDirListing: failed to allocate fileNames[count] in numAnm2 loop\n" );

            strcpy( fileNames[count], anm1[i] );
            count++;
        }
    }
    // 'count' increases continuously
    for( i=0; i<numAct2; i++ )
    {
        if( act1[i][0] != '\0' )
        {
            len = strlen( act1[i] ) + 1;
            if( ( fileNames[count] = (char*)MemAlloc( len ) ) == NULL )
                OSHalt( "error: GetDirListing: failed to allocate fileNames[count] in numAct2 loop\n" );

            strcpy( fileNames[count], act1[i] );
            count++;
        }
    }

    for( i=0; i<numGpl2; i++ )
    {
        if( gpl1[i][0] != '\0' )
        {
            len = strlen( gpl1[i] ) + 1;
            if( ( fileNames[count] = (char*)MemAlloc( len ) ) == NULL )
                OSHalt( "error: GetDirListing: failed to allocate fileNames[count] in numGpl2 loop\n" );

            strcpy( fileNames[count], gpl1[i] );
            count++;
        }
    }

    //------------------------------------------------------

    // free the full temporary string banks
    if( anm1 )
    {
        for( i=0; i<numAnm1; i++ )
        OSFree( anm1[i] );

        OSFree( anm1 );
    }

    if( act1 )
    {
        for( i=0; i<numAct1; i++ )
            OSFree( act1[i] );

        OSFree( act1 );
    }

    if( numGpl1 )
    {
        for( i=0; i<numGpl1; i++ )
            OSFree( gpl1[i] );

        OSFree( gpl1 );
    }

    //------------------------------------------------------

    return fileNames;
}

/*---------------------------------------------------------------------------*
    Name:           SetFileNameBanks
    
    Description:    initialize a list of filenames from a directory:
                    sort filenames into .anm. .act, .gpl file hierarchy.
                    confirm that each .anm has a matching .act, .gpl file.
                    confirm that each .act has a matching .gpl file.
                    produce a combined list of .anm (eliminate matching .act, .gpl), 
                    .act (eliminate matching.gpl) and .gpl filenames.
                                  
    Arguments:      globals:  reads BkgdDirName
                              reads ModelDirName
                              writes RefBkgdName
                              writes NumBkgdObj
                              writes RefObjName
                              writes NumRefObj
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void SetFileNameBanks()
{
    RefBkgdName = GetDirListing( BkgdDirName, &NumBkgdObj );

    // there must be at least 1 model from either ModelDirName or
    // the hard-coded file list.  InitRefObjList will catch this error.
    RefObjName = GetDirListing( ModelDirName, &NumRefObj );
}

/*---------------------------------------------------------------------------*
    Name:           FreeFileNameBanks
    
    Description:    frees the global 2d arrays of filenames.                  
                                                     
    Arguments:      globals:  reads  NumBkgdObj
                              writes RefBkgdName
                              reads  NumRefObj
                              writes RefObjName
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void FreeFileNameBanks( void )
{
    u32 i;

    if( RefBkgdName )
    {
        for( i=0; i<NumBkgdObj; i++ )
        {
            OSFree( RefBkgdName[i] );
        }
        OSFree( RefBkgdName );
        RefBkgdName = NULL;
    }

    if( RefObjName )
    {
        for( i=0; i<NumRefObj; i++ )
        {
            OSFree( RefObjName[i] );
        }
        OSFree( RefObjName );
        RefObjName = NULL;
    }
}

/*---------------------------------------------------------------------------*



                        GENERAL_TICK_FUNCTIONS    



*----------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*
    Name:           AnimTick
    
    Description:    animates instanced objects by calling ANIMTick.
                    uses 'animSpeed' to produce fixed rate animation over time.
                                        
    Arguments:      animSpeed: animation speed adjusted for frame time; produces
                               fixed rate animation.
                    globals:   reads InstObjList
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void AnimTick ( f32 animSpeed )
{
    Node* pNode;
    Obj*  pObj;


    for( pNode = InstObjList; pNode; pNode = pNode->next )
    {
        if( pNode->type != NODE_TYPE_OBJ )
            continue;

        pObj = (Obj*)pNode->data;
        if( !pObj )
            continue;

        if( (pObj->type == OBJ_TYPE_ACTOR_ANIM) && (pObj->hasAnm == ANIM_TYPE_ANIM_BANK) )
        {
            ACTSetSpeed( (ACTActor*)(pObj->pDOA), animSpeed );
            ACTTick( (ACTActor*)(pObj->pDOA) );
        }
    }
}

/*---------------------------------------------------------------------------*
    Name:           DrawTick
    
    Description:    draw all the objects in the scene.                 
                    
    Arguments:      v:        view matrix
                    globals:  reads light1,
                              reads BkgdObj, StageObj, FenceObj,
                              reads InstObjList
                       
    Returns:        none
 *---------------------------------------------------------------------------*/
static void DrawTick ( Mtx v )
{
    Node* pNode;
    Obj*  pObj;


    // transform light position
    LITXForm(light1, v);

    // draw background first
    if( BkgdObj )
        DrawObj( BkgdObj, v );

    // draw all instanced objects
    pNode = InstObjList;
    while( pNode )
    {
        if( pNode->type != NODE_TYPE_OBJ )
        {
            pNode = pNode->next;
            continue;
        }

        pObj = (Obj*)pNode->data;
        if( pObj )
            DrawObj( pObj, v );

        pNode = pNode->next;
    }

    // draw grid lines
    DrawGrid();

    // draw cursor
    DrawFlasher();
}

/*---------------------------------------------------------------------------*
    Name:           CheckReset()
    
    Description:    reset when PadAction.reset == 1.
                    type of reset depends on Csr.mode:
                    FLASHER_MODE_TILE: reset camera to default position
                    FLASHER_MODE_CAM:  reset camera aim point to grid center;
                                       reset GridScale to 1
                    FLASHER_MODE_UTIL: reset default mode
                    FLASHER_MODE_PERF: reset default mode

                
    Arguments:      globals:  reads  Csr,
                              reads  PadAction,
                              reads  InstObjList
                              writes Camera,
                              writes GridScale
                              writes InstObjList values
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void CheckReset( void )
{

    Node* pNode = NULL;
    Obj*  pObj  = NULL;


    if( PadAction.reset == 1 )
    {
       // reset operation depends on cursor mode
       switch( Csr.mode )
       {

         case FLASHER_MODE_TILE:          // reset camera to default position

           ResetCamera();
           break;

         case FLASHER_MODE_CAM:          // reset camera aim point to grid center
           
           Camera.at.x = 0.5f + GRID_OFFSET_H;
           Camera.at.y = 0.0f + GRID_OFFSET_V;
           Camera.at.z = 0.5f + GRID_OFFSET_H;

           break;

         case FLASHER_MODE_UTIL:
         case FLASHER_MODE_PERF:

           break;

       } // end switch

    } // end if( reset )
}

/*---------------------------------------------------------------------------*
    Name:           MyDEMODoneRender()
    
    Description:    rewrite of DEMODoneRender function - performs post-rendering operations.
                    receives a stopwatch activated at the top of the 'main' demoloop.
                    checks stopwatch after GXDrawDone and before VIWaitForRetrace to
                    compute actual frame rendering time in milliseconds.                  
               
    Arguments:      globals:  writes DemoFrameBuffer1,
                              writes DemoFrameBuffer2,
                              writes DemoCurrentBuffer
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void MyDEMODoneRender ( void )
{
    extern void*  DemoFrameBuffer1;   // static global variables from DEMOInit.c
    extern void*  DemoFrameBuffer2;
    extern void*  DemoCurrentBuffer;

    OSTime        swTime;             // current stopwatch time in 'OSTime' format. 

    if (PerfGraphMode == PG_BWBARS)
    {
        // check to see if we should render a new graph
        if (PGFrameCount % 2)
        {
            PERFEndFrame();    
            PERFStopAutoSampling();

            // swap display list buffers
            CurrDL = (u8)((CurrDL+1)%2);
            GXBeginDisplayList(DL[CurrDL], DEFAULT_DL_SIZE);
            PERFDumpScreen();
            DLSize[CurrDL] = GXEndDisplayList();
        }
        if (PGFrameCount > 0)
        {
            // if we've past the first two frames, 
            // we have a graph to render
            PERFPreDraw();
            GXCallDisplayList(DL[CurrDL], DLSize[CurrDL]);
            PERFPostDraw();
        }

        // after this, if PGFrameCount%2 then the next frame is the
        // second half of this measurement frame.
        // if PGFrameCount%2 == 0 then we have ENDED the measurement
        // frame, and all event sampling should be off.
        PGFrameCount++; 
    }

    // check to see if we should change graphing modes.  Should only do
    // so at the end of a measurement frame
    if (PerfChangeMode && (PGFrameCount%2==0))
    {
        PerfChangeMode = FALSE;

        if (PerfGraphMode == PG_BWBARS)
        {
            PerfGraphMode = PG_FRAMEBAR;
            PERFStopAutoSampling();
            PGFrameCount = 0;
        }
        else
        {
            PerfGraphMode = PG_BWBARS;
            PGFrameCount = 0;
        }
        OSReport("New graph mode is %s\n", 
                 (PerfGraphMode == PG_BWBARS ? 
                  "[performance visualizer]" :
                  "[basic frame time]"));
    }

#if !defined(MAC) && !defined(FIFO_IMMEDIATE_MODE)

    // copy out the other framebuffer since GP is a frame behind
    if( DemoCurrentBuffer == DemoFrameBuffer1 )
        GXCopyDisp(DemoFrameBuffer2, GX_TRUE);
    else
        GXCopyDisp(DemoFrameBuffer1, GX_TRUE);

    // send a drawdone token in the current CPU Fifo.
    // use GXSetDrawDone only to initiate a callback which will reveal GP processing time.
    if (PerfGraphMode == PG_FRAMEBAR)
    {
        GXSetDrawDone();            
    }
    GXSetDrawSync( FIFO_DRAWDONE_TOKEN );

    // check stopwatch time after cpu finishes sending
    // instructions but before rendering is complete.
    swTime  = OSCheckStopwatch( &SwMsec );
    MSecCPU = (f32)OSTicksToMilliseconds( ((f32)swTime) );   

    // count wait time if we are still measuring
    if (PerfGraphMode == PG_BWBARS)
    {
        if (PGFrameCount % 2)
        {
            PERFEventStart(PG_WAIT_EVENT);
        }
    }
    
    // wait until GP is finished by polling for the drawdone token in current GP Fifo
    while( GXReadDrawSync() != FIFO_DRAWDONE_TOKEN )
    {        
    }

    if (PerfGraphMode == PG_BWBARS)
    {
        if (PGFrameCount % 2)
        {
            PERFEventEnd(PG_WAIT_EVENT);
        }
    }

#else

    // copy out the EFB to XFB
    GXCopyDisp( DemoCurrentBuffer, GX_TRUE );

    // check stopwatch time after cpu finishes sending
    // instructions but before rendering is complete.
    swTime  = OSCheckStopwatch( &SwMsec );
    MSecCPU = (f32)OSTicksToMilliseconds( ((f32)swTime) );   

    // wait until GP is finished
    // count wait time if we are still measuring
    if (PerfGraphMode == PG_BWBARS)
    {
        if (PGFrameCount % 2)
        {
            PERFEventStart(PG_WAIT_EVENT);
        }
    }
    GXDrawDone();
    if (PerfGraphMode == PG_BWBARS)
    {
        if (PGFrameCount % 2)
        {
            PERFEventEnd(PG_WAIT_EVENT);
        }
    }

#endif

    //============================

    // Wait for vertical retrace and set the next frame buffer
    // Display the buffer which was just filled by GXCopyDisplay
    DEMOSwapBuffers();

}

/*---------------------------------------------------------------------------*



                             REFOBJ_FUNCTIONS      



*----------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*
    Name:           InitRefObj
    
    Description:    allocate and initialize a new RefObj structure
                    from a file
                
    Arguments:      id:       pRefObj->refId is set to this user-assigned id.
                              useful as a handle to a RefObj.
                    fileName: string name of .anm, .act or .gpl file to
                              use for initialization:

                              .anm file creates an actor with animation
                              .act file creates an actor without animation
                              .gpl file creates a display object using
                                   id 0 from the .gpl
    
    Returns:        ptr to newly allocated/initialized RefObj
*---------------------------------------------------------------------------*/
static RefObj* InitRefObj( u32 id, char* fileName )
{
    RefObj* pRefObj = NULL;
    char*   pExt;


    // create a zeroed-out RefObj
    pRefObj = (RefObj*)MemAlloc( sizeof(RefObj) );
    memset( pRefObj, 0, sizeof(RefObj) );

    pRefObj->refId = id;

    // check for an .anm file
    pExt = fileName + strlen( fileName ) - 4;

    // .anm file; create a corresponding .act file name
    if( ( (strcmp( pExt, ".anm")) == 0 ) || ( (strcmp( pExt, ".ANM")) == 0 ) )
    {
        pRefObj->type = OBJ_TYPE_ACTOR_ANIM;

        strcpy( pRefObj->anmName, fileName );

        strcpy( pRefObj->objName, fileName );
        pExt = pRefObj->objName + strlen( pRefObj->objName ) - 4;

        strcpy( pExt, ".act" );

        ANIMGet( &pRefObj->pAnmBank, pRefObj->anmName );
    }
    
    // .gpl or .act file - copy fileName to objName and set type
    else
    {
        if( ( (strcmp( pExt, ".act")) == 0 ) || ( (strcmp( pExt, ".ACT")) == 0 ) )
        {
            pRefObj->type = OBJ_TYPE_ACTOR;
        }

        if( ( (strcmp( pExt, ".gpl")) == 0 ) || ( (strcmp( pExt, ".GPL")) == 0 ) )
        {
            pRefObj->type = OBJ_TYPE_DISP_OBJ;
        }

        strcpy( pRefObj->objName, fileName );
    }

    // set initial 'base' orientation fields
    // ( final value = refObj->base + obj->world )
//    MTXIdentity( pRefObj->baseM );
    CTRLInit( &pRefObj->baseCtrl );

    // initialize ref Obj's object
    pRefObj->pObj = InstanceObj( pRefObj );

    // compute bounding radius after obj initialization
    pRefObj->baseRad = ComputeObjRad( pRefObj->pObj );


    return pRefObj;
}

/*---------------------------------------------------------------------------*
    Name:           InitRefObjList
    
    Description:    creates RefObjList from a list of filenames,
                    sets base srt values for each reference object.

                    1) grabs a hard coded model list from "gxDemos/"
                    2) searches "gxDemos/perfView/models/" and gets all files
                       located there.
                
    Arguments:      globals:  reads NumRefObj to set file list size.
                              reads RefObjName array for filenames.
                              writes RefObjList

    
    Returns:        1 on success; 0 on failure
*---------------------------------------------------------------------------*/
static u32 InitRefObjList( void )
{
    Node*   pNode;
    RefObj* pRefObj;                        
    u32     id;
    char*   namePtr;

    //------------------------------

    // hard coded file list:
    #define NUM_HARD_FILE                       1
    char*   hardDirName                      = "/gxDemos/";
    char    hardFileName[NUM_HARD_FILE][255] = { "SndShrew.anm" };

    //------------------------------

    // leave this here even if you delete hard-coded file variable and code
    RefObjList = NULL;

    //-----------------------------

    // hard-coded file load:  comment out this section to remove hard-coded models
    DVDChangeDir( hardDirName );
    for( id = 0; id < NUM_HARD_FILE; id++ )
    {
        namePtr = hardFileName[ id ];

        // set ids that wont conflict with 'model' dir ids ( 0 to (NumRefObj-1) )
        pRefObj = InitRefObj( ( NumRefObj + id), namePtr );

        pNode   = InitNode( &RefObjList, NULL );

        AttachData( pNode, NODE_TYPE_REF_OBJ, (void*)pRefObj );
    }

    //----------------------------

    // directory search:
    DVDChangeDir( ModelDirName );

    for( id = 0; id < (u32)NumRefObj; id++ )
    {
        namePtr = RefObjName[ id ];

        pRefObj = InitRefObj( id, namePtr );

        pNode   = InitNode( &RefObjList, NULL );

        AttachData( pNode, NODE_TYPE_REF_OBJ, (void*)pRefObj );
    }

    // if no hard coded files and no 'model dir' files,
    if( RefObjList == NULL )
    {
        OSReport( "error: no reference object files found\n" );
        return 0;
    }

    // initialize base orientation of all ref. objects
    InitRefObjBaseSRT( RefObjList );
    return 1;
}

/*---------------------------------------------------------------------------*
    Name:           InitRefObjBaseSRT
    
    Description:    set the base scale/rotate/translate values for
                    a list of reference objects.
                
    Arguments:      pRefObjList: ptr to head of RefObj list.  
                                
    Returns:        none
*---------------------------------------------------------------------------*/
static void InitRefObjBaseSRT( Node* pRefObjList )
{
    Node*      pNode   = NULL;
    Obj*       pObj    = NULL;
    RefObj*    pRefObj = NULL;
    f32        sqRad;

    if( pRefObjList == NULL )
        return;

    // set the basic object orientation
    pNode = pRefObjList;
    while( pNode )
    {
        pRefObj = (RefObj*)( pNode->data );

        // set the base scale so that all objects will fit comfortably
        // in a normalized square
        sqRad   = (f32)sqrtf(2.0f) / pRefObj->baseRad;

        CTRLInit( &pRefObj->baseCtrl );
        CTRLSetScale( &pRefObj->baseCtrl, sqRad, sqRad, sqRad );
        CTRLSetRotation( &pRefObj->baseCtrl, -90.0f, 0.0f, 0.0f );

        pNode = pNode->next;
    }
}

/*---------------------------------------------------------------------------*
    Name:           FreeRefObj
    
    Description:    free a RefObj's data and structure.
                
    Arguments:      ppRefObj: address of a RefObj* to be freed.
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void FreeRefObj( RefObj** ppRefObj )
{

    if( (*ppRefObj) == NULL )
        return;

    // free attached object
    if( (*ppRefObj)->pObj )
    {
        FreeObj( &(*ppRefObj)->pObj );
        (*ppRefObj)->pObj = NULL;
    }

    // free attached anim bank
    if( (*ppRefObj)->pAnmBank )
    {
        ANIMRelease( &(*ppRefObj)->pAnmBank );
        (*ppRefObj)->pAnmBank = NULL;
    }

    // set everything else to 0
    memset( *ppRefObj, 0, sizeof(RefObj) );

    OSFree( *ppRefObj );
    *ppRefObj = NULL;

}

/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*



                             OBJ_FUNCTIONS      



*----------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*
    Name:           InstanceObj
    
    Description:    allocate and Obj structure;
                    instance this Obj using pRefObj as a template
                    should call SetObjPos after calling this function in 
                    so that SetWorldControl is invoked.
                
    Arguments:      pRefObj: RefObj to use as template for instancing.

    Returns:        ptr to newly instanced Obj
*---------------------------------------------------------------------------*/
static Obj* InstanceObj( RefObj* pRefObj )
{
    Obj*          pNewObj = NULL;
    ACTActor*     pAct    = NULL;
    DODisplayObj* pDObj   = NULL;
    GEOPalette*   pGeoPal = NULL;


    // create an instance of an Obj
    pNewObj = (Obj*)MemAlloc( sizeof(Obj) );
    memset( pNewObj, 0, sizeof(Obj) );

    switch( pRefObj->type )
    {
    case OBJ_TYPE_DISP_OBJ:         // display object

        GEOGetPalette( &pGeoPal, pRefObj->objName );        
        DOGet( &( (DODisplayObj*)pNewObj->pDOA ), pGeoPal, 0, 0 );
        pNewObj->type     = OBJ_TYPE_DISP_OBJ;
        pNewObj->objId    = pRefObj->refId;
        pNewObj->pRefObj  = (void*)pRefObj;  
        pNewObj->hasAnm   = ANIM_TYPE_NONE;

        pNewObj->row = 0;
        pNewObj->col = 0;

        break;

    case OBJ_TYPE_ACTOR:            // actor

        ACTGet( &( (ACTActor*)pNewObj->pDOA ), pRefObj->objName );
        pNewObj->type     = OBJ_TYPE_ACTOR;
        pNewObj->objId    = pRefObj->refId;
        pNewObj->pRefObj  = (void*)pRefObj;
        pNewObj->hasAnm   = ANIM_TYPE_NONE;

        pNewObj->row = 0;
        pNewObj->col = 0;

        break;

    case OBJ_TYPE_ACTOR_ANIM:       // animatable actor

        ACTGet( &( (ACTActor*)pNewObj->pDOA ), pRefObj->objName );
        pNewObj->type     = OBJ_TYPE_ACTOR_ANIM;
        pNewObj->objId    = pRefObj->refId;
        pNewObj->pRefObj  = (void*)pRefObj;
        pNewObj->hasAnm   = ANIM_TYPE_ANIM_BANK;

        pNewObj->row = 0;
        pNewObj->col = 0;

        ACTSetAnimation( (ACTActor*)pNewObj->pDOA, pRefObj->pAnmBank, 0, 0, 0.0f );
        break;

    } // end switch

    // set initial 'world' orientation fields:

    // scale to the size of one grid cell
    CTRLSetScale( &pNewObj->worldCtrl, GridScale, GridScale, GridScale );

    // compute the translation and scale world matrix for the model
    //SetWorldControl( pNewObj );

    // set bounding radius
    pNewObj->worldRad = pRefObj->baseRad;


    return pNewObj;
}

/*---------------------------------------------------------------------------*
    Name:           ComputeObjRad
    
    Description:    compute the bounding radius of an Obj;
                    this uses the geometry contained in pObj->DOA.

                    if display object, use the object's untransformed geometry.
                    if actor, the actor's subobjects are first transformed to 
                    their model-relative positions.  The bounding radius is
                    computed from the 'assembled' actor.
                
    Arguments:      pObj: ptr to Obj whose radius is computed
    
    Returns:        radius of pObj
*---------------------------------------------------------------------------*/
static f32 ComputeObjRad( Obj* pObj )
{
    ACTActor*      pAct  = NULL;
    DODisplayObj*  pDObj = NULL;
    ACTBonePtr     pBone = NULL; 
    f32            rMax = 0.0f;
    f32            rTmp = 0.0f;


    switch( pObj->type )
    {
    case OBJ_TYPE_DISP_OBJ:             // display object

        pDObj = (DODisplayObj*)pObj->pDOA;
        rMax  = ComputeDObjRad( pDObj );
        break;

    case OBJ_TYPE_ACTOR:                // actor or actor_anim
    case OBJ_TYPE_ACTOR_ANIM:           // fall through

        pAct = (ACTActor*)pObj->pDOA;

        rTmp = 0.0f;
        rMax = 0.0f;

        // skinned actor = single display object
        if( pAct->skinObject )
        {
            pDObj = pAct->skinObject;
            rTmp  = ComputeDObjRad( pDObj );

            if( rMax < rTmp )
                rMax = rTmp;
        }

        // unskinned portions of actor
        // traverse the actor hierarchy and save the longest overall dimensions
        pBone = (ACTBonePtr)( pAct->drawPriorityList.Head );
        while( pBone )
        {
            pDObj = pBone->dispObj;
            rTmp  = ComputeDObjRad( pDObj );

            if( rMax < rTmp )
                rMax = rTmp;

            pBone = (ACTBonePtr)( pBone->drawPriorityLink.Next );

        }   // end while( pBone )

        break;

    } // end switch

    return rMax;
}

/*---------------------------------------------------------------------------*
    Name:           SetObjPos
    
    Description:    set an Obj's world position
                
    Arguments:      pObj: ptr to Obj
                    pPos: new world position
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void SetObjPos( Obj* pObj, Point3d* pPos )
{

    if( pPos && pObj && (pObj->type != OBJ_TYPE_NONE) )
    {        
        CTRLSetTranslation( &pObj->worldCtrl, pPos->x, pPos->y, pPos->z );

        // compute and set the srt matrix for the object
        SetWorldControl( pObj );
    }
}

/*---------------------------------------------------------------------------*
    Name:           DrawObj
    
    Description:    draw an individual Obj
                
    Arguments:      pObj: ptr to Obj to be drawn.
                       v: camera viewing matrix
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void DrawObj( Obj* pObj, Mtx v )
{
    ACTActor*     pAct;
    DODisplayObj* pDObj;
    RefObj*       pRefObj;

    if( !pObj || pObj->type == OBJ_TYPE_NONE )
        return;

    // find the instanced object's ref. object
    pRefObj = (RefObj*)( pObj->pRefObj );

    switch( pObj->type )
    {
    case OBJ_TYPE_DISP_OBJ:             // display object

        pDObj = (DODisplayObj*)(pObj->pDOA);
        DORender( pDObj, v, 1, light1 );
        break;

    case OBJ_TYPE_ACTOR:                // actor

        pAct = (ACTActor*)(pObj->pDOA);
        ACTRender( pAct, v, 1, light1 ); 
        break;

    case OBJ_TYPE_ACTOR_ANIM:           // actor with animation

        pAct = (ACTActor*)(pObj->pDOA);
        ACTBuildMatrices( pAct );       // needed to find animation
        ACTRender( pAct, v, 1, light1 ); 
        break;

    } // end switch
}

/*---------------------------------------------------------------------------*
    Name:           FreeObj
    
    Description:    free an Obj's data and structure.
                
    Arguments:      ppObj: address of an Obj* to be freed.
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void FreeObj( Obj** ppObj )
{
    Obj* pObj;


    if( (*ppObj) == NULL )
        return;

    // free display object/actor data
    pObj = *ppObj;
    switch( pObj->type )
    {
    case OBJ_TYPE_DISP_OBJ:

        DORelease( &(DODisplayObj*)(pObj->pDOA) );
        break;

    case OBJ_TYPE_ACTOR:
    case OBJ_TYPE_ACTOR_ANIM:

        ACTRelease( &(ACTActor*)(pObj->pDOA) );
        break;

    } // end switch

    // set all other fields to 0
    memset( pObj, 0, sizeof(Obj) );

    // free the object structure
    OSFree( *ppObj );
    *ppObj = NULL;
}

/*---------------------------------------------------------------------------*
    Name:           UpdateInstObj
    
    Description:    whenever number of grid segments changes, object positions must
                    be recomputed so that objects remain centered in grid squares.
                    in addition, when number of grid segments decreases, some instanced
                    objects may lie outside the re-sized grid.
                    this function also deletes those objects. 
                
    Arguments:      globals:  writes InstObjList
                              reads GridSegs
                              reads GridScale
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void UpdateInstObj( void )
{
    Obj*       pObj       = NULL;
    Node*      pObjNode   = NULL;
    u32        row, col;


    // delete any objects that lie outside the resized grid
    pObjNode = InstObjList;
    while( pObjNode )
    {
        pObj = (Obj*)pObjNode->data;

        if( (pObj->row >= GridSegs) || (pObj->col >= GridSegs) )
        {
            FreeNode( &InstObjList, &pObjNode );
            pObjNode = InstObjList;
            continue;
        }

        pObjNode = pObjNode->next;
    }


    // recompute all remaining object positions, scales
    pObjNode = InstObjList;
    while( pObjNode )
    {
        pObj = (Obj*)pObjNode->data;

        row = pObj->row;
        col = pObj->col;

        CTRLSetTranslation( &pObj->worldCtrl, 
                            ( GridScale * 0.5f ) + ( GridScale * col ) + GRID_OFFSET_H,
                            0,
                            ( GridScale * 0.5f ) + ( GridScale * row ) + GRID_OFFSET_H );

        CTRLSetScale( &pObj->worldCtrl, GridScale, GridScale, GridScale );

        // compute and set the srt matrix for the object
        SetWorldControl( pObj );

        pObjNode = pObjNode->next;
    }
}

/*---------------------------------------------------------------------------*
    Name:           SetWorldControl
    
    Description:    Calculates the world control matrix for the given object.
                    Should be called whenever the control (translation,
                    rotation, scale, or matrix) is changed for an object.
                
    Arguments:      pObj: instanced object
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void SetWorldControl( Obj *pObj )
{
    ACTActor*     pAct;
    DODisplayObj* pDObj;
    CTRLControl*  pCtrl;
    RefObj*       pRefObj;
    Mtx           mBase, mF;     

    if( !pObj || pObj->type == OBJ_TYPE_NONE )
        return;

    // find the instanced object's ref. object
    pRefObj = (RefObj*)( pObj->pRefObj );

    // compute the final matrix for this object (final = base + world )
    CTRLBuildMatrix( &pRefObj->baseCtrl, mBase );
    CTRLBuildMatrix( &pObj->worldCtrl, mF );
    MTXConcat( mF, mBase, mF );

    switch( pObj->type )
    {
    case OBJ_TYPE_DISP_OBJ:             // display object

        pDObj = (DODisplayObj*)(pObj->pDOA);
        DOSetWorldMatrix( pDObj, mF );
        break;

    case OBJ_TYPE_ACTOR:                // actor

        pAct = (ACTActor*)(pObj->pDOA);

        pCtrl = ACTGetControl( pAct );
        CTRLSetMatrix( pCtrl, mF );

        ACTBuildMatrices( pAct );
        break;

    case OBJ_TYPE_ACTOR_ANIM:           // actor with animation

        pAct = (ACTActor*)(pObj->pDOA);

        pCtrl = ACTGetControl( pAct );
        CTRLSetMatrix( pCtrl, mF );

        // Actor matrices will be built at runtime
        break;

    } // end switch
}

/*---------------------------------------------------------------------------*



                             FLASHER_FUNCTIONS      



*----------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*
    Name:           InitFlasher
    
    Description:    initialize the cursor; set grid position to (0,0), 
                    mode to CSR_MODE_TILE,
                    animation frame to 0;
                    scale initial vtx positions to match grid cell size.
                
    Arguments:      globals:  writes Csr    
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void InitFlasher()
{
    u32 i;
                                                      // pre-scaled vertices; unit square at origin
    f32 vtx_f32[12] = 
    {
    //         x           y            z       
            0.5f,       0.0f,        0.5f,            // 0
           -0.5f,       0.0f,        0.5f,            // 1
           -0.5f,       0.0f,       -0.5f,            // 2
            0.5f,       0.0f,       -0.5f             // 3  
    };


    for( i = 0; i < 12; i++ )
        Csr.vtx_f32[i] = vtx_f32[i];

    Csr.mode  = FLASHER_MODE_TILE;

    Csr.gridX = 0;
    Csr.gridZ = 0;

    Csr.frame = 0;

    SetFlasherPos();  // set world position 'pos' field from gridX, gridZ fields
    PrintMenu();      // draw the initial new print-window menu for button actions.

}

/*---------------------------------------------------------------------------*
    Name:           UpdateFlasher
    
    Description:    updates cursor mode, animation frame, position
                    based on 'PadAction' states.
                
    Arguments:      globals:  reads  PadAction
                              writes Csr
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void UpdateFlasher( void )
{

    if( PadAction.csrMode == 1 )
    {
        switch( Csr.mode )
        {
        case FLASHER_MODE_TILE:
            Csr.mode = FLASHER_MODE_CAM;
            break;

        case FLASHER_MODE_CAM:
            Csr.mode = FLASHER_MODE_UTIL;
            break;

        case FLASHER_MODE_UTIL:
            Csr.mode = FLASHER_MODE_PERF;
            break;

        case FLASHER_MODE_PERF:
            Csr.mode = FLASHER_MODE_TILE;
            break;
        }

        PrintMenu();    // update the print window button-action menu whenever mode changes
    }

    // update frame for color cycling
    Csr.frame ++;
    if( Csr.frame >= 60 )
        Csr.frame = 0;

    // move the cursor one square at a time
    if( PadAction.csrX == -1 )
    {
        if( Csr.gridX >= 1 )
            Csr.gridX --;
    }
    else if( PadAction.csrX == 1 )
    {
        if( (Csr.gridX + 1) < GridSegs )
            Csr.gridX ++;
    }

    if( PadAction.csrZ == -1 )
    {
        if( Csr.gridZ >= 1 )
            Csr.gridZ --;
    }
    else if( PadAction.csrZ == 1 )
    {
        if( (Csr.gridZ + 1) < GridSegs )
            Csr.gridZ ++;
    }

    // update cursor position based on grid location
    SetFlasherPos();
}

/*---------------------------------------------------------------------------*
    Name:           SetFlasherPos
    
    Description:    set Csr's world position based on its gridX, gridZ fields
                
    Arguments:      globals:  reads  GridScale
                              writes Csr    
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void SetFlasherPos()
{
    f32 halfWid;

    halfWid   = (GridScale * 0.5f);

    Csr.pos.x = halfWid + (Csr.gridX * GridScale) + GRID_OFFSET_H;
    Csr.pos.y = 0.0f + GRID_OFFSET_V;
    Csr.pos.z = halfWid + (Csr.gridZ * GridScale) + GRID_OFFSET_H;
}

/*---------------------------------------------------------------------------*
    Name:           DrawFlasher
    
    Description:    draws Csr as a solid-colored color-cycled quad;
                    note: changes, then restores gx state.
                
    Arguments:      globals:  reads Csr
                              reads GridScale
                              reads Camera
                              reads ViewMtx 
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void DrawFlasher( void )
{
    CTRLControl ctrl;
    Mtx         m;

    GXColor csrClr = {  0,     0,    0,   255  };

    // cheap cursor color cycling
    // map colors to a sine curve - 2PI rad over n frames
    static u8 clrArray[60] = {  52,  59,  66,  73,  80,
                                87,  94, 101, 108, 115,
                               122, 129, 136, 143, 150,
                               157, 164, 171, 178, 185,
                               192, 199, 206, 213, 220, 
                               227, 234, 241, 248, 255,

                               248, 241, 234, 227, 220,
                               213, 206, 199, 192, 185,
                               178, 171, 164, 157, 150,
                               143, 136, 129, 122, 115,
                               108, 101,  94,  87,  80,
                                73,  66,  59,  52,  45  };


    // cursor colors:
    // green  in FLASHER_MODE_TILE,
    // yellow in FLASHER_MODE_CAM
    // blue   in FLASHER_MODE_UTIL
    // white  in FLASHER_MODE_UTIL
    switch( Csr.mode )
    {
      case FLASHER_MODE_TILE:
        csrClr.g = clrArray[ Csr.frame ];
        break;

      case FLASHER_MODE_CAM:
        csrClr.r = clrArray[ Csr.frame ];
        csrClr.g = clrArray[ Csr.frame ];
        break;

      case FLASHER_MODE_UTIL:
        csrClr.b = clrArray[ Csr.frame ];
        break;

      case FLASHER_MODE_PERF:
        csrClr.r = clrArray[ Csr.frame ];
        csrClr.g = clrArray[ Csr.frame ];
        csrClr.b = clrArray[ Csr.frame ];
        break;
    }

    // draw the flasher
    GXClearVtxDesc();

    GXSetChanMatColor( GX_COLOR0, csrClr );

    // disable lighting- use register color only
    GXSetNumChans( 1 );
    GXSetChanCtrl( GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE );

    GXSetTevOrder( GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0 );
    GXSetTevOp( GX_TEVSTAGE0, GX_PASSCLR );
    GXSetNumTexGens( 0 );
    GXSetNumTevStages( 1 );

    // Set current vertex descriptor to enable position and color0.
    // Both use 8b index to access their data arrays.
    //GXSetVtxDesc(GX_VA_POS,GX_INDEX8);
    GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
            
    // Position has 3 elements (x,y,z), each of type f32,
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
       
    // cursor size, offset changes depending on mode
    CTRLInit( &ctrl );
    switch( Csr.mode )
    {
      case FLASHER_MODE_TILE:

        CTRLSetScale( &ctrl, GridScale, GridScale, GridScale );
        CTRLSetTranslation( &ctrl, Csr.pos.x, Csr.pos.y, Csr.pos.z );
        break;

      case FLASHER_MODE_CAM:  // cursor is smaller in cam mode
      case FLASHER_MODE_UTIL:
      case FLASHER_MODE_PERF:

        CTRLSetScale( &ctrl, GridScale * 0.5f, GridScale, GridScale * 0.5f );

        // cursor 'pos' is replaced by camera target location
        CTRLSetTranslation( &ctrl, Camera.at.x, Camera.at.y, Camera.at.z );
        break;
    }

    // make the modelview matrix
    CTRLBuildMatrix( &ctrl, m );
    MTXConcat( ViewMtx, m,  m );

    GXLoadPosMtxImm( m, GX_PNMTX0 );
        
    GXBegin( GX_QUADS, GX_VTXFMT0, 4 );

        GXPosition3f32(Csr.vtx_f32[0], Csr.vtx_f32[1], Csr.vtx_f32[2]);
        GXPosition3f32(Csr.vtx_f32[3], Csr.vtx_f32[4], Csr.vtx_f32[5]);
        GXPosition3f32(Csr.vtx_f32[6], Csr.vtx_f32[7], Csr.vtx_f32[8]);
        GXPosition3f32(Csr.vtx_f32[9], Csr.vtx_f32[10], Csr.vtx_f32[11]);
                   
    GXEnd(); 

}   // end fn

/*---------------------------------------------------------------------------*
    Name:           SelectSquare
    
    Description:    performs an action on a square selected by Csr;
                    action taken depends on Csr.mode and PadAction state:
                
    Arguments:      globals:  reads  GridScale
                              writes InstObjList
                              reads  PadAction
                              reads  Csr
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void SelectSquare()
{
    Obj*    pObj      = NULL;
    RefObj* pRefObj   = NULL;
    Node*   pObjNode  = NULL;
    Node*   pRefNode  = NULL;


    // determine whether the cursor is under an object
    pObj = CollFlasherToObj( InstObjList );

    // cycle forward through object list
    if( PadAction.objSelect == 1 )
    {
        // empty square - instance the 1st RefObj
        if( pObj == NULL )
        {
            pRefObj  = (RefObj*)( RefObjList->data );
            pObj     = InstanceObj( pRefObj );

            SetObjPos( pObj, &Csr.pos );  // set object to cursor location
            pObj->col = Csr.gridX;
            pObj->row = Csr.gridZ;

            pObjNode = InitNode( &InstObjList, NULL );
            AttachData( pObjNode, NODE_TYPE_OBJ, (void*)pObj );
        }

        // occupied square- delete the current instanced object and
        // instance the next object from RefObjList
        else
        {
            pRefObj = (RefObj*)( pObj->pRefObj );

            pRefNode = FindNodeByData( RefObjList,  (void*)pRefObj );
            pObjNode = FindNodeByData( InstObjList, (void*)pObj    );
            FreeNode( &InstObjList, &pObjNode );

            // instance the next RefObj unless we were at the tail;
            // in that case, leave an empty square
            if( pRefNode->next )
            {
                pRefNode = pRefNode->next;
                pObj     = InstanceObj( (RefObj*)(pRefNode->data) );

                SetObjPos( pObj, &Csr.pos ); 
                pObj->col = Csr.gridX;
                pObj->row = Csr.gridZ;

                pObjNode = InitNode( &InstObjList, NULL );
                AttachData( pObjNode, NODE_TYPE_OBJ, (void*)pObj );
            }                                                   
        }   
    }

    // cycle backwards through object list
    else if( PadAction.objSelect == -1 )
    {
        // empty square - instance the last RefObj
        if( pObj == NULL )
        {
            pRefNode = RefObjList;
            while( pRefNode->next )
                pRefNode = pRefNode->next;

            pRefObj  = (RefObj*)( pRefNode->data );
            pObj     = InstanceObj( pRefObj );

            SetObjPos( pObj, &Csr.pos );  // set object to cursor location
            pObj->col = Csr.gridX;
            pObj->row = Csr.gridZ;

            pObjNode = InitNode( &InstObjList, NULL );
            AttachData( pObjNode, NODE_TYPE_OBJ, (void*)pObj );
        }

        // occupied square- delete the current instanced object and
        // instance the next object from RefObjList
        else
        {
            pRefObj = (RefObj*)( pObj->pRefObj );

            pRefNode = FindNodeByData( RefObjList,  (void*)pRefObj );
            pObjNode = FindNodeByData( InstObjList, (void*)pObj    );
            FreeNode( &InstObjList, &pObjNode );

            // instance the previous RefObj unless we were at the head;
            // in that case, leave an empty square
            if( pRefNode->prev )
            {
                pRefNode = pRefNode->prev;
                pObj     = InstanceObj( (RefObj*)(pRefNode->data) );

                SetObjPos( pObj, &Csr.pos ); 
                pObj->col = Csr.gridX;
                pObj->row = Csr.gridZ;

                pObjNode = InitNode( &InstObjList, NULL );
                AttachData( pObjNode, NODE_TYPE_OBJ, (void*)pObj );
            }                                                   
        }   
    }

    // cycle forward through background list
    if( PadAction.bkgdSelect == 1 )
    {
        if( BkgdObj )
        {
            pRefObj  = (RefObj*)( BkgdObj->pRefObj );
            pRefNode = FindNodeByData( BkgdObjList,  (void*)pRefObj );

            if( pRefNode->next )
            {
                pRefNode = pRefNode->next;
                pRefObj = (RefObj*)pRefNode->data;
                BkgdObj =  pRefObj->pObj;
            }
            else
            {
                BkgdObj = NULL;
            }
        }
        else if( NumBkgdObj > 0 )
        {
            // point to 1st background
            pRefNode = BkgdObjList;
            pRefObj = (RefObj*)pRefNode->data;
            BkgdObj = pRefObj->pObj;        
        }
    }
}

/*---------------------------------------------------------------------------*
    Name:           CollFlasherToObj
    
    Description:    search InstObjList and determine if Csr is colliding with
                    any of the instanced objects in the xz plane.

                    this is a 2D collision in the xz plane between the cursor 
                    rectangle and a bounding rect created from the object's 
                    position and bounding radius
                
    Arguments:      pObjList: head of instanced object list 

                    globals:  reads Csr
    
    Returns:        ptr to the first Obj colliding with Csr;
                    NULL if no collision detected.
*---------------------------------------------------------------------------*/
static Obj* CollFlasherToObj( Node* pObjList )
{
    Obj*  pObj     = NULL;
    Node* pNode    = NULL;
    Rect2 csrRect2;
    Rect2 objRect2;
    f32   halfWid, csrWid;
    Vec   translation;

    if( pObjList == NULL )
        return NULL;

    // convert cursor grid coords to a bounding rect.
    // make smaller than a full square for collision purposes
    halfWid = GridScale * 0.5f;
    csrWid  = halfWid - (halfWid * 0.1f);

    csrRect2.left   =  Csr.pos.x - csrWid;
    csrRect2.right  =  Csr.pos.x + csrWid;

    csrRect2.top    =  Csr.pos.z - csrWid;
    csrRect2.bottom =  Csr.pos.z + csrWid;


    pNode = pObjList;
    while( pNode )
    {

        if( pNode->type != NODE_TYPE_OBJ )
        {
            pNode = pNode->next;
            continue;
        }

        pObj  = (Obj*)pNode->data;

        if( pObj && (pObj->type != OBJ_TYPE_NONE) )
        {
            // compute a bounding rect from Obj position and BaseRefRad
            halfWid = GridScale * 0.5f;

            CTRLGetTranslation( &pObj->worldCtrl, &translation.x, &translation.y, &translation.z );
            objRect2.left   =  translation.x - halfWid;
            objRect2.right  =  translation.x + halfWid;

            objRect2.top    =  translation.z - halfWid;
            objRect2.bottom =  translation.z + halfWid;

            // report the first collision found
            if( (Rect2InRect2( &csrRect2, &objRect2 )) == 1 )
            {
                return pObj;
            }
        }

        pNode = pNode->next;
    }

    return NULL;
}

/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*



                              GRID_FUNCTIONS      



*----------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*
    Name:           InitGrid
    
    Description:    create an initial instanced object in grid location (0,0)
                    
    Arguments:      globals:  reads  RefObjList
                              writes InstObjList
                              reads  Csr
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void InitGrid()
{
    /*
    RefObj* pRefObj = NULL;
    RefObj* pDefObj = NULL;
    Obj*    pObj;
    Node*   pNode;
    char*   defName = "10000.gpl";
    u32     i, j;



    pNode = RefObjList;
    while( pNode )
    {
        pRefObj = (RefObj*)( pNode->data );

        if( (strcmp( defName, pRefObj->objName )) == 0 )
        {
            pDefObj = pRefObj;
            break;
        }

        pNode = pNode->next;
    }

    // couldn't find the 'defName' file
    if( pDefObj == NULL )
        return;


    // create initial instanced object(s)

    InstObjList = NULL;

    // place objects in a checkerboard pattern
    for( i = 0; i < DEF_GRID_SEGS; i++ )
    {
        for( j = 0; j < DEF_GRID_SEGS; j++ )
        {
            if( (i%2) == 0 ) // even row
            {
                if( (j%2) == 0 ) // even col
                {
                    Csr.gridX = j;
                    Csr.gridZ = i;
                    SetFlasherPos();

                    pObj = InstanceObj( pRefObj );

                    SetObjPos( pObj, &Csr.pos );
 
                    pNode       = InitNode( &InstObjList, NULL );
                    AttachData( pNode, NODE_TYPE_OBJ, (void*)pObj );
                }
            }
            else             // odd row
            {
                if( (j%2) != 0 ) // odd col
                {
                    Csr.gridX = j;
                    Csr.gridZ = i;
                    SetFlasherPos();

                    pObj = InstanceObj( pRefObj );

                    SetObjPos( pObj, &Csr.pos );
 
                    pNode       = InitNode( &InstObjList, NULL );
                    AttachData( pNode, NODE_TYPE_OBJ, (void*)pObj ); 
                }
            }
        }
    }

    // reset cursor position
    Csr.gridX = 0;
    Csr.gridZ = 0;
    SetFlasherPos();

  */
}

/*---------------------------------------------------------------------------*
    Name:           ScaleGrid
    
    Description:    scales grid up/down depending on PadAction.gridScale
                    total scale is constrained by #defined max, min values.
                    updates instanced object positions, cursor position.
                    
    Arguments:      globals: reads PadAction
                             writes GridSegs
                             writes GridScale
                             writes Csr
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void ScaleGrid()
{
    u32 change = 0;


    if( PadAction.gridScale == 0 )
        return;

    else if( PadAction.gridScale == -1 )
    {
        if( GridSegs > MIN_GRID_SEGS )
        {
            GridSegs --;
            GridScale = 1.0f / GridSegs;
            change = 1;

        }
    }
    else if( PadAction.gridScale == 1 )
    {
        if( GridSegs < MAX_GRID_SEGS )
        {
            GridSegs ++;
            GridScale = 1.0f / GridSegs;
            change = 1;
        }
   }

    if( change )
    {
        UpdateInstObj();

        if( Csr.gridX >= GridSegs )
            Csr.gridX = (GridSegs - 1);

        if( Csr.gridZ >= GridSegs )
            Csr.gridZ = (GridSegs - 1);

        SetFlasherPos(); 
    }

}

/*---------------------------------------------------------------------------*
    Name:           DrawGrid
    
    Description:    draw the grid as a set of GX_LINES
                    
    Arguments:      globals: reads GridSegs
                             reads GridScale
    
    Returns:        none
 *---------------------------------------------------------------------------*/
static void DrawGrid( void )
{
    u32     segs,  i;
    f32     step;
    GXColor lineClr =     {    0,  255,    0,  255  };

    f32     xLine[]     = { 0.0f, 0.0f, 0.0f,
                            1.0f, 0.0f, 0.0f };

    f32     zLine[]     = { 0.0f, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f };


    step = GridScale;
    segs = GridSegs + 1;
 
    xLine[0] += GRID_OFFSET_H;
    xLine[1] += GRID_OFFSET_V;
    xLine[2] += GRID_OFFSET_H;
    xLine[3] += GRID_OFFSET_H;
    xLine[4] += GRID_OFFSET_V;
    xLine[5] += GRID_OFFSET_H;

    zLine[0] += GRID_OFFSET_H;
    zLine[1] += GRID_OFFSET_V;
    zLine[2] += GRID_OFFSET_H;
    zLine[3] += GRID_OFFSET_H;
    zLine[4] += GRID_OFFSET_V;
    zLine[5] += GRID_OFFSET_H;

    // disable lighting - use register color only
    GXSetNumChans( 1 );
    GXSetChanMatColor( GX_COLOR0, lineClr );
    GXSetChanCtrl( GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE );

    // pass the material color
    GXSetTevOrder( GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0 );
    GXSetTevOp( GX_TEVSTAGE0, GX_PASSCLR );
    GXSetNumTexGens( 0 );
    GXSetNumTevStages( 1 );

    // Set current vertex descriptor to enable position and color0.
    // Both use 8b index to access their data arrays.
    GXClearVtxDesc();
    //GXSetVtxDesc(GX_VA_POS,  GX_INDEX8);
    GXSetVtxDesc( GX_VA_POS, GX_DIRECT );
            
    // Position has 3 elements (x,y,z), each of type f32,
    GXSetVtxAttrFmt( GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0 );
       
    GXLoadPosMtxImm( ViewMtx, GX_PNMTX0 );

    GXSetLineWidth( (1*6), GX_TO_ZERO );

    // stride = 3 elements (x,y,z) each of type f32
    for( i=0; i< segs; i++ )
    {
        GXBegin( GX_LINES, GX_VTXFMT0, 2 );

            GXPosition3f32(xLine[0], xLine[1], xLine[2]);
            GXPosition3f32(xLine[3], xLine[4], xLine[5]);

        GXEnd(); 

        xLine[2] += step;
        xLine[5] += step;
    }

    for( i=0; i< segs; i++ )
    {
        GXBegin( GX_LINES, GX_VTXFMT0, 2 );

            GXPosition3f32(zLine[0], zLine[1], zLine[2]);
            GXPosition3f32(zLine[3], zLine[4], zLine[5]);

        GXEnd(); 

        zLine[0] += step;
        zLine[3] += step;
    }

}   // end fn

/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*



                              CAMERA_FUNCTIONS      



*----------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*
    Name:           InitCamera
    
    Description:    set the camera position so that the grid will fit 
                    the viewport extents.
                    reset the N and F clipping planes so that the world is visible.
                    reset the projection matrix.

                    note: RefObj list must be loaded and bounding rads computed 
                          before calling this function.  
                    
    Arguments:      globals:  reads  GridScale
                              writes ResetCam
                              writes Camera
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void InitCamera ( void )
{
    Mtx44  mProj;
    f32    aspect;           // wid/hgt ratio of view frustum
    f32    fovXDeg;          // field of view in X in degrees
    f32    N, F;             // distance to near and far clipping planes

    f32 gridRadius;          // radius of bounding sphere around grid
                             // note: grid is a unit square

    f32    camDegX, camDegY; // camera's rotation about x and y axes
    Vec    camPos;           // camera position
    f32    camT;             // camera tether length
    Mtx    mTmp, mF;         // transformation matrices for camera position


    //---------------------------

    // projection matrix

    aspect  = (640.0f / 480.0f);     // viewport (width:height) aspect ratio
    fovXDeg =   25.0f;               // 1/2 field of view in X in degreees
    N       =    0.001f;             // hand-tweaked values for our backgrounds
    F       =   17.0f;

    // projection matrix - convert fovX to fovY
    MTXPerspective ( mProj, (fovXDeg * 2.0f / aspect), aspect, N, F ); 
    GXSetProjection( mProj, GX_PERSPECTIVE );

    //---------------------------

    // camera placement

    camDegX  =  15.0f;    // camera's ccw rotation about x axis
    camDegY  =  45.0f;    // camera's ccw rotation about y axis

    // radius of bounding sphere around (unit square) grid
    gridRadius = sqrtf( (0.5f * 0.5f * 2.0f) );

    // pre-rotated camera position along z
    camPos.x = 0.0f;
    camPos.y = 0.0f;
    camPos.z = gridRadius + 1.0f; // hand-tweaked

    camT = camPos.z;              // tether length

    // camera rotations
    MTXIdentity( mF );
    MTXRotDeg( mTmp, 'Y', 225.0f );
    MTXConcat( mTmp, mF, mF );
    MTXRotDeg( mTmp, 'X',  15.0f );
    MTXConcat( mTmp, mF, mF );

    MTXMultVec( mF, &camPos, &camPos );


    ResetCam.rotX      = camDegX;     
    ResetCam.rotY      = camDegY;    
    ResetCam.tether    = camT;
    
    ResetCam.tetherMin = 0.01f;               // hand-tweaked
    ResetCam.tetherMax = camT * 4.0f;
                                              // tether shorten-lengthen speed ( zoom )
    ResetCam.tetherSpeed = (ResetCam.tetherMax - ResetCam.tetherMin) / 45.0f;   

    ResetCam.rotSpeedX = 2.0f;                // left-right angular rotation speed ( deg/frame )
    ResetCam.rotSpeedY = 2.0f;                // up-down angular speed ( deg/frame )


    ResetCam.at.x =  0.0f;                    // look at origin   
    ResetCam.at.y =  0.0f;
    ResetCam.at.z =  0.0f;

    ResetCam.pos.x = camPos.x;
    ResetCam.pos.y = camPos.y;
    ResetCam.pos.z = camPos.z;   

    ResetCam.up.x = 0.0f;                     // camera doesn't roll
    ResetCam.up.y = 1.0f;
    ResetCam.up.z = 0.0f;

    //---------------------------

    // initialize the camera
    ResetCamera();
}

/*---------------------------------------------------------------------------*
    Name:           ResetCamera
    
    Description:    set the camera position so that the grid will fit 
                    the viewport extents.
                    reset the N and F clipping planes so that the world is visible.
                    reset the projection matrix.

                    note: RefObj list must be loaded and bounding rads computed 
                          before calling this function.  
                    
    Arguments:      globals:  reads  ResetCam
                              writes Camera
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void ResetCamera( void )
{
    // re-initialize the camera
    memcpy( &Camera, &ResetCam, sizeof(TCam) );
}

/*---------------------------------------------------------------------------*
    Name:           CameraTick
    
    Description:    update the camera's elevation, tether and rotation
                    based on 'PadAction' state.
                
    Arguments:      globals:  reads  PadAction
                              writes Camera
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void CameraTick ( void )
{

    // elevation
    if( PadAction.camRotX == -1 )
    {
        if( (Camera.rotX - Camera.rotSpeedX) >= 1.0f )
            Camera.rotX -= Camera.rotSpeedX;
    }
    else if( PadAction.camRotX == 1 )
    {
        if( (Camera.rotX + Camera.rotSpeedX) <= 89.0f )
            Camera.rotX  += Camera.rotSpeedX;
    }

    // horizontal rotation
    if( PadAction.camRotY == - 1 )
    {
        Camera.rotY -= Camera.rotSpeedY;
        if( Camera.rotY <= -360.0f )
            Camera.rotY += 360.0f;
    }
    else if( PadAction.camRotY == 1 )
    {
        Camera.rotY += Camera.rotSpeedY;
        if( Camera.rotY >= 360.0f )
            Camera.rotY -= 360.0f;
    }

    // camera zoom
    if( PadAction.camZoom == -1 )
    {
        if( (Camera.tether + Camera.tetherSpeed) <= Camera.tetherMax )
            Camera.tether += Camera.tetherSpeed;
    }
    else if( PadAction.camZoom == 1 )
    {
        if( (Camera.tether - Camera.tetherSpeed) >= Camera.tetherMin )
            Camera.tether -= Camera.tetherSpeed;
    }

    // 'free' aiming of camera in xz plane
    AimCamera( PadAction.targetTransX, PadAction.targetTransZ );

    // update the camera's position, orientation and ViewMtx 
    UpdateCamera();
}

/*---------------------------------------------------------------------------*
    Name:           UpdateCamera
    
    Description:    set the camera's position and ViewMtx from camera
                    elevation and rotation parameters.

                    modify camera's 'apparent' position by GridScale
            
    Arguments:      globals:  writes Camera
                              reads ViewMtx
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void UpdateCamera( void )
{
    f32     radX  = Camera.rotX * ( PI / 180.0f );
    f32     radY  = Camera.rotY * ( PI / 180.0f );
    f32     X,Y,Z;
    f32     T, Ts;  


    // CCW rotation from y axis ( y = R sin0 )
    T = Camera.tether;

    Y = T * (f32)sin( (double)radX );

    // horizontal rotation CCW from z axis.
    // use projected 'shadow' of Tether
    Ts = T * (f32)cos( (double)radX );

    X = Ts * (f32)sin( (double)radY );  
    Z = Ts * (f32)cos( (double)radY ); 


    Camera.pos.x =  Camera.at.x + X;
    Camera.pos.y =  Camera.at.y + Y;
    Camera.pos.z =  Camera.at.z + Z;            

    Camera.up.x = 0.0f;                 // camera doesn't roll
    Camera.up.y = 1.0f;
    Camera.up.z = 0.0f;

    // regenerate camera viewing matrix
    MTXLookAt( ViewMtx, &Camera.pos, &Camera.up, &Camera.at );
}

/*---------------------------------------------------------------------------*
    Name:           AimCamera
    
    Description:    change the camera's target in the xz plane.
                    confine the aimpoint within the boundaries of the grid

                    note: RefObj list must be loaded and bounding rads computed 
                          before calling this function.  
                    
    Arguments:      xDir:    move aimpt left and right relative to current camera vector.
                             -1 = move aimpt. 'left'
                             +1 = move aimpt. 'right'
                              0 = no movement

                    zDir:    move aimpt. in z relative to current camera vector.
                             -1 = move aimpt. backward along current camera vector
                             +1 = move aimpt. forward  along current camera vector
                              0 = no movement
    
                    globals: reads  GridSegs
                             writes Camera
    Returns:        none
*---------------------------------------------------------------------------*/
static void AimCamera( s32 xDir, s32 zDir )
{
    Vec vTs;    // tether 'shadow' in xz plane
    Vec vR;     // camera 'right' vector
    Vec vUp;    // camera 'up' vector
    Vec vM;     // final aimPt. movement vector 

    f32 minDim; 
    f32 vel;  
    f32 mag;

    f32 minX, maxX;
    f32 minZ, maxZ;


    // no movement
    if( (xDir == 0) && (zDir == 0) )
        return;


    minDim = GridSegs;
    vel    = ( 1.0f  / 20.0f );

    // compute tether projection onto xz plane
    vTs.x = Camera.at.x - Camera.pos.x;
    vTs.y = 0.0f;
    vTs.z = Camera.at.z - Camera.pos.z;
    VECNormalize( &vTs, &vTs );

    // set 'up' vector to align with y axis
    vUp.x = 0.0f;   vUp.y = 1.0f;   vUp.z = 0.0f;

    // camera 'right': vR = vTs x vUp
    VECCrossProduct( &vTs, &vUp, &vR );
    VECNormalize( &vR,&vR );


    // modify movement vectors based on direction
    vTs.x *= (f32)zDir;
    vTs.y *= (f32)zDir;
    vTs.z *= (f32)zDir;
 
    vR.x *= (f32)xDir;
    vR.y *= (f32)xDir;
    vR.z *= (f32)xDir;
 
    // compute final movement vector
    vM.x = vTs.x + vR.x;
    vM.y = vTs.y + vR.y;
    vM.z = vTs.z + vR.z;

    // this vector could be zero-length
    mag = VECMag( &vM );
    if( mag != 0.0f )
        VECNormalize( &vM, &vM );

    vM.x *= vel;
    vM.y *= vel;
    vM.z *= vel;


    // compute fwd/back position 1st;
    // move some distance along the normalized direction vector
    Camera.at.x += vM.x;
    Camera.at.y = GRID_OFFSET_V;  // y is fixed
    Camera.at.z += vM.z;


    minX = 0.0f + GRID_OFFSET_H;
    maxX = 1.0f + GRID_OFFSET_H;

    minZ = 0.0f + GRID_OFFSET_H;
    maxZ = 1.0f + GRID_OFFSET_H;


    // confine aimPt inside grid bounds
    if( Camera.at.x < minX )
        Camera.at.x = minX;
    if( Camera.at.x > maxX )
       Camera.at.x  = maxX;
    
    if( Camera.at.z < minZ )
        Camera.at.z = minZ;
    if( Camera.at.z > maxZ )
       Camera.at.z  = maxZ;

}

/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*



                             JOYSTICK_FUNCTIONS      



*----------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
    Name:           ProcessInput
    
    Description:    map joypad inputs to PadAction members;
                    this provides a layer of abstraction between the joypad
                    and program state.

                    buttons produce different actions depending on cursor mode.

                    maintains an internal 'previous button' list for 
                    toggling buttons.
                    
    Arguments:      globals:  writes PadAction
                              reads  Csr
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void ProcessInput( void )
{
    static u8  cullAll        = 0;
    static s8  prevSubStickX  = 0;
    static s8  prevSubStickY  = 0;
    u16        button;
    u16        buttonDown;
    s8         stickX;
    s8         stickY;
    s8         subStickX;
    s8         subStickY;
    u8         triggerL;
    u8         triggerR;
    long       numFreeBytes;


    // set all 'PadAction' fields to 0
    memset( &PadAction, 0, sizeof(ActionList) );

    // get current button, stick states
    DEMOPadRead();

    button     = DEMOPadGetButton(0);
    buttonDown = DEMOPadGetButtonDown(0);
    stickX     = DEMOPadGetStickX(0);
    stickY     = DEMOPadGetStickY(0);
    subStickX  = DEMOPadGetSubStickX(0);
    subStickY  = DEMOPadGetSubStickY(0);
    triggerL   = DEMOPadGetTriggerL(0);
    triggerR   = DEMOPadGetTriggerR(0);
 
    // process pad input in hierarchical order.


    // clear all on-screen objects
    if( buttonDown & PAD_BUTTON_MENU )
    {
        FreeNodeList( &InstObjList );
        InstObjList = NULL;
        return;
    }

    // reset = left and right triggers together:
    //         no other action possible.
    //         type of reset will depend on cursor mode
    if( (buttonDown & PAD_TRIGGER_L) && (buttonDown & PAD_TRIGGER_R) )
    {
        PadAction.reset = 1;
        return;
    }

    //  change cursor mode = right trigger: 
    //                       no other action possible
    if( buttonDown & PAD_TRIGGER_R )
    {
        PadAction.csrMode = 1;
        return;
    }


    // other actions share buttons - action taken depends on cursor mode
    switch( Csr.mode )
    {

      case FLASHER_MODE_TILE:     // instance a new object from RefObjList;
                                  // rotate, zoom camera about its aim point;
                                  // move the cursor to a new square.

        // select previous object
        if( buttonDown & PAD_BUTTON_B )
        {
            PadAction.objSelect = -1;
        }

        // select next object
        if( buttonDown & PAD_BUTTON_X )
        {
            PadAction.objSelect = 1;
        }

        //-----------------------

        // zoom camera
        if( (button & PAD_BUTTON_Y) )
        {
            PadAction.camZoom = 1;
        }
        else if( (button & PAD_BUTTON_A) )
        {
            PadAction.camZoom = -1;
        }

        // elevate camera
        if( stickY < 0 )
        {
            PadAction.camRotX = -1;
        }
        else if( stickY > 0 )
        {
            PadAction.camRotX =  1; 
        }

        // rotate camera
        if( stickX < 0 )
        {
            PadAction.camRotY = -1;
        }
        else if( stickX > 0 )
        {
            PadAction.camRotY =  1;
        }

        //------------------------

        // select grid square
        if( !(prevSubStickX) && (subStickX < 0) )
        {
            PadAction.csrX =   -1;
        }
        else if( !(prevSubStickX) && (subStickX > 0) )
        {
            PadAction.csrX =   1;
        }
    
        if( !(prevSubStickY) && (subStickY < 0) )
        {
            PadAction.csrZ =   1;
        }
        else if( !(prevSubStickY) && (subStickY > 0) )
        {
            PadAction.csrZ =  -1;
        }
        
        //-------------------------

        // cycle through backgrounds
        if( buttonDown & PAD_TRIGGER_L )
        {
            PadAction.bkgdSelect = 1;
        }

        //-------------------------

        break;

      case FLASHER_MODE_CAM:      // change camera's aim point;
                                  // scale grid

        // change aimpoint in z
        if( stickY < 0 )
        {
            PadAction.targetTransZ = -1;
        }
        else if( stickY > 0 )
        {
            PadAction.targetTransZ =  1;    
        }

        // change aimpoint in X
        if( stickX < 0 )
        {
            PadAction.targetTransX = -1;
        }
        else if( stickX > 0 )
        {
            PadAction.targetTransX =  1;
        }

        //--------------------------

        // scale grid
        if( buttonDown & PAD_BUTTON_Y )
        {
            PadAction.gridScale = -1;
        }
        else if( buttonDown & PAD_BUTTON_A )
        {
            PadAction.gridScale =  1;
        }

        break;

      case FLASHER_MODE_UTIL:

        // show render time
        if( buttonDown & PAD_BUTTON_A )
            OSReport( "GX: %2.5f CPU: %2.5f\n", MSecGX, MSecCPU );

        // change the cull mode
        if( buttonDown & PAD_BUTTON_B )
        {
            cullAll = (u8)((cullAll + 1) % 2);
            if( cullAll )
                GXSetCullMode( GX_CULL_ALL );
            else
                GXSetCullMode( GX_CULL_BACK );
        }

        // check the number of free bytes left in the heap
        if( buttonDown & PAD_BUTTON_X )
        {
            numFreeBytes = OSCheckHeap( 0 );
            if( numFreeBytes == -1 )
            {
                OSReport( "Corrupted heap.\n" );
            }
            else
            {
                OSReport( "Free memory: %d bytes\n", numFreeBytes );
            }

            OSReport( "Display list sizes are %d and %d\n", DLSize[0], DLSize[1] );
        }

        if ( buttonDown & PAD_BUTTON_Y )
        {
            OSReport("Changing performance graph mode.\n",PGFrameCount);
            PerfChangeMode = TRUE;
        }

        break;

      case FLASHER_MODE_PERF:

        if( buttonDown & PAD_BUTTON_A)
        {
            OSReport("Toggling bandwidth display\n");
            PERFToggleDrawBWBar();
            PERFToggleDrawBWBarKey();
        }

        if( buttonDown & PAD_BUTTON_B)
        {
            OSReport("Toggling CPU efficiency display\n");
            PERFToggleDrawCPUBar();
        }

        if( buttonDown & PAD_BUTTON_X)
        {
            OSReport("Toggling XF utilization display\n");
            PERFToggleDrawXFBars();
        }

        if( buttonDown & PAD_BUTTON_Y)
        {
            OSReport("Toggling Fillrate utilization display\n");            
            PERFToggleDrawRASBar();
        }

        if( buttonDown & PAD_TRIGGER_L )
        {
            // print explanation
            OSReport("Bandwidth bar key :\n");
            OSReport("PINK      Forced idle\n");
            OSReport("CYAN      Refresh\n");
            OSReport("BLUE      PE (efb copy, wrong on HW1_DRIP)\n");
            OSReport("WHITE     VI\n");
            OSReport("BEIGE     IO\n");
            OSReport("RED       DSP\n");
            OSReport("SLATE     CPU writes (wrong on HW1_DRIP)\n");   
            OSReport("YELLOW    CPU reads\n");
            OSReport("GREEN     Texture cache\n"); 
            OSReport("PURPLE    CP (Vertex cache + display lists + fifo)\n");
        }
      
        break;

    } // end switch

    prevSubStickX = subStickX;
    prevSubStickY = subStickY;
}

/*----------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*



                               LIGHT_FUNCTIONS      



*----------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*
    Name:           LightInit           
    
    Description:    initialize the scene light  
                    
    Arguments:      none    
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void LightInit ( void )
{
    GXColor white = {255, 255, 255, 255};

    LITAlloc(&light1);  

    LITInitAttn(light1, 1.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F);

    LITInitPos(light1, 0.0F, 0.0F, 0.0F );

    LITInitColor(light1, white);       

    LITAttach(light1, (Ptr)CubeWorldMatrix, PARENT_MTX);

    // fix the light over the grid
    MTXTrans( CubeWorldMatrix, 0.0f, 10.0f, 0.0f );
}

/*----------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*



                                 GUI_FUNCTIONS      



*----------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*
    Name:           DrawFrameBar    
    
    Description:    draws a bar in the top-left portion of the screen.
                    bar indicates frame rendering time in milliseconds.
                    
    Arguments:      mSecCPU: cpu render time in msec.
                    mSecGX:  gp  render time in msec.
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void DrawFrameBar( f32 mSecCPU, f32 mSecGX )
{

    //---- outlined border -------------

    #define FRM_HGT    (  16.0f )                             // pixel dimensions
    #define FRM_WID    ( 616.0f )
    #define qtrWid     ( (FRM_WID) / 4.0f )


    GXColor frmClr = { 0, 0, 0, 255 };                        // black

    f32 frmPts[]   = { 0.0f,    0.0f,    0.0f,    FRM_HGT,    // left vertical edge
                       0.0f,    0.0f,    FRM_WID, 0.0f,       // top horizontal edge
                       FRM_WID, 0.0f,    FRM_WID, FRM_HGT,    // right vertical edge
                       0.0f,    FRM_HGT, FRM_WID, FRM_HGT,    // bottom horizontal edge
                       
                        qtrWid,         0.0f,  qtrWid,         FRM_HGT,    // 1st divider
                       (qtrWid * 2.0f), 0.0f, (qtrWid * 2.0f), FRM_HGT,    // 2nd divider
                       (qtrWid * 3.0f), 0.0f, (qtrWid * 3.0f), FRM_HGT  }; // 2nd divider



    u8 frmLineWid  = ( 2 * 6 );                               // border line width                

    //---- background rectangle --------

    GXColor bkdClr   = { 64, 64, 64, 255 };                // translucent blue-grey

    f32 bkdPts[]     = { 0.0f, 0.0f, FRM_WID, 0.0f };      // line endpoints; bkd is a
                                                           // single thick line
 
    u8 bkdLineWid    = ( 16 * 6 );                         // 16-pixel wide line

    //---- cpu monitor bar -------------

    GXColor cpuClr   = { 75, 25, 0, 255 };                 // dark orange
    
    f32 cpuPts[]     = { 0.0f, 0.0f, FRM_WID, 0.0f };      // line endpts

    u8 cpuLineWid    = ( 4 * 6 );                          // 4-pixel wide line

    //---- gx monitor bar --------------

    GXColor gxClr    = {   0,  50, 125, 255 };             // deep blue

    f32 gxPts[]      = { 0.0f, 0.0f, FRM_WID, 0.0f };      // line endpts

    u8 gxLineWid     = ( 4 * 6 );                          // 4-pixel wide line                           

    //----------------------------------

    f32 PXL_SCALE_Y = ( 1.0f / 240.0f );             // convert pixel values to screen coords.
    f32 PXL_SCALE_X = ( 1.0f / 320.0f ); 

    //----------------------------------

    // line placement:
    // lines are varying thicknesses with centered width.

    /*
         ______________________________________________
    gx bar  | XXXXXXXXX|XXXXXXXX  |           |           |
            |          |          |           |           |
    cpu bar | XXXXXXXXX|XXX       |           |           |
            -----------------------------------------------
            | 16.67ms  |
            ------------
    */

    // move bar to top, left screen corner
    // + adjust for tv edges
    f32 SCRN_X_OFFSET = ( -304.0f * PXL_SCALE_X );
    f32 SCRN_Y_OFFSET = ( +216.0f * PXL_SCALE_Y );

    // pixel dimensions
    f32 bkdCtr     =  0.0f;
    f32 cpuCtr     = -2.0f;
    f32 gxCtr      = +2.0f;
    f32 frmCtr     = -8.0f;

    //------ other variables -------------

    Mtx44 mProj;                         // projection matrix:
    f32   pSave[GX_PROJECTION_SZ];       // structure to save current projection matrix
    u32   i, j;                          // loop counters to set matrix values

    f32   fpsMax    = ( 16.67f * 4.0f ); // maximum displayable mSec value
    f32   maxLength = 1.0f;              // length of frame bar relative to (-1, +1) viewport
    f32   scale     = 0.0f;              // length of mSec line within normalized frame bar

    Mtx mID;                             // transformation matrix (set to identity)


    // scale line endpts for length of render time

    // gx line
    scale = mSecGX / fpsMax;             // length of total render time in normalized frame bar
    if( scale > 1.0f )
        scale = 1.0f;

    gxPts[2] *= scale;    


    // cpu line
    scale = mSecCPU / fpsMax;            // length of total render time in normalized frame bar
    if( scale > 1.0f )
        scale = 1.0f;

    cpuPts[2] *= scale;    


    // adjust line end points to place them properly wrt each other

    // cpu
    cpuPts[1] += cpuCtr;
    cpuPts[3] += cpuCtr;

    // gx
    gxPts[1] += gxCtr;
    gxPts[3] += gxCtr;

    // frame outline
    frmPts[1]  += frmCtr;   // left vertical edge
    frmPts[3]  += frmCtr;

    frmPts[5]  += frmCtr;   // top edge
    frmPts[7]  += frmCtr;

    frmPts[9]  += frmCtr;   // right vertical edge
    frmPts[11] += frmCtr;  

    frmPts[13] += frmCtr;   // bottom edge
    frmPts[15] += frmCtr;

    frmPts[17] += frmCtr;   // 1st divider
    frmPts[19] += frmCtr;

    frmPts[21] += frmCtr;   // 2nd divider
    frmPts[23] += frmCtr;

    frmPts[25] += frmCtr;   // 3rd divider
    frmPts[27] += frmCtr;

    //----------------------------------

    // debug only
//    OSReport( " cpu: %f  gx:  %f\n", mSecCPU, mSecGX );
 
    //----------------------------------

    GXGetProjectionv( pSave );           // save current perspective projection matrix
 

    for( i = 0; i < 4; i++ )                  // set a new orthographic projection matrix
    {
        for( j = 0; j < 4; j++ )
        {
            mProj[i][j] = 0.0f;
        }
    }

    mProj[0][0] = PXL_SCALE_X;
    mProj[1][1] = PXL_SCALE_Y;
    mProj[2][2] = 1.0f;
    mProj[3][3] = 1.0f;

    mProj[0][3] = SCRN_X_OFFSET;         // translate bar to top,left screen corner
    mProj[1][3] = SCRN_Y_OFFSET;                 // offset a bit for tv edges

    GXSetProjection( mProj, GX_ORTHOGRAPHIC );


    // draw fps bar
    //=========================================================

    // disable lighting - use register color only
    GXSetNumChans( 1 );
    GXSetChanCtrl( GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE );

    // pass the material color in the tev stage
    GXSetTevOrder( GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0 );
    GXSetTevOp( GX_TEVSTAGE0, GX_PASSCLR );
    GXSetNumTexGens( 0 );
    GXSetNumTevStages( 1 );

    GXSetZMode( GX_FALSE, GX_ALWAYS, GX_FALSE );

    // Set current vertex descriptor to enable position and color0.
    // Both use 8b index to access their data arrays.
    GXClearVtxDesc();
    GXSetVtxDesc( GX_VA_POS, GX_DIRECT );
            
    // Position has 2 elements (x,y), each of type f32,
    GXSetVtxAttrFmt( GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0 );
       
    MTXIdentity( mID );
    GXLoadPosMtxImm( mID, GX_PNMTX0 );

    //---- draw lines in back to front order ------------------

    // background first
    GXSetChanMatColor( GX_COLOR0, bkdClr );
    GXSetLineWidth( bkdLineWid, GX_TO_ZERO );

    GXBegin( GX_LINES, GX_VTXFMT0, 2 ); 
        GXPosition3f32(bkdPts[0], bkdPts[1], -1.0F);
        GXPosition3f32(bkdPts[2], bkdPts[3], -1.0F);
                  
    GXEnd(); 

    //---- cpu bar ------------------------------------

    GXSetChanMatColor( GX_COLOR0, cpuClr );
    GXSetLineWidth( cpuLineWid, GX_TO_ZERO );

    GXBegin( GX_LINES, GX_VTXFMT0, 2 );

        GXPosition3f32(cpuPts[0], cpuPts[1], -1.0F);
        GXPosition3f32(cpuPts[2], cpuPts[3], -1.0F);
                  
    GXEnd(); 

    //---- gx bar -------------------------------------

    GXSetChanMatColor( GX_COLOR0, gxClr );
    GXSetLineWidth( gxLineWid, GX_TO_ZERO );

    GXBegin( GX_LINES, GX_VTXFMT0, 2 );

        GXPosition3f32(gxPts[0], gxPts[1], -1.0F);
        GXPosition3f32(gxPts[2], gxPts[3], -1.0F);
                  
    GXEnd(); 


    //---- frame outline --------------------------------------

    GXSetChanMatColor( GX_COLOR0, frmClr );
    GXSetLineWidth( frmLineWid, GX_TO_ZERO );

    GXBegin( GX_LINES, GX_VTXFMT0, 14 ); 

        GXPosition3f32(frmPts[0], frmPts[1], -1.0F);
        GXPosition3f32(frmPts[2], frmPts[3], -1.0F);

        GXPosition3f32(frmPts[4], frmPts[5], -1.0F);
        GXPosition3f32(frmPts[6], frmPts[7], -1.0F);

        GXPosition3f32(frmPts[8],  frmPts[9],  -1.0F);
        GXPosition3f32(frmPts[10], frmPts[11], -1.0F);

        GXPosition3f32(frmPts[12], frmPts[13], -1.0F);
        GXPosition3f32(frmPts[14], frmPts[15], -1.0F);

        GXPosition3f32(frmPts[16], frmPts[17], -1.0F); // vertical dividers
        GXPosition3f32(frmPts[18], frmPts[19], -1.0F);

        GXPosition3f32(frmPts[20], frmPts[21], -1.0F);
        GXPosition3f32(frmPts[22], frmPts[23], -1.0F);

        GXPosition3f32(frmPts[24], frmPts[25], -1.0F);
        GXPosition3f32(frmPts[26], frmPts[27], -1.0F);

    GXEnd(); 


   //=========================================================

    // restore previous state
    GXSetZMode( GX_TRUE, GX_LEQUAL, GX_TRUE );

    for( i = 0; i < 4; i++ )
    {
        for( j = 0; j < 4; j++ )
        {
            mProj[i][j] = 0.0f;
        }
    }

    // restore saved projection matrix
    mProj[0][0] = pSave[1];
    mProj[0][2] = pSave[2];
    mProj[1][1] = pSave[3];
    mProj[1][2] = pSave[4];
    mProj[2][2] = pSave[5];
    mProj[2][3] = pSave[6];
    mProj[3][2] = -1.0f;

    GXSetProjection( mProj, GX_PERSPECTIVE );
}

/*---------------------------------------------------------------------------*
    Name:           PrintMenu   
    
    Description:    prints a menu in the print window.
                    menu gives a list of button actions based on current 
                    cursor mode
                    
    Arguments:      globals:  reads Csr
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void PrintMenu( void )
{

    switch( Csr.mode )
    {
      case FLASHER_MODE_TILE:

        OSReport( "-------------------------------\n" );
        OSReport( "\n" );
        OSReport( "cursor mode:          cycle through object list\n" );
        OSReport( "\n" );                   
        OSReport( "trigger    R:         next cursor mode\n" );
        OSReport( "stick      X, Y:      move camera about aimpoint\n" );
        OSReport( "pad button Y, A:      zoom camera in/out\n" );
        OSReport( "pad button B, X:      select previous/next object\n" );
        OSReport( "substick   X, Y:      move cursor\n" );
        OSReport( "trigger    L:         next background\n" );
        OSReport( "trigger    (L AND R): reset camera\n" );
        OSReport( "\n" );
        OSReport( "-------------------------------\n" );
        break;

      case FLASHER_MODE_CAM:

        OSReport( "-------------------------------\n" );
        OSReport( "\n" );
        OSReport( "cursor mode:          move aimpoint, change number of segments\n" );
        OSReport( "\n" );
        OSReport( "trigger    R:         next cursor mode\n" );
        OSReport( "stick      X, Y:      move camera aim point\n" );
        OSReport( "pad button A, Y:      increase/decrease grid segments\n" );
        OSReport( "trigger    (L AND R): reset camera aim point\n" );
        OSReport( "\n" );
        OSReport( "-------------------------------\n" );
        break;

      case FLASHER_MODE_UTIL:

        OSReport( "-------------------------------\n" );
        OSReport( "\n" );
        OSReport( "cursor mode:          utility mode\n" );
        OSReport( "\n" );
        OSReport( "trigger    R:         next cursor mode\n" );
        OSReport( "pad button Y:         toggle performance display\n" );
        OSReport( "pad button X:         check free memory\n" );
        OSReport( "pad button A:         print GX and CPU time in msec\n" );
        OSReport( "pad button B:         change cull mode\n" );
        OSReport( "trigger    (L AND R): reset to default mode\n" );
        OSReport( "\n" );
        OSReport( "-------------------------------\n" );
        break;
        
      case FLASHER_MODE_PERF:

        OSReport( "-------------------------------\n" );
        OSReport( "\n" );
        OSReport( "cursor mode:          performance graph management mode\n" );
        OSReport( "\n" );
        OSReport( "trigger    R:         next cursor mode\n" );
        OSReport( "pad button A:         toggle bandwidth graph\n" );
        OSReport( "pad button B:         toggle CPU efficiency graph\n" );
        OSReport( "pad button X:         toggle XF utilization graphs\n" );
        OSReport( "pad button Y:         toggle Fillrate utilization graph\n" );
        OSReport( "trigger    L:         print bandwidth bar key\n" );
        OSReport( "trigger    (L AND R): reset to default mode\n" );
        OSReport( "\n" );
        OSReport( "-------------------------------\n" );
        break;
    }
}

/*---------------------------------------------------------------------------*
    Name:           DrawNum
    
    Description:    draws '16.67 ms' underneath the frame bar.
                    uses a hard-coded texture map in CMPR format
                    
    Arguments:      none
    
    Returns:        none
*----------------------------------------------------------------------------*/
/*
static void DrawNum( void )
{

    // hard-coded texture map ('16.67 ms') for a GXTexObj

    static u16           bitWid   = 64;
    static u16           bitHgt   = 16;
    static GXTexFmt      bitFmt   = GX_TF_CMPR;
    static GXTexWrapMode bitWrapS = GX_CLAMP;   
    static GXTexWrapMode bitWrapT = GX_CLAMP;
    static GXBool        bitMip   = GX_FALSE;

           GXTexObj      bitObj;

    //===========================================

    // actual compressed texture data for number bitmap.
    // stored as code so app. doesn't depend on other files.

    static unsigned char bitMap[] ATTRIBUTE_ALIGN(32) = {

    0x00, 0x00, 0xa5, 0xd8, 0x15, 0x15, 0x15, 0x15, 0xc7, 0x3e, 0x5a, 0xeb, 0xaa, 0xaa, 0xaa, 0xfa, 0xa5, 0xb7, 0x00, 0x00,
    0x4d, 0x4a, 0x40, 0x40, 0xa5, 0xd8, 0x21, 0x24, 0x70, 0x70, 0x70, 0x70, 0xa5, 0xd8, 0x9d, 0xb7, 0xaa, 0xaa, 0xaa, 0xaa, 
    0xa5, 0xd8, 0x53, 0x2c, 0x00, 0x00, 0x00, 0x96, 0xa5, 0xd8, 0x10, 0x82, 0x02, 0x09, 0x07, 0x05, 0x9d, 0x97, 0x08, 0x62, 
    0x57, 0x80, 0x20, 0x54, 0xa5, 0xd8, 0x9d, 0xb7, 0xaa, 0xaa, 0xaa, 0xaa, 0xa5, 0xd8, 0x9d, 0xb7, 0xaa, 0xaa, 0xaa, 0xaa, 
    0xa5, 0xd8, 0x9d, 0xb7, 0xaa, 0xaa, 0xaa, 0xaa, 0xa5, 0xd8, 0x9d, 0xb7, 0xaa, 0xaa, 0xaa, 0xaa, 0xa5, 0xd8, 0x9d, 0xb7, 
    0xaa, 0xaa, 0xaa, 0xaa, 0xa5, 0xd8, 0x52, 0xec, 0x00, 0x00, 0x00, 0xdc, 0xa5, 0xd8, 0x18, 0xc3, 0x01, 0x05, 0x26, 0x35, 
    0x9d, 0x97, 0x00, 0x20, 0x74, 0x00, 0x80, 0x58, 0xc7, 0x3d, 0x5a, 0xec, 0xaa, 0xaa, 0xaa, 0xbf, 0xb6, 0x7a, 0x7c, 0x52, 
    0xaa, 0xaa, 0xaa, 0x5e, 0xa5, 0xd8, 0x08, 0x41, 0x15, 0x18, 0x00, 0x00, 0xa5, 0xb7, 0x08, 0x41, 0x5c, 0x18, 0x90, 0xf0, 
    0xa5, 0xd8, 0x9d, 0xb7, 0xaa, 0xaa, 0xaa, 0xaa, 0xa5, 0xd8, 0x9d, 0xb7, 0xaa, 0xaa, 0xaa, 0xaa, 0xa5, 0xd8, 0x9d, 0xb7, 
    0xaa, 0xaa, 0xaa, 0xaa, 0xa5, 0xd8, 0x31, 0xc7, 0x00, 0x00, 0x01, 0x03, 0xa5, 0xd8, 0x9d, 0xb7, 0xaa, 0xaa, 0xaa, 0xaa, 
    0xa5, 0xd8, 0x9d, 0xb7, 0xaa, 0xaa, 0xaa, 0xaa, 0xa5, 0xb7, 0x00, 0x00, 0x00, 0x00, 0xf7, 0x79, 0xa5, 0xb7, 0x08, 0x41, 
    0x00, 0x00, 0x58, 0xbc, 0xa5, 0xd8, 0x9d, 0xb7, 0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x00, 0xa5, 0xd8, 0x54, 0x54, 0x54, 0x54, 
    0x9d, 0xb7, 0x08, 0x41, 0x00, 0x00, 0x35, 0x9a, 0x9d, 0xb7, 0x00, 0x00, 0x01, 0x01, 0xe1, 0xf1, 0x9d, 0xb7, 0x00, 0x00, 
    0x40, 0x40, 0x4a, 0x4f, 0xa5, 0xd7, 0x29, 0x66, 0x70, 0x70, 0x5e, 0x57, 0x00, 0x00, 0xa5, 0xd8, 0x15, 0x15, 0x15, 0x00, 
    0x00, 0x00, 0xa5, 0xd8, 0x55, 0x55, 0x55, 0x00, 0xa5, 0xd8, 0x18, 0xc3, 0x05, 0x06, 0x0d, 0x03, 0x9d, 0x97, 0x18, 0xc3, 
    0x07, 0x0d, 0xa7, 0x5c, 0x00, 0x00, 0xa5, 0xd8, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0xa5, 0xd8, 0x55, 0x55, 0x55, 0x00, 
    0xd7, 0x7f, 0x3a, 0x49, 0xaa, 0xaa, 0xab, 0xa9, 0x9d, 0xb7, 0x08, 0x41, 0x00, 0x00, 0xc0, 0x40, 0x00, 0x00, 0xa5, 0xd8, 
    0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0xa5, 0xd8, 0x55, 0x55, 0x55, 0x00, 0xa5, 0xd8, 0x10, 0xa2, 0x36, 0x24, 0x07, 0x09, 
    0xa5, 0xd8, 0x08, 0x61, 0x34, 0x06, 0xb4, 0x58, 0x00, 0x00, 0xa5, 0xd8, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0xa5, 0xd8, 
    0x55, 0x55, 0x55, 0x00, 0xcf, 0x5e, 0x4a, 0x8a, 0xaa, 0xab, 0xa9, 0xa9, 0xa5, 0xb7, 0x00, 0x21, 0x60, 0x40, 0xc0, 0x80, 
    0x00, 0x00, 0xa5, 0xd8, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0xa5, 0xd8, 0x55, 0x55, 0x55, 0x00, 0xa5, 0xd8, 0x9d, 0xb7, 
    0xaa, 0xaa, 0xaa, 0xaa, 0xa5, 0xd8, 0x31, 0xc7, 0x00, 0x00, 0x03, 0x01, 0x00, 0x00, 0xa5, 0xd8, 0x55, 0x55, 0x55, 0x00, 
    0x00, 0x00, 0xa5, 0xd8, 0x55, 0x55, 0x55, 0x00, 0x74, 0x31, 0x00, 0x00, 0x41, 0x41, 0x61, 0xf3, 0x9d, 0xb7, 0x29, 0x66, 
    0x24, 0x24, 0xf6, 0x67, 0x00, 0x00, 0xa5, 0xd8, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0xa5, 0xd8, 0x55, 0x55, 0x55, 0x00, 
    0x7c, 0x72, 0x08, 0x62, 0x17, 0x0a, 0xd0, 0xb5, 0xa5, 0xf8, 0x10, 0x82, 0xe1, 0x51, 0xd1, 0x61, 0x00, 0x00, 0xa5, 0xd8, 
    0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0xa5, 0xd8, 0x54, 0x54, 0x54, 0x00     };

    //===========================================

    f32 vtx2d[] = { 0.0f, 0.0f,
                    0.0f, 1.0f,
                    1.0f, 1.0f,
                    1.0f, 0.0f  };

    f32 st[]    = { 0.0f, 1.0f, 
                    0.0f, 0.0f,
                    1.0f, 0.0f,
                    1.0f, 1.0f  };

    Mtx44 mProj;                      
    f32   pSave[GX_PROJECTION_SZ];      
    u32   i,j;                                                              
    Mtx   mID;                            


    GXInitTexObj( &bitObj,    
                  (void*)bitMap,
                  bitWid, 
                  bitHgt,    
                  bitFmt,   
                  bitWrapS,    
                  bitWrapT,    
                  bitMip      );

    GXLoadTexObj( &bitObj, GX_TEXMAP0 );



    GXGetProjectionv( pSave );           // save current perspective projection matrix
 

    for(i=0; i<4; i++ )                  // set a new orthographic projection matrix
    {
        for(j=0; j<4; j++ )
        {
            mProj[i][j] = 0.0f;
        }
    }
    mProj[0][0] = 0.25f;  //1.0f;
    mProj[1][1] = 0.067f; //1.0f;
    mProj[2][2] = 1.0f;
    mProj[3][3] = 1.0f;

    mProj[0][3] = -0.95f;                // translate bar to top,left screen corner
    mProj[1][3] =  0.8f;                 // and adjust to place under frame bar 
                                         // ( see DrawFrameBar for coordinates)
        
    GXSetProjection( mProj, GX_ORTHOGRAPHIC );


    // draw fps bar
    //=========================================================

    MTXIdentity( mID );

    GXClearVtxDesc();
    GXInvalidateVtxCache();

    GXSetNumTexGens(1);
    GXSetTexCoordGen( GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY );

    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0,GX_TEXMAP0,GX_COLOR_NULL);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);

    GXSetBlendMode( GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR );

    GXSetZMode( GX_FALSE, GX_ALWAYS, GX_FALSE );

    // Set current vertex descriptor to enable position and color0.
    // Both use 8b index to access their data arrays.

    GXSetVtxDesc(GX_VA_POS,   GX_DIRECT);
    GXSetVtxDesc( GX_VA_TEX0, GX_DIRECT );
    
    // Position has 2 elements (x,y), each of type f32,
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0,GX_TEX_ST, GX_F32, 0);
    
    GXLoadPosMtxImm( mID, GX_PNMTX0 );


    // draw bitmap
    GXBegin( GX_QUADS, GX_VTXFMT0, 4 );
        GXPosition3f32(vtx2d[0], vtx2d[1], -1.0F);
        GXTexCoord2f32(st[0], st[1]);

        GXPosition3f32(vtx2d[2], vtx2d[3], -1.0F);
        GXTexCoord2f32(st[2], st[3]);

        GXPosition3f32(vtx2d[4], vtx2d[5], -1.0F);
        GXTexCoord2f32(st[4], st[5]);

        GXPosition3f32(vtx2d[6], vtx2d[7], -1.0F);
        GXTexCoord2f32(st[6], st[7]);


    GXEnd(); 

 
   //=========================================================

    // restore previous state
    GXSetZMode( GX_TRUE, GX_LEQUAL, GX_TRUE );

    for(i=0; i<4; i++ )
    {
        for(j=0; j<4; j++ )
        {
            mProj[i][j] = 0.0f;
        }
    }

    // restore saved projection matrix
    mProj[0][0] = pSave[1];
    mProj[0][2] = pSave[2];
    mProj[1][1] = pSave[3];
    mProj[1][2] = pSave[4];
    mProj[2][2] = pSave[5];
    mProj[2][3] = pSave[6];
    mProj[3][2] = -1.0f;

   GXSetProjection( mProj, GX_PERSPECTIVE );
   GXSetBlendMode(  GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR );
}
*/

/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*



                               NODE_FUNCTIONS      



*----------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*
    Name:           InsertNode
    
    Description:    inserts a Node into a linked list.
                    
    Arguments:      ppList:  address of the head of the linked list.
                             if *ppList == NULL, pNode will become
                                                the head of *ppList

                    pNode:   ptr to Node to be inserted

                    pCursor: ptr to loaction within *ppList to insert pNode;

                             if pCursor is valid, pNode will be inserted
                                                  BELOW pCursor.
                             if pCursor == NULL,  pNode will become the new
                                                  head of *ppList
    
    Returns:        none
*----------------------------------------------------------------------------*/
static void InsertNode( Node** ppList, Node* pNode, Node* pCursor )
{

    // safety check- only insert unattached nodes
    if( (pNode == NULL) || (pNode->prev) || (pNode->next) )
        return;

    // no list- make pNode the new head
    if( *ppList == NULL )
    {
        *ppList     = pNode;
    }

    // special case: cursor is 0 - make pNode the new head
    else if( pCursor == NULL )
    {
        pNode->next     = *ppList;
        (*ppList)->prev = pNode;
        *ppList         = pNode;
    }

    // cursor is the tail
    else if( pCursor->next == NULL )
    {
        pCursor->next  = pNode;
        pNode->prev    = pCursor;
    }

    // cursor is in the middle
    else
    {
        pNode->prev = pCursor;
        pNode->next = pCursor->next;

        pCursor->next->prev = pNode;
        pCursor->next       = pNode;
    }

}

/*---------------------------------------------------------------------------*
    Name:           InitNode
    
    Description:    convenience fn combining allocation of a node and InsertNode.  
                    allocates a new Node; inserts this Node below pCursor.
                    if *ppList or pCursor == NULL, Node becomes the head of *ppList.
                    ( see InsertNode )
                    
    Arguments:      ppList:  address of the head of the linked list.

                    pCursor: ptr to loaction within *ppList to insert pNode;

                             if pCursor is valid, pNode will be inserted
                                                  BELOW pCursor.
                             if pCursor == NULL,  pNode will become the new
                                                  head of *ppList
    
    Returns:        ptr to the newly allocated/inserted Node
*----------------------------------------------------------------------------*/
static Node* InitNode( Node** ppList, Node* pCursor )
{
    Node* pNode = (Node*)MemAlloc( sizeof(Node) );

    pNode->type = NODE_TYPE_NONE;
    pNode->data = NULL;
    pNode->prev = NULL;
    pNode->next = NULL;

    InsertNode( ppList, pNode, pCursor );

    return pNode;
}

/*---------------------------------------------------------------------------*
    Name:           AttachData
    
    Description:    attaches data to a node.
                    use this fn to associate an Obj*, RefObj* with a Node
                    
    Arguments:      pNode:    ptr to Node receiving data
                    nodeType: corresponds to data being attached;
                              ( one of NODE_TYPE_OBJ, NODE_TYPE_REF_OBJ )
    
                    data:     ptr to data to attach ( (void*)(Obj* or RefObj*) )

    Returns:        none                     
*----------------------------------------------------------------------------*/
static void AttachData( Node* pNode, u32 nodeType, void* data )
{

    if( pNode == NULL )
        return;

    pNode->type = nodeType;
    pNode->data = data;
}

/*---------------------------------------------------------------------------*
    Name:           RemoveNode
    
    Description:    removes a node from the linked list;
                    does NOT free the node's memory.
                    
    Arguments:      ppList:  address of the head of the linked list.

                    pNode:   ptr to Node to be removed from *ppList
    
    Returns:        ptr to Node below pNode; 
                    if *ppList was a single-node list, returns NULL.
                    if pNode was at the tail, returns NULL.                     
*----------------------------------------------------------------------------*/
static Node* RemoveNode( Node** ppList, Node* pNode )
{
    Node* pTmp = NULL;


    // safety check
    if( (*ppList == NULL) || (pNode == NULL) )
        return NULL;


    if( pNode == *ppList )
    {
        // head of single node list
        if( (*ppList)->next == NULL )
        {
            *ppList     = NULL;
            pNode->prev = NULL;
            pNode->next = NULL;
            pTmp        = NULL;
        }

        // head of multi-node list
        else
        {
            *ppList         = pNode->next;
            (*ppList)->prev = NULL;

            pNode->prev     = NULL;
            pNode->next     = NULL;

            pTmp            = (*ppList);
        }
    }

    // pNode is at the tail
    else if( pNode->next == NULL )
    {
        pNode->prev->next = NULL;
        pNode->prev       = NULL;
        pNode->next       = NULL;
        pTmp              = NULL;
    }

    // pNode is in the middle of the list
    else
    {
        pNode->prev->next = pNode->next;
        pNode->next->prev = pNode->prev;
        pTmp              = pNode->next;

        pNode->prev = NULL;
        pNode->next = NULL;
    }

    return pTmp;
}
/*---------------------------------------------------------------------------*
    Name:           FreeNode
    
    Description:    convenience fn combining RemoveNode, freeing a node's data
                    and freeing a node's structure.
                
    Arguments:      ppList:  address of the head of the linked list.

                    pNode:   address of Node* to remove and free from *ppList.
                             when this fn completes, *ppNode is NULL
    
    Returns:        ptr to Node below pNode; 
                    if *ppList was a single-node list, returns NULL.
                    if pNode was at the tail, returns NULL.                     
*----------------------------------------------------------------------------*/
static Node* FreeNode( Node** ppList, Node** ppNode )
{

    // detach *pNode from list
    Node* pNext = RemoveNode( ppList, *ppNode );

    // free node->data
    switch( (*ppNode)->type )
    {
      case NODE_TYPE_NONE:

        // unknown type- free whatever data is there.
        if( (*ppNode)->data )
            OSFree( (*ppNode)->data );
        break;

      case NODE_TYPE_REF_OBJ:

        FreeRefObj( (RefObj**)( &(*ppNode)->data ) );
        break;

      case NODE_TYPE_OBJ:

        FreeObj( (Obj**)( &(*ppNode)->data ) );
        break;

    }// end switch


    // free node structure
    (*ppNode)->type = NODE_TYPE_NONE;
    (*ppNode)->data = NULL;

    OSFree( *ppNode );
    *ppNode = NULL;

    return pNext;
}

/*---------------------------------------------------------------------------*
    Name:           FreeNodeList
    
    Description:    frees an entire linked list;
                    for each node, frees node data, detaches node,
                    frees node structure (see RemoveNode, FreeNode).
                    
    Arguments:      ppList:  address of the head of the linked list.
    
    Returns:        none                    
*----------------------------------------------------------------------------*/
static void FreeNodeList( Node** ppList )
{

    Node* pTmp;

    pTmp = *ppList;
    while( pTmp )
    {   
        pTmp = FreeNode( ppList, &pTmp );       
    }
    *ppList = NULL;
}

/*---------------------------------------------------------------------------*
    Name:           FindNodeByData
    
    Description:    traverses a linked list and locates a node containing
                    pData as its *data member.
                    used to back-reference into a linked list from either
                    an Obj* or RefObj*.
                    
    Arguments:      pList:  head of the linked list to search.

                    pData:  ptr for search; 
                            ( search node->data members for this ptr )
    
    Returns:        ptr to Node containing Data as its '->data' member. 
                    NULL if no Node found.                    
*----------------------------------------------------------------------------*/
static Node* FindNodeByData( Node* pList, void* pData )
{
    Node* pNode;


    pNode = pList;
    while( pNode )
    {
        if( pNode->data == pData )
            break;

        pNode = pNode->next;
    }

    return pNode;
}

/*----------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*



                              FIFO_FUNCTIONS      



*----------------------------------------------------------------------------*/

#if !defined(MAC) && !defined(FIFO_IMMEDIATE_MODE)

/*---------------------------------------------------------------------------*
    Name:           FifoInit
    
    Description:    initializes fifo so that the CPU will calculate one frame
                    adhead of the GP in the same fifo.  we don't worry about
                    holding pointers to deallocate fifo and fifo data at the
                    end of program since this is used until the program is done.
                    should be called after all initialization is done 
                    and just before main loop.
                    
    Arguments:      none
    
    Returns:        none
*----------------------------------------------------------------------------*/
static void FifoInit( void )
{
    GXFifoObj *fifo;
    GXFifoObj *fifoCPUCurrent;

    fifo = MemAlloc( sizeof(GXFifoObj) );

    // get the default fifo and free it
    GXSetDrawDone();
    fifoCPUCurrent = GXGetCPUFifo();

    // allocate new fifos
    GXInitFifoBase( fifo, MemAlloc(FIFO_SIZE), FIFO_SIZE );

    // set the CPU and GP fifo
    GXSetCPUFifo( fifo );
    GXSetGPFifo( fifo );        

    // set a drawdone token in the fifo
    GXSetDrawSync( FIFO_DRAWDONE_TOKEN );

    // install a callback so we can capture the GP rendering time
    GXSetDrawDoneCallback( CheckRenderingTime );

    // free the default fifo when we set GP fifo to a different fifo
    OSFree( GXGetFifoBase(fifoCPUCurrent) );
}

/*---------------------------------------------------------------------------*
    Name:           FifoTick
    
    Description:    changes the GP breakpoint so CPU can calculate next frame
                    while GP processes last frame. should be called before any 
                    commands are sent to GP (at the top of the main loop) 
                    since this function assumes that both the CPU and GP are 
                    not processing.
                    
    Arguments:      none
    
    Returns:        none
*----------------------------------------------------------------------------*/
static void FifoTick( void )
{
    void *readPtr, *writePtr;

    // Set a breakpoint at the current point in the CPU 
    // and disable the previous one to let the GP start processing
    GXFlush();
    GXGetFifoPtrs( GXGetCPUFifo(), &readPtr, &writePtr );
    GXEnableBreakPt( writePtr );

    // send a different token so CPU will prematurely think GP is done from previous 
    // drawdone token
    // we do not send it if we are using the BWBAR performance graph, as
    // that is very sensitive to extra tokens in the pipe.
    if (PerfGraphMode != PG_BWBARS)
    {
        GXSetDrawSync( FIFO_DRAWING_TOKEN );
    }
}

#endif // if !defined(MAC) && !defined(FIFO_IMMEDIATE_MODE)

/*---------------------------------------------------------------------------*
    Name:           CheckRenderingTime
    
    Description:    sets the GP rendering time.  This function is installed as
                    a callback when GXSetDrawDone is called in MyDEMODoneRender.
                    
    Arguments:      none
    
    Returns:        none
*----------------------------------------------------------------------------*/
static void
CheckRenderingTime()
{
    OSTime swTime;          // current stopwatch time in 'OSTime' format. 

    // get the rendering time
    swTime = OSCheckStopwatch( &SwMsec );
    MSecGX = (f32)OSTicksToMilliseconds( ((f32)swTime) );   
}

/*----------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*



                              HELPER_FUNCTIONS      



*----------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*
    Name:           MemAlloc
    
    Description:    allocate memory.  Halt if there is no more space.
                
    Arguments:      size: size of memory to allocate
    
    Returns:        ptr to newly allocated structure
*---------------------------------------------------------------------------*/
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
    Name:           DoFree
    
    Description:    Wrapper around OSFree() for MyPerfInit
                
    Arguments:      block: ptr to memory to be freed
    
    Returns:        none
*---------------------------------------------------------------------------*/
static void DoFree(void* block)
{
    OSFree(block);
}

/*---------------------------------------------------------------------------*
    Name:           Rect2InRect2
    
    Description:    determine if two rectangles intersect.
                    (Rect2 structures)

                    intersection is based on '<' or '>' values;
                    rectangles sharing an edge ( '<=', '>=' ) 
                    are NOT intersecting.
                    
    Arguments:      r1:  ptr to first  Rect2
                    r2:  ptr to second Rect2
    
    Returns:        1 if r1 & r2 intersect.
                    0 if no intersection.
*----------------------------------------------------------------------------*/
static u32 Rect2InRect2( Rect2* r1, Rect2* r2 )
{
    if( !r1 || !r2 )
        return 0;


    if( (r1->left < r2-> right) && (r1->right > r2->left) )
    {
        if( (r1->top < r2->bottom) && (r1->bottom > r2->top) )
        {
            return 1;
        }
    }

    return 0;
}

/*---------------------------------------------------------------------------*
    Name:           ComputeDObjRad
    
    Description:    calculate a DisplayObject's bounding radius:

                    1) compute a bounding cube using the largest
                       displacement of the (x,y,z) dimensions.
                    2) use the diagonal of this cube as the radius.
                    
    Arguments:      pDObj: ptr to DODisplayObj  
    
    Returns:        bounding radius of pDObj.
 *---------------------------------------------------------------------------*/
static f32 ComputeDObjRad( DODisplayObj* pDObj )
{
    u32   pos,  vecElm; 
    Vec   vPos;
    u8    u8Tmp;
    s8    s8Tmp;
    u16   u16Tmp;
    s16   s16Tmp;

    f32*  pF32;
    void* pPos;

    u32   compCount;    
    u32   compType; 
    u32   compShift;
    u32   compSize;
    f32   compScale;    

    f32   dMax = 0.0f;
    f32   fRad = 0.0f;


    compCount = 3;  // x,y,z

    compType  = (u32)(pDObj->positionData->quantizeInfo >> 4);  // u8, s8 etc.

    compShift = (u32)(pDObj->positionData->quantizeInfo & 0x0F);// num. shift bits

    compScale = 1.0f;                                           // de-quantization multiplier
    if( compShift )
    {
        compShift = ( (u32)1 << compShift );                    // produces 2^compShift
        compScale = 1.0f / (f32)compShift;
    }

    switch( compType )                                          // get pPos stride
    {
      case GX_U8:
      case GX_S8:     compSize = 1;   break;
      case GX_U16:    
      case GX_S16:    compSize = 2;   break;
      case GX_F32:    compSize = 4;   break;
    }

    // traverse the entire display object position list, 
    // saving the largest displacement in each dimension.
    pPos = (void*)pDObj->positionData->positionArray;

    for( pos = 0; pos < pDObj->positionData->numPositions; pos++ )
    {       
        // compute an offset for each of x, y, z.
        // extract the quantized value and multiply it by 'scale' 
        // to get the full f32 de-quantized number.
        pF32 = &vPos.x;
        for( vecElm = 0; vecElm < compCount; vecElm++ )
        {
            switch( compType )
            {
              case GX_U8:
                u8Tmp  = ( *(u8*)pPos );
                *pF32  = (f32)u8Tmp * compScale;
                break;

              case GX_S8:
                s8Tmp  = ( *(s8*)pPos );
                *pF32  = (f32)s8Tmp * compScale;
                break;

              case GX_U16:
                u16Tmp = ( *(u16*)pPos );
                *pF32  = (f32)u16Tmp * compScale;
                break;

              case GX_S16:
                s16Tmp = ( *(s16*)pPos );
                *pF32  = (f32)s16Tmp * compScale;
                break;

              case GX_F32:
                *pF32 = ( *(f32*)pPos );
                break;
            }   // end switch( compType )

            // step pF32 and pPos through each of x, y, z
            pF32++; 
            pPos = (void*)( (u8*)pPos + compSize );
        }   // end for( vecElm )

        // convert to absolute values and
        // save the largest displacement along each axis
        if( vPos.x < 0.0f ) vPos.x = -vPos.x;
        if( vPos.y < 0.0f ) vPos.y = -vPos.y;
        if( vPos.z < 0.0f ) vPos.z = -vPos.z;

        if( dMax < vPos.x )
            dMax = vPos.x;
        if( dMax < vPos.y )
            dMax = vPos.y;
        if( dMax < vPos.z )
            dMax = vPos.z;

    }   // end for( pos )

    // set the bounding radius using dMax as the
    // half dimensions of a cube.  This will fit
    // objects comfortably within a grid square.
    fRad = sqrtf( (2.0f * dMax * dMax) );

    return fRad;
}



/*----------------------------------------------------------------------------*/

static u32 Intro()
{
    static RefObj* pRefObj = NULL;
    static char*   defName = "10000.gpl";
    
    static u32     lastRow = 0, lastCol = 0;
    static u32     row     = 0, col     = 0;

    static u32 first   = 1;
    static u32 done    = 0;
    static u32 count   = 0;

    Obj*    pObj  = NULL;
    Node*   pNode = NULL;



    if( done )
        return done;


    // first time through, find the reference object
    if( first )
    {
        InstObjList = NULL;

        pNode = RefObjList;
        while( pNode )
        {
            pRefObj = (RefObj*)( pNode->data );

            if( (strcmp( defName, pRefObj->objName )) == 0 )
            {
                break;
            }

            pNode = pNode->next;
        }

        first = 0;

        // couldn't find the 'defName' file
        if( pRefObj == NULL )
        {
            done = 1;
            return done;
        }

    } // end if( first )


    // delay
    count++;
    if( (count %4) != 0 )
        goto RENDER_LOOP;


    // create initial instanced object(s)
    if( MSecGX < 16.67 )
    {
        if( ((row + col) %2) == 0 ) // checkerboard: even/even or odd/odd
        {
            Csr.gridX = col;
            Csr.gridZ = row;
            SetFlasherPos();

            pObj = InstanceObj( pRefObj );

            SetObjPos( pObj, &Csr.pos );
            pObj->row = row;
            pObj->col = col;
 
            pNode = InitNode( &InstObjList, NULL );
            AttachData( pNode, NODE_TYPE_OBJ, (void*)pObj );
        }
        
        lastRow = row;
        lastCol = col;

        col++;
        if( col >= DEF_GRID_SEGS )
        {
            col = 0;
            row++;

            if( row >= DEF_GRID_SEGS )
            {
                done = 1;
            }
        }
    }

    else
    {
        // free the last object
        // ( list head - list was created in reverse order )
        pNode = InstObjList;

        if( pNode )
            FreeNode( &InstObjList, &pNode );

        // reset cursor position
        Csr.gridX = 0;
        Csr.gridZ = 0;
        SetFlasherPos();

        done = 1;
    }


    // rendering loop

RENDER_LOOP:

    OSStartStopwatch( &SwMsec ); 

    #if !defined(MAC) && !defined(FIFO_IMMEDIATE_MODE)
        FifoTick();                  // synchronize so CPU calculates next frame while
                                     // GP processes last frame
                                     // needs to be called at the top of main loop
    #endif

        DEMOBeforeRender();

        CameraTick();  
        
        UpdateFlasher();

        DrawTick( ViewMtx );        

        DrawFrameBar( MSecCPU, MSecGX ); 
                         
        MyDEMODoneRender();          

        OSStopStopwatch(  &SwMsec ); // reset SwMsec for next frame
        OSResetStopwatch( &SwMsec );

    return done;

}

//-----------------------------------------------------------------------------