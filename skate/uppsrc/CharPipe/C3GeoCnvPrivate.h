/*---------------------------------------------------------------------------*
  Project: [C3Lib]
  File:    [C3GeoCnvPrivate.h]

  Copyright 1998-2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /dolphin/build/charPipeline/c3/include/C3GeoCnvPrivate.h $
    
    6     3/23/00 2:11p John
    Modified function names to runtime library changes.
    
    5     2/18/00 4:53p John
    Now assigns vertex stitching information after hierarchy extraction
    process.
    
    4     2/04/00 6:05p John
    Untabified code.
    
    3     1/20/00 1:12p John
    Added stitching (work in progress).
    
    2     1/04/00 12:07p John
    Added better support for second texture channel.
    
    3     9/29/99 4:18p John
    Changed header to make them all uniform with logs.
    Changed VCD structure to match the GPL format.
    
    2     6/02/99 11:35a Rod
    Added defines
    Cosmetic changes

  $NoKeywords: $

 *---------------------------------------------------------------------------*/

#ifndef _C3GEOCNVPRIVATE_H
#define _C3GEOCNVPRIVATE_H

#if 0
extern "C"{
#endif

#define C3_MAXPRIM_VTX          UINT_MAX
#define C3_PRIM_HEADER_SIZE     3

#define C3_VCD_MASK             0x00000003  // Shift first, then apply the mask

#define C3_VCD_SHIFT_POS_MATRIX         0
#define C3_VCD_SHIFT_POSITION           2
#define C3_VCD_SHIFT_NORMAL             4
#define C3_VCD_SHIFT_COLOR_0            6
#define C3_VCD_SHIFT_COLOR_1            8
#define C3_VCD_SHIFT_TCOORD_BASE        10
#define C3_VCD_SHIFT_TCOORD_INCREMENT   2

typedef struct
{
    DOLayout*   layout;

    char*       strBank;
    u32         strBankSize;

} C3DOLayout;

typedef struct
{
    GEOPalette* geoPal;

    char*       strBank;
    u32         strBankSize;

    C3DOLayout* layoutArray;

} C3GeoPalette;


void        C3ConvertToGeoPalette ( DSList*         geomList, 
                                    C3GeoPalette**  geoPal,
                                    char*           texPalName );
void        C3FreeGeoPalette      ( C3GeoPalette**  geoPal );
DOLayout*   C3ConvertToDOLayout   ( C3GeomObject*   geomObj, 
                                    char**          strBank,
                                    u32*            strBankSize );
void        C3AssignBoneIndices   ( C3GeomObject    *geomObj );
void        C3FreeDOLayout        ( DOLayout**      doLayout );

#if 0
}
#endif


#endif // _C3GEOCNVPRIVATE_H
