/*---------------------------------------------------------------------------*
  Project:  Dolphin Demo Library
  File:     tev-emu.h

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/gxdemo/include/tev-emu.h $
    
    1     3/06/00 12:03p Alligator
    move from gx/tests to demos/gxdemos and rename
    
    2     2/03/00 11:19p Hirose
    
    1     2/02/00 11:32p Hirose
    initial version
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
    tev-emu
        This is the application-level utility library for emulating some
        feature set of the Dolphin Texture Environment (TEV) under the
        MAC emulator which is not able to support actual TEV feature.
 *---------------------------------------------------------------------------*/

#ifndef	__TEV_EMU_H__
#define	__TEV_EMU_H__

/*---------------------------------------------------------------------------*/
#ifdef	__cplusplus
extern	"C" {
#endif

/*---------------------------------------------------------------------------*/
#include <Dolphin/dolphin.h>

/*---------------------------------------------------------------------------*
    Macro definitions
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    Function definitions
 *---------------------------------------------------------------------------*/
#ifdef  EPPC
/*---------------------------------------------------------------------------*/
    // for the actual GX (only macro definitions)
    #define  TESetTevOp(id, mode) \
             GXSetTevOp(id, mode)
    #define  TESetTevColorIn(stg, a, b, c, d) \
             GXSetTevColorIn(stg, a, b, c, d)
    #define  TESetTevAlphaIn(stg, a, b, c, d) \
             GXSetTevAlphaIn(stg, a, b, c, d)
    #define  TESetTevColorOp(stg, op, bias, scale, clamp, out) \
             GXSetTevColorOp(stg, op, bias, scale, clamp, out)
    #define  TESetTevAlphaOp(stg, op, bias, scale, clamp, out) \
             GXSetTevAlphaOp(stg, op, bias, scale, clamp, out)
    #define  TESetTevColor(id, color) \
             GXSetTevColor(id, color)
    #define  TESetTevColorS10(id, color) \
             GXSetTevColorS10(id, color)
    #define  TESetTevClampMode(stg, mode) \
             GXSetTevClampMode(stg, mode)
    #define  TESetAlphaCompare(comp0, ref0, op, comp1, ref1) \
             GXSetAlphaCompare(comp0, ref0, op, comp1, ref1)
    #define  TESetNumTevStages(nStages) \
             GXSetNumTevStages(nStages)
    /*
    #define  TESetTevOrder(stg, coord, map, color) \
             GXSetTevOrder(stg, coord, map, color)
    */
/*---------------------------------------------------------------------------*/
#else
/*---------------------------------------------------------------------------*/
    // for the emulator
    extern void TESetTevColorIn(
                    GXTevStageID  stage,
                    GXTevColorArg a,
                    GXTevColorArg b,
                    GXTevColorArg c,
                    GXTevColorArg d );
    extern void TESetTevAlphaIn(
                    GXTevStageID  stage,
                    GXTevAlphaArg a,
                    GXTevAlphaArg b,
                    GXTevAlphaArg c,
                    GXTevAlphaArg d );
    extern void TESetTevColorOp(
                    GXTevStageID  stage,
                    GXTevOp       op,
                    GXTevBias     bias,
                    GXTevScale    scale,
                    GXBool        clamp,
                    GXTevRegID    out_reg );
    extern void TESetTevAlphaOp(
                    GXTevStageID  stage,
                    GXTevOp       op,
                    GXTevBias     bias,
                    GXTevScale    scale,
                    GXBool        clamp,
                    GXTevRegID    out_reg );
    
    extern void TESetTevOp( GXTevStageID id, GXTevMode mode );
    extern void TESetTevColor( GXTevRegID id, GXColor color );
    extern void TESetTevColorS10( GXTevRegID id, GXColorS10 color );
    extern void TESetTevClampMode( GXTevStageID stage, GXTevClampMode mode );

    extern void TESetAlphaCompare(
                    GXCompare     comp0,
                    u8            ref0,
                    GXAlphaOp     op,
                    GXCompare     comp1,
                    u8            ref1 );
    /*
    extern void TESetTevOrder(
                    GXTevStageID  stage, 
                    GXTexCoordID  coord,
                    GXTexMapID    map,
                    GXChannelID   color );
    */

    extern void TESetNumTevStages( u8 nStages );


    // Pixel level emulation API
    extern void TESetStageInput(
                    GXTevStageID  stage,
                    GXColor*      rasColor,
                    GXColor*      texColor );
    extern void TECombineOnePixel( GXColor* result );

    // Image level emulation API
    extern void TEImgInit( void );
    extern void TEImgSetStageInput(
                    GXTevStageID  stage,
                    void*         rasImg,
                    void*         texImg );
    extern void TEImgSetSize( u16 width, u16 height );
    extern void TEImgCombine( void* resultImg );

/*---------------------------------------------------------------------------*/
#endif

/*---------------------------------------------------------------------------*/
#ifdef	__cplusplus
}
#endif

/*---------------------------------------------------------------------------*/
#endif	//__TEV_EMU_H__

/*============================================================================*/
