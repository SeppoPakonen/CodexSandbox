/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3CnvOpz.c]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/c3/src/C3CnvOpz.c $
    
    22    11/13/00 1:36p John
    Added C3Transform* functions to transform geometry, hierarchy, and
    animation to compensate for new API: C3SetPivotOffset, where rotate and
    scale pivot points are not the same as the translation.
    
    21    7/18/00 7:26p John
    Modified to use new CTRL library.
    
    20    6/27/00 6:03p John
    Substituted quantize.h macros.
    
    19    4/06/00 3:26p John
    Added MTXRowCol to access matrices using this macro from MTX library.
    
    18    3/15/00 2:31p John
    Fixed color quantization bug (incorrect component size was calculated).
    
    17    3/01/00 5:35p John
    Due to initial hardware bug, MAX_FAN_VTX is now 4.
    
    16    2/29/00 7:20p John
    Removed C3Analysis.* code (unused and out of date).
    Removed unnecessary arguments from C3CompressData.
    
    15    2/15/00 4:22p John
    Moved out the quads to fans define to C3Defines.h
    
    14    2/04/00 6:06p John
    Untabified code.
    
    13    2/04/00 2:08p John
    Minor code change.
    
    12    2/01/00 6:51p John
    Added stats for position matrix cache performance.
    Removed static local variables since initialization can be skipped upon
    running from 3D Studio MAX after first time.
    
    11    1/31/00 4:15p John
    Added pruning of unnecessary bones and animations.
    Finished stitching with FIFO matrix cache and stripping.
    
    10    1/20/00 2:33p Mikepc
    fan length is limited to 18 vtx due to flipper bug. 
    #defined MAX_FAN_VTX and used it wherever fan length must be checked.
    
    9     1/20/00 1:14p John
    Added stitching (work in progress).
    Added C3 string table for efficient management of strings.
    
    8     1/04/00 1:12p John
    Added better support for second texture channel.
    
    7     12/20/99 11:21a John
    Added second texture channel.
    Fixed stripping bug when joining primitives w/ different textures.
    
    6     12/09/99 2:49p John
    
    5     12/08/99 7:25p John
    Does not compress colors when enabling strip/fan view inside C3
    library.
    
    4     12/02/99 3:10p John
    Minor code change.
    
    3     12/02/99 2:25p John
    Removed unused material and skinning code.
    Added conversion of quads to fans and vice versa.
    Added more efficient strip/fan code
    
    2     11/22/99 6:31p Ryan
    
    34    11/18/99 10:10a Ryan
    update to fix win32 warnings
    
    33    11/17/99 11:30a Mikepc
    fixed bug in random vtx color generation
    
    32    11/15/99 10:21a Mikepc
    assign random colors to fan & strip primitives.
    
    31    11/12/99 3:10p Mikepc
    
    30    11/12/99 3:08p Mikepc
    added code to C3ColorVerticesByPrim to shade primitives by number of
    triangles.
    
    29    11/12/99 1:56p John
    When exporting colors for strips and fans, now only colors triangles,
    quads, strips, and fans.
    
    28    11/11/99 1:14p John
    New Strip/Fan code. Added option to view strip/fan effectiveness.
    Fixed bug in C3CompareQuat and C3CompareVec.
    
    27    10/22/99 4:28p John
    Fixed quantization shift computation bug.
    
    26    10/21/99 6:22p John
    Mentions object identifier when creating quads and strips.
    
    25    10/20/99 11:05a John
    C3GeomObject.positionList is now processed through
    C3CompressIndexedList.
    
    24    10/01/99 11:14a John
    Can quantize color two ways (with and without alpha)
    
    23    9/29/99 4:28p John
    Changed header to make them all uniform with logs.
    Added position and normal quantization.
    Added external normal table.
    
    22    9/21/99 6:38p John
    Changed name to C3QuantizeFloat.
    Changed functionality and name to C3ComputeQuantizationShift.
    
    21    9/20/99 7:02p John
    Fixed minor bug in computing tex coord quantization shift.
    Renamed C3TextCoord to C3TexCoord for consistency.
    C3CompressIndices function prototype is no longer static.
    
    20    9/17/99 9:34a John
    Added C3Options and cleaned up code.
    
    19    9/14/99 6:48p John
    Added texture coordinate quantization.
    Compensated code in response to C3Stats and C3StatsObj.
    
    18    9/03/99 2:00p John
    Added quantization of color and indices.
    
    17    8/23/99 4:00p John
    
    16    8/19/99 3:46p John
    Added analysis and welding of texture coordinates.
    
    15    8/15/99 10:30p John
    Hashes positions by distance from origin.
    Added code that analyzes position data and welds them if within a
    certain threshold.  Data results are output to a file.

  $NoKeywords: $
 
 *---------------------------------------------------------------------------*/


#include "C3Private.h"

#define FLT_TABLE_SIZE      1024
#define COLOR_TABLE_SIZE    512

#define HASH_FLT( a )  (u16)( (a - c3MinFlt) *  c3HashFltFactor  )


//******************************

// Jan. 20/00
// fan length restriction to due bug
// in Flipper.
// max. length is 4 vtx / 2 tri

// TODO: When increasing MAX_FAN_VTX, there are some unnecessary 
// if statements in C3AddTri2Quad and C3TwoQuads2Fan
#define MAX_FAN_VTX   4

//******************************


// Local types
typedef struct
{ 
    DSLink  link;
    u32     index;
    DSList  list;

} C3List, *C3ListPtr;


// Local functions
static void     C3TransformBoneToPivot( C3HierNode* bone );
static void     C3TransformObjectToPivot( char *geomObjName, Vec *pivotOffset, char *boneName );
static void     C3TransformTrackToPivot( C3Track *track );

static void     C3CompressObjData( C3GeomObject* obj );
static void     C3RemoveObjNullData( C3GeomObject* obj );
static C3Bool   C3IsNullTriangle( C3Primitive* prim );
static void     C3CompressPositions( C3GeomObject* obj );
static void     C3CompressTexCoords( C3GeomObject* obj, u8 channel );
static void     C3CompressColor( C3GeomObject* obj );
static void     C3CompressNormal( C3GeomObject* obj );
static void     C3CompressAnimation( void );
static u16      C3CompressAnimTrack( C3Track* track );
static void     C3HashList( DSHashTable* hashTable, DSList* list );
static void     C3InitHashFloat( float min, float max, u16 size );
static void     C3CompressIndexedList( DSList* list );
static u16      HashPosition( void* position );
static u16      HashTexCoord( void* st );
static u16      HashColor( void* color );
static u16      HashNormal( void* normal );
static u16      HashKeyFrame( void* kf );
static C3Bool   C3CompareTexCoord ( void* p1,   void* p2 );
static C3Bool   C3ComparePosition ( void* p1,   void* p2 );
static C3Bool   C3CompareColor    ( void* c1,   void* c2 );
static C3Bool   C3CompareNormal   ( void* n1,   void* n2 );
static C3Bool   C3CompareAnimKey  ( void* kf1,  void* kf2 );
static C3Bool   C3CompareVec      ( void* a,    void* b );
static C3Bool   C3CompareQuat     ( void* a,    void* b );
static C3Bool   C3CompareMatrix   ( void* a,    void* b );
static C3Bool   C3CompareControl  ( void* c1,   void* c2 );
static C3Bool   C3ComparePrimAttr ( C3Primitive* p1, C3Primitive* p2 );
static C3Bool   C3CompareVertex   ( C3Vertex* v1, C3Vertex* v2 );
static void     C3CopyPosition    ( void* dest, void* src );
static void     C3CopyTexCoord    ( void* dest, void* src );
static void     C3CopyColor       ( void* dest, void* src );

static C3Bool   C3CreateQuad( C3Primitive* pA, C3Primitive* pB );

static void     C3QuadifyGeomObject( C3GeomObject* geomObj );
static void     C3QuadToStrip( C3Primitive* quad );
static C3Bool   C3AddQuadToStrip( C3Primitive* strip, C3Primitive* quad );

// Local variables
static float    c3MinFlt;
static float    c3HashFltFactor;
static C3Bool   c3CurrentlyWelding;

// Local functions for Strip, Fan, Quad creation
static void   C3CreateTriStripsGeomObject(  C3GeomObject* geomObj );
static C3Bool C3AddStripToStrip( C3Primitive* stripA, C3Primitive* stripB );
static C3Bool C3AddTriToStrip(   C3Primitive* strip,  C3Primitive* tri    );

static void   C3ShortGeoStripsToQuads( C3GeomObject* pGeo );
static void   C3StripToQuad( C3GeomObject* pGeo, C3Primitive* pStrip );

static void   C3FansFromGeoQuadPairs( C3GeomObject* pGeo, C3Primitive* fanSubList );
static C3Bool C3TwoQuads2Fan(   C3GeomObject* pGeo, C3Primitive* pQuadA, C3Primitive* pQuadB );

static void   C3FansFromGeoQuads( C3GeomObject* pGeo );
static void   C3JoinGeoFans( C3GeomObject* pGeo );
static void   C3MakeFans(    C3GeomObject* pGeo );

static C3Bool C3AddFan2Fan(     C3GeomObject* pGeo, C3Primitive* pFanA, C3Primitive* pFanB );
static C3Bool C3AddTri2Fan(     C3GeomObject* pGeo, C3Primitive* pFanA, C3Primitive* pFanB );
static C3Bool C3FanFromTriPair( C3GeomObject* pGeo, C3Primitive* pTriA, C3Primitive* pTriB );
static C3Bool C3AddQuad2Fan(    C3GeomObject* pGeo, C3Primitive* pA,    C3Primitive* pB );
static C3Bool C3AddTri2Quad(    C3GeomObject* pGeo, C3Primitive* pA,    C3Primitive* pB );

static void   C3ShortGeoFansToQuads( C3GeomObject* pGeo );

static void   C3ColorVerticesByPrim( C3GeomObject* pGeo );


/*---------------------------------------------------------------------------*
  Name:         C3InitConversionOptimization

  Description:  Initialize the conversion optimization options to their 
                default settings.

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3InitConversionOptimization ( void )
{
    c3CurrentlyWelding  = C3_FALSE;
}


/*---------------------------------------------------------------------------*
  Name:         C3CleanConversionOptimization

  Description:  Clean up any memory allocated in this module

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3CleanConversionOptimization( void )
{
}


/*---------------------------------------------------------------------------*
  Name:         C3TransformData

  Description:  Transforms track information by computing animation relative
                to the hierarchy, and computes in and out controls.
                If pivot offsets are not at the local transformation center,
                then, transforms vertex positions, hierarchy matrices, and
                animation to compensate for pivot point without changing the 
                scene.  This function must be called after data extraction,
                but before C3CompressData since this function may still 
                transform data.

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3TransformData()
{
    C3Actor      *actor;
    C3AnimBank   *animBank;
    C3HierNode   *bone;
    C3Track      *trackCursor;
    C3GeomObject *obj;
    C3Position   *pos;
    f32           distance;

    // Transform hierarchy matrices to center of pivot point
    actor = C3GetActor();
    bone = (C3HierNode*)actor->hierarchy.Root;
    while( bone )
    {
        C3TransformBoneToPivot( bone );

        bone = (C3HierNode*)bone->branch.Next;
    }

    // Transform vertex positions to compensate for hierarchy matrix translation
    // effectively moving hierarchy transformation without moving geometry.
    bone = (C3HierNode*)actor->boneList.Head;
    while( bone )
    {
        if( bone->geomObjName )
        {
            C3TransformObjectToPivot( bone->geomObjName, &bone->pivotOffset, bone->identifier );
        }

        bone = (C3HierNode*)bone->link.Next;
    }

    // Transform animation data to make it relative to hierarchy
    // (because Character Pipeline runtime libraries concatenates hierarchy
    //  orientation matrix with animation matrix).
    // Also, compute in and out controls for certain types of keyframe interpolation.
    animBank = (C3AnimBank*)actor->animBank;
    trackCursor = (C3Track*)((C3AnimBank*)actor->animBank)->trackList.Head;
    while( trackCursor )
    {
        // Put the keyframes in time order
        if( trackCursor->sortKeyFramesNeeded )
            C3SortKeyFrames( trackCursor );

        // Transform the animation track to pivot point, just like hierarchy
        C3TransformTrackToPivot( trackCursor );

        // Apply the transform to all keys in the track
        C3ComputeTrackWithTransform( trackCursor );

        // If exporting keyframe, compute tangents and control points as necessary
        if( C3_INTERPTYPE_ROT( trackCursor->interpolationType ) == C3_INTERPTYPE_SQUAD ||
            C3_INTERPTYPE_ROT( trackCursor->interpolationType ) == C3_INTERPTYPE_SQUADEE )
        {
            C3ComputeTrackInOutControl( CTRL_ROT_QUAT, C3_INTERPTYPE_ROT( trackCursor->interpolationType ), trackCursor );
        }

        if( C3_INTERPTYPE_TRANS( trackCursor->interpolationType ) == C3_INTERPTYPE_HERMITE )
        {
            C3ComputeTrackInOutControl( CTRL_TRANS, C3_INTERPTYPE_HERMITE, trackCursor );
        }

        if( C3_INTERPTYPE_SCALE( trackCursor->interpolationType ) == C3_INTERPTYPE_HERMITE )
        {
            C3ComputeTrackInOutControl( CTRL_SCALE, C3_INTERPTYPE_HERMITE, trackCursor );
        }

        trackCursor = (C3Track*)trackCursor->link.Next;
    }

    obj = C3GetNextObject( NULL );
    while ( obj )
    {
        // Find min and max for the positions in this object
        pos = (C3Position*)obj->positionList.Head;
        while( pos )
        {
            if( pos->x < obj->minPosition.x )
                obj->minPosition.x = pos->x;
            if( pos->y < obj->minPosition.y )
                obj->minPosition.y = pos->y;
            if( pos->z < obj->minPosition.z )
                obj->minPosition.z = pos->z;
            if( pos->x > obj->maxPosition.x )
                obj->maxPosition.x = pos->x;
            if( pos->y > obj->maxPosition.y )
                obj->maxPosition.y = pos->y;
            if( pos->z > obj->maxPosition.z )
                obj->maxPosition.z = pos->z;

            // Set maximum and minimum distances
            distance = (f32)sqrt(pow(pos->x, 2) + pow(pos->y, 2) + pow(pos->z, 2));
            if( distance > obj->maxPosDistance ) 
                obj->maxPosDistance = distance;
            if( distance < obj->minPosDistance )
                obj->minPosDistance = distance;

            pos = (C3Position*)pos->link.Next;
        }

        obj = C3GetNextObject( obj );
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3TransformBoneToPivot

  Description:  If the pivot offset for the given bone is not the same as
                the local transformation center (i.e. not 0,0,0),
                then translate the hierarchy matrix center so that it is 
                at the pivot offset.  This is a recursive function called 
                down through the actor hierarchy, so initial call should
                pass in a root node of the actor hierarchy.

  Arguments:    bone - bone and children to transform

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3TransformBoneToPivot( C3HierNode *bone )
{
    C3HierNode *parent, *child;
    Vec parentPivot = { 0.0f, 0.0f, 0.0f };
    Vec bonePivotFromParent;
    Mtx boneMtx;

    parent = (C3HierNode*)bone->branch.Parent;
    if( parent )
    {
        parentPivot = parent->pivotOffset;
    }

    if( bone->control )
    {
        CTRLBuildMatrix( &bone->control->c, boneMtx );
        MTXMultVecSR( boneMtx, &bone->pivotOffset, &bonePivotFromParent );
        bone->control->c.controlParams.srt.t.x += bonePivotFromParent.x - parentPivot.x;
        bone->control->c.controlParams.srt.t.y += bonePivotFromParent.y - parentPivot.y;
        bone->control->c.controlParams.srt.t.z += bonePivotFromParent.z - parentPivot.z;
    }

    child = (C3HierNode*)bone->branch.Children;
    while( child )
    {
        C3TransformBoneToPivot( child );

        child = (C3HierNode*)child->branch.Next;
    }
}


static void
C3TransformTrackToPivot( C3Track *track )
{
    C3HierNode *bone, *parent;
    C3KeyFrame *key;
    Vec parentPivot = { 0.0f, 0.0f, 0.0f };
    Vec bonePivotFromParent;
    Vec offset;
    Mtx boneMtx;

    bone = C3GetHierNodeFromIdentifier( track->owner );
    if( !bone )
        return;

    parent = (C3HierNode*)bone->branch.Parent;
    if( parent )
    {
        parentPivot = parent->pivotOffset;
    }

    if( bone->control )
    {
        CTRLBuildMatrix( &bone->control->c, boneMtx );
        MTXMultVecSR( boneMtx, &bone->pivotOffset, &bonePivotFromParent );
        VECSubtract( &bonePivotFromParent, &parentPivot, &offset );

        key = (C3KeyFrame*)track->keyList.Head;
        while( key )
        {
            if( key->control.type & CTRL_TRANS )
            {
                VECAdd( &key->control.controlParams.srt.t, &offset, &key->control.controlParams.srt.t );
            }

            key = (C3KeyFrame*)key->link.Next;
        }
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3TransformObjectToPivot

  Description:  Given a geometry object, it will untranslate the geometry
                positions so that the object does not "move", when the 
                hierarchy matrix translation has been changed through
                C3TransformBoneToPivot.  These two functions effectively
                move the hierarchy transformation node without moving the
                geometry.  One caveat to this is when more than one bone
                reference a geometry object (instancing).  All bones that 
                instance the geometry object must have the same pivot offset
                or the data will not be consistent.  This function errors
                checks this condition.

  Arguments:    geomObjName - name of the geometry object
                pivotOffset - bone's pivot offset (amount to untranslate positions)
                boneName    - name of the bone

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3TransformObjectToPivot( char *geomObjName, Vec *pivotOffset, char *boneName )
{
    C3GeomObject *obj;
    C3Position   *cursor;
    C3HierNode   *otherBone;

    obj = C3GetObjectFromIdentifier( geomObjName );
    if( !obj )
        return;

    if( !obj->pivotTransformedBy )
        obj->pivotTransformedBy = boneName;
    else
    {
        otherBone = C3GetHierNodeFromIdentifier( obj->pivotTransformedBy );
        C3_ASSERT( otherBone );

        // Check to make sure pivot are the same
        // since this geometry object is instanced by these bones.  Since
        // we are modifying vertex positions, need to make sure pivots are the same.
        if( !C3_FLOAT_EQUAL(pivotOffset->x, otherBone->pivotOffset.x) ||
            !C3_FLOAT_EQUAL(pivotOffset->y, otherBone->pivotOffset.y) ||
            !C3_FLOAT_EQUAL(pivotOffset->z, otherBone->pivotOffset.z) )
        {
            C3ReportError( "Transform node %s and %s instance %s, but pivots are not the same", boneName, obj->pivotTransformedBy, geomObjName );
            return;
        }
    }

    if( pivotOffset->x == 0.0f && pivotOffset->y == 0.0f && pivotOffset->z == 0.0f )
        return;

    cursor = (C3Position*)(obj->positionList.Head);
    while(cursor)
    {
        cursor->x -= pivotOffset->x;
        cursor->y -= pivotOffset->y;
        cursor->z -= pivotOffset->z;

        cursor = (C3Position*)(cursor->link.Next);
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3ComputeQuantizationShift

  Description:  Intelligently selects the optimal number of shift bits based
                on the type of quantization by looking at the range of
                min and max. 

  Arguments:    quantInfo - top 4 MSBs represent the type of quantization

  Returns:      the full quantization info with type and computed shift.
                
/*---------------------------------------------------------------------------*/
u8
C3ComputeQuantizationShift( u8 quantInfo, f32 min, f32 max )
{
    u8 type = C3_QUANT_TYPE(quantInfo); 
    u8 maxBits;
    u8 shift;
    f32 farthestNumber;
    u8 i;


    // If quantization type is a float, then we don't need to worry
    if ( type == GX_F32 )
        return quantInfo & 0xF0;

    // Figure out how many whole number bits are needed to represent max and min
    farthestNumber = (f32)fabs(max);
    if ( (f32)fabs(min) > farthestNumber )
        farthestNumber = (f32)fabs(min);

    switch ( type )
    {
    case GX_S16:
        maxBits = 15;  // including sign bit
        break;
    case GX_U16:
        maxBits = 16;
        break;
    case GX_S8:
        maxBits = 7;
        break;
    case GX_U8:
        maxBits = 8;
        break;
    default:
        C3_ASSERT( C3_FALSE );
        break;
    }

    // Always leave one bit for the whole number part
    for ( i = 1; i < maxBits; i++ )
    {
        if ( farthestNumber < pow(2, i) )
        {
            break;
        }
    }
    shift = maxBits - i;

    return ((quantInfo & 0xF0) | shift);
}


