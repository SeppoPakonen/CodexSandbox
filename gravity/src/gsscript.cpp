/*
 *   GRAVITY STRIKE -- gsscript.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include <string.h>
#include "gsscript.h"
#include "gsbase.h"
#include "gsobject.h"
#include "gserror.h"
#include "gsplayer.h"
#include "gsmap.h"
#include "gsglob.h"
#include "gslng.h"
#include "gsenemy.h"
#include "tolua.h"

Lua    *glob_lua;           // lua (scripting language) object
int    debug_lua = 0;

void tolua_myerror(lua_State *, char *, tolua_Error*);

int tolua_gslua_open (lua_State* tolua_S);
//void tolua_gslua_close (lua_State* tolua_S);

static int script_set_string(lua_State *L);
static int script_mod(lua_State *L);
static int script_get_key(lua_State *L);
static int script_get_mouse_xy(lua_State *L);
static int script_set_mouse_xy(lua_State *L);
static int script_get_mouse_buttons(lua_State *L);

void tolua_myerror(lua_State *s, char *msg, tolua_Error* err)
{
  char myerror[1000];
  sprintf(myerror, "SCRIPT ERROR: %s (%i, %i, %s)", msg, err->index, err->array, err->type);
  
  #ifndef DEBUG_LUA
  errors->log(0, "SCRIPT ERROR", myerror);
  #endif
  fprintf(stderr, "%s", myerror);
}

gsScript::gsScript()
{
  testint = 0;
  max_sum = 500;
  script_buffer = NULL;
  debug_lua = 0;
}

gsScript::~gsScript()
{
  if (script_buffer) delete[] script_buffer;
}

void gsScript::setglobal(char *n, int i, Lua *l)
{
//  if (!l) l = &lua;
  l->pushnumber((float)i);
  l->pushstring(n);
  l->insert(-2);
  l->settable(LUA_GLOBALSINDEX);
//  l->setglobal(n);
}

void gsScript::setglobal(char *n, float f, Lua *l)
{
//  if (!l) l = &lua;
  l->pushnumber(f);
  l->pushstring(n);
  l->insert(-2);
  l->settable(LUA_GLOBALSINDEX);
//  l->setglobal(n);
}

void gsScript::setglobal(char *n, char *s, Lua *l)
{
//  if (!l) l = &lua;
  l->pushstring(s);
  l->pushstring(n);
  l->insert(-2);
  l->settable(LUA_GLOBALSINDEX);
//  l->setglobal(n);
}

int gsScript::getglobal_int(char *n, Lua *l)
{
  int i;
//  if (!l) l = &lua;

  l->pushstring(n);
  l->gettable(LUA_GLOBALSINDEX);

  if (l->isnumber(-1))
  {
    i = (int)l->tonumber(-1);
    l->pop(1);
    return i;
  }
  else
  {
//    errors->log(1, "script warning", "no value at...", n);
    l->pop(1);
    return 0;
  }
}

float gsScript::getglobal_float(char *n, Lua *l)
{
  float i;
//  if (!l) l = &lua;

  l->pushstring(n);
  l->gettable(LUA_GLOBALSINDEX);

  if (l->isnumber(-1))
  {
    i = (float)l->tonumber(-1);
    l->pop(1);
    return i;
  }
  else
  {
//    errors->log(1, "script warning", "no value at...", n);
    l->pop(1);
    return 0;
  }
}

const char *gsScript::getglobal_string(char *n, Lua *l)
{
  const char *i = NULL;
//  if (!l) l = &lua;

  l->pushstring(n);
  l->gettable(LUA_GLOBALSINDEX);

  if (l->isstring(-1)) i = l->tostring(-1);
  l->pop(1);

  if (i != NULL)
    return i;
  else
    return "";
}

void gsScript::set_standard_globals(Lua *l)
{
  setglobal("LANGUAGE", globals->language, l);
  setglobal("LANG_DE", LANG_DE, l);
  setglobal("LANG_EN", LANG_EN, l);
  setglobal("LANG_CR", LANG_CR, l);
  setglobal("LANG_IT", LANG_IT, l);
  setglobal("LANG_DK", LANG_DK, l);

  setglobal("GRASS", MAP_GRASS, l);
  setglobal("STONE", MAP_STONE, l);
  setglobal("VERSION", float(atof(VERSION)+0.001), l);
}

void gsScript::load_level_init(char *lvlpath, level_info *lvl)
{
  Lua *level_lua = new Lua();
  
  // clear globals
  level_lua->newtable();
  level_lua->replace(LUA_GLOBALSINDEX);

  set_standard_globals(level_lua);

  char path[255];
  strcpy(path, lvlpath);
  int err = level_lua->dofile(path);

  strncpy(lvl->levelfile, get_filename(lvlpath), 20);

//  char tempp[200];
//  sprintf(tempp, "%s/%s", lvl->leveldir, lvl->levelfile);
//  errors->log(2, "loading file", tempp);

  if (err)
  {
    errors->log(0, "ERROR", "while loading level file", lvlpath);
    strcpy(lvl->mapfile, "");
    strcpy(lvl->scriptfile, "");
    strcpy(lvl->initscriptfile, "");
    strcpy(lvl->name, "");
    strcpy(lvl->desc, "");
    strcpy(lvl->missiondesc, "");
    return;
  }

  strncpy(lvl->mapfile, getglobal_string("map", level_lua), 20);

  if (strlen(lvl->mapfile) < 2)
  {
    errors->log(0, "ERROR loading map", "mapname too short!");
    strcpy(lvl->mapfile, "");
  }

  strncpy(lvl->scriptfile, getglobal_string("script", level_lua), 20);

  if (strlen(lvl->scriptfile) > 0 && strlen(lvl->scriptfile) < 3)
  {
    errors->log(1, "loading script", "scriptname too short!", lvl->scriptfile);
    strcpy(lvl->scriptfile, "");
  }

  strncpy(lvl->initscriptfile, getglobal_string("initscript", level_lua), 20);

  if (strlen(lvl->initscriptfile) > 0 && strlen(lvl->initscriptfile) < 3)
  {
    errors->log(1, "loading initscript", "scriptname too short!", lvl->initscriptfile);
    strcpy(lvl->initscriptfile, "");
  }

  // names
  strncpy(lvl->name, getglobal_string("name", level_lua), 50);
  if (strlen(lvl->name) < 2) strcpy(lvl->name, lvl->levelfile);
  strncpy(lvl->desc, getglobal_string("description", level_lua), 200);
  strncpy(lvl->missiondesc, getglobal_string("missiondesc", level_lua), 1000);

  // onescreen map?
  int i;
  i = getglobal_int("onescreen_map", level_lua);
  if (i == 1)
  {
    lvl->onescreen_map = 1;

    i = getglobal_int("onescreen_map_x", level_lua);
    if (i >= 0) lvl->map_start_x = i;
    else lvl->map_start_x = 0;

    i = getglobal_int("onescreen_map_y", level_lua);
    if (i >= 0) lvl->map_start_y = i;
    else lvl->map_start_y = 0;
  }
  else
  {
    lvl->onescreen_map = 0;
  }

  strncpy(lvl->author, getglobal_string("author", level_lua), 49);
  strncpy(lvl->depends_on, getglobal_string("depends_on", level_lua), 99);
  strncpy(lvl->depends_on_txt, getglobal_string("depends_on_txt", level_lua), 249);

  strncpy(lvl->activate_extra_str, getglobal_string("activate_extra_str", level_lua), 34);
  strncpy(lvl->activate_extra_bar_str, getglobal_string("activate_extra_bar", level_lua), 34);

  lvl->activate_singleplayer = getglobal_int("singleplayer", level_lua) > 0 ? 1 : 0;
  lvl->activate_minigame = getglobal_int("minigame", level_lua) > 0 ? 1 : 0;
  lvl->activate_ctf = getglobal_int("capture_the_flag", level_lua) > 0 ? 1 : 0;
  lvl->activate_hh = getglobal_int("hunter_hunted", level_lua) > 0 ? 1 : 0;
  lvl->activate_race = getglobal_int("race", level_lua) > 0 ? 1 : 0;
  lvl->activate_global_script = getglobal_int("global_script", level_lua) > 0 ? 1 : 0;
  lvl->max_players = getglobal_int("max_players", level_lua);
  if (lvl->max_players < 1) lvl->max_players = 1;
  else if (lvl->max_players > 6) lvl->max_players = 6;
  lvl->min_version = getglobal_float("min_version", level_lua);

  // resolution
  lvl->require_resolution_nr = -1;
  int myres = getglobal_int("required_resolution", level_lua);
  for (int mn=0; mn <= 4; mn++)
    if (globals->game_resolution[mn].x == myres)
    {
      lvl->require_resolution_nr = mn;
      break;
    }
  

  // style
  int tmpstyle = MAP_GRASS;
  level_lua->pushstring("style");
  level_lua->gettable(LUA_GLOBALSINDEX);
  
  if (level_lua->isnumber(-1))
  {
    tmpstyle = (int)level_lua->tonumber(-1);
    if (tmpstyle == MAP_STONE) strcpy(lvl->map_style, "gfw2.fmp");
    else strcpy(lvl->map_style, "gfw1.fmp");
  }
  else if (level_lua->isstring(-1))
  {
    strncpy(lvl->map_style, level_lua->tostring(-1), 30);
  }
  else
  {
    strcpy(lvl->map_style, "gfw1.fmp");
  }

  level_lua->pop(1);

  delete level_lua;    
}

void gsScript::do_string(char *cmd)
{
  glob_lua->dostring(cmd);
}

void gsScript::do_init_script(Lua *l, char *dir, char *name)
{
  char path[255];

  // set global var with level directory
  if (dir)
  {
    sprintf(path, "%s%s", globals->level_dir, dir);
    setglobal("LEVEL_DIR", path, l);
  }

  if (dir)
    sprintf(path, "%s%s/%s", globals->level_dir, dir, name);
  else
    sprintf(path, "%s/%s", globals->level_dir, name);


  int err = l->dofile(path);

/*
  int buffer_size = file_size(path);
  char *script_buffer = new char[buffer_size];
  int err = 0;
  memset(script_buffer, 0, buffer_size);

  FILE *sc_f = NULL;
  sc_f = fopen(path, "rb");

  if (sc_f)
  {
    fread(script_buffer, 1, buffer_size, sc_f);
    fclose(sc_f);
    err = l->dobuffer(script_buffer, buffer_size, "initscript");
  }
  else
  {
    errors->log(0, "ERROR loading script", path);
    err = -1;
  }
*/
  if (err)
  {
    errors->log(0, "ERROR executing script", path, err);
  }
}

