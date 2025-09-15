/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     G2D.c (Dolphin 2D API by Paul Donnelly, Nov. 1999)

  Copyright 1998, 1999 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/libraries/G2D/src/G2D.c $
    
    5     3/24/00 4:08p Carl
    Adjusted screen height for overscan
    
    4     1/18/00 7:54p Hirose
    added GXSetNumChans() and GXSetTevOrder() calls to work with
    new GXInit() defaults
    
    3     99/12/14 7:58p Paul
    
    2     99/12/12 10:08p Paul
    
    1     99/12/09 12:30p Paul

 *---------------------------------------------------------------------------*/

#include <math.h>
#include "G2D.h"

/*---------------------------------------------------------------------------*
  Defines
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
   Global variables
 *---------------------------------------------------------------------------*/

#define nScrX 640
#define nScrY 448

static struct
{
    u16        nViewportTlcX;
    u16        nViewportTlcY;
    u16        nViewportWidth;
    u16        nViewportHeight;

    G2DPosOri  poCam;

    f32        rWorldX;
    f32        rWorldY;
    f32        rHalfX;
    f32        rHalfY;
}
glob;


/*---------------------------------------------------------------------------*
    Name:           G2DInitSprite

    Description:	Precalculates sprite texture coordinates

    Arguments:      G2DSprite *sprite      Pointer to sprite

    Returns:        none
 *---------------------------------------------------------------------------*/
void G2DInitSprite( G2DSprite *sprite )
{
    f32 rInvWidth = 1.0F / GXGetTexObjWidth(sprite->to);
    f32 rInvHeight = 1.0F / GXGetTexObjHeight(sprite->to);;

    sprite->rS0 = ((f32)sprite->nTlcS + 0.5F) * rInvWidth;
    sprite->rS1 = ((f32)sprite->nTlcS + sprite->nWidth - 0.5F) * rInvWidth;
    sprite->rT0 = ((f32)sprite->nTlcT + 0.5F) * rInvHeight;
    sprite->rT1 = ((f32)sprite->nTlcT + sprite->nHeight - 0.5F) * rInvHeight;
}


/*---------------------------------------------------------------------------*
    Name:           DrawSprite

    Description:	Draws sprite (Tiles with alpha)

    Arguments:      none

    Returns:        none
 *---------------------------------------------------------------------------*/
void G2DDrawSprite( G2DSprite *sprite, G2DPosOri *po )
{
    f32 rOX, rOY;
    f32 rWX, rWY, rHX, rHY;
    f32 rRelX, rRelY;

    GXClearVtxDesc();
    GXLoadTexObj(sprite->to, GX_TEXMAP0);

    // Set Position Params
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);

    // Set Tex Coord Params
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetNumTexGens(1);
    GXSetNumChans(0);

    // Turn on alpha blending
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

    rOX = po->rOriX * 0.5F;
    rOY = po->rOriY * 0.5F;

    rWX = sprite->nWidth * rOX;
    rWY = sprite->nWidth * rOY;
    rHX = sprite->nHeight * rOX;
    rHY = sprite->nHeight * rOY;

    rRelX = po->rPosX - glob.poCam.rPosX;
    rRelY = po->rPosY - glob.poCam.rPosY;

    if (rRelX >= glob.rHalfX) rRelX -= glob.rWorldX;
    if (rRelX < -glob.rHalfX) rRelX += glob.rWorldX;
    if (rRelY >= glob.rHalfY) rRelY -= glob.rWorldY;
    if (rRelY < -glob.rHalfY) rRelY += glob.rWorldY;

    rRelX += glob.poCam.rPosX;
    rRelY += glob.poCam.rPosY;

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    {
        GXPosition2f32( rRelX - rHX + rWY, rRelY - rHY - rWX );
        GXTexCoord2f32( sprite->rS0, sprite->rT1 );

        GXPosition2f32( rRelX + rHX + rWY, rRelY + rHY - rWX );
        GXTexCoord2f32( sprite->rS0, sprite->rT0 );

        GXPosition2f32( rRelX + rHX - rWY, rRelY + rHY + rWX  );
        GXTexCoord2f32( sprite->rS1, sprite->rT0 );

        GXPosition2f32( rRelX - rHX - rWY, rRelY - rHY + rWX  );
        GXTexCoord2f32( sprite->rS1, sprite->rT1 );
    }
    GXEnd();
}


