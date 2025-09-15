/**********************************************************
 * Copyright (C) 2001 TheyerGFX Pty Ltd
 * All Rights Reserved
 **********************************************************
 * Project:		California Watersports PlayStation2
 **********************************************************
 * File:		testing.c
 * Author:		Mark Theyer
 * Created:		01 May 1997
 **********************************************************
 * Description: Testing stuff not production software
 **********************************************************
 * Functions:
 *	fname() 
 *				func desc.
 **********************************************************
 * Revision History:
 *
 * DATE			AUTHOR	DESCRIPTION OF CHANGE
 *
 * 01-May-97	Theyer	Initial coding.
 * 06-Aug-99	Theyer  Initial update for new game.
 * 31-May-01	Theyer  Ported to PS2
 *
 **********************************************************/

/*
 * includes
 */

#include <surfgame.h>

#include <type/datatype.h>
#include <gfx/gfx.h>
#include <gfx/gfxps2.h>
#include <triangle/triangle.h>
#include <text/text.h>
#include <parse/parse.h>
#include <memory/memory.h>

#define VIFCURSOR_H
#include <dmadebug.h>

/*
 * macros
 */

/*
 * typedefs
 */

/*
 * prototypes
 */

/*
 * global variables
 */

u_long test_tags[64] = {
	// 0-3 sprites
    SCE_GIF_SET_TAG(
        1,							// NLOOP
        0,							// EOP (End Of Packet)
        1,							// PRE (PRIM field Enable, PACKED MODE only)
        SCE_GS_SET_PRIM(			// PRIM (if PRE = 1)
            SCE_GS_PRIM_SPRITE,		// PRIM (Type of drawing primitive)
            0,                      // IIP (Flat Shading/Gouraud Shading)
            1,                      // TME (Texture Mapping Enabled)
            0,                      // FGE (Fogging Enabled)
            1,                      // ABE (Alpha Blending Enabled)
            0,                      // AA1 (1 Pass Anti-Aliasing Enabled)
            1,                      // FST (Method of specifying Texture Coords)
            0,                      // CTXT (Context)
            0                       // FIX (Fragment Value Control)
            ),
        0,					        // FLG (Data format, 0=PACKED, 1=REGLIST, 2=IMAGE)
        3),							// NREG (Number of registers)
    0xEEEL,							// REGS=TEXA,TEX1,TEXFLUSH
    SCE_GIF_SET_TAG(
        1,							// NLOOP
        1,							// EOP (End Of Packet)
        0,							// PRE (PRIM field Enable, PACKED MODE only)
        0,							// PRIM (if PRE = 1)
        1,					        // FLG (Data format, 0=PACKED, 1=REGLIST, 2=IMAGE)
        6),							// NREG (Number of registers)
    0x535361L,						// REGS=XYZ2,UV,XYZ2,UV,TEX0,RGBAQ
	// 4-7 textured triangles
    SCE_GIF_SET_TAG(
        1,							// NLOOP
        0,							// EOP (End Of Packet)
        1,							// PRE (PRIM field Enable, PACKED MODE only)
        SCE_GS_SET_PRIM(			// PRIM (if PRE = 1)
            SCE_GS_PRIM_TRISTRIP,	// PRIM (Type of drawing primitive)
            1,                      // IIP (Flat Shading/Gouraud Shading)
            1,                      // TME (Texture Mapping Enabled)
            0,                      // FGE (Fogging Enabled)
            1,                      // ABE (Alpha Blending Enabled)
            0,                      // AA1 (1 Pass Anti-Aliasing Enabled)
            1,                      // FST (Method of specifying Texture Coords)
            0,                      // CTXT (Context)
            0                       // FIX (Fragment Value Control)
            ),
        0,					        // FLG (Data format, 0=PACKED, 1=REGLIST, 2=IMAGE)
        3),							// NREG (Number of registers)
    0xEEEL,							// REGS=TEXA,TEX1,ALPHA1
    SCE_GIF_SET_TAG(
        1,							// NLOOP
        1,							// EOP (End Of Packet)
        0,							// PRE (PRIM field Enable, PACKED MODE only)
        0,							// PRIM (if PRE = 1)
        1,					        // FLG (Data format, 0=PACKED, 1=REGLIST, 2=IMAGE)
       10),							// NREG (Number of registers)
    0x5353535361L,					// REGS=XYZ2,UV,XYZ2,UV,XYZ2,UV,XYZ2,UV,TEX0,RGBAQ
	// 8-11 textured triangles
    SCE_GIF_SET_TAG(
        4,							// NLOOP
        1,							// EOP (End Of Packet)
        0,							// PRE (PRIM field Enable, PACKED MODE only)
        0,							// PRIM (if PRE = 1)
        0,					        // FLG (Data format, 0=PACKED, 1=REGLIST, 2=IMAGE)
        1),							// NREG (Number of registers)
    0xEL,							// REGS=TEX0,TEX1,TEXA,ALPHA1
    SCE_GIF_SET_TAG(
        9,							// NLOOP
        1,							// EOP (End Of Packet)
        1,							// PRE (PRIM field Enable, PACKED MODE only)
        SCE_GS_SET_PRIM(			// PRIM (if PRE = 1)
            SCE_GS_PRIM_TRISTRIP,	// PRIM (Type of drawing primitive)
            1,                      // IIP (Flat Shading/Gouraud Shading)
            1,                      // TME (Texture Mapping Enabled)
            0,                      // FGE (Fogging Enabled)
            1,                      // ABE (Alpha Blending Enabled)
            0,                      // AA1 (1 Pass Anti-Aliasing Enabled)
            1,                      // FST (Method of specifying Texture Coords)
            0,                      // CTXT (Context)
            0                       // FIX (Fragment Value Control)
            ),
        0,					        // FLG (Data format, 0=PACKED, 1=REGLIST, 2=IMAGE)
        1),							// NREG (Number of registers)
    0xEL,							// REGS=XYZ2,UV,XYZ2,UV,XYZ2,UV,XYZ2,UV,RGBAQ
	// 12 textured triangles
    SCE_GIF_SET_TAG(
        0,							// NLOOP
        0,							// EOP (End Of Packet)
        0,							// PRE (PRIM field Enable, PACKED MODE only)
        SCE_GS_SET_PRIM(			// PRIM (if PRE = 1)
            0,						// PRIM (Type of drawing primitive)
            0,                      // IIP (Flat Shading/Gouraud Shading)
            0,                      // TME (Texture Mapping Enabled)
            0,                      // FGE (Fogging Enabled)
            1,                      // ABE (Alpha Blending Enabled)
            0,                      // AA1 (1 Pass Anti-Aliasing Enabled)
            0,                      // FST (Method of specifying Texture Coords)
            0,                      // CTXT (Context)
            0                       // FIX (Fragment Value Control)
            ),
        0,					        // FLG (Data format, 0=PACKED, 1=REGLIST, 2=IMAGE)
        0),							// NREG (Number of registers)
	// 13-16 textured triangles
    SCE_GIF_SET_TAG(
        4,							// NLOOP
        1,							// EOP (End Of Packet)
        0,							// PRE (PRIM field Enable, PACKED MODE only)
        0,							// PRIM (if PRE = 1)
        0,					        // FLG (Data format, 0=PACKED, 1=REGLIST, 2=IMAGE)
        1),							// NREG (Number of registers)
    0xEL,							// REGS=TEX0,TEX1,TEXA,ALPHA1
    SCE_GIF_SET_TAG(
       12,							// NLOOP
        1,							// EOP (End Of Packet)
        1,							// PRE (PRIM field Enable, PACKED MODE only)
        SCE_GS_SET_PRIM(			// PRIM (if PRE = 1)
            SCE_GS_PRIM_TRISTRIP,	// PRIM (Type of drawing primitive)
            1,                      // IIP (Flat Shading/Gouraud Shading)
            1,                      // TME (Texture Mapping Enabled)
            0,                      // FGE (Fogging Enabled)
            1,                      // ABE (Alpha Blending Enabled)
            0,                      // AA1 (1 Pass Anti-Aliasing Enabled)
            1,                      // FST (Method of specifying Texture Coords)
            0,                      // CTXT (Context)
            0                       // FIX (Fragment Value Control)
            ),
        0,					        // FLG (Data format, 0=PACKED, 1=REGLIST, 2=IMAGE)
        1),							// NREG (Number of registers)
    0xEL,							// REGS=XYZ2,UV,RGBAQ,XYZ2,UV,RGBAQ,XYZ2,UV,RGBAQ,XYZ2,UV,RGBAQ
};

