#include "DolphinRE.h"

bool			EmDisplayListInProgress;
byte*			EmBuffPtr;
void*			EmBuffStart;
void*			EmBuffTop;
CompStruct		GfxTrack[VTXARR_SIZE];
CompStruct		some_gxvert_var[VTXARR_SIZE];
GXAttrType		VCD[VCD_SIZE];
VatStruct		VAT[GX_VA_MAX_ATTR][VCD_SIZE];
VtxArray		GXVtxArray[VTXARR_SIZE];
NTGStruct		GXNumTexGens[VTXARR_SIZE];
TexCoordGen		GXTexCoordGen[VTXARR_SIZE];
uint			NBTCount;
GXColor			CurrentColors[VTXARR_SIZE];


// GXBeginDisplayList  begins a display list and disables writes to the  FIFO currently attached to
// the CPU. After this function is called, GX API functions that normally send command or data into
// the CPU FIFO will send them to the display list buffer instead of the FIFO until GXEndDisplayList
// is called.
//
// Writes to the CPU FIFO will be re-enabled when the function GXEndDisplayList executes.
//
// The application is expected to allocate the memory for the display list buffer.  If the display
// list exceeds the maximum size of the buffer, GXEndDisplayList will return an error.  The address
// of the buffer must be 32-byte aligned.   OSAlloc returns 32-byte-aligned pointers.  You can use
// the macro ATTRIBUTE_ALIGN(32)to align statically allocated buffers.
//
// The CPU's write-gather pipe is used to write graphics commands to the display list.   Therefore,
// the display list buffer must be forced out of the CPU cache prior to being filled.  The command
// DCInvalidateRange may be used for this purpose.  In addition, due to the mechanics of flushing
// the write-gather pipe, the display list buffer should be at least 63 bytes larger than the
// maximum expected amount of data stored.  If the command GXResetWriteGatherPipe is called
// immediately after calling GXBeginDisplayList, then you only need 32 bytes of extra space.
//
// A display list cannot be nested; i.e., the functions GXBeginDisplayList, GXCallDisplayList, and
// GXEndDisplayList cannot be called between GXBeginDisplayList and GXEndDisplayList.
//
// Basically you can put most of GX API commands into a display list. However, since the use of
// display list can bypass all state coherences controlled by GX API in run-time, sometimes it
// brings some state collisions or incoherences that may lead to unexpected behavior or even
// graphics pipeline hang. The most recommended safe way is putting only primitives (regions
// enclosed by GXBegin and GXEnd) that don't cause any state collisions.
//
// To execute a display list, use GXCallDisplayList.

void GXBeginDisplayList(byte* list, u32 size) {
	Dbg_Assert(EmDisplayListInProgress == false);
	Dbg_Assert(size == 0);
	
	EmBuffPtr = list;
	EmBuffStart = list;
	EmBuffTop = (void *)((size_t)list + size);
	EmDisplayListInProgress = true;
}



// This function ends a display list and resumes writing graphics commands to the CPU FIFO.   It
// returns the size of the display list written to the display list buffer since GXBeginDisplayList
// was called.  If the display list size exceeds the size of the buffer allocated the function will
// fault with an ASSERT error.  In the case of optimized libraries, where ASSERT errors do not
// occur, a zero length size will be returned.  The display list size is a multiple of 32B and any
// unsed commands in the last 32B will be padded with GX_NOP.
//
// Due to the mechanics of flushing the write-gather pipe (which is used to create the display
// list), the display buffer should be at least 32 bytes larger than the maximum expected amount of
// data stored.  GXEndDisplayList calls GXFlush, and thus it is not necessary to call GXFlush
// explicitly after creating the display list.
//
// A display list cannot be nested; i.e., no display list functions (GXBeginDisplayList,
// GXEndDisplayList, and GXCallDisplayList) can be called between a GXBeginDisplayList and
// GXEndDisplayList pair.

u32 GXEndDisplayList() {
	u32 displist_size_bytes;
	uint count;
	
	Dbg_Assert(EmDisplayListInProgress == 1);
	Dbg_Assert(EmBuffTop >= EmBuffPtr);
	
	displist_size_bytes = (size_t)EmBuffTop - (size_t)EmBuffStart;
	
	u8* it = (u8*)EmBuffPtr;
	const u8* end = (u8*)EmBuffTop;
	while (it < end)
		*it++ = 0;
	
	EmDisplayListInProgress = 0;
	EmBuffPtr = 0;
	EmBuffTop = 0;
	
	return displist_size_bytes;
}



