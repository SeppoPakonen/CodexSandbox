/*
 *   GRAVITY STRIKE -- gsplcomp.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _GSPLCOMP_
#define _GSPLCOMP_

#include "luaclass.h"

class gsCompPlayer
{
  private:
    char plcomp_ini[100];                          // filename of ini file
    char plcomp_sc[100];                           // filename of sc file
    char *plcomp_script;                           // script to run on each loop
    int  script_size;                              // size of sc script
    Lua  *mylua;                                   // state to run in
    
    int frapela_playmap,
        schwierigkeit,
        frapela_waffenanzahl_sp2,
        frapela_waffenschacht,
        frapela_waffe_single,
        frapela_waffe_double,
        frapela_waffe_bomb,
        frapela_waffe_bigbomb,
        frapela_waffe_rocket,
        frapela_waffe_mine,
        frapela_waffe_freezer,
        frapela_waffe_shrinker,
        frapela_waffe_probe,
        frapela_waffe_gas,
        frapela_waffe_laser,
        frapela_waffe_bumper,
        frapela_waffenart[12],
        frapela_zufi_wand,
        frapela_wandzeit,
        frapela_aktion_zeit,
        frapela_probe_zeit,
        frapela_rocket_zeit,
        frapela_double_zeit,
        frapela_freezer_zeit,
        frapela_shrinker_zeit,
        frapela_mine_zeit,
        frapela_gas_zeit,
        frapela_bomb_zeit,
        frapela_laser_zeit,
        frapela_bumper_zeit;

  public:
    gsCompPlayer(Lua *, int num=2);
    ~gsCompPlayer();

    double lua_random(int U=0, int L=0);
    void load_script();
    void run_ini();
    void run_script();
    
    void run_compiled_ini();
    void run_compiled_script();
};

extern gsCompPlayer *comp_player[];

#endif
