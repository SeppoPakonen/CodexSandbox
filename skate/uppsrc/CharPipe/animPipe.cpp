/*---------------------------------------------------------------------------*
  Project:  character pipeline
  File:     animPipe.c

  Copyright 1998, 1999, 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/anim/src/animPipe.c $
    
    10    11/13/00 1:24p John
    Modified so that animation can replace hierarchy transformation (with
    an option enabled in the track).
    Fixed an animation quantization bug that arises for scale parameters
    when quantization is other than GX_S16.
    Bezier interpolation of euler angles is treated like linear space
    rather than interpolating the shortest angle.
    
    9     9/11/00 3:58p John
    Optimized slightly.
    
    8     8/14/00 6:22p John
    Added user-defined data to ANM, ACT, and GPL formats.
    
    7     7/18/00 7:23p John
    Modified to use new CTRL library.
    Removed quaternion code since unused (can be found in C3Math.c).
    
    6     3/23/00 9:56a Ryan
    update for Character Pipeline cleanup and function prefixing
    
  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <CharPipe/anim.h>

#include <Dolphin/os.h>
#include <Dolphin/gx.h>
#include <math.h>

#include <stdarg.h>

/********************************/
#define EPSILON         0.0001f
#define COS_EPSILON     0.000001f
#define HALFPI          1.570796326794895f

#define QUAT_SIZE       2
#define QUAT_SIZE_FOUR  8
#define QUAT_SIZE_EIGHT 16
#define QUAT_QUANTINFO  0x3E
#define EASE_SIZE       2
#define EASE_SIZE_TWO   4
#define EASE_QUANTINFO  0x3E

#define QUANT_TYPE(a)  ((a)>>4)
#define QUANT_SHIFT(a) ((a)&0x0F)

/********************************/
static void DoMatrixAnimation      ( ANIMPipePtr animPipe, ANIMKeyFramePtr startFrame, ANIMKeyFramePtr endFrame );
static void DoQuatAnimation        ( ANIMPipePtr animPipe, ANIMKeyFramePtr startFrame, ANIMKeyFramePtr endFrame );
static void DoEulerAnimation       ( ANIMPipePtr animPipe, ANIMKeyFramePtr startFrame, ANIMKeyFramePtr endFrame );
static void DoScaleAnimation       ( ANIMPipePtr animPipe, ANIMKeyFramePtr startFrame, ANIMKeyFramePtr endFrame );
static void DoTranslationAnimation ( ANIMPipePtr animPipe, ANIMKeyFramePtr startFrame, ANIMKeyFramePtr endFrame );

static u32  GetDataSize ( u8 quantType );
static void GetData     ( u8 quantType, f32 quantShift, Ptr from, f32 *to, u8 count );

static void Ticktime           ( ANIMPipePtr animPipe );
static f32  NormalizeTracktime ( ANIMPipePtr animPipe, ANIMKeyFramePtr startFrame, ANIMKeyFramePtr endFrame );

static void LinearInterpolateVec        ( f32 *start, f32 *end, f32 time, f32 *final );
static void LinearInterpolateVecEuler   ( f32 *start, f32 *end, f32 time, f32 *final );
static void BezierInterpolateVec        ( f32 *start, f32 *end, f32 u,
                                          f32 *outControl, f32 *inControl, f32 *final );
static void BezierInterpolateVecEuler   ( f32 *start, f32 *end, f32 u,
                                          f32 *outControl, f32 *inControl, f32 *final );
static void HermiteInterpolateVec       ( f32 *start, f32 *end, f32* basis,
                                          f32 *outControl, f32 *inControl, f32 *final );
static void ComputeHermiteBasis         ( f32 u, f32 *v );

static f32        Ease  (f32 u, f32 a, f32 b);
static void Slerp ( Qtrn *p, Qtrn *q, f32 t, Qtrn *r );
static void Squad ( Qtrn *p, Qtrn *a, Qtrn *b, Qtrn *q, f32 t, Qtrn *r );

/********************************/
    
static f32 QuatShift = 1.0f / (f32)(1 << QUANT_SHIFT(QUAT_QUANTINFO));
static f32 EaseShift = 1.0f / (f32)(1 << QUANT_SHIFT(EASE_QUANTINFO));
static u8  QuatType  = (u8)QUANT_TYPE(QUAT_QUANTINFO);
static u8  EaseType  = (u8)QUANT_TYPE(EASE_QUANTINFO);

