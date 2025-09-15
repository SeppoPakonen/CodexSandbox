/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     DEMOPuts.c

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/libraries/demo/src/DEMOPuts.c $
    
    11    10/27/00 3:47p Hirose
    fixed build flags
    
    10    7/07/00 7:09p Dante
    PC Compatibility
    
    9     6/13/00 5:17p Shiki
    Fixed DEMOPrintf().

    8     6/06/00 8:58p Carl
    Fixed text alignment problem.

    7     5/20/00 4:57a Hirose
    fixed TevOrder and NumChans for drawing captions

    6     5/17/00 9:08p Hirose
    deleted tab codes

    5     5/17/00 8:53p Hirose
    fixed pixel processing mode setting defined in DEMOInitCaption

    4     3/06/00 11:54a Alligator
    change GXSetDefaultMtx to GXSetCurrentMtx

    3     1/13/00 5:55p Alligator
    integrate with ArtX GX library code

    2     12/10/99 4:48p Carl
    Fixed Z compare stuff.

    21    11/12/99 4:31p Yasu
    Add GXSetNumTexGens

    20    11/09/99 7:47p Hirose
    added GXSetNumTexGens and GXSetTevOrder

    19    10/29/99 3:45p Hirose
    replaced GXSetTevStages(GXTevStageID) by GXSetNumTevStages(u8)

    18    10/13/99 6:38p Yasu
    Rollback 1 level

    16    9/30/99 10:32p Yasu
    Renamed and clean up functions and enums.

    15    9/29/99 10:29p Yasu
    Parameter of GXSetFogRangeAdj was changed

    14    9/28/99 6:56p Yasu
    Change calling parameter of DEMOInitCaption

    13    9/28/99 2:46a Yasu
    Add more initialize works in DEMOInitCaption

    12    9/27/99 11:03p Yasu
    Z scale was inversed in DEMOSetupScrnSpc

    11    9/24/99 6:47p Yasu
    Change the parameter of DEMOsetuoScrnSpc().

    10    9/24/99 6:34p Yasu
    Let DEMOSetupScrnSpc() out of DEMOInitCaption()

    9     9/21/99 5:24p Yasu
    Change function name cmMtxScreen -> DEMOMtxScreen.

    8     9/14/99 5:12p Yasu
    Fixed small bug.

    7     9/14/99 4:57p Yasu
    Move it into the demo library.
    Change APIs to simpler.

    6     9/10/99 6:38a Yasu
    Add small letters.
    Add some type cast to avoid warning message

    5     9/07/99 9:52p Yasu
    Change file name and prefix

    4     9/01/99 5:45p Ryan

    3     8/28/99 12:08a Yasu
    Change enum name GX_MAT_3x4 -> GX_MTX3x4

    2     8/26/99 7:55p Yasu
    Change function name.

    1     8/25/99 6:21p Yasu
    Initial version
  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <Dolphin/dolphin.h>
#include <stdio.h>
#include <stdarg.h>
#include "Demo.h"

//============================================================================
//  Font data
//============================================================================
static  s32             fontShift  = 0;
static  GXTexObj        fontTexObj;

//============================================================================
//  Functions
//============================================================================

/*---------------------------------------------------------------------------*
    Name:           DEMOSetFontType
    Description:    Set font type
    Arguments:      s32 attr: Font attribute id
    Returns:        none
 *---------------------------------------------------------------------------*/
void    DEMOSetFontType( s32 attr )
{
    // Font type
    switch ( attr )
    {
      case  DM_FT_RVS:
        // Reverse mode
        GXSetBlendMode( GX_BM_LOGIC, GX_BL_ZERO, GX_BL_ZERO, GX_LO_INVCOPY );
        break;

      case  DM_FT_XLU:
        // Translucent mode
        GXSetBlendMode( GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_CLEAR );
        break;

      case  DM_FT_OPQ:
      default:
        // Normal mode
        GXSetBlendMode( GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR );
        break;
    }
    return;
}

/*---------------------------------------------------------------------------*
    Name:           DEMOLoadFont
    Description:    Prepare font texture
    Arguments:      DMTexFlt   texFlt: DMTF_BILERP/DMTF_POINTSAMPLE
                    GXBool bilerp: if you need bilerp image, set this
    Returns:        none
 *---------------------------------------------------------------------------*/