extern SurfData	surf;
extern Ps2Data  ps2;

/*
 * functions
 */


/**********************************************************
 * Function:	test_alpha
 **********************************************************
 * Description: 
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void test_alpha ( 
	int				id,
	Bool			sprite,
	float			r,
	float			g,
	float			b,
	float			x, 
	float			y, 
	float			sx, 
	float			sy, 
	float			trans,
	float			depth
	)
{
	Ps2Packet	*data;
	Ps2Color	 col = { 127, 127, 127, 127 };
	Ps2Texture	*tex;
	Ps2Sprite	*sp;
	int			 x1, y1;
	int			 x2, y2;
	u_int		*rgba;
	u_int		 z;
	int			 width;
	int			 height;

	/* init (no validation!) */
	col.r = (Byte)(r * 255.0f);
	col.g = (Byte)(g * 255.0f);
	col.b = (Byte)(b * 255.0f);
	if ( sprite ) {
		sp  = &ps2.sprites[id-1];
		tex = &ps2.textures[(sp->texid - 1)];
		width  = sp->width;
		height = sp->height;
	} else {
		sp  = NULL;
		tex = &ps2.textures[id-1];
		width  = tex->width;
		height = tex->height;
	}
	data  = ps2.aligned_buffer;
	rgba  = (u_int *)&col;
		
	/* create DMA Tag to draw sprite */
	data[0].b64[0] = ps2DmaEndTag(128);										// DMA tag (size=128 bytes)
	data[0].b64[1] = 0x0L;													// padding
	// GIF Tag
	data[1].b64[0] = test_tags[0];											// GIFTAG
	data[1].b64[1] = test_tags[1];											// REGS
	// ALPHA1 (0x42)
	data[2].b32[0] = 0x01;													// ALPHA1
	data[2].b32[1] = 0x00;													// ALPHA1
	data[2].b64[1] = 0x42L;													// A+D=ALPHA1 (0x42)
	// TEX1 (0x14)
	data[3].b64[0] = 0x60L;													// TEX1
	data[3].b64[1] = 0x14L;													// A+D=TEX1 (0x14)
	// TEXA (0x3B)
	data[4].b32[0] = 0x00;													// TEXA
	data[4].b32[1] = 0x7F;													// TEXA (7F,127=1.0f)
	data[4].b64[1] = 0x3BL;													// A+D=TEXA (0x3B)
	// GIF Tag
	data[5].b64[0] = test_tags[2];											// GIFTAG
	data[5].b64[1] = test_tags[3];											// REGS
	// RGBA(Q)
	data[6].b32[0] = *rgba;													// RGBA
	data[6].f32[1] = 1.0f;													// (Q)
	// TEX0
	data[6].b64[1] = tex->tex0;												// TEX0
	// UV, XYZ2
	data[7].b32[0] = 0x80008;												// UV (add 0.5, 0.5 to UV)
	data[7].b32[1] = 0x00;													// UV
	data[7].b64[1] = 0x0L;													// XYZ2
	// UV, XYZ2
	data[8].b32[0] = (((height - 1)<<20)|((width - 1)<<4));					// UV
	data[8].b32[1] = 0x0;													// UV
	data[8].b64[1] = 0x0L;													// XYZ2

	/* find x and y (works for 640x224 display only!) */
	x1 = 1728 + (int)(x * 640.0f);
	y1 = 1936 + (int)(y * 224.0f);
	x2 = x1 + (int)(((float)width)  * sx * 0.0015625f  * 640.0f);
	y2 = y1 + (int)(((float)height) * sy * 0.00446429f * 224.0f);	
	data[7].b32[2] = (y1<<20)|(x1<<4);				// XYZ2
	data[8].b32[2] = (y2<<20)|(x2<<4);				// XYZ2

	/* find z (works for 24 bit Z buffer only!) */
	z = (u_int)(16777215.0f * (1.0f - depth));
	data[7].b32[3] = z;								// XYZ2
	data[8].b32[3] = z;								// XYZ2
	//printf( "tex->height=%d, width=%d\n", tex->height, tex->width );

	/* transparency */
	data[4].b32[1] = (int)(trans * 127.0f);			// TEXA (7F,127=1.0f)

	/* flush the write buffer (until we work out the problem with uncached memory) */
	ps2FlushWriteBuffer();
	FlushCache(0);

	/* send the sprite drawing code directly to the GIF */
	sceGsSyncPath( 0, 0 );
	sceDmaSync( ps2.dma_channel_GIF,  0, 0 );
	ps2DmaTransferEnable( ps2.dma_channel_GIF, FALSE );
    sceDmaSend( ps2.dma_channel_GIF, (u_long128 *)data );
}


