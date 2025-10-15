/*
 *   GRAVITY STRIKE -- menu/controls/mlevsel.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "mlevsel.h"
#include "allegro5_wrapper.h"
#include "../../gsclear.h"
#include "../../gsglob.h"
#include "../../gserror.h"
#include "../gsmd.h"
#include "mtypes.h"
#include "../mpixels.h"
#include <stdio.h>

#define M_LEVSEL_DISP_ALL       0
#define M_LEVSEL_DISP_RACES     1
#define M_LEVSEL_DISP_MISSIONS  2

extern mPixelRain *mpixels;

mLevelSelect::~mLevelSelect()
{
  if (arrow1) delete(arrow1);
  if (arrow2) delete(arrow2);
  if (bar)    delete(bar);
}

void mLevelSelect::create(int px, int py, int sx, int sy, int pw, int ph)
{
  int asx, asy;

  mControl::create(px, py, sx, sy);
  w = pw;
  h = ph;
  active_entry = 0;
  old_active_entry = -1;

  arrow1 = new mArrowButton();
  arrow2 = new mArrowButton();
  bar    = new mBar();

  asx = asy = 0;

  if (sx) asx = sx+w-14-2; if (sy) asy = sy+2;
  arrow1->create(px+w-14-2, py+2, asx, asy, A_UP);

  if (sx) asx = sx+w-14-2; if (sy) asy = sy+h-14-2;
  arrow2->create(px+w-14-2, py+h-14-2, asx, asy, A_DOWN);

  if (sx) asx = sx+w-14-2; if (sy) asy = sy+16+2;
  bar->create(px+w-14-2, py+16+2, asx, asy, 14, h-36);

  // do not activate when no levels available
  if (globals->max_levels <= 0) set_flag(MC_INACTIVE);
  
  init();
}

void mLevelSelect::init()
{
  FONT *f = (FONT*)globals->fontdat[FONT_IMPACT10].dat;
  int th = text_height(f) + 1;
  max_lines = int(h / th);
  if ((max_lines)*th+2 > h) max_lines--;
  disp_from = 0;
  if (max_lines >= globals->max_levels) max_lines = globals->max_levels-1;
  if (max_lines < 0) max_lines = 0;

  active_entry = 0;
  old_active_entry = -1;
}

void mLevelSelect::draw(BITMAP *v)
{
  if (!(flags & MC_INVISIBLE))
  {
    int c = color1;
    int tc = color1;
//    if (flags & MC_ACTIVE) { c = color2; tc = globals->col_green; }
    int theight;
    char text[50] = "";

    FONT *f = (FONT*)globals->fontdat[FONT_IMPACT10].dat;
    theight = text_height(f);
    text_mode(-1);

    rect(v, (int)x, (int)y, (int)x+w, (int)y+h, c);

    int n=0;
    int active;

    int nc = makecol8(0, 0, 120);

    if (max_lines >= 0 && globals->max_levels > 0)
    {
      // background bar when active entry is visible
      if (active_entry >= disp_from && active_entry <= disp_from+max_lines)
        rectfill(v, (int)x+2, (int)y+(active_entry-disp_from)*theight+3,
                    (int)x+w-18, (int)y+(active_entry-disp_from+1)*theight, nc);
  
      for (n=0; n <= max_lines; n++)
      {
        int len;
        
        if (globals->network_game) sprintf(text, "%s [%d]", globals->level_files[disp_from+n].name, globals->level_files[disp_from+n].max_players);
  /*      else if (globals->singleplayer)
        {
          if (globals->level_files[disp_from+n].activate_race)
            sprintf(text, "[R] %s", globals->level_files[disp_from+n].name);
          else
            sprintf(text, "[M] %s", globals->level_files[disp_from+n].name);
        }
  */      else sprintf(text, "%s", globals->level_files[disp_from+n].name);
        int maxchars = strlen(text);
  
        len = text_length(f, text);
  
        while (len > w-20)
        {
          if (maxchars > 1) maxchars--; else len = 0;
          text[maxchars] = '\0';
          len = text_length(f, text);
        }
  
        active = 0;
        if (strlen(text) > 0) active = 1;
  
        if (mouse_x > x && mouse_x < x+w-20 &&
            mouse_y > y+theight*n && mouse_y < y+theight*(n+1))
        {
          if (active && mouse_b & 1) active_entry = disp_from+n;
          tc = globals->col_green;
        }
        else
          tc = color1;
  
        if (active_entry == disp_from+n) tc = globals->col_green;
  
        aatextout(v, f, text, (int)x+4, (int)y+n*theight+2, tc);
      }

      draw_bar_lines(v);

    }

    arrow1->draw(v);
    arrow2->draw(v);
    bar->draw(v);
   
    clearlist->add(v, x, y, w, h);
    
  }
}

void mLevelSelect::draw_bar_lines(BITMAP *v)
{
  float bp = (float)max_lines / (float)globals->max_levels;
  int   bh = int(bar->get_h() * bp);
  float t1 = (float)bar->get_h() / (float)globals->max_levels;
  int  yab = int(t1 * disp_from);

  rectfill(v, (int)bar->get_x()+1, (int)bar->get_y()+1+yab,
              (int)bar->get_x()+bar->get_w()-1,
              (int)bar->get_y()+bh+1+yab,
              globals->col_blue);

  for (int ty=(int)bar->get_y()+1+yab; ty <= (int)bar->get_y()+bh+1+yab; ty+=2)
    hline(v, (int)bar->get_x()+1, ty, (int)bar->get_x()+bar->get_w()-1, 0);
}

int mLevelSelect::update()
{
  arrow1->update();
  arrow2->update();
  bar->update();

  return (mControl::update());
}

void mLevelSelect::check()
{
  if (arrow1->get_flags() & MC_PUSHED)
  {
    if (disp_from > 0) disp_from--;
    mouse_b = 0;
    arrow1->del_flag(MC_PUSHED);
  }

  if (arrow2->get_flags() & MC_PUSHED)
  {
    if (disp_from+max_lines < globals->max_levels-1) disp_from++;
    mouse_b = 0;
    arrow2->del_flag(MC_PUSHED);
  }

  arrow1->check();
  arrow2->check();
  bar->check();

  mControl::check();
}

void mLevelSelect::set_dest_x(int n)
{
  mControl::set_dest_x(n);

  arrow1->set_dest_x(n+2);
  arrow2->set_dest_x(n+w-2);
  bar->set_dest_x(n+16);
}

void mLevelSelect::set_dest_y(int n)
{
  mControl::set_dest_y(n);

  arrow1->set_dest_y(n+h-14-2);
  arrow2->set_dest_y(n+h-14-2);
  bar->set_dest_y(n+h-14-2);
}

void mLevelSelect::scroll_visible()
{
  if (active_entry > disp_from+max_lines)
    disp_from += active_entry - (disp_from+max_lines) + max_lines/2;

  if (disp_from >= globals->max_levels - max_lines)
    disp_from = globals->max_levels-max_lines-1;
}

void  mLevelSelect::set_int(int n)
{
  if (n <= globals->max_levels && n >= 0)
    active_entry = n;
  else
    active_entry = 0;

  scroll_visible();
}