/*---------------------------------------------------------------------------*
    Name:           FillSection

    Description:    Scanline converts from a tile map to a sort buffer
                    Handles 4 cases separately: 8bit/16bit index, Wrap/No-wrap

    Arguments:      lots!
                    (but the function is inline so that should avoid the
                     parameter-passing overhead)

    Returns:        none
 *---------------------------------------------------------------------------*/
static inline void FillSection( G2DLayer *layer, s8 *aSortBuffer,
                                s32 *nScanLine, s32 nEvent, s16 *nIdx,
                                s32 *nL, s32 *nR, f32 *rLeft, f32 *rRight,
                                f32 rStep0, f32 rStep1, s32 nMapX, s32 nMapY )
{
    s32 nHMask = (1<<layer->nHS)-1;
    s32 nVMask = (1<<layer->nVS)-1;
    s32 nI, nJ, nK, nM;
    s16 nMaterial;
    s16 *pAddr;

    if (layer->nBPI == 1)
    {
        u8 nTile;   // 8 bits per index

        if (layer->bWrap)   // layer wraps
        {
            // scan from top to bottom
            for(; *nScanLine <= nEvent; (*nScanLine)++)
            {
                nJ = *nScanLine - 1;
                nK = ((nJ + nMapY) & nVMask) << layer->nHS;

                // scan from Left to Right
                for(nI=*nL; nI<=*nR; nI++)
                {
                    nTile = ((u8 *)layer->map)[nK + ((nI + nMapX) & nHMask)];
                    nMaterial = layer->tileDesc[nTile].nMaterial;
                    pAddr = (s16 *)&layer->matDesc[nMaterial].nReserved;

                    pAddr[1]++;             // increment count
                    if (*pAddr != *nIdx)    // check continuity
                    {
                        *nIdx += 2;
                        *((s16 *)&aSortBuffer[*pAddr]) = (s16)(*pAddr - *nIdx);
                    }
                    *((u16 *)&aSortBuffer[*nIdx]) = (u16)nTile;
                    *nIdx += 2;
                    aSortBuffer[(*nIdx)++] = (s8)nI;
                    aSortBuffer[(*nIdx)++] = (s8)nJ;
                    *pAddr = *nIdx;
                }
                *rLeft += rStep0;
                *rRight += rStep1;
                *nL = (s32)floor(*rLeft);
                *nR = (s32)floor(*rRight);
            }
        }
        else // 8 bits per index, layer doesn't wrap
        {
            // scan from top to bottom
            for(; *nScanLine <= nEvent; (*nScanLine)++)
            {
                nJ = *nScanLine - 1 + nMapY;
                if (nJ<0)
                {
                    nK = 0;
                }
                else if (nJ>nVMask)
                {
                    nK = nVMask << layer->nHS;
                }
                else
                {
                    nK = nJ << layer->nHS;
                }

                nM = *nR+nMapX;
                // scan from Left to Right
                for(nI=(*nL+nMapX); nI<=nM; nI++)
                {
                    if (nI<0)
                    {
                        nTile = ((u8 *)layer->map)[nK];
                    }
                    else if (nI>nHMask)
                    {
                        nTile = ((u8 *)layer->map)[nK + nHMask];
                    }
                    else
                    {
                        nTile = ((u8 *)layer->map)[nK + nI];
                    }

                    nMaterial = layer->tileDesc[nTile].nMaterial;
                    pAddr = (s16 *)&layer->matDesc[nMaterial].nReserved;

                    pAddr[1]++;             // increment count
                    if (*pAddr != *nIdx)    // check continuity
                    {
                        *nIdx += 2;
                        *((s16 *)&aSortBuffer[*pAddr]) = (s16)(*pAddr - *nIdx);
                    }
                    *((u16 *)&aSortBuffer[*nIdx]) = (u16)nTile;
                    *nIdx += 2;
                    aSortBuffer[(*nIdx)++] = (s8)(nI - nMapX);
                    aSortBuffer[(*nIdx)++] = (s8)(nJ - nMapY);
                    *pAddr = *nIdx;
                }
                *rLeft += rStep0;
                *rRight += rStep1;
                *nL = (s32)floor(*rLeft);
                *nR = (s32)floor(*rRight);
            }
        }
    }
    else
    {
        u16 nTile;  // 16 bits per index

        if (layer->bWrap)   // layer wraps
        {
            // scan from top to bottom
            for(; *nScanLine <= nEvent; (*nScanLine)++)
            {
                nJ = *nScanLine - 1;
                nK = ((nJ + nMapY) & nVMask) << layer->nHS;

                // scan from Left to Right
                for(nI=*nL; nI<=*nR; nI++)
                {
                    nTile = ((u16 *)layer->map)[nK + ((nI + nMapX) & nHMask)];
                    nMaterial = layer->tileDesc[nTile].nMaterial;
                    pAddr = (s16 *)&layer->matDesc[nMaterial].nReserved;

                    pAddr[1]++;             // increment count
                    if (*pAddr != *nIdx)    // check continuity
                    {
                        *nIdx += 2;
                        *((s16 *)&aSortBuffer[*pAddr]) = (s16)(*pAddr - *nIdx);
                    }
                    *((u16 *)&aSortBuffer[*nIdx]) = (u16)nTile;
                    *nIdx += 2;
                    aSortBuffer[(*nIdx)++] = (s8)nI;
                    aSortBuffer[(*nIdx)++] = (s8)nJ;
                    *pAddr = *nIdx;
                }
                *rLeft += rStep0;
                *rRight += rStep1;
                *nL = (s32)floor(*rLeft);
                *nR = (s32)floor(*rRight);
            }
        }
        else // 16 bits per index, layer doesn't wrap
        {
            // scan from top to bottom
            for(; *nScanLine <= nEvent; (*nScanLine)++)
            {
                nJ = *nScanLine - 1 + nMapY;
                if (nJ<0)
                {
                    nK = 0;
                }
                else if (nJ>nVMask)
                {
                    nK = nVMask << layer->nHS;
                }
                else
                {
                    nK = nJ << layer->nHS;
                }

                nM = *nR+nMapX;
                // scan from Left to Right
                for(nI=*nL+nMapX; nI<=nM; nI++)
                {
                    if (nI<0)
                    {
                        nTile = ((u16 *)layer->map)[nK];
                    }
                    else if (nI>nHMask)
                    {
                        nTile = ((u16 *)layer->map)[nK + nHMask];
                    }
                    else
                    {
                        nTile = ((u16 *)layer->map)[nK + nI];
                    }

                    nMaterial = layer->tileDesc[nTile].nMaterial;
                    pAddr = (s16 *)&layer->matDesc[nMaterial].nReserved;

                    pAddr[1]++;             // increment count
                    if (*pAddr != *nIdx)    // check continuity
                    {
                        *nIdx += 2;
                        *((s16 *)&aSortBuffer[*pAddr]) = (s16)(*pAddr - *nIdx);
                    }
                    *((u16 *)&aSortBuffer[*nIdx]) = (u16)nTile;
                    *nIdx += 2;
                    aSortBuffer[(*nIdx)++] = (s8)(nI - nMapX);
                    aSortBuffer[(*nIdx)++] = (s8)(nJ - nMapY);
                    *pAddr = *nIdx;
                }
                *rLeft += rStep0;
                *rRight += rStep1;
                *nL = (s32)floor(*rLeft);
                *nR = (s32)floor(*rRight);
            }
        }
    }
}