// This function will cause the Graphics Processor (GP) to execute graphics commands from the
// display list instead of from the GP FIFO.  When the value of nbytes (a multiple of 32) has been
// read, the graphics processor will resume executing commands from the graphics FIFO.
//
// Graphics commands from a display list are prefetched into a separate 4kB FIFO.   This prevents
// any data prefetched for the main graphics command stream from being lost during the display list
// call.  Note that a display list cannot call another display list.
//
// The display list must be padded to a length of 32B.  All the data in the display list is
// interpreted by the graphics processor, so any unused memory at the end of a display list should
// be set to GX_NOP.  If you create the display list using GXBeginDisplayList/GXEndDisplayList, this
// padding will be done automatically

void GXCallDisplayList(const void *list, u32 nbytes) {
	u16 value16;
	uint value;
	byte *it = (byte *)list;
	int size_left = nbytes;
	
	Dbg_Assert(EmDisplayListInProgress == 0);
	Dbg_Assert((size_left & 0x1fU) == 0);
	
next_cmd:
	do {
		if (size_left < 1) {
			Dbg_Assert(size_left == 0);
			break;
		}
		byte cmd_byte = GetU8(&it, &size_left);
		byte cmd_arg  = cmd_byte & 0x07;
		byte cmd_type = cmd_byte & 0xf8;
		switch (cmd_type) {
		case 0:
			goto next_cmd;
		default:
			Dbg_Assert(false);
			break;
		case GX_QUADS:
			value16 = GetU16(&it, &size_left);
			value = value16;
			GXBegin(GX_QUADS, (GXVtxFmt)cmd_arg, value16);
			break;
		case GX_TRIANGLES:
			value16 = GetU16(&it, &size_left);
			value = value16;
			GXBegin(GX_TRIANGLES, (GXVtxFmt)cmd_arg, value16);
			break;
		case GX_TRIANGLESTRIP:
			value16 = GetU16(&it, &size_left);
			value = value16;
			GXBegin(GX_TRIANGLESTRIP, (GXVtxFmt)cmd_arg, value16);
			break;
		case GX_TRIANGLEFAN:
			value16 = GetU16(&it, &size_left);
			value = value16;
			GXBegin(GX_TRIANGLEFAN, (GXVtxFmt)cmd_arg, value16);
			break;
		case GX_LINES:
			value16 = GetU16(&it, &size_left);
			value = value16;
			GXBegin(GX_LINES, (GXVtxFmt)cmd_arg, value16);
			break;
		case GX_LINESTRIP:
			value16 = GetU16(&it, &size_left);
			value = value16;
			GXBegin(GX_LINESTRIP, (GXVtxFmt)cmd_arg, value16);
			break;
		case GX_POINTS:
			value16 = GetU16(&it, &size_left);
			value = value16;
			GXBegin(GX_POINTS, (GXVtxFmt)cmd_arg, value16);
		}
		uint vtx_len = GetVertexLength();
		Dbg_Assert(vtx_len < 0x100 && value < 0x10000);
		
		uint data_size = (vtx_len & 0xff) * (value & 0xffff);
		size_left = size_left - data_size;
		if (size_left < 0) {
			Dbg_Assert(false);
			break;
		}
		SendPrimitiveData(&it, data_size);
		GXEnd();
	}
	while (true);
}



void SendPrimitiveData(byte **data, int size) {
	u8 arg_u8;
	u16 arg_u16;
	byte tex_mode;
	byte clr_mode;
	byte norm_mode;
	byte pos_mode;
	
loop_begin:
	if (size < 1) {
		Dbg_Assert(size == 0);
		return;
	}
	int i = 0;
	do {
		Dbg_Assert(i < VTXARR_SIZE);
		
		byte mode = GfxTrack[i].mode & 0x7f;
		byte arg  = GfxTrack[i].mode & 0x80;
		
		switch (mode) {
			
		case PRIM_DATA_MAT_IDX0:
		case PRIM_DATA_MAT_IDX1:
			arg_u8 = GetU8(data, &size);
			GXMatrixIndex1u8(arg_u8);
			break;
			
		case PRIM_DATA_POS:
			pos_mode = GfxTrack[i].mode;
			if (pos_mode == 1) {
				SendPositionDirect(i, data, &size);
			}
			else if (pos_mode == 2) {
				arg_u8 = GetU8(data, &size);
				GXPosition1x8(arg_u8);
			}
			else {
				if (pos_mode != 3)
					goto invalid_arg;
				arg_u16 = GetU16(data, &size);
				GXPosition1x16(arg_u16);
			}
			break;
			
		case PRIM_DATA_NORM:
			norm_mode = GfxTrack[i].mode;
			if (norm_mode == 1) {
				SendNormalDirect(i, data, &size);
			}
			else if (norm_mode == 2) {
				arg_u8 = GetU8(data, &size);
				GXNormal1x8(arg_u8);
			}
			else {
				if (norm_mode != 3)
					goto invalid_arg;
				arg_u16 = GetU16(data, &size);
				GXNormal1x16(arg_u16);
			}
			break;
			
		case PRIM_DATA_CLR:
			clr_mode = GfxTrack[i].mode;
			if (clr_mode == 1) {
				SendColorDirect(i, data, &size);
			}
			else if (clr_mode == 2) {
				arg_u8 = GetU8(data, &size);
				GXColor1x8(arg_u8);
			}
			else {
				if (clr_mode != 3)
					goto invalid_arg;
				arg_u16 = GetU16(data, &size);
				GXColor1x16(arg_u16);
			}
			break;
			
		case PRIM_DATA_TEX:
			tex_mode = GfxTrack[i].mode;
			if (tex_mode == 1) {
				SendTexCoordDirect(i, data, &size);
			}
			else if (tex_mode == 2) {
				arg_u8 = GetU8(data, &size);
				GXTexCoord1x8(arg_u8);
			}
			else {
				if (tex_mode != 3)
					goto invalid_arg;
				arg_u16 = GetU16(data, &size);
				GXTexCoord1x16(arg_u16);
			}
			break;
			
		default:
		invalid_arg:
			Dbg_Assert(false);
			break;
		}
		if (arg != 0)
			goto loop_begin;
		i++;
	}
	while (true);
}


