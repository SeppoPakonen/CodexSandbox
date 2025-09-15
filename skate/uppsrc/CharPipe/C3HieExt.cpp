/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3HieExt.c]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/c3/src/C3HieExt.c $
    
    14    11/13/00 1:44p John
    C3BeginHierarchy/C3EndHierarchy now have been tested with C3SetParent,
    so parents can be set arbitrarily rather than being limited to
    recursive extraction to set parenting information.
    Hierarchy transformation is no longer a mtx in the layout, but a
    CTRLControl structure of translation, rotation, and scale.  So
    C3SetMatrix is replaced in favor of C3SetHierControl.
    Added C3SetPivotOffset API so that rotation and scale pivot points do
    not have to be the same as the translation.
    
    
    13    8/14/00 6:15p John
    Added user-defined data to ANM, ACT, and GPL formats.
    
    12    7/18/00 7:28p John
    Modified to use new CTRL library.
    Removed C3NameActor function (was commented out).
    
    11    4/06/00 3:27p John
    Needed C3SetParent for reparenting biped tails.  Please see
    ProcessINode and ConvertINodeAnimationKeyedRotation.
    
    10    3/14/00 1:33p John
    Moved display priority from GPL to ACT.
    
    9     2/29/00 7:22p John
    Removed unnecessary geomObject field data.
    
    8     2/18/00 4:55p John
    Renamed C3SetActorMesh to C3SetObjectSkin.
    
    7     2/04/00 6:07p John
    Untabified code.
    
    6     1/31/00 4:15p John
    Added pruning of unnecessary bones and animations.
    Finished stitching with FIFO matrix cache and stripping.
    
    5     1/20/00 1:14p John
    Added stitching (work in progress).
    Added C3 string table for efficient management of strings.
    
    4     12/13/99 3:39p John
    Moved quantization of hierarchy transform to C3WriteControl.
    
    3     12/08/99 7:26p John
    Commented out inheritance code (not fully supported).
    
    2     12/06/99 3:25p John
    Commented out unused/unsupported functions.
    
    12    11/11/99 1:21p John
    Modified code to adjust for new C3Control parameters.
    
    11    10/21/99 6:25p John
    Snaps the translation portion of hierarchy matrix to the position
    fixed grid.
    
    10    10/19/99 2:41p John
    
    9     10/19/99 2:41p John
    Undone node translation grid snapping
    
    8     10/19/99 11:20a John
    Quantizing the translation portion of object relative matrix according
    to position quantization.
    
    7     10/13/99 2:53p John
    Added instancing.
    
    6     6/08/99 1:39p Rod
    Completed all function headers
    
    5     6/02/99 11:28a Rod
    Changed the point/vec members to lower case x, y, z

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#include "C3Private.h"

// Local functions

static void   C3CreateNewActor( char* name );
static void   C3PushHierNode( C3HierNode *hNode );
static void   C3PopHierNode ( void );
static C3Bool C3IsStackEmpty( void );
static void   C3CleanStack( void );
static void   C3ValidateBoneControl( C3ControlPtr *control );

// Local variables
static DSList      C3stack;        // Stack top
static C3Actor     C3currentActor; 
static u16         C3currentID    = 0;

// Global variables
u32   ActUserDataSize = 0;
void *ActUserData     = NULL;

/*---------------------------------------------------------------------------*
  Name:         C3GetActor

  Description:  returns the current actor

  Arguments:    NONE

  Returns:      pointer to the current actor
/*---------------------------------------------------------------------------*/
C3Actor*
C3GetActor( void )
{
    return &C3currentActor;
}