/*---------------------------------------------------------------------------*
  Name:         C3ComputePositionQuantization

  Description:  Computes the scale and translation factors for the position
                based on the min and max positions

  Arguments:    quantInfo - top 4 MSBs represent the type of quantization
                minPos - minimum position
                maxPos - maximum position
                scale  - place to store the scale factors
                translate - place to store the translate factors

                For quantization Type, we don't care about signed or unsigned.
                We only want to know whether 8, 16, or 32 bits.

  Returns:      NONE
                
/*---------------------------------------------------------------------------*/
void
C3ComputePositionQuantization( u8 quantInfo, C3Position *minPos, C3Position *maxPos,
                               C3Position *scale,  C3Position *translate )
{
    f32 maxPossibleNumber = 0;

    switch( C3_QUANT_TYPE(quantInfo) )
    {
    case GX_F32:
        // For now, we'll write positions out as floats, so no need
        // to translate or scale.  We may want to though in the future
        // so we can have arbitrary origin and better precision.
        translate->x = 0;
        translate->y = 0;
        translate->z = 0;
        scale->x = 1;
        scale->y = 1;
        scale->z = 1;
        return;

    case GX_U16:
    case GX_S16:
        maxPossibleNumber = 65535;
        break;

    case GX_U8:
    case GX_S8:
        maxPossibleNumber = 255;
        break;

    default:
        C3_ASSERT( C3_FALSE );
    }

    translate->x = minPos->x;
    translate->y = minPos->y;
    translate->z = minPos->z;

    scale->x = (f32)(maxPos->x - translate->x) / maxPossibleNumber;
    scale->y = (f32)(maxPos->y - translate->y) / maxPossibleNumber;
    scale->z = (f32)(maxPos->z - translate->z) / maxPossibleNumber;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetComponentSize

  Description:  Returns the number of bytes for a component of the specified 
                target depending on what we will be quantizing the target to.

  Arguments:    target - target for which to retrieve the component size

  Returns:      number of bytes for one component ( Color are defined as one
                component for RGB or RGBA )
/*---------------------------------------------------------------------------*/
u8
C3GetComponentSize( u32 target, u8 channel )
{
    u8  quant = 0;

    quant = C3GetOptionQuantization( target, channel );
    
    return C3GetComponentSizeFromQuantInfo( target, quant );
}


/*---------------------------------------------------------------------------*
  Name:         C3GetComponentSizeFromQuantInfo

  Description:  Returns the number of bytes for a component of the specified 
                target

  Arguments:    target - target type since different for color and the rest
                quantInfo - quantization info with type and shift

  Returns:      number of bytes for one component ( Color are defined as one
                component for RGB or RGBA )
/*---------------------------------------------------------------------------*/
u8
C3GetComponentSizeFromQuantInfo( u32 target, u8 quantInfo )
{
    u8  type  = 0;
    u8  size  = 0;

    type = C3_QUANT_TYPE( quantInfo );

    if( target & C3_TARGET_COLOR || target & C3_TARGET_COLORALPHA )
    {
        switch( type )
        {
            case GX_RGB565:
            case GX_RGBA4:
                size = 2;
                break;

            case GX_RGBA6:
            case GX_RGB8:
                size = 3;
                break;
                        
            case GX_RGBX8:
            case GX_RGBA8:
                size = 4;
                break;

            default:
                C3_ASSERT( C3_FALSE );
                break;
        }
    }
    else
    {
        switch( type )
        {
            case GX_U8:
            case GX_S8:
                size = sizeof( u8 );
                break;

            case GX_U16:
            case GX_S16:
                size = sizeof( u16 );
                break;

            case GX_F32:
                size = sizeof( f32 );
                break;

            default:
                C3_ASSERT( C3_FALSE );
                break;
        }
    }
    return size;
}


/*---------------------------------------------------------------------------*
  Name:         C3QuantizeFloat

  Description:  Converts a float to the specified type

  Arguments:    dest     - destination of the converted float
                destCode - quantization code ( type and shift )
                src      - float to convert

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3QuantizeFloat( void* dest, u8 destCode, f32 src)
{
    u8  quant = 0;
    u8  shift = 0;

    C3_ASSERT( dest );

    quant = C3_QUANT_TYPE(destCode);
    shift = C3_QUANT_SHIFT(destCode);

    switch( quant )
    {
        case GX_F32:
            *((f32*)dest) = src;
            break;
        case GX_U8:
            *((u8*)dest) = (u8)(src * (1 << shift));
            break;
        case GX_S8:
            *((s8*)dest) = (s8)(src * (1 << shift));
            break;
        case GX_U16:
            *((u16*)dest) = (u16)(src * (1 << shift));
            break;
        case GX_S16:
            *((s16*)dest) = (s16)(src * (1 << shift));
            break;
        default:
            C3_ASSERT( C3_FALSE );
            break;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3QuantizeFloatScaleTrans

  Description:  Converts a float to the specified type using the scale and translation

  Arguments:    dest     - destination of the converted float
                destCode - quantization code ( type only - top 4 MSBs )
                src      - float to convert
                scale    - scale to use
                translate - translate to use

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3QuantizeFloatScaleTrans( void* dest, u8 destCode, f32 src, f32 scale, f32 translate )
{
    u8  quant = 0;
    u8  shift = 0;

    C3_ASSERT( dest );

    quant = C3_QUANT_TYPE(destCode);
    shift = C3_QUANT_SHIFT(destCode);

    switch( quant )
    {
        case GX_F32:
            *((f32*)dest) = src;
            break;
        case GX_U8:
        case GX_S8:
            *((u8*)dest) = (u8)( (f32)(src - translate) / scale );
            break;
        case GX_U16:
        case GX_S16:
            *((u16*)dest) = (u16)( (f32)(src - translate) / scale );
            break;
        default:
            C3_ASSERT( C3_FALSE );
            break;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3FloatFromQuant

  Description:  Converts some src, described by the quantization info in srcCode
                and returns its float value.

  Arguments:    src      - source of the quantized number
                srcCode  - quantization code ( type and shift )

  Returns:      Resulting float
/*---------------------------------------------------------------------------*/
f32
C3FloatFromQuant( void* src, u8 srcCode )
{
    u8  quant = 0;
    u8  shift = 0;

    C3_ASSERT( src );

    quant = C3_QUANT_TYPE(srcCode);
    shift = C3_QUANT_SHIFT(srcCode);

    switch( quant )
    {
        case GX_F32:
            return *((f32*)src);
        case GX_U8:
            return ((f32)*((u8*)src) / (1 << shift));
        case GX_S8:
            return ((f32)*((s8*)src) / (1 << shift));
        case GX_U16:
            return ((f32)*((u16*)src) / (1 << shift));
        case GX_S16:
            return ((f32)*((s16*)src) / (1 << shift));
        default:
            C3_ASSERT( C3_FALSE );
            return 0.0F;
            break;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3QuantizeColor

  Description:  Quantitize the color according to the destination 
                    quantization code.

  Arguments:    dest     - destination of the converted color
                destCode - quantization code ( type and shift )
                src      - color to convert

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3QuantizeColor( void* dest, u8 destCode, C3Color* src)
{
    u8  quant = 0;
    u8  shift = 0;


    C3_ASSERT( dest );
    C3_ASSERT( src );

    quant = C3_QUANT_TYPE(destCode);
    shift = C3_QUANT_SHIFT(destCode);

    switch( quant )
    {
        case GX_RGB565:
            *((u16*)dest) = (((u16)(src->r >> 3)) << 11) | (((u16)(src->g >> 2)) << 5) | 
                            ((u16)(src->b >> 3));
            break;

        case GX_RGBA4:
            *((u16*)dest) = (((u16)(src->r >> 4)) << 12) | (((u16)(src->g >> 4)) << 8) |
                            (((u16)(src->b >> 4)) << 4)  | ((u16)(src->a >> 4));
            break;

        case GX_RGBA8:
            *((u32*)dest) = (((u32)src->r) << 24) | (((u32)src->g) << 16) | 
                            (((u32)src->b) << 8) | (u32)src->a;
            break;
  
        case GX_RGB8:
            *((u8*)dest) = src->r;
            *((u8*)dest + 1) = src->g;
            *((u8*)dest + 2) = src->b;
            break;

        case GX_RGBX8:
            *((u32*)dest) = (((u32)src->r) << 24) | (((u32)src->g) << 16) | 
                            (((u32)src->b) << 8);
            break;

        case GX_RGBA6:
            *((u8*)dest)     =  (src->r & 0xFC)       | (src->g >> 6); // top 6 of r and top 2 of g
            *((u8*)dest + 1) = ((src->g & 0x3C) << 2) | (src->b >> 4); // next 4 of g and top 4 of b
            *((u8*)dest + 2) = ((src->b & 0x0C) << 4) | (src->a >> 2); // next 2 of b and top 6 of a
            break;

        default:
            C3_ASSERT( C3_FALSE );
            break;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3ColorFromQuant

  Description:  Quantitize the color according to the destination 
                quantization code

  Arguments:    src     - the source buffer
                srcCode - the quantization
                r       - memory to set red
                g       - memory to set green  
                b       - memory to set blue
                a       - memory to set alpha

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3ColorFromQuant( void* src, u8 srcCode, u8* r, u8* g, u8* b, u8* a )
{
    u8  quant = 0;
    u8  shift = 0;


    C3_ASSERT( src && r && g && b && a );

    *r = *g = *b = *a = 0;

    quant = C3_QUANT_TYPE(srcCode);
    shift = C3_QUANT_SHIFT(srcCode);

    switch( quant )
    {
        case GX_RGB565:
            *r = (( *(u16*)src & 0xF800 ) >> 11) << 3;
            *g = (( *(u16*)src & 0x07E0 ) >> 5 ) << 2;
            *b =  ( *(u16*)src & 0x001F ) << 3;
            break;

        case GX_RGBA4:
            *r = (( *(u16*)src & 0xF000 ) >> 12) << 4;
            *g = (( *(u16*)src & 0x0F00 ) >> 8 ) << 4;
            *b = (( *(u16*)src & 0x00F0 ) >> 4 ) << 4;
            *a =  ( *(u16*)src & 0x000F ) << 4;
            break;

        case GX_RGBA8:
            *r = (u8)(( *(u32*)src & 0xFF000000 ) >> 24);
            *g = (u8)(( *(u32*)src & 0x00FF0000 ) >> 16);
            *b = (u8)(( *(u32*)src & 0x0000FF00 ) >> 8);
            *a = (u8)(( *(u32*)src & 0x000000FF ));
            break;
  
        case GX_RGB8:
        case GX_RGBX8:
            *r = (u8)(( *(u32*)src & 0xFF000000 ) >> 24);
            *g = (u8)(( *(u32*)src & 0x00FF0000 ) >> 16);
            *b = (u8)(( *(u32*)src & 0x0000FF00 ) >> 8);
            break;
                    
        case GX_RGBA6:
            *r = (( *(u16*)src & 0xFC0000 ) >> 18) << 2;
            *g = (( *(u16*)src & 0x03F000 ) >> 12) << 2;
            *b = (( *(u16*)src & 0x000FC0 ) >> 6 ) << 2;
            *a =  ( *(u16*)src & 0x00003F ) << 2;
            break;

        default:
            C3_ASSERT( C3_FALSE );
            break;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3CompressData

  Description:  Runs through the database of objects and makes the duplicate
                data unique (arrays).  Also does welding if weld radius is greater
                than epsilon.  If any null primitives are left over, they are
                removed.

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3CompressData( )
{
    C3GeomObject* object = NULL;

    C3ReportStatus("Compressing data...");

    // Traverse the whole database of objects
    object = C3GetNextObject( object );
    while( object )
    {
        // For each object
        C3CompressObjData( object );
        C3RemoveObjNullData( object );

        object = C3GetNextObject( object );
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3InitHashFloat

  Description:  Initialize the float hash parameters used in the hashing 
                function for floats

  Arguments:    min  - minimum float in the database to convert
                max  - maximum float in the database to convert
                size - size of the hash table

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void 
C3InitHashFloat( float min, float max, u16 size )
{
    C3_ASSERT( min <= max );

    c3MinFlt = min;
    max += 1.0f;    // In case max == min, so we don't divide by 0
    c3HashFltFactor = ((f32)(size - 1) / (max - min));  
}


/*---------------------------------------------------------------------------*
  Name:         C3RemoveObjNullData

  Description:  Removes primitives for which triangles and lines have
                zero area and zero length

  Arguments:    obj - object to remove null data for

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3RemoveObjNullData( C3GeomObject* obj )
{
    C3Primitive* prim;
    C3Primitive* nextPrim;

    prim = (C3Primitive*)(obj->primitiveList.Head); 
    while ( prim )
    {
        // See if primitive has any duplicate positions (only for triangles)
        nextPrim = (C3Primitive*)prim->link.Next;

        // TBD: When we want to better support C3 library for other
        // software, we should also eliminate null quads, lines, ...

        if ( prim->type == C3_PRIM_TRI &&
             C3IsNullTriangle( prim ) ) 
        {
            // Remove this primitive
            DSRemoveListObject( &obj->primitiveList, (void*)prim );
            obj->stats->numPrimitives--;
            obj->stats->numTri--;
            obj->stats->numNullPrimitives++;
        }

        prim = nextPrim;
    }
}

/*---------------------------------------------------------------------------*
  Name:         C3IsNullTriangle

  Description:  Checks to see if a triangle has zero area

  Arguments:    prim - the triangle primitive to test

  Returns:      True if triangle does have zero area.  False if otherwise.
/*---------------------------------------------------------------------------*/
static C3Bool
C3IsNullTriangle( C3Primitive* prim )
{
    C3PtrLink*  ptrLink;
    C3Vertex*   v1 = NULL;
    C3Vertex*   v2 = NULL;
    C3Vertex*   v3 = NULL;

    C3_ASSERT( prim->type == C3_PRIM_TRI );

    ptrLink = (C3PtrLink*)prim->vertexPtrList.Head;
    v1 = (C3Vertex*)ptrLink->ptr;
    ptrLink = (C3PtrLink*)ptrLink->link.Next;
    v2 = (C3Vertex*)ptrLink->ptr;
    ptrLink = (C3PtrLink*)ptrLink->link.Next;   
    v3 = (C3Vertex*)ptrLink->ptr;

    C3_ASSERT( ptrLink->link.Next == NULL );  // Make sure only 3 vertices to triangle

    return ( C3ComparePosition( (void*)v1->position, (void*)v2->position ) ||
             C3ComparePosition( (void*)v2->position, (void*)v3->position ) ||
             C3ComparePosition( (void*)v1->position, (void*)v3->position ) );            
}

/*---------------------------------------------------------------------------*
  Name:         C3CompressObjData

  Description:  Makes the duplicate data unique for the object specified.
                Also welds and removes null primitives.

  Arguments:    obj - object to reduce the duplicate data for

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3CompressObjData( C3GeomObject* obj )
{
    u8 i = 0;
    u16 compressFlags;

    if( !obj )
        return;

    compressFlags = C3GetOptionCompress();

    // Make position list unique
    if( compressFlags & C3_TARGET_POSITION || 
        C3GetOptionWeldRadius( C3_TARGET_POSITION ) > C3_FLT_EPSILON )  
        C3CompressPositions( obj );

    // Make color list unique
    if( compressFlags & C3_TARGET_COLOR && !C3GetOptionEnableStripFanView() )
        C3CompressColor( obj );

    // Make texture coord lists unique
    if( compressFlags & C3_TARGET_TEXCOORD ||
        C3GetOptionWeldRadius( C3_TARGET_TEXCOORD ) > C3_FLT_EPSILON )
    {
        for( i = 0; i < C3_MAX_TEXTURES; i++ )
            C3CompressTexCoords( obj, i );
    }

    if ( compressFlags & C3_TARGET_NORMAL )
        C3CompressNormal( obj );
}

/*---------------------------------------------------------------------------*
  Name:         C3CompressPositions

  Description:  Compress  the position linked list assigning a unique index 
                for duplicate data

  Arguments:    obj - object to reduce the duplicate data for 
                  
  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3CompressPositions( C3GeomObject* obj )
{
    DSHashTable hTable;
    DSList      listArray[ FLT_TABLE_SIZE ];
    C3Position  position;
    u16 unique    = 0;
    u16 i         = 0;
    u32 count     = 0;
    u16 numUnique = 0;

    if( obj->stats->numPositions <= 0 )
        return;

    // Initialize the hash table
    DSInitHTable( &hTable, 
                  FLT_TABLE_SIZE, 
                  listArray,
                  HashPosition, 
                  (Ptr)&position, 
                  &(position.link) );

    // Initialize the hashing parameters
    C3InitHashFloat( obj->minPosDistance, obj->maxPosDistance, FLT_TABLE_SIZE );

    // Hash the list
    C3HashList( &hTable, &obj->positionList );

    // Make each hash list "unique" by sorting by index
    for( i = 0; i < hTable.tableSize; i++ )
    {
        C3MakeIndexedListDataUnique( hTable.table + i,
                                     &unique, 
                                     C3ComparePosition,
                                     numUnique, // base for the index
                                     C3CopyPosition
                                    );
        numUnique = numUnique + unique;
    }  
    obj->stats->numUniquePositions = numUnique;

    if ( C3GetOptionWeldRadius( C3_TARGET_POSITION ) > C3_FLT_EPSILON ) 
    {
        // Weld positions (not just within the index)
        c3CurrentlyWelding = C3_TRUE;
        numUnique = 0;
        for( i = 0; i < hTable.tableSize; i++ )
        {
            C3MakeIndexedHTableDataUnique( &hTable,
                                           i, 
                                           &unique, 
                                           C3ComparePosition,
                                           numUnique,
                                           C3CopyPosition); // base for the index                                  
            numUnique = numUnique + unique;
        }  
        obj->stats->numPosWelded += obj->stats->numUniquePositions - numUnique;
        obj->stats->numUniquePositions = numUnique;
        c3CurrentlyWelding = C3_FALSE;
        
    }

    // Rebuild the list from the hash table
    DSHTableToList( &hTable, &obj->positionList );

    // Make each position in the list unique (remove positions with duplicate indices)
    C3CompressIndexedList( &obj->positionList );

}


/*---------------------------------------------------------------------------*
  Name:         C3CompressTexCoords

  Description:  Compress  the tex coord linked list assigning a unique index 
                for duplicate data

  Arguments:    obj     - object to reduce the duplicate data for 
                channel - texture coordinate channel to compress
                  
  Returns:      NONE
/*---------------------------------------------------------------------------*/

static void
C3CompressTexCoords( C3GeomObject* obj, u8 channel )
{
    DSHashTable hTable;
    DSList      listArray[ FLT_TABLE_SIZE ];
    C3TexCoord  texCoord;
    u16 unique    = 0;
    u16 i         = 0;
    u32 count     = 0;
    u16 numUnique = 0;

    if( obj->stats->numTCoords[channel] <= 0 )
        return;

    // Initialize the hash table
    DSInitHTable( &hTable, 
                  FLT_TABLE_SIZE, 
                  listArray,
                  HashTexCoord,
                  (Ptr)&texCoord, 
                  &(texCoord.link) );

    // Initialize the hashing parameters
    C3InitHashFloat( obj->minTexCoord[channel], obj->maxTexCoord[channel], FLT_TABLE_SIZE );

    // Hash the list
    C3HashList( &hTable, &obj->texCoordList[channel] );

    // Make each hash list "unique" by sorting by index
    for( i = 0; i < hTable.tableSize; i++ )
    {
        C3MakeIndexedListDataUnique( hTable.table + i,
                                     &unique, 
                                     C3CompareTexCoord,
                                     numUnique, // base for the index
                                     C3CopyTexCoord
                                    );
        numUnique = numUnique + unique;
    }  
    obj->stats->numUniqueTCoords[channel] = numUnique;

    if ( C3GetOptionWeldRadius( C3_TARGET_TEXCOORD ) > C3_FLT_EPSILON ) 
    {
        // Weld positions (not just within the index)
        c3CurrentlyWelding = C3_TRUE;
        numUnique = 0;
        for( i = 0; i < hTable.tableSize; i++ )
        {
            C3MakeIndexedHTableDataUnique( &hTable,
                                           i, 
                                           &unique, 
                                           C3CompareTexCoord,
                                           numUnique,
                                           C3CopyTexCoord); // base for the index                                  
            numUnique = numUnique + unique;
        }  
        obj->stats->numTexWelded[channel] += obj->stats->numUniqueTCoords[channel] - numUnique;
        obj->stats->numUniqueTCoords[channel] = numUnique;
        c3CurrentlyWelding = C3_FALSE;
    }

    // Rebuild the list from the hash table
    DSHTableToList( &hTable, &obj->texCoordList[channel] );
}


/*---------------------------------------------------------------------------*
  Name:         C3CompressColor

  Description:  Compress the color linked list assigning a unique index 
                for duplicate data. TODO: Remove unused colors.

  Arguments:    obj - object to reduce the duplicate data for 
                  
  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3CompressColor( C3GeomObject* obj )
{
    DSHashTable hTable;
    DSList      listArray[ COLOR_TABLE_SIZE ];
    C3Color     color;

    C3_ASSERT( C3GetOptionEnableStripFanView() == C3_FALSE );

    if( obj->stats->numColors <= 0 )
        return;
    
    // Initialize the hash table
    DSInitHTable( &hTable, 
                  COLOR_TABLE_SIZE, 
                  listArray,
                  HashColor,
                  (Ptr)&color, 
                  &(color.link) );

    // Initialize the hashing parameters
    C3InitHashFloat( obj->minColDistance, obj->maxColDistance, COLOR_TABLE_SIZE );

    // Compress the indices
    C3CompressIndices(  &obj->colorList, 
                        &obj->stats->numUniqueColors, 
                        C3CompareColor,
                        &hTable,
                        C3CopyColor );
}


/*---------------------------------------------------------------------------*
  Name:         C3CompressNormal

  Description:  Compress the normal linked list assigning a unique index 
                for duplicate data

  Arguments:    obj - object to reduce the duplicate data for 
                  
  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3CompressNormal( C3GeomObject* obj )
{
    DSHashTable hTable;
    DSList      listArray[ FLT_TABLE_SIZE ];
    C3Normal    normal;

    if( obj->stats->numNormals <= 0 )
        return;
    
    // Initialize the hash table
    DSInitHTable( &hTable, 
                  FLT_TABLE_SIZE, 
                  listArray,
                  HashNormal,
                  (Ptr)&normal, 
                  &(normal.link) );

    // Initialize the hashing parameters
    C3InitHashFloat( obj->minNormalX, obj->maxNormalX, FLT_TABLE_SIZE );

    // Compress the indices
    C3CompressIndices(  &obj->normalList, 
                        &obj->stats->numUniqueNormals, 
                        C3CompareNormal,
                        &hTable,
                        C3CopyPosition );
}


/*---------------------------------------------------------------------------*
  Name:         C3CompressAnimation

  Description:  Compress the animation data bank

  Arguments:    NONE
                  
  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3CompressAnimation( void )
{
    C3Actor*    actor;
    C3AnimBank* animBank;
    C3Track*    track;
    u16         numKeys = 0;


    // Make sure the intialization of the hierarchy has been done
    actor = C3GetActor();
    C3_ASSERT( actor != NULL );
 
    animBank = (C3AnimBank*)actor->animBank;   
    if( animBank == NULL)
        return;

    // For each track
    track = (C3Track*)animBank->trackList.Head;
    while( track )
    {
        numKeys = C3CompressAnimTrack( track );
        track = (C3Track*)(track->link.Next);
    }
    animBank->numUniqueKeys = numKeys;
}


/*---------------------------------------------------------------------------*
  Name:         C3CompressAnimTrack

  Description:  Compress an animation track

  Arguments:    track - track to compress

  Returns:      number of unique keys in the track
/*---------------------------------------------------------------------------*/
static u16
C3CompressAnimTrack( C3Track* track )
{
    DSHashTable hTable;
    DSList      listArray[ FLT_TABLE_SIZE ];
    C3KeyFrame  key;


    // Initialize the hash table
    DSInitHTable( &hTable, 
                  FLT_TABLE_SIZE, 
                  listArray,
                  HashKeyFrame, 
                  (Ptr)&key, 
                  &(key.link) );

    // Initialize the hashing parameters
    C3InitHashFloat( track->startTime, track->endTime, FLT_TABLE_SIZE );

    // Compress the indices
    C3CompressIndices(  &track->keyList, 
                        &track->numUniqueKeys, 
                        C3CompareAnimKey,
                        &hTable,
                        NULL );


    return track->numUniqueKeys;
}


/*---------------------------------------------------------------------------*
  Name:         C3CompressIndices

  Description:  Compress the indices of a generic linked list by hashing the
                linked list then making the index of the node unique for 
                duplicate data

  Arguments:    list        - the list to compress
                numUnique   - will contain the number of unique elements 
                              upon return.
                linkAreSame - function to compare the nodes
                hTable      - pre initialized hash table
                  
  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3CompressIndices
( 
  DSList*              list,
  u16*                 numUnique, 
  C3LinkCompareFuncPtr linkAreSame,
  DSHashTable*         hTable,
  C3CopyFuncPtr        copy
)
{
    u16 unique    = 0;
    u16 i         = 0;


    *numUnique = 0;

    // hash the list
    C3HashList( hTable, list );

    // make each hash list unique
    for( i = 0; i < hTable->tableSize; i++ )
    {
        C3MakeIndexedListDataUnique( hTable->table + i,
                                     &unique, 
                                     linkAreSame,
                                     *numUnique, // base for the index
                                     copy
                                    );
        *numUnique = *numUnique + unique;
    }   

    // rebuild the list from the hash table
    DSHTableToList( hTable, list );
}


/*---------------------------------------------------------------------------*
  Name:         C3HashList

  Description:  Hashes the list into the hash table.

  Arguments:    hashTable   hash table to create
                list        list to convert to a hash table
                
  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void 
C3HashList( DSHashTable* hashTable, DSList* list )
{
    void* cursor  = NULL;
    void* current = NULL;


    // for each element in the list
    cursor = list->Head;
    while( cursor )
    {
        current = cursor;
        cursor  = ((C3IndexedLink*)cursor)->link.Next;
  
        // remove the current element from the list
        DSRemoveListObject ( list, current );
        
        // insert in the hash table
        DSInsertHTableObj( hashTable, current );
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3MakeIndexedListDataUnique

  Description:  Indexes the unique nodes in the list starting at the index 
                specified and using the function specified to determine if
                the nodes are the same 

  Arguments:    list        - list to compress
                numUnique   - will contain the number of unique elements 
                              upon return.
                linkAreSame - function to compare the nodes
                indexBase   - the index at which the function will start
                              indexing
                copy        - will copy contents if similar

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3MakeIndexedListDataUnique( DSList* list, u16* numUnique, C3LinkCompareFuncPtr linkAreSame,
                             u16 indexBase, C3CopyFuncPtr copy )
{
    DSList  sortedList;
    void*   cursor    = NULL;
    void*   current   = NULL;
    void*   newCursor = NULL;
    u16     index     = indexBase;

    C3_ASSERT( list );
    C3_ASSERT( numUnique );
    C3_ASSERT( linkAreSame );

    // initialize the new list
    sortedList.Head   = sortedList.Tail = NULL;
    sortedList.Offset = list->Offset; 

    // while there are still element in the old list
    cursor = list->Head;
    while( cursor )
    {
        current = cursor;
        cursor  = ((C3IndexedLink*)cursor)->link.Next;
  
        // remove the current element from the list
        DSRemoveListObject ( list, current );

        // while there are still elements in the new list and the current cursor is still valid
        newCursor = sortedList.Head;
        while( newCursor && current )
        {
            // compare against the new list elements, if the link are the same
            if( linkAreSame( newCursor, current ) )
            {
                // add the element after the compared one
                DSInsertListObject ( &sortedList, newCursor, current );
                if (copy)
                {
                    copy(current, newCursor);
                }
                ((C3IndexedLink*)current)->index = ((C3IndexedLink*)newCursor)->index;
                current = NULL;
                break;
            }
            newCursor = (C3IndexedLink*)C3NextUniqueIndex(newCursor);
        }

        // if the new cursor is still valid, insert the element at the end of the new list
        if( current )
        {
            DSInsertListObject ( &sortedList, NULL, current );
            ((C3IndexedLink*)current)->index = index++;
        }
    }
    
    // replace the information in the passed list
    *list      = sortedList;
    *numUnique = index - indexBase;
}


/*---------------------------------------------------------------------------*
  Name:         C3MakeIndexedHTableDataUnique

  Description:  Same as C3MakeIndexedListDataUnique except it searches other
                hash indices for linkAreSame comparison.  In essence, this 
                function is made for welding

  Arguments:    htable      - hash table
                tableIndex  - index of hash table to compress
                numUnique   - will contain the number of unique elements 
                              upon return.
                linkAreSame - function to compare the nodes
                indexBase   - the index at which the function will start
                              indexing
                copy        - for welding, copies relevant information

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3MakeIndexedHTableDataUnique( DSHashTable* hTable, u16 tableIndex, u16* numUnique, 
                               C3LinkCompareFuncPtr linkAreSame, u16 indexBase,
                               C3CopyFuncPtr copy )
{
    DSList* list;
    DSList  sortedList;
    void*   cursor    = NULL;
    void*   current   = NULL;
    void*   newCursor = NULL;
    u16     index     = indexBase;
    C3Bool  welded    = C3_TRUE;
    u16     currentIndex;

    // initialize the new list
    currentIndex = tableIndex;
    list = hTable->table + currentIndex;
    sortedList.Head   = sortedList.Tail = NULL;
    sortedList.Offset = list->Offset; 

    C3_ASSERT( list );
    C3_ASSERT( numUnique );
    C3_ASSERT( linkAreSame );

    // while there are still element in the old list
    cursor = list->Head;
    while( cursor )
    {
        current = cursor;
        cursor  = ((C3IndexedLink*)cursor)->link.Next;

        // remove the current element from the list
        DSRemoveListObject ( list, current );

        // while there are still elements in the new list and the current cursor is still valid
        newCursor = sortedList.Head;
        while( newCursor && current )
        {
            // compare against the new list elements, if the link are the same
            if( linkAreSame( newCursor, current ) )
            {
                // add the element after the compared one
                DSInsertListObject ( &sortedList, newCursor, current );
                copy(current, newCursor);
                ((C3IndexedLink*)current)->index = ((C3IndexedLink*)newCursor)->index;
                current = NULL;
                break;
            }
            newCursor = (C3IndexedLink*) C3NextUniqueIndex(newCursor);
        }

        // if the new cursor is still valid, insert the element at the end of the new list
        if( current )
        {
            DSInsertListObject ( &sortedList, NULL, current );
            ((C3IndexedLink*)current)->index = index++;
        }
    }

    // If we're done with this list,
    // check the next nonempty hash index to see if we can keep welding
    while ( !cursor && currentIndex < hTable->tableSize - 1 && welded )
    {
        currentIndex++;
        list = hTable->table + currentIndex;
        cursor = DSNextListObj( hTable->table + currentIndex, NULL );
        if ( cursor )
        {
            welded  = C3_FALSE;
        }

        while( cursor )
        {
            current = cursor;
            cursor  = ((C3IndexedLink*)cursor)->link.Next;

            // check if current is the same as any in the sortedlist.
            // if so, insert current into the sorted list.
            newCursor = sortedList.Head;
            while( newCursor && current )
            {
                // compare against the new list elements, if the link are the same
                if( linkAreSame( newCursor, current ) )
                {
                    // add the element after the compared one
                    DSRemoveListObject ( list, current );
                    DSInsertListObject ( &sortedList, newCursor, current );
                    copy(current, newCursor);
                    ((C3IndexedLink*)current)->index = ((C3IndexedLink*)newCursor)->index;
                    welded = C3_TRUE;  // continue welding until we can't
                    break;
                }
                newCursor = (C3IndexedLink*) C3NextUniqueIndex(newCursor);
            }
        }
    }       
    
    // replace the information in the passed list
    *(hTable->table + tableIndex) = sortedList;
    *numUnique = index - indexBase;
}


/*---------------------------------------------------------------------------*
  Name:         C3CompressIndexedList

  Description:  Compresses the list by removing from the linked list all of the
                duplicate indices.  Assumes that the list is ordered with indices 
                in ascending order not starting at -1.  
                Does not deallocate any memory, so need to make sure that objects 
                that are not included in final list can be deallocated.  
                Make sure objects in list are a superset of C3IndexedLink.

  Arguments:    list - list to compress

  Returns:      index for the hash table 
/*---------------------------------------------------------------------------*/
static void
C3CompressIndexedList( DSList* list )
{
    DSList compressedList;
    C3IndexedLink* cursor;
    C3IndexedLink* nextCursor;
    u32 prevIndex = -1;

    C3_ASSERT( list );

    compressedList = *list;
    compressedList.Head = compressedList.Tail = NULL;

    cursor = (C3IndexedLink*)list->Head;
    while ( cursor )
    {
        nextCursor = (C3IndexedLink*)cursor->link.Next;

        DSRemoveListObject( list, (Ptr)cursor );

        if ( cursor->index != prevIndex )
        {
            DSInsertListObject( &compressedList, 0, (Ptr)cursor );
            prevIndex = cursor->index;
        }

        cursor = nextCursor;
    }

    *list = compressedList;
}


/*---------------------------------------------------------------------------*
  Name:         HashPosition

  Description:  Hash function for the position array

  Arguments:    position - position to hash

  Returns:      index for the hash table 
/*---------------------------------------------------------------------------*/
static u16 
HashPosition( void* position )
{
    return HASH_FLT(sqrt(pow(((C3Position*)position)->x, 2) + 
                         pow(((C3Position*)position)->y, 2) + 
                         pow(((C3Position*)position)->z, 2)));
}


/*---------------------------------------------------------------------------*
  Name:         HashTexCoord

  Description:  Hash function for the texture coordinate array
                Hashes on the s value of the tex coord

  Arguments:    st - texture coord to hash

  Returns:      index for the hash table 
/*---------------------------------------------------------------------------*/
static u16 
HashTexCoord( void* st )
{
    return HASH_FLT( ((C3TexCoord*)st)->s );
}


/*---------------------------------------------------------------------------*
  Name:         HashColor

  Description:  Hash function for the color array
                Hashes on all the color parametres 

  Arguments:    color - color to hash...

  Returns:      index for the hash table 
/*---------------------------------------------------------------------------*/
static u16 
HashColor( void* color )
{
    return HASH_FLT(sqrt(pow(((C3Color*)color)->r, 2) + 
                         pow(((C3Color*)color)->g, 2) + 
                         pow(((C3Color*)color)->b, 2) + 
                         pow(((C3Color*)color)->a, 2)));

}


/*---------------------------------------------------------------------------*
  Name:         HashNormal

  Description:  Hash function for the normal array
                Hashes on the x value of the position

  Arguments:    normal - normal to hash

  Returns:      index for the hash table 
/*---------------------------------------------------------------------------*/
static u16 
HashNormal( void* normal )
{
    return HASH_FLT( ((C3Normal*)normal)->x );
}


/*---------------------------------------------------------------------------*
  Name:         HashKeyFrame

  Description:  Hash function for the keyframe array
                Hashes on the key frame time

  Arguments:    kf - key frame to hash

  Returns:      index for the hash table 
/*---------------------------------------------------------------------------*/
static u16 
HashKeyFrame( void* kf )
{
    return HASH_FLT( ((C3KeyFrame*)kf)->time );
}


/*---------------------------------------------------------------------------*
  Name:         C3ComparePosition

  Description:  Compare the positions

  Arguments:    p1 - first position
                p2 - second position

  Returns:      C3_TRUE if they are the same
/*---------------------------------------------------------------------------*/
static C3Bool C3ComparePosition( void* p1, void* p2 )
{
    C3Bool       same   = C3_TRUE;
    C3Position*  pp1    = NULL;
    C3Position*  pp2    = NULL;


    if( (p1 == NULL) && ( p2 == NULL) )
    {
        return C3_TRUE;
    }
    else if( (p1 == NULL) || (p2 == NULL) )
    {
        return C3_FALSE;
    }

    pp1 = (C3Position*)p1;
    pp2 = (C3Position*)p2;

    if( pp1 == pp2 )
        return C3_TRUE;
    else
        return ( C3_FLOAT_EQUAL( pp1->x, pp2->x ) &&
                 C3_FLOAT_EQUAL( pp1->y, pp2->y ) &&
                 C3_FLOAT_EQUAL( pp1->z, pp2->z ) )
               || (c3CurrentlyWelding &&
                   sqrt(pow(pp1->x - pp2->x, 2) +
                        pow(pp1->y - pp2->y, 2) +
                        pow(pp1->z - pp2->z, 2)) < C3GetOptionWeldRadius( C3_TARGET_POSITION) );
}


/*---------------------------------------------------------------------------*
  Name:         C3CompareTexCoord

  Description:  Compare the texture coordinate

  Arguments:    p1 - first texture coord
                p2 - second texture coord

  Returns:      C3_TRUE if they are the same
/*---------------------------------------------------------------------------*/
static C3Bool
C3CompareTexCoord( void* p1, void* p2 )
{
    C3Bool       same   = C3_TRUE;
    C3TexCoord*  pp1    = NULL;
    C3TexCoord*  pp2    = NULL;


    if( (p1 == NULL) && ( p2 == NULL) )
    {
        return C3_TRUE;
    }
    else if( (p1 == NULL) || (p2 == NULL) )
    {
        return C3_FALSE;
    }


    pp1 = (C3TexCoord*)p1;
    pp2 = (C3TexCoord*)p2;

    if(pp1 != pp2)
    {
        if( !pp1 || !pp2 )
            same = C3_FALSE;
        else 
        {
            same =  ( C3_FLOAT_EQUAL( pp1->s, pp2->s ) &&
                      C3_FLOAT_EQUAL( pp1->t, pp2->t ) )
                    || (c3CurrentlyWelding &&
                        sqrt(pow(pp1->s - pp2->s, 2) +
                             pow(pp1->t - pp2->t, 2)) < C3GetOptionWeldRadius( C3_TARGET_TEXCOORD) );
        }
    }

    return same;
}


/*---------------------------------------------------------------------------*
  Name:         C3CompareColor

  Description:  Compare the colors.  If the alpha of any color is not 255, then
                we will be exporting that particular color using quantization
                that includes color alpha.

  Arguments:    c1 - first color
                c2 - second color

  Returns:      C3_TRUE if they are the same
/*---------------------------------------------------------------------------*/
static C3Bool
C3CompareColor( void* c1, void* c2 )
{
    C3Bool      same    = C3_TRUE;
    C3Color*    pc1     = NULL; 
    C3Color*    pc2     = NULL; 
    u32         value1  = 0;
    u32         value2  = 0;
    u8          destCode = 0;


    if( (c1 == NULL) && ( c2 == NULL) )
    {
        return C3_TRUE;
    }
    else if( (c1 == NULL) || (c2 == NULL) )
    {
        return C3_FALSE;
    }


    pc1 = (C3Color*)c1;
    pc2 = (C3Color*)c2;
    
    // Make sure both colors will be quantized to the same thing
    if ( (pc1->a == 255 && pc2->a != 255) ||
         (pc1->a != 255 && pc2->a == 255) )
        return C3_FALSE;

    if ( pc1->a == 255 )
        destCode = C3GetOptionQuantization( C3_TARGET_COLOR, 0 );
    else
        destCode = C3GetOptionQuantization( C3_TARGET_COLORALPHA, 0 );

    if( pc1 != pc2 )
    {
        if( !pc1 || !pc2 )
            same = C3_FALSE;
        else
        {
            // TBD: Perhaps could memoize this to speed up C3 library.
            C3QuantizeColor( (void*)&value1, destCode, pc1);
            C3QuantizeColor( (void*)&value2, destCode, pc2);
            same = ( value1 == value2 );
        }
    }
    return same;
}


/*---------------------------------------------------------------------------*
  Name:         C3CompareNormal

  Description:  Compare the normals

  Arguments:    n1 - first normal
                n2 - second normal

  Returns:      C3_TRUE if they are the same
/*---------------------------------------------------------------------------*/
static C3Bool
C3CompareNormal( void* n1, void* n2 )
{
    C3Bool      same     = C3_TRUE;
    C3Normal*   nn1      = NULL;
    C3Normal*   nn2      = NULL;
    u32         value1   = 0;
    u32         value2   = 0;
    u8          destCode;


    if( (n1 == NULL) && ( n2 == NULL) )
    {
        return C3_TRUE;
    }
    else if( (n1 == NULL) || (n2 == NULL) )
    {
        return C3_FALSE;
    }


    nn1 = (C3Normal*)n1;
    nn2 = (C3Normal*)n2;

    destCode = C3GetOptionQuantization( C3_TARGET_NORMAL, 0 );

    if( nn1 != nn2 )
    {
        if ( !nn1 || !nn2 )
            same = C3_FALSE;
        else
        {
            C3QuantizeFloat( (void*)&value1, destCode, nn1->x );
            C3QuantizeFloat( (void*)&value2, destCode, nn2->x );
            if ( value1 != value2 )
                return C3_FALSE;
            value1 = value2 = 0;
            C3QuantizeFloat( (void*)&value1, destCode, nn1->y );
            C3QuantizeFloat( (void*)&value2, destCode, nn2->y );
            if ( value1 != value2 )
                return C3_FALSE;
            value1 = value2 = 0;
            C3QuantizeFloat( (void*)&value1, destCode, nn1->z );
            C3QuantizeFloat( (void*)&value2, destCode, nn2->z );
            if ( value1 != value2 )
                return C3_FALSE;
        }
    }

    return same;
}


/*---------------------------------------------------------------------------*
  Name:         C3CompareAnimKey

  Description:  Compare the animation keys

  Arguments:    kf1 - first key to compare
                kf2 - second key to compare

  Returns:      C3_TRUE if they are the same
/*---------------------------------------------------------------------------*/
C3Bool
C3CompareAnimKey( void* kf1, void* kf2 )
{
    C3KeyFrame* pKf1 = (C3KeyFrame*)kf1;  
    C3KeyFrame* pKf2 = (C3KeyFrame*)kf2;  
    C3Bool      same = C3_TRUE;


    C3_ASSERT( kf1 && kf2 );

    same = C3CompareControl( &pKf1->control, &pKf2->control );
    // TODO: Add comparison of interp info}

    return same;
}


/*---------------------------------------------------------------------------*
  Name:         C3CompareControl

  Description:  Compare two controls

  Arguments:    c1 - first control to compare
                c2 - second control to compare

  Returns:      C3_TRUE if they are the same
/*---------------------------------------------------------------------------*/
static C3Bool
C3CompareControl( void* c1, void* c2 )
{
    CTRLControl*  cc1 = (CTRLControl*)c1;
    CTRLControl*  cc2 = (CTRLControl*)c2;
    C3Bool      same = C3_TRUE;
    
    if( cc1->type != cc2->type )
        return C3_FALSE;

    if( cc1->type & CTRL_MTX )
    {
        same = C3CompareMatrix( (void*)&cc1->controlParams.mtx.m,
                                (void*)&cc2->controlParams.mtx.m );
        return same;
    }

    if( cc1->type & CTRL_SCALE )
    {
        same = C3CompareVec( (void*)&cc1->controlParams.srt.s,
                             (void*)&cc2->controlParams.srt.s );
        if( !same )
            return same;
    }

    if( cc1->type & CTRL_ROT_QUAT )
    {
        same = C3CompareQuat( (void*)&cc1->controlParams.srt.r,
                              (void*)&cc2->controlParams.srt.r );
        if( !same )
            return same;
    }
    else if( cc1->type & CTRL_ROT_EULER )
    {
        same = C3CompareVec( (void*)&cc1->controlParams.srt.r,
                             (void*)&cc2->controlParams.srt.r );
        if( !same )
            return same;
    }

    if( cc1->type & CTRL_TRANS )
    {
        same = C3CompareVec( (void*)&cc1->controlParams.srt.t,
                             (void*)&cc2->controlParams.srt.t );
        if( !same )
            return same;
    }

    return same;
}


/*---------------------------------------------------------------------------*
  Name:         C3CompareVec

  Description:  Compares 2 vectors

  Arguments:    a - first vector to compare
                b - second vector to compare

  Returns:      C3_TRUE is they are the same
/*---------------------------------------------------------------------------*/
static C3Bool
C3CompareVec( void* a, void* b )
{
    Vec* v1 = (Vec*)a; 
    Vec* v2 = (Vec*)b; 

    return ( C3_FLOAT_EQUAL( v1->x, v2->x ) &&
             C3_FLOAT_EQUAL( v1->y, v2->y ) &&
             C3_FLOAT_EQUAL( v1->z, v2->z ) );
}


/*---------------------------------------------------------------------------*
  Name:         C3CompareQuat

  Description:  Compares 2 quaternions

  Arguments:    a - first quaternion to compare
                b - second quaternion to compare

  Returns:      C3_TRUE is they are the same
/*---------------------------------------------------------------------------*/
static C3Bool
C3CompareQuat( void* a, void* b )
{
    Quaternion* q1 = (Quaternion*)a; 
    Quaternion* q2 = (Quaternion*)b;

    return ( C3_FLOAT_EQUAL( q1->x, q2->x ) &&
             C3_FLOAT_EQUAL( q1->y, q2->y ) &&
             C3_FLOAT_EQUAL( q1->z, q2->z ) &&
             C3_FLOAT_EQUAL( q1->w, q2->w ) );
}


/*---------------------------------------------------------------------------*
  Name:         C3CompareMatrix

  Description:  Compare matrices

  Arguments:    a - first matrix to compare
                b - second matrix to compare

  Returns:      C3_TRUE is they are the same
/*---------------------------------------------------------------------------*/
static C3Bool
C3CompareMatrix( void* a, void* b )
{
    C3Bool  same = C3_TRUE;
    Mtx*    m1 = (Mtx*)a;
    Mtx*    m2 = (Mtx*)b;
    u8      i,j;


    for( i = 0; (i < 4) && (same); i++ )
        for( j = 0; (j < 4) && (same); j++ )
            same = C3_FLOAT_EQUAL( MTXRowCol(*m1,i,j), MTXRowCol(*m2,i,j) );

    return same;
}


/*---------------------------------------------------------------------------*
  Name:         C3CopyPosition

  Description:  Copies x,y,z from src to dest

  Arguments:    src - source of copy
                dest - where to copy to

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3CopyPosition( void* dest, void* src )
{
    ((C3Position*)dest)->x = ((C3Position*)src)->x;
    ((C3Position*)dest)->y = ((C3Position*)src)->y;
    ((C3Position*)dest)->z = ((C3Position*)src)->z;
}


/*---------------------------------------------------------------------------*
  Name:         C3CopyTexCoord

  Description:  Copies s,t from src to dest

  Arguments:    src - source of copy
                dest - where to copy to

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3CopyTexCoord( void* dest, void* src )
{
    ((C3TexCoord*)dest)->s = ((C3TexCoord*)src)->s;
    ((C3TexCoord*)dest)->t = ((C3TexCoord*)src)->t;
}


/*---------------------------------------------------------------------------*
  Name:         C3CopyColor

  Description:  Copies r,g,b,a from src to dest

  Arguments:    src - source of copy
                dest - where to copy to

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3CopyColor( void* dest, void* src )
{
    ((C3Color*)dest)->r = ((C3Color*)src)->r;
    ((C3Color*)dest)->g = ((C3Color*)src)->g;
    ((C3Color*)dest)->b = ((C3Color*)src)->b;
    ((C3Color*)dest)->a = ((C3Color*)src)->a;
}


/*---------------------------------------------------------------------------*
  Name:         C3ConvertToStripFan

  Description:  Converts the primitive of the object to quads/strips/fans

  Arguments:    geomObj - geomObj to convert

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3ConvertToStripFan( C3GeomObject* geomObj )
{
    C3CreateTriStripsGeomObject( geomObj );         // strip, tri
    C3ShortGeoStripsToQuads( geomObj );             // strip, tri, quad

    C3JoinGeoFans( geomObj );                       // strip, tri, quad, fan

    // Convert all quads to fans if C3_QUADS_TO_FANS is defined.  
    // Non-coplanar quads are not a problem in the hardware, but there 
    // can be backface rejection issues in Mac OpenGL, which the emulator uses.
#ifdef C3_QUADS_TO_FANS
    C3FansFromGeoQuads( geomObj );                  // strip, tri,       fan
#else
    C3ShortGeoFansToQuads( geomObj );               // strip, tri, quad, fan
#endif

    if( C3GetOptionEnableStripFanView() )
    {
        C3ColorVerticesByPrim( geomObj );
    }

}


//---------------------------------------------------------------------------------


void
C3QuadifyGeomObject( C3GeomObject* geomObj )
{
    DSList*         triList;
    DSList          quadList;
    C3Primitive*    cursor;
    C3Primitive*    prim;

    
    C3_ASSERT( geomObj );

    //  Copy the list offset
    triList = &geomObj->primitiveList;
    quadList = *triList;
    quadList.Head = quadList.Tail = NULL;

    // Traverse the primitive list
    cursor = (C3Primitive*)triList->Head;
    while( cursor )
    {
        // Remove the primitive from the list
        DSRemoveListObject( triList, (void*)cursor);

        prim = (C3Primitive*)triList->Head;
        while( prim )
        {
            // if can make a quad
            if( (prim->type == C3_PRIM_TRI) && (cursor->type == C3_PRIM_TRI) )
            {
                if( C3ComparePrimAttr( cursor, prim ) &&
                    C3CreateQuad( cursor, prim ) )
                {
                    geomObj->stats->numTri -= 2;
                    geomObj->stats->numQuad++;
                    // remove the tri primtive from the list
                    // TBD Do this more thoroughly
                    DSRemoveListObject( triList, (void*)prim );
                    prim = NULL;
                    continue;
                }
            }
            prim = (C3Primitive*)prim->link.Next;
        }
        DSInsertListObject( &quadList, NULL, (void*)cursor );
        cursor = (C3Primitive*)triList->Head;
    }

    *triList = quadList;
}


/*---------------------------------------------------------------------------*
  Name:         TBD

  Description:  TBD

  Arguments:    TBD

  Returns:      TBD
/*---------------------------------------------------------------------------*/
static C3Bool
C3CreateQuad( C3Primitive* p1, C3Primitive* p2 )
{
    C3Vertex    *vtx1[4];
    C3Vertex    *vtx2[4];
    C3PtrLink   *ptr1[4];
    C3PtrLink   *ptr2[4];
    s8          vtxIndex = -1;
    s8          i,j,iVal;


    ptr1[0] = (C3PtrLink*)p1->vertexPtrList.Head;
    ptr1[1] = (C3PtrLink*)ptr1[0]->link.Next;
    ptr1[2] = (C3PtrLink*)ptr1[1]->link.Next;
    ptr1[3] = (C3PtrLink*)p1->vertexPtrList.Head;

    ptr2[0] = (C3PtrLink*)p2->vertexPtrList.Head;
    ptr2[1] = (C3PtrLink*)ptr2[0]->link.Next;
    ptr2[2] = (C3PtrLink*)ptr2[1]->link.Next;
    ptr2[3] = (C3PtrLink*)p2->vertexPtrList.Head;

    vtx1[0] = (C3Vertex*)ptr1[0]->ptr;
    vtx1[1] = (C3Vertex*)ptr1[1]->ptr;
    vtx1[2] = (C3Vertex*)ptr1[2]->ptr;
    vtx1[3] = (C3Vertex*)ptr1[0]->ptr;

    vtx2[0] = (C3Vertex*)ptr2[0]->ptr;
    vtx2[1] = (C3Vertex*)ptr2[1]->ptr;
    vtx2[2] = (C3Vertex*)ptr2[2]->ptr;
    vtx2[3] = (C3Vertex*)ptr2[0]->ptr;


    for( i = 0; (i < 3) && (vtxIndex == -1); i++ )
    {
        for( j = 0; (j < 3) && (vtxIndex == -1); j++ ) 
        {
            if( C3CompareVertex( vtx1[i], vtx2[j+1]) &&
                C3CompareVertex( vtx1[i+1], vtx2[j]) )
            {
                vtxIndex = (j + 2)%3;
                iVal = (i+2)%3;;
            }
        }
    }

    if( vtxIndex != -1 )
    {
        // Reconstruct the list of vertices in the quad     
        p1->vertexPtrList.Head = p1->vertexPtrList.Tail = NULL;
        DSInsertListObject( &p1->vertexPtrList, NULL, (void*)ptr1[(iVal+2)%3] );
        DSInsertListObject( &p1->vertexPtrList, NULL, (void*)ptr1[iVal] );
        DSInsertListObject( &p1->vertexPtrList, NULL, (void*)ptr2[(vtxIndex+2)%3] );
        DSInsertListObject( &p1->vertexPtrList, NULL, (void*)ptr2[vtxIndex] );
        
        p1->numVertex++;
        p1->type = C3_PRIM_QUAD;
    }

    return (vtxIndex != -1);
}


/*---------------------------------------------------------------------------*
  Name:         TBD

  Description:  Creates a strip with 2 quads (if possible)

  Arguments:    TBD

  Returns:      TBD
/*---------------------------------------------------------------------------*/
static C3Bool
C3QuadsToStrip( C3Primitive* q1, C3Primitive* q2 )
{
    C3Bool      stripDone = C3_FALSE;
    C3Vertex    *AB[4];
    C3Vertex    *CD[4];
    C3PtrLink   *ABp[4];
    C3PtrLink   *CDp[4];
    C3PtrLink   *strip[6];
    u8          i;
 

    C3_ASSERT( (q1->numVertex == 4) && (q2->numVertex == 4) );

    ABp[0] = (C3PtrLink*)q1->vertexPtrList.Head;
    ABp[1] = (C3PtrLink*)ABp[0]->link.Next;
    ABp[2] = (C3PtrLink*)ABp[1]->link.Next;
    ABp[3] = (C3PtrLink*)ABp[2]->link.Next;

    CDp[0] = (C3PtrLink*)q2->vertexPtrList.Head;
    CDp[1] = (C3PtrLink*)CDp[0]->link.Next;
    CDp[2] = (C3PtrLink*)CDp[1]->link.Next;
    CDp[3] = (C3PtrLink*)CDp[2]->link.Next;

    AB[0] = (C3Vertex*)ABp[0]->ptr;
    AB[1] = (C3Vertex*)ABp[1]->ptr;
    AB[2] = (C3Vertex*)ABp[2]->ptr;
    AB[3] = (C3Vertex*)ABp[3]->ptr;

    CD[0] = (C3Vertex*)CDp[0]->ptr;
    CD[1] = (C3Vertex*)CDp[1]->ptr;
    CD[2] = (C3Vertex*)CDp[2]->ptr;
    CD[3] = (C3Vertex*)CDp[3]->ptr;


    if( C3CompareVertex( AB[0], CD[1] ) &&
        C3CompareVertex( AB[3], CD[2] ) )
    {
        strip[0] = ABp[1];      
        strip[1] = ABp[2];
        strip[2] = ABp[0];      
        strip[3] = ABp[3];
        strip[4] = CDp[0];      
        strip[5] = CDp[3];
        stripDone = C3_TRUE;
    }

    if( !stripDone &&
        C3CompareVertex( AB[2], CD[1] ) &&
        C3CompareVertex( AB[1], CD[2] ) )
    {
        strip[0] = ABp[3];      
        strip[1] = ABp[0];
        strip[2] = ABp[2];      
        strip[3] = ABp[1];
        strip[4] = CDp[0];      
        strip[5] = CDp[3];
        stripDone = C3_TRUE;
    }


    if( !stripDone &&
        C3CompareVertex( AB[1], CD[0] ) &&
        C3CompareVertex( AB[2], CD[3] ) )
    {
        strip[0] = CDp[1];      
        strip[1] = CDp[2];
        strip[2] = CDp[0];      
        strip[3] = CDp[3];
        strip[4] = ABp[0];      
        strip[5] = ABp[3];
        stripDone = C3_TRUE;
    }

    if( !stripDone &&
        C3CompareVertex( AB[3], CD[0] ) &&
        C3CompareVertex( AB[0], CD[3] ) )
    {
        strip[0] = CDp[1];      
        strip[1] = CDp[2];
        strip[2] = CDp[0];      
        strip[3] = CDp[3];
        strip[4] = ABp[2];      
        strip[5] = ABp[1];
        stripDone = C3_TRUE;
    }

    if( stripDone )
    {
        q1->vertexPtrList.Head = q2->vertexPtrList.Tail = NULL;
        for( i = 0; i < 6; i++ )
        {
            DSInsertListObject( &q1->vertexPtrList, NULL, (void*)strip[i] );
        }
        q1->type = C3_PRIM_STRIP;
        q1->numVertex = 6;
    }

    return stripDone;
}

//--------------------------------------------------------------------------------------------

static void
C3QuadToStrip( C3Primitive* quad )
{
    C3PtrLink   *quadVtxPtr[4];

    quadVtxPtr[0] = (C3PtrLink*)quad->vertexPtrList.Head;
    quadVtxPtr[1] = (C3PtrLink*)quadVtxPtr[0]->link.Next;
    quadVtxPtr[2] = (C3PtrLink*)quadVtxPtr[1]->link.Next;
    quadVtxPtr[3] = (C3PtrLink*)quadVtxPtr[2]->link.Next;

    DSRemoveListObject( &quad->vertexPtrList, (void*)quadVtxPtr[0] );
    DSRemoveListObject( &quad->vertexPtrList, (void*)quadVtxPtr[1] );
    DSRemoveListObject( &quad->vertexPtrList, (void*)quadVtxPtr[2] );
    DSRemoveListObject( &quad->vertexPtrList, (void*)quadVtxPtr[3] );

    DSInsertListObject( &quad->vertexPtrList, NULL, (void*)quadVtxPtr[1] );
    DSInsertListObject( &quad->vertexPtrList, NULL, (void*)quadVtxPtr[2] );
    DSInsertListObject( &quad->vertexPtrList, NULL, (void*)quadVtxPtr[0] );
    DSInsertListObject( &quad->vertexPtrList, NULL, (void*)quadVtxPtr[3] );

    quad->type = C3_PRIM_STRIP;
}

//--------------------------------------------------------------------------------------

static C3Bool
C3AddQuadToStrip( C3Primitive* strip, C3Primitive* quad )
{
    #define HEAD        0 
    #define HEAD_NEXT   1
    #define TAIL        2
    #define TAIL_PREV   3   
    
    C3Vertex    *stripVtx[4];
    C3Vertex    *quadVtx[4];
    C3PtrLink   *quadVtxPtr[4];
    C3PtrLink   *vtxPtr;
    u8          last;
    u8          secondLast;
    C3Bool      match = C3_FALSE;
    void        *before = NULL;


    if( strip->type != C3_PRIM_STRIP )
        return match;

    quadVtxPtr[0] = (C3PtrLink*)quad->vertexPtrList.Head;
    quadVtxPtr[1] = (C3PtrLink*)quadVtxPtr[0]->link.Next;
    quadVtxPtr[2] = (C3PtrLink*)quadVtxPtr[1]->link.Next;
    quadVtxPtr[3] = (C3PtrLink*)quadVtxPtr[2]->link.Next;

    quadVtx[0] = (C3Vertex*)quadVtxPtr[0]->ptr;
    quadVtx[1] = (C3Vertex*)quadVtxPtr[1]->ptr;
    quadVtx[2] = (C3Vertex*)quadVtxPtr[2]->ptr;
    quadVtx[3] = (C3Vertex*)quadVtxPtr[3]->ptr;

    vtxPtr = (C3PtrLink*)strip->vertexPtrList.Tail;
    stripVtx[TAIL] = (C3Vertex*)(vtxPtr->ptr);
    vtxPtr = (C3PtrLink*)vtxPtr->link.Prev;
    stripVtx[TAIL_PREV] = (C3Vertex*)(vtxPtr->ptr);

    vtxPtr = (C3PtrLink*)strip->vertexPtrList.Head;
    stripVtx[HEAD] = (C3Vertex*)(vtxPtr->ptr);
    vtxPtr = (C3PtrLink*)vtxPtr->link.Next;
    stripVtx[HEAD_NEXT] = (C3Vertex*)(vtxPtr->ptr);


    // Compare strip tail vs quad 2,1
    if( C3CompareVertex( stripVtx[TAIL], quadVtx[2] ) &&
        C3CompareVertex( stripVtx[TAIL_PREV], quadVtx[1] ) )        
    {
        secondLast = 0; // second to last is 0
        last       = 3; // last is 3
        match = C3_TRUE;
    }

    // Compare strip tail vs quad 0,3 
    if( !match &&
        C3CompareVertex( stripVtx[TAIL], quadVtx[0] ) &&
        C3CompareVertex( stripVtx[TAIL_PREV], quadVtx[3] ) )        
    {
        secondLast  = 2;    // second to last is 2
        last        = 1;// last is 1
        match = C3_TRUE;
    }

    // Compare strip head to quad 3,0
    if( !match &&
        C3CompareVertex( stripVtx[HEAD], quadVtx[0] ) &&
        C3CompareVertex( stripVtx[HEAD_NEXT], quadVtx[3] ) )        
    {
        before = strip->vertexPtrList.Head;
        secondLast = 2;
        last = 1;
        match = C3_TRUE;
    }

    // Compare strip head to quad 1,2
    if( !match &&
        C3CompareVertex( stripVtx[HEAD], quadVtx[2] ) &&
        C3CompareVertex( stripVtx[HEAD_NEXT], quadVtx[1] ) )        
    {
        before = strip->vertexPtrList.Head;
        secondLast = 0;
        last = 3;
        match = C3_TRUE;
    }


    // if there is a match
    if( match )
    {
        // remove the second to last vertex from the quad
        DSRemoveListObject( &quad->vertexPtrList, (void*)quadVtxPtr[secondLast] );
        // insert at the end of the strip
        DSInsertListObject( &strip->vertexPtrList, before, (void*)quadVtxPtr[secondLast] );

        if( before )
            before = strip->vertexPtrList.Head;
        // remove the last vertex from the quad
        DSRemoveListObject( &quad->vertexPtrList, (void*)quadVtxPtr[last] );
        // insert at the end of the strip
        DSInsertListObject( &strip->vertexPtrList, before, (void*)quadVtxPtr[last] );

        // increment the vertex count in the strip
        strip->numVertex += 2;
    }

    return match; // return if there is a match flag
}


/*---------------------------------------------------------------------------*
  Name:         C3ComparePrimAttr

  Description:  Compares two primitives to determine whether both primitives
                can be in the same triangle strip or fan.

  Arguments:    p1 - first primitive
                p2 - second primitive

  Returns:      true or false
/*---------------------------------------------------------------------------*/
static C3Bool C3ComparePrimAttr( C3Primitive* p1, C3Primitive* p2 )
{
    C3Bool  same = C3_FALSE;
    u8      i = 0;

    same = C3CompareTextures( p1->texture[0], p2->texture[0] );

    return same;
}


/*---------------------------------------------------------------------------*
  Name:         C3CompareVertex

  Description:  Compares two vertices

  Arguments:    v1 - first vertex
                v2 - second vertex

  Returns:      true or false
/*---------------------------------------------------------------------------*/
static C3Bool
C3CompareVertex( C3Vertex* v1, C3Vertex* v2 )
{
    C3Bool same =  C3_FALSE;
    u16     i   = 0;

    // Compare all aspects of the vertex:
    // NOTE: For boneIndex, we are checking for same position matrix index
    // by seeing if pointers to the index field of C3HierNode is the same
    if( v1->boneIndex == v2->boneIndex &&   
        C3ComparePosition(  v1->position, v2->position ) &&
        C3CompareColor( v1->color, v2->color ) &&
        C3ComparePosition( v1->normal, v2->normal ) )
    {
        same = C3_TRUE;
        for( i = 0; (i < C3_MAX_TEXTURES) && same ; i++)
        {
            same = C3CompareTexCoord( (void*)v1->tCoord[i], (void*)v2->tCoord[i] );
        }
    }
    
    return same;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetVCDDataType

  Description:  Sets the vertex component descriptor type for the
                    specified target.

  Arguments:    obj     - geometry object for which VCD will be set
                target  - target for which to get the VCD
                indexQuantType - one of the C3_VCD_* constants
                channel - texture coordinate channel if target is 
                          C3_TARGET_TEXCOORD

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetVCDDataType( C3GeomObject* obj, u32 target, u16 indexQuantType, u8 channel )
{
    switch( target )
    {
        case C3_TARGET_POSITION:
            // TESTING: Uncomment to only do INDEX16
            //indexQuantType = C3_VCD_INDEX16;

            obj->indexPosQuant = indexQuantType & C3_VCD_MASK;
            break;
             
        case C3_TARGET_NORMAL:
            obj->indexNorQuant = indexQuantType & C3_VCD_MASK;
            break;
             
        case C3_TARGET_COLOR:
            obj->indexColQuant = indexQuantType & C3_VCD_MASK;
            break;
             
        case C3_TARGET_TEXCOORD:
            C3_ASSERT( channel >= 0 && channel < C3_MAX_TEXTURES );
            obj->indexTexQuant[channel] = indexQuantType & C3_VCD_MASK;
            break;

        case C3_TARGET_POS_MATRIX:
            obj->indexPosMatQuant = indexQuantType & C3_VCD_MASK;
            break;

        default:
            C3_ASSERT( C3_FALSE );
            break;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3GetVCDDataType

  Description:  Returns the vertex component descriptor type for to the
                    specified target.

  Arguments:    obj     - geometry object that contains the VCD 
                target  - the target for which to get the VCD
                channel - texture coordinate channel if target is 
                          C3_TARGET_TEXCOORD

  Returns:      the VCD
/*---------------------------------------------------------------------------*/
u16
C3GetVCDDataType( C3GeomObject* obj, u32 target, u8 channel )
{
    u16 dataType = C3_VCD_NONE;

    switch( target )
    {
        case C3_TARGET_POSITION:
            dataType = obj->indexPosQuant;
            break;
             
        case C3_TARGET_NORMAL:
            dataType = obj->indexNorQuant;
            break;

        case C3_TARGET_COLOR:
            dataType = obj->indexColQuant;
            break;
             
        case C3_TARGET_TEXCOORD:
            C3_ASSERT( channel >= 0 && channel < C3_MAX_TEXTURES );
            dataType = obj->indexTexQuant[channel];
            break;

        case C3_TARGET_POS_MATRIX:
            dataType = obj->indexPosMatQuant;
            break;
             
        default:
            C3_ASSERT( C3_FALSE );
            break;
    }

    return dataType;
}


//============================================================================================

static void C3CreateTriStripsGeomObject( C3GeomObject* geomObj )
{
    C3Bool       conv = C3_FALSE;
    C3Primitive  primTmp;
    C3Primitive* primBase, *primAdd;
    DSList       triList,  stripList, otherList, tmpList;
    u32          count;

    
    // for coloring strips ( see code at bottom )
    static u8 rColor = 0, gColor = 0, bColor = 0;


    // geomObj primitive list will be partitioned into 3 lists;
    // strips, leftover triangles and 'other' primitives
    // use 'primTmp' to get the correct offset
    DSInitList( &stripList, (void*)(&primTmp), &primTmp.link );
    DSInitList( &triList,   (void*)(&primTmp), &primTmp.link );
    DSInitList( &otherList, (void*)(&primTmp), &primTmp.link );
    DSInitList( &tmpList,   (void*)(&primTmp), &primTmp.link );


    // convert geomObj->primitive list to quads and triangles
    C3QuadifyGeomObject( geomObj );


    // after quadifying, convert all quads to strips.
    // partition geomObj->primitiveList into 3 lists;
    // single triangles, strips and 'other'.
    primBase = (C3Primitive*)(geomObj->primitiveList.Head);
    while( primBase )
    {
        DSRemoveListObject( &geomObj->primitiveList, (void*)primBase );

        switch( primBase->type )
        {
        case C3_PRIM_TRI:
            DSInsertListObject( &triList, NULL, (void*)primBase );
            break;

        case C3_PRIM_QUAD:  // convert all quads to strips of length 2
            C3QuadToStrip( primBase );
            DSInsertListObject( &stripList, NULL, (void*)primBase );
            break;

        case C3_PRIM_STRIP:
            DSInsertListObject( &stripList, NULL, (void*)primBase );
            break;

        default:  // any other primitive type is placed in the 'miscellaneous' list
            DSInsertListObject( &otherList, NULL, (void*)primBase );
            break;
        }
        primBase = (C3Primitive*)(geomObj->primitiveList.Head);

    } // end while( primBase )


    // compare every strip primitive to every other;
    // progressively gluing strips together into longer strips
    C3ReportStatus( "Creating strips for %s...", geomObj->identifier );

    primBase = (C3Primitive*)(stripList.Head);
    while( primBase )
    {
        DSRemoveListObject( &stripList, (void*)primBase );

        primAdd = (C3Primitive*)(stripList.Head);
        while( primAdd )
        {
            // attempt to join the strips- lengthened strip will end up in 'primBase'
            conv = C3AddStripToStrip( primBase, primAdd ); 

            if( conv == C3_TRUE ) // on successful merge, scan the list again
            {
                DSRemoveListObject( &stripList, (void*)primAdd );
                primAdd = (C3Primitive*)(stripList.Head);
                continue;               
            }
            else                  // otherwise, check the next primitive
            {
                primAdd = (C3Primitive*)( primAdd->link.Next );
            }

        } // end while( primAdd )

        // once ( 0 to n ) quads have been joined to primBase,
        // insert primBase into the temporary strip list
        DSInsertListObject( &tmpList, NULL, (void*)primBase );
        primBase = (C3Primitive*)( stripList.Head );

    } // end while( primBase )

    // re-assign tmpList to stripList- tmpList contains all of the 
    // successfully joined strips and unjoined 'quad'strips
    stripList.Offset  = tmpList.Offset;
    stripList.Head    = tmpList.Head;
    stripList.Tail    = tmpList.Tail;
    tmpList.Head      = tmpList.Tail    = NULL;


    // now, traverse the single triangle list and attempt to add 
    // any leftovers to the strips
    primBase = (C3Primitive*)(stripList.Head);
    while( primBase )
    {
        DSRemoveListObject( &stripList, (void*)primBase );
        
        primAdd = (C3Primitive*)(triList.Head);
        while( primAdd )
        {
            // attempt to add this triangle to an existing strip
            conv = C3AddTriToStrip( primBase, primAdd ); 

            if( conv == C3_TRUE ) // on successful merge, scan the list again
            {
                DSRemoveListObject( &triList, (void*)primAdd );
                primAdd = (C3Primitive*)(triList.Head);
                continue;               
            }
            else
            {
                primAdd = (C3Primitive*)(primAdd->link.Next);
            }

        } // end while( primAdd )

        // once ( 0 to n ) triangles have been joined to primBase,
        // insert primBase into the temporary strip list
        DSInsertListObject( &tmpList, NULL, (void*)primBase );
        primBase = (C3Primitive*)( stripList.Head );

    } // end while( primBase )

    // re-assign tmpList to stripList
    stripList.Offset  = tmpList.Offset;
    stripList.Head    = tmpList.Head;
    stripList.Tail    = tmpList.Tail;
    tmpList.Head      = tmpList.Tail    = NULL;


    // re-assemble the 3 lists into a single geomObj->primitive list
    // and update geomObj->stats where required (tri, quad and strip counts)

    // strip list
    count = 0;
    primBase = (C3Primitive*)(stripList.Head);
    while( primBase )
    {
        count++;
        DSRemoveListObject( &stripList, (void*)primBase );
        DSInsertListObject( &geomObj->primitiveList, NULL, (void*)primBase );
        primBase = (C3Primitive*)(stripList.Head);
    }
    geomObj->stats->numStrip = count;

    // triangle list
    count = 0;
    primBase = (C3Primitive*)(triList.Head);
    while( primBase )
    {
        count++;
        DSRemoveListObject( &triList, (void*)primBase );
        DSInsertListObject( &geomObj->primitiveList, NULL, (void*)primBase );
        primBase = (C3Primitive*)(triList.Head);
    }
    geomObj->stats->numTri = count;

    // since all quads are now strips- set this stat to 0
    geomObj->stats->numQuad = 0;

    // add the 'other' prims back into the list but dont change the stats
    primBase = (C3Primitive*)(otherList.Head);
    while( primBase )
    {
        count++;
        DSRemoveListObject( &otherList, (void*)primBase );
        DSInsertListObject( &geomObj->primitiveList, NULL, (void*)primBase );
        primBase = (C3Primitive*)(otherList.Head);
    }

}

//--------------------------------------------------------------------------------------------

// attempt to add stripB to stripA
static C3Bool C3AddStripToStrip( C3Primitive* stripA, C3Primitive* stripB )
{
    C3PtrLink* a0Ptr, *a1Ptr, *a2Ptr, *aN_2Ptr, *aN_1Ptr, *aNPtr;
    C3PtrLink* b0Ptr, *b1Ptr, *b2Ptr, *bN_2Ptr, *bN_1Ptr, *bNPtr;
    C3Vertex*  a0,    *a1,    *a2,    *aN_2,    *aN_1,    *aN;
    C3Vertex*  b0,    *b1,    *b2,    *bN_2,    *bN_1,    *bN;
    C3Bool     match = C3_FALSE;


    // safety check
    if( ( stripA->type != C3_PRIM_STRIP ) || ( stripB->type != C3_PRIM_STRIP ) )
    {
        return C3_FALSE;
    }

    // make sure that primitives have same texture
    if( !C3ComparePrimAttr( stripA, stripB ) )
    {
        return C3_FALSE;
    }

    // 'numVertex' field is only a u8
    if( (stripA->numVertex + stripB->numVertex - 2) > 255 )
    {
        return C3_FALSE;
    }

    // get the head and tail vertices
    a0Ptr   = (C3PtrLink*)(stripA->vertexPtrList.Head);
    a1Ptr   = (C3PtrLink*)(a0Ptr->link.Next);
    a2Ptr   = (C3PtrLink*)(a1Ptr->link.Next);

    aNPtr   = (C3PtrLink*)(stripA->vertexPtrList.Tail);
    aN_1Ptr = (C3PtrLink*)(aNPtr->link.Prev);
    aN_2Ptr = (C3PtrLink*)(aN_1Ptr->link.Prev);

    b0Ptr   = (C3PtrLink*)(stripB->vertexPtrList.Head);
    b1Ptr   = (C3PtrLink*)(b0Ptr->link.Next);
    b2Ptr   = (C3PtrLink*)(b1Ptr->link.Next);

    bNPtr   = (C3PtrLink*)(stripB->vertexPtrList.Tail);
    bN_1Ptr = (C3PtrLink*)(bNPtr->link.Prev);
    bN_2Ptr = (C3PtrLink*)(bN_1Ptr->link.Prev);

    a0   = (C3Vertex*)(a0Ptr->ptr);
    a1   = (C3Vertex*)(a1Ptr->ptr);
    a2   = (C3Vertex*)(a2Ptr->ptr);

    aN   = (C3Vertex*)(aNPtr->ptr);
    aN_1 = (C3Vertex*)(aN_1Ptr->ptr);
    aN_2 = (C3Vertex*)(aN_2Ptr->ptr);

    b0   = (C3Vertex*)(b0Ptr->ptr);
    b1   = (C3Vertex*)(b1Ptr->ptr);
    b2   = (C3Vertex*)(b2Ptr->ptr);

    bN   = (C3Vertex*)(bNPtr->ptr);
    bN_1 = (C3Vertex*)(bN_1Ptr->ptr);
    bN_2 = (C3Vertex*)(bN_2Ptr->ptr);


    // compare tail of stripA to head of stripB
    if( C3CompareVertex( aN_1, b0 ) && C3CompareVertex( aN, b1 ) )      
    {
        DSRemoveListObject( &stripB->vertexPtrList, (void*)b0Ptr );
        DSRemoveListObject( &stripB->vertexPtrList, (void*)b1Ptr );
        
        // stripB list starts at b2Ptr
        DSAttachList( &stripA->vertexPtrList, &stripB->vertexPtrList );
        stripB->vertexPtrList.Head = stripB->vertexPtrList.Tail = NULL;
        stripA->numVertex += (stripB->numVertex - 2);
        match = C3_TRUE;
    }
    // compare head of stripA to tail of stripB
    else if( C3CompareVertex( a0, bN_1 ) && C3CompareVertex( a1, bN ) )     
    {
        DSRemoveListObject( &stripA->vertexPtrList, (void*)a0Ptr );
        DSRemoveListObject( &stripA->vertexPtrList, (void*)a1Ptr );

        // stripA list starts at a2Ptr
        DSAttachList( &stripB->vertexPtrList, &stripA->vertexPtrList );
        stripA->vertexPtrList.Head = stripB->vertexPtrList.Head;
        stripA->vertexPtrList.Tail = stripB->vertexPtrList.Tail;

        stripB->vertexPtrList.Head = stripB->vertexPtrList.Tail = NULL;
        stripA->numVertex += (stripB->numVertex - 2);
        match = C3_TRUE;
    }
 
    return match; 
}

//--------------------------------------------------------------------------------------------

// attempt to add a triangle to the tail of a strip
static C3Bool C3AddTriToStrip( C3Primitive* strip, C3Primitive* tri )
{
    C3PtrLink* sN_1Ptr, *sNPtr;
    C3Vertex*  sN_1,    *sN;
    C3PtrLink* tPtr[3];
    C3Vertex*  t[3];
    C3Bool     match = C3_FALSE;
    s32        newVtx = -1;


    // safety check
    if( ( strip->type != C3_PRIM_STRIP ) || ( tri->type != C3_PRIM_TRI ) )
    {
        return C3_FALSE;
    }

    // make sure that primitives have same texture
    if( !C3ComparePrimAttr( strip, tri ) )
    {
        return C3_FALSE;
    }

    // 'numVertex' field is only a u8
    if( (strip->numVertex + 1) > 255 )
    {
        return C3_FALSE;
    }

    // get the strip's tail vertices
    sNPtr   = (C3PtrLink*)(strip->vertexPtrList.Tail);
    sN_1Ptr = (C3PtrLink*)(sNPtr->link.Prev);

    sN      = (C3Vertex*)(sNPtr->ptr);
    sN_1    = (C3Vertex*)(sN_1Ptr->ptr);


    // get the triangle vertices
    tPtr[0]   = (C3PtrLink*)(tri->vertexPtrList.Head);
    tPtr[1]   = (C3PtrLink*)(tPtr[0]->link.Next);
    tPtr[2]   = (C3PtrLink*)(tPtr[1]->link.Next);

    t[0] = (C3Vertex*)( tPtr[0]->ptr );
    t[1] = (C3Vertex*)( tPtr[1]->ptr );
    t[2] = (C3Vertex*)( tPtr[2]->ptr );

    // compare tail of stripA to the triangle edges.
    if( C3CompareVertex( sN_1, t[0] ) )     
    {
        if( C3CompareVertex( sN, t[1] ) )
        {
            newVtx = 2;
        }
        else if( C3CompareVertex( sN, t[2] ) )
        {
            newVtx = 1;
        }
    }
    else if( C3CompareVertex( sN_1, t[1] ) )        
    {
        if( C3CompareVertex( sN, t[0] ) )
        {
            newVtx = 2;
        }
        else if( C3CompareVertex( sN, t[2] ) )
        {
            newVtx = 0;
        }
    }
    else if( C3CompareVertex( sN_1, t[2] ) )        
    {
        if( C3CompareVertex( sN, t[0] ) )
        {
            newVtx = 1;
        }
        else if( C3CompareVertex( sN, t[1] ) )
        {
            newVtx = 0;
        }
    }

    if( newVtx >= 0 )
    {
        DSRemoveListObject( &tri->vertexPtrList, (void*)tPtr[0] );
        DSRemoveListObject( &tri->vertexPtrList, (void*)tPtr[1] );
        DSRemoveListObject( &tri->vertexPtrList, (void*)tPtr[2] );

        DSInsertListObject( &strip->vertexPtrList, NULL, (void*)tPtr[newVtx] );
        tri->vertexPtrList.Head = tri->vertexPtrList.Tail = NULL;
        strip->numVertex ++;
        match = C3_TRUE;
    }

    return match; 
}

//-----------------------------------------------------------------------------------------

// turn all strips of length 2 into quads
static void C3ShortGeoStripsToQuads( C3GeomObject* pGeo )
{
    C3Primitive* pPrim;


    pPrim = (C3Primitive*)pGeo->primitiveList.Head;
    while( pPrim )
    {
        C3StripToQuad( pGeo, pPrim );

        pPrim = (C3Primitive*)pPrim->link.Next;
    }
}

//-----------------------------------------------------------------------------------------

// convert a strip of length 2 back into a quad
static void C3StripToQuad( C3GeomObject* pGeo, C3Primitive* pStrip )
{
    C3PtrLink* pLink[4];


    if( pStrip->type != C3_PRIM_STRIP )
        return;
    if( pStrip->numVertex != 4 )
        return;


    pLink[0] = (C3PtrLink*)pStrip->vertexPtrList.Head;
    pLink[1] = (C3PtrLink*)pLink[0]->link.Next;
    pLink[2] = (C3PtrLink*)pLink[1]->link.Next;
    pLink[3] = (C3PtrLink*)pLink[2]->link.Next;

    DSRemoveListObject( &pStrip->vertexPtrList, (void*)pLink[0] );
    DSRemoveListObject( &pStrip->vertexPtrList, (void*)pLink[1] );
    DSRemoveListObject( &pStrip->vertexPtrList, (void*)pLink[2] );
    DSRemoveListObject( &pStrip->vertexPtrList, (void*)pLink[3] );

    pStrip->vertexPtrList.Head = pStrip->vertexPtrList.Tail = NULL;

    DSInsertListObject( &pStrip->vertexPtrList, NULL, (void*)pLink[1] );
    DSInsertListObject( &pStrip->vertexPtrList, NULL, (void*)pLink[3] );
    DSInsertListObject( &pStrip->vertexPtrList, NULL, (void*)pLink[2] );
    DSInsertListObject( &pStrip->vertexPtrList, NULL, (void*)pLink[0] );

    pStrip->type = C3_PRIM_QUAD;

    pGeo->stats->numStrip--;
    pGeo->stats->numQuad++;
}

//------------------------------------------------------------------------------------------

// convert a pair of quads into a fan
// use a sub-list of pGeo->primitiveList for converter speed
static void C3FansFromGeoQuadPairs( C3GeomObject* pGeo, C3Primitive* fanSubList )
{
    C3Primitive* pPrim1, *pPrim2;
    C3Bool       madeFan;


    pPrim1 = fanSubList;
    while( pPrim1 )
    {
        madeFan = C3_FALSE;
        pPrim2 = (C3Primitive*)fanSubList;
        while( pPrim2 )
        {

            if( pPrim1 != pPrim2 )
            {
                if( (madeFan = C3TwoQuads2Fan( pGeo, pPrim1, pPrim2 )) == C3_TRUE )
                {
                    break;
                }
            }
    
        pPrim2 = (C3Primitive*)pPrim2->link.Next;

        } // end while( pPrim2 )


        if( madeFan == C3_TRUE )
        {
            pPrim1 = (C3Primitive*)fanSubList;
            continue;
        }

        pPrim1 = (C3Primitive*)pPrim1->link.Next;

    } // end while( pPrim1 )

}

//-----------------------------------------------------------------------------------------

// convert 2 quads into a single fan
// note:  both these quads are still part of pGeo->primitiveList
static C3Bool C3TwoQuads2Fan( C3GeomObject* pGeo, C3Primitive* pQuadA, C3Primitive* pQuadB )
{
    C3PtrLink*   pLinkA[4];
    C3PtrLink*   pLinkB[4];
    C3Vertex*    pVtxA[4];
    C3Vertex*    pVtxB[4];
    u32          join      = 0;


    // TODO: Should remove this when we know MAX_FAN_VTX is always greater
    // than or equal to 6
    if( MAX_FAN_VTX < 6 )
    {
        return C3_FALSE;
    }
    
    if( pQuadA == pQuadB )
    {
        return C3_FALSE;
    }
    if( (pQuadA->type != C3_PRIM_QUAD) && (pQuadA->type != C3_PRIM_FAN) )
    {
        return C3_FALSE;
    }
    if( (pQuadB->type != C3_PRIM_QUAD) && (pQuadB->type != C3_PRIM_FAN) )
    {
        return C3_FALSE;
    }
    if( (pQuadA->numVertex != 4) || (pQuadB->numVertex != 4) )
    {
        return C3_FALSE;
    }

    // make sure that primitives have same texture
    if( !C3ComparePrimAttr( pQuadA, pQuadB ) )
    {
        return C3_FALSE;
    }

    // get the vertices of each quad
    pLinkA[0] = (C3PtrLink*)pQuadA->vertexPtrList.Head;
    pLinkA[1] = (C3PtrLink*)pLinkA[0]->link.Next;
    pLinkA[2] = (C3PtrLink*)pLinkA[1]->link.Next;
    pLinkA[3] = (C3PtrLink*)pLinkA[2]->link.Next;

    pLinkB[0] = (C3PtrLink*)pQuadB->vertexPtrList.Head;
    pLinkB[1] = (C3PtrLink*)pLinkB[0]->link.Next;
    pLinkB[2] = (C3PtrLink*)pLinkB[1]->link.Next;
    pLinkB[3] = (C3PtrLink*)pLinkB[2]->link.Next;


    pVtxA[0] = (C3Vertex*)pLinkA[0]->ptr;
    pVtxA[1] = (C3Vertex*)pLinkA[1]->ptr;
    pVtxA[2] = (C3Vertex*)pLinkA[2]->ptr;
    pVtxA[3] = (C3Vertex*)pLinkA[3]->ptr;

    pVtxB[0] = (C3Vertex*)pLinkB[0]->ptr;
    pVtxB[1] = (C3Vertex*)pLinkB[1]->ptr;
    pVtxB[2] = (C3Vertex*)pLinkB[2]->ptr;
    pVtxB[3] = (C3Vertex*)pLinkB[3]->ptr;


    // there are 8 cases in which quads can be joined:

    // (A0,B2) (A1,B1)
    if( (C3CompareVertex( pVtxA[0], pVtxB[2] )) && (C3CompareVertex( pVtxA[1], pVtxB[1])) )
    {
        join      = 1;
    }

    // (A0,B0) (A1,B3)
    else if( (C3CompareVertex( pVtxA[0], pVtxB[0] )) && (C3CompareVertex( pVtxA[1], pVtxB[3])) )
    {
        join      = 2;
    }

    // (A0,B0) (A3,B1)  
    else if( (C3CompareVertex( pVtxA[0], pVtxB[0] )) && (C3CompareVertex( pVtxA[3], pVtxB[1])) )        
    {
        join      = 3;
    }

    // (A0,B2) (A3,B3)
    else if( (C3CompareVertex( pVtxA[0], pVtxB[2] )) && (C3CompareVertex( pVtxA[3], pVtxB[3])) )        
    {
        join      = 4;
    }

    // (A2,B0) (A3,B3) 
    else if( (C3CompareVertex( pVtxA[2], pVtxB[0] )) && (C3CompareVertex( pVtxA[3], pVtxB[3])) )            
    {
        join      = 5;
    }

    // (A2,B2) (A3,B1) 
    else if( (C3CompareVertex( pVtxA[2], pVtxB[2] )) && (C3CompareVertex( pVtxA[3], pVtxB[1])) )        
    {
        join      = 6;
    }

    // (A1,B3) (A2,B2) 
    else if( (C3CompareVertex( pVtxA[1], pVtxB[3] )) && (C3CompareVertex( pVtxA[2], pVtxB[2])) )                
    {
        join      = 7;
    }

    // (A1,B1) (A2,B0)
    else if( (C3CompareVertex( pVtxA[1], pVtxB[1] )) && (C3CompareVertex( pVtxA[2], pVtxB[0])) )                    
    {
        join      = 8;
    }

    else // couldn't combine to make a longer fan
    {
        return C3_FALSE;
    }


    // update pGeo's stats
    if( pQuadA->type == C3_PRIM_FAN )
    {
        ;   // do nothing
    }
    else if( pQuadA->type == C3_PRIM_QUAD )
    {
        pGeo->stats->numQuad--;
        pGeo->stats->numFan++;
    }
    if( pQuadB->type == C3_PRIM_FAN )
    {
        pGeo->stats->numFan --;
    }
    else if( pQuadB->type == C3_PRIM_QUAD )
    {
        pGeo->stats->numQuad--;
    }


    // remove both quads' original vertices and replace pQuadA's with a new list
    DSRemoveListObject( &pQuadA->vertexPtrList, (void*)pLinkA[0] );
    DSRemoveListObject( &pQuadA->vertexPtrList, (void*)pLinkA[1] );
    DSRemoveListObject( &pQuadA->vertexPtrList, (void*)pLinkA[2] );
    DSRemoveListObject( &pQuadA->vertexPtrList, (void*)pLinkA[3] );
    pQuadA->vertexPtrList.Head = pQuadA->vertexPtrList.Tail = NULL;

    DSRemoveListObject( &pQuadB->vertexPtrList, (void*)pLinkB[0] );
    DSRemoveListObject( &pQuadB->vertexPtrList, (void*)pLinkB[1] );
    DSRemoveListObject( &pQuadB->vertexPtrList, (void*)pLinkB[2] );
    DSRemoveListObject( &pQuadB->vertexPtrList, (void*)pLinkB[3] );
    pQuadB->vertexPtrList.Head = pQuadB->vertexPtrList.Tail = NULL;

    // remove the pQuadB primitive from pGeo;
    DSRemoveListObject( &pGeo->primitiveList, (void*)pQuadB );

    // convert the pQuadA into a fan;
    pQuadA->type       = C3_PRIM_FAN;  
    pQuadA->numVertex  = 6;


    // make the new fan from pQuadA
    switch( join )
    {

    case 1:             // (A0,B2) (A1,B1)      

        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[2] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[3] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[0] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[1] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[2] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[3] ) );
        break;
        
    case 2:             // (A0,B0) (A1,B3)      

        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[0] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[1] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[2] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[3] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[2] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[3] ) );
        break;

    case 3:             // (A0,B0) (A3,B1)      

        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[0] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[1] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[2] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[3] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[2] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[3] ) );
        break;

    case 4:             // (A0,B2) (A3,B3)      

        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[0] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[1] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[2] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[3] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[0] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[1] ) );
        break;

    case 5:             // (A2,B0) (A3,B3)      

        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[0] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[1] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[2] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[3] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[0] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[1] ) );
        break;
        
    case 6:             // (A2,B2) (A3,B1)      

        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[2] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[3] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[0] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[1] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[0] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[1] ) );
        break;
        
    case 7:             // (A1,B3) (A2,B2)      

        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[2] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[3] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[0] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[1] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[0] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[1] ) );
        break;
        
    case 8:             // (A1,B1) (A2,B0)      

        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[2] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[3] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[0] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkA[1] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[2] ) );
        DSInsertListObject( &pQuadA->vertexPtrList, NULL, (void*)( pLinkB[3] ) );
        break;

    } // end switch( join )


    return C3_TRUE;
}