/*>*******************************(*)*******************************<*/
// INITIALIZATION SECTION
/*>*******************************(*)*******************************<*/
void    ANIMBind        ( ANIMPipePtr animPipe, CTRLControlPtr control, 
                          ANIMTrackPtr animTrack, f32 time )
{
    if(!animTrack)
        return;

    CTRLInit( control );

    animPipe->control = control;
    animPipe->currentTrack = animTrack;
    animPipe->time = time;
    animPipe->replaceHierarchyCtrl = animTrack->replaceHierarchyCtrl;
}

/*>*******************************(*)*******************************<*/
void    ANIMSetTime             ( ANIMPipePtr animPipe, f32 time )
{
    if(!animPipe)
        return;

    animPipe->time = time;
}

/*>*******************************(*)*******************************<*/
void    ANIMSetSpeed            ( ANIMPipePtr animPipe, f32 speed )
{
    if(!animPipe)
        return;

    animPipe->speed = speed;
}

/*>*******************************(*)*******************************<*/
// USAGE SECTION
/*>*******************************(*)*******************************<*/
void    ANIMTick        ( ANIMPipePtr animPipe )
{
    ANIMKeyFramePtr startFrame, endFrame;
    
    if(!animPipe)
        return;

    ANIMGetKeyFrameFromTrack(animPipe->currentTrack, animPipe->time, &startFrame, &endFrame );

    if(animPipe->currentTrack->animType & ANIM_MTX)
    {
        DoMatrixAnimation(animPipe, startFrame, endFrame);
        return;
    }

    if(animPipe->currentTrack->animType & ANIM_SCALE)
        DoScaleAnimation(animPipe, startFrame, endFrame);

    if(animPipe->currentTrack->animType & ANIM_QUAT)
        DoQuatAnimation(animPipe, startFrame, endFrame);
    else if(animPipe->currentTrack->animType & ANIM_ROT)
        DoEulerAnimation(animPipe, startFrame, endFrame);

    if(animPipe->currentTrack->animType & ANIM_TRANS)
        DoTranslationAnimation(animPipe, startFrame, endFrame);

    Ticktime(animPipe); 
}

/*>*******************************(*)*******************************<*/
static void DoMatrixAnimation ( ANIMPipePtr animPipe, ANIMKeyFramePtr startFrame, ANIMKeyFramePtr endFrame )
{
#pragma unused (endFrame)

    f32 shift = 1.0f/(f32)(1<< QUANT_SHIFT(animPipe->currentTrack->quantizeInfo));
    u32 i;

    // Dequantize the matrix
    for( i = 0; i < 12; i++ )
    {
        ((f32*)startFrame->setting)[i] *= shift;
    }

    CTRLSetMatrix(animPipe->control, (MtxPtr)(startFrame->setting));
}