void    DEMOLoadFont( GXTexMapID texMap, GXTexMtx texMtx, DMTexFlt texFlt )
{
    Mtx         fontTMtx;
    u16         width  = 64;
    u16         height = (u16)( (0x80-0x20)*8*8 / width );
#ifdef MACOS
    // set height to power of 2
    height = (u16)( 1u << ( 32 - __cntlzw( (u32)height ) ) );
#endif
#ifdef flagWIN32
	u16 target = 1;
	int i;
	static int converted = 0;

	while (target < height) target = target << 1;
	height = target;

	if (!converted) {
		for(i = 0; i < 768; i++) {
			EndianSwap32(&DEMOFontBitmap[i]);
		}
		converted = 1;
	}
#endif

    // set up and load texture object
    GXInitTexObj( &fontTexObj,          // obj
                  (void*)DEMOFontBitmap,// image_ptr
                  width,                // wight
                  height,               // height
                  GX_TF_I4,             // format
                  GX_CLAMP,             // wrap_s      (don't care)
                  GX_CLAMP,             // wrap_t      (don't care)
                  GX_FALSE );           // mipmap      (don't care)

    fontShift = 1;
    if ( texFlt == DMTF_POINTSAMPLE ){
        GXInitTexObjLOD( &fontTexObj,
                         GX_NEAR,       // min_filt
                         GX_NEAR,       // max_filt
                         0.0f,          // min_lod     (don't care)
                         0.0f,          // max_lod     (don't care)
                         0.0f,          // lod_bias    (don't care)
                         GX_DISABLE,    // bias_clamp  (don't care)
                         GX_FALSE,      // do_edge_lod (don't care)
                         GX_ANISO_1 );  // max_aniso   (don't care)
        fontShift = 0;
    }
    GXLoadTexObj( &fontTexObj, texMap );

    // set texture matrix to fit texture coordinate to texel (1:1)
    // set up texture coord-gen
#ifdef flagEMU
    MTXScale( fontTMtx, 1.0f/(float)width, 1.0f/(float)height, 1.0f );
#else
    MTXScale( fontTMtx, 1.0f/(float)(width-!fontShift),
                        1.0f/(float)(height-!fontShift), 1.0f );
#endif
    GXLoadTexMtxImm( fontTMtx, texMtx, GX_MTX2x4 );
    GXSetNumTexGens( 1 );
    GXSetTexCoordGen( GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, texMtx );

    return;
}

/*---------------------------------------------------------------------------*
    Name:           DEMOSetupScrnSpc
    Description:    Set matrices up to screen coordinates system
    Arguments:
    Returns:        none
 *---------------------------------------------------------------------------*/
void    DEMOSetupScrnSpc( s32 width, s32 height, float depth )
{
    Mtx44  pMtx;
    Mtx    mMtx;

    MTXOrtho( pMtx, 0.0f, (float)height, 0.0f, (float)width, 0.0f, -depth );
    GXSetProjection( pMtx, GX_ORTHOGRAPHIC );
    MTXIdentity( mMtx );
    GXLoadPosMtxImm( mMtx, GX_PNMTX0 );
    GXSetCurrentMtx( GX_PNMTX0 );

    return;
}

/*---------------------------------------------------------------------------*
    Name:           DEMOInitCaption
    Description:    Set rendering pipe to default caption drawing mode
    Arguments:
    Returns:        none
 *---------------------------------------------------------------------------*/
void    DEMOInitCaption( s32 font_type, s32 width, s32 height )
{
    // Set matrices
    DEMOSetupScrnSpc( width, height, 100.0f );

    // Set pixel processing mode
    /*
    GXSetCullMode( GX_CULL_NONE );
    GXSetCoPlanar( GX_DISABLE );
    GXSetColorUpdate( GX_ENABLE );
    GXSetDither( GX_DISABLE );
    GXSetAlphaUpdate( GX_DISABLE );
    GXSetAlphaCompare( GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0 );
    GXSetFog( GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){0} );
    GXSetFogRangeAdj( GX_DISABLE, 0, 0 );
    */
    GXSetZMode( GX_ENABLE, GX_ALWAYS, GX_ENABLE );

    // Set TEV parameters to "REPLACE COLOR"
    GXSetNumChans( 0 );
    GXSetNumTevStages( 1 );
    GXSetTevOp( GX_TEVSTAGE0, GX_REPLACE );
    GXSetTevOrder( GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL );

    // Load Font
    DEMOLoadFont( GX_TEXMAP0, GX_TEXMTX0, DMTF_POINTSAMPLE );

    // Set font attr to normal
    DEMOSetFontType( font_type );

    return;
}

