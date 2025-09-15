#ifndef _System_SDL2_h_
#define _System_SDL2_h_

enum {
	K_DELTA        = 0x010000,

	K_ALT          = 0x080000,
	K_SHIFT        = 0x040000,
	K_CTRL         = 0x020000,

	K_KEYUP        = 0x100000,

	K_MOUSEMIDDLE  = 0x200000,
	K_MOUSERIGHT   = 0x400000,
	K_MOUSELEFT    = 0x800000,
	K_MOUSEDOUBLE  = 0x1000000,
	K_MOUSETRIPLE  = 0x2000000,

	K_SHIFT_CTRL = K_SHIFT|K_CTRL,

#ifdef PLATFORM_COCOA
	K_OPTION       = 0x4000000,
#endif
};

enum {
	#include "Keys.h"
};


#if 0

class SDL2  {
	
    SDL_Window* display_window = NULL;
    SDL_Renderer* display_renderer = NULL;
    SDL_RendererInfo display_renderer_info;
	SDL_GLContext context;
	
	
	static int width, height;
	bool is_open = false;
	bool is_maximized = false;
	bool is_sizeable = false;
	
	
	uint32 prev_ticks;
	uint32 prev_global_ticks;
	
	bool is_lalt = false, is_lshift = false, is_lctrl = false;
	bool is_ralt = false, is_rshift = false, is_rctrl = false;
	uint32 key = 0;
	uint32 mouse_code;
	int mouse_zdelta, x, y, w, h, dx, dy;
	
	struct Point {
		int x, y;
	};
	Point prev_mouse_pt, mouse_pt;
	
	
public:
	SDL2();
	
	void RunTest();
	
    void Initialize();
    void Start();
    void Update(float dt);
    void Stop();
    void Uninitialize();
    
	bool Poll(Event& e);
	void Render();
	
protected:
	friend class Font;
	friend class CoreWindow;
	friend void WinSwapBuffers();
	
      
	bool Open(int w, int h, bool full_screen=false);
	void Close();
	
	SDL2& Maximize(bool b=true);
	SDL2& Sizeable(bool b=true) {is_sizeable = b; return *this;}
	void SwapBuffer();
	void KeyboardPad(int key, bool down);
	
	int GetWidth() {return width;}
	int GetHeight() {return height;}
	
	Callback WhenClose;
	Callback WhenUserShutdownSignal;
	Callback2<int, int> WhenKey;
	Callback1<Point> WhenMouseMove;
	Callback2<Point, int> WhenMouse, WhenMouseScroll;
	
};





inline SDL2& GetScreen() {return Single<SDL2>();}



void WinGLExtInit();

#endif
#endif