/*>*******************************(*)*******************************<*/
static void DoQuatAnimation ( ANIMPipePtr animPipe, ANIMKeyFramePtr startFrame, ANIMKeyFramePtr endFrame )
{
    Qtrn final;
    f32  start[4];
    f32  end[4];
    f32  a[4];
    f32  b[4];
    f32  easeIn, easeOut;
    u32  offset;
    f32  time;
    u32  data;
    u32  paramSize;
    u8   quantType;

    quantType  = (u8)QUANT_TYPE(animPipe->currentTrack->quantizeInfo);

    paramSize = GetDataSize(quantType) * 3;

    offset = 0;
    if(animPipe->currentTrack->animType & ANIM_SCALE) // scale
        offset = paramSize;

    GetData(QuatType, QuatShift, (Ptr)((u32)(startFrame->setting) + offset), start, 4);
    GetData(QuatType, QuatShift, (Ptr)((u32)(endFrame->setting) + offset), end, 4);

    time = NormalizeTracktime(animPipe, startFrame, endFrame);

    // Figure out the offset for the rotation interpolation data depending
    // on scale interpolation type
    offset = 0;
    if(animPipe->currentTrack->animType & ANIM_SCALE)   // scale exists
    {
        data = (u32)((animPipe->currentTrack->interpolationType >> 2) & 0x03);
        if(data == ANIM_BEZIER) // bezier
            offset = paramSize << 1;
        if(data == ANIM_HERMITE) // hermite
            offset = (paramSize << 1) + (EASE_SIZE_TWO);
    }

    switch((animPipe->currentTrack->interpolationType & 0x70) >> 4) // rotation interpolation flag
    {
        case ANIM_SQUADEE:
            GetData(EaseType, EaseShift, (Ptr)((u32)(endFrame->interpolation) + offset + QUAT_SIZE_EIGHT), &easeIn, 1);
            GetData(EaseType, EaseShift, (Ptr)((u32)(startFrame->interpolation) + offset + QUAT_SIZE_EIGHT + EASE_SIZE), &easeOut, 1);

            time = Ease(time, easeOut, easeIn);
            // no break; here intentionally

        case ANIM_SQUAD:
            GetData(QuatType, QuatShift, (Ptr)((u32)(startFrame->interpolation) + offset + QUAT_SIZE_FOUR), a, 4);
            GetData(QuatType, QuatShift, (Ptr)((u32)(endFrame->interpolation) + offset), b, 4);
            
            Squad((Qtrn*)start, (Qtrn*)a, (Qtrn*)b, (Qtrn*)end, time, &final);
            break;

        case ANIM_SLERP:
            Slerp((Qtrn*)start, (Qtrn*)end, time, &final);
            break;

        case ANIM_NONE:
            final.x = start[0];
            final.y = start[1];
            final.z = start[2];
            final.w = start[3];
            break;

        default:
            OSHalt("Unknown interpolation setting!");
            break;
    }

    CTRLSetQuat(animPipe->control, final.x, final.y, final.z, final.w);
}

/*>*******************************(*)*******************************<*/
static void DoEulerAnimation ( ANIMPipePtr animPipe, ANIMKeyFramePtr startFrame, ANIMKeyFramePtr endFrame )
{
    f32 start[3];
    f32 end[3];
    f32 final[3];
    f32 outControl[3];
    f32 inControl[3];
    u32 offset;
    f32 time;
    u32 data;
    u32 paramSize;
    f32 quantShift;
    u8  quantType;

    quantShift = 1.0f / (f32)(1 << QUANT_SHIFT(animPipe->currentTrack->quantizeInfo));
    quantType  = (u8)QUANT_TYPE(animPipe->currentTrack->quantizeInfo);

    //find the size of each setting parameter
    paramSize = GetDataSize(quantType) * 3;

    // find the offset into the setting info where the rotation information lies
    // (if there is scale information present the rotation info will be located 
    // paramSize into the setting info).
    offset = 0;
    if(animPipe->currentTrack->animType & ANIM_SCALE) // scale
        offset = paramSize;

    //Get the rotation values for the starting keyframe
    GetData(quantType, quantShift, (Ptr)((u32)(startFrame->setting) + offset), start, 3);

    //Get the rotation values for the ending keyframe
    GetData(quantType, quantShift, (Ptr)((u32)(endFrame->setting) + offset), end, 3);

    // normalize the time between the two keyframes
    time = NormalizeTracktime(animPipe, startFrame, endFrame);

    // Figure out the offset for the rotation interpolation data depending
    // on scale interpolation type
    offset = 0;
    if(animPipe->currentTrack->animType & ANIM_SCALE)   // scale exists
    {
        data = (u32)((animPipe->currentTrack->interpolationType >> 2) & 0x03);
        if(data == ANIM_BEZIER) // bezier
            offset = (paramSize << 1);
        if(data == ANIM_HERMITE) // hermite
            offset = (paramSize << 1) + (EASE_SIZE_TWO);
    }

    // switch on how the data is to be interpolated
    switch((animPipe->currentTrack->interpolationType & 0x70) >> 4) // rotation interpolation flag
    {
        case ANIM_NONE:
            final[0] = start[0];
            final[1] = start[1];
            final[2] = start[2];
            break;
        case ANIM_LINEAR:
            LinearInterpolateVecEuler(start, end, time, final);
            break;
        case ANIM_BEZIER:
            GetData(quantType, quantShift, (Ptr)((u32)(startFrame->interpolation) + offset + paramSize), outControl, 3);
            GetData(quantType, quantShift, (Ptr)((u32)(endFrame->interpolation) + offset), inControl, 3);

//            BezierInterpolateVecEuler(start, end, time, outControl, inControl, final);
            BezierInterpolateVec(start, end, time, outControl, inControl, final);
            break;
        case ANIM_HERMITE:
            OSHalt("Hermite interpolation not yet supported for Euler angles");
            break;
        default:
            OSHalt("Unknown interpolation setting!");
            break;
    }

    // send the interpolated data to the control
    CTRLSetRotation(animPipe->control, final[0], final[1], final[2]);
}

