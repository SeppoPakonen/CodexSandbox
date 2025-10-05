/*
 *   GRAVITY STRIKE -- enemies/etank2.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "etank2.h"
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

enemyTank2::enemyTank2(int px, int py, float penemy, float pfreq, float pradius, float pspeed, float pstrength, float weaptype)
{
  type          = E_TANK_FLYINGTOWER;

  xradius = yradius = (int)pradius;
  bullet_speed  = pspeed;
  bullet_strength = pstrength;
  bullet_hitp_hull = bullet_hitp_shield = (int)bullet_strength;
  bullet_freq = (int)pfreq;
  cur_bullet_freq = 0;

  maxhit        = 8;

  x             = px;
  y             = py;

  bound_x       = 0;
  bound_y       = 0;
  bound_w       = 0;
  bound_h       = 0;

  image = enemy_tank2_01;

  maxframet = 5;
  maxframe = 1;

  physical_frames = 2;

  w = ow = ((RLE_SPRITE*)globals->gamedat[image].dat)->w;
  h = oh = ((RLE_SPRITE*)globals->gamedat[image].dat)->h;

  frames->add_frames_from_datafile(globals->gamedat, DATA_RLE, image, physical_frames, 0, 0, maxframet);

  curframe = startframe;

  etype = (int)penemy;

  switch (etype)
  {
    case E_TOWER_FLAK    :  user[0] = 30;
                            user[1] = pfreq;
                            user[2] = pradius;
                            user[3] = pspeed;
                            user[4] = pstrength;
                            break;
    case E_TOWER_THREE   :
                            user[0] = pfreq;
                            user[1] = pradius;
                            user[2] = pspeed;
                            user[3] = pstrength;
                            user[4] = 0;
                            user[5] = weaptype;
                            break;
    case E_TOWER_PULSE   :
                            user[0] = pfreq;
                            user[1] = pradius;
                            user[2] = pspeed;
                            user[3] = pstrength;
                            user[4] = 0;
                            break;
    case E_TOWER_SINGLE  :
    default              :  user[0] = D_UP;
                            user[1] = pfreq;
                            user[2] = pradius;
                            user[3] = pspeed;
                            user[4] = pstrength;
                            penemy = E_TOWER_SINGLE;
                            break;
  }
}

void enemyTank2::init()
{
  Enemy *packed_enemy = enemies->add(3500+nr, (int)etype, x, y, psig, user[0], user[1], user[2], user[3], user[4], user[5]);
  if (packed_enemy)
  {
    packed_enemy->set_connected(this);
    packed_enemy->set_bullet_shield_hitp(bullet_hitp_shield);
    packed_enemy->set_bullet_hull_hitp(bullet_hitp_hull);
    set_connected(packed_enemy);
  }
}

enemyTank2::~enemyTank2()
{
}

void enemyTank2::move()
{
  Enemy::move();
  if (freezed)
  {
    x -= xspd;
    y -= yspd;
  }

  // move packed enemy
  if (get_connected())
  {
    get_connected()->set_x( x + w/2 - get_connected()->get_width()/2 );
    get_connected()->set_y( y + 1 - get_connected()->get_height() );
  }
}

void enemyTank2::update()
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
        curframe++;
        curframet = 0;
        if (curframe > maxframe) { curframe = 0; }
      }
    }

//    if (active && curframet == 0 && curframe == maxframe) shoot();
    dispframe = curframe;

    if (onehit) onehit = FALSE;
  }

  move();
}

void enemyTank2::shoot()
{
}

int enemyTank2::hit(long fromplayer, int fromenemy, float n, int fx, int fy, float fxspd, float fyspd, int weapon)
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
    if (get_connected()) get_connected()->set_freezed(60*10);
    return 1;
  }

  if (weapon == W_SHRINKER)
  {
    set_shrinked(60*5);
    if (get_connected()) get_connected()->set_shrinked(60*5);
    return 1;
  }

  hitpoints += n;
  
  gsound->play_sound(SOUND_SHIP_HIT, fx, fy);
  effects->explosions->add(-1, expl_sm2_01, 3, 2, 1, fx, fy, globals->gamedat);
  effects->pixels->add(10, fx, fy, 0, 0, -fxspd/4, fyspd/4, 50, globals->tbl_white, 10, 60*3, 30);

  return 1;
}


void enemyTank2::dead()
{
  if (globals->do_all_hooks || do_hooks)
    script->do_hook_enemy_dead(glob_lua, this);
  if (get_dont_continue()) return;

  if (hitby > 0) 
  {
    gsPlayer *tpl = get_player_by_sig(hitby);
    if (tpl) tpl->inc_enemy_bonus(5);
  }

  int div = 1;
  if (shrinked) div = 3;

  gsound->play_sound(SOUND_EXPL_SMALL1+gsrand()%2, x, y);

  effects->explosions->add(-1, expl_sm1_01, 10, 2, 1, x+w/2-3, y+h/2-3, globals->gamedat);
//  effects->craters->add(x+w/2, y+h-3, 10, 1);
  effects->pixels->add(80/div, x+w/2, y+h/2, 0, 0, 2-(gsrand() % 3), gsrand() % 2, 50, globals->tbl_white, 10, 60*3, 50);
  effects->pixels->add(10/div, x+gsrand()%w, y+gsrand()%h, 10, 10,
                       -hitby_sx/4, hitby_sy/4, 50,
                       (int)PIX_DEBRIS3);

  // kill packed enemy, too
  if (get_connected()) get_connected()->set_hit(get_connected()->get_maxhit()+1);
}

void enemyTank2::set_maxhit(int n)
{
  maxhit = n;
  if (get_connected()) get_connected()->set_maxhit(n);
}

