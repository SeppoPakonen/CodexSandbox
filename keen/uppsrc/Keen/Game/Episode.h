#ifndef UPP_KEEN_EPISODE_H
#define UPP_KEEN_EPISODE_H

//#include "Game.h"

//NAMESPACE_UPP

struct CK_object;

//This structure defines an episode of Commander Keen, providing
//function pointers to episode specific code and data.
typedef struct CK_EpisodeDef
{
	// Identifier
	CK_Episode ep;

	// Extension for data files
	char ext[4];

	// Name of the EPISODE.CKx variable file.
	const char *episodeFile;

	// Scan the 'info layer' of a map.
	void (*scanInfoLayer)();

	// Check if the episode's files are all present.
	bool (*isPresent)();
} CK_EpisodeDef;

extern CK_EpisodeDef *ck_currentEpisode;

//END_UPP_NAMESPACE

#endif