/*>*******************************(*)*******************************<*/
static void DoScaleAnimation ( ANIMPipePtr animPipe, ANIMKeyFramePtr startFrame, ANIMKeyFramePtr endFrame )
{
    f32 start[3];
    f32 end[3];
    f32 final[3];
    f32 outControl[3];
    f32 inControl[3];
    f32 easeOut, easeIn;
    f32 time;
    f32 basis[4];
    u32 data;
    u32 paramSize;
    f32 quantShift;
    u8  quantType;

    quantShift = 1.0f / (f32)(1 << QUANT_SHIFT(animPipe->currentTrack->quantizeInfo));
    quantType  = (u8)QUANT_TYPE(animPipe->currentTrack->quantizeInfo);

    paramSize = GetDataSize(quantType) * 3;

    GetData(quantType, quantShift, (Ptr)startFrame->setting, start, 3);
    GetData(quantType, quantShift, (Ptr)endFrame->setting, end, 3);

    time = NormalizeTracktime(animPipe, startFrame, endFrame);

    switch((animPipe->currentTrack->interpolationType & 0x0C) >> 2) // scale interpolation flag
    {
        case ANIM_NONE:
            final[0] = start[0];
            final[1] = start[1];
            final[2] = start[2];
            break;
        case ANIM_LINEAR:
            LinearInterpolateVec(start, end, time, final);
            break;
        case ANIM_BEZIER:
            GetData(quantType, quantShift, (Ptr)((u32)(startFrame->interpolation) + paramSize), outControl, 3);
            GetData(quantType, quantShift, (Ptr)endFrame->interpolation, inControl, 3);

            BezierInterpolateVec(start, end, time, outControl, inControl, final);
            break;
        case ANIM_HERMITE:
            // Get the out and in control for TCB, as well as ease values
            data = (u32)(startFrame->interpolation) + paramSize;
            GetData(quantType, quantShift, (Ptr)data, outControl, 3);
            GetData(EaseType, EaseShift, (Ptr)(data + paramSize + EASE_SIZE), &easeOut, 1);

            GetData(quantType, quantShift, (Ptr)endFrame->interpolation, inControl, 3);    
            GetData(EaseType, EaseShift, (Ptr)((u32)(endFrame->interpolation) + (paramSize << 1)), &easeIn, 1);

            time = Ease( time, easeOut, easeIn );

            ComputeHermiteBasis(time, basis);
            HermiteInterpolateVec(start, end, basis, outControl, inControl, final);
            break;
        default:
            OSHalt("Unknown interpolation setting!");
            break;
    }

    CTRLSetScale(animPipe->control, final[0], final[1], final[2]);
}

