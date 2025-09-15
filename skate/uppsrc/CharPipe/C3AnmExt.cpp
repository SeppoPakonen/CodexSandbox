/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3AnmExt.c]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/build/charPipeline/c3/src/C3AnmExt.c $
    
    14    11/13/00 1:32p John
    Moved C3SortKeyFrames, C3ComputeTrack* functions to C3AnmExtPrivate.h.
    
    13    8/14/00 6:14p John
    Added user-defined data to ANM, ACT, and GPL formats.
    
    12    7/18/00 7:43p John
    Fixed minor bug in moving code from C3Mtx to MTX.
    
    11    7/18/00 7:25p John
    Modified to use new CTRL library.
    Removed Euler XYZ rotation code (unused and unworking).
    Now uses MTX functions instead of C3Mtx functions.
    
    10    4/06/00 3:25p John
    Added MTXRowCol to access matrices using this macro from MTX library.
    
    9     2/29/00 7:19p John
    Sorts keyframes by time if necessary.
    
    8     2/04/00 6:06p John
    Untabified code.
    
    7     1/31/00 4:15p John
    Added pruning of unnecessary bones and animations.
    Finished stitching with FIFO matrix cache and stripping.
    
    6     1/20/00 1:14p John
    Added stitching (work in progress).
    Added C3 string table for efficient management of strings.
    
    5     12/08/99 7:24p John
    Cleaned up track transformation.
    
    4     12/06/99 3:23p John
    Added pools for C3SeqTrack, C3Track, and C3KeyFrame for memory
    efficiency.  Also, does not add empty keyframes to track.
    
    3     12/02/99 2:22p John
    Added TCB animation extraction.
    
    2     11/22/99 6:31p Ryan
    
    10    11/19/99 4:09p John
    
    13    11/18/99 1:14p John
    Moved math stuff to C3Math
    Extracts biped animation (but no TCB/Ease extraction).
    Adding TCB translation/scale control (work in progress)
    
    12    11/11/99 1:12p John
    New animation format.  Added linear, bezier, slerp, and squad
    extraction.
    Added some Quat code temporarily.
    
    11    10/26/99 2:44p John
    Adding animation (work in progress).
    
    10    10/12/99 11:36a John
    Minor cosmetic change.
    
    9     9/29/99 4:27p John
    Changed header to make them all uniform with logs.

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#include "C3Private.h"

// Local functions
static C3Sequence*  C3ValidateSequence( char* name );
static void         C3FreeAnimationBank( C3AnimBankPtr* animBank );
static void         C3FreeAnimationSequence( C3SequencePtr seq );

static s32          C3CompareKeyFrameTime( const void* key1, const void* key2 );
static void         C3MakeTrackQuaternionsClosest( C3Track *track );
static void         C3ComputeTrackSquadAB( C3Track *track );
static void         C3ComputeTrackSquadTCB( C3Track *track, C3Bool closedLoop );
static float        C3ComputeKeyDU ( C3Track* track, C3KeyFrame *key1 );

static void         C3ComputeTrackTCB( C3Track *track, C3Bool closedLoop, u8 controlType );
static void         C3ComputeTCBKey2( C3KeyFrame *key1, C3KeyFrame *key2, u8 controlType );
static void         C3ComputeTCBKeyMiddle( C3Track* track, C3KeyFrame *key, u8 controlType );
static void         C3ComputeTCBKeyFirst( C3KeyFrame *firstKey, u8 controlType );
static void         C3ComputeTCBKeyLast( C3KeyFrame *lastKey, u8 controlType );
static void         C3CompElementDeriv( float pp, float p, float pn, float *ds, float *dd,
                                        float ksm, float ksp, float kdm, float kdp );

// Local variables
static C3AnimationState  c3CurrentAnim = { NULL, NULL, NULL, NULL, NULL, NULL }; 
static C3AnimationState *c3CurrentState = NULL;
static C3AnimationState *c3LastAnim;

// Global variables
u32   AnmUserDataSize = 0;
void *AnmUserData     = NULL;

