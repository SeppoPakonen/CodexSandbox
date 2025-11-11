#ifndef UPP_KEEN_ACT_H
#define UPP_KEEN_ACT_H

//#include "Game.h"

//NAMESPACE_UPP

void CK_ACT_SetupFunctions();
void CK_ACT_AddFunction(const char *fnName, CK_ACT_Function fn);
void CK_ACT_AddColFunction(const char *fnName, CK_ACT_ColFunction fn);
CK_ACT_Function CK_ACT_GetFunction(const char *fnName);
CK_ACT_ColFunction CK_ACT_GetColFunction(const char *fnName);
void CK_VAR_Startup();
const char *CK_VAR_GetString(const char *name, const char *def);
intptr_t CK_VAR_GetInt(const char *name, intptr_t def);
intptr_t *CK_VAR_GetIntArray(const char *name);
intptr_t CK_VAR_GetIntArrayElement(const char *name, int index);
const char **CK_VAR_GetStringArray(const char *name);
const char *CK_VAR_GetStringArrayElement(const char *name, int index);
CK_action *CK_GetActionByName(const char *name);
CK_action *CK_GetOrCreateActionByName(const char *name);
CK_action *CK_LookupActionFrom16BitOffset(uint16_t offset); // POTENTIALLY SLOW function - Use in game loading only!
void CK_VAR_SetInt(const char *name, intptr_t val);
void CK_VAR_SetString(const char *name, const char *val);
void CK_VAR_LoadVars(const char *filename);

//END_UPP_NAMESPACE

#endif