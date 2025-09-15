/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     raceSphereMap.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/charPipeline/src/raceSphereMap.c $
    
    3     8/11/00 10:42a Ryan
    final checkin for spaceworld
    
    2     8/08/00 12:30p Ryan
    update to fix errors/warnings
    
    1     7/29/00 11:17a Ryan
    initial checkin
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <charPipeline.h>
#include <demo.h>

#include <math.h>

#include "raceSphereMap.h"
#include "raceInit.h"
#include "raceCamera.h"
#include "raceModels.h"


/****************************************************************************/
 
#define SPHERE_MAP_RESOLUTION   8

#define SPHERE_MAP_SIZE         256
#define CUBE_MAP_SIZE           128

/****************************************************************************/

typedef struct
{
    float   sGrid[SPHERE_MAP_RESOLUTION];
    float   tGrid[SPHERE_MAP_RESOLUTION];

    float   sphereGeometryS[SPHERE_MAP_RESOLUTION][SPHERE_MAP_RESOLUTION];
    float   sphereGeometryT[SPHERE_MAP_RESOLUTION][SPHERE_MAP_RESOLUTION];

}SphereMapInfo;

typedef struct
{
    float   sGrid[SPHERE_MAP_RESOLUTION];
    float   tGrid[SPHERE_MAP_RESOLUTION];

    float   sphereGeometryS[SPHERE_MAP_RESOLUTION * 4];
    float   sphereGeometryT[SPHERE_MAP_RESOLUTION * 4];

    float   nrmSphereGeometryS[SPHERE_MAP_RESOLUTION * 4];
    float   nrmSphereGeometryT[SPHERE_MAP_RESOLUTION * 4];

}SphereBackMapInfo;

/****************************************************************************/

static SphereMapInfo Left, Right, Front, Top, Bottom;
static SphereBackMapInfo Back;

static Vec CubeCamLoc = {0.0F, 0.0F, 300.0F};

GXTexObj SphereMap;
static void* SphereMapData;

static GXTexObj TexObj1, TexObj2, TexObj3, TexObj4, TexObj5, TexObj6;

static void* CubeLeft;
static void* CubeRight;
static void* CubeTop;
static void* CubeBottom;
static void* CubeFront;
static void* CubeBack;

GXRenderModeObj *currentRenderMode = &GXNtsc480IntAa;

/****************************************************************************/
static void InitSphereMapFront  ( void );
static void InitSphereMapBack   ( void );
static void InitSphereMapLeft   ( void );
static void InitSphereMapRight  ( void );
static void InitSphereMapTop    ( void );
static void InitSphereMapBottom ( void );

static void InitSphereMapSide ( SphereMapInfo *side );

static void Vec2S_T ( Vec *ref, float *s, float *t );

static void DrawCubeSide    ( SphereMapInfo *info, GXTexObj *texObj );
static void DrawBackMap     ( GXTexObj *texObj );

static void RenderCubeSide  ( VecPtr up, VecPtr at, void *imageData );
static void DisplayMap      ( float dx, float dy, float width, float height, GXTexObj *texObj );

/****************************************************************************/

