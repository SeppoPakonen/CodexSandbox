/*
 *   GRAVITY STRIKE -- weapons/bprobe.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "bprobe.h"
#include "../gsmap.h"
#include "../gsclear.h"
#include "../gsplayer.h"
#include "../gshelper.h"
#include "../gseffect.h"
#include "../gsobject.h"
#include "../gsbase.h"
#include "../gssound.h"
#include "../gsenemy.h"
#include "../gsnetw.h"
#include "../gsglob.h"
#include <math.h>

extern float sinus[];
extern float cosinus[];

// spawned with player structure
bulletProbe::bulletProbe(gsPlayer *p,  int puser, int pupgrade, Image *spr)
{
  // bullet init
  type = W_PROBE;
  by_player = p->get_sig();
  user      = puser;
  upgrade   = pupgrade;
  maxhitp   = 2;

  // stat
  p->inc_bullets_shot(1);

  // image
  image = spr;
  w = image->get_width();
  h = image->get_height();
  maxframet = 20;

  // position & speed
  float xp = p->get_width()/2; float yp = p->get_height()/2;
  float xrot = sinus[ (int)p->get_head() ];
  float yrot = cosinus[ (int)p->get_head() ];

  float dx = xp + xrot*yp;
  float dy = yp - yrot*yp;

  x = p->get_x() + dx - 2;
  y = p->get_y() + dy;

  xspd = yspd = 0;
  dxspd = dyspd = 0.1;

  // max range
  min_x = (int)p->get_x() - globals->playscreen_width * 4;
  max_x = (int)p->get_x() + globals->playscreen_width * 4;
  min_y = (int)p->get_y() - globals->playscreen_height * 4;
  max_y = (int)p->get_y() + globals->playscreen_height * 4;

  // time to live
  orig_ttl = ttl = 60 * 10;

  // strength
  strength    = 10;
  hitp_hull   = 10;
  hitp_shield = 6;

  // temp vars
  teleport_time = 0;
  time_since_fire = 0;
  target = search_target(x, y, p->get_sig());
  angle = 0;
}

bulletProbe::bulletProbe(long psig, int enr, int px, int py, float pxspd, float pyspd, int pstrength, int pshield, int phull, float puser, int pupgrade, Image *spr)
{
  // bullet init
  type = W_PROBE;
  by_player = psig;
  by_enemy  = enr;
  upgrade   = pupgrade;
  user      = puser;
  maxhitp   = 2;

  // image
  image = spr;
  w = image->get_width();
  h = image->get_height();
  maxframet = 20;
  
  // position & speed
  x = px;
  y = py;

  xspd = yspd = 0;
  dxspd = dyspd = 0.1;

  // max range
  min_x = (int)x - globals->playscreen_width * 4;
  max_x = (int)x + globals->playscreen_width * 4;
  min_y = (int)y - globals->playscreen_height * 4;
  max_y = (int)y + globals->playscreen_height * 4;

  // time to live
  orig_ttl = ttl = 60 * 10;

  // strength
  strength    = pstrength;
  hitp_hull   = phull;
  hitp_shield = pshield;

  // temp vars
  teleport_time = 0;
  time_since_fire = 0;
  if (user) orig_ttl = ttl = (int)user*60; else orig_ttl = ttl = 60 * 10;
  target = search_target(x, y, by_player);
  angle = 0;
}


bulletProbe::~bulletProbe()
{
}

int bulletProbe::search_target(int px, int py, long psig)
{
  int bx, by, ret=-1;
  float a=50000, b=0;

  for (int n=0; n < globals->players; n++)
  {
    if (player[n]->get_sig() != psig)
    {
      bx = int(player[n]->get_x() + player[n]->get_width()/2);
      by = int(player[n]->get_y() + player[n]->get_height()/2);
      b = distance( px, py, bx, by );
      if (b <= a) { a = b; ret = n; }
    }
  }

  if (enemies->get_count() > 0)
  {
    Enemy *temp = enemies->get_first_enemy();

    while (temp != NULL)
    {
      if (temp->get_sig() != psig)
      {
        bx = int(temp->get_x() + temp->get_width()/2);
        by = int(temp->get_y() + temp->get_height()/2);
        b = distance( px, py, bx, by );
        if (b <= a) { a = b; ret = temp->get_nr()+1000; }
      }

      temp = temp->get_next();;
    } // while temp != NULL
  }

  return ret;
}

/*
void bulletProbe::move()
{
  time_since_fire++;
  int tpl = get_player_nr_by_sig(target);

  if ( target != 0  && tpl >= 0 &&
       time_since_fire > 50 && !player[tpl]->get_dead() )
  {
     int enx = int(player[tpl]->get_x() + player[tpl]->get_width()/2);
     int eny = int(player[tpl]->get_y() + player[tpl]->get_height()/2);

     float rot = V_GetAngle(enx,eny,(int)x,(int)y);
     float xrot = sin(rot);
     float yrot = cos(rot);

     float scale_x = 0;

     if (x >= enx)
       scale_x = -xrot;
     else
       scale_x = xrot;

     float scale_y = yrot;

     dest_xspd = scale_x * (time_since_fire / 10) / 3;
     dest_yspd = scale_y * (time_since_fire / 10) / 3;

  } // target locked? move to target
  else if (time_since_fire > 50 && target < 0)
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

  x += xspd;
  y -= yspd;

  // draw trail
  effects->pixels->add(1, x+gsrand()%4, y-1+gsrand()%4, 1, 1,
                       0, -globals->particle_gravity, 0,
                       globals->tbl_white, 9, 60*5, 20, 100, 100);

  ttl++;
}
*/

