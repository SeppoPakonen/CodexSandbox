/*---------------------------------------------------------------------------*
  Project:  Dolphin
  File:     raceCamera.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/demos/charPipeline/src/raceCamera.c $
    
    5     8/11/00 10:42a Ryan
    final checkin for spaceworld
    
    4     8/08/00 12:30p Ryan
    update to fix errors/warnings
    
    3     8/05/00 8:42a Ryan
    20-car update
    
    2     8/01/00 10:22a Ryan
    update to fix MAC build bug
    
    1     7/29/00 11:17a Ryan
    initial checkin
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <charPipeline.h>
#include <demo.h>

#include "raceCamera.h"
#include "racePad.h"
#include "raceSphereMap.h"

/****************************************************************************/

#define LERP_SMOOTHNESS         20

#define PROJ_RIGHT   48.0F
#define PROJ_TOP     36.0F
#define PROJ_ZNEAR   75.0F
#define PROJ_ZFAR    10000.0F

#define COPY_OVERLAP 2

/****************************************************************************/

Mtx cameraMatrix;

CameraKeyframe CameraKeyArray[NUM_CAMERA_KEYFRAMES] = {
    //1 car
    0.984190F, 0.177084F, 0.0F,
    -0.099152F, 0.551064F, 0.828548F,
    0.146783F, -0.815452F, 0.559915F, 
    102.738464F, 

    //3 cars
    0.416278F, 0.909231F, 0.0F, 
    -0.4166927F, 0.213776F, 0.858062F,
    0.780178F, -0.357194F, 0.513539F,
    205.009277F,

    //5 cars
    -0.965922F, 0.258818F, 0.000000F,
    -0.150480F, -0.561600F, 0.813607F,
    0.210576F, 0.785883F, 0.581411F,
    280.099487F,

    //8 cars
    -0.965922F, 0.258818F, 0.000000F,
    -0.090851F, -0.339062F, 0.936364F,
    0.242348F, 0.904458F, 0.351024F,
    298.085754F,

    //12 cars
    -0.934512F, 0.355922F, 0.000000F,
    -0.064861F, -0.170301F, 0.983255F,
    0.349962F, 0.918866F, 0.182235F,
    335.248138F
};

Vec CamX;	
Vec CamY;
Vec CamZ;
float CameraLocScale;

static f32 yOverlap;

static GXRenderModeObj *rMode = &GXNtsc480IntAa; 

static u16 copyLines;       // number of scan lines to copy for bottom
static u32 bufferOffset;    // offset into external frame buffer
                            // for copying bottom half of buffer

// Buffer space for 2 lines of video
// This is needed to clear out the overlap lines in f.f. AA mode
static u8 garbage[640*2*VI_DISPLAY_PIX_SZ] ATTRIBUTE_ALIGN(32);

static u8 ZoomInDampFrames = 0;
static u8 ZoomOutDampFrames = 0;

#define DAMP_FRAMES 20
#define ZOOM_SCALE 0.05F

static u8 RotatePosXDamp = 0;
static u8 RotateNegXDamp = 0;
static u8 RotatePosYDamp = 0;
static u8 RotateNegYDamp = 0;

#define ROTATE_SCALE 3.0F

BOOL VideoModeAA = TRUE;


/****************************************************************************/

static void MakeModelMtx    ( Vec xAxis, Vec yAxis, Vec zAxis, Mtx m );
static float LERPFloat      ( float start, float end, float weight );
static Vec LERPVec          ( Vec *start, Vec *end, float weight );

/****************************************************************************/
/*---------------------------------------------------------------------------*
    Name:			CameraInit
    
    Description:	Initialize the projection matrix and load into hardware.
    				
    Arguments:		v  	view matrix	to be passed to ViewInit
					cameraLocScale	scale for the camera's distance from the 
									object - to be passed to ViewInit
    
    Returns:		none
 *---------------------------------------------------------------------------*/
void CameraInit 		( void )
{
    CamX = CameraKeyArray[0].x;	
    CamY = CameraKeyArray[0].y;
    CamZ = CameraKeyArray[0].z;
    CameraLocScale = CameraKeyArray[0].scale;

    yOverlap = ((f32) COPY_OVERLAP / rMode->xfbHeight) * (PROJ_TOP*2.0f);

    // Calculate copy parameters for bottom half of the screen
    copyLines    = (u16) (rMode->efbHeight - COPY_OVERLAP);
    bufferOffset = VIPadFrameBufferWidth(rMode->fbWidth) * copyLines * 
                   (u32) VI_DISPLAY_PIX_SZ;
}

