/*
 *   GRAVITY STRIKE -- enemies/eflak.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "eflak.h"
#include "../gsmap.h"
#include "../gsclear.h"
#include "../gsplayer.h"
#include "../gsglob.h"
#include "../gshelper.h"
#include "../gseffect.h"
#include "../gssound.h"
#include "../gsnetw.h"
#include "../gsbullet.h"

enemyFlak::enemyFlak(int px, int py, float pshootrand, float pfreq, float pradius, float pspeed, float pstrength, float user6)
{
  type          = E_TOWER_FLAK;

  xradius = yradius = (int)pradius;
  bullet_speed  = pspeed;
  bullet_strength = pstrength;
  bullet_hitp_hull = bullet_hitp_shield = (int)bullet_strength;
  bullet_freq = (int)pfreq;
  cur_bullet_freq = 0;

  maxhit        = 15;

  x             = px;
  y             = py;

  bound_x       = 0;
  bound_y       = 0;
  bound_w       = 0;
  bound_h       = -1;

  shoot_random = (int)pshootrand;

  image = shooter_flak_1;

  maxframet = 2;
  maxframe = (int)pfreq;

  physical_frames = 3;

  w = ow = ((RLE_SPRITE*)globals->gamedat[image].dat)->w;
  h = oh = ((RLE_SPRITE*)globals->gamedat[image].dat)->h;

  frames->add_frames_from_datafile(globals->gamedat, DATA_RLE, image, physical_frames, 0, 0, maxframet);

  curframe = startframe;
}

enemyFlak::~enemyFlak()
{
}

void enemyFlak::move()
{
}

void enemyFlak::update()
{
  Enemy::update();

  maxframe = bullet_freq;

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
      active = 1;
  }

  if (shrinked || freezed) active = 0;

  if (maxframe && maxframet && active)
  {
    curframet++;
    if (curframet >= maxframet) { curframe++; curframet = 0; }
    if (curframe > maxframe)  { curframe = startframe; }
    if (curframe > 2)
    {
      dispframe = 0;
      if (curframe == 3 && curframet == 0) shoot();
    }
    else dispframe = curframe;

    if (onehit) onehit = FALSE;
  }
}

void enemyFlak::shoot()
{
  if (globals->do_all_hooks || do_hooks)
    script->do_hook_enemy_shoot(glob_lua, this);
  if (get_dont_continue()) return;

  float pxspd = 0, pyspd = 0;
  int pxpos = (int)x, pypos = (int)y;

  pyspd = bullet_speed; pxspd = 0;
  pxpos += 5; pypos -= 3;

  gsound->play_sound(SOUND_SHOOT_SINGLE, pxpos, pypos);
  Bullet *tempb = effects->bullets->add(W_BOMB, psig, nr, pxpos, pypos, pxspd, pyspd, bullet_strength, bullet_hitp_shield, bullet_hitp_hull, 0, 0);
  tempb->set_ttl(shoot_random - 30 + gsrand()%60);
}

int enemyFlak::hit(long fromplayer, int fromenemy, float n, int fx, int fy, float fxspd, float fyspd, int weapon)
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
    set_freezed(60*6);
    return 1;
  }

  if (weapon == W_SHRINKER)
  {
    set_shrinked(60*5);
    return 1;
  }

  hitpoints += n;

  gsound->play_sound(SOUND_SHIP_HIT, fx, fy);

  effects->explosions->add(-1, expl_sm2_01, 3, 2, 1, fx, fy, globals->gamedat);
  effects->pixels->add(12, fx, fy, 0, 0, -fxspd/4, fyspd/4, 50, globals->tbl_white, 10, 60*3, 30);

  return 1;
//  script->do_hook_enemy_hit(glob_lua, this);
}


void enemyFlak::dead()
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

  gsound->play_sound(SOUND_EXPL_SMALL1+gsrand()%2, x, y);

  effects->explosions->add(-1, expl_sm2_01, 10, 2, 1, x+w/2-3, y+h/2-3, globals->gamedat);
  if (!globals->network_game || net->is_server()) 
    effects->craters->add(x+w/2, y+h-4, 15, 1);
  effects->pixels->add(120/div, x+w/2, y+h/2, 0, 0, 2-(gsrand() % 3), gsrand() % 2, 50, globals->tbl_white, 10, 60*3, 50);
  effects->pixels->add(20/div, x+gsrand()%w, y+gsrand()%h, 10, 10,
                       -hitby_sx/4, hitby_sy/4, 50,
                       (int)PIX_DEBRIS3);
}