/*---------------------------------------------------------------------------*
    Name:			InitSphereMapSide
    
    Description:	Initializes an S,T grid for a face of a cube map
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
void InitSphereMap ( void )
{
    InitSphereMapFront();
    InitSphereMapLeft();
    InitSphereMapRight();
    InitSphereMapTop();
    InitSphereMapBottom();
    InitSphereMapBack();

    InitTextureMap(&SphereMapData, SPHERE_MAP_SIZE, SPHERE_MAP_SIZE, &SphereMap, GX_TF_RGB565);

    InitTextureMap(&CubeTop, CUBE_MAP_SIZE, CUBE_MAP_SIZE, &TexObj1, GX_TF_RGB565);
    InitTextureMap(&CubeBottom, CUBE_MAP_SIZE, CUBE_MAP_SIZE, &TexObj2, GX_TF_RGB565);
    InitTextureMap(&CubeLeft, CUBE_MAP_SIZE, CUBE_MAP_SIZE, &TexObj3, GX_TF_RGB565);
    InitTextureMap(&CubeRight, CUBE_MAP_SIZE, CUBE_MAP_SIZE, &TexObj4, GX_TF_RGB565);
    InitTextureMap(&CubeFront, CUBE_MAP_SIZE, CUBE_MAP_SIZE, &TexObj5, GX_TF_RGB565);
    InitTextureMap(&CubeBack, CUBE_MAP_SIZE, CUBE_MAP_SIZE, &TexObj6, GX_TF_RGB565);
}

/*---------------------------------------------------------------------------*
    Name:			InitSphereMap*
    
    Description:	Initializes a side of the cube map
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static void InitSphereMapFront ( void )
{
    u8 i, j;
    Vec reflection;

    InitSphereMapSide(&Front);

    for ( i = 0; i < SPHERE_MAP_RESOLUTION; i ++ )
    {
        for ( j = 0; j < SPHERE_MAP_RESOLUTION; j ++ )
        {
            reflection.x = Front.sGrid[i];
            reflection.y = Front.tGrid[j];
            reflection.z = 1.0F;

            reflection.x = reflection.x * -2.0F + 1.0F;
            reflection.y = reflection.y * -2.0F + 1.0F;

            VECNormalize(&reflection, &reflection);

            Vec2S_T (&reflection, &(Front.sphereGeometryS[i][j]), &(Front.sphereGeometryT[i][j]));
        }
    }
}

/*---------------------------------------------------------------------------*
    Name:			InitSphereMap*
    
    Description:	Initializes a side of the cube map
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static void InitSphereMapBack ( void )
{
    u8 i;
    Vec reflection;

    InitSphereMapSide((SphereMapInfo *)(&Back));

    for ( i = 0; i < SPHERE_MAP_RESOLUTION; i ++ )
    {
        reflection.x = Back.sGrid[i];
        reflection.y = 0.0F;
        reflection.z = -1.0F;

        reflection.x = reflection.x * 2.0F - 1.0F;
        reflection.y = reflection.y * -2.0F + 1.0F;

        VECNormalize(&reflection, &reflection);

        Vec2S_T (&reflection, &(Back.sphereGeometryS[i]), &(Back.sphereGeometryT[i]));

        reflection.x = Back.sphereGeometryS[i];
        reflection.y = Back.sphereGeometryT[i];
        reflection.z = 0.0F;
        VECNormalize(&reflection, &reflection);
        Back.nrmSphereGeometryS[i] = reflection.x;
        Back.nrmSphereGeometryT[i] = reflection.y;
    }

    for ( i = 0; i < SPHERE_MAP_RESOLUTION; i ++ )
    {
        reflection.x = 1.0F;
        reflection.y = Back.tGrid[i];
        reflection.z = -1.0F;

        reflection.x = reflection.x * 2.0F - 1.0F;
        reflection.y = reflection.y * -2.0F + 1.0F;

        VECNormalize(&reflection, &reflection);

        Vec2S_T (&reflection, &(Back.sphereGeometryS[i + SPHERE_MAP_RESOLUTION]), &(Back.sphereGeometryT[i + SPHERE_MAP_RESOLUTION]));

        reflection.x = Back.sphereGeometryS[i + SPHERE_MAP_RESOLUTION];
        reflection.y = Back.sphereGeometryT[i + SPHERE_MAP_RESOLUTION];
        reflection.z = 0.0F;
        VECNormalize(&reflection, &reflection);
        Back.nrmSphereGeometryS[i + SPHERE_MAP_RESOLUTION] = reflection.x;
        Back.nrmSphereGeometryT[i + SPHERE_MAP_RESOLUTION] = reflection.y;
    }

    for ( i = 0; i < SPHERE_MAP_RESOLUTION; i ++ )
    {
        reflection.x = Back.sGrid[(SPHERE_MAP_RESOLUTION - 1) - i];
        reflection.y = 1.0F;
        reflection.z = -1.0F;

        reflection.x = reflection.x * 2.0F - 1.0F;
        reflection.y = reflection.y * -2.0F + 1.0F;

        VECNormalize(&reflection, &reflection);

        Vec2S_T (&reflection, &(Back.sphereGeometryS[i + (SPHERE_MAP_RESOLUTION * 2)]), &(Back.sphereGeometryT[i + (SPHERE_MAP_RESOLUTION * 2)]));

        reflection.x = Back.sphereGeometryS[i + (SPHERE_MAP_RESOLUTION * 2)];
        reflection.y = Back.sphereGeometryT[i + (SPHERE_MAP_RESOLUTION * 2)];
        reflection.z = 0.0F;
        VECNormalize(&reflection, &reflection);
        Back.nrmSphereGeometryS[i + (SPHERE_MAP_RESOLUTION * 2)] = reflection.x;
        Back.nrmSphereGeometryT[i + (SPHERE_MAP_RESOLUTION * 2)] = reflection.y;
    }

    for ( i = 0; i < SPHERE_MAP_RESOLUTION; i ++ )
    {
        reflection.x = 0.0F;
        reflection.y = Back.tGrid[(SPHERE_MAP_RESOLUTION - 1) - i];
        reflection.z = -1.0F;

        reflection.x = reflection.x * 2.0F - 1.0F;
        reflection.y = reflection.y * -2.0F + 1.0F;

        VECNormalize(&reflection, &reflection);

        Vec2S_T (&reflection, &(Back.sphereGeometryS[i + (SPHERE_MAP_RESOLUTION * 3)]), &(Back.sphereGeometryT[i + (SPHERE_MAP_RESOLUTION * 3)]));

        reflection.x = Back.sphereGeometryS[i + (SPHERE_MAP_RESOLUTION * 3)];
        reflection.y = Back.sphereGeometryT[i + (SPHERE_MAP_RESOLUTION * 3)];
        reflection.z = 0.0F;
        VECNormalize(&reflection, &reflection);
        Back.nrmSphereGeometryS[i + (SPHERE_MAP_RESOLUTION * 3)] = reflection.x;
        Back.nrmSphereGeometryT[i + (SPHERE_MAP_RESOLUTION * 3)] = reflection.y;
    }
}

/*---------------------------------------------------------------------------*
    Name:			InitSphereMap*
    
    Description:	Initializes a side of the cube map
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static void InitSphereMapRight ( void )
{
    u8 i, j;
    Vec reflection;

    InitSphereMapSide(&Right);

    for ( i = 0; i < SPHERE_MAP_RESOLUTION; i ++ )
    {
        for ( j = 0; j < SPHERE_MAP_RESOLUTION; j ++ )
        {
            reflection.z = Right.sGrid[i];
            reflection.y = Right.tGrid[j];
            reflection.x = 1.0F;

            reflection.z = reflection.z * 2.0F - 1.0F;
            reflection.y = reflection.y * -2.0F + 1.0F;

            VECNormalize(&reflection, &reflection);

            Vec2S_T (&reflection, &(Right.sphereGeometryS[i][j]), &(Right.sphereGeometryT[i][j]));
        }
    }
}

/*---------------------------------------------------------------------------*
    Name:			InitSphereMap*
    
    Description:	Initializes a side of the cube map
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static void InitSphereMapLeft ( void )
{
    u8 i, j;
    Vec reflection;

    InitSphereMapSide(&Left);

    for ( i = 0; i < SPHERE_MAP_RESOLUTION; i ++ )
    {
        for ( j = 0; j < SPHERE_MAP_RESOLUTION; j ++ )
        {
            reflection.z = Left.sGrid[i];
            reflection.y = Left.tGrid[j];
            reflection.x = -1.0F;

            reflection.z = reflection.z * -2.0F + 1.0F;
            reflection.y = reflection.y * -2.0F + 1.0F;

            VECNormalize(&reflection, &reflection);

            Vec2S_T (&reflection, &(Left.sphereGeometryS[i][j]), &(Left.sphereGeometryT[i][j]));
        }
    }
}

/*---------------------------------------------------------------------------*
    Name:			InitSphereMap*
    
    Description:	Initializes a side of the cube map
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
//0, 0 goes at back, right corner of the cube
static void InitSphereMapTop ( void )
{
    u8 i, j;
    Vec reflection;

    InitSphereMapSide(&Top);

    for ( i = 0; i < SPHERE_MAP_RESOLUTION; i ++ )
    {
        for ( j = 0; j < SPHERE_MAP_RESOLUTION; j ++ )
        {
            reflection.z = Top.sGrid[i];
            reflection.x = Top.tGrid[j];
            reflection.y = 1.0F;

            reflection.z = reflection.z * -2.0F + 1.0F;
            reflection.x = reflection.x * -2.0F + 1.0F;

            VECNormalize(&reflection, &reflection);

            Vec2S_T (&reflection, &(Top.sphereGeometryS[i][j]), &(Top.sphereGeometryT[i][j]));
        }
    }
}

/*---------------------------------------------------------------------------*
    Name:			InitSphereMap*
    
    Description:	Initializes a side of the cube map
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
//0, 0 goes at front, left corner of the cube
static void InitSphereMapBottom ( void )
{
    u8 i, j;
    Vec reflection;

    InitSphereMapSide(&Bottom);

    for ( i = 0; i < SPHERE_MAP_RESOLUTION; i ++ )
    {
        for ( j = 0; j < SPHERE_MAP_RESOLUTION; j ++ )
        {
            reflection.z = Bottom.tGrid[i];
            reflection.x = Bottom.sGrid[j];
            reflection.y = -1.0F;

            reflection.z = reflection.z * -2.0F + 1.0F;
            reflection.x = reflection.x * 2.0F - 1.0F;

            VECNormalize(&reflection, &reflection);

            Vec2S_T(&reflection, &(Bottom.sphereGeometryS[i][j]), &(Bottom.sphereGeometryT[i][j]));
        }
    }
}

/*---------------------------------------------------------------------------*
    Name:			InitSphereMapSide
    
    Description:	Initializes an S,T grid for a face of a cube map
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static void InitSphereMapSide ( SphereMapInfo *side )
{
    u8 i;

    for ( i = 0; i < SPHERE_MAP_RESOLUTION; i ++ )
    {
        side->sGrid[i] = side->tGrid[i] = (float)i/(float)(SPHERE_MAP_RESOLUTION - 1);
    }
}

/*---------------------------------------------------------------------------*
    Name:			Vec2S_T
    
    Description:	returns the S and T on a sphere corresponding to the given
                    reflected normal.
    				
    Arguments:		ref - reflected normal.
                    s, t - resulting s & t coord
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static void Vec2S_T ( Vec *ref, float *s, float *t )
{
    float temp;

    temp = (ref->x * ref->x) + (ref->y * ref->y) + ((ref->z + 1) * (ref->z + 1));

    temp = sqrtf(temp);

    *s = ref->x / temp;
    *t = ref->y / temp;
}

/*---------------------------------------------------------------------------*
    Name:			DrawSphereMap
    
    Description:	Draws the sphere map
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
void DrawSphereMap ( void )
{
    Mtx44 p;
    Mtx m;

    GXInvalidateTexAll();

    MTXOrtho(p, 1.0F, -1.0F, -1.0F, 1.0F, 0.5F, 1.5F); 

    GXSetProjection(p, GX_ORTHOGRAPHIC);

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0,  GX_TEX_ST, GX_F32, 0);

    MTXIdentity(m);
    GXLoadPosMtxImm(m, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);

    GXSetViewport(0.0F, 0.0F, (float)SPHERE_MAP_SIZE, (float)SPHERE_MAP_SIZE, 0.0F, 1.0F);
    GXSetScissor(0, 0, SPHERE_MAP_SIZE, SPHERE_MAP_SIZE);

    DrawBackMap(&TexObj6);
    DrawCubeSide(&Top, &TexObj1);
    DrawCubeSide(&Bottom, &TexObj2);
    DrawCubeSide(&Left, &TexObj3);
    DrawCubeSide(&Right, &TexObj4);
    DrawCubeSide(&Front, &TexObj5);

    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);

    // turn off filtering
    GXSetCopyFilter(GX_FALSE, 0, GX_FALSE, 0);

    GXSetTexCopySrc(0, 0, SPHERE_MAP_SIZE, SPHERE_MAP_SIZE);
    GXSetTexCopyDst(SPHERE_MAP_SIZE, SPHERE_MAP_SIZE, GX_TF_RGB565, GX_FALSE);
    GXCopyTex(SphereMapData, GX_TRUE);

    // Wait for finishing the copy task in the graphics pipeline
    GXPixModeSync();

    // turn filtering back on
    GXSetCopyFilter(currentRenderMode->aa, currentRenderMode->sample_pattern, GX_TRUE, currentRenderMode->vfilter);
}

/*---------------------------------------------------------------------------*
    Name:			DrawCubeSide
    
    Description:	Draws one side of the cube onto the sphere map
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static void DrawCubeSide ( SphereMapInfo *info, GXTexObj *texObj )
{
    u8 i, j;

    SHDRBindTexture(SHDRReplaceShader, SHADER_TEX0, texObj);
    SHDRExecute(SHDRReplaceShader);

    GXBegin(GX_QUADS, GX_VTXFMT0, (SPHERE_MAP_RESOLUTION - 1) * (SPHERE_MAP_RESOLUTION - 1) * 4);

    for ( i = 0; i < SPHERE_MAP_RESOLUTION - 1; i ++)
    {
        for ( j = 0; j < SPHERE_MAP_RESOLUTION - 1; j ++)
        {
            GXPosition3f32(info->sphereGeometryS[i][j], info->sphereGeometryT[i][j], -1.0F);
            GXTexCoord2f32(info->sGrid[i], info->tGrid[j]);

            GXPosition3f32(info->sphereGeometryS[i][j + 1], info->sphereGeometryT[i][j + 1], -1.0F);
            GXTexCoord2f32(info->sGrid[i], info->tGrid[j + 1]);

            GXPosition3f32(info->sphereGeometryS[i + 1][j + 1], info->sphereGeometryT[i + 1][j + 1], -1.0F);
            GXTexCoord2f32(info->sGrid[i + 1], info->tGrid[j + 1]);

            GXPosition3f32(info->sphereGeometryS[i + 1][j], info->sphereGeometryT[i + 1][j], -1.0F);
            GXTexCoord2f32(info->sGrid[i + 1], info->tGrid[j]);
        }
    }

    GXEnd();
}

/*---------------------------------------------------------------------------*
    Name:			DrawBackMap
    
    Description:	Draws one side of the cube onto the sphere map
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static void DrawBackMap ( GXTexObj *texObj )
{
    u8 i;

    SHDRBindTexture(SHDRReplaceShader, SHADER_TEX0, texObj);
    SHDRExecute(SHDRReplaceShader);    

    GXBegin(GX_QUADS, GX_VTXFMT0, 4); 

    GXPosition3f32(-1.0F, -1.0F, -1.0F);
    GXTexCoord2f32(0.5F, 0.5F);

    GXPosition3f32(-1.0F, 1.0F, -1.0F);
    GXTexCoord2f32(0.5F, 0.5F);

    GXPosition3f32(1.0F, 1.0F, -1.0F);
    GXTexCoord2f32(0.5F, 0.5F);

    GXPosition3f32(1.0F, -1.0F, -1.0F);
    GXTexCoord2f32(0.5F, 0.5F);

    GXEnd();

    GXBegin(GX_QUADS, GX_VTXFMT0, ((SPHERE_MAP_RESOLUTION - 1) * 4) * 4);

    for ( i = 0; i < (SPHERE_MAP_RESOLUTION - 1); i ++)
    {        
        GXPosition3f32(Back.sphereGeometryS[i], Back.sphereGeometryT[i], -1.0F);
        GXTexCoord2f32(Back.sGrid[i], 0.0F);

        GXPosition3f32(Back.nrmSphereGeometryS[i], Back.nrmSphereGeometryT[i], -1.0F);
        GXTexCoord2f32(0.5F, 0.5F);

        GXPosition3f32(Back.nrmSphereGeometryS[i + 1], Back.nrmSphereGeometryT[i + 1], -1.0F);
        GXTexCoord2f32(0.5F, 0.5F);

        GXPosition3f32(Back.sphereGeometryS[i + 1], Back.sphereGeometryT[i + 1], -1.0F);
        GXTexCoord2f32(Back.sGrid[i + 1], 0.0F);

        /////////////////////////////////////////////////////////

        GXPosition3f32(Back.sphereGeometryS[i + SPHERE_MAP_RESOLUTION], Back.sphereGeometryT[i + SPHERE_MAP_RESOLUTION], -1.0F);
        GXTexCoord2f32(1.0F, Back.tGrid[i]);

        GXPosition3f32(Back.nrmSphereGeometryS[i + SPHERE_MAP_RESOLUTION], Back.nrmSphereGeometryT[i + SPHERE_MAP_RESOLUTION], -1.0F);
        GXTexCoord2f32(0.5F, 0.5F);

        GXPosition3f32(Back.nrmSphereGeometryS[i + 1 + SPHERE_MAP_RESOLUTION], Back.nrmSphereGeometryT[i + 1 + SPHERE_MAP_RESOLUTION], -1.0F);
        GXTexCoord2f32(0.5F, 0.5F);

        GXPosition3f32(Back.sphereGeometryS[i + 1 + SPHERE_MAP_RESOLUTION], Back.sphereGeometryT[i + 1 + SPHERE_MAP_RESOLUTION], -1.0F);
        GXTexCoord2f32(1.0F, Back.sGrid[i + 1]);

        /////////////////////////////////////////////////////////

        GXPosition3f32(Back.sphereGeometryS[i + (SPHERE_MAP_RESOLUTION * 2)], Back.sphereGeometryT[i + (SPHERE_MAP_RESOLUTION * 2)], -1.0F);
        GXTexCoord2f32(Back.sGrid[(SPHERE_MAP_RESOLUTION - 1) - i], 1.0F);

        GXPosition3f32(Back.nrmSphereGeometryS[i + (SPHERE_MAP_RESOLUTION * 2)], Back.nrmSphereGeometryT[i + (SPHERE_MAP_RESOLUTION * 2)], -1.0F);
        GXTexCoord2f32(0.5F, 0.5F);

        GXPosition3f32(Back.nrmSphereGeometryS[i + 1 + (SPHERE_MAP_RESOLUTION * 2)], Back.nrmSphereGeometryT[i + 1 + (SPHERE_MAP_RESOLUTION * 2)], -1.0F);
        GXTexCoord2f32(0.5F, 0.5F);

        GXPosition3f32(Back.sphereGeometryS[i + 1 + (SPHERE_MAP_RESOLUTION * 2)], Back.sphereGeometryT[i + 1 + (SPHERE_MAP_RESOLUTION * 2)], -1.0F);
        GXTexCoord2f32(Back.sGrid[(SPHERE_MAP_RESOLUTION - 2) - i], 1.0F);

        /////////////////////////////////////////////////////////

        GXPosition3f32(Back.sphereGeometryS[i + (SPHERE_MAP_RESOLUTION * 3)], Back.sphereGeometryT[i + (SPHERE_MAP_RESOLUTION * 3)], -1.0F);
        GXTexCoord2f32(0.0F, Back.tGrid[(SPHERE_MAP_RESOLUTION - 1) - i]);

        GXPosition3f32(Back.nrmSphereGeometryS[i + (SPHERE_MAP_RESOLUTION * 3)], Back.nrmSphereGeometryT[i + (SPHERE_MAP_RESOLUTION * 3)], -1.0F);
        GXTexCoord2f32(0.5F, 0.5F);

        GXPosition3f32(Back.nrmSphereGeometryS[i + 1 + (SPHERE_MAP_RESOLUTION * 3)], Back.nrmSphereGeometryT[i + 1 + (SPHERE_MAP_RESOLUTION * 3)], -1.0F);
        GXTexCoord2f32(0.5F, 0.5F);

        GXPosition3f32(Back.sphereGeometryS[i + 1 + (SPHERE_MAP_RESOLUTION * 3)], Back.sphereGeometryT[i + 1 + (SPHERE_MAP_RESOLUTION * 3)], -1.0F);
        GXTexCoord2f32(0.0F, Back.tGrid[(SPHERE_MAP_RESOLUTION - 2) - i]);
    }

    GXEnd();
}

/*---------------------------------------------------------------------------*
    Name:			RenderCubeMaps
    
    Description:	Renders the 6 cube sides based on the position and 
                    orientation of the camera
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
void RenderCubeMaps ( void )
{
    Mtx44 p;
    Vec zero = {0.0F, 0.0F, 0.0F};
    Vec negCamX;
    Vec negCamY;
    Vec negCamZ;
    Vec at;

    VECSubtract (&zero, &CamX, &negCamX);
    VECSubtract (&zero, &CamY, &negCamY);
    VECSubtract (&zero, &CamZ, &negCamZ);

    MTXPerspective(p, 90.0F, 1.0F, 50, 7000);

    GXSetProjection(p, GX_PERSPECTIVE);

    GXInvalidateTexAll();

    GXSetViewport(0.0F, 0.0F, (float)CUBE_MAP_SIZE, (float)CUBE_MAP_SIZE, 0.0F, 1.0F);
    GXSetScissor(0, 0, CUBE_MAP_SIZE, CUBE_MAP_SIZE);

    // turn off filtering
    GXSetCopyFilter(GX_FALSE, 0, GX_FALSE, 0);

    //Draw cube sides
    VECAdd(&CamX, &CubeCamLoc, &at);
    RenderCubeSide(&CamY, &at, CubeRight);

    VECAdd(&negCamX, &CubeCamLoc, &at);
    RenderCubeSide(&CamY, &at, CubeLeft);

    VECAdd(&CamY, &CubeCamLoc, &at);
    RenderCubeSide(&CamX, &at, CubeTop);

    VECAdd(&negCamY, &CubeCamLoc, &at);
    RenderCubeSide(&negCamX, &at, CubeBottom);

    VECAdd(&negCamZ, &CubeCamLoc, &at);
    RenderCubeSide(&CamY, &at, CubeBack);

    VECAdd(&CamZ, &CubeCamLoc, &at);
    RenderCubeSide(&CamY, &at, CubeFront);

    // turn filtering back on
    GXSetCopyFilter(currentRenderMode->aa, currentRenderMode->sample_pattern, GX_TRUE, currentRenderMode->vfilter);
}

/*---------------------------------------------------------------------------*
    Name:			RenderCubeSide
    
    Description:	Renders one side of the cube
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static void RenderCubeSide ( VecPtr up, VecPtr at, void *imageData )
{
    Mtx m;

    MTXLookAt(m, &CubeCamLoc, up, at);
    
    SpecularGeometryDraw(m);

    GXSetTexCopySrc(0, 0, CUBE_MAP_SIZE, CUBE_MAP_SIZE);
    GXSetTexCopyDst(CUBE_MAP_SIZE, CUBE_MAP_SIZE, GX_TF_RGB565, GX_FALSE);
    GXCopyTex(imageData, GX_TRUE);
    // Wait for finishing the copy task in the graphics pipeline
    GXPixModeSync();
}

/*---------------------------------------------------------------------------*
    Name:			DisplaySphereMap
    
    Description:	Displays the current sphere map on the screen
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
void DisplaySphereMap ( void )
{
    DisplayMap(10.0F, 15.0F, 128.0F, 128.0F, &SphereMap);

    DisplayMap(374.0F, 79.0F, 64.0F, 64.0F, &TexObj4);
    DisplayMap(438.0F, 79.0F, 64.0F, 64.0F, &TexObj5);
    DisplayMap(502.0F, 79.0F, 64.0F, 64.0F, &TexObj3);
    DisplayMap(566.0F, 79.0F, 64.0F, 64.0F, &TexObj6);

    DisplayMap(502.0F, 15.0F, 64.0F, 64.0F, &TexObj1);
    DisplayMap(502.0F, 143.0F, 64.0F, 64.0F, &TexObj2);
}

/*---------------------------------------------------------------------------*
    Name:			DisplayMap
    
    Description:	Displays a map in a square on the screen
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static void DisplayMap ( float dx, float dy, float width, float height, GXTexObj *texObj )
{
    Mtx44 p;
    Mtx m;

    SHDRBindTexture(SHDRReplaceShader, SHADER_TEX0, texObj);
    SHDRExecute(SHDRReplaceShader);

    GXSetViewport(dx, dy, width, height, 0.0F, 1.0F);

    MTXOrtho(p, 1.0F, -1.0F, -1.0F, 1.0F, 0.5F, 1.5F);

    GXSetProjection(p, GX_ORTHOGRAPHIC);

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0,  GX_TEX_ST, GX_F32, 0);

    MTXIdentity(m);
    GXLoadPosMtxImm(m, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    //turn off alpha blending
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, 
                       GX_LO_CLEAR);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4); 

    GXPosition3f32(-1.0F, -1.0F, -1.0F);
    GXTexCoord2f32(0.0F, 1.0F);

    GXPosition3f32(-1.0F, 1.0F, -1.0F);
    GXTexCoord2f32(0.0F, 0.0F);

    GXPosition3f32(1.0F, 1.0F, -1.0F);
    GXTexCoord2f32(1.0F, 0.0F);

    GXPosition3f32(1.0F, -1.0F, -1.0F);
    GXTexCoord2f32(1.0F, 1.0F);

    GXEnd();

    //turn alpha blending back on
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, 
                       GX_LO_CLEAR);

    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
}


