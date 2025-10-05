/*
 *   GRAVITY STRIKE -- menu/mbullet.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _MBULLET_
#define _MBULLET_

#include "../gsbullet.h"


class mBullet : public gsBullet
{
  public:
    // redefining collision function
    void check_collisions(BITMAP *m);
    // and move
    void move();
};


#endif