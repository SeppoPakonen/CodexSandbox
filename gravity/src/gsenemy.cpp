/*
 *   GRAVITY STRIKE -- gsenemy.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "gsenemy.h"
#include "gshelper.h"
#include "gsplayer.h"
#include "gserror.h"
#include "gssound.h"
#include "gsmap.h"
#include "gsclear.h"
#include "enemies/etypes.h"
#include "gsglob.h"
#include "gsplayer.h"
#include "gseffect.h"
#include "gsnetw.h"

Enemy::Enemy()
{
  type          = E_NONE;

  connected_enemy = NULL;
  xradius       = 0;
  yradius       = 0;
  bullet_speed  = 0;
  bullet_speed_x = 0;
  bullet_speed_y = 0;

  maxhit        = 10;

  x             = 0;
  y             = 0;

  bound_x       = 0;
  bound_y       = 0;
  bound_w       = 0;
  bound_h       = 0;
  image         = 0;

  physical_frames = 0;
  w = ow        = 0;
  h = oh        = 0;

  user = 0;
  frames = new Image();
  wp = new gsWaypoint();

  curframe      = 0;
  curframet     = 0;
  maxframe      = 0;
  maxframet     = 0;
  dispframe     = 0;
  startframe    = 0;
  hitpoints     = 0;
  onehit        = FALSE;
  user          = 0;
  bullet_speed  = 0;
  bullet_strength = 0;
  bullet_hitp_hull = bullet_hitp_shield = bullet_strength;  
  bullet_freq   = 0;
  cur_bullet_freq = 0;
  active        = 1;
  hitby_sx      = 0;
  hitby_sy      = 0;
  teleport_time = 0;
  invisible     = 0;
  freezed       = 0;
  shrinked      = 0;
  maxshrinklevel = 0.4;
  shrinklevel   = 1;
  kill_frames   = 1;
  dont_continue = 0;
  do_hooks      = 0;
  kill          = 0;
  dead_by_network = 0;

  old_wp_x = old_wp_y = 0;

  xspd = yspd = dxspd = dyspd = 0;
  backbuf = NULL;

  for (int un=0; un < 20; un++)
  {
    user_int[un] = 0;
    user_float[un] = 0;
  }
}

Enemy::~Enemy()
{
  if (get_connected()) get_connected()->set_connected(NULL);
  if (kill_frames && frames) delete(frames);
  delete(wp);
}

int Enemy::hit(long frompl, int fromen, float n, int x, int y, float fxspd, float fyspd, int weapon)
{
  if (frompl > 0)
    get_player_by_sig(frompl)->inc_bullets_hit();

  return 0;
}

void Enemy::init()
{
}

void Enemy::update()
{
  float shrinkcount = ((1.0-maxshrinklevel) / 0.01);
  float wdiv = (ow - (ow * maxshrinklevel))/2;
  float hdiv = (oh - (oh * maxshrinklevel))/2;
  if (freezed > 0) freezed--;

  // shrink / unshrink ship
  if (shrinked)
  {
    shrinked--;
    if (shrinklevel > maxshrinklevel)
    {
      shrinklevel -= 0.01;
      w = int(ow * shrinklevel);
      h = int(oh * shrinklevel);
      x += wdiv/shrinkcount;
      y += hdiv/shrinkcount;
    }
  }
  else if (shrinklevel < 1)
  {
    shrinklevel += 0.01;
    w = int(ow * shrinklevel);
    h = int(oh * shrinklevel);
    x -= wdiv/shrinkcount;
    y -= hdiv/shrinkcount;
  }
}

void Enemy::do_damage_effect()
{
  if (!get_shrinked())
  {
    int mymaxhit = get_maxhit();
    if (mymaxhit <= 0) mymaxhit = 1;
    float health = float( float( (get_maxhit()-get_hit()) / (float)mymaxhit ) * 100.0);
    int expltype = EXP_SMALL_2;
    int expllen  = 4;
    int anz = 7;
     
    if (get_width() < 10 || get_height() < 10)
    {
      expltype = EXP_SMALL_4;
      expllen = 2;
      anz = 5;
    }

    int divider = int(health/anz);
    if (divider == 0) divider = 1;

    if (health < 90 && (globals->game_time % divider == 0))
    {
      int plusx = get_width() / 5;
      int plusy = get_height() / 5;
      effects->explosions->add(expltype, expllen, 0,
                               get_x() + plusx + gsrand()%(get_width()-plusx*2),
                               get_y() + plusy + gsrand()%(get_height()-plusy*2) );
    }
  }
}

void Enemy::draw(gsMap *m)
{
  if (!invisible && m->is_on_screen(x, y, w, h) && image >= 0)
  {
    if (!globals->slow_computer) do_damage_effect();

    if (shrinklevel == 1)
    {
      draw_rle_sprite(m->get_vscreen(),
                      frames->get_rle_frame(dispframe),
                      (int)x - m->get_map_x(), (int)y - m->get_map_y());
      clearlist->add(m, x+bound_x, y+bound_y, w+bound_w, h+bound_h);
    }
    else
    {
      BITMAP *tempb = create_bitmap(ow,oh);
      clear_bitmap(tempb);
      draw_rle_sprite(tempb, frames->get_rle_frame(dispframe), 0, 0);
      stretch_sprite(m->get_vscreen(), tempb,
                     (int)x - m->get_map_x(), (int)y - m->get_map_y(),
                     w, h);
      clearlist->add(m, x, y, w, h);
      destroy_bitmap(tempb);
    }
  }
}

void Enemy::wp_calculate_new_speeds(Waypoint *twp)
{
/*  // check if we would hit the walls and adjust waypoint position if neccessary
  int tx = twp->get_x();
  int ty = twp->get_y();

  if (get_nr() == 3) playmap[0]->draw_circle(globals->col_yellow, twp->get_x(), twp->get_y(), 3);
  int xchange = 0, ychange = 0;

  //if (playmap[0]->is_on_map(tx, ty, w, h))
  {
    int n=0;

    if (playmap[0]->is_pixel(tx + w, ty + h/2))
      for (n = tx; n > tx-w/2; n -= 2)
        if (!playmap[0]->is_pixel(n + w, ty + h/2))
        {
          twp->set_x(n);
          xchange = 1;
          break;
        }

    if (playmap[0]->is_pixel(tx, ty + h/2))
      for (n = tx; n < tx+w/2; n += 2)
        if (!playmap[0]->is_pixel(n, ty))
        {
          twp->set_x(n);
          if (xchange) xchange = 2;
          break;
        }


    if (playmap[0]->is_pixel(tx + w/2, ty + h))
      for (n = ty; n > ty-h/2; n -= 2)
        if (!playmap[0]->is_pixel(tx + w/2, n + h))
        {
          twp->set_y(n);
          ychange = 1;
          break;
        }

    if (playmap[0]->is_pixel(tx + w/2, ty))
      for (n = ty; n < ty+h/2; n += 2)
        if (!playmap[0]->is_pixel(tx + w/2, n))
        {
          twp->set_y(n);
          if (ychange) ychange = 2;
          break;
        }
  }

  if (get_nr() == 3 && (xchange || ychange)) playmap[0]->draw_filled_circle(globals->col_yellow, twp->get_x(), twp->get_y(), 3);

  if (xchange == 2 || ychange == 2)
  {
    //wp->remove_all();
    xspd = 0;
    yspd = 0;
    old_wp_x = x;
    old_wp_y = y;
  }
  else
*/ 
  {
    float xdist = twp->get_x() - x;
    float ydist = twp->get_y() - y;
    float dist = distance(x, y, twp->get_x(), twp->get_y());
    float hops = dist / twp->get_speed();
    if (hops != 0)
    {
      xspd = xdist / hops;
      yspd = ydist / hops;
    }
    old_wp_x = twp->get_x();
    old_wp_y = twp->get_y();
  }
}

