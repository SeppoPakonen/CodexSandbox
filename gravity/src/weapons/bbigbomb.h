/*
 *   GRAVITY STRIKE -- weapons/bbigbomb.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _B_BIGBOMB_
#define _B_BIGBOMB_

#include "../gsbullet.h"

class bulletBigBomb : public Bullet
{
  private:
    float xacc;                                    // x acceleration
    float yacc;                                    // y acceleration
    int   bumpcount;                               // for upgrades

  public:
    bulletBigBomb(class gsPlayer *,  int puser, int pupgrade, class Image *);
    bulletBigBomb(long psig, int enr, int px, int py, float pxspd, float pyspd, int pstrength, int pshield, int phull, float puser, int pupgrade, class Image *);
    ~bulletBigBomb();

    void move();
    int  check_collisions();
    int  check_map_collisions(class gsMap *);
    void draw(class gsMap *);
};

#endif

