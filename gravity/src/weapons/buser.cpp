/*
 *   GRAVITY STRIKE -- weapons/bfake.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "buser.h"
#include "../gsbase.h"
#include "../gsobject.h"
#include "../gsmap.h"
#include "../gsclear.h"
#include "../gsplayer.h"
#include "../gshelper.h"
#include "../gseffect.h"
#include "../gssound.h"
#include "../gsenemy.h"
#include "../gserror.h"
#include "../gsscript.h"
#include "../gsglob.h"
#include <tolua.h>
#include <math.h>

extern float sinus[];
extern float cosinus[];

extern gsClear    *clearlist;
extern gsGlobals  *globals;
extern gsPlayer   *player[];
extern gsEffect   *effects;

// spawned with player structure
bulletUser::bulletUser(class gsPlayer *p, int pmode, int pupgrade)
{
  float xp = p->get_width()/2; float yp = p->get_height()/2;
  int a = (int)p->get_head();
  float xrot = sinus[ a ];
  float yrot = cosinus[ a ];

  float dx = xp + xrot*yp;  // middle shot
  float dy = yp - yrot*yp;

  // stat
  p->inc_bullets_shot(1);

  mode = 0;
  if (pmode == 1) { mode = 1; dx = xp - yrot*4; dy = yp - xrot*4; }        // left?
  else if (pmode == 2) { mode = 1; dx = xp + yrot*4; dy = yp + xrot*4; }   // right?

  x = p->get_x() + dx;
  y = p->get_y() + dy;

  xspd = xrot * globals->shoot_speed + p->get_xspd();
  yspd = yrot * globals->shoot_speed - p->get_yspd();

  min_x = (int)p->get_x() - globals->playscreen_width * 2;
  max_x = (int)p->get_x() + globals->playscreen_width * 2;
  min_y = (int)p->get_y() - globals->playscreen_height * 2;
  max_y = (int)p->get_y() + globals->playscreen_height * 2;

  teleport_time = 0;

  by_player = p->get_sig();
  my_by_player = get_player_nr_by_sig(by_player);

  orig_ttl = ttl = 60 * 4; // 4 seconds to live
  w = h = 1;

  if (mode) { type = W_DOUBLE; strength = 2; }
  else { type = W_SINGLE; strength = 1; }
}

// spawned with coordinates and speeds
bulletUser::bulletUser(long psig, int enr, int px, int py, float pxspd, float pyspd, int pstrength, int pshield, int phull, float puser, int pupgrade)
{
  x = px;
  y = py;

  xspd = pxspd;
  yspd = pyspd;

  min_x = (int)x - globals->playscreen_width * 2;
  max_x = (int)x + globals->playscreen_width * 2;
  min_y = (int)y - globals->playscreen_height * 2;
  max_y = (int)y + globals->playscreen_height * 2;

  teleport_time = 0;

  orig_ttl = ttl = 60 * 4; // 4 seconds to live

  strength = pstrength;

  by_player = psig;
  by_enemy = enr;
  w = h = 1;

  my_by_player = get_player_nr_by_sig(by_player);
  type = W_USER;
  
  
  // TEST!
  bLua = new Lua;
//  effects->bullets->bulletLua->getglobals(); // globals aus INI holen
  
//  errors->log(2, "stack pos type", effects->bullets->bulletLua->lua_type(1));
//  script->init_script_commands(bLua, 0);
//  int err = bLua->dofile("./userweap/testwaffe.sc");
//  if (err)
//    errors->log(0, "ERROR executing local bullet script", err);
//  errors->log(2, "testvariable", script->getglobal_int("testvariable", bLua));
}

bulletUser::~bulletUser()
{
  // TEST!
//  script->kill_script_commands(bLua);
  delete bLua;
}

void bulletUser::move()
{
  Bullet::move();

//  x += xspd;
//  y -= yspd;
/*
    char func[100]; int ret = 0;
    strcpy(func, "bullet_move");
    lua.getglobal(func);
    tolua_pushusertype(lua.get_state(),(void*)this,tolua_tag(lua.get_state(),"Bullet"));
    lua.call(1, 1);
    if (lua.isnumber(1)) ret = (int)lua.tonumber(1);
    lua.pop(1);
*/
  ttl--;
}

void bulletUser::move(int no_trail)
{
  x += xspd;
  y -= yspd;

  if (ttl) ttl--;
}

void bulletUser::draw(gsMap *m)
{
}

void bulletUser::draw(BITMAP *m)
{
}