/**********************************************************
 * Function:	test_alpha2
 **********************************************************
 * Description: 
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void test_alpha2 ( 
	int				id,
	Bool			sprite,
	float			r,
	float			g,
	float			b,
	float			x, 
	float			y, 
	float			sx, 
	float			sy, 
	float			trans,
	float			depth
	)
{
	Ps2Sprite	*sp;

	/* init (no validation!) */
	if ( sprite ) {
		sp  = &ps2.sprites[id-1];
		test_alpha( sp->texid, 0, r, g, b, x, y, sx, sy, trans, depth );
	}
}


/**********************************************************
 * Function:	test_strip
 **********************************************************
 * Description: 
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void test_strip ( 
	int				id,
	Bool			sprite,
	float			r,
	float			g,
	float			b,
	float			x, 
	float			y, 
	float			sx, 
	float			sy, 
	float			trans,
	float			depth
	)
{
	Ps2Packet	*data;
	Ps2Color	 col = { 127, 127, 127, 127 };
	Ps2Texture	*tex;
	Ps2Sprite	*sp;
	int			 x1, y1;
	int			 x2, y2;
	u_int		*rgba;
	u_int		 z;
	int			 width;
	int			 height;

	/* init (no validation!) */
	col.r = (Byte)(r * 255.0f);
	col.g = (Byte)(g * 255.0f);
	col.b = (Byte)(b * 255.0f);
	if ( sprite ) {
		sp  = &ps2.sprites[id-1];
		tex = &ps2.textures[(sp->texid - 1)];
		width  = sp->width;
		height = sp->height;
	} else {
		sp  = NULL;
		tex = &ps2.textures[id-1];
		width  = tex->width;
		height = tex->height;
	}
	data  = ps2.aligned_buffer;
	rgba  = (u_int *)&col;
		
	/* create DMA Tag to draw sprite */
	data[0].b64[0] = ps2DmaEndTag(160);										// DMA tag (size=160 bytes)
	data[0].b64[1] = 0x0L;													// padding
	// GIF Tag
	data[1].b64[0] = test_tags[4];											// GIFTAG
	data[1].b64[1] = test_tags[5];											// REGS
	// ALPHA1 (0x42)
	data[2].b32[0] = 0x01;													// ALPHA1
	data[2].b32[1] = 0x00;													// ALPHA1
	data[2].b64[1] = 0x42L;													// A+D=ALPHA1 (0x42)
	// TEX1 (0x14)
	data[3].b64[0] = 0x60L;													// TEX1
	data[3].b64[1] = 0x14L;													// A+D=TEX1 (0x14)
	// TEXA (0x3B)
	data[4].b32[0] = 0x00;													// TEXA
	data[4].b32[1] = 0x7F;													// TEXA (7F,127=1.0f)
	data[4].b64[1] = 0x3BL;													// A+D=TEXA (0x3B)
	// GIF Tag
	data[5].b64[0] = test_tags[6];											// GIFTAG
	data[5].b64[1] = test_tags[7];											// REGS
	// RGBA(Q)
	data[6].b32[0] = *rgba;													// RGBA
	data[6].f32[1] = 1.0f;													// (Q)
	// TEX0
	data[6].b64[1] = tex->tex0;												// TEX0
	// UV, XYZ2
	data[7].b32[0] = 0x80008;												// UV (add 0.5, 0.5 to UV)
	data[7].b32[1] = 0x00;													// UV
	data[7].b64[1] = 0x0L;													// XYZ2
	// UV, XYZ2
	data[8].b32[0] = ((width - 1)<<4);										// UV
	data[8].b32[1] = 0x00;													// UV
	data[8].b64[1] = 0x0L;													// XYZ2
	// UV, XYZ2
	data[9].b32[0] = ((height - 1)<<20);									// UV
	data[9].b32[1] = 0x00;													// UV
	data[9].b64[1] = 0x0L;													// XYZ2
	// UV, XYZ2
	data[10].b32[0] = (((height - 1)<<20)|((width - 1)<<4));				// UV
	data[10].b32[1] = 0x0;													// UV
	data[10].b64[1] = 0x0L;													// XYZ2

	/* find x and y (works for 640x224 display only!) */
	x1 = 1728 + (int)(x * 640.0f);
	y1 = 1936 + (int)(y * 224.0f);
	x2 = x1 + (int)(((float)width)  * sx * 0.0015625f  * 640.0f);
	y2 = y1 + (int)(((float)height) * sy * 0.00446429f * 224.0f);	
	data[7].b32[2]  = (y1<<20)|(x1<<4);				// XYZ2
	data[8].b32[2]  = (y1<<20)|(x2<<4);				// XYZ2
	data[9].b32[2]  = (y2<<20)|(x1<<4);				// XYZ2
	data[10].b32[2] = (y2<<20)|(x2<<4);				// XYZ2

	/* find z (works for 24 bit Z buffer only!) */
	z = (u_int)(16777215.0f * (1.0f - depth));
	data[7].b32[3]  = z;							// XYZ2
	data[8].b32[3]  = z;							// XYZ2
	data[9].b32[3]  = z;							// XYZ2
	data[10].b32[3] = z;							// XYZ2
	//printf( "tex->height=%d, width=%d\n", tex->height, tex->width );

	/* transparency */
	data[4].b32[1] = (int)(trans * 127.0f);			// TEXA (7F,127=1.0f)

	/* flush the write buffer (until we work out the problem with uncached memory) */
	ps2FlushWriteBuffer();
	FlushCache(0);

	/* send the sprite drawing code directly to the GIF */
	sceGsSyncPath( 0, 0 );
	sceDmaSync( ps2.dma_channel_GIF,  0, 0 );
	ps2DmaTransferEnable( ps2.dma_channel_GIF, FALSE );
    sceDmaSend( ps2.dma_channel_GIF, (u_long128 *)data );
}


