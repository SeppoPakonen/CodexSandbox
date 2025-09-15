/*---------------------------------------------------------------------------*
  Project:  Dolphin Demo Library
  File:     DEMOStats.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/libraries/demo/src/DEMOStats.c $
    
    9     11/28/00 8:21p Hirose
    Enhancements / Clean up
    
    8     8/23/00 2:53p Alligator
    allow perf counter api to work with draw sync callback
    
    7     6/20/00 10:37a Alligator
    added texture bandwidth, texture miss rate calculations
    
    6     6/19/00 3:17p Alligator
    added fill rate stat
    
    5     6/13/00 12:03p Alligator
    use DEMOPrintf *after* sampling stat counters
    
    4     6/12/00 7:25p Alligator
    fixed warnings
    
    3     6/12/00 1:46p Alligator
    updated demo statistics to support new api
    
    2     6/06/00 12:02p Alligator
    made changes to perf counter api
    
    1     5/18/00 2:55a Alligator
    new file
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    This DEMO library provides a common application
    framework that is used in all the GX demos distributed
    with the Dolphin SDK.
 *---------------------------------------------------------------------------*/

#include <string.h>
#include <Dolphin/dolphin.h>
#include "Demo.h"

#define FLIPPER_CLOCK   40.0F // MHz

#define DEMO_TEXT_TOP   16
#define DEMO_TEXT_BOT   16
#define DEMO_TEXT_LFT   16
#define DEMO_TEXT_RHT   16

#define DEMO_CHAR_WD    8
#define DEMO_CHAR_HT    8
#define DEMO_CHAR_YSP   2  // vertical space

/*---------------------------------------------------------------------------*
   Global variables
 *---------------------------------------------------------------------------*/
GXBool  DemoStatEnable  = GX_FALSE;

static DEMOStatObj* DemoStat   = NULL;
static u32 DemoStatIndx    = 0;
static u32 DemoStatMaxIndx = 0;
static u32 DemoStatClocks  = 0;

static u32 DemoStatDisp    = 0;
static u32 DemoStatStrLen  = 0;

// other statistics counts that can be counted in parallel
static u32 topPixIn, topPixOut; 
static u32 botPixIn, botPixOut;
static u32 clrPixIn, copyClks;

static u32 vcCheck, vcMiss, vcStall;

static u32 cpReq,
           tcReq,
           cpuRdReq,
           cpuWrReq,
           dspReq,
           ioReq,
           viReq,
           peReq,
           rfReq,
           fiReq;

/*---------------------------------------------------------------------------*
   Function Prototype
 *---------------------------------------------------------------------------*/
extern void DEMOPrintStats( void );
extern void DEMOUpdateStats( GXBool inc );

static void DEMOWriteStats( GXBool update );
/*---------------------------------------------------------------------------*
   Functions
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    Name:           DEMOSetStats
    
    Description:    This function sets an array of GX performance metrics to
                    count. The name of each metric is specified in the text
                    array.  The current stat count is kept in the count array.
                    At the end of each loop, the metric will be printed at
                    the corner of the screen specified, either top-left, top-
                    right, bottom-left, or bottom-right.

                    If stat is NULL or nstats is zero, the performance 
                    monitoring is disabled.

                    DEMODoneRender will call various performance functions
                    depending on which metric is called.

    Arguments:      stat  : An array of GXPerf metrics to count.  
                    nstats: number of stats in the array or number you wish
                            to display.
                    disp  : display style of the stats:
                            0 = top-left
                            2 = bot-left
                            8 = serial output (OSReport)
    
    Returns:        None
 *---------------------------------------------------------------------------*/
void DEMOSetStats( DEMOStatObj *stat, u32 nstats, DEMOStatDispMode disp )
{
    if (stat == NULL || nstats == 0)
    {
        DemoStatEnable = GX_DISABLE;
    }
    else
    {
        DemoStatEnable  = GX_TRUE;
        DemoStat        = stat;
        DemoStatIndx    = 0;
        DemoStatMaxIndx = nstats;
        DemoStatDisp    = disp;
        DemoStatStrLen  = strlen(DemoStat[0].text);
    }
}

