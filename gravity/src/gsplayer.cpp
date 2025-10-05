/*
 *   GRAVITY STRIKE -- gsplayer.cpp
 *
 *
 *   (c) Copyright 2001/2004 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "gsscript.h"
#include "gsplayer.h"
#include "gsglob.h"
#include "gscontrl.h"
#include "gsbullet.h"
#include "gseffect.h"
#include "gsmap.h"
#include "gsclear.h"
#include "gshelper.h"
#include "gspanel.h"
#include "gsbase.h"
#include "gserror.h"
#include "gsplover.h"
#include "gssound.h"
#include "gsobject.h"
#include "gsosd.h"
#include "gslng.h"
#include "gsnetw.h"
#include "gsenemy.h"
#include "gsosd.h"
#include "weapons/btypes.h"
#include <math.h>

#ifndef PI
  #define PI 3.14159
#endif

#ifndef MAX
  #define MAX(A, B)     ((A) > (B)) ? (A) : (B))
#endif

#define WEAPON_UPGRADES  1                // max weapon upgrades

extern float      sinus[];
extern float      cosinus[];

// ship = 0-1, col = 0-4
gsPlayer::gsPlayer(long num, int mship, int col, char *shipfile, gsMap *pmap)
{
  strcpy(shipimage_file, "");
  if (shipfile) strncpy(shipfile_file, shipfile, 50);
  homebase = 0;
  ship_color = 0;
  play_thrust_sound = 0;
  teleport_time = 0;
  home_x = home_y = 0;
  xpos = ypos = 0;
  xspd = yspd = xacc = yacc = xacc2 = yacc2 = 0;
  weight = 0;
  head = 0;
  shot = 0;
  shot_time = 0;
  dmap = pmap;
  show_status_flag = 0;
  avail_weapon_slots = 0;
  pthrust = pextra_action = 0;
  hitby = -1; killedby = -1;
  hitbyweap = -1;
  max_weight = 20;
  refuel_time = 10;
  weight_factor = 1;
  time_alive = 0;
  delirium = 0;
  ppanel = NULL;
  only_thrust_frame = 0;
  invulnerability = 0;
  indestructibility = 0;
  damage = DAMAGE_NONE;
  damage_norepair = 0;
  handicap = 0;
  do_recoil = 1;
  last_hit_bumper = last_hit_gas = 0;
  show_status_mode = 1;
  authorative_dead = false;

  memset(ship, 0, sizeof(ship));
//  ship[0] = ship[1] = ship[2] = ship[3] = NULL;
  memset(shield_frame, 0, sizeof(shield_frame));
  memset(debris, 0, sizeof(debris));
  max_shield_frame = 2;

  land = land_dur = 0;
  thrust = dead = 0;
  for (int n=0; n < 10; n++) set_weapon(n, W_NONE, 0, 0);

  memset(&stats, 0, sizeof(statistics));

  lifes = 5;
  sig = num;

//  errors->log(2, "init player", "registering controls");
  controls = new gsControl(this);

  if (!shipfile)
  {
    load_ship_image((predefined_ships)mship, col, 0);
  }

  init();

//  errors->log(2, "init player", "creating player overlay");
  overlay = new gsPlayerOverlay(this);

  if (shipfile)
  {
//    errors->log(2, "init player", shipfile);
    char tmp[40];
    strcpy(tmp, globals->ship_dir); strcat(tmp, shipfile);
    if (load_ship(tmp))
    {
      set_ship_name("n/a");
    }
  }

  shield_w = ((BITMAP*)(get_shield_frame(0)))->w;
  shield_h = ((BITMAP*)(get_shield_frame(0)))->h;
  shield_w = MAX(shield_w,shield_h);
  if (width > shield_w) shield_w = width;
  if (height > shield_w) shield_w = height;

  back_width = back_height = 25;
  background = create_bitmap(back_width, back_height);

  shield_w = ((BITMAP*)(get_shield_frame(0)))->w;
  shield_h = ((BITMAP*)(get_shield_frame(0)))->h;

  network = 0;

  for (int un=0; n < 20; n++)
  {
    user_int[un] = 0;
    user_float[un] = 0;
  }
}

gsPlayer::~gsPlayer()
{
//  errors->log(1, "test", "destroy ship", get_ship_name());
  if (controls) delete(controls);
  if (background) destroy_bitmap(background);

  if (ship_type == SHIP_CUSTOM)
  {
    if (ship[0]) { destroy_bitmap(ship[0]); ship[0] = NULL; }
    if (ship[1]) { destroy_bitmap(ship[1]); ship[1] = NULL; }
    if (ship[2]) { destroy_bitmap(ship[2]); ship[2] = NULL; }
    if (ship[3]) { destroy_bitmap(ship[3]); ship[3] = NULL; }
  }

  int n;
  for (n=0; n < 4; n++) { if (debris[n]) destroy_bitmap(debris[n]); debris[n] = NULL; }
  for (n=0; n < 10; n++) { if (shield_frame[n]) destroy_bitmap(shield_frame[n]); shield_frame[n] = NULL; }
}

int gsPlayer::load_ship(char *n)
{
  strncpy(shipfile_file, get_filename(n), 40);
  return script->load_ship(this, n);
}

int gsPlayer::load_image(char *n)
{
  int ret = load_ship_image(n, ship_type);
  if (!ret) ship_type = SHIP_CUSTOM;
  return ret;
}

void gsPlayer::init()
{
  temp_thrust = 0;
  dead = 0;
  head = head_to = 0;
  xspd = yspd = xacc = yacc = xacc2 = yacc2 = 0;
  strcpy(ship_name, "");
  strcpy(ship_desc, "");
  strcpy(name, "");
  max_radar_zoom = 10;
  locked_player = -1;
  rocket_turn_speed = 0.06;
  freezed = 0;
  shrinked = 0;
  shrinklevel = 1;
  avail_weapon_slots = 6;
  teleport_time = 0;
  weight = 0;
  pthrust = pextra_action = 0;
  hitby = -1; killedby = -1;
  hitbyweap = -1;
  time_alive = 0;
  delirium = 0;
  bumped = 0;
  invulnerability = 0;
  indestructibility = 0;  
  damage = DAMAGE_NONE;
  damage_norepair = 0;
  handicap = 0;
  do_recoil = 1;
  last_hit_bumper = last_hit_gas = 0;
  authorative_dead = false;

  // init weapons
  int n;
  for (n=0; n < 10; n++) set_weapon(n, W_NONE);
//  for (n=0; n < 5; n++) set_extra(n, EXTRA_NONE);
  set_extra(EXTRA_NONE);

  active_weapon = 0;

  // other
  hull   = max_hull   = 5;
  shield = max_shield = 5;
  fuel   = max_fuel   = 100;
  extra_power = 0; max_extra_power = 100;
  wall_shield = 2; max_wall_shield = 5;

  weapon_recharge = 0.5;
  shield_recharge = 0.003;
  hull_repair     = 0.001;
  wall_shield_recharge = 0.008;
  extra_recharge  = 0.008; // two minutes for full extra power

  // flying characteristics
  turn_speed            = 3.5;
  max_speed             = 6;
  player_acc            = 0.037;
  fuel_dec              = 0.02;
}

void gsPlayer::reinit()
{
  temp_thrust = 0;
  dead = 0;
  head = head_to = 0;
  xspd = yspd = xacc = yacc = xacc2 = yacc2 = 0;
  locked_player = -1;
  freezed = 0;
  shrinked = 0; shrinklevel = 1;
  width = owidth;
  height = oheight;
  weight = 0;
  pthrust = pextra_action = 0;
  hitby = -1; killedby = -1;
  hitbyweap = -1;
  delirium = 0;
  bumped = 0;
  no_fuel_counter = 0;
  damage = DAMAGE_NONE;
  damage_norepair = 0;
  last_hit_bumper = last_hit_gas = 0;
  authorative_dead = false;

  if (time_alive > stats.max_time_alive) stats.max_time_alive = time_alive;
  time_alive = 0;

  // other
  hull   = get_max_hull(1);
  shield = get_max_shield(1);
  fuel   = get_max_fuel();
  wall_shield = 2;
  extra_power = 0;
  if (get_extra_recharge(1) > 0 && get_extra() > 0) extra_power = 20;

//  return_home();
}

void gsPlayer::create_debris_pixels()
{
  BITMAP *shp = ship[0];
  int w = shp->w;
  int h = shp->h;
  int num;

  for (num=0; num < 4; num++)
  {
    if (debris[num]) destroy_bitmap(debris[num]);
    debris[num] = create_bitmap(w/2+1, h/2+1);
    clear_bitmap(debris[num]);
  }

  blit(shp, debris[0], 0, 0, 0, 0, w/2, h/2);
  blit(shp, debris[1], w/2, 0, 0, 0, w/2, h/2);
  blit(shp, debris[2], 0, h/2, 0, 0, w/2, h/2);
  blit(shp, debris[3], w/2, h/2, 0, 0, w/2, h/2);
}

void gsPlayer::player_dead()
{
  int amount;

  if (dead == 1)
  {
    // send network message (only if this is player 0 (local player)
    if (globals->network_game && player[0]->get_sig() == get_sig() && !get_authorative_dead()) 
      net->game_send_player_dead(sig, xpos, ypos);

    // clear pressed keys if this is a network player
    if (network) controls->clear_pressed_keys();

    // hook function
    script->do_hook_player_dead(glob_lua, this);

    // stop thruster sound
    if (play_thrust_sound) { gsound->stop_player_thrusters(sig); play_thrust_sound = 0; }

    // explosion sound
    gsound->expl_medium1(xpos, ypos);

    // explosion effects
    if (shrinked) amount = 100; else amount = 400;
    effects->pixels->add(amount, xpos+width/2, ypos+height/2, 4, 4, 0, 0, 25,
                         globals->tbl_all, 256);

    if (shrinked) amount = 10; else amount = 50;
    effects->pixels->add(amount, xpos+width/2, ypos+height/2, 20, 20, -xspd/4, -yspd/4, 50,
                         NULL, 10, 60*30, 10,
                         2000, 4000, debris_4_01, globals->gamedat);

    for (int n=0; n < 4; n++)
      effects->pixels->add(1, xpos+width/2, ypos+height/2, width, height, -xspd/4, -yspd/4-2, 50,
                           NULL, 10, 60*30, 10,
                           2000, 4000, 0, NULL, debris[n]);

    if (shrinked) amount = 10; else amount = 20;

    // crater
    if (!globals->network_game ||
        (globals->network_game && player[0]->get_sig() == get_sig())) 
      effects->craters->add(xpos+width/2, ypos+height/2, amount, 1);

    // lay extra if not network game or if we are the local player
    if (!globals->network_game ||
        (globals->network_game && player[0]->get_sig() == sig))
      put_extra();

    // frag message if network game
    if (globals->network_game) 
    {
      char killername[50];
      int killer = get_player_nr_by_sig(get_killedby());
      if (killer == -1) strcpy(killername, "n/a");
      else strncpy(killername, player[killer]->get_name(), 49);
      
      net->add_frag_message(get_name(), killername, get_hitbyweap());
      errors->log(2, "adding fragmsg (this, killer, weap)", get_name(), killername, get_hitbyweap());
    }

    dead = 2;

    hull = shield = 0;
    fuel = 0;
    extra_power = 0;
    if (globals->singleplayer || globals->minigame) lifes--;

    if (ppanel)
    {
      ppanel->update_fuel();
      ppanel->update_shields();
      ppanel->update_extra_energy();
      ppanel->update_lifes();
      ppanel->update_frags();
      ppanel->add_anim_shield_explosion();
      if (globals->network_game) ppanel->update_netplayer_stats();
    }
    stats.lostlifes++;

    // return player coordinates to his home
    return_home();
  }
  else
  {
    dead++;

    if (dead == globals->player_respawn-10 && lifes > 0)
    {
      return_home();
      effects->explosions->add(-1, expl_med1_01, 7, 5, 0, xpos, ypos, globals->gamedat);
    }

    if (dead > globals->player_respawn)
    {
      reinit();
      script->do_hook_player_respawn(glob_lua, this);

      if (ppanel)
      {
        ppanel->update_shields();
        ppanel->update_fuel();
        ppanel->update_weapons();
        ppanel->update_frags();
        ppanel->update_lifes();
        if (globals->network_game) ppanel->update_netplayer_stats();
      }
    }
  }
}

void gsPlayer::move()
{
  float dxs = xspd + xacc + xacc2;
  float dys = yspd + yacc + yacc2;
//  float dgs = sqrt( pow(dxs,2) + pow(dys,2) );
/*
  if (dgs < max_speed)
  {
    yspd += yacc + yacc2;
    xspd += xacc + xacc2;
  }
*/
  if (fabs(dxs) <= get_max_speed(1))
    xspd += xacc + xacc2;
  if (fabs(dys) <= get_max_speed(1))
    yspd += yacc + yacc2;
    
  if (dxs > get_max_speed(1)) xspd = get_max_speed(1);
  if (dys > get_max_speed(1)) yspd = get_max_speed(1);
  if (dxs < -get_max_speed(1)) xspd = -get_max_speed(1);
  if (dys < -get_max_speed(1)) yspd = -get_max_speed(1);

  ypos += yspd;
  xpos += xspd;
}

