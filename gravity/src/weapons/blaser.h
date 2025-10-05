/*
 *   GRAVITY STRIKE -- weapons/blaser.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _B_LASER_
#define _B_LASER_

#include "../gsbullet.h"

class bulletLaser : public Bullet
{
  private:

  public:
    bulletLaser(class gsPlayer *,  int puser, int pupgrade, class Image *);
    bulletLaser(long psig, int enr, int px, int py, float pxspd, float pyspd, int pstrength, int pshield, int phull, float puser, int pupgrade, class Image *);
    ~bulletLaser();

    void move();
    int  check_collisions();
    int  check_map_collisions(class gsMap *);
    void draw(class gsMap *);
};

#endif