/**********************************************************
 * Function:	test_strip2
 **********************************************************
 * Description: 
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void test_strip2 ( 
	int				id,
	Bool			sprite,
	float			r,
	float			g,
	float			b,
	float			x, 
	float			y, 
	float			sx, 
	float			sy, 
	float			trans,
	float			depth
	)
{
	Ps2Packet	*data;
	Ps2Color	 col = { 127, 127, 127, 127 };
	Ps2Texture	*tex;
	Ps2Sprite	*sp;
	int			 x1, y1;
	int			 x2, y2;
	u_int		*rgba;
	u_int		 z;
	int			 width;
	int			 height;

	/* init (no validation!) */
	col.r = (Byte)(r * 255.0f);
	col.g = (Byte)(g * 255.0f);
	col.b = (Byte)(b * 255.0f);
	if ( sprite ) {
		sp  = &ps2.sprites[id-1];
		tex = &ps2.textures[(sp->texid - 1)];
		width  = sp->width;
		height = sp->height;
	} else {
		sp  = NULL;
		tex = &ps2.textures[id-1];
		width  = tex->width;
		height = tex->height;
	}
	data  = ps2.aligned_buffer;
	rgba  = (u_int *)&col;
		
	/* create DMA Tag to draw sprite */
	data[0].b64[0] = ps2DmaEndTag(240);										// DMA tag (size=224 bytes)
	data[0].b64[1] = 0x0L;													// padding
	// GIF Tag
	data[1].b64[0] = test_tags[8];											// GIFTAG
	data[1].b64[1] = test_tags[9];											// REGS
	// TEX0 (0x06)
	data[2].b64[0] = tex->tex0;												// TEX0
	data[2].b64[1] = 0x06L;													// A+D=TEX0 (0x06)
	// TEX1 (0x14)
	data[3].b64[0] = 0x60L;													// TEX1
	data[3].b64[1] = 0x14L;													// A+D=TEX1 (0x14)
	// TEXA (0x3B)
	data[4].b32[0] = 0x00;													// TEXA
	data[4].b32[1] = 0x7F;													// TEXA (7F,127=1.0f)
	data[4].b64[1] = 0x3BL;													// A+D=TEXA (0x3B)
	// ALPHA1 (0x42)
	data[5].b32[0] = 0x01;													// ALPHA1
	data[5].b32[1] = 0x00;													// ALPHA1
	data[5].b64[1] = 0x42L;													// A+D=ALPHA1 (0x42)
	// GIF Tag
	data[6].b64[0] = test_tags[10];											// GIFTAG
	data[6].b64[1] = test_tags[11];											// REGS
	// RGBA(Q)
	data[7].b32[0] = *rgba;													// RGBA
	data[7].f32[1] = 1.0f;													// (Q)
	data[7].b64[1] = 0x01L;													// A+D
	// UV
	data[8].b32[0] = 0x80008;												// UV (add 0.5, 0.5 to UV)
	data[8].b32[1] = 0x00;													// UV
	data[8].b64[1] = 0x03L;													// A+D
	// XYZ2
	data[9].b64[0] = 0x0L;													// XYZ2
	data[9].b64[1] = 0x05L;													// A+D
	// UV
	data[10].b32[0] = ((width - 1)<<4);										// UV
	data[10].b32[1] = 0x00;													// UV
	data[10].b64[1] = 0x03L;												// A+D
	// XYZ2
	data[11].b64[0] = 0x0L;													// XYZ2
	data[11].b64[1] = 0x05L;												// A+D
	// UV
	data[12].b32[0] = ((height - 1)<<20);									// UV
	data[12].b32[1] = 0x00;													// UV
	data[12].b64[1] = 0x03L;												// A+D
	// XYZ2
	data[13].b64[0] = 0x0L;													// XYZ2
	data[13].b64[1] = 0x05L;												// A+D
	// UV
	data[14].b32[0] = (((height - 1)<<20)|((width - 1)<<4));				// UV
	data[14].b32[1] = 0x0;													// UV
	data[14].b64[1] = 0x03L;												// A+D
	// XYZ2
	data[15].b64[0] = 0x0L;													// XYZ2
	data[15].b64[1] = 0x05L;												// A+D

	/* find x and y (works for 640x224 display only!) */
	x1 = 1728 + (int)(x * 640.0f);
	y1 = 1936 + (int)(y * 224.0f);
	x2 = x1 + (int)(((float)width)  * sx * 0.0015625f  * 640.0f);
	y2 = y1 + (int)(((float)height) * sy * 0.00446429f * 224.0f);	
	data[9].b32[0]  = (y1<<20)|(x1<<4);				// XYZ2
	data[11].b32[0] = (y1<<20)|(x2<<4);				// XYZ2
	data[13].b32[0] = (y2<<20)|(x1<<4);				// XYZ2
	data[15].b32[0] = (y2<<20)|(x2<<4);				// XYZ2

	/* find z (works for 24 bit Z buffer only!) */
	z = (u_int)(16777215.0f * (1.0f - depth));
	data[9].b32[1]  = z;							// XYZ2
	data[11].b32[1] = z;							// XYZ2
	data[13].b32[1] = z;							// XYZ2
	data[15].b32[1] = z;							// XYZ2
	//printf( "tex->height=%d, width=%d\n", tex->height, tex->width );

	/* transparency */
	data[4].b32[1] = (int)(trans * 127.0f);			// TEXA (7F,127=1.0f)

