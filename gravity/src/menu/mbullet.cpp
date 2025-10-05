/*
 *   GRAVITY STRIKE -- menu/mbullet.cpp
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include <math.h>
#include "mbullet.h"
#include "mpixels.h"
#include "mplayer.h"
#include "../gsglob.h"
#include "../gshelper.h"
#include "../gssound.h"

extern mPixelRain *mpixels;
extern mPlayer    *mplayer[];

void mBullet::check_collisions(BITMAP *m)
{
  Bullet *temp = get_first(), *next_bullet;

  while (temp != NULL)
  {
    next_bullet = temp->get_next();

    int done = FALSE;

    // check if screen left
    if ((temp->get_x() > m->w) ||
        (temp->get_x() < 0) ||
        (temp->get_y() > m->h) ||
        (temp->get_y() < 0))
    {
      remove(temp);
      done = TRUE;
    }

    // collision with "landscape"?
    if (!done && getpixel(m, (int)temp->get_x(), (int)temp->get_y()))
    {
      // add pixels when bullet hits the wall
      float tx, ty;

      if ( temp->get_yspd() < 0 ) ty = -0.6;
      else ty = 0.4;

      tx = ( (fxrand() % 4) - 2 ) / 5;

      float pspd = abs(int(sqrt(temp->get_yspd()*temp->get_yspd() + temp->get_xspd()*temp->get_xspd())));

      mpixels->add( (fxrand() % ((int)(pspd*2)+1)) + 1,
                     temp->get_x() - temp->get_xspd()/4, temp->get_y() + ty*4, 1, 1,
                     tx, ty, 400,
                     globals->tbl_blue, 10, 1000, 50,
                     600, 600);

      remove(temp);
      done = TRUE;
    }

    // collision with players?
    if (!done)
    for (int n=0; n < 2; n++)
    {
      if ( mplayer[n]->get_sig() != temp->get_by_player() &&
           collision((int)temp->get_x(), (int)temp->get_y(), 1, 1,
                     (int)mplayer[n]->get_x(), (int)mplayer[n]->get_y(), 18, 18) )
      {
        mpixels->add(400, mplayer[n]->get_x()+9, mplayer[n]->get_y()+8, 4, 4, 0, 0, 25,
                     globals->tbl_all, 256);

        gsound->menu_dead();

        if (n == 0)
        {
          mplayer[n]->set_position(-100, 240);
          mplayer[1]->inc_frags();
        }
        else
        {
          mplayer[n]->set_position(700, 240);
          mplayer[0]->inc_frags();
        }

        remove(temp);
        done = TRUE;
      }
    }

    temp = next_bullet;
  } // while temp != NULL
}

void mBullet::move()
{
  Bullet *temp = get_first(), *next_bullet;

  while (temp != NULL)
  {
    next_bullet = temp->get_next();
    temp->move(1);
    mpixels->add(1, temp->get_x()-1+fxrand()%2, temp->get_y()-1+fxrand()%2, 1, 1,
                 0, -globals->particle_gravity, 0,
                 globals->tbl_white, 9, 1000, 2, 10, 10);

    temp = next_bullet;
  } // while temp != NULL
}
