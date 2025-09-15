/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3AnmOut.c]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/build/charPipeline/c3/src/C3AnmOut.c $
    
    18    11/13/00 1:35p John
    C3 library now extracts Euler XYZ rotation (including Bezier
    interpolation).
    Hierarchy transformation is no longer a mtx in the layout, but a
    CTRLControl structure of translation, rotation, and scale.
    Animation and hierarchy controls are written to files using separate
    functions.
    Modified so that animation can replace hierarchy transformation (with
    an option enabled in the track).
    
    17    8/14/00 6:14p John
    Added user-defined data to ANM, ACT, and GPL formats.
    
    16    8/04/00 5:05p John
    Eliminated some unused code.
    
    15    7/18/00 7:26p John
    Modified to use new CTRL library.
    Removed Euler XYZ rotation code (unused and unworking).
    Now uses MTX functions instead of C3Mtx functions.
    
    14    6/27/00 6:03p John
    Substituted quantize.h macros.
    
    13    4/06/00 3:26p John
    Added MTXRowCol to access matrices using this macro from MTX library.
    
    12    3/23/00 11:13a John
    Track array was taken out of the ANM format.
    
    11    2/18/00 4:53p John
    Will not output ANM file if less than 2 keyframes.
    
    10    2/04/00 6:06p John
    Untabified code.
    
    9     1/31/00 4:15p John
    Added pruning of unnecessary bones and animations.
    Finished stitching with FIFO matrix cache and stripping.
    
    8     1/20/00 1:14p John
    Added stitching (work in progress).
    Added C3 string table for efficient management of strings.
    
    7     1/07/00 11:37a John
    Fixed bug with ANM format
    
    6     1/04/00 1:12p John
    Added better support for second texture channel.
    
    5     12/15/99 1:19p John
    Added ANM version information.
    
    4     12/13/99 3:39p John
    Moved quantization of hierarchy transform to C3WriteControl.
    
    3     12/08/99 7:25p John
    Added animation quantization.
    
    2     12/02/99 2:23p John
    Fixed the way ease values were written out.
    
    17    11/18/99 1:14p John
    Changed C3GetHierControl
    
    16    11/11/99 1:13p John
    New animation format.  Added linear, bezier, slerp, and squad
    extraction.
    
    15    9/29/99 4:27p John
    Changed header to make them all uniform with logs.
    
    14    7/23/99 5:36p Rod
    Fixed memory release bugs
    
    13    7/19/99 2:22p Rod
    Added a function to allow the output of an external animBank
    
    12    7/09/99 2:39p Rod
    Fixed a bug for non matrix animation ( used to write RST now is SRT)
    
    11    7/01/99 3:44p Rod
    Multiple fixes for the animation output
    
    10    6/21/99 3:26p Rod
    Changed the defines to be GX compliant
    
    9     6/08/99 1:39p Rod
    Completed all function headers
    
    8     6/02/99 11:11a Rod
    Updated the references to controls to the new size of Matrix (3x4)
    Changed the vec/point members to lower case x, y, z

  $NoKeywords: $

 *---------------------------------------------------------------------------*/


#include "C3Private.h"


#define C3_ANIM_HEADER_SIZE     sizeof(ANIMBank)
#define C3_ANIM_SEQUENCE_SIZE   sizeof(ANIMSequence)
#define C3_ANIM_TRACK_SIZE      sizeof(ANIMTrack)   
#define C3_ANIM_KEYFRAME_SIZE   sizeof(ANIMKeyFrame)
#define C3_ANIM_TRACKINDEX_SIZE 2


typedef struct
{
  ANIMBank      animBank;
  
  ANIMKeyFrame* keyFrameArray;
  void*         keyFrameSettings;
  void*         keyFrameInterpSettings;
  char*         stringBank;

  u32           keyFrameSettingSize;
  u32           keyFrameInterpSettingSize;
  u32           stringBankSize;

  u32           keyFrameCursor;
  u32           keyFrameSettingCursor;
  u32           keyFrameInterpSettingCursor;
  u32           stringCursor;

  u32           keyFrameOffset;
  u32           keyFrameSettingOffset;
  u32           keyFrameInterpSettingOffset;
  u32           stringOffset;

  u32           userDataOffset;

} C3ANIMBank, *C3ANIMBankPtr;


static void  C3InitializeAnimBank( C3ANIMBank* animBank );
static void  C3CreateAnimBank( C3AnimBank* bank, C3ANIMBank* animBank );
static void  C3AllocateAnimArray( C3AnimBank* bank, C3ANIMBank* animBank );
static u32   C3CalculateKeyFrameSettingSize( C3Sequence* sequence );
static u32   C3CalculateKeyFrameInterpSettingSize( C3Sequence* sequence );
static void  C3CreateSequences( C3AnimBank* bank, C3ANIMBank* animBank );
static void  C3WriteSequenceName( C3Sequence* sequence, C3ANIMBank* animBank );

static void  C3ConvertTracks( C3AnimBank* bank, C3ANIMBank* animBank );
static u8    C3ConvertKeyFrames( C3Track* track, C3ANIMBank* animBank );

static u32   C3GetAnimControlSize( CTRLControl* control, u8 quantInfo );
static float C3GetTrackTime( C3Track* track );
static u8    C3GetAnimTypeControl( CTRLControl* control );
static u8    C3WriteAnimControl( CTRLControl* control, void* array, u8 quantInfo );

static void  C3WriteAnimBankInt( C3ANIMBank* animBank, char* path );
static void  C3WriteHeader( C3ANIMBank* animBank, FILE* outFile );
static void  C3WriteArrays( C3ANIMBank* animBank, FILE* outFile );

