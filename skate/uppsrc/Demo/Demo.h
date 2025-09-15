/*---------------------------------------------------------------------------*
  Project:  Dolphin DEMO library
  File:     demo.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/demo.h $
    
    17    11/28/00 8:18p Hirose
    Enhancement of DEMOStat library
    Fixed DEMOSetTevOp definition for the emulator
    
    16    11/27/00 4:57p Carl
    Added DEMOSetTevColorIn and DEMOSetTevOp.
    
    15    10/26/00 10:31a Tian
    Added DEMOReInit and DEMOEnableBypassWorkaround.  Automatically repairs
    graphics pipe after a certain framecount based timeout.
    
    14    7/21/00 1:48p Carl
    Removed DEMODoneRenderBottom.
    Added DEMOSwapBuffers.
    
    13    6/20/00 10:37a Alligator
    added texture bandwidth, texture miss rate calculations
    
    12    6/19/00 3:23p Alligator
    added fill rate virtual counter
    
    11    6/12/00 4:32p Hirose
    updated DEMOPad library structures
    
    10    6/12/00 1:46p Alligator
    updated demo statistics to support new api
    
    9     6/06/00 12:02p Alligator
    made changes to perf counter api
    
    8     6/05/00 1:53p Carl
    Added DEMODoneRenderBottom
    
    7     5/18/00 2:56a Alligator
    added DEMOStats stuff
    
    6     5/02/00 3:28p Hirose
    added prototype of DEMOGetCurrentBuffer
    
    5     3/25/00 12:48a Hirose
    added DEMO_PAD_CHECK_INTERVAL macro
    
    4     3/23/00 1:20a Hirose
    added DEMOPad stuff
    
    3     1/19/00 3:43p Danm
    Added GXRenderModeObj *DEMOGetRenderModeObj(void) function.
    
    2     1/13/00 8:56p Danm
    Added GXRenderModeObj * parameter to DEMOInit()
    
    9     9/30/99 2:13p Ryan
    sweep to remove gxmodels libs
    
    8     9/28/99 6:56p Yasu
    Add defines of font type

    7     9/24/99 6:45p Yasu
    Change type of parameter of DEMOSetupScrnSpc().

    6     9/24/99 6:40p Yasu
    Change the number of parameter of DEMOSetupScrnSpc()

    5     9/24/99 6:35p Yasu
    Add DEMOSetupScrnSpc()

    4     9/23/99 5:35p Yasu
    Change function name cmMtxScreen to DEMOMtxScreen

    3     9/17/99 1:33p Ryan
    Added DEMOBeforeRender and DEMODoneRender

    2     9/14/99 5:08p Yasu
    Add some functions which contained in DEMOPut.c

    1     7/23/99 2:36p Ryan

    1     7/20/99 6:42p Alligator
    new demo lib
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __DEMO_H__
#define __DEMO_H__


/*---------------------------------------------------------------------------*/
#include <Dolphin/dolphin.h>
#include <CharPipe/TexPalette.h>
#include <CharPipe/GeoPalette.h>
/*---------------------------------------------------------------------------*/




/*---------------------------------------------------------------------------*
    DEMOInit.c
 *---------------------------------------------------------------------------*/
extern void             DEMOInit             ( GXRenderModeObj* mode );
extern void             DEMOBeforeRender     ( void );
extern void             DEMODoneRender       ( void );
extern void             DEMOSwapBuffers      ( void );
extern GXRenderModeObj* DEMOGetRenderModeObj ( void );
extern void*            DEMOGetCurrentBuffer ( void );

extern void             DEMOEnableBypassWorkaround ( u32 timeoutFrames );
extern void             DEMOReInit           ( GXRenderModeObj *mode );


/*---------------------------------------------------------------------------*
    DEMO misc
 *---------------------------------------------------------------------------*/

// The DEMO versions of SetTevColorIn and SetTevOp are backwards compatible
// with the Rev A versions in that the texture swap mode will be set
// appropriately if one of TEXC/TEXRRR/TEXGGG/TEXBBB is selected.

#if HAVE_DEMO_SET_TEV && ( GX_REV == 1 || defined(flagEMU) )
inline void DEMOSetTevColorIn(GXTevStageID stage,
                              GXTevColorArg a, GXTevColorArg b,
                              GXTevColorArg c, GXTevColorArg d )
    { GXSetTevColorIn(stage, a, b, c, d); }