void gsPlayer::update()
{
  static int old_shield=(int)shield;
  static int old_hull=(int)hull;
  static int old_wall_shield=(int)wall_shield;
  float nplayer_acc;
  float nfuel_dec;

//  controls->update();

  if (!dead && !freezed)
  {
    // get controls
    if (globals->network_game && !network) controls->check_pressed_keys();

    if (!land)
    {
      //if (!network)
      {
        if (!controls->get_disabled())
        {
          if ( controls->left() )   { if (!delirium) head -= get_turn_speed(1); else head += get_turn_speed(1); head_to = head;  }
          if ( controls->right() )  { if (!delirium) head += get_turn_speed(1); else head -= get_turn_speed(1); head_to = head;  }
        }
      }

      if (delirium && gsrand()%50 < 25) { head += ((gsrand()%11)-5); head_to = head; }
      if (damage == DAMAGE_STEERING && gsrand()%30 < 20) { head += ((gsrand()%16)-8); head_to = head; }
      

      if (head_to != head)
      {
        if (head-head_to <= -180) head -= get_turn_speed(1);
        else if (head-head_to > -180 && head-head_to < 0) head += get_turn_speed(1);
        else if (head-head_to <= 180 && head-head_to > 0) head -= get_turn_speed(1);
        else if (head-head_to > 180) head += get_turn_speed(1);
      }
    }

    if (!shrinked)
    {
      if (!controls->get_disabled())
      {
        if ( controls->shoot() )  { if (!shot) shoot(); }
          else shot = 0;

        if ( controls->w1() )     { if (weapon[0].type != W_NONE && weapon[0].bullets != 0 && weapon[0].active) { if (active_weapon == 0) { if (weapon[active_weapon].upgrade < weapon[active_weapon].max_upgrades) weapon[active_weapon].upgrade++; else weapon[active_weapon].upgrade = 0; } active_weapon = 0; } if (ppanel) ppanel->update_weapons(); }
        if ( controls->w2() )     { if (weapon[1].type != W_NONE && weapon[1].bullets != 0 && weapon[1].active) { if (active_weapon == 1) { if (weapon[active_weapon].upgrade < weapon[active_weapon].max_upgrades) weapon[active_weapon].upgrade++; else weapon[active_weapon].upgrade = 0; } active_weapon = 1; } if (ppanel) ppanel->update_weapons(); }
        if ( controls->w3() )     { if (weapon[2].type != W_NONE && weapon[2].bullets != 0 && weapon[2].active) { if (active_weapon == 2) { if (weapon[active_weapon].upgrade < weapon[active_weapon].max_upgrades) weapon[active_weapon].upgrade++; else weapon[active_weapon].upgrade = 0; } active_weapon = 2; } if (ppanel) ppanel->update_weapons(); }
        if ( controls->w4() )     { if (weapon[3].type != W_NONE && weapon[3].bullets != 0 && weapon[3].active) { if (active_weapon == 3) { if (weapon[active_weapon].upgrade < weapon[active_weapon].max_upgrades) weapon[active_weapon].upgrade++; else weapon[active_weapon].upgrade = 0; } active_weapon = 3; } if (ppanel) ppanel->update_weapons(); }
        if ( controls->w5() )     { if (weapon[4].type != W_NONE && weapon[4].bullets != 0 && weapon[4].active) { if (active_weapon == 4) { if (weapon[active_weapon].upgrade < weapon[active_weapon].max_upgrades) weapon[active_weapon].upgrade++; else weapon[active_weapon].upgrade = 0; } active_weapon = 4; } if (ppanel) ppanel->update_weapons(); }
        if ( controls->w6() )     { if (weapon[5].type != W_NONE && weapon[5].bullets != 0 && weapon[5].active) { if (active_weapon == 5) { if (weapon[active_weapon].upgrade < weapon[active_weapon].max_upgrades) weapon[active_weapon].upgrade++; else weapon[active_weapon].upgrade = 0; } active_weapon = 5; } if (ppanel) ppanel->update_weapons(); }
  
        if ( controls->pw() )     { choose_next_weapon(0); if (ppanel) ppanel->update_weapons(); }
        if ( controls->nw() )     { choose_next_weapon(1); if (ppanel) ppanel->update_weapons(); }
       
      }
    }

    // check weapons
    for (int n=0; n < 10; n++)
    {
      if (weapon[n].reload > 0 && weapon[n].cur_reload > 0) weapon[n].cur_reload--;
    }  
    if (ppanel) ppanel->update_weapon_ready();

    // check...
    if (head < 0) { head = head_to = 359; }
    else if (head > 359) {  head = head_to = 0; }

    thrust = FALSE;
    extra_action = FALSE;

    if ( (pthrust || (controls->thrust() /*&& !network */&& !controls->get_disabled())) && fuel > 0 )
    {
      thrust = 1;
      pthrust = FALSE;
    }

    if ( pextra_action > 0 || (controls->extra_action() /*&& !network */&& !controls->get_disabled()) )
    {
      extra_action = TRUE;
      pextra_action--;
    }

    if (invulnerability > 0) invulnerability--;
    if (indestructibility > 0) indestructibility--;

    if ((damage == DAMAGE_THRUSTER || delirium) && gsrand()%50 < 30) { thrust = FALSE; extra_action = FALSE; } // shut down burner on random times

    if (extra_action)
    {
      if (extra_power > 0) 
      {
        if (get_extra() == EXTRA_CLOAK)
          extra_power -= 0.05;
        else
          extra_power -= 0.1;
      }
    }
    else
    {
      if (extra_power <= max_extra_power && get_extra())
        extra_power += get_extra_recharge(1);
    }
    if (ppanel) ppanel->update_extra_energy();

    if (extra_action && extra_power <= 0) extra_action = FALSE;

    // gravitional field around ship
    if (extra_action && get_extra() == EXTRA_DEFLECTOR && !shrinked)
    {
      apply_gravity(70);
      if (extra_power > 0) extra_power -= 0.05;
      
      if (time_alive % 20 == 0)
      {
        int x = (int)xpos + width/2 - 39/2;
        int y = (int)ypos + height/2 - 41/2;
        effects->explosions->add(sig, effect_gravfield_1, 4, 4, 0, x, y, globals->gamedat);
      }
    }

    if (thrust || (extra_action && get_extra() == EXTRA_AFTERBURNER))
    {
      float scale_x = sinus[ (int)head ];
      float scale_y = cosinus[ (int)head ];

      if (shrinked) nplayer_acc = get_player_acc(1)/2.0;
      else nplayer_acc = get_player_acc(1);

      nfuel_dec = get_fuel_dec(1);

      if (extra_action && get_extra() == EXTRA_AFTERBURNER)
      {
        nplayer_acc *= 4;
        if (thrust /*&& !network*/)
        {
          nplayer_acc += get_player_acc(1);
        }
        else nfuel_dec = 0;
      }

      xacc = scale_x * nplayer_acc;
      yacc = - scale_y * nplayer_acc + (globals->level_gravity * globals->gravity_percent);
      if (globals->gravity_percent >= 0) yacc += float(weight/1000.0)*weight_factor;
      else yacc -= float(weight/1000.0)*weight_factor;
  
      fuel -= nfuel_dec;
      land = land_dur = 0;
      land_base = -1;

      if (ppanel) ppanel->update_fuel();
      if (!play_thrust_sound) { gsound->start_player_thrusters(sig); play_thrust_sound = 1; }
    }
    else
    {
      xacc = 0;
      yacc = globals->level_gravity * globals->gravity_percent;
      if (globals->gravity_percent >= 0) yacc += float(weight/1000.0)*weight_factor;
      else yacc -= float(weight/1000.0)*weight_factor;

      if (play_thrust_sound) { gsound->stop_player_thrusters(sig); play_thrust_sound = 0; }
    }

//    if (xacc2-0.001 > 0) xacc2 -= 0.001;
//    if (yacc2-0.001 > 0) yacc2 -= 0.001;

    // update shields
    if (shield < get_max_shield(1) && damage != DAMAGE_SHIELD)
    {
      shield += get_shield_recharge(1);
      if ((int)shield != old_shield) { old_shield = (int)shield; if (ppanel) ppanel->update_shields(); }
    }

    // update hull
    if (hull < get_max_hull(1))
    {
      hull += get_hull_repair(1);
      if ((int)hull != old_hull) { old_hull = (int)hull; if (ppanel) ppanel->update_shields(); }
      if (damage != DAMAGE_NONE && hull > get_max_hull(1)*0.7 && !damage_norepair) damage = DAMAGE_NONE;
    }

    // update wall shields
    if (wall_shield < max_wall_shield)
    {
      wall_shield += get_wall_shield_recharge(1);
      if ((int)wall_shield != old_wall_shield) { old_wall_shield = (int)wall_shield; if (ppanel) ppanel->update_shields(); }
    }

    // fuel leak?
    if (damage == DAMAGE_FUEL && fuel > 0 && !dead)
    {
      fuel -= 0.04;
      if (globals->game_time % 3 == 0)
        effects->pixels->add(1, xpos+width/2, ypos+height/2, 3, 3, 0, globals->particle_gravity, 0, globals->tbl_yellow, 10, 60*5, 30);
      if (ppanel) ppanel->update_fuel();
    }

    // move ship
    if (!land) move();

    // decrease teleport time
    if (teleport_time > 0) teleport_time--;

    if (land) { land_dur++; } else land_dur = 0;
  }
  else if (freezed)
  {
    if (freezed > 0) freezed--;
  }

  // shrink / unshrink ship
  if (shrinked)
  {
    shrinked--;
    if (shrinklevel > 0.4) { shrinklevel -= 0.01; land = 0; }
    width  = int(owidth * shrinklevel);
    height = int(oheight * shrinklevel);
  }
  else if (shrinklevel < 1)
  {
    shrinklevel += 0.01;
    width  = int(owidth * shrinklevel);
    height = int(oheight * shrinklevel);
  }

  if (!dead) time_alive++;

  // do a "shield on" effect when time_alive < indestructible_time
  if (!dead && (time_alive < globals->player_indestructible || invulnerability || indestructibility) && time_alive % 12 == 0)
  {
    int x = (int)xpos + width/2 - shield_w/2;
    int y = (int)ypos + height/2 - shield_h/2;
    effects->explosions->add(sig, effect_shield_1, max_shield_frame+1, 6, 0, x, y, globals->gamedat);
  }

  if (delirium > 0) delirium--;

  if (dead) player_dead();
}