#if 0
	gfxFilePath( "saves" );
	gfxFileSave( "testing.dat", (Byte *)data, 256 );
	exit(1);
#endif

	/* flush the write buffer (until we work out the problem with uncached memory) */
	ps2FlushWriteBuffer();
	FlushCache(0);

	/* send the sprite drawing code directly to the GIF */
	sceGsSyncPath( 0, 0 );
	sceDmaSync( ps2.dma_channel_GIF,  0, 0 );
	ps2DmaTransferEnable( ps2.dma_channel_GIF, FALSE );
    sceDmaSend( ps2.dma_channel_GIF, (u_long128 *)data );
}


/**********************************************************
 * Function:	test_strip3
 **********************************************************
 * Description: 
 * Inputs:	
 * Notes:	
 * Returns:
 **********************************************************/

void test_strip3 ( 
	int				id,
	Bool			sprite,
	float			r,
	float			g,
	float			b,
	float			x, 
	float			y, 
	float			sx, 
	float			sy, 
	float			trans,
	float			depth
	)
{
	Ps2Packet	*data;
	Ps2Color	 col = { 127, 127, 127, 127 };
	Ps2Texture	*tex;
	Ps2Sprite	*sp;
	int			 x1, y1;
	int			 x2, y2;
	u_int		*rgba;
	u_int		 z;
	int			 width;
	int			 height;

	/* init (no validation!) */
	col.r = (Byte)(r * 255.0f);
	col.g = (Byte)(g * 255.0f);
	col.b = (Byte)(b * 255.0f);
	if ( sprite ) {
		sp  = &ps2.sprites[id-1];
		tex = &ps2.textures[(sp->texid - 1)];
		width  = sp->width;
		height = sp->height;
	} else {
		sp  = NULL;
		tex = &ps2.textures[id-1];
		width  = tex->width;
		height = tex->height;
	}
	data  = ps2.aligned_buffer;
	rgba  = (u_int *)&col;
		
	/* create DMA Tag to draw sprite */
	data[0].b64[0] = ps2DmaEndTag(288);										// DMA tag (size=288 bytes)
	data[0].b64[1] = 0x0L;													// padding
	// GIF Tag
	data[1].b64[0] = test_tags[13];											// GIFTAG
	data[1].b64[1] = test_tags[14];											// REGS
	// TEX0 (0x06)
	data[2].b64[0] = tex->tex0;												// TEX0
	data[2].b64[1] = 0x06L;													// A+D=TEX0 (0x06)
	// TEX1 (0x14)
	data[3].b64[0] = 0x60L;													// TEX1
	data[3].b64[1] = 0x14L;													// A+D=TEX1 (0x14)
	// TEXA (0x3B)
	data[4].b32[0] = 0x00;													// TEXA
	data[4].b32[1] = 0x7F;													// TEXA (7F,127=1.0f)
	data[4].b64[1] = 0x3BL;													// A+D=TEXA (0x3B)
	// ALPHA1 (0x42)
	data[5].b32[0] = 0x01;													// ALPHA1
	data[5].b32[1] = 0x00;													// ALPHA1
	data[5].b64[1] = 0x42L;													// A+D=ALPHA1 (0x42)
	// GIF Tag
	data[6].b64[0] = test_tags[15];											// GIFTAG
	data[6].b64[1] = test_tags[16];											// REGS
	// RGBA(Q)
	col.a = (Byte)(255);
	data[7].b32[0] = *rgba;													// RGBA
	data[7].f32[1] = 1.0f;													// (Q)
	data[7].b64[1] = 0x01L;													// A+D
	// UV
	data[8].b32[0] = 0x80008;												// UV (add 0.5, 0.5 to UV)
	data[8].b32[1] = 0x00;													// UV
	data[8].b64[1] = 0x03L;													// A+D
	// XYZ2
	data[9].b64[0] = 0x0L;													// XYZ2
	data[9].b64[1] = 0x05L;													// A+D
	// RGBA(Q)
	col.a = (Byte)(127);
	data[10].b32[0] = *rgba;													// RGBA
	data[10].f32[1] = 1.0f;													// (Q)
	data[10].b64[1] = 0x01L;													// A+D
	// UV
	data[11].b32[0] = ((width - 1)<<4);										// UV
	data[11].b32[1] = 0x00;													// UV
	data[11].b64[1] = 0x03L;												// A+D
	// XYZ2
	data[12].b64[0] = 0x0L;													// XYZ2
	data[12].b64[1] = 0x05L;												// A+D
	// RGBA(Q)
	col.a = (Byte)(25);
	data[13].b32[0] = *rgba;													// RGBA
	data[13].f32[1] = 1.0f;													// (Q)
	data[13].b64[1] = 0x01L;													// A+D
	// UV
	data[14].b32[0] = ((height - 1)<<20);									// UV
	data[14].b32[1] = 0x00;													// UV
	data[14].b64[1] = 0x03L;												// A+D
	// XYZ2
	data[15].b64[0] = 0x0L;													// XYZ2
	data[15].b64[1] = 0x05L;												// A+D
	// RGBA(Q)
	col.a = (Byte)(85);
	data[16].b32[0] = *rgba;													// RGBA
	data[16].f32[1] = 1.0f;													// (Q)
	data[16].b64[1] = 0x01L;													// A+D
	// UV
	data[17].b32[0] = (((height - 1)<<20)|((width - 1)<<4));				// UV
	data[17].b32[1] = 0x0;													// UV
	data[17].b64[1] = 0x03L;												// A+D
	// XYZ2
	data[18].b64[0] = 0x0L;													// XYZ2
	data[18].b64[1] = 0x05L;												// A+D

	/* find x and y (works for 640x224 display only!) */
	x1 = 1728 + (int)(x * 640.0f);
	y1 = 1936 + (int)(y * 224.0f);
	x2 = x1 + (int)(((float)width)  * sx * 0.0015625f  * 640.0f);
	y2 = y1 + (int)(((float)height) * sy * 0.00446429f * 224.0f);	
	data[9].b32[0]  = (y1<<20)|(x1<<4);				// XYZ2
	data[12].b32[0] = (y1<<20)|(x2<<4);				// XYZ2
	data[15].b32[0] = (y2<<20)|(x1<<4);				// XYZ2
	data[18].b32[0] = (y2<<20)|(x2<<4);				// XYZ2

	/* find z (works for 24 bit Z buffer only!) */
	z = (u_int)(16777215.0f * (1.0f - depth));
	data[9].b32[1]  = z;							// XYZ2
	data[12].b32[1] = z;							// XYZ2
	data[15].b32[1] = z;							// XYZ2
	data[18].b32[1] = z;							// XYZ2
	//printf( "tex->height=%d, width=%d\n", tex->height, tex->width );

	/* transparency */
	data[4].b32[1] = (int)(trans * 127.0f);			// TEXA (7F,127=1.0f)

#if 0
	gfxFilePath( "saves" );
	gfxFileSave( "testing.dat", (Byte *)data, 256 );
	exit(1);
#endif

	/* flush the write buffer (until we work out the problem with uncached memory) */
	ps2FlushWriteBuffer();
	FlushCache(0);

	/* send the sprite drawing code directly to the GIF */
	sceGsSyncPath( 0, 0 );
	sceDmaSync( ps2.dma_channel_GIF,  0, 0 );
	ps2DmaTransferEnable( ps2.dma_channel_GIF, FALSE );
    sceDmaSend( ps2.dma_channel_GIF, (u_long128 *)data );
}


