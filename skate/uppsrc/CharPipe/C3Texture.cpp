/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3Texture.c]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/c3/src/C3Texture.c $
    
    17    11/13/00 1:49p John
    Removed newlines from all C3ReportError strings.
    
    16    9/22/00 2:07p John
    TexConv can now handle directory paths with spaces in them.
    
    15    6/21/00 10:56a John
    Fixed path to TexConv using bash environment variable DOLPHIN_ROOT.
    
    14    3/17/00 1:54p John
    Now indexed textures from 0 and x if none exists.
    
    13    2/29/00 7:25p John
    Renamed C3BeginTex to C3BeginTexture and C3EndTex to C3EndTexture.
    
    12    2/16/00 6:34p John
    More robust calling of TexConv.exe.
    
    11    00/02/16 10:51a Paul
    Converts pathnames of the form \\d\dolphin into pathnames of the form
    d:\dolphin
    
    10    2/07/00 4:49p John
    TexConv.exe is now called based on the dolphin path set in the
    environment variable DOLPHIN_ROOT, if it exists.
    
    9     2/04/00 6:08p John
    Untabified code.
    
    8     1/31/00 4:16p John
    Added pruning of unnecessary bones and animations.
    Finished stitching with FIFO matrix cache and stripping.
    
    7     1/20/00 1:15p John
    Added stitching (work in progress).
    Added C3 string table for efficient management of strings.
    
    6     1/04/00 1:12p John
    Added better support for second texture channel.
    
    5     12/15/99 4:32p John
    Changed texture script extension from .txt to .tcs.
    
    4     12/08/99 7:27p John
    Cleaned up C3AddSrcImage to improve memory efficiency.
    
    3     12/06/99 3:27p John
    Commented a function.
    Changed tc.exe to TexConv.exe.
    
    2     12/02/99 2:25p John
    Removed status update for compressing texture list.
    
    18    10/13/99 7:09p John
    Modified tc path to be backslashes.
    
    17    10/13/99 2:55p John
    Checks to see if tc.exe exists before calling it.
    
    16    9/29/99 4:33p John
    Changed header to make them all uniform with logs.
    
    15    9/21/99 6:42p John
    Modified allocation to use pools.
    Fixed memory leaks.
    
    14    9/20/99 6:57p John
    Added limited palette support (no alpha and must be CI8).
    Added hashing of C3SrcImage, C3Image, C3Palette, and C3Texture
    upon compression to speed up conversion.
    
    13    9/17/99 9:34a John
    Added C3Options and cleaned up code.
    
    12    9/14/99 6:52p John
    Added texture filter and LOD parameters for mipmapping.
    Calls tc through command line.
    
    11    9/03/99 2:04p John
    Changed tiling data structure to 8 bits.
    Work in progress for adding texture filter option.
    
  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#include <process.h>
#include "c3Private.h"

#define C3_DOLPHIN_ROOT         "DOLPHIN_ROOT"              // env. variable name
#define C3_DOLPHIN_ROOT_DEFAULT "C:\\dolphin"               // path to default dolphin dir.
#define C3_TEXCONV_PATH         "\\X86\\bin\\TexConv.exe"   // path to TexConv within dolphin dir.

#define C3_EXT_TEX      "tcs"
#define C3_EXT_TEXP     "tpl"
#define HASH_TABLE_SIZE 256

static C3SrcImage*  C3AddSrcImg( char* fileName );
static void         C3OutputTexScript( char* path, char* name );
static void         C3OutputSrcImg( FILE* outFile, DSList* list );
static void         C3OutputImg( FILE* outFile, DSList* list );
static void         C3OutputPal( FILE* outFile, DSList* list );
static void         C3OutputTex( FILE* outFile, DSList* list );
static C3Bool       C3CompareImage( void* t1, void* t2 );
static C3Bool       C3ComparePalette( void* t1, void* t2 );
static C3Bool       C3CompareSrcImage( void* t1, void* t2 );