static void  C3WriteSequenceArray(  C3ANIMBank* animBank, FILE* outFile );
static void  C3WriteTrackArray( C3ANIMBank* animBank, FILE* outFile );
static void  C3WriteKeyFrameArray( C3ANIMBank *animBank, FILE *outFile );

static u8    C3WriteControlInterp( C3ControlInterp *controlInterp, u8 interpType, void* array, u8 quantInfo );
static void  C3FreeAnimBank( C3ANIMBank* animBank );

extern u32   AnmUserDataSize;
extern void *AnmUserData;

/*---------------------------------------------------------------------------*
  Name:         C3WriteAnimation

  Description:  Write the animation file in the specified path.

  Arguments:    path - location to save the file to

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3WriteAnimation( char* path, char* name )
{
    C3Actor*    actor     = NULL;
    char*       fileName  = NULL;


    // Make sure the intialization of the hierarchy has been done
    actor = C3GetActor();
    if( !actor ) 
        C3_ASSERT( actor != NULL );

    if( !actor->animBank || ((C3AnimBank*)actor->animBank)->numKeyframes < 2 )
        return;
  
    // Write the animation
    C3MakePath( path, name, C3_EXT_ANIMATION, &fileName );
    C3WriteAnimBank( fileName, (C3AnimBank*)actor->animBank );

    C3_FREE( fileName );
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteAnimBank

  Description:  Write the animation bank in the specified path.

  Arguments:    fileName - location to save the file to
                animBank - the animation bank to write

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3WriteAnimBank( char* fileName, C3AnimBank* animBank )
{
    C3ANIMBank  trueBank;

    if( animBank->numSequences <= 0 )
        return;

    // Reset the anim bank
    C3InitializeAnimBank( &trueBank );

    // Create the anim bank
    C3CreateAnimBank( animBank, &trueBank );

    // Write the anim bank
    C3WriteAnimBankInt( &trueBank, fileName );

    // Free the anim bank  
    C3FreeAnimBank( &trueBank );

}


/*---------------------------------------------------------------------------*
  Name:         C3InitializeAnimBank

  Description:  Initialize the animation global variables.

  Arguments:    animBank - the animBank to initialize

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3InitializeAnimBank( C3ANIMBank* animBank )
{
    memset( animBank, 0, sizeof(C3ANIMBank) );
}


/*---------------------------------------------------------------------------*
  Name:         C3CreateAnimBank

  Description:  Creates an animBank from the C3AnimBank specified
                Converts the animation from c3 to runtime format

  Arguments:    bank        - bank to convert
                animBank    - anim bank to create ( in C3 wrappers)

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3CreateAnimBank( C3AnimBank* bank, C3ANIMBank* animBank )
{
    animBank->animBank.bankID = bank->id;

    animBank->animBank.numSequences = bank->numSequences;
    animBank->animBank.numTracks    = bank->numTracks;
    animBank->animBank.numKeyFrames = bank->numKeyframes;

    // Allocate the arrays
    C3AllocateAnimArray( bank, animBank );

    // Convert the sequences
    C3CreateSequences( bank, animBank );
  
    // Convert the tracks
    C3ConvertTracks( bank, animBank );
}


/*---------------------------------------------------------------------------*
  Name:         C3AllocateAnimArray

  Description:  Allocates the animation arrays

  Arguments:    bank     - bank to take the information from
                animBank - destination of the arrays

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3AllocateAnimArray( C3AnimBank* bank, C3ANIMBank* animBank )
{
    Ptr         cursor   = NULL;
    C3Sequence* sequence = NULL;
    u32         offset   = C3_ANIM_HEADER_SIZE + 
                           (C3_ANIM_SEQUENCE_SIZE * bank->numSequences) +
                           (C3_ANIM_TRACK_SIZE * bank->numTracks); 

    // Allocate sequences
    animBank->animBank.animSequences = (ANIMSequence*)C3_CALLOC(bank->numSequences, sizeof(ANIMSequence));
    C3_ASSERT( animBank->animBank.animSequences );

    // Allocate track
    animBank->animBank.animSequences->trackArray = (ANIMTrack*)C3_CALLOC(bank->numTracks, sizeof(ANIMTrack));
    C3_ASSERT( animBank->animBank.animSequences->trackArray );

    // Allocate keyframe array
    animBank->keyFrameArray = (ANIMKeyFrame*)C3_CALLOC(bank->numKeyframes, sizeof(ANIMKeyFrame));
    C3_ASSERT( animBank->keyFrameArray );

    animBank->keyFrameCursor = 0;
    animBank->keyFrameOffset = offset;
    offset += C3_ANIM_KEYFRAME_SIZE * bank->numKeyframes; 

    // Count string bank size, keyframe setting size
    cursor = bank->sequenceList.Head;
    while( cursor )
    {
        sequence = (C3Sequence*)cursor;
        animBank->keyFrameSettingSize       += C3CalculateKeyFrameSettingSize( sequence );
        animBank->keyFrameInterpSettingSize += C3CalculateKeyFrameInterpSettingSize( sequence );
        animBank->stringBankSize            += (strlen( sequence->name.str ) + 1);
        cursor = sequence->link.Next;
    }

    // Allocate keyframe setting
    C3_PAD32( animBank->keyFrameSettingSize );
    animBank->keyFrameSettings = (void*)C3_CALLOC(animBank->keyFrameSettingSize, 1);
    C3_ASSERT( animBank->keyFrameSettings );
    animBank->keyFrameSettingCursor = 0;
    animBank->keyFrameSettingOffset = offset;
    offset += animBank->keyFrameSettingSize;

    // Allocate keyframe interpolation setting
    if ( animBank->keyFrameInterpSettingSize > 0 )
    {
        C3_PAD32( animBank->keyFrameInterpSettingSize );
        animBank->keyFrameInterpSettings = (void*)C3_CALLOC(animBank->keyFrameInterpSettingSize, 1);
        C3_ASSERT( animBank->keyFrameInterpSettings );
        animBank->keyFrameInterpSettingCursor = 0;
        animBank->keyFrameInterpSettingOffset = offset;
        offset += animBank->keyFrameInterpSettingSize;
    }
    else
    {
        animBank->keyFrameInterpSettings = NULL;
        animBank->keyFrameInterpSettingCursor = 0;
        animBank->keyFrameInterpSettingOffset = 0;
    }

    // Allocate string bank
    C3_PAD32( animBank->stringBankSize );
    animBank->stringBank = (char*)C3_CALLOC( animBank->stringBankSize, 1);
    C3_ASSERT( animBank->stringBank );
    animBank->stringCursor = 0;
    animBank->stringOffset = offset;
    offset += animBank->stringBankSize;

    // Calculate offset to user defined data (starts on a 32-byte boundary)
    if( AnmUserDataSize > 0 )
    {
        C3_PAD32_BYTES( offset );
        animBank->userDataOffset = offset;
    }
    else
        animBank->userDataOffset = 0;
}


/*---------------------------------------------------------------------------*
  Name:         C3CalculateKeyFrameSettingSize

  Description:  Calculate the size of the settings in the specified sequence

  Arguments:    sequence - sequence to calculate the setting size from

  Returns:      size of the keyframe settings
/*---------------------------------------------------------------------------*/
static u32
C3CalculateKeyFrameSettingSize( C3Sequence* sequence )
{
    C3SeqTrack* cursor = NULL;
    C3KeyFrame* key    = NULL;
    u32         size   = 0;

    cursor = (C3SeqTrack*)sequence->trackList.Head;
    while( cursor )
    {
        key = (C3KeyFrame*)cursor->track->keyList.Head;
        while( key )
        {
            size += C3GetAnimControlSize( &key->control, cursor->track->paramQuantizeInfo );
            key = (C3KeyFrame*)(key->link.Next);
        }
        cursor = (C3SeqTrack*)(cursor->link.Next);
    }

    return size;
}


