#ifndef _Dolphin_posix_h_
#define _Dolphin_posix_h_


void WinSwapBuffers();

void EndianSwap32(void* buffer);
void EndianSwap16(void* buffer);
uint32 EndianSwapReturn32(void* buffer);
uint16 EndianSwapReturn16(void* buffer);


#endif