//-------------------------------------------------------------------------------------------------

// convert all quad primitives into fans
static void C3FansFromGeoQuads( C3GeomObject* pGeo )
{
    C3Primitive* pPrim;


    pPrim = (C3Primitive*)pGeo->primitiveList.Head;
    while( pPrim )
    {
        if( pPrim->type == C3_PRIM_QUAD )
        {
            pPrim->type = C3_PRIM_FAN;

            pGeo->stats->numQuad--;
            pGeo->stats->numFan++;
        }

        pPrim = (C3Primitive*)pPrim->link.Next;
    }
}

//-------------------------------------------------------------------------------------------------

static void C3JoinGeoFans( C3GeomObject* pGeo )
{
    DSList       triList, quadList, fanList, otherList;
    C3Primitive  primTmp;
    C3Primitive* primBase;


    DSInitList( &triList,   (void*)(&primTmp), &primTmp.link );
    DSInitList( &quadList,  (void*)(&primTmp), &primTmp.link );
    DSInitList( &fanList,   (void*)(&primTmp), &primTmp.link );
    DSInitList( &otherList, (void*)(&primTmp), &primTmp.link );


    // partition pGeo->primitiveList into sub-lists
    // by primitive type.
    primBase = (C3Primitive*)(pGeo->primitiveList.Head);
    while( primBase )
    {
        DSRemoveListObject( &pGeo->primitiveList, (void*)primBase );

        switch( primBase->type )
        {
        case C3_PRIM_TRI:
            DSInsertListObject( &triList, NULL, (void*)primBase );
            break;

        case C3_PRIM_QUAD:
            DSInsertListObject( &quadList, NULL, (void*)primBase );
            break;

        case C3_PRIM_FAN:
            DSInsertListObject( &fanList, NULL, (void*)primBase );
            break;

        default:  // any other primitive type is placed in the 'miscellaneous' list
            DSInsertListObject( &otherList, NULL, (void*)primBase );
            break;
        }
        primBase = (C3Primitive*)(pGeo->primitiveList.Head);

    } // end while( primBase )


    // combine the sub-lists into a single 'fan' list.
    // note the order: placing quads first ensures that
    // quads-to-fan combination will happen first-
    // this seems to improve performance.
    // do a pass on the combined sub-lists
    pGeo->primitiveList.Head = NULL;
    pGeo->primitiveList.Tail = NULL;

    primBase = (C3Primitive*)(quadList.Head);
    while( primBase )
    {
        DSRemoveListObject( &quadList, (void*)primBase );
        DSInsertListObject( &pGeo->primitiveList, NULL, (void*)primBase );
        primBase = (C3Primitive*)(quadList.Head);
    }

    primBase = (C3Primitive*)(fanList.Head);
    while( primBase )
    {
        DSRemoveListObject( &fanList, (void*)primBase );
        DSInsertListObject( &pGeo->primitiveList, NULL, (void*)primBase );
        primBase = (C3Primitive*)(fanList.Head);
    }

    primBase = (C3Primitive*)(triList.Head);
    while( primBase )
    {
        DSRemoveListObject( &triList, (void*)primBase );
        DSInsertListObject( &pGeo->primitiveList, NULL, (void*)primBase );
        primBase = (C3Primitive*)(triList.Head);
    }

    C3MakeFans( pGeo );


    // re-attach the 'other' primitives to pGeo
    primBase = (C3Primitive*)(otherList.Head);
    while( primBase )
    {
        DSRemoveListObject( &otherList, (void*)primBase );
        DSInsertListObject( &pGeo->primitiveList, NULL, (void*)primBase );
        primBase = (C3Primitive*)(otherList.Head);
    }

}