/*---------------------------------------------------------------------------*
    Name:           G2DDrawLayer

    Description:    Draw a background layer.  

    Arguments:      G2DLayer  *layer;
                    s8        *aSortBuffer
    Returns:        none
 *---------------------------------------------------------------------------*/
void G2DDrawLayer( G2DLayer *layer, s8 *aSortBuffer )
{
    s16 *pAddr;
    s16 aCount0 = 0;
    s16 aCount1 = 0;
    s16 aCount2 = 0;
    // void *map, s32 nBPI, s32 nHS, s32 nVS,
    // s32 bWrap, TileDesc tileDesc, 

    //s32 nWidth = 1<<layer->nHS;
    //s32 nHeight = 1<<layer->nVS;

    f32 rInvTileWidth = 1.0F / layer->nTileWidth;
    f32 rInvTileHeight = 1.0F / layer->nTileHeight;

    s16 nIdx;
    s32 nI, nJ, nK, nL, nR;
    f32 rX, rY;
    f32 rTlcX, rTrcX, rBlcX, rBrcX;
    f32 rTlcY, rTrcY, rBlcY, rBrcY;
    s32 nScanLine;
    f32 rLeft, rRight, rLeftY, rRightY;
    f32 rStep0, rStep1, rMid;
    s32 nEvent0, nEvent1, nEvent2;
    f32 rFrcX, rFrcY;
    s32 nMapX, nMapY;
    s32 nLocalMapX, nLocalMapY;
    f32 rCamOriX, rCamOriY;
    s16 nTile, nMaterial;

    // Turn on alpha blending always
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

    // Initialize NumTextures + 2 streams for sorting by texture/color
    for( nI=0; nI<layer->nNumMaterials; nI++)
    {
        pAddr = (s16 *)&layer->matDesc[nI].nReserved;

        pAddr[0] = (s16)(nI<<1);    // idx
        pAddr[1] = 0;               // count 
    }
    nIdx = (s16)((nI-1)<<1);

    {
        rFrcX = glob.poCam.rPosX * rInvTileWidth;
        rFrcY = glob.poCam.rPosY * rInvTileHeight;

        nMapX = (s32)rFrcX;
        nMapY = (s32)rFrcY;

        rFrcX -= nMapX;
        rFrcY -= nMapY;
    }

    //  Avoid the need for special cases when the screen is axis aligned
    //  by rotating the screen slightly
    rCamOriX = glob.poCam.rOriX;
    rCamOriY = glob.poCam.rOriY;

    if ((rCamOriX < 0.0001) && (rCamOriX > -0.0001) )
    {
        rCamOriX = 0.0001F;  // This is an approximation to a small rotation
    }
    else if ((rCamOriY < 0.0001) && (rCamOriY > -0.0001) )
    {
        rCamOriY = 0.0001F;
    }

    rX = glob.nViewportWidth  * 0.5F;
    rY = glob.nViewportHeight * 0.5F;

    //  scan-line convert a rectangle for the screen to find the minimal
    //  set of tiles that need to be rendered.

    rTlcX = rFrcX + (((rY * rCamOriX) + (rX * rCamOriY)) * rInvTileWidth);
    rTlcY = rFrcY + (((rY * rCamOriY) - (rX * rCamOriX)) * rInvTileHeight);

    rTrcX = rFrcX + (((rY * rCamOriX) - (rX * rCamOriY)) * rInvTileWidth);
    rTrcY = rFrcY + (((rY * rCamOriY) + (rX * rCamOriX)) * rInvTileHeight);

    rBlcX = rFrcX - (((rY * rCamOriX) - (rX * rCamOriY)) * rInvTileWidth);
    rBlcY = rFrcY - (((rY * rCamOriY) + (rX * rCamOriX)) * rInvTileHeight);

    rBrcX = rFrcX - (((rY * rCamOriX) + (rX * rCamOriY)) * rInvTileWidth);
    rBrcY = rFrcY - (((rY * rCamOriY) - (rX * rCamOriX)) * rInvTileHeight);

    // Sort the corners on Y

    if (rCamOriY < 0)
    {
        if (rCamOriX >= 0)
        {
            nScanLine = 1 + (s32)floor(rTlcY);
            rY = (nScanLine - rTlcY);  // Fraction of Y
            rLeft = rTlcX;
            rLeftY = rBlcY;
            rRightY = rTrcY;
            nEvent2 = (s32)floor(rBrcY);

            rStep0 = rCamOriX / rCamOriY;
            rStep1 = -rCamOriY / rCamOriX;
        }
        else
        {
            nScanLine = 1 + (s32)floor(rTrcY);
            rY = (nScanLine - rTrcY);
            rLeft = rTrcX;
            rLeftY = rTlcY;
            rRightY = rBrcY;
            nEvent2 = (s32)floor(rBlcY);

            rStep0 = -rCamOriY / rCamOriX;       // ALERT: Assumes width==height
            rStep1 = rCamOriX / rCamOriY;
        }
    }
    else
    {
        if (rCamOriX >= 0)
        {
            nScanLine = 1 + (s32)floor(rBlcY);
            rY = (nScanLine - rBlcY);  // Fraction of Y
            rLeft = rBlcX;
            rLeftY = rBrcY;
            rRightY = rTlcY;
            nEvent2 = (s32)floor(rTrcY);

            rStep0 = -rCamOriY / rCamOriX;       // ALERT: Assumes width==height
            rStep1 = rCamOriX / rCamOriY;
        }
        else
        {
            nScanLine = 1 + (s32)floor(rBrcY);
            rY = (nScanLine - rBrcY);
            rLeft = rBrcX;
            rLeftY = rTrcY;
            rRightY = rBlcY;
            nEvent2 = (s32)floor(rTlcY);

            rStep0 = rCamOriX / rCamOriY;       // ALERT: Assumes width==height
            rStep1 = -rCamOriY / rCamOriX;
        }
    }

    {
        f32 rRatio = (f32) layer->nTileHeight / layer->nTileWidth;
        rStep0 *= rRatio;
        rStep1 *= rRatio;
    }

    rRight = rLeft + rY * rStep1;
    rLeft += rY * rStep0;

    // Scanline Rasterization algorithm
    // Top line is a special case
    if (rLeftY < rRightY)
    {
        nEvent0 = (s32)floor(rLeftY);
        nEvent1 = (s32)floor(rRightY);
        rMid = rStep1;
    }
    else
    {
        nEvent0 = (s32)floor(rRightY);
        nEvent1 = (s32)floor(rLeftY);
        rMid = rStep0;
    }

    nL = (s32)floor(rLeft);
    nR = (s32)floor(rRight);

    nLocalMapX = nMapX;
    nLocalMapY = nMapY;

    if (!(layer->bWrap))  // Special care is needed when map doesn't wrap
    {
        f32 rInvTileWidth = 1.0F / layer->nTileWidth;
        f32 rInvTileHeight = 1.0F / layer->nTileHeight;
        f32 rLocalPosX = glob.poCam.rPosX;
        f32 rLocalPosY = glob.poCam.rPosY;
        f32 rSplitX = 0.5F * (glob.rWorldX + (layer->nTileWidth * (1<<layer->nHS)));
        f32 rSplitY = 0.5F * (glob.rWorldY + (layer->nTileHeight * (1<<layer->nVS)));

        if (rLocalPosX >= rSplitX) rLocalPosX -= glob.rWorldX;
        if (rLocalPosY >= rSplitY) rLocalPosY -= glob.rWorldY;

        rFrcX = rLocalPosX * rInvTileWidth;
        rFrcY = rLocalPosY * rInvTileHeight;

        nLocalMapX = (s32)floor(rFrcX);
        nLocalMapY = (s32)floor(rFrcY);
    }

    FillSection(layer, aSortBuffer, &nScanLine, nEvent0, &nIdx, &nL, &nR, &rLeft, &rRight, rStep0, rStep1, nLocalMapX, nLocalMapY);

    {
        pAddr = (s16 *)&layer->matDesc[0].nReserved;
        aCount0 = pAddr[1];
        pAddr = (s16 *)&layer->matDesc[1].nReserved;
        aCount1 = pAddr[1];
        pAddr = (s16 *)&layer->matDesc[2].nReserved;
        aCount2 = pAddr[1];
    }

    // The case where Event0 == Event1 must be handled
    if (nScanLine > rLeftY)
    {
        rLeft -= (nScanLine - rLeftY) * rStep0;
        nL = (s32)floor(rLeft);
        rLeft += (nScanLine - rLeftY - 1) * rStep1;
        rLeftY = 1000;      // make sure the test further down fails
    }

    if (nScanLine > rRightY)
    {
        rRight -= (nScanLine - rRightY) * rStep1;
        nR = (s32)floor(rRight);
        rRight += (nScanLine - rRightY - 1) * rStep0;
        rRightY = 1000;      // make sure the test further down fails
    }

    FillSection(layer, aSortBuffer, &nScanLine, nEvent1, &nIdx, &nL, &nR, &rLeft, &rRight, rMid, rMid, nLocalMapX, nLocalMapY);

    {
        pAddr = (s16 *)&layer->matDesc[0].nReserved;
        aCount0 = pAddr[1];
        pAddr = (s16 *)&layer->matDesc[1].nReserved;
        aCount1 = pAddr[1];
        pAddr = (s16 *)&layer->matDesc[2].nReserved;
        aCount2 = pAddr[1];
    }

    if (nScanLine > rLeftY)
    {
        rLeft -= (nScanLine - rLeftY) * rStep0;
        nL = (s32)floor(rLeft);
        rLeft += (nScanLine - rLeftY - 1) * rStep1;
    }

    if (nScanLine > rRightY)
    {
        rRight -= (nScanLine - rRightY) * rStep1;
        nR = (s32)floor(rRight);
        rRight += (nScanLine - rRightY - 1) * rStep0;
    }

    FillSection(layer, aSortBuffer, &nScanLine, nEvent2+1, &nIdx, &nL, &nR, &rLeft, &rRight, rStep1, rStep0, nLocalMapX, nLocalMapY);

    {
        pAddr = (s16 *)&layer->matDesc[0].nReserved;
        aCount0 = pAddr[1];
        pAddr = (s16 *)&layer->matDesc[1].nReserved;
        aCount1 = pAddr[1];
        pAddr = (s16 *)&layer->matDesc[2].nReserved;
        aCount2 = pAddr[1];
    }

    // We now have the tiles sorted into streams by material:

    for( nMaterial=0; nMaterial<layer->nNumMaterials; nMaterial++)
    {
        pAddr = (s16 *)&layer->matDesc[nMaterial].nReserved;

        if (pAddr[1] == 0)  // count 
        {
            continue;
        }

        switch( layer->matDesc[nMaterial].nCategory )
        {
            case G2D_CTG_EMPTY:
            {
                continue;   // Empty tiles don't need to be rendered
            }

            case G2D_CTG_RGBA_INDEX8:
            {
                // Colored tiles using color index
                GXClearVtxDesc();

                // Set Position Params
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0); //PositionShift);
                GXSetVtxDesc(GX_VA_POS, GX_DIRECT);

                // Set Color for Vertex Format 0
                GXSetNumTexGens(0);
                GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
                GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
                GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
                GXSetArray(GX_VA_CLR0, layer->matDesc[nMaterial].clut, 4);

                GXSetNumChans(1);
                GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX,
                              GX_LIGHT0, GX_DF_NONE, GX_AF_NONE);

                nIdx = (s16)(nMaterial<<1);
                GXBegin(GX_QUADS, GX_VTXFMT0, (u16)(pAddr[1]<<2));
                for(nK = (s32)pAddr[1]; nK--;)
                {
                    f32 rI, rJ;
                    u8 nCI;

                    nTile = *((s16 *)&aSortBuffer[nIdx]);
                    if (nTile < 0)
                    {
                        nIdx -= nTile;
                        nTile = *((s16 *)&aSortBuffer[nIdx]);
                    }
                    nIdx+=2;
                    nI = aSortBuffer[nIdx++];
                    nJ = aSortBuffer[nIdx++];

                    rI = (f32)(nI + nMapX) * layer->nTileWidth;
                    rJ = (f32)(nJ + nMapY) * layer->nTileHeight;

                    nCI = layer->tileDesc[nTile].nCI;

                    GXPosition2f32( rI + (f32)layer->nTileWidth, rJ     );
                    GXColor1x8( nCI );

                    GXPosition2f32( rI + (f32)layer->nTileWidth, rJ + (f32)layer->nTileHeight );
                    GXColor1x8( nCI );

                    GXPosition2f32( rI                     , rJ + (f32)layer->nTileHeight );
                    GXColor1x8( nCI );

                    GXPosition2f32( rI                     , rJ      );
                    GXColor1x8( nCI );
                }
                GXEnd();
                break;
            }

            case G2D_CTG_RGB_DIRECT:
            {
                // Colored tiles using color index
                GXClearVtxDesc();

                // Set Position Params
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0); //PositionShift);
                GXSetVtxDesc(GX_VA_POS, GX_DIRECT);

                // Set Color for Vertex Format 0
                GXSetNumTexGens(0);
                GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
                GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGB, GX_RGB8, 0);
                GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);

                GXSetNumChans(1);
                GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX,
                              GX_LIGHT0, GX_DF_NONE, GX_AF_NONE);

                nIdx = (s16)(nMaterial<<1);
                GXBegin(GX_QUADS, GX_VTXFMT0, (u16)(pAddr[1]<<2));
                for(nK = (s32)pAddr[1]; nK--;)
                {
                    f32 rI, rJ;
                    u8 nR, nG, nB;

                    nTile = *((s16 *)&aSortBuffer[nIdx]);
                    if (nTile < 0)
                    {
                        nIdx -= nTile;
                        nTile = *((s16 *)&aSortBuffer[nIdx]);
                    }
                    nIdx+=2;
                    nI = aSortBuffer[nIdx++];
                    nJ = aSortBuffer[nIdx++];

                    rI = (f32)(nI + nMapX) * layer->nTileWidth;
                    rJ = (f32)(nJ + nMapY) * layer->nTileHeight;

                    nR = layer->tileDesc[nTile].nS;
                    nG = layer->tileDesc[nTile].nT;
                    nB = layer->tileDesc[nTile].nCI;

                    GXPosition2f32( rI + (f32)layer->nTileWidth, rJ     );
                    GXColor3u8( nR, nG, nB );

                    GXPosition2f32( rI + (f32)layer->nTileWidth, rJ + (f32)layer->nTileHeight );
                    GXColor3u8( nR, nG, nB );

                    GXPosition2f32( rI                     , rJ + (f32)layer->nTileHeight );
                    GXColor3u8( nR, nG, nB );

                    GXPosition2f32( rI                     , rJ      );
                    GXColor3u8( nR, nG, nB );
                }
                GXEnd();
                break;
            }

            case G2D_CTG_TEXTURE:
            {
                f32 rInvTexWidth = 1.0F / GXGetTexObjWidth(layer->matDesc[nMaterial].to);
                f32 rInvTexHeight = 1.0F / GXGetTexObjHeight(layer->matDesc[nMaterial].to);
                f32 rWidth = layer->nTileWidth * rInvTexWidth;
                f32 rHeight = layer->nTileHeight * rInvTexHeight;
                f32 rS0 = 0; //0.5F * rInvTexWidth;
                f32 rT0 = 0; //0.5F * rInvTexHeight;
                f32 rS1 = rWidth;// - rInvTexWidth;
                f32 rT1 = rHeight;// - rInvTexHeight;

                // Second, the textured tiles
                GXClearVtxDesc();
                GXLoadTexObj(layer->matDesc[nMaterial].to, GX_TEXMAP0);

                // Set Position Params
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
                GXSetVtxDesc(GX_VA_POS, GX_DIRECT);

                // Set Tex Coord Params
                GXSetNumTexGens(1);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
                GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

                if (layer->matDesc[nMaterial].color)
                {
                    GXSetNumChans(1);
                    GXSetChanMatColor(GX_COLOR0A0, *layer->matDesc[nMaterial].color);
                    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG,
                                  GX_LIGHT0, GX_DF_NONE, GX_AF_NONE);
                    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
                    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
                }
                else
                {
                    GXSetNumChans(0);
                    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
                    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                }

                nIdx = (s16)(nMaterial<<1);
                GXBegin(GX_QUADS, GX_VTXFMT0, (u16)(pAddr[1]<<2));
                for(nK = (s32)pAddr[1]; nK--;)
                {
                    f32 rI, rJ, rS, rT;

                    nTile = *((s16 *)&aSortBuffer[nIdx]);
                    if (nTile < 0)
                    {
                        nIdx -= nTile;
                        nTile = *((s16 *)&aSortBuffer[nIdx]);
                    }
                    nIdx+=2;
                    nI = aSortBuffer[nIdx++];
                    nJ = aSortBuffer[nIdx++];

                    rS = (layer->tileDesc[nTile].nS * rWidth) + rS0;
                    rT = (layer->tileDesc[nTile].nT * rHeight) + rT0;
                    rI = (f32)(nI + nMapX) * layer->nTileWidth;
                    rJ = (f32)(nJ + nMapY) * layer->nTileHeight;

                    GXPosition2f32( rI + (f32)layer->nTileWidth, rJ     );
                    GXTexCoord2f32( rS + rS1               , rT     );

                    GXPosition2f32( rI + (f32)layer->nTileWidth, rJ + (f32)layer->nTileHeight );
                    GXTexCoord2f32( rS + rS1               , rT + rT1 );

                    GXPosition2f32( rI                     , rJ + (f32)layer->nTileHeight );
                    GXTexCoord2f32( rS                     , rT + rT1 );

                    GXPosition2f32( rI                     , rJ      );
                    GXTexCoord2f32( rS                     , rT      );
                }
                GXEnd();
                break;
            }
        }
    }
}


