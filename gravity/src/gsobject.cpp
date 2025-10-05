/*
 *   GRAVITY STRIKE -- gsobject.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

/* TODO (#1#): Funktionen zur Echtzeit-Objektbearbeitung */
/* DONE (#1#): Gravitationswände hinzufügen */
/* TODO (#3#): Objekte und Spezialobjekte "vereinen" */


#include "gsobject.h"
#include "gsmap.h"
#include "gseffect.h"
#include "gshelper.h"
#include "gserror.h"
#include "gsclear.h"
#include "gssound.h"
#include "gsplayer.h"
#include "gsbullet.h"
#include "gsscript.h"
#include "gsnetw.h"
#include "gsglob.h"

extern float sinus[];
extern float cosinus[];

Object::Object(int c, OBJECT_TYPE ptype, int px, int py, int puser, int pmaxframet)
{
  frames        = new Image();
  wp            = new gsWaypoint();
  kill_frames   = 1;
  restrict_to_map = NULL;
  special       = 0;

  type          = ptype;
  nr            = c;
  curframe      = 0;
  curframet     = 0;
  maxframe      = 0;
  maxframet     = pmaxframet;
  dispframe     = 0;
  startframe    = 0;
  image         = -1;
  hit           = 0;
  maxhit        = 1000;
  onehit        = FALSE;
  user          = puser;
  user2         = 0;
  active        = 1;
  hitby_sx      = 0;
  hitby_sy      = 0;
  teleport_time = 0;
  invisible     = 0;
  dont_continue = 0;
  do_hooks      = 0;
  kill          = 0;
  dont_update      = 0;
  dead_by_network = 0;

  x             = px;
  y             = py;

  xspd = yspd = dxspd = dyspd = 0;

  // "bound" (plus values to x/y coordinate)
  bound_x       = 1;
  bound_y       = 1;
  bound_w       = -2;
  bound_h       = -2;
  physical_frames = 1;

  backbuf = NULL;

  switch (type)
  {
    case OBJ_PILE :
                       image    = pile1;
                       bound_x  = 1;
                       bound_y  = 0;
                       bound_w  = -2;
                       bound_h  = -6;
                       maxframe = 10;
                       curframe = gsrand() % 10;
                       if (!maxframet) maxframet = 6;
                       maxhit = 10;
                       physical_frames = 2;
                       break;

    case OBJ_BUILDING1 :
                       image    = obj_building1_1;
                       break;

    case OBJ_BUILDING2 :
                       image    = obj_building2_1;
                       break;

    case OBJ_BUILDING3 :
                       image    = obj_building3_1;
                       break;

    case OBJ_BUILDING4 :
                       image    = obj_building4_1;
                       break;

    case OBJ_BUILDING4_2 :
                       image    = obj_building4_2;
                       break;

    case OBJ_BUILDING_GRAV :
                       image    = obj_b_grav_1;
//                       bound_x  = 8;
//                       bound_y  = 4;
//                       bound_w  = -16;
//                       bound_h  = -10;
                       maxframe = 2;
                       if (!maxframet) maxframet = 5;
                       physical_frames = 3;
                       break;

    case OBJ_TUNNEL1 :
                       image    = obj_tunnel1_1;
                       maxframe = 4;
                       if (!maxframet) maxframet = 12;
                       maxhit   = 200;
                       physical_frames = 5;
                       break;

    case OBJ_MAN1 :
                       image    = obj_man1_1;
                       bound_x  = 0;
                       bound_y  = 0;
                       bound_h  = -1;
                       bound_w  = -1;
                       maxframe = 3;
                       if (!maxframet) maxframet = 7;
                       maxhit   = 1;
                       physical_frames = 4;
                       break;

    case OBJ_DOORH :
                       image    = obj_doorh_1;
                       bound_x  = 3;
                       bound_w  = -6;
                       startframe = 1;
                       maxframe = 3;
                       if (!maxframet) maxframet = 8;
                       maxhit = 5000;
                       physical_frames = 4;
                       break;

    case OBJ_DOORH2 :
                       image    = obj_doorh2_1;
                       bound_x  = 3;
                       bound_w  = -6;
                       startframe = 1;
                       maxframe = 3;
                       if (!maxframet) maxframet = 8;
                       maxhit = 5000;
                       physical_frames = 4;
                       break;

    case OBJ_DOORV :
                       image    = obj_doorv_1;
                       bound_y  = 3;
                       bound_h  = -6;
                       startframe = 1;
                       maxframe = 3;
                       if (!maxframet) maxframet = 8;
                       maxhit = 5000;
                       physical_frames = 4;
                       break;

    case OBJ_DOORV2 :
                       image    = obj_doorv2_1;
                       bound_y  = 3;
                       bound_h  = -6;
                       startframe = 1;
                       maxframe = 3;
                       if (!maxframet) maxframet = 8;
                       maxhit = 5000;
                       physical_frames = 4;
                       break;

    case OBJ_SWITCHL :
                       image    = obj_switchl_1;
                       maxhit = 5000;
                       physical_frames = 2;
                       break;

    case OBJ_SWITCHR :
                       image    = obj_switchr_1;
                       maxhit = 5000;
                       physical_frames = 2;
                       break;

    case OBJ_SWITCHU :
                       image    = obj_switchu_1;
                       maxhit = 5000;
                       physical_frames = 2;
                       break;

    case OBJ_SWITCHD :
                       image    = obj_switchd_1;
                       maxhit = 5000;
                       physical_frames = 2;
                       break;

    case OBJ_POOL1 :
                       image    = obj_pool1_1;
                       maxframe = 3;
                       if (!maxframet) maxframet = 15;
                       maxhit = 3000;
                       physical_frames = 4;
                       break;

    case OBJ_POOL2 :
                       image    = obj_pool2_1;
                       maxframe = 3;
                       if (!maxframet) maxframet = 15;
                       maxhit = 3000;
                       physical_frames = 4;
                       break;

    case OBJ_PPILE1 :
                       image    = obj_ppile1;
                       maxhit = 5000;
                       break;

    case OBJ_PPILE2 :
                       image    = obj_ppile2;
                       maxhit = 5000;
                       break;

    case OBJ_RACEPOLEL :
                       image    = obj_rpolel_1;
                       maxframe = 10;
                       if (!maxframet) maxframet = 8;
                       physical_frames = 6;
                       break;

    case OBJ_RACEPOLER :
                       image    = obj_rpoler_1;
                       maxframe = 10;
                       if (!maxframet) maxframet = 8;
                       physical_frames = 6;
                       break;

    case OBJ_RACEPOLEU :
                       image    = obj_rpoleu_1;
                       maxframe = 10;
                       if (!maxframet) maxframet = 8;
                       physical_frames = 6;
                       break;

    case OBJ_RACEPOLED :
                       image    = obj_rpoled_1;
                       maxframe = 10;
                       if (!maxframet) maxframet = 8;
                       physical_frames = 6;
                       break;

    case OBJ_TREE :
                       image    = obj_tree_1;
                       break;

    case OBJ_TREE2 :
                       image    = obj_tree_2;
                       break;

    case OBJ_STONE_B1 :
                       image    = obj_gstone_b1;
                       bound_x = bound_y = bound_w = bound_h = 0;
                       break;

    case OBJ_STONE_S1 :
                       image    = obj_gstone_s1;
                       bound_x = bound_y = bound_w = bound_h = 0;
                       break;

    case OBJ_STONE_S2 :
                       image    = obj_gstone_s2;
                       bound_x = bound_y = bound_w = bound_h = 0;
                       break;

    case OBJ_BLOCK_1  :
                       image    = obj_block1;
                       bound_x = bound_y = bound_w = bound_h = 0;
                       maxhit = 10;
                       break;

    case OBJ_BLOCK_2  :
                       image    = obj_block2;
                       bound_x = bound_y = bound_w = bound_h = 0;
                       maxhit = 40;
                       break;
  } // switch object_type

  w = ((RLE_SPRITE*)globals->gamedat[image].dat)->w;
  h = ((RLE_SPRITE*)globals->gamedat[image].dat)->h;

  frames->add_frames_from_datafile(globals->gamedat, DATA_RLE, image, physical_frames, 0, 0, maxframet);

  curframe = startframe;

  for (int un=0; un < 20; un++)
  {
    user_int[un] = 0;
    user_float[un] = 0;
  }
}

