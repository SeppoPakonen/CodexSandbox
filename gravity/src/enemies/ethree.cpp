/*
 *   GRAVITY STRIKE -- enemies/ethree.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "ethree.h"
#include "../gsmap.h"
#include "../gsclear.h"
#include "../gsplayer.h"
#include "../gsglob.h"
#include "../gshelper.h"
#include "../gseffect.h"
#include "../gssound.h"
#include "../gsbullet.h"
#include "../gserror.h"
#include "../gsnetw.h"

enemyThreeTower::enemyThreeTower(int px, int py, float pfreq, float pradius, float pspeed, float pstrength, float user5, float weaptype)
{
  type          = E_TOWER_THREE;

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
  bound_w       = 0;
  bound_h       = -1;

  image = shooter1_1;
  maxframet = 0;
  maxframe = 0;

  physical_frames = 1;

  w = ow = ((RLE_SPRITE*)globals->gamedat[image].dat)->w;
  h = oh = ((RLE_SPRITE*)globals->gamedat[image].dat)->h;

  frames->add_frames_from_datafile(globals->gamedat, DATA_RLE, image, physical_frames, 0, 0, maxframet);

  curframe = startframe;
  dispframe = curframe;
}

enemyThreeTower::~enemyThreeTower()
{
}

void enemyThreeTower::move()
{
}

void enemyThreeTower::update()
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
      active = 1;
  }

  if (shrinked || freezed) active = 0;

  if (active && cur_bullet_freq > bullet_freq)
  {
    shoot();
    cur_bullet_freq = 0;
  }
  if (bullet_freq) cur_bullet_freq++;

  if (onehit) onehit = FALSE;
}

void enemyThreeTower::shoot()
{
  if (globals->do_all_hooks || do_hooks)
    script->do_hook_enemy_shoot(glob_lua, this);
  if (get_dont_continue()) return;

  int pxpos = (int)x, pypos = (int)y;
  int buhei = 1;

  if (bullet_type == W_DOUBLE) bullet_type = W_SINGLE;
  switch (bullet_type)
  {
    case W_SINGLE :
         gsound->play_sound(SOUND_SHOOT_SINGLE, pxpos, pypos); break;
    case W_BOMB:
         gsound->play_sound(SOUND_SHOOT_BOMB, pxpos, pypos); 
         buhei = 2;
         break;
    case W_BIGBOMB:
         gsound->play_sound(SOUND_SHOOT_BIGBOMB, pxpos, pypos); 
         buhei = 3;
         break;
    case W_ROCKET:
         gsound->play_sound(SOUND_SHOOT_ROCKET, pxpos, pypos); 
         buhei = 4;
         break;
    case W_MINE:
         gsound->play_sound(SOUND_SHOOT_MINE, pxpos, pypos); 
         buhei = 4;
         break;
    case W_FREEZER:
         gsound->play_sound(SOUND_SHOOT_FREEZER, pxpos, pypos); 
         buhei = 5;
         break;
    case W_SHRINKER:
         gsound->play_sound(SOUND_SHOOT_SHRINKER, pxpos, pypos); 
         buhei = 5;
         break;
    case W_PROBE:
         gsound->play_sound(SOUND_SHOOT_PROBE, pxpos, pypos); 
         buhei = 6;
         break;
    case W_GAS:
         gsound->play_sound(SOUND_SHOOT_GAS, pxpos, pypos); 
         buhei = 5;
         break;
    case W_LASER:
         gsound->play_sound(SOUND_SHOOT_LASER, pxpos, pypos); 
         buhei = 1;
         break;
    case W_BUMPER:
         gsound->play_sound(SOUND_SHOOT_BUMPER, pxpos, pypos); 
         buhei = 4;
         break;
    case W_SHOTGUN:
         gsound->play_sound(SOUND_SHOOT_SHOTGUN, pxpos, pypos); 
         buhei = 2;
         break;
    case W_ACID:
         gsound->play_sound(SOUND_SHOOT_ACID, pxpos, pypos); 
         buhei = 2;
         break;
  }

  Bullet *tb[3] = { NULL };
  tb[0] = effects->bullets->add(bullet_type, psig, nr, x+1, y-buhei-2,    0,  bullet_speed*2,  bullet_strength,  bullet_hitp_shield, bullet_hitp_hull, 0, 0);
  tb[1] = effects->bullets->add(bullet_type, psig, nr, x+1, y-buhei,  bullet_speed,  bullet_speed,  bullet_strength, bullet_hitp_shield, bullet_hitp_hull, 0, 0);
  tb[2] = effects->bullets->add(bullet_type, psig, nr, x+1, y-buhei, -bullet_speed,  bullet_speed,  bullet_strength, bullet_hitp_shield, bullet_hitp_hull, 0, 0);
  
  for (int n=0; n < 3; n++)
    tb[n]->set_ttl(80);
}

int enemyThreeTower::hit(long fromplayer, int fromenemy, float n, int fx, int fy, float fxspd, float fyspd, int weapon)
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


void enemyThreeTower::dead()
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