/*---------------------------------------------------------------------------*
  Name:         C3CalculateKeyFrameInterpSettingSize

  Description:  Calculate the size of the interpolation settings in the specified sequence

  Arguments:    sequence - sequence to calculate the interpolation setting size from

  Returns:      size of the keyframe interpolation settings
/*---------------------------------------------------------------------------*/
static u32
C3CalculateKeyFrameInterpSettingSize( C3Sequence* sequence )
{
    C3SeqTrack* cursor = NULL;
    C3KeyFrame* key    = NULL;
    u32         size   = 0;
    u32         numKeyFrames = 0;
    u32         sizePerKeyFrame = 0;
    u8          type;
    u8          paramSize;

    cursor = (C3SeqTrack*)sequence->trackList.Head;
    while( cursor )
    {
        // Find the number of keyframes
        numKeyFrames = cursor->track->numKeys;
        
        // Find the quantization type
        paramSize = C3GetQuantTypeSize( cursor->track->paramQuantizeInfo );

        // Add interpolation size per keyframe for translation
        sizePerKeyFrame = 0;
        type = C3_INTERPTYPE_TRANS( cursor->track->interpolationType );
        if ( type == C3_INTERPTYPE_BEZIER )
            sizePerKeyFrame += 6 * paramSize;
        else if ( type == C3_INTERPTYPE_HERMITE )
            sizePerKeyFrame += (6 * paramSize) + (2 * C3_EASE_SIZE);

        // Add interpolation size per keyframe for rotation
        type = C3_INTERPTYPE_ROT( cursor->track->interpolationType );
        if ( type == C3_INTERPTYPE_SQUAD )          // Squad on quaternions
            sizePerKeyFrame += 8 * C3_QUATERNION_SIZE;
        else if ( type == C3_INTERPTYPE_SQUADEE )   // Squad on quaternions w/ ease
            sizePerKeyFrame += (8 * C3_QUATERNION_SIZE) + (2 * C3_EASE_SIZE);
        else if ( type == C3_INTERPTYPE_BEZIER )    // Bezier on euler angles
            sizePerKeyFrame += 6 * paramSize;
        else if ( type == C3_INTERPTYPE_HERMITE )   // Hermite (TCB) on euler angles w/ ease
            sizePerKeyFrame += (6 * paramSize) + (2 * C3_EASE_SIZE);

        // Add interpolation size per keyframe for scale
        type = C3_INTERPTYPE_SCALE( cursor->track->interpolationType );
        if ( type == C3_INTERPTYPE_BEZIER )
            sizePerKeyFrame += 6 * paramSize;
        else if ( type == C3_INTERPTYPE_HERMITE )
            sizePerKeyFrame += (6 * paramSize) + (2 * C3_EASE_SIZE);

        size += cursor->track->numKeys * sizePerKeyFrame;

        cursor = (C3SeqTrack*)(cursor->link.Next);
    }

    return size;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetAnimControlSize

  Description:  Calculates the size of the control according to the components
                and the quantizeInfo

  Arguments:    control - control to calculate the size of
                quantizeInfo - quantization info for translation and scale

  Returns:      size of the control 
/*---------------------------------------------------------------------------*/
static u32
C3GetAnimControlSize( CTRLControl* control, u8 quantizeInfo )
{
    u32 size = 0;
    u8 paramSize;

    C3_ASSERT( control );

    paramSize = C3GetQuantTypeSize( quantizeInfo );
  
    if( control->type & CTRL_MTX )
        return (12 * paramSize);

    if( control->type & CTRL_ROT_QUAT )
        size += 4 * C3_QUATERNION_SIZE;
    else if( control->type & CTRL_ROT_EULER )
        size += 3 * paramSize;

    if( control->type & CTRL_TRANS )
        size += 3 * paramSize;

    if( control->type & CTRL_SCALE )
        size += 3 * paramSize;
  
    return size;
}


/*---------------------------------------------------------------------------*
  Name:         C3CreateSequences

  Description:  Creates the runtime sequences from the C3 structures
                Assumes the sequence array in the destination has already 
                been allocated.

  Arguments:    bank     - src of the sequences
                animBank - dst of the sequences

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3CreateSequences( C3AnimBank* bank, C3ANIMBank* animBank )
{
    C3Sequence*     cursor    = NULL;
    C3SeqTrack*     tCursor   = NULL;
    ANIMSequence*   sequence  = NULL; 
    u16             seqIndex  = 0;
    u16             index     = 0;
    u32             offset    = 0;


    cursor = (C3Sequence*)bank->sequenceList.Head;
    while( cursor )
    {
        sequence = animBank->animBank.animSequences + seqIndex++;

        // set the sequence name
        sequence->sequenceName = (char*)(animBank->stringOffset + animBank->stringCursor);
        C3WriteSequenceName( cursor, animBank );  
    
        // set the number of tracks
        sequence->totalTracks = cursor->numTracks;

        cursor = (C3Sequence*)cursor->link.Next;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteSequenceName

  Description:  Copies the sequence name in the anim bank string bank
                Assumes the string bank has been allocated

  Arguments:    sequence - sequence from which to take the name
                animBank - container of the string bank

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3WriteSequenceName( C3Sequence* sequence, C3ANIMBank* animBank )
{
    strcpy( animBank->stringBank + animBank->stringCursor, sequence->name.str );
    animBank->stringCursor += strlen( sequence->name.str ) + 1;
}


/*---------------------------------------------------------------------------*
  Name:         C3ConvertTracks

  Description:  Converts the tracks from the C3 format to the runtime format
                Assumes the keys are in time order...
                Assumes the track array has already been allocated

  Arguments:    bank     - src of the tracks
                animBank - destination of the tracks
                
  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3ConvertTracks( C3AnimBank* bank, C3ANIMBank* animBank )
{
    C3Track    *cursor   = NULL;
    ANIMTrack  *tracks   = NULL;
    u32         tIndex   = 0; 
    C3HierNode *hierNode = NULL;

    // For all tracks
    tracks = animBank->animBank.animSequences->trackArray;
    cursor = (C3Track*)bank->trackList.Head;
    while( cursor )
    {
        // calculate anim time
        tracks[tIndex].animTime = cursor->endTime - cursor->startTime; 

        // set the keyframes
        tracks[tIndex].keyFrames = (ANIMKeyFrame*)(animBank->keyFrameOffset + animBank->keyFrameCursor * C3_ANIM_KEYFRAME_SIZE);
        tracks[tIndex].animType = C3ConvertKeyFrames( cursor, animBank );  
    
        // set total frames
        tracks[tIndex].totalFrames = cursor->numKeys;

        // set track ID
        hierNode = C3GetHierNodeFromIdentifier( cursor->owner );
        if( hierNode )
        {
            tracks[tIndex].trackID = hierNode->id;
        }
        else
        {
            // If we are pruning actor bones, this track should be pruned in C3RemoveUnusedTracks.
            C3ReportError( "Could not find hierarchy node %s to animate", cursor->owner );
            tracks[tIndex].trackID = 0xFFFF;
        }

        // set the param quatization type 
        tracks[tIndex].quantizeInfo = cursor->paramQuantizeInfo;

        // set the interpolation type
        tracks[tIndex].interpolationType = cursor->interpolationType;

        // set the options whether animation is relative to hierarchy or not
        tracks[tIndex].replaceHierarchyCtrl = cursor->replaceHierarchyCtrl;

        cursor = (C3Track*)cursor->link.Next; 
        tIndex++;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3GetTrackTime

  Description:  Calculate the length, in time, of the track

  Arguments:    track - track with a start and an end...

  Returns:      span of the track
/*---------------------------------------------------------------------------*/
static float
C3GetTrackTime( C3Track* track )
{
    C3KeyFrame* start;
    C3KeyFrame* end;

    start = (C3KeyFrame*)track->keyList.Head;
    end   = (C3KeyFrame*)track->keyList.Tail;

    return (end->time - start->time);
}


/*---------------------------------------------------------------------------*
  Name:         C3GetAnimTypeControl

  Description:  Returns the type of animation from a CTRLControl.

  Arguments:    control - control to evaluate

  Returns:      type of the track, bitwise OR combination of: 
                    ANIM_TRANS  ANIM_SCALE  ANIM_ROT  ANIM_QUAT  ANIM_MTX    
/*---------------------------------------------------------------------------*/
static u8
C3GetAnimTypeControl( CTRLControl* control )
{
    u8 type = 0x00;  

    C3_ASSERT( control );

    if( control->type & CTRL_MTX )
        return ANIM_MTX;

    if( control->type & CTRL_TRANS )
        type |= ANIM_TRANS;
        
    if( control->type & CTRL_ROT_QUAT )
        type |= ANIM_QUAT;
    else if( control->type & CTRL_ROT_EULER )
        type |= ANIM_ROT;

    if( control->type & CTRL_SCALE )
        type |= ANIM_SCALE;
  
    return type;
}


/*---------------------------------------------------------------------------*
  Name:         C3ConvertKeyFrames

  Description:  Converts the keyframes from a track to the runtime format
                Assumes the keyframe has been previously allocated.
                All keyframes in the track should have the same controlType, 
                otherwise, an error will be signaled.

  Arguments:    track    - to convert the keyframe from
                animBank - container of the destination keyframe array

  Returns:      animType - animType of keyframes
/*---------------------------------------------------------------------------*/
static u8 
C3ConvertKeyFrames( C3Track* track, C3ANIMBank* animBank )
{
    C3KeyFrame*   cursor    = NULL;
    C3KeyFrame*   current   = NULL;
    ANIMKeyFrame* keyFrame  = NULL;
    u32           prevIndex = -1;
    u8            animType;
    u8            interpTypeScale;
    u8            interpTypeRot;
    u8            interpTypeTrans;
    C3Bool        controlInterpScaleExist;
    C3Bool        controlInterpRotExist;
    C3Bool        controlInterpTransExist;

    // Get animType for first key frame
    animType = C3GetAnimTypeControl( &((C3KeyFrame*)track->keyList.Head)->control );

    // See if there are any interpolation settings for this track
    interpTypeScale = C3_INTERPTYPE_SCALE( track->interpolationType );
    controlInterpScaleExist = interpTypeScale == C3_INTERPTYPE_BEZIER ||
                              interpTypeScale == C3_INTERPTYPE_HERMITE;
    interpTypeRot   = C3_INTERPTYPE_ROT( track->interpolationType );
    controlInterpRotExist   = interpTypeRot   == C3_INTERPTYPE_SQUAD ||
                              interpTypeRot   == C3_INTERPTYPE_SQUADEE ||
                              interpTypeRot   == C3_INTERPTYPE_BEZIER ||
                              interpTypeRot   == C3_INTERPTYPE_HERMITE;
    interpTypeTrans = C3_INTERPTYPE_TRANS( track->interpolationType );
    controlInterpTransExist = interpTypeTrans == C3_INTERPTYPE_BEZIER ||
                              interpTypeTrans == C3_INTERPTYPE_HERMITE;

    // for each keyframe
    cursor = (C3KeyFrame*)track->keyList.Head;
    while( cursor )
    {
        current = cursor;
        cursor  = (C3KeyFrame*)cursor->link.Next;
        if( prevIndex == current->index )
            continue;

        prevIndex = current->index;

        // Make sure that all keyframes have same controlType
        if ( C3GetAnimTypeControl( &current->control ) != animType )
            C3ReportError( "Keyframes in track %d do not have same animType.", track->index );

        keyFrame = animBank->keyFrameArray + animBank->keyFrameCursor;

        // set the time
        keyFrame->time = current->time;

        // set the settings
        keyFrame->setting = (void*)(animBank->keyFrameSettingOffset + animBank->keyFrameSettingCursor);

        animBank->keyFrameSettingCursor += 
            C3WriteAnimControl( &current->control, 
                                (void*)((u8*)animBank->keyFrameSettings + animBank->keyFrameSettingCursor), 
                                track->paramQuantizeInfo );

        // set the interpolation setting offset (assume for now that they exist)
        if ( controlInterpScaleExist || controlInterpRotExist || controlInterpTransExist )
        {
            C3_ASSERT( animBank->keyFrameInterpSettings );

            keyFrame->interpolation = (void*)(animBank->keyFrameInterpSettingOffset + animBank->keyFrameInterpSettingCursor);

            if( controlInterpScaleExist )
            {
                animBank->keyFrameInterpSettingCursor +=
                    C3WriteControlInterp( &current->controlInterpScale,
                                          interpTypeScale,
                                          (void*)((u8*)animBank->keyFrameInterpSettings + animBank->keyFrameInterpSettingCursor),
                                          track->paramQuantizeInfo );
            }
            if( controlInterpRotExist ) 
            {
                animBank->keyFrameInterpSettingCursor +=
                    C3WriteControlInterp( &current->controlInterpRot,
                                          interpTypeRot,
                                          (void*)((u8*)animBank->keyFrameInterpSettings + animBank->keyFrameInterpSettingCursor),
                                          track->paramQuantizeInfo );
            }
            if( controlInterpTransExist )
            {
                animBank->keyFrameInterpSettingCursor +=
                    C3WriteControlInterp( &current->controlInterpTrans,
                                          interpTypeTrans,
                                          (void*)((u8*)animBank->keyFrameInterpSettings + animBank->keyFrameInterpSettingCursor),
                                          track->paramQuantizeInfo );
            }
        }
        else
            keyFrame->interpolation = 0;

        animBank->keyFrameCursor++;
    }

    return animType;
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteAnimBankInt

  Description:  Open a file with the name specified
                Calls function to write the header and the arrays

  Arguments:    animbank - animation bank to write
                path     - file name with path

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3WriteAnimBankInt( C3ANIMBank* animBank, char* path )
{
    FILE* outFile = NULL;


    // open file
    outFile = fopen( path, "wb" );
    C3_ASSERT( outFile );

    // write header
    C3WriteHeader( animBank, outFile );
  
    // write arrays
    C3WriteArrays( animBank, outFile );

    // close file
    fclose( outFile );
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteHeader

  Description:  Write the header information for the animation bank being 
                written

  Arguments:    animBank - anim bank to write to 
                outFile  - file to write to

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3WriteHeader( C3ANIMBank* animBank, FILE* outFile )
{
    u8  buffer[C3_ANIM_HEADER_SIZE];
    u32 offset  = C3_ANIM_HEADER_SIZE;
    u8  count   = 0;

    count += C3WriteU32( C3_ANM_VERSION, (void*)(buffer + count) );

    // sequence array offset
    count += C3WriteU32( offset, (void*)(buffer + count) );

    // bank id
    count += C3WriteU16( animBank->animBank.bankID, (void*)(buffer + count) );

    // total number of sequences
    count += C3WriteU16( animBank->animBank.numSequences, (void*)(buffer + count) );

    // total number of tracks
    count += C3WriteU16( animBank->animBank.numTracks, (void*)(buffer + count) );
 
    // total number of key frames
    count += C3WriteU16( animBank->animBank.numKeyFrames, (void*)(buffer + count) );

    // size of user defined data
    count += C3WriteU32( AnmUserDataSize, (void*)(buffer + count) );

    // offset to user defined data
    count += C3WriteU32( animBank->userDataOffset, (void*)(buffer + count) );

    // write to file
    fwrite( (void*)buffer, C3_ANIM_HEADER_SIZE, 1, outFile);
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteArrays

  Description:  Write the arrays in the animation bank

  Arguments:    animBank - anim bank to write to 
                outFile  - file to write to

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3WriteArrays( C3ANIMBank* animBank, FILE* outFile )
{
    // write sequence array
    C3WriteSequenceArray(  animBank, outFile );

    // write track array
    C3WriteTrackArray( animBank, outFile );

    // write the keyframe array
    C3WriteKeyFrameArray( animBank, outFile );  

    // write keyframe settings array padded 32 bits
    C3WritePadded32bit( (void*)animBank->keyFrameSettings, 
                        animBank->keyFrameSettingSize, outFile );

    // write keyframe interpolation settings array padded 32 bits
    if( animBank->keyFrameInterpSettings )
        C3WritePadded32bit( (void*)animBank->keyFrameInterpSettings, 
                            animBank->keyFrameInterpSettingSize, outFile );

    // write string bank padded 32 bits
    C3WritePadded32bit( (void*)animBank->stringBank, 
                        animBank->stringBankSize, outFile );

    if( animBank->userDataOffset > 0 )
    {
        // pad the file to a 32-byte boundary
        C3WritePadded32byte( 0, 0, outFile );

        // write user defined data padded 32 bits
        C3WritePadded32bit( AnmUserData, AnmUserDataSize, outFile );
    }
}



/*---------------------------------------------------------------------------*
  Name:         C3WriteSequenceArray

  Description:  Write the sequence array 

  Arguments:    animBank - anim bank to write to 
                outFile  - file to write to

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3WriteSequenceArray(  C3ANIMBank* animBank, FILE* outFile )
{
    ANIMSequence*   seq = NULL;
    u32             i   = 0;
    u32             temp;

    // replace the pointer by offset in the file
    // only works for one sequence....
    temp = (C3_ANIM_HEADER_SIZE + (C3_ANIM_SEQUENCE_SIZE * animBank->animBank.numSequences));

    seq = animBank->animBank.animSequences;
    for( i = 0; i < animBank->animBank.numSequences; i++ )
    {
        C3WriteU32F( (u32)seq[i].sequenceName, outFile );
        C3WriteU32F( temp, outFile );
        C3WriteU16F( seq[i].totalTracks, outFile );
        C3WriteU16F( 0, outFile );  // pad
    }
}
 

/*---------------------------------------------------------------------------*
  Name:         C3WriteTrackArray

  Description:  Write the track array

  Arguments:    animBank - anim bank to write to 
                outFile  - file to write to

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void 
C3WriteTrackArray( C3ANIMBank* animBank, FILE* outFile )
{
    ANIMTrack*  track   = NULL;
    u16         i       = 0;

    track = animBank->animBank.animSequences->trackArray;
    for( i = 0; i < animBank->animBank.numTracks; i++ )
    {
        C3WriteFloatF( track[i].animTime, outFile );
        C3WriteU32F( (u32)(track[i].keyFrames), outFile );
        C3WriteU16F( track[i].totalFrames, outFile );
        C3WriteU16F( track[i].trackID, outFile );
        fwrite( (void*)&track[i].quantizeInfo, 1, sizeof(u8), outFile );
        fwrite( (void*)&track[i].animType, 1, sizeof(u8), outFile );
        fwrite( (void*)&track[i].interpolationType, 1, sizeof(u8), outFile );
        fwrite( (void*)&track[i].replaceHierarchyCtrl, 1, sizeof(u8), outFile );
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteKeyFrameArray

  Description:  Write the keyframe array

  Arguments:    animBank - anim bank to write to 
                outFile  - file to write to

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3WriteKeyFrameArray( C3ANIMBank *animBank, FILE *outFile )
{
    ANIMKeyFrame    *kf = NULL;
    u16             i = 0;


    kf = animBank->keyFrameArray;
    for( i = 0; i < animBank->animBank.numKeyFrames; i++)
    {
        C3WriteFloatF( kf[i].time, outFile );
        C3WriteU32F( (u32)kf[i].setting, outFile );
        C3WriteU32F( (u32)kf[i].interpolation, outFile );
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteAnimControl

  Description:  Writes an animation control in the specified array

  Arguments:    control     - control to write
                array       - pointer to where to write the control to 
                quantInfo   - quantization information for components except
                              quaternions since they have a fixed
                              quantization type.

  Returns:      number of bytes written
/*---------------------------------------------------------------------------*/
static u8
C3WriteAnimControl( CTRLControl* control, void* array, u8 quantInfo )
{
    u8*     ptr         = NULL;
    u8      written     = 0;
    u8      i           = 0;
    u8      paramSize;

    C3_ASSERT( control );
    C3_ASSERT( array );

    // Writing out a control for animation
    ptr = (u8*)array;
    paramSize = C3GetQuantTypeSize( quantInfo );

    if( control->type & CTRL_MTX )
    {
        // write the matrix
        for( i = 0; i < 12; i++ )
        {
            written += C3WriteQuantizeFloat( ((f32*)(control->controlParams.mtx.m))[i], ptr + (i * paramSize), quantInfo );
        }
    }
    else
    {
        if( control->type & CTRL_SCALE )
        {
            // write the scale
            written += C3WriteQuantizeFloat( control->controlParams.srt.s.x, ptr, quantInfo );
            ptr += paramSize;
            written += C3WriteQuantizeFloat( control->controlParams.srt.s.y, ptr, quantInfo );
            ptr += paramSize;
            written += C3WriteQuantizeFloat( control->controlParams.srt.s.z, ptr, quantInfo );
            ptr += paramSize;
        }

        if( control->type & CTRL_ROT_QUAT )
        {
            // write the rotation in quaternion
            written += C3WriteQuantizeFloat( control->controlParams.srt.r.x, ptr, C3_QUATERNION_QUANTINFO );
            ptr += C3_QUATERNION_SIZE;
            written += C3WriteQuantizeFloat( control->controlParams.srt.r.y, ptr, C3_QUATERNION_QUANTINFO );
            ptr += C3_QUATERNION_SIZE;
            written += C3WriteQuantizeFloat( control->controlParams.srt.r.z, ptr, C3_QUATERNION_QUANTINFO );
            ptr += C3_QUATERNION_SIZE;
            written += C3WriteQuantizeFloat( control->controlParams.srt.r.w, ptr, C3_QUATERNION_QUANTINFO );
            ptr += C3_QUATERNION_SIZE;
        }
        else if( control->type & CTRL_ROT_EULER )
        {
            // write the rotation in euler XYZ
            written += C3WriteQuantizeFloat( control->controlParams.srt.r.x, ptr, quantInfo );
            ptr += paramSize;
            written += C3WriteQuantizeFloat( control->controlParams.srt.r.y, ptr, quantInfo );
            ptr += paramSize;
            written += C3WriteQuantizeFloat( control->controlParams.srt.r.z, ptr, quantInfo );
            ptr += paramSize;
        }

        if( control->type & CTRL_TRANS )
        {
            // write the translation
            written += C3WriteQuantizeFloat( control->controlParams.srt.t.x, ptr, quantInfo );
            ptr += paramSize;
            written += C3WriteQuantizeFloat( control->controlParams.srt.t.y, ptr, quantInfo );
            ptr += paramSize;
            written += C3WriteQuantizeFloat( control->controlParams.srt.t.z, ptr, quantInfo );
            ptr += paramSize;
        }
    } 

    return written;
}

 
/*---------------------------------------------------------------------------*
  Name:         C3WriteControlInterp

  Description:  Writes interpolation control in the specified array

  Arguments:    control     - control to write
                interpType  - type of interpolation
                array       - pointer to where to write the control to 
                quantInfo   - uses this to quantize in and out controls except for
                              quaternions since they have a fixed quantization type.

  Returns:      number of bytes written
/*---------------------------------------------------------------------------*/
static u8
C3WriteControlInterp( C3ControlInterp *controlInterp, u8 interpType, void* array, u8 quantInfo )
{
    u8* ptr              = (u8*)array;
    u8  easeQuantizeInfo = C3_EASE_QUANTINFO;
    u8  written          = 0;
    u8  paramSize;

    C3_ASSERT( controlInterp );
    C3_ASSERT( array );

    paramSize = C3GetQuantTypeSize( quantInfo );

    switch( interpType )
    {
    case C3_INTERPTYPE_BEZIER:
        written += C3WriteQuantizeFloat( controlInterp->inControl.x, ptr, quantInfo );
        ptr += paramSize;
        written += C3WriteQuantizeFloat( controlInterp->inControl.y, ptr, quantInfo );  
        ptr += paramSize;
        written += C3WriteQuantizeFloat( controlInterp->inControl.z, ptr, quantInfo );
        ptr += paramSize;
        written += C3WriteQuantizeFloat( controlInterp->outControl.x, ptr, quantInfo ); 
        ptr += paramSize;
        written += C3WriteQuantizeFloat( controlInterp->outControl.y, ptr, quantInfo ); 
        ptr += paramSize;
        written += C3WriteQuantizeFloat( controlInterp->outControl.z, ptr, quantInfo );
        ptr += paramSize;
        break;

    case C3_INTERPTYPE_HERMITE:
        written += C3WriteQuantizeFloat( controlInterp->inControl.x, ptr, quantInfo );
        ptr += paramSize;
        written += C3WriteQuantizeFloat( controlInterp->inControl.y, ptr, quantInfo );  
        ptr += paramSize;
        written += C3WriteQuantizeFloat( controlInterp->inControl.z, ptr, quantInfo );  
        ptr += paramSize;
        written += C3WriteQuantizeFloat( controlInterp->outControl.x, ptr, quantInfo ); 
        ptr += paramSize;
        written += C3WriteQuantizeFloat( controlInterp->outControl.y, ptr, quantInfo ); 
        ptr += paramSize;
        written += C3WriteQuantizeFloat( controlInterp->outControl.z, ptr, quantInfo );
        ptr += paramSize;
        written += C3WriteQuantizeFloat( controlInterp->inEase, ptr, easeQuantizeInfo );
        ptr += C3_EASE_SIZE;
        written += C3WriteQuantizeFloat( controlInterp->outEase, ptr, easeQuantizeInfo );
        ptr += C3_EASE_SIZE;
        break;

    case C3_INTERPTYPE_SQUAD:
        quantInfo = C3_QUATERNION_QUANTINFO;
        written += C3WriteQuantizeFloat( controlInterp->inControl.x, ptr, quantInfo );
        ptr += C3_QUATERNION_SIZE;
        written += C3WriteQuantizeFloat( controlInterp->inControl.y, ptr, quantInfo );  
        ptr += C3_QUATERNION_SIZE;
        written += C3WriteQuantizeFloat( controlInterp->inControl.z, ptr, quantInfo );
        ptr += C3_QUATERNION_SIZE;
        written += C3WriteQuantizeFloat( controlInterp->inControl.w, ptr, quantInfo );
        ptr += C3_QUATERNION_SIZE;
        written += C3WriteQuantizeFloat( controlInterp->outControl.x, ptr, quantInfo ); 
        ptr += C3_QUATERNION_SIZE;
        written += C3WriteQuantizeFloat( controlInterp->outControl.y, ptr, quantInfo ); 
        ptr += C3_QUATERNION_SIZE;
        written += C3WriteQuantizeFloat( controlInterp->outControl.z, ptr, quantInfo );
        ptr += C3_QUATERNION_SIZE;
        written += C3WriteQuantizeFloat( controlInterp->outControl.w, ptr, quantInfo );
        ptr += C3_QUATERNION_SIZE;
        break;

    case C3_INTERPTYPE_SQUADEE:
        quantInfo = C3_QUATERNION_QUANTINFO;
        written += C3WriteQuantizeFloat( controlInterp->inControl.x, ptr, quantInfo );  
        ptr += C3_QUATERNION_SIZE;
        written += C3WriteQuantizeFloat( controlInterp->inControl.y, ptr, quantInfo );  
        ptr += C3_QUATERNION_SIZE;
        written += C3WriteQuantizeFloat( controlInterp->inControl.z, ptr, quantInfo );
        ptr += C3_QUATERNION_SIZE;
        written += C3WriteQuantizeFloat( controlInterp->inControl.w, ptr, quantInfo );
        ptr += C3_QUATERNION_SIZE;
        written += C3WriteQuantizeFloat( controlInterp->outControl.x, ptr, quantInfo ); 
        ptr += C3_QUATERNION_SIZE;
        written += C3WriteQuantizeFloat( controlInterp->outControl.y, ptr, quantInfo ); 
        ptr += C3_QUATERNION_SIZE;
        written += C3WriteQuantizeFloat( controlInterp->outControl.z, ptr, quantInfo );
        ptr += C3_QUATERNION_SIZE;
        written += C3WriteQuantizeFloat( controlInterp->outControl.w, ptr, quantInfo );
        ptr += C3_QUATERNION_SIZE;
        written += C3WriteQuantizeFloat( controlInterp->inEase, ptr, easeQuantizeInfo );
        ptr += C3_EASE_SIZE;
        written += C3WriteQuantizeFloat( controlInterp->outEase, ptr, easeQuantizeInfo );
        ptr += C3_EASE_SIZE;
        break;
    }

    return written;
}


/*---------------------------------------------------------------------------*
  Name:         C3RemoveUnusedTracks

  Description:  After an actor has been pruned of unused bones with 
                C3ConvertActor, this function is used to remove the animation
                that was attached to those unused bones.

  Arguments:    animBank - animation bank

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3RemoveUnusedTracks( C3AnimBank *animBank )
{
    C3Sequence *seq;
    C3Track    *track, *next;
    C3SeqTrack *seqTrack;
    C3HierNode *hierNode;

    // Step through all tracks and make sure that bones in actor exists
    track = (C3Track*)animBank->trackList.Head;
    while( track )
    {
        next = (C3Track*)track->link.Next;

        // If bone is not used, it will be set to C3_HIERNODE_NOT_USED by C3RemoveUnusedBones
        hierNode = C3GetHierNodeFromIdentifier( track->owner );
        if( !hierNode || (hierNode && hierNode->id == C3_HIERNODE_NOT_USED) )
        {
            // Remove the track from all the sequences
            seq = (C3Sequence*)animBank->sequenceList.Head;
            while( seq )
            {
                seqTrack = (C3SeqTrack*)seq->trackList.Head;
                while( seqTrack )
                {
                    if( seqTrack->track == track )
                    {
                        DSRemoveListObject( &seq->trackList, (void*)seqTrack );
                        seq->numTracks--;
                    }

                    seqTrack = (C3SeqTrack*)seqTrack->link.Next;
                }

                seq = (C3Sequence*)seq->link.Next;
            }

            // Remove the track from the bank of tracks
            animBank->numKeyframes -= track->numKeys;
            animBank->numTracks--;
            DSRemoveListObject( &animBank->trackList, (void*)track );
        }

        track = next;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3FreeAnimBank

  Description:  Frees the memory allocated for the animation bank

  Arguments:    animBank - animation bank to free

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3FreeAnimBank( C3ANIMBank* animBank )
{
    C3_FREE( animBank->animBank.animSequences );
    C3_FREE( animBank->keyFrameArray );
    C3_FREE( animBank->keyFrameSettings );
    C3_FREE( animBank->keyFrameInterpSettings );
    C3_FREE( animBank->stringBank );
}
