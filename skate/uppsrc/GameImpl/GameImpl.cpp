#include "GameImpl.h"

uint8 * RES_gamecube;

void MemManagerHandlePushContext(Mem::Allocator* alloc)  {
	TODO
}

void MemManagerHandlePopContext(void)  {
	TODO
}

void* MemManagerHandleNew(size_t size, bool assert_on_fail, Mem::Allocator* pAlloc)  {
	TODO
}

int MemManagerHandleAvailable()  {
	TODO
}

void* MemManagerHandleReallocateDown(size_t newSize, void *pOld, Mem::Allocator* pAlloc)  {
	TODO
}

void* MemManagerHandleReallocateUp(size_t newSize, void *pOld, Mem::Allocator* pAlloc)  {
	TODO
}

void* MemManagerHandleReallocateShrink(size_t newSize, void *pOld, Mem::Allocator* pAlloc)  {
	TODO
}

void MemManagerHandleDelete(void* pAddr)  {
	TODO
}

bool MemManagerHandleValid(void* pAddr)  {
	TODO
}

size_t MemManagerHandleGetAllocSize(void* pAddr)  {
	TODO
}

Mem::Heap* MemManagerHandleTopDownHeap()  {
	TODO
}

Mem::Heap* MemManagerHandleBottomUpHeap()  {
	TODO
}

Mem::Heap* MemManagerHandleFrontEndHeap()  {
	TODO
}

Mem::Heap* MemManagerHandleScriptHeap()  {
	TODO
}

Mem::Heap* MemManagerHandleNetworkHeap()  {
	TODO
}

Mem::Heap* MemManagerHandleNetMiscHeap()  {
	TODO
}

Mem::Heap* MemManagerHandleProfilerHeap()  {
	TODO
}

Mem::Heap* MemManagerHandleDebugHeap()  {
	TODO
}

Mem::Heap* MemManagerHandleSkaterHeap(int n)  {
	TODO
}

Mem::Heap* MemManagerHandleSkaterInfoHeap()  {
	TODO
}

Mem::Heap* MemManagerHandleSkaterGeomHeap(int n)  {
	TODO
}

Mem::Heap* MemManagerHandleInternetTopDownHeap()  {
	TODO
}

Mem::Heap* MemManagerHandleInternetBottomUpHeap()  {
	TODO
}

Mem::Heap* MemManagerHandleThemeHeap()  {
	TODO
}

Mem::Heap* MemManagerHandleCutsceneTopDownHeap()  {
	TODO
}

Mem::Heap* MemManagerHandleCutsceneBottomUpHeap()  {
	TODO
}

Mem::Heap* MemManagerHandleAudioHeap()  {
	TODO
}

Mem::Heap* MemManagerHandleGetHeap(u32 whichHeap)  {
	TODO
}

void MemManagerHandleRemoveHeap(Mem::Heap* heap)  {
	TODO
}

int  MemManagerHandleGetContextDirection()  {
	TODO
}

void MemManagerHandleInitCutsceneHeap(int heap_size)  {
	TODO
}

void MemManagerHandleDeleteCutsceneHeap()  {
	TODO
}

Mem::Allocator* MemManagerHandleGetContextAllocator()  {
	TODO
}

void MemManagerHandleDeleteSkaterHeaps()  {
	TODO
}

void MemManagerHandleInitNetMiscHeap()  {
	TODO
}

void MemManagerHandleDeleteNetMiscHeap()  {
	TODO
}

void MemManagerHandleInitOtherHeaps()  {
	TODO // Get MemManager and call InitOtherHeaps
}

void MemManagerHandlePushMemoryMarker(int marker)  {
	TODO
}

void MemManagerHandlePopMemoryMarker(int marker)  {
	TODO
}

void MemManagerHandleInitSkaterHeaps(unsigned int num_skater_heaps_required)  {
	TODO
}

char* MemManagerHandleGetContextName()  {
	TODO
}

Mem::Heap* MemManagerHandleCreateHeap(Mem::Region* region, int dir, char* p_name)  {
	TODO
}

void* get_font_file_address(const char * p_filename) {
	TODO
}

extern "C" {

	void RenderNewParticles(Nx::CParticleStream * p_stream, float lifetime, float midpercent, bool use_mid_color, Image::RGBA * p_color0, Mth::Vector * p0, Mth::Vector * sr, Mth::Vector * su, float * p_params, float nearz) {
		TODO
	}
	
}

unsigned int AllocatorGetId(void* addr) {
	TODO
}

int  CreateSema(SemaParam* params) {
	TODO
}

void DeleteSema(int sema_id) {
	TODO
}

void WaitSema(int sema_id) {
	TODO
}

void SignalSema(int sema_id) {
	TODO
}

namespace SIO {
int LoadIRX(const char *pName, int num_args, char* args, bool assert_on_fail) {
	TODO
}
}

char *g_netdb = "mc0:";
const char* kConnectionTypeDescs[] = {
	"invalid",
	"Ethernet, static IP",
	"Ethernet, DHCP",
	"Ethernet, PPPoE",
	"Modem, PPP",
	"Modem, AOL PPP"
};