Object::Object(int c, SPOBJECT_TYPE ptype, int px, int py, int puser, int pmaxframet)
{
  frames        = new Image();
  wp            = new gsWaypoint();
  kill_frames   = 1;
  restrict_to_map = NULL;
  special       = 1;

  type          = ptype;
  nr            = c;
  curframe      = 0;
  curframet     = 0;
  maxframe      = 0;
  maxframet     = pmaxframet;
  dispframe     = 0;
  startframe    = 0;
  image         = -1;
  hit           = 0;
  maxhit        = 1000;
  onehit        = FALSE;
  user          = puser;
  active        = 1;
  hitby_sx      = 0;
  hitby_sy      = 0;
  teleport_time = 0;
  invisible     = 0;
  do_hooks      = 0;
  kill          = 0;
  dont_update      = 0;
  dead_by_network = 0;

  x             = px;
  y             = py;

  xspd = yspd = dxspd = dyspd = 0;
  physical_frames = 1;

  // "bound" (plus values to x/y coordinate)
  bound_x       = 0;
  bound_y       = 0;
  bound_w       = 0;
  bound_h       = 0;

  backbuf = NULL;

  switch (ptype)
  {
    // special objects
    case SPOBJ_BUMPL :
                       image    = obj_bumpl_1;
                       maxframe = 2;
                       physical_frames = 3;
                       break;
    case SPOBJ_BUMPR :
                       image    = obj_bumpr_1;
                       maxframe = 2;
                       physical_frames = 3;
                       break;
    case SPOBJ_BUMPU :
                       image    = obj_bumpu_1;
                       maxframe = 2;
                       physical_frames = 3;
                       break;
    case SPOBJ_BUMPD :
                       image    = obj_bumpd_1;
                       maxframe = 2;
                       physical_frames = 3;
                       break;
    case SPOBJ_EXTRA :
                       image    = obj_extra1_1;
                       maxframe = 2;
                       dxspd = dyspd = 0.02;
                       physical_frames = 3;
                       break;
    case SPOBJ_TELEPORT1 :
                       image    = effect_tele_01;
                       maxframe = 8;
                       if (!maxframet) maxframet = 6;
                       physical_frames = 9;
                       break;
    case SPOBJ_TELEPORT2 :
                       image    = effect_tele2_01;
                       maxframe = 8;
                       if (!maxframet) maxframet = 6;
                       physical_frames = 9;
                       break;
    case SPOBJ_GRAVTRAP :
                       image    = -1;
                       if (!puser) user = 1;
                       break;
    case SPOBJ_FLAG :
                       switch (user)
                       {
                         case 0 :
                           image = obj_flag1_1; break;
                         case 1 :
                           image = obj_flag2_1; break;
                         case 2 :
                           image = obj_flag3_1; break;
                         case 3 :
                           image = obj_flag4_1; break;
                         case 4 :
                           image = obj_flag5_1; break;
                         case 5 :
                           image = obj_flag6_1; break;
                         default :
                           image = obj_flag1_1;
                       }
                       maxframe = 4;
                       if (!maxframet) maxframet = 6;
                       physical_frames = 5;
                       break;
    case SPOBJ_BACKTILE1 :
                       image    = obj_backtile2;
                       break;
    case SPOBJ_GWALL_L :
                       image    = obj_gwalll_1;
                       maxframe = 3;
                       physical_frames = 4;
                       if (!puser) user = 1;
                       break;
    case SPOBJ_GWALL_R :
                       image    = obj_gwallr_1;
                       maxframe = 3;
                       physical_frames = 4;
                       if (!puser) user = 1;
                       break;
    case SPOBJ_GWALL_U :
                       image    = obj_gwallu_1;
                       maxframe = 3;
                       physical_frames = 4;
                       if (!puser) user = 1;
                       break;
    case SPOBJ_GWALL_D :
                       image    = obj_gwalld_1;
                       maxframe = 3;
                       physical_frames = 4;
                       if (!puser) user = 1;
                       break;
    case SPOBJ_USER : break;
  } // switch object_type

  if (image >= 0)
  {
    w = ((RLE_SPRITE*)globals->gamedat[image].dat)->w;
    h = ((RLE_SPRITE*)globals->gamedat[image].dat)->h;
    frames->add_frames_from_datafile(globals->gamedat, DATA_RLE, image, physical_frames, 0, 0, maxframet);
    backbuf = create_bitmap(w, h);
    clear(backbuf);
  }

  for (int un=0; un < 20; un++)
  {
    user_int[un] = 0;
    user_float[un] = 0;
  }
}

