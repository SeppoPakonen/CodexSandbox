/***************************************************************** 
 * Project		: Game Development
 *****************************************************************
 * File			: tim.h
 * Language		: ANSI C
 * Author		: Mark Theyer
 * Created		: 18 Nov 1999
 *****************************************************************
 * Description	: PlayStation TIM format file library functions
 *****************************************************************
 * 28/12/96		: Initial coding.
 *****************************************************************/

#ifndef THEYER_TIM_H
#define THEYER_TIM_H

#include <type/datatype.h>

/*
 * TIM defines
 */

#define	TIM_FILE_ID				0x00000010

#define TIM_PIXEL_MODE_4BIT		0
#define TIM_PIXEL_MODE_8BIT		1
#define TIM_PIXEL_MODE_16BIT	2
#define TIM_PIXEL_MODE_24BIT	3

#define TIM_DIRECT_COLOR		0
#define TIM_HAS_CLUT			1

/* TIM header */
typedef struct	{
	Dword		id:8,
				version:8,
				reserved:16;
	Dword		pixel_mode:3,
				clut_flag:1,
				reserved2:28;
} TimHeader;

typedef struct {
	int				bnum;
	unsigned short	x;
	unsigned short	y;
	unsigned short	width;
	unsigned short	height;
	unsigned short	data[1];
} TimPixelData;

typedef struct {
	unsigned short	r:5,g:5,b:5,a:1;
} TimPixel;

/*
 * prototypes 
 */

#endif