/*---------------------------------------------------------------------------*
    Name:			SetMainCamera
    
    Description:	Initialize the projection matrix and load into hardware.
    				
    Arguments:		
    
    Returns:		none
 *---------------------------------------------------------------------------*/
void SetMainCamera 		( void )
{
	Mtx44 p;

    GXSetViewport(0.0F, 0.0F,
              (float)GXNtsc480IntDf.fbWidth, (float)GXNtsc480IntDf.xfbHeight, 
              0.0F, 1.0F);
    GXSetScissor(0, 0, (u32)GXNtsc480IntDf.fbWidth, (u32)GXNtsc480IntDf.efbHeight);
	
    MTXFrustum(p, PROJ_TOP, -PROJ_TOP,
			   -PROJ_RIGHT,  PROJ_RIGHT, 
			   PROJ_ZNEAR, PROJ_ZFAR);

	GXSetProjection(p, GX_PERSPECTIVE);
}

/*---------------------------------------------------------------------------*
    Name:			SetMainCamera
    
    Description:	Initialize the projection matrix and load into hardware.
    				
    Arguments:		
    
    Returns:		none
 *---------------------------------------------------------------------------*/
void SetMainCameraTop 		( void )
{
	Mtx44 p;

    GXSetViewport(0.0F, 0.0F,
              (float)GXNtsc480IntAa.fbWidth, (float)GXNtsc480IntAa.xfbHeight, 
              0.0F, 1.0F);
    GXSetScissor(0, 0, (u32)GXNtsc480IntAa.fbWidth, (u32)GXNtsc480IntAa.efbHeight);
	
    MTXFrustum(p, PROJ_TOP, -PROJ_TOP,
			   -PROJ_RIGHT,  PROJ_RIGHT, 
			   PROJ_ZNEAR, PROJ_ZFAR);

	GXSetProjection(p, GX_PERSPECTIVE);
}

/*---------------------------------------------------------------------------*
    Name:			SetMainCamera
    
    Description:	Initialize the projection matrix and load into hardware.
    				
    Arguments:		
    
    Returns:		none
 *---------------------------------------------------------------------------*/
void SetMainCameraBottom 		( void )
{
	Mtx44 p;

    GXSetViewport(0.0F, 0.0F,
              (float)GXNtsc480IntAa.fbWidth, (float)GXNtsc480IntAa.xfbHeight, 
              0.0F, 1.0F);
    GXSetScissor(0, 0, (u32)GXNtsc480IntAa.fbWidth, (u32)GXNtsc480IntAa.efbHeight);
	

    MTXFrustum(p, yOverlap, -2.0f*PROJ_TOP+yOverlap,
               -PROJ_RIGHT, PROJ_RIGHT,
                PROJ_ZNEAR, PROJ_ZFAR);

	GXSetProjection(p, GX_PERSPECTIVE);
}

/*---------------------------------------------------------------------------*
    Name:           CopyBottom
    
    Description:    Copies the bottom half of the frame buffer correctly.
                    
    Arguments:      none
    
    Returns:        none
 *---------------------------------------------------------------------------*/
void CopyBottom( void )
{
    // This first copy copies out the lines we want to display.
    // We want to omit the first two lines from the copy, but one of
    // these lines is used for the vertical filter.  We must adjust
    // the clamp mode to allow for this (turn off top clamping).
    GXSetCopyClamp(GX_CLAMP_BOTTOM); // not TOP
    GXSetDispCopySrc(0, COPY_OVERLAP, rMode->fbWidth, copyLines);
    GXCopyDisp((void *)((u32)DEMOGetCurrentBuffer()+bufferOffset), GX_TRUE);

    // This second copy is to clear out the two lines that we didn't copy.
    // We also must return the clamp mode back to normal.
    GXSetCopyClamp((GXFBClamp)(GX_CLAMP_TOP | GX_CLAMP_BOTTOM));
    GXSetDispCopySrc(0, 0, rMode->fbWidth, COPY_OVERLAP);
    GXCopyDisp((void *)garbage, GX_TRUE);

    // Finally, return the display copy source back to the default.
    // This makes sure the top half will get copied correctly.
    GXSetDispCopySrc(0, 0, rMode->fbWidth, rMode->efbHeight);
}