Object::Object(int c, OBJECT_TYPE ptype, Image *fr, int px, int py, int puser, int pmaxframet)
{
  frames        = fr;
  wp            = new gsWaypoint();
  kill_frames   = 0;
  restrict_to_map = NULL;
  special       = 0;

  type          = ptype;
  nr            = c;
  curframe      = 0;
  curframet     = 0;
  maxframe      = 0;
  maxframet     = pmaxframet;
  if (maxframet == 0) maxframet = 5;
  dispframe     = 0;
  startframe    = 0;
  image         = 0;
  hit           = 0;
  maxhit        = 1000;
  onehit        = FALSE;
  user          = puser;
  active        = 1;
  hitby_sx      = 0;
  hitby_sy      = 0;
  teleport_time = 0;
  invisible     = 0;
  do_hooks      = 0;
  kill          = 0;
  dont_update      = 0;
  dead_by_network = 0;

  x             = px;
  y             = py;

  xspd = yspd = dxspd = dyspd = 0;

  // "bound" (plus values to x/y coordinate)
  bound_x       = 0;
  bound_y       = 0;
  bound_w       = -1;
  bound_h       = -1;
  physical_frames = 1;

  backbuf = NULL;

//  frames.add_frames_from_imagefile(filename, DATA_RLE, maxframet);
  physical_frames = frames->get_framecount();
  maxframe = physical_frames-1;

  w = ((RLE_SPRITE*)frames->get_rle_frame(0))->w;
  h = ((RLE_SPRITE*)frames->get_rle_frame(0))->h;

  curframe = startframe;

  for (int un=0; un < 20; un++)
  {
    user_int[un] = 0;
    user_float[un] = 0;
  }
}

