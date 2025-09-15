/*---------------------------------------------------------------------------*
  Project:  Dolphin Demo Library
  File:     DEMOPad.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/libraries/demo/src/DEMOPad.c $
    
    5     10/27/00 3:47p Hirose
    fixed build flags
    
    4     6/12/00 4:39p Hirose
    reconstructed structure and interface
    
    3     4/26/00 4:59p Carl
    CallBack -> Callback
    
    2     3/25/00 12:50a Hirose
    added some portion from cmn-pad.c
    added pad connection check
    
    1     3/23/00 1:21a Hirose
    Initial version
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <Dolphin/dolphin.h>
#include "Demo.h"

/*---------------------------------------------------------------------------*
   Global Variables
 *---------------------------------------------------------------------------*/
DEMOPadStatus       DemoPad[PAD_MAX_CONTROLLERS];
u32                 DemoNumValidPads;

/*---------------------------------------------------------------------------*
   Local Variables
 *---------------------------------------------------------------------------*/
static PADStatus    Pad[PAD_MAX_CONTROLLERS]; // internal use only
static u32          ResetReq = 0; // for error handling

/*---------------------------------------------------------------------------*/
static u32 PadChanMask[PAD_MAX_CONTROLLERS] =
{
    PAD_CHAN0_BIT, PAD_CHAN1_BIT, PAD_CHAN2_BIT, PAD_CHAN3_BIT
};

/*---------------------------------------------------------------------------*
    Name:           DEMOPadCopy
    
    Description:    This function copies informations of PADStatus into
                    DEMOPadStatus structure. Also attaches some extra
                    informations such as down/up, stick direction.
                    This function is internal use only.
    
    Arguments:      pad   : copy source. (PADStatus) 
                    dmpad : copy destination. (DEMOPadStatus)
    
    Returns:        None
 *---------------------------------------------------------------------------*/
static void DEMOPadCopy( PADStatus* pad, DEMOPadStatus* dmpad )
{
    u16  dirs;

    // Detects which direction is the stick(s) pointing.
    // This can be used when we want to use a stick as direction pad.
    dirs = 0;
    if ( pad->stickX    < - DEMO_STICK_THRESHOLD )
        dirs |= DEMO_STICK_LEFT;
    if ( pad->stickX    >   DEMO_STICK_THRESHOLD )
        dirs |= DEMO_STICK_RIGHT;
    if ( pad->stickY    < - DEMO_STICK_THRESHOLD )
        dirs |= DEMO_STICK_DOWN;
    if ( pad->stickY    >   DEMO_STICK_THRESHOLD )
        dirs |= DEMO_STICK_UP;
    if ( pad->substickX < - DEMO_STICK_THRESHOLD )
        dirs |= DEMO_SUBSTICK_LEFT;
    if ( pad->substickX >   DEMO_STICK_THRESHOLD )
        dirs |= DEMO_SUBSTICK_RIGHT;
    if ( pad->substickY < - DEMO_STICK_THRESHOLD )
        dirs |= DEMO_SUBSTICK_DOWN;
    if ( pad->substickY >   DEMO_STICK_THRESHOLD )
        dirs |= DEMO_SUBSTICK_UP;

    // Get the direction newly detected / released
    dmpad->dirsNew      = PADButtonDown(dmpad->dirs, dirs);
    dmpad->dirsReleased = PADButtonUp(dmpad->dirs, dirs);
    dmpad->dirs         = dirs;

    // Get DOWN/UP status of all buttons
    dmpad->buttonDown = PADButtonDown(dmpad->pst.button, pad->button);
    dmpad->buttonUp   = PADButtonUp(dmpad->pst.button, pad->button);

    // Get delta of analogs
    dmpad->stickDeltaX = (s16)(pad->stickX - dmpad->pst.stickX);
    dmpad->stickDeltaY = (s16)(pad->stickY - dmpad->pst.stickY);
    dmpad->substickDeltaX = (s16)(pad->substickX - dmpad->pst.substickX);
    dmpad->substickDeltaY = (s16)(pad->substickY - dmpad->pst.substickY);

    // Copy current status into DEMOPadStatus field
    dmpad->pst = *pad;
}

