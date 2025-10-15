/*
 *   GRAVITY STRIKE -- weapons/bbomb.h
 *
 *
 *   (c) Copyright 2001/2002 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro5_wrapper.h"

#ifndef _B_BOMB_
#define _B_BOMB_

#include "../gsbullet.h"

class bulletBomb : public Bullet
{
  private:
    float xacc;                                    // x acceleration
    float yacc;                                    // y acceleration
    int   bumpcount;                               // for upgrades

  public:
    bulletBomb(class gsPlayer *,  int puser, int pupgrade, class Image *);
    bulletBomb(long psig, int enr, int px, int py, float pxspd, float pyspd, int pstrength, int pshield, int phull, float puser, int pupgrade, class Image *);

    ~bulletBomb();

    void move();
    int  check_collisions();
    int  check_map_collisions(class gsMap *);
    void add_bomb_bullets();
    void draw(class gsMap *);
};

#endif

