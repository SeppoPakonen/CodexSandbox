/*******************************************************************************
 * 
 *   (C) 1999 ARTX INC..  ALL RIGHTS RESERVED.  UNPUBLISHED -- RIGHTS
 *   RESERVED UNDER THE COPYRIGHT LAWS OF THE UNITED STATES.  USE OF A
 *   COPYRIGHT NOTICE IS PRECAUTIONARY ONLY AND DOES NOT IMPLY PUBLICATION
 *   OR DISCLOSURE.
 *
 *   THIS SOFTWARE CONTAINS CONFIDENTIAL INFORMATION AND TRADE SECRETS OF
 *   ARTX INC..  USE, DISCLOSURE, OR REPRODUCTION IS PROHIBITED WITHOUT
 *   THE PRIOR EXPRESS WRITTEN PERMISSION OF ARTX INC..
 *
 *                   RESTRICTED RIGHTS LEGEND
 *
 *   Use, duplication, or disclosure by the Government is subject to
 *   restrictions as set forth in subparagraph (c)(1)(ii) of the Rights
 *   in Technical Data and Computer Software clause at DFARS 252.227-7013
 *   or subparagraphs (c)(1) and (2) of Commercial Computer Software --
 *   Restricted Rights at 48 CFR 52.227-19, as applicable. 
 *
 *   ArtX Inc.
 *   3400 Hillview Ave, Bldg 5
 *   Palo Alto, CA 94304
 *
 ******************************************************************************/

#ifndef __GXBUMP_H__
#define __GXBUMP_H__

#include "types.h"


//******************************************************************************
//
//  Graphics Library Indirect Texture (Bump) functions.
//  
//******************************************************************************

#if 0
extern "C" {
#endif

//------------------------------------------------------------------------------
// Bump (indirect texture functions).
//------------------------------------------------------------------------------
extern void GXSetNumIndStages (u8 nIndStages);
extern void GXSetIndTexOrder (GXIndTexStageID ind_stage, GXTexCoordID tex_coord,
			      GXTexMapID tex_map);
extern void GXSetIndTexCoordScale (GXIndTexStageID ind_state, 
				   GXIndTexScale scale_s, GXIndTexScale scale_t);
extern void GXSetIndTexMtx (GXIndTexMtxID mtx_sel, f32 offset[2][3], s8 scale_exp);
extern void GXSetTevIndirect (GXTevStageID tev_stage, GXIndTexStageID ind_stage,
		      GXIndTexFormat format, GXIndTexBiasSel bias_sel, 
		      GXIndTexMtxID matrix_sel,
		      GXIndTexWrap wrap_s, GXIndTexWrap wrap_t,
		      GXBool add_prev, GXBool ind_lod, GXIndTexAlphaSel alpha_sel);

//------------------------------------------------------------------------------
// Convinience functions
//------------------------------------------------------------------------------
extern void GXSetTevDirect (GXTevStageID tev_stage);
extern void GXSetTevIndWarp (GXTevStageID tev_stage, GXIndTexStageID ind_stage,
			     GXBool signed_offset, GXBool replace_mode,
			     GXIndTexMtxID matrix_sel);
extern void GXSetTevIndTile (GXTevStageID tev_stage, GXIndTexStageID ind_stage,
			     u16 tilesize_s, u16 tilesize_t, 
			     u16 tilespacing_s, u16 tilespacing_t, 
			     GXIndTexFormat format, GXIndTexMtxID matrix_sel,
			     GXIndTexBiasSel bias_sel, GXIndTexAlphaSel alpha_sel);
extern void GXSetTevIndBumpST (GXTevStageID tev_stage, GXIndTexStageID ind_stage, 
			       GXIndTexMtxID matrix_sel);
extern void GXSetTevIndBumpXYZ (GXTevStageID tev_stage, GXIndTexStageID ind_stage, 
			       GXIndTexMtxID matrix_sel);
extern void GXSetTevIndRepeat (GXTevStageID tev_stage);

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#if 0
}
#endif

#endif // __GXBUMP_H__
