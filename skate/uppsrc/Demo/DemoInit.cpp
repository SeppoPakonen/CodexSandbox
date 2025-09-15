/*---------------------------------------------------------------------------*
  Project:  Dolphin Demo Library
  File:     DEMOInit.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/libraries/demo/src/DEMOInit.c $

    47    11/30/00 6:41p Tian
    Fixed memory leak in DEMOReInit.

    46    11/27/00 4:57p Carl
    Added DEMOSetTevColorIn and DEMOSetTevOp.

    45    10/28/00 5:57p Hirose

    44    10/27/00 1:47a Hirose
    Simplification / Clean up. Removed obsolete flags.

    43    10/26/00 10:32a Tian
    Added DEMOReInit and DEMOEnableBypassWorkaround.  When used, the
    standard DEMOBeforeRender and DEMODoneRender functions will attempt to
    repair the graphics pipe after a timeout.

    42    8/27/00 5:45p Alligator
    print warning messages through a callback function

    41    8/18/00 2:44p Dante
    Added !defined(flagWIN32) directive

    40    8/08/00 4:21p Hirose
    moved GXDrawDone call from DEMOSwapBuffers to DEMODoneRender

    39    8/02/00 3:06p Tian
    Allocate framebuffers from beginning of heap to avoid VI bug (FB's must
    be in lower 16MB).

    38    7/21/00 1:48p Carl
    Removed full-frame aa-specific code
    (moved back to demo; see frb-aa-full.c).
    Added DEMOSwapBuffers().

    37    7/18/00 7:08p Hashida
    Fixed a problem that MAC build failed.

    36    7/17/00 11:59a Hashida
    Added bad memory support.

    35    7/07/00 7:09p Dante
    PC Compatibility

    34    6/26/00 5:43p Alligator
    print stats before copy, but still include copy time, lose first
    counter on first loop.

    33    6/13/00 12:03p Alligator
    use DEMOPrintf *after* sampling stat counters

    32    6/12/00 1:46p Alligator
    updated demo statistics to support new api

    31    6/07/00 10:38a Howardc
    add comment about gamma correction

    30    6/05/00 1:55p Carl
    Added DEMODoneRenderBottom

    29    5/24/00 3:18a Carl
    OddField is gone; use DemoDrawField instead.

    28    5/24/00 1:02a Ryan
    added default gamma setting of 1.7

    27    5/19/00 1:09p Carl
    GXSetVerifyLevel needed to be #ifdef _DEBUG

    26    5/18/00 3:02a Alligator
    add DEMOStat suff, set verify level to 0

    25    5/02/00 4:05p Hirose
    added function descriptions / deleted tabs
    defined some global variables as static
    attached prefix to exported global variables

    24    4/07/00 5:57p Carl
    Updated GXSetCopyFilter call.

    23    3/24/00 6:48p Carl
    Overscan adjust no longer changes predefined modes.

    22    3/23/00 5:26p Carl
    Added code to adjust for overscan.

    21    3/23/00 1:22a Hirose
    changed to call DEMOPadInit instead of PADInit

    20    3/16/00 6:14p Danm
    Cleaned up code per code review.

    19    2/29/00 5:54p Ryan
    update for dvddata folder rename

    18    2/29/00 5:05p Danm
    Select render mode based on TV format.

    17    2/21/00 7:56p Tian
    Changed order of PAD and VI init, since PAD requires that VI be
    initialized

    16    2/18/00 6:31p Carl
    Changed __SINGLEFRAME #ifdefs to CSIM_OUTPUT
    (__SINGLEFRAME is not applicable to library code, only test code).
    Removed even/odd field #ifdefs; use OddField variable instead.

    15    2/17/00 6:08p Carl
    Removed clear from GXCopyDisp at end of single-frame test

    14    2/17/00 6:03p Hashida
    Changed the way to allocate frame buffer.

    13    2/14/00 6:40p Danm
    Enabled GXCopyDisp in DEMODoneRender.

    12    2/12/00 5:16p Alligator
    Integrate ArtX source tree changes

    11    2/11/00 3:23p Danm
    Chaged dst stride on GXCopyDisp from pixel stride to byte stride.

    10    2/08/00 1:50p Alligator
    do copy in DEMOInit to clear the color and Z buffer prior to rendering
    the first frame.

    9     1/28/00 10:55p Hashida
    Added VIFlush().
    Changed to wait for two frames after tv mode is changed.

    8     1/26/00 3:59p Hashida
    Changed to use VIConfigure.

    7     1/25/00 4:12p Danm
    Changed default render mode for all platforms to GXNtsc480IntDf.

    6     1/21/00 4:41p Alligator
    added even/odd field select for __SINGLEFRAME field tests

    5     1/19/00 3:43p Danm
    Added GXRenderModeObj *DEMOGetRenderModeObj(void) function.

    4     1/13/00 8:53p Danm
    Added GXRenderModeObj * parameter to DEMOInit()

    3     1/13/00 5:55p Alligator
    integrate with ArtX GX library code

    2     1/11/00 3:18p Danm
    Reversed init of VI and GX for single field support

    16    11/16/99 9:43a Hashida
    Moved DVDInit before using arenaLo.

    15    11/01/99 6:09p Tian
    Framebuffers no longer allocated if generating CSIM output.

    14    10/13/99 4:32p Alligator
    change GXSetViewport, GXSetScissor to use xorig, yorig, wd, ht

    13    10/07/99 12:14p Alligator
    initialize rmode before using it in BeforeRender

    12    10/06/99 10:38a Alligator
    changed enums for compressed Z format
    updated Init functions to make GX_ZC_LINEAR default

    11    9/30/99 10:32p Yasu
    Renamed and clean up functions and enums.

    10    9/28/99 4:14p Hashida
    Moved GXCopyDisp to GXDoneRender

    9     9/23/99 3:23p Tian
    New GXFifo/GXInit APIs, cleanup.

    8     9/22/99 7:08p Yasu
    Fixed paramter of GXSetViewPort

    7     9/22/99 6:12p Yasu
    Changed the parameters of GXSetDispCopySrc().
    Fixed small bugs.

    6     9/21/99 2:33p Alligator
    add aa flag, if aa set 16b pix format

    5     9/20/99 11:02a Ryan

    4     9/17/99 5:24p Ryan
    commented GXCopy temporarily

    3     9/17/99 1:33p Ryan
    update to include new VI API's

    2     7/28/99 4:21p Ryan
    Added DVDInit() and DVDSetRoot

    1     7/20/99 6:06p Alligator
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    This DEMO library provides a common application
    framework that is used in all the GX demos distributed
    with the Dolphin SDK.
 *---------------------------------------------------------------------------*/