/*---------------------------------------------------------------------------*
    Name:           DEMOWriteStats
    
    Description:    This function calls a metric function for the next stat 
                    to be counted. 

    Arguments:      none
    
    Returns:        None
 *---------------------------------------------------------------------------*/
static void DEMOWriteStats( GXBool update )
{
    u32 cnt0, cnt1, cnt2, cnt3, cnt4;
    u32 cnt5, cnt6, cnt7, cnt8, cnt9;

    switch (DemoStat[DemoStatIndx].stat_type)
    {
      case DEMO_STAT_GP0:
        cnt0 = GXReadGP0Metric();
        GXSetGP0Metric((GXPerf0)DemoStat[DemoStatIndx].stat);
        GXClearGP0Metric();
        if (update)
        {
            DemoStat[DemoStatIndx].count = cnt0;
        }
        break;

      case DEMO_STAT_GP1:
        cnt0 = GXReadGP1Metric();
        GXSetGP1Metric((GXPerf1)DemoStat[DemoStatIndx].stat);
        GXClearGP1Metric();
        if (update)
        {
            DemoStat[DemoStatIndx].count = cnt0;
        }
        break;

      case DEMO_STAT_MEM:
        GXReadMemMetric( &cnt0, &cnt1, &cnt2, &cnt3, &cnt4,
                         &cnt5, &cnt6, &cnt7, &cnt8, &cnt9 );
        GXClearMemMetric();
        if (update)
        {
            cpReq     = cnt0;
            tcReq     = cnt1;
            cpuRdReq  = cnt2;
            cpuWrReq  = cnt3;
            dspReq    = cnt4;
            ioReq     = cnt5;
            viReq     = cnt6;
            peReq     = cnt7;
            rfReq     = cnt8;
            fiReq     = cnt9;
        }
        break;

      case DEMO_STAT_PIX:
        GXReadPixMetric(&cnt0, &cnt1, &cnt2, &cnt3, &cnt4, &cnt5);
        GXClearPixMetric();
        if (update)
        {
            topPixIn  = cnt0;
            topPixOut = cnt1; 
            botPixIn  = cnt2;
            botPixOut = cnt3;
            clrPixIn  = cnt4; 
            copyClks  = cnt5;
        }
        break;

      case DEMO_STAT_VC:
        GXReadVCacheMetric(&cnt0, &cnt1, &cnt2);
        GXSetVCacheMetric(GX_VC_POS);
        GXClearVCacheMetric();
        if (update)
        {
            vcCheck = cnt0;
            vcMiss  = cnt1;
            vcStall = cnt2;
        }
        break;

      case DEMO_STAT_FR: // fill rate info
        GXReadPixMetric(&cnt0, &cnt1, &cnt2, &cnt3, &cnt4, &cnt5);
        GXClearPixMetric();
        if (update)
        {
            topPixIn  = cnt0;
            topPixOut = cnt1; 
            botPixIn  = cnt2;
            botPixOut = cnt3;
            clrPixIn  = cnt4; 
            copyClks  = cnt5;
        }
        DemoStatClocks = GXReadGP0Metric();
        GXSetGP0Metric(GX_PERF0_CLOCKS);
        GXClearGP0Metric();
        break;
        
      case DEMO_STAT_TBP: // texture B/pixel
      case DEMO_STAT_TBW: // texture bandwidth/pixel
        GXReadPixMetric(&cnt0, &cnt1, &cnt2, &cnt3, &cnt4, &cnt5);
        GXClearPixMetric();
        if (update)
        {
            topPixIn  = cnt0;
            topPixOut = cnt1; 
            botPixIn  = cnt2;
            botPixOut = cnt3;
            clrPixIn  = cnt4; 
            copyClks  = cnt5;
        }

        // valid on second call
        DemoStatClocks = GXReadGP0Metric();
        GXSetGP0Metric(GX_PERF0_CLOCKS);
        GXClearGP0Metric();

        // valid on second call
        GXReadMemMetric( &cnt0, &cnt1, &cnt2, &cnt3, &cnt4,
                         &cnt5, &cnt6, &cnt7, &cnt8, &cnt9 );
        GXClearMemMetric();
        tcReq = cnt1;
        break;
      
      case DEMO_STAT_MYC: 
      case DEMO_STAT_MYR: 
        // do nothing, user will compute
        break;
      default:
        OSHalt("DEMOSetStats: Unknown demo stat type\n");
      break;
    }
}