void bulletProbe::move()
{
  Bullet::move();

  Enemy *tpl_en = NULL;
  gsPlayer *tpl_pl = NULL;

  if (target >= 0 && target < 1000)
    tpl_pl = player[target];
  else if (target >= 1000)
    tpl_en = enemies->get_enemy(target-1000);

  int okay = 1;
  if (tpl_pl && tpl_pl->get_dead()) okay = 0;

  xspd += dxspd;
  yspd += dyspd;

  if (xspd >= 0.4 || xspd <= -0.3) dxspd = -dxspd;
  if (yspd >= 0.6 || yspd <= -0.5) dyspd = -dyspd;

  if ((tpl_pl && tpl_pl->get_dead()) || !tpl_en)
    target = search_target(x, y, by_player);

  if ((tpl_pl || tpl_en) && okay)
  {
    int enx=0, eny=0;
    if (tpl_en)
    {
      enx = int(tpl_en->get_x() + tpl_en->get_width()/2);
      eny = int(tpl_en->get_y() + tpl_en->get_height()/2);
    }
    else if (tpl_pl)
    {
      enx = int(tpl_pl->get_x() + tpl_pl->get_width()/2);
      eny = int(tpl_pl->get_y() + tpl_pl->get_height()/2);
    }
  
    float rot = V_GetAngle(enx,eny,(int)x,(int)y);
    float xrot = sin(rot);
    float yrot = cos(rot);
  
    float scale_x = 0;
  
    if (x >= enx)
      scale_x = -xrot;
    else
      scale_x = xrot;
  
    float scale_y = yrot;
  
    float dest_xspd = scale_x * 2.5;
    float dest_yspd = scale_y * 2.5;

    // move probe
    x += xspd;
    y -= yspd;

    // don't move when player cloaked
    if (tpl_pl && tpl_pl->get_extra_action() && tpl_pl->get_extra() == EXTRA_CLOAK)
    {
      dest_xspd = dest_yspd = 0;
    }

    if (dest_xspd || dest_yspd)
    {
      if (ttl) ttl--;

      // fire
      if (ttl%20 == 0)
      {
        float plus = (gsrand()%5+1)/2.0;
        if (upgrade == 1)
        {
          gsound->shoot_single(x, y, 30);
          effects->bullets->add(W_LASER, by_player, by_enemy, x, y+3, dest_xspd*3, dest_yspd*3, 2, 3, 0, 0, 0);
        }
        else
        {
          gsound->shoot_single(x, y, 30);
          effects->bullets->add(W_SINGLE, by_player, by_enemy, x, y+3, dest_xspd*plus, dest_yspd*plus, 1, 1, 1, 0, 0);
        }
        
        int pl = get_player_nr_by_sig(by_player);
        if (pl >= 0)
        {
          statistics *plstat = player[pl]->get_statistics();
          plstat->bullets_shot++;
          player[pl]->set_statistics(plstat);
        }
      }
    }

    if (!playmap[0]->is_tile(x + dest_xspd*2, y - dest_yspd*2))
    {
      x += dest_xspd/3.0;
      y -= dest_yspd/3.0;
    }
  }

}

void bulletProbe::draw(gsMap *m)
{
  if (!invisible && m->is_on_screen(x, y, w, h))
    if (getpixel(m->get_vscreen(), (int)x-m->get_map_x(), (int)y-m->get_map_y()) == 0)
    {
      draw_rle_sprite(m->get_vscreen(), image->get_rle_frame(curframe), (int)x-m->get_map_x(), (int)y-m->get_map_y());
      clearlist->add(m, x, y, w, h);
    }
}

void bulletProbe::hit(float n)
{
  Bullet::hit(n);
  if (kill) effects->explosions->add(-1, expl_sm1_01, 10, 1, 0, x-2, y-2, globals->gamedat);
}