/*---------------------------------------------------------------------------*
  Name:         C3InitAnimationExtraction

  Description:  Initialize the animation extraction

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3InitAnimationExtraction( void )
{
    C3Sequence  sequence;
    C3Track     track;


    c3CurrentState = &c3CurrentAnim;

    memset( &c3CurrentAnim, 0, sizeof(C3AnimationState) );

    // Make sure the intialization of the hierarchy has been done
    if( !(c3CurrentAnim.actor = C3GetActor()) )
        C3_ASSERT( c3CurrentAnim.actor );

    // Create the bank
    c3CurrentAnim.bank = (C3AnimBank*)C3_CALLOC(1, sizeof(C3AnimBank));
    if( !c3CurrentAnim.bank )
        C3_ASSERT( c3CurrentAnim.bank );
    memset( c3CurrentAnim.bank, 0, sizeof(C3AnimBank) );

    c3CurrentAnim.actor->animBank = (Ptr)c3CurrentAnim.bank;

    // Initialize the animation list
    DSInitList( &c3CurrentAnim.bank->sequenceList, (Ptr)&sequence, &sequence.link );

    // Initialize the track list
    DSInitList( &c3CurrentAnim.bank->trackList, (Ptr)&track, &track.link );
}


/*---------------------------------------------------------------------------*
  Name:         C3CleanAnimationExtraction

  Description:  Clean the animation extraction

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3CleanAnimationExtraction( void )
{
    if( c3CurrentAnim.actor )
        c3CurrentAnim.actor->animBank = NULL;

    C3FreeAnimationBank( &c3CurrentAnim.bank );

    memset( &c3CurrentAnim, 0, sizeof(C3AnimationState) );
}


/*---------------------------------------------------------------------------*
  Name:         C3SetCurrentAnim

  Description:  Save the current animation pointer and set a new one for
                the extraction of animation

  Arguments:    currAnim - the new animation state 

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetCurrentAnim( C3AnimationState* currAnim )
{
    C3_ASSERT( c3LastAnim == NULL );

    c3LastAnim = c3CurrentState;
    c3CurrentState = currAnim;
}


/*---------------------------------------------------------------------------*
  Name:         C3RestoreCurrentAnim

  Description:  Restores the animation pointer saved by a call to the function
                C3SetCurrentAnim

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3RestoreCurrentAnim( void )
{
    c3CurrentState = c3LastAnim;
    c3LastAnim = NULL;
}


/*---------------------------------------------------------------------------*
  Name:         C3BeginAnimation

  Description:  Start the creation of an animation node that animates the
                hierarchy node given by hierNodeName.  C3BeginAnimation/
                C3EndAnimation does not need to be called within the context
                of C3BeginHierarchyNode/C3EndHierarchy node if you send in 
                a valid hierarchy node name.  Otherwise, if hierNodeName is NULL,
                it will assume you are animating the current hierarchy node,
                so this function must be called between the function calls of
                C3BeginHierarchyNode/C3EndHierarchyNode.  C3BeginAnimation
                cannot be nested (unlike hierarchy nodes), and the animation
                must be ended before another starts.

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3BeginAnimation( char* hierNodeName )
{
    C3HierNode *hierNode = NULL;

    // Make sure we are not currently generating an animation
    C3_ASSERT( c3CurrentAnim.owner == NULL );
    C3_ASSERT( c3CurrentAnim.actor != NULL );
    C3_ASSERT( c3CurrentAnim.bank != NULL);

    c3CurrentState = &c3CurrentAnim;

    c3CurrentState->keyFrame = NULL;
    c3CurrentState->track    = NULL;
    c3CurrentState->sequence = NULL;

    if( hierNodeName )
    {
        // Animate the hierarchy node specified by the argument
        c3CurrentState->owner = C3AddStringInTable( hierNodeName );
    }
    else
    {
        // Animate the current hierarchy node
        hierNode = C3GetCurrentHierNode();
        if( hierNode )
            c3CurrentState->owner = hierNode->identifier;
        else
            C3ReportError( "C3BeginAnimation has NULL argument but is not called within C3BeginHierarchyNode/C3EndHierarchyNode context" );
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3EndAnimation

  Description:  End the creation of an animation node

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3EndAnimation( void )
{
    // Make sure the track is ended
    if( c3CurrentState->track )
        C3EndTrack();

    c3CurrentState->owner = NULL;
}


/*---------------------------------------------------------------------------*
  Name:         C3BeginTrack

  Description:  Create a track in the specified sequence.

  Arguments:    sequenceName - name of the sequence to create the track in

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3BeginTrack( char* sequenceName )
{
    C3KeyFrame  keyframe;
    char        nullSeq[] = "NULL";

    C3_ASSERT( c3CurrentState->actor );

    // Allow the conversion of non existing sequence
    if( !sequenceName )
    {
        sequenceName = C3AddStringInTable( nullSeq );
    }

    // Find the sequence from the sequence name
    c3CurrentState->sequence = C3ValidateSequence( sequenceName );

    // Create a new track
    c3CurrentState->track = C3PoolTrack();
    
    // Initialize track with default options
    memset( c3CurrentState->track, 0, sizeof(C3Track) ); 
    C3SetInterpTypeTranslation( C3_INTERPTYPE_NONE );
    C3SetInterpTypeRotation( C3_INTERPTYPE_NONE );
    C3SetInterpTypeScale( C3_INTERPTYPE_NONE );

    // At runtime, replace the hierarchy control with the animation control.
    // This is provided for backward compatibility.  Should always be true.
    c3CurrentState->track->replaceHierarchyCtrl = 1;

    // Initialize the list
    DSInitList( &c3CurrentState->track->keyList, (Ptr)&keyframe, &keyframe.link );

    // Set the target node
    c3CurrentState->track->owner = c3CurrentState->owner;
}


/*---------------------------------------------------------------------------*
  Name:         C3EndTrack

  Description:  End a track

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3EndTrack( void )
{
    C3SeqTrack *seqTrack = NULL;

    C3_ASSERT( c3CurrentState->track->endTime > 0 );

    // Make sure the last keyframe was ended
    if( c3CurrentState->keyFrame )
        C3EndKeyFrame( );

    if( c3CurrentState->track->numKeys == 0 )
    {
        c3CurrentState->track->owner = NULL;
    }
    else
    {
        // Create a sequence track pointer 
        seqTrack = C3PoolSeqTrack();

        // Add the track to the sequence
        seqTrack->track = c3CurrentState->track;
        DSInsertListObject( &c3CurrentState->sequence->trackList, NULL, (Ptr)seqTrack ); 
        c3CurrentState->sequence->numTracks++;

        // Add the track to the bank
        DSInsertListObject( &c3CurrentState->bank->trackList, NULL, (Ptr)c3CurrentState->track ); 
        c3CurrentState->track->index = c3CurrentState->bank->numTracks++;
    }

    c3CurrentState->track = NULL;
    c3CurrentState->sequence = NULL;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetStartTime

  Description:  Set the start time of the current track

  Arguments:    time - time to set

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetStartTime( float time )
{
    if( c3CurrentState->track )
        c3CurrentState->track->startTime = time;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetEndTime

  Description:  Set the end time of the current track

  Arguments:    time - time to set

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetEndTime( float time )
{
    if( c3CurrentState->track )
        c3CurrentState->track->endTime = time;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetInterpTypeTranslation

  Description:  Set the interpolation type for translation

  Arguments:    interpType - interpolation type to set
                C3_INTERPTYPE_NONE, C3_INTERPTYPE_LINEAR,
                C3_INTERPTYPE_BEZIER, C3_INTERPTYPE_HERMITE.

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetInterpTypeTranslation( u8 interpType )
{
    if( c3CurrentState->track )
        c3CurrentState->track->interpolationType |= (interpType << C3_INTERPTYPE_SHIFT_TRANS);  
}


/*---------------------------------------------------------------------------*
  Name:         C3SetInterpTypeScale

  Description:  Set the interpolation type for scale

  Arguments:    interpType - interpolation type to set
                C3_INTERPTYPE_NONE, C3_INTERPTYPE_LINEAR,
                C3_INTERPTYPE_BEZIER, C3_INTERPTYPE_HERMITE.

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetInterpTypeScale( u8 interpType )
{
    if( c3CurrentState->track )
        c3CurrentState->track->interpolationType |= (interpType << C3_INTERPTYPE_SHIFT_SCALE);  
}


/*---------------------------------------------------------------------------*
  Name:         C3SetInterpTypeRotation

  Description:  Set the interpolation type for rotation

  Arguments:    interpType - interpolation type to set
                C3_INTERPTYPE_NONE, C3_INTERPTYPE_LINEAR,
                C3_INTERPTYPE_BEZIER, C3_INTERPTYPE_HERMITE,
                C3_INTERPTYPE_SLERP, C3_INTERPTYPE_SQUAD, 
                C3_INTERPTYPE_SQUADEE.

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetInterpTypeRotation( u8 interpType )
{
    if( c3CurrentState->track )
        c3CurrentState->track->interpolationType |= (interpType << C3_INTERPTYPE_SHIFT_ROT);    
}


/*---------------------------------------------------------------------------*
  Name:         C3ValidateSequence

  Description:  Make sure the sequence is valid and retrieves a pointer to it.
                - will create a new sequence if the sequence is not found

  Arguments:    name - name of the sequence to find

  Returns:      the sequence requested
/*---------------------------------------------------------------------------*/
static C3Sequence*
C3ValidateSequence( char* name )
{
    C3SeqTrack track;
    C3Sequence *cursor;
    C3Sequence *seq     = NULL;


    // search the sequence in the sequence list of the bank
    cursor = (C3Sequence*)(c3CurrentState->bank->sequenceList.Head);
    while( cursor )
    {
        if( strcmp( cursor->name.str, name ) == 0 )
        {
            seq = cursor;
            break;
        }

        cursor = (C3Sequence*)seq->link.Next;
    }

    // If not found
    if( !seq )
    {
        // Create a new sequence
        seq = (C3Sequence*)C3_CALLOC(1, sizeof(C3Sequence));
        C3_ASSERT( seq );

        // Initialize the sequence
        DSInitList( &seq->trackList, (Ptr)&track, &track.link );

        seq->name.str = C3AddStringInTable( name );

        // Add the sequence to the bank
        DSInsertListObject( &c3CurrentState->bank->sequenceList, NULL, (Ptr)seq );
        c3CurrentState->bank->numSequences++;
    }

    // set the current sequence pointer
    c3CurrentState->sequence = seq;

    return seq;
}