//-------------------------------------------------------------------------------------

static void C3MakeFans( C3GeomObject* pGeo )
{
    C3Primitive*  pPrim1, *pPrim2;
    C3Bool        newFan  = C3_FALSE;
    C3Bool        anyFan  = C3_FALSE;

    C3Primitive   primTmp;
    DSList        fanList, otherList;

    u8 p1numVtx, p2numVtx;

    static C3Bool (*fanFn[3][3])( C3GeomObject* pGeo, C3Primitive* pPrim1, C3Primitive* pPrim2 )

                                = {   C3FanFromTriPair,   C3AddTri2Quad,    C3AddTri2Fan,
                                      C3AddTri2Quad,      C3TwoQuads2Fan,   C3AddQuad2Fan,
                                      C3AddTri2Fan,       C3AddTri2Quad,    C3AddFan2Fan    };


    DSInitList( &fanList,   (void*)&primTmp, &primTmp.link );
    DSInitList( &otherList, (void*)&primTmp, &primTmp.link );


    pPrim1 = (C3Primitive*)( pGeo->primitiveList.Head );
    while( pPrim1 )
    {
        DSRemoveListObject( &pGeo->primitiveList, (void*)pPrim1 );

        p1numVtx = (pPrim1->numVertex - 3);
        if( p1numVtx > 2 )
            p1numVtx = 2;

        newFan  = C3_FALSE;
        anyFan  = C3_FALSE;

        pPrim2  = (C3Primitive*)( pGeo->primitiveList.Head );
        while( pPrim2 )
        {

            p2numVtx = (pPrim2->numVertex - 3);
            if( p2numVtx > 2 )
                p2numVtx = 2;

            newFan = ( fanFn[p1numVtx][p2numVtx] )( pGeo, pPrim1, pPrim2 );

    
            if( newFan == C3_TRUE ) // pPrim2 will be gone from pGeo
            {
                anyFan  = C3_TRUE;
                pPrim2  = (C3Primitive*)( pGeo->primitiveList.Head );
                continue;
            }

            pPrim2 = (C3Primitive*)pPrim2->link.Next;

        } // end while( pPrim2 )


        if( anyFan == C3_TRUE )
        {
            DSInsertListObject( &fanList, NULL, (void*)pPrim1 );
        }
        else
        {
            DSInsertListObject( &otherList, NULL, (void*)pPrim1 );
        }

        pPrim1 = (C3Primitive*)( pGeo->primitiveList.Head );

    } // end while( pPrim1 )


    pGeo->primitiveList.Head = pGeo->primitiveList.Tail = NULL;
    DSAttachList( &pGeo->primitiveList, &fanList );
    DSAttachList( &pGeo->primitiveList, &otherList );

}