#include <Dolphin/dolphin.h>
#include "Demo.h"

/*---------------------------------------------------------------------------*
    External functions
 *---------------------------------------------------------------------------*/
extern void DEMOUpdateStats(GXBool inc);
extern void DEMOPrintStats(void);

/*---------------------------------------------------------------------------*
    Global variables
 *---------------------------------------------------------------------------*/
void*   DemoFrameBuffer1;
void*   DemoFrameBuffer2;
void*   DemoCurrentBuffer;

/*---------------------------------------------------------------------------*
    Static variables
 *---------------------------------------------------------------------------*/
static GXBool  DemoFirstFrame = GX_TRUE;

#define DEFAULT_FIFO_SIZE (256 * 1024)

static void*        DefaultFifo;
static GXFifoObj*   DefaultFifoObj;

static GXRenderModeObj *rmode;
static GXRenderModeObj rmodeobj;

/*---------------------------------------------------------------------------*
    Automatic frame skipping for bypass bug.
 *---------------------------------------------------------------------------*/
static BOOL BypassWorkaround = FALSE;

// used to count missed frames by VI retrace callback handler
static vu32 FrameCount;
static u32  FrameMissThreshold; // number of frames to be considered a timeout

// tokens are used instead of GXDrawDone
#define DEMO_START_FRAME_TOKEN  0xFEEB
#define DEMO_END_FRAME_TOKEN    0xB00B


/*---------------------------------------------------------------------------*
    Fragmented memory systems
 *---------------------------------------------------------------------------*/
typedef struct {
	void*           start;
	void*           end;
} meminfo;

/*---------------------------------------------------------------------------*
    Foward references
 *---------------------------------------------------------------------------*/
static void __DEMOInitRenderMode(GXRenderModeObj* mode);
static void __DEMOInitMem(void);
static void __DEMOInitGX(void);
static void __DEMOInitVI(void);
static void __DEMOInitForEmu(void);

static void __BypassRetraceCallback(u32 count);
static void __BypassDoneRender(void);

static void LoadMemInfo(void);


/*---------------------------------------------------------------------------*
   Functions
 *---------------------------------------------------------------------------*/

/*===========================================================================*


    Initialization


 *===========================================================================*/

/*---------------------------------------------------------------------------*
    Name:           DEMOInit

    Description:    This function initializes the components of
                    the operating system and its device drivers.
                    The mode parameter allows the application to
                    override the default render mode. It then allocates
                    all of main memory except the area for external
                    framebuffer into a heap than can be managed
                    with OSAlloc. This function initializes the video
                    controller to run at 640x480 interlaced display,
                    with 60Hz refresh.

    Arguments:      mode  : render mode
                            Default render mode will be used when
                            NULL is given as this argument.

    Returns:        None
 *---------------------------------------------------------------------------*/
