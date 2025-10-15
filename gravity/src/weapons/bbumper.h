/*
 *   GRAVITY STRIKE -- weapons/bbumper.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _B_BUMPER_
#define _B_BUMPER_

#include "../gsbullet.h"

class bulletBumper : public Bullet
{
  private:
    float xacc;                                    // x acceleration
    float yacc;                                    // y acceleration
    int   rot;                                     // angle for rotation
    int   bumpcount;                               // for upgrades

  public:
    bulletBumper(class gsPlayer *,  int puser, int pupgrade, class Image *);
    bulletBumper(long psig, int enr, int px, int py, float pxspd, float pyspd, int pstrength, int pshield, int phull, float puser, int pupgrade, class Image *);

    ~bulletBumper();

    void move();
    int  check_collisions();
    int  check_map_collisions(class gsMap *);
    void draw(class gsMap *);
    void hit(float n);
};

#endif

