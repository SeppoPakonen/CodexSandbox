#include "DolphinSDL.h"

#if 0

WinKey winKey;


int SDL2::width = 800, SDL2::height = 600;





SDL2::SDL2() {
	
	prev_ticks = SDL_GetTicks();
	prev_global_ticks = GetGlobalTicks();
	
}

void SDL2::Initialize() {
	
	is_open = Open(width, height, false);
	
	
}

void SDL2::Start() {
	
}

void SDL2::Update(float dt) {
	if (!is_open) return;
	
	Event e;
	while (Poll(e)) {
		
	}
	
	Render();
	
}

void SDL2::Stop() {
	if (!is_open) return;
	
}

void SDL2::Uninitialize() {
	if (!is_open) return;
	
	Close();
	
}





void GLAPIENTRY
#ifdef flagHAVE_DIRECT_OPENGL
MessageCallback( GLenum source,
                 GLenum type,
                 Gxu id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
#else
MessageCallback( DbgSource source,
                 DbgType type,
                 Gxu id,
                 DbgSeverity severity,
                 Uint length,
                 const GLchar* message,
                 const void* userParam )
#endif
{
	char s[512];
	sprintf(s, "GL CALLBACK: %s source = 0x%x, type = 0x%x, severity = 0x%x, message = %s",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
	    source, type, severity, message );
	
	LOG((const char*)s);
}


bool SDL2::Open(int w, int h, bool full_screen) {
	width = w;
	height = h;
	
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;
	
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	uint32 flags = SDL_WINDOW_OPENGL;
	
	//if (full_screen)
	//	flags |= SDL_WINDOW_FULLSCREEN;
	
	if (is_sizeable)
		flags |= SDL_WINDOW_RESIZABLE;
	
	if (is_maximized)
		flags |= SDL_WINDOW_MAXIMIZED;

    if (SDL_CreateWindowAndRenderer(w, h, flags, &display_window, &display_renderer) == -1)
        return false;
    
    SDL_GetRendererInfo(display_renderer, &display_renderer_info);
    
	if ((display_renderer_info.flags & SDL_RENDERER_ACCELERATED) == 0 ||
        (display_renderer_info.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
        return false;
	}
	
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	context = SDL_GL_CreateContext(display_window);
	
	
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		LOG("Glew error: " << (const char*)glewGetErrorString(err));
		return false;
	}
	
	#ifdef flagHAVE_DIRECT_OPENGL
	if (0) {
		glEnable              (GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	}
	#endif
	
    TTF_Init();
    
	#if defined flagDX12_PBR || defined flagOPENGL_PBR
	if (full_screen)
		SDL_SetWindowFullscreen(display_window, SDL_WINDOW_FULLSCREEN);
	#endif
	
    return true;
}

void SDL2::Close() {
    WhenClose();
    
    TTF_Quit();
	
    SDL_Quit();
    
    is_open = false;
}

SDL2& SDL2::Maximize(bool b) {
	if (is_open) {
		/*SDL_SysWMinfo info;
		SDL_VERSION(&info.version);
		SDL_GetWMInfo(&info);
		ShowWindow(info.window, SW_MAXIMIZE);*/
	}
	is_maximized = true;
	return *this;
}

bool SDL2::Poll(Event& e) {
	SDL_Event event;
	
	// Process the events
	while (SDL_PollEvent(&event)) {
	switch (event.type) {
		
		case SDL_WINDOWEVENT:
			
			if (event.window.event == SDL_WINDOWEVENT_LEAVE) {
				//DeepMouseLeave();
			}
			else if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
				w = event.window.data1;
				h = event.window.data2;
			}
			else if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
				e.type = EVENT_SHUTDOWN;
				WhenUserShutdownSignal();
				return true;
			}
			break;
		
			
		case SDL_KEYDOWN:
		
			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:	event.type = SDL_QUIT; break;
				case SDLK_LALT:		is_lalt = true; break;
				case SDLK_RALT:		is_ralt = true; break;
				case SDLK_LSHIFT:	is_lshift = true; break;
				case SDLK_RSHIFT:	is_rshift = true; break;
				case SDLK_LCTRL:	is_lctrl = true; break;
				case SDLK_RCTRL:	is_rctrl = true; break;
			}
			
			key = event.key.keysym.sym;
			if (is_lalt || is_ralt)			key |= K_ALT;
			if (is_lshift || is_rshift)		key |= K_SHIFT;
			if (is_lctrl || is_rctrl)		key |= K_CTRL;
			
			KeyboardPad(key, true);
			WhenKey(key, 1);
			
			break;
			
		case SDL_KEYUP:
		
			switch (event.key.keysym.sym) {
				case SDLK_LALT:		is_lalt = false; break;
				case SDLK_RALT:		is_ralt = false; break;
				case SDLK_LSHIFT:	is_lshift = false; break;
				case SDLK_RSHIFT:	is_rshift = false; break;
				case SDLK_LCTRL:	is_lctrl = false; break;
				case SDLK_RCTRL:	is_rctrl = false; break;
			}
			
			key = event.key.keysym.sym | K_KEYUP;
			if (is_lalt || is_ralt)			key |= K_ALT;
			if (is_lshift || is_rshift)		key |= K_SHIFT;
			if (is_lctrl || is_rctrl)		key |= K_CTRL;
			
			KeyboardPad(key & ~K_KEYUP, false);
			
			WhenKey(key, 1);
			
			break;
			
		case SDL_MOUSEMOTION:
			mouse_pt = Point(event.motion.x, event.motion.y);
			WhenMouseMove(mouse_pt);
			prev_mouse_pt = mouse_pt;
			break;
		
		case SDL_MOUSEWHEEL:
			WhenMouseScroll(prev_mouse_pt, event.wheel.y);
			break;
			
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			
			mouse_code = 0;
			//mouse_zdelta = 0;
			if (event.button.state == SDL_PRESSED) {
				if (event.button.clicks == 1) {
					if (event.button.button == SDL_BUTTON_LEFT)
						mouse_code = LEFT_DOWN;
					else if (event.button.button == SDL_BUTTON_MIDDLE)
						mouse_code = MIDDLE_DOWN;
					else if (event.button.button == SDL_BUTTON_RIGHT)
						mouse_code = RIGHT_DOWN;
				}
				else if (event.button.clicks == 2) {
					if (event.button.button == SDL_BUTTON_LEFT)
						mouse_code = LEFT_DOUBLE;
					else if (event.button.button == SDL_BUTTON_MIDDLE)
						mouse_code = MIDDLE_DOUBLE;
					else if (event.button.button == SDL_BUTTON_RIGHT)
						mouse_code = RIGHT_DOUBLE;
				}
				else {
					if (event.button.button == SDL_BUTTON_LEFT)
						mouse_code = LEFT_TRIPLE;
					else if (event.button.button == SDL_BUTTON_MIDDLE)
						mouse_code = MIDDLE_TRIPLE;
					else if (event.button.button == SDL_BUTTON_RIGHT)
						mouse_code = RIGHT_TRIPLE;
				}
				/*else if (event.button.button == SDL_BUTTON_WHEELUP)
					mouse_zdelta = 120;
				else if (event.button.button == SDL_BUTTON_WHEELDOWN)
					mouse_zdelta = -120;*/
			}
			else if (event.button.state == SDL_RELEASED) {
				if (event.button.button == SDL_BUTTON_LEFT)
					mouse_code = LEFT_UP;
				else if (event.button.button == SDL_BUTTON_MIDDLE)
					mouse_code = MIDDLE_UP;
				else if (event.button.button == SDL_BUTTON_RIGHT)
					mouse_code = RIGHT_UP;
			}
			
			mouse_pt = Point(event.button.x, event.button.y);
			/*if (mouse_zdelta)
				DeepMouseWheel(mouse_pt, mouse_zdelta, 0);
			else*/
			WhenMouse(mouse_pt, mouse_code);
			prev_mouse_pt = mouse_pt;
			break;
		
			
		default:
			break;
		}
	}
	
	
	return false;
}

