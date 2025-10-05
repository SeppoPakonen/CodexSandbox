/*
 *   GRAVITY STRIKE -- enemies/eblue1.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "eblue1.h"
#include "../gsmap.h"
#include "../gsclear.h"
#include "../gsplayer.h"
#include "../gsglob.h"
#include "../gshelper.h"
#include "../gseffect.h"
#include "../gssound.h"
#include "../gserror.h"
#include "../gsbullet.h"
#include "../gseffect.h"

#include <math.h>

enemyShipBlue1::enemyShipBlue1(int px, int py, float pfreq, float pradius, float pspeed, float pstrength, float targplayer, float weaptype)
{
  type          = E_SHIP_BLUE1;

  f_add         = 1;
  xradius = yradius = (int)pradius;
  bullet_speed  = pspeed;
  bullet_strength = pstrength;
  bullet_hitp_hull = bullet_hitp_shield = bullet_strength;
  
  bullet_freq = (int)pfreq;
  bullet_type = (int)weaptype;
  if (!bullet_type) bullet_type = W_SINGLE;
  cur_bullet_freq = 0;

  maxhit        = 20;

  x             = px;
  y             = py;

  bound_x       = 0;
  bound_y       = 0;
  bound_w       = 0;
  bound_h       = 0;

  image = enemy_s1_01;

  maxframet = 10;
  maxframe = 4;

  physical_frames = 5;

  w = ow = ((RLE_SPRITE*)globals->gamedat[image].dat)->w;
  h = oh = ((RLE_SPRITE*)globals->gamedat[image].dat)->h;

  frames->add_frames_from_datafile(globals->gamedat, DATA_RLE, image, physical_frames, 0, 0, maxframet);

  curframe = startframe;
}

enemyShipBlue1::~enemyShipBlue1()
{
}

void enemyShipBlue1::move()
{
  Enemy::move();
  if (freezed)
  {
    x -= xspd;
    y -= yspd;
  }
}

void enemyShipBlue1::update()
{
  Enemy::update();

  if (globals->do_all_hooks || do_hooks)
    script->do_hook_enemy_update(glob_lua, this);
  if (kill || get_dont_continue()) return;

  active = 0;
  if (xradius && yradius)
  for (int n=0; n < globals->players; n++)
  {
    if (player[n]->get_sig() != psig &&
        !(player[n]->get_extra_action() && player[n]->get_extra() == EXTRA_CLOAK) &&
        player[n]->get_x() > x+w/2-xradius/2 &&
        player[n]->get_x() < x+w/2+xradius/2 &&
        player[n]->get_y() > y+h/2-yradius/2 &&
        player[n]->get_y() < y+h/2+yradius/2)
    {
      active = 1;
      break;
    }
  }

  if (shrinked || freezed) active = 0;

  if (maxframe && maxframet)
  {
    if (!freezed)
    {
      curframet++;
      if (curframet == maxframet)
      {
        curframe += f_add;
        curframet = 0;
        if (curframe == maxframe || curframe == 0) { f_add = -f_add; }
      }
    }

    if (active && cur_bullet_freq > bullet_freq)
    {
      shoot();
      cur_bullet_freq = 0;
    }
    if (bullet_freq) cur_bullet_freq++;
    dispframe = curframe;

    if (onehit) onehit = FALSE;
  }

  move();
}

void enemyShipBlue1::shoot()
{
  if (globals->do_all_hooks || do_hooks)
    script->do_hook_enemy_shoot(glob_lua, this);
  if (get_dont_continue()) return;

  float pxspd = 0, pyspd = 0;
  int pxpos = (int)x+w/2, pypos = (int)y+h/2;

  if (fabs(bullet_speed) >= 1)
    while (fabs(pxspd) + fabs(pyspd) < 0.5)
    {
      pxspd = bullet_speed/2 - (gsrand() % (int)bullet_speed);
      pyspd = bullet_speed/2 - (gsrand() % (int)bullet_speed);
    }

  if (bullet_type == W_DOUBLE) bullet_type = W_SINGLE;
  switch (bullet_type)
  {
    case W_SINGLE :
         gsound->play_sound(SOUND_SHOOT_SINGLE, pxpos, pypos); break;
    case W_BOMB:
         gsound->play_sound(SOUND_SHOOT_BOMB, pxpos, pypos); break;
    case W_BIGBOMB:
         gsound->play_sound(SOUND_SHOOT_BIGBOMB, pxpos, pypos); break;
    case W_ROCKET:
         gsound->play_sound(SOUND_SHOOT_ROCKET, pxpos, pypos); break;
    case W_MINE:
         gsound->play_sound(SOUND_SHOOT_MINE, pxpos, pypos); break;
    case W_FREEZER:
         gsound->play_sound(SOUND_SHOOT_FREEZER, pxpos, pypos); break;
    case W_SHRINKER:
         gsound->play_sound(SOUND_SHOOT_SHRINKER, pxpos, pypos); break;
    case W_PROBE:
         gsound->play_sound(SOUND_SHOOT_PROBE, pxpos, pypos); break;
    case W_GAS:
         gsound->play_sound(SOUND_SHOOT_GAS, pxpos, pypos); break;
    case W_LASER:
         gsound->play_sound(SOUND_SHOOT_LASER, pxpos, pypos); break;
    case W_BUMPER:
         gsound->play_sound(SOUND_SHOOT_BUMPER, pxpos, pypos); break;
    case W_SHOTGUN:
         gsound->play_sound(SOUND_SHOOT_SHOTGUN, pxpos, pypos); break;
    case W_ACID:
         gsound->play_sound(SOUND_SHOOT_ACID, pxpos, pypos); break;
  }

  effects->bullets->add(bullet_type, psig, nr, pxpos, pypos, pxspd, pyspd, bullet_strength, bullet_hitp_shield, bullet_hitp_hull, 0, 0);
}

int enemyShipBlue1::hit(long fromplayer, int fromenemy, float n, int fx, int fy, float fxspd, float fyspd, int weapon)
{
  if (fromenemy == nr) return 0;
  if (fromplayer == psig && psig != -1) return 1;
  if (fromplayer == 0 && fromenemy != 0) return 1;

  if (fromplayer > 0 && n > 0)
  {
    gsPlayer *tpl = get_player_by_sig(fromplayer);
    if (tpl) tpl->inc_bullets_hit();
  }

  if (n < 0) n = -n;

  onehit = TRUE;
  hitby = fromplayer;
  hitby_sx = fxspd;
  hitby_sy = fyspd;

  if (globals->do_all_hooks || do_hooks)
    script->do_hook_enemy_hit(glob_lua, this, weapon, n, fx, fy);
  if (get_dont_continue()) return 1;

  if (weapon == W_FREEZER)
  {
    set_freezed(60*5);
    return 1;
  }

  if (weapon == W_SHRINKER)
  {
    if (maxhit-hitpoints <= 20) set_shrinked(60*3);
    else hitpoints += 15;
    return 1;
  }

  hitpoints += n;

  gsound->play_sound(SOUND_SHIP_HIT, fx, fx);

  effects->explosions->add(-1, expl_sm2_01, 3, 2, 1, fx, fy, globals->gamedat);
  effects->pixels->add(10, fx, fy, 0, 0, -fxspd/4, fyspd/4, 50, globals->tbl_blue, 10, 60*3, 30);

  return 1;
}

void enemyShipBlue1::dead()
{
  if (globals->do_all_hooks || do_hooks)
    script->do_hook_enemy_dead(glob_lua, this);
  if (get_dont_continue()) return;

  if (hitby > 0) 
  {
    gsPlayer *tpl = get_player_by_sig(hitby);
    if (tpl) tpl->inc_enemy_bonus(50);
  }

  int div = 1;
  if (shrinked) div = 3;

  gsound->play_sound(SOUND_EXPL_MEDIUM1+gsrand()%2, x, y);

  for (int n=0; n < 10/div; n++)
  {
    effects->explosions->add(EXP_MED_1, 4, gsrand()%30, x+gsrand()%w, y+gsrand()%h);
    effects->pixels->add(60/div, x+gsrand()%w, y+gsrand()%h, 10, 10, 2-gsrand()%4, 2-gsrand()%4, 50, globals->tbl_blue, 10, 60*5, 30);
    effects->pixels->add(10/div, x+gsrand()%w, y+gsrand()%h, 10, 10,
                         2-gsrand()%4, 2-gsrand()%4, 50,
                         (int)PIX_DEBRIS1);
  }
}

