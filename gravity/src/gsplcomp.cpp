/*
 *   GRAVITY STRIKE -- gsplcomp.cpp (computer enemy (c) by Frapela)
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#define PI (3.14159265358979323846)
#define RADIANS_PER_DEGREE (PI/180.0)
#define FROMRAD(a)	((a)/RADIANS_PER_DEGREE)
#define TORAD(a)	((a)*RADIANS_PER_DEGREE)

#include "luaclass.h"
#include "gsplcomp.h"
#include "gsglob.h"
#include "gserror.h"
#include "gsplayer.h"
#include "gsscript.h"
#include "gscontrl.h"
#include "gsobject.h"
#include "gsenemy.h"
#include "gsmap.h"
#include "gshelper.h"

#include <stdio.h>
#include <math.h>

gsCompPlayer::gsCompPlayer(Lua *l, int num)
{
  strcpy(plcomp_ini, "./levels/frapa_cg.ini");
  strcpy(plcomp_sc,  "./levels/frapa_cg.sc");
  
  mylua = l;

  plcomp_script = NULL;
  script_size = 0;
  load_script();

  script->setglobal("comp_player", num, l);
}

gsCompPlayer::~gsCompPlayer()
{
  delete[] plcomp_script;
  plcomp_script = NULL;
}

void gsCompPlayer::load_script()
{
  if (globals->use_script_computer_player)
  {
    script_size = file_size(plcomp_sc);
  
    if (script_size <= 0)
      errors->log(1, "computer player", "couldn't find necessary script files!");
  
    if (plcomp_script)
    {
      delete[] plcomp_script; plcomp_script = NULL;
    }
    plcomp_script = new char[script_size];
    memset(plcomp_script, 0, script_size);
  
    FILE *tmp = fopen(plcomp_sc, "rb");
    fread(plcomp_script, 1, script_size, tmp);
    fclose(tmp);
  }
}

void gsCompPlayer::run_ini()
{
  if (globals->use_script_computer_player)
  {
    int err = mylua->dofile(plcomp_ini);
    if (err) errors->log(1, "computer player", "error while script initialization", err);
    errors->log(2, "computer player", "running scripted (Lua) computer player");
  }
  else
  {
    errors->log(2, "computer player", "running compiled computer player");
    run_compiled_ini();
  }
}

void gsCompPlayer::run_script()
{
  if (globals->use_script_computer_player)
  {
    int err = mylua->dobuffer(plcomp_script, script_size, "computerplayer");
    if (err) errors->log(1, "computer player", "error while script execution", err);
  }
  else
  {
    run_compiled_script();
  }
}

double gsCompPlayer::lua_random(int U, int L)
{
  double r = (double)(gsrand()%RAND_MAX) / (double)RAND_MAX;

  if (L == 0)
  {
    return r;
  }
  else if (U == 0)
  {
    return (int)(r*U)+1;
  }
  else
  {
    return (int)(r*(U-L+1))+L;
  }
  
  return 1;
}

void gsCompPlayer::run_compiled_ini()
{
  /* original Lua code by Frapela */
  
  if (globals->onescreen_map == -1)
    frapela_playmap = 0;
  else
    frapela_playmap = 1;
    
  schwierigkeit = globals->computer_player_strength + 1;
    
  player[1]->controls->disable();
    
  frapela_waffenanzahl_sp2 = 0;
  frapela_waffenschacht    = 0;
  frapela_waffe_single     = -1;
  frapela_waffe_double     = -1;
  frapela_waffe_bomb       = -1;
  frapela_waffe_bigbomb    = -1;
  frapela_waffe_rocket     = -1;
  frapela_waffe_mine       = -1;
  frapela_waffe_freezer    = -1;
  frapela_waffe_shrinker   = -1;
  frapela_waffe_probe      = -1;
  frapela_waffe_gas        = -1;
  frapela_waffe_laser      = -1;
  frapela_waffe_bumper     = -1;

  for (int i=0; i <= player[1]->get_avail_weapon_slots(); i++)
  {
    frapela_waffenanzahl_sp2 = frapela_waffenanzahl_sp2 + 1;
    frapela_waffenart[i] = player[1]->get_weapon_slot(i)->type;
    
    if      (frapela_waffenart[i] == W_SINGLE)
      frapela_waffe_single  = i;
    else if (frapela_waffenart[i] == W_DOUBLE)
      frapela_waffe_double  = i;
    else if (frapela_waffenart[i] == W_BOMB)
      frapela_waffe_bomb    = i;
    else if (frapela_waffenart[i] == W_BIGBOMB)
      frapela_waffe_bigbomb = i;
    else if (frapela_waffenart[i] == W_ROCKET)
      frapela_waffe_rocket  = i;
    else if (frapela_waffenart[i] == W_MINE)
      frapela_waffe_mine    = i;
    else if (frapela_waffenart[i] == W_FREEZER)
      frapela_waffe_freezer = i;
    else if (frapela_waffenart[i] == W_SHRINKER)
      frapela_waffe_shrinker= i;
    else if (frapela_waffenart[i] == W_PROBE)
      frapela_waffe_probe   = i;
    else if (frapela_waffenart[i] == W_GAS)
      frapela_waffe_gas     = i;
    else if (frapela_waffenart[i] == W_LASER)
      frapela_waffe_laser   = i;
    else if (frapela_waffenart[i] == W_BUMPER)
      frapela_waffe_bumper  = i;
    
    if      (schwierigkeit == 11)
      player[1]->set_weapon( i, frapela_waffenart[i],                                      -1,                                     -1 );
    else if (schwierigkeit == 10)
      player[1]->set_weapon( i, frapela_waffenart[i], player[1]->get_weapon_slot(i)->bullets*10, player[1]->get_weapon_slot(i)->reload/ 5 );
    else if (schwierigkeit ==  9)
      player[1]->set_weapon( i, frapela_waffenart[i], player[1]->get_weapon_slot(i)->bullets* 3, player[1]->get_weapon_slot(i)->reload/ 2 );
    else if (schwierigkeit ==  8)
      player[1]->set_weapon( i, frapela_waffenart[i], player[1]->get_weapon_slot(i)->bullets*10, player[1]->get_weapon_slot(i)->reload    );
    else if (schwierigkeit ==  7)
      player[1]->set_weapon( i, frapela_waffenart[i], player[1]->get_weapon_slot(i)->bullets* 3, player[1]->get_weapon_slot(i)->reload    );
    else if (schwierigkeit ==  6)
      player[1]->set_weapon( i, frapela_waffenart[i], player[1]->get_weapon_slot(i)->bullets   , player[1]->get_weapon_slot(i)->reload    );
    else if (schwierigkeit ==  5)
      player[1]->set_weapon( i, frapela_waffenart[i], player[1]->get_weapon_slot(i)->bullets/ 3, player[1]->get_weapon_slot(i)->reload    );
    else if (schwierigkeit ==  4)
      player[1]->set_weapon( i, frapela_waffenart[i], player[1]->get_weapon_slot(i)->bullets/10, player[1]->get_weapon_slot(i)->reload    );
    else if (schwierigkeit ==  3)
      player[1]->set_weapon( i, frapela_waffenart[i], player[1]->get_weapon_slot(i)->bullets/ 3, player[1]->get_weapon_slot(i)->reload* 2 );
    else if (schwierigkeit ==  2)
      player[1]->set_weapon( i, frapela_waffenart[i], player[1]->get_weapon_slot(i)->bullets/10, player[1]->get_weapon_slot(i)->reload* 5 );
    else if (schwierigkeit ==  1)
      player[1]->set_weapon( i, frapela_waffenart[i],                                       0,                                      0 );
  } // for
  
  frapela_zufi_wand     = 0;
  frapela_wandzeit      = 0;
  frapela_aktion_zeit   = 0;

  frapela_probe_zeit    = 0;
  frapela_rocket_zeit   = 0;
  frapela_double_zeit   = 0;
  frapela_freezer_zeit  = 0;
  frapela_shrinker_zeit = 0;
  frapela_mine_zeit     = 0;
  frapela_gas_zeit      = 0;
  frapela_bomb_zeit     = 0;
  frapela_laser_zeit    = 0;
  frapela_bumper_zeit   = 0;
}

