#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "id_cfg.h"

#ifdef _WIN32

#include <windows.h>
#include <conio.h>

typedef enum US_Win32ConsoleMode
{
	US_W32Console_Never,
	US_W32Console_IfAvailable,
	US_W32Console_PreferExisting,
	US_W32Console_CreateNew
} US_Win32ConsoleMode;

const char *US_Win32ConsoleMode_Strings[] = {
	"Never",
	"IfAvailable",
	"PreferExisting",
	"CreateNew"
};

static bool us_console_createdW32Console = false;

// Checks if the terminal is compatible with our B8000 text mode emulation.
bool US_TerminalOk()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD consoleMode;
	US_Win32ConsoleMode mode = CFG_GetConfigEnum("win32_consoleMode", US_Win32ConsoleMode_Strings, US_W32Console_IfAvailable);

	if (mode == US_W32Console_Never)
		return false;

	if (mode == US_W32Console_CreateNew)
		FreeConsole();

	// MSDN notes that this is a good way to check a console is a console.
	if (!GetConsoleMode(hConsole, &consoleMode))
	{
		// Try to allocate a console if we don't have one.
		if (mode >= US_W32Console_PreferExisting)
		{
			if (!AllocConsole())
				return false;
			us_console_createdW32Console = true;
		}
		else
			return false;
	}

	// Check if CP 437 is available
	UINT oldCP = GetConsoleOutputCP();
	if (!SetConsoleOutputCP(437))
		return false;

	SetConsoleOutputCP(oldCP);

	return true;
}

void US_PrintB8000Text(const uint8_t *textscreen, int numChars)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO oldInfo;
	DWORD oldMode;
	UINT oldCP = GetConsoleOutputCP();
	GetConsoleScreenBufferInfo(hConsole, &oldInfo);
	GetConsoleMode(hConsole, &oldMode);
	SetConsoleOutputCP(437);
	SetConsoleMode(hConsole, oldMode & ~ENABLE_WRAP_AT_EOL_OUTPUT);

	for (int i = 0; i < numChars; ++i)
	{
		uint8_t ch = textscreen[i * 2];
		uint8_t attrib = textscreen[i * 2 + 1];

		SetConsoleTextAttribute(hConsole, attrib);
		WriteConsoleA(hConsole, &ch, 1, NULL, NULL);

		// We want to print a newline after every 80 characters
		if ((i % 80) == 79)
		{
			SetConsoleTextAttribute(hConsole, oldInfo.wAttributes);
			WriteConsoleA(hConsole, "\r\n", 2, NULL, NULL);
		}

	}
	SetConsoleOutputCP(oldCP);
	SetConsoleMode(hConsole, oldMode);
	SetConsoleTextAttribute(hConsole, oldInfo.wAttributes);
	if (CFG_GetConfigBool("us_pressKeyOnExit", us_console_createdW32Console))
	{
		printf("Press any key to exit...");
		_getch();
		puts("\n");
	}
}

#elif defined(__DJGPP__)

#include <conio.h>
#include <sys/movedata.h>

bool US_TerminalOk()
{
	return true;
}

void US_PrintB8000Text(const uint8_t *textscreen, int numChars)
{
	dosmemput(textscreen, numChars * 2, 0xB8000);
	gotoxy(0, numChars / 80);
}


#else
// Conversion for Codepage 437 (the IBM BIOS font character set) to unicode.
uint32_t cp437[] = {
	// So called "non-printable" characters.
	0x0000, 0x263A, 0x263B, 0x2665, 0x2666, 0x2663, 0x2660, 0x2022,
	0x25D8, 0x25CB, 0x25D9, 0x2642, 0x2640, 0x266A, 0x266B, 0x263C,
	0x25BA, 0x25C4, 0x2195, 0x203C, 0x00B6, 0x00A7, 0x25AC, 0x21A8,
	0x2191, 0x2193, 0x2192, 0x2190, 0x221F, 0x2194, 0x25B2, 0x25BC,
	// ASCII range
	0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
	0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
	0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
	0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
	0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
	0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,
	0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
	0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
	0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067,
	0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
	0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
	0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x2302,
	// Accented and special characters
	0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x00E0, 0x00E5, 0x00E7,
	0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x00EC, 0x00C4, 0x00C5,
	0x00C9, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00F2, 0x00FB, 0x00F9,
	0x00FF, 0x00D6, 0x00DC, 0x00A2, 0x00A3, 0x00A5, 0x20A7, 0x0192,
	0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA,
	0x00BF, 0x2310, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,
	// Box-drawing
	0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556,
	0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,
	0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F,
	0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
	0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B,
	0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
	// Greek letters and mathematical symbols.
	0x03B1, 0x00DF, 0x0393, 0x03C0, 0x03A3, 0x03C3, 0x00B5, 0x03C4,
	0x03A6, 0x0398, 0x03A8, 0x03B4, 0x221E, 0x03C6, 0x03B5, 0x2229,
	0x2261, 0x00B1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00F7, 0x2248,
	0x00B0, 0x2219, 0x00B7, 0x221A, 0x207F, 0x00B2, 0x25A0, 0x00A0};

// Conversion table for EGA colours to ANSI.
int colorconv[] = {
	0, // Black,
	4, // Blue,
	2, // Green,
	6, // Cyan,
	1, // Red,
	5, // Magenta,
	3, // Yellow/Brown,
	7  // White
};

// Terminal types we support.
const char *okterms[] = {
	"xterm",
	"xterm-16color",
	"xterm-88color",
	"xterm-256color",
	"ansi",
	"rxvt",
	"rxvt-16color",
	"rxvt-256color",
	"konsole",
	"konsole-16color",
	"konsole-256color",
	0};

// Checks if the terminal is compatible with our B8000 text mode emulation.
bool US_TerminalOk()
{
	// We need a UTF-8 character encoding.
	const char *lang = getenv("LANG");
	if (!lang)
		return false;
	if (!strstr(lang, "UTF-8"))
		return false;

	// We need a terminal which supports ANSI escape codes.
	const char *term = getenv("TERM");
	if (!term)
		return false;
	for (int i = 0; okterms[i]; ++i)
		if (!strcmp(term, okterms[i]))
			return true;

	return false;
}

void US_PrintB8000Text(const uint8_t *textscreen, int numChars)
{
	for (int i = 0; i < numChars; ++i)
	{
		uint8_t ch = textscreen[i * 2];
		uint8_t attrib = textscreen[i * 2 + 1];

		// Decode the attribute byte.
		int fgcol = colorconv[attrib & 7];
		bool bold = attrib & 8;
		int bgcol = colorconv[(attrib >> 4) & 7];
		bool blinking = attrib & 0x80;
		// Set colours
		printf("\x1b[%d;%dm", fgcol + 30, bgcol + 40);
		// Is bold?
		if (bold)
			printf("\x1b[1m");
		// Is blinking
		if (blinking)
			printf("\x1b[5m");

		uint32_t unich = cp437[ch];

		// Output as UTF-8 (
		if (unich < 0x80)
			printf("%c", unich);
		else if (unich < 0x800)
			printf("%c%c", (unich >> 6) | 0xC0, (unich & 0x3F) | 0x80);
		else if (unich < 0x10000)
			printf("%c%c%c", (unich >> 12) | 0xE0, ((unich >> 6) & 0x3F) | 0x80, (unich & 0x3F) | 0x80);
		else
			printf("HELP!");

		// Reset
		printf("\x1b[0m");

		// We want to print a newline after every 80 characters
		if ((i % 80) == 79)
			printf("\n");
	}
}
#endif
