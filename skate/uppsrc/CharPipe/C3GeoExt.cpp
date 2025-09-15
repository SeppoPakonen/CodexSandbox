/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3GeoExt.c]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/build/charPipeline/c3/src/C3GeoExt.c $
    
    18    12/04/00 7:56p John
    Added multitexturing ability (work in progress).
    
    17    11/13/00 1:40p John
    Removed C3FreePrimitive since freeing is taken care of by C3Pool*
    functions.
    C3 library now works with extraction of quads.
    Fixed statistics dealing with stripping efficiency when extracting
    quads.
    Min and max positions are calculated later since positions can be
    transformed due to C3SetPivotOffset API.
    Normalizes normals set with C3SetNormal.
    
    16    8/14/00 6:14p John
    Added user-defined data to ANM, ACT, and GPL formats.
    
    15    6/27/00 6:05p John
    Added optimization to not include color indices if there is only one
    color for a geometry object (assumes runtime will set register color).
    
    14    3/23/00 2:11p John
    Modified function names to runtime library changes.
    
    13    3/14/00 1:32p John
    Moved display priority from GPL to ACT.
    
    12    2/29/00 7:22p John
    Reports error if wrong number of vertices sent to C3BeginPolyPrimitive
    and C3EndPolyPrimitive.
    
    11    2/18/00 4:54p John
    Now assigns vertex stitching information after hierarchy extraction
    process.
    
    10    2/04/00 6:07p John
    Untabified code.
    
    9     1/31/00 4:15p John
    Added pruning of unnecessary bones and animations.
    Finished stitching with FIFO matrix cache and stripping.
    
    8     1/20/00 1:14p John
    Added stitching (work in progress).
    Added C3 string table for efficient management of strings.
    
    7     1/04/00 1:12p John
    Added better support for second texture channel.
    
    6     12/20/99 11:21a John
    Added second texture channel.
    
    5     12/10/99 2:29p John
    
    4     12/08/99 7:26p John
    Removed printfs.
    
    3     12/06/99 3:25p John
    Removed C3Free* functions which were unnecessary due to the usage of
    memory pools.
    
    2     12/02/99 2:23p John
    Removed unused material and skinning code.
    
    26    11/18/99 1:16p John
    Minor code cleaning.
    
    25    11/11/99 1:15p John
    Commented out old material and skinning functions.
    
    24    10/21/99 6:23p John
    Added drawing priority.
    
    23    10/13/99 3:31p John
    Modified code to accomodate new PositionHeader and LightingHeader.
    
    22    10/13/99 2:53p John
    Added Default Normal Table.
    
    21    10/01/99 11:16a John
    Can quantize color two ways (with and without alpha)
    Added position and tex. coordinate quantization per object.
    
    20    9/29/99 4:30p John
    Changed header to make them all uniform with logs.
    Added position and normal quantization.
    Added external normal table.
    
    19    9/21/99 6:41p John
    Added some minor comments.
    
    18    9/20/99 7:03p John
    Cleaned up code.
    Renamed C3TextCoord to C3TexCoord for consistency.
    
    17    9/17/99 9:34a John
    Added C3Options and cleaned up code.
    
    16    9/14/99 6:49p John
    Modified code in response to C3Stats and C3StatsObj.
    
    15    9/03/99 2:02p John
    Added quantization of color and indices.
    Added removal of null primitives.
    
    14    8/23/99 4:00p John
    
    13    8/19/99 3:46p John
    Added analysis and welding of texture coordinates.
    
    12    8/15/99 10:31p John
    Added code to handle minPosition and maxPosition of type C3Position
    instead of f32 since positions are now hashed by distance from origin.

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#include "C3Private.h"

// Local types
typedef struct
{
    C3GeomObject *geomObject; // currentObject
    C3Primitive  *primitive;  // currentPrimitive;
    C3Vertex     *vertex;     // currentVertex;
    C3PtrLink    *vertexPtr;  // currentVertexPtr;

} C3CurrentState;

// Local functions
static void         C3BeginPrimitive ( void );
static void         C3EndPrimitive   ( void );

static void         C3InitObjectDatabase      ( void );
static void         C3AddObjectToDatabase     ( C3GeomObject* obj ); 

static void         C3LoadExternalNormalTable();


// Local variables
static C3CurrentState   C3current;               // Current state variable
static DSList           C3geomObjectList;        // list of objects
static u32              c3geomObjCount = 0;