void DEMOInit(GXRenderModeObj *mode) {
	/*----------------------------------------------------------------*
	 *  Initialize OS                                                 *
	 *----------------------------------------------------------------*/
	OSInit();
	
	/*----------------------------------------------------------------*
	 *  Initialize DVD                                                *
	 *   (need to initialize DVD BEFORE program uses                  *
	 *   arena because DVDInit might change arena lo.)                *
	 *----------------------------------------------------------------*/
	DVDInit();
	
	/*----------------------------------------------------------------*
	 *  Initialize VI                                                 *
	 *----------------------------------------------------------------*/
	VIInit();
	
	/*----------------------------------------------------------------*
	 *  Initialize game PAD (PADInit is called by DEMOPadInit)        *
	 *----------------------------------------------------------------*/
	DEMOPadInit();
	
	/*----------------------------------------------------------------*
	 *  Set up rendering mode                                         *
	 *  (which refrects to GX/VI configurations and XFB size below)   *
	 *----------------------------------------------------------------*/
	__DEMOInitRenderMode(mode);
	
	/*----------------------------------------------------------------*
	 *  Initialize memory configuration (framebuffers / heap)         *
	 *----------------------------------------------------------------*/
	__DEMOInitMem();
	
	/*----------------------------------------------------------------*
	 *  Configure VI by given render mode                             *
	 *----------------------------------------------------------------*/
	VIConfigure(rmode);
	
	/*----------------------------------------------------------------*
	 *  Initialize Graphics                                           *
	 *----------------------------------------------------------------*/
	// Alloc default 256K fifo
	DefaultFifo     = OSAlloc(DEFAULT_FIFO_SIZE);
	DefaultFifoObj  = GXInit(DefaultFifo, DEFAULT_FIFO_SIZE);
	
	// Configure GX
	__DEMOInitGX();
	
	/*----------------------------------------------------------------*
	 *  Emulator only initialization portion                          *
	 *----------------------------------------------------------------*/
#ifdef flagEMU
	__DEMOInitForEmu();
#endif
	
	/*----------------------------------------------------------------*
	 *  Start up VI                                                   *
	 *----------------------------------------------------------------*/
	__DEMOInitVI();
}

/*---------------------------------------------------------------------------*
    Name:           __DEMOInitRenderMode

    Description:    This function sets up rendering mode which configures
                    GX and VI. If mode == NULL, this function use a default
                    rendering mode according to the TV format.

    Arguments:      None

    Returns:        None
 *---------------------------------------------------------------------------*/
static void __DEMOInitRenderMode(GXRenderModeObj* mode) {
	// If an application specific render mode is provided,
	// overwrite the default render mode
	if (mode != NULL) {
		rmode = mode;
	} else {
		switch (VIGetTvFormat()) {
		case VI_NTSC:
			rmode = &GXNtsc480IntDf;
			break;
		case VI_PAL:
			rmode = &GXPal528IntDf;
			break;
		case VI_MPAL:
			rmode = &GXMpal480IntDf;
			break;
		default:
			OSHalt("DEMOInit: invalid TV format\n");
			break;
		}
		
		// Trim off from top & bottom 16 scanlines (which will be overscanned).
		// So almost all demos actually render only 448 lines (in NTSC case.)
		// Since this setting is just for SDK demos, you can specify this
		// in order to match your application requirements.
		GXAdjustForOverscan(rmode, &rmodeobj, 0, 16);
		
		rmode = &rmodeobj;
	}
}

/*---------------------------------------------------------------------------*
    Name:           __DEMOInitMem

    Description:    This function allocates external framebuffers from
                    arena and sets up a heap

    Arguments:      None

    Returns:        None
 *---------------------------------------------------------------------------*/