//-------------------------------------------------------------------------------------------------

// pFanA is already gone from pGeo->primitiveList;
// pFanB must be removed on success
static C3Bool C3AddFan2Fan( C3GeomObject* pGeo, C3Primitive* pFanA, C3Primitive* pFanB )
{
    C3PtrLink*   pLinkA[3];
    C3PtrLink*   pLinkB[3];
    C3Vertex*    pVtxA[3];
    C3Vertex*    pVtxB[3];
    DSList       pTmp       = { 0, NULL, NULL };



    if( (pFanA->type != C3_PRIM_FAN) || (pFanB->type != C3_PRIM_FAN) )
    {
        return C3_FALSE;
    }

    if( (pFanA->numVertex < 5) || (pFanB->numVertex < 5) )
    {
        return C3_FALSE;
    }

    // make sure that primitives have same texture
    if( !C3ComparePrimAttr( pFanA, pFanB ) )
    {
        return C3_FALSE;
    }

    // max. fan length is MAX_FAN_VTX vertices
    if( (pFanA->numVertex + pFanB->numVertex - 2) > MAX_FAN_VTX )
    {
        return C3_FALSE;
    }

    // get the common, second and last vertices of each fan
    pLinkA[0] = (C3PtrLink*)pFanA->vertexPtrList.Head;
    pLinkA[1] = (C3PtrLink*)pLinkA[0]->link.Next;
    pLinkA[2] = (C3PtrLink*)pFanA->vertexPtrList.Tail;

    pLinkB[0] = (C3PtrLink*)pFanB->vertexPtrList.Head;
    pLinkB[1] = (C3PtrLink*)pLinkB[0]->link.Next;
    pLinkB[2] = (C3PtrLink*)pFanB->vertexPtrList.Tail;

    pVtxA[0] = (C3Vertex*)pLinkA[0]->ptr;
    pVtxA[1] = (C3Vertex*)pLinkA[1]->ptr;
    pVtxA[2] = (C3Vertex*)pLinkA[2]->ptr;

    pVtxB[0] = (C3Vertex*)pLinkB[0]->ptr;
    pVtxB[1] = (C3Vertex*)pLinkB[1]->ptr;
    pVtxB[2] = (C3Vertex*)pLinkB[2]->ptr;


    // fans must share a common 1st vertex.
    if( (C3CompareVertex( pVtxA[0], pVtxB[0] )) == C3_FALSE )
    {
        return C3_FALSE;
    }


    // there are two cases where fans can be joined:
    if( (C3CompareVertex( pVtxA[1], pVtxB[2] )) == C3_TRUE )        // head of A to tail of B           
    {
        DSRemoveListObject( &pFanA->vertexPtrList, (void*)pLinkA[0] );
        DSRemoveListObject( &pFanA->vertexPtrList, (void*)pLinkA[1] );

        pTmp.Offset = pFanA->vertexPtrList.Offset;
        pTmp.Head   = pFanA->vertexPtrList.Head;
        pTmp.Tail   = pFanA->vertexPtrList.Tail;

        pFanA->vertexPtrList.Head = pFanA->vertexPtrList.Tail = NULL;
        DSAttachList( &pFanA->vertexPtrList, &pFanB->vertexPtrList );
        DSAttachList( &pFanA->vertexPtrList, &pTmp );

    }
    else if( (C3CompareVertex( pVtxA[2], pVtxB[1] )) == C3_TRUE )   // tail of A to head of B           
    {
        DSRemoveListObject( &pFanB->vertexPtrList, (void*)pLinkB[0] );
        DSRemoveListObject( &pFanB->vertexPtrList, (void*)pLinkB[1] );
        DSAttachList( &pFanA->vertexPtrList, &pFanB->vertexPtrList );
    }
    else
    {
        return C3_FALSE;
    }

    // update pGeo's stats
    pGeo->stats->numFan--; 

    // update the length of the new fan;
    pFanA->numVertex  += (pFanB->numVertex - 2);

    // remove the 'pFanB' primitive from pGeo;
    DSRemoveListObject( &pGeo->primitiveList, (void*)pFanB );

    return C3_TRUE;
}