// Global variables
DOLightingHeader c3externalNormalHeader;  // used only for normals
DSList           C3externalNormalList;    // external normal table to index into (if option is on)

u32              GplUserDataSize = 0;
void            *GplUserData     = NULL;

/*---------------------------------------------------------------------------*
  Name:         C3GetGeomObjList

  Description:  Returns the global object list

  Arguments:    NONE

  Returns:      pointer to the list of geom objects
/*---------------------------------------------------------------------------*/
DSList* 
C3GetGeomObjList(void)
{
    return &C3geomObjectList;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetGeomObjCount

  Description:  Returns the number of geom objects in the database

  Arguments:    NONE

  Returns:      the number of geometric objects
/*---------------------------------------------------------------------------*/
u32
C3GetGeomObjCount( void )
{
    return c3geomObjCount;
}


/*---------------------------------------------------------------------------*
  Name:         C3InitGeometryExtraction

  Description:  Initialize the geometry extraction specific variables, states

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3InitGeometryExtraction ( void )
{
    C3Normal    nor;

    // Initialize the current states to default
    C3current.geomObject = NULL;   
    C3current.primitive  = NULL; 
    C3current.vertex     = NULL; 
    C3current.vertexPtr  = NULL;
    
    // Initialize the object database
    C3InitObjectDatabase();

    // Initialize the external normal table (list)
    DSInitList( &C3externalNormalList, (void*)&nor, &nor.link );

#ifdef C3_GENERATE_NORMAL_TABLE
    // Since we're using the external normal table name as the output filename
    C3SetOptionUseDefaultNormalTable( C3_FALSE );
    C3SetOptionUseExternalNormalTable( C3_FALSE );
#endif

    if ( C3GetOptionEnableLighting() && C3GetOptionUseExternalNormalTable() )
        C3LoadExternalNormalTable();
}


/*---------------------------------------------------------------------------*
  Name:         C3LoadExternalNormalTable

  Description:  Loads the external normal table.

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3LoadExternalNormalTable()
{
    C3Normal*   normal;
    FILE*       nFile;
//  FILE*       outFile;
    u32         i;
    u32         compSize;

    memset( &c3externalNormalHeader, 0, sizeof(DOLightingHeader) );

    // Initialize the normal table by loading it from the file
    nFile = fopen( C3GetOptionExternalNormalTablePath() , "rb");
    if ( !nFile ) 
    {
        C3ReportError("Could not find external normal table.  Using generated normals.");
        C3SetOptionUseExternalNormalTable( C3_FALSE );
        return;         
    }
    fread( &c3externalNormalHeader.numNormals, sizeof(u16), 1, nFile );
    if ( c3externalNormalHeader.numNormals == 0 )
    {
        fclose( nFile );
        C3ReportError("No normals in external normal table.  Using generated normals.");
        C3SetOptionUseExternalNormalTable( C3_FALSE );
        return;
    }
    fread( &c3externalNormalHeader.quantizeInfo, sizeof(u8), 1, nFile );
    fread( &c3externalNormalHeader.compCount, sizeof(u8), 1, nFile );
    C3_ASSERT( c3externalNormalHeader.compCount == 3 );

//  outFile = fopen("D:\\temp\\normalTable.txt", "wt");

    compSize = C3GetComponentSizeFromQuantInfo( C3_TARGET_NORMAL, c3externalNormalHeader.quantizeInfo );
    for ( i = 0; i < c3externalNormalHeader.numNormals; i++ )
    {
        // Read each normal into the normal list
        normal = C3PoolNormal();

        fread( &normal->x, compSize, 1, nFile );
        fread( &normal->y, compSize, 1, nFile );
        fread( &normal->z, compSize, 1, nFile );

        normal->x = C3FloatFromQuant( (void*)&normal->x, c3externalNormalHeader.quantizeInfo );
        normal->y = C3FloatFromQuant( (void*)&normal->y, c3externalNormalHeader.quantizeInfo );
        normal->z = C3FloatFromQuant( (void*)&normal->z, c3externalNormalHeader.quantizeInfo );

        normal->index = i;
        DSInsertListObject( &C3externalNormalList, NULL, (void*)normal );

        // TESTING: Print it out to make sure they're okay
//      fprintf( outFile, "%.15f, %.15f, %.15f,\n", normal->x, normal->y, normal->z );
    }

//  fclose( outFile );
    fclose( nFile );
}


/*---------------------------------------------------------------------------*
  Name:         C3BeginObject

  Description:  Start the creation of new geomObject

  Arguments:    identifier - null terminated string

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3BeginObject( char* identifier )
{
    C3GeomObject*   newObject = NULL;
    C3StatsObj*     newStatsObj = NULL;
    C3OptionObj*    newOptionObj = NULL;
    u8              i         = 0;
    C3Color         color;     // For list intialization
    C3Position      pos;
    C3Normal        normal;    
    C3Primitive     primitive; 
    C3TexCoord      texCoord;  
    C3Vertex        vertex;

    C3_ASSERT( identifier && !C3current.geomObject );

    // create a new objects
    newObject = C3PoolGeomObject();

    // Zero out the new object
    memset( newObject, 0, sizeof(C3GeomObject) );
    
    // Add the identifier to the string table if string isn't there already
    newObject->identifier = C3AddStringInTable( identifier );

    newObject->minPosition.x = FLT_MAX;
    newObject->minPosition.y = FLT_MAX;
    newObject->minPosition.z = FLT_MAX;
    newObject->maxPosition.x = -FLT_MAX;
    newObject->maxPosition.y = -FLT_MAX;
    newObject->maxPosition.z = -FLT_MAX;
    newObject->minNormalX = FLT_MAX;
    newObject->maxNormalX = -FLT_MAX;

    newObject->minPosDistance = FLT_MAX;
    newObject->minColDistance = FLT_MAX;

    newObject->useVertexAlpha = C3_FALSE;
    newObject->colorAllWhite = C3_TRUE;   // Assume true until proven false in C3SetColor

    DSInitList( &newObject->colorList,     (void*)&color,     &(color.link) );
    DSInitList( &newObject->positionList,  (void*)&pos ,      &(pos.link) );
    DSInitList( &newObject->normalList,    (void*)&normal,    &(normal.link) );
    DSInitList( &newObject->vertexList,    (void*)&vertex,    &(vertex.link) );
    DSInitList( &newObject->primitiveList, (void*)&primitive, &(primitive.link) );

    for( i = 0; i < C3_MAX_TEXTURES; i++ )
    {
        DSInitList( &newObject->texCoordList[i], (void*)&texCoord, &(texCoord.link) );
        newObject->minTexCoord[i] = FLT_MAX;
        newObject->maxTexCoord[i] = -FLT_MAX;
        newObject->minTexDistance[i] = FLT_MAX;
        newObject->maxTexDistance[i] = -FLT_MAX;
    }

    newObject->indexPosMatQuant = C3_VCD_DIRECT;
    newObject->indexPosQuant = C3_VCD_INDEX16;   // Default is 16-bit indices
    newObject->indexColQuant = C3_VCD_INDEX16;
    for( i = 0; i < C3_MAX_TEXTURES; i++ )
        newObject->indexTexQuant[i] = C3_VCD_NONE;
    newObject->indexNorQuant = C3_VCD_NONE;

    newStatsObj = C3NewStatsObj( newObject->identifier );
    newObject->stats = newStatsObj;
    
    newOptionObj = C3NewOptionObj( newObject->identifier );
    newObject->options = newOptionObj;

    // set this object to be the current object`
    C3current.geomObject = newObject;
}


/*---------------------------------------------------------------------------*
  Name:         C3EndObject

  Description:  End the current object and adds it to the database

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3EndObject( void )
{
    C3_ASSERT( C3current.geomObject );

    // add current object to the global list
    C3AddObjectToDatabase( C3current.geomObject ); 

    // add the stats
    C3AddStatsObj( C3current.geomObject->stats );

    // add the options
    C3AddOptionObj( C3current.geomObject->options );

    // remove the object from being the current object
    C3current.geomObject = NULL;
}


/*---------------------------------------------------------------------------*
  Name:         C3BeginPrimitive

  Description:  Begin the definition of a primitive

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void 
C3BeginPrimitive( void )
{
    C3Primitive*  newPrimitive;
    C3PtrLink       vertex;
    u16             i;


    // make sure there is a current object
    C3_ASSERT( !C3current.primitive && C3current.geomObject );

    // create a new primitive
    newPrimitive = C3PoolPrimitive();
 
    // set the values according to current object
    newPrimitive->link.Next = NULL;
    newPrimitive->link.Prev = NULL;

    newPrimitive->numVertex = 0;

    DSInitList( &newPrimitive->vertexPtrList, (void*)&vertex, &(vertex.link) );

    for( i = 0; i < C3_MAX_TEXTURES; i++ )
    {
        newPrimitive->texture[i] = C3current.geomObject->textures[i];
        newPrimitive->texWrap[i] = C3_REPEAT_S & C3_REPEAT_T;
    }

    newPrimitive->normal   = NULL;
    newPrimitive->color    = C3current.geomObject->color;
  
    // set the new primitive to the current primitive
    C3current.primitive = newPrimitive;
}


/*---------------------------------------------------------------------------*
  Name:         C3EndPrimitive

  Description:  End the creation of a primitive

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void 
C3EndPrimitive( void )
{
    C3_ASSERT( C3current.geomObject && C3current.primitive );

    // add the current primitive to the current object list
    DSInsertListObject( &C3current.geomObject->primitiveList, 
                        NULL, 
                        (void*) C3current.primitive );  
    C3current.geomObject->stats->numPrimitives++;

    // remove the primitive from being the current primitive
    C3current.primitive = NULL;
}


/*---------------------------------------------------------------------------*
  Name:         C3BeginPrimitiveLine

  Description:  Begins the creation of line primitive

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3BeginLinePrimitive( void )
{
    // TBD set a code to force the presentation to the points defined in the
    //  set list (ie all points, all lines or line strip )
    C3BeginPrimitive();
}

/*---------------------------------------------------------------------------*
  Name:         C3EndLinePrimitive

  Description:  End the creation of a line primitive, sets the proper type 
                according to the number of points in the primitive

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3EndLinePrimitive( void )
{
    u8 type = C3_PRIM_POINT;

    C3_ASSERT( C3current.primitive );

    switch( C3current.primitive->numVertex )
    {
        case 0:
            C3ReportError("Invalid number of vertices in line primitive (%i)", C3current.primitive->numVertex );
            C3current.primitive = NULL;
            return;

        case 1:
            type = C3_PRIM_POINT;
            C3current.geomObject->stats->numPoint++;
            break;

        case 2:
            type = C3_PRIM_LINE;
            C3current.geomObject->stats->numLine++;
            break;

        default:
            type = C3_PRIM_LINE_STRIP;
            C3current.geomObject->stats->numLineStrip++;
            break;
    }
    C3current.primitive->type = type;
    C3EndPrimitive();
}


/*---------------------------------------------------------------------------*
  Name:         C3BeginPolyPrimitive

  Description:  Begin the creation of a polygone primitive

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3BeginPolyPrimitive( void )
{
    C3BeginPrimitive();
}


/*---------------------------------------------------------------------------*
  Name:         C3EndPolyPrimitive

  Description:  End the creation of a polygone primitive, sets the proper type 
                according to the number of points in the primitive

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3EndPolyPrimitive( void )
{
    u8 type;

    C3_ASSERT( C3current.primitive );

    switch( C3current.primitive->numVertex )
    {
        case 3:
            type = C3_PRIM_TRI;
            C3current.geomObject->stats->numTri++;
            C3current.geomObject->stats->numVerticesForStripping += 3;
            break;

        case 4:
            type = C3_PRIM_QUAD;
            C3current.geomObject->stats->numQuad++;
            C3current.geomObject->stats->numVerticesForStripping += 6;
            break;

        default:
            C3ReportError( "C3 library can only extract triangles or quads from %s (Too many vertices: %d)",
                           C3current.geomObject->identifier, C3current.primitive->numVertex);
            C3current.primitive = NULL;
            return;
    }

    C3current.primitive->type = type;

    // Since we need CW polygon, reverse the list if they are CCW
    if( C3GetOptionSrcVertexOrder() == C3_CCW )
    {
        DSList  newList = C3current.primitive->vertexPtrList;
        void    *cursor, *temp;

        C3current.primitive->vertexPtrList.Tail = NULL;
        C3current.primitive->vertexPtrList.Head = NULL;
        temp = newList.Head;
        DSRemoveListObject( &newList, temp );
        DSInsertListObject( &C3current.primitive->vertexPtrList, NULL, temp );
        
        cursor = newList.Tail;
        while( cursor )
        {   
            temp = cursor;
            cursor = ((C3PtrLink*)cursor)->link.Prev;
            DSRemoveListObject( &newList, temp );
            DSInsertListObject( &C3current.primitive->vertexPtrList, NULL, temp );
        }
    }    
    
    C3EndPrimitive();  
}


/*---------------------------------------------------------------------------*
  Name:         C3BeginVertex

  Description:  Begin the creation of a new vertex, initializing the values
                based on

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3BeginVertex( void )
{
    C3Vertex*   vtx      = NULL;
    C3PtrLink*  vtxPtr   = NULL;
    u16           i        = 0;
    C3Color*    colorPtr = NULL;


    C3_ASSERT( !C3current.vertex && C3current.geomObject && C3current.primitive );

    // create new vertex
    vtx = C3PoolVertex();

    vtxPtr = C3PoolPtrLink();
    C3_ASSERT( vtxPtr );
    vtxPtr->ptr = (void*)vtx;

    // get the inherited values
    colorPtr = C3current.primitive->color;
    if( !colorPtr )
        colorPtr = C3current.geomObject->color;

    // set the values according to current primitive
    vtx->link.Prev = NULL;
    vtx->link.Next = NULL;
    vtx->position  = NULL;
    vtx->color     = colorPtr;
    vtx->normal    = C3current.primitive->normal;
    vtx->boneIndex = NULL;
  
    for( i = 0; i < C3_MAX_TEXTURES; i++)
    {
        vtx->tCoord[i] = NULL;
    }

    // set the new vertex to be the current vertex
    C3current.vertex    = vtx;
    C3current.vertexPtr = vtxPtr;
}


/*---------------------------------------------------------------------------*
  Name:         C3EndVertex

  Description:  Ends the creation of a vertex, adds the new vertex to the 
                proper lists

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3EndVertex( void )
{
    C3_ASSERT( C3current.primitive && C3current.vertex );

    // add the vertex to the global vertex list
    DSInsertListObject( &C3current.geomObject->vertexList, NULL, (void*) C3current.vertex );  
    C3current.vertex->index = C3current.geomObject->stats->numVertices++;

    // add the vertex ptr to the primitive list
    DSInsertListObject( &C3current.primitive->vertexPtrList, 
                        NULL, 
                        (void*) C3current.vertexPtr );  

    // add the vertex to the primitive list
    C3current.primitive->numVertex++;

    // remove vertex from being the current vertex
    C3current.vertex    = NULL;
    C3current.vertexPtr = NULL;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetPosition

  Description:  Adds a new position to the current vertex

  Arguments:    X - x component
                Y - y component
                Z - z component

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3SetPosition( float X, float Y, float Z )
{
    C3Position* positionPtr = NULL;

    C3_ASSERT( C3current.geomObject && C3current.primitive && C3current.vertex );

    // Create the position
    positionPtr = C3PoolPosition();
    positionPtr->x = X;
    positionPtr->y = Y;
    positionPtr->z = Z;

    // Add position to object position list
    DSInsertListObject( &C3current.geomObject->positionList, NULL, (void*) positionPtr );  
    positionPtr->index = C3current.geomObject->stats->numPositions++;
    C3current.geomObject->stats->numUniquePositions = (u16)C3current.geomObject->stats->numPositions;
    C3current.vertex->position = positionPtr;

    // Compute min and max components in C3Options.c
}


/*---------------------------------------------------------------------------*
  Name:         C3SetTexCoord

  Description:  Adds a new texture coord to the current vertex

  Arguments:    s     - s component ( or u )
                t     - t component ( or v )
                channel - which channel to add the texture for

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3SetTexCoord( float s, float t, u8 channel )
{
    C3TexCoord*  texCoordPtr = NULL;
    f32          distance;

    C3_ASSERT( C3current.vertex && channel >= 0 && channel < C3_MAX_TEXTURES );

    // create the texture coord
    texCoordPtr = C3PoolTexCoord();
    texCoordPtr->s = s;
    texCoordPtr->t = t;

    // add normal to object texture coord list
    DSInsertListObject( &C3current.geomObject->texCoordList[channel], NULL, (void*)texCoordPtr );  
    texCoordPtr->index = C3current.geomObject->stats->numTCoords[channel]++;
    C3current.geomObject->stats->numUniqueTCoords[channel] = (u16)C3current.geomObject->stats->numTCoords[channel];

    C3current.vertex->tCoord[channel] = texCoordPtr;

    if( s < C3current.geomObject->minTexCoord[channel] )
        C3current.geomObject->minTexCoord[channel] = s;

    if( t < C3current.geomObject->minTexCoord[channel] )
        C3current.geomObject->minTexCoord[channel] = t;

    if( s > C3current.geomObject->maxTexCoord[channel] )
        C3current.geomObject->maxTexCoord[channel] = s;

    if( t > C3current.geomObject->maxTexCoord[channel] )
        C3current.geomObject->maxTexCoord[channel] = t;

    distance = (f32)sqrt(pow(s, 2) + pow(t, 2));
    if( distance < C3current.geomObject->minTexDistance[channel] )
        C3current.geomObject->minTexDistance[channel] = distance;

    if( distance > C3current.geomObject->maxTexDistance[channel] )
        C3current.geomObject->maxTexDistance[channel] = distance;

}


/*---------------------------------------------------------------------------*
  Name:         C3SetColor

  Description:  Adds a color object to the current object being created
                ( vertex, primitive, geomObject )

  Arguments:    Red   - red component
                Green - green component
                Blue  - blue component
                Alpha - alpha component

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3SetColor( u8 Red, u8 Green, u8 Blue, u8 Alpha )
{
    C3Color*  colorPtr = NULL;
    f32 distance;

    C3_ASSERT( C3current.geomObject ); 
  
    // create the color
    colorPtr = C3PoolColor();
    colorPtr->r = Red;
    colorPtr->g = Green;
    colorPtr->b = Blue;
    colorPtr->a = Alpha;

    // add color to object color list
    DSInsertListObject( &C3current.geomObject->colorList, NULL, (void*)colorPtr );  
    colorPtr->index = C3current.geomObject->stats->numColors++;
    C3current.geomObject->stats->numUniqueColors = (u16)C3current.geomObject->stats->numColors;

    if( C3current.vertex )
        C3current.vertex->color = colorPtr;
    else if( C3current.primitive )
        C3current.primitive->color = colorPtr;
    else
        C3current.geomObject->color = colorPtr;

    // set that we are using the vertex alpha for this object
    if ( Alpha != 255 )
        C3current.geomObject->useVertexAlpha = C3_TRUE;

    // check if the color is nonwhite
    if( Red != 255 || Green != 255 || Blue != 255 || Alpha != 255 )
        C3current.geomObject->colorAllWhite = C3_FALSE;

    distance = (f32)sqrt(pow(colorPtr->r, 2) + pow(colorPtr->g, 2) +
                         pow(colorPtr->b, 2) + pow(colorPtr->a, 2));
    if( distance < C3current.geomObject->minColDistance )
        C3current.geomObject->minColDistance = distance;
    if( distance > C3current.geomObject->maxColDistance )
        C3current.geomObject->maxColDistance = distance;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetNormal

  Description:  Adds a new normal to the current primitiv or vertex being 
                created

  Arguments:    nX - x component
                nY - y component
                nZ - z component

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3SetNormal( float nX, float nY, float nZ )
{
    C3Normal* normalPtr = NULL;

    C3_ASSERT( C3current.geomObject && C3current.primitive );

    // Create the normal
    normalPtr = C3PoolNormal();
    normalPtr->x = nX;
    normalPtr->y = nY;
    normalPtr->z = nZ;

    // Normalize the normal
    if( C3_FLOAT_EQUAL( normalPtr->x, 0 ) && 
        C3_FLOAT_EQUAL( normalPtr->y, 0 ) && 
        C3_FLOAT_EQUAL( normalPtr->z, 0 ) )
    {
        C3ReportError( "C3SetNormal: zero-length normal set in %s", C3current.geomObject->identifier );
        normalPtr->x = 1.0f;
    }
    VECNormalize( (Vec*)&normalPtr->x, (Vec*)&normalPtr->x );

    // Add normal to object normal list
    DSInsertListObject( &C3current.geomObject->normalList, NULL, (void*) normalPtr );  
    normalPtr->index = C3current.geomObject->stats->numNormals++;
    C3current.geomObject->stats->numUniqueNormals = (u16)C3current.geomObject->stats->numNormals;

    if( normalPtr->x < C3current.geomObject->minNormalX )
        C3current.geomObject->minNormalX = normalPtr->x;
    if( normalPtr->x > C3current.geomObject->maxNormalX )
        C3current.geomObject->maxNormalX = normalPtr->x;
    
    if( C3current.vertex )
        C3current.vertex->normal = normalPtr;
    else
        if( C3current.primitive )
            C3current.primitive->normal = normalPtr;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetVertexMatrix

  Description:  Sets the bone that the matrix will be rigid with in stitching.
                If stitching is not enabled with C3SetOptionEnableStitching,
                this function should not be called.

  Arguments:    boneName - name of the bone in hierarchy

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetVertexMatrix( char* boneName )
{
    C3_ASSERT( C3current.vertex );

    // A little hack: Even though boneIndex is a u16*, it will initially
    // point to a char* as the name of the bone.  After all hierarchy 
    // information has been extracted, then we will convert these bone
    // names in boneIndex to a pointer to an index in the hierarchy node
    // in C3AssignBoneIndices in C3ProcessOptionsAfterCompression.
    C3current.vertex->boneIndex = (u16*)C3AddStringInTable( boneName );
}


/*---------------------------------------------------------------------------*
  Name:         C3GetCurrentPrim

  Description:  Returns the current primitive being created

  Arguments:    NONE

  Returns:      pointer to the current primitive
/*---------------------------------------------------------------------------*/
C3Primitive*    
C3GetCurrentPrim( void )
{
    return C3current.primitive;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetCurrentGeomObj

  Description:  Returns the current geom object being created 

  Arguments:    NONE

  Returns:      pointer to the current geom object 
/*---------------------------------------------------------------------------*/
C3GeomObject*   
C3GetCurrentGeomObj( void )
{
    return C3current.geomObject;
}


/*---------------------------------------------------------------------------*
  Name:         C3InitObjectDatabase

  Description:  Initialize the global list of geom object 

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3InitObjectDatabase( void )
{
    C3GeomObject  geomObject;

    DSInitList(&C3geomObjectList, (void*)&geomObject, &(geomObject.link));
    c3geomObjCount = 0;
}


/*---------------------------------------------------------------------------*
  Name:         C3AddObjectToDatabase

  Description:  Inserts a geometric object in the object database

  Arguments:    obj - object to add to the database

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void         
C3AddObjectToDatabase( C3GeomObject* obj )
{
    C3_ASSERT( obj );

    obj->index = c3geomObjCount++;
    DSInsertListObject( &C3geomObjectList, NULL, (void*) obj );
}


/*---------------------------------------------------------------------------*
  Name:         C3GetObjectFromIdentifier

  Description:  Returns the object pointer for the specified identifier

  Arguments:    identifier - name of the object to get

  Returns:      returns a pointer to the object if found and NULL if not found
/*---------------------------------------------------------------------------*/
C3GeomObject* 
C3GetObjectFromIdentifier ( char* identifier )
{
    C3GeomObject* cursor = NULL;
    
    if( !identifier )
        return NULL;

    // run the list
    cursor = (C3GeomObject*)C3geomObjectList.Head;  
    while( cursor )
    {
        // for each object compare to the identifier
        if( !strcmp( cursor->identifier, identifier ) )
            return cursor;

        cursor = (C3GeomObject*)cursor->link.Next;
    }

    return NULL;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetNextObject 

  Description:  Returns the next object in the database

  Arguments:    obj - pointer to current object or NULL

  Returns:      pointer to the next object 
/*---------------------------------------------------------------------------*/
C3GeomObject*
C3GetNextObject( C3GeomObject* obj )
{
    C3GeomObject* next = NULL;

    if( obj == NULL )
    {
        next =  (C3GeomObjectPtr)C3geomObjectList.Head;
    }
    else
    {
        next = (C3GeomObjectPtr)obj->link.Next;
    }
    return next;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetGPLUserData

  Description:  Sets the user-defined data and size to be included in the
                GPL file.  The data will be written out as is, so the bytes in
                the user data may need to swapped for endian correctness.

  Arguments:    size - size of data
                data - pointer to the data

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3SetGPLUserData( u32 size, void *data )
{
    GplUserDataSize = size;
    GplUserData     = data;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetGPLUserData

  Description:  Returns the current user-defined data and size.

  Arguments:    size - size of data to set
                data - pointer to the data to set

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3GetGPLUserData( u32 *size, void **data )
{
    *size = GplUserDataSize;
    *data = GplUserData;
}