/*---------------------------------------------------------------------------*
    Name:           DEMOUpdateStats
    
    Description:    This function calls GXPerfMetric for the next stat to be
                    counted. 

                    DEMOBeforeRender and DEMODoneRender will call this 
                    function if stats are enabled.

    Arguments:      none
    
    Returns:        None
 *---------------------------------------------------------------------------*/
void DEMOUpdateStats( GXBool inc )
{    
    DEMOWriteStats(inc);

    if (inc)
    {
        DemoStatIndx++;
        if (DemoStatIndx == DemoStatMaxIndx)
        {
            DemoStatIndx = 0;
        }
    }
}

/*---------------------------------------------------------------------------*
    Name:           DEMOPrintStats
    
    Description:    This function prints the statistics currently being
                    counted. Only one statistic gets updated each frame,
                    but they are printed every frame.

                    DEMODoneRender will call this function if stats are
                    enabled.

    Arguments:      none
    
    Returns:        None
 *---------------------------------------------------------------------------*/
#define DEMOStatPrintf(str, val) \
    DEMOPrintf(text_x, text_y, 0, str, DemoStat[i].text, val)

#define DEMOStatOSReport(str, val) \
    OSReport(str, DemoStat[i].text, val)

void DEMOPrintStats( void )
{
    GXRenderModeObj* rmode;
    u32              i;
    s16              text_x, text_y, text_yinc;
    u16              wd, ht;
    f32              rate;

    if (DemoStatDisp == DEMO_STAT_IO) // dump to serial output
    {
        for (i = 0; i < DemoStatMaxIndx; i++)
        {
            switch (DemoStat[i].stat_type)
            {
              case DEMO_STAT_PIX:
                switch(DemoStat[i].stat)
                {
                  case DEMO_STAT_PIX_TI:
                    DEMOStatOSReport("%s: %8d\n", topPixIn);
                    break;
                  case DEMO_STAT_PIX_TO:
                    DEMOStatOSReport("%s: %8d\n", topPixOut);
                    break;
                  case DEMO_STAT_PIX_BI:
                    DEMOStatOSReport("%s: %8d\n", botPixIn);
                    break;
                  case DEMO_STAT_PIX_BO:
                    DEMOStatOSReport("%s: %8d\n", botPixOut);
                    break;
                  case DEMO_STAT_PIX_CI:
                    DEMOStatOSReport("%s: %8d\n", clrPixIn);
                    break;
                  case DEMO_STAT_PIX_CC:
                    DEMOStatOSReport("%s: %8d\n", copyClks);
                    break;
                }
                break;

              case DEMO_STAT_FR:
                rate = FLIPPER_CLOCK * (f32)(topPixIn + botPixIn) /
                                       (f32)(DemoStatClocks - copyClks);
                DEMOStatOSReport("%s: %8.2f\n", rate);
                break;

              case DEMO_STAT_TBW:
                rate = FLIPPER_CLOCK * (f32)(tcReq*32) /
                                       (f32)(DemoStatClocks - copyClks);
                DEMOStatOSReport("%s: %8.2f\n", rate);
                break;

              case DEMO_STAT_TBP:
                rate = (f32)(tcReq*32) / (topPixIn + botPixIn);
                DEMOStatOSReport("%s: %8.2f\n", rate);
                break;

              case DEMO_STAT_VC:
                switch(DemoStat[i].stat)
                {
                  case DEMO_STAT_VC_CHK:
                    DEMOStatOSReport("%s: %8d\n", vcCheck);
                    break;
                  case DEMO_STAT_VC_MISS:
                    DEMOStatOSReport("%s: %8d\n", vcMiss);
                    break;
                  case DEMO_STAT_VC_STALL:
                    DEMOStatOSReport("%s: %8d\n", vcStall);
                    break;
                }
                break;

              case DEMO_STAT_MYR: // use stat as a second input
                rate = (f32) DemoStat[i].stat / (f32) DemoStat[i].count;
                DEMOStatOSReport("%s: %8.2f\n", rate);
                break;

              case DEMO_STAT_MEM:
                switch(DemoStat[i].stat)
                {
                  case DEMO_STAT_MEM_CP:
                    DEMOStatOSReport("%s: %8d\n", cpReq);
                    break;
                  case DEMO_STAT_MEM_TC:
                    DEMOStatOSReport("%s: %8d\n", tcReq);
                    break;
                  case DEMO_STAT_MEM_CPUR:
                    DEMOStatOSReport("%s: %8d\n", cpuRdReq);
                    break;
                  case DEMO_STAT_MEM_CPUW:
                    DEMOStatOSReport("%s: %8d\n", cpuWrReq);
                    break;
                  case DEMO_STAT_MEM_DSP:
                    DEMOStatOSReport("%s: %8d\n", dspReq);
                    break;
                  case DEMO_STAT_MEM_IO:
                    DEMOStatOSReport("%s: %8d\n", ioReq);
                    break;
                  case DEMO_STAT_MEM_VI:
                    DEMOStatOSReport("%s: %8d\n", viReq);
                    break;
                  case DEMO_STAT_MEM_PE:
                    DEMOStatOSReport("%s: %8d\n", peReq);
                    break;
                  case DEMO_STAT_MEM_RF:
                    DEMOStatOSReport("%s: %8d\n", rfReq);
                    break;
                  case DEMO_STAT_MEM_FI:
                    DEMOStatOSReport("%s: %8d\n", fiReq);
                    break;
                }
                break;

              default:
                DEMOStatOSReport("%s: %8d\n", DemoStat[i].count);
                break;
            }
        }
    }
    else  // dump to screen
    {
        rmode = DEMOGetRenderModeObj();

        switch (DemoStatDisp)
        {
          case DEMO_STAT_TL:
            // text origin is top-left
            text_x = DEMO_TEXT_LFT; 
            text_y = DEMO_TEXT_TOP;
            text_yinc = DEMO_CHAR_HT + DEMO_CHAR_YSP;
            wd = rmode->fbWidth;
            ht = rmode->xfbHeight;
            break;
          case DEMO_STAT_BL:
            // text origin is bottom-left
            text_x = DEMO_TEXT_LFT; 
            text_y = (s16)(rmode->xfbHeight - DEMO_TEXT_BOT - DEMO_CHAR_HT);
            text_yinc = -(DEMO_CHAR_HT + DEMO_CHAR_YSP);
            wd = rmode->fbWidth;
            ht = rmode->xfbHeight;
            break;
          case DEMO_STAT_TLD:
            // double-sized fonts, text origin is top-left
            text_x = (s16)(DEMO_TEXT_LFT / 2);
            text_y = (s16)(DEMO_TEXT_TOP / 2);
            text_yinc = DEMO_CHAR_HT + DEMO_CHAR_YSP / 2;
            wd = (u16)(rmode->fbWidth / 2);
            ht = (u16)(rmode->xfbHeight / 2);
            break;
          case DEMO_STAT_BLD:
            // double-sized fonts, text origin is bottom-left
            text_x = (s16)(DEMO_TEXT_LFT / 2);
            text_y = (s16)((rmode->xfbHeight - DEMO_TEXT_BOT - DEMO_CHAR_HT) / 2);
            text_yinc = -(DEMO_CHAR_HT + DEMO_CHAR_YSP / 2);
            wd = (u16)(rmode->fbWidth / 2);
            ht = (u16)(rmode->xfbHeight / 2);
            break;
        }

        // Init DEMOPuts library fonts
        DEMOInitCaption(DM_FT_OPQ, wd, ht);

        for ( i = 0; i < DemoStatMaxIndx; i++ )
        {
            switch (DemoStat[i].stat_type)
            {
              case DEMO_STAT_PIX:
                // Pix Metric
                switch(DemoStat[i].stat)
                {
                  case DEMO_STAT_PIX_TI:
                    DEMOStatPrintf("%s: %8d\n", topPixIn);
                    break;
                  case DEMO_STAT_PIX_TO:
                    DEMOStatPrintf("%s: %8d\n", topPixOut);
                    break;
                  case DEMO_STAT_PIX_BI:
                    DEMOStatPrintf("%s: %8d\n", botPixIn);
                    break;
                  case DEMO_STAT_PIX_BO:
                    DEMOStatPrintf("%s: %8d\n", botPixOut);
                    break;
                  case DEMO_STAT_PIX_CI:
                    DEMOStatPrintf("%s: %8d\n", clrPixIn);
                    break;
                  case DEMO_STAT_PIX_CC:
                    DEMOStatPrintf("%s: %8d\n", copyClks);
                }
                break;

              case DEMO_STAT_FR:
                // Fill rate (MPixels/Sec)
                rate = FLIPPER_CLOCK * (f32)(topPixIn + botPixIn) /
                                       (f32)(DemoStatClocks - copyClks);
                DEMOStatPrintf("%s: %8.2f\n", rate);
                break;

              case DEMO_STAT_TBW:
                // Texture bandwidth (MB/sec)
                rate = FLIPPER_CLOCK * (f32)(tcReq*32) /
                                       (f32)(DemoStatClocks - copyClks);
                DEMOStatPrintf("%s: %8.2f\n", rate);
                break;

              case DEMO_STAT_TBP:
                // Texture bandwidth per pixel
                rate = (f32)(tcReq*32) /
                       (f32)(topPixIn - botPixIn);
                DEMOStatPrintf("%s: %8.3f\n", rate);
                break;

              case DEMO_STAT_VC:
                // VertexCache Metric
                switch(DemoStat[i].stat)
                {
                  case DEMO_STAT_VC_CHK:
                    DEMOStatPrintf("%s: %8d\n", vcCheck);
                    break;
                  case DEMO_STAT_VC_MISS:
                    DEMOStatPrintf("%s: %8d\n", vcMiss);
                    break;
                  case DEMO_STAT_VC_STALL:
                    DEMOStatPrintf("%s: %8d\n", vcStall);
                }
                break;

              case DEMO_STAT_MEM:
                // Mem Access Metric
                switch(DemoStat[i].stat)
                {
                  case DEMO_STAT_MEM_CP:
                    DEMOStatPrintf("%s: %8d\n", cpReq);
                    break;
                  case DEMO_STAT_MEM_TC:
                    DEMOStatPrintf("%s: %8d\n", tcReq);
                    break;
                  case DEMO_STAT_MEM_CPUR:
                    DEMOStatPrintf("%s: %8d\n", cpuRdReq);
                    break;
                  case DEMO_STAT_MEM_CPUW:
                    DEMOStatPrintf("%s: %8d\n", cpuWrReq);
                    break;
                  case DEMO_STAT_MEM_DSP:
                    DEMOStatPrintf("%s: %8d\n", dspReq);
                    break;
                  case DEMO_STAT_MEM_IO:
                    DEMOStatPrintf("%s: %8d\n", ioReq);
                    break;
                  case DEMO_STAT_MEM_VI:
                    DEMOStatPrintf("%s: %8d\n", viReq);
                    break;
                  case DEMO_STAT_MEM_PE:
                    DEMOStatPrintf("%s: %8d\n", peReq);
                    break;
                  case DEMO_STAT_MEM_RF:
                    DEMOStatPrintf("%s: %8d\n", rfReq);
                    break;
                  case DEMO_STAT_MEM_FI:
                    DEMOStatPrintf("%s: %8d\n", fiReq);
                    break;
                }
                break;

              case DEMO_STAT_GP0:
              case DEMO_STAT_GP1:
              case DEMO_STAT_MYC:
                DEMOStatPrintf("%s: %8d", DemoStat[i].count);
                break;

              case DEMO_STAT_MYR: // use stat as a second input
                rate = (f32) DemoStat[i].stat / (f32) DemoStat[i].count;
                DEMOStatPrintf("%s: %8.3f", rate);
                break;

              default:
                OSReport("Undefined stat type %d in DEMOPrintStats()\n", 
                         DemoStat[i].stat_type);
                break;
            }

            // update current line
            text_y += text_yinc;
        }
    } // screen dump
}

/*===========================================================================*/
