#include "DolphinStub.h"

u32 OSGetProgressiveMode() {
	TODO
}

void OSSleepThread(OSThreadQueue* queue) {
	TODO
}

u32 OSGetEuRgb60Mode() {
	TODO
}

void OSInitThreadQueue(OSThreadQueue* queue) {
	TODO
}

s32 OSResumeThread(OSThread* thread) {
	TODO
}
BOOL OSCreateThread(OSThread*  thread,
		void* (*func)(void*),
		void*      param,
		void*      stack,
		u32        stackSize,
		OSPriority priority,
		u16        attr) {
	TODO
}
        
void OSSetAlarm(OSAlarm* alarm, OSTime tick, OSAlarmHandler handler) {
	TODO
}

s32 OSSuspendThread(OSThread* thread) {
	TODO
}

BOOL OSIsThreadSuspended(OSThread* thread) {
	TODO
}

void OSSetProgressiveMode(u32 on) {
	TODO
}

BOOL OSGetResetButtonState() {
	TODO
}

void OSResetSystem(int reset, u32 resetCode, BOOL forceMenu) {
	TODO
}

void OSSetEuRgb60Mode(u32 on) {
	TODO
}


u32 OSCachedToPhysical(const void* caddr) {
	TODO
}


OSResetCallback OSSetResetCallback(OSResetCallback callback) {
	TODO
}


u32 OSGetSoundMode() {
	TODO
}

void OSTicksToCalendarTime(OSTime ticks, OSCalendarTime* td) {
	TODO
}

void OSWakeupThread(OSThreadQueue* queue) {
	TODO
}
