
#ifndef THEYER_GFXDEFS_H
#define THEYER_GFXDEFS_H

/* ticks per second */
#ifdef GFX_FIXEDPOINT_API
#define GFX_TICKS_PER_SECOND	245760	
#else
#define GFX_TICKS_PER_SECOND		60
#endif

/* object types */
#define GFX_OBJ_STANDARD			 0
#define GFX_OBJ_SKINNED				 1

/* pivot world */
#define GFX_PIVOT_WORLD				-1

/* texture format */
#define GFX_NO_TEXTURE 				-1
#define GFX_TEXTURE_16BIT			 1
#define GFX_TEXTURE_8BIT			 2
#define GFX_TEXTURE_4BIT			 3
#define GFX_TEXTURE_24BIT			 4
#define GFX_TEXTURE_32BIT			 5
#define GFX_NO_SPRITE				-1

/* object handler attributes */
#define GFX_SUBDIV_NONE				 0
#define GFX_SUBDIV_2x2				 1
#define GFX_SUBDIV_4x4				 2
#define GFX_SUBDIV_8x8				 3
#define GFX_SUBDIV_16x16			 4
#define GFX_SUBDIV_32x32			 5

/* split screen modes */
#define GFX_SPLIT_SCREEN_NONE		 0
#define GFX_SPLIT_SCREEN_VERTICAL	 1
#define GFX_SPLIT_SCREEN_HORIZONTAL	 2
#define GFX_SPLIT_SCREEN_QUAD		 3

/* split screens */
#define GFX_SPLIT_SCREEN_ONE		 0
#define GFX_SPLIT_SCREEN_TWO		 1
#define GFX_SPLIT_SCREEN_FULL		 2

/* args for gfxReset() */
#define GFX_RESET_TEXTURES	0x00000001
#define GFX_RESET_SPRITES	0x00000002
#define GFX_RESET_MODELS	0x00000004
#define GFX_RESET_ALL		0xFFFFFFFF 

/* axis */
#define GFX_X_AXIS					'X'
#define GFX_Y_AXIS					'Y'
#define GFX_Z_AXIS					'Z'

/* colormap */
#define GFX_NO_COLORMAP				-1

/* graphics init arguments */
#define GFX_INITARG_SETVIDEOMODE	 1
#define GFX_INITARG_USEWINDOW		 2

/* graphics video modes */
#define GFX_VIDEO_MODE_NONE			-1
#define GFX_VIDEO_MODE_PAL			 0
#define GFX_VIDEO_MODE_NTSC			 1

/* playstation memory buffers */
#define GFX_PSX_OT_MEMORY			 1
#define GFX_PSX_FILE_MEMORY			 2
#define GFX_PSX_GPU_MEMORY			 3
#define GFX_PSX_FILE_AND_GPU_MEMORY	 4

/* Playstation2 memory buffers */
#define GFX_PS2_FILE_MEMORY			 5

/* PlayStation2 data type arg for gfxPs2AddData() */
#define GFX_PS2_DRAWING_CODE		 1
#define GFX_PS2_JOINT_DATA			 2
#define GFX_PS2_POINT_DATA			 3
#define GFX_PS2_NORMAL_DATA			 4
#define GFX_PS2_ST_DATA				 5
#define GFX_PS2_RGBA_DATA			 6
#define GFX_PS2_MODEL_LIMITS		 7
#define GFX_PS2_ZSORT_DATA			 8

/* PlayStation2 mem arg 3 for gfxPs2MemoryDump() */
#define GFX_PS2_VU0_MEM				 0
#define GFX_PS2_VU1_MEM				 1
#define GFX_PS2_SPR_MEM				 2

#endif