static void __DEMOInitMem(void) {
	void*    arenaLo;
	void*    arenaHi;
	u32      fbSize;
	
	arenaLo = OSGetArenaLo();
	arenaHi = OSGetArenaHi();
	
	
	/*----------------------------------------------------------------*
	 *  Allocate external framebuffers                                *
	 *----------------------------------------------------------------*/
	
	// In HW1, we should allocate the buffers at the beginning of arena (<16MB).
	// Need to do this before OSInitAlloc is called.
	fbSize = VIPadFrameBufferWidth(rmode->fbWidth) * rmode->xfbHeight *
			 (u32)VI_DISPLAY_PIX_SZ;
	DemoFrameBuffer1 = 0;//(void*)OSRoundUp32B((u32)arenaLo);
	DemoFrameBuffer2 = 0;//(void*)OSRoundUp32B((u32)DemoFrameBuffer1 + fbSize);
	DemoCurrentBuffer = DemoFrameBuffer2;
	
	arenaLo = 0;//(void*)OSRoundUp32B((u32)DemoFrameBuffer2 + fbSize);
	OSSetArenaLo(arenaLo);
	
	
#ifndef flagEMU  // for Emulation, we don't need to handle bad memory
	
	/*----------------------------------------------------------------*
	 *  To handle bad memory (fragmented memory management system)    *
	 *----------------------------------------------------------------*/
	
	// Almost all systems are free from this issue right now.
	
	if ((OSGetConsoleType() == OS_CONSOLE_DEVHW1)
		&& (OSGetPhysicalMemSize() != 0x400000)
		&& (OSGetConsoleSimulatedMemSize() < 0x1800000)) {
		// DEVHW1 can be either a 4MB system or a full memory system
		// and "phys. memsize != 0x400000" means this is not a 4MB system
		// (note: 4MB system is guaranteed to have good memory so we don't
		// need to handle bad memory)
		// "Console sim memsize < 0x1800000" means user set console sim
		// mem size less than 24MB, which means this system has bad memory
		// so we need to take care of that (if you are sure your system
		// has good memory, set 0 (default) as console sim mem size.)
		LoadMemInfo();
		
		return;
	}
#endif // EMU
	
	
	/*----------------------------------------------------------------*
	 *  Create a heap                                                 *
	 *----------------------------------------------------------------*/
	
	// OSInitAlloc should only ever be invoked once.
	arenaLo = OSGetArenaLo();
	arenaHi = OSGetArenaHi();
	arenaLo = OSInitAlloc(arenaLo, arenaHi, 1); // 1 heap
	OSSetArenaLo(arenaLo);
	
	// Ensure boundaries are 32B aligned
	arenaLo = (void*)OSRoundUp32B(arenaLo);
	arenaHi = (void*)OSRoundDown32B(arenaHi);
	
	// The boundaries given to OSCreateHeap should be 32B aligned
	OSSetCurrentHeap(OSCreateHeap(arenaLo, arenaHi));
	// From here on out, OSAlloc and OSFree behave like malloc and free
	// respectively
	OSSetArenaLo(arenaLo = arenaHi);
}

/*---------------------------------------------------------------------------*
    Name:           __DEMOInitGX

    Description:    This function performs GX configuration by using
                    current rendering mode

    Arguments:      None

    Returns:        None
 *---------------------------------------------------------------------------*/
static void __DEMOInitGX(void) {
	/*----------------------------------------------------------------*
	 *  GX configuration by a render mode obj                         *
	 *----------------------------------------------------------------*/
	
	// These are all necessary codes that can refrect descriptions
	// of a rendering mode object into the GX configuration.
	GXSetViewport(0.0F, 0.0F, (f32)rmode->fbWidth, (f32)rmode->xfbHeight,
				  0.0F, 1.0F);
	GXSetScissor(0, 0, (u32)rmode->fbWidth, (u32)rmode->efbHeight);
	GXSetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
	GXSetDispCopyDst(rmode->fbWidth, rmode->xfbHeight);
	GXSetDispCopyYScale((f32)(rmode->xfbHeight) / (f32)(rmode->efbHeight));
	GXSetCopyFilter(rmode->aa, rmode->sample_pattern, GX_TRUE, rmode->vfilter);
	
	if (rmode->aa)
		GXSetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
	else
		GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
		
		
	/*----------------------------------------------------------------*
	 *  Miscellaneous GX initialization for demos                     *
	 *  (Since actually these are not necessary for all applications, *
	 *   You can remove them if you feel unnecessary.)                *
	 *----------------------------------------------------------------*/
	
	// Clear embedded framebuffer
	GXCopyDisp(DemoCurrentBuffer, GX_TRUE);
	
	// Verify (warning) messages are turned off by default
#ifdef _DEBUG
	GXSetVerifyLevel(GX_WARN_NONE);
#endif
	
	// Gamma correction
	GXSetDispCopyGamma(GX_GM_1_7);
	
	//
	// NOTE :   GAMMA CORRECT IS SET TO 1.7
	//
	// Many video game developers don't like gamma correction.
	// All SDK demos were made by Macintosh based emulators prior to
	// Dolphin hardware becoming available. So the DEMO library set gamma
	// to 1.7 to give demos correct colors as they were selected on computer
	// monitor.
	//
	// For more detail discussion about gamma correction. See Gamma Correction
	// section of the Demonstration Library document
	//
}

/*---------------------------------------------------------------------------*
    Name:           __DEMOInitVI

    Description:    This function performs VI start up settings that are
                    necessary at the beginning of each demo

    Arguments:      None

    Returns:        None
 *---------------------------------------------------------------------------*/
static void __DEMOInitVI(void) {
	u32 nin;
	
	// Double buffering initialization
	VISetNextFrameBuffer(DemoFrameBuffer1);
	DemoCurrentBuffer = DemoFrameBuffer2;
	
	// Tell VI device driver to write the current VI settings so far
	VIFlush();
	
	// Wait for retrace to start first frame
	VIWaitForRetrace();
	
	// Because of hardware restriction, we need to wait one more
	// field to make sure mode is safely changed when we change
	// INT->DS or DS->INT. (VIInit() sets INT mode as a default)
	nin = (u32)rmode->viTVmode & 1;
	if (nin)
		VIWaitForRetrace();
}