/*---------------------------------------------------------------------------*
    Name:           G2DSetCamera

    Description:    Set up a viewing matrix corresponding to a camera
                    at the supplied position and orientation

    Arguments:      G2DPosOri *po           Position and Orientation
    Returns:        none
 *---------------------------------------------------------------------------*/
void G2DSetCamera( G2DPosOri *po )
{
    Mtx mView;
    Vec vPos;
    Vec vUp;
    Vec vAt;
    f32 rX, rY;

    glob.poCam = *po;

    // Setup vertex transform (no need to transform normals for 2D)

    vUp.x = po->rOriX;
    vUp.y = po->rOriY;
    vUp.z = 0.0F;

    rX = ((nScrX - glob.nViewportWidth)>>1)  - glob.nViewportTlcX;
    rY = ((nScrY - glob.nViewportHeight)>>1) - glob.nViewportTlcY;

    vPos.x = po->rPosX - (vUp.x * rY) - (vUp.y * rX);
    vPos.y = po->rPosY + (vUp.x * rX) - (vUp.y * rY);
    vPos.z = -300.0F;

    vAt.x = vPos.x;
    vAt.y = vPos.y;
    vAt.z = 0.0F;

    MTXLookAt(mView, &vPos, &vUp, &vAt);
	GXLoadPosMtxImm(mView, GX_PNMTX0);
}


