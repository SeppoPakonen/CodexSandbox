#include "DolphinSDL.h"

u32 ARGetDMAStatus() {
	return 1;
}

void ARReset() {
	
}

u32 ARInit(u32 *stack_index_addr, u32 num_entries) {
	return 0;
}

u32 ARGetBaseAddress() {
	LOG("ARGetBaseAddress(): pass, WARNING: returning 0 as memory address");
	return 0;
}
