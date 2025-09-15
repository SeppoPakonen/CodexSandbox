//============================================================================
// PC Dolphin Emulator Version 2.6
// Nintendo of America, Inc.
// Product Support Group
// Dante Treglia II
// danttr01@noa.nintendo.com
// April 12, 2000
// Copyright (c) 2000 Nintendo of America Inc.
//============================================================================

#include <dolphin.h>
#include <win32/win32.h>

static OPENFILENAME ofn ;

char WINFileName[255];
char WINTitleName[255];
char FilePath[255];
char FileName[255];
char FileType[255];

u8 WINGetFile(char * filter) {
	static char szFilter[] = "Dolphin Files (*.gpl; *.act; *.anm; *.tpl)\0*.gpl;*.act;*.anm;*.tpl\0"; 
	u8 value;
	char * tempP;

	ofn.lStructSize       = sizeof(OPENFILENAME);
	ofn.hwndOwner         = NULL;
	ofn.hInstance         = NULL;
	if (filter) ofn.lpstrFilter       = filter;
	else ofn.lpstrFilter              = szFilter;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter    = 0;
	ofn.nFilterIndex      = 0;
	ofn.lpstrFile         = WINFileName;
	ofn.nMaxFile          = _MAX_PATH;
	ofn.lpstrFileTitle    = FileName;
	ofn.nMaxFileTitle     = _MAX_FNAME + _MAX_EXT;
	ofn.lpstrInitialDir   = NULL;
	ofn.lpstrTitle        = "Open File";
	ofn.Flags             = OFN_OVERWRITEPROMPT;
	ofn.nFileOffset       = 0;
	ofn.nFileExtension    = 0;
	ofn.lpstrDefExt       = "gpl";
	ofn.lCustData         = 0L;
	ofn.lpfnHook          = NULL;
	ofn.lpTemplateName    = NULL;

	// Hack: Couldn't get the keyup message
	winKey.button = 0;

	value = GetOpenFileName(&ofn);

	if (!value) return 0;

	// Path
	if (tempP = strstr(WINFileName, FileName)) {
		strncpy(FilePath, WINFileName, (int)(tempP - WINFileName));
	}
	// Type
	if (tempP = strrchr(FileName, '.')) {
		strcpy(FileType, ++tempP);
	}	



	return 1;
}

