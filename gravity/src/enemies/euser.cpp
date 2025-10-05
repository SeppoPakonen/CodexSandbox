/*
 *   GRAVITY STRIKE -- enemies/euser.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "euser.h"
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

enemyUser::enemyUser(int px, int py, float uimage, float pfreq, float pspeedx, float pspeedy, float pstrength, float weaptype)
{
  type          = E_USER;

  kill_frames   = 0;
  f_add         = 1;

  xradius = yradius = (int)500;
  bullet_speed_x  = pspeedx;
  bullet_speed_y  = pspeedy;
  bullet_strength = pstrength;
  bullet_hitp_hull = bullet_hitp_shield = (int)bullet_strength;
  bullet_freq = (int)pfreq;
  cur_bullet_freq = 0;
  bullet_type = (int)weaptype;

  user_image    = (int)uimage;

  maxhit        = 47;

  x             = px;
  y             = py;

  bound_x       = 0;
  bound_y       = 0;
  bound_w       = 0;
  bound_h       = 0;

  maxframet = 10;

  if (frames) delete(frames);
  frames = images->get_image(user_image);

  maxframe = frames->get_framecount()-1;

  physical_frames = maxframe+1;

  w = ow = ((RLE_SPRITE*)frames->get_rle_frame(0))->w;
  h = oh = ((RLE_SPRITE*)frames->get_rle_frame(0))->h;

  curframe = startframe;
}

enemyUser::~enemyUser()
{
}

void enemyUser::move()
{
  Enemy::move();
  if (freezed)
  {
    x -= xspd;
    y -= yspd;
  }
}

void enemyUser::update()
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
//        if (curframe == maxframe || curframe == 0) { f_add = -f_add; }
        if (curframe > maxframe) curframe = 0;
      }
    }
    dispframe = curframe;
  }

  if (active && cur_bullet_freq > bullet_freq)
  {
    shoot();
    cur_bullet_freq = 0;
  }
  if (bullet_freq) cur_bullet_freq++;
  if (onehit) onehit = FALSE;

  move();
}

void enemyUser::shoot()
{
  if (globals->do_all_hooks || do_hooks)
    script->do_hook_enemy_shoot(glob_lua, this);
  if (get_dont_continue()) return;

  float pxspd = bullet_speed_x, pyspd = bullet_speed_y;
  int pxpos = (int)x+w/2, pypos = (int)y+h/2;

  if (bullet_speed_x == -1000)
    pxspd = 2 - (gsrand() % 4);

  if (bullet_speed_y == -1000)
    pyspd = 2 - (gsrand() % 4);

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

int enemyUser::hit(long fromplayer, int fromenemy, float n, int fx, int fy, float fxspd, float fyspd, int weapon)
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

  effects->explosions->add(-1, expl_sm2_01, 3, 2, 1, fx, fy, globals->gamedat);
  effects->pixels->add(10, fx, fy, 0, 0, -fxspd/4, fyspd/4, 50, globals->tbl_blue, 10, 60*3, 30);

  return 1;
}

void enemyUser::dead()
{
  if (globals->do_all_hooks || do_hooks)
    script->do_hook_enemy_dead(glob_lua, this);
  if (get_dont_continue()) return;

  if (hitby > 0) 
  {
    gsPlayer *tpl = get_player_by_sig(hitby);
    if (tpl) tpl->inc_enemy_bonus(100);
  }

  int div = 1;
  if (shrinked) div = 3;

  for (int n=0; n < 10/div; n++)
  {
    effects->explosions->add(EXP_MED_1, 4, gsrand()%30, x+gsrand()%w, y+gsrand()%h);
    effects->pixels->add(60/div, x+gsrand()%w, y+gsrand()%h, 10, 10, 2-gsrand()%4, 2-gsrand()%4, 50, globals->tbl_blue, 10, 60*5, 30);
    effects->pixels->add(10/div, x+gsrand()%w, y+gsrand()%h, 10, 10,
                         2-gsrand()%4, 2-gsrand()%4, 50,
                         (int)PIX_DEBRIS1);
  }
}