inline void DEMOSetTevOp(GXTevStageID stage, GXTevMode mode)
    { GXSetTevOp(stage, mode); }
#else
void DEMOSetTevColorIn(GXTevStageID stage,
                       GXTevColorArg a, GXTevColorArg b,
                       GXTevColorArg c, GXTevColorArg d );

void DEMOSetTevOp(GXTevStageID stage, GXTevMode mode);
#endif


/*---------------------------------------------------------------------------*
    DEMOPad.c
 *---------------------------------------------------------------------------*/
// used to detect which direction is the stick(s) pointing
#define DEMO_STICK_THRESHOLD      48

#define DEMO_STICK_UP             0x1000
#define DEMO_STICK_DOWN           0x2000
#define DEMO_STICK_LEFT           0x4000
#define DEMO_STICK_RIGHT          0x8000
#define DEMO_SUBSTICK_UP          0x0100
#define DEMO_SUBSTICK_DOWN        0x0200
#define DEMO_SUBSTICK_LEFT        0x0400
#define DEMO_SUBSTICK_RIGHT       0x0800

// interval for checking pad connection
#define DEMO_PAD_CHECK_INTERVAL   30

// extended pad status structure
typedef struct
{
    // contains PADStatus structure
    PADStatus   pst;
    
    // extended field
    u16         buttonDown;
    u16         buttonUp;
    u16         dirs;
    u16         dirsNew;
    u16         dirsReleased;
    s16         stickDeltaX;
    s16         stickDeltaY;
    s16         substickDeltaX;
    s16         substickDeltaY;
} DEMOPadStatus;

// the entity which keeps current pad status
extern DEMOPadStatus    DemoPad[PAD_MAX_CONTROLLERS];
extern u32              DemoNumValidPads;

// main function prototypes
extern void     DEMOPadInit( void );
extern void     DEMOPadRead( void );

// inline functions for getting each component
inline u16 DEMOPadGetButton(u32 i)
    { return DemoPad[i].pst.button; }

inline u16 DEMOPadGetButtonUp(u32 i)
    { return DemoPad[i].buttonUp; }

inline u16 DEMOPadGetButtonDown(u32 i)
    { return DemoPad[i].buttonDown; }

inline u16 DEMOPadGetDirs(u32 i)
    { return DemoPad[i].dirs; }

inline u16 DEMOPadGetDirsNew(u32 i)
    { return DemoPad[i].dirsNew; }

inline u16 DEMOPadGetDirsReleased(u32 i)
    { return DemoPad[i].dirsReleased; }

inline s8  DEMOPadGetStickX(u32 i)
    { return DemoPad[i].pst.stickX; }

inline s8  DEMOPadGetStickY(u32 i)
    { return DemoPad[i].pst.stickY; }

inline s8  DEMOPadGetSubStickX(u32 i)
    { return DemoPad[i].pst.substickX; }

inline s8  DEMOPadGetSubStickY(u32 i)
    { return DemoPad[i].pst.substickY; }

inline u8  DEMOPadGetTriggerL(u32 i)
    { return DemoPad[i].pst.triggerLeft; }

inline u8  DEMOPadGetTriggerR(u32 i)
    { return DemoPad[i].pst.triggerRight; }

inline u8  DEMOPadGetAnalogA(u32 i)
    { return DemoPad[i].pst.analogA; }

inline u8  DEMOPadGetAnalogB(u32 i)
    { return DemoPad[i].pst.analogB; }

inline s8  DEMOPadGetErr(u32 i)
    { return DemoPad[i].pst.err; }

/*---------------------------------------------------------------------------*
    DEMOPuts.c
 *---------------------------------------------------------------------------*/
// filtering type for DEMOLoadFont texture
typedef enum
{
    DMTF_POINTSAMPLE,	// Point sampling
    DMTF_BILERP	        // Bilerp filtering
} DMTexFlt;

// caption font type
#define	DM_FT_OPQ	0
#define	DM_FT_RVS	1
#define	DM_FT_XLU	2