/*---------------------------------------------------------------------------*
    Name:           __DEMOInitForEmu

    Description:    This function performs emulator only portion of
                    initialization.

    Arguments:      None

    Returns:        None
 *---------------------------------------------------------------------------*/
static void __DEMOInitForEmu(void) {
	// Set up DVDRoot
#ifdef MACOS
	DVDSetRoot("DOLPHIN/dvddata/");
#endif
	
#ifdef flagWIN32
	char path[270] = "";
	strcat(path, installPath);
	strcat(path, "\\dvddata");
	DVDSetRoot(path);
#endif
}



/*===========================================================================*


    Basic demo framework control functions


 *===========================================================================*/

/*---------------------------------------------------------------------------*
    Name:           DEMOBeforeRender

    Description:    This function sets up the viewport to render the
                    appropriate field if field rendering is enabled.
                    Field rendering is a property of the render mode.

    Arguments:      None

    Returns:        None
 *---------------------------------------------------------------------------*/
void DEMOBeforeRender(void) {

#ifndef flagEMU
	if (BypassWorkaround) {
		// This will overwrite the end frame token from the previous frame.
		GXSetDrawSync(DEMO_START_FRAME_TOKEN);
	}
#endif // !EMU
	
	
	// Set up viewport
	if (rmode->field_rendering) {
		GXSetViewportJitter(
			0.0F, 0.0F, (float)rmode->fbWidth, (float)rmode->xfbHeight,
			0.0F, 1.0F, VIGetNextField());
	} else {
		GXSetViewport(
			0.0F, 0.0F, (float)rmode->fbWidth, (float)rmode->xfbHeight,
			0.0F, 1.0F);
	}
	
	// Invalidate vertex cache in GP
	GXInvalidateVtxCache();
	
#ifndef flagEMU
	// Invalidate texture cache in GP
	GXInvalidateTexAll();
#endif // !EMU
	
}

/*---------------------------------------------------------------------------*
    Name:           DEMODoneRender

    Description:    This function copies the embedded frame buffer (EFB)
                    to the external frame buffer (XFB) via GXCopyDisp,
                    and then calls DEMOSwapBuffers.

    Arguments:      None

    Returns:        None
 *---------------------------------------------------------------------------*/
void DEMODoneRender(void) {
	// SU bypass workaround version (Turned off by default)
	if (BypassWorkaround) {
		__BypassDoneRender();
		return;
	}
	
	// Statistics support (Turned off by default)
	if (DemoStatEnable) {
		GXDrawDone();             // make sure data through pipe
		DEMOUpdateStats(GX_TRUE); // sample counters here
		DEMOPrintStats();         // don't include time to print stats
		GXDrawDone();             // make sure data through pipe
		DEMOUpdateStats(GX_FALSE); // reset counters here, include copy time
	}
	
	// Set Z/Color update to make sure eFB will be cleared at GXCopyDisp.
	// (If you want to control these modes by yourself in your application,
	//  please comment out this part.)
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetColorUpdate(GX_TRUE);
	
	// Issue display copy command
	GXCopyDisp(DemoCurrentBuffer, GX_TRUE);
	
	// Wait until everything is drawn and copied into XFB.
	GXDrawDone();
	
	// Set the next frame buffer
	DEMOSwapBuffers();
}

/*---------------------------------------------------------------------------*
    Name:           DEMOSwapBuffers

    Description:    This function finishes copying via GXDrawDone, sets
                    the next video frame buffer, waits for vertical
                    retrace, and swaps internal rendering buffers.

    Arguments:      None

    Returns:        None
 *---------------------------------------------------------------------------*/
void DEMOSwapBuffers(void) {
	// Display the buffer which was just filled by GXCopyDisplay
	VISetNextFrameBuffer(DemoCurrentBuffer);
	
	// If this is the first frame, turn off VIBlack
	if (DemoFirstFrame) {
		VISetBlack(FALSE);
		DemoFirstFrame = GX_FALSE;
	}
	
	// Tell VI device driver to write the current VI settings so far
	VIFlush();
	
	// Wait for vertical retrace.
	VIWaitForRetrace();
	
	// Swap buffers
	if (DemoCurrentBuffer == DemoFrameBuffer1)
		DemoCurrentBuffer = DemoFrameBuffer2;
	else
		DemoCurrentBuffer = DemoFrameBuffer1;
}

/*---------------------------------------------------------------------------*
    Name:           DEMOSetTevColorIn

    Description:    This function is a Rev A compatible version of
                    GXSetTevColorIn.  It will set the swap mode if
                    one of TEXC/TEXRRR/TEXGGG/TEXBBB is selected.

                    It doesn't error check for multiple swaps, however.

                    Note that it is available for Rev A or Rev B.
                    For Rev A, it is defined as an inline in demo.h.

    Arguments:      Same as GXSetTevColorIn

    Returns:        None
 *---------------------------------------------------------------------------*/