Object::Object(int c, SPOBJECT_TYPE ptype, Image *fr, int px, int py, int puser, int pmaxframet)
{
  frames        = fr;
  wp            = new gsWaypoint();
  kill_frames   = 0;
  restrict_to_map = NULL;
  special       = 1;

  type          = ptype;
  nr            = c;
  curframe      = 0;
  curframet     = 0;
  maxframe      = 0;
  maxframet     = pmaxframet;
  if (maxframet == 0) maxframet = 5;
  dispframe     = 0;
  startframe    = 0;
  image         = 0;
  hit           = 0;
  maxhit        = 1000;
  onehit        = FALSE;
  user          = puser;
  active        = 1;
  hitby_sx      = 0;
  hitby_sy      = 0;
  teleport_time = 0;
  invisible     = 0;
  do_hooks      = 0;
  kill          = 0;
  dont_update      = 0;
  dead_by_network = 0;
  
  x             = px;
  y             = py;

  xspd = yspd = dxspd = dyspd = 0;

  // "bound" (plus values to x/y coordinate)
  bound_x       = 0;
  bound_y       = 0;
  bound_w       = 0;
  bound_h       = 0;
  physical_frames = 1;

  backbuf = NULL;

//  frames->add_frames_from_imagefile(filename, DATA_RLE, maxframet);
  physical_frames = frames->get_framecount();
  maxframe = physical_frames-1;

  w = ((RLE_SPRITE*)frames->get_rle_frame(0))->w;
  h = ((RLE_SPRITE*)frames->get_rle_frame(0))->h;

  curframe = startframe;

  backbuf = create_bitmap(w, h);

  for (int un=0; un < 20; un++)
  {
    user_int[un] = 0;
    user_float[un] = 0;
  }
}


Object::~Object()
{
  if (backbuf) destroy_bitmap(backbuf);
  if (kill_frames && frames) delete(frames);
  if (wp) delete(wp);
}

void Object::save_background(gsMap *m)
{
  if (image >= 0 && backbuf)
    if (m->is_on_map(x, y, backbuf->w, backbuf->h))
      blit(m->get_vscreen(), backbuf, (int)x-m->get_map_x(), (int)y-m->get_map_y(), 0, 0, backbuf->w, backbuf->h);
}

void Object::draw(gsMap *m)
{
  if (!invisible && m->is_on_screen(x, y, w, h) && image >= 0 &&
      (restrict_to_map == NULL || restrict_to_map == m))
  {
    draw_rle_sprite(m->get_vscreen(),
                    frames->get_rle_frame(dispframe),
                    (int)x - m->get_map_x(), (int)y - m->get_map_y());

    if (!is_special() && !dont_update) clearlist->add(m, x+bound_x, y+bound_y, w+bound_w, h+bound_h);
  }
}

void Object::script_redraw()
{
  gsMap *m = playmap[0];
  
  if (!invisible && m->is_on_screen(x, y, w, h) && image >= 0 &&
      (restrict_to_map == NULL || restrict_to_map == m))
  {
    draw_rle_sprite(m->get_vscreen(),
                    frames->get_rle_frame(dispframe),
                    (int)x - m->get_map_x(), (int)y - m->get_map_y());

    if (!is_special() && !dont_update) clearlist->add(m, x+bound_x, y+bound_y, w+bound_w, h+bound_h);
  }
  
  if (globals->splitscreen) 
  {
    m = playmap[1];
  
    if (!invisible && m->is_on_screen(x, y, w, h) && image >= 0 &&
        (restrict_to_map == NULL || restrict_to_map == m))
    {
      draw_rle_sprite(m->get_vscreen(),
                      frames->get_rle_frame(dispframe),
                      (int)x - m->get_map_x(), (int)y - m->get_map_y());

      if (!is_special() && !dont_update) clearlist->add(m, x+bound_x, y+bound_y, w+bound_w, h+bound_h);
    }
  }
  
}

void Object::restore_background(gsMap *m)
{
  if (image >= 0 && backbuf)
    if (m->is_on_map(x, y, backbuf->w, backbuf->h))
      blit(backbuf, m->get_vscreen(), 0, 0, (int)x-m->get_map_x(), (int)y-m->get_map_y(), backbuf->w, backbuf->h);
}

int Object::add_hitpoints(int from, float n, int weap, int fx, int fy, float fxspd, float fyspd)
{
  onehit = TRUE;
  hitby = from;
  hitby_sx = fxspd;
  hitby_sy = fyspd;

  if (globals->do_all_hooks || do_hooks)
    script->do_hook_object_hit(glob_lua, this, weap, n, fx, fy);
  if (get_dont_continue()) return 1;

  hit += n;

  return 0;
}

RLE_SPRITE* Object::get_current_sprite()
{
  return frames->get_rle_frame(dispframe);
}

void Object::move()
{
  int nonox = 0, nonoy = 0;
  int nomove = 0;

  if (wp->get_count() > 0)
  {
    Waypoint *twp;
  
    twp = wp->get(wp->get_current());

    if (twp)
    {
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
        }
      }

      // calculate new speeds on waypoint or on start
      if (!nomove && xspd == 0 && yspd == 0)
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
      }

      if (xspd > 0)
        if (x+xspd > twp->get_x()) { x = twp->get_x(); nonox = 1; }
      if (xspd < 0)
        if (x+xspd < twp->get_x()) { x = twp->get_x(); nonox = 1; }
  
      if (yspd > 0)
        if (y+yspd > twp->get_y()) { y = twp->get_y(); nonoy = 1; }
      if (yspd < 0)
        if (y+yspd < twp->get_y()) { y = twp->get_y(); nonoy = 1; }

    } // if twp
  } // count > 0?

  if (!nonox) x += xspd;
  if (!nonoy) y += yspd;

  xspd += dxspd;
  yspd += dyspd;
}

