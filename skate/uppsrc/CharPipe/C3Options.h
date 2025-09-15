/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3Options.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/include/charPipeline/c3/C3Options.h $
    
    8     11/13/00 1:59p John
    Removed texture filter option since it does not belong within C3
    library (should be controlled from C3 library caller).
    
    7     2/29/00 7:18p John
    Renamed C3ProcessOptionsAfterOptimization to
    C3ProcessOptionsAfterCompression.
    
    6     2/04/00 6:06p John
    Untabified code.
    
    5     1/20/00 1:13p John
    Added stitching (work in progress).
    
    4     1/04/00 6:58p John
    Added little/big endian output  option
    
    3     1/04/00 1:12p John
    Added better support for second texture channel.
    
    2     12/20/99 11:21a John
    Added second texture channel.
    
    6     11/11/99 1:11p John
    Added option to view strip/fan effectiveness.
    
    5     10/13/99 2:59p John
    Changed position quantization back to a power of 2 scale with a given
    ruler.  
    Added lighting with ambient percentage.
    Added default normal table.
    
    4     10/01/99 11:13a John
    Added position and tex. coordinate quantization per object.
    
    3     9/29/99 4:25p John
    Changed header to make them all uniform with logs.
    Added position and normal quantization.
    Added external normal table.
    
    2     9/17/99 9:33a John
    Added C3Options and cleaned up code.
    
  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _C3OPTIONS_H
#define _C3OPTIONS_H

#if 0
extern "C" {
#endif

typedef struct
{
    DSLink  link;
    char*   identifier;             // identifier of geomObj this option is for
    u8      texCoordQuantization[C3_MAX_TEXTURES];  // holds type and shift after computed

} C3OptionObj;

void    C3InitOptions( );
void    C3ProcessOptionsAfterCompression();

C3OptionObj* C3NewOptionObj( char* identifier );
void         C3AddOptionObj( C3OptionObj* obj );
DSList*      C3GetOptionObjList( );
C3OptionObj* C3GetOptionObjFromId( char* identifier );

void    C3SetOptionReportStatusFunc( C3ReportStatusFunc func );
void    C3SetOptionReportErrorFunc( C3ReportErrorFunc func );
C3ReportStatusFunc C3GetOptionReportStatusFunc( );
C3ReportErrorFunc  C3GetOptionReportErrorFunc( );

void    C3SetOptionOutputEndian( u8 endianType );
u8      C3GetOptionOutputEndian( );

void    C3SetOptionSrcVertexOrder( u8 vtxOrder );
u8      C3GetOptionSrcVertexOrder( );

void    C3SetOptionFileExportFlag( u32 fileExportflag );
u32     C3GetOptionFileExportFlag( );

void    C3SetOptionUseDefaultNormalTable( C3Bool flag );
C3Bool  C3GetOptionUseDefaultNormalTable( );

void    C3SetOptionUseExternalNormalTable( C3Bool flag );
C3Bool  C3GetOptionUseExternalNormalTable( );

void    C3SetOptionExternalNormalTablePath( char* name );
char*   C3GetOptionExternalNormalTablePath( );

void    C3SetOptionEnableStripFan( C3Bool flag );
C3Bool  C3GetOptionEnableStripFan( );
void    C3SetOptionEnableStripFanView( C3Bool flag );
C3Bool  C3GetOptionEnableStripFanView( );

void    C3SetOptionCompress( u16 targets );
u16     C3GetOptionCompress( );

void    C3SetOptionPositionRange( f32 range );
f32     C3GetOptionPositionRange( );

void    C3SetOptionQuantization( u32 target, u8 channel, u8 quantInfo );
u8      C3GetOptionQuantization( u32 target, u8 channel );
char*   C3GetOptionQuantizationString( u16 target, u8 type );

void    C3SetOptionEnableStitching( C3Bool flag );
C3Bool  C3GetOptionEnableStitching( );

void    C3SetOptionEnableLighting( C3Bool flag );
C3Bool  C3GetOptionEnableLighting( );

void    C3SetOptionAmbientPercentage( f32 percent );
f32     C3GetOptionAmbientPercentage( );

void    C3SetOptionWeldRadius( u32 target, f32 radius );
f32     C3GetOptionWeldRadius( u32 target );

#if 0
}
#endif

#endif // _C3OPTIONS_H
