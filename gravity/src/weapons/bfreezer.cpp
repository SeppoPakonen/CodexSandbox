/*
 *   GRAVITY STRIKE -- weapons/bfreezer.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "bfreezer.h"
#include "../gsmap.h"
#include "../gsclear.h"
#include "../gsplayer.h"
#include "../gshelper.h"
#include "../gseffect.h"
#include "../gsbase.h"
#include "../gsobject.h"
#include "../gssound.h"
#include "../gsenemy.h"
#include "../gsglob.h"
#include <math.h>

extern float sinus[];
extern float cosinus[];

// spawned with player structure
bulletFreezer::bulletFreezer(gsPlayer *p,  int puser, int pupgrade)
{
  // bullet init
  type = W_FREEZER;
  by_player = p->get_sig();
  user      = puser;
  upgrade   = pupgrade;

  // stats
  p->inc_bullets_shot(1);

  // image
  w = h = 13;

  // position & speed
  float xp = p->get_width()/2; float yp = p->get_height()/2;
  float xrot = sinus[ (int)p->get_head() ];
  float yrot = cosinus[ (int)p->get_head() ];

  float dx = xp + xrot*yp;
  float dy = yp - yrot*yp;

  x = p->get_x() + dx-1;
  y = p->get_y() + dy;

  xspd = xrot * globals->shoot_speed_bomb*2;// + p->get_xspd();
  yspd = yrot * globals->shoot_speed_bomb*2;// - p->get_yspd();

  // max range
  min_x = (int)p->get_x() - globals->playscreen_width * 2;
  max_x = (int)p->get_x() + globals->playscreen_width * 2;
  min_y = (int)p->get_y() - globals->playscreen_height * 2;
  max_y = (int)p->get_y() + globals->playscreen_height * 2;

  // time to live
  orig_ttl = ttl = 60 * 5;

  // strength
  strength    = 60 * 6;
  hitp_hull   = 0;
  hitp_shield = 0;

  // temp vars
  teleport_time = 0;
  turn_speed = orig_turn_speed = 0.044;
  
  if (upgrade == 1) 
  {
    orig_ttl = ttl = 60 * 10;
    target = search_target(x, y, by_player);
  }
}

bulletFreezer::bulletFreezer(long psig, int enr, int px, int py, float pxspd, float pyspd, int pstrength, int pshield, int phull, float puser, int pupgrade)
{
  // bullet init
  type = W_FREEZER;
  by_player = psig;
  by_enemy  = enr;
  upgrade   = pupgrade;
  user      = puser;

  // image
  w = h = 13;
  
  // position & speed
  x = px;
  y = py;

  xspd = pxspd;
  yspd = pyspd;

  // max range
  min_x = (int)x - globals->playscreen_width * 2;
  max_x = (int)x + globals->playscreen_width * 2;
  min_y = (int)y - globals->playscreen_height * 2;
  max_y = (int)y + globals->playscreen_height * 2;

  // time to live
  orig_ttl = ttl = 60 * 5;

  // strength
  strength    = pstrength;
  if (strength == 0) strength = 60 * 6;  
  hitp_hull   = phull;
  hitp_shield = pshield;

  // temp vars
  teleport_time = 0;
  turn_speed = orig_turn_speed = 0.044;

  if (upgrade == 1) 
  {
    orig_ttl = ttl = 60 * 10;
    target = search_target(x, y, by_player);
  }

}


bulletFreezer::~bulletFreezer()
{
}

void bulletFreezer::move()
{
  Bullet::move();

  // upgrade 1: target locking
  if (upgrade == 1)
  {
    gsPlayer *tpl_pl = NULL;
    
    if (target > 0)
    {
      tpl_pl = get_player_by_sig(target);
      if (!tpl_pl)
      {
        target = search_target(x, y, by_player);
        tpl_pl = get_player_by_sig(target);
      }
    }
  
    int okay = 1;
    if (tpl_pl && tpl_pl->get_dead()) okay = 0;
  
    if ( tpl_pl && okay)
    {
       int enx=0, eny=0;
       if (tpl_pl)
       {
         enx = int(tpl_pl->get_x() + tpl_pl->get_width()/2);
         eny = int(tpl_pl->get_y() + tpl_pl->get_height()/2);
       }
  
       float rot = V_GetAngle(enx,eny,(int)x,(int)y);
       float xrot = sin(rot);
       float yrot = cos(rot);
  
       float scale_x = 0;
  
       // target cloaked? if so, decrease turn speed
       if (tpl_pl)
       {
         if ((tpl_pl->get_extra_action() && tpl_pl->get_extra() == EXTRA_CLOAK))
           turn_speed = orig_turn_speed/2;
         else
           turn_speed = orig_turn_speed;
       }
  
       if (x >= enx)
         scale_x = -xrot;
       else
         scale_x = xrot;
  
       float scale_y = yrot;
  
       dest_xspd = scale_x * 4;
       dest_yspd = scale_y * 4;
       
    } // target locked? move to target
    else if (target == 0)
    {
      target = search_target(x, y, by_player);
    }
    else
    {
      dest_xspd = xspd;
      dest_yspd = yspd;
    }
  
    if (xspd > dest_xspd) xspd -= turn_speed;
    else if (xspd < dest_xspd) xspd += turn_speed;
    if (yspd > dest_yspd) yspd -= turn_speed;
    else if (yspd < dest_yspd) yspd += turn_speed;
  } // if upgrade == 1


  x += xspd;
  y -= yspd;

  if (ttl) ttl--;

}

void bulletFreezer::draw(gsMap *m)
{
  if (!invisible && m->is_on_screen(x-4, y-4, 8, 8))
    if (getpixel(m->get_vscreen(), (int)x-m->get_map_x(), (int)y-m->get_map_y()) == 0)
    {
//      putpixel(m->get_vscreen(), x, y, globals->col_yellow);
//      clearlist->add(m,x,y,1,1);
      effects->pixels->add(4, x+1, y, 12, 12,
                           0, -globals->particle_gravity, 0,
                           globals->tbl_white, 9, 60*5, 15, 100, 100);
    }
}

int bulletFreezer::search_target(int px, int py, long psig)
{
  int bx, by, ret=0;
  float a=50000, b=0;

  for (int n=0; n < globals->players; n++)
  {
    if (player[n]->get_sig() != psig)
    {
      bx = int(player[n]->get_x() + player[n]->get_width()/2);
      by = int(player[n]->get_y() + player[n]->get_height()/2);
      b = distance( px, py, bx, by );
      if (b <= a) { a = b; ret = player[n]->get_sig(); }
    }
  }

  return ret;
}


int bulletFreezer::check_collisions()
{
  float newx1, newy1, newx2, newy2;
  if (just_coll > 0) just_coll--;

  // min_x/y or max_x/y reached or ttl expired?
  if ( ttl == 0 ||
       x < min_x || x > max_x ||
       y < min_y || y > max_y )
  {
    return 1;
  }

  // collision with players?
  for (int n=0; n < globals->players; n++)
  {
    if ( !player[n]->get_dead() &&        // is player alive?
         by_player != player[n]->get_sig() && // is the bullet shot by another player?
         collision( x-7, y-7, 13, 13,
                    player[n]->get_x()+3, player[n]->get_y()+3,
                    player[n]->get_width()-6, player[n]->get_height()-6,
                    xspd, -yspd, player[n]->get_xspd(), player[n]->get_yspd(),
                    &newx1, &newy1, &newx2, &newy2, 1 ) )
    {
      // adjust position
      x = newx1; y = newy1;
      
      if (player[n]->get_time_alive() > globals->player_indestructible && !player[n]->get_indestructibility())
      {
        player[n]->set_freezed(strength);
        gsound->hit_freezer(player[n]->get_x(), player[n]->get_y());
        gsPlayer *p = get_player_by_sig(by_player); 
        if (p) p->inc_bullets_hit(1);
      }
      return 1;
    } // collision?
  } // for players

  // collision with objects?
  Object *temp_o;
  temp_o = objects->get_first_object();
  while (temp_o != NULL)
  {
    if (!temp_o->get_dont_update() && 
        collision( x-5, y-5, 10, 10,
                   temp_o->get_x(), temp_o->get_y(),
                   temp_o->get_width(), temp_o->get_height() ) )
    {
      // hit object
      if (temp_o->add_hitpoints(by_player, 0, type, x, y, xspd, yspd))
         return 1;

      switch (temp_o->get_type())
      {
         case OBJ_DOORH : case OBJ_DOORV : case OBJ_DOORH2 : case OBJ_DOORV2 :
              if (temp_o->get_maxframe() != 0) return 1;
              break;

         default :
              return 1;
              break;
      }
    }

    temp_o = temp_o->get_next();
  }

  // collision with special objects?
  temp_o = objects->get_first_spobject();
  int bumped = 0;
  if (!just_coll)  
  while (temp_o != NULL)
  {
    // hit spobject
    if (temp_o->add_hitpoints(by_player, 0, type, x, y, xspd, yspd))
      return 1;

    if (temp_o->get_active() &&
        collision( x, y, 10, 10,
                   temp_o->get_x(), temp_o->get_y(),
                   temp_o->get_width(), temp_o->get_height(),
                   xspd, -yspd, temp_o->get_xspd(), temp_o->get_yspd(),
                   &newx1, &newy1, &newx2, &newy2, 1 ) )
    {
      switch (temp_o->get_type())
      {
        case SPOBJ_BUMPU : case SPOBJ_BUMPD :
             // adjust position
             x = newx1; y = newy1;
             if (!bumped)
             {
               temp_o->set_curframe(1);
               yspd = -yspd;
               bumped = 1;
               just_coll = 2; // double-bump-prevention-hack (consecutive frames)
             }
             return 0; // don't delete bullet
             break;
        case SPOBJ_BUMPL : case SPOBJ_BUMPR :
             // adjust position
             x = newx1; y = newy1;
             if (!bumped)
             {
               temp_o->set_curframe(1);
               xspd = -xspd;
               bumped = 1;
               just_coll = 2; // double-bump-prevention-hack (consecutive frames)
             }
             return 0; // don't delete bullet
             break;
        case SPOBJ_TELEPORT1 : case SPOBJ_TELEPORT2 :
             if (teleport_time == 0)
             {
/*               if (collision( x, y,
                              5, 5,
                              temp_o->get_x()+1, temp_o->get_y()+1,
                              temp_o->get_width()-2, temp_o->get_height()-2)
                              xspd, -yspd, temp_o->get_xspd(), temp_o->get_yspd() )
*/               {
                 Object *temp = objects->get_spobject(temp_o->get_user());
                 effects->explosions->add(-1, effect_beam2_01, 10, 1, 0, x, y, globals->gamedat);
                 set_x(temp->get_x()+temp->get_width()/2);
                 set_y(temp->get_y()+temp->get_height()/2);
                 reset_min_max();
                 teleport_time = 30;
                 effects->explosions->add(-1, effect_beam2_01, 10, 1, 0, x, y, globals->gamedat);
                 gsound->beam_bullet(x, y);
               }
             }
             return 0; // don't delete bullet
             break;
      }
    }

    temp_o = temp_o->get_next();
  }

  // collision with enemies?
  Enemy *temp_e;
  temp_e = enemies->get_first_enemy();
  while (temp_e != NULL)
  {
    if (collision( x-7, y-7, 13, 13,
                   temp_e->get_x(), temp_e->get_y(),
                   temp_e->get_width(), temp_e->get_height(),
                   xspd, -yspd, temp_e->get_xspd(), temp_e->get_yspd(),
                   &newx1, &newy1, &newx2, &newy2, 1 ) )
    {
      // adjust position
      x = newx1; y = newy1;
      
      if (temp_e->hit(by_player, by_enemy, 0, x, y, xspd, yspd, W_FREEZER))
      {
        gsPlayer *p = get_player_by_sig(by_player); 
        if (p) p->inc_bullets_hit(1);
        return 1;
      }
    }

    temp_e = temp_e->get_next();
  }

  return 0;
}

int bulletFreezer::check_map_collisions(gsMap *m)
{
  // collision with landscape?
  if (m->is_on_map(x, y, 1, 1))
  {
    if  (m->is_pixel(x, y))
    {
      return 1;
    } // if getpixel
  }

  return 0;
}