/*---------------------------------------------------------------------------*
  Name:         C3InitHierarchyExtraction

  Description:  Initialize the needed variable and structure for the 
                hierarchy extraction

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3InitHierarchyExtraction ( void )
{
    C3HierNode  hNode;
    C3Control   control;

    C3currentID = 0;

    // set the local variables to default values
    DSInitList( &C3stack, (Ptr)&hNode, &hNode.link );

    // Init the actor
    C3currentActor.identifier         = NULL;
    C3currentActor.numBones           = 0;
    C3currentActor.numControls        = 0;
    C3currentActor.animBank           = NULL;
    C3currentActor.boundingSphere     = NULL;
    C3currentActor.skinMeshIdentifier = NULL;
    C3currentActor.skinIdOffset       = 0;


    // Initialize the lists
    DSInitList( &C3currentActor.boneList, (Ptr)&hNode, &hNode.link );
    DSInitList( &C3currentActor.controlList, (Ptr)&control, &control.link );

    // Initialize the tree
    DSInitTree( &C3currentActor.hierarchy, (Ptr)&hNode, &hNode.branch );
}


/*---------------------------------------------------------------------------*
  Name:         C3CleanHierarchyExtraction

  Description:  Free the memory allocated for the extraction process

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3CleanHierarchyExtraction( void )
{
    C3currentID = 0;
    C3CleanStack();
}


/*---------------------------------------------------------------------------*
  Name:         C3BeginHierarchyNode

  Description:  Start the creation of a new hierarchy node.  Calls to 
                C3BeginHierarchyNode/C3EndHierarchy node can be called 
                recursively, automatically setting parent information.

  Arguments:    name - name of the node

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3BeginHierarchyNode( char* name )
{
    C3HierNode *hNode = NULL;

    C3_ASSERT( name );

    // create a new HierNode
    hNode = C3PoolHierNode();

    // zero the memory (i.e. default pivot offset is 0, 0, 0)
    memset( hNode, 0, sizeof( C3HierNode ) );

    hNode->identifier = C3AddStringInTable( name );
    hNode->id = C3currentID++;

    // set the default inheritance
    hNode->inheritanceFlag = C3_INHERIT_ALL;

    // parent node to the top of stack
    DSInsertBranchBelow( &C3currentActor.hierarchy, (Ptr)C3GetCurrentHierNode(), (Ptr)hNode );

    // add node to database
    DSInsertListObject( &C3currentActor.boneList, NULL, (Ptr)hNode );
    hNode->index = C3currentActor.numBones;
    C3currentActor.numBones++;

    // push the new node on the stack
    C3PushHierNode( hNode );
}


/*---------------------------------------------------------------------------*
  Name:         C3EndHierarchyNode

  Description:  Ends the description of a hierarchy node and inserts it 
                in the hierarchy node database

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3EndHierarchyNode( void )
{
    C3HierNode *hNode = NULL;

    hNode = (C3HierNode*)C3stack.Head;
    // pop the top of the stack
    C3PopHierNode();
}


/*---------------------------------------------------------------------------*
  Name:         C3SetParent

  Description:  Sets the parent of the current node to the specified node.
                Parent should already have been extracted using 
                C3BeginHierarchyNode/C3EndHierarchyNode.

  Arguments:    name - name of the parent

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3SetParent( char* name )
{
    C3HierNode *hNode   = NULL;
    C3HierNode *curNode = NULL;

    // make sure the stack is not empty
    if( !C3IsStackEmpty() )
    {
        // get the pointer from the name in database
        hNode = C3GetHierNodeFromIdentifier( name );
        if( hNode )
        {
            curNode = C3GetCurrentHierNode();
            // Remove the node from any tree it is in
            DSRemoveBranch ( &C3currentActor.hierarchy, (Ptr)curNode );

            // parent the passed node to the current node
            DSInsertBranchBelow ( &C3currentActor.hierarchy, (Ptr)hNode, (Ptr)curNode );
        }
        else
        {
            C3ReportError( "C3SetParent: Could not find parent %s.  Parent should be extracted before this is called", name );
        }
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3AddChild

  Description:  Add a child to the current node

  Arguments:    name - name of the child to add

  Returns:      NONE
/*---------------------------------------------------------------------------*/
/*
void 
C3AddChild( char* name )
{
    C3HierNode *hNode = NULL;

    // make sure the stack is not empty
    if( !C3IsStackEmpty() )
    {
        // get the pointer from the name in database
        hNode = C3GetHierNodeFromIdentifier( name );
        if( hNode )
        {
            // Remove the node from any tree it is in
            DSRemoveBranch ( &C3currentActor.hierarchy, (Ptr)hNode );

            // parent the passed node to the current node
            DSInsertBranchBelow ( &C3currentActor.hierarchy, C3stack.Head, (Ptr)hNode );
        }
    }
}
*/