extern	void	DEMOSetFontType( s32 );
extern	void	DEMOSetupScrnSpc( s32, s32, float );
extern	void	DEMOInitCaption( s32, s32, s32 );
extern	void	DEMOLoadFont ( GXTexMapID, GXTexMtx, DMTexFlt );
extern	void	DEMOPuts     ( s16, s16, s16, const char* );
extern	void	DEMOPrintf   ( s16, s16, s16, const char*, ... );
extern	void	DEMOMtxScreen( Mtx, Mtx44, float, float, float );

/*---------------------------------------------------------------------------*
    DEMOFont.c
 *---------------------------------------------------------------------------*/
extern	u32	DEMOFontBitmap[];

/*---------------------------------------------------------------------------*
    DEMOStats.c
 *---------------------------------------------------------------------------*/
// statistic data display style
typedef enum
{
    DEMO_STAT_TL,       // top left
    DEMO_STAT_BL,       // bottom left
    DEMO_STAT_TLD,      // double-size font, top-left
    DEMO_STAT_BLD,      // double-size font, bottom-left
    DEMO_STAT_IO        // dump using OSReport
} DEMOStatDispMode;

// statistic types
typedef enum
{
    DEMO_STAT_GP0,      // GXReadGP0Metric
    DEMO_STAT_GP1,      // GXReadGP1Metric
    DEMO_STAT_MEM,      // GXReadMemMetric
    DEMO_STAT_PIX,      // GXReadPixMetric
    DEMO_STAT_VC,       // GXReadVCacheMetric
    DEMO_STAT_FR,       // Fill rate calc
    DEMO_STAT_TBW,      // Texture bandwidth calc
    DEMO_STAT_TBP,      // Texture B/pixel
    DEMO_STAT_MYC,      // print out user-computed count
    DEMO_STAT_MYR       // print out user-computed rate (stat/count)
} DEMOStatType;

// used as "stat" argument when stat_type == DEMO_STAT_MEM
typedef enum
{
    DEMO_STAT_MEM_CP,   // GXReadMemMetric(CP Req.)
    DEMO_STAT_MEM_TC,   // GXReadMemMetric(TC Req.)
    DEMO_STAT_MEM_CPUR, // GXReadMemMetric(CPU Rd Req.)
    DEMO_STAT_MEM_CPUW, // GXReadMemMetric(CPU Wr Req.)
    DEMO_STAT_MEM_DSP,  // GXReadMemMetric(DSP Req.)
    DEMO_STAT_MEM_IO,   // GXReadMemMetric(IO Req.)
    DEMO_STAT_MEM_VI,   // GXReadMemMetric(VI Req.)
    DEMO_STAT_MEM_PE,   // GXReadMemMetric(PE Req.)
    DEMO_STAT_MEM_RF,   // GXReadMemMetric(RF Req.)
    DEMO_STAT_MEM_FI    // GXReadMemMetric(FI Req.)
} DEMOMemStatArg;

// used as "stat" argument when stat_type == DEMO_STAT_PIX
typedef enum
{
    DEMO_STAT_PIX_TI,   // GXReadPixMetric(Top Pixel In)
    DEMO_STAT_PIX_TO,   // GXReadPixMetric(Top Pixel Out)
    DEMO_STAT_PIX_BI,   // GXReadPixMetric(Bottom Pixel In)
    DEMO_STAT_PIX_BO,   // GXReadPixMetric(Bottom Pixel Out)
    DEMO_STAT_PIX_CI,   // GXReadPixMetric(Color Pixel In)
    DEMO_STAT_PIX_CC    // GXReadPixMetric(Copy Clocks)
} DEMOPixStatArg;

// used as "stat" argument when stat_type == DEMO_STAT_VC
typedef enum
{
    DEMO_STAT_VC_CHK,   // GXReadVCacheMetric(Check)
    DEMO_STAT_VC_MISS,  // GXReadVCacheMetric(Miss)
    DEMO_STAT_VC_STALL  // GXReadVCacheMetric(Stall)
} DEMOVcStatArg;

typedef struct
{
    char            text[50];  // 8 x 50 = 400 pixels
    DEMOStatType    stat_type; // statics type
    u32             stat;      // metric to measure
    u32             count;     // count returned from metric function
} DEMOStatObj;


// Global variables
extern GXBool  DemoStatEnable;

// Function Prototype
extern void DEMOSetStats (
    DEMOStatObj*        stat,
    u32                 nstats,
    DEMOStatDispMode    disp );

/*---------------------------------------------------------------------------*/



#endif // __DEMO_H__

/*===========================================================================*/
