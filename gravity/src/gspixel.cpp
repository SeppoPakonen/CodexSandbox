/*
 *   GRAVITY STRIKE -- gspixel.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "gspixel.h"
#include "gsmap.h"
#include "gseffect.h"
#include "gsclear.h"
#include "gsobject.h"
#include "gshelper.h"
#include "gsglob.h"
#include "gserror.h"
#include <math.h>

extern gsClear    *clearlist;
extern gsEffect   *effects;
extern gsGlobals  *globals;


Pixel::Pixel(float px, float py,
             float pxspd, float pyspd, float pxspd2, float pyspd2,
             int pstartx, int pstarty,
             int pmax_xab, int pmax_yab, int *pcolor_table,
             int pmaxtable, int plifetime, int pdspd, BITMAP *pimage, float pvdec)
{
  bounce = 0;
  lifetime = plifetime;

  x = px;
  y = py;
  xspd = pxspd;
  yspd = pyspd;
  xspd2 = pxspd2;
  yspd2 = pyspd2;
  startx = pstartx;
  starty = pstarty;
  max_xab = pmax_xab;
  max_yab = pmax_yab;

  int a = (pmaxtable/2) + (fxrand() % pmaxtable/2);
  tcolor = a;
  color_table = pcolor_table;
  if (color_table)
    color = color_table[a];
  else
    color = 0;

  dspd = pdspd;
  vdec = pvdec;

  image = pimage;
}

Pixel::~Pixel()
{
}

void Pixel::draw(gsMap *m)
{
  if (image && m->is_on_screen(x, y, image->w, image->h))
  {
    
    int nx = fast_ftol(x - m->get_map_x());
    int ny = fast_ftol(y - m->get_map_y());

    draw_sprite(m->get_vscreen(), image, nx, ny);
    clearlist->add(m, x, y, image->w, image->h);
  }
  else if (m->is_on_screen(x, y, 1, 1))
  {
    
    int nx = fast_ftol(x);
    int ny = fast_ftol(y);

    // Replace direct pixel access with proper Allegro 5 function call
    putpixel(m->get_vscreen(), nx - m->get_map_x(), ny - m->get_map_y(), color);
    clearlist->add(m, nx, ny, 1, 1);
  }
}

void Pixel::draw(BITMAP *m)
{
  
  int nx = fast_ftol(x);
  int ny = fast_ftol(y);

  if (image)
  {
    draw_sprite(m, image, nx, ny);
    clearlist->add(m, nx, ny, image->w, image->h);
  }
  else
  {
    putpixel(m, nx, ny, color);
//    putpixel(m, (int)x, (int)y, color);
    clearlist->add(m, nx, ny, 1, 1);
  }
}

void Pixel::move(float gravity)
{
  if (!vdec)
  {
    yspd += gravity*globals->gravity_percent + yspd2*globals->gravity_percent;
  }
  else if (vdec && fabs(xspd)+fabs(yspd) > 0.01)// && lifetime < 6000-30)
  {
    xspd *= vdec;
    xspd2 *= vdec;
    yspd *= vdec;
  }

  x += xspd+xspd2;
  y += yspd;
  lifetime--;
}

gsPixelRain::gsPixelRain()
{
  first_pixel=NULL;
  last_pixel=NULL;
  count = 0;
}

gsPixelRain::~gsPixelRain()
{
  Pixel *temp, *next_pixel;
  temp = first_pixel;

  while (temp != NULL)
  {
    next_pixel = temp->next;
    delete temp;
    temp = next_pixel;
  }
}

void gsPixelRain::add(int anz, int startx, int starty, int xradius, int yradius,
                      float xab, float yab, int spd,
                      int what, int maxtable, int lifetime, int dspd,
                      int max_xab, int max_yab, float vdec)
{
  int *tbl;
  int img = 0;

  switch (what)
  {
    case PIX_BLUE     : tbl = globals->tbl_blue; break;
    case PIX_YELLOW   : tbl = globals->tbl_yellow; break;
    case PIX_GREEN    : tbl = globals->tbl_green; break;
    case PIX_WHITE    : tbl = globals->tbl_white; break;
    case PIX_RED      : tbl = globals->tbl_red; break;
    case PIX_BLUEWHITE: tbl = globals->tbl_bluewhite; break;
    case PIX_ALL      : tbl = globals->tbl_all; break;
    case PIX_DEBRIS1  : tbl = NULL; img = debris_1_01; dspd = 10; break;
    case PIX_DEBRIS2  : tbl = NULL; img = debris_2_01; dspd = 10; break;
    case PIX_DEBRIS3  : tbl = NULL; img = debris_3_01; dspd = 10; break;
    case PIX_DEBRIS4  : tbl = NULL; img = debris_4_01; dspd = 10; break;
    case PIX_DEBRIS5  : tbl = NULL; img = debris_5_01; dspd = 10; break;
    default           : tbl = globals->tbl_all;
  }

  add(anz, startx, starty, xradius, yradius,
      xab, yab, spd,
      tbl, maxtable, lifetime, dspd, max_xab, max_yab,
      img, (DATAFILE*)globals->gamedat, NULL, vdec);
}

void gsPixelRain::add(int anz, int sx, int sy, int xradius, int yradius,
                    float xab, float yab, int spd,
                    int *color_table, int maxtable, int lifetime, int dspd,
                    int max_xab, int max_yab, int img,
                    DATAFILE *dataf, BITMAP *bit, float vdec)
{
  
  Pixel *TempPixel;
  float r;

  anz = fast_ftol(anz * float(globals->pixamount/100.0));

//  if (anz == 0) anz = 1;
  if (xradius == 0) xradius = 1;
  if (yradius == 0) yradius = 1;

  for (int nr=0; nr < anz; nr++)
  {
    float xs, ys, xs2, ys2;
    if (spd==0)
    {
      xs = 0;
      ys = 0;
      xs2 = xab;
      ys2 = yab;
    }
    else
    {
      r = (1.0-(fxrand() % 2) * 3.0) / ((fxrand() % 9)+1);
      xs = 0.01+(xab+(6.0/((fxrand() % spd)+1))*r);
      r = (1.0-(fxrand() % 2) * 3.0) / ((fxrand() % 9)+1);
      ys = 0.01+(yab+(6.0/((fxrand() % spd)+1))*r);
      xs2=ys2=0;
    }

    int nsx = fast_ftol(sx+fxrand()%xradius-xradius/2);
    int nsy = fast_ftol(sy+fxrand()%yradius-yradius/2);
    
    TempPixel = new Pixel(nsx, nsy,
                          xs, ys, xs2, ys2,
                          sx, sy,
                          max_xab, max_yab,
                          color_table, maxtable, lifetime,
                          dspd, NULL, vdec);

    if (img) TempPixel->image = (BITMAP*)dataf[img + fxrand() % dspd].dat;
    else if (bit) TempPixel->image = bit;

    if (TempPixel != NULL)
    {
      if (first_pixel==NULL)
      {
        last_pixel = first_pixel = TempPixel;
        first_pixel->next = first_pixel->prev = NULL;
        TempPixel->next = NULL; TempPixel->prev = NULL;
      }
      else
      {
        last_pixel->next = TempPixel;
        TempPixel->prev = last_pixel;
        last_pixel = TempPixel;
        TempPixel->next = NULL;
      }
    } // TempPixel != NULL

  } // for


  count += anz;
}

void gsPixelRain::draw(gsMap *m)
{
  Pixel *temp = first_pixel;

  while (temp != NULL)
  {
    temp->draw(m);
    temp = temp->next;
  }
}

void gsPixelRain::draw(BITMAP *m)
{
  Pixel *temp = first_pixel;

  while (temp != NULL)
  {
    temp->draw(m);
    temp = temp->next;
  }
}

void gsPixelRain::move(float gravity)
{
  int done;
  Pixel *temp = first_pixel, *next_pixel;

  while (temp != NULL)
  {
    next_pixel = temp->next;
    done = FALSE;

    temp->move(gravity);

    if (!temp->image)
    {
      // Pixelfarbe dekrementieren
      if (temp->dspd > 0)
        if (temp->lifetime % temp->dspd == 0)
          if (temp->tcolor > 0)
          {
            temp->tcolor--;
            temp->color = temp->color_table[temp->tcolor];
          }
          else
          {
            remove(temp);
            done=TRUE;
          }
    } // if !temp->image

    // check lifetime
    if (!done && temp->lifetime <= 0)
    {
      if (temp->image)
        effects->explosions->add(-1, expl_sm3_01, 5, 6, 0, temp->x-2, temp->y-2, globals->gamedat);
      remove(temp);
      done=TRUE;
    }

    temp = next_pixel;
  } // while temp != NULL
}

void gsPixelRain::check_collisions(gsMap *m)
{
  
  Pixel *temp = first_pixel, *next_pixel;

  while (temp != NULL)
  {
    next_pixel = temp->next;

    int done = FALSE;

    // Checken, ob zulaessiges Gebiet verlassen
    if (temp->max_xab && temp->max_yab)
      if ((temp->x > temp->startx+temp->max_xab/2) ||
          (temp->x < temp->startx-temp->max_xab/2) ||
          (temp->y > temp->starty+temp->max_yab/2) ||
          (temp->y < temp->starty-temp->max_yab/2))
      {
        remove(temp);
        done = TRUE;
      }

    // Mit Landschaft kollidiert?
    if (!done && !temp->image)
    {
     int x1 = fast_ftol(temp->x - m->get_map_x() + temp->xspd*4);
     int y1 = fast_ftol(temp->y - m->get_map_y() + temp->yspd*4);

     int x2 = fast_ftol(temp->x - m->get_map_x());
     int y2 = fast_ftol(temp->y - m->get_map_y());

     if (m->is_on_map(temp->x, temp->y, 1, 1))
     if ( getpixel(m->get_vscreen(),
                   x1, y1) ||
          getpixel(m->get_vscreen(),
                   x2, y2) )
      {
        if (temp->bounce < 2 && temp->yspd > 1)
        {
          temp->yspd = -temp->yspd/3;
          temp->bounce++;
        }
        else
        {
          remove(temp);
          done = TRUE;
        }
      } // if getpixel
    }
    else if (!done && temp->image)
    {
      int w = ((BITMAP*)temp->image)->w;
      int h = ((BITMAP*)temp->image)->h;

      if ( getpixel(m->get_vscreen(),
                    (int)temp->x - m->get_map_x()+w/2,
                    (int)temp->y - m->get_map_y()+h/2 - h - (int)temp->yspd) > 10 ||
           getpixel(m->get_vscreen(),
                    (int)temp->x - m->get_map_x()+w/2,
                    (int)temp->y - m->get_map_y()+h/2 + h + (int)temp->yspd) > 10 ||
           getpixel(m->get_vscreen(),
                    (int)temp->x - m->get_map_x()+w/2 - w - (int)temp->xspd,
                    (int)temp->y - m->get_map_y()+h/2) > 10 ||
           getpixel(m->get_vscreen(),
                    (int)temp->x - m->get_map_x()+w/2 + w + (int)temp->xspd,
                    (int)temp->y - m->get_map_y()+h/2) > 10 )
      {
        if (temp->bounce < 2 && temp->yspd > 1)
        {
          temp->yspd = -temp->yspd/3;
          temp->bounce++;
        }
        else
        {
          effects->explosions->add(-1, expl_sm3_01, 5, 6, 0, temp->x-2, temp->y-2, globals->gamedat);
          remove(temp);
          done = TRUE;
        }
      } // if getpixel

    } // mit Landschaft kollidiert

    /* Mit Objekten kollidiert?
    Object *fob = objects->get_first_object(), *nextob;
    if (!done)
    while (fob)
    {
      nextob = fob->get_next();

      if ( collision((int)temp->x, (int)temp->y, 1, 1,
                     (int)fob->get_x(), (int)fob->get_y(),
                     fob->get_width(), fob->get_height()) )
      {
        if (temp->bounce < 2 && temp->yspd > 1)
        {
          temp->yspd = -temp->yspd/3;
          temp->bounce++;
        }
        else
        {
          if (temp->image)
            effects->explosions->add(-1, expl_sm3_01, 5, 6, 0, temp->x-2, temp->y-2, globals->gamedat);
          remove(temp);
          done = TRUE;
        }
      }

      fob = nextob;
    }
    */

    temp = next_pixel;
  } // while temp != NULL
}