int Object::update()
{
  if (globals->do_all_hooks || do_hooks)
    script->do_hook_object_update(glob_lua, this);
    
  if (kill) return 1;
  
try
{
  if (get_dont_continue()) return 0;

  move();

  if (teleport_time > 0) teleport_time--;

  if (maxframe && maxframet && active)
  {
    curframet++;
    if (curframet >= maxframet) { curframe++; curframet = 0; }
    if (curframe  > maxframe)  { curframe = startframe; }

    switch (type)
    {
      case OBJ_PILE   :  if (curframe > 1) dispframe = 0; else dispframe = curframe;
                         break;

      case OBJ_POOL1  :  case OBJ_POOL2 :
                         if (curframe > 2) dispframe = 1; else dispframe = curframe;
                         break;

      case OBJ_RACEPOLEL :
      case OBJ_RACEPOLER :
      case OBJ_RACEPOLEU :
      case OBJ_RACEPOLED :
                         if (curframe > 5) dispframe = 10-curframe;
                         else dispframe = curframe;
                         break;

      default         :  dispframe = curframe;
    } // switch type
  } // if animation

  // specials
  if (active)
  switch (type)
  {
    case OBJ_SWITCHL : case OBJ_SWITCHR : case OBJ_SWITCHU : case OBJ_SWITCHD :
         if (onehit)
         {
           onehit = FALSE;
           if (dispframe == 0) dispframe = 1; else dispframe = 0;
         }
         break;

    case SPOBJ_EXTRA :
         if (xspd >= 0.2 || xspd <= -0.2) dxspd = -dxspd;
         if (yspd >= 0.2 || yspd <= -0.2) dyspd = -dyspd;
         onehit = FALSE;
         break;

    case SPOBJ_BUMPU : case SPOBJ_BUMPD : case SPOBJ_BUMPL : case SPOBJ_BUMPR :
         if (curframe > 0)
         {
           switch (curframe)
           {
             case 1 : dispframe = 0;
                      gsound->bump_wall(x, y);
                      curframe++;
                      break;
             case 2 : dispframe = 1; curframe++; break;
             case 3 : dispframe = 2; curframe++; break;
             case 4 : dispframe = 0; curframe = 0; break;
           }
         }
         onehit = FALSE;
         break;

    case SPOBJ_GRAVTRAP :
         apply_gravity();
         break;
         
    case SPOBJ_GWALL_L : case SPOBJ_GWALL_R : case SPOBJ_GWALL_U : case SPOBJ_GWALL_D :
         apply_lin_gravity();
         maxframet = user;                           
         break;

    default :
         if (onehit) onehit = FALSE;
  }

  // object dead?
  if (hit >= maxhit)
  {
    // if this object was killed by us (and not over network), send it to the others!
    if (globals->network_game && !dead_by_network)
    {
      bool spec = 0; if (is_special()) spec = 1;
      net->game_send_object_dead(nr, spec);
    }

    clearlist->add(playmap[0], x, y, w, h);
    if (globals->splitscreen) clearlist->add(playmap[1], x, y, w, h);

    if (globals->do_all_hooks || do_hooks)
      script->do_hook_object_dead(glob_lua, this);
    if (get_dont_continue()) return 1;

    int n,m;

    switch (type)
    {
      case OBJ_BUILDING_GRAV :
      case OBJ_BUILDING1 :
      case OBJ_BUILDING2 :
      case OBJ_BUILDING3 :
      case OBJ_BUILDING4 :
      case OBJ_BUILDING4_2 :
      case OBJ_TUNNEL1 :
      case OBJ_POOL1 :
      case OBJ_POOL2 :
           effects->explosions->add(EXP_BIG_1, 4, 0, x+w/2 - 20, y+h-60);
           
           if (!globals->network_game || net->is_server()) 
           {
             effects->craters->add(x+w/4, y+h-gsrand()%10, 25, 1);
             effects->craters->add(x+w-w/4, y+h-gsrand()%10, 25, 1);
           }

           for (m=0; m < 2; m++)
             gsound->play_sound(SOUND_EXPL_MEDIUM1+gsrand()%4, x, y);

           for (n=0; n < 10; n++)
           {
             effects->explosions->add(EXP_SMALL_2+gsrand()%4, 3, 20+gsrand()%80, x+gsrand()%w, y+gsrand()%h);
             effects->pixels->add(70, x+gsrand()%w, y+gsrand()%h, 15, 15,
                                  2-gsrand()%4, -gsrand()%3, 50,
                                  PIX_BLUEWHITE+gsrand()%4);
             effects->pixels->add(20, x+gsrand()%w, y+gsrand()%h, 15, 15,
                                  2-gsrand()%4, -gsrand()%2, 50,
                                  PIX_DEBRIS1+gsrand()%3);
           }
           break;

      default :
           effects->explosions->add(-1, expl_sm1_01, 10, 2, 1, x+w/2-3, y+h/2-3, globals->gamedat);
           if (!globals->network_game || net->is_server()) 
             effects->craters->add(x+w/2, y+h-3, 10, 1);
           effects->pixels->add(80, x+w/2, y+h/2, 0, 0, 2-(gsrand() % 3), gsrand() % 2, 50, globals->tbl_white, 10, 60*3, 50);
           break;
    }

    return 1;
  }

  check_special_collisions();
}
catch( ... )
{
  errors->log(2, "Exception in Objekt-Update aufgetreten!");
}

  return 0;
}