void gsPlayer::shoot()
{
  int tn = (weapon[active_weapon].type == W_DOUBLE) ? 1 : 0;

  if (dead) return;

  if ( damage != DAMAGE_WEAPONS &&
      (weapon[active_weapon].bullets > tn || weapon[active_weapon].bullets == -1) &&
       weapon[active_weapon].active &&
       (weapon[active_weapon].reload == -1 || weapon[active_weapon].cur_reload == 0) &&
       !(get_extra_action() && get_extra() == EXTRA_CLOAK))
  {
    weapon[active_weapon].cur_reload = int(weapon[active_weapon].reload * (1+0.08*handicap));
   
    // heavy weapon reload handicap
    if (weapon[active_weapon].cur_reload < 25 &&
       (weapon[active_weapon].type == W_BIGBOMB || weapon[active_weapon].type == W_ROCKET ||
        weapon[active_weapon].type == W_FREEZER || weapon[active_weapon].type == W_SHRINKER))
      weapon[active_weapon].cur_reload = 25;

    switch(weapon[active_weapon].type)
    {
      case W_DOUBLE :
           effects->bullets->add( (enum WEAPON_TYPE) weapon[active_weapon].type,
                                  this,
                                  weapon[active_weapon].upgrade);
           if (weapon[active_weapon].bullets > 0) weapon[active_weapon].bullets -= 2;
//           stats.bullets_shot += 2;
           break;
  
      default :
           effects->bullets->add( (enum WEAPON_TYPE) weapon[active_weapon].type,
                                  this,
                                  weapon[active_weapon].upgrade);
           if (weapon[active_weapon].bullets > 0) weapon[active_weapon].bullets--;
//           if (weapon[active_weapon].type == W_BOMB) stats.bullets_shot += 8;
//           else if (weapon[active_weapon].type == W_SHOTGUN) stats.bullets_shot += 9;
//           else stats.bullets_shot++;
           break;
    }

    if (ppanel) ppanel->update_bullets();
  }
  else
  {
    // don't shoot, play "klak" sound
    gsound->shoot_empty(xpos, ypos, 30);

    if (weapon[active_weapon].bullets <= tn && weapon[active_weapon].bullets != -1)
    {
      weapon[active_weapon].bullets = 0;
      choose_next_available_weapon(active_weapon);
      if (ppanel) ppanel->update_weapons();
    }
  }

  shot = 1;
}

void gsPlayer::do_damage(int dmg)
{ 
  damage = dmg; // for invocation over network

  gsound->ship_damage(xpos, ypos);

  if (!globals->network_game ||
      (globals->network_game && player[0]->get_sig() == sig))
  {
    OsdBitmap *to = dmap->osd->get_osdbit(1000);
    to->clear();
    to->draw_text(FONT_IMPACT10, get_name(), globals->col_green, 0, 0, 1);
    to->draw_text(FONT_IMPACT14, language->get_string(T_GAME_DAMAGE), globals->col_orange, 0, 17, 1);
    to->draw_text(FONT_IMPACT14, language->get_string(TEXT_STRINGS(T_GAME_DAMAGE+damage)), globals->col_yellow, 0, 38, 1);
    to->set_active(1);
    to->set_timeout(60*2);
  }
  
  if (damage == DAMAGE_LANDING) land_dur = 0;
}

void gsPlayer::hit(int from_player, int from_enemy, float hullhit, float shieldhit, int weap, int fx, int fy, int nostat)
{
  float n = hullhit + shieldhit;
  int max_minus_shield = int(-get_max_shield(1)/3);
  if (max_minus_shield < -5) max_minus_shield = -5;
  
  int executor = get_player_nr_by_sig(from_player);

  int hull_damage = 0, shield_damage = 0;
  float new_shield, new_hull, diff_hits;

  hitbyweap = weap;
  
  // hit by bumper or gas - only set variables, then exit
  if (weap == W_BUMPER)
  {
    hitby = from_player;
    last_hit_bumper = globals->game_time;
    return;
  }
  else if (weap == W_GAS)
  {
    hitby = from_player;
    last_hit_bumper = globals->game_time;
    return;
  }
  
  if (shield > 0 && weap != W_ACID)
  {
    new_shield = shield - shieldhit;
    if (new_shield < max_minus_shield)
    {
      diff_hits = fabs(new_shield - max_minus_shield);
      if (hullhit > diff_hits)
        new_hull = hull - diff_hits;
      else
        new_hull = hull - hullhit;

      new_shield = max_minus_shield;
      hull_damage = 1;
    }
    shield_damage = 1;
  }
  else
  {
    new_hull = hull - hullhit;
    hull_damage = 1;
  }

  if (time_alive > globals->player_indestructible && n > 0 && invulnerability == 0)
  {
    if (n > 0)
    {
      hitby = from_player;
      if (globals->do_all_hooks || do_hooks)
        script->do_hook_player_hit(glob_lua, this, n, weap, fx, fy);
      if (get_dont_continue()) return;
    }
    
    if (shield_damage)
    {
      shield = new_shield;
  
      // display shield
      int x = (int)xpos + width/2 - shield_w/2;
      int y = (int)ypos + height/2 - shield_h/2;
      effects->explosions->add(sig, effect_shield_1, max_shield_frame+1, 6, 0, x, y, globals->gamedat);

      gsound->shield_hit(x, y);
    }
  
    if (hull_damage && !indestructibility)
    {
      hull = new_hull;
  
      // play "tock" sound
      gsound->hull_hit(xpos, ypos, 60);
      
      // damage? (when network: only if this is the local player)
      if ((!globals->network_game ||
           globals->network_game && player[0]->get_sig() == sig) &&
          damage == DAMAGE_NONE && hull < get_max_hull(1)*0.5 && hull > 0)
      {
//        damage = (gsrand() % (DAMAGE_MAXDAMAGE-1)) + 1;
        damage = myrand(DAMAGE_MAXDAMAGE-1)+1;
        
        do_damage(damage);

        // send over network
        if (globals->network_game)
          net->game_send_player_damage(sig, damage);
      }
    }
  
    // check if dead
    if (hull <= 0 /*&& !network */)
    {
      if (from_player >= 0 && executor >= 0 && !globals->network_game)
        player[executor]->inc_frags();

      if (!network) player_dead();

      killedby = from_player;
      
      // handicaps
      if (globals->handicap)
      {
        if (from_player >= 0 && executor >= 0)
        {
          if (get_handicap() == 0)
            player[executor]->set_handicap(player[executor]->get_handicap() + 1);
          set_handicap( get_handicap() - 1 );
        }
      }
  //    dec_frags();
    }
  
    if (from_player >= 0 && executor >= 0 && !nostat)
      player[executor]->inc_bullets_hit();

    // update shields
    if (ppanel) ppanel->update_shields();
  }
  // if indestructible, draw shield anim
  else
  {
    // display shield
    int x = (int)xpos + width/2 - shield_w/2;
    int y = (int)ypos + height/2 - shield_h/2;
    effects->explosions->add(sig, effect_shield_1, max_shield_frame+1, 6, 0, x, y, globals->gamedat);

    gsound->shield_hit(x, y);
  }
}

void gsPlayer::save_background()
{
  if (!dead)
  {
    if (dmap->is_on_map(xpos, ypos, back_width, back_height))
    {
      blit((BITMAP*)dmap->get_vscreen(), background,
           (int)xpos - dmap->get_map_x() + owidth/2 - back_width/2,
           (int)ypos - dmap->get_map_y() + oheight/2 - back_height/2,
           0, 0, back_width, back_height);
    }
  }
}


typedef struct { int x, y; } xystruc;
xystruc line_points[20];
int curpoint;

void get_linepoints(BITMAP *bmp, int x, int y, int d)
{
  if (curpoint < 20)
  {
    line_points[curpoint].x = x;
    line_points[curpoint].y = y;
    curpoint++;
  }
}

void gsPlayer::display_exhaust_pixels(gsMap *m1, gsMap *m2)
{
  if (thrust)
  {
    float xp = width/2 - 4; float yp = height/2 - 1;
    float phead;
    float xrot, yrot;
    float dx, dy;
    float px1, py1, px2, py2;
  
    // Point 1 (left end of extinguisher)
    phead = (head-45 < 0) ? 360 + (head-45) : head-45;
    xrot = sinus[ (int)phead ];
    yrot = cosinus[ (int)phead ];
    dx = xp - yrot*7;
    dy = yp - xrot*7;
    px1 = xpos + dx;
    py1 = ypos + dy;
  
    // Point 2 (right end of extinguisher)
    phead = (head+225 > 360) ? (head+225) - 360 : head+225;
    xrot = sinus[ (int)phead ];
    yrot = cosinus[ (int)phead ];
    dx = xp - yrot*7;
    dy = yp - xrot*7;
    px2 = xpos + dx;
    py2 = ypos + dy;
  
    // find a point somewhere on the line (px1,py1)-(px2,py2)
    curpoint = 0;
    do_line(m1->get_vscreen(), (int)px1, (int)py1, (int)px2, (int)py2, 0, get_linepoints);
    int rp = gsrand() % curpoint;
  
    // draw pixels...

    float pyspd = ((yspd > 0 && yacc > 0) || (yspd < 0 && yacc < 0)) ? -yspd/2 : yspd/2;
    float pxspd = ((xspd > 0 && xacc > 0) || (xspd < 0 && xacc < 0)) ? -xspd : xspd;

    pxspd = pyspd = 0;

    effects->pixels->add(1, line_points[rp].x, line_points[rp].y, 1, 1,
                         pxspd, pyspd -(globals->particle_gravity * globals->gravity_percent), 0,
                         globals->tbl_red, 10, 60*3, 20);
  }
}

void gsPlayer::display(gsMap *m)
{
  int shaded = 0;
  BITMAP *shaded_ship = NULL;
  BITMAP *s2 = (temp_thrust < 15) ? ship[1] : ship[2];
  BITMAP *s  = (thrust || only_thrust_frame) ? s2 : ship[0];
  if (extra_action && get_extra() == EXTRA_AFTERBURNER) s = ship[3];
  if (extra_action && get_extra() == EXTRA_CLOAK && extra_power > 0)
  {
    if (!globals->network_game || network)
      s = NULL;
    else
    {
      shaded = 1;
      shaded_ship = create_bitmap(s->w, s->h);
      clear_bitmap(shaded_ship);
      draw_lit_sprite(shaded_ship, s, 0, 0, globals->col_lblue);
      s = shaded_ship;
    }
  }

  temp_thrust++;
  if (temp_thrust >= 30) temp_thrust = 0;

  if (!dead && s)
  {
    if (m->is_on_screen(xpos, ypos, back_width, back_height))
    {
      if (shrinklevel == 1.0)
        rotate_sprite((BITMAP*)m->get_vscreen(),
                      s,
                      (int)xpos - m->get_map_x(),
                      (int)ypos - m->get_map_y(),
                      ftofix(head / 1.40625));
      else
      {
        rotate_scaled_sprite((BITMAP*)m->get_vscreen(),
                      s,
                      (int)xpos - m->get_map_x(),
                      (int)ypos - m->get_map_y(),
                      ftofix(head / 1.4),
                      ftofix(shrinklevel));
      }
    }

    if (delirium)
      effects->pixels->add(2, xpos+8, ypos+8,
                          14, 14, 0, 0, 28,
                          PIX_BLUE, 10, 4000, 15);

  } // if !dead

  if (shaded)
  {
    destroy_bitmap(shaded_ship);
  }
}