/*---------------------------------------------------------------------------*
  Name:         C3BeginKeyFrame

  Description:  Create a new keyframe

  Arguments:    time - time at which to create the keyframe

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3BeginKeyFrame( f32 time )
{
    C3KeyFrame *kf = NULL;

    C3_ASSERT( c3CurrentState->track && c3CurrentState->sequence );
    C3_ASSERT( c3CurrentState->actor );

    // Create a new keyframe
    kf = C3PoolKeyFrame();
    memset( kf, 0, sizeof(C3KeyFrame) );

    // Set the time
    kf->time = time;

    // The index has already been set to 0.  The index is used until C3EndKeyFrame
    // to test if the keyframe will be filled with at least a translation, 
    // rotation, or scale.

    c3CurrentState->keyFrame = kf;
}


/*---------------------------------------------------------------------------*
  Name:         C3EndKeyFrame

  Description:  End a the current keyframe

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3EndKeyFrame( void )
{
    C3_ASSERT( c3CurrentState->track && c3CurrentState->keyFrame );

    // If the flag has been set, then this is not an empty keyframe
    if( c3CurrentState->keyFrame->index != 0 )
    {
        // Check to see if we need to sort keyframes when we end track
        // Compare current key frame with last extracted keyframe
        if( c3CurrentState->track->keyList.Tail &&
            c3CurrentState->keyFrame->time < ((C3KeyFrame*)c3CurrentState->track->keyList.Tail)->time )
            c3CurrentState->track->sortKeyFramesNeeded = C3_TRUE;

        // Add the keyframe in the current track
        DSInsertListObject( &c3CurrentState->track->keyList, NULL, (Ptr)c3CurrentState->keyFrame );
        c3CurrentState->bank->numKeyframes++;
        c3CurrentState->keyFrame->index = c3CurrentState->track->numKeys++;
    }

    // Remove the keyframe pointer
    c3CurrentState->keyFrame = NULL;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetKeyTranslation

  Description:  Set the translation for the key

  Arguments:    x - translation along x axis
                y - translation along y axis
                z - translation along z axis

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3SetKeyTranslation( float x, float y, float z )
{
    // Make sure there is a current control
    if( c3CurrentState->keyFrame )
    {
        CTRLSetTranslation( &c3CurrentState->keyFrame->control, x, y, z );

        // Set the flag to signal that this is not an empty keyframe
        c3CurrentState->keyFrame->index = -1;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3SetKeyRotationQuat

  Description:  Set the rotation information for the key in quaternion

  Arguments:    x, y, z, w - quaternion

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3SetKeyRotationQuat( float x, float y, float z, float w )
{
    // Make sure there is a current control
    if( c3CurrentState->keyFrame )
    {
        CTRLSetQuat( &c3CurrentState->keyFrame->control, x, y, z, w );

        // Normalize the quaternion
        C3QuatNormalize( &c3CurrentState->keyFrame->control.controlParams.srt.r, 
                         &c3CurrentState->keyFrame->control.controlParams.srt.r );

        // Set the flag to signal that this is not an empty keyframe
        c3CurrentState->keyFrame->index = -1;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3SetKeyRotationEuler

  Description:  Set the rotation information for the key

  Arguments:    x - rotation on the x axis (degrees)
                y - rotation on the y axis (degrees)
                z - rotation on the z axis (degrees)

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3SetKeyRotationEuler( float x, float y, float z )
{
    // Make sure there is a current control
    if( c3CurrentState->keyFrame )
    {
        CTRLSetRotation( &c3CurrentState->keyFrame->control, x, y, z );

        // Set the flag to signal that this is not an empty keyframe
        c3CurrentState->keyFrame->index = -1;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3SetKeyScale

  Description:  Set the scaling information for the key

  Arguments:    x - scale on the x axis (degrees)
                y - scale on the y axis (degrees)
                z - scale on the z axis (degrees)

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3SetKeyScale( float x, float y, float z )
{
    // Make sure there is a current control
    if( c3CurrentState->keyFrame )
    {
        CTRLSetScale( &c3CurrentState->keyFrame->control, x, y, z );

        // Set the flag to signal that this is not an empty keyframe
        c3CurrentState->keyFrame->index = -1;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3SetKeyMatrix

  Description:  Set a matrix as the key frame information

  Arguments:    matrix - matrix to set

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3SetKeyMatrix( MtxPtr matrix )
{
    // make sure there is a current control
    if( c3CurrentState->keyFrame )
    {
        CTRLSetMatrix( &c3CurrentState->keyFrame->control, matrix );

        // Set the flag to signal that this is not an empty keyframe
        c3CurrentState->keyFrame->index = -1;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3SetKeyInControl

  Description:  Set the in control for translation, rotation, or scale 
                (depending on controlType) for the key frame.
                For bezier interpolation, parameters should be the tangent
                of the curve.

  Arguments:    controlType - translation, rotation, or scale
                CTRL_TRANS, CTRL_ROT_QUAT, CTRL_ROT_EULER, CTRL_SCALE.
                x, y, z, w - parameters (w will be ignored if appropriate)

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetKeyInControl( u8 controlType, float x, float y, float z, float w )
{
    if( c3CurrentState->keyFrame )
    {
        if( controlType & CTRL_TRANS )
        {
            c3CurrentState->keyFrame->controlInterpTrans.inControl.x = x;
            c3CurrentState->keyFrame->controlInterpTrans.inControl.y = y;
            c3CurrentState->keyFrame->controlInterpTrans.inControl.z = z;
            c3CurrentState->keyFrame->controlInterpTrans.inControl.w = w;
        }
        else if( controlType & CTRL_ROT_QUAT || controlType & CTRL_ROT_EULER )
        {
            c3CurrentState->keyFrame->controlInterpRot.inControl.x = x;
            c3CurrentState->keyFrame->controlInterpRot.inControl.y = y;
            c3CurrentState->keyFrame->controlInterpRot.inControl.z = z;
            c3CurrentState->keyFrame->controlInterpRot.inControl.w = w;
        }
        else if( controlType & CTRL_SCALE )
        {
            c3CurrentState->keyFrame->controlInterpScale.inControl.x = x;
            c3CurrentState->keyFrame->controlInterpScale.inControl.y = y;
            c3CurrentState->keyFrame->controlInterpScale.inControl.z = z;
            c3CurrentState->keyFrame->controlInterpScale.inControl.w = w;
        }
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3SetKeyOutControl

  Description:  Set the out control for translation, rotation, or scale 
                (depending on controlType) for the key frame.
                For bezier interpolation, parameters should be the tangent
                of the curve.

  Arguments:    controlType - translation, rotation, or scale
                CTRL_TRANS, CTRL_ROT_QUAT, CTRL_ROT_EULER, CTRL_SCALE.
                x, y, z, w - parameters (w will be ignored if appropriate)

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetKeyOutControl( u8 controlType, float x, float y, float z, float w )
{
    if( c3CurrentState->keyFrame )
    {
        if( controlType & CTRL_TRANS )
        {
            c3CurrentState->keyFrame->controlInterpTrans.outControl.x = x;
            c3CurrentState->keyFrame->controlInterpTrans.outControl.y = y;
            c3CurrentState->keyFrame->controlInterpTrans.outControl.z = z;
            c3CurrentState->keyFrame->controlInterpTrans.outControl.w = w;
        }
        else if( controlType & CTRL_ROT_QUAT || controlType & CTRL_ROT_EULER )
        {
            c3CurrentState->keyFrame->controlInterpRot.outControl.x = x;
            c3CurrentState->keyFrame->controlInterpRot.outControl.y = y;
            c3CurrentState->keyFrame->controlInterpRot.outControl.z = z;
            c3CurrentState->keyFrame->controlInterpRot.outControl.w = w;
        }
        else if( controlType & CTRL_SCALE )
        {
            c3CurrentState->keyFrame->controlInterpScale.outControl.x = x;
            c3CurrentState->keyFrame->controlInterpScale.outControl.y = y;
            c3CurrentState->keyFrame->controlInterpScale.outControl.z = z;
            c3CurrentState->keyFrame->controlInterpScale.outControl.w = w;
        }
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3SetKeyTCB

  Description:  Set the tension, continuity, and bias parameters 
                for translation, rotation, or scale (depending on controlType) 
                for the key frame.  IMPORTANT: Uses the outControl
                datastructure to store TCB values.

  Arguments:    controlType - translation, rotation, or scale
                CTRL_TRANS, CTRL_ROT_QUAT, CTRL_ROT_EULER, CTRL_SCALE.
                t, c, b     - tension, continuity, and bias (between -1.0 and 1.0)

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetKeyTCB( u8 controlType, float tension, float continuity, float bias )
{
    C3SetKeyOutControl( controlType, tension, continuity, bias, 0 );
}


/*---------------------------------------------------------------------------*
  Name:         C3SetKeyEase

  Description:  Set the ease in and ease out for translation, rotation, or scale 
                (depending on controlType) for the key frame.

  Arguments:    controlType - translation, rotation, or scale
                CTRL_TRANS, CTRL_ROT_QUAT, CTRL_ROT_EULER, CTRL_SCALE.
                easeIn  - ease-in, should be between -1.0 and 1.0
                easeOut - ease-out, should be between -1.0 and 1.0

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetKeyEase( u8 controlType, float easeIn, float easeOut )
{
    if( c3CurrentState->keyFrame )
    {
        if( controlType & CTRL_TRANS )
        {
            c3CurrentState->keyFrame->controlInterpTrans.inEase = easeIn;
            c3CurrentState->keyFrame->controlInterpTrans.outEase = easeOut;
        }
        else if( controlType & CTRL_ROT_QUAT || controlType & CTRL_ROT_EULER )
        {
            c3CurrentState->keyFrame->controlInterpRot.inEase = easeIn;
            c3CurrentState->keyFrame->controlInterpRot.outEase = easeOut;
        }
        else if( controlType & CTRL_SCALE )
        {
            c3CurrentState->keyFrame->controlInterpScale.inEase = easeIn;
            c3CurrentState->keyFrame->controlInterpScale.outEase = easeOut;
        }
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3SortKeyFrames

  Description:  Sorts the keyframes in the track in ascending time order.

  Arguments:    track - all keys in this track will be sorted

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SortKeyFrames( C3Track *track )
{
    C3KeyFrame *keyFrameArray, *cursor;
    u32        i;
    DSLink     link;
    
    if( track->numKeys < 2 )
        return;

    // Copy keyframe linked list to an array, so we can do a quicksort on it
    keyFrameArray = (C3KeyFrame*) C3_CALLOC( track->numKeys, sizeof(C3KeyFrame) );
    i = 0;
    cursor = (C3KeyFrame*)track->keyList.Head;
    while( cursor )
    {
        memcpy( &keyFrameArray[i], cursor, sizeof(C3KeyFrame) );

        i++;
        cursor = (C3KeyFrame*)cursor->link.Next;
    }

    // Verify the number of keyframes
    C3_ASSERT( i == track->numKeys );

    // Sort the keyframe array by time (qsort only works with arrays)
    qsort( keyFrameArray, track->numKeys, sizeof(C3KeyFrame), C3CompareKeyFrameTime );

    // In order, copy over the keyframes back to the keyList
    // without altering the link information
    cursor = (C3KeyFrame*)track->keyList.Head;
    i = 0;
    while( cursor )
    {
        link = cursor->link;
        memcpy( cursor, &keyFrameArray[i], sizeof(C3KeyFrame) );
        cursor->link = link;
        
        cursor->index = i;

        i++;
        cursor = (C3KeyFrame*)cursor->link.Next;
    }

    C3_FREE( keyFrameArray );
}


/*---------------------------------------------------------------------------*
  Name:         C3CompareKeyFrameTime

  Description:  Compares the times for two keyframes and returns -1 if key1
                time is less than key2 time, 1 if greater, and 0 if equal.

  Arguments:    key1 - first keyframe to compare
                key2 - second keyframe to compare

  Returns:      -1 if key1 time is less than key2, 1 if greater, 0 if equal
/*---------------------------------------------------------------------------*/
static s32
C3CompareKeyFrameTime( const void* key1, const void* key2 )
{
    if( ((C3KeyFrame*)key1)->time < ((C3KeyFrame*)key2)->time )
        return -1;
    else if( ((C3KeyFrame*)key1)->time > ((C3KeyFrame*)key2)->time )
        return 1;

    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         C3ComputeTrackWithTransform

  Description:  Transforms positions, rotations, and scale of animation
                keyframes to make them relative to the ACT bone's 
                orientation control's.  Must be called before
                C3ComputeTrackInOutControl.  Parts of this function may be 
                3DSMAX Release 3.1 specific.

  Arguments:    track - all keys in this track will be transformed 

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3ComputeTrackWithTransform( C3Track *track )
{
    C3KeyFrame *cursor, *next;
    f32         du;
    C3HierNode *hierNode;
    u8          interpTypeScale, interpTypeRot, interpTypeTrans;
    /*
    Mtx         rotTransform;
    Quaternion  rotQuat;
    Vec         temp, s;
    f32        *scaleTransform;
    u32         i;
    Mtx         invHierMtx;
    */
    interpTypeScale = C3_INTERPTYPE_SCALE( track->interpolationType );
    interpTypeRot   = C3_INTERPTYPE_ROT( track->interpolationType );
    interpTypeTrans = C3_INTERPTYPE_TRANS( track->interpolationType );

    // Make sure animation control replaces hierarchy control since we will
    // not be transforming animation to be relative to hierarchy anymore.
    C3_ASSERT( track->replaceHierarchyCtrl == 1 );

    hierNode = C3GetHierNodeFromIdentifier( track->owner );
    if( !hierNode )
        // This animation track has no owner so it should be pruned later
        return;

    cursor = (C3KeyFrame*)track->keyList.Head;
    while( cursor )
    {
        next = (C3KeyFrame*)cursor->link.Next;

        // Calculate the du for bezier control point calculation
        du = C3ComputeKeyDU( track, cursor ) / 3.0f;

        // Calculate dy, the bezier control point offsets for in and out tangents
        // dy = tan(angle) * du (inControl and outControl should be tangent value)
        if( interpTypeTrans == C3_INTERPTYPE_BEZIER )
        {
            VECScale( (Vec*)&cursor->controlInterpTrans.outControl, (Vec*)&cursor->controlInterpTrans.outControl, du );
                    
            if( next )
                VECScale( (Vec*)&next->controlInterpTrans.inControl, (Vec*)&next->controlInterpTrans.inControl, du );
        }

        if ( interpTypeRot == C3_INTERPTYPE_BEZIER )
        {
            VECScale( (Vec*)&cursor->controlInterpRot.outControl, (Vec*)&cursor->controlInterpRot.outControl, du );

            if ( next )
                VECScale( (Vec*)&next->controlInterpRot.inControl, (Vec*)&next->controlInterpRot.inControl, du );
        }

        if( interpTypeScale == C3_INTERPTYPE_BEZIER )
        {
            VECScale( (Vec*)&cursor->controlInterpScale.outControl, (Vec*)&cursor->controlInterpScale.outControl, du );

            if( next )
                VECScale( (Vec*)&next->controlInterpScale.inControl, (Vec*)&next->controlInterpScale.inControl, du );
        }

        cursor = next;
    }

    /*
        C3_ASSERT( hierNode->control && hierNode->control->c.type == CTRL_MTX );
        MTXInverse( hierNode->control->c.controlParams.mtx.m, invHierMtx );

        // Make a quaternion from transform matrix to transform quaternions
        C3MtxToQuat( invHierMtx, &rotQuat );

        // Make the rotation into a matrix to transform vectors w/o translation or scale
        MTXQuat( rotTransform, &rotQuat );

        cursor = (C3KeyFrame*)track->keyList.Head;
        while( cursor )
        {
            next = (C3KeyFrame*)cursor->link.Next;

            // === Transform matrix
            if( cursor->control.type & CTRL_MTX )
            {
                MTXConcat( invHierMtx, cursor->control.controlParams.mtx.m, cursor->control.controlParams.mtx.m );
                cursor = next;
                continue;
            }

            // === Transform translation
            if( cursor->control.type & CTRL_TRANS )
            {
                MTXMultVec( invHierMtx, &cursor->control.controlParams.srt.t, &cursor->control.controlParams.srt.t );
            }

            // Calculate dy, the bezier control point offsets for in and out tangents
            // dy = tan(angle) * du (inControl and outControl should be tangent value)
            if( interpTypeTrans == C3_INTERPTYPE_BEZIER )
            {
                MTXMultVec( rotTransform, (Vec*)&cursor->controlInterpTrans.outControl, (Vec*)&cursor->controlInterpTrans.outControl );
                    
                if( next )
                {
                    MTXMultVec( rotTransform, (Vec*)&next->controlInterpTrans.inControl, (Vec*)&next->controlInterpTrans.inControl );  
                }
            }

            // === Transform rotation
            if( interpTypeRot == C3_INTERPTYPE_SLERP ||
                interpTypeRot == C3_INTERPTYPE_SQUAD ||
                interpTypeRot == C3_INTERPTYPE_SQUADEE )
            {
                cursor->control.controlParams.srt.r = C3QuatMultiply( cursor->control.controlParams.srt.r, rotQuat );

                // In and Out controls have yet to be computed, but when they are computed,
                // the controls will be based off these transformed quaternions.

                // Transform from left-hand coordinate system to right-hand coordinate system
                cursor->control.controlParams.srt.r.w = -cursor->control.controlParams.srt.r.w;
            } 

            // === Transform scale
            if( cursor->control.type & CTRL_SCALE )
            {
                // Extract the scale from the transformation matrix
                scaleTransform = (f32*)&s;
                for( i = 0; i < 3; i++ ) 
                {
                    temp.x = MTXRowCol(invHierMtx,0,i);
                    temp.y = MTXRowCol(invHierMtx,1,i);
                    temp.z = MTXRowCol(invHierMtx,2,i);
                    scaleTransform[i] = (f32)sqrt(temp.x*temp.x + temp.y*temp.y + temp.z*temp.z);
                }

                cursor->control.controlParams.srt.s.x *= scaleTransform[0];
                cursor->control.controlParams.srt.s.y *= scaleTransform[1];
                cursor->control.controlParams.srt.s.z *= scaleTransform[2];
            }

            cursor = next;
        }

    */
}


/*---------------------------------------------------------------------------*
  Name:         C3ComputeTrackInOutControl

  Description:  Computes in and out controls (or tangents) for a track
                depending on the interpolation type.  Should be called after
                C3ComputeTrackWithTransform.

  Arguments:    controlType - translation, rotation, or scale
                CTRL_TRANS, CTRL_ROT_QUAT, CTRL_SCALE.
                interpType - interpolation type
                C3_INTERPTYPE_*
                track - track to compute in and out controls for

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3ComputeTrackInOutControl( u8 controlType, u8 interpType, C3Track* track )
{
    switch( interpType )
    {
    case C3_INTERPTYPE_SQUAD:
        C3_ASSERT( controlType & CTRL_ROT_QUAT );
        C3MakeTrackQuaternionsClosest( track );
        C3ComputeTrackSquadAB( track );
        break;

    case C3_INTERPTYPE_SQUADEE:
        C3_ASSERT( controlType & CTRL_ROT_QUAT );
        C3MakeTrackQuaternionsClosest( track );
        C3ComputeTrackSquadTCB( track, C3_FALSE );
        break;

    case C3_INTERPTYPE_HERMITE:
        if( controlType & CTRL_TRANS )
            C3ComputeTrackTCB( track, C3_FALSE, CTRL_TRANS );
        else if( controlType & CTRL_SCALE )
            C3ComputeTrackTCB( track, C3_FALSE, CTRL_SCALE );
        break;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3MakeTrackQuaternionsClosest

  Description:  Makes successive quaternions in a track closest on the 4D
                hypershere since any rotation can be represented by two
                anti-polar quaternions.  Assumes keyframes are in time order
                in the keylist.

  Arguments:    track - track to make quaternions the closest

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3MakeTrackQuaternionsClosest( C3Track *track )
{
    C3KeyFrame *cursor, *prev;

    // Make sure we don't rotate the long way around
    cursor = (C3KeyFrame*)track->keyList.Head;
    prev = NULL;
    while( cursor )
    {
        C3_ASSERT( cursor->control.type & CTRL_ROT_QUAT );

        if( prev )
            C3QuatMakeClosest( &cursor->control.controlParams.srt.r, 
                               prev->control.controlParams.srt.r );

        prev = cursor;
        cursor = (C3KeyFrame*)cursor->link.Next;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3ComputeTrackSquadAB

  Description:  Computes the in and out controls (a and b quaternions) for a 
                squad-type interpolation.  Assumes keyframes are in time order
                in the keyList

  Arguments:    track - track to compute squad quaternion tangents

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3ComputeTrackSquadAB( C3Track *track )
{
    C3KeyFrame *cursor;
    C3KeyFrame *prevKey, *nextKey;
    Quaternion prevQuat, nextQuat, a;

    cursor = (C3KeyFrame*)track->keyList.Head;
    while( cursor )
    {
        prevKey = (C3KeyFrame*)cursor->link.Prev;
        nextKey = (C3KeyFrame*)cursor->link.Next;

        if( prevKey )
            prevQuat = prevKey->control.controlParams.srt.r;
        if( nextKey )
            nextQuat = nextKey->control.controlParams.srt.r;
        if( !prevKey )
            prevQuat = nextQuat;
        if( !nextKey )
            nextQuat = prevQuat;

        a = C3QuatCompA( prevQuat, cursor->control.controlParams.srt.r, nextQuat );
        cursor->controlInterpRot.inControl = a;
        cursor->controlInterpRot.outControl = a;

        cursor = nextKey;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3ComputeTrackSquadTCB

  Description:  Computes the in and out controls (a and b quaternions) for a 
                squad-type interpolation with TCB controls.  Assumes keyframes 
                are in time order in the keyList.  Also assumes that the
                outControl x, y, and z values contain the tension, continuity, 
                and bias parameters.

  Arguments:    track - track to compute squad TCB quaternion tangents
                closedLoop - true if the animation loop is closed

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3ComputeTrackSquadTCB( C3Track *track, C3Bool closedLoop )
{
    s32 n = track->numKeys;
    s32 i = 0;
    s32 j;
    Quaternion qprev, qnext, qnextWrap;
    Quaternion qp, qm, qa, qb, qae, qbe;    
    float      *qpPtr, *qmPtr, *qaPtr, *qbPtr;
    f32 tm,cm,cp,bm,bp,tmcm,tmcp,ksm,ksp,kdm,kdp,c;
    f32 dt,fp,fn;
    f32 temp;
            
    C3KeyFrame *cursor;
    C3KeyFrame *prevKey, *nextKey, *headKey, *tailKey;

    headKey = (C3KeyFrame*)track->keyList.Head;
    tailKey = (C3KeyFrame*)track->keyList.Tail;

    // So we can index Quaternions with [i] operator where 0 <= i <= 3
    qpPtr = (float*)&qp;
    qmPtr = (float*)&qm;
    qaPtr = (float*)&qa;
    qbPtr = (float*)&qb;

    cursor = headKey;
    while( cursor )
    {
        prevKey = (C3KeyFrame*)cursor->link.Prev;
        nextKey = (C3KeyFrame*)cursor->link.Next;

        if( i > 0 || closedLoop ) 
        {
            if( i == 0 ) 
                qprev = tailKey->control.controlParams.srt.r;
            else 
                qprev = prevKey->control.controlParams.srt.r;
            if( C3QuatDot( qprev, cursor->control.controlParams.srt.r ) < 0.0f ) 
                qprev = C3QuatScale( qprev, -1 );
            qm = C3QuatLnDif( qprev, cursor->control.controlParams.srt.r );
        }

        if( i < n-1 || closedLoop ) 
        {
            qnextWrap = i == n-1 ? headKey->control.controlParams.srt.r : nextKey->control.controlParams.srt.r;   

            qnext = qnextWrap;
            if( C3QuatDot( qnext, cursor->control.controlParams.srt.r ) < 0.0f ) 
                qnext = C3QuatScale( qnext, -1 );
            qp = C3QuatLnDif( cursor->control.controlParams.srt.r, qnext );
        }

        if( i == 0 && !closedLoop )   
            qm = qp;
        if( i == n-1 && !closedLoop ) 
            qp = qm;

        fp = fn = 1.0f;
        cm = 1.0f - cursor->controlInterpRot.outControl.y;  // Continuity
        if( i > 0 && i < n-1 || closedLoop ) 
        {
            if( i == 0 ) 
            {
                temp = (float)(track->endTime - tailKey->time + headKey->time - track->startTime);
                dt = 0.5f * (float)(temp + ((C3KeyFrame*)headKey->link.Next)->time - headKey->time);
                fp = ((float)temp) / dt;
                fn = ((float)(((C3KeyFrame*)headKey->link.Next)->time - headKey->time)) / dt;
            } 
            else if( i == n-1 ) 
            {
                temp = (float)(track->endTime - tailKey->time + headKey->time - track->startTime);
                dt = 0.5f * (float)(temp + tailKey->time - ((C3KeyFrame*)tailKey->link.Prev)->time);
                fp = ((float)temp) / dt;
                fn = ((float)(tailKey->time - ((C3KeyFrame*)tailKey->link.Prev)->time)) / dt;
            }
            else 
            {
                dt = 0.5f * (float)( nextKey->time - prevKey->time );
                fp = (float)(cursor->time - prevKey->time) / dt;
                fn = (float)(nextKey->time - cursor->time) / dt;
            }
            c = (float)fabs(cursor->controlInterpRot.outControl.y); // Continuity
            fp = fp + c - c * fp;
            fn = fn + c - c * fn;
        }

        tm = 0.5f * (1.0f - cursor->controlInterpRot.outControl.x); // Tension
        cp = 2.0f - cm;
        bm = 1.0f - cursor->controlInterpRot.outControl.z;          // Bias
        bp = 2.0f - bm;      
        tmcm = tm * cm; 
        tmcp = tm * cp;
        ksm  = 1.0f - tmcm * bp * fp;   
        ksp  = -tmcp * bm * fp;
        kdm  = tmcp * bp * fn;  
        kdp  = tmcm * bm * fn - 1.0f;

        for (j = 0; j < 4; j++) 
        {
            qaPtr[j] = 0.5f * (kdm * qmPtr[j] + kdp * qpPtr[j]);
            qbPtr[j] = 0.5f * (ksm * qmPtr[j] + ksp * qpPtr[j]);
        }

        qae = C3QuatExp( qa );
        qbe = C3QuatExp( qb );

        cursor->controlInterpRot.inControl  = C3QuatMultiply( cursor->control.controlParams.srt.r, qae );
        cursor->controlInterpRot.outControl = C3QuatMultiply( cursor->control.controlParams.srt.r, qbe );

        cursor = nextKey;
        i++;
    }
    
    // Verify track->numKeys
    C3_ASSERT( i == n );
}


static void
C3ComputeTrackTCB( C3Track *track, C3Bool closedLoop, u8 controlType )
{
    u16     numKeys = track->numKeys;
    C3KeyFrame* key = (C3KeyFrame*)track->keyList.Head;

    if( numKeys == 2 && !closedLoop )
    {
        C3ComputeTCBKey2( key, (C3KeyFrame*)key->link.Next, controlType );
        return;
    }

    if( closedLoop )
    {
        while( key )
        {
            C3ComputeTCBKeyMiddle( track, key, controlType );
            key = (C3KeyFrame*)key->link.Next;
        }
    }
    else
    {
        // Compute the middle keys first
        key = (C3KeyFrame*)key->link.Next;
        while( key->link.Next )
        {
            C3ComputeTCBKeyMiddle( track, key, controlType );
            key = (C3KeyFrame*)key->link.Next;
        }

        // Then do first and last
        C3ComputeTCBKeyFirst( (C3KeyFrame*)track->keyList.Head, controlType );
        C3ComputeTCBKeyLast( (C3KeyFrame*)track->keyList.Tail, controlType );
    }
}


static void
C3ComputeTCBKey2( C3KeyFrame *key1, C3KeyFrame *key2, u8 controlType )
{
    f32 dp, t0, t1;
    f32 *outtan, *intan, *value1, *value2;  // So we can access Vec* or Quaternion* as float[]
    u32 j;

    C3_ASSERT( key1 && key2 );

    t0 = t1 = 0;
    outtan = intan = value1 = value2 = NULL;

    if( controlType & CTRL_TRANS )
    {
        t0 = 1.0f - key1->controlInterpTrans.outControl.x;  // tension in 1st keyframe
        t1 = 1.0f - key2->controlInterpTrans.outControl.x;  // tension in 2nd keyframe
        outtan = (f32*)&key1->controlInterpTrans.outControl;
        intan  = (f32*)&key2->controlInterpTrans.inControl;
        value1 = (f32*)&key1->control.controlParams.srt.t;
        value2 = (f32*)&key2->control.controlParams.srt.t;
    }
    else if( controlType & CTRL_SCALE )
    {
        t0 = 1.0f - key1->controlInterpScale.outControl.x;  // tension in 1st keyframe
        t1 = 1.0f - key2->controlInterpScale.outControl.x;  // tension in 2nd keyframe
        outtan = (f32*)&key1->controlInterpScale.outControl;
        intan  = (f32*)&key2->controlInterpScale.inControl;
        value1 = (f32*)&key1->control.controlParams.srt.s;
        value2 = (f32*)&key2->control.controlParams.srt.s;
    }
    else
        C3_ASSERT( C3_FALSE );

    // For x, y, and z
    for(j = 0; j < 3; j++) 
    {
        dp = value2[j] - value1[j];
        outtan[j] = t0 * dp;
        intan[j] = t1 * dp;
    }
}


static void
C3ComputeTCBKeyMiddle( C3Track* track, C3KeyFrame *key, u8 controlType )
{
    f32 tm,cm,cp,bm,bp,tmcm,tmcp,ksm,ksp,kdm,kdp,c;
    f32 dt,fp,fn;
    s32 n = track->numKeys;
    f32 temp;
    s32 j;

    // So we can access Vec* or Quaternion* with the [i] operator, where 0 <= i < 3 for x, y, and z
    f32 *intan, *outtan;
    f32 *valueHead, *valueTail, *valuePrev, *value, *valueNext;

    C3KeyFrame *headKey = (C3KeyFrame*)track->keyList.Head;
    C3KeyFrame *tailKey = (C3KeyFrame*)track->keyList.Tail;
    C3KeyFrame *prevKey = (C3KeyFrame*)key->link.Prev;
    C3KeyFrame *nextKey = (C3KeyFrame*)key->link.Next;

    // fp,fn apply speed correction when continuity is 0.0 
    if( key == headKey ) 
    {
        temp = (float)(track->endTime - tailKey->time + headKey->time - track->startTime);
        dt = 0.5f * (float)(temp + ((C3KeyFrame*)headKey->link.Next)->time - headKey->time);
        fp = ((float)temp) / dt;
        fn = ((float)(((C3KeyFrame*)headKey->link.Next)->time - headKey->time)) / dt;
    } 
    else if( key == tailKey ) 
    {
        temp = (float)(track->endTime - tailKey->time + headKey->time - track->startTime);
        dt = 0.5f * (float)(temp + tailKey->time - ((C3KeyFrame*)tailKey->link.Prev)->time);
        fp = ((float)temp) / dt;
        fn = ((float)(tailKey->time - ((C3KeyFrame*)tailKey->link.Prev)->time)) / dt;
    } 
    else 
    {
        dt = 0.5f * (float)( nextKey->time - prevKey->time );
        fp = (float)(key->time - prevKey->time) / dt;
        fn = (float)(nextKey->time - key->time) / dt;
    }

    if( controlType & CTRL_TRANS )
    {
        c  = (float)fabs(key->controlInterpTrans.outControl.y);     // Continuity for translation
        cm = 1.0f - key->controlInterpTrans.outControl.y;
        tm = 0.5f * (1.0f - key->controlInterpTrans.outControl.x);  // Tension for translation
        bm = 1.0f - key->controlInterpTrans.outControl.z;           // Bias for translation
    }
    else if( controlType & CTRL_SCALE )
    {
        c  = (float)fabs(key->controlInterpScale.outControl.y);     // Continuity for scale
        cm = 1.0f - key->controlInterpScale.outControl.y;
        tm = 0.5f * (1.0f - key->controlInterpScale.outControl.x);  // Tension for scale
        bm = 1.0f - key->controlInterpScale.outControl.z;           // Bias for scale
    }
    else
        C3_ASSERT( C3_FALSE );

    fp = fp + c - c * fp;
    fn = fn + c - c * fn;
    cp = 2.0f - cm;
    bp = 2.0f - bm;      
    tmcm = tm*cm;
    tmcp = tm*cp;
    ksm = tmcm*bp*fp;   
    ksp = tmcp*bm*fp;
    kdm = tmcp*bp*fn;   
    kdp = tmcm*bm*fn;

    if( controlType & CTRL_TRANS )
    {
        intan  = (f32*)&key->controlInterpTrans.inControl;
        outtan = (f32*)&key->controlInterpTrans.outControl;
        if( headKey ) valueHead = (f32*)&headKey->control.controlParams.srt.t;
        if( tailKey ) valueTail = (f32*)&tailKey->control.controlParams.srt.t;
        if( prevKey ) valuePrev = (f32*)&prevKey->control.controlParams.srt.t;
        if( nextKey ) valueNext = (f32*)&nextKey->control.controlParams.srt.t;
        value = (f32*)&key->control.controlParams.srt.t;
    }
    else if( controlType & CTRL_SCALE )
    {
        intan  = (f32*)&key->controlInterpScale.inControl;
        outtan = (f32*)&key->controlInterpScale.outControl;
        if( headKey ) valueHead = (f32*)&headKey->control.controlParams.srt.s;
        if( tailKey ) valueTail = (f32*)&tailKey->control.controlParams.srt.s;
        if( prevKey ) valuePrev = (f32*)&prevKey->control.controlParams.srt.s;
        if( nextKey ) valueNext = (f32*)&nextKey->control.controlParams.srt.s;
        value = (f32*)&key->control.controlParams.srt.s;
    }

    // For x, y, and z
    for( j = 0; j < 3; j++ )
    {
        if( key == headKey ) 
        {
            C3CompElementDeriv( valueTail[j], value[j], valueNext[j],
                                &intan[j], &outtan[j], ksm, ksp, kdm, kdp );
        } 
        else if( key == tailKey ) 
        {
            C3CompElementDeriv( valuePrev[j], value[j], valueHead[j],
                                &intan[j], &outtan[j], ksm, ksp, kdm, kdp );
        } 
        else 
        {
            C3CompElementDeriv( valuePrev[j], value[j], valueNext[j],
                                &intan[j], &outtan[j], ksm, ksp, kdm, kdp );
        }
    }
}


static void
C3ComputeTCBKeyFirst( C3KeyFrame *firstKey, u8 controlType )
{
    f32 t;
    C3KeyFrame *nextKey;
    f32 *intan, *outtan, *valueNext, *value;
    s32 j;

    nextKey = (C3KeyFrame*)firstKey->link.Next;

    if( controlType & CTRL_TRANS )
    {
        t = 0.5f * (1.0f - firstKey->controlInterpTrans.outControl.x);  // Tension
        intan  = (f32*)&nextKey->controlInterpTrans.inControl;
        outtan = (f32*)&firstKey->controlInterpTrans.outControl;
        value  = (f32*)&firstKey->control.controlParams.srt.t;
        valueNext = (f32*)&nextKey->control.controlParams.srt.t;
    }
    else if( controlType & CTRL_SCALE )
    {
        t = 0.5f * (1.0f - firstKey->controlInterpScale.outControl.x);  // Tension
        intan  = (f32*)&nextKey->controlInterpScale.inControl;
        outtan = (f32*)&firstKey->controlInterpScale.outControl;
        value  = (f32*)&firstKey->control.controlParams.srt.s;
        valueNext = (f32*)&nextKey->control.controlParams.srt.s;
    }
    else
        C3_ASSERT( C3_FALSE );

    // For x, y, and z
    for( j = 0; j < 3; j++ ) 
    {
        outtan[j] = t * (3.0f * (valueNext[j] - value[j]) - intan[j]);
    }
}


static void
C3ComputeTCBKeyLast( C3KeyFrame *lastKey, u8 controlType )
{
    f32 t;
    C3KeyFrame *prevKey;
    f32 *intan, *outtan, *valuePrev, *value;
    s32 j;

    prevKey = (C3KeyFrame*)lastKey->link.Prev;

    if( controlType & CTRL_TRANS )
    {
        t = 0.5f * (1.0f - lastKey->controlInterpTrans.outControl.x);   // Tension
        intan  = (f32*)&lastKey->controlInterpTrans.inControl;
        outtan = (f32*)&prevKey->controlInterpTrans.outControl;
        value  = (f32*)&lastKey->control.controlParams.srt.t;
        valuePrev = (f32*)&prevKey->control.controlParams.srt.t;
    }
    else if( controlType & CTRL_SCALE )
    {
        t = 0.5f * (1.0f - lastKey->controlInterpScale.outControl.x);   // Tension
        intan  = (f32*)&lastKey->controlInterpScale.inControl;
        outtan = (f32*)&prevKey->controlInterpScale.outControl;
        value  = (f32*)&lastKey->control.controlParams.srt.s;
        valuePrev = (f32*)&prevKey->control.controlParams.srt.s;
    }
    else
        C3_ASSERT( C3_FALSE );

    // For x, y, and z
    for( j = 0; j < 3; j++ ) 
    {
        intan[j] = -t * (3.0f * (valuePrev[j] - value[j]) + outtan[j]);
    }
}


static void 
C3CompElementDeriv( float pp, float p, float pn, float *ds, float *dd,
                    float ksm, float ksp, float kdm, float kdp )
{
    float delm, delp;

    delm = p - pp;
    delp = pn - p;
    *ds  = ksm*delm + ksp*delp;
    *dd  = kdm*delm + kdp*delp;
}


// Computes time difference between key1 and next key in track
static float
C3ComputeKeyDU ( C3Track* track, C3KeyFrame *key1 )
{
    C3KeyFrame* key2;

    C3_ASSERT( key1 );
    key2 = (C3KeyFrame*)key1->link.Next;

    if( key2 && key2->time > key1->time )
        return (key2->time - key1->time);
    else
        return ((track->endTime - key1->time) + (((C3KeyFrame*)track->keyList.Head)->time - track->startTime));
}


/*---------------------------------------------------------------------------*
  Name:         C3FreeAnimationBank

  Description:  Free the animation bank data

  Arguments:    animBank - the animbank to free

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3FreeAnimationBank( C3AnimBankPtr* animBank )
{
    C3_ASSERT( animBank );

    if( *animBank )
    {
        C3FreeAnimationSequence( (C3SequencePtr)(*animBank)->sequenceList.Head );
        (*animBank)->sequenceList.Head = (*animBank)->trackList.Tail = NULL;

        (*animBank)->trackList.Head = (*animBank)->trackList.Tail = NULL;
        C3_FREE( *animBank );
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3FreeAnimationSequence

  Description:  Recursively frees a sequence list

  Arguments:    seq - head of the sequence list

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3FreeAnimationSequence( C3SequencePtr seq )
{
    Ptr         cursor = NULL;
    C3TrackPtr  temp   = NULL;
  
    if( !seq )
        return;

    // recusively free the list of sequences
    if( seq->link.Next )
        C3FreeAnimationSequence( (C3SequencePtr)seq->link.Next );
    seq->link.Next = NULL;

    // free the name 
    C3_FREE( seq );
}


/*---------------------------------------------------------------------------*
  Name:         C3SetANMUserData

  Description:  Sets the user-defined data and size to be included in the
                ANM file.  The data will be written out as is, so the bytes in
                the user data may need to swapped for endian correctness.

  Arguments:    size - size of data
                data - pointer to the data

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3SetANMUserData( u32 size, void *data )
{
    AnmUserDataSize = size;
    AnmUserData     = data;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetANMUserData

  Description:  Returns the current user-defined data and size.

  Arguments:    size - size of data to set
                data - pointer to the data to set

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3GetANMUserData( u32 *size, void **data )
{
    *size = AnmUserDataSize;
    *data = AnmUserData;
}