int bulletUser::check_collisions()
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
         collision( x, y, 1, 1, // so... is there a collision?
                    player[n]->get_x(), player[n]->get_y(),
                    player[n]->get_width(), player[n]->get_height(),
                    xspd, -yspd, player[n]->get_xspd(), player[n]->get_yspd(),
                    &newx1, &newy1, &newx2, &newy2, 1 ) )
    {
      // adjust position
      x = newx1; y = newy1;
      
      player[n]->hit(by_player, by_enemy, hitp_hull, hitp_shield, type, xspd, yspd);
      return 1;
    } // collision?
  } // for players

  // collision with objects?
  Object *temp_o;
  temp_o = objects->get_first_object();
  while (temp_o != NULL)
  {
    if (!temp_o->get_dont_update() && 
        collision( x, y, 1, 1,
                   temp_o->get_x(), temp_o->get_y(),
                   temp_o->get_width(), temp_o->get_height() ) )
    {
      switch (temp_o->get_type())
      {
         case OBJ_DOORH : case OBJ_DOORV : case OBJ_DOORH2 : case OBJ_DOORV2 :
              if (temp_o->get_maxframe() != 0) return 1;
              break;

         default :
              if (temp_o->add_hitpoints(by_player, strength, type, x, y, xspd, yspd))
                return 1;

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
    if (temp_o->get_active() &&
        collision( x, y, 1, 1,
                   temp_o->get_x(), temp_o->get_y(),
                   temp_o->get_width(), temp_o->get_height(),
                   xspd, -yspd, temp_o->get_xspd(), temp_o->get_yspd(),
                   &newx1, &newy1, &newx2, &newy2, 1 ) )
    {
      // hit spobject
      if (temp_o->add_hitpoints(by_player, 0, type, x, y, xspd, yspd))
        return 1;

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
               if (collision( x, y,
                              1, 1,
                              temp_o->get_x()+1, temp_o->get_y()+1,
                              temp_o->get_width()-2, temp_o->get_height()-2 ) )
               {
                 Object *temp = objects->get_spobject(temp_o->get_user());
                 effects->explosions->add(-1, effect_beam2_01, 10, 1, 0, x, y, globals->gamedat);
                 set_x(temp->get_x()+temp->get_width()/2);
                 set_y(temp->get_y()+temp->get_height()/2);
                 reset_min_max();
                 teleport_time = 15;
                 effects->explosions->add(-1, effect_beam2_01, 10, 1, 0, x, y, globals->gamedat);
                 gsound->beam_bullet(x, y);
               }
             }
             return 0; // don't delete bullet
             break;

      } // type of special object?
    } // collision?

    temp_o = temp_o->get_next();
  }

  // collision with enemies?
  Enemy *temp_e;
  temp_e = enemies->get_first_enemy();
  while (temp_e != NULL)
  {
    if (collision( x, y, 1, 1,
                   temp_e->get_x(), temp_e->get_y(),
                   temp_e->get_width(), temp_e->get_height(),
                   xspd, -yspd, temp_e->get_xspd(), temp_e->get_yspd(),
                   &newx1, &newy1, &newx2, &newy2, 1 ) )
    {
      // adjust position
      x = newx1; y = newy1;
      
      if (temp_e->hit(by_player, by_enemy, strength, x, y, xspd, yspd, W_SINGLE))
        return 1;
    }

    temp_e = temp_e->get_next();
  }

  return 0;
}

int bulletUser::check_map_collisions(gsMap *m)
{
  // collision with landscape?
  if (m->is_on_map(x, y, 1, 1))
  {
    if  (m->is_pixel(x, y))
    {
      // add pixels when bullet hits the wall
      float tx, ty;

      if ( yspd < 0 ) ty = -0.6;
      else ty = 0.4;

      tx = ( (gsrand() % 4) - 2 ) / 5;

      float pspd = abs(int(sqrt(yspd*yspd + xspd*xspd)));

      int *coltbl = globals->tbl_green;

      if (!strcmp(m->get_style(), "gfw1.fmp"))
        coltbl = globals->tbl_green;
      else
        coltbl = globals->tbl_white;

      effects->pixels->add( (gsrand() % ((int)(pspd*2)+1)) + 1,
                             x - xspd/4, y + ty*4, 1, 1,
                             tx, ty, 400,
                             coltbl, 10, 60*10, 50,
                             600, 600);

      return 1;
    } // if getpixel
  }

  return 0;
}