void gsScript::init_text(Lua *l)
{
  set_standard_globals(l);

  int a = T_MENU_YES;
  setglobal("MENU_YES",               a, l);
  setglobal("MENU_NO",              ++a, l);
  setglobal("MENU_OK",              ++a, l);
  setglobal("MENU_SINGLEPLAYER",    ++a, l);
  setglobal("MENU_SPLITSCREEN",     ++a, l);
  setglobal("MENU_MINIGAME",        ++a, l);
  setglobal("MENU_NETGAME",         ++a, l);
  setglobal("MENU_OPTIONS",         ++a, l);
  setglobal("MENU_CREDITS",         ++a, l);
  setglobal("MENU_SHIPBUILDER",     ++a, l);
  setglobal("MENU_BACK",            ++a, l);
  setglobal("MENU_LANGUAGE",        ++a, l);
  setglobal("MENU_LANG_DE",         ++a, l);
  setglobal("MENU_LANG_EN",         ++a, l);
  setglobal("MENU_LANG_CR",         ++a, l);
  setglobal("MENU_LANG_IT",         ++a, l);
  setglobal("MENU_LANG_DK",         ++a, l);
  setglobal("MENU_PIXAMOUNT",       ++a, l);
  setglobal("MENU_RESOLUTION",      ++a, l);
  setglobal("MENU_VOLUME",          ++a, l);
  setglobal("MENU_MVOLUME",         ++a, l);
  setglobal("MENU_CONTROLS",        ++a, l);
  setglobal("MENU_2PCONTROLS",      ++a, l);
  setglobal("MENU_SPCONTROLS",      ++a, l);
  setglobal("MENU_QUIT",            ++a, l);

  setglobal("MENU_CAP_OPTIONS",     ++a, l);
  setglobal("MENU_CAP_CONTROLS",    ++a, l);
  setglobal("MENU_CAP_2PCONTROLS",  ++a, l);
  setglobal("MENU_CAP_SPCONTROLS",  ++a, l);
  setglobal("MENU_CAP_SINGLEPLAYER", ++a, l);
  setglobal("MENU_CAP_SPLITSCREEN", ++a, l);
  setglobal("MENU_CAP_MINIGAME",    ++a, l);
  setglobal("MENU_CAP_NETGAME",     ++a, l);
  setglobal("MENU_CAP_NETGAME_SERVER", ++a, l);
  setglobal("MENU_CAP_NETGAME_CLIENT", ++a, l);
  setglobal("MENU_CAP_SHIPBUILDER", ++a, l);
  setglobal("MENU_CAP_CHOOSESHIP",  ++a, l);

  setglobal("MENU_DISP_RACES",       ++a, l);
  setglobal("MENU_DISP_MISSIONS",    ++a, l);
  setglobal("MENU_DISP_DOGFIGHTS",   ++a, l);
  setglobal("MENU_CAP_AUTHOR",       ++a, l);

  setglobal("MENU_NEWSHIP",         ++a, l);
  setglobal("MENU_EDITWEAPONS",     ++a, l);
  setglobal("MENU_EDITSHIP",        ++a, l);
  setglobal("MENU_MSG_SHIPLOADERROR", ++a, l);
  setglobal("MENU_MSG_REALLYDELETE", ++a, l);
  setglobal("MENU_SAVE",            ++a, l);
  setglobal("MENU_SAVENEW",         ++a, l);
  setglobal("MENU_DELETE",          ++a, l);
  setglobal("MENU_SHIP_NAME",       ++a, l);
  setglobal("MENU_SHIP_DESC",       ++a, l);
  setglobal("MENU_SHIP_LOADIMAGE",  ++a, l);
  setglobal("MENU_SHIP_LOADIMAGE_FS", ++a, l);
  setglobal("MENU_SHIP_REMPOINTS",  ++a, l);
  setglobal("MENU_SHIP_HULL",       ++a, l);
  setglobal("MENU_SHIP_SHIELD",     ++a, l);
  setglobal("MENU_SHIP_HULL_RE",    ++a, l);
  setglobal("MENU_SHIP_SHIELD_RE",  ++a, l);
  setglobal("MENU_SHIP_WEAPON_RE",  ++a, l);
  setglobal("MENU_SHIP_WALLSHIELD", ++a, l);
  setglobal("MENU_SHIP_RADAR_ZOOM", ++a, l);
  setglobal("MENU_SHIP_WEAPON_SLOTS", ++a, l);
  setglobal("MENU_SHIP_TURN_SPEED", ++a, l);
  setglobal("MENU_SHIP_MAX_SPEED",  ++a, l);
  setglobal("MENU_SHIP_ACCELERATION", ++a, l);
  setglobal("MENU_SHIP_FUEL_CONSUMPTION", ++a, l);
  setglobal("MENU_SHIP_WEAPON_STORAGE", ++a, l);
  setglobal("MENU_SHIP_EXTRA",      ++a, l);
  setglobal("MENU_SHIP_AFTERBURNER",++a, l);
  setglobal("MENU_SHIP_DEFLECTOR",  ++a, l);
  setglobal("MENU_SHIP_CLOAK", ++a, l);
  setglobal("MENU_SHIP_WEAPONS",    ++a, l);
  setglobal("MENU_SHIP_TEXT_WEAPAVAIL", ++a, l);
  setglobal("MENU_SHIP_TEXT_WEAPSLOTS", ++a, l);
  setglobal("MENU_SHIP_TEXT_WEAPDESC", ++a, l);
  setglobal("MENU_SHIP_W_SBLASTER", ++a, l);
  setglobal("MENU_SHIP_W_DBLASTER", ++a, l);
  setglobal("MENU_SHIP_W_BOMB",     ++a, l);
  setglobal("MENU_SHIP_W_BIGBOMB",  ++a, l);
  setglobal("MENU_SHIP_W_ROCKETS",  ++a, l);
  setglobal("MENU_SHIP_W_MINES",    ++a, l);
  setglobal("MENU_SHIP_W_FREEZER",  ++a, l);
  setglobal("MENU_SHIP_W_SHRINKER", ++a, l);
  setglobal("MENU_SHIP_W_PROBE",    ++a, l);
  setglobal("MENU_SHIP_W_GAS",      ++a, l);
  setglobal("MENU_SHIP_W_LASER",    ++a, l);
  setglobal("MENU_SHIP_W_BUMPER",   ++a, l);
  setglobal("MENU_SHIP_W_SHOTGUN",  ++a, l);
  setglobal("MENU_SHIP_W_ACID",     ++a, l);
  setglobal("MENU_SHIP_WI_SBLASTER", ++a, l);
  setglobal("MENU_SHIP_WI_DBLASTER", ++a, l);
  setglobal("MENU_SHIP_WI_BOMB",     ++a, l);
  setglobal("MENU_SHIP_WI_BIGBOMB",  ++a, l);
  setglobal("MENU_SHIP_WI_ROCKETS",  ++a, l);
  setglobal("MENU_SHIP_WI_MINES",    ++a, l);
  setglobal("MENU_SHIP_WI_FREEZER",  ++a, l);
  setglobal("MENU_SHIP_WI_SHRINKER", ++a, l);
  setglobal("MENU_SHIP_WI_PROBE",    ++a, l);
  setglobal("MENU_SHIP_WI_GAS",      ++a, l);
  setglobal("MENU_SHIP_WI_LASER",    ++a, l);
  setglobal("MENU_SHIP_WI_BUMPER",   ++a, l);
  setglobal("MENU_SHIP_WI_SHOTGUN",  ++a, l);
  setglobal("MENU_SHIP_WI_ACID",     ++a, l);
  setglobal("MENU_SHIP_SLOT1",      ++a, l);
  setglobal("MENU_SHIP_SLOT2",      ++a, l);
  setglobal("MENU_SHIP_SLOT3",      ++a, l);
  setglobal("MENU_SHIP_SLOT4",      ++a, l);
  setglobal("MENU_SHIP_SLOT5",      ++a, l);
  setglobal("MENU_SHIP_SLOT6",      ++a, l);
  setglobal("MENU_SHIP_DAMAGE_SP",  ++a, l);
  setglobal("MENU_SHIP_DAMAGE_MP",  ++a, l);
  setglobal("MENU_PLAY",            ++a, l);
  setglobal("MENU_PLAY_MINIGAME",   ++a, l);
  setglobal("MENU_NAME_P1",         ++a, l);
  setglobal("MENU_NAME_P2",         ++a, l);
  setglobal("MENU_2P_CHOOSESHIP",   ++a, l);
  setglobal("MENU_2P_EDITSHIP",     ++a, l);
  setglobal("MENU_GRAVITY",         ++a, l);
  setglobal("MENU_MAXFRAGS",        ++a, l);
  setglobal("MENU_MAXTIME",         ++a, l);
  setglobal("MENU_EXTRAS",          ++a, l);
  setglobal("MENU_ONESCREEN_MAP",   ++a, l);
  setglobal("MENU_SCRIPTING",       ++a, l);
  setglobal("MENU_LEVELEXTRA",      ++a, l);
  setglobal("MENU_CAPTURE_THE_FLAG",++a, l);
  setglobal("MENU_HUNTER_HUNTED",   ++a, l);
  setglobal("MENU_LAPS",            ++a, l);
  setglobal("MENU_COMPUTER_PLAYER", ++a, l);
  setglobal("MENU_COMPUTER_PLAYER_STRENGTH", ++a, l);
  setglobal("MENU_NEXTPAGE",        ++a, l);
  setglobal("MENU_LASTPAGE",        ++a, l);
  setglobal("MENU_2NDPLAYER",       ++a, l);
  setglobal("MENU_LEVELINFO",       ++a, l);
  setglobal("MENU_LEVEL_LOCKED",    ++a, l);
  setglobal("MENU_HANDICAP",        ++a, l);
  
  setglobal("MENU_SERVER",          ++a, l);
  setglobal("MENU_CLIENT",          ++a, l);
  setglobal("MENU_I_AM_READY",      ++a, l);
  setglobal("MENU_READY",           ++a, l);
  setglobal("MENU_LEVEL_NOT_AVAILABLE", ++a, l);

  setglobal("GAME_EXTRA",           ++a, l);
  setglobal("GAME_AMMOPACK",        ++a, l);
  setglobal("GAME_TIME",            ++a, l);
  setglobal("GAME_TIMEREM",         ++a, l);
  setglobal("GAME_CARGO",           ++a, l);
  setglobal("GAME_MAN",             ++a, l);
  setglobal("GAME_SOMETHING",       ++a, l);
  setglobal("GAME_WEIGHT",          ++a, l);
  setglobal("GAME_CARGOREC",        ++a, l);
  setglobal("GAME_HOMEBASE",        ++a, l);
  setglobal("GAME_PANEL_FUEL",      ++a, l);
  setglobal("GAME_PANEL_EXTRA",     ++a, l);
  setglobal("GAME_PANEL_FRAGS",     ++a, l);
  setglobal("GAME_PANEL_SHIPS",     ++a, l);
  setglobal("GAME_MISSION_DONE",    ++a, l);
  setglobal("GAME_MISSION_FAILED",  ++a, l);
  setglobal("GAME_YOU_ARE_DEAD",    ++a, l);
  setglobal("GAME_EXTRA_UPGRADE",   ++a, l);
  setglobal("GAME_EXTRA_FUEL",      ++a, l);
  setglobal("GAME_EXTRA_FULLSHIELD", ++a, l);
  setglobal("GAME_EXTRA_FULLWSHIELD", ++a, l);
  setglobal("GAME_EXTRA_INVULNERABILITY", ++a, l);
  setglobal("GAME_DAMAGE",           ++a, l);
  setglobal("GAME_DAMAGE_THRUSTER",  ++a, l);
  setglobal("GAME_DAMAGE_STEERING",  ++a, l);
  setglobal("GAME_DAMAGE_FUEL",      ++a, l);
  setglobal("GAME_DAMAGE_WEAPONS",   ++a, l);
  setglobal("GAME_DAMAGE_SHIELD",    ++a, l);
  setglobal("GAME_DAMAGE_LANDING",   ++a, l);
  setglobal("GAME_DAMAGE_RADAR",     ++a, l);
  setglobal("GAME_ESC_EXIT",         ++a, l);
  setglobal("GAME_ESC_RESTART",      ++a, l);
  setglobal("GAME_ESC_OTHER_KEY",    ++a, l);
  setglobal("GAME_ESC_CONTINUE",     ++a, l);

  setglobal("STATS_CAP_1PLAYER",    ++a, l);
  setglobal("STATS_CAP_2PLAYER",    ++a, l);
  setglobal("STATS_CAP_NETWORK",    ++a, l);
  setglobal("STATS_MODE",           ++a, l);
  setglobal("STATS_MODE_DOGFIGHT",  ++a, l);
  setglobal("STATS_MODE_RACE",      ++a, l);
  setglobal("STATS_PLAYER",         ++a, l);
  setglobal("STATS_HITPERCENT",     ++a, l);
  setglobal("STATS_FRAGS",          ++a, l);
  setglobal("STATS_SUICIDES",       ++a, l);
  setglobal("STATS_EXTRASTAKEN",    ++a, l);
  setglobal("STATS_WINNER",         ++a, l);
  setglobal("STATS_DRAW",           ++a, l);
  setglobal("STATS_PLACE",          ++a, l);
  setglobal("STATS_SCORE",          ++a, l);
  setglobal("STATS_LAP",            ++a, l);
  setglobal("STATS_TIME",           ++a, l);
  setglobal("STATS_OVERALL_TIME",   ++a, l);
  setglobal("STATS_TIMEPLAYED",     ++a, l);
  setglobal("STATS_BONUS",          ++a, l);
  setglobal("STATS_ENEMY_BONUS",    ++a, l);
  setglobal("STATS_LOSTLIFES",      ++a, l);
  setglobal("STATS_MISSION_STATUS", ++a, l);
  setglobal("STATS_COMPLETED",      ++a, l);
  setglobal("STATS_FAILED",         ++a, l);
  setglobal("STATS_HISCORE_NEW",    ++a, l);
  setglobal("STATS_HISCORE_OLD",    ++a, l);
  setglobal("STATS_HISCORE_DISABLED", ++a, l);

  setglobal("NET_JOIN_LOBBY",       ++a, l);
  setglobal("NET_LEAVE_LOBBY",      ++a, l);
  
  setglobal("NET_FRAGMSG_SINGLE_BLASTER_1", ++a, l);
  setglobal("NET_FRAGMSG_SINGLE_BLASTER_2", ++a, l);
  setglobal("NET_FRAGMSG_SINGLE_BLASTER_3", ++a, l);
  setglobal("NET_FRAGMSG_SINGLE_BLASTER_4", ++a, l);
  setglobal("NET_FRAGMSG_SINGLE_BLASTER_5", ++a, l);
  
  setglobal("NET_FRAGMSG_DOUBLE_BLASTER_1", ++a, l);
  setglobal("NET_FRAGMSG_DOUBLE_BLASTER_2", ++a, l);
  setglobal("NET_FRAGMSG_DOUBLE_BLASTER_3", ++a, l);
  setglobal("NET_FRAGMSG_DOUBLE_BLASTER_4", ++a, l);
  setglobal("NET_FRAGMSG_DOUBLE_BLASTER_5", ++a, l);

  setglobal("NET_FRAGMSG_BOMB_1", ++a, l);
  setglobal("NET_FRAGMSG_BOMB_2", ++a, l);
  setglobal("NET_FRAGMSG_BOMB_3", ++a, l);
  setglobal("NET_FRAGMSG_BOMB_4", ++a, l);
  setglobal("NET_FRAGMSG_BOMB_5", ++a, l);

  setglobal("NET_FRAGMSG_BIGBOMB_1", ++a, l);
  setglobal("NET_FRAGMSG_BIGBOMB_2", ++a, l);
  setglobal("NET_FRAGMSG_BIGBOMB_3", ++a, l);
  setglobal("NET_FRAGMSG_BIGBOMB_4", ++a, l);
  setglobal("NET_FRAGMSG_BIGBOMB_5", ++a, l);

  setglobal("NET_FRAGMSG_ROCKET_1", ++a, l);
  setglobal("NET_FRAGMSG_ROCKET_2", ++a, l);
  setglobal("NET_FRAGMSG_ROCKET_3", ++a, l);
  setglobal("NET_FRAGMSG_ROCKET_4", ++a, l);
  setglobal("NET_FRAGMSG_ROCKET_5", ++a, l);

  setglobal("NET_FRAGMSG_MINE_1", ++a, l);
  setglobal("NET_FRAGMSG_MINE_2", ++a, l);
  setglobal("NET_FRAGMSG_MINE_3", ++a, l);
  setglobal("NET_FRAGMSG_MINE_4", ++a, l);
  setglobal("NET_FRAGMSG_MINE_5", ++a, l);
  
  setglobal("NET_FRAGMSG_SHRINKER_1", ++a, l);
  setglobal("NET_FRAGMSG_SHRINKER_2", ++a, l);
  setglobal("NET_FRAGMSG_SHRINKER_3", ++a, l);
  setglobal("NET_FRAGMSG_SHRINKER_4", ++a, l);
  setglobal("NET_FRAGMSG_SHRINKER_5", ++a, l);

  setglobal("NET_FRAGMSG_PROBE_1", ++a, l);
  setglobal("NET_FRAGMSG_PROBE_2", ++a, l);
  setglobal("NET_FRAGMSG_PROBE_3", ++a, l);
  setglobal("NET_FRAGMSG_PROBE_4", ++a, l);
  setglobal("NET_FRAGMSG_PROBE_5", ++a, l);

  setglobal("NET_FRAGMSG_GAS_1", ++a, l);
  setglobal("NET_FRAGMSG_GAS_2", ++a, l);
  setglobal("NET_FRAGMSG_GAS_3", ++a, l);
  setglobal("NET_FRAGMSG_GAS_4", ++a, l);
  setglobal("NET_FRAGMSG_GAS_5", ++a, l);

  setglobal("NET_FRAGMSG_BUMPER_1", ++a, l);
  setglobal("NET_FRAGMSG_BUMPER_2", ++a, l);
  setglobal("NET_FRAGMSG_BUMPER_3", ++a, l);
  setglobal("NET_FRAGMSG_BUMPER_4", ++a, l);
  setglobal("NET_FRAGMSG_BUMPER_5", ++a, l);

  setglobal("NET_FRAGMSG_SHOTGUN_1", ++a, l);
  setglobal("NET_FRAGMSG_SHOTGUN_2", ++a, l);
  setglobal("NET_FRAGMSG_SHOTGUN_3", ++a, l);
  setglobal("NET_FRAGMSG_SHOTGUN_4", ++a, l);
  setglobal("NET_FRAGMSG_SHOTGUN_5", ++a, l);

  setglobal("NET_FRAGMSG_ACID_1", ++a, l);
  setglobal("NET_FRAGMSG_ACID_2", ++a, l);
  setglobal("NET_FRAGMSG_ACID_3", ++a, l);
  setglobal("NET_FRAGMSG_ACID_4", ++a, l);
  setglobal("NET_FRAGMSG_ACID_5", ++a, l);

  setglobal("NET_FRAGMSG_SUICIDE_1", ++a, l);
  setglobal("NET_FRAGMSG_SUICIDE_2", ++a, l);
  setglobal("NET_FRAGMSG_SUICIDE_3", ++a, l);
  setglobal("NET_FRAGMSG_SUICIDE_4", ++a, l);
  setglobal("NET_FRAGMSG_SUICIDE_5", ++a, l);

  setglobal("NET_FRAGMSG_PLSUICIDE_1", ++a, l);
  setglobal("NET_FRAGMSG_PLSUICIDE_2", ++a, l);
  setglobal("NET_FRAGMSG_PLSUICIDE_3", ++a, l);
  setglobal("NET_FRAGMSG_PLSUICIDE_4", ++a, l);
  setglobal("NET_FRAGMSG_PLSUICIDE_5", ++a, l);

  setglobal("NET_FRAGMSG_JOIN_1", ++a, l);
  setglobal("NET_FRAGMSG_JOIN_2", ++a, l);
  setglobal("NET_FRAGMSG_JOIN_3", ++a, l);
  setglobal("NET_FRAGMSG_JOIN_4", ++a, l);
  setglobal("NET_FRAGMSG_JOIN_5", ++a, l);

  setglobal("NET_FRAGMSG_QUIT_1", ++a, l);
  setglobal("NET_FRAGMSG_QUIT_2", ++a, l);
  setglobal("NET_FRAGMSG_QUIT_3", ++a, l);
  setglobal("NET_FRAGMSG_QUIT_4", ++a, l);
  setglobal("NET_FRAGMSG_QUIT_5", ++a, l);

  setglobal("MENU_TEXT_PLAYER1",    ++a, l);
  setglobal("MENU_TEXT_PLAYER2",    ++a, l);

  a = T_CONTROL_LEFT;
  setglobal("CONTROL_LEFT",           a, l);
  setglobal("CONTROL_RIGHT",        ++a, l);
  setglobal("CONTROL_THRUST",       ++a, l);
  setglobal("CONTROL_SHOOT",        ++a, l);
  setglobal("CONTROL_NEXTWEAP",     ++a, l);
  setglobal("CONTROL_PREVWEAP",     ++a, l);
  setglobal("CONTROL_W1",           ++a, l);
  setglobal("CONTROL_W2",           ++a, l);
  setglobal("CONTROL_W3",           ++a, l);
  setglobal("CONTROL_W4",           ++a, l);
  setglobal("CONTROL_W5",           ++a, l);
  setglobal("CONTROL_W6",           ++a, l);
  setglobal("CONTROL_EXTRA_ACTION", ++a, l);
  setglobal("CONTROL_EXTRA2",       ++a, l);
  setglobal("CONTROL_PAUSE",        ++a, l);
  setglobal("CONTROL_SCREENSHOT",   ++a, l);
  setglobal("CONTROL_PANEL",        ++a, l);
  setglobal("CONTROL_DEBUG",        ++a, l);
  setglobal("CONTROL_CHAT",         ++a, l);

  a = T_KEY_0_PAD;
  setglobal("KEY_0_PAD",              a, l);
  setglobal("KEY_1_PAD",            ++a, l);
  setglobal("KEY_2_PAD",            ++a, l);
  setglobal("KEY_3_PAD",            ++a, l);
  setglobal("KEY_4_PAD",            ++a, l);
  setglobal("KEY_5_PAD",            ++a, l);
  setglobal("KEY_6_PAD",            ++a, l);
  setglobal("KEY_7_PAD",            ++a, l);
  setglobal("KEY_8_PAD",            ++a, l);
  setglobal("KEY_9_PAD",            ++a, l);
  setglobal("KEY_F1",               ++a, l);
  setglobal("KEY_F2",               ++a, l);
  setglobal("KEY_F3",               ++a, l);
  setglobal("KEY_F4",               ++a, l);
  setglobal("KEY_F5",               ++a, l);
  setglobal("KEY_F6",               ++a, l);
  setglobal("KEY_F7",               ++a, l);
  setglobal("KEY_F8",               ++a, l);
  setglobal("KEY_F9",               ++a, l);
  setglobal("KEY_F10",              ++a, l);
  setglobal("KEY_F11",              ++a, l);
  setglobal("KEY_F12",              ++a, l);
  setglobal("KEY_ESC",              ++a, l);
  setglobal("KEY_TILDE",            ++a, l);
  setglobal("KEY_MINUS",            ++a, l);
  setglobal("KEY_EQUALS",           ++a, l);
  setglobal("KEY_BACKSPACE",        ++a, l);
  setglobal("KEY_TAB",              ++a, l);
  setglobal("KEY_OPENBRACE",        ++a, l);
  setglobal("KEY_CLOSEBRACE",       ++a, l);
  setglobal("KEY_ENTER",            ++a, l);
  setglobal("KEY_COLON",            ++a, l);
  setglobal("KEY_QUOTE",            ++a, l);
  setglobal("KEY_BACKSLASH",        ++a, l);
  setglobal("KEY_BACKSLASH2",       ++a, l);
  setglobal("KEY_COMMA",            ++a, l);
  setglobal("KEY_STOP",             ++a, l);
  setglobal("KEY_SLASH",            ++a, l);
  setglobal("KEY_SPACE",            ++a, l);
  setglobal("KEY_INSERT",           ++a, l);
  setglobal("KEY_DEL",              ++a, l);
  setglobal("KEY_HOME",             ++a, l);
  setglobal("KEY_END",              ++a, l);
  setglobal("KEY_PGUP",             ++a, l);
  setglobal("KEY_PGDN",             ++a, l);
  setglobal("KEY_LEFT",             ++a, l);
  setglobal("KEY_RIGHT",            ++a, l);
  setglobal("KEY_UP",               ++a, l);
  setglobal("KEY_DOWN",             ++a, l);
  setglobal("KEY_SLASH_PAD",        ++a, l);
  setglobal("KEY_ASTERISK",         ++a, l);
  setglobal("KEY_MINUS_PAD",        ++a, l);
  setglobal("KEY_PLUS_PAD",         ++a, l);
  setglobal("KEY_DEL_PAD",          ++a, l);
  setglobal("KEY_ENTER_PAD",        ++a, l);
  setglobal("KEY_PRTSCR",           ++a, l);
  setglobal("KEY_PAUSE",            ++a, l);

  a = T_KEY_LSHIFT;
  setglobal("KEY_LSHIFT",             a, l);
  setglobal("KEY_RSHIFT",           ++a, l);
  setglobal("KEY_LCONTROL",         ++a, l);
  setglobal("KEY_RCONTROL",         ++a, l);
  setglobal("KEY_ALT",              ++a, l);
  setglobal("KEY_ALTGR",            ++a, l);
  setglobal("KEY_LWIN",             ++a, l);
  setglobal("KEY_RWIN",             ++a, l);
  setglobal("KEY_MENU",             ++a, l);
  setglobal("KEY_SCRLOCK",          ++a, l);
  setglobal("KEY_NUMLOCK",          ++a, l);
  setglobal("KEY_CAPSLOCK",         ++a, l);

  setglobal("JOY0_UP",              ++a, l);
  setglobal("JOY0_DOWN",            ++a, l);
  setglobal("JOY0_LEFT",            ++a, l);
  setglobal("JOY0_RIGHT",           ++a, l);
  setglobal("JOY0_B1",              ++a, l);
  setglobal("JOY0_B2",              ++a, l);
  setglobal("JOY0_B3",              ++a, l);
  setglobal("JOY0_B4",              ++a, l);
  setglobal("JOY0_B5",              ++a, l);
  setglobal("JOY0_B6",              ++a, l);
  setglobal("JOY1_UP",              ++a, l);
  setglobal("JOY1_DOWN",            ++a, l);
  setglobal("JOY1_LEFT",            ++a, l);
  setglobal("JOY1_RIGHT",           ++a, l);
  setglobal("JOY1_B1",              ++a, l);
  setglobal("JOY1_B2",              ++a, l);
  setglobal("JOY1_B3",              ++a, l);
  setglobal("JOY1_B4",              ++a, l);
  setglobal("JOY1_B5",              ++a, l);
  setglobal("JOY1_B6",              ++a, l);

  l->registerfunction("set_string", script_set_string);
}