void Object::check_collisions(int special)
{
  
  int move = 0;
  if (xspd || yspd) move = 1;

  if (script->is_flag(script->flag_hook_object_with_object) &&
      (globals->do_all_hooks || do_hooks))
  {
    int ox = fast_ftol(x+w*0.2);
    int oy = fast_ftol(y+h*0.2);
    int ow = fast_ftol(w-w*0.1);
    int oh = fast_ftol(h-h*0.1);

    // collison with normal objects?
    Object *temp = objects->get_first_object(), *next_object;
    while (temp != NULL)
    {
      next_object = temp->get_next();
      if (temp != this && (move || temp->get_xspd() || temp->get_yspd()))
      {
        int tx = fast_ftol(temp->get_x()+w*0.2);
        int ty = fast_ftol(temp->get_y()+h*0.2);
        int tw = fast_ftol(temp->get_width()-w*0.1);
        int th = fast_ftol(temp->get_height()-h*0.1);

        if (collision(ox, oy, ow, oh,
                      tx, ty, tw, th))
        {
          script->do_hook_object_with_object(glob_lua, this, temp);
          break;
        }
      }
      temp = next_object;      
    } // while temp != NULL
    
    // collision with special objects?
    temp = objects->get_first_spobject();
    while (temp != NULL)    
    {
      next_object = temp->get_next();
      if (temp != this && (move || temp->get_xspd() || temp->get_yspd()))
      {
        int tx = fast_ftol(temp->get_x()+w*0.2);
        int ty = fast_ftol(temp->get_y()+h*0.2);
        int tw = fast_ftol(temp->get_width()-w*0.1);
        int th = fast_ftol(temp->get_height()-h*0.1);

        if (collision(ox, oy, ow, oh,
                      tx, ty, tw, th))
        {
          script->do_hook_object_with_object(glob_lua, this, temp);
          break;
        }
      }
      temp = next_object;      
    } // while temp != NULL

  }
}

void Object::check_special_collisions()
{
  
  Object *temp = objects->get_first_spobject();

  int ox = fast_ftol(x);
  int oy = fast_ftol(y);

  while (temp && temp->get_type() == SPOBJ_TELEPORT1)
  {
    int tx = fast_ftol(temp->get_x());
    int ty = fast_ftol(temp->get_y());

    if (temp->get_nr() != nr &&
        collision(ox, oy, w, h,
                  tx, ty, temp->get_width(), temp->get_height()))
    {
      switch (temp->get_type())
      {
        case SPOBJ_TELEPORT1 :
             if (teleport_time == 0 &&
                 (get_type() == SPOBJ_EXTRA || get_type() == SPOBJ_FLAG))
             {
                Object *temp2 = objects->get_spobject(temp->get_user());
                effects->explosions->add(-1, effect_beam2_01, 10, 1, 0, x, y, globals->gamedat);
                set_x(temp2->get_x()+temp2->get_width()/2);
                set_y(temp2->get_y()+temp2->get_height()/2);
                teleport_time = 120;
                effects->explosions->add(-1, effect_beam2_01, 10, 1, 0, x, y, globals->gamedat);
                gsound->beam_bullet(x, y);
             }
             break;
      } // switch type
    }

    temp = temp->get_next();
  }
}

void Object::apply_gravity_to(int px, int py, float *pxs, float *pys, int ptype)
{
  
  float angle,scale_x,scale_y,xrot,yrot;
  int gx,gy;
  float s;
  float pi = 3.1415926;

  gx = fast_ftol(x);
  gy = fast_ftol(y);

  angle = V_GetAngle(px,py,gx,gy);
  int ma = fast_ftol(angle*180.0/pi);
  xrot = sinus[ma];
  yrot = cosinus[ma];

//  globals->user_float[0] = angle*180.0/pi;
//  globals->user_float[1] = yrot;

  if (px >= gx)
    scale_x = -xrot;
  else
    scale_x = xrot;

  scale_y = yrot;

  if (user > 100) user = 100; if (user <= 0) user = 1;
  if (ptype==0) s = (100-user);
  else s = (100-user)/2.5;

  int is = fast_ftol(s);
  
  *pxs += scale_x/is;
  if (ptype == 0)
    *pys += scale_y/is;
  else
    *pys -= scale_y/is;

/*
  if (user > 100) user = 100; if (user <= 0) user = 1;
  s = user/100.0/10.0;
  
  int maxdist = 320;
  int dist = (int)distance((int)gx, (int)gy, (int)px, (int)py);
  if (dist > maxdist) dist = maxdist;
  
  float d = dist/maxdist;
  float a = 1-d;
  s = a * s;
  
  if (s != 0)
  {
    *pxs += scale_x*s;
    if (ptype == 0)
      *pys += scale_y*s;
    else
      *pys -= scale_y*s;
  }
*/
}

