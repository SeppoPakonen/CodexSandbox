/*
 *   GRAVITY STRIKE -- gsradar.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "gsradar.h"
#include "gspanel.h"
#include "gsmap.h"
#include "gsplayer.h"
#include "gsobject.h"
#include "gsbase.h"
#include "gsenemy.h"
#include "gshelper.h"
#include "gsglob.h"

gsRadar::gsRadar(gsMap *m, gsPlayer *p, int w, int h)
{
  minimap = radarbit = NULL;
  min_zoom = 10;
  max_zoom = 30;

  map = m;
  pplayer = p;
  zoom = p->get_max_radar_zoom();

  width = w;
  height = h;

  radarbit = create_bitmap(width, height);
  create_minimap();
}

gsRadar::~gsRadar()
{
  if (minimap) destroy_bitmap(minimap);
  if (radarbit) destroy_bitmap(radarbit);
}

void gsRadar::create_minimap()
{
  BITMAP *tempbit;
  if (minimap) destroy_bitmap(minimap);

  // get width of minimap
  mini_width  = int((globals->tile_w * globals->map_w) / zoom);
  mini_height = int((globals->tile_h * globals->map_h) / zoom);

  // create minimap
  minimap = create_bitmap(mini_width, mini_height);

  int draw_w = globals->tile_w * 10;
  int draw_h = globals->tile_h * 10;

  tempbit = create_bitmap(draw_w, draw_h);

  for (int w=0; w < 10; w++)
    for (int h=0; h < 10; h++)
    {
      // draw 100 tiles
      MapDrawBG(tempbit, draw_w * w, draw_h * h, 0, 0, draw_w, draw_h);

      // scale
      stretch_blit(tempbit, minimap,
                   0, 0, draw_w, draw_h,
                   int(draw_w / zoom) * w, int(draw_h / zoom) * h,
                   int(draw_w / zoom), int(draw_h / zoom));
    }

  destroy_bitmap(tempbit);
}

void gsRadar::update()
{
  
  int pos_x, pos_y, pwidth, pheight;

  int pl_x = fast_ftol(pplayer->get_x() / zoom);
  int pl_y = fast_ftol(pplayer->get_y() / zoom);

  int start_x = (pl_x - width/2 < 0) ? 0 : pl_x - width/2;
  int start_y = (pl_y - height/2 < 0) ? 0 : pl_y - height/2;

  // blit minimap to radarbit
  blit(minimap, radarbit, start_x, start_y, 0, 0, width, height);

  if (start_x != 0) pl_x = width/2 + 1;
  if (start_y != 0) pl_y = height/2 + 1;

  // yellow cross at player location
  if (!(pplayer->get_extra_action() && pplayer->get_extra() == EXTRA_CLOAK))
  {
    putpixel(radarbit, pl_x-1, pl_y-1, globals->col_yellow);
    putpixel(radarbit, pl_x+1, pl_y-1, globals->col_yellow);
    putpixel(radarbit, pl_x, pl_y, globals->col_orange);
    putpixel(radarbit, pl_x-1, pl_y+1, globals->col_yellow);
    putpixel(radarbit, pl_x+1, pl_y+1, globals->col_yellow);
  }

  // show other players
  for (int n=0; n < globals->players; n++)
  {
    if (player[n]->get_sig() != pplayer->get_sig() && !player[n]->get_dead() &&
        !(player[n]->get_extra_action() && player[n]->get_extra() == EXTRA_CLOAK))
    {
      pl_x = fast_ftol(player[n]->get_x() / zoom - start_x);
      pl_y = fast_ftol(player[n]->get_y() / zoom - start_y);

      // cross at player location
      if (pl_x >= 0 && pl_x <= width && pl_y >= 0 && pl_y <= height)
      {
        putpixel(radarbit, pl_x-1, pl_y-1, globals->col_grey);
        putpixel(radarbit, pl_x+1, pl_y-1, globals->col_grey);
        putpixel(radarbit, pl_x, pl_y, globals->col_white);
        putpixel(radarbit, pl_x-1, pl_y+1, globals->col_grey);
        putpixel(radarbit, pl_x+1, pl_y+1, globals->col_grey);
      }
    }
  }

  // display bases
  Base *temp;
  temp = bases->get_first_base();

  while (temp != NULL)
  {
    pos_x = fast_ftol(temp->get_land_x() / zoom - start_x);
    pos_y = fast_ftol(temp->get_land_y() / zoom - start_y);
    pwidth = fast_ftol(temp->get_land_w() / zoom);

    if (pos_x - pwidth >= 0 && pos_x <= width && pos_y >= 0 && pos_y <= height)
    {
      hline(radarbit, pos_x, pos_y, pos_x + pwidth, globals->col_orange);
    }

    temp = temp->get_next();
  }

  // some important special objects
  Object *tempo;
  tempo = objects->get_first_spobject();

  while (tempo != NULL)
  {
    if (!tempo->get_invisible() && tempo->get_active())
    switch (tempo->get_type())
    {
      case SPOBJ_FLAG :
        pos_x = fast_ftol(tempo->get_x() / zoom - start_x);
        pos_y = fast_ftol(tempo->get_y() / zoom - start_y);
    
        if (pos_x >= 0 && pos_x <= width && pos_y >= 0 && pos_y <= height)
        {
          rect(radarbit, pos_x, pos_y, pos_x+1, pos_y+1, globals->col_orange);
        }
        break;

      case SPOBJ_TELEPORT1 :
        pos_x = fast_ftol(tempo->get_x() / zoom - start_x);
        pos_y = fast_ftol(tempo->get_y() / zoom - start_y);
    
        if (pos_x >= 0 && pos_x <= width && pos_y >= 0 && pos_y <= height)
        {
          circle(radarbit, pos_x+1, pos_y+1, 2, globals->col_lblue);
        }
        break;

      case SPOBJ_TELEPORT2 :
        pos_x = fast_ftol(tempo->get_x() / zoom - start_x);
        pos_y = fast_ftol(tempo->get_y() / zoom - start_y);
    
        if (pos_x >= 0 && pos_x <= width && pos_y >= 0 && pos_y <= height)
        {
          circle(radarbit, pos_x, pos_y, 1, globals->col_lblue);
        }
        break;

      case SPOBJ_EXTRA :
        pos_x = fast_ftol(tempo->get_x() / zoom - start_x);
        pos_y = fast_ftol(tempo->get_y() / zoom - start_y);
    
        if (pos_x >= 0 && pos_x <= width && pos_y >= 0 && pos_y <= height)
        {
          circle(radarbit, pos_x, pos_y, 1, globals->col_yellow);
        }
        break;

      case SPOBJ_BUMPL : case SPOBJ_BUMPR :
        pos_x = fast_ftol(tempo->get_x() / zoom - start_x);
        pos_y = fast_ftol(tempo->get_y() / zoom - start_y);
        pwidth = fast_ftol(tempo->get_height() / zoom);
    
        if (pos_x >= 0 && pos_x <= width && pos_y+pwidth >= 0 && pos_y <= height)
        {
          vline(radarbit, pos_x, pos_y, pos_y+pwidth, globals->col_orange);
        }
        break;

      case SPOBJ_BUMPU : case SPOBJ_BUMPD :
        pos_x = fast_ftol(tempo->get_x() / zoom - start_x);
        pos_y = fast_ftol(tempo->get_y() / zoom - start_y);
        pwidth = fast_ftol(tempo->get_width() / zoom);
    
        if (pos_x+pwidth >= 0 && pos_x <= width && pos_y >= 0 && pos_y <= height)
        {
          hline(radarbit, pos_x, pos_y, pos_x+pwidth, globals->col_orange);
        }
        break;

      case SPOBJ_GRAVTRAP :
        pos_x = fast_ftol(tempo->get_x() / zoom - start_x);
        pos_y = fast_ftol(tempo->get_y() / zoom - start_y);
    
        if (pos_x >= 0 && pos_x <= width && pos_y >= 0 && pos_y <= height)
        {
          circle(radarbit, pos_x+1, pos_y+1, 2, globals->col_red);
        }
        break;

    } // switch type

    tempo = tempo->get_next();
  }

  // ...and some important objects
  tempo = objects->get_first_object();

  while (tempo != NULL)
  {
    if (!tempo->get_invisible() && tempo->get_active())
    switch (tempo->get_type())
    {
      case OBJ_DOORH : case OBJ_DOORH2 :
        pos_x = fast_ftol(tempo->get_x() / zoom - start_x);
        pos_y = fast_ftol(tempo->get_y() / zoom - start_y);
        pwidth = fast_ftol(tempo->get_width() / zoom);

        if (pos_x+pwidth >= 0 && pos_x <= width && pos_y >= 0 && pos_y <= height)
        {
          if (tempo->get_dispframe() > 0)
            hline(radarbit, pos_x+1, pos_y, pos_x+pwidth+1, globals->col_lblue);
          else
          {
            putpixel(radarbit, pos_x+1, pos_y, globals->col_lblue);
            putpixel(radarbit, pos_x+pwidth+1, pos_y, globals->col_lblue);
          }
        }
        break;

      case OBJ_STONE_B1 : case OBJ_USER :
        pos_x = fast_ftol((tempo->get_x()+tempo->get_width()/2) / zoom - start_x);
        pos_y = fast_ftol((tempo->get_y()+tempo->get_height()/2) / zoom - start_y);
   
        if (pos_x >= 0 && pos_x <= width && pos_y >= 0 && pos_y <= height)
        {
          rect(radarbit, pos_x, pos_y, pos_x+1, pos_y+1, globals->col_lblue);
        }
        break;

    } // switch type

    tempo = tempo->get_next();
  }

  // Enemies
  Enemy *tempe = enemies->get_first_enemy();

  while (tempe != NULL)
  {
    if (!tempe->get_invisible())
    {
      pos_x = fast_ftol(tempe->get_x() / zoom - start_x);
      pos_y = fast_ftol(tempe->get_y() / zoom - start_y);
      pwidth = fast_ftol(tempe->get_width() / zoom);
      pheight = fast_ftol(tempe->get_height() / zoom);

      if (pos_x+pwidth >= 0 && pos_x <= width && pos_y+pheight >= 0 && pos_y <= height)
      {
        rect(radarbit, pos_x, pos_y, pos_x+pwidth, pos_y+pheight, globals->col_orange);
      }
    }
    tempe = tempe->get_next();
  }

}

void gsRadar::set_zoom(float n)
{
  if (zoom >= min_zoom && zoom <= max_zoom)
  {
    zoom = n;
    create_minimap();
    update();
  }
}