void gsPlayer::clear(gsMap *m)
{
  if (!dead && m->is_on_map(xpos, ypos, back_width, back_height))
  {
    blit(background, (BITMAP*)m->get_vscreen(),
                     0, 0,
                     (int)xpos - m->get_map_x() + owidth/2 - back_width/2,
                     (int)ypos - m->get_map_y() + oheight/2 - back_height/2,
                     back_width, back_height);
  }
}

int gsPlayer::over_base()
{
  Base *temp;
  temp = bases->get_first_base();

  int xp = (int)get_x();
  float yp = (int)get_y() + get_height() - 1;

  while (temp != NULL)
  {
    // landing pad hit?
    if ( yp <= temp->get_land_y() && yp >= temp->get_land_y()-10 &&
         xp >= temp->get_land_x() &&
         xp + get_width() <= temp->get_land_x() + temp->get_land_w())
    {
      // angle correct? speed acceptable?
      if ( (get_head() >= 345 || get_head() <= 15) && get_yspd() < 3 )
      {
        return temp->get_nr();
      } // if angle correct
    } // if land_hit

    temp = temp->get_next();
  } // while bases...

  return -1;
}

void gsPlayer::check_map_collisions()
{
  
  int e1,e2,e3,e4,e5,e6,e7,e8;
  int xp,yp;

  if (!dead)
  {
    // ausserhalb Karte?
    if (!network &&
        (xpos < 0 || xpos > globals->map_w*globals->tile_w ||
         ypos < 0 || ypos > globals->map_h*globals->tile_h))
    {
      dead = 1;
      stats.suicides++;
      set_killedby(-1);
      set_hitbyweap(-1);
      return;
    }

    if (dmap->is_on_map( xpos, ypos, back_width, back_height ))
    {
      xp = fast_ftol(xpos + (width / 2) - dmap->get_map_x() + xspd);
      yp = fast_ftol(ypos + (height / 2) - dmap->get_map_y() + yspd);

      // check for bump
      e1 = e2 = e3 = e4 = e5 = e6 = e7 = e8 = FALSE;
      if (getpixel(dmap->get_vscreen(),xp-width/2+3,  yp) > 0) e1 = TRUE;
      if (getpixel(dmap->get_vscreen(),xp+width/2-3,  yp) > 0) e2 = TRUE;
      if (getpixel(dmap->get_vscreen(),xp, yp-height/2+3) > 0) e3 = TRUE;
      if (getpixel(dmap->get_vscreen(),xp, yp+height/2-3) > 0) e4 = TRUE;

      if (!bumped && over_base() < 0)
      {
        if (e1 || e2)
        {
          if (wall_shield >= 1)
          {
            xspd = -xspd/1.5;
            if (globals->network_game) net->game_send_player_position();
            bumped = 1;
          }
          else
          {
            if (!network) 
            {
              dead = 1;
              stats.suicides++;
            }
            set_killedby(-1);
            set_hitbyweap(-1);
          }
        }

        if (!dead)
        if (e3 || (e4 && (fabs(yspd) > 1.5 || fabs(xspd) > 1.5 || (head < 345 && head > 15))))
        {
          if (wall_shield >= 1)
          {
            yspd = -yspd/1.5;
            if (globals->network_game) net->game_send_player_position();
            bumped = 1;
          }
          else
          {
            if (!network)
            {
              dead = 1;
              stats.suicides++;
            }
            set_killedby(-1);
            set_hitbyweap(-1);
          }
        }

        if (bumped)
        {
          // display shield
          int x = fast_ftol(xpos + owidth/2 - shield_w/2);
          int y = fast_ftol(ypos + oheight/2 - shield_h/2);
          effects->explosions->add(sig, effect_shield_1, max_shield_frame+1, 6, 0, x, y, globals->gamedat);
          gsound->wall_hit(x, y);
          if (invulnerability == 0) wall_shield--;
        }
      }
      else if (bumped < 5 && bumped > 0) bumped++; else bumped = 0;

      // check for land
      if (!bumped && !dead)
      {
        e1 = e2 = e3 = e4 = e5 = e6 = e7 = e8 = FALSE;
        if (getpixel(dmap->get_vscreen(),xp  ,yp-4) > 0) e1 = TRUE;
        if (getpixel(dmap->get_vscreen(),xp-4,yp  ) > 0) e2 = TRUE;
        if (getpixel(dmap->get_vscreen(),xp+4,yp  ) > 0) e3 = TRUE;
        if (getpixel(dmap->get_vscreen(),xp-4,yp+height/2+1) > 0) e4 = TRUE;
        if (getpixel(dmap->get_vscreen(),xp+4,yp+height/2+1) > 0) e5 = TRUE;
        if (getpixel(dmap->get_vscreen(),xp-5,yp+height/2-1) > 0) e6 = TRUE;
        if (getpixel(dmap->get_vscreen(),xp+5,yp+height/2-1) > 0) e7 = TRUE;

        if (e1 || e2 || e3 || e4 || e5 || e6 || e7)
        {
          // Landed on landscape?
          if ( (head > 345 || head < 15) &&
               fabs(xspd) < 1.5 && fabs(yspd) < 1.5 && e6 && e7 && !(e1 || e2 || e3))
          {
            xspd = yspd = 0;
            land = 1;
          }
        }

        if (land && !e4 && !e5)
          land = 0;
      }
    } // if on map
  } // if !dead
  
  // check if suicide was caused by bumper or delirium bomb
  if (dead == 1 && (last_hit_bumper > 0 || last_hit_gas > 0))
  {
    if (globals->game_time - last_hit_bumper <= 60*3 ||
        globals->game_time - last_hit_gas <= 60*6)
    {
      stats.suicides--;
      
      if (last_hit_bumper) set_hitbyweap(W_BUMPER);
      else set_hitbyweap(W_GAS);

      if (get_hitby() >= 0 && !globals->network_game)
        player[get_player_nr_by_sig(get_hitby())]->inc_frags();

      set_killedby(get_hitby());
    }
  }  
}

// this player collides with another one
int gsPlayer::do_player_collision(gsPlayer *opl)
{
  // xpos
  float cx1 = xpos+width/2; float cx2 = opl->get_x()+opl->get_width()/2;
  // ypos
  float cy1 = ypos+height/2; float cy2 = opl->get_y()+opl->get_height()/2;
  // xspd
  float dx1 = xspd; float dx2 = opl->get_xspd();
  // yspd
  float dy1 = yspd; float dy2 = opl->get_yspd();
  // radiis
  float r1  = MAX(width,height)/2; float r2 = MAX(opl->get_width(),opl->get_height())/2;
  // masses
  float m1  = r1 * PI;
  float m2  = r2 * PI;
/*
  if (fabs(cx2-cx1) < 6 && fabs(cy2-cy1) < 6)
  {
    dead = 1;
    opl->set_dead();
    return 0;
  }
*/
  // get correct cx/cy
//  cx1 -= dx1; cy1 -= dy1;
//  cx2 -= dx2; cy2 -= dy2;
  static int overlapped_flag = 0;
  float t = get_time_to_collision(cx1,cy1,dx1,dy1,r1, cx2,cy2,dx2,dy2,r2);
//errors->log(1, "t", t);
  // start overlapped
  if (t >= VERY_LONG_TIME*2 && overlapped_flag == 10)
  {
    dead = 1;
    opl->set_dead();
    overlapped_flag = 0;
    inc_suicides();
    opl->inc_suicides();
    set_killedby(opl->get_sig());
    set_hitbyweap(-2);
    opl->set_killedby(get_sig());
    opl->set_hitbyweap(-2);
    if (ppanel) ppanel->update_frags();
    if (opl->get_ppanel()) opl->get_ppanel()->update_frags();
    return 1;
  }
  else if (t >= VERY_LONG_TIME*2) overlapped_flag++;

  if (t >= VERY_LONG_TIME) return 0; // no collision
  cx1 += t * dx1;
  cy1 += t * dy1;
  cx2 += t * dx2;
  cy2 += t * dy2;
  // --

  float tx,ty,contactAngle;
  tx = cx1 - cx2;
  ty = cy1 - cy2;

  if (tx != 0)
  {
    contactAngle = atan(ty/tx);
    if (contactAngle < 0) contactAngle = -contactAngle;
  }
  else
    contactAngle = PI/2;

  float cxspd,cyspd;
  cxspd = dx2 - dx1;
  cyspd = dy2 - dy1;
  float speed = sqrt( cyspd*cyspd + cxspd*cxspd );
  float contxspd, contyspd;
  contxspd = sin(contactAngle) * speed;
  contyspd = cos(contactAngle) * speed;

  float closingAngle;
  if (cxspd != 0)
  {
    closingAngle = atan(cyspd/cxspd);
    if (closingAngle < 0) closingAngle = -closingAngle;
  }
  else
    closingAngle = PI/2;

  float massFactor = 2 / (m1+m2);
  float mass1,mass2;
  mass1 = m1 * massFactor;
  mass2 = m2 * massFactor;

  float adiff,portion;

  if (cx1 > cx2)
  {
    if (cy1 < cy2)
    {
      if (cyspd < 0)
      {
        if (cxspd < 0) adiff = PI - contactAngle - closingAngle;
        else adiff = contactAngle - closingAngle;
      }
      else
        adiff = contactAngle + closingAngle;

      portion = cos(adiff);
      dx1 += contxspd * portion * mass2;
      dy1 -= contyspd * portion * mass2;
      dx2 -= contxspd * portion * mass1;
      dy2 += contyspd * portion * mass1;
    }
    else
    {
      if (cyspd > 0)
      {
        if (cxspd < 0) adiff = PI - contactAngle - closingAngle;
        else adiff = contactAngle - closingAngle;
      }
      else
        adiff = contactAngle + closingAngle;

      portion = cos(adiff);
      dx1 += contxspd * portion * mass2;
      dy1 += contyspd * portion * mass2;
      dx2 -= contxspd * portion * mass1;
      dy2 -= contyspd * portion * mass1;
    }
  }
  else
  {
    if (cy1 < cy2)
    {
      if (cyspd < 0)
      {
        if (cxspd < 0) adiff = contactAngle - closingAngle;
        else adiff = PI - contactAngle - closingAngle;
      }
      else
        adiff = contactAngle + closingAngle;

      portion = cos(adiff);
      dx1 -= contxspd * portion * mass2;
      dy1 -= contyspd * portion * mass2;
      dx2 += contxspd * portion * mass1;
      dy2 += contyspd * portion * mass1;
    }
    else
    {
      if (cyspd > 0)
      {
        if (cxspd < 0) adiff = contactAngle - closingAngle;
        else adiff = PI - contactAngle - closingAngle;
      }
      else
        adiff = contactAngle + closingAngle;

      portion = cos(adiff);
      dx1 -= contxspd * portion * mass2;
      dy1 += contyspd * portion * mass2;
      dx2 += contxspd * portion * mass1;
      dy2 -= contyspd * portion * mass1;
    }
  }

  float fact=1;

  if (!get_shrinked() && !opl->get_shrinked())
  {
    if (fabs(dx1) >= get_max_speed(1)) fact = get_max_speed(1)/dx1;
    xspd = dx1*fact;
    fact = 1;
    if (fabs(dy1) >= get_max_speed(1)) fact = get_max_speed(1)/dy1;
    yspd = dy1*fact;
    land = 0;
  
    fact = 1;
    if (fabs(dx2) >= opl->get_max_speed(1)) fact = opl->get_max_speed(1)/dx2;
    opl->set_xspd(dx2*fact);
    fact = 1;
    if (fabs(dy2) >= opl->get_max_speed(1)) fact = opl->get_max_speed(1)/dy2;
    opl->set_yspd(dy2*fact);
    opl->set_land(0);
  }
//  errors->log(1, "coll", dgs, opl->get_max_speed());

  // check for shields & deaths
  int x,y;
  if (wall_shield >= 1 && !get_shrinked() && !opl->get_shrinked())
  {
    // display shield
    x = (int)xpos + owidth/2 - shield_w/2;
    y = (int)ypos + oheight/2 - shield_h/2;
    effects->explosions->add(sig, effect_shield_1, max_shield_frame+1, 6, 0, x, y, globals->gamedat);
    gsound->ship_hit(x, y);
    if (!invulnerability) wall_shield--;
  }
  else
  {
    // normal suicides (unshrinked players collide)
    if (!get_shrinked() && !opl->get_shrinked())
    {
      dead = 1;
      inc_suicides();
      set_killedby(opl->get_sig());
      set_hitbyweap(-2);
    }
    // this player is shrinked, the other is not (he kills us)
    else if (get_shrinked() && !opl->get_shrinked())
    {
      dead = 1;
      opl->inc_frags();
      set_killedby(opl->get_sig());
      set_hitbyweap(W_SHRINKER);
    }
    // both players are shrinked: double suicide
    else if (get_shrinked() && opl->get_shrinked())
    {
      dead = 1;
      inc_suicides();
      set_killedby(opl->get_sig());
      set_hitbyweap(-2);
    }
    if (ppanel) ppanel->update_frags();
  }

  if (opl->get_wall_shield() >= 1 && !get_shrinked() && !opl->get_shrinked())
  {
    // display shield
    x = (int)opl->get_x() + opl->get_owidth()/2 - opl->get_shield_w()/2;
    y = (int)opl->get_y() + opl->get_oheight()/2 - opl->get_shield_h()/2;
    effects->explosions->add(opl->get_sig(), effect_shield_1, opl->get_max_shield_frame()+1, 6, 0, x, y, globals->gamedat);
    gsound->ship_hit(x, y);
    if (!opl->get_invulnerability()) opl->set_wall_shield(opl->get_wall_shield()-1);
    if (!globals->network_game) opl->get_ppanel()->update_shields();
  }
  else
  {
    // normal suicides (unshrinked players collide)
    if (!opl->get_shrinked() && !get_shrinked())
    {
      opl->set_dead();
      opl->inc_suicides();
      opl->set_killedby(get_sig());
      opl->set_hitbyweap(-2);
    }
    // this player is not shrinked, the other is (we kill him)
    else if (opl->get_shrinked() && !get_shrinked())
    {
      opl->set_dead();
      inc_frags();
      opl->set_killedby(get_sig());
      opl->set_hitbyweap(W_SHRINKER);
    }
    // both players are shrinked: double suicide
    else if (opl->get_shrinked() && get_shrinked())
    {
      opl->set_dead();
      opl->inc_suicides();
      opl->set_killedby(get_sig());
      opl->set_hitbyweap(-2);
    }
  }

  return 1;
}