/*---------------------------------------------------------------------------*
    Name:           DEMOPadRead
    
    Description:    Calls PADRead() and perform clamping. Get information
                    of button down/up and sets them into extended field.
                    This function also checks whether controllers are
                    actually connected.
    
    Arguments:      None
    
    Returns:        None
 *---------------------------------------------------------------------------*/
void DEMOPadRead( void )
{
    static u32  count = 0;
    s32         i;

    // Read current PAD status
    PADRead(Pad);
    
    // Try resetting pad channels which have been not valid
    if ( ResetReq )
    {
        ++count;
        if ( count >= DEMO_PAD_CHECK_INTERVAL )
        {
            if ( PADReset(ResetReq) )
            {
                ResetReq = count = 0;
            }
        }
    }
    
    // Clamp analog inputs
    PADClamp(Pad);

    DemoNumValidPads = 0;
    for ( i = 0 ; i < PAD_MAX_CONTROLLERS ; i++ )
    {
        // Connection check
        if ( Pad[i].err == PAD_ERR_NONE )
        {
            ++DemoNumValidPads;
        }
        else if ( Pad[i].err == PAD_ERR_NO_CONTROLLER )
        {
            ResetReq |= PadChanMask[i];
        }
        
        // Copy status into DEMOPadStatus, add extra informatioins
        DEMOPadCopy(&Pad[i], &DemoPad[i]);
    }

    return;
}

/*---------------------------------------------------------------------------*
    Name:           DEMOPadInit
    
    Description:    Initialize PAD library and exported status
    
    Arguments:      None
    
    Returns:        None
 *---------------------------------------------------------------------------*/
void DEMOPadInit( void )
{
    static u32  init = 0;
    s32         i;

    // Initialize pad interface (first time only)
    if ( !init )
    {
        PADInit();
        ++init;
    }

    // Reset exported pad status
    for ( i = 0 ; i < PAD_MAX_CONTROLLERS ; i++ )
    {
        DemoPad[i].pst.button = 0;
        DemoPad[i].pst.stickX = 0;
        DemoPad[i].pst.stickY = 0;
        DemoPad[i].pst.substickX = 0;
        DemoPad[i].pst.substickY = 0;
        DemoPad[i].pst.triggerLeft = 0;
        DemoPad[i].pst.triggerRight = 0;
        DemoPad[i].pst.analogA = 0;
        DemoPad[i].pst.analogB = 0;
        DemoPad[i].pst.err = 0;
        DemoPad[i].buttonDown = 0;
        DemoPad[i].buttonUp = 0;
        DemoPad[i].dirs = 0;
        DemoPad[i].dirsNew = 0;
        DemoPad[i].dirsReleased = 0;
        DemoPad[i].stickDeltaX = 0;
        DemoPad[i].stickDeltaY = 0;
        DemoPad[i].substickDeltaX = 0;
        DemoPad[i].substickDeltaY = 0;
    }

}

#if 0 // Currently this stuff is not used.
//============================================================================
//   PAD-QUEUE Functions
//
//   This set of functions helps the game engine with constant animation
//   rate.
//
//   [Sample Code]
//
//       BOOL  isRetraced;
//       while ( !gameDone )
//       {
//           do
//           {
//               isRetraced = DEMOPadQueueRead();
//               Do_animation( );
//           }
//           while ( !isRetraced );
//
//           Do_rendering( );
//       }
//
//============================================================================
#define DEMO_PADQ_DEPTH    8

void DEMOPadQueueInit      ( void );
BOOL DEMOPadQueueRead      ( void );
void DEMOPadQueueFlush     ( void );