//------------------------------------------------------------------------------------------

// this function is meant to add a triangle to a fan of min. length 3
static C3Bool C3AddTri2Fan( C3GeomObject* pGeo, C3Primitive* pFanA, C3Primitive* pFanB )
{
    C3Primitive* pFan,      *pTri;
    C3PtrLink*   pLinkF[3], *pLinkT[3];
    C3Vertex*    pVtxF[3],  *pVtxT[3];
    u32 join     = 0;


    if( (pFanA->type == C3_PRIM_TRI) && (pFanB->type == C3_PRIM_FAN) )
    {
        pTri = pFanA;
        pFan = pFanB;
    }
    else if( (pFanB->type == C3_PRIM_TRI) && (pFanA->type == C3_PRIM_FAN) )
    {
        pTri = pFanB;
        pFan = pFanA;
    }
    else
    {
        return C3_FALSE;
    }

    // make sure that primitives have same texture
    if( !C3ComparePrimAttr( pFanA, pFanB ) )
    {
        return C3_FALSE;
    }

    // min. fan length is 3 triangles; max # vtx after tri addition is MAX_FAN_VTX
    if( (pFan->numVertex < 5) || ( (pFan->numVertex + 1) > MAX_FAN_VTX ) )
    {
        return C3_FALSE;
    }


     pLinkF[0] = (C3PtrLink*)pFan->vertexPtrList.Head;
     pLinkF[1] = (C3PtrLink*)pLinkF[0]->link.Next;
     pLinkF[2] = (C3PtrLink*)pFan->vertexPtrList.Tail;

     pLinkT[0] = (C3PtrLink*)pTri->vertexPtrList.Head;
     pLinkT[1] = (C3PtrLink*)pLinkT[0]->link.Next;
     pLinkT[2] = (C3PtrLink*)pLinkT[1]->link.Next;


     pVtxF[0] = (C3Vertex*)pLinkF[0]->ptr;
     pVtxF[1] = (C3Vertex*)pLinkF[1]->ptr;
     pVtxF[2] = (C3Vertex*)pLinkF[2]->ptr;

     pVtxT[0] = (C3Vertex*)pLinkT[0]->ptr;
     pVtxT[1] = (C3Vertex*)pLinkT[1]->ptr;
     pVtxT[2] = (C3Vertex*)pLinkT[2]->ptr;


     // tri to head of fan
     if( (C3CompareVertex( pVtxF[0], pVtxT[0])) && (C3CompareVertex( pVtxF[1], pVtxT[2])) )
     {
         DSRemoveListObject( &pTri->vertexPtrList, (void*)pLinkT[1] );
         DSInsertListObject( &pFan->vertexPtrList, (void*)pLinkF[1], (void*)pLinkT[1] );
     }
     else if( (C3CompareVertex( pVtxF[0], pVtxT[1])) && (C3CompareVertex( pVtxF[1], pVtxT[0])) )
     {
        DSRemoveListObject( &pTri->vertexPtrList, (void*)pLinkT[2] );
        DSInsertListObject( &pFan->vertexPtrList, (void*)pLinkF[1], (void*)pLinkT[2] );
     }
     else if( (C3CompareVertex( pVtxF[0], pVtxT[2])) && (C3CompareVertex( pVtxF[1], pVtxT[1])) )
     {
        DSRemoveListObject( &pTri->vertexPtrList, (void*)pLinkT[0] );
        DSInsertListObject( &pFan->vertexPtrList, (void*)pLinkF[1], (void*)pLinkT[0] );
     }
     // tri to tail of fan
     else if( (C3CompareVertex( pVtxF[0], pVtxT[0])) && (C3CompareVertex( pVtxF[2], pVtxT[1])) )
     {
        DSRemoveListObject( &pTri->vertexPtrList, (void*)pLinkT[2] );
        DSInsertListObject( &pFan->vertexPtrList, NULL, (void*)pLinkT[2] );
     }
     else if( (C3CompareVertex( pVtxF[0], pVtxT[1])) && (C3CompareVertex( pVtxF[2], pVtxT[2])) )
     {
        DSRemoveListObject( &pTri->vertexPtrList, (void*)pLinkT[0] );
        DSInsertListObject( &pFan->vertexPtrList, NULL, (void*)pLinkT[0] );
     }
     else if( (C3CompareVertex( pVtxF[0], pVtxT[2])) && (C3CompareVertex( pVtxF[2], pVtxT[0])) )
     {
        DSRemoveListObject( &pTri->vertexPtrList, (void*)pLinkT[1] );
        DSInsertListObject( &pFan->vertexPtrList, NULL, (void*)pLinkT[1] );
     }
     else
     {
         return C3_FALSE;
     }

     // update pGeo stats
    pGeo->stats->numTri--;

    // update the length of the new fan;
    pFan->numVertex++;

    // pFanB will be removed, so if it was the fan, swap it for pFanA
    if( pTri == pFanA )
    {
        pFanA->type = C3_PRIM_FAN;
        pFanA->vertexPtrList.Head = pFanA->vertexPtrList.Tail = NULL;
        DSAttachList( &pFanA->vertexPtrList, &pFanB->vertexPtrList );
    }

    // remove the triangle primitive from pGeo;
    DSRemoveListObject( &pGeo->primitiveList, (void*)pFanB );

    return C3_TRUE;
}