void SDL2::Render() {
	uint32 ticks = SDL_GetTicks();
	uint32 diff = ticks - prev_ticks;
	if (diff > 0) {
		AddGlobalTicks(diff);
		prev_ticks = ticks;
	}
	
	
	
}


void SDL2::SwapBuffer() {
	SDL_GL_SwapWindow(display_window);
}

void WinSwapBuffers() {
	GetScreen().SwapBuffer();
}

void SDL2::KeyboardPad(int key, bool down) {
	
	if      (key == K_LEFT)		winKey.stickX = down ? -127 : 0;
	else if (key == K_RIGHT)	winKey.stickX = down ? +127 : 0;
	else if (key == K_UP)		winKey.stickY = down ? -127 : 0;
	else if (key == K_DOWN)		winKey.stickY = down ? +127 : 0;
	else if (key == K_A)		winKey.substickX = down ? -127 : 0;
	else if (key == K_D)		winKey.substickX = down ? +127 : 0;
	else if (key == K_S)		winKey.substickX = down ? -127 : 0;
	else if (key == K_W)		winKey.substickX = down ? +127 : 0;
	
	if (down) {
		if      (key == K_J)			winKey.button |= WINKEY_PAD_BUTTON_LEFT;
		if      (key == K_L)			winKey.button |= WINKEY_PAD_BUTTON_RIGHT;
		if      (key == K_I)			winKey.button |= WINKEY_PAD_BUTTON_UP;
		if      (key == K_K)			winKey.button |= WINKEY_PAD_BUTTON_DOWN;
		if      (key == K_NUMPAD2)		winKey.button |= WINKEY_PAD_BUTTON_A;
		if      (key == K_NUMPAD4)		winKey.button |= WINKEY_PAD_BUTTON_B;
		if      (key == K_NUMPAD5)		winKey.button |= WINKEY_PAD_TRIGGER_Z;
		if      (key == K_NUMPAD6)		winKey.button |= WINKEY_PAD_BUTTON_X;
		if      (key == K_NUMPAD8)		winKey.button |= WINKEY_PAD_BUTTON_Y;
		if      (key == K_NUMPAD7)		winKey.button |= WINKEY_PAD_TRIGGER_L;
		if      (key == K_NUMPAD9)		winKey.button |= WINKEY_PAD_TRIGGER_R;
		if      (key == K_NUMPAD0)		winKey.button |= WINKEY_PAD_BUTTON_MENU;
		if      (key == K_NUMPAD1)		winKey.triggerLeft = 255;
		if      (key == K_NUMPAD3)		winKey.triggerRight = 255;
		if      (key == K_ESCAPE)		WhenUserShutdownSignal();
	}
	else {
		if      (key == K_J)			winKey.button &= ~(u32)WINKEY_PAD_BUTTON_LEFT;
		if      (key == K_L)			winKey.button &= ~(u32)WINKEY_PAD_BUTTON_RIGHT;
		if      (key == K_I)			winKey.button &= ~(u32)WINKEY_PAD_BUTTON_UP;
		if      (key == K_K)			winKey.button &= ~(u32)WINKEY_PAD_BUTTON_DOWN;
		if      (key == K_NUMPAD2)		winKey.button &= ~(u32)WINKEY_PAD_BUTTON_A;
		if      (key == K_NUMPAD4)		winKey.button &= ~(u32)WINKEY_PAD_BUTTON_B;
		if      (key == K_NUMPAD5)		winKey.button &= ~(u32)WINKEY_PAD_TRIGGER_Z;
		if      (key == K_NUMPAD6)		winKey.button &= ~(u32)WINKEY_PAD_BUTTON_X;
		if      (key == K_NUMPAD8)		winKey.button &= ~(u32)WINKEY_PAD_BUTTON_Y;
		if      (key == K_NUMPAD7)		winKey.button &= ~(u32)WINKEY_PAD_TRIGGER_L;
		if      (key == K_NUMPAD9)		winKey.button &= ~(u32)WINKEY_PAD_TRIGGER_R;
		if      (key == K_NUMPAD0)		winKey.button &= ~(u32)WINKEY_PAD_BUTTON_MENU;
		if      (key == K_NUMPAD1)		winKey.triggerLeft = 0;
		if      (key == K_NUMPAD3)		winKey.triggerRight = 0;
	}
	
	
	
}

#else


void WinSwapBuffers() {
	TODO
}

#endif
