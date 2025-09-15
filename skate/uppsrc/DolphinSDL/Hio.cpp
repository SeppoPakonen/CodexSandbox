#include "DolphinSDL.h"


BOOL HIOEnumDevices ( HIOEnumCallback callback ) {
	TODO
}

BOOL HIOInit        ( s32 chan, HIOCallback callback ) {
	TODO
}

BOOL HIOInitEx      ( s32 chan, u32 dev, HIOCallback callback ) {
	TODO
}

BOOL HIOReadMailbox ( u32* word ) {
	TODO
}

BOOL HIOWriteMailbox( u32  word ) {
	TODO
}

BOOL HIORead        ( u32 addr, void* buffer, s32 size ) {
	TODO
}

BOOL HIOWrite       ( u32 addr, void* buffer, s32 size ) {
	TODO
}

BOOL HIOReadAsync   ( u32 addr, void* buffer, s32 size, HIOCallback callback ) {
	TODO
}

BOOL HIOWriteAsync  ( u32 addr, void* buffer, s32 size, HIOCallback callback ) {
	TODO
}

BOOL HIOReadStatus  ( u32* status ) {
	TODO
}

