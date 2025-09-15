#include "DolphinSDL.h"

FILE* outfile;
GXBool GXinBegin;
volatile OSHeapHandle   __OSCurrHeap = -1;



void* OSAllocFromHeap(OSHeapHandle heap, u32 size) {
	return malloc(size);
}

void OSFreeToHeap(OSHeapHandle heap, void* ptr) {
	free(ptr);
}

void OSReport(const char* msg, ...) {
	unsigned int cCharsWritten;
	char s[256];
	va_list argptr;
	int cnt;

	va_start(argptr, msg);
	cnt = vsprintf(s, msg, argptr);

	// Outfile
	if (outfile) fprintf(outfile, "%s", s);

	va_end(argptr);

	#if defined flagWIN32
	if (outputWinHandle)
		WriteConsole(outputWinHandle, s, strlen(s), &cCharsWritten, NULL);
	#elif defined flagPOSIX
	printf("%s", s);
	
	#else
	#error Not implemented
	#endif
	
}

void OSPanic(const char* file, int line, const char* msg, ...) {
	unsigned int cCharsWritten;
	char s[256];
	char s1[256];
	va_list argptr;
	int cnt;

	va_start(argptr, msg);
	cnt = vsprintf(s, msg, argptr);

	// OutFile
	if (outfile) vfprintf(outfile, msg, argptr);
	if (outfile) fprintf(outfile, " in \"%s\" on line %d.\n", file, line);

	va_end(argptr);

	sprintf(s1, " in \"%s\" on line %d.\n", file, line);
	strcat(s, s1);

	#if defined flagWIN32
	if (outputWinHandle)
		WriteConsole(outputWinHandle, s, strlen(s), &cCharsWritten, NULL);
	
	//WinDestroy();
	#elif defined flagPOSIX
	printf("%s", s);
	
	#else
	#error Not implemented
	#endif
	
	__BREAK__
}

OSTime OSGetTime ( void ) {
	return SysGetTickCount();
}


OSTick OSGetTick ( void ) {
	return SysGetTickCount();
}

void OSInit(void) {
	outfile = fopen("output.txt", "w");
}

void DCInvalidateRange(void* start, u32 nBytes) {}
void DCFlushRange(void* start, u32 nBytes) {}
void DCStoreRange(void* start, u32 nBytes) {}
void ICInvalidateRange(void* start, u32 nBytes) {}

OSHeapHandle OSSetCurrentHeap(OSHeapHandle heap) {
	return 0;
}

void* OSInitAlloc(void* arenaStart, void* arenaEnd, int maxHeaps) {
	return 0;
}

OSHeapHandle OSCreateHeap(void* start, void* end) {	
	return 0;
}

void* OSGetArenaHi(void) {
	return 0;
}

void* OSGetArenaLo(void) {
	return 0;
}

void OSSetArenaHi(void* newHi) {}
void OSSetArenaLo(void* newLo) {}

BOOL OSDisableInterrupts(void) {
	return 0;
}

BOOL OSEnableInterrupts(void) {
	return 1;
}

BOOL OSRestoreInterrupts(BOOL level) {
	return 1;
}

void OSInitStopwatch     ( OSStopwatch* sw, char* name )
{
    sw->name    = name;
    sw->total   = 0;
    sw->hits    = 0;

    // XXX should use limits
    sw->min     = 0xFFFFFFFF;
    sw->max     = 0;
}

void OSStartStopwatch(OSStopwatch* sw)
{
    sw->running = TRUE;
    sw->last    = OSGetTime();
}

void OSStopStopwatch(OSStopwatch* sw)
{
    OSTime  interval;

    if (sw->running) {
        interval    = OSGetTime() - sw->last;
        sw->total   += interval;
        sw->running = FALSE;
        sw->hits++;

        if (interval > sw->max)
        {
            sw->max = interval;
        }

        if (interval < sw->min)
        {
            sw->min = interval;
        }
    }
}

OSTime OSCheckStopwatch(OSStopwatch* sw)
{
    OSTime      currTotal = sw->total;

    if (sw->running)
    {
        currTotal += OSGetTime() - sw->last;
    }
    return  currTotal;
}

void OSResetStopwatch(OSStopwatch* sw) {
    OSInitStopwatch(sw, sw->name);
}

void OSDumpStopwatch(OSStopwatch* sw) {
    OSReport("Stopwatch [%s]\t:\n", sw->name);
    OSReport("\tTotal= %lld us\n", OSTicksToMicroseconds(sw->total));
    OSReport("\tHits = %d \n", sw->hits);
    OSReport("\tMin  = %lld us\n", OSTicksToMicroseconds(sw->min));
    OSReport("\tMax  = %lld us\n", OSTicksToMicroseconds(sw->max));
    OSReport("\tMean = %lld us\n", OSTicksToMicroseconds(sw->total/sw->hits));
}



u8 OSGetLanguage() {
	return OS_LANG_ENGLISH;
}


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