void Enemy::move()
{
  int nonox = 0, nonoy = 0;
  int nomove = 0;

  if (wp->get_count() > 0)
  {
    Waypoint *twp;
  
    twp = wp->get(wp->get_current());
    if (!twp)
    {
      xspd = yspd = 0;
      return;
    }

    if (shrinked && shrinklevel > maxshrinklevel)
    {
      wp_calculate_new_speeds(twp);
//      errors->log(2, "newspeed", shrinklevel);
    }

    if (twp->get_x() != old_wp_x || twp->get_y() != old_wp_y)
      wp_calculate_new_speeds(twp);

    // check if waypoint hit
    if (x == twp->get_x() && y == twp->get_y())
    {
      xspd = yspd = 0;

      if (twp->get_curpause() < twp->get_pause())
      {
        twp->set_curpause( twp->get_curpause() + 1 );
        nomove = 1;
      }
      else
      {
        twp->set_curpause(0);
        wp->set_next();
        twp = wp->get(wp->get_current());
        if (!twp)
        {
          xspd = yspd = 0;
          return;
        }
      }
    }

    // calculate new speeds on waypoint or on start
    if (!nomove && xspd == 0 && yspd == 0)
    {
      wp_calculate_new_speeds(twp);
    }

    if (xspd > 0)
      if (x+xspd > twp->get_x()) { x = twp->get_x(); nonox = 1; }
    if (xspd < 0)
      if (x+xspd < twp->get_x()) { x = twp->get_x(); nonox = 1; }
  
    if (yspd > 0)
      if (y+yspd > twp->get_y()) { y = twp->get_y(); nonoy = 1; }
    if (yspd < 0)
      if (y+yspd < twp->get_y()) { y = twp->get_y(); nonoy = 1; }

  } // count > 0?


  if (!nonox) x += xspd;
  if (!nonoy) y += yspd;
}