#if ( GX_REV > 1 )
void DEMOSetTevColorIn(GXTevStageID stage,
		GXTevColorArg a, GXTevColorArg b,
		GXTevColorArg c, GXTevColorArg d) {
	u32 swap = 0;
	
	if (a == GX_CC_TEXC) {
		swap = GX_CC_TEXRRR - 1;
	} else
		if (a >= GX_CC_TEXRRR) {
			swap = a;
			a = GX_CC_TEXC;
		}
		
	if (b == GX_CC_TEXC) {
		swap = GX_CC_TEXRRR - 1;
	} else
		if (b >= GX_CC_TEXRRR) {
			swap = b;
			b = GX_CC_TEXC;
		}
		
	if (c == GX_CC_TEXC) {
		swap = GX_CC_TEXRRR - 1;
	} else
		if (c >= GX_CC_TEXRRR) {
			swap = c;
			c = GX_CC_TEXC;
		}
		
	if (d == GX_CC_TEXC) {
		swap = GX_CC_TEXRRR - 1;
	} else
		if (d >= GX_CC_TEXRRR) {
			swap = d;
			d = GX_CC_TEXC;
		}
		
	GXSetTevColorIn(stage, a, b, c, d);
	
	if (swap > 0)
		GXSetTevSwapMode(stage, GX_TEV_SWAP0,
						 (GXTevSwapSel)(swap - GX_CC_TEXRRR + 1));
}
#endif

/*---------------------------------------------------------------------------*
    Name:           DEMOSetTevOp

    Description:    This function is a Rev-A compatible version of
                    GXSetTevOp.  It will set the swap mode if
                    GX_CC_TEXC is selected for an input.

                    Note that it is available for Rev A or Rev B.
                    For Rev A, it is defined as an inline in demo.h.

    Arguments:      Same as GXSetTevOp

    Returns:        None
 *---------------------------------------------------------------------------*/