// check collisions with other players
void gsPlayer::check_player_collisions()
{
//  static int collided = 0;

  if (globals->network_game && net->is_client()) return;

  if (!dead)
  for (int n=0; n < globals->players; n++)
  {
    // check if player is not ourselves and player isn't dead
    if (player[n]->get_sig() != sig && !player[n]->get_dead())
    {
      if (radius_player_collision(this, player[n]))
      {
        if (do_player_collision(player[n]))
        {
          // if network game server: send status of collided players to all clients
          if (globals->network_game && net->is_server())
          {
            net->game_send_player_authorative_update(get_sig());
            net->game_send_player_authorative_update(player[n]->get_sig());
            if (get_dead()) set_authorative_dead(true);
            if (player[n]->get_dead()) set_authorative_dead(true);
          }
        }
      }
    }
  }
}

void gsPlayer::check_stuff()
{
  int firstplayer = globals->network_game ? 0 : 1;

  if (!dead && !network)
  for (int n=firstplayer; n < globals->players; n++)
  {
    // check if player is not ourselves and player isn't dead
    if (player[n]->get_sig() != sig && !player[n]->get_dead())
    {
      // check for 'line of sight' and activate ship status of 'seen' ship
      if (show_status_mode == 1)
      {
        float dist = distance(xpos, ypos, player[n]->get_x(), player[n]->get_y());
        int mydist = globals->network_game ? 250 : 150;

        if (dist < mydist) 
          player[n]->show_status();
        else 
          player[n]->hide_status();
      }
      else if (show_status_mode == 2)
      {
        player[n]->show_status();
      }
    }
  } // for players

  // check if landed
  if (!dead && !land)
  {
    bases->check_if_landed(this);
  }

  if (land)
  {
    if (damage == DAMAGE_LANDING && get_land_dur() == 30)
      player_dead();
    else if (damage != DAMAGE_LANDING)
      check_landing();
  } // if land

  check_object_collisions();
  check_enemy_collisions();
}

void gsPlayer::check_landing()
{
  Base *mybase = bases->get_base(get_land_base());
  char cargotext[40];
  strcpy(cargotext, "");

  if (mybase)
  {
    if (get_land_dur() == 10)
    {
      script->do_hook_player_land(glob_lua, this, mybase);
    }

    // returning cargo
    if (weight > 0 && mybase->get_nr() == get_homebase() && get_land_dur() == 60)
    {
      sprintf(cargotext, "%s (%d)", language->get_string(T_GAME_CARGOREC), weight);
      OsdBitmap *to = dmap->osd->get_osdbit(1000);
      to->clear();
      to->draw_text(FONT_IMPACT10, language->get_string(T_GAME_HOMEBASE), globals->col_green, 0, 0, 1);
      to->draw_text(FONT_IMPACT14, cargotext, globals->col_yellow, 0, 17, 1);
      to->set_active(1);
      to->set_timeout(60*3);
  
      if (globals->do_all_hooks || do_hooks)
        script->do_hook_player_returns_cargo(glob_lua, this);
      if (get_dont_continue()) return;

      gsound->cargo_returned(xpos, ypos);      
      weight = 0;
    }
  
    // cargo?
    if (mybase->get_cargo_type() != CARGO_NONE &&
        mybase->get_cargo_weight() && get_land_dur() == 60 &&
        mybase->get_land_x() + mybase->get_land_w()/2 - 15 < xpos+width/2 &&
        mybase->get_land_x() + mybase->get_land_w()/2 + 15 > xpos+width/2 &&
        weight+mybase->get_cargo_weight() <= max_weight)
    {
      switch (mybase->get_cargo_type())
      {
        case CARGO_NORMAL :
             if (strlen(mybase->get_cargo_name()) == 0)
               sprintf(cargotext, "%s: %d", language->get_string(T_GAME_WEIGHT), mybase->get_cargo_weight());
             else
               sprintf(cargotext, "%s", mybase->get_cargo_name());
             weight += mybase->get_cargo_weight();
             break;
  
        case CARGO_OBJECT :
             if (strlen(mybase->get_cargo_name()) == 0)
             {
               if (mybase->get_cargo_object()->get_type() == OBJ_MAN1)
                 sprintf(cargotext, "%s (%d)", language->get_string(T_GAME_MAN), mybase->get_cargo_weight());
               else
                 sprintf(cargotext, "%s (%d)", language->get_string(T_GAME_SOMETHING), mybase->get_cargo_weight());
             }
             else
               sprintf(cargotext, "%s", mybase->get_cargo_name());
  
             weight += mybase->get_cargo_weight();
             break;
      } // switch cargo type
  
      // show message
      OsdBitmap *to = dmap->osd->get_osdbit(1000);
      to->clear();
      to->draw_text(FONT_IMPACT10, language->get_string(T_GAME_CARGO), globals->col_green, 0, 0, 1);
      to->draw_text(FONT_IMPACT14, cargotext, globals->col_yellow, 0, 17, 1);
      to->set_active(1);
      to->set_timeout(60*3);
  
      if (globals->do_all_hooks || do_hooks)
        script->do_hook_player_takes_cargo(glob_lua, this, mybase);
      if (get_dont_continue()) return;
  
      gsound->cargo_taken(xpos, ypos);
      mybase->set_cargo_type(CARGO_NONE);
      mybase->set_cargo_weight(0);
      mybase->set_cargo_name("");
    }
  } // if mybase

  // fuel update
  if (get_land_dur() % get_refuel_time() == 0)
  {
    if (get_fuel() <= get_max_fuel()-1)
    {
      if (get_land_base() == get_homebase())
        set_fuel(get_fuel()+1);
      else if ( mybase &&
               ( mybase->get_refuel_percent() != 0 &&
                 ( mybase->get_refuel_player() == 0 ||
                   mybase->get_refuel_player() == get_sig() ) ))
        set_fuel(get_fuel()+ float(mybase->get_refuel_percent() / 100.0));
      else
        set_fuel(get_fuel()+0.1);
      if (ppanel) ppanel->update_fuel();
    }
    else { set_fuel(max_fuel); if (ppanel) ppanel->update_fuel(); }
  }

}

// check collisions with special objects
void gsPlayer::check_object_collisions()
{
  Object *tempob, *next_object;
  tempob = objects->get_first_spobject();
  int bumped = 0;
  float newx1, newy1, newx2, newy2;

  while (tempob != NULL)
  {
    next_object = tempob->get_next();

    if ( !dead && tempob->get_active() &&
         collision( xpos, ypos, width, height,
                    tempob->get_x(), tempob->get_y(),
                    tempob->get_width(), tempob->get_height(),
                    xspd, yspd, tempob->get_xspd(), tempob->get_yspd(),
                    &newx1, &newy1, &newx2, &newy2, 1 ) )
    {
      // adjust positions
//      xpos = newx1; ypos = newy1; creates problems!
      
      if (globals->do_all_hooks || do_hooks)
        script->do_hook_player_with_spobject(glob_lua, this, tempob);
      if (get_dont_continue()) return;

      switch (tempob->get_type())
      {
        case SPOBJ_EXTRA :
             //if (!network)
             {
               apply_weapon_pack(tempob->get_user(), tempob->get_user2());
               stats.extras_taken++;
             }
             gsound->extra_taken(xpos, ypos);
             objects->remove_special(tempob);
             break;

        case SPOBJ_BUMPU :
             if (!bumped && yspd > 0) // avoid multiple bumping (error when falling 'between' two bumpers)
             {
               tempob->set_curframe(1);
               yspd = -yspd;
               bumped = 1;
             }
             break;

        case SPOBJ_BUMPD :
             if (!bumped && yspd < 0) // avoid multiple bumping (error when falling 'between' two bumpers)
             {
               tempob->set_curframe(1);
               yspd = -yspd;
               bumped = 1;
             }
             break;

        case SPOBJ_BUMPL :
             if (!bumped && xspd < 0)
             {
               tempob->set_curframe(1);
               xspd = -xspd;
               bumped = 1;
             }
             break;

        case SPOBJ_BUMPR :
             if (!bumped && xspd > 0)
             {
               tempob->set_curframe(1);
               xspd = -xspd;
               bumped = 1;
             }
             break;

        case SPOBJ_TELEPORT1 :
             if (teleport_time == 0)
             {
               if (collision( xpos, ypos, width, height,
                              tempob->get_x()+8, tempob->get_y()+8,
                              tempob->get_width()-16, tempob->get_height()-16) )
               {
                 Object *temp = objects->get_spobject(tempob->get_user());
                 effects->explosions->add(-1, effect_beam1_1, 5, 4, 0, xpos, ypos, globals->gamedat);
                 /*if (!network)*/
                 {
                   set_x(temp->get_x()+4);
                   set_y(temp->get_y()+4);
                   teleport_time = 60;
                 }
                 effects->explosions->add(sig, effect_beam1_1, 5, 4, 0, xpos, ypos, globals->gamedat);
                 gsound->beam_ship(xpos, ypos);
               }
             }
             break;

      } // type of object?
    } // if collision

    tempob = next_object;
  } // loop through objects

}