void Object::apply_lin_gravity_to(int px, int py, float *pxs, float *pys, float factor)
{
  int tx1,ty1,tx2,ty2,tw,th,xab,yab,xabmax,yabmax;
  float p;

  tx1 = px;
  ty1 = py;
  tx2 = (int)x;
  ty2 = (int)y;
  tw  = w;
  th  = h;

  if (user > 200) user = 200; if (user <= 0) user = 1;

  switch (type)
  {
    case SPOBJ_GWALL_L :
                   xab = tx2-tx1;
                   xabmax = user*10;

                   if ( (ty1 > ty2) && (ty1 < ty2+th) &&
                        (xab < xabmax) && (xab > 0) )
                   {
                     p = 1.0-((float)xab/(float)xabmax);
                     *pxs += ((float)user * p) * factor;
                   }
                   break;
    case SPOBJ_GWALL_R :
                   xab = tx1-tx2;
                   xabmax = user*10;

                   if ( (ty1 > ty2) && (ty1 < ty2+th) &&
                        (xab < xabmax) && (xab > 0) )
                   {
                     p = 1.0-((float)xab/(float)xabmax);
                     *pxs -= ((float)user * p) * factor;
                   }
                   break;
    case SPOBJ_GWALL_U :
                   yab = ty2-ty1;
                   yabmax = user*10;

                   if ( (tx1 > tx2) && (tx1 < tx2+tw) &&
                        (yab < yabmax) && (yab > 0) )
                   {
                     p = 1.0-((float)yab/(float)yabmax);
                     *pys -= ((float)user * p) * factor;
                   }
                   break;
    case SPOBJ_GWALL_D :
                   yab = ty1-ty2;
                   yabmax = user*10;

                   if ( (tx1 > tx2) && (tx1 < tx2+tw) &&
                        (yab < yabmax) && (yab > 0) )
                   {
                     p = 1.0-((float)yab/(float)yabmax);
                     *pys += ((float)user * p) * factor;
                   }
                   break;
  } // switch type

}

void Object::apply_gravity()
{
  float xs, ys;

  // players
  for (int n=0; n < globals->players; n++)
  {
    xs = player[n]->get_xspd();
    ys = player[n]->get_yspd();

    if (!player[n]->get_land())
    {
      apply_gravity_to( player[n]->get_x()+5, player[n]->get_y()+5,
                        &xs, &ys, 0 );

      player[n]->set_xspd(xs);
      player[n]->set_yspd(ys);
    }
  }

  // bullets
  Bullet *bul = effects->bullets->get_first(), *nextbul;
  while (bul)
  {
    nextbul = bul->get_next();

    xs = bul->get_xspd();
    ys = bul->get_yspd();

    apply_gravity_to( bul->get_x(), bul->get_y(),
                      &xs, &ys, 1 );

    bul->set_xspd(xs);
    bul->set_yspd(ys);

    bul = nextbul;
  }
}

void Object::apply_lin_gravity()
{
  float xs, ys;

  // players
  for (int n=0; n < globals->players; n++)
  {
    xs = player[n]->get_xspd();
    ys = player[n]->get_yspd();

    if (!player[n]->get_land())
    {
      apply_lin_gravity_to( player[n]->get_x()+5, player[n]->get_y()+5,
                            &xs, &ys, 0.001 );

      player[n]->set_xspd(xs);
      player[n]->set_yspd(ys);
    }
  }

  // bullets
  Bullet *bul = effects->bullets->get_first(), *nextbul;
  while (bul)
  {
    nextbul = bul->get_next();

    xs = bul->get_xspd();
    ys = bul->get_yspd();

    switch (bul->get_type())
    {
      case W_MINE :
         break;
      default:
         apply_lin_gravity_to( bul->get_x(), bul->get_y(),
                               &xs, &ys, 0.003 );      
    }

    bul->set_xspd(xs);
    bul->set_yspd(ys);

    bul = nextbul;
  }
}

gsObject::gsObject()
{
  first_object = last_object = NULL;
  first_spobject = last_spobject = NULL;
  count = 0;
}

gsObject::~gsObject()
{
  Object *temp, *next_object;
  temp = first_object;

  while (temp != NULL)
  {
    next_object = temp->get_next();
    delete temp;
    temp = next_object;
  }

  temp = first_spobject;

  while (temp != NULL)
  {
    next_object = temp->get_next();
    delete temp;
    temp = next_object;
  }
}

