/*
 *   GRAVITY STRIKE -- menu/controls/mtextbox.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "mtextbox.h"
#include "allegttf.h"
#include "../../gsclear.h"
#include "../../gsglob.h"
#include "../../gserror.h"
#include "../../gslng.h"
#include "../../gshelper.h"
#include "../gsmd.h"
#include "mtypes.h"
#include "../mpixels.h"
#include <stdio.h>

extern mPixelRain *mpixels;

mTextBox::~mTextBox()
{
  if (arrow1) delete(arrow1);
  if (arrow2) delete(arrow2);
  if (bar)    delete(bar);
  if (textlines)
    for (int i=0; i < textlines; i++)
      delete[](text[i]); 
}

void mTextBox::create(int px, int py, int sx, int sy, int pw, int ph)
{
  int asx, asy;

  mControl::create(px, py, sx, sy);
  w = pw;
  h = ph;

  arrow1 = new mArrowButton();
  arrow2 = new mArrowButton();
  bar    = new mBar();
  
//errors->log(2, "creating textbox!");

  FONT *f = (FONT*)globals->fontdat[FONT_FIXED].dat;
  int th = text_height(f) + 1;
  max_lines = h / th;
  disp_from = 0;

  asx = asy = 0;

  if (sx) asx = sx+w-14-2; if (sy) asy = sy+2;
  arrow1->create(px+w-14-2, py+2, asx, asy, A_UP);

  if (sx) asx = sx+w-14-2; if (sy) asy = sy+h-14-2;
  arrow2->create(px+w-14-2, py+h-14-2, asx, asy, A_DOWN);

  if (sx) asx = sx+w-14-2; if (sy) asy = sy+16+2;
  bar->create(px+w-14-2, py+16+2, asx, asy, 14, h-36);
  
  textlines = 0;
  for (int i=0; i < 500; i++)
    text[i] = NULL;
  
}

void mTextBox::set_text(char *f)
{
  int size = 0, i = 0;
  char tmpline[1000];
  strncpy(filename, f, 254);

  textlines = 0;
  disp_from = 0;
  lang_begin = 0; lang_end = 0;

  if (textlines)
    for (i = 0; i < textlines; i++)
      delete[](text[i]); 

  if (!exists(filename)) return;
 
  size = file_size(filename);
  
  char lngstr[20], lng_other[20];
  lang_begin = lang_end = -1;
  strcpy(lng_other, "%%LANG_");
  switch (globals->language)
  {
    case LANG_EN : strcpy(lngstr, "%%LANG_EN%%"); break;
    case LANG_DE : strcpy(lngstr, "%%LANG_DE%%"); break;
    case LANG_CR : strcpy(lngstr, "%%LANG_CR%%"); break;
    default : strcpy(lngstr, "%%LANG_EN%%");
  }
 
  if (size > 0)
  {
    FILE *t = fopen(filename, "rt");

    for (i=0;
         i < 1000 && fgets(tmpline, 1000, t) != NULL;
         )
    {
      text[i] = new char[strlen(tmpline) + 1];
      strcpy(text[i], tmpline);

      if (strstr(text[i], lngstr))
      {
        if (lang_begin == -1) lang_begin = i+1;
      }
      else if (strstr(text[i], lng_other))
      {
        if (lang_begin >= 0) lang_end = i;
      }
      
      i++;
    }
    
    textlines = i;
    
    if (lang_begin == -1 && lang_end == -1) 
    {
      lang_begin = 0;
      lang_end = textlines;
    }
    else if (lang_begin > 0 && lang_end == -1)
    {
      lang_end = textlines;
    }
    
    disp_from = lang_begin;
  
    fclose(t);
  }
  
}

void mTextBox::draw(BITMAP *v)
{
  if (!(flags & MC_INVISIBLE))
  {
    int c = color1;
    int tc = color1;
    int theight;

    FONT *f = (FONT*)globals->fontdat[FONT_FIXED].dat;
    theight = text_height(f);
    text_mode(-1);

    rect(v, (int)x, (int)y, (int)x+w, (int)y+h, c);
    rectfill(v, (int)x+1, (int)y+1, (int)x+w-2, (int)y+h-2, globals->col_blue);

    int n;

    for (n=0; n <= max_lines; n++)
    {
      char myline[100];

      if (textlines > n+disp_from)
      {
        strncpy(myline, text[n+disp_from], 88);
        int tl;
        for (tl=0; tl < strlen(myline); tl++)
        {
         // aatextout(v, f, myline, (int)x+4, (int)y+n*theight+2, globals->col_yellow);
           aacharout(v, f, myline[tl], (int)x+4+ tl*6, (int)y+n*theight+2, globals->col_yellow);
        }
      }
    }

    arrow1->draw(v);
    arrow2->draw(v);
    bar->draw(v);

    draw_bar_lines(v);

    clearlist->add(v, x, y, w, h);
  }
}

void mTextBox::draw_bar_lines(BITMAP *v)
{
  if (textlines && lang_end > 0 && lang_begin >= 0 && lang_end-lang_begin >= max_lines)
  {
    float bp = (float)max_lines / float(lang_end-lang_begin);
    int   bh = int(bar->get_h() * bp);
    float t1 = (float)bar->get_h() / float(lang_end-lang_begin);
    int  yab = int(t1 * (disp_from-lang_begin));

    rectfill(v, (int)bar->get_x()+1, (int)bar->get_y()+1+yab,
                (int)bar->get_x()+bar->get_w()-1,
                (int)bar->get_y()+bh+1+yab,
                globals->col_blue);

    for (int ty=(int)bar->get_y()+1+yab; ty <= (int)bar->get_y()+bh+1+yab; ty+=2)
      hline(v, (int)bar->get_x()+1, ty, (int)bar->get_x()+bar->get_w()-1, 0);
  }
}

int mTextBox::update()
{
  arrow1->update();
  arrow2->update();
  bar->update();

  return (mControl::update());
}

void mTextBox::check()
{
  if (arrow1->get_flags() & MC_PUSHED || key[KEY_UP])
  {
    if (disp_from > lang_begin) disp_from--;
//    mouse_b = 0;
    arrow1->del_flag(MC_PUSHED);
  }

  if (arrow2->get_flags() & MC_PUSHED || key[KEY_DOWN])
  {
    if (disp_from+max_lines < lang_end-1) disp_from++;
//    mouse_b = 0;
    arrow2->del_flag(MC_PUSHED);
  }

  arrow1->check();
  arrow2->check();
  bar->check();

  mControl::check();
}

void mTextBox::set_dest_x(int n)
{
  mControl::set_dest_x(n);

  arrow1->set_dest_x(n+w-16);
  arrow2->set_dest_x(n+w-16);
  bar->set_dest_x(n+w-16);
}

void mTextBox::set_dest_y(int n)
{
  mControl::set_dest_y(n);

  arrow1->set_dest_y(n-14-2);
  arrow2->set_dest_y(n+h-14-2);
  bar->set_dest_y(n-14-2);
}