// check collisions with enemies

      /* TODO (#1#): Pixel-Perfect Collision System zumindest für 
                     Spieler/Gegner-Kollisionen */

void gsPlayer::check_enemy_collisions()
{
  
  Enemy *tempen, *next_enemy;
  tempen = enemies->get_first_enemy();
  float newx1, newy1, newx2, newy2;

  if (!dead)
  while (tempen != NULL)
  {
    next_enemy = tempen->get_next();

/*
    // bounding box pruefen
    if ( collision( (int)xpos, (int)ypos,
                    width, height,
                    int(tempen->get_x()+tempen->get_width()), int(tempen->get_y()+tempen->get_height()),
                    int(tempen->get_width()-tempen->get_width()), int(tempen->get_height()-tempen->get_height())) )
    {
      int ppcol = 0;
      
      BITMAP *enemy_bitmap = create_bitmap(tempen->get_width(), tempen->get_height());
      clear_bitmap(enemy_bitmap);
      draw_rle_sprite(enemy_bitmap, tempen->get_current_sprite(), 0, 0);
      
      BITMAP *player_bitmap = create_bitmap(back_width, back_height);
      clear_bitmap(player_bitmap);
      rotate_sprite(player_bitmap, ship[0], 0, 0, ftofix(head / 1.40625));
      
      // alle pixel von Spielerschiff durchgehen
      int relx = get_x() - tempen->get_x();
      int rely = get_y() - tempen->get_y();
      
      for (int pbx=0; pbx < back_width; pbx++)
      {
        if (ppcol) break;
        for (int pby=0; pby < back_height; pby++)
        {
          if (getpixel(player_bitmap, pbx, pby) != 0)
          {
            if (getpixel(enemy_bitmap, relx+pbx, rely+pby) != 0)
            {
              ppcol = 1;
              break;
            }
          }
        }
      }
      
      destroy_bitmap(enemy_bitmap);
      destroy_bitmap(player_bitmap);
      
      
      if (ppcol)
      {
        if (globals->do_all_hooks || do_hooks)
          script->do_hook_player_with_enemy(glob_lua, this, tempen);
        if (get_dont_continue()) return;
  
        if (tempen->get_shrinked()) tempen->hit(get_sig(), 0, tempen->get_maxhit()-tempen->get_hit()+1, xpos, ypos, xspd, yspd);
        else
        {
          hit(tempen->get_sig(), tempen->get_nr(), 10);
          tempen->hit(get_sig(), 0, -15, xpos, ypos, xspd, yspd);
          xspd = -xspd;
          yspd = -yspd;
        }
      } // if ppcol
      
    } // if collision
*/


    int enx = fast_ftol(tempen->get_width()-tempen->get_width()*0.4);
    int eny = fast_ftol(tempen->get_height()-tempen->get_height()*0.4);
    
    if ( collision( xpos, ypos, width, height,
                    tempen->get_x()+tempen->get_width()*0.2, tempen->get_y()+tempen->get_height()*0.2,
                    enx, eny,
                    xspd, yspd, tempen->get_xspd(), tempen->get_yspd(),
                    &newx1, &newy1, &newx2, &newy2, 1 ) )
    {
      // adjust positions
      xpos = newx1; ypos = newy1;
      
      if (globals->do_all_hooks || do_hooks)
        script->do_hook_player_with_enemy(glob_lua, this, tempen);
      if (get_dont_continue()) return;

      if (tempen->get_shrinked()) tempen->hit(get_sig(), 0, tempen->get_maxhit()-tempen->get_hit()+1, xpos, ypos, xspd, yspd);
      else
      {
        hit(tempen->get_sig(), tempen->get_nr(), 10, 10);
        tempen->hit(get_sig(), 0, -15, xpos, ypos, xspd, yspd);
        xspd = -xspd;
        yspd = -yspd;
      }
    } // if collision

    tempen = next_enemy;
  } // loop through objects

}


int gsPlayer::load_ship_image(char *n, int last_ship_type)
{
  PALETTE pal;

  BITMAP *bit = load_bitmap(n, pal);

  int error = 0, nr, shield_ok = 0;

  for (nr=0; nr < 10; nr++) { if (shield_frame[nr]) destroy_bitmap(shield_frame[nr]); shield_frame[nr] = NULL; }
  memset(shield_frame, 0, sizeof(shield_frame));

  if (bit)
  {
    int w = bit->w;
    int h = bit->h;
    int startpos[15];
    int shipw=0, shiph=0;
    int topleft = getpixel(bit, 0, 0);

    memset(&startpos, 0, sizeof(startpos));

    // find ship width
    int x;
    for (x=1; x < w; x++)
      if (getpixel(bit, x, 1) == topleft) break;
    shipw = x - 1;

    // find ship height
    int y;
    for (y=1; y < h; y++)
      if (getpixel(bit, 0, y) != topleft) break;
    shiph = y - 2;

    // start coords
    startpos[0] = 0;
    startpos[1] = startpos[0] + shipw + 1;
    startpos[2] = startpos[1] + shipw + 1;
    startpos[3] = startpos[2] + shipw + 1;

    // check them
    if (shipw <= 17 && shiph <= 17 &&
        shipw >= 5  && shiph >= 5 &&
        getpixel(bit, startpos[1], 0) == topleft &&
        getpixel(bit, startpos[2], 0) == topleft &&
        getpixel(bit, startpos[3], 0) == topleft)
    {
      // create images
      if (last_ship_type == SHIP_CUSTOM)
      {
        if (ship[0]) { destroy_bitmap(ship[0]); ship[0] = NULL; }
        if (ship[1]) { destroy_bitmap(ship[1]); ship[1] = NULL; }
        if (ship[2]) { destroy_bitmap(ship[2]); ship[2] = NULL; }
        if (ship[3]) { destroy_bitmap(ship[3]); ship[3] = NULL; }
      }

      ship[0] = create_bitmap(shipw, shiph);
      ship[1] = create_bitmap(shipw, shiph);
      ship[2] = create_bitmap(shipw, shiph);
      ship[3] = create_bitmap(shipw, shiph);
      width = owidth = shipw;
      height = oheight = shiph;

      // blit image data
      blit( bit, ship[0], startpos[0]+1, 1, 0, 0, shipw, shiph );
      blit( bit, ship[1], startpos[1]+1, 1, 0, 0, shipw, shiph );
      blit( bit, ship[2], startpos[2]+1, 1, 0, 0, shipw, shiph );
      blit( bit, ship[3], startpos[3]+1, 1, 0, 0, shipw, shiph );

      ship_type = SHIP_CUSTOM;
      strncpy(shipimage_file, get_filename(n), 50);
    }
    else
    {
      errors->log(0, "load ship image", "ERROR: wrong format!");
      ship_type = SHIP_PIRANHA;
      load_ship_image(ship_type, 0, last_ship_type);
      error = 1;
    }

    // is there a shield anim...? If so, load it!
    if (h > shiph+2)
    {
      int shieldw=0, shieldh=0;
      int shieldy=0;

      // find top-left
      for (shieldy=shiph+2; shieldy < h; shieldy++)
        if (getpixel(bit, 0, shieldy) == topleft) break;

      memset(&startpos, 0, sizeof(startpos));

      if (shieldy > shiph) // is it really there?
      {
        // find height
        for (y=shieldy; y < h; y++)
          if (getpixel(bit, 0, y) != topleft) break;
        shieldh = y - 2 - shieldy;

        // find width
        for (x=1; x < w; x++)
          if (getpixel(bit, x, shieldy+1) == topleft) break;
        shieldw = x - 1;

        // find last bounding line (from right)
        for (x=w; x > 0; x--)
          if (getpixel(bit, x, shieldy) == topleft) break;
        int lastbound = x;

        // how much frames...?
        int frameanz = (lastbound+1) / (shieldw+1);
//        errors->log(2, "load ship's shield: ", "frames:", frameanz);

        if (frameanz > 0)
        {
          if (frameanz > 10) frameanz = 10;

          shield_ok = 1;

          // create images
          for (int sc=0; sc < frameanz; sc++)
          {
            shield_frame[sc] = create_bitmap(shieldw, shieldh);
            blit(bit, shield_frame[sc], ((shieldw+1) * sc)+1, shieldy+1,
                                        0, 0, shieldw, shieldh);
          }
          max_shield_frame = frameanz - 1;

        } // if frames > 0
        else errors->log(2, "load ship's shield: ", "ERROR: no frames!");

      } // if there's a shield image...
//      else errors->log(2, "load ship's shield: ", "Warning: no shield image!");

    }

    if (!shield_ok) // no shield anim...
    {
      // init default shield frames
      for (int tnum=0; tnum <= 2; tnum++)
      {
        BITMAP *sf = (BITMAP *)globals->gamedat[ effect_shield_1 + tnum ].dat;
        shield_frame[tnum] = create_bitmap(sf->w, sf->h);
        blit(sf, shield_frame[tnum], 0, 0, 0, 0, sf->w, sf->h);
      }
    }

    destroy_bitmap(bit);
  }
  else
  {
    errors->log(0, "load ship image", "ERROR: file not found or no tga/pcx/bmp!");
    ship_type = SHIP_PIRANHA;
    load_ship_image(ship_type, 0, last_ship_type);
    error = 1;
  }

  // create the big debris pixels...
  create_debris_pixels();

  return error;
}

void gsPlayer::load_ship_image(predefined_ships s, int col, int last_ship_type)
{
  int first_ship = ship1_1_1;
  int myship = 0;

  if (last_ship_type == SHIP_CUSTOM)
  {
    if (ship[0]) { destroy_bitmap(ship[0]); ship[0] = NULL; }
    if (ship[1]) { destroy_bitmap(ship[1]); ship[1] = NULL; }
    if (ship[2]) { destroy_bitmap(ship[2]); ship[2] = NULL; }
    if (ship[3]) { destroy_bitmap(ship[3]); ship[3] = NULL; }
  }

  switch(s)
  {
    case SHIP_PIRANHA  : myship = 0; break;
    case SHIP_RAZOR    : myship = 1; break;
    case SHIP_BOMBER   : myship = 2; break;
    default            : myship = 0;
  }

  ship[0] = (BITMAP *)globals->shipdat[ first_ship + myship*20 + col*4 + 0 ].dat;
  ship[1] = (BITMAP *)globals->shipdat[ first_ship + myship*20 + col*4 + 1 ].dat;
  ship[2] = (BITMAP *)globals->shipdat[ first_ship + myship*20 + col*4 + 2 ].dat;
  ship[3] = (BITMAP *)globals->shipdat[ first_ship + myship*20 + col*4 + 3 ].dat;

  ship_type = s;
  ship_color = col;

  width = owidth = ship[0]->w;
  height = oheight = ship[0]->h;

  for (int nr=0; nr < 10; nr++) { if (shield_frame[nr]) destroy_bitmap(shield_frame[nr]); shield_frame[nr] = NULL; }

  // init default shield frames
  for (int tnum=0; tnum <= 2; tnum++)
  {
    BITMAP *sf = (BITMAP *)globals->gamedat[ effect_shield_1 + tnum ].dat;
    shield_frame[tnum] = create_bitmap(sf->w, sf->h);
    blit(sf, shield_frame[tnum], 0, 0, 0, 0, sf->w, sf->h);
  }

  create_debris_pixels();

  strncpy(shipimage_file, "", 40);
}

void gsPlayer::set_weapon(int n, int t, int bullets, int reload)
{
  if (n < REAL_WEAPON_COUNT && n >= 0)
  {
    weapon[n].type        = t;
    weapon[n].bullets     = bullets;
    weapon[n].reload      = reload;
    weapon[n].cur_reload  = 0;
    weapon[n].max_upgrades = 0;
    weapon[n].upgrade     = 0;
    weapon[n].active      = 1;
//    if (t == W_SINGLE) { weapon[n].bullets = -1; weapon[n].reload = -1; }
    if (ppanel) ppanel->update_weapons();
  }
  else
    errors->log(0, "ADDRESSING ERROR", "player->set_weapon", n);
}