Object *gsObject::add(int num, int ptype, int px, int py, int puser, int pmaxframet)
{
  Object *temp_object = NULL;

  if (ptype < 1000)
    temp_object = new Object(num, (OBJECT_TYPE)ptype, px, py, puser, pmaxframet);
  else if (ptype >= 10000)
    temp_object = new Object(num, (OBJECT_TYPE)OBJ_USER, images->get_image(ptype), px, py, puser, pmaxframet);


  if (temp_object != NULL)
  {
    if (first_object==NULL)
    {
      last_object = first_object = temp_object;
      first_object->set_next(NULL); first_object->set_prev(NULL);
      temp_object->set_next(NULL); temp_object->set_prev(NULL);
    }
    else
    {
      last_object->set_next(temp_object);
      temp_object->set_prev(last_object);
      last_object = temp_object;
      temp_object->set_next(NULL);
    }
  } // temp_object != NULL

  count++;
  temp_object->set_dont_continue(0);
  return temp_object;
}

Object *gsObject::add_special(int num, int ptype, int px, int py, int puser, int pmaxframet)
{
  Object *temp_object = NULL;

  if (ptype < 1000)
    temp_object = new Object(num, (SPOBJECT_TYPE)ptype, px, py, puser, pmaxframet);
  else if (ptype >= 10000)
    temp_object = new Object(num, (SPOBJECT_TYPE)OBJ_USER, images->get_image(ptype), px, py, puser, pmaxframet);

  if (temp_object != NULL)
  {
    if (first_spobject==NULL)
    {
      last_spobject = first_spobject = temp_object;
      first_spobject->set_next(NULL); first_spobject->set_prev(NULL);
      temp_object->set_next(NULL); temp_object->set_prev(NULL);
    }
    else
    {
      last_spobject->set_next(temp_object);
      temp_object->set_prev(last_spobject);
      last_spobject = temp_object;
      temp_object->set_next(NULL);
    }
  } // temp_object != NULL

  count++;
  temp_object->set_dont_continue(0);
  return temp_object;
}

void gsObject::remove(Object *temp)
{
  Object *next_object, *prev_object;

  if (temp != NULL)
  {
    prev_object = temp->get_prev();
    next_object = temp->get_next();

    if (prev_object==NULL) first_object = next_object;
    else prev_object->set_next(next_object);

    if (next_object==NULL) last_object = prev_object;
    else next_object->set_prev(prev_object);

    delete temp;
    temp = NULL;

    count--;
  }
}

void gsObject::remove_special(Object *temp)
{
  Object *next_object, *prev_object;

  if (temp != NULL)
  {
    prev_object = temp->get_prev();
    next_object = temp->get_next();

    if (prev_object==NULL) first_spobject = next_object;
    else prev_object->set_next(next_object);

    if (next_object==NULL) last_spobject = prev_object;
    else next_object->set_prev(prev_object);

    delete temp;
    temp = NULL;

    count--;
  }
}

void gsObject::remove_for_script(Object *temp)
{
  if (temp) temp->remove();
  else errors->log(1, "script error", "tried removing non-existent object!");
}

void gsObject::draw(gsMap *m)
{
  Object *temp;
  temp = first_object;

  while (temp != NULL)
  {
    if (!temp->get_dont_update() && !temp->get_remove()) temp->draw(m);
    temp = temp->get_next();
  }
}

void gsObject::draw_special(gsMap *m)
{
  Object *temp;

  temp = first_spobject;

  while (temp != NULL)
  {
    if (!temp->get_remove()) temp->draw(m);
    temp = temp->get_next();
  }
}

void gsObject::restore_background(gsMap *m)
{
  Object *temp;

  temp = first_spobject;

  while (temp != NULL)
  {
    if (!temp->get_remove()) temp->restore_background(m);
    temp = temp->get_next();
  }
}

void gsObject::save_background(gsMap *m)
{
  Object *temp;

  temp = first_spobject;

  while (temp)
  {
    if (!temp->get_remove()) temp->save_background(m);
    temp = temp->get_next();
  }
}

void gsObject::update()
{
  Object *temp, *next_object;
  temp = first_object;

  while (temp != NULL)
  {
    next_object = temp->get_next();
    if (!temp->get_remove()) temp->check_collisions(0);
    if (temp->update()) remove(temp);
    temp = next_object;
  }

  temp = first_spobject;

  while (temp != NULL)
  {
    next_object = temp->get_next();
    if (!temp->get_remove()) temp->check_collisions(1);
    if (temp->update()) remove_special(temp);
    temp = next_object;
  }

}

Object *gsObject::get_object(int n)
{
  Object *temp;
  temp = first_object;

  while (temp != NULL)
  {
    if (temp->get_nr() == n) return temp;
    temp = temp->get_next();
  }

  return NULL;
}

Object *gsObject::get_spobject(int n)
{
  Object *temp;
  temp = first_spobject;

  while (temp != NULL)
  {
    if (temp->get_nr() == n) return temp;
    temp = temp->get_next();
  }

  return NULL;
}


