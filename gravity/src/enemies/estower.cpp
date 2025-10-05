/*
 *   GRAVITY STRIKE -- enemies/estower.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "estower.h"
#include "../gsmap.h"
#include "../gsclear.h"
#include "../gsplayer.h"
#include "../gsglob.h"
#include "../gshelper.h"
#include "../gseffect.h"
#include "../gssound.h"
#include "../gsbullet.h"
#include "../gsnetw.h"
#include "../gserror.h"

enemySingleTower::enemySingleTower(int px, int py, float pdir, float pfreq, float pradius, float pspeed, float pstrength, float weaptype)
{
  type          = E_TOWER_SINGLE;

  xradius = yradius = (int)pradius;
  bullet_speed  = pspeed;
  bullet_strength = pstrength;
  bullet_hitp_hull = bullet_hitp_shield = (int)bullet_strength;
  bullet_freq = (int)pfreq;
  bullet_type = (int)weaptype;
  if (!bullet_type) bullet_type = W_SINGLE;
  cur_bullet_freq = 0;

  maxhit        = 5;

  x             = px;
  y             = py;

  bound_x       = 0;
  bound_y       = 0;
  bound_w       = -1;
  bound_h       = -1;

  direction = (int)pdir;
  switch (direction)
  {
    case 0  :  image = shooter_left1; break;
    case 1  :  image = shooter_right1; break;
    case 2  :  image = shooter_up1; break;
    case 3  :  image = shooter_down1; break;
    default :  image = shooter_left1; break;
  }

  maxframet = 2;
  maxframe = (int)pfreq;

  physical_frames = 3;

  w = ow = ((RLE_SPRITE*)globals->gamedat[image].dat)->w;
  h = oh = ((RLE_SPRITE*)globals->gamedat[image].dat)->h;

  frames->add_frames_from_datafile(globals->gamedat, DATA_RLE, image, physical_frames, 0, 0, maxframet);

  curframe = startframe;
}

enemySingleTower::~enemySingleTower()
{
}

void enemySingleTower::move()
{
}

void enemySingleTower::update()
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
    if (curframe  > maxframe)  { curframe = startframe; }
    if (curframe > 2)
    {
      dispframe = 0;
      if (curframe == 3 && curframet == 0) shoot();
    }
    else dispframe = curframe;

    if (onehit) onehit = FALSE;
  }
}

void enemySingleTower::shoot()
{
  if (globals->do_all_hooks || do_hooks)
    script->do_hook_enemy_shoot(glob_lua, this);
  if (get_dont_continue()) return;

  float pxspd = 0, pyspd = 0;
  int pxpos = (int)x, pypos = (int)y;

  switch (direction)
  {
    case D_LEFT   :  pxspd = -bullet_speed; pyspd = 0;
                     pxpos += 2; pypos += 2;
                     break;
    case D_RIGHT  :  pxspd = bullet_speed; pyspd = 0;
                     pxpos += w; pypos += 2;
                     break;
    case D_UP     :  pyspd = bullet_speed; pxspd = 0;
                     pxpos += 2; pypos += 1;
                     break;
    case D_DOWN   :  pyspd = -bullet_speed; pxspd = 0;
                     pxpos += 2; pypos += h+1;
                     break;
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

  effects->bullets->add(bullet_type, psig, nr, pxpos, pypos, pxspd, pyspd, bullet_strength,  bullet_hitp_shield, bullet_hitp_hull, 0, 0);

}

int enemySingleTower::hit(long fromplayer, int fromenemy, float n, int fx, int fy, float fxspd, float fyspd, int weapon)
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
    set_freezed(60*10);
    return 1;
  }

  if (weapon == W_SHRINKER)
  {
    set_shrinked(60*10);
    return 1;
  }

  hitpoints += n;
  gsound->play_sound(SOUND_SHIP_HIT, fx, fy);

  effects->explosions->add(-1, expl_sm2_01, 3, 2, 1, fx, fy, globals->gamedat);
  effects->pixels->add(10, fx, fy, 0, 0, -fxspd/4, fyspd/4, 50, globals->tbl_white, 10, 60*3, 30);

  return 1;
}


void enemySingleTower::dead()
{
  if (globals->do_all_hooks || do_hooks)
    script->do_hook_enemy_dead(glob_lua, this);
  if (get_dont_continue()) return;

  if (hitby > 0) 
  {
    gsPlayer *tpl = get_player_by_sig(hitby);
    if (tpl) tpl->inc_enemy_bonus(20);
  }

  int div = 1;
  if (shrinked) div = 3;

  gsound->play_sound(SOUND_EXPL_SMALL1+gsrand()%2, x, y);

  effects->explosions->add(-1, expl_sm1_01, 10, 2, 1, x+w/2-3, y+h/2-3, globals->gamedat);
  if (!globals->network_game || net->is_server()) 
    effects->craters->add(x+w/2, y+h-3, 10, 1);
  effects->pixels->add(80/div, x+w/2, y+h/2, 0, 0, 2-(gsrand() % 3), gsrand() % 2, 50, globals->tbl_white, 10, 60*3, 50);
  effects->pixels->add(10/div, x+gsrand()%w, y+gsrand()%h, 10, 10,
                       -hitby_sx/4, hitby_sy/4, 50,
                       (int)PIX_DEBRIS3);
}