void gsScript::load_text()
{
  char file[255];
  Lua *text_lua = new Lua();

  //lua.settop(0);
  // clear globals
//  lua.newtable();
//  lua.replace(LUA_GLOBALSINDEX);

  init_text(text_lua);

  strcpy(file, globals->data_dir);
  strcat(file, globals->language_file_name);

  int error = text_lua->dofile(file);
  
  if (error)
  {
    errors->log(0, "script", "load text: file not found or invalid", file);
  }
  else
  {
    errors->log(2, "script", "load text: successfully loaded string tables", globals->language);
  }
  
  delete text_lua;
}

void error_wrong_number(char *t, int n)
{
  errors->log(0, "script", "load player ship: wrong argument!", n);
}

void gsScript::set_player_weapon(gsPlayer *p, int slot, int weap)
{
  if (weap == W_NONE)
  {
    p->set_weapon(slot, (WEAPON_TYPE)weap);
  }
  else if (weap >= W_SINGLE && weap <= W_NONE-2)
  {
    // check for double weapon
    int doubleweap = 0;
    weaponslot *wslot;
    for (int n=0; n < 6; n++)
    {
      if (n != slot)
      {
        wslot = p->get_weapon_slot(n);
        if (wslot->type == weap) { doubleweap = 1; break; }
      }
    } // for

    if (!doubleweap) p->set_weapon(slot, (WEAPON_TYPE)weap);

  } // if not W_NONE
  else // completely different value -> set noweap
  {
    p->set_weapon(slot, W_NONE);
  }

}