/*---------------------------------------------------------------------------*
    Name:			CameraAnim
    
    Description:	Animates the camera and object based on the joystick's 
					state.
    				
    Arguments:		none
    
    Returns:		none
 *---------------------------------------------------------------------------*/
void CameraAnim ( void )
{
	Mtx trans;
    Vec temp;
    Vec yAxis = {0.0F, 0.0F, 1.0F};
    float dot;
    Mtx rot;

	u16 buttons = JoyReadButtons();
	s8 stickX = JoyReadStickX();
	s8 stickY = JoyReadStickY();
	s8 subStickY = JoyReadSubstickY();
	u8 triggerR = JoyReadTriggerR();

    // capture camera state
    if(triggerR > 30)
    {
        OSReport("\nCamX = %fF, %fF, %fF,\n", CamX.x, CamX.y, CamX.z);
        OSReport("CamY = %fF, %fF, %fF,\n", CamY.x, CamY.y, CamY.z);
        OSReport("CamZ = %fF, %fF, %fF,\n", CamZ.x, CamZ.y, CamZ.z);
        OSReport("CamLocalScale = %fF,\n\n\n", CameraLocScale);
    }

    //change video mode
    if(buttons & PAD_BUTTON_A)
    {
        if(VideoModeAA)
        {
            VideoModeAA = FALSE;
            SetUpIntDF();
            currentRenderMode = &GXNtsc480IntDf;
            OSReport("Antialiasing off\n");
        }
        else
        {
            VideoModeAA = TRUE;
            SetUpAA();
            currentRenderMode = &GXNtsc480IntAa;
            OSReport("Antialiasing on\n");
        }
        
    }
    
    // Move camera
	//if(buttons & PAD_BUTTON_Y)
    if(subStickY > 75)
    {
        //if the camera is zooming out still
        if(ZoomOutDampFrames)
        {
            CameraLocScale *= (1.0F + (ZOOM_SCALE * (((f32)ZoomOutDampFrames))/((f32)DAMP_FRAMES)));
            ZoomOutDampFrames --;
        }
        else
        {
            CameraLocScale *= (1.0F - (ZOOM_SCALE * (((f32)ZoomInDampFrames))/((f32)DAMP_FRAMES)));
    	    if(CameraLocScale < 0.001F)
    		    CameraLocScale = 0.001F;

            if(ZoomInDampFrames < DAMP_FRAMES)
                ZoomInDampFrames ++;
        }
   	}
	//if(buttons & PAD_BUTTON_A)
    else if(subStickY < -75)
	{
        //if the camera is zooming in still
        if(ZoomInDampFrames)
        {
            CameraLocScale *= (1.0F - (ZOOM_SCALE * (((f32)ZoomInDampFrames))/((f32)DAMP_FRAMES)));
            if(CameraLocScale < 0.001F)
    		    CameraLocScale = 0.001F;

            ZoomInDampFrames --;
        }
        else
        {
            CameraLocScale *= (1.0F + (ZOOM_SCALE * (((f32)ZoomOutDampFrames))/((f32)DAMP_FRAMES)));

            if(ZoomOutDampFrames < DAMP_FRAMES)
                ZoomOutDampFrames ++;
        }
	}
    else
    {
        if(ZoomInDampFrames)
        {
            CameraLocScale *= (1.0F - (ZOOM_SCALE * (((f32)ZoomInDampFrames))/((f32)DAMP_FRAMES)));
            if(CameraLocScale < 0.001F)
    		    CameraLocScale = 0.001F;

            ZoomInDampFrames --;
        }

        if(ZoomOutDampFrames)
        {
            CameraLocScale *= (1.0F + (ZOOM_SCALE * (((f32)ZoomOutDampFrames))/((f32)DAMP_FRAMES)));
            ZoomOutDampFrames --;
        }
    }

	// Rotate camera
    if(stickX > 30 || stickX < -30)
	 {
		if(stickX > 30) 
        {
            if(RotateNegXDamp)
            {
                MTXRotDeg(rot, 'z', -(ROTATE_SCALE * ( ((f32)RotateNegXDamp)/((f32)DAMP_FRAMES) ) ) );
                RotateNegXDamp --;
            }
            else
            {
                MTXRotDeg(rot, 'z', ROTATE_SCALE * ( ((f32)RotatePosXDamp)/((f32)DAMP_FRAMES) ) );

                if(RotatePosXDamp < DAMP_FRAMES)
                    RotatePosXDamp++;
            }
        }
		else if(stickX < -30)
        {
            if(RotatePosXDamp)
            {
                MTXRotDeg(rot, 'z', ROTATE_SCALE * ( ((f32)RotatePosXDamp)/((f32)DAMP_FRAMES) ) );
                RotatePosXDamp --;
            }
            else
            {
                MTXRotDeg(rot, 'z', -(ROTATE_SCALE * ( ((f32)RotateNegXDamp)/((f32)DAMP_FRAMES) ) ) );
                if(RotateNegXDamp < DAMP_FRAMES)
                    RotateNegXDamp++;
            }
        }
		MTXMultVec(rot, &CamX, &CamX);
    	MTXMultVec(rot, &CamZ, &CamZ); 
        MTXMultVec(rot, &CamY, &CamY); 
    }
    else
    {
        if(RotatePosXDamp)
        {
            MTXRotDeg(rot, 'z', ROTATE_SCALE * ( ((f32)RotatePosXDamp)/((f32)DAMP_FRAMES) ) );
            RotatePosXDamp --;
            MTXMultVec(rot, &CamX, &CamX);
    	    MTXMultVec(rot, &CamZ, &CamZ); 
            MTXMultVec(rot, &CamY, &CamY);
        }
        if(RotateNegXDamp)
        {
            MTXRotDeg(rot, 'z', -(ROTATE_SCALE * ( ((f32)RotateNegXDamp)/((f32)DAMP_FRAMES) ) ) );
            RotateNegXDamp --;
            MTXMultVec(rot, &CamX, &CamX);
    	    MTXMultVec(rot, &CamZ, &CamZ); 
            MTXMultVec(rot, &CamY, &CamY);
        } 
    }

    if(stickY > 30 || stickY < -30)
    {
		if(stickY > 30)
        {
            if(RotateNegYDamp)
            {
                MTXRotAxis(rot, &CamX, ROTATE_SCALE * ( ((f32)RotateNegYDamp)/((f32)DAMP_FRAMES) ) );
                RotateNegYDamp --;
            }
            else
            {
                MTXRotAxis(rot, &CamX, -( ROTATE_SCALE * ( ((f32)RotatePosYDamp)/((f32)DAMP_FRAMES) ) ) );

                if(RotatePosYDamp < DAMP_FRAMES)
                    RotatePosYDamp++;
            }
        }
		else if(stickY < -30)
        {
            if(RotatePosYDamp)
            {
                MTXRotAxis(rot, &CamX, -( ROTATE_SCALE * ( ((f32)RotatePosYDamp)/((f32)DAMP_FRAMES) ) ) );
                RotatePosYDamp --;
            }
            else
            {
                MTXRotAxis(rot, &CamX, ROTATE_SCALE * ( ((f32)RotateNegYDamp)/((f32)DAMP_FRAMES) ) );
                if(RotateNegYDamp < DAMP_FRAMES)
                    RotateNegYDamp++;
            }
        }

	    MTXMultVec(rot, &CamZ, &temp); 
        dot = VECDotProduct(&temp, &yAxis);
        if(dot > 0.05F && dot < 0.9F)
        {
            CamZ = temp;
            MTXMultVec(rot, &CamY, &CamY);
        }
        else
        {
            RotateNegYDamp = RotatePosYDamp = 0;
        }
 	}
    else
    {
        if(RotatePosYDamp)
        {
            MTXRotAxis(rot, &CamX, -( ROTATE_SCALE * ( ((f32)RotatePosYDamp)/((f32)DAMP_FRAMES) ) ) );
                RotatePosYDamp --;
            MTXMultVec(rot, &CamZ, &temp); 
            dot = VECDotProduct(&temp, &yAxis);
            if(dot > 0.05F && dot < 0.9F)
            {
                CamZ = temp;
                MTXMultVec(rot, &CamY, &CamY);
            }
            else
            {
                RotateNegYDamp = RotatePosYDamp = 0;
            }
        }
        if(RotateNegYDamp)
        {
            MTXRotAxis(rot, &CamX, ROTATE_SCALE * ( ((f32)RotateNegYDamp)/((f32)DAMP_FRAMES) ) );
                RotateNegYDamp --;
            MTXMultVec(rot, &CamZ, &temp); 
            dot = VECDotProduct(&temp, &yAxis);
            if(dot > 0.05F && dot < 0.9F)
            {
                CamZ = temp;
                MTXMultVec(rot, &CamY, &CamY);
            }
            else
            {
                RotateNegYDamp = RotatePosYDamp = 0;
            }
        } 
    }

	MakeModelMtx(CamX, CamY, CamZ, cameraMatrix);	// Make a new camera matrix
	MTXTranspose(cameraMatrix, cameraMatrix);
	MTXTrans(trans, 0.0F, 0.0F, -8.0F * CameraLocScale);
	MTXConcat(trans, cameraMatrix, cameraMatrix);
}