int bulletProbe::check_collisions()
{
  float newx1, newy1, newx2, newy2;
  if (just_coll > 0) just_coll--;

  // min_x/y or max_x/y reached or ttl expired?
  if ( ttl == 0 ||
       x < min_x || x > max_x ||
       y < min_y || y > max_y )
  {
    effects->explosions->add(-1, expl_sm1_01, 10, 1, 0, x-2, y-2, globals->gamedat);
    return 1;
  }

  // collision with players?
  for (int n=0; n < globals->players; n++)
  {
    if ( !player[n]->get_dead() &&        // is player alive?
         by_player != player[n]->get_sig() &&  // is the bullet shot by another player?
         collision( x, y, w, h,
                    player[n]->get_x(), player[n]->get_y(),
                    player[n]->get_width(), player[n]->get_height(),
                    xspd, -yspd, player[n]->get_xspd(), player[n]->get_yspd(),
                    &newx1, &newy1, &newx2, &newy2, 1 ) )
    {
      // adjust position
      x = newx1; y = newy1;
      
      player[n]->hit(by_player, by_enemy, hitp_hull, hitp_shield, type, xspd, yspd);
//      gsound->play(by_player, snd_exp_sm_1, 1000, 0, x, y, 100);
      effects->explosions->add(-1, expl_sm1_01, 10, 1, 0, x-2, y-2, globals->gamedat);
      return 1;
    } // collision?
  } // for players

  // collision with objects?
  Object *temp_o;
  temp_o = objects->get_first_object();
  while (temp_o != NULL)
  {
    if (!temp_o->get_dont_update() && 
        temp_o->get_active() &&
        collision( x, y, w, h,
                   temp_o->get_x(), temp_o->get_y(),
                   temp_o->get_width(), temp_o->get_height() ) )
    {
      if (temp_o->add_hitpoints(by_player, strength, type, x, y, xspd, yspd))
        return 1;

      effects->explosions->add(-1, expl_sm1_01, 10, 1, 0, x-2, y-2, globals->gamedat);
      gsound->expl_small1(x, y);

      switch (temp_o->get_type())
      {
         case OBJ_DOORH : case OBJ_DOORV : case OBJ_DOORH2 : case OBJ_DOORV2 :
              if (temp_o->get_maxframe() != 0) return 1;
              break;

         default :
              float tx, ty;
              if ( yspd < 0 ) ty = -0.6;
              else ty = 0.4;        
              tx = ( (gsrand() % 4) - 2 ) / 5;        
              float pspd = abs(int(sqrt(yspd*yspd + xspd*xspd)));
              effects->pixels->add( (gsrand() % ((int)(pspd*8)+1)) + 1,
                                     x - xspd/4, y + ty*4, 1, 1,
                                     tx, ty, 400,
                                     globals->tbl_white, 10, 60*10, 50,
                                     600, 600);

              return 1;
              break;
      }
    }

    temp_o = temp_o->get_next();
  }

  // collision with special objects?
  temp_o = objects->get_first_spobject();
  int bumped = 0;
  while (temp_o != NULL)
  {
    if (collision( x, y, w, h,
                   temp_o->get_x(), temp_o->get_y(),
                   temp_o->get_width(), temp_o->get_height() ) )
    {
      // hit spobject
      if (temp_o->add_hitpoints(by_player, 0, type, x, y, xspd, yspd))
        return 1;

      switch (temp_o->get_type())
      {
        case SPOBJ_BUMPU : case SPOBJ_BUMPD :
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
               if (collision( x, y, 5, 5,
                              temp_o->get_x()+1, temp_o->get_y()+1,
                              temp_o->get_width()-2, temp_o->get_height()-2 ) )
               {
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
    if (collision( x, y, w, h,
                   temp_e->get_x(), temp_e->get_y(),
                   temp_e->get_width(), temp_e->get_height(),
                   xspd, -yspd, temp_e->get_xspd(), temp_e->get_yspd(),
                   &newx1, &newy1, &newx2, &newy2, 1 ) )
    {
      // adjust position
      x = newx1; y = newy1;
      
      if (temp_e->hit(by_player, by_enemy, strength, x, y, xspd, yspd, W_PROBE))
      {
        effects->explosions->add(-1, expl_sm1_01, 10, 1, 0, x-2, y-2, globals->gamedat);
        return 1;
      }
    }

    temp_e = temp_e->get_next();
  }

  return 0;
}

int bulletProbe::check_map_collisions(gsMap *m)
{
  // collision with landscape?
  if (m->is_on_map(x, y, 1, 1))
  {
    if  (m->is_pixel((int)x, (int)y) ||
         m->is_pixel((int)x-2, (int)y) ||
         m->is_pixel((int)x+6, (int)y) ||
         m->is_pixel((int)x, (int)y-5) ||
         m->is_pixel((int)x, (int)y+5) )
    {
      // add pixels when bullet hits the wall
      float tx, ty;

      if ( yspd < 0 ) ty = -0.6;
      else ty = 0.4;

      tx = ( (gsrand() % 4) - 2 ) / 5;

      float pspd = abs(int(sqrt(yspd*yspd + xspd*xspd)));

      int *coltbl = NULL;
      if (!strcmp(m->get_style(), "gfw1.fmp"))
        coltbl = globals->tbl_green;
      else
        coltbl = globals->tbl_white;

      effects->pixels->add( (gsrand() % ((int)(pspd*2)+1)) + 50,
                             x - xspd/4, y + ty*4, 1, 1,
                             tx, ty, 400,
                             coltbl, 10, 60*10, 50,
                             600, 600);

      // add a crater
      if (!globals->network_game || net->is_server()) 
        effects->craters->add(x+1, y+1, 6, 1);

      gsound->expl_small1(x, y);

      // do a little explosion
      effects->explosions->add(-1, expl_sm1_01, 10, 1, 0, x-2, y-2, globals->gamedat);

      return 1;
    } // if getpixel
  }

  return 0;
}