int gsScript::load_ship(gsPlayer *p, char *file)
{
  char shipimage[255];
  int last_type = p->get_ship_type();
  Lua *ship_lua = new Lua();

  ship_lua->settop(0);
  ship_lua->newtable();
  ship_lua->replace(LUA_GLOBALSINDEX);

  // push player globals
  setglobal("PIRANHA", SHIP_PIRANHA, ship_lua);
  setglobal("RAZOR",   SHIP_RAZOR, ship_lua);
  setglobal("BOMBER",  SHIP_BOMBER, ship_lua);
  setglobal("CUSTOM",  SHIP_CUSTOM, ship_lua);

  // weapons
  setglobal("W_NONE",      W_NONE, ship_lua);
  setglobal("W_SINGLE",    W_SINGLE, ship_lua);
  setglobal("W_DOUBLE",    W_DOUBLE, ship_lua);
  setglobal("W_BOMB",      W_BOMB, ship_lua);
  setglobal("W_BIGBOMB",   W_BIGBOMB, ship_lua);
  setglobal("W_ROCKET",    W_ROCKET, ship_lua);
  setglobal("W_MINE",      W_MINE, ship_lua);
  setglobal("W_FREEZER",   W_FREEZER, ship_lua);
  setglobal("W_SHRINKER",  W_SHRINKER, ship_lua);
  setglobal("W_PROBE",     W_PROBE, ship_lua);
  setglobal("W_GAS",       W_GAS, ship_lua);
  setglobal("W_LASER",     W_LASER, ship_lua);
  setglobal("W_BUMPER",    W_BUMPER, ship_lua);
  setglobal("W_SHOTGUN",   W_SHOTGUN, ship_lua);
  setglobal("W_ACID",      W_ACID, ship_lua);

  // extras
  setglobal("E_NONE",         EXTRA_NONE, ship_lua);
  setglobal("E_AFTERBURNER",  EXTRA_AFTERBURNER, ship_lua);
  setglobal("E_DEFLECTOR",    EXTRA_DEFLECTOR, ship_lua);
  setglobal("E_CLOAK",        EXTRA_CLOAK, ship_lua);

  // language stuff
  setglobal("LANGUAGE", globals->language, ship_lua);
  setglobal("LANG_DE", LANG_DE, ship_lua);
  setglobal("LANG_EN", LANG_EN, ship_lua);
  setglobal("LANG_CR", LANG_CR, ship_lua);
  setglobal("LANG_IT", LANG_IT, ship_lua);
  setglobal("LANG_DK", LANG_DK, ship_lua);

  setglobal("VERSION", float(atof(VERSION)+0.001), ship_lua);

  int error = 0;

  int load_from_datafile = -1;
  char sfilename[255];
  if (strchr(file, '#'))
  {
    strncpy(sfilename, get_filename(file), 50);
    // Convert to lowercase manually
    for (int i = 0; sfilename[i]; i++) {
        sfilename[i] = tolower(sfilename[i]);
    }

    if (!strcmp(sfilename, "defnew_shp")) load_from_datafile = defnew_shp;
    else if (!strcmp(sfilename, "def1_shp")) load_from_datafile = def1_shp;
    else if (!strcmp(sfilename, "def2_shp")) load_from_datafile = def2_shp;
    else if (!strcmp(sfilename, "def3_shp")) load_from_datafile = def3_shp;
  }

  // load standard ships from datafile (v0.8)
  char defnew[1500]; memset(defnew, 0, sizeof(defnew));
  if (load_from_datafile > -1)
  {
    char mypath[255];
    sprintf(mypath, "%s%s#%s", globals->data_dir, globals->shipdata_name, sfilename);
//    errors->log(2, "opening ship from ship file", mypath);
    packfile_password(globals->pwd);
    PACKFILE *P = pack_fopen(mypath, "rp");
    if (P) 
    {
      pack_fread(defnew, 1500, P);
      pack_fclose(P);
      packfile_password(NULL);
      error = ship_lua->dostring(defnew);
    }
    else
    {
      errors->log(2, "can't load", mypath);
      error = -6;
    } 
  }
  else if (!exists(file)) error = -5;
  else if (file_size(file) < 500) error = -4;
  else error = ship_lua->dofile(file);

  if (error)
  {
    errors->log(0, "script", "load player ship: file not found or invalid", file, error);
    p->load_ship_image((predefined_ships)SHIP_PIRANHA, 0, 0);
    p->init();
  }
  else
  {
    int sum = 0;
    int i;
    float f;

    i = getglobal_int("type", ship_lua);
  
    if (i < 0 || i > 4) { error = 1; error_wrong_number("type", i); }

    p->set_ship_type((predefined_ships)i);

    int ship_color = 0;
    if (i != SHIP_CUSTOM)
    {
      i = getglobal_int("color", ship_lua);
      if (i >= 0 && i <= 4) ship_color = i;
    }

    strncpy(shipimage, getglobal_string("image", ship_lua), 20);

    p->set_ship_name(getglobal_string("name", ship_lua));

    p->set_ship_desc(getglobal_string("description", ship_lua));

    i = getglobal_int("radar_zoom", ship_lua);
    if (i < 0 || i > 20) { error = 1; error_wrong_number("radar_zoom", i); }
    if (i) sum += i;
    p->set_percent_value(PV_RADAR_ZOOM, i);

    i = getglobal_int("weapon_slots", ship_lua);
    if (i < 0 || i > 6) { error = 1; error_wrong_number("weapon_slots", i); }
    if (i) sum += i*5;
    p->set_percent_value(PV_WEAPON_SLOTS, i);

    i = getglobal_int("hull", ship_lua);
    if (i < 0 || i > 20) { error = 1; error_wrong_number("hull", i); }
    sum += i*4;
    p->set_percent_value(PV_HULL, i);

    i = getglobal_int("shield", ship_lua);
    if (i < 0 || i > 20) { error = 1; error_wrong_number("shield", i); }
    sum += i*8;
    p->set_percent_value(PV_SHIELD, i);

    i = getglobal_int("shield_recharge", ship_lua);
    if (i < 0 || i > 100) { error = 1; error_wrong_number("shield_recharge", i); }
    sum += i;
    f = (i / 10526.32) + 0.001;
    p->set_percent_value(PV_SHIELD_RE, i);

    i = getglobal_int("wall_shield", ship_lua);
    if (i < 0 || i > 100) { error = 1; error_wrong_number("wall_shield", i); }
    sum += i;
    f = (i / 5000) + 0.002;
    p->set_percent_value(PV_WALLSHIELD, i);

    i = getglobal_int("hull_repair", ship_lua);
    if (i < 0 || i > 100) { error = 1; error_wrong_number("hull_repair", i); }
    sum += i;
    f = (i / 15306.0) + 0.0002;
    p->set_percent_value(PV_HULL_RE, i);

    i = getglobal_int("turn_speed", ship_lua);
    if (i < 0 || i > 100) { error = 1; error_wrong_number("turn_speed", i); }
    sum += i;
    f = i / 8.0;
    p->set_percent_value(PV_TURN_SPEED, i);

    i = getglobal_int("maximum_speed", ship_lua);
    if (i < 0 || i > 100) { error = 1; error_wrong_number("maximum_speed", i); }
    sum += i;
    f = i / 11.0;
    p->set_percent_value(PV_MAX_SPEED, i);

    i = getglobal_int("acceleration", ship_lua);
    if (i < 0 || i > 100) { error = 1; error_wrong_number("acceleration", i); }
    sum += i;
    f = (i / 700) + 0.01;
    p->set_percent_value(PV_ACCELERATION, i);

    i = getglobal_int("fuel_consumption", ship_lua);
    if (i < 0 || i > 100) { error = 1; error_wrong_number("fuel_consumption", i); }
    sum += i;
    f = 0.1 - ((i / 1052.63) + 0.005);
    p->set_percent_value(PV_FUEL_CONS, i);

    i = getglobal_int("weapon_storage", ship_lua);
    if (i < 0 || i > 100) { error = 1; error_wrong_number("weapon_storage", i); }
    sum += i;
    p->set_percent_value(PV_WEAPON_STORAGE, i);

    i = getglobal_int("weapon_recharge", ship_lua);
    if (i < 0 || i > 100) { error = 1; error_wrong_number("weapon_recharge", i); }
    sum += i;
    p->set_percent_value(PV_WEAPON_RE, i);

    i = getglobal_int("extras", ship_lua);
    if (i == EXTRA_NONE)           { p->set_extra(EXTRA_NONE); }
    if (i == EXTRA_AFTERBURNER)    { p->set_extra(EXTRA_AFTERBURNER); sum += 30; }
    else if (i == EXTRA_DEFLECTOR) { p->set_extra(EXTRA_DEFLECTOR); sum += 30; }
    else if (i == EXTRA_CLOAK)     { p->set_extra(EXTRA_CLOAK); sum += 30; }

    testint = sum;

    // weapons
    i = getglobal_int("weapon_slot_1", ship_lua);
    set_player_weapon(p, 0, i);
    i = getglobal_int("weapon_slot_2", ship_lua);
    set_player_weapon(p, 1, i);
    i = getglobal_int("weapon_slot_3", ship_lua);
    set_player_weapon(p, 2, i);
    i = getglobal_int("weapon_slot_4", ship_lua);
    set_player_weapon(p, 3, i);
    i = getglobal_int("weapon_slot_5", ship_lua);
    set_player_weapon(p, 4, i);
    i = getglobal_int("weapon_slot_6", ship_lua);
    set_player_weapon(p, 5, i);

    p->convert_percents_to_shipvalues();
    p->set_standard_bullets();

    if (error)
    {
      errors->log(0, "script", "load player ship: wrong arguments, playship resetted to default values!");
      p->init();
    }
    else if (sum > max_sum)
    {
      errors->log(0, "script", "load player ship: sum exceeds limit, playship resetted to default values!", sum);
      error = 1;
      p->init();
    }
    else
    {
      if (p->get_ship_type() == SHIP_CUSTOM)
      {
        char tmp[50];
        strcpy(tmp, globals->ship_dir); strcat(tmp, shipimage);

//        errors->log(2, "loading player ship", "trying to load custom ship image", tmp);

        if (p->load_ship_image(tmp, last_type))
          errors->log(0, "loading player ship", "error loading custom ship image");
//        else
//          errors->log(2, "loading player ship", "successfully loaded");
      }
      else
      {
//         errors->log(2, "loading player ship", "loading predefined ship image");
         p->load_ship_image((predefined_ships)p->get_ship_type(), ship_color, last_type);
      }

      p->reinit();
    }
  }

  return error;
}

