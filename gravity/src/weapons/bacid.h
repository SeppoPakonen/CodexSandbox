/*
 *   GRAVITY STRIKE -- weapons/bacid.h
 *
 *
 *   (c) Copyright 2001/2005 by Jens Hassler <jh@gravity-strike.de>
 *       www.gravity-strike.de
 *
 *   see source.txt for details
 *
 */

#include "allegro.h"

#ifndef _B_ACID_
#define _B_ACID_

#include "../gsbullet.h"

class bulletAcid : public Bullet
{
  private:
    float xacc;                                    // x acceleration
    float yacc;                                    // y acceleration

  public:
    bulletAcid(class gsPlayer *,  int puser, int pupgrade, class Image *, int pspawn);
    bulletAcid(long psig, int enr, int px, int py, float pxspd, float pyspd, int pstrength, int pshield, int phull, float puser, int pupgrade, class Image *, int pspawn);

    ~bulletAcid();

    void move();
    int  check_collisions();
    int  check_map_collisions(class gsMap *);
    void draw(class gsMap *);
};

#endif