void Enemy::check_collisions()
{

  if (script->is_flag(script->flag_hook_enemy_with_enemy) &&
      (globals->do_all_hooks || do_hooks))
  {
    Enemy *temp = enemies->get_first_enemy(), *next_enemy;

    while (temp != NULL)
    {
      next_enemy = temp->get_next();
  
      if (temp != this)
      {
        int nw1 = fast_ftol(w-w*0.1);
        int nh1 = fast_ftol(h-h*0.1);
        int nw2 = fast_ftol(temp->get_width()-w*0.1);
        int nh2 = fast_ftol(temp->get_height()-h*0.1);

        if (collision(x+w*0.2, y+h*0.2, nw1, nh1,
                      temp->get_x()+w*0.2, temp->get_y()+h*0.2,
                      nw2, nh2))
        {
//          errors->log(2, "hook enemy with enemy", this->get_nr(), temp->get_nr());
          script->do_hook_enemy_with_enemy(glob_lua, this, temp);
          break;
        }
      }
  
      temp = next_enemy;
    } // while temp != NULL
  }
}

RLE_SPRITE* Enemy::get_current_sprite()
{
  return frames->get_rle_frame(dispframe);
}

void Enemy::find_path(int targetx, int targety)
{
  int len = playmap[0]->find_path(x, y, targetx, targety);
  
  if (len > 0)
  {
    PathNode *mynodes = new PathNode[len];
    playmap[0]->store_path(mynodes);
  
    // create waypoints
    wp->remove_all();
    for (int n=0; n < len; n++)
    {
      float myspd = (len-n)/3.0 + 0.2;
      //Waypoint *twp = wp->add(mynodes[n].x-w/2+(rand()%10-5), mynodes[n].y-h/2+(rand()%10-5), myspd);
      Waypoint *twp = wp->add(mynodes[n].x-w/2, mynodes[n].y-h/2, myspd);
      if (get_nr() == 3) playmap[0]->draw_filled_circle(globals->col_blue, twp->get_x(), twp->get_y(), 3);
    }

    wp->set_current(0);
    delete[] mynodes;
  }

  wp->set_current(0);
}

gsEnemy::gsEnemy()
{
  first_enemy=NULL;
  last_enemy=NULL;
  count = 0;
}

gsEnemy::~gsEnemy()
{
  Enemy *temp = first_enemy, *next_enemy;

  while (temp != NULL)
  {
    next_enemy = temp->get_next();
    delete temp;
    temp = next_enemy;
  } // while temp != NULL
}

