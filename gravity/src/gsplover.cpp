/*
 *   GRAVITY STRIKE -- gsplover.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "gsplover.h"
#include "gsmap.h"
#include "gsplayer.h"
#include "gsglob.h"

gsPlayerOverlay::gsPlayerOverlay(gsPlayer *p)
{
  opl = p;
  back = NULL;

  w = 50;
  h = 40;

  back = create_bitmap(w, h);
  clear_bitmap(back);

  strcpy(pl_name, "");
}

gsPlayerOverlay::~gsPlayerOverlay()
{
  if (back) destroy_bitmap(back);
}

void gsPlayerOverlay::set_map(gsMap *m)
{
  omap = m;
}

void gsPlayerOverlay::update_player_name()
{
  int maxlen = 10;
  strncpy(pl_name, opl->get_name(), maxlen);
  if ((signed)strlen(opl->get_name()) > maxlen)
  {
    if (pl_name[maxlen-1] == ' ') maxlen--;
    pl_name[maxlen] = '.';
    pl_name[maxlen+1] = '\0';
  }
}

void gsPlayerOverlay::save()
{
  x = int(opl->get_x() - omap->get_map_x() - w/2 + opl->get_width()/2);
  y = int(opl->get_y() - omap->get_map_y() - h/2 + opl->get_height()/2);

  // save background
  blit( omap->get_vscreen(), back,
        x, y, 0, 0, w, h );
}

void gsPlayerOverlay::draw()
{
  x = int(opl->get_x() - omap->get_map_x() - w/2 + opl->get_width()/2);
  y = int(opl->get_y() - omap->get_map_y() - h/2 + opl->get_height()/2);

  // draw bounding lines
  int bc = globals->col_grey;
  hline( omap->get_vscreen(), x, y, x+4, bc );
  hline( omap->get_vscreen(), x, y+h-1, x+4, bc );
  hline( omap->get_vscreen(), x+w-1, y, x+w-5, bc );
  hline( omap->get_vscreen(), x+w-1, y+h-1, x+w-5, bc );
  vline( omap->get_vscreen(), x, y, y+h-1, bc );
  vline( omap->get_vscreen(), x+w-1, y, y+h-1, bc );

  // draw status bars
  float hullpercent   = opl->get_hull() / opl->get_max_hull();
  float shieldpercent = opl->get_shield() / opl->get_max_shield();
  BITMAP *hullbit = (BITMAP *)globals->paneldat[panel_statline_hull].dat;
  BITMAP *shieldbit = (BITMAP *)globals->paneldat[panel_statline_shield].dat;

  masked_blit( hullbit, omap->get_vscreen(),
               0, 0, x + w/2 - hullbit->w/2, y+h-7, int(hullbit->w*hullpercent), hullbit->h );
  masked_blit( hullbit, omap->get_vscreen(),
               0, 0, x + w/2 - hullbit->w/2, y+h-6, int(hullbit->w*hullpercent), hullbit->h );
  masked_blit( shieldbit, omap->get_vscreen(),
               0, 0, x + w/2 - hullbit->w/2, y+h-4, int(shieldbit->w*shieldpercent), shieldbit->h );
  masked_blit( shieldbit, omap->get_vscreen(),
               0, 0, x + w/2 - hullbit->w/2, y+h-3, int(shieldbit->w*shieldpercent), shieldbit->h );

  // name
  text_mode(-1);
  textout_centre_ex( omap->get_vscreen(), (FONT*)globals->fontdat[FONT_MINIFONT].dat, pl_name, x+w/2, y+2, globals->col_yellow, -1);
}

void gsPlayerOverlay::restore()
{
  // restore background
  blit( back, omap->get_vscreen(), 0, 0,
        x, y, w, h );
}