/*---------------------------------------------------------------------------*
    Name:           DEMOPuts
    Description:    output strings until detects NULL code
    Arguments:      s16  x, y, z: coordinate at the origin
                    char *string: output strings
    Returns:        none
 *---------------------------------------------------------------------------*/
void    DEMOPuts( s16 x, s16 y, s16 z, const char* string )
{
    const char* str = string;
    s32   s, t;
    s32   c, w, len, i;

    // set up vertex descriptors
    GXClearVtxDesc();
    GXSetVtxDesc( GX_VA_POS,  GX_DIRECT );
    GXSetVtxDesc( GX_VA_TEX0, GX_DIRECT );
    GXSetVtxAttrFmt( GX_VTXFMT0, GX_VA_POS,  GX_POS_XYZ, GX_S16, 0 );
    GXSetVtxAttrFmt( GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST,  GX_S16, 1 );

    len = 0;
    while (1){

        // search visible letters
        c = *str ++;
        if ( ' ' <= c && c <= 0x7f )
        {
            len ++;
        }
        // check if end of line
        else
        {
            if ( len > 0 )
            {
                // put a line
                GXBegin( GX_QUADS, GX_VTXFMT0, (u16)(len*4) );
                for ( i = 0; i < len; i ++ )
                {
                    // draw a letter
                    w = string[i] - ' ';
                    s = ( w % 8 ) * 16 + fontShift;
                    t = ( w / 8 ) * 16 + fontShift;
                    GXPosition3s16( (s16)(i*8+x  ), (s16)(y   ), z );
                    GXTexCoord2s16( (s16)(s      ), (s16)(t   )    );
                    GXPosition3s16( (s16)(i*8+x+8), (s16)(y   ), z );
                    GXTexCoord2s16( (s16)(s+16   ), (s16)(t   )    );
                    GXPosition3s16( (s16)(i*8+x+8), (s16)(y+8 ), z );
                    GXTexCoord2s16( (s16)(s+16   ), (s16)(t+16)    );
                    GXPosition3s16( (s16)(i*8+x  ), (s16)(y+8 ), z );
                    GXTexCoord2s16( (s16)(s      ), (s16)(t+16)    );
                }
                GXEnd( );
                len = 0;
            }
            string = str;
            // continue if CR/LF
            if ( c == '\n' ) y += 8;
            // terminate if else
            else break;
        }
    }
    return;
}

/*---------------------------------------------------------------------------*
    Name:           DEMOPrintf
    Description:    output formatted strings
                    Strings must be shorter than 256 words.
    Arguments:      s16 x, y, z: coordinate at the origin.
                    char *fmt:   format string
                    ...:         any other parameter
    Returns:        none
 *---------------------------------------------------------------------------*/
void    DEMOPrintf( s16 x, s16 y, s16 z, const char* fmt, ... )
{
    va_list  vlist;
    char     buf[256];

    // Get output string
    va_start( vlist, fmt );
    vsprintf( buf, fmt, vlist );
    va_end( vlist );

    // Feed to puts
    DEMOPuts( x, y, z, buf );

    return;
}

/*---------------------------------------------------------------------------*
    Name:           DEMOMtxScreen
    Description:    Get screen-to-camera translation matrix
    Arguments:      Mtx    mmtx : result will be set
                    Mtx44  pmtx : projection matrix
                    float  vp_w : width of viewport
                    float  vp_h : height of viewport
                    float  near : near clip plane
    Returns:        none
 *---------------------------------------------------------------------------*/
void    DEMOMtxScreen( float mmtx[3][4], float pmtx[4][4],
                       float vp_w, float vp_h, float nnear )
{
    float winZ = nnear * 1.00000f;
    float winX = winZ / pmtx[0][0];
    float winY = winZ / pmtx[1][1];

    MTXIdentity( mmtx );
    mmtx[0][0] =   2.0f * winX / vp_w;
    mmtx[1][1] = - 2.0f * winY / vp_h;
    mmtx[2][2] = - 1.0f;
    mmtx[0][3] = - winX;
    mmtx[1][3] =   winY;
    mmtx[2][3] = - winZ;

    return;
}

/*======== End of DEMOPuts.c ========*/