//------------------------------------------------------------------------------------------

// pTriA is already gone from pGeo->primitiveList;
// pTriB must be removed on success
static C3Bool C3FanFromTriPair( C3GeomObject* pGeo, C3Primitive* pTriA, C3Primitive* pTriB )
{


    if( (pTriA->type != C3_PRIM_TRI) || (pTriB->type != C3_PRIM_TRI) )
    {
        return C3_FALSE;
    }

    // C3CreateQuad will decrement numTri by 2, increment numQuad by 1,
    // and change pTriA into type C3_PRIM_QUAD
    if( C3ComparePrimAttr( pTriA, pTriB ) && C3CreateQuad( pTriA, pTriB ) )
    {
        DSRemoveListObject( &pGeo->primitiveList, (void*)pTriB );

        pTriA->type = C3_PRIM_FAN;
        pGeo->stats->numQuad--;
        pGeo->stats->numFan++;
        return C3_TRUE;
    }

    return C3_FALSE;
}

//------------------------------------------------------------------------------------------

static C3Bool C3AddQuad2Fan( C3GeomObject* pGeo, C3Primitive* pA, C3Primitive* pB )
{
    C3Primitive* pFan,      *pQuad;
    C3PtrLink*   pLinkF[3], *pLinkQ[4];
    C3Vertex*    pVtxF[3],  *pVtxQ[4];
    u32 join     = 0;


    if( (pA->type == C3_PRIM_FAN) && (pB->type == C3_PRIM_QUAD) )
    {
        pFan  = pA;
        pQuad = pB;
    }
    else if( (pA->type == C3_PRIM_FAN) && ( (pB->type == C3_PRIM_FAN) && (pB->numVertex == 4) ) )
    {
        pFan  = pA;
        pQuad = pB;
    }

    else if( (pB->type == C3_PRIM_FAN) && (pA->type == C3_PRIM_QUAD) )
    {
        pFan  = pB;
        pQuad = pA;
    }
    else if( (pB->type == C3_PRIM_FAN) && ( (pA->type == C3_PRIM_FAN) && (pA->numVertex == 4) ) )
    {
        pFan  = pB;
        pQuad = pA;
    }
    else
    {
        return C3_FALSE;
    }

    // make sure that primitives have same texture
    if( !C3ComparePrimAttr( pA, pB ) )
    {
        return C3_FALSE;
    }

    // min. fan length is 3 tri; max # vtx after addition is MAX_FAN_VTX
    if( (pFan->numVertex < 5) || ( (pFan->numVertex + 2) > MAX_FAN_VTX ) )
    {
        return C3_FALSE;
    }


     pLinkF[0] = (C3PtrLink*)pFan->vertexPtrList.Head;
     pLinkF[1] = (C3PtrLink*)pLinkF[0]->link.Next;
     pLinkF[2] = (C3PtrLink*)pFan->vertexPtrList.Tail;

     pLinkQ[0] = (C3PtrLink*)pQuad->vertexPtrList.Head;
     pLinkQ[1] = (C3PtrLink*)pLinkQ[0]->link.Next;
     pLinkQ[2] = (C3PtrLink*)pLinkQ[1]->link.Next;
     pLinkQ[3] = (C3PtrLink*)pLinkQ[2]->link.Next;


     pVtxF[0] = (C3Vertex*)pLinkF[0]->ptr;
     pVtxF[1] = (C3Vertex*)pLinkF[1]->ptr;
     pVtxF[2] = (C3Vertex*)pLinkF[2]->ptr;

     pVtxQ[0] = (C3Vertex*)pLinkQ[0]->ptr;
     pVtxQ[1] = (C3Vertex*)pLinkQ[1]->ptr;
     pVtxQ[2] = (C3Vertex*)pLinkQ[2]->ptr;
     pVtxQ[3] = (C3Vertex*)pLinkQ[3]->ptr;


     // 4 ways to add a quad:
     if( (C3CompareVertex( pVtxF[0], pVtxQ[0])) && (C3CompareVertex( pVtxF[1], pVtxQ[3])) )
     {
         DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[1] );
         DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[2] );

         DSInsertListObject( &pFan->vertexPtrList, (void*)pLinkF[1], (void*)pLinkQ[1] );
         DSInsertListObject( &pFan->vertexPtrList, (void*)pLinkF[1], (void*)pLinkQ[2] );
     }
     else if( (C3CompareVertex( pVtxF[0], pVtxQ[2])) && (C3CompareVertex( pVtxF[1], pVtxQ[1])) )
     {
         DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[3] );
         DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[0] );

         DSInsertListObject( &pFan->vertexPtrList, (void*)pLinkF[1], (void*)pLinkQ[3] );
         DSInsertListObject( &pFan->vertexPtrList, (void*)pLinkF[1], (void*)pLinkQ[0] );
     }
     else if( (C3CompareVertex( pVtxF[0], pVtxQ[0])) && (C3CompareVertex( pVtxF[2], pVtxQ[1])) )
     {
         DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[2] );
         DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[3] );

         DSInsertListObject( &pFan->vertexPtrList, NULL, (void*)pLinkQ[2] );
         DSInsertListObject( &pFan->vertexPtrList, NULL, (void*)pLinkQ[3] );
     }
     else if( (C3CompareVertex( pVtxF[0], pVtxQ[2])) && (C3CompareVertex( pVtxF[2], pVtxQ[3])) )
     {
         DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[0] );
         DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[1] );

         DSInsertListObject( &pFan->vertexPtrList, NULL, (void*)pLinkQ[0] );
         DSInsertListObject( &pFan->vertexPtrList, NULL, (void*)pLinkQ[1] );
     }
     else
     {
         return C3_FALSE;
     }

     // update pGeo stats
     if( pQuad->type == C3_PRIM_QUAD )
     {
        pGeo->stats->numQuad--;
     }
     else if( pQuad->type == C3_PRIM_FAN )
     {
         pGeo->stats->numFan--;
     }

    // update the length of the other fan;
    pFan->numVertex  += 2;

    if( pA == pQuad )
    {
        pA->type = C3_PRIM_FAN;
        pA->numVertex = pFan->numVertex;
        pA->vertexPtrList.Head = pA->vertexPtrList.Tail = NULL;
        DSAttachList( &pA->vertexPtrList, &pB->vertexPtrList );
    }

    // remove the quad primitive from pGeo;
    DSRemoveListObject( &pGeo->primitiveList, (void*)pB );


    return C3_TRUE;
}