void SendPositionDirect(int arg, byte **data, int *size) {
	u8 x_u8, y_u8, z_u8;
	s8 x_s8, y_s8, z_s8;
	u16 x_u16, y_u16, z_u16;
	u16 x_s16, y_s16, z_s16;
	float x_f32, y_f32, z_f32;
	bool has_extra_args;
	
	has_extra_args = some_gxvert_var[arg].mode;
	
	if (has_extra_args == false) {
		switch (some_gxvert_var[arg].attr_type) {
			
		case ARGTYPE_U8:
			y_u8 = GetU8(data, size);
			x_u8 = GetU8(data, size);
			GXPosition2u8(x_u8, y_u8);
			break;
			
		case ARGTYPE_S8:
			y_s8 = GetU8(data, size);
			x_s8 = GetU8(data, size);
			GXPosition2s8(x_s8, y_s8);
			break;
			
		case ARGTYPE_U16:
			y_u16 = GetU16(data, size);
			x_u16 = GetU16(data, size);
			GXPosition2u16(x_u16, y_u16);
			break;
			
		case ARGTYPE_S16:
			y_s16 = GetU16(data, size);
			x_s16 = GetU16(data, size);
			GXPosition2s16(x_s16, y_s16);
			break;
			
		case ARGTYPE_F32:
			y_f32 = GetF32(data, size);
			x_f32 = GetF32(data, size);
			GXPosition2f32(x_f32, y_f32);
			break;
			
		default:
			goto invalid_type;
		}
	} else {
		if (has_extra_args == true) {
			switch (some_gxvert_var[arg].attr_type) {
				
			case ARGTYPE_U8:
				z_u8 = GetU8(data, size);
				y_u8 = GetU8(data, size);
				x_u8 = GetU8(data, size);
				GXPosition3u8(x_u8, y_u8, z_u8);
				break;
				
			case ARGTYPE_S8:
				z_s8 = GetU8(data, size);
				y_s8 = GetU8(data, size);
				x_s8 = GetU8(data, size);
				GXPosition3s8(x_s8, y_s8, z_s8);
				break;
				
			case ARGTYPE_U16:
				z_u16 = GetU16(data, size);
				y_u16 = GetU16(data, size);
				x_u16 = GetU16(data, size);
				GXPosition3u16(x_u16, y_u16, z_u16);
				break;
				
			case ARGTYPE_S16:
				z_s16 = GetU16(data, size);
				y_s16 = GetU16(data, size);
				x_s16 = GetU16(data, size);
				GXPosition3s16(x_s16, y_s16, z_s16);
				break;
				
			case ARGTYPE_F32:
				z_f32 = GetF32(data, size);
				y_f32 = GetF32(data, size);
				x_f32 = GetF32(data, size);
				GXPosition3f32(x_f32, y_f32, z_f32);
				break;
			}
		}
	invalid_type:
		PANIC("decode error");
	}
}



void SendNormalDirect(int arg, byte **data, int *size) {
	u8		z_u8,	y_u8,	x_u8;
	u16		z_u16,	y_u16,	x_u16;
	float	z_f32,	y_f32,	x_f32;
	
	Dbg_Assert(arg < COMMON_SIZE);
	
	byte arg_type = some_gxvert_var[arg].attr_type;
	if (arg_type == 1) {
		z_u8 = GetU8(data, size);
		y_u8 = GetU8(data, size);
		x_u8 = GetU8(data, size);
		GXNormal3s8(x_u8, y_u8, z_u8);
	}
	else if (arg_type == 3) {
		z_u16 = GetU16(data, size);
		y_u16 = GetU16(data, size);
		x_u16 = GetU16(data, size);
		GXNormal3s16(x_u16, y_u16, z_u16);
	}
	else if (arg_type == 4) {
		z_f32 = GetF32(data, size);
		y_f32 = GetF32(data, size);
		x_f32 = GetF32(data, size);
		GXNormal3f32(x_f32, y_f32, z_f32);
	}
	else {
		PANIC("decode error");
	}
}