/*>*******************************(*)*******************************<*/
static void DoTranslationAnimation ( ANIMPipePtr animPipe, ANIMKeyFramePtr startFrame, ANIMKeyFramePtr endFrame )
{
    f32 start[3];
    f32 end[3];
    f32 final[3];
    f32 outControl[3];
    f32 inControl[3];
    f32 easeOut, easeIn;
    u32 offset;
    f32 time;
    f32 basis[4];
    u32 data;
    u32 paramSize;
    f32 quantShift;
    u8  quantType;

    quantShift = 1.0f / (f32)(1 << QUANT_SHIFT(animPipe->currentTrack->quantizeInfo));
    quantType  = (u8)QUANT_TYPE(animPipe->currentTrack->quantizeInfo);

    paramSize = GetDataSize(quantType) * 3;

    offset = 0;
    if(animPipe->currentTrack->animType & ANIM_SCALE) // scale
        offset = paramSize;
    if(animPipe->currentTrack->animType & ANIM_ROT) // Euler rotation
        offset += paramSize;
    if(animPipe->currentTrack->animType & ANIM_QUAT) // quaternion rotation
        offset += QUAT_SIZE_FOUR;

    GetData(quantType, quantShift, (Ptr)((u32)(startFrame->setting) + offset), start, 3);
    GetData(quantType, quantShift, (Ptr)((u32)(endFrame->setting) + offset), end, 3);

    time = NormalizeTracktime(animPipe, startFrame, endFrame);

    // Figure out the offset for the translation interpolation data depending
    // on scale and rotation interpolation types
    offset = 0;
    if(animPipe->currentTrack->animType & ANIM_SCALE)   // scale exists
    {
        data = (u32)(animPipe->currentTrack->interpolationType >> 2) & 0x03;
        if(data == ANIM_BEZIER) // bezier
            offset = paramSize << 1;
        if(data == ANIM_HERMITE) // hermite
            offset = (paramSize << 1) + EASE_SIZE_TWO;
    }

    if(animPipe->currentTrack->animType & ANIM_QUAT) // quaternion rotation exists
    {
        data = (u32)(animPipe->currentTrack->interpolationType >> 4) & 0x07;
        if(data == ANIM_SQUAD) // squad
            offset += QUAT_SIZE_EIGHT;
        if(data == ANIM_SQUADEE) // squadee
            offset += QUAT_SIZE_EIGHT + EASE_SIZE_TWO;
    }
    else if(animPipe->currentTrack->animType & ANIM_ROT) // euler rotation exists
    {
        data = (u32)(animPipe->currentTrack->interpolationType >> 4) & 0x07;
        if(data == ANIM_BEZIER) // bezier
            offset += (paramSize << 1);
        if(data == ANIM_HERMITE) // hermite
            offset += (paramSize << 1) + (EASE_SIZE_TWO);
    }

    switch(animPipe->currentTrack->interpolationType & 0x03) // translation interpolation flag
    {
        case ANIM_NONE:
            final[0] = start[0];
            final[1] = start[1];
            final[2] = start[2];
            break;
        case ANIM_LINEAR:
            LinearInterpolateVec(start, end, time, final);
            break;
        case ANIM_BEZIER:
            GetData(quantType, quantShift, (Ptr)((u32)(startFrame->interpolation) + offset + paramSize), outControl, 3);
            GetData(quantType, quantShift, (Ptr)((u32)(endFrame->interpolation) + offset), inControl, 3);

            BezierInterpolateVec(start, end, time, outControl, inControl, final);
            break;
        case ANIM_HERMITE:
            // Get the out and in control for TCB, as well as ease values
            data = (u32)(startFrame->interpolation) + offset + paramSize;
            GetData(quantType, quantShift, (Ptr)data, outControl, 3);
            GetData(EaseType, EaseShift, (Ptr)(data + paramSize + EASE_SIZE), &easeOut, 1);

            data = (u32)(endFrame->interpolation + offset);
            GetData(quantType, quantShift, (Ptr)data, inControl, 3);
            GetData(EaseType, EaseShift, (Ptr)(data + (paramSize << 1)), &easeIn, 1);

            time = Ease( time, easeOut, easeIn );

            ComputeHermiteBasis(time, basis);
            HermiteInterpolateVec(start, end, basis, outControl, inControl, final);
            break;
        default:
            OSHalt("Unknown interpolation setting!");
            break;
    }

    CTRLSetTranslation(animPipe->control, final[0], final[1], final[2]);
}

/*>*******************************(*)*******************************<*/
static u32 GetDataSize ( u8 quantType )
{
    switch((GXCompType)quantType)
    {
        case GX_U8:
        case GX_S8:     return 1;
        case GX_U16:
        case GX_S16:    return 2;
        case GX_F32:    return 4;
    }
    
    return 0;
}

/*>*******************************(*)*******************************<*/
static void GetData( u8 quantType, f32 quantShift, Ptr from, f32 *to, u8 count )
{
    // NOTE: To make this function truly efficient, we should use
    // paired-singles function in Gekko CPU to cast and dequantize/shift
    // floats in two instructions (store and load) in a manner similar
    // to OSFastCast API (which casts, but assumes a shift of 1).
    u8  i;

    // Cast to a float from appropriate type
    switch((GXCompType)quantType)
    {
        case GX_U8:
            for( i = 0; i < count; i++ )
            {
                to[i] = (f32)((u8*)from)[i] * quantShift;
            }
            return;
        case GX_S8:
            for( i = 0; i < count; i++ )
            {
                to[i] = (f32)((s8*)from)[i] * quantShift;
            }
            return;
        case GX_U16:
            for( i = 0; i < count; i++ )
            {
                to[i] = (f32)((u16*)from)[i] * quantShift;
            }
            return;
        case GX_S16:
            for( i = 0; i < count; i++ )
            {
                to[i] = (f32)((s16*)from)[i] * quantShift;
            }
            return;
        case GX_F32:
            for( i = 0; i < count; i++ )
            {
                to[i] = ((f32*)from)[i];
            }
            return;
    }
}