void gsPixelRain::remove(Pixel *temp)
{
  Pixel *next_pixel, *prev_pixel;

  if (temp != NULL)
  {
    prev_pixel = temp->prev;
    next_pixel = temp->next;

    if (prev_pixel==NULL) first_pixel = next_pixel;
    else prev_pixel->next = next_pixel;

    if (next_pixel==NULL) last_pixel = prev_pixel;
    else next_pixel->prev = prev_pixel;

    delete temp;
    temp = NULL;

    count--;
  }
}

void gsPixelRain::clear(gsMap *m)
{
  Pixel *temp = first_pixel, *next_pixel;

  while (temp != NULL)
  {
    next_pixel = temp->next;

    putpixel(m->get_vscreen(), (int)temp->x - m->get_map_x(), (int)temp->y - m->get_map_y(), 0);
//    putpixel(m->get_vscreen(), (int)temp->x - m->get_map_x(), (int)temp->y - m->get_map_y(), 0);

    temp = next_pixel;
  }
}

void gsPixelRain::clear(BITMAP *m)
{
  Pixel *temp = first_pixel, *next_pixel;

  while (temp != NULL)
  {
    next_pixel = temp->next;

    putpixel(m, (int)temp->x, (int)temp->y, 0);
//    putpixel(m, (int)temp->x, (int)temp->y, 0);

    temp = next_pixel;
  }
}

