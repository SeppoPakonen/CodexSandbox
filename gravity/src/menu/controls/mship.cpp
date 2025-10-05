/*
 *   GRAVITY STRIKE -- menu/controls/mship.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "mship.h"
#include "allegttf.h"
#include "../../gsplayer.h"
#include "../../gsclear.h"
#include "../../gsglob.h"
#include "../../gslng.h"
#include "../../gshelper.h"
#include "../../gserror.h"
#include "../gsmd.h"
#include "mtypes.h"
#include "../mpixels.h"

extern mPixelRain *mpixels;

mShipIcon::~mShipIcon()
{
  if (pl)   delete(pl);
}

void mShipIcon::create(int px, int py, int sx, int sy, int pw, int ph)
{
  mControl::create(px, py, sx, sy);
  w = pw;
  h = ph;
  error = 0;
  shipimage1 = shipimage2 = NULL;
  strcpy(text, "n/a");
  rot = 0;
  dead = 0;
}

void mShipIcon::set_text(char *n)
{
  if (strcmp(n, "createnewship") == 0)
  {
    pl = NULL;
    strcpy(text, language->get_string(T_MENU_NEWSHIP));
    newship = 1;
    strcpy(filename, n);
  }
  else
  {
    newship = 0;
    strncpy(filename, n, 30);
    pl = new gsPlayer(0, 0, 0, n);
    shipimage1 = pl->get_ship(0);
    shipimage2 = pl->get_ship(1);
    char tmp[11];
    strncpy(tmp, pl->get_ship_name(), 10);
    strupr(tmp);
    strncpy(text, tmp, 10);
    
    stat_defense = ( pl->get_ship_value(PV_HULL)*5 + 
                     pl->get_ship_value(PV_SHIELD)*5 + 
                     pl->get_ship_value(PV_HULL_RE) +
                     pl->get_ship_value(PV_SHIELD_RE) +
                     pl->get_ship_value(PV_WALLSHIELD) ) / 5;
                     
    stat_agility = ( pl->get_ship_value(PV_TURN_SPEED) +
                     pl->get_ship_value(PV_MAX_SPEED) +
                     pl->get_ship_value(PV_ACCELERATION) +
                     pl->get_ship_value(PV_FUEL_CONS) ) / 4;
                     
    stat_attack  = ( pl->get_ship_value(PV_WEAPON_RE) +
                     pl->get_ship_value(PV_WEAPON_STORAGE) +
                     int(pl->get_ship_value(PV_WEAPON_SLOTS)*16.7) ) / 3;

//    sprintf(text, "%d-%d-%d", stat_defense, stat_attack, stat_agility);

    if (strcmp(text, "N/A") == 0) error = 1;
  }
}

void mShipIcon::draw(BITMAP *v)
{
  if (!(flags & MC_INVISIBLE))
  {
    int c = color1;
    int tc = color2;
    BITMAP *shipimage;
    FONT *f = (FONT*)globals->fontdat[FONT_IMPACT10].dat;
    text_mode(-1);
    char buf[21];
    strncpy(buf, filename, 20);

    if (strstr(strupr(buf), "DEF")) { tc = globals->col_yellow; }

    if (flags & MC_ACTIVE && !(flags & MC_INACTIVE))
    {
      c = color2; tc = globals->col_green;
      shipimage = shipimage2;

      if (newship) tc = globals->col_yellow;

      if (strstr(filename, "def")) { tc = globals->col_orange; }

      if (!newship && strlen(pl->get_ship_desc()) > 0)
      {
        mytextout_center(v, f, pl->get_ship_desc(), 40, 600, 400, globals->col_yellow);
      }
    }
    else
    {
      shipimage = shipimage1;
    }

    int tlen = 0, theight = 0;
    if (strlen(text) > 0)
    {
      tlen = text_length(f, text);
      theight = text_height(f);
    }
    rect(v, (int)x, (int)y, (int)x+w, (int)y+h-theight-2, c);

    if (shipimage && !dead && !newship)
    {
      rotate_sprite(v, shipimage, int(x+1+w/4-shipimage->w/2), int(y+(h-theight-2)/2-shipimage->h/2), ftofix(rot));
      // info bars
      rect(v, int(x+4+w/2), int(y+10),    int(x+4+w/2+20), int(y+10+2), globals->col_blue);
      hline(v, int(x+4+w/2+1), int(y+11), int(x+4+w/2+1+(stat_agility/5)*0.9), globals->col_green);

      rect(v, int(x+4+w/2), int(y+10+5),  int(x+4+w/2+20), int(y+10+7), globals->col_blue);
      hline(v, int(x+4+w/2+1), int(y+16), int(x+4+w/2+1+(stat_attack/5)*0.9), globals->col_orange);

      rect(v, int(x+4+w/2), int(y+10+10), int(x+4+w/2+20), int(y+10+12), globals->col_blue);
      hline(v, int(x+4+w/2+1), int(y+21), int(x+4+w/2+1+(stat_defense/5)*0.9), globals->col_yellow);
    }
    else
    if (newship && !dead)
    {
      aatextout(v, f, "?", (int)x+w/2-2, (int)y+(h-theight-2)/2-5, tc);
    }

    int l = strlen(text);
    if (l > 0)
    {
      char text2[10]; strcpy(text2, "");
      if (l > 9)
      {
        strncpy(text2, text, 8);
        text2[8] = '.';
        text2[9] = '\0';
      }
      else
      {
        strcpy(text2, text);
      }

      tlen = text_length(f, text2);
      aatextout(v, f, text2, (int)x+w/2-tlen/2, (int)y+h-theight, tc);
    }

    clearlist->add(v, x, y, w, h);
  }
}

int mShipIcon::update()
{
  if (flags & MC_ACTIVE)
  {
    rot++;
    if (rot > 256) rot = 1;
  }
  else if (rot > 0) rot--;

  return (mControl::update());
}

void mShipIcon::do_explosion()
{
  dead = 1;
  del_flag(MC_ACTIVE);
  set_flag(MC_INACTIVE);

  mpixels->add(400, int(x+w/2), int(y+(h-22)/2),
               4, 4, 0, 0, 25,
               globals->tbl_all, 256, 1000);
}