static  PADStatus    DemoPadQueue[DEMO_PADQ_DEPTH][PAD_MAX_CONTROLLERS];
static  vu32         DemoQin  = 0;
static  vu32         DemoQout = 0;
static  vu32         DemoQnum = 0;
static  u32          DemoLastFrame = 0;

/*---------------------------------------------------------------------------*
    Name:           DEMOPadViCallback
    Description:    This function should be called once every frame.
    Arguments:      None
    Returns:        None
 *---------------------------------------------------------------------------*/
static void DEMOPadViCallback( void* arg )
{
    #pragma  unused (arg)

    // If queue is not full, advance pointer of ring buffer.
    if ( DemoQnum < DEMO_PADQ_DEPTH )
    {
        DemoQin = ( DemoQin + 1 ) % DEMO_PADQ_DEPTH;
        DemoQnum ++;
    }

    // Read the latest pad status into pad queue
    // If queue is full, the latest pad status will be overwritten.
    PADRead( DemoPadQueue[DemoQin] );

    return;
}

/*---------------------------------------------------------------------------*
    Name:           DEMOPadQueueRead
    Description:    Read gamepad state and set to DemoPad[].
                    No need to care controller error.
                    (When error, PADState is set to zero in PADRead.)
    Arguments:      None
    Returns:        None
 *---------------------------------------------------------------------------*/
BOOL DEMOPadQueueRead( void )
{
    BOOL    waitRetrace;
    u32     i;

#ifdef  EMU
    //
    // This is fake code for emulation of pad-read based on video interrupt.
    //
    while ( VIGetRetraceCount() != DemoLastFrame )
    {
        DemoLastFrame ++;
        DEMOPadViCallback( NULL );
    }
#endif

    // If queue is empty, wait for next pad input.
    waitRetrace = GX_FALSE;
    if ( DemoQnum == 0 )
    {
        waitRetrace = GX_TRUE;
#ifdef  EMU
        // emulate pad-read on video interrupt
        while ( VIGetRetraceCount() == DemoLastFrame ) {}
        DemoLastFrame ++;
        DEMOPadViCallback( NULL );
#else
        // spin until coming next pad input
        while ( DemoQnum == 0 ) {}
#endif
    }

    // Copy status to DemoPad
    DemoQout = ( DemoQout + 1 ) % DEMO_PADQ_DEPTH;
    for ( i = 0 ; i < PAD_MAX_CONTROLLERS ; ++i )
    {
        DEMOPadCopy(&DemoPadQueue[DemoQout][i], &DemoPad[i]);
    }
    DemoQnum --;

    return waitRetrace;
}

/*---------------------------------------------------------------------------*
    Name:           DEMOPadQueueFlush
    Description:    Flush Pad-Queue
    Arguments:      None
    Returns:        None
 *---------------------------------------------------------------------------*/
void DEMOPadQueueFlush( void )
{
#ifdef  EMU
    // Reset queue paramter
    DemoQin = DemoQout = DemoQnum = 0;
    DemoLastFrame = VIGetRetraceCount();
#else
    // Reset queue paramter with interrupt disable
    BOOL    intr;
    intr = OSDisableInterrupts();
    DemoQin = DemoQout = DemoQnum = 0;
    (void)OSRestoreInterrupts(intr);
#endif
    return;
}

/*---------------------------------------------------------------------------*
    Name:           DEMOPadQueueInit
    Description:    Initialize Pad-Queue utility routines
    Arguments:      None
    Returns:        None
 *---------------------------------------------------------------------------*/
void DEMOPadQueueInit( void )
{
    // Initialize basic pad function
    DEMOPadInit();

    // Reset pad queue and initialize pad HW
    DEMOPadQueueFlush();

    // Register vi Callback function
#ifndef flagEMU
    // write code here..........
    // Register Callback function
#endif

    return;
}
/*---------------------------------------------------------------------------*/
#endif // 0


/*===========================================================================*/