//------------------------------------------------------------------------------------------

static C3Bool C3AddTri2Quad( C3GeomObject* pGeo, C3Primitive* pA, C3Primitive* pB )
{
    C3Primitive* pTri,      *pQuad;
    C3PtrLink*   pLinkT[3], *pLinkQ[4];
    C3Vertex*    pVtxT[3],  *pVtxQ[4];


    // TODO: Get rid of this if we know MAX_FAN_VTX will always be greater
    // than or equal to 5.
    if( MAX_FAN_VTX < 5 )
    {
        return C3_FALSE;
    }

    if( (pA->type == C3_PRIM_TRI) && ( (pB->type == C3_PRIM_QUAD) || ((pB->type == C3_PRIM_FAN) && (pB->numVertex == 4)) ) )
    {
        pTri  = pA;
        pQuad = pB;
    }
    else if( (pB->type == C3_PRIM_TRI) && ( (pA->type == C3_PRIM_QUAD) || ((pA->type == C3_PRIM_FAN) && (pA->numVertex == 4)) ) )
    {
        pTri  = pB;
        pQuad = pA;
    }
    else
    {
        return C3_FALSE;
    }

    // make sure that primitives have same texture
    if( !C3ComparePrimAttr( pA, pB ) )
    {
        return C3_FALSE;
    }

    pLinkT[0] = (C3PtrLink*)pTri->vertexPtrList.Head;
    pLinkT[1] = (C3PtrLink*)pLinkT[0]->link.Next;
    pLinkT[2] = (C3PtrLink*)pLinkT[1]->link.Next;

    pLinkQ[0] = (C3PtrLink*)pQuad->vertexPtrList.Head;
    pLinkQ[1] = (C3PtrLink*)pLinkQ[0]->link.Next;
    pLinkQ[2] = (C3PtrLink*)pLinkQ[1]->link.Next;
    pLinkQ[3] = (C3PtrLink*)pLinkQ[2]->link.Next;

    pVtxT[0] = (C3Vertex*)pLinkT[0]->ptr;
    pVtxT[1] = (C3Vertex*)pLinkT[1]->ptr;
    pVtxT[2] = (C3Vertex*)pLinkT[2]->ptr;

    pVtxQ[0] = (C3Vertex*)pLinkQ[0]->ptr;
    pVtxQ[1] = (C3Vertex*)pLinkQ[1]->ptr;
    pVtxQ[2] = (C3Vertex*)pLinkQ[2]->ptr;
    pVtxQ[3] = (C3Vertex*)pLinkQ[3]->ptr;


    // 12 cases for triangle to quad match:

    // triangle to quad 0,1
    if( (C3CompareVertex( pVtxT[0], pVtxQ[0] )) && (C3CompareVertex( pVtxT[2], pVtxQ[1] )) )
    {
        DSRemoveListObject( &pTri->vertexPtrList, (void*)pLinkT[1] );
        DSInsertListObject( &pQuad->vertexPtrList, (void*)pLinkQ[1], (void*)pLinkT[1] );
    }
    else if( (C3CompareVertex( pVtxT[2], pVtxQ[0] )) && (C3CompareVertex( pVtxT[1], pVtxQ[1] )) )
    {
        DSRemoveListObject( &pTri->vertexPtrList, (void*)pLinkT[0] );
        DSInsertListObject( &pQuad->vertexPtrList, (void*)pLinkQ[1], (void*)pLinkT[0] );
    }
    else if( (C3CompareVertex( pVtxT[1], pVtxQ[0] )) && (C3CompareVertex( pVtxT[0], pVtxQ[1] )) )
    {
        DSRemoveListObject( &pTri->vertexPtrList, (void*)pLinkT[2] );
        DSInsertListObject( &pQuad->vertexPtrList, (void*)pLinkQ[1], (void*)pLinkT[2] );
    }

    // triangle to quad 0,3
    else if( (C3CompareVertex( pVtxT[0], pVtxQ[0] )) && (C3CompareVertex( pVtxT[1], pVtxQ[3] )) )
    {
        DSRemoveListObject( &pTri->vertexPtrList, (void*)pLinkT[2] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkT[2] );
    }
    else if( (C3CompareVertex( pVtxT[2], pVtxQ[0] )) && (C3CompareVertex( pVtxT[0], pVtxQ[3] )) )
    {
        DSRemoveListObject( &pTri->vertexPtrList, (void*)pLinkT[1] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkT[1] );
    }
    else if( (C3CompareVertex( pVtxT[1], pVtxQ[0] )) && (C3CompareVertex( pVtxT[2], pVtxQ[3] )) )
    {
        DSRemoveListObject( &pTri->vertexPtrList, (void*)pLinkT[0] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkT[0] );
    }

    // triangle to quad 2,1
    else if( (C3CompareVertex( pVtxT[0], pVtxQ[2] )) && (C3CompareVertex( pVtxT[1], pVtxQ[1] )) )
    {
        DSRemoveListObject( &pTri->vertexPtrList, (void*)pLinkT[2] );

        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[0] );
        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[1] );
        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[2] );
        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[3] );
        pQuad->vertexPtrList.Head = pQuad->vertexPtrList.Tail = NULL;

        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[2] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[3] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[0] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[1] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkT[2] );
    }
    else if( (C3CompareVertex( pVtxT[2], pVtxQ[2] )) && (C3CompareVertex( pVtxT[0], pVtxQ[1] )) )
    {
        DSRemoveListObject( &pTri->vertexPtrList, (void*)pLinkT[1] );

        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[0] );
        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[1] );
        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[2] );
        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[3] );
        pQuad->vertexPtrList.Head = pQuad->vertexPtrList.Tail = NULL;

        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[2] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[3] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[0] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[1] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkT[1] );
    }
    else if( (C3CompareVertex( pVtxT[1], pVtxQ[2] )) && (C3CompareVertex( pVtxT[2], pVtxQ[1] )) )
    {
        DSRemoveListObject( &pTri->vertexPtrList, (void*)pLinkT[0] );

        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[0] );
        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[1] );
        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[2] );
        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[3] );
        pQuad->vertexPtrList.Head = pQuad->vertexPtrList.Tail = NULL;

        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[2] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[3] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[0] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[1] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkT[0] );
    }
    
    // triangle to quad 2,3
    else if( (C3CompareVertex( pVtxT[0], pVtxQ[2] )) && (C3CompareVertex( pVtxT[2], pVtxQ[3] )) )
    {
        DSRemoveListObject( &pTri->vertexPtrList, (void*)pLinkT[1] );

        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[0] );
        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[1] );
        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[2] );
        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[3] );
        pQuad->vertexPtrList.Head = pQuad->vertexPtrList.Tail = NULL;

        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[2] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkT[1] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[3] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[0] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[1] );
    }
    else if( (C3CompareVertex( pVtxT[2], pVtxQ[2] )) && (C3CompareVertex( pVtxT[1], pVtxQ[3] )) )
    {
        DSRemoveListObject( &pTri->vertexPtrList, (void*)pLinkT[0] );

        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[0] );
        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[1] );
        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[2] );
        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[3] );
        pQuad->vertexPtrList.Head = pQuad->vertexPtrList.Tail = NULL;

        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[2] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkT[0] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[3] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[0] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[1] );
    }
    else if( (C3CompareVertex( pVtxT[1], pVtxQ[2] )) && (C3CompareVertex( pVtxT[0], pVtxQ[3] )) )
    {
        DSRemoveListObject( &pTri->vertexPtrList, (void*)pLinkT[2] );

        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[0] );
        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[1] );
        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[2] );
        DSRemoveListObject( &pQuad->vertexPtrList, (void*)pLinkQ[3] );
        pQuad->vertexPtrList.Head = pQuad->vertexPtrList.Tail = NULL;

        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[2] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkT[2] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[3] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[0] );
        DSInsertListObject( &pQuad->vertexPtrList, NULL, (void*)pLinkQ[1] );
    }
    else
    {
        return C3_FALSE;
    }


     // update pGeo stats
     if( pQuad->type == C3_PRIM_QUAD )
     {
        pQuad->type = C3_PRIM_FAN;
        pGeo->stats->numQuad--;
        pGeo->stats->numFan++;
     }
     pGeo->stats->numTri --;

    // update the length of the new fan;
    pQuad->numVertex  ++;

    if( pA == pTri )
    {
        pA->type = C3_PRIM_FAN;
        pA->numVertex = pQuad->numVertex;
        pA->vertexPtrList.Head = pA->vertexPtrList.Tail = NULL;
        DSAttachList( &pA->vertexPtrList, &pB->vertexPtrList );
    }

    // remove the triangle primitive from pGeo;
    DSRemoveListObject( &pGeo->primitiveList, (void*)pB );


    return C3_TRUE;
}

//------------------------------------------------------------------------------------------

// convert all fans of length 2 into quads
static void   C3ShortGeoFansToQuads( C3GeomObject* pGeo )
{
    C3Primitive* pPrim;


    pPrim = (C3Primitive*)pGeo->primitiveList.Head;
    while( pPrim )
    {
        if( (pPrim->type == C3_PRIM_FAN) && (pPrim->numVertex == 4) )
        {
            pPrim->type = C3_PRIM_QUAD;

            pGeo->stats->numQuad++;
            pGeo->stats->numFan--;
        }

        pPrim = (C3Primitive*)pPrim->link.Next;
    }
}

//------------------------------------------------------------------------------------------

// In order for this function to work properly, colors cannot be compressed.

// give each primitive a distinct color
// color triangles red, quads blue, lines black
// and other multi-triangle primitives an arbitrary color
static void C3ColorVerticesByPrim( C3GeomObject* pGeo )
{
    C3Primitive* pPrim;
    C3PtrLink*   pLink;
    C3Vertex*    pVtx;
    C3Color*     pClr;
    s32          upper, lower, range;
    u8           rClr,  gClr,  bClr;



    // range for vertex colors
    upper = 240;
    lower = 30;
    range = upper - lower;


    pPrim = (C3Primitive*)pGeo->primitiveList.Head;
    while( pPrim )
    {
        rClr = (u8)( lower + ( rand() % range ) );
        gClr = (u8)( lower + ( rand() % range ) );
        bClr = (u8)( lower + ( rand() % range ) );

        pLink = (C3PtrLink*)pPrim->vertexPtrList.Head;
        while( pLink )
        {
            pVtx = (C3Vertex*)pLink->ptr;
            pClr = (C3Color*)pVtx->color;


            switch( pPrim->type )
            {

            case C3_PRIM_TRI:       // triangles are red
                pClr->r = 255;
                pClr->g =   0;
                pClr->b =   0;
                pClr->a = 255;
                break;

            case C3_PRIM_QUAD:      // quads are blue
                pClr->r =      0;
                pClr->g =      0;
                pClr->b =    255;
                pClr->a =    255;
                break;

            case C3_PRIM_STRIP:     // strips are random colors
                  
                pClr->r = rClr;
                pClr->g = gClr;
                pClr->b = bClr;
                pClr->a =  255;
                break;

            case C3_PRIM_FAN:       // fans are random colors

                if( pPrim->numVertex == 4 ) // 2-tri fans are treated as quads
                {
                    pClr->r =      0;
                    pClr->g =      0;
                    pClr->b =    255;
                    pClr->a =    255;
                }
                else
                {
                    pClr->r = rClr;
                    pClr->g = gClr;
                    pClr->b = bClr;
                    pClr->a =  255;
                }
                break;

            case C3_PRIM_LINE_STRIP: // the wireframe retains its color
                break;

            default:                // other primitive types are black
                pClr->r =      0;
                pClr->g =      0;
                pClr->b =      0;
                pClr->a =    255;
                break;

            } // end switch( pPrim->type )

            pLink = (C3PtrLink*)pLink->link.Next;

        } // end while( pLink )

        pPrim = (C3Primitive*)pPrim->link.Next;
    } // end while( pPrim )

}

//------------------------------------------------------------------------------------------