/*---------------------------------------------------------------------------*
    Name:           G2DInitWorld

    Description:    Initialize world

    Arguments:      u32 nWorldX    world width (in pixels)
                    u32 nWorldY    world height (in pixels)
    Returns:        none
 *---------------------------------------------------------------------------*/
void G2DInitWorld( u32 nWorldX, u32 nWorldY )
{
    Mtx44 mProjection;

    // Store world dimensions in global variables
    glob.rWorldX = (f32)nWorldX;
    glob.rWorldY = (f32)nWorldY;
    glob.rHalfX  = (f32)(nWorldX>>1);
    glob.rHalfY  = (f32)(nWorldY>>1);

    // Turn off Z buffer
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_TRUE);

    // Set up orthographic projection
    MTXOrtho(mProjection, nScrY>>1, -(nScrY>>1), -(nScrX>>1), nScrX>>1, 100, 1000);
    GXSetProjection(mProjection, GX_ORTHOGRAPHIC);
}


/*---------------------------------------------------------------------------*
    Name:           G2DSetViewport

    Description:    Set up viewport in screenspace

    Arguments:      u16 nLeft     Leftmost X coordinate of viewport (in pixels)
                    u16 nTop      Topmost Y coordinate of viewport (in pixels)
                    u16 nWidth    Width of viewport (in pixels)
                    u16 nHeight   Height of viewport (in pixels)
    Returns:        none
 *---------------------------------------------------------------------------*/
void G2DSetViewport( u16 nLeft, u16 nTop, u16 nWidth, u16 nHeight )
{
    // Set up internal global variable used by renderer
    glob.nViewportTlcX = nLeft;
    glob.nViewportTlcY = nTop;
    glob.nViewportWidth = nWidth;
    glob.nViewportHeight = nHeight;

    // Use scissoring to clip to the viewport
    GXSetScissor(nLeft, nTop, nWidth, nHeight);
}
