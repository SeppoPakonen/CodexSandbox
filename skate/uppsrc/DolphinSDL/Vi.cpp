#include "DolphinSDL.h"


u32			ScreenWidth  = 640;
u32			ScreenHeight = 480;
u32			retraceCount;

// SDL window + GL context for the main VI surface
static SDL_Window* s_window = nullptr;
static SDL_GLContext s_glctx = nullptr;

void VIInit() {
	// Minimal SDL2 + OpenGL window initialization
	static bool initialized = false;
	if (initialized)
		return;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return;
	}

	// Request a reasonable GL context
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	Uint32 win_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

	s_window = SDL_CreateWindow(
		"DolphinSDL",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		(int)ScreenWidth,
		(int)ScreenHeight,
		win_flags
	);
	if (!s_window) {
		return;
	}

	s_glctx = SDL_GL_CreateContext(s_window);
	if (!s_glctx) {
		return;
	}

	// Enable vsync if possible
	SDL_GL_SetSwapInterval(1);

	// Initialize GLEW after creating a valid context
	GLenum err = glewInit();
	(void)err; // ignore error for now; not all platforms provide GLEW the same way

	// Basic GL defaults
	glViewport(0, 0, (GLsizei)ScreenWidth, (GLsizei)ScreenHeight);
	glClearColor(0.f, 0.f, 0.f, 1.f);

	// Expose swap via a weak-level function pointer so SDL2.cpp can call it
	// without tight coupling. Store in SDL window user data.
	SDL_SetWindowData(s_window, "__GLCTX__", (void*)s_glctx);
	SDL_SetWindowData(s_window, "__WINDOW__", (void*)s_window);

	retraceCount = 0;
	initialized = true;
}

void VIWaitForRetrace() {
	// Crude retrace wait: yield a tiny bit to avoid a hot loop
	SDL_Delay(1);
}
void VIConfigureTVScreen(u16 xOrg, u16 yOrg, u16 xSize, VITVMode mode) {}
void VIConfigureXFrameBuffer(u16 xSize, u16 ySize, VIXFBMode mode) {}
void VIConfigurePan(u16 xOrg, u16 yOrg, u16 width, u16 height) {}
void VISetNextFrameBuffer (void *fb) {}
void VISetFrameBuffer (void *fb) {}
void VISetBlack(BOOL black) {
	if (black) {
		glClearColor(0.f, 0.f, 0.f, 1.f);
	} else {
		glClearColor(0.f, 0.f, 0.f, 1.f);
	}
}

u32 VIGetRetraceCount() {
	return retraceCount;
}

u32 VIGetNextField() {
	return 0;
}

void VIFlush() {
	// Present the back buffer and count a retrace
	if (s_window)
		SDL_GL_SwapWindow(s_window);
	retraceCount++;
}

// Provide buffer swap for external callers
void WinSwapBuffers() {
	if (s_window)
		SDL_GL_SwapWindow(s_window);
}

void VIConfigure(GXRenderModeObj* rm) {
	
}

u32 VIGetTvFormat() {
	return VI_PAL;
}

VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback cb) {
    return cb;
}

VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback cb) {
    return cb;
}
