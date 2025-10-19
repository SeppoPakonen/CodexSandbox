/*
 *   GRAVITY STRIKE -- gsscript.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#ifndef _GSSCRIPT_
#define _GSSCRIPT_

//#include <lua.h>
#include "luaclass.h"
#include "tolua.h"

#include "gsglob.h"

#define MAX_HOOKS 10

extern Lua *glob_lua;              // lua (scripting language) object
extern int debug_lua;               // only debug the lua script?

extern void tolua_myerror(lua_State *, char *, tolua_Error*);


class gsScript
{
  private:
    int  testint;
    int  max_sum;
    char *script_buffer;                           // location of ingame script
    long int script_buffer_size;                   // size of buffer

    void sort_level_files();

  public:
    gsScript();
    ~gsScript();

    void  read_level_files(int=0, char *n="");

    int   get_testint()                             { return testint; }
    int   get_gcthreshold(Lua *);
    int   get_gccount(Lua *);
    void  set_gcthreshold(Lua *, int n);

    // functions
    void  setglobal(char *n, int i, Lua *l);
    void  setglobal(char *n, float i, Lua *l);
    void  setglobal(char *n, char *s, Lua *l);
    int   getglobal_int(char *n, Lua *l);
    float getglobal_float(char *n, Lua *l);
    const char *getglobal_string(char *n, Lua *l);
    void set_standard_globals(Lua *l);
    
    void load_game_config();                       // loads config file
    void load_level_init(char *n, level_info *m);  // run init file n and store vars in m
    void init_script_commands(Lua *, int newtable=1); // init commands for lua scripts
    void kill_script_commands(Lua *);              // kills the commands
    void do_init_script(Lua *, char *dir, char *n);       // executes init script
    void load_ingame_script(char *dir, char *n, char *o=NULL);// load init script into buffer
    int  init_ingame_script();                     // initializes script
    int  do_ingame_script(Lua *);                       // executes ingame script
    void clear_ingame_script();                    // clears ingame script
    void set_player_weapon(class gsPlayer *, int, int); // set weapon
    int  load_ship(class gsPlayer *, char *);      // loads ship definition from given file
    void do_string(char *cmd);                     // executes the given command

    void init_text(Lua *);                              // init language script
    void load_text();                              // load text strings from script

    void init_hooks(Lua *);
    int is_flag(int *f);
    int flag_hook_object_dead[MAX_HOOKS];
    int do_hook_object_dead(Lua *, class Object *);      // function called when object dead
    int flag_hook_object_update[MAX_HOOKS];
    int do_hook_object_update(Lua *, class Object *);    // function called when object moves
    int flag_hook_object_hit[MAX_HOOKS];
    int do_hook_object_hit(Lua *, class Object *o, int weap, int phitp, int fx, int fy);
    int flag_hook_object_with_object[MAX_HOOKS];
    int do_hook_object_with_object(Lua *, class Object *o1, class Object *o2);
    int flag_hook_player_dead[MAX_HOOKS];
    int do_hook_player_dead(Lua *, class gsPlayer *);    // function called when player dead
    int flag_hook_player_respawn[MAX_HOOKS];
    int do_hook_player_respawn(Lua *, class gsPlayer *);    // function called when player respawned
    int flag_hook_player_hit[MAX_HOOKS];
    int do_hook_player_hit(Lua *, class gsPlayer *, int weap, int phitp, float fxspd, float fyspd);     // function called when player hit
    int flag_hook_player_with_spobject[MAX_HOOKS];
    int do_hook_player_with_spobject(Lua *, class gsPlayer *, class Object *); // function called when player hits spobject
    int flag_hook_player_takes_cargo[MAX_HOOKS];
    int do_hook_player_takes_cargo(Lua *, class gsPlayer *, class Base *); // function called when player takes up cargo
    int flag_hook_player_returns_cargo[MAX_HOOKS];
    int do_hook_player_returns_cargo(Lua *, class gsPlayer *); // function called when player returns cargo
    int flag_hook_player_land[MAX_HOOKS];
    int do_hook_player_land(Lua *, class gsPlayer *, class Base *); // function called one time when player lands
    int flag_hook_init_level[MAX_HOOKS];
    int do_hook_init_level(Lua *); // function called on level init
    int flag_hook_exit_level[MAX_HOOKS];
    int do_hook_exit_level(Lua *); // function called on level exit
    int flag_hook_keyboard_hit[MAX_HOOKS];
    int do_hook_keyboard_hit(Lua *, int); // function called on keypress

    int flag_hook_enemy_hit[MAX_HOOKS];
    int do_hook_enemy_hit(Lua *, class Enemy *, int weapon, int hitp, int fx, int fy);
    int flag_hook_enemy_update[MAX_HOOKS];
    int do_hook_enemy_update(Lua *, class Enemy *);
    int flag_hook_enemy_dead[MAX_HOOKS];
    int do_hook_enemy_dead(Lua *, class Enemy *);
    int flag_hook_enemy_shoot[MAX_HOOKS];
    int do_hook_enemy_shoot(Lua *, class Enemy *);
    int flag_hook_player_with_enemy[MAX_HOOKS];
    int do_hook_player_with_enemy(Lua *, class gsPlayer *, class Enemy *);
    int flag_hook_enemy_with_enemy[MAX_HOOKS];
    int do_hook_enemy_with_enemy(Lua *, class Enemy *, class Enemy *);

    int flag_hook_bullet_destroyed[MAX_HOOKS];
    int do_hook_bullet_destroyed(Lua *, class Bullet *);
};

extern gsScript *script;

#endif