/*---------------------------------------------------------------------------*
  Name:         C3AddSibling

  Description:  Add a sibling to the current node

  Arguments:    name - name of the sibling

  Returns:      NONE
/*---------------------------------------------------------------------------*/
/*
void 
C3AddSibling( char* name )
{
    C3HierNode *hNode   = NULL;
    C3HierNode *curNode = NULL;


    // make sure the stack is not empty
    if( !C3IsStackEmpty() )
    {
        // get the pointer from the name in database
        hNode = C3GetHierNodeFromIdentifier( name );
        if( hNode )
        {
            curNode = C3GetCurrentHierNode();

            // Remove the node from any tree it is in
            DSRemoveBranch ( &C3currentActor.hierarchy, (Ptr)hNode );

            // parent the passed node to the current node
            DSInsertBranchBeside( &C3currentActor.hierarchy,  (Ptr)curNode, (Ptr)hNode );
        }
    }
}
*/

/*---------------------------------------------------------------------------*
  Name:         C3SetObject

  Description:  Attaches a geometry object to the currect hierarchy node.  
                Must be called between C3BeginHierarchyNode and 
                C3EndHierarchyNode.

  Arguments:    identifier - name of the geometry object to add 
                             (which was passed to C3BeginObject).

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3SetObject( char* identifier )
{
    C3GeomObject *geomObject = NULL;
    C3HierNode   *hNode      = NULL;

    C3_ASSERT( identifier );
    C3_ASSERT( !C3IsStackEmpty() );

    // get the object from the object database  
    geomObject = C3GetObjectFromIdentifier( identifier );
    hNode = C3GetCurrentHierNode();
    C3_ASSERT( hNode );
    hNode->geomObjName = C3AddStringInTable( identifier );
}


/*---------------------------------------------------------------------------*
  Name:         C3SetObjectInNode

  Description:  Attaches a geometry object to the given hierarchy node.
                Similar to C3SetObject, except it can be called outside the
                C3BeginHierarchyNode/C3EndHierarchyNode context.

  Arguments:    geomIdentifier - name of the geometry object to add 
                                 (which was passed to C3BeginObject).
                hierIdentifier - name of the hierarchy node
                                 (which was passed to C3BeginHierarchyNode).

  Returns:      NONE
/*---------------------------------------------------------------------------*/
/* COMMENTED OUT BECAUSE THIS FUNCTION NEVER USED AND NEVER TESTED
void
C3SetObjectInNode( char* geomIdentifier, char* hierIdentifier )
{
    C3GeomObject *geomObject = NULL;
    C3HierNode   *hNode      = NULL;

    C3_ASSERT( geomIdentifier, hierIdentifier );

    geomObject = C3GetObjectFromIdentifier( geomIdentifier );
    hNode = C3GetHierNodeFromIdentifier( hierIdentifier );
    if( hNode )
    {
        hNode->geomObjName = C3AddStringInTable( geomIdentifier );
    }
    else
    {
        C3ReportError( "Could not find hierarchy node %s", hierIdentifier );
    }
}
*/