char _glob_level_dir[255], *_loadtype;

int read_level(const char *fname, int attrib, void *mparam)
{
  int param = (int)mparam;
//errors->log(2, "loading level", (char*)fname, param);
  float my_version = atof(VERSION)+0.001;
//  errors->log(2, "my version", my_version);
  strcpy(globals->level_files[globals->max_levels].leveldir, _glob_level_dir);
  script->load_level_init((char*)fname, &globals->level_files[globals->max_levels]);
  if (globals->level_files[globals->max_levels].min_version <= my_version)
  {
    // loadall
    if (param == 1)
    {
      globals->max_levels++;
      return 0;
    } 
    
    // singleplayer
    if (globals->singleplayer && 
        globals->level_files[globals->max_levels].activate_singleplayer &&
        !globals->level_files[globals->max_levels].activate_minigame)
    {
      if (!strcmp(_loadtype, "r") && !globals->level_files[globals->max_levels].activate_race)
        return 0;
      if (!strcmp(_loadtype, "m") && globals->level_files[globals->max_levels].activate_race)
        return 0;
        
      globals->max_levels++;
      return 0;
    }
    
    // twoplayer || network
    if (!globals->minigame &&
        !globals->singleplayer &&
        globals->level_files[globals->max_levels].max_players >= 2 && 
        !globals->level_files[globals->max_levels].activate_minigame)
    {
      if (!strcmp(_loadtype, "r") && !globals->level_files[globals->max_levels].activate_race)
        return 0;
      if (!strcmp(_loadtype, "m") && globals->level_files[globals->max_levels].activate_race)
        return 0;

      globals->max_levels++;
      return 0;
    }

    // minigame        
    if (globals->minigame &&
        globals->level_files[globals->max_levels].activate_minigame)
    {
      globals->max_levels++;
      return 0;
    } 
  }

  return 0;
}