static void         C3CompressListData( DSList* list, DSHashFunc *hashFunc, C3LinkCompareFuncPtr compare );
static u16          HashSrcImage( void* srcImage );
static u16          HashImage( void* image );
static u16          HashPalette( void* palette );
static u16          HashTextures( void* texture );

DSList      c3SrcList;
DSList      c3ImgList;
DSList      c3PalList;
DSList      c3TexList;

C3Texture*  c3Tex           = NULL;
C3TexFmt    c3TexFmt        = GX_NONE;
C3Bool      c3TexCompressed = C3_FALSE;
char        c3TexConvPath[C3_MAX_PATH];


/*---------------------------------------------------------------------------*
  Name:         C3InitializeTextures

  Description:  Initialize the list of texture information

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3InitializeTextures( void )
{
    C3Texture   tex;
    C3Palette   pal;
    C3Image     img;
    C3SrcImage  srcImg;


    c3TexCompressed = C3_FALSE;

    DSInitList( &c3SrcList, (void*)&srcImg, &srcImg.link );
    DSInitList( &c3ImgList, (void*)&img, &img.link );
    DSInitList( &c3PalList, (void*)&pal, &pal.link );
    DSInitList( &c3TexList, (void*)&tex, &tex.link );
}


/*---------------------------------------------------------------------------*
  Name:         C3BeginTexture

  Description:  Begins the creation of a texture.  Texture will be assigned
                to the nearest enclosing block of C3BeginObject or C3BeginPrimitive.

  Arguments:    channel - texture channel to set the texture

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3BeginTexture( u8 channel )
{
    C3Primitive*    prim   = NULL;
    C3GeomObject*   geoObj = NULL;

    C3_ASSERT( c3Tex == NULL && channel >= 0 && channel < C3_MAX_TEXTURES );

    c3Tex = C3PoolTexture();
    C3_ASSERT( c3Tex );

    c3TexFmt = RGBA8;
    c3Tex->img = NULL;
    c3Tex->pal = NULL;
    c3Tex->tiling = C3_CLAMP_S | C3_CLAMP_T;
    c3Tex->filterType = C3_FILTER_NEAR;

    // add the texture to the current extraction object
    if( prim = C3GetCurrentPrim() )
    {
        prim->texture[channel] = c3Tex; 
    }
    else
    {
        if( geoObj = C3GetCurrentGeomObj() )
        {
            geoObj->textures[channel] = c3Tex;
        }
        else 
            C3_ASSERT( geoObj );
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3EndTexture

  Description:  End the creation of a texture 

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3EndTexture( void )
{
    DSInsertListObject( &c3TexList, NULL, (void*)c3Tex);
    c3TexFmt = RGBA8;
    c3Tex    = NULL;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetImage

  Description:  Set the source of the image for the current texture

  Arguments:    fileName - name of the file to use

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void    
C3SetImage( char* fileName )
{
    C3_ASSERT( c3Tex ); 

    if ( !c3Tex->img )
    {
        c3Tex->img = C3PoolImage();
        C3_ASSERT( c3Tex->img );
        DSInsertListObject( &c3ImgList, NULL, (void*)c3Tex->img );
        c3Tex->img->texFmt  = c3TexFmt;
        c3Tex->img->minLOD  = 255;
        c3Tex->img->maxLOD  = 255;
        c3Tex->img->baseLOD = 255;
        c3Tex->img->alphaMap = NULL;
    }

    c3Tex->img->colorMap = C3AddSrcImg( fileName );
}


/*---------------------------------------------------------------------------*
  Name:         C3SetImageAlpha

  Description:  Set the source of the image alpha for the current texture

  Arguments:    fileName - name of the file to use 

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetImageAlpha( char* fileName )
{
    C3_ASSERT( c3Tex ); 

    if ( !c3Tex->img )
    {
        c3Tex->img = C3PoolImage();
        C3_ASSERT( c3Tex->img );
        DSInsertListObject( &c3ImgList, NULL, (void*)c3Tex->img );
        c3Tex->img->texFmt = c3TexFmt;
        c3Tex->img->minLOD  = 255;
        c3Tex->img->maxLOD  = 255;
        c3Tex->img->baseLOD = 255;
        c3Tex->img->colorMap = NULL;
    }

    c3Tex->img->alphaMap = C3AddSrcImg( fileName );
}


/*---------------------------------------------------------------------------*
  Name:         C3SetPalImage

  Description:  Set the source of the palette for the current texture

  Arguments:    fileName - name of the file to use

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void    
C3SetPalImage( char* fileName )
{
    C3_ASSERT( c3Tex ); 

    if ( !c3Tex->pal )
    {
        c3Tex->pal = C3PoolPalette();
        C3_ASSERT( c3Tex->pal );
        DSInsertListObject( &c3PalList, NULL, (void*)c3Tex->pal );
        c3Tex->pal->texFmt = c3TexFmt;
        c3Tex->pal->alphaMap = NULL;
    }

    c3Tex->pal->colorMap = C3AddSrcImg( fileName );
}


/*---------------------------------------------------------------------------*
  Name:         C3SetPalAlpha

  Description:  Set the source of the palette alpha for the current texture

  Arguments:    fileName - name of the file to use

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetPalAlpha( char* fileName )
{
    C3_ASSERT( c3Tex ); 

    if ( !c3Tex->pal )
    {
        c3Tex->pal = C3PoolPalette();
        C3_ASSERT( c3Tex->pal );
        DSInsertListObject( &c3PalList, NULL, (void*)c3Tex->pal );
        c3Tex->pal->texFmt = c3TexFmt;
        c3Tex->pal->colorMap = NULL;
    }

    c3Tex->pal->alphaMap = C3AddSrcImg( fileName );
}


/*---------------------------------------------------------------------------*
  Name:         C3SetTexFmt

  Description:  Set the current texture format that will affect the creation 
                off all the subsequent iamge and palette until reset.

  Arguments:    texFmt - texture format to set

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetTexFmt( C3TexFmt texFmt )
{
    C3_ASSERT( c3Tex ); 
    c3TexFmt = texFmt;
}


/*---------------------------------------------------------------------------*
  Name:         C3WriteTextures

  Description:  Outputs the texture script and then call the function to 
                convert the textures ( optionally in text format as well )

  Arguments:    path - path where to create the file
                name - name of the file

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3WriteTextures( char* path, char* name )
{
    char* fileNamePath    = NULL;
    char* fileNamePal     = NULL;
    char* ext             = NULL;
    char* dolphinRoot     = NULL;
    char  cmdLine[256];
    FILE* testOpen        = NULL;

    if( !c3TexList.Head )
        return;

    C3MakePath( path, name, C3_EXT_TEX, &fileNamePath );
    C3MakePath( path, name, C3_EXT_TEXP, &fileNamePal );

    C3ReportStatus("Writing texture script...");
    C3OutputTexScript( path, name );

    // Invoke the texture converter with the script
    C3ReportStatus("Writing texture palette...");

    // Figure out where TexConv.exe should be
    dolphinRoot = getenv( C3_DOLPHIN_ROOT );
    if( dolphinRoot && strlen(dolphinRoot) > 4 && dolphinRoot[0] == '/' && dolphinRoot[1] == '/' )
    {
        // Assuming that format is similar to //d/dolphin (i.e. bash shell path)
        sprintf( c3TexConvPath, "%c:\\%s\\%s", dolphinRoot[2], &dolphinRoot[4], C3_TEXCONV_PATH );
    }
    else
    {
        // Use the default dolphin root path
        sprintf( c3TexConvPath, "%s%s", C3_DOLPHIN_ROOT_DEFAULT, C3_TEXCONV_PATH );
    }

    // Test to see if TexConv.exe is there
    testOpen = fopen( c3TexConvPath, "rb" );
    if( !testOpen )
    {
        C3ReportError( "Could not find texture converter at\n%s. Please check environment variable %s", c3TexConvPath, C3_DOLPHIN_ROOT );
    }
    else
    {
        // Call TexConv.exe with the script file we generated
        fclose( testOpen );
        sprintf( cmdLine, "%s \"%s\" \"%s\"", c3TexConvPath, fileNamePath, fileNamePal ); 
        system( cmdLine ); 
    }

    C3_FREE( fileNamePath );
    C3_FREE( fileNamePal );
}


/*---------------------------------------------------------------------------*
  Name:         C3AddSrcImg

  Description:  Creates a new source image and adds it to the list of source
                images if the fileName is not currently in the list.

  Arguments:    fileName - name of the source image file to get the image from

  Returns:      pointer to the source image
/*---------------------------------------------------------------------------*/
static C3SrcImage*
C3AddSrcImg( char* fileName )
{
    C3SrcImage* srcImage = NULL;
    C3SrcImage* cursor;
    char        buffer[C3_MAX_PATH];

    // Change to a path with forward slashes
    strcpy( buffer, fileName );
    C3MakePathForwardSlash( buffer );

    // Search through the list and see if the filename already exists
    cursor = (C3SrcImage*)c3SrcList.Head;
    while( cursor )
    {
        if( !strcmp( cursor->fileName, buffer ) )
            return cursor;

        cursor = (C3SrcImage*)cursor->link.Next;
    }

    // Otherwise, add the filename to the list
    srcImage = C3PoolSrcImage();
    C3_ASSERT( srcImage );
    srcImage->fileName = C3AddStringInTable( buffer );
    DSInsertListObject( &c3SrcList, NULL, (void*)srcImage );

    return srcImage;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetTexTiling

  Description:  Sets the tiling info for the texture

  Arguments:    tiling - one of the C3Wrap type

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetTexTiling( u8 tiling )
{
    C3_ASSERT( c3Tex ); 

    c3Tex->tiling = tiling;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetTexFilterType

  Description:  Sets the filter type for the current texture.

  Arguments:    Should be one of the C3_FILTER_* constants

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3SetTexFilterType( u8 method )
{
    C3_ASSERT( c3Tex );

    c3Tex->filterType = method;
}


/*---------------------------------------------------------------------------*
  Name:         C3SetImgLOD

  Description:  Sets the LOD parameters for mipmapping per image.

  Arguments:    tiling - one of the C3Wrap type

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void
C3SetImgLOD( u8 minLOD, u8 maxLOD, u8 baseLOD )
{
    C3_ASSERT( c3Tex ); 
    C3_ASSERT( c3Tex->img );

    c3Tex->img->minLOD = minLOD;
    c3Tex->img->maxLOD = maxLOD;
    c3Tex->img->baseLOD = baseLOD;
}


/*---------------------------------------------------------------------------*
  Name:         C3OutputTexScript

  Description:  Outputs the textures texture script as specified in the TplConv
                specifications

  Arguments:    path - path where to create the file
                name - name of the file

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3OutputTexScript( char* path, char* name )
{
    FILE*   outFile         = NULL; 
    char*   fileNamePath    = NULL;


    // For now, always compress texture data
    C3CompressTextureData();

    C3MakePath( path, name, C3_EXT_TEX, &fileNamePath );

    // open the script file 
    outFile = fopen( fileNamePath, "w+" );
    C3_ASSERT( outFile );
    if( !outFile )
    {
        C3ReportError( "Could not open the file %s. File probably already opened",
                        fileNamePath );
        return;
    }

    // output the source image list
    C3OutputSrcImg( outFile, &c3SrcList );
        
    // output the image list
    C3OutputImg( outFile, &c3ImgList );

    // output the palette list 
    C3OutputPal( outFile, &c3PalList );

    // output the texture list
    C3OutputTex( outFile, &c3TexList );

    // close the script file
    fclose( outFile );

    C3_FREE( fileNamePath );
}


/*---------------------------------------------------------------------------*
  Name:         C3OutputSrcImg

  Description:  Outputs the source image information as specified in the TplConv
                specifications

  Arguments:    outFile - file to write to
                list    - list containing the source image information

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3OutputSrcImg( FILE* outFile, DSList* list )
{
    C3SrcImage* cursor  = NULL;
    u32         prev    = -1;

    fprintf( outFile, "\n" );

    cursor = (C3SrcImage*)(list->Head);
    while( cursor )
    {
        if( cursor->index != prev )
        {
            fprintf( outFile, "file %i = %s\n", cursor->index, cursor->fileName );
        }
        
        prev = cursor->index;
        cursor = (C3SrcImage*)(cursor->link.Next);
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3OutputImg

  Description:  Outputs the image information as specified in the TplConv
                specifications

  Arguments:    outFile - file to write to
                list    - list containing the image information

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void 
C3OutputImg( FILE* outFile, DSList* list )
{
    C3Image*    cursor  = NULL;
    u32         prev    = -1;
    
    fprintf( outFile, "\n" );

    cursor = (C3Image*)(list->Head);
    while( cursor )
    {
        if( cursor->index != prev )
        {
            fprintf( outFile, "image %i = ", cursor->index );
            if( cursor->colorMap )
                fprintf( outFile, "%i, ", cursor->colorMap->index );
            else
                fprintf( outFile, "x, " );
            if( cursor->alphaMap )
                fprintf( outFile, "%i, ", cursor->alphaMap->index );
            else
                fprintf( outFile, "x, " );
            fprintf( outFile, "%s", C3GetTexFmtStr( cursor->texFmt ) );
            if ( cursor->maxLOD != 255 )
                fprintf( outFile, ", %i, %i, %i", cursor->minLOD, cursor->maxLOD, cursor->baseLOD );
            fprintf(outFile, "\n");
        }
        
        prev = cursor->index;
        cursor = (C3Image*)(cursor->link.Next);
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3OutputPal
  
  Description:  Outputs the palette information as specified in the TplConv
                specifications

  Arguments:    outFile - file to write to
                list    - list containing the palette information

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void 
C3OutputPal( FILE* outFile, DSList* list )
{
    C3Palette*  cursor  = NULL;
    u32         prev    = -1;
    
    fprintf( outFile, "\n" );

    cursor = (C3Palette*)(list->Head);
    while( cursor )
    {
        if( cursor->index != prev )
        {
            fprintf( outFile, "palette %i = ", cursor->index );
            if( cursor->colorMap )
                fprintf( outFile, "%i, ", cursor->colorMap->index );
            else
                fprintf( outFile, "x, " );
            fprintf( outFile, "%s\n", C3GetTexFmtStr( cursor->texFmt ) );
        }
        
        prev = cursor->index;
        cursor = (C3Palette*)(cursor->link.Next);
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3OutputTex

  Description:  Outputs the textures information as specified in the TplConv
                specifications

  Arguments:    outFile - file to write to
                list    - list containing the texture information

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3OutputTex( FILE* outFile, DSList* list )
{
    C3Texture*  cursor  = NULL;
    u32         prev    = -1;
    
    fprintf( outFile, "\n" );

    cursor = (C3Texture*)(list->Head);
    while( cursor )
    {
        if( cursor->index != prev )
        {
            fprintf( outFile, "texture %i = ", cursor->index );
            if( cursor->img )
                fprintf( outFile, "%i, ", cursor->img->index );
            else
                fprintf( outFile, "x, " );
            if( cursor->pal )
                fprintf( outFile, "%i\n", cursor->pal->index );
            else
                fprintf( outFile, "x\n" );
        }
        
        prev = cursor->index;
        cursor = (C3Texture*)(cursor->link.Next);
    }
}


/*---------------------------------------------------------------------------*
  Name:         C3GetTexFmtStr

  Description:  Return a string for type of the texFmt specified

  Arguments:    texFmt - texture format to get the string for

  Returns:      string for the specified texture format
/*---------------------------------------------------------------------------*/
char*
C3GetTexFmtStr( C3TexFmt texFmt )
{

    switch( texFmt )
    {
        case I4:        return C3_STR( I4 );

        case I8:        return C3_STR( I8 );

        case IA4:       return C3_STR( IA4 );

        case IA8:       return C3_STR( IA8 );

        case RGB565:    return C3_STR( RGB565 );

        case RGB5A3:    return C3_STR( RGB5A3 );

        case RGBA8:     return C3_STR( RGBA8 );

        case CI4:       return C3_STR( CI4 );

        case CI8:       return C3_STR( CI8 );

        case CI14_X2:   return C3_STR( CI14_X2 );

        case CMPR:      return C3_STR( CMPR );

        default:
            C3_ASSERT( C3_FALSE );
            break;      
    }

    return NULL;
}


/*---------------------------------------------------------------------------*
  Name:         C3GetTexFmt

  Description:  Return the type of the texFmt specified in the string

  Arguments:    str - format in all uppercase

  Returns:      type of texture format
/*---------------------------------------------------------------------------*/
C3TexFmt
C3GetTexFmt( char* str )
{
    if ( !str )
        return DONT_KNOW;

    if ( !strcmp(str, C3_STR(I4)) )
    {
        return I4;
    }
    else if ( !strcmp(str, C3_STR(I8)) )
    {
        return I8;
    }
    else if ( !strcmp(str, C3_STR(IA4)) )
    {
        return IA4;
    }
    else if ( !strcmp(str, C3_STR(IA8)) )
    {
        return IA8;
    }
    else if ( !strcmp(str, C3_STR(CI4)) )
    {
        return CI4;
    }
    else if ( !strcmp(str, C3_STR(CI8)) )
    {
        return CI8;
    }
    else if ( !strcmp(str, C3_STR(CI14_X2)) )
    {
        return CI14_X2;
    }
    else if ( !strcmp(str, C3_STR(RGB565)) )
    {
        return RGB565;
    }
    else if ( !strcmp(str, C3_STR(RGBA8)) )
    {
        return RGBA8;
    }
    else if ( !strcmp(str, C3_STR(RGB5A3)) )
    {
        return RGB5A3;
    }
    else if ( !strcmp(str, C3_STR(CMPR)) )
    {
        return CMPR;
    }

    return DONT_KNOW;
}


/*---------------------------------------------------------------------------*
  Name:         C3CompareTextures

  Description:  Compares 2 texture  

  Arguments:    t1 - texture  1
                t2 - texture  2

  Returns:      C3_TRUE if they are the same 
/*---------------------------------------------------------------------------*/
C3Bool
C3CompareTextures( void* t1, void* t2 )
{
    C3Texture*  pt1  = (C3Texture*)t1;
    C3Texture*  pt2  = (C3Texture*)t2;
    C3Bool      same = C3_TRUE;

    // If they are both NULL pointers or the same
    if( pt1 != pt2 )
    {
        if( !pt1 || !pt2 ) // If one of them is NULL
        {
            same = C3_FALSE;
        }
        else
        {
            if( c3TexCompressed )
                same = (pt1->index == pt2->index );
            else
            {
                if( !C3CompareImage( pt1->img, pt2->img) )
                    same = C3_FALSE;
                if( !C3CompareImage( pt1->pal, pt2->pal ))
                    same = C3_FALSE;
            }
        }
    }  
    return same;
}


/*---------------------------------------------------------------------------*
  Name:         C3CompareImage

  Description:  Compares 2 texture images 

  Arguments:    t1 - texture image 1
                t2 - texture image 2

  Returns:      C3_TRUE if they are the same 
/*---------------------------------------------------------------------------*/
C3Bool
C3CompareImage( void* t1, void* t2 )
{
    C3Image*  pt1  = (C3Image*)t1;
    C3Image*  pt2  = (C3Image*)t2;
    C3Bool    same = C3_TRUE;


    // If they are both NULL pointers or the same
    if( pt1 != pt2 )
    {
        if( !pt1 || !pt2 ) // If one of them is NULL
        {
            same = C3_FALSE;
        }
        else
        {
//          if( pt1->minLOD  != pt2->minLOD ||
//              pt1->maxLOD  != pt2->maxLOD ||
//              pt1->baseLOD != pt2->baseLOD )
//              same = C3_FALSE;
            if( pt1->texFmt != pt2->texFmt )
                same = C3_FALSE;
            if( !C3CompareSrcImage( pt1->colorMap, pt2->colorMap ))
                same = C3_FALSE;
            if( !C3CompareSrcImage( pt2->alphaMap, pt1->alphaMap ))
                same = C3_FALSE;
        }
    }  
    return same;
}


/*---------------------------------------------------------------------------*
  Name:         C3ComparePalette

  Description:  Compares 2 texture palette 

  Arguments:    t1 - texture palette 1
                t2 - texture palette 2

  Returns:      C3_TRUE if they are the same 
/*---------------------------------------------------------------------------*/
C3Bool
C3ComparePalette( void* t1, void* t2 )
{
    C3Palette*  pt1  = (C3Palette*)t1;
    C3Palette*  pt2  = (C3Palette*)t2;
    C3Bool      same = C3_TRUE;


    
    // If they are not the same
    if( pt1 != pt2 )
    {
        if( !pt1 || !pt2 ) // If one of them is NULL
        {
            same = C3_FALSE;
        }
        else
        {
            if( pt1->texFmt != pt2->texFmt )
                return C3_FALSE;
            if( !C3CompareSrcImage( pt1->colorMap, pt2->colorMap ))
                return C3_FALSE;
            if( !C3CompareSrcImage( pt2->alphaMap, pt1->alphaMap ))
                return C3_FALSE;
        }
    }  

    return same;
}


/*---------------------------------------------------------------------------*
  Name:         C3CompareSrcImage

  Description:  Compare 2 source image

  Arguments:    t1 - pointer to source image 1
                t2 - pointer to source image 2

  Returns:      C3_TRUE if they are the same
/*---------------------------------------------------------------------------*/
C3Bool
C3CompareSrcImage( void* t1, void* t2 )
{
    C3SrcImage*  pt1  = (C3SrcImage*)t1;
    C3SrcImage*  pt2  = (C3SrcImage*)t2;
    C3Bool       same = C3_TRUE;


    // If they not the same
    if( pt1 != pt2 )
    {
        if( !pt1 || !pt2 ) // If one of them is NULL
        {
            same = C3_FALSE;
        }
        else
        {
            if( strcmp( pt1->fileName, pt2->fileName) != 0 )
            {
                same = C3_FALSE;
            }
        }
    }  
    return same;
}


/*---------------------------------------------------------------------------*
  Name:         C3CompressTextureData

  Description:  Compresses the duplicate data in the texture lists

  Arguments:    NONE

  Returns:      NONE
/*---------------------------------------------------------------------------*/
void 
C3CompressTextureData( void )
{
    u16 tmp = 0;

    if( c3TexCompressed )
        return;

    C3CompressListData( &c3SrcList, HashSrcImage, C3CompareSrcImage );
    C3CompressListData( &c3ImgList, HashImage, C3CompareImage );
    C3CompressListData( &c3PalList, HashPalette, C3ComparePalette );
    C3CompressListData( &c3TexList, HashTextures, C3CompareTextures );

    c3TexCompressed = C3_TRUE;
}


/*---------------------------------------------------------------------------*
  Name:         C3CompressListData

  Description:  Hashes the list according to hashFunc, and removes duplicates
                in each hash index using the compare function.

  Arguments:    list - list to hash
                hashFunc - hash function to use on each element of the list
                compare  - compare function to determine equality of 
                           elements in list

  Returns:      NONE
/*---------------------------------------------------------------------------*/
static void
C3CompressListData( DSList* list, DSHashFunc *hashFunc, C3LinkCompareFuncPtr compare )
{
    DSHashTable hTable;
    DSList      listArray[ HASH_TABLE_SIZE ];
    C3SrcImage  srcImage;
    u16         numUnique;

    // Initialize the hash table
    DSInitHTable( &hTable, 
                  HASH_TABLE_SIZE, 
                  listArray,
                  hashFunc,
                  (Ptr)&srcImage, 
                  &(srcImage.link) );

    // Compress the indices
    C3CompressIndices( list, &numUnique, compare, &hTable, NULL );  
}


/*---------------------------------------------------------------------------*
  Name:         HashSrcImage

  Description:  Hash function for a srcImage

  Arguments:    srcImage - srcImage to hash

  Returns:      index for the hash table 
/*---------------------------------------------------------------------------*/
static u16 
HashSrcImage( void* srcImage )
{
    u32 checksum = 0;
    u32 i;
    char* fileName = NULL;

    if ( !srcImage )
        return 0;

    fileName = ((C3SrcImage*)srcImage)->fileName;
    if ( !fileName )
        return 0;

    for ( i = 0; i < strlen(fileName); i++ )
    {
        checksum += (u32)fileName[i];
    }

    return (u16)(checksum % HASH_TABLE_SIZE);
}


/*---------------------------------------------------------------------------*
  Name:         HashImage

  Description:  Hash function for an image

  Arguments:    image - image to hash

  Returns:      index for the hash table 
/*---------------------------------------------------------------------------*/
static u16 
HashImage( void* image )
{
    u32 checksum = 0;
    u32 i;
    char* colorMapFileName = NULL;
    char* alphaMapFileName = NULL;

    if ( !image )
        return 0;

    if ( ((C3Image*)image)->colorMap )
        colorMapFileName = ((C3Image*)image)->colorMap->fileName;
    if ( ((C3Image*)image)->alphaMap )
        alphaMapFileName = ((C3Image*)image)->alphaMap->fileName;

    if ( colorMapFileName )
        for ( i = 0; i < strlen(colorMapFileName); i++ )
            checksum += (u32)colorMapFileName[i];
    if ( alphaMapFileName )
        for ( i = 0; i < strlen(alphaMapFileName); i++ )
            checksum += (u32)alphaMapFileName[i];

    checksum += (u32)((C3Image*)image)->texFmt; 

    return (u16)(checksum % HASH_TABLE_SIZE);
}


/*---------------------------------------------------------------------------*
  Name:         HashPalette

  Description:  Hash function for a palette

  Arguments:    palette - palette to hash

  Returns:      index for the hash table 
/*---------------------------------------------------------------------------*/
static u16 
HashPalette( void* palette )
{
    u32 checksum = 0;
    u32 i;
    char* colorMapFileName = NULL;
    char* alphaMapFileName = NULL;

    if ( !palette )
        return 0;

    if ( ((C3Palette*)palette)->colorMap )
        colorMapFileName = ((C3Palette*)palette)->colorMap->fileName;
    if ( ((C3Palette*)palette)->alphaMap )
        alphaMapFileName = ((C3Palette*)palette)->alphaMap->fileName;

    if ( colorMapFileName )
        for ( i = 0; i < strlen(colorMapFileName); i++ )
            checksum += (u32)colorMapFileName[i];
    if ( alphaMapFileName )
        for ( i = 0; i < strlen(alphaMapFileName); i++ )
            checksum += (u32)alphaMapFileName[i];

    checksum += (u32)((C3Palette*)palette)->texFmt; 

    return (u16)(checksum % HASH_TABLE_SIZE);
}


/*---------------------------------------------------------------------------*
  Name:         HashTextures

  Description:  Hash function for a texture

  Arguments:    texture - texture to hash

  Returns:      index for the hash table 
/*---------------------------------------------------------------------------*/
static u16 
HashTextures( void* texture )
{
    u32 checksum = 0;

    if ( !texture )
        return 0;

    checksum += HashImage( ((C3Texture*)texture)->img );
    checksum += HashPalette( ((C3Texture*)texture)->pal );

    return (u16)(checksum % HASH_TABLE_SIZE);
}