/*>*******************************(*)*******************************<*/
void LinearInterpolateVec ( f32 *start, f32 *end, f32 time, f32 *final )
{
    u32 i;

    for( i = 0; i < 3; i++ )
    {
        final[i] = (end[i] * time) + (start[i] * (1.0F - time));
    }
}

/*>*******************************(*)*******************************<*/
void LinearInterpolateVecEuler ( f32 *start, f32 *end, f32 time, f32 *final )
{
    f32 diff;
    u32 i;

    for( i = 0; i < 3; i++ )
    {
        ASSERT( fabs(start[i]) >= 0.0F && fabs(start[i]) <=  360.0F );
        ASSERT( fabs(end[i]) >= 0.0F && fabs(end[i]) <=  360.0F );                   

        diff = start[i] - end[i];

        // Make sure we always interpolate < 180 degrees
        // For example, start is 45 degrees, and end is 315 degrees, then make 
        // sure end is -45 degrees so we interpolate in the right direction.
        if(diff > 180.0F)
            start[i] -= 360.0F;
        else if(diff < -180.0F)
            end[i] -= 360.0F;

        final[i] = (end[i] * time) + (start[i] * (1.0F - time));
    }
}

/*>*******************************(*)*******************************<*/
static void Ticktime ( ANIMPipePtr animPipe )
{
    animPipe->time += animPipe->speed;

    if(animPipe->time > animPipe->currentTrack->animTime)
    {
        animPipe->time -= animPipe->currentTrack->animTime;
    }
}

/*>*******************************(*)*******************************<*/
static f32 NormalizeTracktime ( ANIMPipePtr animPipe, ANIMKeyFramePtr startFrame, ANIMKeyFramePtr endFrame )
{
    f32 time;

    time = animPipe->time - startFrame->time;
    if(!time) 
    {
        return 0.0F;
    }

    if(endFrame->time > startFrame->time)
    {
        return time / (endFrame->time - startFrame->time);
    }
    else
    {
        return time / (animPipe->currentTrack->animTime - startFrame->time);
    }
}

/*>*******************************(*)*******************************<*/
static void BezierInterpolateVecEuler ( f32 *start, f32 *end, f32 u,
                                        f32 *outControl, f32 *inControl, f32 *final )
{
    f32 diff[3];
    u32 i;

    
    for( i = 0; i < 3; i++ )
    {
        ASSERT( fabs(start[i]) >= 0.0F && fabs(start[i]) <=  360.0F );
        ASSERT( fabs(end[i]) >= 0.0F && fabs(end[i]) <=  360.0F );                   

        diff[i] = start[i] - end[i];

        // Make sure we always interpolate < 180 degrees
        // For example, start is 45 degrees, and end is 315 degrees, then make 
        // sure end is -45 degrees so we interpolate in the right direction.
        if(diff[i] > 180.0F)
            start[i] -= 360.0F;
        else if(diff[i] < -180.0F)
            end[i] -= 360.0F;
    }
    
    BezierInterpolateVec(start, end, u, outControl, inControl, final);
}

/*>*******************************(*)*******************************<*/ 
void BezierInterpolateVec( f32 *start, f32 *end, f32 u, f32 *outControl, f32 *inControl, f32 *final )
{
    f32 s = 1.0f - u;
    f32 u2 = u*u;
    f32 out;
    f32 in;
    u32 i;

    for( i = 0; i < 3; i++ )
    {
        out = outControl[i] + start[i];
        in  =  inControl[i] + end[i];

        final[i] = (((s*start[i] + (3.0f*u)*out)*s + (3.0f*u2)*in)*s) + u*u2*end[i];
    }
}

/*>*******************************(*)*******************************<*/
void HermiteInterpolateVec( f32 *start, f32 *end, f32 *basis, f32 *outControl, f32 *inControl, f32 *final )
{
    u32 i;

    for( i = 0; i < 3; i++ )
    {
        final[i] = (start[i]*basis[0] + end[i]*basis[1] + outControl[i]*basis[2] + inControl[i]*basis[3]);
    }
}

