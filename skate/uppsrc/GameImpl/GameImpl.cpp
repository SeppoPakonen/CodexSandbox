#include "GameImpl.h"

uint8 * RES_gamecube;

void MemManagerHandlePushContext(Mem::Allocator* alloc)  {
	Mem::Manager::sHandle().PushContext(alloc);
}

void MemManagerHandlePopContext(void)  {
	Mem::Manager::sHandle().PopContext();
}

void* MemManagerHandleNew(size_t size, bool assert_on_fail, Mem::Allocator* pAlloc)  {
	return Mem::Manager::sHandle().New(size, assert_on_fail, pAlloc);
}

int MemManagerHandleAvailable()  {
	return Mem::Manager::sHandle().Available();
}

void* MemManagerHandleReallocateDown(size_t newSize, void *pOld, Mem::Allocator* pAlloc)  {
	return Mem::Manager::sHandle().ReallocateDown(newSize, pOld, pAlloc);
}

void* MemManagerHandleReallocateUp(size_t newSize, void *pOld, Mem::Allocator* pAlloc)  {
	return Mem::Manager::sHandle().ReallocateUp(newSize, pOld, pAlloc);
}

void* MemManagerHandleReallocateShrink(size_t newSize, void *pOld, Mem::Allocator* pAlloc)  {
	return Mem::Manager::sHandle().ReallocateShrink(newSize, pOld, pAlloc);
}

void MemManagerHandleDelete(void* pAddr)  {
	Mem::Manager::sHandle().Delete(pAddr);
}

bool MemManagerHandleValid(void* pAddr)  {
	return Mem::Manager::sHandle().Valid(pAddr);
}

size_t MemManagerHandleGetAllocSize(void* pAddr)  {
	return Mem::Manager::sHandle().GetAllocSize(pAddr);
}

Mem::Heap* MemManagerHandleTopDownHeap()  {
	return Mem::Manager::sHandle().TopDownHeap();
}

Mem::Heap* MemManagerHandleBottomUpHeap()  {
	return Mem::Manager::sHandle().BottomUpHeap();
}

Mem::Heap* MemManagerHandleFrontEndHeap()  {
	return Mem::Manager::sHandle().FrontEndHeap();
}

Mem::Heap* MemManagerHandleScriptHeap()  {
	return Mem::Manager::sHandle().ScriptHeap();
}

Mem::Heap* MemManagerHandleNetworkHeap()  {
	return Mem::Manager::sHandle().NetworkHeap();
}

Mem::Heap* MemManagerHandleNetMiscHeap()  {
	return Mem::Manager::sHandle().NetMiscHeap();
}

Mem::Heap* MemManagerHandleProfilerHeap()  {
	return Mem::Manager::sHandle().ProfilerHeap();
}

Mem::Heap* MemManagerHandleDebugHeap()  {
	return Mem::Manager::sHandle().DebugHeap();
}

Mem::Heap* MemManagerHandleSkaterHeap(int n)  {
	return Mem::Manager::sHandle().SkaterHeap(n);
}

Mem::Heap* MemManagerHandleSkaterInfoHeap()  {
	return Mem::Manager::sHandle().SkaterInfoHeap();
}

Mem::Heap* MemManagerHandleSkaterGeomHeap(int n)  {
	return Mem::Manager::sHandle().SkaterGeomHeap(n);
}

Mem::Heap* MemManagerHandleInternetTopDownHeap()  {
	return Mem::Manager::sHandle().InternetTopDownHeap();
}

Mem::Heap* MemManagerHandleInternetBottomUpHeap()  {
	return Mem::Manager::sHandle().InternetBottomUpHeap();
}

Mem::Heap* MemManagerHandleThemeHeap()  {
	return Mem::Manager::sHandle().ThemeHeap();
}

Mem::Heap* MemManagerHandleCutsceneTopDownHeap()  {
	return Mem::Manager::sHandle().CutsceneTopDownHeap();
}

Mem::Heap* MemManagerHandleCutsceneBottomUpHeap()  {
	return Mem::Manager::sHandle().CutsceneBottomUpHeap();
}

Mem::Heap* MemManagerHandleAudioHeap()  {
	return Mem::Manager::sHandle().AudioHeap();
}

Mem::Heap* MemManagerHandleGetHeap(u32 whichHeap)  {
	return Mem::Manager::sHandle().GetHeap(whichHeap);
}

void MemManagerHandleRemoveHeap(Mem::Heap* heap)  {
	Mem::Manager::sHandle().RemoveHeap(heap);
}

int  MemManagerHandleGetContextDirection()  {
	return (int)Mem::Manager::sHandle().GetContextDirection();
}

void MemManagerHandleInitCutsceneHeap(int heap_size)  {
	Mem::Manager::sHandle().InitCutsceneHeap(heap_size);
}

void MemManagerHandleDeleteCutsceneHeap()  {
	Mem::Manager::sHandle().DeleteCutsceneHeap();
}

Mem::Allocator* MemManagerHandleGetContextAllocator()  {
	return Mem::Manager::sHandle().GetContextAllocator();
}

void MemManagerHandleDeleteSkaterHeaps()  {
	Mem::Manager::sHandle().DeleteSkaterHeaps();
}

void MemManagerHandleInitNetMiscHeap()  {
	Mem::Manager::sHandle().InitNetMiscHeap();
}

void MemManagerHandleDeleteNetMiscHeap()  {
	Mem::Manager::sHandle().DeleteNetMiscHeap();
}

void MemManagerHandleInitOtherHeaps()  {
	Mem::Manager::sHandle().InitOtherHeaps();
}

void MemManagerHandlePushMemoryMarker(int marker)  {
	Mem::Manager::sHandle().PushMemoryMarker((uint32)marker);
}

void MemManagerHandlePopMemoryMarker(int marker)  {
	Mem::Manager::sHandle().PopMemoryMarker((uint32)marker);
}

void MemManagerHandleInitSkaterHeaps(unsigned int num_skater_heaps_required)  {
	Mem::Manager::sHandle().InitSkaterHeaps((int)num_skater_heaps_required);
}

char* MemManagerHandleGetContextName()  {
	return Mem::Manager::sHandle().GetContextName();
}

Mem::Heap* MemManagerHandleCreateHeap(Mem::Region* region, int dir, char* p_name)  {
	return Mem::Manager::sHandle().CreateHeap(region, (Mem::Allocator::Direction)dir, p_name);
}

void* get_font_file_address(const char * p_filename) {
	// TODO: Provide lookup for permanent font data within RES_gamecube.
	// For now, return NULL to indicate not found.
	return NULL;
}

	extern "C" {

	void RenderNewParticles(Nx::CParticleStream * p_stream, float lifetime, float midpercent, bool use_mid_color, Image::RGBA * p_color0, Mth::Vector * p0, Mth::Vector * sr, Mth::Vector * su, float * p_params, float nearz) {
		// TODO: Implement platform-specific particle rendering hook if needed.
	}
	
}

unsigned int AllocatorGetId(void* addr) {
	return Mem::Allocator::sGetId(addr);
}

int  CreateSema(SemaParam* params) {
	// Stub for non-PS2 platforms. Returns a dummy semaphore ID.
	(void)params;
	return 0;
}

void DeleteSema(int sema_id) {
	(void)sema_id;
}

void WaitSema(int sema_id) {
	(void)sema_id;
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