Enemy *gsEnemy::add(int num, int t, int px, int py, long psig, float user1, float user2, float user3, float user4, float user5, float user6)
{
  Enemy *TempEnemy = NULL;

  switch(t)
  {
    case E_NONE :
                       // do nothing, just exit
                       break;
    case E_USER :
                       TempEnemy = new enemyUser(px, py, user1, user2, user3, user4, user5, user6);
                       break;
    case E_TOWER_SINGLE :
                       TempEnemy = new enemySingleTower(px, py, user1, user2, user3, user4, user5, user6);
                       break;
    case E_TOWER_THREE :
                       TempEnemy = new enemyThreeTower(px, py, user1, user2, user3, user4, user5, user6);
                       break;
    case E_TOWER_PULSE :
                       TempEnemy = new enemyPulseTower(px, py, user1, user2, user3, user4, user5, user6);
                       break;
    case E_TOWER_FLAK :
                       TempEnemy = new enemyFlak(px, py, user1, user2, user3, user4, user5, user6);
                       break;
    case E_TANK_MOBILETOWER :
                       TempEnemy = new enemyTank1(px, py, user1, user2, user3, user4, user5, user6);
                       break;
    case E_TANK_FLYINGTOWER :
                       TempEnemy = new enemyTank2(px, py, user1, user2, user3, user4, user5, user6);
                       break;
    case E_SHIP_BLUE1 :
                       TempEnemy = new enemyShipBlue1(px, py, user1, user2, user3, user4, user5, user6);
                       break;
    case E_SHIP_BLUE2 :
                       TempEnemy = new enemyShipBlue2(px, py, user1, user2, user3, user4, user5, user6);
                       break;
    case E_SHIP_BIGRED :
                       TempEnemy = new enemyShipBigRed(px, py, user1, user2, user3, user4, user5, user6);
                       break;
    case E_SHIP_RELASH :
                       TempEnemy = new enemyShipRelash(px, py, user1, user2, user3, user4, user5, user6);
                       break;
    case E_SHIP_BIGBOSS :
                       TempEnemy = new enemyShipBigBoss(px, py, user1, user2, user3, user4, user5, user6);
                       break;
    default : break;
  } // case enemy type

  if (TempEnemy)
  {
    TempEnemy->set_nr(num);
    TempEnemy->set_sig(psig);
    add_to_list(TempEnemy);

//  errors->log(2, "sig/num", psig, num);

    TempEnemy->init();
  }

  return TempEnemy;
}

void gsEnemy::add_to_list(Enemy *TempEnemy)
{
  if (TempEnemy != NULL)
  {
    if (first_enemy==NULL)
    {
      last_enemy = first_enemy = TempEnemy;
      first_enemy->set_next(NULL); first_enemy->set_prev(NULL);
      TempEnemy->set_next(NULL); TempEnemy->set_prev(NULL);
    }
    else
    {
      last_enemy->set_next(TempEnemy);
      TempEnemy->set_prev(last_enemy);
      last_enemy = TempEnemy;
      TempEnemy->set_next(NULL);
    }
  } // TempEnemy != NULL

  count++;
}

void gsEnemy::remove(Enemy *temp)
{
  Enemy *next_enemy, *prev_enemy;

  if (temp != NULL)
  {
    prev_enemy = temp->get_prev();
    next_enemy = temp->get_next();

    if (prev_enemy==NULL) first_enemy = next_enemy;
    else prev_enemy->set_next(next_enemy);

    if (next_enemy==NULL) last_enemy = prev_enemy;
    else next_enemy->set_prev(prev_enemy);

    delete temp;
    temp = NULL;

    count--;
  }
}

void gsEnemy::update()
{
  Enemy *temp = first_enemy, *next_enemy;

  while (temp != NULL)
  {
    next_enemy = temp->get_next();
    if (!temp->get_remove()) temp->check_collisions();
    temp->update();
    
    if (temp->get_remove()) 
    { 
      remove(temp); 
    }
    
    else if (temp->get_hit() >= temp->get_maxhit())
    {
      temp->dead();

      // if this enemy was killed by us (and not over network), send it to the others!
      if (globals->network_game && !temp->get_dead_by_network())
        net->game_send_enemy_dead(temp->get_nr());

      remove(temp);
    }
    temp = next_enemy;
  } // while temp != NULL
    
}

void gsEnemy::remove_for_script(Enemy *temp)
{
  temp->remove();
}

void gsEnemy::draw(gsMap *m)
{
  Enemy *temp = first_enemy, *next_enemy;

  while (temp != NULL)
  {
    next_enemy = temp->get_next();
    if (!temp->get_remove()) temp->draw(m);
    temp = next_enemy;
  } // while temp != NULL
}

Enemy *gsEnemy::get_enemy(int n)
{
  Enemy *temp;
  temp = first_enemy;

  while (temp != NULL)
  {
    if (temp->get_nr() == n && !temp->get_remove()) return temp;
    temp = temp->get_next();
  }

  return NULL;
}