void gsCompPlayer::run_compiled_script()
{
  /* original Lua code by Frapela */
  
  // Variablen updaten
  float frapela_spieler2_x       = player[1]->get_x() + (player[1]->get_width()/2);
  float frapela_spieler2_y       = player[1]->get_y() + (player[1]->get_height()/2);
  float frapela_spieler2_xgeschw = player[1]->get_xspd();
  float frapela_spieler2_ygeschw = player[1]->get_yspd();
  float frapela_spieler1_x       = player[0]->get_x() + (player[0]->get_width()/2);
  float frapela_spieler1_y       = player[0]->get_y() + (player[0]->get_height()/2);
  float frapela_spieler1_xgeschw = player[0]->get_xspd();
  float frapela_spieler1_ygeschw = player[0]->get_yspd();
  float frapela_spieler_dist     = sqrt( ((frapela_spieler2_x-frapela_spieler1_x)*(frapela_spieler2_x-frapela_spieler1_x)) + ((frapela_spieler2_y-frapela_spieler1_y)*(frapela_spieler2_y-frapela_spieler1_y)) );

  int   frapela_spobj_anz        = 0;
  float frapela_alt_spobj_dist   = 600;
  float frapela_alt_gegner_dist  = 600;
  int   frapela_spobj_nr[1000]    = { 0 };
  int   frapela_spobj_typ[1000]   = { 0 };
  float frapela_spobj_da         = 0;
  float frapela_spobj_da_zufi    = 0;
  float frapela_gegner_da        = 0;
  float frapela_gegner_da_zufi   = 0;
  
  float frapela_waffenzeit, frapela_schusswinkel, frapela_multiplikator, frapela_schussanpassung;
  Object *frapela_next_spobj, *frapela_objekt, *frapela_next_obj;
  Enemy *frapela_enemy, *frapela_next_enemy;
  float frapela_neu_spobj_x, frapela_neu_spobj_y, frapela_neu_spobj_dist;
  float frapela_alt_spobj_x, frapela_alt_spobj_y, frapela_alt_spobj_winkel, frapela_multi_spobj;
  float frapela_obj_x1, frapela_obj_y1, frapela_obj_x2, frapela_obj_y2;
  float frapela_neu_gegner_x, frapela_neu_gegner_y, frapela_neu_gegner_dist;
  float frapela_alt_gegner_x, frapela_alt_gegner_y, frapela_alt_gegner_winkel;
  float frapela_gegner_xgeschw, frapela_gegner_ygeschw;
  float frapela_gegner_multiplikator, frapela_gegner_schussanpassung;
  float frapela_geschw_multi = 45;

  if (schwierigkeit <= 7)
  {
    frapela_probe_zeit    = frapela_probe_zeit    - player[1]->get_weapon_slot(frapela_waffe_probe)->reload;
    frapela_rocket_zeit   = frapela_rocket_zeit   - player[1]->get_weapon_slot(frapela_waffe_rocket)->reload;
    frapela_double_zeit   = frapela_double_zeit   - player[1]->get_weapon_slot(frapela_waffe_double)->reload;
    frapela_freezer_zeit  = frapela_freezer_zeit  - player[1]->get_weapon_slot(frapela_waffe_freezer)->reload;
    frapela_shrinker_zeit = frapela_shrinker_zeit - player[1]->get_weapon_slot(frapela_waffe_shrinker)->reload;
    frapela_mine_zeit     = frapela_mine_zeit     - player[1]->get_weapon_slot(frapela_waffe_mine)->reload;
    frapela_gas_zeit      = frapela_gas_zeit      - player[1]->get_weapon_slot(frapela_waffe_gas)->reload;
    frapela_laser_zeit    = frapela_laser_zeit    - player[1]->get_weapon_slot(frapela_waffe_laser)->reload;
    frapela_bumper_zeit   = frapela_bumper_zeit   - player[1]->get_weapon_slot(frapela_waffe_bumper)->reload;
  }
  
  
  // Programm
  if (player[1]->get_freezed() == 0)
  {
    if (frapela_wandzeit + frapela_zufi_wand <= globals->game_time)
    {
      if (player[1]->get_fuel() < 1) player[1]->set_fuel( player[1]->get_fuel() + 99 );

      frapela_waffenzeit = frapela_spieler_dist / (schwierigkeit * 2 - 1);
      if (frapela_waffenzeit < 1) frapela_waffenzeit = 1;
      
      if (globals->game_time % (int)frapela_waffenzeit == 0)
      {
        if      ((frapela_waffe_probe >= 0) && (player[1]->get_weapon_slot(frapela_waffe_probe)->bullets != 0) && (frapela_probe_zeit + player[1]->get_weapon_slot(frapela_waffe_probe)->reload < globals->game_time))
        {
          frapela_waffenschacht = frapela_waffe_probe;
          frapela_probe_zeit    = globals->game_time;
        }
        else if ((frapela_waffe_rocket >= 0) && (player[1]->get_weapon_slot(frapela_waffe_rocket)->bullets != 0) && (frapela_rocket_zeit + player[1]->get_weapon_slot(frapela_waffe_rocket)->reload < globals->game_time))
        {
          frapela_waffenschacht = frapela_waffe_rocket;
          frapela_rocket_zeit   = globals->game_time;
        }
        else if ((frapela_waffe_double >= 0) && (player[1]->get_weapon_slot(frapela_waffe_double)->bullets != 0) && (frapela_double_zeit + player[1]->get_weapon_slot(frapela_waffe_double)->reload < globals->game_time))
        {
          frapela_waffenschacht = frapela_waffe_double;
          frapela_double_zeit   = globals->game_time;
        }
        else if ((frapela_waffe_freezer >= 0) && (player[1]->get_weapon_slot(frapela_waffe_freezer)->bullets != 0) && (frapela_freezer_zeit + player[1]->get_weapon_slot(frapela_waffe_freezer)->reload < globals->game_time))
        {
          frapela_waffenschacht = frapela_waffe_freezer;
          frapela_freezer_zeit  = globals->game_time;
        }
        else if ((frapela_waffe_shrinker >= 0) && (player[1]->get_weapon_slot(frapela_waffe_shrinker)->bullets != 0) && (frapela_shrinker_zeit + player[1]->get_weapon_slot(frapela_waffe_shrinker)->reload < globals->game_time))
        {
          frapela_waffenschacht = frapela_waffe_shrinker;
          frapela_shrinker_zeit = globals->game_time;
        }
        else if ((frapela_waffe_mine >= 0) && (player[1]->get_weapon_slot(frapela_waffe_mine)->bullets != 0) && (frapela_mine_zeit + player[1]->get_weapon_slot(frapela_waffe_mine)->reload < globals->game_time))
        {
          frapela_waffenschacht = frapela_waffe_mine;
          frapela_mine_zeit     = globals->game_time;
        }
        else if ((frapela_waffe_gas >= 0) && (player[1]->get_weapon_slot(frapela_waffe_gas)->bullets != 0) && (frapela_gas_zeit + player[1]->get_weapon_slot(frapela_waffe_gas)->reload < globals->game_time))
        {
          frapela_waffenschacht = frapela_waffe_gas;
          frapela_gas_zeit      = globals->game_time;
        }
        else if ((frapela_waffe_laser >= 0) && (player[1]->get_weapon_slot(frapela_waffe_laser)->bullets != 0) && (frapela_gas_zeit + player[1]->get_weapon_slot(frapela_waffe_laser)->reload < globals->game_time))
        {
          frapela_waffenschacht = frapela_waffe_laser;
          frapela_gas_zeit      = globals->game_time;
        }
        else if ((frapela_waffe_bumper >= 0) && (player[1]->get_weapon_slot(frapela_waffe_bumper)->bullets != 0) && (frapela_gas_zeit + player[1]->get_weapon_slot(frapela_waffe_bumper)->reload < globals->game_time))
        {
          frapela_waffenschacht = frapela_waffe_bumper;
          frapela_gas_zeit      = globals->game_time;
        }
        else
        {
          if (frapela_waffe_single  >= 0)
            frapela_waffenschacht = frapela_waffe_single;
          else
            frapela_waffenschacht = (int)lua_random(frapela_waffenanzahl_sp2) - 1;
        } // globals->game_time % frapela_waffenzeit

      
        if (frapela_waffe_bigbomb >= 0)
          if (player[1]->get_weapon_slot(frapela_waffe_bigbomb)->bullets != 0)
            if (frapela_spieler2_y - frapela_spieler1_y < 0)
              if (frapela_spieler2_x - ( (frapela_spieler1_y - frapela_spieler2_y) * ((frapela_spieler1_xgeschw - frapela_spieler2_xgeschw)/2) ) <= frapela_spieler1_x + 10)
                if (frapela_spieler2_x - ( (frapela_spieler1_y - frapela_spieler2_y) * ((frapela_spieler1_xgeschw - frapela_spieler2_xgeschw)/2) ) >= frapela_spieler1_x - 10)
                  frapela_waffenschacht = frapela_waffe_bigbomb;
      
        if (frapela_spieler_dist < 100)
          if (frapela_waffe_bomb >= 0)
            if (player[1]->get_weapon_slot(frapela_waffe_bomb)->bullets != 0)
              if (frapela_bomb_zeit + player[1]->get_weapon_slot(frapela_waffe_bomb)->reload < globals->game_time)
              {
                frapela_bomb_zeit = globals->game_time;
                frapela_waffenschacht = frapela_waffe_bomb;
              }
              
        if ((player[0]->get_freezed() > 0) || (player[0]->get_shrinked() > 0))
          if (frapela_waffe_single  >= 0)
            frapela_waffenschacht = frapela_waffe_single;

        player[1]->set_active_weapon( frapela_waffenschacht );
      
      } // if globals->game_time % waffenzeit
      

      frapela_schusswinkel = FROMRAD(asin( (frapela_spieler2_x-frapela_spieler1_x) / frapela_spieler_dist ));
      if (fabs(frapela_schusswinkel) < 45)
        frapela_multiplikator = fabs(frapela_schusswinkel) / 6;
      else
        frapela_multiplikator = fabs((fabs(frapela_schusswinkel)-90)) / 6;
      
      if ( ( frapela_spieler_dist > 50 ) && ( player[1]->get_shrinked() == 0 ))
      {
        if (frapela_spieler2_y - frapela_spieler1_y < 0)
        {
          if (frapela_spieler2_x - frapela_spieler1_x < 0)
            frapela_schussanpassung = (frapela_multiplikator * ((-1)*frapela_spieler1_xgeschw + frapela_spieler2_xgeschw)) + (frapela_multiplikator * (frapela_spieler1_ygeschw - frapela_spieler2_ygeschw));
          else
            frapela_schussanpassung = (frapela_multiplikator * ((-1)*frapela_spieler1_xgeschw + frapela_spieler2_xgeschw)) + (frapela_multiplikator * (frapela_spieler2_ygeschw - frapela_spieler1_ygeschw));
          
          player[1]->set_head( 180 + frapela_schusswinkel + frapela_schussanpassung);
        }
        else
        {
          if (frapela_spieler2_x - frapela_spieler1_x < 0)
            frapela_schussanpassung = (frapela_multiplikator * (frapela_spieler1_xgeschw + ((-1)*frapela_spieler2_xgeschw))) + (frapela_multiplikator * (frapela_spieler1_ygeschw - frapela_spieler2_ygeschw));
          else
            frapela_schussanpassung = (frapela_multiplikator * ((-1)*frapela_spieler2_xgeschw + frapela_spieler1_xgeschw)) + (frapela_multiplikator * ((-1)*frapela_spieler1_ygeschw + frapela_spieler2_ygeschw));
          
          if (360 - frapela_schusswinkel + frapela_schussanpassung <= 360)
            player[1]->set_head( 360 - frapela_schusswinkel + frapela_schussanpassung );
          else
            player[1]->set_head(   0 - frapela_schusswinkel + frapela_schussanpassung );
        }
        
        if (frapela_spieler_dist < 600)
        {
          frapela_aktion_zeit = globals->game_time;
          if (globals->game_time % (int)frapela_waffenzeit == 0)
          {
            if ((player[0]->get_dead() == 0) && (player[1]->get_dead() == 0))
              player[1]->shoot();
          }
        }
      }
      
      else if (( player[0]->get_shrinked() == 0 ) || ( player[1]->get_shrinked() > 0 ))
      {
        if (frapela_spieler2_y - frapela_spieler1_y < 0)
        {
          player[1]->set_head( frapela_schusswinkel );
          if (frapela_spieler_dist < 130)
            player[1]->set_thrust();
        }
        else
        {
          player[1]->set_head( 180 - frapela_schusswinkel );
          if (frapela_spieler_dist < 130)
            player[1]->set_thrust();
        }
      }
      
      if (lua_random() < 0.2)
        player[1]->set_thrust();
      else if (player[0]->get_shrinked() > 0)
        if (lua_random() < 0.8) player[1]->set_thrust();


      // spobjects
      Object *frapela_spobjekt = objects->get_first_spobject();
      
      while (frapela_spobjekt)
      {
        frapela_next_spobj = frapela_spobjekt->get_next();
        frapela_spobj_anz = frapela_spobj_anz + 1;
        frapela_spobj_nr[frapela_spobj_anz]  = frapela_spobjekt->get_nr();
        frapela_spobj_typ[frapela_spobj_anz] = frapela_spobjekt->get_type();
        
        if ((frapela_spobj_typ[frapela_spobj_anz] == 505) || (frapela_spobj_typ[frapela_spobj_anz] == 0))
        {
          frapela_neu_spobj_x      = frapela_spobjekt->get_x() + (frapela_spobjekt->get_width()/2);
          frapela_neu_spobj_y      = frapela_spobjekt->get_y() + (frapela_spobjekt->get_height()/2);
          frapela_neu_spobj_dist   = sqrt( ((frapela_spieler2_x-frapela_neu_spobj_x)*(frapela_spieler2_x-frapela_neu_spobj_x)) + ((frapela_spieler2_y-frapela_neu_spobj_y)*(frapela_spieler2_y-frapela_neu_spobj_y)) );
          
          if (frapela_neu_spobj_dist < frapela_alt_spobj_dist)
          {
            if (frapela_spobj_typ[frapela_spobj_anz] == 505)
              frapela_spobj_da       = 505;
            else
              frapela_spobj_da       =   1;
            
            frapela_alt_spobj_x      = frapela_neu_spobj_x;
            frapela_alt_spobj_y      = frapela_neu_spobj_y;
            frapela_alt_spobj_dist   = frapela_neu_spobj_dist;
            frapela_alt_spobj_winkel = FROMRAD(asin( (frapela_spieler2_x-frapela_alt_spobj_x)/(frapela_alt_spobj_dist) ));
          }
        }
        
        frapela_spobjekt = frapela_next_spobj;
      }
      
      if (( frapela_spobj_da == 0 ) && ( frapela_spieler_dist > 600 ))
      {
        frapela_spobjekt = objects->get_first_spobject();
        
        while (frapela_spobjekt)
        {
          frapela_next_spobj = frapela_spobjekt->get_next();
          frapela_spobj_anz = frapela_spobj_anz + 1;
          frapela_spobj_nr[frapela_spobj_anz]  = frapela_spobjekt->get_nr();
          frapela_spobj_typ[frapela_spobj_anz] = frapela_spobjekt->get_type();
            if (frapela_spobj_typ[frapela_spobj_anz] == 506)
            {
              frapela_neu_spobj_x      = frapela_spobjekt->get_x() + (frapela_spobjekt->get_width()/2);
              frapela_neu_spobj_y      = frapela_spobjekt->get_y() + (frapela_spobjekt->get_height()/2);
              frapela_neu_spobj_dist   = sqrt( (frapela_spieler2_x-frapela_neu_spobj_x)*(frapela_spieler2_x-frapela_neu_spobj_x) + (frapela_spieler2_y-frapela_neu_spobj_y)*(frapela_spieler2_y-frapela_neu_spobj_y) );
              
              if (frapela_neu_spobj_dist < frapela_alt_spobj_dist)
              {
                frapela_spobj_da         = 506;
                frapela_alt_spobj_x      = frapela_neu_spobj_x;
                frapela_alt_spobj_y      = frapela_neu_spobj_y;
                frapela_alt_spobj_dist   = frapela_neu_spobj_dist;
                frapela_alt_spobj_winkel = FROMRAD(asin( (frapela_spieler2_x-frapela_alt_spobj_x)/(frapela_alt_spobj_dist) ));
              }
            }
          frapela_spobjekt = frapela_next_spobj;
        }
      }
      
      if (frapela_alt_spobj_dist < 600)
      {
        if (fabs(frapela_alt_spobj_winkel) < 45)
          frapela_multi_spobj = fabs(frapela_alt_spobj_winkel) * ( (600-frapela_alt_spobj_dist)/800 );
        else
          frapela_multi_spobj = fabs( (fabs(frapela_alt_spobj_winkel) - 90) ) * ( (600-frapela_alt_spobj_dist)/800 );
        
        if (frapela_spieler2_y - frapela_alt_spobj_y < 0)
        {
          if (frapela_spieler2_x - frapela_alt_spobj_x < 0)
            frapela_alt_spobj_winkel = frapela_alt_spobj_winkel + (frapela_multi_spobj*frapela_spieler2_xgeschw) - (frapela_multi_spobj*frapela_spieler2_ygeschw);
          else
            frapela_alt_spobj_winkel = frapela_alt_spobj_winkel + (frapela_multi_spobj*frapela_spieler2_xgeschw) + (frapela_multi_spobj*frapela_spieler2_ygeschw);
          
          player[1]->set_head( 180 + frapela_alt_spobj_winkel );
        }
        else
        {
          if (frapela_spieler2_x - frapela_alt_spobj_x < 0)
            frapela_alt_spobj_winkel = frapela_alt_spobj_winkel + (frapela_multi_spobj*frapela_spieler2_xgeschw) + (frapela_multi_spobj*frapela_spieler2_ygeschw);
          else
            frapela_alt_spobj_winkel = frapela_alt_spobj_winkel + (frapela_multi_spobj*frapela_spieler2_xgeschw) - (frapela_multi_spobj*frapela_spieler2_ygeschw);
          
          if (frapela_alt_spobj_winkel > 0)
            player[1]->set_head( 360 - frapela_alt_spobj_winkel );
          else
            player[1]->set_head( 0   - frapela_alt_spobj_winkel );
          
        }
        
        if (lua_random() < 0.6)
          player[1]->set_thrust();
        
      }
      
      frapela_spobj_da_zufi = frapela_spobj_da;


      // enemies
      frapela_enemy = enemies->get_first_enemy();
      
      while (frapela_enemy)
      {
        frapela_next_enemy = frapela_enemy->get_next();
        frapela_neu_gegner_x    = frapela_enemy->get_x() + (frapela_enemy->get_width()/2);
        frapela_neu_gegner_y    = frapela_enemy->get_y() + (frapela_enemy->get_height()/2);
        frapela_neu_gegner_dist = sqrt( (frapela_spieler2_x-frapela_neu_gegner_x)*(frapela_spieler2_x-frapela_neu_gegner_x) + (frapela_spieler2_y-frapela_neu_gegner_y)*(frapela_spieler2_y-frapela_neu_gegner_y) );
        
        if (frapela_neu_gegner_dist < frapela_alt_gegner_dist)
        {
          frapela_gegner_da         = 1;
          frapela_alt_gegner_x      = frapela_neu_gegner_x;
          frapela_alt_gegner_y      = frapela_neu_gegner_y;
          frapela_alt_gegner_dist   = frapela_neu_gegner_dist;
          frapela_alt_gegner_winkel = FROMRAD(asin( (frapela_spieler2_x-frapela_alt_gegner_x)/(frapela_alt_gegner_dist) ));
          frapela_gegner_xgeschw    = frapela_enemy->get_xspd();
          frapela_gegner_ygeschw    = frapela_enemy->get_yspd();
        }
        
        frapela_enemy = frapela_next_enemy;
      }
      
      if (frapela_alt_gegner_dist < 600)
      {
        if (frapela_alt_gegner_dist < 50 + ( ( fabs(frapela_spieler2_xgeschw + frapela_spieler2_ygeschw) ) * 50 ))
        {
          if (frapela_spieler2_y - frapela_alt_gegner_y < 0)
          {
            player[1]->set_head( frapela_alt_gegner_winkel );
            frapela_wandzeit = globals->game_time;
            frapela_zufi_wand = 20 + (int)lua_random(10);
          }
          else
          {
            player[1]->set_head( 180 - frapela_alt_gegner_winkel );
            frapela_wandzeit = globals->game_time; 
            frapela_zufi_wand = 10 + (int)lua_random(10);
          }
        }
        
        else if (frapela_alt_gegner_dist < 600)
        {
          if (frapela_spieler_dist > 600)
          {
            if (frapela_alt_spobj_dist == 600)
            {
              if (frapela_waffe_single  >= 0)
                player[1]->set_active_weapon( frapela_waffe_single );
              
              if (fabs( frapela_alt_gegner_winkel ) <= 45)
                frapela_gegner_multiplikator = 9;
              else
                frapela_gegner_multiplikator = 9;
              
              if (frapela_spieler2_y - frapela_alt_gegner_y < 0)
              {
                if (frapela_spieler2_x - frapela_alt_gegner_x < 0)
                  frapela_gegner_schussanpassung = (frapela_gegner_multiplikator * ((-1)*frapela_gegner_xgeschw + frapela_spieler2_xgeschw)) + (frapela_gegner_multiplikator * (frapela_gegner_ygeschw - frapela_spieler2_ygeschw));
                else
                  frapela_gegner_schussanpassung = (frapela_gegner_multiplikator * ((-1)*frapela_gegner_xgeschw + frapela_spieler2_xgeschw)) + (frapela_gegner_multiplikator * (frapela_spieler2_ygeschw - frapela_gegner_ygeschw));
                
                player[1]->set_head( 180 + frapela_alt_gegner_winkel + frapela_gegner_schussanpassung);
              }
              else
              {
                if (frapela_spieler2_x - frapela_alt_gegner_x < 0)
                  frapela_gegner_schussanpassung = (frapela_gegner_multiplikator * (frapela_gegner_xgeschw + ((-1)*frapela_spieler2_xgeschw))) + (frapela_gegner_multiplikator * (frapela_gegner_ygeschw - frapela_spieler2_ygeschw));
                else
                  frapela_gegner_schussanpassung = (frapela_gegner_multiplikator * ((-1)*frapela_spieler2_xgeschw + frapela_gegner_xgeschw)) + (frapela_gegner_multiplikator * ((-1)*frapela_gegner_ygeschw + frapela_spieler2_ygeschw));
                
                if (360 - frapela_alt_gegner_winkel + frapela_gegner_schussanpassung <= 360)
                  player[1]->set_head( 360 - frapela_alt_gegner_winkel + frapela_gegner_schussanpassung );
                else
                  player[1]->set_head(   0 - frapela_alt_gegner_winkel + frapela_gegner_schussanpassung );
              }
              
              if (globals->game_time % 8 == 0)
              {
                if (frapela_alt_gegner_dist < 200)
                {
                  frapela_aktion_zeit = globals->game_time;
                  player[1]->shoot();
                }
              }
            }
          }
        }
      }
      
      frapela_gegner_da_zufi = frapela_gegner_da;
            

      // objects
      frapela_objekt = objects->get_first_object();
      
      while (frapela_objekt)
      {
        frapela_next_obj = frapela_objekt->get_next();
        frapela_obj_x1   = frapela_objekt->get_x();
        frapela_obj_y1   = frapela_objekt->get_y();
        if (sqrt( (frapela_spieler2_x-frapela_obj_x1)*(frapela_spieler2_x-frapela_obj_x1) + (frapela_spieler2_y-frapela_obj_y1)*(frapela_spieler2_y-frapela_obj_y1) ) < 300)
        {
          frapela_obj_x2 = frapela_objekt->get_x() + frapela_objekt->get_width();
          frapela_obj_y2 = frapela_objekt->get_y() + frapela_objekt->get_height();
          
          if     (( frapela_obj_x2 < frapela_spieler2_x ) && ( frapela_obj_y1 > frapela_spieler2_y ) && ( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi) < frapela_obj_x2 ) && ( frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) > frapela_obj_y1 ))
          {
              player[1]->set_head(45);
              frapela_wandzeit = globals->game_time;
              frapela_zufi_wand = 20 + (int)lua_random(20);
          }
          else if (( frapela_obj_x1 > frapela_spieler2_x ) && ( frapela_obj_y1 > frapela_spieler2_y ) && ( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi) > frapela_obj_x1 ) && ( frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) > frapela_obj_y1 ))
          {
              player[1]->set_head(315);
              frapela_wandzeit = globals->game_time;
              frapela_zufi_wand = 20 + (int)lua_random(20);
          }
          else if (( frapela_obj_x2 < frapela_spieler2_x ) && ( frapela_obj_y2 < frapela_spieler2_y ) && ( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi) < frapela_obj_x2 ) && ( frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) < frapela_obj_y2 ))
          {
              player[1]->set_head(135);
              frapela_wandzeit = globals->game_time;
              frapela_zufi_wand = 10 + (int)lua_random(10);
          }
          else if (( frapela_obj_x1 > frapela_spieler2_x ) && ( frapela_obj_y2 < frapela_spieler2_y ) && ( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi) > frapela_obj_x1 ) && ( frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) < frapela_obj_y2 ))
          {
              player[1]->set_head(225);
              frapela_wandzeit = globals->game_time;
              frapela_zufi_wand = 10 + (int)lua_random(10);
          }
          else if (( frapela_obj_x1 < frapela_spieler2_x ) && ( frapela_obj_x2 > frapela_spieler2_x ) && ( frapela_obj_y1 > frapela_spieler2_y ) && ( frapela_obj_y1 < (frapela_spieler2_y+3) + (frapela_spieler2_ygeschw*frapela_geschw_multi) ))
          {
              player[1]->set_head(  0);
              frapela_wandzeit = globals->game_time;
              frapela_zufi_wand = 20 + (int)lua_random(30);
          }
          else if (( frapela_obj_y1 < frapela_spieler2_y ) && ( frapela_obj_y2 > frapela_spieler2_y ) && ( frapela_obj_x2 < frapela_spieler2_x ) && ( frapela_obj_x2 > (frapela_spieler2_x-3) + (frapela_spieler2_xgeschw*frapela_geschw_multi) ))
          {
              player[1]->set_head( 90);
              frapela_wandzeit = globals->game_time;
              frapela_zufi_wand = 10 + (int)lua_random(20);
          }
          else if (( frapela_obj_y1 < frapela_spieler2_y ) && ( frapela_obj_y2 > frapela_spieler2_y ) && ( frapela_obj_x1 > frapela_spieler2_x ) && ( frapela_obj_x1 < (frapela_spieler2_x+3) + (frapela_spieler2_xgeschw*frapela_geschw_multi) ))
          {
              player[1]->set_head(270);
              frapela_wandzeit = globals->game_time;
              frapela_zufi_wand = 10 + (int)lua_random(20);
          }
          else if (( frapela_obj_x1 < frapela_spieler2_x ) && ( frapela_obj_x2 > frapela_spieler2_x ) && ( frapela_obj_y2 < frapela_spieler2_y ) && ( frapela_obj_y2 > (frapela_spieler2_y-3) + (frapela_spieler2_ygeschw*frapela_geschw_multi) ))
          {
              player[1]->set_head(180);
              frapela_wandzeit = globals->game_time;
              frapela_zufi_wand = 10 + (int)lua_random(7);
          }
        }
        frapela_objekt = frapela_next_obj;
      }      
           
      
    } // wandzeit < game_time
    
    
    // rest (??)
    float frapela_u     = 0;
    float frapela_ul    = 0;
    float frapela_l     = 0;
    float frapela_ol    = 0;
    float frapela_o     = 0;
    float frapela_or    = 0;
    float frapela_r     = 0;
    float frapela_ur    = 0;
    float frapela_ul_ol = 0;
    float frapela_ol_or = 0;
    float frapela_or_ur = 0;
    float frapela_ur_ul = 0;
    
    if (    ( playmap[frapela_playmap]->is_pixel( frapela_spieler2_x, (frapela_spieler2_y+3) + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) && ( frapela_spieler2_ygeschw > 0 ))
      frapela_u  = 1;
    else if (( playmap[frapela_playmap]->is_pixel( (frapela_spieler2_x-3) + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y) == 1 ) && ( frapela_spieler2_xgeschw < 0 ))
      frapela_l  = 1;
    else if (( playmap[frapela_playmap]->is_pixel( (frapela_spieler2_x+3) + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y) == 1 ) && ( frapela_spieler2_xgeschw > 0 ) )
      frapela_r  = 1;
    else if (( playmap[frapela_playmap]->is_pixel( frapela_spieler2_x, (frapela_spieler2_y-3) + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) && ( frapela_spieler2_ygeschw < 0 ) )
      frapela_o  = 1;
    
    if (    ( playmap[frapela_playmap]->is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) && ( playmap[frapela_playmap]->is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) && ( frapela_spieler2_ygeschw > 0 ) )
      frapela_ur_ul = 1;
    else if (( playmap[frapela_playmap]->is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) && ( playmap[frapela_playmap]->is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) && ( frapela_spieler2_xgeschw < 0 ) )
      frapela_ul_ol = 1;
    else if (( playmap[frapela_playmap]->is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) && ( playmap[frapela_playmap]->is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) && ( frapela_spieler2_xgeschw > 0 ) )
      frapela_or_ur = 1;
    else if (( playmap[frapela_playmap]->is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) && ( playmap[frapela_playmap]->is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) && ( frapela_spieler2_ygeschw < 0 ) )
      frapela_ol_or = 1;
    else if (( playmap[frapela_playmap]->is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) && ( frapela_spieler2_xgeschw < 0 ) && ( frapela_spieler2_ygeschw > 0 ) )
      frapela_ul = 1;
    else if (( playmap[frapela_playmap]->is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) && ( frapela_spieler2_xgeschw > 0 ) && ( frapela_spieler2_ygeschw > 0 ) )
      frapela_ur = 1;
    else if (( playmap[frapela_playmap]->is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) && ( frapela_spieler2_xgeschw < 0 ) && ( frapela_spieler2_ygeschw < 0 ) )
      frapela_ol = 1;
    else if (( playmap[frapela_playmap]->is_pixel( frapela_spieler2_x + (frapela_spieler2_xgeschw*frapela_geschw_multi), frapela_spieler2_y + (frapela_spieler2_ygeschw*frapela_geschw_multi) ) == 1 ) && ( frapela_spieler2_xgeschw > 0 ) && ( frapela_spieler2_ygeschw < 0 ) )
      frapela_or = 1;
    
    if (    ( frapela_ur_ul == 1 ) && ( frapela_l + frapela_r == 0 ) )
    {
      if (lua_random() < 0.5 )
        player[1]->set_head( lua_random(5) );
      else
        player[1]->set_head(360 - lua_random(5));
      
      frapela_wandzeit  = globals->game_time;
      if (frapela_aktion_zeit + 180 > globals->game_time )
        frapela_zufi_wand = 20 + (int)lua_random(40);
      else
      {
        frapela_aktion_zeit = globals->game_time - 120;
        frapela_zufi_wand = 120;
      }
    }
    
    else if (( frapela_ul_ol == 1 ) && ( frapela_u + frapela_o == 0 ) )
    {
      player[1]->set_head( 95 - lua_random(10));
      frapela_wandzeit  = globals->game_time;
      if (frapela_aktion_zeit + 180 > globals->game_time ) 
        frapela_zufi_wand = 20 + (int)lua_random(20);
      else
      {
        frapela_aktion_zeit = globals->game_time - 120;
        frapela_zufi_wand = 120;
      }
    }
    else if (( frapela_or_ur == 1 ) && ( frapela_u + frapela_o == 0 ) )
    {
      player[1]->set_head(275 - lua_random(10));
      frapela_wandzeit  = globals->game_time;
      if (frapela_aktion_zeit + 180 > globals->game_time ) 
        frapela_zufi_wand = 20 + (int)lua_random(20);
      else
      {
        frapela_aktion_zeit = globals->game_time - 120;
        frapela_zufi_wand = 120;
      }
    }
    else if (( frapela_ol_or == 1 ) && ( frapela_l + frapela_r == 0 ) )
    {
      player[1]->set_head(185 - lua_random(10));
      frapela_wandzeit  = globals->game_time;
      if (frapela_aktion_zeit + 180 > globals->game_time ) 
        frapela_zufi_wand = 10 + (int)lua_random(10);
      else
      {
        frapela_aktion_zeit = globals->game_time - 120;
        frapela_zufi_wand = 120;
      }
    }
    else if (frapela_ul == 1 )
    {
      player[1]->set_head( 50 - lua_random(10));
      frapela_wandzeit  = globals->game_time;
      if (frapela_aktion_zeit + 180 > globals->game_time ) 
        frapela_zufi_wand = 20 + (int)lua_random(30);
      else
      {
        frapela_aktion_zeit = globals->game_time - 120;
        frapela_zufi_wand = 120;
      }
    }
    else if (frapela_ur == 1 )
    {
      player[1]->set_head(320 - lua_random(10));
      frapela_wandzeit  = globals->game_time;
      if (frapela_aktion_zeit + 180 > globals->game_time ) 
        frapela_zufi_wand = 20 + (int)lua_random(30);
      else
      {
        frapela_aktion_zeit = globals->game_time - 120;
        frapela_zufi_wand = 120;
      }
    }
    else if (frapela_ol == 1 )
    {
      player[1]->set_head(140 - lua_random(10));
      frapela_wandzeit  = globals->game_time;
      if (frapela_aktion_zeit + 180 > globals->game_time ) 
        frapela_zufi_wand = 10 + (int)lua_random(20);
      else
      {
        frapela_aktion_zeit = globals->game_time - 120;
        frapela_zufi_wand = 120;
      }
    }
    else if (frapela_or == 1 )
    {
      player[1]->set_head(230 - lua_random(10));
      frapela_wandzeit  = globals->game_time;
      if (frapela_aktion_zeit + 180 > globals->game_time ) 
        frapela_zufi_wand = 10 + (int)lua_random(20);
      else
      {
        frapela_aktion_zeit = globals->game_time - 120;
        frapela_zufi_wand = 120;
      }
    }
    else if (frapela_u  == 1 )
    {
      if (lua_random() < 0.5 )
        player[1]->set_head( lua_random(5) );
      else
        player[1]->set_head(360 - lua_random(5));
      
      frapela_wandzeit  = globals->game_time;
      
      if (frapela_aktion_zeit + 180 > globals->game_time ) 
        frapela_zufi_wand = 20 + (int)lua_random(40);
      else
      {
        frapela_aktion_zeit = globals->game_time - 120;
        frapela_zufi_wand = 120;
      }
    }
    else if (frapela_l  == 1 )
    {
      player[1]->set_head( 95 - lua_random(10));
      frapela_wandzeit  = globals->game_time;
      if (frapela_aktion_zeit + 180 > globals->game_time ) 
        frapela_zufi_wand = 20 + (int)lua_random(20);
      else
      {
        frapela_aktion_zeit = globals->game_time - 120;
        frapela_zufi_wand = 120;
      }
    }
    else if (frapela_r  == 1 )
    {
      player[1]->set_head(275 - lua_random(10));
      frapela_wandzeit  = globals->game_time;
      if (frapela_aktion_zeit + 180 > globals->game_time ) 
        frapela_zufi_wand = 20 + (int)lua_random(20);
      else
      {
        frapela_aktion_zeit = globals->game_time - 120;
        frapela_zufi_wand = 120;
      }
    }
    else if (frapela_o  == 1 )
    {
      player[1]->set_head(185 - lua_random(10));
      frapela_wandzeit  = globals->game_time;
      
      if (frapela_aktion_zeit + 180 > globals->game_time ) 
        frapela_zufi_wand = 10 + (int)lua_random(10);
      else
      {
        frapela_aktion_zeit = globals->game_time - 120;
        frapela_zufi_wand = 120;
      }
    }
    
    if (frapela_spobj_da_zufi == 505 )
    {
      if (lua_random() < 0.998 )
        frapela_zufi_wand     = 10;
      else
      {
        frapela_spobj_da_zufi = 0;
        frapela_zufi_wand     = 180;
      }
    }
    else if (frapela_spobj_da_zufi == 506 )
    {
      if (lua_random() < 0.998 )
        frapela_zufi_wand     = 10;
      else
      {
        if (lua_random() < 0.3 )
          frapela_wandzeit    = globals->game_time;
          
        frapela_spobj_da_zufi = 0;
        frapela_zufi_wand     = 180;
      }
    }
    
    if (frapela_wandzeit + frapela_zufi_wand > globals->game_time )
      player[1]->set_thrust();    
    
    
  } // player not freezed
  
}