void debug_init (
	void
	)
{
	DMA_init( "-S" );
}

void debug_file (
	Text	name,
	int		test
	)
{
	Byte	*data;
	int		 result;

	DMA_init( "-S" );

	gfxFilePath( "saves" );
	data = gfxFileLoad( name );
	switch( test ) {
	case DEBUG_TEST_XGKICK:
		result = GIF_disassembleXGKICK( (void *)data );
		puts(gOutputBuffer);
		printf( "result=0x%X\n", result );
		break;
	case DEBUG_TEST_DMA_GIF:
		result = GIF_disassembleDmaList( ps2.dma_channel_GIF, (void *)data );
		puts(gOutputBuffer);
		printf( "result=0x%X\n", result );
		break;
	}
}

#if 0
void conv_test (
	void
	)
{
	int			i, j, t, *p;
	float		f, g;

	p = (int *)ps2ScratchPad;
#define NLOOP	1000000

	PS2_SET_TIMER1(0);
	for ( i=0; i<NLOOP; i++ ) {
		;
	}
	t = PS2_GET_TIMER1();
	printf( "for loop do nothing %d times took %d hsyncs\n", i, t );

	PS2_SET_TIMER1(0);
	for ( (*p)=0; (*p)<NLOOP; (*p)++ ) {
		;
	}
	t = PS2_GET_TIMER1();
	printf( "for loop do nothing (on scratch pad) %d times took %d hsyncs\n", i, t );

	PS2_SET_TIMER1(0);
	i = 0;
	while( i<NLOOP ) {
		i++;
	}
	t = PS2_GET_TIMER1();
	printf( "while loop do nothing %d times took %d hsyncs\n", i, t );

	PS2_SET_TIMER1(0);
	i = 0;
loop:
	i++;
	if ( i<NLOOP ) goto loop;
	t = PS2_GET_TIMER1();
	printf( "goto loop do nothing %d times took %d hsyncs\n", i, t );

	PS2_SET_TIMER1(0);
	j = 12333.123213f;
	for ( i=0; i<NLOOP; i++, j+= 1.2f ) {
		f = (float)j;
		g = f;
	}
	t = PS2_GET_TIMER1();
	printf( "int to float conv %d times took %d hsyncs\n", i, t );

	PS2_SET_TIMER1(0);
	f = 0.0213f;
	for ( i=0; i<NLOOP; i++ ) {
		f += 1.02134f;
	}
	t = PS2_GET_TIMER1();
	printf( "float add %d times took %d hsyncs\n", i, t );

	PS2_SET_TIMER1(0);
	f = 0.0213f;
	for ( i=0; i<NLOOP; i++, f+=1.0f ) {
		j = (int)f;
		t = j;
	}
	t = PS2_GET_TIMER1();
	printf( "float add and int conv %d times took %d hsyncs\n", i, t );

	PS2_SET_TIMER1(0);
	g = 0.0123f;
	for ( i=0; i<NLOOP; i++, j++ ) {
		f = g;
	}
	t = PS2_GET_TIMER1();
	printf( "float copy %d times took %d hsyncs\n", i, t );

	PS2_SET_TIMER1(0);
	f = 0.0213f;
	for ( i=0; i<NLOOP; i++ ) {
		j = i;
	}
	t = PS2_GET_TIMER1();
	printf( "int copy %d times took %d hsyncs\n", i, t );

	PS2_PERF_SAVE_CPU_CYCLES();
	f = (float)j;
	g = f;
	t = PS2_PERF_GET_CPU_CYCLES();
	printf( "int to float conv took %d CPU cycles\n", t );

	PS2_PERF_SAVE_CPU_CYCLES();
	j = (int)f;
	i = j;
	t = PS2_PERF_GET_CPU_CYCLES();
	printf( "float to int conv took %d CPU cycles\n", t );

	PS2_PERF_SAVE_CPU_CYCLES();
	j = i;
	t = PS2_PERF_GET_CPU_CYCLES();
	printf( "int copy took %d CPU cycles\n", t );

	PS2_PERF_SAVE_CPU_CYCLES();
	g = f;
	t = PS2_PERF_GET_CPU_CYCLES();
	printf( "float copy took %d CPU cycles\n", t );

	PS2_PERF_SAVE_CPU_CYCLES();
	for ( i=0; i<1; i++ ) {
		;
	}
	t = PS2_PERF_GET_CPU_CYCLES();
	printf( "for loop do nothing %d times took %d CPU cycles\n", 1, t );

	PS2_PERF_SAVE_CPU_CYCLES();
	for ( i=0; i<1; i++ ) {
		;
	}
	t = PS2_PERF_GET_CPU_CYCLES();
	printf( "for loop do nothing %d times took %d CPU cycles\n", 1, t );

	PS2_PERF_SAVE_CPU_CYCLES();
	for ( i=0; i<1; i++ ) {
		;
	}
	t = PS2_PERF_GET_CPU_CYCLES();
	printf( "for loop do nothing %d times took %d CPU cycles\n", 1, t );

	j = 60;
	PS2_PERF_SAVE_CPU_CYCLES();
	i = (j % 100);
	t = PS2_PERF_GET_CPU_CYCLES();
	printf( "int quotient divide (%d) took %d CPU cycles\n", i, t );

	j = 60;
	PS2_PERF_SAVE_CPU_CYCLES();
	i = (j % 100);
	t = PS2_PERF_GET_CPU_CYCLES();
	printf( "int quotient divide (%d) took %d CPU cycles\n", i, t );

	j = 60;
	PS2_PERF_SAVE_CPU_CYCLES();
	i = (j % 100);
	t = PS2_PERF_GET_CPU_CYCLES();
	printf( "int quotient divide (%d) took %d CPU cycles\n", i, t );

	j = 60;
	PS2_PERF_SAVE_CPU_CYCLES();
	i = j - (j * (j / 100));
	t = PS2_PERF_GET_CPU_CYCLES();
	printf( "int quotient divide v2 (%d) took %d CPU cycles\n", i, t );

	j = 60;
	PS2_PERF_SAVE_CPU_CYCLES();
	i = j - (j * (j / 100));
	t = PS2_PERF_GET_CPU_CYCLES();
	printf( "int quotient divide v2 (%d) took %d CPU cycles\n", i, t );

	j = 160;
	PS2_PERF_SAVE_CPU_CYCLES();
	i = j - (j * (j / 100));
	t = PS2_PERF_GET_CPU_CYCLES();
	printf( "int quotient divide v2 (%d) took %d CPU cycles\n", i, t );

	PS2_PERF_SAVE_CPU_CYCLES();
	i = (123275 / 100);
	t = PS2_PERF_GET_CPU_CYCLES();
	printf( "int divide took %d CPU cycles\n", t );

	PS2_PERF_SAVE_CPU_CYCLES();
	f = (g / 100.0f);
	t = PS2_PERF_GET_CPU_CYCLES();
	printf( "float divide took %d CPU cycles\n", t );

	PS2_PERF_SAVE_CPU_CYCLES();
	i = (123275 * 120);
	t = PS2_PERF_GET_CPU_CYCLES();
	printf( "int multiply took %d CPU cycles\n", t );

	PS2_PERF_SAVE_CPU_CYCLES();
	f = (g * 120.0f);
	t = PS2_PERF_GET_CPU_CYCLES();
	printf( "float multiply took %d CPU cycles\n", t );
}

void loop_test()
{
	int		i, j, k;

	for ( i=0; i<17; i++ ) {
		for ( j=0; j<11; j++ );
			for ( k=0; k<3; k++ );
		for ( j=0; j<11; j++ );
			for ( k=0; k<3; k++ );
		for ( j=0; j<5; j++ );
			for ( k=0; k<3; k++ );
		for ( j=0; j<5; j++ );
			for ( k=0; k<3; k++ );
	}

	for ( i=0;i<187; i++ )
		for ( j=0;j<6; j++ );
}

void random_test()
{
	float	fmin;
	float	fmax;
	float	frand;
	int		i;

	for ( i=0; i<50; i++ ) {
		fmin = (float)i;
		fmax = (fmin * (float)(i * 2));
		frand = gfxRandom( fmin, fmax );
		printf( "random for min=%.2f,max=%.2f is %.2f\n", fmin, fmax, frand );
	}
}

#endif
