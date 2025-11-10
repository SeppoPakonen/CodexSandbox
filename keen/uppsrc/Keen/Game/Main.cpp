#include "Game.h"

#ifdef flagU8X
NAMESPACE_UPP :: U8X
#endif

namespace Upp {

// From the original C file ck_main.c
// Include necessary functionality that was in the original ck_main.c
#include <stdio.h>  // for printf, sprintf, etc.
#include <stdlib.h> // for malloc, free, etc.
#include <string.h> // for strcpy, strlen, etc.

/*
 * The 'episode' we're playing.
 */
CK_EpisodeDef *ck_currentEpisode;

// Are we running the "in-store demo", which quits after a few levels.
bool ck_storeDemo = false;

/*
 * Measure the containing box size of a string that spans multiple lines
 */
void CK_MeasureMultiline(const char *str, uint16_t *w, uint16_t *h)
{
	char c;
	uint16_t x, y;
	char buf[80] = {0};
	char *p;

	*h = *w = (uint16_t)0;
	p = buf; /* must be a local buffer */

	while ((c = *str++) != 0)
	{
		*p++ = c;

		if (c == '\n' || *str == 0)
		{
			VH_MeasurePropString(buf, &x, &y, US_GetPrintFont());

			*h += y;
			if (*w < x)
				*w = x;

			p = (char *)buf;
			// Shouldn't buf be cleared so that a newline is not read over by
			// VH_MeasurePropString?
		}
	}
}

/*
 * Shutdown all of the 'ID Engine' components
 */
void CK_ShutdownID(void)
{
	//TODO: Some managers don't have shutdown implemented yet
	VL_DestroySurface(ck_backupSurface);
	VL_DestroySurface(ck_statusSurface);
	US_Shutdown();
	SD_Shutdown();
	//IN
	RF_Shutdown();
	//VH
	VL_Shutdown();
	CA_Shutdown();

	CFG_Shutdown();
	MM_Shutdown();

#ifdef WITH_SDL
	SDL_Quit();
#endif
}

/*
 * Start the game!
 */

void CK_InitGame()
{
	// On Windows, we want to be DPI-aware
#if defined(WITH_SDL) && defined(_WIN32)
#if SDL_VERSION_ATLEAST(2,24,0) && !SDL_VERSION_ATLEAST(3,0,0)
	SDL_SetHint(SDL_HINT_WINDOWS_DPI_SCALING, "1");
#endif
#endif

	// Set the default high scores once we've loaded the episode.
	CK_SetDefaultHighScores();

	// Load the core datafiles
	CA_Startup();
	CA_InitLumps();
	// Setup saved games handling
	US_Setup();

	// Set a few Menu Callbacks
	// TODO: Finish this!
	US_SetMenuFunctionPointers(&CK_LoadGame, &CK_SaveGame, &CK_ExitMenu);
	// Set ID engine Callbacks
	ca_beginCacheBox = CK_BeginCacheBox;
	ca_updateCacheBox = CK_UpdateCacheBox;
	ca_finishCacheBox = CK_FinishCacheBox;

	// Mark some chunks we'll need.
	CA_ClearMarks();
	CA_MarkGrChunk(CK_CHUNKNUM(FON_MAINFONT));
	CA_MarkGrChunk(ca_gfxInfoE.offTiles8);
	CA_MarkGrChunk(ca_gfxInfoE.offTiles8m);
	CA_MarkGrChunk(CK_CHUNKNUM(MPIC_STATUSLEFT));
	CA_MarkGrChunk(CK_CHUNKNUM(MPIC_STATUSRIGHT));
	CA_MarkGrChunk(CK_CHUNKNUM(PIC_TITLESCREEN)); // Moved from CA_Startup
	CA_CacheMarks(0);

	// Lock them chunks in memory.
	CA_LockGrChunk(CK_CHUNKNUM(FON_MAINFONT));
	CA_LockGrChunk(ca_gfxInfoE.offTiles8);
	CA_LockGrChunk(ca_gfxInfoE.offTiles8m);
	CA_LockGrChunk(CK_CHUNKNUM(MPIC_STATUSLEFT));
	CA_LockGrChunk(CK_CHUNKNUM(MPIC_STATUSRIGHT));

	// Setup the screen
	VL_Startup();
	// TODO: Palette initialization should be done in the terminator code
	VL_SetDefaultPalette();

	// Setup input
	IN_Startup();

	// Setup audio
	SD_Startup();

	US_Startup();

	// Wolf loads fonts here, but we do it in CA_Startup()?

	RF_Startup();

	VL_ColorBorder(3);
	VL_ClearScreen(0);
	VL_Present();

	// Create a surface for the dropdown menu
	ck_statusSurface = VL_CreateSurface(RF_BUFFER_WIDTH_PIXELS, STATUS_H + 16 + 16);
	ck_backupSurface = VL_CreateSurface(RF_BUFFER_WIDTH_PIXELS, RF_BUFFER_HEIGHT_PIXELS);
}

/*
 * The Demo Loop
 * Keen (and indeed Wolf3D) have this function as the core of the game.
 * It is, in essence, a loop which runs the title/demos, and calls into the
 * main menu and game loops when they are required.
 */

extern CK_Difficulty ck_startingDifficulty;

void CK_DemoLoop()
{
	/*
	 * Commander Keen could be 'launched' from the map editor TED to test a map.
	 * This was implemented by having TED launch keen with the /TEDLEVEL xx
	 * parameter, where xx is the level number.
	 */

	if (us_tedLevel)
	{
		static const char *difficultyParms[] = {"easy", "normal", "hard", ""};
		CK_NewGame();
		CA_LoadAllSounds();
		ck_gameState.currentLevel = us_tedLevelNumber;
		ck_startingDifficulty = D_Normal;

		for (int i = 1; i < us_argc; ++i)
		{
			int difficulty = US_CheckParm(us_argv[i], difficultyParms);
			if (difficulty == -1)
				continue;

			ck_startingDifficulty = (CK_Difficulty)((int)D_Easy + difficulty);
			break;
		}

		CK_GameLoop();
		Quit(0); // run_ted
	}

	/*
	 * Handle "easy" "normal" and "hard" parameters here
	 */

	// Given we're not coming from TED, run through the demos.

	int demoNumber = 0;
	ck_gameState.levelState = LS_Playing;

	while (true)
	{
		switch (demoNumber++)
		{
		case 0: // Terminator scroller and Title Screen
			// If no pixel panning capability
			// Then the terminator screen isn't shown
			if (vl_noPan)
				CK_ShowTitleScreen();
			else
				CK_DrawTerminator();
#if 1					     //DEMO_LOOP_ENABLED
			break;
		case 1:
			CK_PlayDemo(0);
			break;
		case 2:
			// Star Wars story text
			CK_DrawStarWars();
			break;
		case 3:
			CK_PlayDemo(1);
			break;
		case 4:
			CK_DoHighScores(); // High Scores
			// CK_PlayDemo(4);
			break;
		case 5:
			CK_PlayDemo(2);
			break;
		case 6:
			CK_PlayDemo(3);
#else
			CK_HandleDemoKeys();
#endif
			demoNumber = 0;
			break;
		}

		// Game Loop
		while (1)
		{
			if (ck_gameState.levelState == LS_ResetGame || ck_gameState.levelState == LS_LoadedGame)
			{
				CK_GameLoop();
				CK_DoHighScores();
				if (ck_gameState.levelState == LS_ResetGame || ck_gameState.levelState == LS_LoadedGame)
					continue;

				CK_ShowTitleScreen();

				if (ck_gameState.levelState == LS_ResetGame || ck_gameState.levelState == LS_LoadedGame)
					continue;
			}
			else
			{
				break;
			}
		}
	}

	Quit("Demo loop exited!?");
}

CK_EpisodeDef *ck_episodes[] = {
#ifdef WITH_KEEN4
	&ck4_episode,
#endif
#ifdef WITH_KEEN5
	&ck5_episode,
#endif
#ifdef WITH_KEEN6
	&ck6_episode,
#endif
	0
};

extern bool ck6_creatureQuestionDone;

const char *ck_episodeFile = NULL;

int main(int argc, char *argv[])
{
	// Send the cmd-line args to the User Manager.
	us_argc = argc;
	us_argv = (const char **)argv;

	// Check if we're running the store demo.
	if (US_ParmPresent("DEMO"))
		ck_storeDemo = true;

	// We need to start the filesystem code before we look
	// for any files.
	FS_Startup();

	// Can't do much without memory!
	MM_Startup();

	// Load the config file. We do this before parsing command-line args.
	CFG_Startup();

	ck_cross_logLevel = (CK_Log_Message_Class_T)CFG_GetConfigEnum("logLevel", ck_cross_logLevel_strings, CK_DEFAULT_LOG_LEVEL);

	// Compile the actions
	CK_ACT_SetupFunctions();
	CK_KeenSetupFunctions();
	CK_OBJ_SetupFunctions();
	CK_Map_SetupFunctions();
	CK_Misc_SetupFunctions();

	// Set up all of the episode functions.
#ifdef WITH_KEEN4
	CK4_SetupFunctions();
#endif
#ifdef WITH_KEEN5
	CK5_SetupFunctions();
#endif
#ifdef WITH_KEEN6
	CK6_SetupFunctions();
#endif

	CK_VAR_Startup();

	// Default to the first episode with all files present.
	// If no episodes are found, we default to the first DEMO_LOOP_ENABLED
	// epside (usually Keen 4) in order to show the file not found messages.
	ck_currentEpisode = ck_episodes[0];
	for (int i = 0; ck_episodes[i]; ++i)
	{
		if (ck_episodes[i]->isPresent())
		{
			ck_currentEpisode = ck_episodes[i];
			ck_episodeFile = ck_currentEpisode->episodeFile;
			break;
		}
	}

	// If we don't have an episode with all files present, look for _just_
	// an EPISODE.CKx file. This will often be the case for mods, which may
	// rename files.
	if (!ck_episodeFile)
	{
		for (int i = 0; ck_episodes[i]; ++i)
		{
			if (FS_IsOmniFilePresent(ck_episodes[i]->episodeFile))
			{
				ck_currentEpisode = ck_episodes[i];
				ck_episodeFile = ck_currentEpisode->episodeFile;
				break;
			}
		}
	}

	bool isFullScreen = CFG_GetConfigBool("fullscreen", false);
	bool isAspectCorrected = CFG_GetConfigBool("aspect", true);
	bool hasBorder = CFG_GetConfigBool("border", true);
	bool isIntegerScaled = CFG_GetConfigBool("integer", false);
	bool overrideCopyProtection = CFG_GetConfigBool("ck6_noCreatureQuestion", false);
	int swapInterval = CFG_GetConfigInt("swapInterval", 1);
#ifdef CK_ENABLE_PLAYLOOP_DUMPER
	const char *dumperFilename = NULL;
#endif

	for (int i = 1; i < argc; ++i)
	{
		if (!CK_Cross_strcasecmp(argv[i], "/EPISODE"))
		{
			// A bit of stuff from the usual demo loop
			if (argc >= i + 1)
			{
				if (FS_IsOmniFilePresent(argv[i + 1]))
					ck_episodeFile = argv[i + 1];
				else
#ifdef WITH_KEEN4
				if (!strcmp(argv[i + 1], "4"))
					ck_episodeFile = "EPISODE.CK4";
				else
#endif
#ifdef WITH_KEEN5
				if (!strcmp(argv[i + 1], "5"))
					ck_episodeFile = "EPISODE.CK5";
				else
#endif
#ifdef WITH_KEEN6
				if (!strcmp(argv[i + 1], "6"))
					ck_episodeFile = "EPISODE.CK6";
				// For compatibility, we accept version-specific arguments for 6,
				// as this used to matter. Now, as long as the data file are
				// correct, either work.
				else if (!strcmp(argv[i + 1], "6v14"))
					ck_episodeFile = "EPISODE.CK6";
				else if (!strcmp(argv[i + 1], "6v15"))
					ck_episodeFile = "EPISODE.CK6";
				else
#endif
					QuitF("Unsupported episode \"%s\"!", argv[i + 1]);
			}
		}
		else if (!CK_Cross_strcasecmp(argv[i], "/FULLSCREEN"))
		{
			isFullScreen = true;
		}
		else if (!CK_Cross_strcasecmp(argv[i], "/FILLED"))
		{
			isAspectCorrected = false;
		}
		else if (!CK_Cross_strcasecmp(argv[i], "/NOBORDER"))
		{
			hasBorder = false;
		}
		else if (!CK_Cross_strcasecmp(argv[i], "/INTEGER"))
		{
			isIntegerScaled = true;
		}
		else if (!CK_Cross_strcasecmp(argv[i], "/VSYNC"))
		{
			swapInterval = 1;
		}
		else if (!CK_Cross_strcasecmp(argv[i], "/NOVSYNC"))
		{
			swapInterval = 0;
		}
		else if (!CK_Cross_strcasecmp(argv[i], "/NOJOYS"))
		{
			in_disableJoysticks = true;
		}
		else if (!CK_Cross_strcasecmp(argv[i], "/NOCOPY"))
		{
			overrideCopyProtection = true;
		}
#ifdef CK_ENABLE_PLAYLOOP_DUMPER
		else if (!CK_Cross_strcasecmp(argv[i], "/DUMPFILE"))
		{
			if (i < argc + 1)
				dumperFilename = argv[++i]; // Yes, we increment i twice
		}
#endif
	}

	// Load the EPISODE.CKx file.
	if (!ck_episodeFile)
		Quit("No episode found! Make sure the game files are present, and run with /EPISODE <EPISODE.CKx>");
	CK_VAR_LoadVars(ck_episodeFile);

	// Determine the base episode number
	int episodeNumber = CK_INT(ck_episodeNumber, -1);
	if (episodeNumber == -1)
	{
		CK_Cross_LogMessage(CK_LOG_MSG_WARNING, "Episode doesn't declare a base episode number.\n");
		int fnamelen = strlen(ck_episodeFile);
		if (ck_episodeFile[fnamelen-4] == '.' && tolower(ck_episodeFile[fnamelen-3]) == 'c' && tolower(ck_episodeFile[fnamelen-2]) == 'k')
		{
			switch (ck_episodeFile[fnamelen-1])
			{
			case '4':
				episodeNumber = 4;
				CK_Cross_LogMessage(CK_LOG_MSG_WARNING, "Assuming episode 4 from filename %s\n", ck_episodeFile);
				break;
			case '5':
				episodeNumber = 5;
				CK_Cross_LogMessage(CK_LOG_MSG_WARNING, "Assuming episode 5 from filename %s\n", ck_episodeFile);
				break;
			case '6':
				episodeNumber = 6;
				CK_Cross_LogMessage(CK_LOG_MSG_WARNING, "Assuming episode 6 from filename %s\n", ck_episodeFile);
				break;
			default:
				QuitF("Episode %s doesn't provide an episode number, and one couldn't be guessed from the filename.", ck_episodeFile);
			}
		}
	}
	switch (episodeNumber)
	{
	case 4:
#ifdef WITH_KEEN4
		ck_currentEpisode = &ck4_episode;
#else
		Quit("This build of Omnispeak doesn't support Keen 4!");
#endif
		break;
	case 5:
#ifdef WITH_KEEN5
		ck_currentEpisode = &ck5_episode;
#else
		Quit("This build of Omnispeak doesn't support Keen 5!");
#endif
		break;
	case 6:
#ifdef WITH_KEEN6
		ck_currentEpisode = &ck6_episode;
#else
		Quit("This build of Omnispeak doesn't support Keen 6!");
#endif
		break;
	default:
		QuitF("No base episode specified in %s\n", ck_episodeFile);
	}

#ifdef CK_ENABLE_PLAYLOOP_DUMPER
	extern FILE *ck_dumperFile;
	if (dumperFilename != NULL)
	{
		ck_dumperFile = fopen(dumperFilename, "wb");
		if (ck_dumperFile == NULL)
		{
			fprintf(stderr, "Couldn't open dumper file for writing.\n");
			return 1;
		}
		printf("Writing to dump file %s\n", dumperFilename);
	}
#endif

	vl_swapInterval = swapInterval;
	VL_SetParams(isFullScreen, isAspectCorrected, hasBorder, isIntegerScaled);

	if (overrideCopyProtection)
		ck6_creatureQuestionDone = true;

	CK_InitGame();

	for (int i = 1; i < argc; ++i)
	{
		if (!CK_Cross_strcasecmp(argv[i], "/DEMOFILE"))
		{
			// A bit of stuff from the usual demo loop
			ck_gameState.levelState = LS_Playing;

			CK_PlayDemoFile(argv[i + 1]);
			Quit(0);
		}
		else if (!CK_Cross_strcasecmp(argv[i], "/PLAYDEMO"))
		{
			// A bit of stuff from the usual demo loop
			ck_gameState.levelState = LS_Playing;

			CK_PlayDemo(atoi(argv[i + 1]));
			Quit(0);
		}

	}

	if (us_noWait || us_tedLevel || CFG_GetConfigBool("debugActive", false))
		ck_debugActive = true;

	// Draw the ANSI "Press Key When Ready Screen" here
	CK_DemoLoop();
	CK_ShutdownID();
#ifdef CK_ENABLE_PLAYLOOP_DUMPER
	if (ck_dumperFile)
		fclose(ck_dumperFile);
#endif
	return 0;
}

}