/*---------------------------------------------------------------------------*
    Name:			MakeModelMtx
    
    Description:	computes a model matrix from 3 vectors representing an 
					object's coordinate system.
    				
    Arguments:		xAxis	vector for the object's X axis
					yAxis	vector for the object's Y axis
					zAxis	vector for the object's Z axis
    
    Returns:		none
 *---------------------------------------------------------------------------*/
static void MakeModelMtx ( Vec xAxis, Vec yAxis, Vec zAxis, Mtx m )
{
	VECNormalize(&xAxis,&xAxis);
	VECNormalize(&yAxis,&yAxis);
	VECNormalize(&zAxis,&zAxis);

	m[0][0] = xAxis.x;
	m[0][1] = xAxis.y;
	m[0][2] = xAxis.z;
	m[0][3] = 0.0F;

	m[1][0] = yAxis.x;
	m[1][1] = yAxis.y;
	m[1][2] = yAxis.z;
	m[1][3] = 0.0F;

	m[2][0] = zAxis.x;
	m[2][1] = zAxis.y;
	m[2][2] = zAxis.z;
	m[2][3] = 0.0F;

	MTXInverse(m, m);
}

/*---------------------------------------------------------------------------*/
static float LERPFloat ( float start, float end, float weight )
{
    return (start * (1.0F - weight)) + (end * weight);
}

