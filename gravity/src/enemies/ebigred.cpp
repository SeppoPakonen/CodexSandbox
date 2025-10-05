/*
 *   GRAVITY STRIKE -- enemies/ebigred.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "ebigred.h"
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

enemyShipBigRed::enemyShipBigRed(int px, int py, float pfreq, float pradius, float pspeed, float pstrength, float targplayer, float user6)
{
  type          = E_SHIP_BLUE1;

  f_add         = 1;
  xradius = yradius = (int)pradius;
  bullet_speed  = pspeed;
  bullet_strength = pstrength;
  bullet_hitp_hull = bullet_hitp_shield = (int)bullet_strength;

  maxhit        = 100;

  x             = px;
  y             = py;

  bound_x       = 0;
  bound_y       = 0;
  bound_w       = 0;
  bound_h       = 0;

  image = enemy_s2_01;

  maxframet = 20;
  maxframe = 2;

  physical_frames = 3;

  w = ow = ((RLE_SPRITE*)globals->gamedat[image].dat)->w;
  h = oh = ((RLE_SPRITE*)globals->gamedat[image].dat)->h;

  frames->add_frames_from_datafile(globals->gamedat, DATA_RLE, image, physical_frames, 0, 0, maxframet);

  curframe = startframe;
}

enemyShipBigRed::~enemyShipBigRed()
{
}

void enemyShipBigRed::move()
{
  Enemy::move();
  if (freezed)
  {
    x -= xspd;
    y -= yspd;
  }
}

void enemyShipBigRed::update()
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

//    if (active && curframet == 0 && curframe == maxframe) shoot();
    dispframe = curframe;

    if (onehit) onehit = FALSE;
  }

  move();
}

void enemyShipBigRed::shoot()
{
/*  float pxspd = 0, pyspd = 0;
  int pxpos = (int)x+w/2, pypos = (int)y+h/2;

  pxspd = bullet_speed/2 - gsrand() % (int)bullet_speed;
  pyspd = bullet_speed/2 - gsrand() % (int)bullet_speed;

  gsound->shoot_single(SOUND_SHOOT_SINGLE, -1);
  effects->bullets->add(W_SINGLE, psig, pxpos, pypos, pxspd, pyspd, bullet_strength, 0, 0, nr);
*/
}

int enemyShipBigRed::hit(long fromplayer, int fromenemy, float n, int fx, int fy, float fxspd, float fyspd, int weapon)
{
  if (fromenemy == nr) return 0;
  if (fromplayer == psig && psig != -1) return 1;
  if (fromplayer == 0 && fromenemy != 0) return 1;

  if (fromplayer > 0 && n > 0) 
  {
    gsPlayer *tmppl = get_player_by_sig(fromplayer);
    if (tmppl) tmppl->inc_bullets_hit();
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

  gsound->play_sound(SOUND_SHIP_HIT, fx, fy);

  effects->explosions->add(-1, expl_sm2_01, 3, 2, 1, fx, fy, globals->gamedat);
  effects->pixels->add(10, fx, fy, 0, 0, -fxspd/4, fyspd/4, 50, globals->tbl_red, 10, 60*3, 30);

  return 1;
}

void enemyShipBigRed::dead()
{
  if (globals->do_all_hooks || do_hooks)
    script->do_hook_enemy_dead(glob_lua, this);
  if (get_dont_continue()) return;

  if (hitby > 0) get_player_by_sig(hitby)->inc_enemy_bonus(150);

  int div = 1;
  if (shrinked) div = 3;

  for (int m=0; m < 3; m++)
    gsound->play_sound(SOUND_EXPL_MEDIUM1+gsrand()%3, x, y);

  for (int n=0; n < 20/div; n++)
  {
    effects->explosions->add(EXP_MED_1, 4, gsrand()%30, x+gsrand()%w, y+gsrand()%h);
    effects->pixels->add(100/div, x+gsrand()%w, y+gsrand()%h, 10, 10, 2-gsrand()%4, 2-gsrand()%4, 50, globals->tbl_red, 10, 60*5, 30);
    effects->pixels->add(20/div, x+gsrand()%w, y+gsrand()%h, 10, 10,
                         2-gsrand()%4, 2-gsrand()%4, 50,
                         (int)PIX_DEBRIS2);
  }
}

