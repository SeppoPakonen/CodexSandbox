#include "Game.h"
#include <stdio.h>
#include <cstdlib>
#include <cstdarg>

NAMESPACE_UPP

void Quit(const char *msg)
{
	// Shutdown VL early to return to text mode.
	VL_Shutdown();
	if (!msg || !(*msg))
	{
		// Avoid trying to re-print the order screen if caching it failed.
		static bool quitting = false;
		if (US_TerminalOk() && !quitting)
		{
			quitting = true;
			CA_CacheGrChunk(CK_CHUNKNUM(EXTERN_ORDERSCREEN));
			// There is a 7-byte BSAVE header at the start of the
			// chunk, and we don't want to print the last row, as
			// originally it would be overwritten by DOS anyway.
			US_PrintB8000Text((uint8_t *)(ca_graphChunks[CK_CHUNKNUM(EXTERN_ORDERSCREEN)]) + 7, 2000 - 80);
		}
		else
			CK_Cross_LogMessage(CK_LOG_MSG_NORMAL, "Thanks for playing Commander Keen!\n");
		CK_ShutdownID();
		exit(0);
	}
	else
	{
		CK_Cross_puts(msg);
		CK_ShutdownID();
#ifdef WITH_SDL
#if SDL_VERSION_ATLEAST(1, 3, 0)
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Omnispeak", msg, NULL);
#endif
#endif
		exit(-1);
	}
}

void QuitF(const char *msg, ...)
{
	char msg_buf[512];
	va_list args;
	va_start(args, msg);
	vsnprintf(msg_buf, sizeof(msg_buf), msg, args);
	va_end(args);
	Quit(msg_buf);
}

END_UPP_NAMESPACE