void SendColorDirect(int arg, byte **data, int *size) {
	u8 r, b, g, a;
	u16 clr_u16;
	
	Dbg_Assert(arg < COMMON_SIZE);
	
	switch (some_gxvert_var[arg].attr_type) {
	case 0:
	case 3:
		clr_u16 = GetU16(data, size);
		GXColor1u16(clr_u16);
		break;
	case 1:
	case 4:
		a = GetU8(data, size);
		b = GetU8(data, size);
		g = GetU8(data, size);
		GXColor3u8(g, b, a);
		break;
	case 2:
	case 5:
		a = GetU8(data, size);
		b = GetU8(data, size);
		g = GetU8(data, size);
		r = GetU8(data, size);
		GXColor4u8(r, g, b, a);
		break;
	default:
		PANIC("decode error");
	}
}


void SendTexCoordDirect(int arg, byte **data, int *size) {
	u8 arg_u8, arg_s8;
	u16 arg_u16, arg_s16;
	float arg_f32;
	
	u8 t_u8, s_u8;
	s8 t_s8, s_s8;
	u16 t_u16, s_u16;
	s16 t_s16, s_s16;
	float t_f32, s_f32;
	
	Dbg_Assert(arg < COMMON_SIZE);
	
	bool has_extra_arg = some_gxvert_var[arg].mode;
	bool fail = false;
	
	if (!has_extra_arg) {
		switch (some_gxvert_var[arg].attr_type) {
			
		case ARGTYPE_U8:
			arg_u8 = GetU8(data, size);
			GXTexCoord1u8(arg_u8);
			break;
			
		case ARGTYPE_S8:
			arg_s8 = GetU8(data, size);
			GXTexCoord1s8(arg_s8);
			break;
			
		case ARGTYPE_U16:
			arg_u16 = GetU16(data, size);
			GXTexCoord1u16(arg_u16);
			break;
			
		case ARGTYPE_S16:
			arg_s16 = GetU16(data, size);
			GXTexCoord1s16(arg_s16);
			break;
			
		case ARGTYPE_F32:
			arg_f32 = GetF32(data, size);
			GXTexCoord1f32((f32)arg_f32);
			break;
			
		default:
			fail = true;
			
		}
	}
	else {
		switch (some_gxvert_var[arg].attr_type) {
			
		case 0:
			t_u8 = GetU8(data, size);
			s_u8 = GetU8(data, size);
			GXTexCoord2u8(s_u8, t_u8);
			break;
			
		case 1:
			t_s8 = GetU8(data, size);
			s_s8 = GetU8(data, size);
			GXTexCoord2s8(s_s8, t_s8);
			break;
			
		case 2:
			t_u16 = GetU16(data, size);
			s_u16 = GetU16(data, size);
			GXTexCoord2u16(s_u16, t_u16);
			break;
			
		case 3:
			t_s16 = GetU16(data, size);
			s_s16 = GetU16(data, size);
			GXTexCoord2s16(s_s16, t_s16);
			break;
			
		case 4:
			t_f32 = GetF32(data, size);
			s_f32 = GetF32(data, size);
			GXTexCoord2f32(s_f32, t_f32);
			break;
			
		default:
			fail = true;
			
		}
	}
	
	if (fail) {
		PANIC("decode error");
	}
}


float GetF32(byte **data, int *size_left) {
	float value = (float)CONCAT13((*data)[3], CONCAT12((*data)[2], CONCAT11((*data)[1], **data)));
	*data = *data + 4;
	*size_left = *size_left + -4;
	return value;
}


u16 GetU16(byte **data, int *size_left) {
	u16 output = (u16)CONCAT11((*data)[1], **data);
	*data = *data + 2;
	*size_left = *size_left - 2;
	return output;
}


u8 GetU8(byte **data, int *size_left) {
	u8 output = **data;
	*data = *data + 1;
	*size_left = *size_left - 1;
	return output;
}


uint GetVertexLength() {
	uint vtx_len = 0;
	for(int i = 0; i < VCD_SIZE; i++)
		vtx_len += VCDComponentLength(VCD[i], i);
	return vtx_len;
}


uint VCDComponentLength(uint vcd, uint i) {
	
	switch (vcd) {
	case 0:
		break;
	case 1:
		if ((i & 0xff) < 8)
			break;
		PANIC("GX DIRECT not");
	case 2:
	case 3:
		break;
	default:
		PANIC("Illigal VCDs");
		break;
	}
	
	return vcd;
}