#if ( GX_REV > 1 )
void DEMOSetTevOp(GXTevStageID id, GXTevMode mode) {
	GXTevColorArg carg = GX_CC_RASC;
	GXTevAlphaArg aarg = GX_CA_RASA;
	
	if (id != GX_TEVSTAGE0) {
		carg = GX_CC_CPREV;
		aarg = GX_CA_APREV;
	}
	
	switch (mode) {
	case GX_MODULATE:
		DEMOSetTevColorIn(id, GX_CC_ZERO, GX_CC_TEXC, carg, GX_CC_ZERO);
		GXSetTevAlphaIn(id, GX_CA_ZERO, GX_CA_TEXA, aarg, GX_CA_ZERO);
		break;
	case GX_DECAL:
		DEMOSetTevColorIn(id, carg, GX_CC_TEXC, GX_CC_TEXA, GX_CC_ZERO);
		GXSetTevAlphaIn(id, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, aarg);
		break;
	case GX_BLEND:
		DEMOSetTevColorIn(id, carg, GX_CC_ONE, GX_CC_TEXC, GX_CC_ZERO);
		GXSetTevAlphaIn(id, GX_CA_ZERO, GX_CA_TEXA, aarg, GX_CA_ZERO);
		break;
	case GX_REPLACE:
		DEMOSetTevColorIn(id, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
		GXSetTevAlphaIn(id, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
		break;
	case GX_PASSCLR:
		GXSetTevColorIn(id, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, carg);
		GXSetTevAlphaIn(id, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, aarg);
		break;
	default:
		ASSERTMSG(0, "DEMOSetTevOp: Invalid Tev Mode");
		break;
	}
	
	GXSetTevColorOp(id, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
	GXSetTevAlphaOp(id, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
}
#endif

/*---------------------------------------------------------------------------*
    Name:           DEMOGetRenderModeObj

    Description:    This function returns the current rendering mode.
                    It is most useful to inquire what the default
                    rendering mode is.

    Arguments:      None

    Returns:        None
 *---------------------------------------------------------------------------*/
GXRenderModeObj* DEMOGetRenderModeObj(void) {
	return rmode;
}

/*---------------------------------------------------------------------------*
    Name:           DEMOGetCurrentBuffer

    Description:    This function returns the pointer to external
                    framebuffer currently active. Since this library
                    swiches double buffer DemoFrameBuffer1/DemoFrameBuffer2,
                    the returned pointer will be one of them.

    Arguments:      None

    Returns:        None
 *---------------------------------------------------------------------------*/
void* DEMOGetCurrentBuffer(void) {
	return DemoCurrentBuffer;
}



/*===========================================================================*


    HW1 Bypass bug hang auto-recovery system


 *===========================================================================*/
/*---------------------------------------------------------------------------*
    Name:           DEMOEnableBypassWorkaround

    Description:    Sets up the DEMO library to attempt to repair the
                    graphics pipe whenever a timeout of
                    /timeoutFrames/ occurs.  This will serve as a
                    temporary workaround for the insidious, vicious,
                    and just plain nasty Bypass Bug.

    Arguments: timeoutFrames        The number of frames to wait in
                                    DEMODoneRender before aborting the
                                    graphics pipe.  Should be at least
                                    equal to your standard frame rate
                                    (e.g. 60hz games should use a value of 1,
                                    30hz games should use a value of 2)

    Returns:        None
 *---------------------------------------------------------------------------*/
void DEMOEnableBypassWorkaround(u32 timeoutFrames) {
#ifndef flagEMU
	BypassWorkaround = TRUE;
	FrameMissThreshold = timeoutFrames;
	VISetPreRetraceCallback(__BypassRetraceCallback);
#else
#pragma unused (timeoutFrames)
#endif
}

/*---------------------------------------------------------------------------*
    Name:           __BypassRetraceCallback

    Description:    VI callback to count missed frames for BypassWorkaround

    Arguments:      Unused

    Returns:        None
 *---------------------------------------------------------------------------*/
static void __BypassRetraceCallback(u32 count) {
#pragma unused (count)
	FrameCount++;
}


/*---------------------------------------------------------------------------*
    Name:           __BypassDoneRender

    Description:    Called in lieu of the standard DEMODoneRender if
                    BypassWorkaround == TRUE.

                    Uses a token to check for end of frame so that we can
                    also count missed frames at the same time.

    Arguments:      None

    Returns:        None
 *---------------------------------------------------------------------------*/
static void __BypassDoneRender() {
	BOOL abort = FALSE;
	GXCopyDisp(DemoCurrentBuffer, GX_TRUE);
	GXSetDrawSync(DEMO_END_FRAME_TOKEN);
	
	FrameCount = 0;
	
	while ((GXReadDrawSync() != DEMO_END_FRAME_TOKEN) && !abort) {
		if (FrameCount >= FrameMissThreshold) {
			OSReport("---------WARNING : ABORTING FRAME----------\n");
			abort = TRUE;
			DEMOReInit(rmode); // XXX RMODE?
		}
	}
	
	DEMOSwapBuffers();
}

/*---------------------------------------------------------------------------*
    Name:           DEMOReInit

    Description:    Re-initializes the graphics pipe.  Makes no assumptions
                    about the Fifo (allowing you to change it in your program
                    if needed).

    Arguments:      mode   render mode object

    Returns:        None
 *---------------------------------------------------------------------------*/
void DEMOReInit(GXRenderModeObj *mode) {
	// Create a temporary FIFO while the current one is reset
	GXFifoObj   tmpobj;
	void*       tmpFifo = OSAlloc(64 * 1024);
	
	// Get data on current Fifo.
	GXFifoObj*  realFifoObj = GXGetCPUFifo();
	void*   realFifoBase = GXGetFifoBase(realFifoObj);
	u32     realFifoSize = GXGetFifoSize(realFifoObj);
	
	// Abort the GP
	GXAbortFrame();
	
	GXInitFifoBase(&tmpobj, tmpFifo, 64*1024);
	
	GXSetCPUFifo(&tmpobj);
	GXSetGPFifo(&tmpobj);
	
	/*----------------------------------------------------------------*
	 *  Initialize Graphics again
	 *----------------------------------------------------------------*/
	__DEMOInitRenderMode(mode);
	
	// This will re-initialize the pointers for the original FIFO.
	DefaultFifoObj = GXInit(realFifoBase, realFifoSize);
	
	__DEMOInitGX();
	
	// NOTE: the VI settings do not necessarily have to be reset, but
	//       just to be safe, we do so anyway
	VIConfigure(rmode);
	__DEMOInitVI();
	
	// remove the temporary fifo
	OSFree(tmpFifo);
}



/*===========================================================================*


    Fragmented memory management system


 *===========================================================================*/

/*---------------------------------------------------------------------------*
    Name:           LoadMemInfo

    Description:    This function is to load memory info, which is provided
                    as a file named "/meminfo.bin" and adds good memory areas
                    to heap. Note that a heap should be initialized before
                    calling this function.

    Arguments:      None

    Returns:        None
 *---------------------------------------------------------------------------*/
// NUM_MEMENTRY * sizeof(meminfo) bytes are allocated in stack (LoadMemInfo())
// Since this buffer is used for DVD DMA, NUM_MEMENTRY * sizeof(meminfo) should
// be a multiple of 32, i.e., NUM_MEMENTRY should be a multiple of 4 assuming
// sizeof(meminfo) is 8.
#define NUM_MEMENTRY            4
#define MIN(x, y)               (((x) < (y))? (x):(y))

static void LoadMemInfo(void) {
	void*               arenaHiOld;
	void*               arenaLo;
	void*               arenaHi;
	void*               simMemEnd;
	DVDFileInfo         fileInfo;
	u32                 length;
	u32                 transferLength;
	s32                 offset;
	u32                 i;
	u32                 indexMax;
	
	// Buffer for a DVD file "/meminfo.bin". Since we modify heap
	// based on the information in this file, allocate the buffer
	// in stack is the safest. +31 is for alignment margin.
	char*               buf[sizeof(meminfo)*NUM_MEMENTRY + 31];
	meminfo*            memEntry;
	
	// Pre-allocate bad memory blocks so that OSAlloc won't use the
	// area. Info on the location of bad memory blocks is provided
	// by a DVD file "/meminfo.bin".
	
	OSReport("\nNow, try to find memory info file...\n\n");
	
	// Check to see if we can find a memory info file
	if (DVDOpen("/meminfo.bin", &fileInfo) == FALSE) {
		OSReport("\nCan't find memory info file. Use /XXX toolname/ to maximize available\n");
		OSReport("memory space. For now, we only use the first %dMB.\n",
				 OSGetConsoleSimulatedMemSize() / 1024 / 1024);
		         
		// do nothing but normal initialization
		
		// Create a heap
		// OSInitAlloc should only ever be invoked once.
		arenaLo = OSGetArenaLo();
		arenaHi = OSGetArenaHi();
		
		arenaLo = OSInitAlloc(arenaLo, arenaHi, 1); // 1 heap
		OSSetArenaLo(arenaLo);
		
		// Ensure boundaries are 32B aligned
		arenaLo = (void*)OSRoundUp32B(arenaLo);
		arenaHi = (void*)OSRoundDown32B(arenaHi);
		
		// The boundaries given to OSCreateHeap should be 32B aligned
		OSSetCurrentHeap(OSCreateHeap(arenaLo, arenaHi));
		// From here on out, OSAlloc and OSFree behave like malloc and free
		// respectively
		OSSetArenaLo(arenaLo = arenaHi);
		
		return;
	}
	
	// Ensure alignment because this buffer is used for DMA
	memEntry = (meminfo*)OSRoundUp32B(buf);
	
	// Remember the start and end of current hi mem.
	arenaHiOld = OSGetArenaHi();
	simMemEnd = OSPhysicalToCached(OSGetConsoleSimulatedMemSize());
	
	// Push arenahi to the end of physical memory. Note that beyond
	// /simMemEnd/ there might be bad memory blocks. We will pre-allocate
	// those blocks using OSAllocFixed() so that later OSAlloc won't
	// use the bad blocks.
	OSSetArenaHi(OSPhysicalToCached(OSGetPhysicalMemSize()));
	
	// Create a heap
	// OSInitAlloc should only ever be invoked once.
	arenaLo = OSGetArenaLo();
	arenaHi = OSGetArenaHi();
	
	arenaLo = OSInitAlloc(arenaLo, arenaHi, 1); // 1 heap
	OSSetArenaLo(arenaLo);
	
	// Ensure boundaries are 32B aligned
	arenaLo = (void*)OSRoundUp32B(arenaLo);
	arenaHi = (void*)OSRoundDown32B(arenaHi);
	
	// The boundaries given to OSCreateHeap should be 32B aligned
	OSSetCurrentHeap(OSCreateHeap(arenaLo, arenaHi));
	// From here on out, OSAlloc and OSFree behave like malloc and free
	// respectively
	OSSetArenaLo(arenaLo = arenaHi);
	
	// Allocate from /arenaHiOld/ to /simMemEnd/ so that OSAlloc
	// doesn't destroy hi mem. XXX OSAllocFixed will destroy the area?
	OSAllocFixed(&arenaHiOld, &simMemEnd);
	
	length = DVDGetLength(&fileInfo);
	offset = 0;
	
	while (length > 0) {
		OSReport("loop\n");
		// Since we have limited size of memory for the file,
		// we might need to divide the file. /transferLength/ will
		// hold the transfer length of this loop.
		transferLength = MIN(length, sizeof(meminfo) * NUM_MEMENTRY);
		
		if (DVDRead(&fileInfo, memEntry,
					(s32)OSRoundUp32B(transferLength), offset) < 0) {
			OSHalt("An error occurred when issuing read to /meminfo.bin\n");
		}
		
		indexMax = transferLength / sizeof(meminfo);
		
		for (i = 0; i < indexMax; i++) {
			OSReport("start: 0x%08x, end: 0x%08x\n", memEntry[i].start,
					 memEntry[i].end);
			OSAllocFixed(&(memEntry[i].start), &(memEntry[i].end));
			OSReport("Removed 0x%08x - 0x%08x from the current heap\n",
					 memEntry[i].start, (size_t)memEntry[i].end - 1);
		}
		
		length -= transferLength;
		offset += transferLength;
	}
	
	DVDClose(&fileInfo);
	//OSDumpHeap(__OSCurrHeap);
}


/*===========================================================================*/
