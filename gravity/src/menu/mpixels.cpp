/*
 *   GRAVITY STRIKE -- menu/mpixels.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "mpixels.h"

void mPixelRain::check_collisions(BITMAP *m)
{
  Pixel *temp = get_first(), *next_pixel;

  while (temp != NULL)
  {
    next_pixel = temp->next;

    int done = FALSE;

    // check if screen left
      if ((temp->x > m->w) ||
          (temp->x < 0) ||
          (temp->y > m->h) ||
          (temp->y < 0))
      {
        remove(temp);
        done = TRUE;
      }

    // Mit Landschaft kollidiert?
    if (!done && !temp->image)
    {
      if (getpixel(m, (int)temp->x + (int)temp->xspd*4, (int)temp->y + (int)temp->yspd*4) ||
          getpixel(m, (int)temp->x, (int)temp->y))
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

      if ( getpixel(m,
                    (int)temp->x + w/2,
                    (int)temp->y + h/2 - h - (int)temp->yspd) > 10 ||
           getpixel(m,
                    (int)temp->x + w/2,
                    (int)temp->y + h/2 + h + (int)temp->yspd) > 10 ||
           getpixel(m,
                    (int)temp->x + w/2 - w - (int)temp->xspd,
                    (int)temp->y + h/2) > 10 ||
           getpixel(m,
                    (int)temp->x + w/2 + w + (int)temp->xspd,
                    (int)temp->y + h/2) > 10 )
      {
        if (temp->bounce < 2 && temp->yspd > 1)
        {
          temp->yspd = -temp->yspd/3;
          temp->bounce++;
        }
        else
        {
//          effects->explosions->add(-1, expl_sm3_01, 5, 6, 0, temp->x-2, temp->y-2, globals->data);
          remove(temp);
          done = TRUE;
        }
      } // if getpixel

    }

    temp = next_pixel;
  } // while temp != NULL
}