void gsScript::read_level_files(int loadall, char *loadtype)
{
  // Use standard C++ file operations instead of Allegro functions
  char path[255];

  strcpy(path, globals->level_dir);
  strcat(path, "*.*");

  globals->max_levels = 0;

  _loadtype = loadtype;
  
  // TODO: Implement file scanning using standard C++ functions
  // For now, just log that this function is not fully implemented
  errors->log(1, "script", "read_level_files: file scanning not implemented");

  sort_level_files();

//errors->log(2, "init", "loaded level files...", globals->max_levels);
}

void gsScript::sort_level_files()
{
  int minIndex,i,k;
  level_info memo;

  for (i=0; i < globals->max_levels-1; i++)
  {
    minIndex = i;
    for (k=i; k < globals->max_levels; k++)
    {
      if (stricmp(globals->level_files[k].name,
                  globals->level_files[minIndex].name) < 0) minIndex = k;
    }
    memo = globals->level_files[i];
    globals->level_files[i] = globals->level_files[minIndex];
    globals->level_files[minIndex] = memo;
  }
}

void gsScript::load_game_config()
{
  Lua *config_lua = new Lua;
  
  // clear globals
//  config_lua->newtable();
//  config_lua->replace(LUA_GLOBALSINDEX);

  set_standard_globals(config_lua);

  char path[80];
  strcpy(path, globals->data_dir);
  strcat(path, globals->configfile_name);
  int error = config_lua->dofile(path);

  if (!error)
  {
    int i;

    i = getglobal_int("language", config_lua);
    if (i >= 0 && i <= 2) globals->language = i;

    i = getglobal_int("pixels", config_lua);
    if (i >= 0 && i <= 200) globals->pixamount = i;

    i = getglobal_int("sound_volume", config_lua);
    if (i >= 0 && i <= 100) globals->soundvol = i;

    i = getglobal_int("music_volume", config_lua);
    if (i >= 0 && i <= 100) globals->musicvol = i;

    i = getglobal_int("gravity", config_lua);
    if (i >= -100 && i <= 200) globals->gravity_percent = i/100.0;

    i = getglobal_int("extras", config_lua);
    if (i >= 0 && i <= 10) globals->extra_amount = i;

    i = getglobal_int("maxfrags", config_lua);
    if (i >= 1 && i <= 50) globals->max_frags = i;

    i = getglobal_int("maxtime", config_lua);
    if (i >= 0 && i <= 3000) globals->max_time = i;

    i = getglobal_int("slow_computer", config_lua);
    if (i > 0) globals->slow_computer = 1;

    i = getglobal_int("use_script_computer_player", config_lua);
    if (i > 0) globals->use_script_computer_player = 1;

    i = getglobal_int("resolution", config_lua);
    if (i >= 0 && i <= 4) globals->use_resolution_nr = i;

    strncpy(globals->server_ip, getglobal_string("server_ip", config_lua), 16);

    strncpy(globals->pplayer[0].pname, getglobal_string("player1_name", config_lua), 20);
    if (strlen(globals->pplayer[0].pname) == 0) strcpy(globals->pplayer[0].pname, "Player 1");
    strncpy(globals->pplayer[1].pname, getglobal_string("player2_name", config_lua), 20);
    if (strlen(globals->pplayer[1].pname) == 0) strcpy(globals->pplayer[1].pname, "Player 2");
    strncpy(globals->pplayer[0].pship, getglobal_string("player1_ship", config_lua), 20);
    strncpy(globals->pplayer[1].pship, getglobal_string("player2_ship", config_lua), 20);
    strncpy(globals->last_level, getglobal_string("default_map", config_lua), 20);
    strncpy(globals->last_splevel, getglobal_string("default_spmap", config_lua), 20);
    strncpy(globals->last_minigame, getglobal_string("default_minigame", config_lua), 20);
  } // if no error
  
  delete config_lua;
}

void gsScript::load_ingame_script(char *dir, char *name, char *glob)
{
  char sc_path[255], gl_path[255];
  int sc_size, gl_size;

  sprintf(sc_path, "%s%s/%s", globals->level_dir, dir, name);
  if (glob) sprintf(gl_path, "%s/%s", globals->level_dir, glob);

  sc_size = gl_size = 0;
  if (name && exists(sc_path)) sc_size = file_size(sc_path);
  else name = NULL;
  if (glob && exists(gl_path)) gl_size = file_size(gl_path);
  else glob = NULL;

  script_buffer_size = sc_size + gl_size;

  if (script_buffer)
  {
    delete[] script_buffer; script_buffer = NULL;
  }

  if (script_buffer_size > 0)
  {
    script_buffer = new char[script_buffer_size];
    memset(script_buffer, 0, script_buffer_size);
  }

  FILE *sc_f = NULL;
  if (name) sc_f = fopen(sc_path, "rb");
  FILE *gl_f = NULL;
  if (glob) gl_f = fopen(gl_path, "rb");

  if (gl_f)
  {
    fread(script_buffer, 1, gl_size, gl_f);
    fclose(gl_f);
  }
  else
  {
    errors->log(1, "script", "could not load global script", glob);
  }

  if (sc_f)
  {
    fread(script_buffer+gl_size, 1, sc_size, sc_f);
    fclose(sc_f);
  }
  else if (name)
  {
    errors->log(1, "script", "could not load level script", name);
  }
}

int gsScript::init_ingame_script()
{
  if (script_buffer)
  {
//    lua.dobuffer(script_buffer, script_buffer_size, "levelscript");
  }

  return 0;
}

int gsScript::do_ingame_script(Lua *l)
{
  if (script_buffer)
  {
    int err = l->dobuffer(script_buffer, script_buffer_size, "levelscript");
/*    lua.getglobal("__ingame_script__");
    lua.call(0, 0);
    lua.pop(1);
*/
  }

  return 0;
}

void gsScript::clear_ingame_script()
{
  if (script_buffer) { delete[] script_buffer; script_buffer = NULL; }
}

void gsScript::init_script_commands(Lua *l, int newtable)
{
  if (newtable==1) 
  {
    l->newtable();
    l->replace(LUA_GLOBALSINDEX);
  }

  l->open_base();
  l->open_math();
  l->open_string();
  l->open_io();
  l->open_table();

  // clear stack
  l->settop(0);

  // init lua
  tolua_gslua_open(l->get_state());

  // globals & functions
  set_standard_globals(l);

  l->registerfunction("int_mod", script_mod);
  l->registerfunction("get_key", script_get_key);
  l->registerfunction("get_mouse_xy", script_get_mouse_xy);
  l->registerfunction("set_mouse_xy", script_set_mouse_xy);
  l->registerfunction("get_mouse_buttons", script_get_mouse_buttons);
}

void gsScript::kill_script_commands(Lua *l)
{
 // tolua_gslua_close(l->get_state());
}

int gsScript::is_flag(int *f)
{
  for (int n=0; n < MAX_HOOKS; n++)
  {
    if (f[n]) return 1;
  }
  return 0;
}