/*>*******************************(*)*******************************<*/
static void ComputeHermiteBasis(f32 u, f32 *v) 
{
    f32 u2,u3,a;
    
    u2 = u*u;
    u3 = u2*u;
    a  = 2.0f*u3 - 3.0f*u2;
    v[0] = 1.0f + a;
    v[1] = -a;
    v[2] = u - 2.0f*u2 + u3;
    v[3] = -u2 + u3;
}

/*>*******************************(*)*******************************<*/
static f32 Ease(f32 u, f32 a, f32 b) 
{
    f32 k;
    f32 s = a + b;
/*    
    if (u==0.0f || u==1.0f) return u;
    if (s == 0.0) return u;
    if (s > 1.0f) 
    {
        a = a/s;
        b = b/s;
    }
    k = 1/(2.0f - a - b);
    if (u < a) return ((k/a)*u*u);
    else if (u < 1.0f - b) return (k*(2.0f*u - a));
    else 
    {
        u = 1.0f - u;
        return (1.0f - (k/b) *u*u);
    }
*/
    if (u==0.0f || u==1.0f || s == 0.0f) return u;
    if (s > 1.0f) 
    {
        s = 1.0f / s;
        a = a * s;
        b = b * s;
        k = 0.5f - s;
    }
    else
        k = 0.5f - (1.0f / s);

    if (u < a) 
        return ((k/a)*u*u);
    else if (u < 1.0f - b) 
        return (k*(2.0f*u - a));
    else 
    {
        u = 1.0f - u;
        return (1.0f - (k/b) * u * u);
    }
}

/*>*******************************(*)*******************************<*/
void Slerp ( Qtrn *p, Qtrn *q, f32 t, Qtrn *r )
{
    f32 cosom, omega, sinom, scale0, scale1;

    // Use the dot product to get the cosine of the angle between the quaternions
    cosom = p->x*q->x + p->y*q->y + p->z*q->z + p->w*q->w;

    // Calculate coefficients
    if ( (1.0f + cosom) > COS_EPSILON )
    {
        if ( (1.0f - cosom) > COS_EPSILON )
        {
            // Standard case (slerp)
            omega = acosf( cosom );
            cosom = t * omega;              // cosom is unused so use as temp. var.
            sinom = 1.0f / sinf( omega );
            scale0 = sinf( omega - cosom ) * sinom;
            scale1 = sinf( cosom ) * sinom;
        }
        else
        {
            // Start and q quaternions are very close
            // So do a linear interpolation
            scale0 = 1.0f - t;
            scale1 = t;
        }

        // Calculate the final values
        r->x = (scale0 * p->x) + (scale1 * q->x);
        r->y = (scale0 * p->y) + (scale1 * q->y);
        r->z = (scale0 * p->z) + (scale1 * q->z);
        r->w = (scale0 * p->w) + (scale1 * q->w);
    }
    else
    {
        // Since we found the long way around, use the shorter route
        // Note: 3DSMAX Slerp code in this section is incorrect!!!

        // Start by making result orthogonal to q quat
        r->x = -q->y; 
        r->y = q->x;
        r->z = -q->w;
        r->w = q->z;
        cosom = t * HALFPI;                 // cosom is unused so use as temp. var.
        scale0 = sinf( HALFPI - cosom );
        scale1 = sinf( cosom );

        // Multiply by the scale
        r->x = (scale0 * p->x) + (scale1 * r->x);
        r->y = (scale0 * p->y) + (scale1 * r->y);
        r->z = (scale0 * p->z) + (scale1 * r->z);
        r->w = (scale0 * p->w) + (scale1 * r->w);
    }
}

/* --Squad(p,a,b,q; t) = Slerp(Slerp(p,q;t), Slerp(a,b;t); 2(1-t)t).---*/
void Squad ( Qtrn *p, Qtrn *a, Qtrn *b, Qtrn *q, f32 t, Qtrn *r ) 
{ 
    f32 k = 2.0f * (1.0f - t) * t;
    Qtrn r1, r2;

    Slerp(p,q,t,&r1);
    Slerp(a,b,t,&r2);
    Slerp(&r1, &r2, k, r);
}