void gsPlayer::set_active_weapon(int n)
{
  active_weapon = n;
  if (ppanel) ppanel->update_weapons();
}
/*
void gsPlayer::set_extra(int n, int t, int energy, int reload)
{
  if (n < 6 && n >= 0)
  {
    extra.type        = t;
    extra.energy      = energy;
    extra.reload      = reload;
    extra.cur_reload  = 0;
  }
  else
    errors->log(0, "ADDRESSING ERROR", "player->set_extra", n);
}
*/

void gsPlayer::return_home()
{
  set_x(home_x);
  set_y(home_y);
}

void gsPlayer::set_homebase(int n)
{
  homebase = n;
  Base *temp = bases->get_base(n);

  if (temp != NULL)
  {
    home_x = int(temp->get_land_x() + temp->get_land_w() / 2 - width/2);
    home_y = int(temp->get_land_y() - height - 2);
  }
}

void gsPlayer::set_frags(int n)
{
  stats.frags = n;
  if (ppanel) ppanel->update_frags();
}

void gsPlayer::inc_frags()
{
  stats.frags++;
  if (ppanel) ppanel->update_frags();
}

void gsPlayer::dec_frags()
{
  stats.frags--;
  if (ppanel) ppanel->update_frags();
}

void gsPlayer::draw_overlay()
{
  if (show_status_flag && !dead && !(extra_action && get_extra() == EXTRA_CLOAK)) overlay->draw();
}

void gsPlayer::restore_overlay()
{
  if (show_status_flag && !dead) overlay->restore();
}

void gsPlayer::save_overlay_back()
{
  if (show_status_flag && !dead) overlay->save();
}

void gsPlayer::show_status_set_map(class gsMap *m)
{
  overlay->set_map(m);
}

void gsPlayer::convert_percents_to_shipvalues()
{
  int i;
  float f;

  i = get_cur_percent_value(PV_RADAR_ZOOM);
  set_max_radar_zoom(i+10);

  i = get_cur_percent_value(PV_WEAPON_SLOTS);
  set_avail_weapon_slots(i);

  i = get_cur_percent_value(PV_HULL);
  set_max_hull(i);
  if (get_hull() > get_max_hull()) set_hull(get_max_hull());

  i = get_cur_percent_value(PV_SHIELD);
  set_max_shield(i);
  if (get_shield() > get_max_shield()) set_shield(get_max_shield());

  i = get_cur_percent_value(PV_SHIELD_RE);
//  f = (i / 10526.32) + 0.001;
  f = (i / 10526.32) + 0.001; // war: 15526.32
  if (i == 0) f = 0;
  set_shield_recharge(f);

  i = get_cur_percent_value(PV_HULL_RE);
//  f = (i / 15306) + 0.0002;
  f = (i / 15306.0) + 0.0002; // war: 18306
  if (i == 0) f = 0;
  set_hull_repair(f);

  i = get_cur_percent_value(PV_WALLSHIELD);
  f = (i / 25000.0) + 0.0002;
  if (i == 0) f = 0;
  set_wall_shield_recharge(f);

  i = get_cur_percent_value(PV_TURN_SPEED);
//  f = i / 10.0;
  f = i / 10.0;
  set_turn_speed(f);

  i = get_cur_percent_value(PV_MAX_SPEED);
//  f = i / 8.33;
  f = i / 11.0;
  set_max_speed(f);

  i = get_cur_percent_value(PV_ACCELERATION);
//  f = (i / 526.32) + 0.01;
  f = (i / 700.0) + 0.01;
  set_player_acc(f);

  i = get_cur_percent_value(PV_FUEL_CONS);
  f = 0.13 - ((i / 1052.63) + 0.005);
//  set_fuel_dec(f/1.7);
  set_fuel_dec(f/2.0);
  set_extra_recharge((float)i / (60.0*40.0)); // i / ticks to full extra

  i = get_cur_percent_value(PV_WEAPON_STORAGE);
  set_weapon_storage(i/100.0);

  i = get_cur_percent_value(PV_WEAPON_RE);
  set_weapon_recharge(i/100.0);
}

void gsPlayer::set_ship_value(int what, int val)
{
  switch (what)
  {
    case PV_HULL_RE :
    case PV_SHIELD_RE :
    case PV_WALLSHIELD :
    case PV_WEAPON_RE :
    case PV_TURN_SPEED :
    case PV_MAX_SPEED :
    case PV_ACCELERATION :
    case PV_FUEL_CONS :
    case PV_WEAPON_STORAGE :
         if (val > 100) val = 100;
         if (val < 0) val = 0;
         break;

    case PV_HULL :
    case PV_SHIELD :
    case PV_RADAR_ZOOM :
         if (val > 20) val = 20;
         if (val < 0) val = 0;
         break;

    case PV_WEAPON_SLOTS :
         if (val > 6) val = 6;
         if (val < 0) val = 0;
         break;
  } // switch

  set_percent_value(what, val);
  convert_percents_to_shipvalues();
  if (ppanel) ppanel->update_all();
}

int gsPlayer::get_ship_value(int what)
{
  return get_percent_value(what);
}

int get_player_nr_by_sig(long s)
{
  for (int n=0; n < globals->players; n++)
    if (player[n] && player[n]->get_sig() == s)
      return n;
  return -1;
}

gsPlayer *get_player_by_sig(long s)
{
  for (int n=0; n < globals->players; n++)
    if (player[n]->get_sig() == s)
      return player[n];
  return NULL;
}
/*
int gsPlayer::check_for_extra(int m)
{
//  for (int n=0; n < 5; n++)
    if (extra.type == m) return 1;

  return 0;
}
*/
void gsPlayer::turn_to(int px, int py)
{
  if (xpos != px || ypos != py)
  {
    head_to = (V_GetAngle(px, py, (int)xpos, (int)ypos)) / PI * 180;
    if (xpos >= px) head_to = 360-head_to;
  }
}

void gsPlayer::set_standard_bullets()
{
  weaponslot *slot;
  for (int i=0; i < 6; i++)
  {
    slot = get_weapon_slot(i);

    switch (slot->type)
    {
      case W_SINGLE :      slot->bullets = -1;
                           slot->reload = -1;
                           break;
      case W_DOUBLE :      slot->bullets = 1500;
                           slot->reload = 30;
                           break;
      case W_BOMB   :      slot->bullets = 300;
                           slot->reload = 60*3;
                           break;
      case W_BIGBOMB:      slot->bullets = 175;
                           slot->reload = 60*5;
                           break;
      case W_ROCKET :      slot->bullets = 150;
                           slot->reload = 60*4;
                           break;
      case W_MINE   :      slot->bullets = 200;
                           slot->reload = 60;
                           break;
      case W_FREEZER:      slot->bullets = 125;
                           slot->reload = 60*5;
                           break;
      case W_SHRINKER:     slot->bullets = 125;
                           slot->reload = 60*5;
                           break;
      case W_PROBE  :      slot->bullets = 100;
                           slot->reload = 60*10;
                           break;
      case W_GAS    :      slot->bullets = 100;
                           slot->reload = 60*10;
                           break;
      case W_LASER  :      slot->bullets = 1000;
                           slot->reload = 25;
                           break;
      case W_BUMPER :      slot->bullets = 300;
                           slot->reload = 60*3;
                           break;
      case W_SHOTGUN :     slot->bullets = 300;
                           slot->reload = 40;
                           break;
      case W_ACID  :       slot->bullets = 300;
                           slot->reload = 70;
                           break;
    }

    if (slot->type != W_NONE)
    {
      int tmp_reload = 0;
      int myslots = get_avail_weapon_slots();
      if (myslots == 0) myslots = 1;
      if (slot->bullets != -1) slot->bullets = int((slot->bullets / myslots) * weapon_storage);

      if (slot->reload != -1)
      {
        tmp_reload  = int(slot->reload  * (1.1-weapon_recharge));

        if (tmp_reload < 25 &&
           (slot->type == W_BIGBOMB || slot->type == W_ROCKET ||
            slot->type == W_FREEZER || slot->type == W_SHRINKER))
          tmp_reload = 25;
  
        slot->reload = tmp_reload;
      }
    }

    set_weapon_slot(i, slot);

  } // for
}