void gsScript::init_hooks(Lua *l)
{
  int n;
  char func[50];

  // object_dead
  for (n=0; n < MAX_HOOKS; n++)
  {
    sprintf(func, "hook_object_dead_%d", n);

    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    
    if (l->isfunction(-1))
    {
      flag_hook_object_dead[n] = 1;
      l->pop(1);
      errors->log(2, "script init", "found hook_object_dead", n);
    }
    else { flag_hook_object_dead[n] = 0; l->pop(1); }
  
    // object_update
    sprintf(func, "hook_object_update_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    if (l->isfunction(-1))
    {
      flag_hook_object_update[n] = 1;
      l->pop(1);
      errors->log(2, "script init", "found hook_object_update", n);
    }
    else { flag_hook_object_update[n] = 0; l->pop(1); }
  
    // object_hit
    sprintf(func, "hook_object_hit_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    if (l->isfunction(-1))
    {
      flag_hook_object_hit[n] = 1;
      l->pop(1);
      errors->log(2, "script init", "found hook_object_hit", n);
    }
    else { flag_hook_object_hit[n] = 0; l->pop(1); }

    // object_with_object
    sprintf(func, "hook_object_with_object_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    if (l->isfunction(-1))
    {
      flag_hook_object_with_object[n] = 1;
      l->pop(1);
      errors->log(2, "script init", "found hook_object_with_object", n);
    }
    else { flag_hook_object_with_object[n] = 0; l->pop(1); }
    
    // player_dead
    sprintf(func, "hook_player_dead_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    if (l->isfunction(-1))
    {
      flag_hook_player_dead[n] = 1;
      l->pop(1);
      errors->log(2, "script init", "found hook_player_dead", n);
    }
    else { flag_hook_player_dead[n] = 0; l->pop(1); }

    // player_respawn
    sprintf(func, "hook_player_respawn_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    if (l->isfunction(-1))
    {
      flag_hook_player_respawn[n] = 1;
      l->pop(1);
      errors->log(2, "script init", "found hook_player_respawn", n);
    }
    else { flag_hook_player_respawn[n] = 0; l->pop(1); }
    
    // player_hit
    sprintf(func, "hook_player_hit_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    if (l->isfunction(-1))
    {
      flag_hook_player_hit[n] = 1;
      l->pop(1);
      errors->log(2, "script init", "found hook_player_hit", n);
    }
    else { flag_hook_player_hit[n] = 0; l->pop(1); }
  
    // collision with object
    sprintf(func, "hook_player_with_spobject_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    if (l->isfunction(-1))
    {
      flag_hook_player_with_spobject[n] = 1;
      l->pop(1);
      errors->log(2, "script init", "found hook_player_with_spobject", n);
    }
    else { flag_hook_player_with_spobject[n] = 0; l->pop(1); }

    // player takes up cargo
    sprintf(func, "hook_player_takes_cargo_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    if (l->isfunction(-1))
    {
      flag_hook_player_takes_cargo[n] = 1;
      l->pop(1);
      errors->log(2, "script init", "found hook_player_takes_cargo", n);
    }
    else { flag_hook_player_takes_cargo[n] = 0; l->pop(1); }

    // player returns up cargo
    sprintf(func, "hook_player_returns_cargo_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    if (l->isfunction(-1))
    {
      flag_hook_player_returns_cargo[n] = 1;
      l->pop(1);
      errors->log(2, "script init", "found hook_player_returns_cargo", n);
    }
    else { flag_hook_player_returns_cargo[n] = 0; l->pop(1); }

    // player lands
    sprintf(func, "hook_player_land_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    if (l->isfunction(-1))
    {
      flag_hook_player_land[n] = 1;
      l->pop(1);
      errors->log(2, "script init", "found hook_player_land", n);
    }
    else { flag_hook_player_land[n] = 0; l->pop(1); }

    // level-init
    sprintf(func, "hook_init_level_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    if (l->isfunction(-1))
    {
      flag_hook_init_level[n] = 1;
      l->pop(1);
      errors->log(2, "script init", "found hook_init_level", n);
    }
    else { flag_hook_init_level[n] = 0; l->pop(1); }

    // level-exit
    sprintf(func, "hook_exit_level_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    if (l->isfunction(-1))
    {
      flag_hook_exit_level[n] = 1;
      l->pop(1);
      errors->log(2, "script init", "found hook_exit_level", n);
    }
    else { flag_hook_exit_level[n] = 0; l->pop(1); }


    // enemy_hit
    sprintf(func, "hook_enemy_hit_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    if (l->isfunction(-1))
    {
      flag_hook_enemy_hit[n] = 1;
      l->pop(1);
      errors->log(2, "script init", "found hook_enemy_hit", n);
    }
    else { flag_hook_enemy_hit[n] = 0; l->pop(1); }

    // bullet_destroyed
    sprintf(func, "hook_bullet_destroyed_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    if (l->isfunction(-1))
    {
      flag_hook_bullet_destroyed[n] = 1;
      l->pop(1);
      errors->log(2, "script init", "found hook_bullet_destroyed", n);
    }
    else { flag_hook_bullet_destroyed[n] = 0; l->pop(1); }

    // enemy_update
    sprintf(func, "hook_enemy_update_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    if (l->isfunction(-1))
    {
      flag_hook_enemy_update[n] = 1;
      l->pop(1);
      errors->log(2, "script init", "found hook_enemy_update", n);
    }
    else { flag_hook_enemy_update[n] = 0; l->pop(1); }

    // enemy_dead
    sprintf(func, "hook_enemy_dead_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    if (l->isfunction(-1))
    {
      flag_hook_enemy_dead[n] = 1;
      l->pop(1);
      errors->log(2, "script init", "found hook_enemy_dead", n);
    }
    else { flag_hook_enemy_dead[n] = 0; l->pop(1); }

    // enemy_shoot
    sprintf(func, "hook_enemy_shoot_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    if (l->isfunction(-1))
    {
      flag_hook_enemy_shoot[n] = 1;
      l->pop(1);
      errors->log(2, "script init", "found hook_enemy_shoot", n);
    }
    else { flag_hook_enemy_shoot[n] = 0; l->pop(1); }

    // player_with_enemy
    sprintf(func, "hook_player_with_enemy_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    if (l->isfunction(-1))
    {
      flag_hook_player_with_enemy[n] = 1;
      l->pop(1);
      errors->log(2, "script init", "found hook_player_with_enemy", n);
    }
    else { flag_hook_player_with_enemy[n] = 0; l->pop(1); }

    // enemy_with_enemy
    sprintf(func, "hook_enemy_with_enemy_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    if (l->isfunction(-1))
    {
      flag_hook_enemy_with_enemy[n] = 1;
      l->pop(1);
      errors->log(2, "script init", "found hook_enemy_with_enemy", n);
    }
    else { flag_hook_enemy_with_enemy[n] = 0; l->pop(1); }

    // keyboard_hit
    sprintf(func, "hook_keyboard_hit_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    if (l->isfunction(-1))
    {
      flag_hook_keyboard_hit[n] = 1;
      l->pop(1);
      errors->log(2, "script init", "found hook_keyboard_hit", n);
    }
    else { flag_hook_keyboard_hit[n] = 0; l->pop(1); }

  } // for

}

int gsScript::do_hook_object_dead(Lua *l, Object *o)
{
  int ret = 0;

  o->set_dont_continue(0);

  for (int n=0; n < MAX_HOOKS; n++)
  if (flag_hook_object_dead[n])
  {
    char func[50];
    sprintf(func, "hook_object_dead_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    tolua_pushusertype(l->get_state(),(void*)o,"Object");
    l->call(1, 1);
    if (l->isnumber(-1)) ret = (int)l->tonumber(-1);
    l->pop(1);
  }

  return ret;
}

int gsScript::do_hook_object_update(Lua *l, Object *o)
{
  int ret = 0;

  o->set_dont_continue(0);

  for (int n=0; n < MAX_HOOKS; n++)
  if (flag_hook_object_update[n])
  {
    char func[50];
    sprintf(func, "hook_object_update_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    tolua_pushusertype(l->get_state(),(void*)o,"Object");
    l->call(1, 1);
    if (l->isnumber(-1)) ret = (int)l->tonumber(-1);
    l->pop(1);
  }

  return ret;
}

int gsScript::do_hook_object_hit(Lua *l, Object *o, int weap, int phitp, int fx, int fy)
{
  int ret = 0;

  o->set_dont_continue(0);

  for (int n=0; n < MAX_HOOKS; n++)
  if (flag_hook_object_hit[n])
  {
    char func[50];
    sprintf(func, "hook_object_hit_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    tolua_pushusertype(l->get_state(),(void*)o,"Object");
    tolua_pushnumber(l->get_state(), weap);
    tolua_pushnumber(l->get_state(), phitp);
    tolua_pushnumber(l->get_state(), fx);
    tolua_pushnumber(l->get_state(), fy);
    l->call(5, 1);
    if (l->isnumber(-1)) ret = (int)l->tonumber(-1);
    l->pop(1);
  }

  return ret;
}

int gsScript::do_hook_object_with_object(Lua *l, Object *o1, Object *o2)
{
  int ret = 0;

  for (int n=0; n < MAX_HOOKS; n++)
  if (flag_hook_object_with_object[n])
  {
    char func[50];
    sprintf(func, "hook_object_with_object_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    tolua_pushusertype(l->get_state(),(void*)o1,"Object");
    tolua_pushusertype(l->get_state(),(void*)o2,"Object");
    l->call(2, 1);
    if (l->isnumber(-1)) ret = (int)l->tonumber(-1);
    l->pop(1);
  }

  return ret;
}

int gsScript::do_hook_player_dead(Lua *l, gsPlayer *o)
{
  int ret = 0;

  o->set_dont_continue(0);

  for (int n=0; n < MAX_HOOKS; n++)
  if (flag_hook_player_dead[n])
  {
    char func[50];
    sprintf(func, "hook_player_dead_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    tolua_pushusertype(l->get_state(),(void*)o,"gsPlayer");
    l->call(1, 1);
    if (l->isnumber(-1)) ret = (int)l->tonumber(-1);
    l->pop(1);
  }

  return ret;
}

int gsScript::do_hook_player_respawn(Lua *l, gsPlayer *o)
{
  int ret = 0;

  o->set_dont_continue(0);

  for (int n=0; n < MAX_HOOKS; n++)
  if (flag_hook_player_respawn[n])
  {
    char func[50];
    sprintf(func, "hook_player_respawn_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    tolua_pushusertype(l->get_state(),(void*)o,"gsPlayer");
    l->call(1, 1);
    if (l->isnumber(-1)) ret = (int)l->tonumber(-1);
    l->pop(1);
  }

  return ret;
}

int gsScript::do_hook_player_hit(Lua *l, gsPlayer *o, int weap, int phitp, float fxspd, float fyspd)
{
  int ret = 0;

  o->set_dont_continue(0);

  for (int n=0; n < MAX_HOOKS; n++)
  if (flag_hook_player_hit[n])
  {
    char func[50];
    sprintf(func, "hook_player_hit_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    tolua_pushusertype(l->get_state(),(void*)o,"gsPlayer");
    tolua_pushnumber(l->get_state(), weap);
    tolua_pushnumber(l->get_state(), phitp);
    tolua_pushnumber(l->get_state(), fxspd);
    tolua_pushnumber(l->get_state(), fyspd);
    l->call(5, 1);
    if (l->isnumber(-1)) ret = (int)l->tonumber(-1);
    l->pop(1);
  }

  return ret;
}

int gsScript::do_hook_player_with_spobject(Lua *l, gsPlayer *p, Object *o)
{
  int ret = 0;

  p->set_dont_continue(0);

  for (int n=0; n < MAX_HOOKS; n++)
  if (flag_hook_player_with_spobject[n])
  {
    char func[50];
    sprintf(func, "hook_player_with_spobject_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    tolua_pushusertype(l->get_state(),(void*)p,"gsPlayer");
    tolua_pushusertype(l->get_state(),(void*)o,"Object");
    l->call(2, 1);
    if (l->isnumber(-1)) ret = (int)l->tonumber(-1);
    l->pop(1);
  }

  return ret;
}

int gsScript::do_hook_player_takes_cargo(Lua *l, gsPlayer *p, Base *b)
{
  int ret = 0;

  p->set_dont_continue(0);

  for (int n=0; n < MAX_HOOKS; n++)
  if (flag_hook_player_takes_cargo[n])
  {
    char func[50];
    sprintf(func, "hook_player_takes_cargo_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    tolua_pushusertype(l->get_state(),(void*)p,"gsPlayer");
    tolua_pushusertype(l->get_state(),(void*)b,"Base");
    l->call(2, 1);
    if (l->isnumber(-1)) ret = (int)l->tonumber(-1);
    l->pop(1);
  }

  return ret;
}

int gsScript::do_hook_player_returns_cargo(Lua *l, gsPlayer *p)
{
  int ret = 0;

  p->set_dont_continue(0);

  for (int n=0; n < MAX_HOOKS; n++)
  if (flag_hook_player_returns_cargo[n])
  {
    char func[50];
    sprintf(func, "hook_player_returns_cargo_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    tolua_pushusertype(l->get_state(),(void*)p,"gsPlayer");
    l->call(1, 1);
    if (l->isnumber(-1)) ret = (int)l->tonumber(-1);
    l->pop(1);
  }

  return ret;
}

int gsScript::do_hook_player_land(Lua *l, gsPlayer *p, Base *b)
{
  int ret = 0;

  p->set_dont_continue(0);

  for (int n=0; n < MAX_HOOKS; n++)
  if (flag_hook_player_land[n])
  {
    char func[50];
    sprintf(func, "hook_player_land_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    tolua_pushusertype(l->get_state(),(void*)p,"gsPlayer");
    tolua_pushusertype(l->get_state(),(void*)b,"Base");
    l->call(2, 1);
    if (l->isnumber(-1)) ret = (int)l->tonumber(-1);
    l->pop(1);
  }

  return ret;
}

int gsScript::do_hook_init_level(Lua *l)
{
  int ret = 0;

  for (int n=0; n < MAX_HOOKS; n++)
  if (flag_hook_init_level[n])
  {
    char func[50];
    sprintf(func, "hook_init_level_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    l->call(0, 1);
    if (l->isnumber(-1)) ret = (int)l->tonumber(-1);
    l->pop(1);
  }

  return ret;
}

int gsScript::do_hook_exit_level(Lua *l)
{
  int ret = 0;

  for (int n=0; n < MAX_HOOKS; n++)
  if (flag_hook_exit_level[n])
  {
    char func[50];
    sprintf(func, "hook_exit_level_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    l->call(0, 1);
    if (l->isnumber(-1)) ret = (int)l->tonumber(-1);
    l->pop(1);
  }

  return ret;
}

int gsScript::do_hook_enemy_hit(Lua *l, Enemy *e, int pweap, int phitp, int fx, int fy)
{
  int ret = 0;

  e->set_dont_continue(0);

  for (int n=0; n < MAX_HOOKS; n++)
  if (flag_hook_enemy_hit[n])
  {
    char func[50];
    sprintf(func, "hook_enemy_hit_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    tolua_pushusertype(l->get_state(),(void*)e,"Enemy");
    tolua_pushnumber(l->get_state(), pweap);
    tolua_pushnumber(l->get_state(), phitp);
    tolua_pushnumber(l->get_state(), fx);
    tolua_pushnumber(l->get_state(), fy);
    l->call(5, 1);
    if (l->isnumber(-1)) ret = (int)l->tonumber(-1);
    l->pop(1);
  }

  return ret;
}

int gsScript::do_hook_enemy_update(Lua *l, Enemy *e)
{
  int ret = 0;

  e->set_dont_continue(0);

  for (int n=0; n < MAX_HOOKS; n++)
  if (flag_hook_enemy_update[n])
  {
    char func[50];
    sprintf(func, "hook_enemy_update_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    tolua_pushusertype(l->get_state(),(void*)e,"Enemy");
    l->call(1, 1);
    if (l->isnumber(-1)) ret = (int)l->tonumber(-1);
    l->pop(1);
  }

  return ret;
}

int gsScript::do_hook_enemy_dead(Lua *l, Enemy *e)
{
  int ret = 0;

  e->set_dont_continue(0);

  for (int n=0; n < MAX_HOOKS; n++)
  if (flag_hook_enemy_dead[n])
  {
    char func[50];
    sprintf(func, "hook_enemy_dead_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    tolua_pushusertype(l->get_state(),(void*)e,"Enemy");
    l->call(1, 1);
    if (l->isnumber(-1)) ret = (int)l->tonumber(-1);
    l->pop(1);
  }

  return ret;
}

int gsScript::do_hook_enemy_shoot(Lua *l, Enemy *e)
{
  int ret = 0;

  e->set_dont_continue(0);

  for (int n=0; n < MAX_HOOKS; n++)
  if (flag_hook_enemy_shoot[n])
  {
    char func[50];
    sprintf(func, "hook_enemy_shoot_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    tolua_pushusertype(l->get_state(),(void*)e,"Enemy");
    l->call(1, 1);
    if (l->isnumber(-1)) ret = (int)l->tonumber(-1);
    l->pop(1);
  }

  return ret;
}

int gsScript::do_hook_bullet_destroyed(Lua *l, Bullet *e)
{
  int ret = 0;

  e->set_dont_continue(0);

  for (int n=0; n < MAX_HOOKS; n++)
  if (flag_hook_bullet_destroyed[n])
  {
    char func[50];
    sprintf(func, "hook_bullet_destroyed_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    tolua_pushusertype(l->get_state(),(void*)e,"Bullet");
    l->call(1, 1);
    if (l->isnumber(-1)) ret = (int)l->tonumber(-1);
    l->pop(1);
  }

  return ret;
}

int gsScript::do_hook_player_with_enemy(Lua *l, gsPlayer *p, Enemy *e)
{
  int ret = 0;

  p->set_dont_continue(0);

  for (int n=0; n < MAX_HOOKS; n++)
  if (flag_hook_player_with_enemy[n])
  {
    char func[50];
    sprintf(func, "hook_player_with_enemy_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    tolua_pushusertype(l->get_state(),(void*)p,"gsPlayer");
    tolua_pushusertype(l->get_state(),(void*)e,"Enemy");
    l->call(2, 1);
    if (l->isnumber(-1)) ret = (int)l->tonumber(-1);
    l->pop(1);
  }

  return ret;
}

int gsScript::do_hook_enemy_with_enemy(Lua *l, Enemy *e1, Enemy *e2)
{
  int ret = 0;

  for (int n=0; n < MAX_HOOKS; n++)
  if (flag_hook_enemy_with_enemy[n])
  {
    char func[50];
    sprintf(func, "hook_enemy_with_enemy_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    tolua_pushusertype(l->get_state(),(void*)e1,"Enemy");
    tolua_pushusertype(l->get_state(),(void*)e2,"Enemy");
    l->call(2, 1);
    if (l->isnumber(-1)) ret = (int)l->tonumber(-1);
    l->pop(1);
  }

  return ret;
}

int gsScript::do_hook_keyboard_hit(Lua *l, int k)
{
  int ret = 0;

  for (int n=0; n < MAX_HOOKS; n++)
  if (flag_hook_keyboard_hit[n])
  {
    char func[50];
    sprintf(func, "hook_keyboard_hit_%d", n);
    l->pushstring(func);
    l->gettable(LUA_GLOBALSINDEX);
    tolua_pushnumber(l->get_state(), k);
    l->call(1, 1);
    if (l->isnumber(-1)) ret = (int)l->tonumber(-1);
    l->pop(1);
  }

  return ret;
}

static int script_set_string(lua_State *L)
{
  int n = lua_gettop(L);

  if (n == 2)
  {
    int error = 0;
    if (!lua_isnumber(L, 1))
    {
      errors->log(0, "script",  "set text: argument 1 of wrong type!");
      error = 1;
    }
    if (!lua_isstring(L, 2))
    {
      errors->log(0, "script", "set text: argument 2 of wrong type!");
      error = 1;
    }

    if (!error)
    {
      int num = (int)lua_tonumber(L, 1);
      char *s = (char*)lua_tostring(L, 2);

      language->set_string((TEXT_STRINGS)num, s);
    }
  }
  else
  {
    errors->log(0, "script", "set string: wrong argument count (not 2)!");
  }

  return 0;
}

static int script_mod(lua_State *L)
{
  int n = lua_gettop(L);    /* number of arguments */
  static int number = 0;

  number++;

  if (n == 2)
  {
    int num1 = (int)lua_tonumber(L, 1);
    int num2 = (int)lua_tonumber(L, 2);
    if (num2 == 0) return 0;
    lua_pushnumber(L, num1 % num2);
    return 1;
  }
  else
  {
    errors->log(0, "script", "mod command: wrong argument count (must be 2)!", number);
  }

  return 0;
}

static int script_get_key(lua_State *L)
{
  int n = lua_gettop(L);    /* number of arguments */
  static int number = 0;

  number++;

  if (n == 1)
  {
    int k = (int)lua_tonumber(L, 1);
    if (key[k]) lua_pushnumber(L, 1);
    else lua_pushnumber(L, 0);
    return 1;
  }
  else
  {
    errors->log(0, "script", "get_key command: wrong argument count (must be 1)!", number);
  }

  return 0;
}

static int script_set_mouse_xy(lua_State *L)
{
  int n = lua_gettop(L);    /* number of arguments */

  if (n == 2)
  {
    int mx = (int)lua_tonumber(L, 1);
    int my = (int)lua_tonumber(L, 2);
    
    position_mouse(mx, my);
  }
  else
  {
    errors->log(0, "script", "set_mouse_xy command: wrong argument count (must be 2)!");
  }

  return 0;
}

static int script_get_mouse_xy(lua_State *L)
{
  int n = lua_gettop(L);    /* number of arguments */

  if (n == 0)
  {
    lua_pushnumber(L, mouse_x);
    lua_pushnumber(L, mouse_y);
    return 2;
  }
  else
  {
    errors->log(0, "script", "get_mouse_xy command: wrong argument count (must be 0)!");
  }

  return 0;
}

static int script_get_mouse_buttons(lua_State *L)
{
  int n = lua_gettop(L);    /* number of arguments */

  if (n == 0)
  {
    if (mouse_b & 1) lua_pushnumber(L, 1); else lua_pushnil(L);
    if (mouse_b & 2) lua_pushnumber(L, 1); else lua_pushnil(L);
    return 2;
  }
  else
  {
    errors->log(0, "script", "get_mouse_buttons command: wrong argument count (must be 0)!");
  }

  return 0;
}

int gsScript::get_gcthreshold(Lua *l)
{
  return lua_getgcthreshold(l->get_state());
}

int gsScript::get_gccount(Lua *l)
{
  return lua_getgccount(l->get_state());
}

void gsScript::set_gcthreshold(Lua *l, int n)
{
  lua_setgcthreshold(l->get_state(), n);
}


