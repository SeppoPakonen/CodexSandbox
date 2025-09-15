/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3CnvOpzPrivate.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/build/charPipeline/c3/include/C3CnvOpzPrivate.h $
    
    4     11/13/00 1:26p John
    Added C3TransformData to transform geometry, hierarchy, and animation
    to compensate for new API: C3SetPivotOffset, where rotate and scale
    pivot points are not the same as the translation.
    
    3     2/04/00 6:05p John
    Untabified code.
    
    2     1/04/00 12:07p John
    Added better support for second texture channel.
    
    11    11/11/99 1:08p John
    New Strip/Fan code. Added option to view strip/fan effectiveness.
    Fixed bug in C3CompareQuat and C3CompareVec.
    
    10    9/29/99 4:18p John
    Changed header to make them all uniform with logs.
    Added position and normal quantization.
    Added external normal table.
    
  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _C3CNVOPZPRIVATE_H
#define _C3CNVOPZPRIVATE_H

#if 0
extern "C" {
#endif // __cplusplus

// Value for the descriptors ( gx.h )
#define C3_VCD_NONE          GX_NONE
#define C3_VCD_INDEX16       GX_INDEX16
#define C3_VCD_INDEX8        GX_INDEX8
#define C3_VCD_DIRECT        GX_DIRECT
#define C3_VCD_DIFFUSE_CLR   GEN_DIFFUSE_CLR

typedef C3Bool(*C3LinkCompareFuncPtr)(void*, void*);
typedef void(*C3CopyFuncPtr)(void*, void*);


void    C3InitConversionOptimization ( );
void    C3CleanConversionOptimization( );

void    C3TransformData             ();

void    C3QuantizeFloat             ( void* dest, u8 destCode, float src);
void    C3QuantizeFloatScaleTrans   ( void* dest, u8 destCode, f32 src, f32 scale, f32 translate );
void    C3QuantizeColor             ( void* dest, u8 destCode, C3Color* color );
f32     C3FloatFromQuant            ( void* src, u8 srcCode );
void    C3ColorFromQuant            ( void* src, u8 srcCode, u8* r, u8* g, u8* b, u8* a );

C3Bool  C3CompareTextures           ( void* t1, void* t2 );

void    C3MakeIndexedListDataUnique  ( DSList*               list,
                                       u16*                  numUnique, 
                                        C3LinkCompareFuncPtr  linkAreSame,
                                        u16                   baseIndex,
                                        C3CopyFuncPtr         copy );
void    C3MakeIndexedHTableDataUnique( DSHashTable*          hTable,
                                       u16                   tableIndex,
                                       u16*                  numUnique, 
                                       C3LinkCompareFuncPtr  linkAreSame,
                                       u16                   baseIndex,
                                       C3CopyFuncPtr         copy );
void    C3ConvertToStripFan             ( C3GeomObject* geomObj );
u8      C3GetComponentSize              ( u32 target, u8 channel );
u8      C3GetComponentSizeFromQuantInfo ( u32 target, u8 quantInfo );

void    C3SetVCDDataType            ( C3GeomObject* obj, u32 target, u16 indexQuantType, u8 channel );
u16     C3GetVCDDataType            ( C3GeomObject* obj, u32 target, u8 channel );

void    C3CompressIndices           ( DSList*              list,
                                      u16*                 numUnique, 
                                      C3LinkCompareFuncPtr linkAreSame,
                                      DSHashTable*         hTable,
                                      C3CopyFuncPtr        copy );

void    C3ComputePositionQuantization( u8 quantInfo, C3Position *minPos, C3Position *maxPos,
                                       C3Position *scale,  C3Position *translate );


#if 0
}
#endif // __cplusplus


#endif // _C3CNVOPZPRIVATE_H