void gsPlayer::apply_weapon_pack(int my_extra_type, int locrand)
{
  weaponslot *slot;
  int i;
  char ttext1[100], ttext2[100];

  //int locrand = gsrand();

  OsdBitmap *to = dmap->osd->get_osdbit(1000);

  // weapon upgrade
  if (my_extra_type == -1 && get_avail_weapon_slots() > 0)
  {
    int myokay = 0;
    int upgrade_possible[6] = {0};
    int upgrade_possible_count = 0;
    
    // check which slots are upgradable
    for (i=0; i < get_avail_weapon_slots(); i++)
    {
      slot = get_weapon_slot(i);
      if (slot->max_upgrades < WEAPON_UPGRADES)
      {
        upgrade_possible[upgrade_possible_count] = i;
        upgrade_possible_count++;
      }
    }
    
    if (upgrade_possible_count > 0)
    {
      // do upgrade
      //int myr = myrand(upgrade_possible_count);
      int myr = locrand % upgrade_possible_count;
      slot = get_weapon_slot(upgrade_possible[myr]);
      slot->max_upgrades++;
      slot->upgrade = slot->max_upgrades;
      set_weapon_slot(upgrade_possible[myr], slot);
      set_active_weapon(upgrade_possible[myr]);

      // write message if local player
      if (!globals->network_game ||
          (globals->network_game && player[0]->get_sig() == sig))
      {
        to->clear();
        strncpy(ttext1, language->get_string(T_GAME_EXTRA_UPGRADE), 50);
        strncpy(ttext2, language->get_string(TEXT_STRINGS(T_MENU_SHIP_W_SBLASTER + slot->type)), 50);
        to->draw_text(FONT_IMPACT10, get_name(), globals->col_green, 0, 0, 1);
        to->draw_text(FONT_IMPACT14, ttext1, globals->col_yellow, 0, 17, 1);
        to->draw_text(FONT_IMPACT14, ttext2, globals->col_white, 0, 38, 1);
      }
    }
    else
    {
      // no upgrade possible? convert to ammo pack...
      //my_extra_type = 25-((gsrand()%10)*2);
      my_extra_type = 25-((locrand % 10)*2);
    }
  }
  
  // full fuel
  else if (my_extra_type == -2)
  {
    if (fuel < max_fuel/1.5)
    {
      fuel = max_fuel;
      if (ppanel) ppanel->update_fuel();
      if (!globals->network_game ||
          (globals->network_game && player[0]->get_sig() == sig))
      {
        to->clear();
        strncpy(ttext1, language->get_string(T_GAME_EXTRA_FUEL), 50);
        to->draw_text(FONT_IMPACT10, get_name(), globals->col_green, 0, 0, 1);
        to->draw_text(FONT_IMPACT14, ttext1, globals->col_yellow, 0, 17, 1);
      }
    }
    else
    {
      // no fuel needed -> give ammopack
      //my_extra_type = 25-((gsrand()%10)*2);
      my_extra_type = 25-((locrand % 10)*2);
    }
  }
  
  // full shield & hull
  else if (my_extra_type == -3)
  {
    if (shield < get_max_shield(1)-get_max_shield(1)/10 || hull < get_max_hull(1)-get_max_hull(1)/10)
    {
      shield = get_max_shield(1);
      hull = get_max_hull(1);
      if (ppanel) ppanel->update_shields();
      if (!globals->network_game ||
          (globals->network_game && player[0]->get_sig() == sig))
      {
        to->clear();
        strncpy(ttext1, language->get_string(T_GAME_EXTRA_FULLSHIELD), 50);
        to->draw_text(FONT_IMPACT10, get_name(), globals->col_green, 0, 0, 1);
        to->draw_text(FONT_IMPACT14, ttext1, globals->col_yellow, 0, 17, 1);
        damage = DAMAGE_NONE;
      }
    }
    else
    {
      // no hullshield upgrade needed -> give ammopack
      //my_extra_type = 25-((gsrand()%10)*2);
      my_extra_type = 25-((locrand % 10)*2);
    }
  }

  // full wall shields
  else if (my_extra_type == -4)
  {
    if (wall_shield <= max_wall_shield/2)
    {
      wall_shield = max_wall_shield;
      if (ppanel) ppanel->update_shields();
      if (!globals->network_game ||
          (globals->network_game && player[0]->get_sig() == sig))
      {
        to->clear();
        strncpy(ttext1, language->get_string(T_GAME_EXTRA_FULLWSHIELD), 50);
        to->draw_text(FONT_IMPACT10, get_name(), globals->col_green, 0, 0, 1);
        to->draw_text(FONT_IMPACT14, ttext1, globals->col_yellow, 0, 17, 1);
      }
    }
    else
    {
      // no wallshield upgrade needed -> give ammopack
      //my_extra_type = 25-((gsrand()%10)*2);
      my_extra_type = 25-((locrand % 10)*2);
    }
  }

  // indestructibility (15-25 seconds)
  else if (my_extra_type == -5)
  {
    //int rndtime = myrand(11) + 15;
    int rndtime = locrand % 11 + 15;
    indestructibility = 60 * rndtime;
    if (!globals->network_game ||
        (globals->network_game && player[0]->get_sig() == sig))
    {
      to->clear();
      sprintf(ttext1, "%s (%d s)", language->get_string(T_GAME_EXTRA_INVULNERABILITY), rndtime);
      to->draw_text(FONT_IMPACT10, get_name(), globals->col_green, 0, 0, 1);
      to->draw_text(FONT_IMPACT14, ttext1, globals->col_yellow, 0, 17, 1);
    }
  }

  // ammo
  if (my_extra_type > 0)
  {    
    int strength = my_extra_type;
    int addbullets = 0;
    if (get_avail_weapon_slots() > 0)
    for (i=0; i < 6; i++)
    {
      slot = get_weapon_slot(i);
  
      if (slot->bullets != -1) // do nothing if unlimited bullets
      {
        switch (slot->type)
        {
          case W_SINGLE :      addbullets = strength * 15 * 3;
                               break;
          case W_DOUBLE :      addbullets = strength * 12 * 3;
                               break;
          case W_BOMB   :      addbullets = strength * 3 * 3;
                               break;
          case W_BIGBOMB:      addbullets = strength * 2 * 3;
                               break;
          case W_ROCKET :      addbullets = int(strength * 1.3) * 3;
                               break;
          case W_MINE   :      addbullets = strength * 2 * 3;
                               break;
          case W_FREEZER:      addbullets = int(strength * 1.5) * 3;
                               break;
          case W_SHRINKER:     addbullets = int(strength * 1.5) * 3;
                               break;
          case W_PROBE  :      addbullets = int(strength / 2.5) * 3;
                               break;
          case W_GAS    :      addbullets = int(strength / 1.7 * 3);
                               break;
          case W_LASER  :      addbullets = strength * 14 * 3;
                               break;
          case W_BUMPER :      addbullets = strength * 5 * 3;
                               break;
          case W_SHOTGUN:      addbullets = strength * 4 * 3;
                               break;
          case W_ACID   :      addbullets = strength * 4 * 3;
                               break;
        }
    
        slot->bullets += int((addbullets / get_avail_weapon_slots()) * weapon_storage);
      } // if not unlimited
    } // for

    if (!globals->network_game ||
        (globals->network_game && player[0]->get_sig() == sig))
    {
      to->clear();
      sprintf(ttext1, "%s (%d)", language->get_string(T_GAME_AMMOPACK), strength);
      to->draw_text(FONT_IMPACT10, get_name(), globals->col_green, 0, 0, 1);
      to->draw_text(FONT_IMPACT14, ttext1, globals->col_yellow, 0, 17, 1);
    }
  }


  to->set_active(1);
  to->set_timeout(60*2);

  if (ppanel) ppanel->update_weapons();
}

void gsPlayer::put_extra()
{
  static int extranum = 1000;

  if (globals->extra_amount)
  if (myrand(6-(globals->extra_amount/2)) == 0)
  {
    int my_extra_type = 0;
    int rr = myrand(10);
    int better_extra = myrand(6)+1;

    switch (better_extra)
    {
      case 1     :     my_extra_type = -1; break; // weapon_upgrade
      case 2     :     my_extra_type = -2; break; // fuel upgrade
      case 3     :     my_extra_type = -3; break; // full hull & shield
      case 4     :     my_extra_type = -4; break; // full wall shield
      case 5     :     my_extra_type = -5; break; // indestructibility
      default    :     my_extra_type = 25-(rr*2); // ammo
    }
  
    int extra_random = gsrand();
    Object *myob = objects->add_special(extranum, SPOBJ_EXTRA, xpos, ypos, my_extra_type, rr+1);
    myob->set_user2(extra_random);
    
    if (globals->network_game && player[0]->get_sig() == sig)
      net->game_send_extra(my_extra_type, rr, xpos, ypos, extra_random);

  }
}

void gsPlayer::choose_next_available_weapon(int prev_weap)
{
  weaponslot *tmp;
  int n;

  if (prev_weap > 0)
  for (n=prev_weap-1; n >= 0; n--)
  {
    if (n >= 0 && n <= 6)
    {
      tmp = get_weapon_slot(n);
      if (tmp->type != W_NONE && tmp->bullets != 0)
      {
        active_weapon = n;
        return;
      }
    }
  }

  if (prev_weap < 6)
  for (n=6; n > prev_weap; n--)
  {
    if (n >= 0 && n <= 6)
    {
      tmp = get_weapon_slot(n);
      if (tmp->type != W_NONE && tmp->bullets != 0)
      {
        active_weapon = n;
        return;
      }
    }
  }

}

void gsPlayer::choose_next_weapon(int dir)
{
  for (int n=0; n <= 6; n++)
  {
    if (dir == 0) active_weapon--; else active_weapon++;
    if (active_weapon < 0) active_weapon = 6;
    else if (active_weapon > 5) active_weapon = 0;

    if (weapon[active_weapon].type != W_NONE && weapon[active_weapon].bullets != 0)
      break;
  }
}

void gsPlayer::set_name(const char *a)
{
  strncpy(name, a, 30); strupr(name);
  overlay->update_player_name();
  strncpy(stats.pname, a, 50);
}

void gsPlayer::set_lifes(int n)
{
  lifes = n;
  if (ppanel) ppanel->update_lifes();
}

void gsPlayer::set_extra_power(int n)
{
  if (n <= max_extra_power &&  n >= 0) extra_power = n;
  if (ppanel) ppanel->update_extra_energy();
}

void gsPlayer::set_fuel(float n)
{
  if (n <= max_fuel && n >= 0) fuel = n;
  if (ppanel) ppanel->update_fuel();
}


void gsPlayer::apply_gravity_to(int px, int py, float *pxs, float *pys, int st, int ptype)
{
  float angle,scale_x,scale_y,xrot,yrot;
  int gx,gy;
  float s;
  float pi = 3.1415926;

  // grav position is center of ship
  gx = (int)xpos+width/2;
  gy = (int)ypos+height/2;

  angle = V_GetAngle(px,py,gx,gy);
  xrot = sinus[int(angle*180.0/pi)];
  yrot = cosinus[int(angle*180.0/pi)];

  if (px >= gx)
    scale_x = -xrot;
  else
    scale_x = xrot;

  scale_y = yrot;

  if (st > 100) st = 100; if (st <= 0) st = 1;
  s = st/100.0;
  
  int maxdist = 100;
  int dist = (int)distance((int)gx, (int)gy, (int)px, (int)py);
  if (dist > maxdist) dist = maxdist;
  
  float d = float(float(dist)/float(maxdist));
  float a = 1-d;
  s = a * s;
  
  if (s != 0)
  {
    *pxs -= scale_x*s;
    if (ptype == 0)
      *pys -= scale_y*s;
    else
      *pys += scale_y*s;
  }
}

void gsPlayer::apply_gravity(int st)
{
  float xs, ys;

  // players
  for (int n=0; n < globals->players; n++)
  {
    if (player[n]->get_sig() != sig)
    {
      xs = player[n]->get_xspd();
      ys = player[n]->get_yspd();

      if (!player[n]->get_land())
      {
        apply_gravity_to( player[n]->get_x()+player[n]->get_width()/2, player[n]->get_y()+player[n]->get_height()/2,
                          &xs, &ys, st, 0 );

        player[n]->set_xspd(xs);
        player[n]->set_yspd(ys);
      }
    }
  }

  // foreign bullets
  Bullet *bul = effects->bullets->get_first(), *nextbul;
  while (bul)
  {
    nextbul = bul->get_next();

    if (bul->get_by_player() != sig)
    {
      xs = bul->get_xspd();
      ys = bul->get_yspd();

      apply_gravity_to( bul->get_x()+bul->get_width()/2, bul->get_y()+bul->get_height()/2,
                        &xs, &ys, st, 1 );

      bul->set_xspd(xs);
      bul->set_yspd(ys);
    }
    
    bul = nextbul;
  }
}

void gsPlayer::set_handicap(int n)
{
  if (n >= 0 && n <= 10) 
  { 
    handicap = n; 
    if (ppanel) ppanel->update_all();
    
    if (get_hull() > get_max_hull(1)) set_hull(get_max_hull(1));
    if (get_shield() > get_max_shield(1)) set_shield(get_max_shield(1));
  }  
}
  

/*
void gsPlayer::do_handicaps()
{
  // new ship value = old ship value - (old ship value * (handicap * 0.8 / 10.0))
  int what;
  
  what = PV_HULL;         set_cur_percent_value( what, get_percent_value(what) - (get_percent_value(what) * (handicap * 0.8 / 10.0)) );
  what = PV_SHIELD;       set_cur_percent_value( what, get_percent_value(what) - (get_percent_value(what) * (handicap * 0.8 / 10.0)) );
  what = PV_HULL_RE;      set_cur_percent_value( what, get_percent_value(what) - (get_percent_value(what) * (handicap * 0.8 / 10.0)) );
  what = PV_SHIELD_RE;    set_cur_percent_value( what, get_percent_value(what) - (get_percent_value(what) * (handicap * 0.8 / 10.0)) );
  what = PV_WALLSHIELD;   set_cur_percent_value( what, get_percent_value(what) - (get_percent_value(what) * (handicap * 0.8 / 10.0)) );
  what = PV_WEAPON_RE;    set_cur_percent_value( what, get_percent_value(what) - (get_percent_value(what) * (handicap * 1.0 / 10.0)) );
  what = PV_TURN_SPEED;   set_cur_percent_value( what, get_percent_value(what) - (get_percent_value(what) * (handicap * 0.8 / 10.0)) );
  what = PV_MAX_SPEED;    set_cur_percent_value( what, get_percent_value(what) - (get_percent_value(what) * (handicap * 0.8 / 10.0)) );
  what = PV_ACCELERATION; set_cur_percent_value( what, get_percent_value(what) - (get_percent_value(what) * (handicap * 0.8 / 10.0)) );
  what = PV_FUEL_CONS;    set_cur_percent_value( what, get_percent_value(what) - (get_percent_value(what) * (handicap * 0.8 / 10.0)) );
  
  convert_percents_to_shipvalues();
  if (ppanel) ppanel->update_all();
}
*/