/*---------------------------------------------------------------------------*/
static Vec LERPVec ( Vec *start, Vec *end, float weight )
{
    Vec temp;

    temp.x = LERPFloat(start->x, end->x, weight);
    temp.y = LERPFloat(start->y, end->y, weight);
    temp.z = LERPFloat(start->z, end->z, weight);

    return temp;
}

/*---------------------------------------------------------------------------*/
void SetUpAA ( void )
{
    VIConfigure(&GXNtsc480IntAa);

    GXSetDispCopySrc(0, 0, GXNtsc480IntAa.fbWidth, GXNtsc480IntAa.efbHeight);
    GXSetDispCopyDst(GXNtsc480IntAa.fbWidth, GXNtsc480IntAa.xfbHeight);
    GXSetDispCopyYScale(1.0F); 
    GXSetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
    GXSetCopyFilter(GXNtsc480IntAa.aa, GXNtsc480IntAa.sample_pattern, GX_TRUE, GXNtsc480IntAa.vfilter);

    GXSetDither(GX_ENABLE);

    // Tell VI device driver to write the current VI settings so far
    VIFlush();
    
    // Wait for retrace to start first frame
    VIWaitForRetrace();
}

/*---------------------------------------------------------------------------*/
void SetUpIntDF ( void )
{
    VIConfigure(&GXNtsc480IntDf);

    GXSetDispCopySrc(0, 0, GXNtsc480IntDf.fbWidth, GXNtsc480IntDf.efbHeight);
    GXSetDispCopyDst(GXNtsc480IntDf.fbWidth, GXNtsc480IntDf.xfbHeight);
    GXSetDispCopyYScale((f32)(GXNtsc480IntDf.xfbHeight) / (f32)(GXNtsc480IntDf.efbHeight)); 
    GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
    GXSetCopyFilter(GXNtsc480IntDf.aa, GXNtsc480IntDf.sample_pattern, GX_TRUE, GXNtsc480IntDf.vfilter);

    GXSetDither(GX_DISABLE);

    // Tell VI device driver to write the current VI settings so far
    VIFlush();
    
    // Wait for retrace to start first frame
    VIWaitForRetrace();
}
