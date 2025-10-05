/*
 *   GRAVITY STRIKE -- enemies/erelash.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "erelash.h"
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

enemyShipRelash::enemyShipRelash(int px, int py, float pfreq, float pradius, float pspeed, float pstrength, float targplayer, float user6)
{
  type          = E_SHIP_RELASH;

  f_add         = 1;
  xradius = yradius = (int)pradius;
  bullet_speed  = pspeed;
  bullet_strength = pstrength;
  bullet_hitp_hull = bullet_hitp_shield = (int)bullet_strength;
  bullet_freq = (int)pfreq;
  cur_bullet_freq = 0;

  maxhit        = 50;

  x             = px;
  y             = py;

  bound_x       = 0;
  bound_y       = 0;
  bound_w       = 0;
  bound_h       = 0;

  image = enemy_n1_1;

  maxframet = 15;
  maxframe = 4;

  physical_frames = 5;

  w = ow = ((RLE_SPRITE*)globals->gamedat[image].dat)->w;
  h = oh = ((RLE_SPRITE*)globals->gamedat[image].dat)->h;

  frames->add_frames_from_datafile(globals->gamedat, DATA_RLE, image, physical_frames, 0, 0, maxframet);

  curframe = startframe;
}

enemyShipRelash::~enemyShipRelash()
{
}

void enemyShipRelash::move()
{
  Enemy::move();
  if (freezed)
  {
    x -= xspd;
    y -= yspd;
  }
}

void enemyShipRelash::update()
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
        if (curframe > maxframe) { curframe = 0; }
      }
    }

    if (active && cur_bullet_freq >= bullet_freq)
    {
      shoot();
      cur_bullet_freq = 0;
    }
    cur_bullet_freq++;
    dispframe = curframe;

    if (onehit) onehit = FALSE;
  }

  move();
}

void enemyShipRelash::shoot()
{
  if (globals->do_all_hooks || do_hooks)
    script->do_hook_enemy_shoot(glob_lua, this);
  if (get_dont_continue()) return;

  int pxpos = (int)x+w/2, pypos = (int)y+h/2;

  gsound->play_sound(SOUND_SHOOT_SINGLE, pxpos, pypos);
  effects->bullets->add(W_BIGBOMB, psig, nr, pxpos, pypos, 0, 0, bullet_strength,  bullet_hitp_shield, bullet_hitp_hull, 0, 0);
}

int enemyShipRelash::hit(long fromplayer, int fromenemy, float n, int fx, int fy, float fxspd, float fyspd, int weapon)
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
  float percent = hitpoints / maxhit;
  if (percent == 0) percent = 1;

  gsound->play_sound(SOUND_SHIP_HIT, fx, fy);

  effects->explosions->add(-1, expl_sm2_01, 3, 2, 1, fx, fy, globals->gamedat);
  effects->pixels->add(n*2*percent, fx, fy, 0, 0, -fxspd/4, fyspd/4, 50, globals->tbl_white, 10, 60*3, 30);
  effects->pixels->add(n*percent, fx, fy, 0, 0, -fxspd/4, fyspd/4, 50, globals->tbl_blue, 10, 60*3, 30);
  if (percent > 0.7) effects->pixels->add(1 + n/15, fx, fy, 1, 1,
                                          -fxspd/5, fyspd/5, 50,
                                          (int)PIX_DEBRIS3);

  return 1;
}

void enemyShipRelash::dead()
{
  if (globals->do_all_hooks || do_hooks)
    script->do_hook_enemy_dead(glob_lua, this);
  if (get_dont_continue()) return;

  if (hitby > 0) 
  {
    gsPlayer *tpl = get_player_by_sig(hitby);
    if (tpl) tpl->inc_enemy_bonus(200);
  }

  int div = 1;
  if (shrinked) div = 3;

  gsound->play_sound(SOUND_EXPL_BIG1, x, y);

  for (int n=0; n < 10/div; n++)
  {
    effects->explosions->add(EXP_MED_1, 4, gsrand()%40, x+gsrand()%w, y+gsrand()%h);
    effects->pixels->add(100/div, x+gsrand()%w, y+gsrand()%h, 10, 10, 2-gsrand()%4, 2-gsrand()%4, 50, globals->tbl_white, 10, 60*5, 30);
    effects->pixels->add(20/div, x+gsrand()%w, y+gsrand()%h, 10, 10,
                         2-gsrand()%4, 2-gsrand()%4, 50,
                         (int)PIX_DEBRIS3);
    effects->pixels->add(10/div, x+gsrand()%w, y+gsrand()%h, 10, 10,
                         2-gsrand()%4, 2-gsrand()%4, 50,
                         (int)PIX_DEBRIS1);
  }
}