/*---------------------------------------------------------------------------*
  Name:         C3SetObjectSkin

  Description:  Sets the stitched geometry object given by identifier 
                to the current actor.

  Arguments:    identifier - identifier for the geom obj

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetObjectSkin( char* identifier )
{ 
    C3_ASSERT( identifier );
    C3currentActor.skinMeshIdentifier = C3AddStringInTable( identifier );
}


/*---------------------------------------------------------------------------*
  Name:         C3SetDisplayPriority

  Description:  Sets the runtime display priority for the current object.  
                0 is drawn first and 254 is drawn last.  255 is reserved
                for depth sorting.  Default drawing priority is 0 if this
                function is not called.

  Arguments:    NONE

  Returns:      pointer to the current primitive
/*---------------------------------------------------------------------------*/
void
C3SetDisplayPriority( u8 priority )
{
    C3HierNode  *hNode;

    hNode = C3GetCurrentHierNode();
    C3_ASSERT( hNode );
    hNode->displayPriority = priority;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetMatrix

  Description:  Set a transformation matrix for the current node as an
                arbitrary 4x3 matrix.
                The translation portion of this matrix will be snapped to the
                position quantization grid in order to maintain a fixed world
                grid of positions.

  Arguments:    matrix - matrix describing the transformation

  Returns:      NONE
/*---------------------------------------------------------------------------*/
/*
void 
C3SetMatrix( MtxPtr matrix )
{
    C3HierNode *hNode = NULL;

    C3_ASSERT( matrix );
    C3_ASSERT( !C3IsStackEmpty() );

    hNode = C3GetCurrentHierNode();
    C3_ASSERT( hNode );
    C3ValidateBoneControl( &hNode->control );

    // Set the matrix flag 
    hNode->control->c.type = CTRL_MTX;

    MTXCopy( matrix, hNode->control->c.controlParams.mtx.m );
}
*/

void
C3SetHierControl( u8 controlType, f32 x, f32 y, f32 z, f32 w )
{
    C3HierNode *hNode = NULL;

    C3_ASSERT( !C3IsStackEmpty() );

    hNode = C3GetCurrentHierNode();
    C3_ASSERT( hNode );
    C3ValidateBoneControl( &hNode->control );

    switch( controlType )
    {
        case CTRL_ROT_QUAT:
            CTRLSetQuat( &hNode->control->c, x, y, z, w );
            break;

        case CTRL_ROT_EULER:
            CTRLSetRotation( &hNode->control->c, x, y, z );
            break;

        case CTRL_TRANS:
            CTRLSetTranslation( &hNode->control->c, x, y, z );
            break;

        case CTRL_SCALE:
            CTRLSetScale( &hNode->control->c, x, y, z );
            break;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3SetPivotOffset

  Description:  The rotate pivot and scale pivot may not necessarily be the
                center of the transformation matrix set by C3SetMatrix, as
                in the case of Maya.  (In 3DSMAX, this function need not be
                called since pivot offsets are always 0).

  Arguments:    x, y, z - translation offset from center of transformation matrix

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetPivotOffset( f32 x, f32 y, f32 z )
{
    C3HierNode *hNode = NULL;

    C3_ASSERT( !C3IsStackEmpty() );

    hNode = C3GetCurrentHierNode();
    C3_ASSERT(hNode);

    hNode->pivotOffset.x = x;
    hNode->pivotOffset.y = y;
    hNode->pivotOffset.z = z;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetInheritanceFlag

  Description:  Sets the inheritance flag for the current node

  Arguments:    inheritance - type of inheritance to set

  Returns:      NONE
/*---------------------------------------------------------------------------*/
/*
void 
C3SetInheritanceFlag( u8 inheritance )
{
    C3HierNode *hNode = NULL;

    C3_ASSERT( !C3IsStackEmpty() );

    hNode = C3GetCurrentHierNode();

    // set the inheritance flags
    hNode->inheritanceFlag = inheritance;
}
*/


/*---------------------------------------------------------------------------*
  Name:         C3PushHierNode

  Description:  Push the specified node on the hierarchy node stack

  Arguments:    hNode - node to push on the stack

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3PushHierNode( C3HierNode *hNode )
{
    C3PtrLinkPtr node = NULL;

    node = (C3PtrLinkPtr)C3_CALLOC( 1, sizeof(C3PtrLink) );
    C3_ASSERT( node );

    node->ptr = (Ptr)hNode;

    DSInsertListObject( &C3stack, C3stack.Head, (Ptr)node );
}


/*---------------------------------------------------------------------------*
  Name:         C3PopHierNode

  Description:  Pops the top of the hierarchy node stack

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3PopHierNode( void )
{
    C3PtrLinkPtr node = NULL;

    node = (C3PtrLinkPtr)C3stack.Head;
    DSRemoveListObject( &C3stack, C3stack.Head );
    C3_FREE( node );
}


/*---------------------------------------------------------------------------*
  Name:         C3GetCurrentHierNode

  Description:  Returns the current hierachy node (top of the stack)

  Arguments:    NONE

  Returns:      current hiernode NULL if stack is empty
/*---------------------------------------------------------------------------*/
C3HierNodePtr
C3GetCurrentHierNode( void )
{
    C3HierNode *ptr = NULL;

    if( C3stack.Head )
        ptr = (C3HierNodePtr)(((C3PtrLinkPtr)(C3stack.Head))->ptr);

    return ptr;
}


/*---------------------------------------------------------------------------*
  Name:         C3IsStackEmpty

  Description:  Verifies if the stack is empty

  Arguments:    NONE

  Returns:      C3_TRUE if empty...
/*---------------------------------------------------------------------------*/
static C3Bool
C3IsStackEmpty( void )
{
    return( C3stack.Head == NULL );
}


/*---------------------------------------------------------------------------*
  Name:         C3CleanStack

  Description:  Frees the stack

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3CleanStack( void )
{
    Ptr node = NULL;
  
    node = C3stack.Head;
    while( node )
    {
        node = C3stack.Head;
        DSRemoveListObject( &C3stack, C3stack.Head );
        C3_FREE( (C3PtrLinkPtr)node );
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3ValidateBoneControl

  Description:  Allocates a control if the passed pointer is NULL. 
                Then inserts the new control in the actor control list

  Arguments:    control - pointer to memory control

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3ValidateBoneControl( C3ControlPtr *control )
{
    C3_ASSERT( control );

    if( *control == NULL )
    {
        *control = C3PoolControl();

        CTRLSetScale( &(*control)->c, 1, 1, 1 );
        CTRLSetQuat( &(*control)->c, 0, 0, 0, 1 );
        CTRLInit( &(*control)->c );
        DSInsertListObject( &C3currentActor.controlList, NULL, (Ptr)(*control) );
        (*control)->index = C3currentActor.numControls++;
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3GetHierNodeFromIdentifier

  Description:  Retrieves a node from the identifier passed in

  Arguments:    identifier - name of the node to retrieve

  Returns:      node retrieved pointer or NULL if not found
/*---------------------------------------------------------------------------*/
C3HierNodePtr
C3GetHierNodeFromIdentifier( char *identifier )
{
    C3HierNode *hNode  = NULL;
    C3HierNode *cursor = NULL;

    if( !identifier )
        return NULL;
    
    // search for the node in the bone list of the current actor
    cursor = (C3HierNodePtr)C3currentActor.boneList.Head;
    while( cursor )
    {
        if( strcmp( identifier, cursor->identifier ) == 0 )
        {
            hNode = cursor;
            cursor = NULL;
            continue;
        }
        cursor = (C3HierNodePtr)cursor->link.Next;
    }

    return hNode;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetACTUserData

  Description:  Sets the user-defined data and size to be included in the
                ACT file.  The data will be written out as is, so the bytes in
                the user data may need to swapped for endian correctness.

  Arguments:    size - size of data
                data - pointer to the data

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3SetACTUserData( u32 size, void *data )
{
    ActUserDataSize = size;
    ActUserData     = data;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetACTUserData

  Description:  Returns the current user-defined data and size.

  Arguments:    size - size of data to set
                data - pointer to the data to set

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3GetACTUserData( u32 *size, void **data )
{
    *size = ActUserDataSize;
    *data = ActUserData;
}